// XTPCoreTreeControl.cpp : implementation file
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

#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"

#include "XTPCoreTreeControl.h"
#include "XTPCoreTreeItem.h"
#include "XTPCoreTreePaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPCoreTreeControl, CWnd)
CLIPFORMAT CXTPCoreTreeControl::m_cfItem = (CLIPFORMAT)::RegisterClipboardFormat(_T("CoreTreeItem"));

//////////////////////////////////////////////////////////////////////////
// CTreeDropTarget

class CXTPCoreTreeControl::CTreeDropTarget : public COleDropTarget
{
public:
	CTreeDropTarget()
	{
		m_nHoverTime = 500;
		SystemParametersInfo(SPI_GETMOUSEHOVERTIME, 0, &m_nHoverTime, 0);
	}


	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
	{
		CXTPCoreTreeControl* pTree = DYNAMIC_DOWNCAST(CXTPCoreTreeControl, pWnd);
		if (!pTree)
			return DROPEFFECT_NONE;


		CRect rectClient;
		pWnd->GetClientRect(&rectClient);
		CRect rect = rectClient;

		// hit-test against inset region
		UINT nTimerID = MAKEWORD(-1, -1);
		rect.InflateRect(0, -nScrollInset);

		const int nTimerHover = 10;

		if (rectClient.PtInRect(point) && !rect.PtInRect(point))
		{
			// determine which way to scroll along both X & Y axis
			if (point.y < rect.top)
				nTimerID = SB_LINEUP;
			else if (point.y >= rect.bottom)
				nTimerID = SB_LINEDOWN;
		}
		else if (rectClient.PtInRect(point))
		{
			nTimerID = nTimerHover;
		}

		if (nTimerID == nTimerHover)
		{
			// save tick count when timer ID changes
			DWORD dwTick = GetTickCount();
			if (point != m_ptLastPoint)
			{
				m_ptLastPoint = point;
				m_dwLastTick = dwTick;
				m_nScrollDelay = m_nHoverTime;
			}

			// scroll if necessary
			if (dwTick - m_dwLastTick > m_nScrollDelay)
			{
				pTree->OnDragHover(pDataObject, dwKeyState, point);
				m_dwLastTick = dwTick;
			}

		}
		else if (nTimerID != MAKEWORD(-1, -1))
		{
			// save tick count when timer ID changes
			DWORD dwTick = GetTickCount();
			if (nTimerID != m_nTimerID)
			{
				m_dwLastTick = dwTick;
				m_nScrollDelay = nScrollDelay;
			}

			// scroll if necessary
			if (dwTick - m_dwLastTick > m_nScrollDelay)
			{
				pTree->OnVScroll(nTimerID, 0, NULL);
				m_dwLastTick = dwTick;
				m_nScrollDelay = nScrollInterval;
			}
		}

		m_nTimerID = nTimerID;

		return pTree->OnDragOver(pDataObject, dwKeyState, point);
	}

	virtual void OnDragLeave(CWnd* pWnd)
	{
		CXTPCoreTreeControl* pTree = DYNAMIC_DOWNCAST(CXTPCoreTreeControl, pWnd);

		if (pTree)
		{
			pTree->OnDragOver(NULL, 0, CPoint(-1, -1));
		}
	}
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point)
	{
		CXTPCoreTreeControl* pTree = DYNAMIC_DOWNCAST(CXTPCoreTreeControl, pWnd);

		if (pTree)
		{
			return pTree->OnDrop(pDataObject, dropEffect, point);
		}
		return FALSE;
	}

	CPoint m_ptLastPoint;
	int m_nHoverTime;
};

/////////////////////////////////////////////////////////////////////////////
// CXTPCoreTreeControl

CXTPCoreTreeControl::CXTPCoreTreeControl()
{
	RegisterWindowClass();

	m_pItems = new CXTPCoreTreeItems();
	m_pItems->m_pControl = this;

	m_pPaintManager = new CXTPCoreTreePaintManager();
	m_pPaintManager->RefreshMetrics();

	m_nClientHeight = 0;
	m_rcControl.SetRectEmpty();

	m_bPreSubclassWindow = TRUE;


	m_bScrollVisible = FALSE;
	m_bInUpdateScrollBar = FALSE;

	m_pImageManager = new CXTPImageManager();
	m_nLockRedraw = 0;

	m_ptPressed = CPoint(-1, -1);

	m_pFocusedItem = NULL;
	m_pDraggingItem = NULL;
	m_pPressedItem = NULL;
	m_pDragOverItem = NULL;

	m_nDropPosition = xtpCoreTreeDropAfter;

	m_pDropTarget = new CTreeDropTarget();

	m_pMarkupContext = NULL;
}

