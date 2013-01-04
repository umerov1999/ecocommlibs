// XTPPropertyPage.cpp : implementation file
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"

#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabControl.h"

#include "../Util/XTPControlTheme.h"
#include "../ListBox/XTPListBox.h"
#include "../Util/XTPGlobal.h"
#include "../Resize/XTPResizeRect.h"
#include "../Resize/XTPResizePoint.h"
#include "../Resize/XTPResize.h"
#include "XTPPropertyPage.h"
#include "XTPPropertySheet.h"
#include "XTPPropertyPageNavigator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyPage property page

IMPLEMENT_DYNCREATE(CXTPPropertyPage, CPropertyPage)

CXTPPropertyPage::CXTPPropertyPage()
	: CPropertyPage(), CXTPResize(this, xtpResizeNoClipChildren | xtpResizeNoSizeIcon)
{
	CommonConstruct();
}
CXTPPropertyPage::CXTPPropertyPage(UINT nIDTemplate, UINT nIDCaption)
	: CPropertyPage(nIDTemplate, nIDCaption), CXTPResize(this, xtpResizeNoClipChildren | xtpResizeNoSizeIcon)
{
	CommonConstruct();
}
CXTPPropertyPage::CXTPPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption)
	: CPropertyPage(lpszTemplateName, nIDCaption), CXTPResize(this, xtpResizeNoClipChildren | xtpResizeNoSizeIcon)
{
	CommonConstruct();
}

void CXTPPropertyPage::CommonConstruct()
{
	m_pSheet = 0;
	m_dwData = 0;
	m_bModified = FALSE;
	m_bInsideUpdate = FALSE;
	m_totalDev = CSize(0, 0);
	m_lineDev = CXTPDrawHelpers::Dlu2Pix(5, 5);
	m_bCreated = FALSE;
}


CXTPPropertyPage::~CXTPPropertyPage()
{
}

void CXTPPropertyPage::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
	m_psp.pszTitle = m_strCaption;
	m_psp.dwFlags |= PSP_USETITLE;
}

CString CXTPPropertyPage::GetCaption() const
{
	return m_strCaption;
}

void CXTPPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPPropertyPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

#pragma pack(push, 1)

struct DLGEXTEMPLATE
{
	WORD dlgVer;
	WORD wSignature;
	DWORD helpID;
	DWORD exStyle;
	DWORD dwStyle;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
};

#pragma pack(pop)


BOOL CXTPPropertyPage::LoadTemplate(HGLOBAL& hTemplate, HINSTANCE& hInst)
{
	hTemplate = 0;
	hInst = AfxGetInstanceHandle();

	if ((m_psp.dwFlags & PSP_DLGINDIRECT) && m_psp.pResource)
	{
		CDialogTemplate dlgTemp((LPCDLGTEMPLATE)m_psp.pResource);
		hTemplate = dlgTemp.Detach();
	}
	else
	{

		hInst = AfxFindResourceHandle(m_psp.pszTemplate, RT_DIALOG);
		if (!hInst)
			return FALSE;

		HRSRC hResource = ::FindResource(hInst, m_psp.pszTemplate, RT_DIALOG);
		if (!hResource)
			return FALSE;

		hTemplate = LoadResource(hInst, hResource);
		if (!hTemplate)
			return FALSE;

		DLGTEMPLATE* pTemplate = (DLGTEMPLATE*)LockResource(hTemplate);

		CDialogTemplate dlgTemp(pTemplate);

		UnlockResource(hTemplate);
		FreeResource(hTemplate);

		hTemplate = dlgTemp.Detach();
	}
	return TRUE;

}

