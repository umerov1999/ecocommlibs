// XTPFontComboBox.cpp : implementation of the CXTPFontComboBox class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include "stdafx.h"
#include "../Resource.h"

#include "Common/XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "../Util/XTPControlTheme.h"

#include "../Util/XTPGlobal.h"
#include "../ListBox/XTPListBox.h"

#include "XTPComboBox.h"
#include "XTPFontComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CXTPFontEnum
//////////////////////////////////////////////////////////////////////

const int BMW = 16;

CXTPFontEnum::CXTPFontEnum()
{
	Init();
}

CXTPFontEnum::~CXTPFontEnum()
{

}

CXTPFontEnum& CXTPFontEnum::Get()
{
	static CXTPFontEnum fontEnum;
	return fontEnum;
}

bool CXTPFontEnum::DoesFontExist(CString& strFaceName)
{
	POSITION pos;
	for (pos = m_listFonts.GetHeadPosition(); pos; m_listFonts.GetNext(pos))
	{
		CXTPLogFont& lf = m_listFonts.GetAt(pos);
		if (strFaceName.CompareNoCase(lf.lfFaceName) == 0)
		{
			strFaceName = lf.lfFaceName; // adjust case if necessary
			return true;
		}
	}
	return false;
}

CXTPLogFont* CXTPFontEnum::GetLogFont(const CString& strFaceName)
{
	POSITION pos;
	for (pos = m_listFonts.GetHeadPosition(); pos; m_listFonts.GetNext(pos))
	{
		CXTPLogFont& lf = m_listFonts.GetAt(pos);
		if (strFaceName == lf.lfFaceName)
		{
			return &lf;
		}
	}
	return NULL;
}

bool CXTPFontEnum::AddFont(const LOGFONT* pLF, DWORD dwType)
{
	// See if the font name has already been added.
	CString strFaceName = pLF->lfFaceName;
	strFaceName.Remove('@');

	if (DoesFontExist(strFaceName) != 0)
	{
		return false;
	}

	LOGFONT lfGlobal;
	XTPAuxData().font.GetLogFont(&lfGlobal);

	// Save the CXTPLogFont struct.
	CXTPLogFont lf;

	MEMCPY_S((void*)&lf, (const void*)pLF, sizeof(CXTPLogFont));

	lf.dwType = dwType;
	lf.lfHeight = -(::GetSystemMetrics(SM_CYVTHUMB)-(::GetSystemMetrics(SM_CYEDGE)*2));
	lf.lfWidth = 0;

	m_listFonts.AddTail(lf);

	return true;
}

BOOL CALLBACK CXTPFontEnum::EnumFontFamExProc(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX* /*lpntm*/, DWORD dwFontType, LPARAM lParam)
{
	CXTPFontEnum* pFontEnum = (CXTPFontEnum*)lParam;
	if (pFontEnum != NULL)
	{
		if (pFontEnum->AddFont(&pelf->elfLogFont, dwFontType))
		{
			// TODO:
		}
	}

	return TRUE;
}

int CXTPFontEnum::GetMaxWidth()
{
	CWindowDC dc(NULL);
	CFont* p = dc.SelectObject(&XTPAuxData().font);

	int iMaxWidth = 0;

	POSITION pos;
	for (pos = m_listFonts.GetHeadPosition(); pos; m_listFonts.GetNext(pos))
	{
		CXTPLogFont& lf = m_listFonts.GetAt(pos);

		int nWidth = dc.GetTextExtent(lf.lfFaceName).cx;
		iMaxWidth = __max(iMaxWidth, nWidth);
	}

	dc.SelectObject(p);
	return iMaxWidth;
}

