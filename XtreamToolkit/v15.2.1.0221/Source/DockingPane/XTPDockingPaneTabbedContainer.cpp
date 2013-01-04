// XTPDockingPaneTabbedContainer.cpp : implementation of the CXTPDockingPaneTabbedContainer class.
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

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"

#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManager.h"

#include "XTPDockingPaneDefines.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneBaseContainer.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneContext.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPanePaintManager.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPaneAutoHidePanel.h"
#include "XTPDockingPaneMiniWnd.h"
#include "XTPDockingPaneSidePanel.h"
#include "XTPDockingPaneSplitterContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CTabManagerDropTarget

class CXTPDockingPaneTabbedContainer::CContainerDropTarget : public COleDropTarget
{
public:
	CContainerDropTarget()
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
		CXTPDockingPaneTabbedContainer* pControl = (CXTPDockingPaneTabbedContainer*)pWnd;
		ASSERT_VALID(pControl);

		if (!pControl->GetPaintManager()->m_bSelectOnDragOver)
			return DROPEFFECT_NONE;

		if (m_dwDragLastTick != (DWORD)-1 && pControl->GetPaintManager()->m_bSelectOnDragOver == 2)
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
			CXTPTabManagerItem* pItem = pControl->CXTPTabManager::HitTest(point);

			if (pItem)
			{
				CXTPDockingPane* pPane = pControl->GetItemPane(pItem->GetIndex());
				if (pControl->GetSelected() != pPane)
				{
					pControl->SelectPane(pPane, FALSE, FALSE);
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

// CXTPDockingPaneTabbedContainer

void CXTPDockingPaneTabbedContainer::RedrawControl(LPCRECT lpRect, BOOL /*bAnimate*/)
{
	if (!m_nLockReposition)
	{
		if (GetSafeHwnd()) InvalidateRect(lpRect, FALSE);
	}
}

void CXTPDockingPaneTabbedContainer::Reposition()
{
	if (!m_nLockReposition)
	{
		InvalidatePane(FALSE);
	}

}

CXTPTabPaintManager* CXTPDockingPaneTabbedContainer::GetPaintManager() const
{
	return CXTPDockingPaneBase::GetPaintManager()->GetTabPaintManager();
}

void CXTPDockingPaneTabbedContainer::SetPaintManager(CXTPTabPaintManager* /*pPaintManager*/)
{

}

BOOL CXTPDockingPaneTabbedContainer::DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) const
{
	if (!pItem)
		return TRUE;

	if (!CXTPDockingPaneBase::GetPaintManager()->GetTabPaintManager()->m_bShowIcons)
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



IMPLEMENT_DYNAMIC(CXTPDockingPaneTabbedContainer, CWnd)

CXTPDockingPaneTabbedContainer::CXTPDockingPaneTabbedContainer(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBaseContainer(xtpPaneTypeTabbedContainer, pLayout)
{

	m_pSelectedPane = 0;
	m_pTrackingPane = 0;

	m_nLockReposition = 0;

	m_bActive = FALSE;
	m_bTitleVisible = TRUE;
	m_bDelayRedraw = FALSE;

	m_pCaptionButtons = new CXTPDockingPaneCaptionButtons();
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_CLOSE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_MAXIMIZE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_RESTORE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_AUTOHIDE, this));
	m_pCaptionButtons->Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_MENU, this));

	m_pDropTarget = new CContainerDropTarget();
	m_bMaximized = FALSE;

	m_bEnsureSelectedTab = FALSE;

	EnableAutomation();

}

CXTPDockingPaneCaptionButton* CXTPDockingPaneTabbedContainer::GetCloseButton() const
{
	return FindCaptionButton(XTP_IDS_DOCKINGPANE_CLOSE);
}
CXTPDockingPaneCaptionButton* CXTPDockingPaneTabbedContainer::GetPinButton() const
{
	return FindCaptionButton(XTP_IDS_DOCKINGPANE_AUTOHIDE);
}


void CXTPDockingPaneTabbedContainer::Init(CXTPDockingPane* pPane, CWnd* pFrame)
{
	ASSERT(pPane);
	if (!pPane)
		return;

	m_pDockingSite = pFrame;
	m_szDocking = pPane->m_szDocking;
	m_rcWindow = pPane->m_rcWindow;

	_InsertPane(pPane);
	SelectPane(pPane);
}

CXTPDockingPaneTabbedContainer::~CXTPDockingPaneTabbedContainer()
{
	if (m_hWnd) DestroyWindow();

	delete m_pDropTarget;
}

void CXTPDockingPaneTabbedContainer::SetDockingSite(CWnd* pFrame)
{
	m_pDockingSite = pFrame;

	if (m_hWnd && !pFrame->GetSafeHwnd())
	{
		pFrame = GetDockingPaneManager()->GetSite();
	}

	if (m_hWnd)
	{
		if (GetParent() != pFrame) CWnd::SetParent(pFrame);
	}

	POSITION pos = GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)GetNext(pos);
		pPane->SetDockingSite(m_pDockingSite);
	}

}

BOOL CXTPDockingPaneTabbedContainer::IsTabsVisible() const
{
	return m_hWnd && !DYNAMIC_DOWNCAST(CXTPDockingPaneAutoHideWnd, GetParent())
		&& (GetItemCount() > 1 || CXTPDockingPaneBase::GetPaintManager()->m_bDrawSingleTab);
}

void CXTPDockingPaneTabbedContainer::InvalidatePane(BOOL bSelectionChanged)
{
	if (!GetSafeHwnd())
		return;

	if (m_pParentContainer == 0)
		return;

	if (m_nLockReposition)
		return;

	m_nLockReposition += 1;
	OnTabsChanged();
	m_nLockReposition -= 1;

	CRect rect = m_rcWindow;
	CXTPDockingPaneBase::GetPaintManager()->AdjustClientRect(this, rect, TRUE);

	if (bSelectionChanged)
	{
		POSITION pos = GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)GetNext(pos);
			CRect rcPane = m_pSelectedPane == pPane ? rect : CRect(0, 0, 0, 0);
			pPane->OnSizeParent(m_pDockingSite, rcPane, 0);
		}
	}
	Invalidate(FALSE);

	m_pParentContainer->InvalidatePane(bSelectionChanged);
}

