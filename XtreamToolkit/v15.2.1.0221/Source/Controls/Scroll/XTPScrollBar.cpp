// XTPScrollBar.cpp : implementation of the CXTPScrollBar class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPWinThemeWrapper.h"
#include "../Util/XTPControlTheme.h"

#include "XTPScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define XTP_HTSCROLLUP          60
#define XTP_HTSCROLLDOWN        61
#define XTP_HTSCROLLUPPAGE      62
#define XTP_HTSCROLLDOWNPAGE    63
#define XTP_HTSCROLLTHUMB       64
#define XTP_HTSCROLLPOPUP       65


IMPLEMENT_DYNAMIC(CXTPScrollBar, CScrollBar)

/////////////////////////////////////////////////////////////////////////////
// CXTPScrollBar
/////////////////////////////////////////////////////////////////////////////

CXTPScrollBar::CXTPScrollBar()
{
	m_bUseVisualStyle = TRUE;
	m_nTheme = xtpControlThemeDefault;


	m_bPreSubclassInit = TRUE;

	ZeroMemory(&m_spi, sizeof(m_spi));

	m_pSBTrack = NULL;

	m_themeScrollBar = new CXTPWinThemeWrapper();
}

CXTPScrollBar::~CXTPScrollBar()
{
	SAFE_DELETE(m_themeScrollBar);
}

void CXTPScrollBar::SetTheme(XTPControlTheme nTheme)
{
	m_nTheme = nTheme;

	m_bUseVisualStyle = (m_nTheme == xtpControlThemeDefault);

	RefreshMetrics();

	RedrawScrollBar();
}

void CXTPScrollBar::SetUseVisualStyle(BOOL bUseVisualStyle/* = TRUE*/)
{
	m_bUseVisualStyle = bUseVisualStyle;

	RefreshMetrics();
	RedrawScrollBar();
}



BEGIN_MESSAGE_MAP(CXTPScrollBar, CScrollBar)
//{{AFX_MSG_MAP(CXTPScrollBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(SBM_SETSCROLLINFO, OnSetScrollInfo)
	ON_MESSAGE(SBM_GETSCROLLINFO, OnGetScrollInfo)
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_WM_SYSCOLORCHANGE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CXTPScrollBar::Init()
{
	m_spi.fVert = GetStyle() & SBS_VERT;

	RefreshMetrics();
	Invalidate(FALSE);
}

