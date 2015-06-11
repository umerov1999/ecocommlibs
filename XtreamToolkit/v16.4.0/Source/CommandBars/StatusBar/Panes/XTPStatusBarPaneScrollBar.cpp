#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/Hook/XTPWinEventHook.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPane.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneScrollBar.h>

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarScrollBarPane

IMPLEMENT_DYNAMIC(CXTPStatusBarScrollBarPane, CXTPStatusBarPane)

CXTPStatusBarScrollBarPane::CXTPStatusBarScrollBarPane()
{
	m_nMax = 100;
	m_nMin = 0;
	m_nValue = 0;
	m_nPage = 0;

	m_nStyle |= SBPS_NOBORDERS;
	m_bButton = TRUE;
	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(0, 0, 0, 0);

	m_spi.fVert = FALSE;
	m_spi.fSizebox = FALSE;
}

CXTPStatusBarScrollBarPane::~CXTPStatusBarScrollBarPane()
{

}

CRect CXTPStatusBarScrollBarPane::GetScrollBarRect()
{
	CRect rcItem(GetRect());
	rcItem.DeflateRect(m_rcPadding);

	int nHeight = GetScrollBarPaintManager()->m_cyHScroll;
	if (rcItem.Height() > nHeight)
	{
		rcItem.top = (rcItem.top + rcItem.bottom - nHeight) / 2;
		rcItem.bottom = rcItem.top + nHeight;
	}

	return rcItem;
}

void CXTPStatusBarScrollBarPane::GetScrollInfo(SCROLLINFO* psi)
{
	psi->cbSize = sizeof(SCROLLINFO);

	psi->fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	psi->nMax = m_nMax;
	psi->nMin = m_nMin;
	psi->nPage = m_nPage;
	psi->nPos = m_nValue;
}

BOOL CXTPStatusBarScrollBarPane::IsScrollBarEnabled() const
{
	return IsEnabled();
}

CWnd* CXTPStatusBarScrollBarPane::GetParentWindow() const
{
	return m_pStatusBar;
}

CXTPScrollBarPaintManager* CXTPStatusBarScrollBarPane::GetScrollBarPaintManager() const
{
	return (CXTPScrollBarPaintManager*)m_pStatusBar->GetPaintManager()->GetGalleryPaintManager();
}

void CXTPStatusBarScrollBarPane::OnDraw(CDC* pDC, CRect /*rcItem*/)
{
	if (!m_pSBTrack || !m_pSBTrack->bTrackThumb)
		SetupScrollInfo();

	GetScrollBarPaintManager()->DrawScrollBar(pDC, this);
}

void CXTPStatusBarScrollBarPane::DoScroll(int cmd, int pos)
{
#ifndef _XTP_ACTIVEX
	CWnd* pParent = m_pStatusBar->GetParent();

	NMXTPSCROLL nmScroll;
	nmScroll.hdr.code = XTP_SBN_SCROLL;
	nmScroll.hdr.idFrom = GetID();
	nmScroll.hdr.hwndFrom = m_pStatusBar->GetSafeHwnd();
	nmScroll.pSender = this;
	nmScroll.nPos = pos;
	nmScroll.nSBCode = cmd;

	LRESULT lResult = 0;
	AFX_NOTIFY notify;
	notify.pResult = &lResult;
	notify.pNMHDR = (NMHDR*)&nmScroll.hdr;

	pParent->OnCmdMsg(GetID(), MAKELONG(XTP_SBN_SCROLL, WM_NOTIFY), &notify, NULL);
#else
	m_pStatusBar->FireSliderPaneClick(GetIDispatch(FALSE), cmd, pos);
#endif
}

void CXTPStatusBarScrollBarPane::RedrawScrollBar()
{
	Redraw();
}

void CXTPStatusBarScrollBarPane::OnMouseMove(CPoint point)
{
	int ht = HitTestScrollBar(point);

	if (ht != m_spi.ht)
	{
		m_spi.ht = ht;
		Redraw();
	}
}


void CXTPStatusBarScrollBarPane::OnLButtonDown(CPoint point)
{
	PerformTrackInit(m_pStatusBar->m_hWnd, point, &m_spi, (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE);
}

void CXTPStatusBarScrollBarPane::SetPos(int nValue)
{
	if (m_nValue != nValue)
	{
		m_nValue = nValue;
		Redraw();
	}
}

int CXTPStatusBarScrollBarPane::GetPos() const
{
	return m_nValue;
}

void CXTPStatusBarScrollBarPane::SetRange(int nMin, int nMax)
{
	m_nMin = nMin;
	m_nMax = nMax;
}

INT_PTR CXTPStatusBarScrollBarPane::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	int ht = HitTestScrollBar(point);
	if (ht < XTP_HTSCROLLUP || ht > XTP_HTSCROLLTHUMB)
		return -1;

	CString strTip = m_strToolTipPart[ht - XTP_HTSCROLLUP];
	if (strTip.IsEmpty())
		return -1;

	CXTPToolTipContext::FillInToolInfo(pTI, m_pStatusBar->m_hWnd, GetScrollBarPartRect(ht), ht, strTip);

	return ht;
}

void CXTPStatusBarScrollBarPane::SetTooltipPart(int nPart, LPCTSTR lpszTooltip)
{
	if (nPart >= XTP_HTSCROLLUP && nPart <= XTP_HTSCROLLTHUMB)
	{
		m_strToolTipPart[nPart - XTP_HTSCROLLUP] = lpszTooltip;
	}
	else
	{
		if (nPart == SB_LINELEFT) m_strToolTipPart[0] = lpszTooltip;
		if (nPart == SB_LINERIGHT) m_strToolTipPart[1] = lpszTooltip;
		if (nPart == SB_PAGELEFT) m_strToolTipPart[2] = lpszTooltip;
		if (nPart == SB_PAGERIGHT) m_strToolTipPart[3] = lpszTooltip;
		if (nPart == SB_THUMBTRACK) m_strToolTipPart[4] = lpszTooltip;
	}
}