void CXTPDockingPaneTabbedContainer::OnSizeParent(CWnd* pParent, CRect rect, LPVOID lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	ASSERT(!IsEmpty());

	SetDockingSite(pParent);
	m_rcWindow = rect;

	if (lpLayout == 0 || lpLayout->hDWP != NULL)
	{
		CRect rectOld;
		::GetWindowRect(m_hWnd, rectOld);
		HWND hWndParent = ::GetParent(m_hWnd);
		::ScreenToClient(hWndParent, &rectOld.TopLeft());
		::ScreenToClient(hWndParent, &rectOld.BottomRight());
		if (rectOld != rect || m_bDelayRedraw)
		{
			SetWindowPos(&CWnd::wndBottom, rect.left, rect.top, rect.Width(), rect.Height(), 0);
			Invalidate(FALSE);
			m_bDelayRedraw = FALSE;
		}
		else
		{
			SetWindowPos(&CWnd::wndBottom, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW | SWP_NOACTIVATE);
		}

		m_nLockReposition += 1;
		OnTabsChanged();
		m_nLockReposition -= 1;

		GetPinButton()->SetState(IsHidden() ? xtpPanePinVisible | xtpPanePinPushed: DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, pParent) == 0 ? xtpPanePinVisible : 0);

		CXTPDockingPaneBase::GetPaintManager()->AdjustClientRect(this, rect, TRUE);

		POSITION pos = GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)GetNext(pos);

			CRect rcPane = m_pSelectedPane == pPane ? rect : CRect(0, 0, 0, 0);
			pPane->OnSizeParent(pParent, rcPane, lParam);
		}

		if (m_bEnsureSelectedTab)
		{
			m_bEnsureSelectedTab = FALSE;
			EnsureSelectedTabVisible();
		}
	}
}

void CXTPDockingPaneTabbedContainer::EnsureSelectedTabVisible()
{
	if (!m_hWnd)
		return;

	m_nLockReposition += 1;
	CXTPTabManager::EnsureVisible(GetPaneTab(m_pSelectedPane));
	CRect rect = m_rcWindow;
	CXTPDockingPaneBase::GetPaintManager()->AdjustClientRect(this, rect, TRUE);
	m_nLockReposition -= 1;
	Invalidate(FALSE);
}


void CXTPDockingPaneTabbedContainer::OnTabsChanged()
{
	if (!m_hWnd)
		return;

	m_nLockReposition += 1;

	DeleteAllItems();

	m_bCloseItemButton = GetDockingPaneManager()->m_bShowCloseTabButton;

	POSITION pos = GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)GetNext(pos);

		CXTPTabManagerItem* pItem = AddItem(GetItemCount());
		if (m_pSelectedPane == pPane) SetSelectedItem(pItem);

		pItem->SetCaption(pPane->GetTabCaption());
		pItem->SetColor(pPane->GetItemColor());
		pItem->SetTooltip(pPane->GetTitle());
		pItem->SetEnabled(pPane->GetEnabled() & xtpPaneEnableClient);
		pItem->SetClosable((pPane->GetOptions() & xtpPaneNoCloseable) == 0);

		pItem->SetData((DWORD_PTR)pPane);
	}
	//////////////////////////////////////////////////////////////////////////

	m_pCaptionButtons->CheckForMouseOver(CPoint(-1, -1));

	m_nLockReposition -= 1;
}

void CXTPDockingPaneTabbedContainer::CreateContainer()
{
	SAFE_CALLPTR(m_pParentContainer, CreateContainer());

	if (!m_hWnd)
	{
		Create(_T("XTPDockingPaneTabbedContainer"), _T(""), WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), GetDockingSite(), 0);
		m_pDropTarget->Register(this);
	}
}

void CXTPDockingPaneTabbedContainer::_InsertPane(CXTPDockingPane* pPane, BOOL bSetFocus)
{
	ASSERT(m_pLayout);
	if (!m_pLayout)
		return;

	if (!m_hWnd && !m_pLayout->IsUserLayout())
	{
		CreateContainer();
	}

	m_lstPanes.AddTail(pPane);

	pPane->SetParentContainer(this);
	pPane->SetDockingSite(GetDockingSite());

	m_bDelayRedraw = TRUE;
	GetDockingPaneManager()->RecalcFrameLayout(this, TRUE);

	SelectPane(pPane, bSetFocus);

	SAFE_CALLPTR(m_pParentContainer, OnChildContainerChanged(this));

}

void CXTPDockingPaneTabbedContainer::RemovePane(CXTPDockingPaneBase* pPane)
{
	ASSERT(pPane->GetContainer() == this);

	POSITION pos = m_lstPanes.Find((CXTPDockingPane*)pPane);
	ASSERT(pos);

	m_lstPanes.RemoveAt(pos);
	OnTabsChanged();

	pPane->SetParentContainer(NULL);

	m_pParentContainer->OnChildContainerChanged(this);

	if (m_lstPanes.IsEmpty())
	{
		if (m_bActive)
		{
			GetDockingPaneManager()->GetSite()->SetFocus();
			m_bActive = FALSE;
			GetDockingPaneManager()->OnActivatePane(FALSE, m_pSelectedPane);
		}
		DestroyWindow();
	}

	m_bDelayRedraw = TRUE;
	GetDockingPaneManager()->RecalcFrameLayout(this, TRUE);

	if (pPane == m_pSelectedPane)
	{
		SelectPane((CXTPDockingPane*)GetLastPane());
	}
}


BEGIN_MESSAGE_MAP(CXTPDockingPaneTabbedContainer, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_GETOBJECT, OnGetObject)
END_MESSAGE_MAP()



// CXTPDockingPaneTabbedContainer message handlers

void CXTPDockingPaneTabbedContainer::OnDestroy()
{
	CWnd::OnDestroy();
}

