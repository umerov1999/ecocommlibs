// XTPRibbonPopups.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include "Common/XTPToolTipContext.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBase.h>

#include <TabManager/Includes.h>

#include "CommandBars/XTPCommandBarsDefines.h"
#include <CommandBars/XTPCommandBarKeyboardTip.h>
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPMouseManager.h"

#include "CommandBars/XTPPaintManager.h"

#include "XTPRibbonPaintManager.h"
#include "XTPRibbonGroups.h"
#include "XTPRibbonGroup.h"
#include "XTPRibbonTab.h"
#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CXTPRibbonScrollableBar::CControlGroupsScroll : public CXTPControlButton
{
public:
	CControlGroupsScroll(BOOL bScrollLeft)
	{
		SetFlags(xtpFlagSkipFocus | xtpFlagNoMovable);
		m_bScrollLeft = bScrollLeft;
	}

	void Draw(CDC* pDC)
	{
		GetParent()->GetPaintManager()->GetRibbonPaintManager()->DrawRibbonScrollButton(pDC, this, m_bScrollLeft);
	}

	BOOL IsCustomizeMovable() const
	{
		return FALSE;
	}

	void OnLButtonDown(CPoint point)
	{
		OnClick(FALSE, point);
	}

	virtual void OnClick(BOOL /*bKeyboard  = FALSE */, CPoint /*pt  = CPoint */)
	{
		if (GetKeyState(VK_LBUTTON) >= 0)
			return;

		m_bPressed = TRUE;

		CXTPRibbonScrollableBar* pScrollableBar = GetScrollableBar(m_pParent);

		pScrollableBar->OnGroupsScroll(m_bScrollLeft);

		int cmsTimer = GetDoubleClickTime() * 4 / 5;
		m_pParent->SetTimer(IDSYS_SCROLL, cmsTimer, NULL);
		BOOL bFirst = TRUE;

		m_pParent->SetCapture();

		while (::GetCapture() == m_pParent->GetSafeHwnd() && IsVisible())
		{
			MSG msg;

			if (!::GetMessage(&msg, NULL, 0, 0))
			{
				AfxPostQuitMessage((int)msg.wParam);
				break;
			}

			if (msg.message == WM_TIMER && msg.wParam == IDSYS_SCROLL)
			{
				pScrollableBar->OnGroupsScroll(m_bScrollLeft);

				if (bFirst)
				{
					cmsTimer = GetDoubleClickTime() / 10;
					m_pParent->SetTimer(IDSYS_SCROLL, cmsTimer, NULL);
				}
				bFirst = FALSE;
			}
			else if (msg.message == WM_LBUTTONUP)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		ReleaseCapture();
		m_pParent->KillTimer(IDSYS_SCROLL);

		m_bPressed = FALSE;
		if (!IsVisible()) m_bSelected = FALSE;
		RedrawParent(FALSE);
	}

protected:
	BOOL m_bScrollLeft;
};

CXTPRibbonBarMorePopupToolBar::CXTPRibbonBarMorePopupToolBar(CXTPRibbonBar* pRibbonBar)
{
	m_bTrackOnHover = FALSE;
	m_rcBorders = CRect(3, 3, 3, 3);
	m_pRibbonBar = pRibbonBar;
}

IMPLEMENT_DYNAMIC(CXTPRibbonBarMorePopupToolBar, CXTPPopupToolBar)

void CXTPRibbonBarMorePopupToolBar::FillCommandBarEntry(CDC* pDC)
{
	CXTPRibbonPaintManager* pPaintManager = GetPaintManager()->GetRibbonPaintManager();
	pPaintManager->FillMorePopupToolBarEntry(pDC, this);
}

CSize CXTPRibbonBarMorePopupToolBar::GetIconSize() const
{
	return m_pRibbonBar->GetIconSize();
}
CSize CXTPRibbonBarMorePopupToolBar::GetButtonSize() const
{
	return m_pRibbonBar->GetButtonSize();
}

CXTPRibbonScrollableBar::CXTPRibbonScrollableBar()
{
	m_pParent = NULL;
	m_pControlScrollGroupsLeft = NULL;
	m_pControlScrollGroupsRight = NULL;
	m_pHighlightedGroup = NULL;

	m_nGroupsScrollPos = 0;
}

void CXTPRibbonScrollableBar::InitScrollableBar(CXTPCommandBar* pParent)
{
	m_pControlScrollGroupsRight = pParent->GetControls()->Add(new CControlGroupsScroll(FALSE));
	m_pControlScrollGroupsLeft = pParent->GetControls()->Add(new CControlGroupsScroll(TRUE));
	m_nGroupsScrollPos = 0;
	m_pParent = pParent;
}

void CXTPRibbonScrollableBar::ShowScrollableRect(CXTPRibbonGroups* pGroups, CRect rc)
{
	int nGroupsScrollPos = pGroups->GetScrollPos();
	CRect rcGroups(GetGroupsRect());

	if (rc.left < 12 && m_pControlScrollGroupsLeft->IsVisible())
	{
		nGroupsScrollPos = nGroupsScrollPos + rc.left - 12;
		if (nGroupsScrollPos < 0) nGroupsScrollPos = 0;
	}
	else if (rc.right > rcGroups.right && m_pControlScrollGroupsRight->IsVisible())
	{
		nGroupsScrollPos = nGroupsScrollPos + rc.right - rcGroups.Width() + 12;
		if (nGroupsScrollPos < 0) nGroupsScrollPos = 0;
	}

	if (nGroupsScrollPos != m_nGroupsScrollPos)
	{
		m_nGroupsScrollPos = nGroupsScrollPos;
		pGroups->GetParentTab()->GetParent()->OnRecalcLayout();
	}
}

void CXTPRibbonScrollableBar::EnableGroupsScroll(BOOL bScrollLeft, BOOL bScrollRight)
{
	ASSERT(m_pControlScrollGroupsRight);
	ASSERT(m_pControlScrollGroupsLeft);
	if (!m_pControlScrollGroupsRight || !m_pControlScrollGroupsLeft)
		return;

	CRect rcGroups(GetGroupsRect());

	if (bScrollLeft)
	{
		m_pControlScrollGroupsLeft->SetHideFlag(xtpHideScroll, FALSE);
		m_pControlScrollGroupsLeft->SetRect(CRect(rcGroups.left - 1, rcGroups.top, rcGroups.left + 12, rcGroups.bottom));
	}
	else
	{
		m_pControlScrollGroupsLeft->SetHideFlag(xtpHideScroll, TRUE);
	}

	if (bScrollRight)
	{
		m_pControlScrollGroupsRight->SetHideFlag(xtpHideScroll, FALSE);
		m_pControlScrollGroupsRight->SetRect(CRect(rcGroups.right - 12, rcGroups.top, rcGroups.right + 1, rcGroups.bottom));
	}
	else
	{
		m_pControlScrollGroupsRight->SetHideFlag(xtpHideScroll, TRUE);
	}
}



//////////////////////////////////////////////////////////////////////////
// CXTPRibbonTabPopupToolBar

CXTPRibbonTabPopupToolBar::CXTPRibbonTabPopupToolBar(CXTPRibbonTab* pRibbonTab)
{

	InitScrollableBar(this);

	m_pRibbonBar = pRibbonTab->GetRibbonBar();
	m_pRibbonTab = (CXTPRibbonTab*)pRibbonTab->GetRuntimeClass()->CreateObject();

	m_pRibbonTab->m_pParent = this;
	m_pRibbonTab->m_pRibbonBar = m_pRibbonBar;

	m_pRibbonTab->Copy(pRibbonTab);

	m_pRibbonTab->SetRect(pRibbonTab->GetRect());

	m_bTrackOnHover = FALSE;
	SetFlags(xtpFlagSmartLayout);

	EnableAnimation(m_pRibbonBar->IsAnimationEnabled());
}

CXTPRibbonTabPopupToolBar::~CXTPRibbonTabPopupToolBar()
{
	m_pRibbonTab->OnRemoved();
	m_pRibbonTab->InternalRelease();
}

CRect CXTPRibbonTabPopupToolBar::GetGroupsRect() const
{
	return m_rcGroups;
}

void CXTPRibbonTabPopupToolBar::OnGroupsScroll(BOOL bScrollLeft)
{
	int nGroupsScrollPos = m_pRibbonTab->GetGroups()->GetScrollPos();

	SetPopuped(-1);
	HideKeyboardTips();

	if (bScrollLeft)
	{
		nGroupsScrollPos -= 40;
		if (nGroupsScrollPos < 0)
			nGroupsScrollPos = 0;
	}
	else
	{
		nGroupsScrollPos += 40;
	}

	if (nGroupsScrollPos != m_nGroupsScrollPos)
	{
		m_nGroupsScrollPos = nGroupsScrollPos;
		OnRecalcLayout();
	}
}

void CXTPRibbonTabPopupToolBar::EnsureVisible(CXTPControl* pControl)
{
	if (pControl->GetRibbonGroup() &&
		(m_pControlScrollGroupsLeft->IsVisible() || m_pControlScrollGroupsRight->IsVisible()))
	{
		ShowScrollableRect(m_pRibbonTab->GetGroups(), pControl->GetRect());
	}
}

void CXTPRibbonTabPopupToolBar::CreateKeyboardTips()
{
	CreateGroupKeyboardTips(m_pRibbonTab);
}

CSize CXTPRibbonTabPopupToolBar::CalcDynamicLayout(int, DWORD /*nMode*/)
{
	CRect rcRibbonBar;
	m_pRibbonBar->GetWindowRect(rcRibbonBar);

	CRect rcRibbonTab = m_pRibbonTab->GetRect();
	m_pRibbonBar->ClientToScreen(rcRibbonTab);

	CRect rcWork = XTPMultiMonitor()->GetWorkArea(rcRibbonTab);

	int nHeight = m_pRibbonBar->CalcGroupsHeight();
	int nWidth = rcRibbonBar.Width();

	m_ptPopup.y = m_rcExclude.bottom;
	m_ptPopup.x = rcRibbonBar.left;

	if (m_ptPopup.x < rcWork.left)
	{
		nWidth -= rcWork.left - m_ptPopup.x;
		m_ptPopup.x = rcWork.left;
	}

	if (m_ptPopup.x + nWidth > rcWork.right)
	{
		nWidth = rcWork.right - m_ptPopup.x - 4;
	}

	if (m_ptPopup.y + nHeight > rcWork.bottom)
	{
		m_ptPopup.y = m_rcExclude.top - nHeight;
	}

	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));

	UpdateShortcuts();

	m_rcGroups.SetRect(0, 0, nWidth, nHeight);
	m_pRibbonTab->GetGroups()->CalcDynamicSize(&dc, m_rcGroups.Width(), LM_COMMIT,
		CRect(m_rcGroups.left + 4, m_rcGroups.top + 3, 6, 3));

	m_rcTearOffGripper.SetRectEmpty();


	return CSize(nWidth, nHeight);
}

