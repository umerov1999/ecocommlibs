// XTPScrollBase.cpp : implementation of the CXTPCommandBarScrollBarCtrl class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>


CRect OffsetSourceRect(CRect rc, int nState)
{
	rc.OffsetRect(0, (nState - 1) * rc.Height());
	return rc;
}


int SBPosFromPx(XTP_SCROLLBAR_POSINFO *pSBInfo, int px)
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




XTP_SLIDERTICKS::XTP_SLIDERTICKS(double* pTicks, int nCount)
{
	this->nCount = nCount;
	this->pTicks = new double[nCount];

	for (int i = 0; i < nCount; i++)
		this->pTicks[i] = pTicks[i];
}

XTP_SLIDERTICKS::~XTP_SLIDERTICKS()
{
	SAFE_DELETE(pTicks);
}

//////////////////////////////////////////////////////////////////////////
//  CXTPScrollBase

CXTPScrollBase::CXTPScrollBase()
{
	ZeroMemory(&m_spi, sizeof(m_spi));
	m_spi.fVert = TRUE;
	m_spi.fSizebox = FALSE;

	m_pSBTrack = NULL;

	m_scrollBarStyle = xtpScrollStyleDefault;

	m_bAnimation = FALSE;
	m_nTimerElapse = 0;

	m_pTicks = NULL;
}

CXTPScrollBase::~CXTPScrollBase()
{
	SAFE_DELETE(m_pTicks);
}

void CXTPScrollBase::SetTicks(double* pTicks, int nCount)
{
	SAFE_DELETE(m_pTicks);

	if (nCount)
	{
		m_pTicks = new XTP_SLIDERTICKS(pTicks, nCount);
	}
}

void CXTPScrollBase::CalcScrollBarInfo(LPRECT lprc, XTP_SCROLLBAR_POSINFO *pSBInfo, SCROLLINFO* pSI)
{
	int cpx;
	DWORD dwRange;
	int denom;
	BOOL fVert = pSBInfo->fVert;

	pSBInfo->rc = *lprc;
	pSBInfo->pxPopup = 0;

	CXTPScrollBarPaintManager* pPaintManager = GetScrollBarPaintManager();

	if (fVert)
	{
		pSBInfo->pxTop = lprc->top;
		pSBInfo->pxBottom = lprc->bottom;
		pSBInfo->pxLeft = lprc->left;
		pSBInfo->pxRight = lprc->right;
		pSBInfo->cpxThumb = pPaintManager->m_cyVScroll;
	}
	else
	{
		pSBInfo->pxTop = lprc->left;
		pSBInfo->pxBottom = lprc->right;
		pSBInfo->pxLeft = lprc->top;
		pSBInfo->pxRight = lprc->bottom;
		pSBInfo->cpxThumb = pPaintManager->m_cxHScroll;
	}

	pSBInfo->pos = pSI->nPos;
	pSBInfo->page = pSI->nPage;
	pSBInfo->posMin = pSI->nMin;
	pSBInfo->posMax = pSI->nMax;

	if (pSBInfo->pos > pSBInfo->posMax)
		pSBInfo->pos = pSBInfo->posMax;
	if (pSBInfo->pos < pSBInfo->posMin)
		pSBInfo->pos = pSBInfo->posMin;

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

	if (pPaintManager->m_cThumb > 0) pSBInfo->cpxThumb = pPaintManager->m_cThumb;

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

void CXTPScrollBase::SetupScrollInfo()
{
	CRect rc = GetScrollBarRect();

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(&si);

	CalcScrollBarInfo(&rc, &m_spi, &si);
}

CRect CXTPScrollBase::GetScrollBarPartRect(int nPart) const
{
	CRect rc(m_spi.rc);

	if(!m_spi.fSizebox)
	{
		switch (nPart)
		{
			case XTP_HTSCROLLUP: return m_spi.fVert ? CRect(rc.left, rc.top, rc.right, rc.top + m_spi.pxUpArrow) : CRect(rc.left, rc.top, rc.left + m_spi.pxUpArrow, rc.bottom);
			case XTP_HTSCROLLDOWN: return m_spi.fVert ? CRect(rc.left, rc.bottom - m_spi.pxDownArrow, rc.right, rc.bottom) : CRect(rc.right - m_spi.pxDownArrow, rc.top, rc.right, rc.bottom);
			case XTP_HTSCROLLUPPAGE: return m_spi.fVert ? CRect(rc.left, rc.top + m_spi.pxUpArrow, rc.right, rc.top + m_spi.pxThumbTop) : CRect(rc.left + m_spi.pxUpArrow, rc.top, rc.left + m_spi.pxThumbTop, rc.bottom);
			case XTP_HTSCROLLDOWNPAGE: return m_spi.fVert ? CRect(rc.left, rc.top + m_spi.pxThumbBottom, rc.right, rc.top + m_spi.pxDownArrow) : CRect(rc.left + m_spi.pxThumbBottom, rc.top, rc.left + m_spi.pxDownArrow, rc.bottom);
			case XTP_HTSCROLLTHUMB: return m_spi.fVert ? CRect(rc.left, rc.top + m_spi.pxThumbTop, rc.right, rc.top + m_spi.pxThumbBottom) : CRect(rc.left + m_spi.pxThumbTop, rc.top, rc.left + m_spi.pxThumbBottom, rc.bottom);
		}
	}

	return rc;
}

int CXTPScrollBase::HitTestScrollBar(POINT pt) const
{
	if(m_spi.fSizebox)
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

void CXTPScrollBase::EndScroll(BOOL fCancel)
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = m_pSBTrack;

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


void CXTPScrollBase::MoveThumb(int px)
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = m_pSBTrack;

	if ((pSBTrack == NULL) || (px == pSBTrack->pxOld))
		return;

	XTP_SCROLLBAR_POSINFO *pSBInfo = m_pSBTrack->pSBInfo;

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

void CXTPScrollBase::TrackThumb(UINT message, CPoint pt)
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = m_pSBTrack;
	if (!pSBTrack)
		return;

	XTP_SCROLLBAR_POSINFO *pSBInfo = pSBTrack->pSBInfo;

	if (HIBYTE(message) != HIBYTE(WM_MOUSEFIRST))
		return;

	if (pSBInfo == NULL)
		return;

	int px;

	if (!PtInRect(&pSBTrack->rcTrack, pt) && (GetScrollBarPaintManager()->m_cThumb == 0))
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

void CXTPScrollBase::TrackBox(UINT message, CPoint point)
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = m_pSBTrack;

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
			pSBTrack->hTimerSB = SetTimer(m_pSBTrack->hWndTrack, IDSYS_SCROLL, m_nTimerElapse = cmsTimer, NULL);

			DoScroll(pSBTrack->cmdSB, 0);
		}
	}
}