void CXTPDockingPaneTabbedContainer::DeletePane()
{
	InternalRelease();
}

void CXTPDockingPaneTabbedContainer::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}

BOOL CXTPDockingPaneTabbedContainer::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (GetDockingPaneManager()->m_bShowSizeCursorWhileDragging)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		if (CXTPDockingPaneBase::GetPaintManager()->GetCaptionGripperRect(this).PtInRect(pt))
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			return TRUE;
		}

	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


LRESULT CXTPDockingPaneTabbedContainer::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		CXTPClientRect rc(this);
		CXTPDockingPaneBase::GetPaintManager()->DrawPane(pDC, this, rc);
	}

	return TRUE;
}

void CXTPDockingPaneTabbedContainer::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint);

	CXTPDockingPaneBase::GetPaintManager()->DrawPane(&dc, this, rc);
}


void CXTPDockingPaneTabbedContainer::ShowTitle(BOOL bShow)
{
	if (m_bTitleVisible != bShow)
	{
		m_bTitleVisible = bShow;

		m_bDelayRedraw = TRUE;
		GetDockingPaneManager()->RecalcFrameLayout(this, TRUE);
	}
}

BOOL CXTPDockingPaneTabbedContainer::IsTitleVisible() const
{
	return m_bTitleVisible &&
		(m_pSelectedPane && ((m_pSelectedPane->GetOptions() & xtpPaneNoCaption) == 0)) &&
		CXTPDockingPaneBase::GetPaintManager()->m_bShowCaption;
}


CString CXTPDockingPaneTabbedContainer::GetTitle() const
{
	return m_pSelectedPane ? m_pSelectedPane->GetTitle() : _T("");
}

void CXTPDockingPaneTabbedContainer::OnFocusChanged()
{
	if (!GetSafeHwnd() || IsEmpty())
		return;

	CWnd* pFocus = GetFocus();
	BOOL bActive = (pFocus->GetSafeHwnd() && (pFocus == this || IsChild(pFocus) ||
		(pFocus->GetOwner()->GetSafeHwnd() && IsChild(pFocus->GetOwner()))));

	if (bActive != m_bActive)
	{
		m_bActive = bActive;

		GetDockingPaneManager()->OnActivatePane(bActive, m_pSelectedPane);

		Invalidate(FALSE);
	}
	else if (m_bActive && GetDockingPaneManager()->GetActivePane() != m_pSelectedPane)
	{
		GetDockingPaneManager()->OnActivatePane(bActive, m_pSelectedPane);
	}
}

#define DOCKINGPANE_HITCAPTION -2

int CXTPDockingPaneTabbedContainer::HitTest(CPoint point) const
{
	CXTPClientRect rc(this);

	if (IsTitleVisible())
	{
		if (rc.PtInRect(point))
		{
			CXTPDockingPaneBase::GetPaintManager()->AdjustCaptionRect(this, rc);
			if (!rc.PtInRect(point))
			{
				return DOCKINGPANE_HITCAPTION;
			}
		}
	}

	if (IsTabsVisible())
	{
		CXTPTabManagerItem* pItem = CXTPTabManager::HitTest(point);
		return pItem ? pItem->GetIndex() : -1;
	}
	return -1;
}

CXTPTabManagerItem* CXTPDockingPaneTabbedContainer::GetPaneTab(CXTPDockingPane* pPane) const
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		if (GetItemPane(i) == pPane)
			return GetItem(i);
	}
	return NULL;
}

void CXTPDockingPaneTabbedContainer::SelectPane(CXTPDockingPane* pPane, BOOL bSetFocus, BOOL bDelayRedraw)
{
	if (m_pSelectedPane != pPane)
	{
		m_pSelectedPane = pPane;

		if (bDelayRedraw)
		{
			m_bEnsureSelectedTab = TRUE;
			m_bDelayRedraw = TRUE;

			GetDockingPaneManager()->RecalcFrameLayout(this, TRUE);
			SAFE_CALLPTR(m_pParentContainer, OnChildContainerChanged(this));
		}
		else
		{
			InvalidatePane(TRUE);
			EnsureSelectedTabVisible();
		}

		if (m_hWnd && m_pSelectedPane)
		{
			SetWindowText(GetTitle());
		}
	}

	if (bSetFocus)
	{
		if (m_bActive) GetDockingPaneManager()->OnActivatePane(TRUE, m_pSelectedPane);
		SAFE_CALLPTR(pPane, SetFocus());
	}

}

void CXTPDockingPaneTabbedContainer::OnRButtonUp(UINT /*nFlags*/, CPoint point)
{
	XTP_DOCKINGPANE_CLICK menu;
	menu.pContainer = this;
	menu.pPane = GetItemPane(HitTest(point));
	if (!menu.pPane) menu.pPane = m_pSelectedPane;
	ClientToScreen(&point);
	menu.pt = point;
	menu.rcExclude.SetRectEmpty();

	if (GetDockingPaneManager()->NotifyOwner(XTP_DPN_CONTEXTMENU, (LPARAM)&menu))
		return;

}

void CXTPDockingPaneTabbedContainer::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	int nHit = HitTest(point);
	if (nHit >= 0)
	{
		SelectPane(GetItemPane(nHit));
		GetDockingPaneManager()->RecalcFrameLayout(this);
	}

	if (m_pSelectedPane)
		m_pSelectedPane->SetFocus();
	else
		SetFocus();

	GetDockingPaneManager()->UpdatePanes();

	GetDockingPaneManager()->NotifyOwner(XTP_DPN_RCLICK, (LPARAM)(CXTPDockingPaneBase*)this);
}

void CXTPDockingPaneTabbedContainer::OnCaptionLButtonDown(CPoint point)
{
	_RestoreFocus();

	if (GetKeyState(VK_LBUTTON) < 0)
	{
		if (GetDockingPaneManager()->NotifyAction(xtpPaneActionDragging, m_pSelectedPane))
			return;

		CXTPDockingPaneContext* pContext = GetDockingPaneManager()->GetDockingContext();
		pContext->Drag(this, point);
	}
}