void CXTPRibbonTabPopupToolBar::DrawCommandBar(CDC* pDC, CRect rcClipBox)
{
	CXTPRibbonPaintManager* pPaintManager = GetPaintManager()->GetRibbonPaintManager();

	pPaintManager->FillTabPopupToolBar(pDC, this);
	pPaintManager->FillGroupRect(pDC, m_pRibbonTab, m_rcGroups);
	pDC->SetBkMode(TRANSPARENT);

	CXTPRibbonGroups* pGroups = m_pRibbonTab->GetGroups();
	for (int i = 0; i < pGroups->GetCount(); i++)
	{
		CXTPRibbonGroup* pGroup = pGroups->GetAt(i);

		if (pGroup->IsVisible() && CRect().IntersectRect(rcClipBox, pGroup->GetRect()))
		{
			pGroup->Draw(pDC, rcClipBox);
		}
	}

	if (m_pControlScrollGroupsLeft->IsVisible())
	{
		m_pControlScrollGroupsLeft->Draw(pDC);
	}

	if (m_pControlScrollGroupsRight->IsVisible())
	{
		m_pControlScrollGroupsRight->Draw(pDC);
	}
}

CSize CXTPRibbonTabPopupToolBar::GetButtonSize() const
{
	return m_pRibbonBar->GetButtonSize();
}

