// XTPScrollBase.cpp : implementation of the CXTPCommandBarScrollBarCtrl class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPColorManager.h"

#include "XTPCommandBarsDefines.h"
#include "XTPCommandBars.h"
#include "XTPPaintManager.h"
#include "XTPScrollBase.h"
#include "XTPMouseManager.h"


#define IDSYS_SCROLL 23345
#define IDSYS_ANIMATE 23346


//////////////////////////////////////////////////////////////////////////
//  CXTPScrollBase

CXTPScrollBase::SLIDERTICKS::SLIDERTICKS(double* pTicks, int nCount)
{
	this->nCount = nCount;
	this->pTicks = new double[nCount];

	for (int i = 0; i < nCount; i++)
		this->pTicks[i] = pTicks[i];
}

CXTPScrollBase::SLIDERTICKS::~SLIDERTICKS()
{
	SAFE_DELETE(pTicks);
}

CXTPScrollBase::CXTPScrollBase()
{
	ZeroMemory(&m_spi, sizeof(m_spi));
	m_spi.fVert = TRUE;

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
		m_pTicks = new SLIDERTICKS(pTicks, nCount);
	}
}

void CXTPScrollBase::CalcScrollBarInfo(LPRECT lprc, SCROLLBARPOSINFO* pSBInfo, SCROLLINFO* pSI)
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
	GetScrollInfo(&si);

	CalcScrollBarInfo(&rc, &m_spi, &si);
}

CRect CXTPScrollBase::GetScrollBarPartRect(int nPart) const
{
	CRect rc(m_spi.rc);

	switch (nPart)
	{
		case XTP_HTSCROLLUP: return m_spi.fVert ? CRect(rc.left, rc.top, rc.right, rc.top + m_spi.pxUpArrow) : CRect(rc.left, rc.top, rc.left + m_spi.pxUpArrow, rc.bottom);
		case XTP_HTSCROLLDOWN: return m_spi.fVert ? CRect(rc.left, rc.bottom - m_spi.pxDownArrow, rc.right, rc.bottom) : CRect(rc.right - m_spi.pxDownArrow, rc.top, rc.right, rc.bottom);
		case XTP_HTSCROLLUPPAGE: return m_spi.fVert ? CRect(rc.left, rc.top + m_spi.pxUpArrow, rc.right, rc.top + m_spi.pxThumbTop) : CRect(rc.left + m_spi.pxUpArrow, rc.top, rc.left + m_spi.pxThumbTop, rc.bottom);
		case XTP_HTSCROLLDOWNPAGE: return m_spi.fVert ? CRect(rc.left, rc.top + m_spi.pxThumbBottom, rc.right, rc.top + m_spi.pxDownArrow) : CRect(rc.left + m_spi.pxThumbBottom, rc.top, rc.left + m_spi.pxDownArrow, rc.bottom);
		case XTP_HTSCROLLTHUMB: return m_spi.fVert ? CRect(rc.left, rc.top + m_spi.pxThumbTop, rc.right, rc.top + m_spi.pxThumbBottom) : CRect(rc.left + m_spi.pxThumbTop, rc.top, rc.left + m_spi.pxThumbBottom, rc.bottom);
	}

	return rc;
}

int CXTPScrollBase::HitTestScrollBar(POINT pt) const
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

void CXTPScrollBase::EndScroll(BOOL fCancel)
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

AFX_INLINE int SBPosFromPx(CXTPScrollBase::SCROLLBARPOSINFO*  pSBInfo, int px)
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


void CXTPScrollBase::MoveThumb(int px)
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

void CXTPScrollBase::TrackThumb(UINT message, CPoint pt)
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
		pSBTrack->hTimerSB = SetTimer(m_pSBTrack->hWndTrack, IDSYS_SCROLL, m_nTimerElapse = GetDoubleClickTime() / 10, NULL);

		DoScroll(pSBTrack->cmdSB, 0);
	}
}

void CXTPScrollBase::CalcTrackDragRect(SCROLLBARTRACKINFO* pSBTrack) const
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

