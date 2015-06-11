// XTPSyntaxEditPropertiesPage.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Resource.h"

// common includes
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"

// syntax editor includes
#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"
#include "XTPSyntaxEditLexPtrs.h"
#include "XTPSyntaxEditLexClassSubObjT.h"
#include "XTPSyntaxEditSectionManager.h"
#include "XTPSyntaxEditLexCfgFileReader.h"
#include "XTPSyntaxEditLexClassSubObjDef.h"
#include "XTPSyntaxEditLexClass.h"
#include "XTPSyntaxEditLexColorFileReader.h"
#include "XTPSyntaxEditCtrl.h"
#include "XTPSyntaxEditDoc.h"
#include "XTPSyntaxEditView.h"
#include "XTPSyntaxEditPaintManager.h"
#include "XTPSyntaxEditColorSampleText.h"
#include "XTPSyntaxEditColorComboBox.h"
#include "XTPSyntaxEditPropertiesPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//===========================================================================
// EnumFontFamExProc
//===========================================================================

AFX_STATIC int CALLBACK EnumFontFamExProc(
	ENUMLOGFONTEX *lpelfe,    // logical-font data
	NEWTEXTMETRICEX *lpntme,  // physical-font data
	DWORD FontType,           // type of font
	LPARAM lParam)            // application-defined data
{
	CComboBox* pWndCombo = DYNAMIC_DOWNCAST(CComboBox, (CComboBox*)lParam);
	if (::IsWindow(pWndCombo->GetSafeHwnd()))
	{
		switch (pWndCombo->GetDlgCtrlID())
		{
		case XTP_IDC_EDIT_COMB_NAMES:
			{
				//if (lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH)
				{
					// Make sure the fonts are only added once.
					if (pWndCombo->FindStringExact(0, (LPCTSTR)lpelfe->elfFullName) == CB_ERR)
					{
						// Add to list
						pWndCombo->AddString((LPCTSTR)lpelfe->elfLogFont.lfFaceName);
					}
				}
			}
			break;
		case XTP_IDC_EDIT_COMB_SIZES:
			{
				if (FontType != TRUETYPE_FONTTYPE)
				{
					CWindowDC dc(NULL);
					CString csSize;
					csSize.Format(_T("%i"), ::MulDiv(lpntme->ntmTm.tmHeight - lpntme->ntmTm.tmInternalLeading,
						72, ::GetDeviceCaps(dc.m_hDC, LOGPIXELSY)));

					// Make sure the fonts are only added once.
					if (pWndCombo->FindStringExact(0, (LPCTSTR)csSize) == CB_ERR)
					{
						// Add to list
						pWndCombo->AddString((LPCTSTR)csSize);
					}
				}
			}
			break;
		case XTP_IDC_EDIT_COMB_SCRIPT:
			{
				if (lpelfe->elfScript[0] != _T('\0'))
				{
					// Make sure the fonts are only added once.
					if (pWndCombo->FindStringExact(0, (LPCTSTR)lpelfe->elfScript) == CB_ERR)
					{
						// Add to list
						int iIndex = pWndCombo->AddString((LPCTSTR)lpelfe->elfScript);
						if (iIndex != CB_ERR)
						{
							pWndCombo->SetItemData(iIndex,
								(DWORD)lpelfe->elfLogFont.lfCharSet);
						}
					}
				}
			}
			break;
		}
	}
	return TRUE;
}


//===========================================================================
// CXTPSyntaxEditTipListBox
//===========================================================================

CXTPSyntaxEditTipListBox::CXTPSyntaxEditTipListBox()
{
	m_dwIdx = (DWORD)MAKELONG(-1, -2);
}

int CXTPSyntaxEditTipListBox::HitTest(LPPOINT pPoint /*= NULL*/) const
{
	CPoint point;
	if (pPoint == NULL)
	{
		::GetCursorPos(&point);
		ScreenToClient(&point);
		pPoint = &point;
	}

	BOOL bOutside;
	int iIndex = (int)ItemFromPoint(*pPoint, bOutside);

	if (bOutside)
		return LB_ERR;

	return iIndex;
}

int CXTPSyntaxEditTipListBox::HitTest(CPoint point, BOOL bIsClient /*= FALSE*/) const
{
	if (!bIsClient)
		ScreenToClient(&point);

	return HitTest(&point);
}

int CXTPSyntaxEditTipListBox::ShowTip(CPoint point, BOOL bIsClient /*= FALSE*/)
{
	if (!bIsClient)
		ScreenToClient(&point);

	if (!::IsWindow(m_wndInfoTip.m_hWnd))
		m_wndInfoTip.Create(this);

	int iIndex = HitTest(&point);
	if (iIndex != LB_ERR)
	{
		m_wndInfoTip.ShowTip(iIndex);
	}

	return iIndex;
}

//2 next functions modifications - fixes for Issue # 21638
BOOL CXTPSyntaxEditTipListBox::SelChanged() //const
{
	BOOL bRes = (LOWORD(m_dwIdx) != HIWORD(m_dwIdx));
	DWORD dw = (DWORD)MAKELONG(LOWORD(m_dwIdx), GetCurSel());
	m_dwIdx = dw;
	return bRes;
	//return (LOWORD(m_dwIdx) != HIWORD(m_dwIdx));
}

BOOL CXTPSyntaxEditTipListBox::PreTranslateMessage(MSG* pMsg)
{
	UINT uMessage = pMsg->message;

	switch (uMessage)
	{
	case WM_MOUSEMOVE:
		ShowTip(pMsg->pt);
		break;

	case WM_LBUTTONDOWN:
		//m_dwIdx = (DWORD)MAKELONG(GetCurSel(), HIWORD(m_dwIdx));
		ShowTip(pMsg->pt);
		break;

	case WM_LBUTTONUP:
		m_dwIdx = (DWORD)MAKELONG(GetCurSel(), HIWORD(m_dwIdx));
		//m_dwIdx = (DWORD)MAKELONG(LOWORD(m_dwIdx), GetCurSel());
		break;

	case WM_KEYDOWN:
	case WM_MOUSEWHEEL:
		m_wndInfoTip.HideTip();
		break;
	}

	return CListBox::PreTranslateMessage(pMsg);
}

//===========================================================================
// CXTPSyntaxEditTipComboBox
//===========================================================================

BEGIN_MESSAGE_MAP(CXTPSyntaxEditTipComboBox, CComboBox)
	//{{AFX_MSG_MAP(CXTPSyntaxEditTipComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HBRUSH CXTPSyntaxEditTipComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		if (m_wndListBox.GetSafeHwnd() == NULL)
			m_wndListBox.SubclassWindow(pWnd->GetSafeHwnd());
	}

	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CXTPSyntaxEditTipComboBox::OnDestroy()
{
	if (m_wndListBox.GetSafeHwnd() != NULL)
		m_wndListBox.UnsubclassWindow();

	CComboBox::OnDestroy();
}
