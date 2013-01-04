// XTPMarkupButton.cpp: implementation of the CXTPMarkupButton class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "Common/XTPWinThemeWrapper.h"

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupControl.h"
#include "XTPMarkupContext.h"
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupScrollViewer.h"
#include "XTPMarkupBuilder.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define XTP_HTSCROLLUP          60          // <combine CXTPScrollBase::HitTestScrollBar@POINT>
#define XTP_HTSCROLLDOWN        61          // <combine CXTPScrollBase::HitTestScrollBar@POINT>
#define XTP_HTSCROLLUPPAGE      62          // <combine CXTPScrollBase::HitTestScrollBar@POINT>
#define XTP_HTSCROLLDOWNPAGE    63          // <combine CXTPScrollBase::HitTestScrollBar@POINT>
#define XTP_HTSCROLLTHUMB       64          // <combine CXTPScrollBase::HitTestScrollBar@POINT>
#define XTP_HTSCROLLPOPUP       65          // <combine CXTPScrollBase::HitTestScrollBar@POINT>

#define IDSYS_SCROLL 63345


IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupScrollBar, CXTPMarkupControl)

void CXTPMarkupScrollBar::RegisterMarkupClass()
{

}

CXTPMarkupScrollBar::CXTPMarkupScrollBar()
{
	m_orientation = xtpMarkupOrientationVertical;

	m_pSBTrack = NULL;
	ZeroMemory(&m_spi, sizeof(m_spi));
	ZeroMemory(&m_si, sizeof(m_si));
}

void CXTPMarkupScrollBar::SetScrollInfo(SCROLLINFO* pScrollInfo)
{
	if (m_si.nMax != pScrollInfo->nMax || m_si.nPage != pScrollInfo->nPage || m_si.nPos != pScrollInfo->nPos)
	{
		m_si = *pScrollInfo;
		m_bArrangeDirty = TRUE;
	}
}

void CXTPMarkupScrollBar::GetScrollInfo(SCROLLINFO* pScrollInfo)
{
	*pScrollInfo = m_si;
}