void CXTPScrollBase::DoAnimate()
{

}
void CXTPScrollBase::ContScroll()
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = m_pSBTrack;
	ASSERT(pSBTrack);

	if (pSBTrack == NULL)
		return;

	CPoint pt;
	GetCursorPos(&pt);

	ScreenToClient(m_pSBTrack->hWndTrack, &pt);

	TrackBox(WM_NULL, pt);

	if (pSBTrack->fHitOld)
	{
		pSBTrack->hTimerSB = SetTimer(m_pSBTrack->hWndTrack, IDSYS_SCROLL, m_nTimerElapse = GetDoubleClickTime() / 10, NULL);

		DoScroll(pSBTrack->cmdSB, 0);
	}
}

void CXTPScrollBase::CalcTrackDragRect(XTP_SCROLLBAR_TRACKINFO *pSBTrack) const
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

void CXTPScrollBase::PerformTrackInit(HWND hWnd, CPoint point, XTP_SCROLLBAR_POSINFO *pSBInfo, BOOL bDirect)
{

	int px = m_spi.fVert ? point.y : point.x;

	XTP_SCROLLBAR_TRACKINFO *pSBTrack = new XTP_SCROLLBAR_TRACKINFO;
	ZeroMemory(pSBTrack, sizeof(XTP_SCROLLBAR_TRACKINFO));

	pSBTrack->cmdSB = (UINT)-1;
	pSBTrack->bTrackThumb = FALSE;
	pSBTrack->pSBInfo = pSBInfo;
	pSBTrack->hWndTrack = hWnd;

	m_pSBTrack = pSBTrack;

	//CXTPMouseManager* pMouseManager = XTPMouseManager();

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
		RedrawScrollBar();
	}

	//pMouseManager->LockMouseMove();

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
			else if (cmd == WM_TIMER && msg.wParam == IDSYS_ANIMATE)
			{
				DoAnimate();
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

	//pMouseManager->UnlockMouseMove();

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

	while (IsWindow(hWnd) && m_bAnimation)
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		UINT cmd = msg.message;

		if (cmd == WM_TIMER && msg.wParam == IDSYS_ANIMATE)
		{
			DoAnimate();
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}

XTPScrollBarStyle CXTPScrollBase::GetScrollBarStyle() const
{
	return m_scrollBarStyle;
}


HBRUSH CXTPScrollBase::GetCltColor(CDC *pDC) const
{
	HBRUSH hBrush = NULL;

	CWnd *pWndParent = GetParentWindow();

	if (NULL != pWndParent && NULL != pDC)
	{
		LRESULT lResult = ::DefWindowProc(pWndParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, WPARAM(pDC->GetSafeHdc()), LPARAM(pWndParent->GetSafeHwnd()));
		hBrush = HBRUSH(lResult);
	}

	return hBrush;
}
