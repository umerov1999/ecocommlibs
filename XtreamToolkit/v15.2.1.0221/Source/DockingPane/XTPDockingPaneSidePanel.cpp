// XTPDockingPaneSidePanel.cpp : implementation of the CXTPDockingPaneSidePanel class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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
#include "Resource.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPSystemHelpers.h"

#include "TabManager/XTPTabManager.h"

#include "XTPDockingPaneDefines.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneBaseContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneSidePanel.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPaneContext.h"
#include "XTPDockingPaneAutoHidePanel.h"
#include "XTPDockingPanePaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TID_CHECKACTIVE 1
#define TID_SLIDEIN 2
#define TID_SLIDEOUT 3


AFX_INLINE int ResetStepsCount()
{
	if (CXTPDockingPaneAutoHideWnd::m_nAnimationInterval == 0)
		return 1;

	return max(1, CXTPDockingPaneAutoHideWnd::m_nAnimationDuration / CXTPDockingPaneAutoHideWnd::m_nAnimationInterval);
}

CXTPDockingPaneSidePanel::CXTPDockingPaneSidePanel(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBaseContainer(xtpPaneTypeSidePanel, pLayout)
{
	m_direction = xtpPaneDockLeft;
	m_bActive = FALSE;

	m_pCaptionButtons = new CXTPDockingPaneCaptionButtons();
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_CLOSE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_AUTOHIDE, this));

	m_bSlideOut = m_bExpanded = m_bCollapsed = FALSE;

	m_nSlideStep = 0;
	m_nStepsCount = ResetStepsCount();

	m_nDeactivationCount = 0;

}

CXTPDockingPaneSidePanel::~CXTPDockingPaneSidePanel()
{

}

void CXTPDockingPaneSidePanel::DeletePane()
{
	InternalRelease();
}

void CXTPDockingPaneSidePanel::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}

BOOL CXTPDockingPaneSidePanel::Init(CXTPDockingPaneBase* pBasePane, XTPDockingPaneDirection direction, CRect rc)
{
	CXTPDockingPaneBaseList lstPanes;
	pBasePane->FindPane(xtpPaneTypeDockingPane, &lstPanes);
	if (lstPanes.GetCount() == 0)
		return FALSE;

	m_direction = direction;
	m_rcWindow = rc;
	CreateContainer();

	CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)GetDockingPaneManager()->
		OnCreatePane(xtpPaneTypeTabbedContainer, m_pLayout);

	BOOL bInit = FALSE;

	POSITION pos = lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lstPanes.GetNext(pos);

		if (pPane->GetContainer())
		{
			pPane->GetContainer()->RemovePane(pPane);
		}

		if (!bInit)
		{
			pContainer->Init((CXTPDockingPane*)pPane, this);
		}
		else
		{
			pContainer->_InsertPane(pPane, FALSE);
		}
		bInit = TRUE;
	}

	_InsertPane(pContainer);

	OnFocusChanged();

	m_nIdleFlags = 0;

	return TRUE;
}

void CXTPDockingPaneSidePanel::CreateContainer()
{
	m_bInRecalcLayout = TRUE;

	if (!m_hWnd)
	{
		VERIFY(Create(m_rcWindow));
	}

	m_bInRecalcLayout = FALSE;
}

BOOL CXTPDockingPaneSidePanel::Create(CRect rc)
{
	if (m_hWnd)
		return TRUE;

	if (!CreateEx(0, _T("XTPDockingPaneSidePanel"), _T(""), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD, rc, GetDockingPaneManager()->GetSite(), 0))
	{
		return FALSE;
	}
	m_pCaptionButtons->CheckForMouseOver(CPoint(-1, -1));

	return TRUE;
}

void CXTPDockingPaneSidePanel::PostNcDestroy()
{
	// prevent auto deleting
}

void CXTPDockingPaneSidePanel::RecalcLayout(BOOL /*bNotify*/)
{
	if (m_bInRecalcLayout)
		return;

	m_bInRecalcLayout = TRUE;

	CXTPDockingPaneManager* pManager = GetDockingPaneManager();
	CWnd* pSite = GetDockingSite();
	CRect rcClient = pManager->GetClientPane()->GetPaneWindowRect();
	pSite->ScreenToClient(rcClient);

	OnSizeParentEx(pManager, pSite, rcClient);

	m_bInRecalcLayout = FALSE;


}