void CXTPScrollBase::PerformTrackInit(HWND hWnd, CPoint point, SCROLLBARPOSINFO* pSBInfo, BOOL bDirect)
{

	int px = m_spi.fVert ? point.y : point.x;

	SCROLLBARTRACKINFO* pSBTrack = new SCROLLBARTRACKINFO;
	memset(pSBTrack, 0, sizeof(SCROLLBARTRACKINFO));

	pSBTrack->cmdSB = (UINT)-1;
	pSBTrack->bTrackThumb = FALSE;
	pSBTrack->pSBInfo = pSBInfo;
	pSBTrack->hWndTrack = hWnd;

	m_pSBTrack = pSBTrack;

	CXTPMouseManager* pMouseManager = XTPMouseManager();

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

	pMouseManager->LockMouseMove();

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
	pMouseManager->UnlockMouseMove();

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
	if (m_scrollBarStyle != xtpScrollStyleDefault)
		return m_scrollBarStyle;

	switch (GetScrollBarPaintManager()->GetPaintManager()->BaseTheme())
	{
		case xtpThemeOffice2000:
			return xtpScrollStyleFlat;

		case xtpThemeResource:
		case xtpThemeRibbon:
			return xtpScrollStyleOffice2007Light;
	}

	return xtpScrollStyleSystem;
}


//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarScrollBarCtrl

CXTPCommandBarScrollBarCtrl::CXTPCommandBarScrollBarCtrl()
{
	m_pCommandBars = 0;
}

CXTPCommandBarScrollBarCtrl::~CXTPCommandBarScrollBarCtrl()
{

}

BEGIN_MESSAGE_MAP(CXTPCommandBarScrollBarCtrl, CScrollBar)
	//{{AFX_MSG_MAP(CXTPStatusBar)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_ENABLE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(SBM_SETSCROLLINFO, OnSetScrollInfo)
	ON_MESSAGE(SBM_GETSCROLLINFO, OnGetScrollInfo)
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CRect CXTPCommandBarScrollBarCtrl::GetScrollBarRect()
{
	return CXTPClientRect(this);
}

void CXTPCommandBarScrollBarCtrl::GetScrollInfo(SCROLLINFO* psi)
{
	psi->cbSize = sizeof(SCROLLINFO);

	psi->fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	::GetScrollInfo(m_hWnd, SB_CTL, psi);
}

BOOL CXTPCommandBarScrollBarCtrl::IsScrollBarEnabled() const
{
	return IsWindowEnabled();
}

CWnd* CXTPCommandBarScrollBarCtrl::GetParentWindow() const
{
	return GetParent();
}

CXTPScrollBarPaintManager* CXTPCommandBarScrollBarCtrl::GetScrollBarPaintManager() const
{
	if (m_pCommandBars)
		return (CXTPScrollBarPaintManager*)m_pCommandBars->GetPaintManager()->GetGalleryPaintManager();

	return (CXTPScrollBarPaintManager*)XTPPaintManager()->GetGalleryPaintManager();
}

void CXTPCommandBarScrollBarCtrl::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPBufferDC dc(dcPaint);

	OnDraw(&dc);
}

LRESULT CXTPCommandBarScrollBarCtrl::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC) OnDraw(pDC);
	return 1;
}

void CXTPCommandBarScrollBarCtrl::OnDraw(CDC* pDC)
{
	DefWindowProc(WM_PAINT, (WPARAM)pDC->GetSafeHdc(), 0);

	m_spi.fVert = GetStyle() & SBS_VERT;

	if (!m_pSBTrack || !m_pSBTrack->bTrackThumb)
		SetupScrollInfo();

	GetScrollBarPaintManager()->DrawScrollBar(pDC, this);
}

void CXTPCommandBarScrollBarCtrl::DoScroll(int cmd, int pos)
{
	GetParent()->SendMessage((UINT)(m_spi.fVert ? WM_VSCROLL : WM_HSCROLL),
		MAKELONG(cmd, pos), (LPARAM)m_hWnd);
}

void CXTPCommandBarScrollBarCtrl::RedrawScrollBar()
{
	if (m_hWnd) Invalidate(FALSE);
}


