// XTPDockingPaneAutoHidePanel.cpp : implementation of the CXTPDockingPaneAutoHidePanel class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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

#include "stdafx.h"
#include "Common/Resource.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPSystemHelpers.h"

#include <TabManager/Includes.h>

#include "XTPDockingPaneDefines.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneBaseContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneAutoHidePanel.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPanePaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TID_CHECKACTIVE 1
#define TID_CLOSE 4
#define TID_SLIDEIN 2
#define TID_SLIDEOUT 3

const int SPLITTER_GAP = 4;

double CXTPDockingPaneAutoHideWnd::m_dAnimationDelay = -1;
int CXTPDockingPaneAutoHideWnd::m_nAnimationInterval = 16;                 // smother animation
int CXTPDockingPaneAutoHideWnd::m_nAnimationDuration = 128;                // fast slide completion
BOOL CXTPDockingPaneAutoHidePanel::m_bCloseActiveWindow = TRUE;
DWORD CXTPDockingPaneAutoHideWnd::m_nMouseHoverDelay = HOVER_DEFAULT;
UINT CXTPDockingPaneAutoHideWnd::m_nInactiveCollapseDelay = 100;

void CXTPDockingPaneAutoHideWnd::GetMinMaxInfo (LPMINMAXINFO pMinMaxInfo, BOOL bIncludeSplitter /*= TRUE*/) const
{
	ZeroMemory(pMinMaxInfo, sizeof(MINMAXINFO));
	pMinMaxInfo->ptMaxTrackSize = CPoint(32000, 32000);

	if (!m_pPane || !m_pPane->m_pSelectedPane)
		return;

	m_pPane->m_pSelectedPane->GetMinMaxInfo(pMinMaxInfo);
	m_pPane->AdjustMinMaxInfoClientRect(pMinMaxInfo, TRUE);

	if (!bIncludeSplitter)
		return;

	if (m_direction == xtpPaneDockLeft || m_direction == xtpPaneDockRight)
	{
		pMinMaxInfo->ptMinTrackSize.x += SPLITTER_GAP;
		pMinMaxInfo->ptMaxTrackSize.x += SPLITTER_GAP;
	}
	else
	{
		pMinMaxInfo->ptMinTrackSize.y += SPLITTER_GAP;
		pMinMaxInfo->ptMaxTrackSize.y += SPLITTER_GAP;
	}
}

CXTPDockingPaneAutoHideWnd::CXTPDockingPaneAutoHideWnd(CXTPDockingPaneAutoHidePanel* pPanel, CXTPDockingPaneTabbedContainer* pPane)
	: m_pPanel(pPanel), m_pPane(pPane)
{
	ASSERT(pPane);
	if (!pPane)
		return;

	ASSERT(pPane->m_pSelectedPane);
	if (!pPane->m_pSelectedPane)
		return;

	m_bTracking = FALSE;

	m_direction = m_pPanel->GetDirection();
	m_bHoriz = (m_direction == xtpPaneDockTop || m_direction == xtpPaneDockBottom);
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	CWnd* pSite = pManager->GetSite();
	CXTPDockingPaneBase* pTopPane = pManager->GetTopPane();

	CRect rcClient = pTopPane->GetPaneWindowRect();

	MINMAXINFO mmInfo;
	GetMinMaxInfo(&mmInfo);

	CRect rcAvail, rc;
	BOOL bResizable = GetAvailableRect(rcAvail, rc);

	CSize szDocking(pPane->m_pSelectedPane->m_szDocking.cx + SPLITTER_GAP, pPane->m_pSelectedPane->m_szDocking.cy + SPLITTER_GAP);

	switch (pManager->GetRTLDirection(m_direction))
	{
	case xtpPaneDockLeft:
		szDocking.cx = min(szDocking.cx, rcAvail.right + SPLITTER_GAP - rcClient.left);
		break;

	case xtpPaneDockRight:
		szDocking.cx = min(szDocking.cx, rcClient.right - rcAvail.left);
		break;

	case xtpPaneDockTop:
		szDocking.cy = min(szDocking.cy, rcAvail.bottom + SPLITTER_GAP - rcClient.top);
		break;

	case xtpPaneDockBottom:
		szDocking.cy = min(szDocking.cy, rcClient.bottom - rcAvail.top);
		break;
	}

	szDocking.cx = max(mmInfo.ptMinTrackSize.x, min(mmInfo.ptMaxTrackSize.x, szDocking.cx));
	szDocking.cy = max(mmInfo.ptMinTrackSize.y, min(mmInfo.ptMaxTrackSize.y, szDocking.cy));

	switch (pManager->GetRTLDirection(m_direction))
	{
		case xtpPaneDockLeft:
			rcClient.right = rcClient.left + szDocking.cx;
			break;

		case xtpPaneDockRight:
			rcClient.left = rcClient.right - szDocking.cx;
			break;

		case xtpPaneDockTop:
			rcClient.bottom = rcClient.top + szDocking.cy;
			break;

		case xtpPaneDockBottom:
			rcClient.top = rcClient.bottom - szDocking.cy;
			break;
	}

	pSite->ScreenToClient(&rcClient);

	HCURSOR hCursor = !bResizable || pManager->IsSplittersLocked() ? AfxGetApp()->LoadStandardCursor(IDC_ARROW) :
		XTPResourceManager()->LoadCursor(!m_bHoriz ? XTP_IDC_HSPLITBAR :  XTP_IDC_VSPLITBAR);
	m_rcWindow = rcClient;

	CreateEx(pSite->GetExStyle() & WS_EX_LAYOUTRTL, AfxRegisterWndClass(0, hCursor), _T(""),
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rcClient, pSite, 0);


	m_nSlideStep = 0;
	m_nStepsCount = 1;

	if (m_dAnimationDelay != .0 && m_nAnimationInterval != 0)
	{
		if (m_dAnimationDelay > 0)
		{
			// keep the animation delay behaviour relative to 50 ms frame interval as per original
			// implementation
			m_nStepsCount = int((m_bHoriz ? m_rcWindow.Width() : m_rcWindow.Height()) * m_dAnimationDelay * 50 / m_nAnimationInterval);
		}
		else
		{
			m_nStepsCount = m_nAnimationDuration/m_nAnimationInterval;
		}
	}
	if (m_nStepsCount < 1) m_nStepsCount = 1;


	m_bSlideOut = FALSE;
	if (m_nStepsCount > 1)
		SetTimer(TID_SLIDEIN, m_nAnimationInterval, NULL);
	DoSlideStep();
	m_pPane->ShowWindow(SW_SHOW);

	SetTimer(TID_CHECKACTIVE, m_nInactiveCollapseDelay, NULL);

	m_nDeactivationCount = 6;

}