void CXTPDockingPaneSidePanel::OnFocusChanged()
{
	CXTPDockingPaneTabbedContainer* pContainer = GetTopContainer();
	if (pContainer)
	{
		pContainer->OnFocusChanged();
		BOOL bActive = pContainer->IsActive();

		if (bActive != m_bActive)
		{
			m_bActive = bActive;
			InvalidatePane(FALSE);
		}
	}
}

void CXTPDockingPaneSidePanel::InvalidatePane(BOOL /*bSelectionChanged*/)
{
	if (m_hWnd)
	{
		Invalidate(FALSE);
	}
}

CXTPDockingPaneTabbedContainer* CXTPDockingPaneSidePanel::GetTopContainer() const
{
	return (CXTPDockingPaneTabbedContainer*)GetFirstPane();
}

void CXTPDockingPaneSidePanel::OnChildContainerChanged(CXTPDockingPaneBase* /*pContainer*/)
{
	if (!m_hWnd)
		return;

	m_bExpanded = FALSE;
	m_nSlideStep = 0;

	if (IsEmpty())
	{
		DestroyWindow();
	}
	else
	{
		PostMessage(WM_IDLEUPDATECMDUI);
	}

	GetDockingPaneManager()->RecalcFrameLayout(NULL, TRUE);
}

void CXTPDockingPaneSidePanel::RemovePane(CXTPDockingPaneBase* pPane)
{
	POSITION pos = m_lstPanes.Find(pPane);
	ASSERT(pos);

	m_lstPanes.RemoveAt(pos);

	if (IsEmpty())
	{
		DestroyWindow();
	}

	pPane->m_pParentContainer = NULL;
}

struct CXTPDockingPaneSidePanel::LENGTH
{
	int nPos;
	int nSize;
	int nIndex;
	int nHeight;
	CXTPDockingPaneSidePanel* pPanel;

	int StartPos() {
		return nPos;
	}
	int EndPos() {
		return nPos + nSize;
	}
};

int _cdecl CXTPDockingPaneSidePanel::CompareLength(const void *arg1, const void *arg2)
{
	int& dOffset1 = ((LENGTH*)arg1)->nPos;
	int& dOffset2 = ((LENGTH*)arg2)->nPos;
	return dOffset1 - dOffset2;
}

void CXTPDockingPaneSidePanel::SortLength(LENGTH* pLength, int nFirstIndex, int nLastIndex)
{
	if (nLastIndex - nFirstIndex < 1)
		return;

	qsort(pLength + nFirstIndex, nLastIndex - nFirstIndex + 1, sizeof(LENGTH), CompareLength);

	for (int i = nFirstIndex; i <= nLastIndex; i++)
	{
		CXTPDockingPaneSidePanel* pPanel = pLength[i].pPanel;
		pPanel->m_nLengthIndex = i;
	}
}

void CXTPDockingPaneSidePanel::MovePanel(XTPDockingPaneDirection direction, CRect rect)
{
	m_direction = direction;
	m_rcWindow = rect;

	m_pLayout->MoveToTail(this);
	RecalcLayout(FALSE);
	InvalidatePane(FALSE);
}

void CXTPDockingPaneSidePanel::MovePanel(LENGTH* pLength, CRect rect)
{
	m_bInRecalcLayout = TRUE;

	if (m_bCollapsed && m_bSlideOut)
	{
		m_bSlideOut = FALSE;
		m_bExpanded = FALSE;
		m_nSlideStep = 0;
		KillTimer(TID_SLIDEOUT);
		OnAction(xtpPaneActionCollapsed);
	}

	BOOL bHorizontal = IsHorizontal();
	CSize sz = bHorizontal ? CSize(pLength->nSize, pLength->nHeight) : CSize(pLength->nHeight, pLength->nSize);

	int nMinHeight = GetMinHeight();

	if (m_bCollapsed && !m_bExpanded)
	{
		if (bHorizontal) sz.cy = nMinHeight; else sz.cx = nMinHeight;
	}
	else
	{
		if (bHorizontal) sz.cy = max(sz.cy, nMinHeight); else sz.cx = max(sz.cx, nMinHeight);
	}

	CPoint pt;

	switch (m_direction)
	{
	case xtpPaneDockLeft: pt = CPoint(rect.left, pLength->nPos + rect.top); break;
	case xtpPaneDockTop: pt = CPoint(rect.left + pLength->nPos, rect.top); break;
	case xtpPaneDockRight: pt = CPoint(rect.right - sz.cx, rect.top + pLength->nPos); break;
	case xtpPaneDockBottom: pt = CPoint(rect.left + pLength->nPos, rect.bottom - sz.cy); break;
	}

	SetWindowPos(&CWnd::wndTop, pt.x, pt.y, sz.cx, sz.cy, SWP_NOOWNERZORDER | SWP_SHOWWINDOW);

	CRect rcClient;
	GetClientRect(rcClient);

	GetPaintManager()->AdjustClientRect(this, rcClient);

	CXTPDockingPaneTabbedContainer* pContainer = GetTopContainer();
	if (pContainer)
	{
		pContainer->OnSizeParent(this, rcClient, NULL);
	}
	m_bInRecalcLayout = FALSE;
}