int CXTPMarkupScrollBar::HitTestScrollBar(POINT pt) const
{
	if (!IsEnabled())
		return HTNOWHERE;

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

void CXTPMarkupScrollBar::EndScroll(BOOL fCancel)
{
	SCROLLBARTRACKINFO* pSBTrack = m_pSBTrack;

	if (pSBTrack)
	{
		pSBTrack->cmdSB = 0;
		GetMarkupContext()->ReleaseMouseCapture(this);

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

int CXTPMarkupScrollBar::SBPosFromPx(CXTPMarkupScrollBar::SCROLLBARPOSINFO*  pSBInfo, int px)
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


void CXTPMarkupScrollBar::MoveThumb(int px)
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

void CXTPMarkupScrollBar::TrackThumb(UINT message, CPoint pt)
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

void CXTPMarkupScrollBar::TrackBox(UINT message, CPoint point)
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

void CXTPMarkupScrollBar::ContScroll()
{
	SCROLLBARTRACKINFO* pSBTrack = m_pSBTrack;
	ASSERT(pSBTrack);

	if (pSBTrack == NULL)
		return;

	CPoint pt;
	GetCursorPos(&pt);

	ScreenToClient(m_pSBTrack->hWndTrack, &pt);

	TrackBox(WM_NULL, pt);

	if (pSBTrack->fHitOld)
	{
		pSBTrack->hTimerSB = ::SetTimer(m_pSBTrack->hWndTrack, IDSYS_SCROLL, GetDoubleClickTime() / 10, NULL);

		DoScroll(pSBTrack->cmdSB, 0);
	}
}

void CXTPMarkupScrollBar::CalcTrackDragRect(SCROLLBARTRACKINFO* pSBTrack) const
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

void CXTPMarkupScrollBar::DoScroll(int cmd, int pos)
{
	CXTPMarkupScrollViewer* pViewer = MARKUP_DYNAMICCAST(CXTPMarkupScrollViewer, GetLogicalParent());
	if (pViewer)
	{
		pViewer->Scroll(m_spi.fVert ? SB_VERT : SB_HORZ, cmd, pos);
	}
}

void CXTPMarkupScrollBar::ScreenToClient(HWND hWnd,  LPPOINT lpPoint)
{
	::ScreenToClient(hWnd, lpPoint);

	CXTPMarkupMouseEventArgs e(0);
	e.m_hWnd = hWnd;
	e.m_point = *lpPoint;

	*lpPoint = e.GetPosition(this);
}

void CXTPMarkupScrollBar::PerformTrackInit(HWND hWnd, CPoint point, SCROLLBARPOSINFO* pSBInfo, BOOL bDirect)
{
	CXTPMarkupContext* pMarkupContext = GetMarkupContext();

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

		pMarkupContext->CaptureMouse(this);

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

	pMarkupContext->CaptureMouse(this);

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

	while (pMarkupContext->GetMouseCapture() == this)
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

				ScreenToClient(hWnd, &ptScreen);

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
		ScreenToClient(hWnd, &point);
		m_spi.ht = HitTestScrollBar(point);

		RedrawScrollBar();
	}
}


void CXTPMarkupScrollBar::OnMouseLeave(CXTPMarkupMouseEventArgs* e)
{
	if (m_spi.ht != HTNOWHERE)
	{
		m_spi.ht = HTNOWHERE;
		RedrawScrollBar();
	}

	CXTPMarkupControl::OnMouseLeave(e);
}

void CXTPMarkupScrollBar::OnMouseEnter(CXTPMarkupMouseEventArgs* e)
{
	CXTPMarkupControl::OnMouseEnter(e);
}

void CXTPMarkupScrollBar::OnMouseMove(CXTPMarkupMouseEventArgs* e)
{
	int ht = HitTestScrollBar(e->GetPosition(this));

	if (ht != m_spi.ht)
	{
		m_spi.ht = ht;
		RedrawScrollBar();
	}

	e->SetHandled();
}

void CXTPMarkupScrollBar::OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* e)
{
	CXTPMarkupControl::OnMouseLeftButtonUp(e);
}

void CXTPMarkupScrollBar::OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* e)
{
	e->SetHandled();

	if (IsEnabled())
	{
		PerformTrackInit(e->m_hWnd, e->GetPosition(this), &m_spi, (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE);
	}
}


void CXTPMarkupScrollBar::RedrawScrollBar()
{
	InvalidateVisual();
}


void CXTPMarkupScrollBar::CalcScrollBarInfo(LPRECT lprc, SCROLLBARPOSINFO* pSBInfo, SCROLLINFO* pSI)
{
	int cpx;
	DWORD dwRange;
	int denom;


	BOOL fVert = pSBInfo->fVert = (m_orientation == xtpMarkupOrientationVertical);

	pSBInfo->rc = *lprc;
	pSBInfo->pxPopup = 0;

	if (fVert)
	{
		pSBInfo->pxTop = lprc->top;
		pSBInfo->pxBottom = lprc->bottom;
		pSBInfo->pxLeft = lprc->left;
		pSBInfo->pxRight = lprc->right;
		pSBInfo->cpxThumb = GetSystemMetrics(SM_CYVSCROLL);
	}
	else
	{
		pSBInfo->pxTop = lprc->left;
		pSBInfo->pxBottom = lprc->right;
		pSBInfo->pxLeft = lprc->top;
		pSBInfo->pxRight = lprc->bottom;
		pSBInfo->cpxThumb = GetSystemMetrics(SM_CXHSCROLL);
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

		pSBInfo->cpxThumb = max(max(16, pSBInfo->cpxThumb / 2), i);
	}

	pSBInfo->pxMin = pSBInfo->pxTop + cpx;
	pSBInfo->cpx = pSBInfo->pxBottom - cpx - pSBInfo->cpxThumb - pSBInfo->pxMin;

	denom = dwRange - (pSBInfo->page ? pSBInfo->page : 1);
	if (denom)
		pSBInfo->pxThumbTop = MulDiv(pSBInfo->pos - pSBInfo->posMin,
		pSBInfo->cpx, denom) + pSBInfo->pxMin;
	else
		pSBInfo->pxThumbTop = pSBInfo->pxMin;

	pSBInfo->pxThumbBottom = pSBInfo->pxThumbTop + pSBInfo->cpxThumb;
}

CSize CXTPMarkupScrollBar::ArrangeOverride(CSize szFinalSize)
{
	CalcScrollBarInfo(CRect(0, 0, szFinalSize.cx, szFinalSize.cy), &m_spi, &m_si);

	return szFinalSize;
}

CSize CXTPMarkupScrollBar::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize /*szAvailableSize*/)
{
	BOOL bHorizontal = (m_orientation == xtpMarkupOrientationHorizontal);

	if (bHorizontal)
		return CSize(0, GetSystemMetrics(SM_CYHSCROLL));

	return CSize(GetSystemMetrics(SM_CXVSCROLL), 0);
}