void CXTPScrollBar::PreSubclassWindow()
{
	CScrollBar::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPScrollBar::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CScrollBar::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

void CXTPScrollBar::OnSysColorChange()
{
	CScrollBar::OnSysColorChange();
	XTPColorManager()->RefreshColors();

	RefreshMetrics();

	Invalidate(FALSE);
}



//////////////////////////////////////////////////////////////////////////
// Drawing.

//////////////////////////////////////////////////////////////////////////
//  CScrollBarCtrl

#define IDSYS_SCROLL 23345


void CXTPScrollBar::CalcScrollBarInfo(LPRECT lprc, SCROLLBARPOSINFO* pSBInfo, SCROLLINFO* pSI)
{
	int cpx;
	DWORD dwRange;
	int denom;
	BOOL fVert = pSBInfo->fVert;

	pSBInfo->rc = *lprc;
	pSBInfo->pxPopup = 0;

	if (fVert)
	{
		pSBInfo->pxTop = lprc->top;
		pSBInfo->pxBottom = lprc->bottom;
		pSBInfo->pxLeft = lprc->left;
		pSBInfo->pxRight = lprc->right;
		pSBInfo->cpxThumb = m_cyVScroll;
	}
	else
	{
		pSBInfo->pxTop = lprc->left;
		pSBInfo->pxBottom = lprc->right;
		pSBInfo->pxLeft = lprc->top;
		pSBInfo->pxRight = lprc->bottom;
		pSBInfo->cpxThumb = m_cxHScroll;
	}

	pSBInfo->pos = pSI->nPos;
	pSBInfo->page = pSI->nPage;
	pSBInfo->posMin = pSI->nMin;
	pSBInfo->posMax = pSI->nMax;

	dwRange = ((DWORD)(pSBInfo->posMax - pSBInfo->posMin)) + 1;

	cpx = min((pSBInfo->pxBottom - pSBInfo->pxTop) / 2, pSBInfo->cpxThumb);

	pSBInfo->pxUpArrow  = pSBInfo->pxTop    + cpx;
	pSBInfo->pxDownArrow = pSBInfo->pxBottom - cpx;

	if ((pSBInfo->page != 0) && (dwRange != 0))
	{
		int i = MulDiv(pSBInfo->pxDownArrow - pSBInfo->pxUpArrow,
			pSBInfo->page, dwRange);

		pSBInfo->cpxThumb = max(pSBInfo->cpxThumb / 2, i);
	}

	pSBInfo->pxMin = pSBInfo->pxTop + cpx;
	pSBInfo->cpx = pSBInfo->pxBottom - cpx - pSBInfo->cpxThumb - pSBInfo->pxMin;

	denom = dwRange - (pSBInfo->page ? pSBInfo->page : 1);
	if (denom)
		pSBInfo->pxThumbTop = MulDiv(pSBInfo->pos - pSBInfo->posMin,
		pSBInfo->cpx, denom) + pSBInfo->pxMin;
	else
		pSBInfo->pxThumbTop = pSBInfo->pxMin - 1;

	pSBInfo->pxThumbBottom = pSBInfo->pxThumbTop + pSBInfo->cpxThumb;
}

void CXTPScrollBar::SetupScrollInfo()
{
	CRect rc = GetScrollBarRect();

	SCROLLINFO si;
	GetScrollInfo(&si);

	CalcScrollBarInfo(&rc, &m_spi, &si);
}

int CXTPScrollBar::HitTestScrollBar(POINT pt)
{
	int px = m_spi.fVert ? pt.y : pt.x;

	if (!::PtInRect(&m_spi.rc, pt))
		return HTNOWHERE;

	if (px < m_spi.pxUpArrow)
		return XTP_HTSCROLLUP;

	if (m_spi.pxPopup > 0 && px > m_spi.pxPopup)
		return XTP_HTSCROLLPOPUP;

	if (px >= m_spi.pxDownArrow)
		return XTP_HTSCROLLDOWN;

	if (px < m_spi.pxThumbTop)
		return XTP_HTSCROLLUPPAGE;

	if (px < m_spi.pxThumbBottom)
		return XTP_HTSCROLLTHUMB;

	if (px < m_spi.pxDownArrow)
		return XTP_HTSCROLLDOWNPAGE;

	return HTERROR;
}


//////////////////////////////////////////////////////////////////////////
// Scrolling

void CXTPScrollBar::EndScroll(BOOL fCancel)
{
	SCROLLBARTRACKINFO* pSBTrack = m_pSBTrack;

	if (pSBTrack)
	{
		pSBTrack->cmdSB = 0;
		ReleaseCapture();

		if (pSBTrack->bTrackThumb)
		{
			if (fCancel)
			{
				pSBTrack->posOld = pSBTrack->pSBInfo->pos;
			}

			DoScroll(SB_THUMBPOSITION, pSBTrack->posOld);

			RedrawScrollBar();
		}
		else
		{

			if (pSBTrack->hTimerSB != 0)
			{
				::KillTimer(pSBTrack->hWndTrack, pSBTrack->hTimerSB);
				pSBTrack->hTimerSB = 0;
			}
		}


		DoScroll(SB_ENDSCROLL, 0);
	}
}

int CXTPScrollBar::SBPosFromPx(CXTPScrollBar::SCROLLBARPOSINFO*  pSBInfo, int px)
{
	if (px < pSBInfo->pxMin)
	{
		return pSBInfo->posMin;
	}

	if (px >= pSBInfo->pxMin + pSBInfo->cpx)
	{
		return (pSBInfo->posMax - (pSBInfo->page ? pSBInfo->page - 1 : 0));
	}

	if (pSBInfo->cpx)
		return (pSBInfo->posMin + MulDiv(pSBInfo->posMax - pSBInfo->posMin -
		(pSBInfo->page ? pSBInfo->page - 1 : 0),
		px - pSBInfo->pxMin, pSBInfo->cpx));
	else
		return (pSBInfo->posMin - 1);
}


void CXTPScrollBar::MoveThumb(int px)
{
	SCROLLBARTRACKINFO* pSBTrack = m_pSBTrack;

	if ((pSBTrack == NULL) || (px == pSBTrack->pxOld))
		return;

	SCROLLBARPOSINFO* pSBInfo = m_pSBTrack->pSBInfo;

pxReCalc:

	pSBTrack->posNew = SBPosFromPx(pSBInfo, px);

	if (pSBTrack->posNew != pSBTrack->posOld)
	{
		DoScroll(SB_THUMBTRACK, pSBTrack->posNew);

		pSBTrack->posOld = pSBTrack->posNew;

		if (px >= pSBInfo->pxMin + pSBInfo->cpx)
		{
			px = pSBInfo->pxMin + pSBInfo->cpx;
			goto pxReCalc;
		}

	}

	pSBInfo->pxThumbTop = px;
	pSBInfo->pxThumbBottom = pSBInfo->pxThumbTop + pSBInfo->cpxThumb;
	pSBTrack->pxOld = px;

	RedrawScrollBar();
}

void CXTPScrollBar::TrackThumb(UINT message, CPoint pt)
{
	SCROLLBARTRACKINFO* pSBTrack = m_pSBTrack;
	if (!pSBTrack)
		return;

	SCROLLBARPOSINFO* pSBInfo = pSBTrack->pSBInfo;

	if (HIBYTE(message) != HIBYTE(WM_MOUSEFIRST))
		return;

	if (pSBInfo == NULL)
		return;

	int px;

	if (!PtInRect(&pSBTrack->rcTrack, pt))
		px = pSBInfo->pxStart;
	else
	{
		px = (pSBTrack->pSBInfo->fVert ? pt.y : pt.x) + pSBTrack->dpxThumb;
		if (px < pSBInfo->pxMin)
			px = pSBInfo->pxMin;
		else if (px >= pSBInfo->pxMin + pSBInfo->cpx)
			px = pSBInfo->pxMin + pSBInfo->cpx;
	}

	MoveThumb(px);

	if (message == WM_LBUTTONUP || GetKeyState(VK_LBUTTON) >= 0)
	{
		EndScroll(FALSE);
	}
}

void CXTPScrollBar::TrackBox(UINT message, CPoint point)
{
	SCROLLBARTRACKINFO* pSBTrack = m_pSBTrack;

	if (pSBTrack == NULL)
		return;

	if (message != WM_NULL && HIBYTE(message) != HIBYTE(WM_MOUSEFIRST))
		return;

	if ((pSBTrack->cmdSB == SB_PAGEUP || pSBTrack->cmdSB == SB_PAGEDOWN))
	{
		int* pLength = (int *)&pSBTrack->rcTrack;

		if (pSBTrack->pSBInfo->fVert)
			pLength++;

		if (pSBTrack->cmdSB == SB_PAGEUP)
			pLength[2] = pSBTrack->pSBInfo->pxThumbTop;
		else
			pLength[0] = pSBTrack->pSBInfo->pxThumbBottom;
	}

	BOOL fHit = PtInRect(&pSBTrack->rcTrack, point);

	BOOL fHitChanged = fHit != (BOOL)pSBTrack->fHitOld;

	if (fHitChanged)
	{
		pSBTrack->fHitOld = fHit;
		RedrawScrollBar();
	}

	int cmsTimer = GetDoubleClickTime() / 10;

	switch (message)
	{
	case WM_LBUTTONUP:
		EndScroll(FALSE);
		break;

	case WM_LBUTTONDOWN:
		pSBTrack->hTimerSB = 0;
		cmsTimer = GetDoubleClickTime() * 4 / 5;

		/*
		*** FALL THRU **
		*/

	case WM_MOUSEMOVE:
		if (fHit && fHitChanged)
		{
			pSBTrack->hTimerSB = ::SetTimer(m_pSBTrack->hWndTrack, IDSYS_SCROLL, cmsTimer, NULL);

			DoScroll(pSBTrack->cmdSB, 0);
		}
	}
}

void CXTPScrollBar::ContScroll()
{
	SCROLLBARTRACKINFO* pSBTrack = m_pSBTrack;
	ASSERT(pSBTrack);

	if (pSBTrack == NULL)
		return;

	CPoint pt;
	GetCursorPos(&pt);

	::ScreenToClient(m_pSBTrack->hWndTrack, &pt);

	TrackBox(WM_NULL, pt);

	if (pSBTrack->fHitOld)
	{
		pSBTrack->hTimerSB = ::SetTimer(m_pSBTrack->hWndTrack, IDSYS_SCROLL, GetDoubleClickTime() / 10, NULL);

		DoScroll(pSBTrack->cmdSB, 0);
	}
}

void CXTPScrollBar::CalcTrackDragRect(SCROLLBARTRACKINFO* pSBTrack) const
{

	int     cx;
	int     cy;
	LPINT   pwX, pwY;

	pwX = pwY = (LPINT)&pSBTrack->rcTrack;

	if (pSBTrack->pSBInfo->fVert)
	{
		cy = GetSystemMetrics(SM_CYVTHUMB);
		pwY++;
	}
	else
	{
		cy = GetSystemMetrics(SM_CXHTHUMB);
		pwX++;
	}

	cx = (pSBTrack->pSBInfo->pxRight - pSBTrack->pSBInfo->pxLeft) * 8;
	cy *= 2;

	*(pwX + 0) = pSBTrack->pSBInfo->pxLeft - cx;
	*(pwY + 0) = pSBTrack->pSBInfo->pxTop - cy;
	*(pwX + 2) = pSBTrack->pSBInfo->pxRight + cx;
	*(pwY + 2) = pSBTrack->pSBInfo->pxBottom + cy;
}

void CXTPScrollBar::PerformTrackInit(HWND hWnd, CPoint point, SCROLLBARPOSINFO* pSBInfo, BOOL bDirect)
{

	int px = m_spi.fVert ? point.y : point.x;

	SCROLLBARTRACKINFO* pSBTrack = new SCROLLBARTRACKINFO;
	memset(pSBTrack, 0, sizeof(SCROLLBARTRACKINFO));

	pSBTrack->cmdSB = (UINT)-1;
	pSBTrack->bTrackThumb = FALSE;
	pSBTrack->pSBInfo = pSBInfo;
	pSBTrack->hWndTrack = hWnd;

	m_pSBTrack = pSBTrack;

	RECT rcSB;
	LPINT pwX = (LPINT)&rcSB;
	LPINT pwY = pwX + 1;
	if (!pSBInfo->fVert)
		pwX = pwY--;

	*(pwX + 0) = pSBInfo->pxLeft;
	*(pwY + 0) = pSBInfo->pxTop;
	*(pwX + 2) = pSBInfo->pxRight;
	*(pwY + 2) = pSBInfo->pxBottom;


	if (px < pSBInfo->pxUpArrow)
	{
		pSBInfo->ht = XTP_HTSCROLLUP;
		pSBTrack->cmdSB = SB_LINEUP;
		*(pwY + 2) = pSBInfo->pxUpArrow;

	}
	else if (px >= pSBInfo->pxDownArrow)
	{
		pSBInfo->ht = XTP_HTSCROLLDOWN;
		pSBTrack->cmdSB = SB_LINEDOWN;
		*(pwY + 0) = pSBInfo->pxDownArrow;
	}
	else if (px < pSBInfo->pxThumbTop)
	{
		pSBInfo->ht = XTP_HTSCROLLUPPAGE;
		pSBTrack->cmdSB = SB_PAGEUP;
		*(pwY + 0) = pSBInfo->pxUpArrow;
		*(pwY + 2) = pSBInfo->pxThumbTop;
	}
	else if (px < pSBInfo->pxThumbBottom)
	{
		pSBInfo->ht = XTP_HTSCROLLTHUMB;
DoThumbPos:

		if (pSBInfo->pxDownArrow - pSBInfo->pxUpArrow <= pSBInfo->cpxThumb)
		{
			delete m_pSBTrack;
			m_pSBTrack = NULL;
			return;
		}

		pSBTrack->cmdSB = SB_THUMBPOSITION;
		CalcTrackDragRect(pSBTrack);

		pSBTrack->pxOld = pSBInfo->pxStart = pSBInfo->pxThumbTop;
		pSBTrack->posNew = pSBTrack->posOld = pSBInfo->pos;
		pSBTrack->dpxThumb = pSBInfo->pxStart - px;

		pSBTrack->bTrackThumb = TRUE;

		::SetCapture(hWnd);

		DoScroll(SB_THUMBTRACK, pSBTrack->posOld);

	}
	else if (px < pSBInfo->pxDownArrow)
	{
		pSBInfo->ht = XTP_HTSCROLLDOWNPAGE;
		pSBTrack->cmdSB = SB_PAGEDOWN;
		*(pwY + 0) = pSBInfo->pxThumbBottom;
		*(pwY + 2) = pSBInfo->pxDownArrow;
	}

	if ((bDirect && pSBTrack->cmdSB != SB_LINEUP && pSBTrack->cmdSB != SB_LINEDOWN))
	{
		if (pSBTrack->cmdSB != SB_THUMBPOSITION)
		{
			goto DoThumbPos;
		}
		pSBTrack->dpxThumb = -(pSBInfo->cpxThumb / 2);
	}

	::SetCapture(hWnd);

	if (pSBTrack->cmdSB != SB_THUMBPOSITION)
	{
		CopyRect(&pSBTrack->rcTrack, &rcSB);
	}

	if (!pSBTrack->bTrackThumb)
	{
		TrackBox(WM_LBUTTONDOWN, point);
	}
	else
	{
		TrackThumb(WM_LBUTTONDOWN, point);

	}

	while (::GetCapture() == hWnd)
	{
			MSG msg;

			if (!::GetMessage(&msg, NULL, 0, 0))
			{
				AfxPostQuitMessage((int)msg.wParam);
				break;
			}

			if (!IsWindow(hWnd))
				break;

			UINT cmd = msg.message;

			if (cmd == WM_TIMER && msg.wParam == IDSYS_SCROLL)
			{
				ContScroll();
			}
			else if (cmd >= WM_MOUSEFIRST && cmd <= WM_MOUSELAST)
			{
				CPoint ptScreen = msg.pt;

				::ScreenToClient(hWnd, &ptScreen);

				if (!pSBTrack->bTrackThumb)
				{
					TrackBox(cmd, ptScreen);
				}
				else
				{
					TrackThumb(cmd, ptScreen);
				}
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

	}

	if (pSBTrack->hTimerSB != 0)
	{
		::KillTimer(hWnd, pSBTrack->hTimerSB);
	}

	delete m_pSBTrack;
	m_pSBTrack = NULL;

	if (IsWindow(hWnd))
	{
		GetCursorPos(&point);
		::ScreenToClient(hWnd, &point);
		m_spi.ht = HitTestScrollBar(point);

		RedrawScrollBar();
	}
}



CRect CXTPScrollBar::GetScrollBarRect()
{
	return CXTPClientRect(this);
}

void CXTPScrollBar::GetScrollInfo(SCROLLINFO* psi)
{
	psi->cbSize = sizeof(SCROLLINFO);

	psi->fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	::GetScrollInfo(m_hWnd, SB_CTL, psi);
}

BOOL CXTPScrollBar::IsScrollBarEnabled() const
{
	return IsWindowEnabled();
}

CWnd* CXTPScrollBar::GetParentWindow() const
{
	return GetParent();
}

void CXTPScrollBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	CXTPBufferDC dcMem(dc);

	DefWindowProc(WM_PAINT, (WPARAM)dcMem.GetSafeHdc(), 0);

	m_spi.fVert = GetStyle() & SBS_VERT;

	if (!m_pSBTrack || !m_pSBTrack->bTrackThumb)
		SetupScrollInfo();

	DrawScrollBar(&dcMem);
}

void CXTPScrollBar::DoScroll(int cmd, int pos)
{
	GetParent()->SendMessage((UINT)(m_spi.fVert ? WM_VSCROLL : WM_HSCROLL),
		MAKELONG(cmd, pos), (LPARAM)m_hWnd);
}

void CXTPScrollBar::RedrawScrollBar()
{
	if (m_hWnd) Invalidate(FALSE);
}


void CXTPScrollBar::OnEnable(BOOL /*bEnable*/)
{
	Invalidate(FALSE);
}

void CXTPScrollBar::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPScrollBar::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	int ht = HitTestScrollBar(point);

	if (ht != m_spi.ht)
	{
		m_spi.ht = ht;
		Invalidate(FALSE);

		if (m_spi.ht != HTNOWHERE)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}
	}
}