CSize CXTPRibbonTabPopupToolBar::GetIconSize() const
{
	return m_pRibbonBar->GetIconSize();

}

CRect CXTPRibbonTabPopupToolBar::CalculatePopupRect(CPoint ptPopup, CSize sz)
{
	return CRect(ptPopup, sz);
}

CXTPRibbonGroup* CXTPRibbonTabPopupToolBar::HitTestGroup(CPoint point) const
{
	if (!m_rcGroups.PtInRect(point))
		return NULL;

	return m_pRibbonTab->GetGroups()->HitTest(point);
}

BOOL CXTPRibbonTabPopupToolBar::IsKeyboardCuesVisible() const
{
	return m_pRibbonBar->IsKeyboardCuesVisible();
}

void CXTPRibbonTabPopupToolBar::OnKeyboardTip(CXTPCommandBarKeyboardTip* pTip)
{
	CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, pTip->m_pOwner);
	if (pControl)
	{

		if (pTip->m_bVisible == FALSE && pControl->GetHideFlags() == xtpHideWrap)
		{
			CXTPRibbonGroup* pGroup = pControl->GetRibbonGroup();
			if (pGroup && pGroup->GetControlGroupPopup())
			{
				EnsureVisible(pGroup->GetControlGroupPopup());
				pGroup->GetControlGroupPopup()->OnUnderlineActivate();

				int nIndex = pGroup->IndexOf(pControl);

				CXTPCommandBar* pPopupToolBar = pGroup->GetControlGroupPopup()->GetCommandBar();
				if (pPopupToolBar && pPopupToolBar->IsTrackingMode() && nIndex >= 0 &&
					nIndex < pPopupToolBar->GetControlCount())
				{
					pPopupToolBar->GetControl(nIndex)->OnUnderlineActivate();

				}
			}

			return;
		}

		if (pControl->IsVisible())
		{
			pControl->OnUnderlineActivate();
		}

		return;
	}
}