void CXTPDockingPaneTabbedContainer::NormalizeDockingSize()
{
	XTPDockingPaneDirection direction = GetDockingPaneManager()->GetPaneDirection(this);
	BOOL bHoriz = (direction == xtpPaneDockLeft) || (direction == xtpPaneDockRight);

	if (bHoriz) m_szDocking.cx = m_rcWindow.Width();
	else m_szDocking.cy = m_rcWindow.Height();

	POSITION pos = GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = GetNext(pos);
		if (!pPane->IsEmpty())
		{
			if (bHoriz) pPane->m_szDocking.cx = m_szDocking.cx;
			else pPane->m_szDocking.cy = m_szDocking.cy;
		}
	}
}

void CXTPDockingPaneTabbedContainer::ClosePane(CXTPDockingPane* pPane)
{
	if (!pPane)
		return;

	if ((pPane->GetOptions() & xtpPaneNoCloseable) != 0)
		return;

	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	pPane->InternalAddRef();
	if (!pManager->NotifyAction(xtpPaneActionClosing, pPane))
	{
		pPane->Close();
		pManager->NotifyAction(xtpPaneActionClosed, pPane);
	}
	pPane->InternalRelease();
}

void CXTPDockingPaneTabbedContainer::OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton)
{
	CXTPTabManager::OnNavigateButtonClick(pButton->GetID());

	CXTPTabManagerItem* pItem = pButton->GetItem() ? pButton->GetItem() : m_pSelected;

	if (pItem && (pButton->GetID() == xtpTabNavigateButtonClose) && pItem->IsClosable())
	{
		CXTPDockingPane* pSelectedPane =  (CXTPDockingPane*)pItem->GetData();

		ClosePane(pSelectedPane);
	}
}

void CXTPDockingPaneTabbedContainer::OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton)
{
	CXTPDockingPane* pSelectedPane = m_pSelectedPane;

	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	switch (pButton->GetID())
	{
	case XTP_IDS_DOCKINGPANE_CLOSE:

		if (pManager->m_bCloseGroupOnButtonClick)
		{
			POSITION pos = m_lstPanes.GetTailPosition();
			while (pos)
			{
				CXTPDockingPane* pPane = (CXTPDockingPane*)m_lstPanes.GetPrev(pos);

				ClosePane(pPane);
			}

		}
		else if (pSelectedPane)
		{
			ClosePane(pSelectedPane);
		}
		break;


	case XTP_IDS_DOCKINGPANE_AUTOHIDE:

		if (!IsHidden())
		{
			if (!pManager->NotifyAction(xtpPaneActionUnpinning, pSelectedPane))
			{
				GetDockingSite()->SetFocus();
				NormalizeDockingSize();
				pSelectedPane->Hide();
				pManager->NotifyAction(xtpPaneActionUnpinned, pSelectedPane);
			}

		}
		else
		{
			GetDockingPaneManager()->ToggleDocking(pManager->m_bHideGroupOnButtonClick ?
				(CXTPDockingPaneBase*)this : (CXTPDockingPaneBase*)pSelectedPane);
		}

		break;

	case XTP_IDS_DOCKINGPANE_MAXIMIZE:
		Maximize();
		break;

	case XTP_IDS_DOCKINGPANE_RESTORE:
		Restore();
		break;

	}
}

BOOL CXTPDockingPaneTabbedContainer::OnCaptionButtonDown(CXTPDockingPaneCaptionButton* pButton)
{
	switch (pButton->GetID())
	{
	case XTP_IDS_DOCKINGPANE_MENU:
		if (m_pSelectedPane)
		{
			InternalAddRef();

			CXTPDockingPaneManager* pManager = GetDockingPaneManager();
			XTP_DOCKINGPANE_CLICK menu;

			menu.pContainer = this;
			menu.rcExclude = pButton->GetRect();
			ClientToScreen(&menu.rcExclude);

			menu.pt = GetExStyle() & WS_EX_LAYOUTRTL ? CPoint(menu.rcExclude.right, menu.rcExclude.bottom) : CPoint(menu.rcExclude.left, menu.rcExclude.bottom);
			menu.pPane = m_pSelectedPane;
			pButton->m_bPressed = TRUE;
			Invalidate(FALSE);

			pManager->NotifyOwner(XTP_DPN_CONTEXTMENU, (LPARAM)&menu);

			pButton->m_bPressed = FALSE;
			if (m_hWnd) Invalidate(FALSE);

			InternalRelease();
		}
		return TRUE;

	}
	return FALSE;
}


void CXTPDockingPaneTabbedContainer::_RestoreFocus()
{
	if (m_pSelectedPane)
		m_pSelectedPane->SetFocus();
	else
		SetFocus();

	GetDockingPaneManager()->UpdatePanes();
}


void CXTPDockingPaneTabbedContainer::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{

	CXTPDockingPaneCaptionButton* pButton = HitTestCaptionButton(point);

	if (pButton)
	{
		_RestoreFocus();

		if (m_pSelectedPane && OnCaptionButtonDown(pButton))
			return;

		if (pButton->Click(this, point) && m_pSelectedPane)
		{
			OnCaptionButtonClick(pButton);
		}

		return;
	}

	if (IsTabsVisible() && PerformClick(m_hWnd, point, TRUE))
		return;

	int nHit = HitTest(point);
	if (nHit == DOCKINGPANE_HITCAPTION && !IsHidden())
	{
		ClientToScreen(&point);
		OnCaptionLButtonDown(point);
	}
	else if (nHit >= 0)
	{
		CXTPDockingPane* pPane = GetItemPane(nHit);
		if (GetDockingPaneManager()->NotifyAction(xtpPaneActionDragging, pPane))
		{
			SelectPane(pPane, TRUE, FALSE);
			PerformMouseMove(m_hWnd, point);
			Invalidate(FALSE);
			return;
		}

		m_lstRects.RemoveAll();
		for (int j = 0; j < GetItemCount(); j++)
		{
			CRect rc(GetItem(j)->GetRect());
			m_lstRects.Add(rc);
		}

		m_pTrackingPane = pPane;
		SelectPane(m_pTrackingPane, TRUE, FALSE);

		PerformMouseMove(m_hWnd, point);

		SetCapture();

		Invalidate(FALSE);
	}
	else
	{
		_RestoreFocus();
	}
}

