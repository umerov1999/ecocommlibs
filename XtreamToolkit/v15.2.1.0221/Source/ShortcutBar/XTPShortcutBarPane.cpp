// XTPShortcutBarPane.cpp : implementation of the CXTPShortcutBarPane class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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
#include "Common/Resource.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"

#include "XTPShortcutBarPaintManager.h"
#include "XTPShortcutBarPane.h"
#include "XTPShortcutBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarPaneItem

CXTPShortcutBarPaneItem::CXTPShortcutBarPaneItem(LPCTSTR lpszCaption, CWnd* pWnd, int nHeight)
{
	m_strCaption = lpszCaption;
	m_pWndClient = pWnd;
	m_nHeight = nHeight;
	m_bShowCaption = TRUE;
	m_pPane = NULL;

	m_bExpandable = TRUE;
	m_bExpanded = TRUE;
}

void CXTPShortcutBarPaneItem::SetHeight(int nHeight)
{
	m_nHeight = max(0, nHeight);
}


/////////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarPane

IMPLEMENT_DYNAMIC(CXTPShortcutBarPane, CWnd)

CXTPShortcutBarPane::CXTPShortcutBarPane()
{
	m_pShortcutBar = NULL;

	m_bShowCaption = TRUE;
	m_nMinClientHeight = 0;

	m_rcIndent.SetRect(0, 5, 0, 5);
	m_pHighlighted = NULL;

	m_hHandCursor = AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(32649));

	if (m_hHandCursor == 0)
		m_hHandCursor = XTPResourceManager()->LoadCursor(XTP_IDC_HAND);

	m_rcMinimizeButton.SetRectEmpty();
	m_bMinimizeButtonHighlighted = FALSE;
	m_bMinimizeButtonPressed = FALSE;

	m_bFlatStyle = FALSE;
}

CXTPShortcutBarPane::~CXTPShortcutBarPane()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		delete m_arrItems[i];
	}
}

CXTPShortcutBarPaneItem* CXTPShortcutBarPane::AddItem(LPCTSTR lpszCaption, CWnd* pWnd, int nHeight)
{
	CXTPShortcutBarPaneItem* pItem = new CXTPShortcutBarPaneItem(lpszCaption, pWnd, nHeight);
	pItem->m_pPane = this;

	m_arrItems.Add(pItem);

	return pItem;
}


BOOL CXTPShortcutBarPane::Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent)
{
	m_pShortcutBar = pParent;
	m_strCaption = lpszCaption;

	if (!CWnd::Create(AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CXTPEmptyRect(), pParent, 0))
	{
		return FALSE;
	}

	return TRUE;
}

CXTPShortcutBarPaneItem* CXTPShortcutBarPane::GetItem(int nIndex) const
{
	if (nIndex >= 0 && nIndex < m_arrItems.GetSize())
		return m_arrItems[nIndex];
	return NULL;
}

void CXTPShortcutBarPane::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;

	if (m_hWnd)
	{
		Invalidate(FALSE);
	}
}

void CXTPShortcutBarPaneItem::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;

	if (m_pPane && m_pPane->m_hWnd)
	{
		m_pPane->Invalidate(FALSE);
	}
}

CXTPShortcutBarPaneItem* CXTPShortcutBarPane::HitTest(CPoint point) const
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->GetCaptionRect().PtInRect(point))
			return m_arrItems[i];
	}

	return NULL;
}


BEGIN_MESSAGE_MAP(CXTPShortcutBarPane, CWnd)
	//{{AFX_MSG_MAP(CXTPShortcutBarPane)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarPane message handlers