void CXTPDockingPaneSidePanel::OnSizeParentEx(CSidePanelArray& arrSide, CWnd* /*pParent*/, CRect rect)
{
	XTPDockingPaneDirection direction = arrSide[0]->m_direction;
	BOOL bHorizontal = direction == xtpPaneDockTop || direction == xtpPaneDockBottom;

	int nCount = (int)arrSide.GetSize(), i, j;

	LENGTH* pLength = new LENGTH[nCount];

	for (i = 0; i < nCount; i++)
	{
		CXTPDockingPaneSidePanel* pPanel = arrSide[i];

		CRect rcPanel(pPanel->m_rcWindow);

		MINMAXINFO mmi;
		pPanel->GetMinMaxInfo(&mmi);

		if (rcPanel.Height() > mmi.ptMaxTrackSize.y) rcPanel.bottom = rcPanel.top + mmi.ptMaxTrackSize.y;
		if (rcPanel.Height() < mmi.ptMinTrackSize.y) rcPanel.bottom = rcPanel.top + mmi.ptMinTrackSize.y;
		if (rcPanel.Width() > mmi.ptMaxTrackSize.x) rcPanel.right = rcPanel.left + mmi.ptMaxTrackSize.x;
		if (rcPanel.Width() < mmi.ptMinTrackSize.x) rcPanel.right = rcPanel.left + mmi.ptMinTrackSize.x;

		pLength[i].nSize = bHorizontal ? rcPanel.Width() : rcPanel.Height();
		pLength[i].nPos = bHorizontal ? rcPanel.left : rcPanel.top;
		pLength[i].nIndex = i;
		pLength[i].pPanel = pPanel;

		pLength[i].nHeight = bHorizontal ? min(rect.Height(), rcPanel.Height()) : min(rect.Width(), rcPanel.Width());
	}

	SortLength(pLength, 0, nCount - 1);

	for (i = 1; i < nCount; i++)
	{
		int nLengthIndex = arrSide[i]->m_nLengthIndex;
		int nLeft = pLength[nLengthIndex].StartPos();
		int nRight = pLength[nLengthIndex].EndPos();

		for (j = nLengthIndex - 1; j >= 0; j--)
		{
			if (pLength[j].EndPos() > nLeft)
			{
				if (pLength[j].nIndex < i)
				{
					pLength[j].nPos = nLeft - pLength[j].nSize;
					nLeft = pLength[j].StartPos();
				}
			}
			else break;
		}
		SortLength(pLength, j + 1, nLengthIndex - 1);

		for (j = nLengthIndex + 1; j < nCount; j++)
		{
			if (pLength[j].StartPos() < nRight)
			{
				if (pLength[j].nIndex < i)
				{
					pLength[j].nPos = nRight;
					nRight = pLength[j].EndPos();
				}
			}
			else break;
		}
		SortLength(pLength, nLengthIndex + 1, j - 1);
	}


	int nBegin = 0;
	int nEnd = bHorizontal ? rect.Width() : rect.Height();
	int nRight = nEnd;
	int nLeft = nBegin;

	for (i = nCount - 1; i >= 0; i--)
	{
		if (pLength[i].EndPos() > nRight)
		{
			pLength[i].nPos = nRight - pLength[i].nSize;
			nRight = pLength[i].StartPos();
		}
		else break;
	}
	for (i = 0; i < nCount; i++)
	{
		if (pLength[i].StartPos() < nLeft)
		{
			pLength[i].nPos = nLeft;
			nLeft = pLength[i].EndPos();
		}
		else break;
	}

	int nTotal = pLength[nCount - 1].EndPos();
	int cxExtra = nTotal - nEnd;
	if (cxExtra > 0)
	{
		nLeft = 0;

		for (i = 0; i < nCount; i++)
		{
			pLength[i].nPos = nLeft;

			int cxAddExtra = nTotal == 0 ? cxExtra / nCount : cxExtra * pLength[i].nSize / nTotal;

			nTotal -= pLength[i].nSize;
			pLength[i].nSize -= cxAddExtra;

			cxExtra -= cxAddExtra;
			nLeft = pLength[i].EndPos();
		}
	}

	for (i = 0; i < nCount; i++)
	{
		int nIndex = pLength[i].nIndex;
		CXTPDockingPaneSidePanel* pPanel = arrSide[nIndex];

		pPanel->MovePanel(&pLength[i], rect);
	}

	delete[] pLength;
}