CXTPDockingPane* CXTPDockingPaneTabbedContainer::GetItemPane(int nIndex) const
{
	return nIndex >= 0 ? (CXTPDockingPane*)GetItem(nIndex)->GetData() : NULL;
}

void CXTPDockingPaneTabbedContainer::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	if (PerformClick(m_hWnd, point, TRUE))
		return;

	if (HitTestCaptionButton(point))
		return;

	int nHit = HitTest(point);
	if (nHit == DOCKINGPANE_HITCAPTION)
	{
		CXTPDockingPane* pSelected = GetSelected();

		if (IsHidden() && pSelected && ((pSelected->GetOptions() & xtpPaneNoHideable) != 0))
			return;

		if (!IsHidden() && pSelected && ((pSelected->GetOptions() & xtpPaneNoFloatableByCaptionDoubleClick) != 0))
			return;

		GetDockingPaneManager()->ToggleDocking(this);
	}
	else if (nHit >= 0)
	{
		CXTPDockingPane* pPane = GetItemPane(nHit);

		if ((pPane->GetOptions() & xtpPaneNoFloatableByTabDoubleClick) == 0)
		{
			GetDockingPaneManager()->ToggleDocking(pPane);
		}
	}

}


void CXTPDockingPaneTabbedContainer::_Swap(CXTPDockingPane* p1, CXTPDockingPane* p2)
{
	POSITION pos1 = m_lstPanes.Find(p1);
	POSITION pos2 = m_lstPanes.Find(p2);
	ASSERT(pos1 && pos2);

	m_lstPanes.SetAt(pos1, p2);
	m_lstPanes.SetAt(pos2, p1);
}

void CXTPDockingPaneTabbedContainer::OnMouseMove(UINT nFlags, CPoint point)
{
	PerformMouseMove(m_hWnd, point);

	if (m_pTrackingPane)
	{
		for (int i = 0; i < m_lstRects.GetSize(); i++)
		{
			if (GetItemPane(i) != m_pTrackingPane && m_lstRects[i].PtInRect(point))
			{
				_Swap(GetItemPane(i), m_pTrackingPane);

				InvalidatePane(TRUE);
				return;
			}
		}


		if (CXTPTabManager::HitTest(point) == NULL)
		{
			if (GetDockingPaneManager()->NotifyAction(xtpPaneActionDetaching, m_pTrackingPane))
				return;

			ASSERT(m_pSelectedPane == m_pTrackingPane);
			m_pTrackingPane = NULL;
			ReleaseCapture();

			ClientToScreen(&point);

			CXTPDockingPaneContext* pContext = GetDockingPaneManager()->GetDockingContext();
			pContext->Drag(m_pSelectedPane, point);
		}
	}
	else
	{
		m_pCaptionButtons->CheckForMouseOver(point);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CXTPDockingPaneTabbedContainer::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPDockingPaneTabbedContainer::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pTrackingPane)
	{
		m_pTrackingPane = NULL;
		if (GetCapture() == this) ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPDockingPaneTabbedContainer::OnCaptureChanged(CWnd* pWnd)
{
	m_pTrackingPane = NULL;
	CWnd::OnCaptureChanged(pWnd);
}

void CXTPDockingPaneTabbedContainer::OnParentContainerChanged(CXTPDockingPaneBase* pContainer)
{
	m_pDockingSite = pContainer->m_pDockingSite;
	m_bTitleVisible = TRUE;
	if (pContainer->GetType() == xtpPaneTypeAutoHidePanel)
	{
		if (m_hWnd) ShowWindow(SW_HIDE); //?
	}
}

BOOL CXTPDockingPaneTabbedContainer::CanAttach(CRect& rcClient, CPoint pt) const
{
	CRect rcTitle(rcClient);
	BOOL bVertical = IsCaptionVertical();

	if (m_bTitleVisible)
	{
		if (bVertical)
		{
			rcClient.left += CXTPDockingPaneBase::GetPaintManager()->GetCaptionHeight() + 3;
		}
		else
		{
			rcClient.top += CXTPDockingPaneBase::GetPaintManager()->GetCaptionHeight() + 3;
		}
	}
	else
	{
		SAFE_CALLPTR(m_pDockingSite, GetWindowRect(rcTitle));
	}

	if (bVertical)
	{
		rcTitle.right = rcClient.left;
	}
	else
	{
		rcTitle.bottom = rcClient.top;
	}

	if (rcTitle.PtInRect(pt))
		return TRUE;

	if (IsTabsVisible())
	{
		CRect rcTabs(rcClient);
		rcClient.bottom -= CXTPDockingPaneBase::GetPaintManager()->GetTabsHeight();
		rcTabs.top = rcClient.bottom;

		if (rcTabs.PtInRect(pt))
			return TRUE;
	}

	return FALSE;
}

void CXTPDockingPaneTabbedContainer::Copy(CXTPDockingPaneBase* pCloneBase, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions)
{
	CXTPDockingPaneTabbedContainer* pClone = (CXTPDockingPaneTabbedContainer*)pCloneBase;
	ASSERT(pClone);
	if (!pClone)
		return;

	m_bActive = FALSE;
	m_pDockingSite = GetDockingPaneManager()->GetSite();
	m_pSelectedPane = m_pTrackingPane = NULL;

	m_rcWindow = pClone->m_rcWindow;
	m_szDocking = pClone->m_szDocking;
	m_bTitleVisible = TRUE;
	m_nLockReposition = 0;

	if (pClone->m_pLayout != m_pLayout)
	{
		m_bMaximized = pClone->m_bMaximized;
	}

	CXTPDockingPane* pSelected = NULL;
	if (pMap)
	{
		POSITION pos = pClone->GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)pClone->GetNext(pos);
			CXTPDockingPane* pNewPane = (CXTPDockingPane*)pPane->Clone(m_pLayout, pMap);
			if (pClone->GetSelected() == pPane) pSelected = pNewPane;
			_InsertPane(pNewPane);
		}
	}
	else
	{

		POSITION pos = pClone->m_lstPanes.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)pClone->GetNext(pos);

			ASSERT(pPane->GetType() == xtpPaneTypeDockingPane);

			if ((((CXTPDockingPane*)pPane)->GetOptions() & dwIgnoredOptions) == 0)
			{
				if (pClone->GetSelected() == pPane && pSelected == NULL) pSelected = pPane;

				pPane->m_pParentContainer->RemovePane(pPane);
				_InsertPane(pPane);
			}
		}
	}
	if (pSelected) SelectPane(pSelected, FALSE);


}