CXTPDockingPaneManager* CXTPDockingPaneAutoHideWnd::GetDockingPaneManager() const
{
	ASSERT(m_pPanel);
	return m_pPanel ? m_pPanel->GetDockingPaneManager() : NULL;
}


void CXTPDockingPaneAutoHideWnd::DoSlideStep()
{
	CRect rc(m_rcWindow);

	switch (m_direction)
	{
		case xtpPaneDockLeft:
			rc.right = rc.left + (m_nSlideStep + 1) * m_rcWindow.Width() / m_nStepsCount;
			break;

		case xtpPaneDockRight:
			rc.left = rc.right - (m_nSlideStep + 1) * m_rcWindow.Width() / m_nStepsCount;
			break;

		case xtpPaneDockTop:
			rc.bottom = rc.top + (m_nSlideStep + 1) * m_rcWindow.Height() / m_nStepsCount;
			break;

		case xtpPaneDockBottom:
			rc.top = rc.bottom - (m_nSlideStep + 1) * m_rcWindow.Height() / m_nStepsCount;
			break;
	}
	SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
	Invalidate(FALSE);
}

void CXTPDockingPaneAutoHideWnd::RecalcLayout(BOOL /*bNotify*/)
{
	if (!m_pPane || !m_pPane->GetSafeHwnd())
		return;

	AFX_SIZEPARENTPARAMS layout;
	layout.bStretch = TRUE;
	GetClientRect(&layout.rect);
	CRect rcClient(layout.rect);
	layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess

	CRect rcPane(CPoint(0, 0), m_rcWindow.Size());

	switch (m_direction)
	{
		case xtpPaneDockLeft:
			rcPane.OffsetRect(- m_rcWindow.Width() + rcClient.Width(), 0);
			rcPane.right -= SPLITTER_GAP;
			break;

		case xtpPaneDockTop:
			rcPane.OffsetRect(0, - m_rcWindow.Height() + rcClient.Height());
			rcPane.bottom -= SPLITTER_GAP;
			break;

		case xtpPaneDockRight:
			rcPane.left += SPLITTER_GAP;
			break;

		case xtpPaneDockBottom:
			rcPane.top += SPLITTER_GAP;
			break;
	}

	m_pPane->OnSizeParent(this, rcPane, &layout);

	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
	{
		TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
	}
}

IMPLEMENT_DYNAMIC(CXTPDockingPaneAutoHideWnd, CMiniFrameWnd)

BEGIN_MESSAGE_MAP(CXTPDockingPaneAutoHideWnd, CMiniFrameWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CXTPDockingPaneAutoHideWnd::CloseWindow()
{
	if (!IsWindow(m_hWnd))
		return;

	ShowWindow(SW_HIDE);

	if (m_pPane && m_pPane->GetSafeHwnd() && m_pPane->GetParent() == this)
	{
		m_pPane->ShowWindow(SW_HIDE);
		m_pPane->SetDockingSite(GetDockingPaneManager()->GetSite());
	}
	m_pPane = 0;

	if (m_pPanel && m_pPanel->m_pActiveWnd == this)
		m_pPanel->m_pActiveWnd = 0;

	PostMessage(WM_CLOSE);
}

void CXTPDockingPaneAutoHideWnd::PostNcDestroy()
{
	if (m_pPanel && m_pPanel->m_pActiveWnd == this)
		m_pPanel->m_pActiveWnd = 0;

	if (m_dwRef <= 1)
	{
		delete this;
	}
	else
	{
		InternalRelease();
	}
}

void CXTPDockingPaneAutoHideWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_CLOSE)
	{
		CloseWindow();
		return;
	}

	if (!m_pPane)
		return;

	if (m_pPanel && m_pPanel->m_pActiveWnd != this)
		return;

	if (nIDEvent == TID_CHECKACTIVE)
	{
		if (m_bTracking)
			return;

		CPoint pt;
		GetCursorPos(&pt);
		if (!m_pPane->IsActive() && !CXTPWindowRect(this).PtInRect(pt) && !CXTPWindowRect(m_pPanel).PtInRect(pt))
		{
			if (!m_bSlideOut && CXTPDrawHelpers::IsTopParentActive(m_hWnd))
			{
				if (--m_nDeactivationCount <= 0)
				{
					m_nDeactivationCount = 0;

					InternalAddRef();

					if (!CXTPDockingPaneAutoHidePanel::m_bCloseActiveWindow || GetDockingPaneManager()->NotifyAction(xtpPaneActionCollapsing, m_pPane->GetSelected()))
					{
						m_nDeactivationCount = 6;
					}
					else
					{
						m_bSlideOut = TRUE;
						if (m_hWnd) SetTimer(TID_SLIDEOUT, m_nAnimationInterval, NULL);
					}

					InternalRelease();
				}
			}
		}
		else
		{
			m_nDeactivationCount = 6;
			if (m_bSlideOut)
			{
				m_bSlideOut = FALSE;
				KillTimer(TID_SLIDEOUT);
				SetTimer(TID_SLIDEIN, m_nAnimationInterval, NULL);
			}
		}
		return;
	}

	if (nIDEvent == TID_SLIDEOUT && m_bSlideOut)
	{
		m_nSlideStep--;
		if (m_nSlideStep > -1) DoSlideStep();
		else
		{
			m_bSlideOut = FALSE;
			KillTimer(TID_SLIDEOUT);

			InternalAddRef();

			GetDockingPaneManager()->NotifyAction(xtpPaneActionCollapsed, m_pPane->GetSelected());
			CloseWindow();

			InternalRelease();
		}
		return;
	}

	if (nIDEvent == TID_SLIDEIN && !m_bSlideOut)
	{
		m_nSlideStep++;
		if (m_nSlideStep < m_nStepsCount) DoSlideStep();
		else
		{
			m_nSlideStep = m_nStepsCount - 1;
			KillTimer(TID_SLIDEIN);

			GetDockingPaneManager()->NotifyAction(xtpPaneActionExpanded, m_pPane->GetSelected());
		}
		return;
	}
}


void CXTPDockingPaneAutoHideWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bSlideOut)
	{
		OnTimer(TID_CHECKACTIVE);
		return;
	}

	CMiniFrameWnd::OnMouseMove(nFlags, point);
}


void CXTPDockingPaneAutoHideWnd::OnClose()
{
	CMiniFrameWnd::OnClose();
}

BOOL CXTPDockingPaneAutoHideWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPDockingPaneAutoHideWnd::OnPaint()
{
	CXTPDockingPanePaintManager* pPaintManager = NULL;

	if (m_pPane)
	{
		pPaintManager = m_pPane->CXTPDockingPaneBase::GetPaintManager();
	}

	if (pPaintManager)
	{
		CPaintDC dc(this);
		CXTPClientRect rc(this);
		pPaintManager->DrawAutoHidePanel(&dc, rc, m_direction);
	}
	else
	{
		Default();
	}
}

void CXTPDockingPaneAutoHideWnd::Reposition(CRect rc, CRect rcClient)
{
	CWnd* pSite = GetDockingPaneManager()->GetSite();

	switch (GetDockingPaneManager()->GetRTLDirection(m_direction))
	{
	case xtpPaneDockLeft:  rc.left = rcClient.left; break;
	case xtpPaneDockRight: rc.right = rcClient.right; break;
	case xtpPaneDockTop: rc.top = rcClient.top; break;
	case xtpPaneDockBottom: rc.bottom = rcClient.bottom; break;
	}
	if (!m_bHoriz) m_pPane->m_pSelectedPane->m_szDocking.cx = rc.Width();
	else m_pPane->m_pSelectedPane->m_szDocking.cy = rc.Height();

	pSite->ScreenToClient(&rc);
	m_rcWindow = rc;
	MoveWindow(m_rcWindow);
	Invalidate(FALSE);
}

BOOL CXTPDockingPaneAutoHideWnd::GetAvailableRect(CRect& rcAvail, CRect& rc) const
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();
	if (!pManager)
		return FALSE;

	CRect rcClient = pManager->GetTopPane()->GetPaneWindowRect();

	int nGap = pManager->m_nSplitterGap + SPLITTER_GAP;

	rcAvail = rcClient;

	MINMAXINFO mmInfo;
	GetMinMaxInfo(&mmInfo, m_direction == xtpPaneDockRight || m_direction == xtpPaneDockBottom);

	switch (m_direction)
	{
		case xtpPaneDockLeft:
			rcAvail.DeflateRect(max(mmInfo.ptMinTrackSize.x, nGap), 0, nGap, 0);

			if (rcAvail.right - rcClient.left > mmInfo.ptMaxTrackSize.x)
				rcAvail.right = mmInfo.ptMaxTrackSize.x + rcClient.left;

			if (rcAvail.left >= rcAvail.right)
				return FALSE;

			rc.left = rc.right - SPLITTER_GAP;
			break;

		case xtpPaneDockRight:
			rcAvail.DeflateRect(nGap, 0, max(mmInfo.ptMinTrackSize.x, nGap), 0);

			if (rcClient.right - rcAvail.left > mmInfo.ptMaxTrackSize.x)
				rcAvail.left = rcClient.right - mmInfo.ptMaxTrackSize.x;

			if (rcAvail.left >= rcAvail.right)
				return FALSE;

			rc.right = rc.left + SPLITTER_GAP;
			break;

		case xtpPaneDockTop:
			rcAvail.DeflateRect(0, max(mmInfo.ptMinTrackSize.y, nGap), 0, nGap);

			if (rcAvail.bottom - rcClient.top > mmInfo.ptMaxTrackSize.y)
				rcAvail.bottom = mmInfo.ptMaxTrackSize.y + rcClient.top;

			if (rcAvail.top >= rcAvail.bottom)
				return FALSE;

			rc.top = rc.bottom - SPLITTER_GAP;
			break;

		case xtpPaneDockBottom:
			rcAvail.DeflateRect(0, nGap, 0, max(mmInfo.ptMinTrackSize.y, nGap));

			if (rcClient.bottom - rcAvail.top > mmInfo.ptMaxTrackSize.y)
				rcAvail.top = rcClient.bottom - mmInfo.ptMaxTrackSize.y;

			if (rcAvail.top >= rcAvail.bottom)
				return FALSE;

			rc.bottom = rc.top + SPLITTER_GAP;
			break;
	}
	return TRUE;
}

void CXTPDockingPaneAutoHideWnd::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	if (!pManager || pManager->IsSplittersLocked())
		return;

	if (!m_pPane)
		return;

	CXTPWindowRect rc(this);
	CRect rcClient = pManager->GetTopPane()->GetPaneWindowRect();

	CRect rcAvail;
	if (!GetAvailableRect(rcAvail, rc))
		return;

	if (m_pPane->OnAction(xtpPaneActionSplitterResizing))
		return;

	m_bTracking = TRUE;

	if (pManager->IsSplitterTrackerUsed())
	{
		if (GetExStyle() & WS_EX_LAYOUTRTL)
			point = rc.TopLeft();
		else ClientToScreen(&point);


		CXTPSplitterTracker tracker;

		if (tracker.Track(this, rcAvail, rc, point, !m_bHoriz))
		{
			Reposition(rc, rcClient);
		}
	}
	else
	{
		SetCapture();

		while (CWnd::GetCapture() == this)
		{
			MSG msg;

			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			if (msg.message == WM_MOUSEMOVE)
			{
				point = CPoint(msg.lParam);
				ClientToScreen(&point);

				point.x = max(min(point.x, rcAvail.right), rcAvail.left);
				point.y = max(min(point.y, rcAvail.bottom), rcAvail.top);

				if (!m_bHoriz)
				{
					if (rc.left == point.x)
						continue;
					rc.OffsetRect(point.x - rc.left, 0);
				}
				else
				{
					if (rc.top == point.y)
						continue;
					rc.OffsetRect(0, point.y - rc.top);
				}

				Reposition(rc, rcClient);
			}
			else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) break;
			else if (msg.message == WM_LBUTTONUP) break;
			else ::DispatchMessage(&msg);
		}

		if (CWnd::GetCapture() == this) ReleaseCapture();
	}

	m_bTracking = FALSE;

	m_pPane->OnAction(xtpPaneActionSplitterResized);
}