BEGIN_MESSAGE_MAP(CXTPRibbonTabPopupToolBar, CXTPPopupToolBar)
	//{{AFX_MSG_MAP(CXTPRibbonBar)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPRibbonScrollableBar::HighlightGroup(CXTPRibbonGroup* pHighlightedGroup)
{
	if (m_pHighlightedGroup)
	{
		CRect rcRedraw(m_pHighlightedGroup->GetRect());
		m_pHighlightedGroup = NULL;

		m_pParent->Redraw(rcRedraw);
	}

	m_pHighlightedGroup = pHighlightedGroup;

	if (m_pHighlightedGroup)
	{
		m_pParent->Redraw(m_pHighlightedGroup->GetRect());
	}
}

void CXTPRibbonTabPopupToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCommandBars* pCommandBars = GetCommandBars ();
	CXTPMouseManager* pMouseManager = pCommandBars->GetMouseManager();

	BOOL bMouseLocked = pMouseManager->IsMouseLocked() || !pMouseManager->IsTopParentActive(m_hWnd)
		|| pMouseManager->IsTrackedLock(this) || m_nPopuped != -1;

	CXTPRibbonGroup* pHighlightedGroup = bMouseLocked || !m_pRibbonBar->GetRibbonPaintManager()->m_bHotTrackingGroups ?
		NULL : HitTestGroup(point);

	if (pHighlightedGroup != m_pHighlightedGroup)
	{
		HighlightGroup(pHighlightedGroup);

		if (m_pHighlightedGroup)
		{
			pMouseManager->TrackMouseLeave(*this);
		}
	}

	CXTPPopupToolBar::OnMouseMove(nFlags, point);
}

BOOL CXTPRibbonTabPopupToolBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	HighlightGroup(NULL);

	return CXTPPopupToolBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard);
}

void CXTPRibbonTabPopupToolBar::OnMouseLeave()
{
	HighlightGroup(NULL);

	CXTPPopupToolBar::OnMouseLeave();
}

BOOL CXTPRibbonTabPopupToolBar::ProcessSpecialKey(XTPSpecialKey key)
{
	if (key != xtpKeyEscape)
		HideKeyboardTips();

	if (CXTPRibbonScrollableBar::ProcessSpecialKey(key))
		return TRUE;

	if (key == xtpKeyEscape && !m_pRibbonBar->m_bKeyboardSelect)
	{
		GetCommandBars()->ClosePopups();
		return TRUE;
	}

	return CXTPPopupToolBar::ProcessSpecialKey(key);
}

