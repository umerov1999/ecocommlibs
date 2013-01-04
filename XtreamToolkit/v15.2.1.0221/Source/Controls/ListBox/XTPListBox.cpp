// XTEListBox.cpp : implementation of the CXTPListBox class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"

#include "../Util/XTPControlTheme.h"
#include "../Util/XTPGlobal.h"
#include "XTPListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPListBox
/////////////////////////////////////////////////////////////////////////////
CXTPListBox::CXTPListBox()
: m_bPreSubclassInit(true)
{
	m_nStyle = xtpListBoxStandard;
	m_nItemHeight = 0;
	m_nTextPadding = 2;
	m_nHotItem = -1;
}

CXTPListBox::~CXTPListBox()
{

}

IMPLEMENT_DYNAMIC(CXTPListBox, CListBox)

BEGIN_MESSAGE_MAP(CXTPListBox, CListBox)
	//{{AFX_MSG_MAP(CXTPListBox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)

	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPListBox::Initialize(bool bAutoFont/*= true*/)
{
	// set the font for the list box.
	if (bAutoFont)
	{
		SetFont(&XTPAuxData().font);
	}
}

void CXTPListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;

	if ((lpDIS->itemID != (UINT)-1) && (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		COLORREF clrWindow = GetBackColor();
		COLORREF clrWindowText = IsWindowEnabled() ? GetXtremeColor(COLOR_WINDOWTEXT) : GetXtremeColor(COLOR_GRAYTEXT);
		BOOL bSelected = ((lpDIS->itemState & ODS_SELECTED) != 0);
		CRect rcText(rcItem);
		rcText.DeflateRect(m_nTextPadding, 0);

		if (bSelected)
		{
			clrWindow = IsWindowEnabled() ? GetXtremeColor(COLOR_HIGHLIGHT) : GetXtremeColor(COLOR_GRAYTEXT);
			clrWindowText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
		}

		if (m_nStyle == xtpListBoxOfficeXP && bSelected && IsWindowEnabled())
		{
			clrWindowText = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
			clrWindow = GetXtremeColor(XPCOLOR_HIGHLIGHT);
			pDC->Draw3dRect(rcItem, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
			rcItem.DeflateRect(1, 1);
		}

		if (m_nStyle == xtpListBoxOffice2007)
		{
			CXTPResourceImages* pImages = XTPResourceImages();

			BOOL bHasFocus = ::GetFocus() == m_hWnd;
			BOOL bHighlighted = (int)lpDIS->itemID == m_nHotItem;

			if ((bSelected || bHighlighted) && IsWindowEnabled())
			{
				CXTPResourceImage* pImage = pImages->LoadFile(_T("LISTBOX"));
				if (pImage)
				{
					pImage->DrawImage(pDC, rcItem, pImage->GetSource(bSelected && bHighlighted ? 2 :
						bHasFocus && bSelected ? 1 : !bHasFocus && bSelected ? 3 : 0, 4), CRect(4, 4, 4, 4), COLORREF_NULL);
					clrWindow = COLORREF_NULL;
				}
			}

			pDC->SetBkMode(TRANSPARENT);
			clrWindowText = pImages->GetImageColor(_T("LISTBOX"), _T("NormalText"));
		}

		// set the text and text background colors, then repaint the item.
		pDC->SetBkColor(clrWindow);
		pDC->SetTextColor(clrWindowText);
		if (clrWindow != COLORREF_NULL) pDC->FillSolidRect(&rcItem, clrWindow);

		CString strText;
		GetText(lpDIS->itemID, strText);

		pDC->DrawText(strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
	}
	if ((lpDIS->itemAction & ODA_FOCUS) && (m_nStyle != xtpListBoxOffice2007))
		pDC->DrawFocusRect(&lpDIS->rcItem);
}

void CXTPListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemHeight = m_nItemHeight > 0 ? m_nItemHeight : ::GetSystemMetrics(SM_CYMENU)-2;
}

BOOL CXTPListBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

COLORREF CXTPListBox::GetBackColor()
{
	return GetXtremeColor(COLOR_WINDOW);
}

void CXTPListBox::OnPaint()
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect r;
	GetClientRect(&r);

	// Paint to a memory device context to reduce screen flicker.
	CXTPBufferDC memDC(dc, r);
	memDC.FillSolidRect(r, GetBackColor());

	CPoint ptOrg = memDC.GetWindowOrg();

	// Let the window do its default painting   ...
	CListBox::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
	memDC.SetWindowOrg(ptOrg);
}

LRESULT CXTPListBox::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	CListBox::DefWindowProc(WM_ERASEBKGND, wParam, 0);
	return CListBox::DefWindowProc(WM_PRINTCLIENT, wParam, lParam);
}

void CXTPListBox::Init()
{
	// make sure the style is set to LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS.
	DWORD dwStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if ((dwStyle & LBS_OWNERDRAWVARIABLE) == 0)
		dwStyle |= LBS_OWNERDRAWVARIABLE;

	if ((dwStyle & LBS_HASSTRINGS) == 0)
		dwStyle |= LBS_HASSTRINGS;

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
}

void CXTPListBox::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();

	if (m_bPreSubclassInit)
		Init();
}

BOOL CXTPListBox::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CListBox::PreCreateWindow(cs))
		return FALSE;

	m_bPreSubclassInit = false;

	return TRUE;
}

int CXTPListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init();

	return 0;
}



void CXTPListBox::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CListBox::OnMouseMove(nFlags, point);

	if (m_nStyle != xtpListBoxOffice2007)
		return;

	BOOL bOutside = FALSE;
	int nHotItem = ItemFromPoint(point, bOutside);
	if (bOutside) nHotItem = -1;

	if (nHotItem != m_nHotItem)
	{
		m_nHotItem = nHotItem;
		Invalidate(FALSE);

		if (m_nHotItem != -1)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}
	}
}

void CXTPListBox::OnSetFocus(CWnd* pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);

	if (m_nStyle == xtpListBoxOffice2007)
	{
		Invalidate(FALSE);
	}
}

void CXTPListBox::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

	if (m_nStyle == xtpListBoxOffice2007)
	{
		Invalidate(FALSE);
	}
}

void CXTPListBox::SetTheme(XTPControlTheme nTheme)
{
	if (nTheme == xtpControlThemeResource)
		SetListStyle(xtpListBoxOffice2007);
	else if (nTheme == xtpControlThemeOfficeXP)
		SetListStyle(xtpListBoxOfficeXP);
	else
		SetListStyle(xtpListBoxStandard);

}