BOOL CXTPPropertyPage::LoadTemplate()
{
	HGLOBAL hTemplate = NULL;
	HINSTANCE hInstance = NULL;

	if (!LoadTemplate(hTemplate, hInstance))
		return FALSE;

	if (!hTemplate)
		return  FALSE;

	DLGTEMPLATE* lpDialogTemplate = (DLGTEMPLATE*)GlobalLock(hTemplate);
	if (!lpDialogTemplate)
		return FALSE;

	DLGEXTEMPLATE* lpDialogExTemplate = (DLGEXTEMPLATE*)lpDialogTemplate;

	BOOL bDialogEx = lpDialogExTemplate->wSignature == 0xFFFF;

	m_lineDev = CXTPDrawHelpers::Dlu2Pix(5, 5);

	if ((m_psp.dwFlags & PSP_USETITLE) == 0)
	{
		WORD* lpTitle = bDialogEx ? (WORD*)(lpDialogExTemplate + 1) : (WORD*)(lpDialogTemplate + 1);

		if (*lpTitle == (WORD)-1)        // Skip menu name string or ordinal
			lpTitle += 2;
		else
			while(*lpTitle++);

		if (*lpTitle == (WORD)-1)        // Skip class name string or ordinal
			lpTitle += 2;
		else
			while(*lpTitle++);

		m_strCaption = CString((LPWSTR)lpTitle);
	}

	CDialogTemplate dlgTemp(hTemplate);
	dlgTemp.GetSizeInPixels(&m_totalDev);

	GlobalUnlock(hTemplate);
	GlobalFree(hTemplate);

	return TRUE;
}



BOOL CXTPPropertyPage::CreatePage(CWnd* pParentWnd)
{
	if (GetSafeHwnd())
		return TRUE;

	HGLOBAL hTemplate = NULL;
	HINSTANCE hInstance = NULL;

	if (!LoadTemplate(hTemplate, hInstance))
		return FALSE;

	if (!hTemplate)
		return FALSE;

	DLGTEMPLATE* lpDialogTemplate = (DLGTEMPLATE*)GlobalLock(hTemplate);
	if (!lpDialogTemplate)
		return FALSE;

	DLGEXTEMPLATE* lpDialogExTemplate = (DLGEXTEMPLATE*)lpDialogTemplate;

	DWORD lSaveStyle;
	if (lpDialogExTemplate->wSignature == 0xFFFF)
		lSaveStyle = lpDialogExTemplate->dwStyle;
	else
		lSaveStyle = lpDialogTemplate->style;

	DWORD dwNewStyle = (lSaveStyle & (DS_LOCALEDIT | WS_CLIPCHILDREN | DS_SETFONT | DS_FIXEDSYS))
		| WS_CHILD | WS_TABSTOP | DS_3DLOOK | DS_CONTROL;

	if (lpDialogExTemplate->wSignature == 0xFFFF)
		lpDialogExTemplate->dwStyle = dwNewStyle;
	else
		lpDialogTemplate->style = dwNewStyle;

	BOOL bResult = CreateDlgIndirect(lpDialogTemplate, pParentWnd, hInstance);

	CSize sz;
	CXTPWindowRect rc(this);
	m_totalDev = rc.Size();

	m_bCreated = TRUE;


	GlobalUnlock(hTemplate);
	GlobalFree(hTemplate);

	return bResult;
}

void CXTPPropertyPage::EndDialog(int nID)
{
	// Normally you shouldn't call EndDialog from a page. But in case it does
	// happen during error situations, call CPropertySheet::EndDialog instead.

	CXTPPropertySheet* pParent = DYNAMIC_DOWNCAST(CXTPPropertySheet, GetParent());
	if (pParent != NULL)
		pParent->EndDialog(nID);
}

BEGIN_MESSAGE_MAP(CXTPPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPPropertyPage)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyPage message handlers

BOOL CXTPPropertyPage::OnEraseBkgnd(CDC* pDC)
{
	HBRUSH hBrush = m_pSheet ? m_pSheet->GetNavigator()->GetPageBrush(pDC, this) : NULL;
	if (hBrush)
	{
		CXTPClientRect rc(this);
		::FillRect(pDC->GetSafeHdc(), rc, hBrush);
		return TRUE;
	}

	return CPropertyPage::OnEraseBkgnd(pDC);
}