BOOL CXTPMarkupScrollBar::IsEnabled() const
{
	return m_spi.posMax - m_spi.posMin - m_spi.page + 1 > 0;
}

void CXTPMarkupScrollBar::OnRender(CXTPMarkupDrawingContext* pDC)
{
	#define GETPARTSTATE(ht, pressed, hot, normal, disabled) \
		(!bEnabled ? disabled : nPressetHt == ht ? pressed : nHotHt == ht ? hot : normal)

	SCROLLBARTRACKINFO* pSBTrack = m_pSBTrack;
	SCROLLBARPOSINFO* pSBInfo = &m_spi;

	BOOL nPressetHt = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1) : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;


	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
	{
		return;
	}

	BOOL bEnabled = IsEnabled();

	int nBtnTrackSize =   pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	BOOL bUseVisualStyle = TRUE;

	CXTPWinThemeWrapper m_themeScrollBar;
	m_themeScrollBar.OpenTheme(0, L"SCROLLBAR");

	COLORREF clrFace = GetSysColor(COLOR_3DFACE);
	COLORREF m_crBackHilite = clrFace, m_crBackPushed = clrFace, m_crBack = clrFace;

	HDC hDC = pDC->GetDC();

	if (pSBInfo->fVert)
	{
		CRect rcVScroll(pSBInfo->rc);

		CRect rcArrowUp(rcVScroll.left, rcVScroll.top, rcVScroll.right, pSBInfo->pxUpArrow);
		CRect rcArrowDown(rcVScroll.left, pSBInfo->pxDownArrow, rcVScroll.right, rcVScroll.bottom);
		CRect rcTrack(rcVScroll.left, rcArrowUp.bottom, rcVScroll.right, rcArrowDown.top);

		CRect rcLowerTrack(rcTrack.left, rcTrack.top, rcTrack.right, rcTrack.top + nBtnTrackPos);
		CRect rcBtnTrack(rcTrack.left, rcLowerTrack.bottom, rcTrack.right, rcLowerTrack.bottom + nBtnTrackSize);
		CRect rcUpperTrack(rcTrack.left, rcBtnTrack.bottom, rcTrack.right, rcTrack.bottom);

		if (bUseVisualStyle  && m_themeScrollBar.IsAppThemed())
		{
			m_themeScrollBar.DrawThemeBackground(hDC, SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLUP, ABS_UPPRESSED, ABS_UPHOT, ABS_UPNORMAL, ABS_UPDISABLED),  rcArrowUp, NULL);
			m_themeScrollBar.DrawThemeBackground(hDC, SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLDOWN, ABS_DOWNPRESSED, ABS_DOWNHOT, ABS_DOWNNORMAL, ABS_DOWNDISABLED),  rcArrowDown, NULL);

			if (!rcTrack.IsRectEmpty())
			{
				if (!rcLowerTrack.IsRectEmpty())
					m_themeScrollBar.DrawThemeBackground(hDC, SBP_LOWERTRACKVERT, GETPARTSTATE(XTP_HTSCROLLUPPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcLowerTrack, NULL);

				if (!rcBtnTrack.IsRectEmpty())
				{
					m_themeScrollBar.DrawThemeBackground(hDC, SBP_THUMBBTNVERT, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcBtnTrack, NULL);
					if (rcBtnTrack.Height() > 13)
						m_themeScrollBar.DrawThemeBackground(hDC, SBP_GRIPPERVERT, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcBtnTrack, NULL);
				}

				if (!rcUpperTrack.IsRectEmpty())
					m_themeScrollBar.DrawThemeBackground(hDC, SBP_UPPERTRACKVERT, GETPARTSTATE(XTP_HTSCROLLDOWNPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcUpperTrack, NULL);
			}
		}
		else
		{
			{
				DrawFrameControl(hDC, &rcArrowUp, DFC_SCROLL, DFCS_SCROLLUP | (!bEnabled ? DFCS_INACTIVE : 0) | (nPressetHt == XTP_HTSCROLLUP ? DFCS_PUSHED : 0));
				DrawFrameControl(hDC, &rcArrowDown, DFC_SCROLL, DFCS_SCROLLDOWN | (!bEnabled ? DFCS_INACTIVE : 0)  | (nPressetHt == XTP_HTSCROLLDOWN ? DFCS_PUSHED : 0));
			}


			HBRUSH hbrRet = (HBRUSH)SendMessage(::GetDesktopWindow(), WM_CTLCOLORSCROLLBAR, (WPARAM)hDC, 0);
			::SetBkColor(hDC, GetSysColor(COLOR_3DHILIGHT));
			::SetTextColor(hDC, GetSysColor(COLOR_BTNFACE));
			::FillRect(hDC, &rcTrack, hbrRet);

			if (nPressetHt == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(hDC, &rcLowerTrack);
			}

			if (!rcTrack.IsRectEmpty() && !rcBtnTrack.IsRectEmpty())
			{
				::SetBkColor(hDC, nPressetHt == XTP_HTSCROLLTHUMB ? m_crBackPushed : nHotHt == XTP_HTSCROLLTHUMB ? m_crBackHilite : m_crBack);
				::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcBtnTrack, NULL, 0, NULL);

				::DrawEdge(hDC, &rcBtnTrack, EDGE_RAISED, (UINT)(BF_ADJUST | BF_RECT));

			}

			if (nPressetHt == XTP_HTSCROLLDOWNPAGE)
			{
				::InvertRect(hDC, &rcUpperTrack);
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


		if (bUseVisualStyle  && m_themeScrollBar.IsAppThemed())
		{
			m_themeScrollBar.DrawThemeBackground(hDC, SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLUP, ABS_LEFTPRESSED, ABS_LEFTHOT, ABS_LEFTNORMAL, ABS_LEFTDISABLED),  rcArrowLeft, NULL);
			m_themeScrollBar.DrawThemeBackground(hDC, SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLDOWN, ABS_RIGHTPRESSED, ABS_RIGHTHOT, ABS_RIGHTNORMAL, ABS_RIGHTDISABLED),  rcArrowRight, NULL);

			if (!rcTrack.IsRectEmpty())
			{
				if (!rcLowerTrack.IsRectEmpty())
					m_themeScrollBar.DrawThemeBackground(hDC, SBP_LOWERTRACKHORZ, GETPARTSTATE(XTP_HTSCROLLUPPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcLowerTrack, NULL);

				if (!rcBtnTrack.IsRectEmpty())
				{
					m_themeScrollBar.DrawThemeBackground(hDC, SBP_THUMBBTNHORZ, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcBtnTrack, NULL);
					if (rcBtnTrack.Width() > 13)
						m_themeScrollBar.DrawThemeBackground(hDC, SBP_GRIPPERHORZ, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcBtnTrack, NULL);
				}

				if (!rcUpperTrack.IsRectEmpty())
					m_themeScrollBar.DrawThemeBackground(hDC, SBP_UPPERTRACKHORZ, GETPARTSTATE(XTP_HTSCROLLDOWNPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  rcUpperTrack, NULL);
			}
		}
		else
		{
			{
				DrawFrameControl(hDC, &rcArrowLeft, DFC_SCROLL, DFCS_SCROLLLEFT | (!bEnabled ? DFCS_INACTIVE : 0) | (nPressetHt == XTP_HTSCROLLUP ? DFCS_PUSHED : 0));
				DrawFrameControl(hDC, &rcArrowRight, DFC_SCROLL, DFCS_SCROLLRIGHT | (!bEnabled ? DFCS_INACTIVE : 0)  | (nPressetHt == XTP_HTSCROLLDOWN ? DFCS_PUSHED : 0));
			}

			HBRUSH hbrRet = (HBRUSH)SendMessage(::GetDesktopWindow(), WM_CTLCOLORSCROLLBAR, (WPARAM)hDC, 0);
			::SetBkColor(hDC, GetSysColor(COLOR_3DHILIGHT));
			::SetTextColor(hDC, GetSysColor(COLOR_BTNFACE));
			::FillRect(hDC, &rcTrack, hbrRet);


			if (nPressetHt == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(hDC, &rcLowerTrack);
			}

			if (!rcTrack.IsRectEmpty() && !rcBtnTrack.IsRectEmpty())
			{
				::SetBkColor(hDC, nPressetHt == XTP_HTSCROLLTHUMB ? m_crBackPushed : nHotHt == XTP_HTSCROLLTHUMB ? m_crBackHilite : m_crBack);
				::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rcBtnTrack, NULL, 0, NULL);

				::DrawEdge(hDC, &rcBtnTrack, EDGE_RAISED, (UINT)(BF_ADJUST | BF_RECT));
			}

			if (nPressetHt == XTP_HTSCROLLDOWNPAGE)
			{
				::InvertRect(hDC, &rcUpperTrack);
			}
		}
	}

	pDC->ReleaseDC(hDC);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupScrollBarGripper

class CXTPMarkupScrollBarGripper : public CXTPMarkupUIElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupScrollBarGripper)

public:
	void OnRender(CXTPMarkupDrawingContext* pDC);
};

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupScrollBarGripper, CXTPMarkupUIElement)

void CXTPMarkupScrollBarGripper::RegisterMarkupClass()
{
}

void CXTPMarkupScrollBarGripper::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CRect rc(GetFinalRect());
	pDC->FillSolidRect(rc, GetSysColor(COLOR_BTNFACE));
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupScrollViewer

CXTPMarkupDependencyProperty* CXTPMarkupScrollViewer::m_pVerticalScrollBarVisibilityProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupScrollViewer::m_pHorizontalScrollBarVisibilityProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"ScrollViewer", CXTPMarkupScrollViewer, CXTPMarkupContentControl)

void CXTPMarkupScrollViewer::RegisterMarkupClass()
{
	m_pVerticalScrollBarVisibilityProperty = CXTPMarkupDependencyProperty::Register(L"VerticalScrollBarVisibility", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupScrollViewer),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertScrollBarVisibility, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pHorizontalScrollBarVisibilityProperty = CXTPMarkupDependencyProperty::Register(L"HorizontalScrollBarVisibility", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupScrollViewer),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertScrollBarVisibility, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

}