INT_PTR CXTPRibbonTabPopupToolBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	INT_PTR nHit = CXTPPopupToolBar::OnToolHitTest(point, pTI);
	if (nHit == -1)
		return -1;

	if (pTI != NULL && pTI->cbSize == sizeof(XTP_TOOLTIP_TOOLINFO_EX) && m_pToolTipContext
		&& m_pToolTipContext->IsShowTitleAndDescription())
	{
		XTP_TOOLTIP_CONTEXT* tc = ((XTP_TOOLTIP_TOOLINFO_EX*)pTI)->pToolInfo;
		if (tc && tc->pObject && ((CXTPControl*)tc->pObject)->GetRibbonGroup())
		{
			tc->rcExclude = m_rcGroups;
		}
	}

	return nHit;
}

IMPLEMENT_DYNAMIC(CXTPRibbonTabPopupToolBar, CXTPPopupToolBar)

CXTPRibbonScrollableBar* AFX_CDECL GetScrollableBar(CXTPCommandBar* pCommandBar)
{
	if (pCommandBar->IsRibbonBar())
		return (CXTPRibbonScrollableBar*)(CXTPRibbonBar*)(pCommandBar);

	if (pCommandBar->IsKindOf(RUNTIME_CLASS(CXTPRibbonTabPopupToolBar)))
		return (CXTPRibbonScrollableBar*)(CXTPRibbonTabPopupToolBar*)(pCommandBar);

	if (pCommandBar->IsKindOf(RUNTIME_CLASS(CXTPRibbonGroupPopupToolBar)))
		return (CXTPRibbonScrollableBar*)(CXTPRibbonGroupPopupToolBar*)(pCommandBar);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// CXTPRibbonGroupPopupToolBar

IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonGroupPopupToolBar, CXTPPopupToolBar)

CXTPRibbonGroupPopupToolBar::CXTPRibbonGroupPopupToolBar(CXTPRibbonGroup* pRibbonGroup /*= NULL*/)
{
	if (!pRibbonGroup)
	{
		m_pRibbonBar = NULL;
		m_pRibbonGroup = NULL;
		m_pParentGroup = NULL;
		return;
	}
	m_pParent = this;
	m_pRibbonBar = pRibbonGroup->GetRibbonBar();
	m_pRibbonGroup = (CXTPRibbonGroup*)pRibbonGroup->GetRuntimeClass()->CreateObject();
	m_pRibbonGroup->m_pParent = this;
	m_pRibbonGroup->m_pRibbonBar = m_pRibbonBar;
	m_pParentGroup = pRibbonGroup;

	m_pRibbonGroup->OnGroupAdded();
	m_pRibbonGroup->Copy(pRibbonGroup);

	int nHeight = m_pRibbonBar->GetRibbonPaintManager()->GetGroupCaptionHeight();
	m_rcBorders.SetRect(2, 2, 3, nHeight + 1);

	m_bTrackOnHover = FALSE;
	SetFlags(xtpFlagSmartLayout);

	EnableAnimation(m_pRibbonBar->IsAnimationEnabled());
}

CXTPRibbonGroupPopupToolBar::~CXTPRibbonGroupPopupToolBar()
{
	m_pRibbonGroup->OnGroupRemoved();
	m_pRibbonGroup->InternalRelease();
}

CSize CXTPRibbonGroupPopupToolBar::CalcDynamicLayout(int, DWORD /*nMode*/)
{
	m_bDynamicLayout = TRUE;

	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));

	UpdateShortcuts();

	int nGroupHeight = m_pRibbonBar->CalcGroupsHeight();

	m_pRibbonGroup->OnBeforeCalcSize(&dc);
	int nWidth = m_pRibbonGroup->OnCalcDynamicSize(&dc);
	m_pRibbonGroup->OnAdjustBorders(nWidth, CRect(0, 0, 0, 0));
	m_pRibbonGroup->OnAfterCalcSize();

	m_rcTearOffGripper.SetRectEmpty();

	m_bDynamicLayout = FALSE;

	return CSize(nWidth + m_rcBorders.left + m_rcBorders.right, nGroupHeight - 7);
}