HBRUSH CXTPPropertyPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_DLG ||
		nCtlColor == CTLCOLOR_STATIC ) && m_pSheet)
	{
		HBRUSH hBrush = m_pSheet->GetNavigator()->GetPageBrush(pDC, this);
		if (hBrush)
		{
			CXTPWindowRect rcPaint(pWnd);
			CXTPWindowRect rcBrush(m_hWnd);
			::SetBrushOrgEx(pDC->GetSafeHdc(), rcBrush.left - rcPaint.left, rcBrush.top - rcPaint.top, NULL);

			::SetBkMode(pDC->GetSafeHdc(), TRANSPARENT);
			::SetTextColor(pDC->GetSafeHdc(), GetSysColor(COLOR_BTNTEXT));
			return hBrush;
		}
	}

	return CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
}

LRESULT CXTPPropertyPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	return CPropertyPage::WindowProc(message, wParam, lParam);
}

BOOL CXTPPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CXTPResize::Init();

	return TRUE;
}

void CXTPPropertyPage::AdjustResizeRect(CSize& szWindow)
{
	CSize szMin = m_totalDev;
	szWindow.cx = max(szWindow.cx, szMin.cx);
	szWindow.cy = max(szWindow.cy, szMin.cy);
}


void CXTPPropertyPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// UpdateBars() handles locking out recursion
	UpdateBars();

	CXTPResize::Size();

}


void CXTPPropertyPage::GetScrollBarSizes(CSize& sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = GetStyle();

	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		sizeSb.cx = GetSystemMetrics(SM_CXVSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cx -= CX_BORDER;
	}
	if (GetScrollBarCtrl(SB_HORZ) == NULL)
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy = GetSystemMetrics(SM_CYHSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cy -= CY_BORDER;
	}
}

BOOL CXTPPropertyPage::GetTrueClientSize(CSize& size, CSize& sizeSb)
	// return TRUE if enough room to add scrollbars if needed
{
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

CPoint CXTPPropertyPage::GetDeviceScrollPosition() const
{
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);

	return pt;
}

// helper to return the state of the scrollbars without actually changing
//  the state of the scrollbars
void CXTPPropertyPage::GetScrollBarState(CSize sizeClient, CSize& needSb,
	CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient)
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// enough room to add scrollbars
	sizeRange = m_totalDev - sizeClient;
		// > 0 => need to scroll
	ptMove = GetDeviceScrollPosition();
		// point to move to (start at current scroll pos)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// need a horizontal scrollbar after all
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
		ptMove.x = sizeRange.cx;
	if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
		ptMove.y = sizeRange.cy;

	// now update the bars as appropriate
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, and ptMove area now all updated
}

void CXTPPropertyPage::ScrollToDevicePosition(POINT ptDev)
{
	ASSERT(ptDev.x >= 0);
	ASSERT(ptDev.y >= 0);

	// Note: ScrollToDevicePosition can and is used to scroll out-of-range
	//  areas as far as CXTPPropertyPage is concerned -- specifically in
	//  the print-preview code.  Since OnScrollBy makes sure the range is
	//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos(SB_HORZ, ptDev.x);
	int yOrig = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, ptDev.y);
	ScrollPage(xOrig - ptDev.x, yOrig - ptDev.y);
}

void CXTPPropertyPage::ScrollPage(int xAmount, int yAmount)
{
	if (xAmount == 0 && yAmount == 0)
		return;

	ScrollWindow(xAmount, yAmount);

	for (int i = 0; i < (int)m_arrItems.GetSize(); i++)
	{
		CXTPResizeRect& rrcWindow = m_arrItems[i]->m_rrcWindow;
		rrcWindow.left += xAmount;
		rrcWindow.right += xAmount;
		rrcWindow.top += yAmount;
		rrcWindow.bottom += yAmount;
	}
}