CXTPMarkupScrollViewer::CXTPMarkupScrollViewer()
{
	m_pVerticalScrollBar = NULL;
	m_pHorizontalScrollBar = NULL;
	m_pScrollBarGripper = NULL;

	m_xOffset = m_yOffset = 0;
	SetValue(m_pClipToBoundsProperty, CXTPMarkupBool::CreateTrueValue());
}

CXTPMarkupScrollViewer::~CXTPMarkupScrollViewer()
{
	MARKUP_RELEASE(m_pVerticalScrollBar);
	MARKUP_RELEASE(m_pHorizontalScrollBar);
	MARKUP_RELEASE(m_pScrollBarGripper);
}

int CXTPMarkupScrollViewer::GetVisualChildrenCount() const
{
	return (GetContent() != NULL ? 1 : 0) + (m_pVerticalScrollBar ? 1 : 0) + (m_pHorizontalScrollBar ? 1 : 0) + (m_pScrollBarGripper ? 1 : 0);
}

CXTPMarkupVisual* CXTPMarkupScrollViewer::GetVisualChild(int nIndex) const
{
	CXTPMarkupVisual* pContent = GetContent();
	if (pContent) if (nIndex == 0) return pContent; else nIndex--;
	if (m_pVerticalScrollBar) if (nIndex == 0) return m_pVerticalScrollBar; else nIndex--;
	if (m_pHorizontalScrollBar) if (nIndex == 0) return m_pHorizontalScrollBar; else nIndex--;
	return m_pScrollBarGripper;
}