void CXTPScrollBar::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	if ((GetStyle() & WS_TABSTOP))
		SetFocus();

	PerformTrackInit(m_hWnd, point, &m_spi, (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE);
}

void CXTPScrollBar::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	CScrollBar::OnContextMenu(pWnd, pos);
	Invalidate(FALSE);
}

void CXTPScrollBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

LRESULT CXTPScrollBar::OnSetScrollInfo(WPARAM wParam, LPARAM lParam)
{
	LRESULT bResult = DefWindowProc(SBM_SETSCROLLINFO, FALSE, lParam);

	if (wParam)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}

	return bResult;
}

LRESULT CXTPScrollBar::OnGetScrollInfo(WPARAM wParam, LPARAM lParam)
{
	LRESULT bResult = DefWindowProc(SBM_GETSCROLLINFO, wParam, lParam);

	LPSCROLLINFO lpsi = (LPSCROLLINFO)lParam;

	if (lpsi && (lpsi->fMask & SIF_TRACKPOS) && m_pSBTrack)
	{
		lpsi->nTrackPos = m_pSBTrack->posNew;
	}

	return bResult;
}


void CXTPScrollBar::OnSetFocus(CWnd* /*pOldWnd*/)
{

}


void CXTPScrollBar::RefreshMetrics()
{
	RefreshXtremeColors();
	m_cxHScroll = GetSystemMetrics(SM_CXHSCROLL);
	m_cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
	m_cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	m_cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

	m_themeScrollBar->CloseThemeData();

	if (m_bUseVisualStyle)
	{
		m_themeScrollBar->OpenTheme(0, L"SCROLLBAR");
	}

	COLORREF clrWindow = GetXtremeColor(COLOR_WINDOW);
	COLORREF clrFace = GetXtremeColor(COLOR_3DFACE);

	m_crBackHilite = m_crBackPushed = m_crBack = clrFace;
	m_crBorderHilite = GetXtremeColor(COLOR_3DSHADOW);
	m_crBorder = GetXtremeColor(COLOR_3DSHADOW);

	if (m_nTheme == xtpControlThemeUltraFlat)
	{

		COLORREF clrPushed = GetXtremeColor(COLOR_3DSHADOW);
		clrPushed = XTPColorManager()->LightColor(clrPushed,
			clrFace, 0x32);
		m_crBackPushed = clrPushed;

		COLORREF clrHighlight = clrFace;
		clrHighlight = XTPColorManager()->LightColor(clrHighlight, clrWindow, 0x1E);
		m_crBackHilite = clrHighlight;
	}
	else if ((m_nTheme == xtpControlThemeOfficeXP) || (m_nTheme == xtpControlThemeOffice2003))
	{
		m_crBackPushed = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
		m_crBackHilite = GetXtremeColor(XPCOLOR_HIGHLIGHT);
		m_crBorderHilite = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
	}
	else if (m_nTheme == xtpControlThemeFlat)
	{
		m_crBorder = m_crBorderHilite = GetXtremeColor(COLOR_WINDOWFRAME);
	}
}