void CXTPPropertyPage::UpdateBars()
{
	if (m_totalDev == CSize(0, 0) || !m_bCreated)
		return;

	// UpdateBars may cause window to be resized - ignore those resizings
	if (m_bInsideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	CSize sizeClient;
	CSize sizeSb;

	// get client rect
	if (!GetTrueClientSize(sizeClient, sizeSb))
	{
		// no room for scroll bars (common for zero sized elements)
		CRect rect;
		GetClientRect(&rect);
		if (rect.right > 0 && rect.bottom > 0)
		{
			// if entire client area is not invisible, assume we have
			//  control over our scrollbars
			EnableScrollBarCtrl(SB_BOTH, FALSE);
		}
		m_bInsideUpdate = FALSE;
		return;
	}

	// enough room to add scrollbars
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// get the current scroll bar state given the true client area
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
	if (needSb.cx)
		sizeClient.cy -= sizeSb.cy;
	if (needSb.cy)
		sizeClient.cx -= sizeSb.cx;

	// first scroll the window as needed
	ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	if (needSb.cx)
	{
		info.nPage = sizeClient.cx;
		info.nMax = m_totalDev.cx-1;
		if (!SetScrollInfo(SB_HORZ, &info, TRUE))
			SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
	}
	EnableScrollBarCtrl(SB_VERT, needSb.cy);
	if (needSb.cy)
	{
		info.nPage = sizeClient.cy;
		info.nMax = m_totalDev.cy-1;
		if (!SetScrollInfo(SB_VERT, &info, TRUE))
			SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// remove recursion lockout
	m_bInsideUpdate = FALSE;
}
void CXTPPropertyPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
}

void CXTPPropertyPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	OnScroll(MAKEWORD(-1, nSBCode), nPos);
}

BOOL CXTPPropertyPage::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	// we don't handle anything but scrolling just now
	if (fFlags & (MK_SHIFT | MK_CONTROL))
		return FALSE;

	// we can't get out of it--perform the scroll ourselves
	return DoMouseWheel(fFlags, zDelta, point);
}

// This function isn't virtual. If you need to override it,
// you really need to override OnMouseWheel() here or in
// CSplitterWnd.

BOOL CXTPPropertyPage::DoMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	UNUSED_ALWAYS(point);
	UNUSED_ALWAYS(fFlags);

	// if we have a vertical scroll bar, the wheel scrolls that
	// if we have _only_ a horizontal scroll bar, the wheel scrolls that
	// otherwise, don't do any work at all

	DWORD dwStyle = GetStyle();
	CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
	BOOL bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_VSCROLL);

	pBar = GetScrollBarCtrl(SB_HORZ);
	BOOL bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_HSCROLL);

	if (!bHasVertBar && !bHasHorzBar)
		return FALSE;

	BOOL bResult = FALSE;
	UINT uWheelScrollLines = 3;

	if (bHasVertBar)
	{
		bResult = OnScrollBy(CSize(0, (zDelta < 0 ? 1 : -1) * m_lineDev.cy * uWheelScrollLines), TRUE);
	}
	else if (bHasHorzBar)
	{
		bResult = OnScrollBy(CSize((zDelta < 0 ? 1 : -1) * m_lineDev.cy * uWheelScrollLines, 0), TRUE);
	}

	if (bResult)
		UpdateWindow();

	return bResult;
}


BOOL CXTPPropertyPage::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// calc new x position
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.cx;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.cx;
		break;
	case SB_PAGEUP:
		x -= CXTPClientRect(this).Width();
		break;
	case SB_PAGEDOWN:
		x += CXTPClientRect(this).Width();
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_lineDev.cy;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.cy;
		break;
	case SB_PAGEUP:
		y -= CXTPClientRect(this).Height();
		break;
	case SB_PAGEDOWN:
		y += CXTPClientRect(this).Height();
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
		UpdateWindow();

	return bResult;
}

BOOL CXTPPropertyPage::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		ScrollPage(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
	}
	return TRUE;
}

