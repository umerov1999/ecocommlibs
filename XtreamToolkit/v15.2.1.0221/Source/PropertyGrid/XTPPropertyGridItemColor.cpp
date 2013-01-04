// XTPPropertyGridItemColor.cpp : implementation of the CXTPPropertyGridItemColor class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemColor.h"
#include "XTPPropertyGridView.h"

#ifdef _XTP_INCLUDE_CONTROLS
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Dialog/XTPColorDialog.h"
#include "Controls/Popup/XTPColorSelectorCtrl.h"
#include "Controls/Popup/XTPColorPopup.h"
#include "Controls/Defines.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemColor

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemColor, CXTPPropertyGridItem)

CXTPPropertyGridItemColor::CXTPPropertyGridItemColor(LPCTSTR strCaption, COLORREF clr, COLORREF* pBindColor)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindColor = pBindColor;
	SetColor(clr);
	m_nFlags = xtpGridItemHasEdit | xtpGridItemHasExpandButton;
	m_strDefaultValue = m_strValue;
	m_colorEditor = xtpGridItemColorExtendedDialog;
}

CXTPPropertyGridItemColor::CXTPPropertyGridItemColor(UINT nID, COLORREF clr, COLORREF* pBindColor)
	: CXTPPropertyGridItem(nID)
{
	m_pBindColor = pBindColor;
	SetColor(clr);
	m_nFlags = xtpGridItemHasEdit | xtpGridItemHasExpandButton;
	m_strDefaultValue = m_strValue;
	m_colorEditor = xtpGridItemColorExtendedDialog;
}

CXTPPropertyGridItemColor::~CXTPPropertyGridItemColor()
{
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CXTPPropertyGridItemColor::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	COLORREF clr = dc.GetTextColor();
	CRect rcSample(rcValue.left - 2, rcValue.top + 1, rcValue.left + 18, rcValue.bottom - 1);
	CXTPPenDC pen(dc, clr);
	CXTPBrushDC brush(dc, m_clrValue);
	dc.Rectangle(rcSample);

	CRect rcText(rcValue);
	rcText.left += 25;

	dc.DrawText(m_strValue, rcText, DT_SINGLELINE | DT_VCENTER);

	return TRUE;
}

CRect CXTPPropertyGridItemColor::GetValueRect()
{
	CRect rcValue(CXTPPropertyGridItem::GetValueRect());
	rcValue.left += 25;
	return rcValue;
}

COLORREF AFX_CDECL CXTPPropertyGridItemColor::StringToRGB(LPCTSTR str)
{
	int nRed = NextNumber(str);
	int nGreen = NextNumber(str);
	int nBlue = NextNumber(str);

	return RGB(__min(nRed, 255), __min(nGreen, 255), __min(nBlue, 255));
}