void DrawArrowGlyph(CDC* pDC, CRect rcArrow, BOOL bHorz, BOOL bUpArrow, BOOL bEnabled)
{
	int nHeight = min(rcArrow.Width(), rcArrow.Height());
	if (nHeight < 6) return;

	int x = rcArrow.left + ((rcArrow.Width() - nHeight) / 2) + 2;
	int y = rcArrow.top + ((rcArrow.Height() - nHeight) / 2) + 2;
	nHeight -= 4;

	COLORREF clr = bEnabled ? RGB(0, 0, 0) : GetSysColor(COLOR_3DSHADOW);

	HFONT hFont = CreateFont(nHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("MARLETT"));
	HFONT hOldFont = (HFONT)SelectObject(pDC->GetSafeHdc(), hFont);

	SetTextColor(pDC->GetSafeHdc(), clr);
	pDC->SetBkMode(TRANSPARENT);
	TextOut(pDC->GetSafeHdc(), x, y, bHorz ? (bUpArrow ? _T("3") : _T("4")) : (bUpArrow ? _T("5") : _T("6")), 1);

	SelectObject(pDC->GetSafeHdc(), hOldFont);
	DeleteObject(hFont);
}

void CXTPScrollBar::DrawScrollBar(CDC* pDC)
{
	#define GETPARTSTATE(ht, pressed, hot, normal, disabled) \
		(!bEnabled ? disabled : nPressetHt == ht ? pressed : nHotHt == ht ? hot : normal)

	CXTPScrollBar::SCROLLBARTRACKINFO* pSBTrack = GetScrollBarTrackInfo();
	CXTPScrollBar::SCROLLBARPOSINFO* pSBInfo = GetScrollBarPosInfo();

	BOOL nPressetHt = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1) : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;


	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
	{
		return;
	}

	BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0) && IsScrollBarEnabled();

	int nBtnTrackSize =   pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	if (m_nTheme == xtpControlThemeResource && !(m_bUseVisualStyle  && m_themeScrollBar->IsAppThemed()))
	{
		DrawScrollBar2007(pDC);
		return;
	}


	if (pSBInfo->fVert)
	{
		CRect rcVScroll(pSBInfo->rc);

		CRect rcArrowUp(rcVScroll.left, rcVScroll.top, rcVScroll.right, pSBInfo->pxUpArrow);
		CRect rcArrowDown(rcVScroll.left, pSBInfo->pxDownArrow, rcVScroll.right, rcVScroll.bottom);
		CRect rcTrack(rcVScroll.left, rcArrowUp.bottom, rcVScroll.right, rcArrowDown.top);

		CRect rcLowerTrack(rcTrack.left, rcTrack.top, rcTrack.right, rcTrack.top + nBtnTrackPos);
		CRect rcBtnTrack(rcTrack.left, rcLowerTrack.bottom, rcTrack.right, rcLowerTrack.bottom + nBtnTrackSize);
		CRect rcUpperTrack(rcTrack.left, rcBtnTrack.bottom, rcTrack.right, rcTrack.bottom);


		if (m_bUseVisualStyle  && m_themeScrollBar->IsAppThemed())
		{
			m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLUP, ABS_UPPRESSED, ABS_UPHOT, ABS_UPNORMAL, ABS_UPDISABLED),  rcArrowUp, NULL);
			m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLDOWN, ABS_DOWNPRESSED, ABS_DOWNHOT, ABS_DOWNNORMAL, ABS_DOWNDISABLED),  rcArrowDown, NULL);

			if (!rcTrack.IsRectEmpty())
			{
				if (!rcLowerTrack.IsRectEmpty())
					m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_LOWERTRACKVERT, GETPARTSTATE(XTP_HTSCROLLUPPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcLowerTrack, NULL);

				if (!rcBtnTrack.IsRectEmpty())
				{
					m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_THUMBBTNVERT, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcBtnTrack, NULL);
					if (rcBtnTrack.Height() > 13)
						m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_GRIPPERVERT, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcBtnTrack, NULL);
				}

				if (!rcUpperTrack.IsRectEmpty())
					m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_UPPERTRACKVERT, GETPARTSTATE(XTP_HTSCROLLDOWNPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcUpperTrack, NULL);
			}
		}
		else
		{
			if (m_nTheme != xtpControlThemeDefault)
			{
				pDC->FillSolidRect(rcArrowUp, nPressetHt == XTP_HTSCROLLUP ? m_crBackPushed : nHotHt == XTP_HTSCROLLUP ? m_crBackHilite : m_crBack);

				if (m_nTheme != xtpControlThemeOffice2000)
				{
					COLORREF clr = nPressetHt == XTP_HTSCROLLUP || nHotHt == XTP_HTSCROLLUP ? m_crBorderHilite : m_crBorder;
					pDC->Draw3dRect(rcArrowUp, clr, clr);
				}
				else
				{
					if (bEnabled && nPressetHt == XTP_HTSCROLLUP)
						pDC->Draw3dRect(rcArrowUp,GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
					else
						pDC->Draw3dRect(rcArrowUp, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				}
				DrawArrowGlyph(pDC, rcArrowUp, FALSE, TRUE, bEnabled);

				pDC->FillSolidRect(rcArrowDown, nPressetHt == XTP_HTSCROLLDOWN ? m_crBackPushed : nHotHt == XTP_HTSCROLLDOWN ? m_crBackHilite : m_crBack);

				if (m_nTheme != xtpControlThemeOffice2000)
				{
					COLORREF clr = nPressetHt == XTP_HTSCROLLDOWN || nHotHt == XTP_HTSCROLLDOWN ? m_crBorderHilite : m_crBorder;
					pDC->Draw3dRect(rcArrowDown, clr, clr);
				}
				else
				{
					if (bEnabled && nPressetHt == XTP_HTSCROLLDOWN)
						pDC->Draw3dRect(rcArrowDown,GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
					else
						pDC->Draw3dRect(rcArrowDown, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				}
				DrawArrowGlyph(pDC, rcArrowDown, FALSE, FALSE, bEnabled);

			}
			else
			{
				DrawFrameControl(pDC->GetSafeHdc(), &rcArrowUp, DFC_SCROLL, DFCS_SCROLLUP | (!bEnabled ? DFCS_INACTIVE : 0) | (nPressetHt == XTP_HTSCROLLUP ? DFCS_PUSHED : 0));
				DrawFrameControl(pDC->GetSafeHdc(), &rcArrowDown, DFC_SCROLL, DFCS_SCROLLDOWN | (!bEnabled ? DFCS_INACTIVE : 0)  | (nPressetHt == XTP_HTSCROLLDOWN ? DFCS_PUSHED : 0));
			}

			CWnd* pParent = GetParentWindow();
			HBRUSH hbrRet = (HBRUSH)::DefWindowProc(pParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pParent->GetSafeHwnd());

			::FillRect(pDC->GetSafeHdc(), &rcTrack, hbrRet);

			if (nPressetHt == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &rcLowerTrack);
			}

			if (!rcTrack.IsRectEmpty() && !rcBtnTrack.IsRectEmpty())
			{
				pDC->FillSolidRect(rcBtnTrack, nPressetHt == XTP_HTSCROLLTHUMB ? m_crBackPushed : nHotHt == XTP_HTSCROLLTHUMB ? m_crBackHilite : m_crBack);

				if (m_nTheme == xtpControlThemeDefault)
				{
					DrawEdge(pDC->GetSafeHdc(), &rcBtnTrack, EDGE_RAISED, (UINT)(BF_ADJUST | BF_RECT));
				}
				else if (m_nTheme != xtpControlThemeOffice2000)
				{
					COLORREF clr = nPressetHt == XTP_HTSCROLLTHUMB || nHotHt == XTP_HTSCROLLTHUMB ? m_crBorderHilite : m_crBorder;
					pDC->Draw3dRect(rcBtnTrack, clr, clr);
				}
				else
				{
					pDC->Draw3dRect(rcBtnTrack, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				}
			}

			if (nPressetHt == XTP_HTSCROLLDOWNPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &rcUpperTrack);
			}
		}
	}
	else
	{
		CRect rcHScroll(pSBInfo->rc);

		CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
		CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);
		CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);

		CRect rcLowerTrack(rcTrack.left, rcTrack.top, rcTrack.left + nBtnTrackPos, rcTrack.bottom);
		CRect rcBtnTrack(rcLowerTrack.right, rcTrack.top, rcLowerTrack.right + nBtnTrackSize, rcTrack.bottom);
		CRect rcUpperTrack(rcBtnTrack.right, rcTrack.top, rcTrack.right, rcTrack.bottom);


		if (m_bUseVisualStyle  && m_themeScrollBar->IsAppThemed())
		{
			m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLUP, ABS_LEFTPRESSED, ABS_LEFTHOT, ABS_LEFTNORMAL, ABS_LEFTDISABLED),  rcArrowLeft, NULL);
			m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLDOWN, ABS_RIGHTPRESSED, ABS_RIGHTHOT, ABS_RIGHTNORMAL, ABS_RIGHTDISABLED),  rcArrowRight, NULL);

			if (!rcTrack.IsRectEmpty())
			{
				if (!rcLowerTrack.IsRectEmpty())
					m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_LOWERTRACKHORZ, GETPARTSTATE(XTP_HTSCROLLUPPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcLowerTrack, NULL);

				if (!rcBtnTrack.IsRectEmpty())
				{
					m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_THUMBBTNHORZ, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcBtnTrack, NULL);
					if (rcBtnTrack.Width() > 13)
						m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_GRIPPERHORZ, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcBtnTrack, NULL);
				}

				if (!rcUpperTrack.IsRectEmpty())
					m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_UPPERTRACKHORZ, GETPARTSTATE(XTP_HTSCROLLDOWNPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcUpperTrack, NULL);
			}
		}
		else
		{
			if (m_nTheme != xtpControlThemeDefault)
			{
				pDC->FillSolidRect(rcArrowLeft, nPressetHt == XTP_HTSCROLLUP ? m_crBackPushed : nHotHt == XTP_HTSCROLLUP ? m_crBackHilite : m_crBack);

				if (m_nTheme != xtpControlThemeOffice2000)
				{
					COLORREF clr = nPressetHt == XTP_HTSCROLLUP || nHotHt == XTP_HTSCROLLUP ? m_crBorderHilite : m_crBorder;
					pDC->Draw3dRect(rcArrowLeft, clr, clr);
				}
				else
				{
					if (bEnabled && nPressetHt == XTP_HTSCROLLUP)
						pDC->Draw3dRect(rcArrowLeft,GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
					else
						pDC->Draw3dRect(rcArrowLeft, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				}
				DrawArrowGlyph(pDC, rcArrowLeft, TRUE, TRUE, bEnabled);

				pDC->FillSolidRect(rcArrowRight, nPressetHt == XTP_HTSCROLLDOWN ? m_crBackPushed : nHotHt == XTP_HTSCROLLDOWN ? m_crBackHilite : m_crBack);

				if (m_nTheme != xtpControlThemeOffice2000)
				{
					COLORREF clr = nPressetHt == XTP_HTSCROLLDOWN || nHotHt == XTP_HTSCROLLDOWN ? m_crBorderHilite : m_crBorder;
					pDC->Draw3dRect(rcArrowRight, clr, clr);
				}
				else
				{
					if (bEnabled && nPressetHt == XTP_HTSCROLLDOWN)
						pDC->Draw3dRect(rcArrowRight,GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
					else
						pDC->Draw3dRect(rcArrowRight, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				}
				DrawArrowGlyph(pDC, rcArrowRight, TRUE, FALSE, bEnabled);
			}
			else
			{
				DrawFrameControl(pDC->GetSafeHdc(), &rcArrowLeft, DFC_SCROLL, DFCS_SCROLLLEFT | (!bEnabled ? DFCS_INACTIVE : 0) | (nPressetHt == XTP_HTSCROLLUP ? DFCS_PUSHED : 0));
				DrawFrameControl(pDC->GetSafeHdc(), &rcArrowRight, DFC_SCROLL, DFCS_SCROLLRIGHT | (!bEnabled ? DFCS_INACTIVE : 0)  | (nPressetHt == XTP_HTSCROLLDOWN ? DFCS_PUSHED : 0));
			}

			CWnd* pParent = GetParentWindow();
			HBRUSH hbrRet = (HBRUSH)::DefWindowProc(pParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pParent->GetSafeHwnd());

			::FillRect(pDC->GetSafeHdc(), &rcTrack, hbrRet);

			if (nPressetHt == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &rcLowerTrack);
			}

			if (!rcTrack.IsRectEmpty() && !rcBtnTrack.IsRectEmpty())
			{
				pDC->FillSolidRect(rcBtnTrack, nPressetHt == XTP_HTSCROLLTHUMB ? m_crBackPushed : nHotHt == XTP_HTSCROLLTHUMB ? m_crBackHilite : m_crBack);

				if (m_nTheme == xtpControlThemeDefault)
				{
					DrawEdge(pDC->GetSafeHdc(), &rcBtnTrack, EDGE_RAISED, (UINT)(BF_ADJUST | BF_RECT));
				}
				else if (m_nTheme != xtpControlThemeOffice2000)
				{
					COLORREF clr = nPressetHt == XTP_HTSCROLLTHUMB || nHotHt == XTP_HTSCROLLTHUMB ? m_crBorderHilite : m_crBorder;
					pDC->Draw3dRect(rcBtnTrack, clr, clr);
				}
				else
				{
					pDC->Draw3dRect(rcBtnTrack, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				}
			}

			if (nPressetHt == XTP_HTSCROLLDOWNPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &rcUpperTrack);
			}
		}
	}
}

AFX_INLINE CRect OffsetSourceRect(CRect rc, int nState)
{
	rc.OffsetRect(0, (nState - 1) * rc.Height());
	return rc;
}

void CXTPScrollBar::DrawScrollBar2007(CDC* pDC)
{
	BOOL bLight = TRUE;

	#define GETPARTSTATE2(ht) \
	(!bEnabled ? 0 : nPressetHt == ht ? 3 : nHotHt == ht ? 2 : nHotHt > 0 || nPressetHt > 0 ? 1 : 0)

	CXTPScrollBar::SCROLLBARTRACKINFO* pSBTrack = GetScrollBarTrackInfo();
	CXTPScrollBar::SCROLLBARPOSINFO* pSBInfo = GetScrollBarPosInfo();

	BOOL nPressetHt = pSBTrack ? pSBInfo->ht : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;

	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
	{
		return;
	}

	BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0) && IsScrollBarEnabled();

	int nBtnTrackSize =   pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	CXTPResourceImages* pImages = XTPResourceImages();


	CXTPResourceImage* pImageArrowGlyphs = pImages->LoadFile(_T("CONTROLGALLERYSCROLLARROWGLYPHS"));
	ASSERT(pImageArrowGlyphs);
	if (!pImageArrowGlyphs)
		return;
	CRect rcArrowGripperSrc(0, 0, 9, 9);

	if (pSBInfo->fVert)
	{
		CXTPResourceImage* pImageBackground = pImages->LoadFile(
			bLight ? _T("CONTROLGALLERYSCROLLVERTICALLIGHT") : _T("CONTROLGALLERYSCROLLVERTICALDARK"));
		ASSERT(pImageBackground);
		if (!pImageBackground)
			return;

		pImageBackground->DrawImage(pDC, pSBInfo->rc, pImageBackground->GetSource(0, 2), CRect(1, 0, 1, 0));


		CRect rcVScroll(pSBInfo->rc);
		rcVScroll.DeflateRect(1, 0);

		CRect rcArrowUp(rcVScroll.left, rcVScroll.top, rcVScroll.right, pSBInfo->pxUpArrow);
		CRect rcArrowDown(rcVScroll.left, pSBInfo->pxDownArrow, rcVScroll.right, rcVScroll.bottom);

		CXTPResourceImage* pImage =  pImages->LoadFile(
			bLight ? _T("CONTROLGALLERYSCROLLARROWSVERTICALLIGHT") : _T("CONTROLGALLERYSCROLLARROWSVERTICALDARK"));
		ASSERT(pImage);
		if (!pImage)
			return;

		int nState = GETPARTSTATE2(XTP_HTSCROLLUP);
		if (nState != 0)
		{
			pImage->DrawImage(pDC, rcArrowUp, pImage->GetSource(nState, 4), CRect(3, 3, 3, 3));
		}

		CRect rcArrowUpGripper(CPoint((rcArrowUp.left + rcArrowUp.right - 9) / 2, (rcArrowUp.top + rcArrowUp.bottom - 9) / 2), CSize(9, 9));
		pImageArrowGlyphs->DrawImage(pDC, rcArrowUpGripper, OffsetSourceRect(rcArrowGripperSrc, !bEnabled ? ABS_UPDISABLED : nState != 0 ? ABS_UPHOT : ABS_UPNORMAL), CRect(0, 0, 0, 0), RGB(255, 0, 255));

		nState = GETPARTSTATE2(XTP_HTSCROLLDOWN);
		if (nState != 0)
		{
			pImage->DrawImage(pDC, rcArrowDown, pImage->GetSource(nState, 4), CRect(3, 3, 3, 3));
		}

		CRect rcArrowDownGripper(CPoint((rcArrowDown.left + rcArrowDown.right - 9) / 2, (rcArrowDown.top + rcArrowDown.bottom - 9) / 2), CSize(9, 9));
		pImageArrowGlyphs->DrawImage(pDC, rcArrowDownGripper, OffsetSourceRect(rcArrowGripperSrc, !bEnabled ? ABS_DOWNDISABLED : nState != 0 ? ABS_DOWNHOT : ABS_DOWNNORMAL), CRect(0, 0, 0, 0), RGB(255, 0, 255));


		CRect rcTrack(rcVScroll.left, rcArrowUp.bottom, rcVScroll.right, rcArrowDown.top);

		if (!rcTrack.IsRectEmpty())
		{
			CRect rcLowerTrack(rcTrack.left - 1, rcTrack.top, rcTrack.right + 1, rcTrack.top + nBtnTrackPos);
			CRect rcBtnTrack(rcTrack.left, rcLowerTrack.bottom, rcTrack.right, rcLowerTrack.bottom + nBtnTrackSize);
			CRect rcUpperTrack(rcTrack.left - 1, rcBtnTrack.bottom, rcTrack.right + 1, rcTrack.bottom);

			if (!rcLowerTrack.IsRectEmpty() && (GETPARTSTATE2(XTP_HTSCROLLUPPAGE) == 3))
			{
				pImageBackground->DrawImage(pDC, rcLowerTrack,
					pImageBackground->GetSource(1, 2), CRect(1, 0, 1, 0));
			}

			if (!rcBtnTrack.IsRectEmpty())
			{
				nState = GETPARTSTATE2(XTP_HTSCROLLTHUMB);
				if (nState > 0) nState--;

				pImage =  pImages->LoadFile(_T("CONTROLGALLERYSCROLLTHUMBVERTICAL"));
				ASSERT(pImage);
				if (!pImage)
					return;

				pImage->DrawImage(pDC, rcBtnTrack, pImage->GetSource(nState, 3), CRect(5, 5, 5, 5));


				if (rcBtnTrack.Height() > 10)
				{
					pImage =  pImages->LoadFile(_T("CONTROLGALLERYSCROLLTHUMBGRIPPERVERTICAL"));
					CRect rcGripper(CPoint(rcBtnTrack.CenterPoint().x - 3, rcBtnTrack.CenterPoint().y - 4), CSize(8, 8));

					pImage->DrawImage(pDC, rcGripper, pImage->GetSource(nState, 3), CRect(0, 0, 0, 0));
				}
			}

			if (!rcUpperTrack.IsRectEmpty() && (GETPARTSTATE2(XTP_HTSCROLLDOWNPAGE) == 3))
			{
				pImageBackground->DrawImage(pDC, rcUpperTrack,
					pImageBackground->GetSource(1, 2), CRect(1, 0, 1, 0));
			}
		}
	}
	else
	{
		CXTPResourceImage* pImageBackground = pImages->LoadFile(
			bLight ? _T("CONTROLGALLERYSCROLLHORIZONTALLIGHT") : _T("CONTROLGALLERYSCROLLHORIZONTALDARK"));
		if (!pImageBackground)
		{
			return;
		}

		pImageBackground->DrawImage(pDC, pSBInfo->rc, pImageBackground->GetSource(0, 2), CRect(0, 1, 0, 1));

		CRect rcHScroll(pSBInfo->rc);
		rcHScroll.DeflateRect(0, 1);

		CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
		CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);

		CXTPResourceImage* pImage =  pImages->LoadFile(
			bLight ? _T("CONTROLGALLERYSCROLLARROWSHORIZONTALLIGHT") : _T("CONTROLGALLERYSCROLLARROWSHORIZONTALDARK"));
		ASSERT(pImage);
		if (!pImage)
			return;

		int nState = GETPARTSTATE2(XTP_HTSCROLLUP);
		if (nState != 0)
		{
			pImage->DrawImage(pDC, rcArrowLeft, pImage->GetSource(nState, 4), CRect(3, 3, 3, 3));
		}

		CRect rcArrowLeftGripper(CPoint((rcArrowLeft.left + rcArrowLeft.right - 9) / 2, (rcArrowLeft.top + rcArrowLeft.bottom - 9) / 2), CSize(9, 9));
		pImageArrowGlyphs->DrawImage(pDC, rcArrowLeftGripper, OffsetSourceRect(rcArrowGripperSrc, !bEnabled ? ABS_LEFTDISABLED : nState != 0 ? ABS_LEFTHOT : ABS_LEFTNORMAL), CRect(0, 0, 0, 0), RGB(255, 0, 255));

		nState = GETPARTSTATE2(XTP_HTSCROLLDOWN);
		if (nState != 0)
		{
			pImage->DrawImage(pDC, rcArrowRight, pImage->GetSource(nState, 4), CRect(3, 3, 3, 3));
		}

		CRect rcArrowRightGripper(CPoint((rcArrowRight.left + rcArrowRight.right - 9) / 2, (rcArrowRight.top + rcArrowRight.bottom - 9) / 2), CSize(9, 9));
		pImageArrowGlyphs->DrawImage(pDC, rcArrowRightGripper, OffsetSourceRect(rcArrowGripperSrc, !bEnabled ? ABS_RIGHTDISABLED : nState != 0 ? ABS_RIGHTHOT : ABS_RIGHTNORMAL), CRect(0, 0, 0, 0), RGB(255, 0, 255));



		CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);

		if (!rcTrack.IsRectEmpty())
		{
			CRect rcLowerTrack(rcTrack.left, rcTrack.top - 1, rcTrack.left + nBtnTrackPos, rcTrack.bottom + 1);
			CRect rcBtnTrack(rcLowerTrack.right, rcTrack.top, rcLowerTrack.right + nBtnTrackSize, rcTrack.bottom);
			CRect rcUpperTrack(rcBtnTrack.right, rcTrack.top - 1, rcTrack.right, rcTrack.bottom + 1);

			if (!rcLowerTrack.IsRectEmpty() && (GETPARTSTATE2(XTP_HTSCROLLUPPAGE) == 3))
			{
				pImageBackground->DrawImage(pDC, rcLowerTrack,
					pImageBackground->GetSource(1, 2), CRect(0, 1, 0, 1));
			}

			if (!rcBtnTrack.IsRectEmpty())
			{
				nState = GETPARTSTATE2(XTP_HTSCROLLTHUMB);
				if (nState > 0) nState--;

				pImage =  pImages->LoadFile(_T("CONTROLGALLERYSCROLLTHUMBHORIZONTAL"));
				ASSERT(pImage);
				if (!pImage)
					return;

				pImage->DrawImage(pDC, rcBtnTrack, pImage->GetSource(nState, 3), CRect(5, 5, 5, 5));


				if (rcBtnTrack.Width() > 10)
				{
					pImage =  pImages->LoadFile(_T("CONTROLGALLERYSCROLLTHUMBGRIPPERHORIZONTAL"));
					CRect rcGripper(CPoint(rcBtnTrack.CenterPoint().x - 3, rcBtnTrack.CenterPoint().y - 4), CSize(8, 8));

					pImage->DrawImage(pDC, rcGripper, pImage->GetSource(nState, 3), CRect(0, 0, 0, 0));
				}
			}

			if (!rcUpperTrack.IsRectEmpty() && (GETPARTSTATE2(XTP_HTSCROLLDOWNPAGE) == 3))
			{
				pImageBackground->DrawImage(pDC, rcUpperTrack,
					pImageBackground->GetSource(1, 2), CRect(0, 1, 0, 1));
			}
		}
	}
}

void CXTPScrollBar::SetScrollBarStyle(int nTheme)
{
	SetTheme((XTPControlTheme)nTheme);
}

void CXTPScrollBar::SetCommandBars(void*)
{
	TRACE(_T("WARNING: CXTPScrollBar renamed to CXTPCommandBarScrollBarCtrl\n"));
}
