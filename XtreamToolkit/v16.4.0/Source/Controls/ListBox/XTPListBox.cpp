// XTPListBox.cpp : implementation of the CXTPListBox class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls//Util/XTPGlobal.h>
#include <Controls/ListBox/XTPListBox.h>

#include <Controls/ListBox/Themes/XTPListBoxThemeOfficeXP.h>
#include <Controls/ListBox/Themes/XTPListBoxThemeOffice2003.h>
#include <Controls/ListBox/Themes/XTPListBoxThemeOffice2013.h>
#include <Controls/ListBox/Themes/XTPListBoxThemeResource.h>
#include <Controls/ListBox/Themes/XTPListBoxThemeDefault.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CXTPListBoxTheme::DrawItemText(CDC *pDC, CRect rcText, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS)
{
	CString strText;
	pListBox->GetText(lpDIS->itemID, strText);

	pDC->DrawText(strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
}


/////////////////////////////////////////////////////////////////////////////
// CXTPListBox
/////////////////////////////////////////////////////////////////////////////
CXTPListBox::CXTPListBox()
	: m_bPreSubclassInit(TRUE)
	, m_pTheme(NULL)
	, m_theme(xtpControlThemeDefault)
{
	m_nItemHeight = 0;
	m_nTextPadding = 2;
	m_nHotItem = -1;

	SetTheme(xtpControlThemeDefault);
}

CXTPListBox::~CXTPListBox()
{
	SAFE_DELETE(m_pTheme);
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
	CDC *pDC = CDC::FromHandle(lpDIS->hDC);

	if (NULL != m_pTheme)
	{
		m_pTheme->DrawItem(pDC, this, lpDIS);
	}
#if 0
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
#endif
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

	m_bPreSubclassInit = FALSE;

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
/*
	if (m_nStyle != xtpListBoxOffice2007)
		return;
*/
	BOOL bOutside = FALSE;
	int nHotItem = 0;
	if (TryItemFromPoint(point, bOutside, nHotItem))
	{
		if (bOutside) nHotItem = -1;

		if (nHotItem != m_nHotItem)
		{
			m_nHotItem = nHotItem;
			Invalidate(FALSE);

			if (m_nHotItem != -1)
			{
				TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT };
				_TrackMouseEvent(&tme);
			}
		}
	}
}

void CXTPListBox::OnSetFocus(CWnd *pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);
	Invalidate(FALSE);
}

void CXTPListBox::OnKillFocus(CWnd *pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);
	Invalidate(FALSE);
}

void CXTPListBox::SetTheme(XTPControlTheme theme)
{
	SAFE_DELETE(m_pTheme);

	switch(theme)
	{
	case xtpControlThemeDefault:
		m_pTheme = new CXTPListBoxThemeDefault();
		break;
	case xtpControlThemeOfficeXP:
		m_pTheme = new CXTPListBoxThemeOfficeXP();
		break;
	case xtpControlThemeOffice2013:
		m_pTheme = new CXTPListBoxThemeOffice2013();
		break;
	case xtpControlThemeResource:
		m_pTheme = new CXTPListBoxThemeResource();
		break;
	}

	if (m_hWnd)
	{
		Invalidate(FALSE);
	}
}

void CXTPListBox::SetListStyle(XTPListBoxStyle style)
{
	switch(style)
	{
	case xtpListBoxOfficeXP:    SetTheme(xtpControlThemeOfficeXP); break;
	case xtpListBoxOffice2007:  SetTheme(xtpControlThemeResource); break;
	case xtpListBoxOffice2013:  SetTheme(xtpControlThemeOffice2013); break;
	}
}

XTPListBoxStyle CXTPListBox::GetListStyle() const
{
	switch(m_theme)
	{
	case xtpControlThemeOfficeXP: return xtpListBoxOfficeXP;
	case xtpControlThemeResource: return xtpListBoxOffice2007;
	case xtpControlThemeOffice2013: return xtpListBoxOffice2013;
	default:                      return xtpListBoxStandard;
	}
}


BOOL CXTPListBox::HasFocus() const
{
	return (::GetFocus() == GetSafeHwnd());
}

BOOL CXTPListBox::TryItemFromPoint(CPoint pt, BOOL& bOutside, int& nIndex) const
{
	BOOL bFound = FALSE;
	int nCount = GetCount();
	if (0 < nCount)
	{
		nIndex = ItemFromPoint(pt, bOutside);
		if (!(0 <= nIndex && nIndex < nCount))
		{
			nIndex = -1;
		}
	}
	else
	{
		nIndex = -1;
	}

	return bFound;
}