CString AFX_CDECL CXTPPropertyGridItemColor::RGBToString(COLORREF clr)
{
	CString str;
	str.Format(_T("%i; %i; %i"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
	return str;
}

void CXTPPropertyGridItemColor::SetValue(CString strValue)
{
	SetColor(StringToRGB(strValue));
}

void CXTPPropertyGridItemColor::SetColor(COLORREF clr)
{
	m_clrValue = clr;

	if (m_pBindColor)
	{
		*m_pBindColor = clr;
	}

	CXTPPropertyGridItem::SetValue(RGBToString(clr));
}

void CXTPPropertyGridItemColor::BindToColor(COLORREF* pBindColor)
{
	m_pBindColor = pBindColor;
	if (m_pBindColor)
	{
		*m_pBindColor = m_clrValue;
	}
}


void CXTPPropertyGridItemColor::OnBeforeInsert()
{
	if (m_pBindColor && *m_pBindColor != m_clrValue)
	{
		SetColor(*m_pBindColor);
	}
}

void CXTPPropertyGridItemColor::SetEditorStyle(XTPPropertyGridItemColorEditor editor)
{
	m_colorEditor = editor;

	if (m_colorEditor == xtpGridItemColorPopup)
		SetFlags(xtpGridItemHasEdit | xtpGridItemHasComboButton);
	else
		SetFlags(xtpGridItemHasEdit | xtpGridItemHasExpandButton);
}

XTPPropertyGridItemColorEditor CXTPPropertyGridItemColor::GetEditorStyle()
{
	return m_colorEditor;
}


void CXTPPropertyGridItemColor::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	if (m_pGrid->SendNotifyMessage(XTP_PGN_INPLACEBUTTONDOWN, (LPARAM)pButton) == TRUE)
		return;

	if (!OnRequestEdit())
		return;

	CWnd* pGrid = m_pGrid;

#ifdef _XTP_INCLUDE_CONTROLS
	if (m_colorEditor == xtpGridItemColorPopup)
	{
		class CPropertyGridItemColorColorPopup: public CXTPColorPopup
		{
		public:
			CPropertyGridItemColorColorPopup()
				: CXTPColorPopup(TRUE)
			{
				m_pItem = NULL;
			}
			~CPropertyGridItemColorColorPopup()
			{
				CMDTARGET_RELEASE(m_pItem);
			}

			BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
			{
				if (message == CPN_XTP_SELENDOK)
				{
					CString strValue = m_pItem->RGBToString((COLORREF)wParam);
					if (m_pItem->OnAfterEdit(strValue))
					{
						m_pItem->OnValueChanged(strValue);
					}
					*pResult = 0;
					return TRUE;
				}
				if (message == CPN_XTP_SELENDCANCEL)
				{
					m_pItem->OnCancelEdit();
				}
				return CXTPColorPopup::OnWndMsg(message, wParam, lParam, pResult);\
			}

			CXTPPropertyGridItemColor* m_pItem;
		};

		CPropertyGridItemColorColorPopup *pColorPopup = new CPropertyGridItemColorColorPopup();
		pColorPopup->SetTheme(xtpControlThemeOfficeXP);

		CRect rcItem= GetItemRect();
		m_pGrid->ClientToScreen(&rcItem);

		COLORREF clrDefault = StringToRGB(m_strDefaultValue);
		pColorPopup->Create(rcItem, m_pGrid, CPS_XTP_RIGHTALIGN|CPS_XTP_USERCOLORS|CPS_XTP_EXTENDED|CPS_XTP_MORECOLORS|CPS_XTP_SHOW3DSELECTION|CPS_XTP_SHOWHEXVALUE, GetColor(), clrDefault);
		pColorPopup->SetOwner(m_pGrid);
		pColorPopup->SetFocus();
		pColorPopup->AddListener(pColorPopup->GetSafeHwnd());
		pColorPopup->m_pItem = this;

		InternalAddRef();
	}
	else if (m_colorEditor == xtpGridItemColorExtendedDialog)
	{
		InternalAddRef();

		CXTPColorDialog dlg(m_clrValue, m_clrValue, CPS_XTP_SHOW3DSELECTION|CPS_XTP_SHOWEYEDROPPER, m_pGrid);

		if (dlg.DoModal() == IDOK)
		{
			CString strValue = RGBToString(dlg.GetColor());

			if (OnAfterEdit(strValue))
			{
				OnValueChanged(strValue);
				SAFE_INVALIDATE(pGrid);
			}
		}
		else
		{
			OnCancelEdit();
		}

		InternalRelease();
	}
	else
#endif
	{
		InternalAddRef();

		CColorDialog dlg(m_clrValue, 0, m_pGrid);

		if (dlg.DoModal() == IDOK)
		{
			CString strValue = RGBToString(dlg.GetColor());

			if (OnAfterEdit(strValue))
			{
				OnValueChanged(strValue);
				SAFE_INVALIDATE(pGrid);
			}
		}
		else
		{
			OnCancelEdit();
		}

		InternalRelease();
	}
}