void CXTPCommandBarScrollBarCtrl::OnEnable(BOOL /*bEnable*/)
{
	RedrawScrollBar();
}

void CXTPCommandBarScrollBarCtrl::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPCommandBarScrollBarCtrl::OnMouseMove(UINT /*nFlags*/, CPoint point)
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


void CXTPCommandBarScrollBarCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	PerformTrackInit(m_hWnd, point, &m_spi, (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE);
}

void CXTPCommandBarScrollBarCtrl::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	CScrollBar::OnContextMenu(pWnd, pos);
	Invalidate(FALSE);
}

void CXTPCommandBarScrollBarCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

LRESULT CXTPCommandBarScrollBarCtrl::OnSetScrollInfo(WPARAM wParam, LPARAM lParam)
{
	LRESULT bResult = DefWindowProc(SBM_SETSCROLLINFO, FALSE, lParam);

	if (wParam)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}

	return bResult;
}

LRESULT CXTPCommandBarScrollBarCtrl::OnGetScrollInfo(WPARAM wParam, LPARAM lParam)
{
	LRESULT bResult = DefWindowProc(SBM_GETSCROLLINFO, wParam, lParam);

	LPSCROLLINFO lpsi = (LPSCROLLINFO)lParam;

	if (lpsi && (lpsi->fMask & SIF_TRACKPOS) && m_pSBTrack)
	{
		lpsi->nTrackPos = m_pSBTrack->posNew;
	}

	return bResult;
}


void CXTPCommandBarScrollBarCtrl::OnSetFocus(CWnd* pOldWnd)
{
	pOldWnd;
}


//////////////////////////////////////////////////////////////////////////
// CXTPScrollBarPaintManager

CXTPScrollBarPaintManager::CXTPScrollBarPaintManager(CXTPPaintManager* pPaintManager)
{
	m_pPaintManager = pPaintManager;

	m_cxHScroll = GetSystemMetrics(SM_CXHSCROLL);
	m_cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
	m_cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	m_cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

	m_cThumb = 0;

	m_themeScrollBar = new CXTPWinThemeWrapper();
}

CXTPScrollBarPaintManager::~CXTPScrollBarPaintManager()
{
	SAFE_DELETE(m_themeScrollBar);
}


void CXTPScrollBarPaintManager::RefreshMetrics()
{
	m_cxHScroll = GetSystemMetrics(SM_CXHSCROLL);
	m_cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
	m_cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	m_cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

	m_themeScrollBar->OpenTheme(0, L"SCROLLBAR");
}

void CXTPScrollBarPaintManager::DrawArrowGlyph(CDC* pDC, CRect rcArrow, BOOL bHorz, BOOL bUpArrow, BOOL bEnabled)
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