//////////////////////////////////////////////////////////////////////////
// CPanelDropTarget

class CXTPDockingPaneAutoHidePanel::CPanelDropTarget : public COleDropTarget
{
public:
	CPanelDropTarget()
	{
		m_ptDragLastPoint = CPoint(-1, -1);
		m_dwDragLastTick = 0;
	}

	void OnDragLeave(CWnd* /*pWnd*/)
	{
		m_dwDragLastTick = 0;
		m_ptDragLastPoint = CPoint(-1, -1);
	}
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint point)
	{
		CXTPDockingPaneAutoHidePanel* pPanel = (CXTPDockingPaneAutoHidePanel*)pWnd;
		ASSERT_VALID(pPanel);

		if (!pPanel->GetPaintManager()->m_bSelectOnDragOver)
			return DROPEFFECT_NONE;

		if (m_dwDragLastTick != (DWORD)-1 && pPanel->GetPaintManager()->m_bSelectOnDragOver == 2)
		{
			DWORD dwTick = GetTickCount();

			if (point != m_ptDragLastPoint)
			{
				m_dwDragLastTick = dwTick;
				m_ptDragLastPoint = point;
			}

			if (dwTick - m_dwDragLastTick > CXTPTabPaintManager::m_nSelectOnDragOverDelay)
			{
				m_dwDragLastTick = (DWORD)-1;
			}
		}
		else
		{
			CXTPDockingPane* pPane = pPanel->HitTest(point);
			if (pPane)
			{
				if (!(pPanel->m_pActiveWnd && pPanel->m_pActiveWnd->GetPane()->GetSelected() == pPane))
				{
					pPanel->ShowPane(pPane, FALSE);
				}
			}
			else
			{
				m_dwDragLastTick = 0;
			}
		}

		return DROPEFFECT_NONE;
	}

protected:
	DWORD m_dwDragHoverMode;
	DWORD m_dwDragLastTick;
	CPoint m_ptDragLastPoint;
};

//////////////////////////////////////////////////////////////////////////
// CAutoHidePanelTabManager

class CXTPDockingPaneAutoHidePanel::CAutoHidePanelTabManager : public CXTPTabManager
{
public:
	CAutoHidePanelTabManager()
	{
		FindNavigateButton(xtpTabNavigateButtonLeft)->SetFlags(xtpTabNavigateButtonNone);
		FindNavigateButton(xtpTabNavigateButtonRight)->SetFlags(xtpTabNavigateButtonNone);
	}

	virtual void Reposition()
	{

	}
	virtual BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) const
	{
		return GetPanel()->DrawIcon(pDC, pt, pItem, bDraw, szIcon);
	}

	virtual void RedrawControl(LPCRECT lpRect, BOOL /*bAnimate*/)
	{
		if (GetPanel()->m_hWnd) GetPanel()->InvalidateRect(lpRect, FALSE);
	}

	virtual CXTPTabPaintManager* GetPaintManager() const
	{
		return GetPanel()->GetPaintManager();
	}

	CXTPTabManagerItem* Add()
	{
		return AddItem(GetItemCount());
	}

	XTPTabPosition GetPosition() const
	{
		return GetPanel()->GetPosition();
	}

	void PerformMouseMove(HWND hwnd, CPoint pt)
	{
		CXTPTabManager::PerformMouseMove(hwnd, pt);
	}

	CXTPDockingPaneAutoHidePanel* GetPanel() const;

public:
	CAutoHidePanelTabManagersArray* m_pArray;
	CRect m_rcManager;
};

class CXTPDockingPaneAutoHidePanel::CAutoHidePanelTabManagersArray : public CArray<CAutoHidePanelTabManager*, CAutoHidePanelTabManager*>
{
public:

	~CAutoHidePanelTabManagersArray()
	{
		FreeAll();
	}

	void FreeAll()
	{
		for (int i = 0; i < GetSize(); i++)
		{
			delete GetAt(i);
		}
		RemoveAll();
	}
	CAutoHidePanelTabManager* AddManager()
	{
		CAutoHidePanelTabManager* pManager = new CAutoHidePanelTabManager();
		Add(pManager);
		pManager->m_pArray = this;
		return pManager;
	}

	void Draw(CDC* pDC)
	{
		CRect rcClipBox;
		pDC->GetClipBox(rcClipBox);

		CXTPTabPaintManager* pPaintManager = m_pAutoHidePanel->GetPaintManager();
		for (int i = 0; i < GetSize(); i++)
		{
			CAutoHidePanelTabManager* pManager = GetAt(i);

			if (CRect().IntersectRect(rcClipBox, pManager->m_rcManager))
				pPaintManager->DrawTabControl(pManager, pDC, pManager->m_rcManager);
		}
	}



public:
	CXTPDockingPaneAutoHidePanel* m_pAutoHidePanel;
};



CXTPDockingPaneAutoHidePanel* CXTPDockingPaneAutoHidePanel::CAutoHidePanelTabManager::GetPanel() const
{
	return m_pArray->m_pAutoHidePanel;
}






//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneAutoHidePanel


CXTPDockingPaneAutoHidePanel::CXTPDockingPaneAutoHidePanel(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBaseContainer(xtpPaneTypeAutoHidePanel, pLayout)
{
	m_direction = xtpPaneDockLeft;
	m_pActiveWnd = 0;
	m_pTabManagers = new CAutoHidePanelTabManagersArray();
	m_pTabManagers->m_pAutoHidePanel = this;

	m_pDropTarget = new CPanelDropTarget();
	m_rcFrameRect.SetRectEmpty();

	FindNavigateButton(xtpTabNavigateButtonLeft)->SetFlags(xtpTabNavigateButtonNone);
	FindNavigateButton(xtpTabNavigateButtonRight)->SetFlags(xtpTabNavigateButtonNone);


#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPDockingPaneAutoHidePanel::~CXTPDockingPaneAutoHidePanel()
{
	if (m_pActiveWnd)
	{
		m_pActiveWnd->m_pPane = 0;
		m_pActiveWnd->m_pPanel = 0;
		CloseActiveWindow();
	}

	if (m_hWnd)
		DestroyWindow();

	delete m_pTabManagers;
	delete m_pDropTarget;
}

CXTPTabPaintManager* CXTPDockingPaneAutoHidePanel::GetPaintManager() const
{
	return CXTPDockingPaneBaseContainer::GetPaintManager()->GetPanelPaintManager();
}

void CXTPDockingPaneAutoHidePanel::Reposition()
{
	GetDockingPaneManager()->RecalcFrameLayout(this);
}

BOOL CXTPDockingPaneAutoHidePanel::DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) const
{
	if (!pItem)
		return TRUE;

	if (GetPaintManager()->m_bShowIcons == FALSE)
		return FALSE;

	CXTPImageManagerIcon* pImage = ((CXTPDockingPane*)pItem->GetData())->GetIcon(szIcon.cx);

	if (!pImage)
		return FALSE;

	if (!bDraw)
	{
		return TRUE;
	}

	pItem->DrawImage(pDC, CRect(pt, szIcon), pImage);

	return TRUE;

}

void CXTPDockingPaneAutoHidePanel::RedrawControl(LPCRECT lpRect, BOOL /*bAnimate*/)
{
	if (m_hWnd) InvalidateRect(lpRect, FALSE);
}

int CXTPDockingPaneAutoHidePanel::GetPanelHeight() const
{
	CXTPTabPaintManagerTheme* pAppearanceSet = GetPaintManager()->GetAppearanceSet();

	return pAppearanceSet->GetButtonHeight(NULL) +
		pAppearanceSet->m_rcHeaderMargin.bottom + pAppearanceSet->m_rcHeaderMargin.top;
}

int CXTPDockingPaneAutoHidePanel::GetItemsLength() const
{
	return m_nItemsLength;
}

void CXTPDockingPaneAutoHidePanel::RefreshManagersArray(CDC* pDC, CRect rcClient)
{
	BOOL bVertical = m_direction == xtpPaneDockLeft || m_direction == xtpPaneDockRight;
	BOOL bIndent = bVertical && (m_pLayout->m_wndPanels[xtpPaneDockTop] != NULL && !m_pLayout->m_wndPanels[xtpPaneDockTop]->IsEmpty());

	int nPanelIndent = bIndent ? GetPanelHeight() : 0;

	rcClient.top += nPanelIndent;

	CXTPDockingPaneManager* pPaneManager = GetDockingPaneManager();

	if (bVertical)
	{
		rcClient.top += pPaneManager->GetClientMargin();
	}
	else
	{
		rcClient.left += pPaneManager->GetClientMargin();
	}

	CRect rcWindow(rcClient);

	m_pTabManagers->FreeAll();

	CRect rcMargins = GetPaintManager()->GetAppearanceSet()->GetHeaderMargin();

	int nMarginIndent = rcMargins.left + rcMargins.right + 8;

	POSITION pos = GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)GetNext(pos);
		if (pContainer->IsEmpty())
			continue;

		CAutoHidePanelTabManager* pManager = m_pTabManagers->AddManager();

		POSITION posPane = pContainer->GetHeadPosition();
		while (posPane)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)pContainer->GetNext(posPane);

			CXTPTabManagerItem* pItem = pManager->Add();

			pItem->SetData((DWORD_PTR)pPane);
			if (pContainer->GetSelected() == pPane && pPaneManager->GetPaintManager()->m_bAutoHidePanelHighlightSelected) pManager->SetSelectedItem(pItem);

			pItem->SetCaption(pPane->GetTabCaption());
			pItem->SetColor(pPane->GetItemColor());
			pItem->SetEnabled(pPane->GetEnabled() & xtpPaneEnableClient);
		}
		pManager->GetPaintManager()->RepositionTabControl(pManager, pDC, rcClient);
		pManager->m_rcManager = rcClient;

		if (bVertical)
			rcClient.top += pManager->GetItemsLength() + nMarginIndent;
		else
			rcClient.left += pManager->GetItemsLength() + nMarginIndent;
	}

	if (pPaneManager->m_bShowPanelScrollButtons)
	{
		m_nItemsLength = (!bVertical ? rcClient.left - rcWindow.left : rcClient.top - rcWindow.top) - nMarginIndent;

		CXTPTabManagerNavigateButton* pButtonLeft = FindNavigateButton(xtpTabNavigateButtonLeft);
		CXTPTabManagerNavigateButton* pButtonRight = FindNavigateButton(xtpTabNavigateButtonRight);

		BOOL bAddButton = !bVertical ? rcClient.left > rcClient.right : rcClient.top > rcClient.bottom;

		if (bAddButton)
		{
			pButtonLeft->SetFlags(xtpTabNavigateButtonAlways);
			pButtonRight->SetFlags(xtpTabNavigateButtonAlways);

			GetPaintManager()->RepositionNavigateButtons(this, rcWindow);
		}
		else
		{
			pButtonLeft->SetFlags(xtpTabNavigateButtonNone);
			pButtonRight->SetFlags(xtpTabNavigateButtonNone);
			pButtonLeft->SetRect(CRect(0, 0, 0, 0));
			pButtonRight->SetRect(CRect(0, 0, 0, 0));
			m_nHeaderOffset = 0;
		}

		rcClient = rcWindow;

		if (bAddButton)
		{
			if (m_nHeaderOffset < 0)
			{
				int nWindowEnd = bVertical ? rcWindow.bottom : rcWindow.right;

				if (m_nItemsLength + m_nHeaderOffset < nWindowEnd - 24 - nMarginIndent - nPanelIndent)
				{
					m_nHeaderOffset = min(0, nWindowEnd - 24 - nMarginIndent - m_nItemsLength - nPanelIndent);
				}
			}

			if (bVertical) rcClient.top += m_nHeaderOffset; else rcClient.left += m_nHeaderOffset;

			for (int i = 0; i < m_pTabManagers->GetSize(); i++)
			{
				CAutoHidePanelTabManager* pManager = m_pTabManagers->GetAt(i);

				if (bVertical)
					rcClient.bottom = rcClient.top + pManager->GetItemsLength() + nMarginIndent;
				else
					rcClient.right = rcClient.left + pManager->GetItemsLength() + nMarginIndent;

				pManager->GetPaintManager()->RepositionTabControl(pManager, pDC, rcClient);
				pManager->m_rcManager = rcClient;

				if (bVertical)
					rcClient.top = rcClient.bottom;
				else
					rcClient.left = rcClient.right;
			}
		}
	}
}