CXTPCoreTreeControl::~CXTPCoreTreeControl()
{
	SAFE_DELETE(m_pItems);
	SAFE_DELETE(m_pPaintManager);

	CMDTARGET_RELEASE(m_pImageManager);

	SAFE_DELETE(m_pDropTarget);

	XTPMarkupReleaseContext(m_pMarkupContext);


}

BOOL CXTPCoreTreeControl::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance, _T("XTPCoreTree"), CS_DBLCLKS);
}


void CXTPCoreTreeControl::SetPaintManager(CXTPCoreTreePaintManager* pPaintManager)
{
	ASSERT (pPaintManager);

	SAFE_DELETE(m_pPaintManager);

	m_pPaintManager = pPaintManager;
	m_pPaintManager->RefreshMetrics();
}

void CXTPCoreTreeControl::EnableMarkup(BOOL bEnable)
{
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnable)
	{
		m_pMarkupContext = XTPMarkupCreateContext();
	}
}

void CXTPCoreTreeControl::SetImageManager(CXTPImageManager* pImageManager)
{
	CMDTARGET_RELEASE(m_pImageManager);

	m_pImageManager = pImageManager;

}

void CXTPCoreTreeControl::SetFocusedItem(CXTPCoreTreeItem* pItem)
{
	if (m_pFocusedItem == pItem)
		return;

	if (pItem && pItem->IsLabel())
		return;

	m_pFocusedItem = pItem;

	EnsureVisible(m_pFocusedItem);

	RedrawControl();

	SendMessageToParent(TVN_SELCHANGED);
}

LRESULT CXTPCoreTreeControl::SendMessageToParent(int nMessage, NMHDR* pNMHDR)
{
	if (!IsWindow(m_hWnd))
		return 0;

	NMHDR nmhdr;
	if (pNMHDR == NULL)
		pNMHDR = &nmhdr;

	pNMHDR->hwndFrom = GetSafeHwnd();
	pNMHDR->idFrom = GetDlgCtrlID();
	pNMHDR->code = nMessage;

	CWnd *pOwner = GetOwner();
	LRESULT res = 0;
	if (pOwner && IsWindow(pOwner->m_hWnd))
		res = pOwner->SendMessage(WM_NOTIFY, pNMHDR->idFrom, (LPARAM)pNMHDR);

	return res;
}


void CXTPCoreTreeControl::RedrawControl(LPCRECT lpcRect)
{
	if (m_nLockRedraw > 0)
	{
		return;
	}


	if (m_hWnd) InvalidateRect(lpcRect, FALSE);
}

void CXTPCoreTreeControl::SetLockRedraw(BOOL bLockRedraw)
{
	m_nLockRedraw += bLockRedraw ? +1 : -1;
	ASSERT(m_nLockRedraw >= 0);

	if (m_nLockRedraw == 0)
		RecalcLayout();
}


void CXTPCoreTreeControl::EnsureVisible(CXTPCoreTreeItem* pItem)
{
	if (!pItem || !m_hWnd)
		return;

	CXTPCoreTreeItem* pParentItem = pItem->GetParentItem();

	while (pParentItem != NULL)
	{
		pParentItem->SetExpanded(TRUE);
		pParentItem = pParentItem->GetParentItem();
	}

	CXTPClientRect rcClient(this);
	CRect rc = pItem->GetRect();

	if (rc.bottom > rcClient.bottom + GetScrollOffset())
	{
		OnScrollChanged(min(rc.top, rc.bottom - rcClient.Height()));
	}
	else if (rc.top < GetScrollOffset())
	{
		OnScrollChanged(rc.top);
	}

}

int CXTPCoreTreeControl::GetScrollOffset() const
{
	if (!m_bScrollVisible)
		return 0;

	return GetScrollPos(SB_VERT);
}

void CXTPCoreTreeControl::OnScrollChanged(int nScrollOffset)
{
	SetScrollPos(SB_VERT, nScrollOffset);
	RedrawControl();
}