void CXTPPropertyPage::OnNcPaint()
{
	Default();

	if (!m_pSheet)
		return;

	CWindowDC dc(this);
	CXTPWindowRect rcPage(this);
	rcPage.OffsetRect(-rcPage.TopLeft());

	if (m_pSheet->GetPageBorderStyle() == xtpPageBorderFrame)
	{
		BOOL bFrame = FALSE;
		CXTPWinThemeWrapper wrpTreeView;
		wrpTreeView.OpenTheme(0, L"LISTBOX");
		if (wrpTreeView.IsAppThemed())
		{
			COLORREF clr;
			if (SUCCEEDED(wrpTreeView.GetThemeColor(0, 0, TMT_BORDERCOLOR, &clr)))
			{
				dc.Draw3dRect(rcPage, clr, clr);
				rcPage.DeflateRect(1, 1);
				dc.Draw3dRect(rcPage, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_WINDOW));
				rcPage.DeflateRect(1, 1);
				bFrame = TRUE;
			}
		}
		if (!bFrame)
		{
			dc.DrawEdge(rcPage, EDGE_SUNKEN, BF_RECT);
			rcPage.DeflateRect(2, 2);
		}
	}
	else if (m_pSheet->GetPageBorderStyle() == xtpPageBorderBottomLine)
	{
		dc.FillSolidRect(rcPage.left, rcPage.bottom - 2, rcPage.Width(), 1, GetXtremeColor(COLOR_BTNSHADOW));
		dc.FillSolidRect(rcPage.left, rcPage.bottom - 1, rcPage.Width(), 1, GetXtremeColor(COLOR_BTNHIGHLIGHT));
		rcPage.DeflateRect(0, 0, 0, 2);
	}

	if ((GetStyle() & (WS_HSCROLL | WS_VSCROLL)) == (WS_HSCROLL | WS_VSCROLL))
	{
		CRect rcSizeIcon(rcPage.right - GetSystemMetrics(SM_CXVSCROLL),
			rcPage.bottom - GetSystemMetrics(SM_CYHSCROLL), rcPage.right, rcPage.bottom);

		HBRUSH hBrush = m_pSheet->GetNavigator()->GetPageBrush(&dc, this);
		if (hBrush)
		{
			CXTPClientRect rc(this);
			::FillRect(dc, rcSizeIcon, hBrush);
		}
		else
		{
			dc.FillSolidRect(rcSizeIcon, GetXtremeColor(COLOR_3DFACE));
		}

	}
}

void CXTPPropertyPage::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if (!m_pSheet)
	{
		CPropertyPage::OnNcCalcSize(bCalcValidRects, lpncsp);
		return;
	}

	if (m_pSheet->GetPageBorderStyle() == xtpPageBorderFrame)
	{
		RECT& rc = lpncsp->rgrc[0];
		rc.left   += 2;
		rc.top    += 2;
		rc.right  -= 2;
		rc.bottom -= 2;
	}

	if (m_pSheet->GetPageBorderStyle() == xtpPageBorderBottomLine)
	{
		lpncsp->rgrc[0].bottom -= 2;
	}

	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CXTPPropertyPage::OnChildSetFocus(HWND hWndFocus)
{
	if ((GetStyle() & (WS_HSCROLL | WS_VSCROLL)) == 0)
		return;

	CWnd* pWnd = CWnd::FromHandle(hWndFocus);
	if (!pWnd)
		return;

	CRect rc;
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);

	CXTPClientRect rcClient(this);

	CSize szScroll(0, 0);

	if (rc.bottom > rcClient.bottom)
		szScroll.cy = rc.bottom - rcClient.bottom;
	if (rc.top - szScroll.cy < rcClient.top)
		szScroll.cy = rc.top - rcClient.top;

	if (rc.right > rcClient.right)
		szScroll.cx = rc.right - rcClient.right;
	if (rc.left - szScroll.cx < rcClient.left)
		szScroll.cx = rc.left - rcClient.left;

	if (szScroll != CSize(0, 0))
	{
		OnScrollBy(szScroll, TRUE);
	}
}




//////////////////////////////////////////////////////////////////////////
// CXTPPropertyPageCaption

CXTPPropertyPageStaticCaption::CXTPPropertyPageStaticCaption()
{

};

BEGIN_MESSAGE_MAP(CXTPPropertyPageStaticCaption, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CXTPPropertyPageStaticCaption::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPPropertyPageStaticCaption::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint);
	CXTPClientRect rc(this);

	dc.FillSolidRect(rc, RGB(221, 231, 238));
	dc.FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, RGB(197, 197, 197));
	rc.DeflateRect(7, 0);

	CString strText;
	GetWindowText(strText);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 21, 110));
	CXTPFontDC font(&dc, &XTPAuxData().fontBold);
	dc.DrawText(strText, rc, DT_VCENTER | DT_SINGLELINE);
}