void CXTPDockingPaneSidePanel::OnSizeParentEx(CXTPDockingPaneManager* pManager, CWnd* pParent, CRect rect)
{
	rect.DeflateRect(pManager->GetSideDockingMargin());

	CXTPDockingPaneBaseList& list = pManager->GetPaneStack();
	CSidePanelArray arrPanels[4];

	POSITION pos = list.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = list.GetNext(pos);
		if (pPane->GetType() == xtpPaneTypeSidePanel)
		{
			CXTPDockingPaneSidePanel* pPanel = (CXTPDockingPaneSidePanel*)pPane;

			if (!pPanel->IsEmpty())
			{
				arrPanels[pPanel->m_direction].Add(pPanel);
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (arrPanels[i].GetSize() > 0)
		{
			OnSizeParentEx(arrPanels[i], pParent, rect);
		}
	}
}

void CXTPDockingPaneSidePanel::_InsertPane(CXTPDockingPaneBase* pPane)
{
	ASSERT(m_lstPanes.IsEmpty());
	ASSERT(pPane->GetType() == xtpPaneTypeTabbedContainer);

	if (!m_hWnd && !m_pLayout->IsUserLayout() && pPane->GetPaneHwnd())
	{
		CreateContainer();
	}

	m_lstPanes.AddTail(pPane);

	pPane->SetParentContainer(this);
	pPane->SetDockingSite(this);

	pPane->OnParentContainerChanged(this);

	((CXTPDockingPaneTabbedContainer*)pPane)->ShowTitle(FALSE);

}

void CXTPDockingPaneSidePanel::Copy(CXTPDockingPaneBase* pCloneBase, CXTPPaneToPaneMap* pMap, DWORD /*dwIgnoredOptions*/)
{
	CXTPDockingPaneSidePanel* pClone = (CXTPDockingPaneSidePanel*)pCloneBase;

	ASSERT(pClone);
	if (!pClone)
		return;

	m_direction = pClone->m_direction;
	m_rcWindow = pClone->m_rcWindow;
	m_bCollapsed = pClone->m_bCollapsed;

	m_pDockingSite = GetDockingPaneManager()->GetSite();

	CXTPDockingPaneTabbedContainer* pContainer = pClone->GetTopContainer();
	if (pContainer)
	{
		_InsertPane(pContainer->Clone(m_pLayout, pMap));
	}
}

CString CXTPDockingPaneSidePanel::GetTitle() const
{
	CXTPDockingPaneTabbedContainer* pContainer = GetTopContainer();
	if (!pContainer)
		return _T("");

	return pContainer->GetTitle();
}

BEGIN_MESSAGE_MAP(CXTPDockingPaneSidePanel, CMiniFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_NCHITTEST_EX()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

void CXTPDockingPaneSidePanel::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint);

	GetPaintManager()->DrawSidePanel(&dc, this, rc);
}

void CXTPDockingPaneSidePanel::OnSize(UINT nType, int cx, int cy)
{
	CMiniFrameWnd::OnSize(nType, cx, cy);

	Invalidate(FALSE);
}

CXTPDockingPaneCaptionButton* CXTPDockingPaneSidePanel::HitTestCaptionButton(CPoint point) const
{
	for (int i = 0; i < m_pCaptionButtons->GetSize(); i++)
	{
		CXTPDockingPaneCaptionButton* pButton = m_pCaptionButtons->GetAt(i);
		if (pButton->PtInRect(point))
			return pButton->IsEnabled() ? pButton : NULL;
	}
	return NULL;
}