XTPTabPosition CXTPDockingPaneAutoHidePanel::GetPosition() const
{
	switch (GetDirection())
	{
		case xtpPaneDockLeft: return xtpTabPositionRight;
		case xtpPaneDockRight: return xtpTabPositionLeft;
		case xtpPaneDockTop: return xtpTabPositionBottom;
		case xtpPaneDockBottom: return xtpTabPositionTop;
	}
	return xtpTabPositionTop;
}

void CXTPDockingPaneAutoHidePanel::_InsertPane(CXTPDockingPaneBase* pPane)
{
	m_lstPanes.AddTail(pPane);

	pPane->SetParentContainer(this);
	pPane->SetDockingSite(m_pDockingSite);

	GetDockingPaneManager()->RecalcFrameLayout(this, TRUE);

	pPane->OnParentContainerChanged(this);

	if (pPane->GetType() == xtpPaneTypeTabbedContainer)
	{
		CWnd* pFocus = GetFocus();
		if (((CXTPDockingPaneTabbedContainer*)pPane)->GetSafeHwnd() && pFocus->GetSafeHwnd() && (pFocus == ((CXTPDockingPaneTabbedContainer*)pPane) || ((CXTPDockingPaneTabbedContainer*)pPane)->IsChild(pFocus)))
			GetDockingPaneManager()->GetSite()->SetFocus();
	}

	if (m_hWnd) Invalidate(FALSE);
}

void CXTPDockingPaneAutoHidePanel::RemovePane(CXTPDockingPaneBase* pPane)
{
	POSITION pos = m_lstPanes.Find(pPane);
	ASSERT(pos);

	m_lstPanes.RemoveAt(pos);
	if (m_hWnd) Invalidate(FALSE);
	GetDockingPaneManager()->RecalcFrameLayout(this, TRUE);

	if (m_lstPanes.IsEmpty())
	{
		if (m_hWnd) DestroyWindow();
	}

	pPane->m_pParentContainer = NULL;
}

void CXTPDockingPaneAutoHidePanel::Create(CWnd* pParent)
{
	CWnd::Create(_T("XTPDockingPaneAutoHidePanel"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CXTPEmptyRect(), pParent, 0);
	m_pDropTarget->Register(this);
}

void CXTPDockingPaneAutoHidePanel::OnSizeParentEx(CWnd* pParent, CRect& rect, LPVOID lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	if (IsEmpty())
	{
		if (m_hWnd) DestroyWindow();
		return;
	}

	if (!m_hWnd)
		Create(pParent);

	if (m_rcFrameRect != rect)
	{
		CloseActiveWindow();
		m_rcFrameRect = rect;
	}

	int nHeight = GetPanelHeight();

	CRect rcClient(rect);
	switch (m_direction)
	{
		case xtpPaneDockLeft:
			rcClient.right = rect.left = rect.left + nHeight;
			break;

		case xtpPaneDockRight:
			rcClient.left = rect.right = rect.right - nHeight;
			break;

		case xtpPaneDockTop:
			rcClient.bottom = rect.top = rect.top + nHeight;
			break;

		case xtpPaneDockBottom:
			rcClient.top = rect.bottom = rect.bottom - nHeight;
			break;
	}

	CClientDC dc(this);
	CRect rcPanel(rcClient);
	rcPanel.OffsetRect(-rcPanel.TopLeft());
	RefreshManagersArray(&dc, rcPanel);

	if (lpLayout->hDWP != NULL)
	{
		MoveWindow(rcClient);
		Invalidate(FALSE);
	}
}


BEGIN_MESSAGE_MAP(CXTPDockingPaneAutoHidePanel, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

LRESULT CXTPDockingPaneAutoHidePanel::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		OnDraw(pDC);
	}

	return TRUE;
}

void CXTPDockingPaneAutoHidePanel::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint);
	OnDraw(&dc);
}

void CXTPDockingPaneAutoHidePanel::OnDraw(CDC* pDC)
{
	CXTPClientRect rc(this);

	CXTPTabPaintManager* pPaintManager = GetPaintManager();

	CXTPPaintManagerColorGradient& clr = pPaintManager->GetColorSet()->m_clrAutoHideFace;

	if (clr.clrDark == clr.clrLight)
	{
		pDC->FillSolidRect(rc, clr.clrDark);
	}
	else
	{
		CRect rcFill(rc);

		if (m_direction == xtpPaneDockRight)
		{
			rcFill.left -= CXTPClientRect(GetParent()).Width() - rcFill.Width();
		}

		rcFill.right = rcFill.left + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2);
		XTPDrawHelpers()->GradientFill(pDC, rcFill, clr.clrLight, clr.clrDark, TRUE, rc);
	}

	if (FindNavigateButton(xtpTabNavigateButtonLeft)->GetFlags() == xtpTabNavigateButtonAlways)
	{
		CRect rcButton(FindNavigateButton(xtpTabNavigateButtonLeft)->GetRect());
		pPaintManager->DrawTabControl(this, pDC, rc);

		if (IsHorizontalPosition())
			pDC->ExcludeClipRect(rcButton.left, 0, rc.right, rc.bottom);
		else
			pDC->ExcludeClipRect(0, rcButton.top, rc.right, rc.bottom);
		pPaintManager->m_bClipHeader = FALSE;
	}

	m_pTabManagers->Draw(pDC);
}