CSize CXTPMarkupScrollViewer::ArrangeOverride(CSize szFinalSize)
{
	CXTPMarkupUIElement* pContent = GetContent();
	if (!pContent)
		return szFinalSize;

	CSize szViewport(szFinalSize);

	if (m_pVerticalScrollBar) szViewport.cx -= m_pVerticalScrollBar->GetDesiredSize().cx;
	if (m_pHorizontalScrollBar) szViewport.cy -= m_pHorizontalScrollBar->GetDesiredSize().cy;;

	if (m_pVerticalScrollBar)
	{
		if (m_yOffset > 0 && pContent->GetDesiredSize().cy < m_yOffset + szViewport.cy)
		{
			m_yOffset = max(0, pContent->GetDesiredSize().cy - szViewport.cy);
		}

		SCROLLINFO si;
		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.nMax = pContent->GetDesiredSize().cy - 1;
		si.nPage = szViewport.cy;
		si.nPos = m_yOffset;
		m_pVerticalScrollBar->SetScrollInfo(&si);

		m_pVerticalScrollBar->Arrange(CRect(szFinalSize.cx - m_pVerticalScrollBar->GetDesiredSize().cx, 0, szFinalSize.cx, szViewport.cy));
	}

	if (m_pHorizontalScrollBar)
	{
		if (m_xOffset > 0 && pContent->GetDesiredSize().cx < m_xOffset + szViewport.cx)
		{
			m_xOffset = max(0, pContent->GetDesiredSize().cx - szViewport.cx);
		}

		SCROLLINFO si;
		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.nMax = pContent->GetDesiredSize().cx - 1;
		si.nPage = szViewport.cx;
		si.nPos = m_xOffset;
		m_pHorizontalScrollBar->SetScrollInfo(&si);

		m_pHorizontalScrollBar->Arrange(CRect(0, szFinalSize.cy - m_pHorizontalScrollBar->GetDesiredSize().cy, szViewport.cx, szFinalSize.cy));
	}

	if (m_pScrollBarGripper && m_pHorizontalScrollBar && m_pVerticalScrollBar)
	{
		m_pScrollBarGripper->Arrange(CRect(szFinalSize.cx - m_pVerticalScrollBar->GetDesiredSize().cx, szFinalSize.cy - m_pHorizontalScrollBar->GetDesiredSize().cy, szFinalSize.cx, szFinalSize.cy));
	}

	CRect rcContent(0, 0, max(szViewport.cx, pContent->GetDesiredSize().cx), max(szViewport.cy, pContent->GetDesiredSize().cy));
	rcContent.OffsetRect(-m_xOffset, -m_yOffset);
	pContent->Arrange(rcContent);

	return szFinalSize;
}

void CXTPMarkupScrollViewer::CreateScrollBar(int nBar)
{
	if (nBar == SB_VERT && m_pVerticalScrollBar == NULL)
	{
		m_pVerticalScrollBar = (CXTPMarkupScrollBar*)m_pMarkupContext->CreateMarkupObject(MARKUP_TYPE(CXTPMarkupScrollBar));
		m_pVerticalScrollBar->SetOrientation(xtpMarkupOrientationVertical);
		m_pVerticalScrollBar->SetLogicalParent(this);
	}

	if (nBar == SB_HORZ && m_pHorizontalScrollBar == NULL)
	{
		m_pHorizontalScrollBar = (CXTPMarkupScrollBar*)m_pMarkupContext->CreateMarkupObject(MARKUP_TYPE(CXTPMarkupScrollBar));
		m_pHorizontalScrollBar->SetOrientation(xtpMarkupOrientationHorizontal);
		m_pHorizontalScrollBar->SetLogicalParent(this);
	}
}

CSize CXTPMarkupScrollViewer::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	CXTPMarkupUIElement* pContent = GetContent();
	if (!pContent)
		return CSize(0, 0);

	CSize szScroll(0, 0);

	XTPMarkupScrollBarVisibility verticalBarVisibility = GetVerticalScrollBarVisibility();
	XTPMarkupScrollBarVisibility horizontalBarVisibility = GetHorizontalScrollBarVisibility();

	CSize szViewportSize(INT_MAX, INT_MAX);

	if (horizontalBarVisibility == xtpMarkupScrollBarDisabled) szViewportSize.cx = szAvailableSize.cx;
	if (verticalBarVisibility == xtpMarkupScrollBarDisabled) szViewportSize.cy = szAvailableSize.cy;

	if (verticalBarVisibility == xtpMarkupScrollBarAuto)
	{
		pContent->Measure(pDC, szViewportSize);

		verticalBarVisibility = (pContent->GetDesiredSize().cy > szAvailableSize.cy) ? xtpMarkupScrollBarVisible : xtpMarkupScrollBarDisabled;
	}

	if (verticalBarVisibility == xtpMarkupScrollBarVisible)
	{
		CreateScrollBar(SB_VERT);
		m_pVerticalScrollBar->Measure(pDC, szAvailableSize);
		szScroll.cx = m_pVerticalScrollBar->GetDesiredSize().cx;

		if (szViewportSize.cx != INT_MAX) szViewportSize.cx = max(0, szViewportSize.cx - szScroll.cx);
	}
	else
	{
		MARKUP_RELEASE(m_pVerticalScrollBar);
		m_yOffset = 0;
	}

	if (horizontalBarVisibility == xtpMarkupScrollBarAuto)
	{
		pContent->Measure(pDC, szViewportSize);

		horizontalBarVisibility = (pContent->GetDesiredSize().cx > szAvailableSize.cx - szScroll.cx) ? xtpMarkupScrollBarVisible : xtpMarkupScrollBarDisabled;
	}

	if (horizontalBarVisibility == xtpMarkupScrollBarVisible)
	{
		CreateScrollBar(SB_HORZ);
		m_pHorizontalScrollBar->Measure(pDC, szAvailableSize);
		szScroll.cy = m_pHorizontalScrollBar->GetDesiredSize().cy;
		if (szViewportSize.cy != INT_MAX)  szViewportSize.cy = max(0, szViewportSize.cy - szScroll.cy);
	}
	else
	{
		MARKUP_RELEASE(m_pHorizontalScrollBar);
		m_xOffset = 0;
	}

	if (m_pVerticalScrollBar && m_pHorizontalScrollBar)
	{
		if (m_pScrollBarGripper == NULL)
		{
			m_pScrollBarGripper = (CXTPMarkupScrollBarGripper*)m_pMarkupContext->CreateMarkupObject(MARKUP_TYPE(CXTPMarkupScrollBarGripper));
			m_pScrollBarGripper->SetLogicalParent(this);
		}
	}
	else
	{
		MARKUP_RELEASE(m_pScrollBarGripper);
	}

	pContent->Measure(pDC, szViewportSize);
	return CSize(min(szAvailableSize.cx, pContent->GetDesiredSize().cx + szScroll.cx), min(szAvailableSize.cy, pContent->GetDesiredSize().cy + szScroll.cy));
}