void CXTPDockingPaneSidePanel::OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	switch (pButton->GetID())
	{
		case XTP_IDS_DOCKINGPANE_CLOSE:
			{
				CXTPDockingPaneBaseList lstPanes;
				FindPane(xtpPaneTypeDockingPane, &lstPanes);

				POSITION pos = lstPanes.GetTailPosition();
				while (pos)
				{
					CXTPDockingPane* pPane = (CXTPDockingPane*)lstPanes.GetPrev(pos);

					if ((pPane->GetOptions() & xtpPaneNoCloseable) != 0)
						continue;

					pPane->InternalAddRef();

					if (!pManager->NotifyAction(xtpPaneActionClosing, pPane))
					{
						pPane->Close();
						pManager->NotifyAction(xtpPaneActionClosed, pPane);
					}

					pPane->InternalRelease();
				}
			}
			break;

		case XTP_IDS_DOCKINGPANE_AUTOHIDE:
			OnPinButtonClick();
			break;
	}
}

CXTPDockingPane* CXTPDockingPaneSidePanel::GetSelectedPane() const
{
	if (GetTopContainer())
		return GetTopContainer()->GetSelected();
	return NULL;
}

BOOL CXTPDockingPaneSidePanel::IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton)
{
	CXTPDockingPane* pSelectedPane = GetSelectedPane();

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
		return pSelectedPane && ((pSelectedPane->GetOptions() & xtpPaneNoCloseable) == 0);

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		pButton->SetState(m_bCollapsed ? xtpPanePinPushed : 0);
		return pSelectedPane && ((pSelectedPane->GetOptions() & xtpPaneNoHideable) == 0);
	}

	return TRUE;
}


void CXTPDockingPaneSidePanel::_RestoreFocus()
{
	CXTPDockingPaneTabbedContainer* pContainer = GetTopContainer();
	if (pContainer) pContainer->_RestoreFocus();
}

void CXTPDockingPaneSidePanel::OnCaptionLButtonDown(CPoint point)
{
	if (GetKeyState(VK_LBUTTON) < 0)
	{
		if (OnAction(xtpPaneActionDragging))
			return;

		CXTPDockingPaneContext* pContext = GetDockingPaneManager()->GetDockingContext();

		CRect rcWindow;
		GetWindowRect(rcWindow);

		pContext->Drag(GetTopContainer(), point, rcWindow);
	}
}

void CXTPDockingPaneSidePanel::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	if (m_bCollapsed && !m_bExpanded)
	{
		Expand();
		GetCursorPos(&point);
		ScreenToClient(&point);
		InvalidatePane(FALSE);
		UpdateWindow();
	}

	_RestoreFocus();

	CXTPDockingPaneCaptionButton* pButton = HitTestCaptionButton(point);

	if (pButton)
	{
		if (pButton->Click(this, point))
		{
			OnCaptionButtonClick(pButton);
		}

		return;
	}

	CRect rcCaption = GetPaintManager()->GetPaneCaptionRect(this);

	if (rcCaption.PtInRect(point))
	{
		ClientToScreen(&point);
		OnCaptionLButtonDown(point);
	}
}

void CXTPDockingPaneSidePanel::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	if (HitTestCaptionButton(point))
		return;

	CRect rcCaption = GetPaintManager()->GetPaneCaptionRect(this);

	if (rcCaption.PtInRect(point))
	{
		GetDockingPaneManager()->ToggleDocking(GetTopContainer());
	}
}


void CXTPDockingPaneSidePanel::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pCaptionButtons->CheckForMouseOver(point);

	if (m_bCollapsed && point != CPoint(-1, -1) && !m_bExpanded)
	{
		TRACKMOUSEEVENT tme =
		{
			sizeof(TRACKMOUSEEVENT), TME_HOVER, m_hWnd, CXTPDockingPaneAutoHideWnd::m_nMouseHoverDelay
		};
		_TrackMouseEvent(&tme);
	}

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CXTPDockingPaneSidePanel::OnMouseHover(WPARAM, LPARAM)
{
	if (m_bCollapsed)
	{
		if (!CXTPDrawHelpers::IsTopParentActive(m_hWnd))
			return 1;

		Expand();
	}

	return 1;
}