CXTPTabManagerItem* CXTPDockingPaneAutoHidePanel::GetPaneTab(CXTPDockingPane* pPane) const
{
	for (int i = 0; i < m_pTabManagers->GetSize(); i++)
	{
		CAutoHidePanelTabManager* pTabManager = m_pTabManagers->GetAt(i);

		for (int j = 0; j < pTabManager->GetItemCount(); j++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(j);

			if ((CXTPDockingPane*)pItem->GetData() == pPane)
			{
				return pItem;
			}
		}
	}
	return NULL;
}

CXTPDockingPane* CXTPDockingPaneAutoHidePanel::HitTest(CPoint point) const
{
	for (int i = 0; i < m_pTabManagers->GetSize(); i++)
	{
		CXTPTabManagerItem* pItem = m_pTabManagers->GetAt(i)->HitTest(point);
		if (pItem)
		{
			return (CXTPDockingPane*)pItem->GetData();
		}

	}
	return NULL;
}

void CXTPDockingPaneAutoHidePanel::CloseActiveWindows()
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_CALLPTR(m_pLayout->m_wndPanels[i], CloseActiveWindow());
	}
}

void CXTPDockingPaneAutoHidePanel::CloseActiveWindow(BOOL bDelayed)
{
	if (!m_bCloseActiveWindow)
		return;

	if (m_pActiveWnd)
	{
		if (m_pActiveWnd->m_pPane && m_pActiveWnd->m_pPane->GetSelected() != NULL)
		{
			if (GetDockingPaneManager()->NotifyAction(xtpPaneActionCollapsing, m_pActiveWnd->m_pPane->GetSelected()))
				return;

			GetDockingPaneManager()->NotifyAction(xtpPaneActionCollapsed,
				m_pActiveWnd->m_pPane->GetSelected());
		}

		if (bDelayed)
		{
			// let the window close itself
			if (m_pActiveWnd->m_pPane)
			{
				m_pActiveWnd->m_pPane->ShowWindow(SW_HIDE);
				m_pActiveWnd->m_pPane->SetDockingSite(GetDockingPaneManager()->GetSite());
				m_pActiveWnd->m_pPane = 0;
			}
			m_pActiveWnd->SetTimer(TID_CLOSE, 50, NULL);
		}
		else
		{
			m_pActiveWnd->CloseWindow();
		}

		m_pActiveWnd = 0;
	}
}

void CXTPDockingPaneAutoHidePanel::ShowPane(CXTPDockingPane* pPane, BOOL bSetFocus)
{
	if (m_pActiveWnd && m_pActiveWnd->m_pPane && m_pActiveWnd->m_pPane->GetSelected() == pPane)
	{
		if (bSetFocus) m_pActiveWnd->m_pPane->SelectPane(pPane, TRUE, FALSE);

		if (m_pActiveWnd->m_bSlideOut)
		{
			m_pActiveWnd->OnTimer(TID_CHECKACTIVE);
		}
		return;
	}

	CloseActiveWindows();

	if (GetDockingPaneManager()->NotifyAction(xtpPaneActionExpanding, pPane))
		return;

	CXTPDockingPaneTabbedContainer* pContainer = ((CXTPDockingPaneTabbedContainer*)pPane->m_pParentContainer);

	ASSERT(pContainer);
	if (!pContainer)
		return;
	ASSERT(pContainer->GetType() == xtpPaneTypeTabbedContainer);

	pContainer->SelectPane(pPane, FALSE, TRUE);
	CXTPDockingPaneAutoHideWnd* pActiveWnd = new CXTPDockingPaneAutoHideWnd(this, pContainer);

	if (bSetFocus) pContainer->SelectPane(pPane, TRUE, FALSE);

	if (m_hWnd) Invalidate(FALSE);
	GetDockingPaneManager()->RecalcFrameLayout(this);

	m_pActiveWnd = pActiveWnd;
}

void CXTPDockingPaneAutoHidePanel::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	if (m_pHighlightedNavigateButton)
	{
		m_pHighlightedNavigateButton->PerformClick(m_hWnd, point);
		return;
	}

	CXTPDockingPane* pPane = HitTest(point);
	if (pPane)
	{
		if (m_pActiveWnd && m_pActiveWnd->m_pPane->GetSelected() == pPane)
		{
			pPane->SetFocus();

		}
		else
		{
			ShowPane(pPane);
		}
	}
}

void CXTPDockingPaneAutoHidePanel::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	CXTPDockingPane* pPane = HitTest(point);
	if (pPane)
	{
		GetDockingPaneManager()->NotifyOwner(XTP_DPN_RCLICK, (LPARAM)(CXTPDockingPaneBase*)pPane);
	}
}

void CXTPDockingPaneAutoHidePanel::OnRButtonUp(UINT /*nFlags*/, CPoint point)
{
	XTP_DOCKINGPANE_CLICK menu;
	menu.pPane = HitTest(point);
	menu.pContainer = this;
	ClientToScreen(&point);
	menu.pt = point;
	menu.rcExclude.SetRectEmpty();

	if (GetDockingPaneManager()->NotifyOwner(XTP_DPN_CONTEXTMENU, (LPARAM)&menu))
		return;
}