BOOL CXTPDockingPaneTabbedContainer::IsHidden() const
{
	return GetContainer() && GetContainer()->GetType() == xtpPaneTypeAutoHidePanel;
}

void CXTPDockingPaneTabbedContainer::Show(BOOL bSetFocus)
{
	if (m_pSelectedPane &&m_pParentContainer &&
		m_pParentContainer->GetType() == xtpPaneTypeAutoHidePanel)
	{
		((CXTPDockingPaneAutoHidePanel*)m_pParentContainer)->ShowPane(m_pSelectedPane, bSetFocus);
	}
	else if (DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, GetParentFrame()))
	{
		((CXTPDockingPaneMiniWnd*)GetParentFrame())->Expand();
	}
}

void CXTPDockingPaneTabbedContainer::ShowPane(CXTPDockingPane* pPane, BOOL bSetFocus)
{
	ASSERT(pPane);

	if (m_pParentContainer && m_pParentContainer->GetType() == xtpPaneTypeAutoHidePanel)
	{
		((CXTPDockingPaneAutoHidePanel*)m_pParentContainer)->ShowPane(pPane, bSetFocus);
	}
	else
	{
		SelectPane(pPane, bSetFocus);

		if (DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, GetParentFrame()))
			((CXTPDockingPaneMiniWnd*)GetParentFrame())->Expand();

		if (m_pParentContainer && m_pParentContainer->GetType() == xtpPaneTypeSidePanel)
		{
			((CXTPDockingPaneSidePanel*)m_pParentContainer)->Expand();
		}
	}

}

BOOL CXTPDockingPaneTabbedContainer::IsAllowMaximize() const
{
	if (!m_pParentContainer || m_pParentContainer->GetType() != xtpPaneTypeSplitterContainer)
		return FALSE;

	if (!GetDockingPaneManager()->IsCaptionMaximizeButtonsVisible())
		return FALSE;

	CXTPDockingPaneBaseList lst;
	((CXTPDockingPaneSplitterContainer*)m_pParentContainer)->FindChildPane(xtpPaneTypeTabbedContainer, &lst);

	if (lst.GetCount() < 2)
		return FALSE;

	lst.RemoveAll();
	((CXTPDockingPaneSplitterContainer*)m_pParentContainer)->FindChildPane(xtpPaneTypeClient, &lst);

	if (!lst.IsEmpty())
		return FALSE;


	return TRUE;
}

void CXTPDockingPaneTabbedContainer::Maximize()
{
	if (!m_pDockingSite || !m_pParentContainer)
		return;

	CXTPDockingPaneBaseList lst;
	((CXTPDockingPaneSplitterContainer*)m_pParentContainer)->FindChildPane(xtpPaneTypeTabbedContainer, &lst);
	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneTabbedContainer* pTabbedContainer = (CXTPDockingPaneTabbedContainer*)lst.GetNext(pos);
		pTabbedContainer->m_bMaximized = pTabbedContainer == this;
	}

	GetDockingPaneManager()->RecalcFrameLayout(this);
}

void CXTPDockingPaneTabbedContainer::Restore()
{
	if (!m_pDockingSite || !m_pParentContainer)
		return;

	m_bMaximized = FALSE;
	GetDockingPaneManager()->RecalcFrameLayout(this);
}

BOOL CXTPDockingPaneTabbedContainer::IsPaneMaximized() const
{
	if (!IsAllowMaximize())
		return FALSE;

	return m_bMaximized;
}

BOOL CXTPDockingPaneTabbedContainer::IsPaneRestored() const
{
	if (!IsAllowMaximize())
		return FALSE;

	return !m_bMaximized;
}

BOOL CXTPDockingPaneTabbedContainer::IsPaneMinimized() const
{
	if (m_bMaximized)
		return FALSE;

	if (!m_pParentContainer || m_pParentContainer->GetType() != xtpPaneTypeSplitterContainer)
		return FALSE;

	if (!GetDockingPaneManager()->IsCaptionMaximizeButtonsVisible())
		return FALSE;

	CXTPDockingPaneBaseList lst;
	((CXTPDockingPaneSplitterContainer*)m_pParentContainer)->FindChildPane(xtpPaneTypeTabbedContainer, &lst);
	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneTabbedContainer* pTabbedContainer = (CXTPDockingPaneTabbedContainer*)lst.GetNext(pos);
		if (pTabbedContainer->m_bMaximized)
			return TRUE;
	}
	return FALSE;
}



BOOL CXTPDockingPaneTabbedContainer::IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton)
{
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
		return m_pSelectedPane && ((m_pSelectedPane->GetOptions() & xtpPaneNoCloseable) == 0);

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
		return m_pSelectedPane && ((m_pSelectedPane->GetOptions() & xtpPaneNoHideable) == 0) && (pButton->GetState() & xtpPanePinVisible);

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_MAXIMIZE)
		return IsPaneRestored();

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_RESTORE)
		return IsPaneMaximized();

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_MENU)
		return m_pSelectedPane && (m_pSelectedPane->GetOptions() & xtpPaneHasMenuButton);

	return TRUE;
}