void CXTPShortcutBarPane::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPShortcutBarPaneItem* pItem = HitTest(point);
	if (pItem && !pItem->m_bExpandable)
		pItem = NULL;

	BOOL bMinimizeButtonHighlighted = m_rcMinimizeButton.PtInRect(point);
	if (bMinimizeButtonHighlighted != m_bMinimizeButtonHighlighted)
	{
		m_bMinimizeButtonHighlighted = bMinimizeButtonHighlighted;
		Invalidate(FALSE);

		if (m_bMinimizeButtonHighlighted)
		{
			XTPTrackMouseEvent(m_hWnd, TME_LEAVE);
		}
	}

	if (pItem != m_pHighlighted)
	{
		m_pHighlighted = pItem;
		Invalidate(FALSE);

		if (m_pHighlighted)
		{
			XTPTrackMouseEvent(m_hWnd, TME_LEAVE);
		}
	}


	CWnd::OnMouseMove(nFlags, point);
}

void CXTPShortcutBarPane::OnMouseLeave()
{
	if (m_pHighlighted)
	{
		m_pHighlighted = NULL;
		Invalidate(FALSE);
	}

	if (m_bMinimizeButtonHighlighted)
	{
		m_bMinimizeButtonHighlighted = FALSE;
		Invalidate(FALSE);
	}
}


void CXTPShortcutBarPane::OnLButtonDown(UINT nFlags, CPoint point)
{

	if (m_bMinimizeButtonHighlighted)
	{

		m_bMinimizeButtonPressed = TRUE;
		Invalidate(FALSE);
		SetCapture();
		BOOL bAccept = FALSE;


		while (::GetCapture() == m_hWnd)
		{
			MSG msg;
			VERIFY(::GetMessage(&msg, NULL, 0, 0));

			if (msg.message == WM_LBUTTONUP)
			{
				bAccept = m_bMinimizeButtonHighlighted;
				break;
			}

			if (msg.message == WM_MOUSEMOVE)
			{
				point = CPoint(LOWORD(msg.lParam), HIWORD(msg.lParam));
				BOOL bMinimizeButtonHighlighted = m_rcMinimizeButton.PtInRect(point);

				if (bMinimizeButtonHighlighted != m_bMinimizeButtonHighlighted)
				{
					m_bMinimizeButtonHighlighted = bMinimizeButtonHighlighted;
					Invalidate(FALSE);
				}
				continue;
			}

			DispatchMessage (&msg);
		}

		ReleaseCapture();

		m_bMinimizeButtonPressed = FALSE;
		Invalidate(FALSE);

		if (bAccept)
		{
			m_pShortcutBar->OnMinimizeButtonClicked();
		}
		return;
	}

	if (m_pHighlighted && m_pHighlighted->IsExpandable())
	{
		SetCapture();
		BOOL bAccept = FALSE;

		CXTPShortcutBarPaneItem* pHighlighted = m_pHighlighted;

		while (::GetCapture() == m_hWnd)
		{
			MSG msg;
			VERIFY(::GetMessage(&msg, NULL, 0, 0));

			if (msg.message == WM_LBUTTONUP)
			{
				bAccept = m_pHighlighted != NULL;
				break;
			}

			if (msg.message == WM_MOUSEMOVE)
			{
				point = CPoint(LOWORD(msg.lParam), HIWORD(msg.lParam));
				CXTPShortcutBarPaneItem* pItem = pHighlighted->GetCaptionRect().PtInRect(point) ? pHighlighted : NULL;

				if (pItem != m_pHighlighted)
				{
					m_pHighlighted = pItem;
					Invalidate(FALSE);
				}
				continue;
			}

			DispatchMessage (&msg);
		}

		ReleaseCapture();

		if (bAccept && m_pHighlighted)
		{
			m_pHighlighted->SetExpanded(!m_pHighlighted->IsExpanded());
		}
		return;
	}



	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CXTPShortcutBarPane::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_pHighlighted)
	{
		::SetCursor(m_hHandCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPShortcutBarPane::RecalcLayout()
{
	if (!m_hWnd)
		return;

	CXTPClientRect rc(this);

	CXTPShortcutBarPaintManager* pPaintManager = m_pShortcutBar->GetPaintManager();
	ASSERT(pPaintManager);
	if (!pPaintManager)
		return;

	int nPaneCaptionHeight = m_bShowCaption ? pPaintManager->DrawPaneCaption(NULL, this, FALSE) : 0;
	int nItemCaptionHeight = pPaintManager->DrawPaneItemCaption(NULL, NULL, FALSE);

	int nTop = nPaneCaptionHeight;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarPaneItem* pItem = m_arrItems[i];

		int nClientTop = nTop + (pItem->m_bShowCaption ? nItemCaptionHeight : 0);
		pItem->m_rcCaption.SetRect(0, nTop, rc.right, nClientTop);

		if (pItem->IsExpanded())
		{
			CRect rcClient(0 + m_rcIndent.left, m_rcIndent.top + nClientTop, rc.right - m_rcIndent.right, m_rcIndent.top + nClientTop + pItem->m_nHeight);
			if (rcClient.bottom > rc.bottom) rcClient.bottom = rc.bottom;
			if (i == m_arrItems.GetSize() - 1) rcClient.bottom = rc.bottom;

			pItem->m_rcClient = rcClient;

			nTop = nClientTop + pItem->m_nHeight + m_rcIndent.top + m_rcIndent.bottom;
			if (pItem->m_pWndClient)
			{
				pItem->m_pWndClient->SetWindowPos(0, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
					SWP_NOZORDER | SWP_SHOWWINDOW);
			}
		}
		else
		{
			pItem->m_rcClient.SetRectEmpty();
			nTop = nClientTop;

			if (pItem->m_pWndClient)
			{
				pItem->m_pWndClient->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_HIDEWINDOW);
			}
		}
	}

	m_rcMinimizeButton.SetRectEmpty();

	if (m_pShortcutBar->IsMinimizeButtonVisible() && (nPaneCaptionHeight > 0 || m_bFlatStyle))
	{

		CSize sz(18, 18);
		m_rcMinimizeButton = CRect(CPoint(rc.right - sz.cx - 4, max(0, (nPaneCaptionHeight - sz.cy) / 2)), sz);
	}
	m_bMinimizeButtonHighlighted = FALSE;

	Invalidate(FALSE);
}

void CXTPShortcutBarPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	RecalcLayout();
}

BOOL CXTPShortcutBarPane::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPShortcutBarPane::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPBufferDC dc(dcPaint);
	OnDraw(&dc);
}

LRESULT CXTPShortcutBarPane::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		OnDraw(pDC);
	}

	return TRUE;
}

COLORREF CXTPShortcutBarPane::GetBackgroundColor() const
{
	return m_bFlatStyle ? GetShortcutBar()->GetPaintManager()->m_clrFlatBackground : GetXtremeColor(COLOR_WINDOW);
}


COLORREF CXTPShortcutBarPane::GetTextColor() const
{
	return m_bFlatStyle ? GetShortcutBar()->GetPaintManager()->m_clrFlatTextColor : GetXtremeColor(COLOR_WINDOWTEXT);
}

void CXTPShortcutBarPane::OnDraw(CDC* pDC)
{
	CXTPClientRect rc(this);
	pDC->FillSolidRect(rc, GetBackgroundColor());

	CXTPShortcutBarPaintManager* pPaintManager = m_pShortcutBar->GetPaintManager();
	ASSERT(pPaintManager);
	if (!pPaintManager)
		return;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarPaneItem* pItem = m_arrItems[i];

		if (pItem->m_bShowCaption)
		{
			pPaintManager->DrawPaneItemCaption(pDC, pItem, TRUE);
		}
	}
	if (m_bShowCaption)
	{
		pPaintManager->DrawPaneCaption(pDC, this, TRUE);
	}
	else if (!m_rcMinimizeButton.IsRectEmpty())
	{
		pPaintManager->DrawPaneCaptionMinimizeButton(pDC, this);

	}
}