void CXTPScrollBarPaintManager::DrawScrollBar(CDC* pDC, CXTPScrollBase* pGallery)
{
	#define GETPARTSTATE(ht, pressed, hot, normal, disabled) \
		(!bEnabled ? disabled : nPressetHt == ht ? pressed : nHotHt == ht ? hot : normal)

	CXTPScrollBase::SCROLLBARTRACKINFO* pSBTrack = pGallery->GetScrollBarTrackInfo();
	CXTPScrollBase::SCROLLBARPOSINFO* pSBInfo = pGallery->GetScrollBarPosInfo();

	BOOL nPressetHt = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1) : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;


	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
	{
		return;
	}

	BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0) && pGallery->IsScrollBarEnabled();

	int nBtnTrackSize =   pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;
	XTPScrollBarStyle style = pGallery->GetScrollBarStyle();

	if (pSBInfo->fVert)
	{
		CRect rcVScroll(pSBInfo->rc);

		CRect rcArrowUp(rcVScroll.left, rcVScroll.top, rcVScroll.right, pSBInfo->pxUpArrow);
		CRect rcArrowDown(rcVScroll.left, pSBInfo->pxDownArrow, rcVScroll.right, rcVScroll.bottom);
		CRect rcTrack(rcVScroll.left, rcArrowUp.bottom, rcVScroll.right, rcArrowDown.top);

		CRect rcLowerTrack(rcTrack.left, rcTrack.top, rcTrack.right, rcTrack.top + nBtnTrackPos);
		CRect rcBtnTrack(rcTrack.left, rcLowerTrack.bottom, rcTrack.right, rcLowerTrack.bottom + nBtnTrackSize);
		CRect rcUpperTrack(rcTrack.left, rcBtnTrack.bottom, rcTrack.right, rcTrack.bottom);


		if (style == xtpScrollStyleSystem  && m_themeScrollBar->IsAppThemed())
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
			if (style == xtpScrollStyleFlat)
			{
				pDC->FillSolidRect(rcArrowUp, GetXtremeColor(COLOR_3DFACE));
				if (bEnabled && nPressetHt == XTP_HTSCROLLUP)
					pDC->Draw3dRect(rcArrowUp,GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
				else
					pDC->Draw3dRect(rcArrowUp, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));

				DrawArrowGlyph(pDC, rcArrowUp, FALSE, TRUE, bEnabled);

				pDC->FillSolidRect(rcArrowDown, GetXtremeColor(COLOR_3DFACE));
				if (bEnabled && nPressetHt == XTP_HTSCROLLDOWN)
					pDC->Draw3dRect(rcArrowDown,GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
				else
					pDC->Draw3dRect(rcArrowDown, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				DrawArrowGlyph(pDC, rcArrowDown, FALSE, FALSE, bEnabled);

			}
			else
			{
				DrawFrameControl(pDC->GetSafeHdc(), &rcArrowUp, DFC_SCROLL, DFCS_SCROLLUP | (!bEnabled ? DFCS_INACTIVE : 0) | (nPressetHt == XTP_HTSCROLLUP ? DFCS_PUSHED : 0));
				DrawFrameControl(pDC->GetSafeHdc(), &rcArrowDown, DFC_SCROLL, DFCS_SCROLLDOWN | (!bEnabled ? DFCS_INACTIVE : 0)  | (nPressetHt == XTP_HTSCROLLDOWN ? DFCS_PUSHED : 0));
			}

			CWnd* pParent = pGallery->GetParentWindow();
			HBRUSH hbrRet = (HBRUSH)DefWindowProc(pParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pParent->GetSafeHwnd());

			::FillRect(pDC->GetSafeHdc(), &rcTrack, hbrRet);

			if (nPressetHt == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &rcLowerTrack);
			}

			if (!rcTrack.IsRectEmpty() && !rcBtnTrack.IsRectEmpty())
			{
				pDC->FillSolidRect(rcBtnTrack, GetXtremeColor(COLOR_3DFACE));

				if (style == xtpScrollStyleFlat)
				{
					pDC->Draw3dRect(rcBtnTrack, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				}
				else
				{
					DrawEdge(pDC->GetSafeHdc(), &rcBtnTrack, EDGE_RAISED, (UINT)(BF_ADJUST | BF_RECT));
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


		if (style == xtpScrollStyleSystem  && m_themeScrollBar->IsAppThemed())
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
			if (style == xtpScrollStyleFlat)
			{
				pDC->FillSolidRect(rcArrowLeft, GetXtremeColor(COLOR_3DFACE));
				if (bEnabled && nPressetHt == XTP_HTSCROLLUP)
					pDC->Draw3dRect(rcArrowLeft,GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
				else
					pDC->Draw3dRect(rcArrowLeft, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));

				DrawArrowGlyph(pDC, rcArrowLeft, TRUE, TRUE, bEnabled);

				pDC->FillSolidRect(rcArrowRight, GetXtremeColor(COLOR_3DFACE));
				if (bEnabled && nPressetHt == XTP_HTSCROLLDOWN)
					pDC->Draw3dRect(rcArrowRight,GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
				else
					pDC->Draw3dRect(rcArrowRight, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				DrawArrowGlyph(pDC, rcArrowRight, TRUE, FALSE, bEnabled);

			}
			else
			{
				DrawFrameControl(pDC->GetSafeHdc(), &rcArrowLeft, DFC_SCROLL, DFCS_SCROLLLEFT | (!bEnabled ? DFCS_INACTIVE : 0) | (nPressetHt == XTP_HTSCROLLUP ? DFCS_PUSHED : 0));
				DrawFrameControl(pDC->GetSafeHdc(), &rcArrowRight, DFC_SCROLL, DFCS_SCROLLRIGHT | (!bEnabled ? DFCS_INACTIVE : 0)  | (nPressetHt == XTP_HTSCROLLDOWN ? DFCS_PUSHED : 0));
			}

			CWnd* pParent = pGallery->GetParentWindow();
			HBRUSH hbrRet = (HBRUSH)DefWindowProc(pParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pParent->GetSafeHwnd());

			::FillRect(pDC->GetSafeHdc(), &rcTrack, hbrRet);

			if (nPressetHt == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &rcLowerTrack);
			}

			if (!rcTrack.IsRectEmpty() && !rcBtnTrack.IsRectEmpty())
			{
				pDC->FillSolidRect(rcBtnTrack, GetXtremeColor(COLOR_3DFACE));

				if (style == xtpScrollStyleFlat)
				{
					pDC->Draw3dRect(rcBtnTrack, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
				}
				else
				{
					DrawEdge(pDC->GetSafeHdc(), &rcBtnTrack, EDGE_RAISED, (UINT)(BF_ADJUST | BF_RECT));
				}
			}

			if (nPressetHt == XTP_HTSCROLLDOWNPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &rcUpperTrack);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPSliderPaintManager

CXTPSliderPaintManager::CXTPSliderPaintManager(CXTPPaintManager* pPaintManager)
	: CXTPScrollBarPaintManager(pPaintManager)
{
}

void CXTPSliderPaintManager::RefreshMetrics()
{
	CXTPScrollBarPaintManager::RefreshMetrics();

	m_cyHScroll = 16;
	m_cxHScroll = 16;

	m_cThumb = 11;
}

void CXTPSliderPaintManager::DrawScrollBar(CDC* pDC, CXTPScrollBase* pGallery)
{
	CXTPScrollBase::SCROLLBARTRACKINFO* pSBTrack = pGallery->GetScrollBarTrackInfo();
	CXTPScrollBase::SCROLLBARPOSINFO* pSBInfo = pGallery->GetScrollBarPosInfo();

	BOOL nPressetHt = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1) : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;

	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
		return;

	BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0) && pGallery->IsScrollBarEnabled();

	int nBtnTrackSize =   pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	COLORREF clrFace = GetXtremeColor(COLOR_3DFACE);
	COLORREF clrShadow = GetXtremeColor(COLOR_3DSHADOW);
	COLORREF clrLight = GetXtremeColor(COLOR_3DHIGHLIGHT);

	if (!pSBInfo->fVert)
	{
		CRect rcHScroll(pSBInfo->rc);

		CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
		CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);
		CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);
		CRect rcBtnTrack(rcTrack.left + nBtnTrackPos, rcTrack.top, rcTrack.left + nBtnTrackPos + nBtnTrackSize, rcTrack.bottom);

		CRect rcTrackDest(CPoint(rcTrack.left, (rcTrack.top + rcTrack.bottom - 2) / 2), CSize(rcTrack.Width(), 2));

		pDC->FillSolidRect(rcTrackDest.left, rcTrackDest.top, rcTrackDest.Width(), 1, clrShadow);
		pDC->FillSolidRect(rcTrackDest.left, rcTrackDest.top + 1, rcTrackDest.Width(), 1, clrLight);

		CXTPScrollBase::SLIDERTICKS* pTicks = pGallery->GetTicks();
		if (!pTicks || (pSBInfo->posMax <= pSBInfo->posMin))
		{
			CRect rcTrackTickDest(CPoint((rcTrackDest.left + rcTrackDest.right - 2) / 2,
				(rcTrackDest.top + rcTrackDest.bottom - 8) / 2), CSize(2, 8));

			pDC->FillSolidRect(rcTrackTickDest.left, rcTrackTickDest.top, 1, 4, clrShadow);
			pDC->FillSolidRect(rcTrackTickDest.left, rcTrackTickDest.top + 5, 1, 2, clrShadow);
			pDC->FillSolidRect(rcTrackTickDest.left + 1, rcTrackTickDest.top + 1, 1, 7, clrLight);
		}
		else
		{
			rcTrackDest.DeflateRect(6, 0);

			for (int i = 0; i < pTicks->nCount; i++)
			{
				double dTick = pTicks->pTicks[i];
				double dPos = (dTick - pSBInfo->posMin) * rcTrackDest.Width() / (pSBInfo->posMax - pSBInfo->posMin);

				if (dPos >= 0 && dPos <= rcTrackDest.Width())
				{
					CRect rcTrackTickDest(CPoint(rcTrackDest.left + (int)dPos - 1,
						(rcTrackDest.top + rcTrackDest.bottom - 8) / 2), CSize(2, 8));

					pDC->FillSolidRect(rcTrackTickDest.left, rcTrackTickDest.top, 1, 4, clrShadow);
					pDC->FillSolidRect(rcTrackTickDest.left, rcTrackTickDest.top + 5, 1, 2, clrShadow);
					pDC->FillSolidRect(rcTrackTickDest.left + 1, rcTrackTickDest.top + 1, 1, 7, clrLight);
				}
			}
		}

		m_pPaintManager->AlphaEllipse(pDC, rcArrowLeft, clrShadow, nHotHt == XTP_HTSCROLLUP ? clrLight : clrFace);
		if (nPressetHt != XTP_HTSCROLLUP)
		{
			rcArrowLeft.DeflateRect(1, 1);
			m_pPaintManager->AlphaEllipse(pDC, rcArrowLeft, clrLight, nHotHt == XTP_HTSCROLLUP ? clrLight : clrFace);
		}
		CPoint ptCenter = rcArrowLeft.CenterPoint();
		pDC->FillSolidRect(ptCenter.x - 4, ptCenter.y - 1, 8, 2, 0);

		m_pPaintManager->AlphaEllipse(pDC, rcArrowRight, clrShadow, nHotHt == XTP_HTSCROLLDOWN ? clrLight : clrFace);
		if (nPressetHt != XTP_HTSCROLLDOWN)
		{
			rcArrowRight.DeflateRect(1, 1);
			m_pPaintManager->AlphaEllipse(pDC, rcArrowRight, clrLight, nHotHt == XTP_HTSCROLLDOWN ? clrLight : clrFace);
		}
		ptCenter = rcArrowRight.CenterPoint();
		pDC->FillSolidRect(ptCenter.x - 4, ptCenter.y - 1, 8, 2, 0);
		pDC->FillSolidRect(ptCenter.x - 1, ptCenter.y - 4, 2, 8, 0);

		if (bEnabled)
		{
			CRect rc(CPoint((rcBtnTrack.left + rcBtnTrack.right - 8) / 2,
				(rcBtnTrack.top + rcBtnTrack.bottom - 13) / 2), CSize(8, 13));

			POINT pts[] = {rc.left, rc.top, rc.right, rc.top,
				rc.right,  rc.bottom - 4, rc.left + 4, rc.bottom, rc.left, rc.bottom - 4, rc.left, rc.top};

			CXTPPenDC pen (*pDC, nHotHt == XTP_HTSCROLLTHUMB ? clrLight : clrFace);
			CXTPBrushDC brush (*pDC, nHotHt == XTP_HTSCROLLTHUMB ? clrLight : clrFace);

			pDC->Polygon(pts, sizeof(pts) / sizeof(POINT));

			pen.Color(clrShadow);
			pDC->Polyline(pts, sizeof(pts) / sizeof(POINT));

			if (nPressetHt != XTP_HTSCROLLTHUMB)
			{
				POINT pts2[] = {rc.left + 1, rc.top + 1, rc.right - 1, rc.top + 1,
					rc.right - 1,  rc.bottom - 4, rc.left + 4, rc.bottom - 1, rc.left + 1, rc.bottom - 4, rc.left + 1, rc.top + 1};

				pen.Color(clrLight);
				pDC->Polyline(pts2, sizeof(pts2) / sizeof(POINT));
			}
		}
	}
}