void CXTPRibbonGroupPopupToolBar::DrawCommandBar(CDC* pDC, CRect rcClipBox)
{
	UNREFERENCED_PARAMETER(rcClipBox);

	m_pRibbonBar->GetRibbonPaintManager()->DrawRibbonGroupPopupToolBar(pDC, m_pRibbonGroup);
}

void CXTPRibbonGroupPopupToolBar::CreateKeyboardTips()
{
	if (!m_pParentGroup)
		return;

	CXTPRibbonTab* pSelectedTab = m_pParentGroup->GetParentTab();

	for (int i = 0; i < pSelectedTab->GetGroups()->GetCount(); i++)
	{
		CXTPRibbonGroup* pGroup = pSelectedTab->GetGroups()->GetAt(i);
		BOOL bVisible = pGroup->IsVisible() && m_pParentGroup == pGroup;

		if (bVisible)
		{
			pGroup = m_pRibbonGroup;
		}

		for (int j = 0; j < pGroup->GetCount(); j++)
		{
			CXTPControl* pControl = pGroup->GetAt(j);
			CreateControlKeyboardTips(pControl, NULL, bVisible);
		}

		if ((pGroup->GetControlGroupOption()->GetHideFlags() & xtpHideGeneric) == 0)
		{
			CreateControlKeyboardTips(pGroup->GetControlGroupOption(), NULL, bVisible);
		}

		CreateControlKeyboardTips(pGroup->GetControlGroupPopup(), _T("Z"), bVisible);
	}
}

CXTPRibbonGroup* CXTPRibbonGroupPopupToolBar::HitTestGroup(CPoint point) const
{
	if (m_pRibbonGroup && m_pRibbonGroup->GetRect().PtInRect(point))
		return m_pRibbonGroup;

	return NULL;
}


CSize CXTPRibbonGroupPopupToolBar::GetIconSize() const
{
	return m_pRibbonBar->GetIconSize();
}

CSize CXTPRibbonGroupPopupToolBar::GetButtonSize() const
{
	return m_pRibbonBar->GetButtonSize();
}

void CXTPRibbonGroupPopupToolBar::OnGroupsScroll(BOOL /*bScrollLeft*/)
{
}

CRect CXTPRibbonGroupPopupToolBar::GetGroupsRect() const
{
	return CXTPClientRect(this);
}

BOOL CXTPRibbonGroupPopupToolBar::ProcessSpecialKey(XTPSpecialKey key)
{
	if (key != xtpKeyEscape)
		HideKeyboardTips();

	if (CXTPRibbonScrollableBar::ProcessSpecialKey(key))
		return TRUE;

	return CXTPPopupToolBar::ProcessSpecialKey(key);
}


BEGIN_MESSAGE_MAP(CXTPRibbonGroupPopupToolBar, CXTPPopupToolBar)
	//{{AFX_MSG_MAP(CXTPRibbonBar)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPRibbonGroupPopupToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCommandBars* pCommandBars = GetCommandBars ();
	CXTPMouseManager* pMouseManager = pCommandBars->GetMouseManager();

	BOOL bMouseLocked = pMouseManager->IsMouseLocked() || !pMouseManager->IsTopParentActive(m_hWnd)
		|| pMouseManager->IsTrackedLock(this) || m_nPopuped != -1;

	CXTPRibbonGroup* pHighlightedGroup = bMouseLocked || !m_pRibbonBar->GetRibbonPaintManager()->m_bHotTrackingGroups ?
		NULL : m_pRibbonGroup;

	if (pHighlightedGroup != m_pHighlightedGroup)
	{
		HighlightGroup(pHighlightedGroup);

		if (m_pHighlightedGroup)
		{
			pMouseManager->TrackMouseLeave(*this);
		}
	}

	CXTPPopupToolBar::OnMouseMove(nFlags, point);
}

BOOL CXTPRibbonGroupPopupToolBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	HighlightGroup(NULL);

	return CXTPPopupToolBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard);
}

void CXTPRibbonGroupPopupToolBar::OnMouseLeave()
{
	HighlightGroup(NULL);

	CXTPPopupToolBar::OnMouseLeave();
}

BOOL CXTPRibbonGroupPopupToolBar::IsKeyboardCuesVisible() const
{
	return m_pRibbonBar->IsKeyboardCuesVisible();
}