CXTPCoreTreeItem* CXTPCoreTreeControl::HitTestTreeItems(CXTPCoreTreeItems* pItems, CPoint point) const
{
	POSITION pos = pItems->GetHeadPosition();
	while (pos)
	{
		CXTPCoreTreeItem* pItem = pItems->GetNext(pos);

		if (pItem->GetRect().PtInRect(point))
			return pItem;

		if (pItem->IsExpanded() && pItem->HasChildren())
		{
			pItem = HitTestTreeItems(pItem->GetChildren(), point);
			if (pItem != NULL)
				return pItem;
		}
	}

	return NULL;
}

CXTPCoreTreeItem* CXTPCoreTreeControl::HitTest(CPoint point) const
{
	point.y += GetScrollOffset();

	return HitTestTreeItems(m_pItems, point);
}



BOOL CXTPCoreTreeControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CreateEx(0, _T("XTPCoreTree"), NULL, dwStyle, rect, pParentWnd, nID))
	{
		return FALSE;
	}

	m_pDropTarget->Register(this);

	return TRUE;
}


void CXTPCoreTreeControl::OnItemRemoved(CXTPCoreTreeItem* pItem)
{
	if (pItem == m_pFocusedItem)
	{
		SetFocusedItem(NULL);
	}

	if (pItem == m_pPressedItem)
		m_pPressedItem = NULL;
}

void CXTPCoreTreeControl::ExpandeTreeItems(CXTPCoreTreeItems* pItems)
{
	POSITION pos = pItems->GetHeadPosition();
	while (pos)
	{
		CXTPCoreTreeItem* pItem = pItems->GetNext(pos);

		if (pItem->HasChildren())
		{
			pItem->SetExpanded(TRUE);

			ExpandeTreeItems(pItem->GetChildren());
		}
	}
}

BEGIN_MESSAGE_MAP(CXTPCoreTreeControl, CWnd)
	//{{AFX_MSG_MAP(CXTPCoreTreeControl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETDLGCODE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCPAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_MESSAGE(WM_SETFONT, OnSetFont)

END_MESSAGE_MAP()



int CXTPCoreTreeControl::RecalcTreeItems(CXTPCoreTreeItems* pItems, CDC* pDC, int nLevel, LPCRECT lprcMargin)
{
	int nTop = lprcMargin->top;
	int nWidth = m_rcControl.Width() - lprcMargin->left - lprcMargin->right;

	POSITION pos = pItems->GetHeadPosition();
	while (pos)
	{
		CXTPCoreTreeItem* pItem = pItems->GetNext(pos);

		CSize sz = pItem->CalcSize(pDC, nWidth);

		pItem->SetRect(CRect(m_rcControl.left + lprcMargin->left, nTop, m_rcControl.right - lprcMargin->right, nTop + sz.cy));

		nTop += sz.cy;

		if (pItem->IsExpanded() && pItem->HasChildren())
		{
			CRect rcMargin(lprcMargin);
			rcMargin.top = nTop;

			rcMargin.left += m_pPaintManager->GetChildrenIndent(pItem);

			nTop = RecalcTreeItems(pItem->GetChildren(), pDC, nLevel + 1, rcMargin);
		}
	}

	return nTop;
}

void CXTPCoreTreeControl::RecalcLayout()
{
	if (!GetSafeHwnd())
		return;

	if (m_nLockRedraw > 0)
		return;


	CXTPClientRect rc(this);

	m_rcControl = rc;
	CRect rcMargin(0, 0, 0, 0);

	{
		CClientDC dc(this);

		int nPos = RecalcTreeItems(m_pItems, &dc, 0, rcMargin);

		m_nClientHeight = nPos;
	}

	UpdateScrollBar();

	RedrawControl();
}

void CXTPCoreTreeControl::UpdateScrollBar()
{
	if (m_bInUpdateScrollBar)
		return;

	m_bInUpdateScrollBar = TRUE;

	CXTPClientRect rc(this);
	m_bScrollVisible = rc.Height() < m_nClientHeight;

	if (m_bScrollVisible)
	{
		ShowScrollBar(SB_VERT, TRUE);

		SCROLLINFO  si ;

		si.cbSize = sizeof(SCROLLINFO) ;
		si.fMask = SIF_PAGE | SIF_RANGE ;

		GetScrollInfo(SB_VERT, &si, si.fMask);

		UINT nControlHeight = (UINT)rc.Height();

		if (si.nPage != nControlHeight || si.nMax != m_nClientHeight - 1 || si.nMin != 0)
		{
			si.nPage = nControlHeight ;
			si.nMax = m_nClientHeight - 1;
			si.nMin = 0 ;
			si.fMask = SIF_PAGE | SIF_RANGE ;
			SetScrollInfo(SB_VERT, &si) ;
		}
		EnableScrollBarCtrl(SB_VERT, TRUE);
	}
	else
	{
		SetScrollPos(SB_VERT, 0);
		ShowScrollBar(SB_VERT, FALSE);
	}

	m_bInUpdateScrollBar = FALSE;
}