void CXTPFontEnum::Init(CDC* pDC/*= NULL*/, BYTE nCharSet/*= DEFAULT_CHARSET*/)
{
	m_listFonts.RemoveAll();

	// Enumerate all styles of all fonts for the ANSI character set
	CXTPLogFont lf;
	lf.lfFaceName[ 0 ] = '\0';
	lf.lfCharSet = nCharSet;

	if (pDC == NULL || pDC->m_hDC == NULL)
	{
		CWindowDC dc(NULL);
		::EnumFontFamiliesEx(dc.m_hDC, &lf,
			(FONTENUMPROC)EnumFontFamExProc, (LPARAM)this, 0);
	}
	else
	{
		::EnumFontFamiliesEx(pDC->m_hDC, &lf,
			(FONTENUMPROC)EnumFontFamExProc, (LPARAM)this, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPFontListBox
/////////////////////////////////////////////////////////////////////////////

CXTPFontListBox::CXTPFontListBox()
{
	m_csSymbol = _T("AaBbCc");
	m_dwStyle  = xtpFontBoth;

	VERIFY(m_ilFontType.Create(
		XTP_IDB_FONTTYPE, BMW, 1, RGB(255, 0, 255)));
}

CXTPFontListBox::~CXTPFontListBox()
{
}

BEGIN_MESSAGE_MAP(CXTPFontListBox, CXTPListBox)
	//{{AFX_MSG_MAP(CXTPFontListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPFontListBox message handlers

void CXTPFontListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC*  pDC       = CDC::FromHandle(lpDIS->hDC);
	UINT  itemState = lpDIS->itemState;
	UINT  itemID    = lpDIS->itemID;
	CRect rcItem    = lpDIS->rcItem;

	if (itemID == (UINT)-1)
	{
		return;
	}

	CXTPLogFont* pLF = (CXTPLogFont*)lpDIS->itemData;
	if (pLF == NULL)
	{
		return;
	}

	BOOL bDisabled = ((itemState & ODS_DISABLED) == ODS_DISABLED);
	BOOL bSelected = ((itemState & ODS_SELECTED) == ODS_SELECTED);
	BOOL bFocus    = ((itemState & ODS_FOCUS)    == ODS_FOCUS);

	// draw background.
	if (bDisabled)
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_GRAYTEXT));
		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		if (bSelected)
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_HIGHLIGHT));
		}
		else
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));
			pDC->SetBkColor(GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_WINDOW));
		}

		// draw focus rectangle.
		if (bFocus)
		{
			pDC->DrawFocusRect(&rcItem);
		}
	}

	CString csFaceName = pLF->lfFaceName;

	// true type or device flag set by SetItemData
	if (pLF->dwType & TRUETYPE_FONTTYPE)
	{
		m_ilFontType.Draw(pDC, 1, rcItem.TopLeft(), ILD_NORMAL);
	}

	// Define the size of the text area to draw
	CRect rcText(rcItem);
	rcText.left += BMW + 6;

	pDC->SetBkMode(TRANSPARENT);

	if (m_dwStyle & xtpFontGUI)
	{
		CXTPFontDC fontDC(pDC, &XTPAuxData().font);
		pDC->DrawText(csFaceName, rcText, DT_VCENTER | DT_SINGLELINE);
	}

	if (m_dwStyle & xtpFontSample)
	{
		if (m_dwStyle & xtpFontGUI)
			rcText.left = rcText.left + CXTPFontEnum::Get().GetMaxWidth() + 5;

		if ((m_dwStyle & xtpFontGUI) || (pLF->lfCharSet == SYMBOL_CHARSET))
			csFaceName = m_csSymbol;

		// Create the font to be displayed and initialize the device context.
		CFont font;
		font.CreateFontIndirect(pLF);

		// Draw the text item.
		CXTPFontDC fontDC(pDC, &font);
		pDC->DrawText(csFaceName, rcText, DT_VCENTER | DT_SINGLELINE);
	}
}

void CXTPFontListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_LISTBOX);
	lpMIS->itemHeight = (::GetSystemMetrics(SM_CYVTHUMB)-::GetSystemMetrics(SM_CYEDGE));
	lpMIS->itemWidth = 0;
}

int CXTPFontListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	ASSERT(lpCIS->CtlType == ODT_LISTBOX);

	int iItem1 = (int)(WORD)lpCIS->itemID1;
	if (iItem1 == -1)
	{
		return -1;
	}

	CString strItem1;
	GetText(iItem1, strItem1);

	int iItem2 = (int)(WORD)lpCIS->itemID2;
	if (iItem2 == -1)
	{
		return -1;
	}

	CString strItem2;
	GetText(iItem2, strItem2);

	return strItem1.Collate(strItem2);
}

void CXTPFontListBox::Initialize(bool /*bAutoFont = true*/)
{
	CXTPListBox::Initialize(false);

	CXTPFontList& fontList = CXTPFontEnum::Get().GetFontList();

	POSITION pos;
	for (pos = fontList.GetHeadPosition(); pos; fontList.GetNext(pos))
	{
		CXTPLogFont& lf = fontList.GetAt(pos);
		int iIndex = AddString(lf.lfFaceName);

		SetItemDataPtr(iIndex, &lf);
	}
}

bool CXTPFontListBox::GetSelFont(CXTPLogFont& lf)
{
	int iCurSel = GetCurSel();
	if (iCurSel == LB_ERR)
	{
		return false;
	}

	CXTPLogFont* pLF = (CXTPLogFont*)GetItemDataPtr(iCurSel);
	if (pLF == NULL)
	{
		return false;
	}

	MEMCPY_S((void*)&lf, (const void*)pLF, sizeof(CXTPLogFont));

	return true;
}

bool CXTPFontListBox::GetSelFont(CString& strFaceName)
{
	CXTPLogFont lf;
	if (GetSelFont(lf))
	{
		strFaceName = lf.lfFaceName;
		return true;
	}
	return false;
}