void CXTPDockingPaneSidePanel::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPDockingPaneSidePanel::OnPinButtonClick()
{
	if (!m_hWnd)
		return;

	BOOL bPinning = m_bCollapsed;

	if (OnAction(bPinning ? xtpPaneActionPinning : xtpPaneActionUnpinning))
		return;

	if (!m_bCollapsed)
	{
		Collapse(TRUE);
	}
	else
	{
		if (m_nStepsCount != m_nSlideStep)
		{
			SetWindowPos(0, 0, 0, m_rcWindow.Width(), m_rcWindow.Height(), SWP_NOZORDER | SWP_NOMOVE);
		}
		m_bCollapsed = FALSE;
		m_bExpanded = FALSE;

		KillTimer(TID_CHECKACTIVE);
		KillTimer(TID_SLIDEOUT);

	}
	InvalidatePane(FALSE);

	OnAction(bPinning ? xtpPaneActionPinned : xtpPaneActionUnpinned);
}

int CXTPDockingPaneSidePanel::GetMinHeight()
{
	CRect rc = GetPaintManager()->GetPaneCaptionRect(this);
	return 2 + (IsHorizontal() ? rc.Height() : rc.Width());
}

void CXTPDockingPaneSidePanel::DoSlideStep(BOOL bActivate)
{
	m_bInRecalcLayout = TRUE;

	int nMinHeight = GetMinHeight();
	BOOL bHorizontal = IsHorizontal();

	CXTPWindowRect rc(this);
	int nSize = max(nMinHeight, m_nSlideStep * ( bHorizontal ? m_rcWindow.Height() : m_rcWindow.Width()) / m_nStepsCount);

	switch (m_direction)
	{
		case xtpPaneDockLeft: rc.right = rc.left + nSize; break;
		case xtpPaneDockTop: rc.bottom = rc.top + nSize; break;
		case xtpPaneDockRight: rc.left = rc.right - nSize; break;
		case xtpPaneDockBottom: rc.top = rc.bottom - nSize; break;
	}

	GetParent()->ScreenToClient(rc);

	SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), (!bActivate ? SWP_NOZORDER | SWP_NOACTIVATE : SWP_NOACTIVATE));
	Invalidate(FALSE);

	CRect rcClient;
	GetClientRect(rcClient);

	GetPaintManager()->AdjustClientRect(this, rcClient);

	CXTPDockingPaneTabbedContainer* pContainer = GetTopContainer();
	if (pContainer)
	{
		pContainer->OnSizeParent(this, rcClient, NULL);
	}


	m_bInRecalcLayout = FALSE;
}


void CXTPDockingPaneSidePanel::Expand()
{
	if (m_bCollapsed)
	{
		if (m_bSlideOut)
		{
			m_bSlideOut = FALSE;
			m_bExpanded = FALSE;
			KillTimer(TID_SLIDEOUT);
			OnAction(xtpPaneActionCollapsed);
		}

		if (OnAction(xtpPaneActionExpanding))
			return;

		m_bExpanded = TRUE;
		m_nSlideStep = m_nStepsCount = ResetStepsCount();
		DoSlideStep(TRUE);

		m_nDeactivationCount = 8;
		SetTimer(TID_CHECKACTIVE, 100, NULL);

		OnAction(xtpPaneActionExpanded);
	}
}

void CXTPDockingPaneSidePanel::Collapse(BOOL bDelay)
{
	m_nSlideStep = m_nStepsCount = ResetStepsCount();

	m_bCollapsed = TRUE;
	m_bExpanded = TRUE;

	if (!bDelay)
	{
		if (!OnAction(xtpPaneActionCollapsing))
		{
			m_bExpanded = FALSE;
			m_nSlideStep = 0;
			DoSlideStep(FALSE);
			KillTimer(TID_SLIDEOUT);
			KillTimer(TID_CHECKACTIVE);
			OnAction(xtpPaneActionCollapsed);
			return;
		}
	}

	if (m_bSlideOut)
	{
		m_bSlideOut = FALSE;
		KillTimer(TID_SLIDEOUT);
		OnAction(xtpPaneActionCollapsed);
	}

	m_nDeactivationCount = 6;
	SetTimer(TID_CHECKACTIVE, 100, NULL);
}