void CXTPCoreTreeControl::DrawTreeItems(CXTPCoreTreeItems* pItems, CDC* pDC, CRect rcClipBox)
{
	POSITION pos = pItems->GetHeadPosition();
	while (pos)
	{
		CXTPCoreTreeItem* pItem = pItems->GetNext(pos);

		if (CRect().IntersectRect(pItem->GetRect(), rcClipBox))
		{
			pItem->Draw(pDC);
		}


		if (pItem->IsExpanded() && pItem->HasChildren())
		{
			DrawTreeItems(pItem->GetChildren(), pDC, rcClipBox);
		}
	}
}

void CXTPCoreTreeControl::OnDraw(CDC* pDC, CRect rcClipBox)
{
	pDC->SetBkMode(TRANSPARENT);

	m_pPaintManager->FillTreeControl(pDC, this);

	int nScrollOffset = GetScrollOffset();

	CPoint pt = pDC->GetViewportOrg();
	pDC->SetViewportOrg(pt.x, pt.y - nScrollOffset);

	rcClipBox.OffsetRect(0, nScrollOffset);

	DrawTreeItems(m_pItems, pDC, rcClipBox);

	if (m_pDragOverItem)
	{
		m_pPaintManager->DrawDropRect(pDC, m_pDragOverItem);
	}

	pDC->SetViewportOrg(pt);
}


BOOL CXTPCoreTreeControl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//
	// PreCreateWindow is called when a control is dynamically
	// created. We want to set m_bPreSubclassWindow to FALSE
	// here so the control is initialized from CWnd::Create and
	// not CWnd::PreSubclassWindow.
	//

	m_bPreSubclassWindow = FALSE;

	return TRUE;
}

void CXTPCoreTreeControl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	if (m_bPreSubclassWindow)
	{
		m_pDropTarget->Register(this);
	}

	RecalcLayout();
}



/////////////////////////////////////////////////////////////////////////////
// CXTPCoreTreeControl message handlers

void CXTPCoreTreeControl::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CRect rcPaint(dcPaint.m_ps.rcPaint);


	CXTPBufferDC dc(dcPaint);

	OnDraw(&dc, rcPaint);
}

LRESULT CXTPCoreTreeControl::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		OnDraw(pDC, CXTPClientRect(this));
	}

	return TRUE;
}



BOOL CXTPCoreTreeControl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPCoreTreeControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	RecalcLayout();
}

CXTPCoreTreeItem* CXTPCoreTreeControl::GetNextFocusableItem(CXTPCoreTreeItem* pFocusedItem) const
{
	do
	{
		if (!pFocusedItem)
		{
			pFocusedItem = m_pItems->GetFirstItem();
		}
		else if (pFocusedItem->HasChildren() && pFocusedItem->IsExpanded())
		{
			pFocusedItem = pFocusedItem->GetFirstChildItem();
		}
		else
		{
			while (pFocusedItem && pFocusedItem->GetNextSiblingItem() == NULL)
			{
				pFocusedItem = pFocusedItem->GetParentItem();
			}

			if (pFocusedItem)
			{
				pFocusedItem = pFocusedItem->GetNextSiblingItem();
			}

		}

		if (pFocusedItem && !pFocusedItem->IsLabel())
			return pFocusedItem;

		if (!pFocusedItem)
			return NULL;
	}
	while (pFocusedItem);

	return NULL;
}