void CXTPDockingPaneAutoHidePanel::OnMouseMove(UINT nFlags, CPoint point)
{
	PerformMouseMove(m_hWnd, point);

	if (m_pHighlightedNavigateButton)
	{
		point = CPoint(-1, -1);
	}


	for (int i = 0; i < m_pTabManagers->GetSize(); i++)
	{
		m_pTabManagers->GetAt(i)->PerformMouseMove(m_hWnd, point);
	}



	CXTPDockingPane* pPane = HitTest(point);
	if (pPane)
	{
		TRACKMOUSEEVENT tme =
		{
			sizeof(TRACKMOUSEEVENT), TME_HOVER | TME_LEAVE, m_hWnd, CXTPDockingPaneAutoHideWnd::m_nMouseHoverDelay
		};
		_TrackMouseEvent(&tme);

	}
	CWnd::OnMouseMove(nFlags, point);
}

void CXTPDockingPaneAutoHidePanel::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

LRESULT CXTPDockingPaneAutoHidePanel::OnMouseHover(WPARAM, LPARAM lParam)
{
	if (m_pHighlightedNavigateButton)
		return 1;

	CPoint point(lParam);
	CXTPDockingPane* pPane = HitTest(point);
	if (pPane)
	{
		if (!CXTPDrawHelpers::IsTopParentActive(m_hWnd) || (m_pActiveWnd && m_pActiveWnd->m_pPane->GetSelected() == pPane))
			return 1;

		if (pPane->GetOptions() & xtpPaneNoHoverShow)
			return 1;


		ShowPane(pPane, FALSE);
	}

	return 1;
}


void CXTPDockingPaneAutoHidePanel::Copy(CXTPDockingPaneBase* pCloneBase, CXTPPaneToPaneMap* pMap, DWORD /*dwIgnoredOptions*/)
{
	CXTPDockingPaneAutoHidePanel* pClone = (CXTPDockingPaneAutoHidePanel*)pCloneBase;

	ASSERT(pClone);
	if (!pClone)
		return;

	m_direction = pClone->m_direction;
	m_pDockingSite = GetDockingPaneManager()->GetSite();

	POSITION pos = pClone->GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = pClone->GetNext(pos);
		if (!pPane->IsEmpty() || pMap)
		{
			_InsertPane(pPane->Clone(m_pLayout, pMap));
		}
	}
}

void CXTPDockingPaneAutoHidePanel::InvalidatePane(BOOL /*bSelectionChanged*/)
{
	if (!m_hWnd)
		return;

	CClientDC dc(this);
	CXTPClientRect rcPanel(this);

	RefreshManagersArray(&dc, rcPanel);
	Invalidate(FALSE);
}


void CXTPDockingPaneAutoHidePanel::OnChildContainerChanged(CXTPDockingPaneBase* /*pContainer*/)
{
	if (m_pActiveWnd)
	{
		if (!m_bCloseActiveWindow)
		{
			GetDockingPaneManager()->RecalcFrameLayout(this);

			m_pActiveWnd->RecalcLayout(TRUE);
		}
		else
		{
			CloseActiveWindow(GetDockingPaneManager()->m_bAttachingPane);

			GetDockingPaneManager()->RecalcFrameLayout(this, TRUE);
		}
	}
}

void CXTPDockingPaneAutoHidePanel::DeletePane()
{
	InternalRelease();
}

void CXTPDockingPaneAutoHidePanel::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPDockingPaneAutoHidePanel, CCmdTarget)
	DISP_FUNCTION_ID(CXTPDockingPaneAutoHidePanel, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPDockingPaneAutoHidePanel, "Pane", DISPID_VALUE, OleGetPane, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPDockingPaneAutoHidePanel, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneAutoHidePanel, "ParentContainer", 20, OleGetContainer, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneAutoHidePanel, "Type", 21, OleGetType, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneAutoHidePanel, "hWnd", DISPID_HWND, OleGetHwnd, SetNotSupported, VT_HANDLE)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneAutoHidePanel, "IsEmpty", 22, OleIsEmpty, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneAutoHidePanel, "Position", 23, OleGetPosition, SetNotSupported, VT_I4)
END_DISPATCH_MAP()

// {ABC0C93B-61BC-4172-893C-CD3463CF2E01}
static const IID IID_IDockingPaneContainer =
{ 0xabc0c93b, 0x61bc, 0x4172, { 0x89, 0x3c, 0xcd, 0x34, 0x63, 0xcf, 0x2e, 0x1 } };

IMPLEMENT_OLETYPELIB_EX(CXTPDockingPaneAutoHidePanel, IID_IDockingPaneContainer)


BEGIN_INTERFACE_MAP(CXTPDockingPaneAutoHidePanel, CCmdTarget)
	INTERFACE_PART(CXTPDockingPaneAutoHidePanel, IID_IDockingPaneContainer, Dispatch)
END_INTERFACE_MAP()


IMPLEMENT_ENUM_VARIANT(CXTPDockingPaneAutoHidePanel);

LPDISPATCH CXTPDockingPaneAutoHidePanel::OleGetContainer()
{
	return m_pParentContainer ? m_pParentContainer->OleGetDispatch(TRUE) : NULL;
}

int CXTPDockingPaneAutoHidePanel::OleGetPosition()
{
	return GetDockingPaneManager()->GetPaneDirection(this);
}

int CXTPDockingPaneAutoHidePanel::OleGetType()
{
	return m_type;
}

BOOL CXTPDockingPaneAutoHidePanel::OleIsEmpty()
{
	return IsEmpty();
}

HWND CXTPDockingPaneAutoHidePanel::OleGetHwnd()
{
	return m_hWnd;
}

int CXTPDockingPaneAutoHidePanel::OleGetItemCount()
{
	return m_lstPanes.GetCount();
}

LPDISPATCH CXTPDockingPaneAutoHidePanel::OleGetPane(int nIndex)
{
	return OleGetItem(nIndex - 1);
}

LPDISPATCH CXTPDockingPaneAutoHidePanel::OleGetItem(int nIndex)
{
	POSITION pos = m_lstPanes.FindIndex(nIndex);
	if (!pos)
		return NULL;

	CXTPDockingPaneBase* pPane = m_lstPanes.GetAt(pos);
	return pPane ? pPane->OleGetDispatch(TRUE) : 0;
}

LPDISPATCH CXTPDockingPaneAutoHidePanel::OleGetDispatch(BOOL bAddRef)
{
	return GetIDispatch(bAddRef);
}


#endif