void CXTPDockingPaneSidePanel::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_SLIDEOUT && m_bSlideOut)
	{
		m_nSlideStep--;

		if (m_nSlideStep > -1)
			DoSlideStep();
		else
		{
			m_bSlideOut = FALSE;
			m_bExpanded = FALSE;
			KillTimer(TID_SLIDEOUT);
			OnAction(xtpPaneActionCollapsed);
		}
	}

	if (nIDEvent == TID_CHECKACTIVE)
	{
		CPoint pt;
		GetCursorPos(&pt);

		CWnd* pFocus = GetFocus();
		BOOL bActive = (pFocus->GetSafeHwnd() && (pFocus == this || IsChild(pFocus)));

		if (!bActive && !m_bSlideOut && !CXTPWindowRect(this).PtInRect(pt) && ::GetCapture() == NULL)
		{
			if (--m_nDeactivationCount <= 0)
			{
				if (OnAction(xtpPaneActionCollapsing))
				{
					m_nDeactivationCount = 6;
					return;
				}

				m_bSlideOut = TRUE;
				SetTimer(TID_SLIDEOUT, CXTPDockingPaneAutoHideWnd::m_nAnimationInterval, NULL);

				KillTimer(TID_CHECKACTIVE);
			}
		}
	}

	CMiniFrameWnd::OnTimer(nIDEvent);
}

void CXTPDockingPaneSidePanel::GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo) const
{
	CXTPDockingPaneBase::GetMinMaxInfo(pMinMaxInfo);

	if (IsEmpty())
		return;

	GetTopContainer()->GetMinMaxInfo(pMinMaxInfo);

	CSize szBorder(6, 6);

	int nCaptionHeight = GetPaintManager()->GetCaptionHeight();
	if (IsHorizontal()) szBorder.cy += nCaptionHeight; else szBorder.cx += nCaptionHeight;

	pMinMaxInfo->ptMinTrackSize.x += szBorder.cx;
	pMinMaxInfo->ptMinTrackSize.y += szBorder.cy;

	pMinMaxInfo->ptMaxTrackSize.x += szBorder.cx;
	pMinMaxInfo->ptMaxTrackSize.y += szBorder.cy;

	pMinMaxInfo->ptMinTrackSize.x = max(pMinMaxInfo->ptMinTrackSize.x, nCaptionHeight * 2);
	pMinMaxInfo->ptMinTrackSize.y = max(pMinMaxInfo->ptMinTrackSize.y, nCaptionHeight * 2);

}

BOOL CXTPDockingPaneSidePanel::IsHorizontal() const
{
	return m_direction == xtpPaneDockTop || m_direction == xtpPaneDockBottom;
}

BOOL CXTPDockingPaneSidePanel::IsResizable(int nHit)
{
	if (IsHorizontal())
	{
		if (nHit == HTTOP) return m_direction == xtpPaneDockBottom;
		if (nHit == HTBOTTOM) return m_direction == xtpPaneDockTop;
		if (nHit == HTLEFT || nHit == HTRIGHT) return TRUE;
	}
	else
	{
		if (nHit == HTLEFT) return m_direction == xtpPaneDockRight;
		if (nHit == HTRIGHT) return m_direction == xtpPaneDockLeft;
		if (nHit == HTTOP || nHit == HTBOTTOM) return TRUE;
	}

	return FALSE;
}