LRESULT CXTPDockingPaneTabbedContainer::OnHelpHitTest(WPARAM, LPARAM lParam)
{
	CXTPDockingPane* pPane = GetSelected();
	CPoint point((DWORD)lParam);

	int nHit = HitTest(point);

	if (nHit >= 0)
	{
		pPane = GetItemPane(nHit);
	}

	if (!pPane)
		return 0;

	int nIDHelp = pPane->m_nIDHelp;

	if (nIDHelp == 0)
	{
		pPane->m_hwndChild ? (int)::GetDlgCtrlID(pPane->m_hwndChild): 0;
	}

	if (nIDHelp == 0)
	{
		nIDHelp = pPane->GetID();
	}

	return HID_BASE_RESOURCE + nIDHelp;
}

CXTPDockingPaneCaptionButton* CXTPDockingPaneTabbedContainer::HitTestCaptionButton(CPoint point) const
{
	if (IsTitleVisible())
	{
		for (int i = 0; i < m_pCaptionButtons->GetSize(); i++)
		{
			CXTPDockingPaneCaptionButton* pButton = m_pCaptionButtons->GetAt(i);
			if (pButton->PtInRect(point))
				return pButton->IsEnabled() ? pButton : NULL;
		}
	}

	return NULL;
}

INT_PTR CXTPDockingPaneTabbedContainer::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
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

	nHit = HitTest(point);
	if (nHit == DOCKINGPANE_HITCAPTION)
	{
		CString strTip = GetSelected()->GetTitleToolTip();

		if (strTip.IsEmpty())
			return -1;

		nHit = 2;

		CRect rcCaption;
		CXTPDockingPaneBase::GetPaintManager()->GetCaptionRect(this, rcCaption);

		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, rcCaption, nHit, strTip);

		return nHit;
	}

	return PerformToolHitTest(m_hWnd, point, pTI);
}

void CXTPDockingPaneTabbedContainer::AdjustMinMaxInfoClientRect(LPMINMAXINFO pMinMaxInfo, BOOL bCaptionOnly) const
{
	CRect rc(0, 0, 1000, 1000);
	if (bCaptionOnly)
	{
		CXTPDockingPaneBase::GetPaintManager()->AdjustCaptionRect(this, rc);
	}
	else
	{
		CXTPDockingPaneBase::GetPaintManager()->AdjustClientRect((CXTPDockingPaneTabbedContainer*)this, rc, FALSE);
	}

	pMinMaxInfo->ptMinTrackSize.x += 1000 - rc.Width();
	pMinMaxInfo->ptMinTrackSize.y += 1000 - rc.Height();

	pMinMaxInfo->ptMaxTrackSize.x += 1000 - rc.Width();
	pMinMaxInfo->ptMaxTrackSize.y += 1000 - rc.Height();

}

void CXTPDockingPaneTabbedContainer::GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo) const
{
	CXTPDockingPaneBase::GetMinMaxInfo(pMinMaxInfo);

	if (IsEmpty())
		return;

	if (IsPaneMinimized())
	{
		if (((CXTPDockingPaneSplitterContainer*)m_pParentContainer)->IsHoriz())
			pMinMaxInfo->ptMaxTrackSize.x = 0;
		else
			pMinMaxInfo->ptMaxTrackSize.y = 0;

		AdjustMinMaxInfoClientRect(pMinMaxInfo, FALSE);
	}
	else
	{
		POSITION pos = GetHeadPosition();
		while (pos)
		{
			CXTPDockingPaneBase* pPane = GetNext(pos);

			MINMAXINFO info;
			pPane->GetMinMaxInfo(&info);

			pMinMaxInfo->ptMinTrackSize.x = max(pMinMaxInfo->ptMinTrackSize.x, info.ptMinTrackSize.x);
			pMinMaxInfo->ptMinTrackSize.y = max(pMinMaxInfo->ptMinTrackSize.y, info.ptMinTrackSize.y);

			pMinMaxInfo->ptMaxTrackSize.x = min(pMinMaxInfo->ptMaxTrackSize.x, info.ptMaxTrackSize.x);
			pMinMaxInfo->ptMaxTrackSize.y = min(pMinMaxInfo->ptMaxTrackSize.y, info.ptMaxTrackSize.y);
		}
		AdjustMinMaxInfoClientRect(pMinMaxInfo);
	}
}


BOOL CXTPDockingPaneTabbedContainer::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	if (pManager)
	{
		pManager->GetToolTipContext()->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}