bool CXTPFontListBox::SetSelFont(CXTPLogFont& lf)
{
	return SetSelFont(lf.lfFaceName);
}

bool CXTPFontListBox::SetSelFont(const CString& strFaceName)
{
	int iCount = GetCount();
	int i;
	for (i = 0; i < iCount; ++i)
	{
		CXTPLogFont* pLF = (CXTPLogFont*)GetItemDataPtr(i);
		if (pLF != NULL)
		{
			if (strFaceName.CompareNoCase(pLF->lfFaceName) == 0)
			{
				SetCurSel(i);
				return true;
			}
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////
// CXTPFontComboBox
///////////////////////////////////////////////////////////////////////////


CXTPFontComboBox::CXTPFontComboBox()
{
	m_csSymbol  = _T("AaBbCc");
	m_dwStyle   = xtpFontBoth;
}

CXTPFontComboBox::~CXTPFontComboBox()
{

}

IMPLEMENT_DYNAMIC(CXTPFontComboBox, CXTPComboBox)

BEGIN_MESSAGE_MAP(CXTPFontComboBox, CXTPComboBox)
	//{{AFX_MSG_MAP(CXTPFontComboBox)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC*  pDC       = CDC::FromHandle(lpDIS->hDC);
	UINT  itemState = lpDIS->itemState;
	UINT  itemID    = lpDIS->itemID;
	CRect rcItem    = lpDIS->rcItem;

	if (itemID == (UINT)-1)
	{
		return;
	}

	CXTPLogFont* pLF = (CXTPLogFont*)lpDIS->itemData;
	if (pLF == NULL)
	{
		return;
	}

	BOOL bDisabled = ((itemState & ODS_DISABLED) == ODS_DISABLED);
	BOOL bSelected = ((itemState & ODS_SELECTED) == ODS_SELECTED);
	BOOL bFocus    = ((itemState & ODS_FOCUS)    == ODS_FOCUS);

	// draw background.
	if (bDisabled)
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_GRAYTEXT));
		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		if (bSelected)
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_HIGHLIGHT));
		}
		else
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));
			pDC->SetBkColor(GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(&rcItem, GetXtremeColor(COLOR_WINDOW));
		}

		// draw focus rectangle.
		if (bFocus)
		{
			pDC->DrawFocusRect(&rcItem);
		}
	}

	CString csFaceName = pLF->lfFaceName;

	// true type or device flag set by SetItemData
	if (pLF->dwType & TRUETYPE_FONTTYPE)
	{
		m_ilFontType.Draw(pDC, 1, rcItem.TopLeft(), ILD_NORMAL);
	}

	// Define the size of the text area to draw
	CRect rcText(rcItem);
	rcText.left += BMW + 6;

	pDC->SetBkMode(TRANSPARENT);

	if (m_dwStyle & xtpFontGUI)
	{
		CXTPFontDC fontDC(pDC, &XTPAuxData().font);
		pDC->DrawText(csFaceName, rcText, DT_VCENTER | DT_SINGLELINE);
	}

	if (m_dwStyle & xtpFontSample)
	{
		if (m_dwStyle & xtpFontGUI)
			rcText.left = rcText.left + CXTPFontEnum::Get().GetMaxWidth() + 5;

		if ((m_dwStyle & xtpFontGUI) || (pLF->lfCharSet == SYMBOL_CHARSET))
			csFaceName = m_csSymbol;

		// Create the font to be displayed and initialize the device context.
		CFont font;
		font.CreateFontIndirect(pLF);

		// Draw the text item.
		CXTPFontDC fontDC(pDC, &font);
		pDC->DrawText(csFaceName, rcText, DT_VCENTER | DT_SINGLELINE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPFontComboBox diagnostics

#ifdef _DEBUG
void CXTPFontComboBox::AssertValid() const
{
	CXTPComboBox::AssertValid();

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

	// combo box is owner drawn, and must be created with the
	// following styles:

	ASSERT(dwStyle & CBS_OWNERDRAWFIXED);
	ASSERT(dwStyle & CBS_DROPDOWN);
	ASSERT(dwStyle & CBS_SORT);
	ASSERT(dwStyle & CBS_HASSTRINGS);
}
#endif

void CXTPFontComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS->CtlType == ODT_COMBOBOX);
	lpMIS->itemHeight = (::GetSystemMetrics(SM_CYVTHUMB)-::GetSystemMetrics(SM_CYEDGE));
	lpMIS->itemWidth = 0;
}

int CXTPFontComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	ASSERT_VALID(this);
	ASSERT(lpCIS->CtlType == ODT_LISTBOX);

	int iItem1 = (int)(WORD)lpCIS->itemID1;
	if (iItem1 == -1)
	{
		return -1;
	}

	CString strItem1;
	GetLBText(iItem1, strItem1);

	int iItem2 = (int)(WORD)lpCIS->itemID2;
	if (iItem2 == -1)
	{
		return -1;
	}

	CString strItem2;
	GetLBText(iItem2, strItem2);

	return strItem1.Collate(strItem2);
}

void CXTPFontComboBox::InitControl(LPCTSTR lpszFaceName, UINT nWidth/*= 0*/, BOOL bEnable/*= TRUE*/)
{
	ASSERT_VALID(this);
	ResetContent();

	// MFCBUG: adjust height so display is the same as non-owner drawn
	// CComboBoxes. MFC adjusts the height of an owner-drawn CComboBox
	// 2-3 pixels larger than a non owner-drawn combo.

	SetItemHeight(-1, (::GetSystemMetrics(SM_CYVTHUMB)-::GetSystemMetrics(SM_CYEDGE)));

	if (nWidth != 0)
	{
		SetDroppedWidth(nWidth);
	}

	// moved from constructor so resource is loaded when this gets called
	if (!m_ilFontType.m_hImageList)
	{
		VERIFY(m_ilFontType.Create(
			XTP_IDB_FONTTYPE, BMW, 1, RGB(255, 0, 255)));
	}

	EnableAutoCompletion(bEnable);

	CXTPFontList& fontList = CXTPFontEnum::Get().GetFontList();

	POSITION pos;
	for (pos = fontList.GetHeadPosition(); pos; fontList.GetNext(pos))
	{
		CXTPLogFont& lf = fontList.GetAt(pos);
		int iIndex = AddString(lf.lfFaceName);

		SetItemDataPtr(iIndex, &lf);
	}

	if (lpszFaceName && _tcslen(lpszFaceName))
	{
		SetSelFont(lpszFaceName);
	}
}

bool CXTPFontComboBox::GetSelFont(CXTPLogFont& lf)
{
	int iCurSel = GetCurSel();
	if (iCurSel == CB_ERR)
	{
		CString strWindowText;
		GetWindowText(strWindowText);

		iCurSel = SelectString(-1, strWindowText);

		if (iCurSel == CB_ERR)
		{
			return false;
		}
	}

	CXTPLogFont* pLF = (CXTPLogFont*)GetItemDataPtr(iCurSel);
	if (pLF == NULL)
	{
		return false;
	}

	MEMCPY_S((void*)&lf, (const void*)pLF, sizeof(CXTPLogFont));

	return true;
}

bool CXTPFontComboBox::GetSelFont(CString& strFaceName)
{
	CXTPLogFont lf;
	if (GetSelFont(lf))
	{
		strFaceName = lf.lfFaceName;
		return true;
	}
	return false;
}

bool CXTPFontComboBox::SetSelFont(CXTPLogFont& lf)
{
	return SetSelFont(lf.lfFaceName);
}

bool CXTPFontComboBox::SetSelFont(const CString& strFaceName)
{
	int iCount = GetCount();
	int i;
	for (i = 0; i < iCount; ++i)
	{
		CXTPLogFont* pLF = (CXTPLogFont*)GetItemDataPtr(i);
		if (pLF != NULL)
		{
			if (strFaceName.CompareNoCase(pLF->lfFaceName) == 0)
			{
				SetCurSel(i);
				return true;
			}
		}
	}

	return false;
}

BOOL CXTPFontComboBox::OnDropDown()
{
	GetSelFont(m_csSelected);
	return FALSE; // continue routing.
}

void CXTPFontComboBox::NotifyOwner(UINT nCode)
{
	CWnd* pWndOwner = GetOwner();
	if (::IsWindow(pWndOwner->GetSafeHwnd()))
	{
		pWndOwner->SendMessage(WM_COMMAND,
			MAKEWPARAM(GetDlgCtrlID(), nCode), (LPARAM)m_hWnd);
	}
}

BOOL CXTPFontComboBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				if (GetDroppedState() == TRUE)
				{
					ShowDropDown(FALSE);
					SetSelFont(m_csSelected);
					NotifyOwner(CBN_SELENDCANCEL);
				}
				return TRUE;
			}

		case VK_SPACE:
		case VK_RETURN:
			{
				if (GetDroppedState() == TRUE)
				{
					ShowDropDown(FALSE);
					NotifyOwner(CBN_SELENDOK);
				}
				return TRUE;
			}

		case VK_UP:
		case VK_DOWN:
			{
				if (GetDroppedState() == FALSE)
				{
					ShowDropDown(TRUE);
					return TRUE;
				}
				break;
			}
		}
	}

	return CXTPComboBox::PreTranslateMessage(pMsg);
}