void CXTPDockingPaneSidePanel::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
	{
		_RestoreFocus();
		Expand();
		SetCapture();
		m_pLayout->MoveToTail(this);

		CXTPWindowRect rcWindow(this);

		CRect rcClient(GetDockingPaneManager()->GetClientPane()->GetPaneWindowRect());
		rcClient.DeflateRect(GetDockingPaneManager()->GetSideDockingMargin());

		rcWindow.OffsetRect(-rcClient.TopLeft());


		MINMAXINFO mmi;
		GetMinMaxInfo(&mmi);

		BOOL bResizeTop = nHitTest == HTTOP || nHitTest == HTTOPLEFT || nHitTest == HTTOPRIGHT;
		BOOL bResizeBottom = nHitTest == HTBOTTOM || nHitTest == HTBOTTOMLEFT || nHitTest == HTBOTTOMRIGHT;
		BOOL bResizeRight = nHitTest == HTRIGHT || nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMRIGHT;
		BOOL bResizeLeft = nHitTest == HTLEFT || nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMLEFT;

		while (::GetCapture() == m_hWnd)
		{
			MSG msg;

			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			if (msg.message == WM_MOUSEMOVE)
			{
				CPoint pt = CPoint(msg.lParam);
				ClientToScreen(&pt);

				CPoint ptOffset = pt - point;
				point = pt;

				if (bResizeTop) rcWindow.top += ptOffset.y;
				if (bResizeBottom) rcWindow.bottom += ptOffset.y;
				if (bResizeRight) rcWindow.right += ptOffset.x;
				if (bResizeLeft) rcWindow.left += ptOffset.x;

				m_rcWindow = rcWindow;

				if (m_rcWindow.Height() > mmi.ptMaxTrackSize.y)
				{
					if (bResizeTop) m_rcWindow.top = m_rcWindow.bottom - mmi.ptMaxTrackSize.y; else m_rcWindow.bottom = m_rcWindow.top + mmi.ptMaxTrackSize.y;
				}
				if (m_rcWindow.Height() < mmi.ptMinTrackSize.y)
				{
					if (bResizeTop) m_rcWindow.top = m_rcWindow.bottom - mmi.ptMinTrackSize.y; else m_rcWindow.bottom = m_rcWindow.top + mmi.ptMinTrackSize.y;
				}

				if (m_rcWindow.Width() > mmi.ptMaxTrackSize.x)
				{
					if (bResizeLeft) m_rcWindow.left = m_rcWindow.right - mmi.ptMaxTrackSize.x; else m_rcWindow.right = m_rcWindow.left + mmi.ptMaxTrackSize.x;
				}
				if (m_rcWindow.Width() < mmi.ptMinTrackSize.x)
				{
					if (bResizeLeft) m_rcWindow.left = m_rcWindow.right - mmi.ptMinTrackSize.x; else m_rcWindow.right = m_rcWindow.left + mmi.ptMinTrackSize.x;
				}

				RecalcLayout(FALSE);
			}
			else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) break;
			else if (msg.message == WM_LBUTTONUP) break;
			else ::DispatchMessage(&msg);
		}

		if (CWnd::GetCapture() == this) ReleaseCapture();
	}
}

LRESULT CXTPDockingPaneSidePanel::OnNcHitTest(CPoint point)
{
	if (m_bCollapsed && !m_bExpanded)
		return HTCLIENT;

	CXTPWindowRect rcWindow(this);
	CRect rcBorders = rcWindow;
	rcBorders.DeflateRect(3, 3);

	if (rcWindow.PtInRect(point) && !rcBorders.PtInRect(point))
	{
		rcBorders.DeflateRect(8, 8);

		int ht = 0;

		if (point.y < rcBorders.top && IsResizable(HTTOP))
			ht = (HTTOP - HTSIZEFIRST + 1);
		else if (point.y >= rcBorders.bottom  && IsResizable(HTBOTTOM))
			ht = (HTBOTTOM - HTSIZEFIRST + 1);

		if (point.x < rcBorders.left && IsResizable(HTLEFT))
			ht += (HTLEFT - HTSIZEFIRST + 1);
		else if (point.x >= rcBorders.right  && IsResizable(HTRIGHT))
			ht += (HTRIGHT - HTSIZEFIRST + 1);

		return (LRESULT)(ht + HTSIZEFIRST - 1);
	}

	return HTCLIENT;
}

INT_PTR CXTPDockingPaneSidePanel::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{

	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	CXTPDockingPaneCaptionButton* pButton = HitTestCaptionButton(point);

	if (pButton)
	{
		nHit = (INT_PTR)pButton->GetID();
		CString strTip;
		XTPResourceManager()->LoadString(&strTip, (UINT)nHit);

		if (strTip.GetLength() == 0)
			return -1;

		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, pButton->GetRect(), nHit, strTip);

		return nHit;
	}

	return -1;
}

BOOL CXTPDockingPaneSidePanel::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	if (pManager)
	{
		pManager->GetToolTipContext()->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CMiniFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPDockingPaneSidePanel::OnRButtonUp(UINT /*nFlags*/, CPoint point)
{
	XTP_DOCKINGPANE_CLICK menu;
	menu.pPane = GetSelectedPane();
	menu.pContainer = this;
	ClientToScreen(&point);
	menu.pt = point;
	menu.rcExclude.SetRectEmpty();

	if (GetDockingPaneManager()->NotifyOwner(XTP_DPN_CONTEXTMENU, (LPARAM)&menu))
		return;
}