CXTPCoreTreeItem* CXTPCoreTreeControl::GetPrevFocusableItem(CXTPCoreTreeItem* pFocusedItem) const
{

	do
	{
		if (!pFocusedItem)
		{
			pFocusedItem = m_pItems->GetLastItem();

			while (pFocusedItem && pFocusedItem->HasChildren() && pFocusedItem->IsExpanded())
			{
				pFocusedItem = pFocusedItem->GetChildren()->GetLastItem();
			}
		}
		else if (pFocusedItem->GetPrevSiblingItem() != NULL)
		{
			pFocusedItem = pFocusedItem->GetPrevSiblingItem();

			while (pFocusedItem->HasChildren() && pFocusedItem->IsExpanded())
			{
				pFocusedItem = pFocusedItem->GetChildren()->GetLastItem();
			}
		}
		else
		{
			pFocusedItem = pFocusedItem->GetParentItem();
		}

		if (pFocusedItem && !pFocusedItem->IsLabel())
			return pFocusedItem;

		if (!pFocusedItem)
			return NULL;
	}
	while (pFocusedItem);

	return NULL;
}

void CXTPCoreTreeControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_DOWN:
		{
			CXTPCoreTreeItem* pItem = GetNextFocusableItem(m_pFocusedItem);
			if (pItem != NULL)
			{
				SetFocusedItem(pItem);
			}
		}
		break;

	case VK_UP:
		{
			CXTPCoreTreeItem* pItem = GetPrevFocusableItem(m_pFocusedItem);
			if (pItem != NULL)
			{
				SetFocusedItem(pItem);
			}
		}
		break;

	case VK_LEFT:
		{
			if (m_pFocusedItem && m_pFocusedItem->HasChildren() && m_pFocusedItem->IsExpanded())
			{
				m_pFocusedItem->SetExpanded(FALSE);
			}
			else
			{
				CXTPCoreTreeItem* pItem = m_pFocusedItem ? m_pFocusedItem->GetParentItem() : NULL;
				if (pItem != NULL)
				{
					SetFocusedItem(pItem);
				}
			}
		}
		break;

	case VK_BACK:
		{
			CXTPCoreTreeItem* pItem = m_pFocusedItem ? m_pFocusedItem->GetParentItem() : NULL;
			if (pItem != NULL)
			{
				SetFocusedItem(pItem);
			}
		}
		break;

	case VK_RIGHT:
		{
			if (m_pFocusedItem && m_pFocusedItem->HasChildren())
			{
				if (!m_pFocusedItem->IsExpanded())
				{
					m_pFocusedItem->SetExpanded(TRUE);
				}
				else
				{
					CXTPCoreTreeItem* pItem = GetNextFocusableItem(m_pFocusedItem);
					if (pItem != NULL)
					{
						SetFocusedItem(pItem);
					}
				}
			}
		}
		break;

	case VK_HOME:
		{
			CXTPCoreTreeItem* pItem = GetNextFocusableItem(NULL);

			if (pItem != NULL)
			{
				SetFocusedItem(pItem);
			}
		}
		break;

	case VK_END:
		{
			CXTPCoreTreeItem* pItem = GetPrevFocusableItem(NULL);

			if (pItem != NULL)
			{
				SetFocusedItem(pItem);
			}
		}
		break;

	case VK_NEXT:
		{
			CXTPClientRect rc(this);
			int nItems = (rc.Height() / m_pPaintManager->GetItemHeight()) - 2;

			CXTPCoreTreeItem* pItem = m_pFocusedItem;

			for (int i = 0; i < nItems; i++)
			{
				CXTPCoreTreeItem* pNextItem = GetNextFocusableItem(pItem);
				if (!pNextItem)
					break;
				pItem = pNextItem;
			}

			if (pItem != NULL)
			{
				SetFocusedItem(pItem);
			}
		}
		break;

	case VK_PRIOR:
		{
			CXTPClientRect rc(this);
			int nItems = (rc.Height() / m_pPaintManager->GetItemHeight()) - 2;

			CXTPCoreTreeItem* pItem = m_pFocusedItem;

			for (int i = 0; i < nItems; i++)
			{
				CXTPCoreTreeItem* pNextItem = GetPrevFocusableItem(pItem);
				if (!pNextItem)
					break;
				pItem = pNextItem;
			}

			if (pItem != NULL)
			{
				SetFocusedItem(pItem);
			}
		}
		break;

	case VK_SPACE:

		if (m_pFocusedItem && m_pFocusedItem->IsCheckboxVisible())
		{
			OnCheckClicked(m_pFocusedItem);
		}
		break;

	case VK_ADD:

		if (m_pFocusedItem && m_pFocusedItem->HasChildren())
		{
			m_pFocusedItem->SetExpanded(TRUE);
		}
		break;

	case VK_SUBTRACT:

		if (m_pFocusedItem && m_pFocusedItem->HasChildren())
		{
			m_pFocusedItem->SetExpanded(FALSE);
		}
		break;

	case VK_MULTIPLY:

		if (m_pFocusedItem && m_pFocusedItem->HasChildren())
		{
			SetLockRedraw(TRUE);

			m_pFocusedItem->SetExpanded(TRUE);
			ExpandeTreeItems(m_pFocusedItem->GetChildren());

			SetLockRedraw(FALSE);
		}
		break;
	}


	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPCoreTreeControl::StartDragItem(CXTPCoreTreeItem* /*pItem*/)
{

}