BOOL CXTPDockingPaneTabbedContainer::IsCaptionVertical() const
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	switch (pManager->GetCaptionDirection())
	{
		case xtpPaneCaptionHorizontal:
			return FALSE;

		case xtpPaneCaptionAutoBySize:
			return m_rcWindow.Width() > m_rcWindow.Height();

		case xtpPaneCaptionAutoByPosition:
			XTPDockingPaneDirection direction = pManager->GetPaneDirection(this);
			return (direction == xtpPaneDockTop) || (direction == xtpPaneDockBottom);

	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Accessible

CCmdTarget* CXTPDockingPaneTabbedContainer::GetAccessible()
{
	return this;
}

HRESULT CXTPDockingPaneTabbedContainer::GetAccessibleParent(IDispatch* FAR* ppdispParent)
{
	*ppdispParent = NULL;

	if (GetSafeHwnd())
	{
		return AccessibleObjectFromWindow(GetSafeHwnd(), OBJID_WINDOW, IID_IDispatch, (void**)ppdispParent);
	}
	return E_FAIL;
}

HRESULT CXTPDockingPaneTabbedContainer::GetAccessibleChildCount(long FAR* pChildCount)
{
	if (pChildCount == 0)
		return E_INVALIDARG;

	*pChildCount = GetItemCount() + 1 + (int)m_pCaptionButtons->GetSize();

	return S_OK;
}

HRESULT CXTPDockingPaneTabbedContainer::GetAccessibleChild(VARIANT varChild, IDispatch* FAR* ppdispChild)
{
	*ppdispChild = NULL;
	int nChild = GetChildIndex(&varChild) - 1;

	if (nChild == 0 && GetSelected())
		return AccessibleObjectFromWindow(GetSelected()->m_hwndChild, OBJID_WINDOW, IID_IDispatch, (void**)ppdispChild);

	nChild--;

	if (nChild >= 0 && nChild < GetItemCount())
	{
		CXTPDockingPane* pPane = GetItemPane(nChild);
		if (pPane)
		{
			*ppdispChild = pPane->GetIDispatch(TRUE);
		}
		return S_OK;
	}

	nChild -= GetItemCount();

	if (nChild >= 0 && nChild < m_pCaptionButtons->GetSize())
	{
		return S_FALSE;
	}

	return E_INVALIDARG;
}

HRESULT CXTPDockingPaneTabbedContainer::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	int nChild = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF || nChild == -1)
	{
		*pszName = GetTitle().AllocSysString();
		return S_OK;
	}

	nChild -= 1 + GetItemCount() + 1;

	if (nChild >= 0 && nChild < m_pCaptionButtons->GetSize())
	{
		int nId = m_pCaptionButtons->GetAt(nChild)->GetID();
		CString strTip;
		XTPResourceManager()->LoadString(&strTip, nId);

		*pszName = strTip.AllocSysString();
		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPDockingPaneTabbedContainer::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	pvarRole->vt = VT_EMPTY;
	int nChild = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_PAGETABLIST;
		return S_OK;
	}

	nChild -= 1 + GetItemCount() + 1;

	if (nChild >= 0 && nChild < m_pCaptionButtons->GetSize())
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_PUSHBUTTON;

		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPDockingPaneTabbedContainer::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	pvarState->vt = VT_I4;
	pvarState->lVal = 0;

	int nChild = GetChildIndex(&varChild);

	nChild -= 1 + GetItemCount() + 1;

	if (nChild >= 0 && nChild < m_pCaptionButtons->GetSize())
	{
		if (!IsTitleVisible() || !IsCaptionButtonVisible(m_pCaptionButtons->GetAt(nChild)))
			pvarState->lVal |= STATE_SYSTEM_INVISIBLE;

		return S_OK;
	}


	return S_OK;
}


HRESULT CXTPDockingPaneTabbedContainer::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	if (!GetSafeHwnd())
		return S_OK;

	int nChild = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF)
	{
		CRect rc;
		GetWindowRect(&rc);

		*pxLeft = rc.left;
		*pyTop = rc.top;
		*pcxWidth = rc.Width();
		*pcyHeight = rc.Height();

		return S_OK;
	}

	nChild -= 1 + GetItemCount() + 1;

	if (nChild >= 0 && nChild < m_pCaptionButtons->GetSize())
	{
		CXTPDockingPaneCaptionButton* pButton = m_pCaptionButtons->GetAt(nChild);
		if (!IsTitleVisible() || !IsCaptionButtonVisible(pButton))
			return S_FALSE;

		CRect rc;
		GetWindowRect(&rc);
		rc.OffsetRect(pButton->GetRect().TopLeft());

		*pxLeft = rc.left;
		*pyTop = rc.top;
		*pcxWidth = pButton->GetRect().Width();
		*pcyHeight = pButton->GetRect().Height();

		return S_OK;
	}



	return E_INVALIDARG;
}

HRESULT CXTPDockingPaneTabbedContainer::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (pvarID == NULL)
		return E_INVALIDARG;

	pvarID->vt = VT_EMPTY;

	if (!GetSafeHwnd())
		return S_FALSE;

	if (!CXTPWindowRect(this).PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	CPoint pt(xLeft, yTop);
	ScreenToClient(&pt);

	CXTPDockingPaneCaptionButton* pButton = HitTestCaptionButton(pt);

	if (pButton)
	{
		int nIndex = 0;
		for (; nIndex < m_pCaptionButtons->GetSize(); nIndex++)
			if (m_pCaptionButtons->GetAt(nIndex) == pButton)
				break;

		pvarID->vt = VT_I4;
		pvarID->lVal = 2 + GetItemCount() + nIndex;
		return S_OK;
	}

	int nItem = HitTest(pt);
	if (nItem == DOCKINGPANE_HITCAPTION)
	{
		pvarID->vt = VT_I4;
		pvarID->lVal = 0;
		return S_OK;
	}

	if (nItem == -1)
	{
		pvarID->vt = VT_DISPATCH;
		if (GetSelected())
			return AccessibleObjectFromWindow(GetSelected()->m_hwndChild, OBJID_WINDOW, IID_IDispatch, (void**)&pvarID->pdispVal);

		return S_FALSE;
	}

	pvarID->vt = VT_DISPATCH;
	pvarID->pdispVal = GetItemPane(nItem)->GetIDispatch(TRUE);

	return S_OK;
}

HRESULT CXTPDockingPaneTabbedContainer::GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
{
	int nChild = GetChildIndex(&varChild);

	nChild -= 1 + GetItemCount() + 1;

	if (nChild >= 0 && nChild < m_pCaptionButtons->GetSize())
	{

		*pszDefaultAction = SysAllocString(L"Press");

		return S_OK;
	}

	return S_FALSE;
}

HRESULT CXTPDockingPaneTabbedContainer::AccessibleDoDefaultAction(VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	int nChild = GetChildIndex(&varChild);

	nChild -= 1 + GetItemCount() + 1;

	if (nChild >= 0 && nChild < m_pCaptionButtons->GetSize())
	{
		OnCaptionButtonClick(m_pCaptionButtons->GetAt(nChild));

		return S_OK;
	}

	return S_FALSE;
}


LRESULT CXTPDockingPaneTabbedContainer::OnGetObject(WPARAM wParam, LPARAM lParam)
{
	if (((LONG)lParam) != OBJID_CLIENT)
		return (LRESULT)Default();

	LPUNKNOWN lpUnknown = GetInterface(&IID_IAccessible);
	if (!lpUnknown)
		return E_FAIL;

	return LresultFromObject(IID_IAccessible, wParam, lpUnknown);
}


BEGIN_INTERFACE_MAP(CXTPDockingPaneTabbedContainer, CCmdTarget)
	INTERFACE_PART(CXTPDockingPaneTabbedContainer, IID_IAccessible, ExternalAccessible)
END_INTERFACE_MAP()