void CXTPMarkupScrollViewer::Scroll(int nBar, int nSBCode, int pos)
{
	CXTPMarkupScrollBar* pScrollBar = nBar == SB_VERT ? m_pVerticalScrollBar : m_pHorizontalScrollBar;
	if (!pScrollBar)
		return;

	SCROLLINFO si;
	pScrollBar->GetScrollInfo(&si);

	int nCurPos = si.nPos;
	int nPage = (int)si.nPage;
	int nLimit = max(0, si.nMax - max(nPage - 1, 0));

	// decide what to do for each different scroll event
	switch (nSBCode)
	{
	case SB_TOP:
		nCurPos = 0;
		break;
	case SB_BOTTOM:
		nCurPos = nLimit;
		break;
	case SB_LINEUP:
		nCurPos = max(nCurPos - 16, 0);
		break;
	case SB_LINEDOWN:
		nCurPos = min(nCurPos + 16, nLimit);
		break;
	case SB_PAGEUP:
		nCurPos = max(nCurPos - nPage, 0);
		break;
	case SB_PAGEDOWN:
		nCurPos = min(nCurPos + nPage, nLimit);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:

		nCurPos = pos;
		break;
	}

	SetScrollPos(nBar, nCurPos);
}

int CXTPMarkupScrollViewer::GetScrollPos(int nBar)
{
	return (nBar == SB_VERT) ? m_yOffset : m_xOffset;
}

int CXTPMarkupScrollViewer::GetScrollLimit(int nBar)
{
	CXTPMarkupScrollBar* pScrollBar = nBar == SB_VERT ? m_pVerticalScrollBar : m_pHorizontalScrollBar;
	if (!pScrollBar)
		return 0;

	SCROLLINFO si;
	pScrollBar->GetScrollInfo(&si);

	int nPage = (int)si.nPage;
	int nLimit = max(0, si.nMax - max(nPage - 1, 0));

	return nLimit;
}

void CXTPMarkupScrollViewer::SetScrollPos(int nBar, int pos)
{
	int& nOffset = (nBar == SB_VERT) ? m_yOffset : m_xOffset;

	if (!m_bNeverArranged && !m_bArrangeDirty)
	{
		int nLimit = GetScrollLimit(nBar);
		if (pos > nLimit) pos = nLimit;
	}

	if (pos < 0) pos = 0;

	if (nOffset != pos)
	{
		nOffset = pos;
		InvalidateArrange();
	}
}

void CXTPMarkupScrollViewer::OnMouseWheel(CXTPMarkupMouseWheelEventArgs* e)
{
	SetScrollPos(SB_VERT, e->m_nDelta < 0 ? GetScrollPos(SB_VERT) + 48 : GetScrollPos(SB_VERT) - 48);

	e->SetHandled();
}