BOOL CXTPCoreTreeControl::DragItem(CXTPCoreTreeItem* pItem, DROPEFFECT dropEffectMask)
{
	ASSERT(pItem);
	if (!pItem)
		return FALSE;

	COleDataSource ds;
	if (!pItem->PrepareDrag(ds))
		return FALSE;

	m_pDraggingItem = pItem;


	DROPEFFECT dropEffect = ds.DoDragDrop(dropEffectMask);

	BOOL bRemoved = FALSE;

	if ((dropEffect == DROPEFFECT_MOVE) && (dropEffectMask & DROPEFFECT_MOVE))
	{
		if (m_pDraggingItem)
		{
			m_pDraggingItem->Remove();
			bRemoved = TRUE;
		}
	}


	m_pDraggingItem = NULL;

	return bRemoved;
}

void CXTPCoreTreeControl::SetDragOverItem(CXTPCoreTreeItem* pDragOverItem, XTPCoreTreeDropPosition nDropPosition)
{
	if (m_pDragOverItem != pDragOverItem || nDropPosition != m_nDropPosition)
	{
		m_pDragOverItem = pDragOverItem;
		m_nDropPosition = nDropPosition;

		RedrawControl();
	}

};

DROPEFFECT CXTPCoreTreeControl::OnDragOver(COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint /*point*/)
{
	return DROPEFFECT_NONE;

}

void CXTPCoreTreeControl::OnDragHover(COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint point)
{
	CXTPCoreTreeItem* pItem = HitTest(point);

	if (pItem && pItem->HasChildren())
	{
		pItem->SetExpanded(TRUE);
	}
}


BOOL CXTPCoreTreeControl::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint /*point*/)
{
	if (!m_pDragOverItem)
		return FALSE;

	CXTPCoreTreeItem* pDragOverItem = m_pDragOverItem;
	m_pDragOverItem = NULL;

	RedrawControl();

	if (dropEffect != DROPEFFECT_COPY && dropEffect != DROPEFFECT_MOVE)
		return FALSE;

	if (!pDataObject || !pDataObject->IsDataAvailable(GetClipboardFormat()))
		return FALSE;


	CXTPCoreTreeItem* pItemDrop = CXTPCoreTreeItem::CreateFromOleData(pDataObject);
	if (!pItemDrop)
		return FALSE;


	if (m_nDropPosition == xtpCoreTreeDropAfter)
	{
		pDragOverItem->GetParentItems()->AddItemAfter(pItemDrop, pDragOverItem);
	}
	else if (m_nDropPosition == xtpCoreTreeDropBefore)
	{
		pDragOverItem->GetParentItems()->AddItemBefore(pItemDrop, pDragOverItem);
	}
	else if (m_nDropPosition == xtpCoreTreeDropChild)
	{
		pDragOverItem->GetChildren()->AddItemBefore(pItemDrop, NULL);
		pDragOverItem->SetExpanded();
	}
	else
	{
		ASSERT(FALSE);
	}

	SetFocusedItem(pItemDrop);

	return TRUE;
}


void CXTPCoreTreeControl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pPressedItem && (abs(m_ptPressed.x - point.x) + abs(m_ptPressed.y - point.y) > 4))
	{
		CXTPCoreTreeItem* pItem = m_pPressedItem;
		ReleaseCapture();

		StartDragItem(pItem);
		return;
	}


	CWnd::OnMouseMove(nFlags, point);
}

void CXTPCoreTreeControl::OnCheckClicked(CXTPCoreTreeItem* pItem)
{
	if (pItem->IsEnabled())
	{
		pItem->SetChecked(!pItem->IsChecked());

		SendMessageToParent(XTP_TVN_CHECKCHANGED);
	}
}

void CXTPCoreTreeControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	CXTPCoreTreeItem* pItem = HitTest(point);

	if (pItem != NULL)
	{
		m_ptPressed = point;

		point.y += GetScrollOffset();

		if (pItem->HasChildren() && pItem->GetButtonRect().PtInRect(point))
		{
			pItem->SetExpanded(!pItem->IsExpanded());

			if (pItem->IsParent(m_pFocusedItem))
			{
				SetFocusedItem(pItem);
			}
		}
		else if (pItem->GetCheckboxRect().PtInRect(point))
		{
			SetFocusedItem(pItem);
			OnCheckClicked(pItem);
		}
		else
		{
			SetFocusedItem(pItem);

			SetCapture();
			m_pPressedItem = pItem;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPCoreTreeControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_pPressedItem = NULL;
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPCoreTreeControl::OnCaptureChanged(CWnd* pWnd)
{
	m_pPressedItem = NULL;

	CWnd::OnCaptureChanged(pWnd);
}



void CXTPCoreTreeControl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CXTPCoreTreeItem* pItem = HitTest(point);

	if (pItem)
	{
		point.y += GetScrollOffset();

		if (pItem->GetCheckboxRect().PtInRect(point))
		{
			OnCheckClicked(pItem);
		}
		else if (pItem->HasChildren())
		{
			pItem->SetExpanded(!pItem->IsExpanded());
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}


void CXTPCoreTreeControl::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	m_pPaintManager->RefreshMetrics();
	RedrawControl();
}

UINT CXTPCoreTreeControl::OnGetDlgCode()
{
	return DLGC_WANTARROWS | DLGC_WANTCHARS;
}


void CXTPCoreTreeControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL)
	{
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	// its horizontal scroll bar
	int nCurPos = GetScrollPos(SB_VERT);

	// decide what to do for each diffrent scroll event
	switch (nSBCode)
	{
	case SB_LEFT: nCurPos = 0; break;
	case SB_RIGHT: nCurPos = GetScrollLimit(SB_VERT); break;
	case SB_LINELEFT: nCurPos = max(nCurPos - 6, 0); break;
	case SB_LINERIGHT: nCurPos = min(nCurPos + 6, GetScrollLimit(SB_VERT)); break;
	case SB_PAGELEFT: nCurPos = max(nCurPos - CXTPClientRect(this).Height(), 0); break;
	case SB_PAGERIGHT: nCurPos = min(nCurPos + CXTPClientRect(this).Height(), GetScrollLimit(SB_VERT)); break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		{
			SCROLLINFO si;
			ZeroMemory(&si, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;
			if (!GetScrollInfo(SB_VERT, &si))
				return;
			nCurPos = si.nTrackPos;
		}
		break;
	}

	OnScrollChanged(nCurPos);
}

BOOL CXTPCoreTreeControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_bScrollVisible)
	{
		int nCurPos = GetScrollPos(SB_VERT);

		int nDelta = 3 * m_pPaintManager->GetItemHeight();

		if (zDelta > 0) nCurPos = max(nCurPos - nDelta, 0);
		else nCurPos = min(nCurPos + nDelta, GetScrollLimit(SB_VERT));

		OnScrollChanged(nCurPos);

		RedrawControl();

		return TRUE;
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

LRESULT CXTPCoreTreeControl::OnSetFont(WPARAM wParam, LPARAM lParam)
{
	HFONT hFont = (HFONT)wParam;

	LOGFONT lf;

	if (GetObject(hFont, sizeof(LOGFONT), &lf))
	{
		m_pPaintManager->SetFontIndirect(&lf);
	}

	return CWnd::DefWindowProc(WM_SETFONT, wParam, lParam);
}

void CXTPCoreTreeControl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}


void CXTPCoreTreeControl::OnNcPaint()
{
	Default();

	if (GetPaintManager()->IsAppThemed() && GetExStyle() & WS_EX_CLIENTEDGE)
	{
		CWindowDC dc(this);

		CXTPWindowRect rc(this);
		rc.OffsetRect(-rc.TopLeft());

		dc.Draw3dRect(rc, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));
		rc.DeflateRect(1, 1);
		dc.Draw3dRect(rc, GetSysColor(COLOR_WINDOW), GetSysColor(COLOR_WINDOW));
	}
}

void CXTPCoreTreeControl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	if (m_pFocusedItem)
	{
		RedrawControl(m_pFocusedItem->GetScreenRect());
	}
}

void CXTPCoreTreeControl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	if (m_pFocusedItem)
	{
		RedrawControl(m_pFocusedItem->GetScreenRect());
	}
}
