// XTPCommandBar.cpp : implementation of the CXTPCommandBar class.
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
#include <Common/XTPImageManager.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPMouseManager.h>
#include <CommandBars/XTPKeyboardManager.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPShortcutManager.h>
#include <CommandBars/Customize/XTPCustomizeTools.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBarAnimation.h>
#include <CommandBars/XTPControlCustom.h>
#include <CommandBars/XTPCommandBarList.h>
#include <CommandBars/XTPCommandBarsOptions.h>
#include <CommandBars/XTPCommandBarKeyboardTip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef CBRS_GRIPPER
#define CBRS_GRIPPER                    0x00400000L
#endif

int CXTPCommandBar::s_nHoverDelay = 300;
int CXTPCommandBar::s_nExpandDelay = 5000;
int CXTPCommandBar::s_nExpandHoverDelay = 900;

AFX_INLINE BOOL IsCustomizeCommand(int nID)
{
	return (nID >= XTP_ID_CUSTOMIZE_RESET && nID <= XTP_ID_CUSTOMIZE_BEGINAGROUP);
}

//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarList


CXTPCommandBarList::~CXTPCommandBarList()
{
	RemoveAll();
}

CXTPCommandBar* CXTPCommandBarList::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrBars.GetSize() ? m_arrBars[nIndex] : NULL;
}

int CXTPCommandBarList::GetCount() const
{
	return (int)m_arrBars.GetSize();
}

CXTPCommandBar* CXTPCommandBarList::Add(CXTPCommandBar* pCommandBar)
{
	m_arrBars.Add(pCommandBar);
	return pCommandBar;
}

BOOL CXTPCommandBarList::Lookup(CXTPCommandBar* pCommandBar) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (GetAt(i) == pCommandBar)
			return TRUE;
	}
	return FALSE;
}

void CXTPCommandBarList::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPCommandBar* pBar = GetAt(i);
		pBar->InternalRelease();
	}
	m_arrBars.RemoveAll();
}

void CXTPCommandBarList::Remove(CXTPCommandBar* pCommandBar)
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (GetAt(i) == pCommandBar)
		{
			m_arrBars.RemoveAt(i);
			pCommandBar->InternalRelease();
		}
	}
}

CXTPCommandBar* CXTPCommandBarList::FindCommandBar(UINT nBarID) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPCommandBar* pBar = GetAt(i);
		if (pBar->GetBarID() == nBarID)
			return pBar;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPCommandBar

IMPLEMENT_XTP_COMMANDBAR(CXTPCommandBar, CWnd)

CXTPCommandBar::CXTPCommandBar()
{
	EnableAutomation();

#ifdef _XTP_ACTIVEX
	m_bAutoMenuLoad = FALSE;
	EnableTypeLib();
#endif

	m_pCommandBars = 0;
	m_nIdleFlags = 0;
	m_barType = xtpBarTypeNormal;
	m_buttonStyle = xtpButtonAutomatic;

	m_pControls = (CXTPControls*) CXTPControls::CreateObject();
	m_pControls->SetParent(this);

	m_barPosition = xtpBarNone;
	m_bTracking = FALSE;

	m_nClickedControl = -1;
	m_nSelected = -1;
	m_nPopuped = -1;
	m_hwndFocus = 0;
	m_dwFlags = 0;
	m_pOwner = NULL;
	m_bGrabFocus = TRUE;

	m_nBarID = 0;
	m_pSite = 0;

	m_szButtonSpacing = CSize(0, 0);

	m_nTextOrientation = xtpBarTextAuto;

	m_szIcons = CSize(0, 0);
	m_szButtons = CSize(0, 0);

	m_nMRUWidth = 32767;

	m_dwStyle = CBRS_GRIPPER;
	m_bCustomizable = TRUE;

	m_nStateFlags = 0;

	m_rcMarker.SetRectEmpty();
	m_dwData = 0;
	m_pReturnCmd = NULL;
	m_bTrackOnHover = TRUE;

	m_bCustomizeDialogPresent = TRUE;
	m_bTextBelow = XTP_BOOL_DEFAULT;
	m_bExecOnRButton = FALSE;
	m_bIgnoreUpdateHandler = FALSE;
	m_bRecursePopup = FALSE;
	m_nLockRecurse = 0;

	m_pPaintManager = NULL;
	m_pImageManager = NULL;

	m_bKeyboardSelect = FALSE;

	m_nLockRedraw = 0;
	m_pToolTipContext = 0;

	m_bComboBar = FALSE;

	m_bMultiLine = FALSE;

	m_bAnimatePopup = FALSE;
	m_bVerticalMarker = FALSE;

	m_pAnimation = new CXTPCommandBarAnimation(this);

	m_pMouseMoveLockControl = NULL;
}

CXTPCommandBar::~CXTPCommandBar()
{
	if(NULL != m_pMouseMoveLockControl)
	{
		UnlockMouseMove();
	}

	if (IsTrackingMode())
	{
		SetTrackingMode(FALSE);
	}

	XTPHookManager()->RemoveAll(this);
	XTPMouseManager()->RemoveSelected(this);

	if (m_pControls)
	{
		m_pControls->RemoveAll();
		m_pControls->SetParent(NULL);

		CMDTARGET_RELEASE(m_pControls);
	}

	CMDTARGET_RELEASE(m_pPaintManager);

	CMDTARGET_RELEASE(m_pImageManager)

	CMDTARGET_RELEASE(m_pToolTipContext);

	delete m_pAnimation;
}

void CXTPCommandBar::SetControls(CXTPControls* pControls)
{
	CMDTARGET_RELEASE(m_pControls);

	m_pControls = pControls;

	if (m_pControls)
	{
		m_pControls->SetParent(this);
	}
}

void CXTPCommandBar::OnRemoved()
{
	if (IsTrackingMode()) SetTrackingMode(FALSE);

	XTPHookManager()->RemoveAll(this);
	XTPMouseManager()->RemoveSelected(this);

	if (m_pControls)
	{
		m_pControls->RemoveAll();
	}

	DestroyWindow();
}

void CXTPCommandBar::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CXTPCommandBar, CWnd)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_MESSAGE_VOID(WM_INITIALUPDATE, OnInitialUpdate)
	ON_WM_CREATE()
	ON_WM_DESTROY()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)

	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)

	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_NCHITTEST_EX()
	ON_WM_NCLBUTTONDOWN()

	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCMOUSEMOVE()

	ON_MESSAGE(WM_GETOBJECT, OnGetObject)

END_MESSAGE_MAP()


// CXTPCommandBar message handlers

int CXTPCommandBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars() ;

	if (m_barPosition != xtpBarListBox && pCommandBars)
	{
		pCommandBars->GetDropSource()->Register(this);
	}

	if (pCommandBars)
	{
		CMDTARGET_RELEASE(m_pToolTipContext);

		m_pToolTipContext = pCommandBars->GetToolTipContext();
		CMDTARGET_ADDREF(m_pToolTipContext);
	}
	else if (m_barType != xtpBarTypePopup)
	{
		EnableToolTips();
	}

	return 0;
}
void CXTPCommandBar::OnDestroy()
{
	if (m_barPosition != xtpBarListBox)
	{
		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars)
		{
			pCommandBars->GetDropSource()->UnRegister(this);
		}
	}

	if (IsTrackingMode()) SetTrackingMode(FALSE);

	m_pAnimation->OnDestroy();

	CWnd::OnDestroy();
}

BOOL CXTPCommandBar::IsMouseLocked() const
{
	return !m_bComboBar && (GetCommandBars()->GetMouseManager()->IsMouseLocked());

}

void CXTPCommandBar::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	CXTPCommandBars* pCommandBars = GetCommandBars ();
	CXTPMouseManager* pMouseManager = pCommandBars->GetMouseManager();

	BOOL bActive = IsTrackingMode() || pMouseManager->IsTopParentActive(m_hWnd);

	if (!m_hWnd || pMouseManager->IsTrackedLock(this) ||
		(!bActive && GetPosition() != xtpBarPopup))
		return;

	if(IsMouseLocked())
	{
		if(NULL != m_pMouseMoveLockControl)
		{
			CXTPControl* pControl = m_pControls->HitTest(point);
			if (m_pMouseMoveLockControl == pControl)
			{
				pMouseManager->TrackMouseLeave(*this);
				pControl->OnMouseMove(point);
				SetSelected(pControl->GetIndex());
			}
			else
			{
				m_pMouseMoveLockControl->OnSetSelected(TRUE);
			}
		}

		return;
	}

	if (pCommandBars && pCommandBars->IsCustomizeMode())
	{
		CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, pCommandBars->GetDragControl());
		if (pControl)
			pControl->OnCustomizeMouseMove(point);
		return;
	}

	if (m_nPopuped != -1 && !m_bTrackOnHover)
		return;

	BOOL bLockSelected = ((GetPosition() != xtpBarPopup) || m_nPopuped != -1) || m_bKeyboardSelect;
	int nSelected = (IsTrackingMode() && bLockSelected) ? m_nSelected : -1;

	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl)
	{
		nSelected = pControl->GetIndex();
		pMouseManager->TrackMouseLeave(*this);
		pControl->OnMouseMove(point);
	}

	SetSelected(nSelected);
}

void CXTPCommandBar::OnMouseLeave()
{
	if (!IsMouseLocked() && !(IsTrackingMode() && (GetPosition() != xtpBarPopup || m_nPopuped != -1)) && (m_nLockRecurse == 0))
		SetSelected(-1);
}
void CXTPCommandBar::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	OnMouseLeave();
	CWnd::OnNcMouseMove(nHitTest, point);
}

void CXTPCommandBar::OnTrackEnter()
{
	SetSelected(-1);
}



void CXTPCommandBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPControl* pControl = m_pControls->HitTest(point);

	if ((m_nPopuped != -1) && (GetPosition() == xtpBarPopup && GetType() == xtpBarTypeNormal)
		&& (!pControl || pControl->GetIndex() != m_nPopuped))
	{
		if (m_nPopuped == m_nSelected) SetSelected(-1);
		SetPopuped(-1);
	}

	if (pControl)
	{

		if (!IsCustomizeMode() && (pControl->IsCustomizeMovable()) &&
			DYNAMIC_DOWNCAST(CXTPToolBar, this) && (GetKeyState(VK_MENU) < 0))
		{
			CXTPCommandBars* pCommandBars = GetCommandBars();
			if (pCommandBars && pCommandBars->IsCustomizeAvail() && pCommandBars->GetCommandBarsOptions()->bAltDragCustomization)
			{
				pCommandBars->SetQuickCustomizeMode(TRUE);

				SetPopuped(-1);
				SetSelected(-1);
				pControl->CustomizeStartDrag(point);

				pCommandBars->SetQuickCustomizeMode(FALSE);
				pCommandBars->RecalcFrameLayout();
				pCommandBars->ClosePopups();
				return;
			}
		}


		pControl->OnLButtonDown(point);
		return;
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPCommandBar::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	if (IsCustomizeMode())
		return;

	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl)
	{
		pControl->OnLButtonDblClk(point);
	}
}

void CXTPCommandBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsCustomizeMode())
		return;

	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl)
	{
		pControl->OnLButtonUp(point);
		return;
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPCommandBar::OnRButtonUp(UINT /*nFlags*/, CPoint point)
{
	if (IsCustomizeMode())
		return;

	CXTPControl* pControl = m_pControls->HitTest(point);

	if (pControl)
	{
		if (GetSite()->SendMessage(WM_XTP_CONTROLRBUTTONUP, (WPARAM)(LPPOINT)&point, (LPARAM)pControl) == TRUE)
			return;

		if (GetRootParent()->m_bExecOnRButton)
		{
			pControl->OnLButtonUp(point);
			return;
		}

		if (pControl->OnRButtonUp(point))
			return;
	}

	CXTPCommandBar* pCommandBar = GetRootParent();
	if (pCommandBar && pCommandBar != this)
	{
		pCommandBar->OnPopupRButtonUp(this, point);
	}
}

void CXTPCommandBar::EnsureVisible(CXTPControl* /*pControl*/)
{

}

BOOL CXTPCommandBar::SetSelected(int nSelected, BOOL bKeyboard)
{
	if (m_nSelected == nSelected)
		return FALSE;

	m_bKeyboardSelect = bKeyboard;

	if (!GetSafeHwnd())
		return FALSE;


	if (m_nSelected != -1)
	{
		CXTPControl* pSelected = GetControl(m_nSelected);
		if (pSelected) pSelected->OnMouseMove(CPoint(-1, -1));
		if (pSelected) pSelected->OnSetSelected(FALSE);
	}

	m_nSelected = nSelected;

	if (m_nSelected != -1)
	{
		CXTPControl* pSelected = GetControl(nSelected);
		ASSERT(pSelected);
		if (!pSelected)
			return FALSE;

		if (bKeyboard)
			EnsureVisible(pSelected);

		pSelected->OnSetSelected(bKeyboard ? max(TRUE_KEYBOARD_NEXT, bKeyboard) : TRUE);

		if (IsTrackingMode()) AccessibleNotifyWinEvent(EVENT_OBJECT_FOCUS , m_hWnd, OBJID_MENU, m_nSelected + 1);

		XTPMouseManager()->SetSelected(this);
		if (!bKeyboard && m_bTrackOnHover) SetTimer(XTP_TID_HOVER, s_nHoverDelay, NULL);
	}
	else
	{
		KillTimer(XTP_TID_HOVER);
	}

	return TRUE;
}

BOOL CXTPCommandBar::SetPopuped(int nPopuped, BOOL bKeyboard)
{
	if (m_nPopuped == nPopuped)
		return FALSE;

	if (m_nPopuped != -1)
	{
		CXTPControl* pControl = GetControl(m_nPopuped);
		m_nPopuped = -1;
		pControl->OnSetPopup(FALSE);
	}

	if (nPopuped != -1)
	{
		CXTPControl* pControl = GetControl(nPopuped);
		if (!pControl || !pControl->GetEnabled())
		{
			return FALSE;
		}

		m_nPopuped = nPopuped;
		SetTrackingMode(TRUE_POPUP, FALSE, bKeyboard);
		SetSelected(nPopuped, bKeyboard);

		if (!m_bTracking)
		{
			SetSelected(-1);
			m_nPopuped = -1;
			return FALSE;
		}

		InternalAddRef();

		if (!pControl->OnSetPopup(TRUE))
		{
			m_nPopuped = -1;

			InternalRelease();
			return FALSE;
		}

		InternalRelease();

		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars && pCommandBars->GetCommandBarsOptions()->bShowFullAfterDelay && m_hWnd)
		{
			SetTimer(XTP_TID_EXPANDED_POPUP, s_nExpandDelay, NULL);
		}

	}

	return TRUE;
}


LRESULT CXTPCommandBar::OnIdleUpdateCmdUI(WPARAM, LPARAM)
{
	if (GetSafeHwnd() && (GetStyle() & WS_VISIBLE))
	{
		UpdateDocTemplateControls();

		OnUpdateCmdUI();

		if (m_nIdleFlags & xtpIdleRedraw)
		{
			Redraw();
		}
		if (m_nIdleFlags & xtpIdleLayout)
		{
			OnRecalcLayout();
			m_nIdleFlags &= ~xtpIdleLayout;
		}

	}
	return 0L;
}

class CXTPCommandBar::CCommandBarCmdUI : public CCmdUI
{
public:
	CCommandBarCmdUI()
	{
		m_bCheckedChanged = FALSE;
		m_pControl = 0;
	}

public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);

	BOOL DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHandler);

public:
	CXTPControl* m_pControl;
	BOOL m_bCheckedChanged;

};

void CXTPCommandBar::CCommandBarCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;

	ASSERT(m_pControl != NULL);
	if (!m_pControl)
		return;

	if (m_pControl->GetAction())
	{
		m_pControl->SetEnabled(bOn ? -1 : FALSE);
	}
	else
	{
		m_pControl->SetEnabled(bOn);

		if (!bOn && !m_bCheckedChanged) m_pControl->SetChecked(FALSE);
	}
}

void CXTPCommandBar::CCommandBarCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0 =>off, 1 =>on, 2 =>indeterminate
	m_bCheckedChanged = TRUE;

	ASSERT(m_pControl != NULL);
	if (!m_pControl)
		return;

	m_pControl->SetChecked(nCheck);
}

void CXTPCommandBar::CCommandBarCmdUI::SetText(LPCTSTR lpszCaption)
{
	ASSERT(m_pControl != NULL);
	if (!m_pControl)
		return;

	m_pControl->SetCaption(lpszCaption);
}

BOOL CXTPCommandBar::CCommandBarCmdUI::DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHandler)
{
	BOOL bHandled = CXTPToolBar::DoUpdateCustomizeMenuControl(m_pControl, pTarget);
	if (!bHandled)
	{
		bHandled = CCmdUI::DoUpdate(pTarget, bDisableIfNoHandler);
	}
	return bHandled;
}


void CXTPCommandBar::OnInitialUpdate()
{
	// update the indicators before becoming visible
	OnIdleUpdateCmdUI(TRUE, 0L);
}

void CXTPCommandBar::OnUpdateCmdUI()
{
	if (!GetSafeHwnd())
		return;

	CWnd* pTarget = GetOwnerSite();
	CXTPCommandBars* pCommandBars = GetCommandBars();
	BOOL bDisableIfNoHndler = pCommandBars ? pCommandBars->GetCommandBarsOptions()->bDisableCommandIfNoHandler : FALSE;
	BOOL bIgnoreUpdateHandler = GetRootParent()->m_bIgnoreUpdateHandler;

	if (!pTarget)
		return;

	CXTPControl* pControl;

#ifdef _XTP_ACTIVEX
	CWnd* pSite = GetSite();
	BOOL bCustomTarget = pSite != pTarget;

	if (bCustomTarget)
	{
		for (int i = 0; i < GetControlCount(); i++)
		{
			pControl = GetControl(i);
			if (!pControl || pControl->GetParent() != this)
				continue;

			if (bIgnoreUpdateHandler)
				continue;

			if (!IsCustomizeCommand(pControl->GetID()))
			{
				if (pSite->SendMessage(WM_XTP_UPDATE_COMMAND, (WPARAM)pControl))
					pControl->m_dwFlags |= xtpFlagManualUpdate;
			}
		}

	}
	if (pSite == pTarget && pCommandBars != NULL)
	{
		for (int i = 0; i < GetControlCount(); i++)
		{
			pControl = GetControl(i);
			if (!pControl || pControl->GetParent() != this)
				continue;

			if (!pControl->IsVisible(xtpHideScroll | xtpHideGeneric | xtpHideExpand))
				continue;

			if (bIgnoreUpdateHandler)
				continue;

			if (IsCustomizeMode() && !pCommandBars->IsQuickCustomizeMode())
			{
				if (pControl->m_bEnabled != (BOOL)-1)
				{
					pControl->SetEnabled(TRUE);
				}
			}
			else
				pTarget->SendMessage(WM_XTP_UPDATE_COMMAND, (WPARAM)pControl);
		}
	}
	else
	{


#endif

	CCommandBarCmdUI state;
	state.m_pOther = this;

	for (state.m_nIndex = 0; (int)state.m_nIndex < GetControlCount(); state.m_nIndex++)
	{
		pControl = m_pControls->GetAt(state.m_nIndex);

		ASSERT(pControl);
		if (!pControl)
			continue;

		if (!pControl->IsVisible(xtpHideScroll | xtpHideGeneric | xtpHideExpand))
			continue;

		pControl->OnIdleUpdate();

		if (bIgnoreUpdateHandler || (pControl->m_dwFlags & xtpFlagManualUpdate))
			continue;

#ifdef _XTP_ACTIVEX
		if (bCustomTarget && pCommandBars && (pControl->GetID() < 9000 || pControl->GetID() > 10000))
			continue;
#endif


		state.m_pControl = pControl;
		state.m_nID = pControl->GetID();
		state.m_bCheckedChanged = FALSE;
		state.m_bEnableChanged = FALSE;

		if (CWnd::OnCmdMsg(0,
			MAKELONG((int)CN_UPDATE_COMMAND_UI, WM_COMMAND + WM_REFLECT_BASE),
			&state, NULL))
			continue;

		// allow the toolbar itself to have update handlers
		if (CWnd::OnCmdMsg(state.m_nID, (int)CN_UPDATE_COMMAND_UI, &state, NULL))
			continue;

		if (pCommandBars && pCommandBars->IsCustomizeMode())
		{
			if (pControl->m_bEnabled != (BOOL)-1)
			{
				state.Enable(TRUE);
			}
		}
		else
		{
			// allow the owner to process the update
			state.DoUpdate(pTarget, bDisableIfNoHndler);
		}
	}
#ifdef _XTP_ACTIVEX
	}
#endif

	// update the dialog controls added to the toolbar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);

}

BOOL CXTPControls::LoadMenu(CMenu* pMenu)
{
	if (!pMenu)
		return FALSE;

	BOOL bSeparator = FALSE;
	int nCount = ::GetMenuItemCount(pMenu->m_hMenu);
	MENUITEMINFO info;

	for (int i = 0; i < nCount; i++)
	{
		ZeroMemory(&info, sizeof(MENUITEMINFO));
		info.cbSize = sizeof(MENUITEMINFO);
		info.fMask = MIIM_TYPE | MIIM_STATE;

		// Check to see if the item is a separator, we don't want
		// to use GetMenuItemID(i) because it may not equal zero.
		::GetMenuItemInfo(pMenu->m_hMenu, i, TRUE, &info);

		if (((info.fType & MFT_SEPARATOR) == MFT_SEPARATOR) || (pMenu->GetMenuItemID(i) == 0))
			bSeparator = TRUE;
		else
		{
			CXTPControl* pControl = AddMenuItem(pMenu, i);

			if (!pControl)
				continue;

			if (((info.fType & MFT_MENUBARBREAK) == MFT_MENUBARBREAK) || ((info.fType & MFT_MENUBREAK) == MFT_MENUBREAK))
			{
				pControl->SetFlags(pControl->GetFlags() | xtpFlagWrapRow);
				pControl->SetBeginGroup((info.fType & MFT_MENUBARBREAK) ? TRUE : FALSE);
			}
			if (bSeparator)
			{
				pControl->SetBeginGroup(TRUE);
				bSeparator = FALSE;
			}
			if (pControl->GetAction() == NULL && info.fState & MFS_CHECKED)
			{
				pControl->SetChecked(TRUE);
			}
			if (pControl->GetAction() == NULL && info.fState & MFS_DISABLED)
			{
				pControl->SetEnabled(FALSE);
			}
		}
	}
	return TRUE;
}

BOOL CXTPCommandBar::LoadMenu(CMenu* pMenu, BOOL bRemoveControls)
{
	if (!pMenu || !::IsMenu(pMenu->m_hMenu))
		return FALSE;

	if (bRemoveControls)
		m_pControls->RemoveAll();

	if (!m_pControls->LoadMenu(pMenu))
		return FALSE;

	CFrameWnd* pFrame = GetParentFrame();
	if (pFrame) pFrame->DelayRecalcLayout();
	return TRUE;
}

BOOL CXTPCommandBar::ProcessSpecialKey(XTPSpecialKey key)
{
	if (key == xtpKeySpace)
	{
		if (GetType() == xtpBarTypeMenuBar)
		{
			GetCommandBars()->ClosePopups();
			GetSite()->PostMessage(WM_SYSCOMMAND, SC_KEYMENU, (LPARAM)_T(' '));
			return TRUE;
		}
		return FALSE;
	}

	if (key == xtpKeyLeft) key = xtpKeyPrev;
	if (key == xtpKeyRight) key = xtpKeyNext;

	BOOL bPopupToolBar = (GetPosition() == xtpBarPopup && GetType() == xtpBarTypeNormal);

	if (key == xtpKeyNext || key == xtpKeyPrev)
		HideKeyboardTips();

	if (m_nSelected != -1 && m_nSelected == m_nPopuped && bPopupToolBar)
	{
		if (key == xtpKeyNext)
			return FALSE;

		if (key == xtpKeyPrev)
			key = xtpKeyBack;
	}
	else if (key == xtpKeyBack && m_nPopuped == -1 && m_nSelected != -1 && (bPopupToolBar || !GetParentCommandBar()))
	{
		CXTPControlPopup* pControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, GetControl(m_nSelected));
		if (pControlPopup && pControlPopup->GetEnabled())
			key = xtpKeyPopup;
	}
	if (((key == xtpKeyPopup) || (key == xtpKeyBack)) && m_nPopuped == -1 && (bPopupToolBar || (GetPosition() == xtpBarFloating)))
	{
		int nDirection = key == xtpKeyPopup ? +1 : -1;

		if (m_nSelected == -1)
		{
			SetSelected(m_pControls->GetNext(-1, nDirection), nDirection ? TRUE_KEYBOARD_NEXT : TRUE_KEYBOARD_PREV);
			return TRUE;
		}

		CXTPControlPopup* pControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, GetControl(m_nSelected));
		if (!pControlPopup || !pControlPopup->GetEnabled())
		{
			CXTPControl* pSelected = GetControl(m_nSelected), *pNextSelected = NULL;
			int nSelected = m_nSelected;
			ASSERT(pSelected);
			CRect rcSelected = pSelected->GetRect();
			CRect rcSelectedRow = pSelected->m_rcRow;

			do
			{
				nSelected = m_pControls->GetNext(nSelected, nDirection);
				ASSERT(nSelected != -1);

				CXTPControl* pCondidate = GetControl(nSelected);
				CRect rcNextSelected = pCondidate->GetRect(), rcNextSelectedRow = pCondidate->GetRowRect();

				if (pNextSelected && (pNextSelected->m_rcRow.top == rcNextSelectedRow.top)
					&& ((rcNextSelected.left <= rcSelected.left && (rcNextSelected.left > pNextSelected->GetRect().left))
					|| (rcNextSelected.left >= rcSelected.left && (pNextSelected->GetRect().left > rcNextSelected.left))))
				{
					pNextSelected = pCondidate;
				}
				else if (!pNextSelected && rcNextSelectedRow.top != rcSelectedRow.top && !rcNextSelectedRow.IsRectEmpty())
				{
					pNextSelected = pCondidate;
				}
				else if (pNextSelected)
				{
					SetSelected(pNextSelected->GetIndex(), nDirection > 0 ? TRUE_KEYBOARD_NEXT : TRUE_KEYBOARD_PREV);
					break;
				}

			}
			while ((nSelected != m_nSelected) && (nSelected != -1));

			return TRUE;
		}
	}

	switch (key)
	{
		case xtpKeyNext:
			SetSelected(m_pControls->GetNext(m_nSelected, +1), TRUE_KEYBOARD_NEXT);
			return TRUE;

		case xtpKeyPrev:
			SetSelected(m_pControls->GetNext(m_nSelected, -1), TRUE_KEYBOARD_PREV);
			return TRUE;

		case xtpKeyHome:
			SetSelected(m_pControls->GetNext(-1, +1), TRUE_KEYBOARD_NEXT);
			return TRUE;

		case xtpKeyEnd:
			SetSelected(m_pControls->GetNext(GetControlCount(), -1), TRUE_KEYBOARD_PREV);
			return TRUE;

		case xtpKeyPopup:
			if (m_nSelected != -1 && m_nSelected != m_nPopuped)
			{
				GetControl(m_nSelected)->OnSetSelected(TRUE_KEYBOARD_NEXT);
				return SetPopuped(m_nSelected);
			}
			break;

		case xtpKeyBack:
			if (m_nPopuped != -1)
			{
				SetPopuped(-1);
				SetTrackingMode(TRUE, FALSE);
				return TRUE;
			}
			break;

		case xtpKeyEscape:
			{
				int nLevel = IsKeyboardTipsVisible();
				if (nLevel > 1)
				{
					GetCommandBars()->ShowKeyboardTips(this, nLevel - 1);
					return TRUE;
				}
				if (GetParentCommandBar() != NULL)
					GetParentCommandBar()->SetTrackingMode(TRUE, FALSE);

				SetTrackingMode(FALSE);
			}
			return TRUE;

		case xtpKeyReturn:
			if (m_nSelected != -1)
			{
				GetControl(m_nSelected)->OnSetSelected(TRUE_KEYBOARD_NEXT);
				GetControl(m_nSelected)->OnClick(TRUE);
			}
			return TRUE;

	}
	return FALSE;
}

BOOL CXTPCommandBar::IsHelpMode() const
{
	CFrameWnd* pFrame = GetFrameSite();
	return pFrame && pFrame->m_bHelpMode;
}

BOOL CXTPCommandBar::OnHookKeyDown(UINT nChar, LPARAM lParam)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	if (pCommandBars && pCommandBars->IsCustomizeMode())
	{
		return FALSE_EXIT;
	}

	if (IsHelpMode())
		return FALSE_EXIT;

	if (m_nLockRecurse > 0)
		return TRUE;

	InternalAddRef();

	GetSite()->SendMessage(WM_XTP_COMMANDBARKEYDOWN, (WPARAM)&nChar, (LPARAM)this);

	if (InternalRelease() == 0)
		return TRUE;

	CXTPControl* pControl = GetControl(m_nSelected);

	if (pControl && pControl->IsFocused())
	{
		BOOL bResult = pControl->OnHookKeyDown(nChar, lParam);

		if ((bResult == TRUE) || (bResult == FALSE_EXIT))
			return bResult;
	}

	if (!GetSafeHwnd())
		return TRUE;

	switch (nChar)
	{
		case VK_TAB:
		{
			pControl = GetControl(m_nSelected);

			if (DYNAMIC_DOWNCAST(CXTPControlCustom, pControl) && ::GetFocus() && ::IsChild(m_hWnd, ::GetFocus()))
				return FALSE_EXIT;

			if ((GetKeyState(VK_CONTROL) < 0) && pCommandBars)
			{
				CXTPCommandBar* pToolBar = GetRootParent();

				if (DYNAMIC_DOWNCAST(CXTPToolBar, pToolBar))
				{
					if (pCommandBars->SetToolBarFocus((CXTPToolBar*)pToolBar, GetKeyState(VK_SHIFT) >= 0 ? +1 : -1))
						return TRUE;
				}
			}

			ProcessSpecialKey(GetKeyState(VK_SHIFT) >= 0 ? xtpKeyNext : xtpKeyPrev);

			pControl = GetControl(m_nSelected);

			if (pControl && pControl->IsFocusable())
				pControl->SetFocused(TRUE);

			return TRUE;
		}

		case VK_RETURN: return ProcessSpecialKey(xtpKeyReturn);
		case VK_ESCAPE: return ProcessSpecialKey(xtpKeyEscape);

		case VK_SPACE:
			if (ProcessSpecialKey(xtpKeySpace))
				return TRUE;
	}

	if (::GetFocus() && ::IsChild(m_hWnd, ::GetFocus()))
		return FALSE_EXIT;

	BOOL bHoriz = m_barPosition == xtpBarTop || m_barPosition == xtpBarBottom || m_barPosition == xtpBarFloating
		|| (m_barPosition == xtpBarPopup && m_barType == xtpBarTypeNormal) ;

	BOOL bLayoutRTL = IsLayoutRTL();

	switch (nChar)
	{
		case VK_LEFT: return ProcessSpecialKey(bHoriz ? (bLayoutRTL ? xtpKeyRight: xtpKeyLeft) : (bLayoutRTL ? xtpKeyPopup : xtpKeyBack));
		case VK_RIGHT: return ProcessSpecialKey(bHoriz ? (bLayoutRTL ? xtpKeyLeft : xtpKeyRight) : (!bLayoutRTL ? xtpKeyPopup : xtpKeyBack));
		case VK_UP: return ProcessSpecialKey(bHoriz ? xtpKeyBack : xtpKeyPrev);
		case VK_DOWN: return ProcessSpecialKey(bHoriz ? xtpKeyPopup : xtpKeyNext);
		case VK_HOME: return ProcessSpecialKey(xtpKeyHome);
		case VK_END: return ProcessSpecialKey(xtpKeyEnd);
	}

	if (nChar && MapVirtualKey(nChar, 2))
	{
		if (IsKeyboardTipsVisible() && GetKeyState(VK_CONTROL) >= 0)
		{
			OnKeyboardTipChar(nChar);
			return TRUE;
		}

		if (pCommandBars && DYNAMIC_DOWNCAST(CXTPToolBar, this))
		{
			if (pCommandBars->OnFrameAccel(nChar))
				return TRUE;
		}

		BOOL bSelectOnly = FALSE;
		int nIndex = GetKeyState(VK_CONTROL) < 0 ? -1 : FindAccelEx(nChar, bSelectOnly);
		if (nIndex != -1)
		{
			SetSelected(nIndex, TRUE_KEYBOARD_NEXT);

			if (!bSelectOnly)
			{
				GetControl(nIndex)->OnUnderlineActivate();
			}
			return TRUE;
		}
		if (nChar != VK_SHIFT && nChar != VK_CONTROL) MessageBeep(0);
	}

	return TRUE;
}

void CXTPCommandBar::HideKeyboardTips()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars) pCommandBars->HideKeyboardTips();
}

void CXTPCommandBar::LockMouseMove(CXTPControl* pControl)
{
	ASSERT(NULL != pControl);

	UnlockMouseMove();

	m_pMouseMoveLockControl = pControl;
	XTPMouseManager()->LockMouseMove();
}

void CXTPCommandBar::UnlockMouseMove()
{
	if(NULL != m_pMouseMoveLockControl)
	{
		XTPMouseManager()->UnlockMouseMove();
		m_pMouseMoveLockControl = NULL;
	}
}

void CXTPCommandBar::OnKeyboardTip(CXTPCommandBarKeyboardTip* pTip)
{
	CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, pTip->m_pOwner);
	if (pControl)
	{
		pControl->OnUnderlineActivate();
		return;
	}

	CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, pTip->m_pOwner);
	if (pToolBar)
	{
		pToolBar->GetCommandBars()->SetToolBarFocus(pToolBar, FALSE);
	}
}

void CXTPCommandBar::OnKeyboardTipChar(UINT chAccel)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return;

	for (int i = 0; i < (int)pCommandBars->m_keyboardTips.arr.GetSize(); i++)
	{
		CXTPCommandBarKeyboardTip* pTip = pCommandBars->m_keyboardTips.arr[i];
		if (pTip->m_strTip.GetLength() > pCommandBars->m_keyboardTips.nKey)
		{
			if (CXTPShortcutManager::CompareAccelKey(pTip->m_strTip[pCommandBars->m_keyboardTips.nKey], chAccel))
			{
				if (pTip->m_strTip.GetLength() - 1 > pCommandBars->m_keyboardTips.nKey)
				{
					pCommandBars->m_keyboardTips.nKey++;

					CString str = pTip->m_strTip.Left(pCommandBars->m_keyboardTips.nKey);
					for (int j = (int)pCommandBars->m_keyboardTips.arr.GetSize() - 1; j >= 0; j--)
					{
						pTip = pCommandBars->m_keyboardTips.arr[j];
						if (pTip->m_strTip.Left(pCommandBars->m_keyboardTips.nKey) != str)
						{
							pCommandBars->m_keyboardTips.arr.RemoveAt(j);
							pTip->DestroyWindow();
							delete pTip;
						}
					}
				}
				else
				{
					if (pTip->m_bEnabled)
						OnKeyboardTip(pTip);
					else
						MessageBeep(0);
				}
				return;
			}
		}
	}

	if (chAccel != VK_SHIFT && chAccel != VK_CONTROL) MessageBeep(0);
}

BOOL CXTPCommandBar::Popup(CXTPControlPopup* /*pControlPopup*/, BOOL)
{
	return FALSE;
}

BOOL CXTPCommandBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

int CXTPCommandBar::OnHookMessage(HWND /*hWnd*/, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& /*lResult*/)
{
	if (IsTrackingMode())
	{
		switch (nMessage)
		{
			case WM_KEYDOWN:
				return OnHookKeyDown((UINT)wParam, lParam);

			case WM_MOUSEWHEEL:
				{
					CXTPControl* pControl = GetControl(m_nSelected);

					if (pControl && pControl->IsFocused())
					{
						pControl->OnHookMouseWheel(LOWORD(wParam), (short) HIWORD(wParam),
							CPoint(LOWORD(lParam), HIWORD(lParam)));
					}
					else
					{
						HWND hWnd = ::GetFocus();
						if (hWnd && hWnd != m_hWnd && ::IsChild(m_hWnd, hWnd))
						{
							return FALSE_EXIT;
						}
					}

					return TRUE;
				}
		}
	}
	return FALSE;
}

void CXTPCommandBar::OnTrackLost()
{
	SetTrackingMode(FALSE);
}

BOOL CXTPCommandBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	if (m_bTracking && bMode || m_bTracking == bMode)
	{
		m_bTracking = bMode;
		return FALSE;
	}

	m_bTracking = bMode;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	CXTPMouseManager* pMouseManager = pCommandBars->GetMouseManager();

	if (bMode)
	{
		if (GetType() == xtpBarTypeMenuBar)
			GetSite()->SendMessage(WM_XTP_INITMENU, (WPARAM)this);

		m_bAnimatePopup = !bSelectFirst && !bKeyboard;
		m_bKeyboardSelect = bKeyboard;

		if (m_bGrabFocus)
		{
			if (m_hwndFocus == 0)
				m_hwndFocus = ::GetFocus() ? ::GetFocus() : GetSite()->GetSafeHwnd();
			::HideCaret(m_hwndFocus);
		}

		pMouseManager->SetTrack(this, FALSE);

		XTPKeyboardManager()->HookKeyboard(this);

		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		if (bSelectFirst)
		{
			SetSelected(m_pControls->GetNext(-1, +1), TRUE_KEYBOARD_NEXT);
		}

	}
	else
	{
		m_bAnimatePopup = FALSE;
		m_bKeyboardSelect = FALSE;

		pMouseManager->RemoveTrack(this);

		XTPKeyboardManager()->UnhookKeyboard(this);

		SetPopuped(-1);
		if (bSelectFirst) SetSelected(-1);

		if (m_hwndFocus && m_bGrabFocus)
		{
			::ShowCaret(m_hwndFocus);
			m_hwndFocus = 0;
		}

		Redraw();

		if (!pMouseManager->IsTrackedLock())
			pMouseManager->SetForceExpanded(FALSE);

		if (m_hWnd && GetFocus() == this && GetParent())
		{
			CWnd* pWndNext = GetParent()->GetNextDlgTabItem(this);
			if (pWndNext != NULL)
				pWndNext->SetFocus();
		}
		m_nLockRecurse = 0;
	}

	if (pCommandBars)
	{
		pCommandBars->OnTrackingModeChanged(this, bMode);
	}

	return TRUE;
}


int CXTPCommandBar::GetControlCount() const
{
	return m_pControls->GetCount();
}

CXTPControl* CXTPCommandBar::GetControl(int nIndex) const
{
	//Ensure the index falls within the correct range.
	if ((nIndex >= 0) && (nIndex < GetControlCount()))
	{
		return m_pControls->GetAt(nIndex);
	}
	return NULL;
}

void CXTPCommandBar::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	if (nIDEvent == XTP_TID_SHOWKEYBOARDTIPS)
	{
		CXTPMouseManager* pMouseManager = GetCommandBars()->GetMouseManager();
		int nCount = (int)pMouseManager->GetTrackArray().GetSize();
		CXTPCommandBar* pCommandBar = nCount > 0 ? pMouseManager->GetTrackArray().GetAt(nCount - 1) : 0;

		if (pCommandBar == this && !IsKeyboardTipsVisible()) GetCommandBars()->ShowKeyboardTips(this);
	}
	if (nIDEvent == XTP_TID_ANIMATION)
	{
		m_pAnimation->OnAnimate();
		return;
	}

	if (nIDEvent == XTP_TID_HOVER && m_nLockRecurse == 0)
	{
		KillTimer(XTP_TID_HOVER);

		if (m_nSelected != -1)
			GetControl(m_nSelected)->OnMouseHover();
	}
	if (nIDEvent == XTP_TID_EXPANDED_POPUP && m_nLockRecurse == 0)
	{
		KillTimer(XTP_TID_EXPANDED_POPUP);

		if (m_nPopuped != -1 && m_nSelected == m_nPopuped)
		{
			CXTPControlPopup* pControl = DYNAMIC_DOWNCAST(CXTPControlPopup, GetControl(m_nSelected));
			if (pControl && pControl->IsCursorOver())
			{
				pControl->ExpandCommandBar();
			}
		}
	}
}

void CXTPCommandBar::OnControlsChanged()
{
	m_nSelected = -1;
	m_nPopuped = -1;
	DelayLayout();
}

void CXTPCommandBar::Redraw(LPCRECT lpRect, BOOL bAnimate)
{
	if (m_nLockRedraw > 0)
	{
		m_nIdleFlags |= xtpIdleRedraw;
		return;
	}

	if (lpRect == NULL)
	{
		m_nIdleFlags &= ~xtpIdleRedraw;
	}

	if (m_hWnd && IsVisible())
	{
		m_pAnimation->RedrawRect(lpRect, bAnimate);
	}
}

void CXTPCommandBar::EnableAnimation(BOOL bEnable)
{
	m_pAnimation->m_bAnimation = bEnable;
	Redraw();
}

BOOL CXTPCommandBar::IsAnimationEnabled() const
{
	return m_pAnimation->m_bAnimation;
}

void CXTPCommandBar::EnableDoubleBuffer(BOOL bEnable)
{
	m_pAnimation->m_bDoubleBuffer = bEnable;
}

void CXTPCommandBar::SetShowGripper(BOOL bShow)
{
	if (bShow)
		m_dwStyle |= CBRS_GRIPPER;
	else
		m_dwStyle &= ~CBRS_GRIPPER;
}

BOOL CXTPCommandBar::GetShowGripper() const
{
	return m_dwStyle & CBRS_GRIPPER ? TRUE : FALSE;
}

void CXTPCommandBar::OnPaint()
{
	CPaintDC paintDC(this);
	m_pAnimation->OnPaint(paintDC);
}

LRESULT CXTPCommandBar::OnPrintClient(WPARAM wParam, LPARAM)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		DrawCommandBar(pDC, CXTPClientRect(this));
	}
	return TRUE;
}

CRect CXTPCommandBar::GetBorders()
{
	return GetPaintManager()->GetCommandBarBorders(this);
}

void CXTPCommandBar::FillCommandBarEntry(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = GetPaintManager();
	pPaintManager->FillCommandBarEntry(pDC, this);
}

void CXTPCommandBar::DrawCommandBar(CDC* pDC, CRect rcClipBox)
{
#if 0
	_int64 nPerfomanceEnd;
	_int64 nPerfomanceStart;

	QueryPerformanceCounter((LARGE_INTEGER*)&nPerfomanceStart);
#endif

	if (!pDC || !pDC->GetSafeHdc())
		return;

	CXTPPaintManager* pPaintManager = GetPaintManager();
	CXTPCommandBars* pCommandBars = GetCommandBars();

	FillCommandBarEntry(pDC);

	if (GetShowGripper() || GetPosition() == xtpBarFloating)
		pPaintManager->DrawCommandBarGripper(pDC, this);

	pDC->SetBkMode (TRANSPARENT);

	CXTPFontDC font(pDC, pPaintManager->GetCommandBarFont(this));

	BOOL bFirst = TRUE;
	for (int i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl || pControl->GetParent() != this || !pControl->IsVisible())
			continue;

		if (pControl->GetBeginGroup() && !bFirst)
		{
			pPaintManager->DrawCommandBarSeparator(pDC, this, pControl);
		}
		bFirst = FALSE;

		if (CRect().IntersectRect(rcClipBox, pControl->GetRect()))
		{
			pControl->Draw(pDC);
		}

		if (pCommandBars && pCommandBars->IsCustomizeMode() && (pCommandBars->m_pDragSelected == pControl))
		{
			CRect rc = pControl->GetRect();
			pDC->Draw3dRect(rc, 0, 0);
			rc.DeflateRect(1, 1);
			pDC->Draw3dRect(rc, 0, 0);
		}
	}

	if (pCommandBars && pCommandBars->IsCustomizeMode() && !m_rcMarker.IsRectEmpty())
	{
		CustomizeDrawMarker(pDC);
	}

#if 0
	QueryPerformanceCounter((LARGE_INTEGER*)&nPerfomanceEnd);
	TRACE(_T("TotalCounter = %i \n"), int(nPerfomanceEnd - nPerfomanceStart));
#endif
}

void CXTPCommandBar::CustomizeDrawMarker(CDC* pDC)
{

	CBrush brush(GetXtremeColor(COLOR_3DFACE));
	CBrush* pBrush = (CBrush*)pDC->SelectObject(&brush);

	if (m_bVerticalMarker)
	{
		pDC->PatBlt(m_rcMarker.left, m_rcMarker.top, 1, m_rcMarker.Height(), PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 1, m_rcMarker.top + 1, 1, m_rcMarker.Height() - 2, PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 2, m_rcMarker.top + 2, m_rcMarker.Width() - 4, 2, PATINVERT);
		pDC->PatBlt(m_rcMarker.right - 1, m_rcMarker.top, 1, m_rcMarker.Height(), PATINVERT);
		pDC->PatBlt(m_rcMarker.right - 2, m_rcMarker.top + 1, 1, m_rcMarker.Height() - 2, PATINVERT);

	}
	else
	{
		pDC->PatBlt(m_rcMarker.left, m_rcMarker.top, m_rcMarker.Width(), 1, PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 1, m_rcMarker.top + 1, m_rcMarker.Width() - 2, 1, PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 2, m_rcMarker.top + 2, 2, m_rcMarker.Height() - 4, PATINVERT);
		pDC->PatBlt(m_rcMarker.left, m_rcMarker.bottom - 1, m_rcMarker.Width(), 1, PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 1, m_rcMarker.bottom - 2, m_rcMarker.Width() - 2, 1, PATINVERT);
	}

	pDC->SelectObject(pBrush);
}


int CXTPCommandBar::FindAccel(TCHAR chAccel) const
{
	for (int i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl || pControl->GetParent() != this || !pControl->IsVisible())
			continue;

		CString strCaption = pControl->GetCaption();
		int nAmdIndex = CXTPShortcutManager::FindAccelPos(strCaption);
		if (nAmdIndex == -1 || nAmdIndex == strCaption.GetLength() - 1)
			continue;
		TCHAR ch = CXTPShortcutManager::ToUpper(strCaption[nAmdIndex + 1]);
		if (chAccel == ch)
		{
			return pControl->GetIndex();
		}
	}
	return -1;
}


int CXTPCommandBar::FindAccelEx(UINT chAccel, BOOL& bSelectOnly) const
{
	int nItem = m_pControls->GetNext(m_nSelected, +1);
	if (nItem == -1)
		return -1;

	bSelectOnly = FALSE;
	BOOL bSelectOnlyCond = FALSE;

	int nStart = nItem;

	int nAmpCondidate = -1, nCondidate = -1;

	do
	{
		CXTPControl* pControl = GetControl(nItem);
		ASSERT(pControl);
		if (!pControl)
			return -1;

		CString strCaption = pControl->GetCaption();
		if (!strCaption.IsEmpty() && pControl->GetEnabled())
		{
			int nAmpIndex = CXTPShortcutManager::FindAccelPos(strCaption);
			BOOL bFound = nAmpIndex > -1 && nAmpIndex < strCaption.GetLength() - 1;

			if (CXTPShortcutManager::CompareAccelKey(strCaption[bFound ? nAmpIndex + 1 : 0], chAccel))
			{
				if (bFound)
				{
					if (nAmpCondidate == -1)
						nAmpCondidate = pControl->GetIndex();
					else
					{
						bSelectOnly = TRUE;
						return nAmpCondidate;
					}
				}
				else
				{
					if (nCondidate == -1)
						nCondidate = pControl->GetIndex();
					else
					{
						bSelectOnlyCond = TRUE;
					}
				}
			}
		}
		nItem = m_pControls->GetNext(nItem, +1, TRUE, FALSE, FALSE);
	}
	while (nItem != nStart);

	if (nAmpCondidate != -1)
		return nAmpCondidate;

	bSelectOnly = bSelectOnlyCond;
	return nCondidate;
}

CFrameWnd* CXTPCommandBar::GetFrameSite() const
{
	CWnd* pSite = GetSite();

	return pSite && pSite->IsFrameWnd() ? (CFrameWnd*)pSite : NULL;
}

CWnd* CXTPCommandBar::GetSite() const
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
		return pCommandBars->GetSite();

	CXTPCommandBar* pRoot = GetRootParent();

	if (pRoot->m_pSite) return pRoot->m_pSite;
	if (pRoot->m_hWnd) return pRoot->GetOwner();

	return AfxGetMainWnd();
}

CWnd* CXTPCommandBar::GetOwnerSite() const
{
	CXTPCommandBar* pRoot = (CXTPCommandBar*)this;
	CXTPCommandBar* pParent = pRoot->GetParentCommandBar();

	while (pRoot)
	{
		if (pRoot->m_pOwner != NULL)
			return pRoot->m_pOwner;

		if (pRoot->m_hWndOwner != NULL)
			return CWnd::FromHandle(pRoot->m_hWndOwner);

		if (!pParent)
			return pRoot->GetSite();

		pRoot = pParent;
		pParent = pRoot->GetParentCommandBar();
	}

	ASSERT(FALSE);
	return NULL;
}

CXTPCommandBar* CXTPCommandBar::GetRootParent() const
{
	CXTPCommandBar* pRoot = (CXTPCommandBar*)this;
	CXTPCommandBar* pParent = pRoot->GetParentCommandBar();

	while (pParent != NULL)
	{
		pRoot = pParent;
		pParent = pRoot->GetParentCommandBar();
	}

	return pRoot;
}

CXTPCommandBars* CXTPCommandBar::GetCommandBars() const
{
	const CXTPCommandBar* pCommandBar = this;
	do
	{
		if (pCommandBar->m_pCommandBars)
			return pCommandBar->m_pCommandBars;

		pCommandBar = pCommandBar->GetParentCommandBar();

	}
	while (pCommandBar != NULL);

	return NULL;
}

BOOL CXTPCommandBar::IsCustomizeMode() const
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	return pCommandBars ? pCommandBars->IsCustomizeMode() : FALSE;
}


DROPEFFECT CXTPCommandBar::OnCustomizeDragEnter(CXTPControl* pDataObject, CPoint point)
{
	m_rcMarker.SetRectEmpty();

	return OnCustomizeDragOver(pDataObject, point);
}

void CXTPCommandBar::OnCustomizeDragLeave()
{
	if (!m_rcMarker.IsRectEmpty())
	{
		m_rcMarker.SetRectEmpty();
		Redraw();
	}
}

void CXTPCommandBar::CustomizeFindDropIndex(CXTPControl* pDataObject, const CPoint& point,
	CRect& rcMarker, int& nDropIndex, BOOL& bDropAfter)
{

	CXTPControl* pControlMarker = NULL;
	int nOffset = -1;
	int nCount = GetControlCount();

	BOOL bVert = (m_barPosition == xtpBarLeft || m_barPosition == xtpBarRight || m_barType == xtpBarTypePopup);

	for (int i = 0; i < nCount; i++)
	{
		CXTPControl* pControl = GetControl(i);

		if (!pControl->IsVisible() || !pControl->IsCustomizeMovable())
			continue;

		CRect rc = pControl->GetRect();

		if (!m_bMultiLine)
		{
			if (pControl->GetWrap())
			{
				nOffset = -1;
				if (!bVert && rc.top > point.y) break;
				else if (bVert && rc.right < point.x) break;
			}
		}
		else if (!bVert)
		{
			if (point.y < rc.top || point. y > rc.bottom)
				continue;
		}
		else
		{
			if (point.x < rc.left || point. x > rc.right)
				continue;
		}

		if (bVert)
		{
			if (nOffset == -1 || nOffset > abs(rc.CenterPoint().y - point.y))
			{
				nOffset = abs(rc.CenterPoint().y - point.y);
				pControlMarker = pControl;

				bDropAfter = rc.CenterPoint().y < point.y;

				if (bDropAfter)
				{
					if (i >= nCount - 1 || GetControl(i + 1)->GetBeginGroup())
						rcMarker.SetRect(rc.left, rc.bottom - 6, rc.right, rc.bottom);
					else
						rcMarker.SetRect(rc.left, rc.bottom, rc.right, rc.bottom + 6);
				}
				else
				{
					rcMarker.SetRect(rc.left, rc.top, rc.right, rc.top + 6);

				}

			}

		}
		else
		{
			if (nOffset == -1 || nOffset > abs(rc.CenterPoint().x - point.x))
			{
				nOffset = abs(rc.CenterPoint().x - point.x);
				pControlMarker = pControl;

				bDropAfter = rc.CenterPoint().x < point.x;

				if (bDropAfter)
				{
					if (i >= nCount - 1 || GetControl(i + 1)->GetBeginGroup() || GetControl(i + 1)->GetWrap())
						rcMarker.SetRect(rc.right - 6, rc.top, rc.right, rc.bottom);
					else
						rcMarker.SetRect(rc.right, rc.top, rc.right + 6, rc.bottom);
				}
				else
				{
					rcMarker.SetRect(rc.left, rc.top, rc.left + 6, rc.bottom);
				}
			}
		}
		m_bVerticalMarker = bVert;
	}

	if (pControlMarker == NULL)
	{
		CRect rc;
		GetClientRect(&rc);
		if (IsRibbonBar())
		{
			rcMarker.SetRectEmpty();
		}
		else
		{
			rcMarker.SetRect(rc.left + 2, rc.top + 2, rc.left + 8, rc.bottom - 2);
			bDropAfter = TRUE;
		}
		nDropIndex = -1;
		m_bVerticalMarker = FALSE;
	}
	else
	{
		nDropIndex = pControlMarker->GetIndex();
	}

	if (pDataObject == pControlMarker)
		rcMarker.SetRectEmpty();
}


DROPEFFECT CXTPCommandBar::OnCustomizeDragOver(CXTPControl* pDataObject, CPoint point)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return DROPEFFECT_NONE;

	if ((!IsCustomizable() && !pCommandBars->m_bDesignerMode) || (!(GetStyle() & WS_VISIBLE)))
		return DROPEFFECT_NONE;

	ASSERT(pDataObject);

	CRect rcMarker;
	int nDropIndex;
	BOOL bDropAfter;
	CustomizeFindDropIndex (pDataObject, point, rcMarker, nDropIndex, bDropAfter);

	DROPEFFECT dropEffect = (DROPEFFECT)-1;

	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl)
	{
		if (GetParentCommandBar() == NULL && m_nPopuped != pControl->GetIndex())
		{
			XTPMouseManager()->SendTrackLost();
		}

		pControl->OnCustomizeDragOver(pDataObject, point, dropEffect);
	}

	if (!pDataObject->IsCustomizeDragOverAvail(this, point, dropEffect))
		return DROPEFFECT_NONE;

	m_rcMarker = rcMarker;

	Redraw();

	return dropEffect;
}

void CXTPCommandBar::OnCustomizeDrop(CXTPControl* pDataObject, DROPEFFECT& dropEffect, CPoint ptDrop, CPoint ptDrag)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	ASSERT(pCommandBars);
	if (!pCommandBars)
		return;

	CRect rcMarker;
	int nDropIndex;
	BOOL bDropAfter;

	CustomizeFindDropIndex (pDataObject, ptDrop, rcMarker, nDropIndex, bDropAfter);

	if (nDropIndex == -1 || pDataObject != GetControl(nDropIndex))
	{
		if (bDropAfter) nDropIndex++;

		SetPopuped(-1);
		SetSelected(-1);

		BOOL bBeginGroup = FALSE;
		if (bDropAfter == FALSE && nDropIndex >= 0 && nDropIndex < GetControlCount())
		{
			bBeginGroup = GetControl(nDropIndex)->GetBeginGroup();
			if (bBeginGroup)
			{
				GetControl(nDropIndex)->SetBeginGroup(FALSE);
			}
		}

		CXTPControl* pClone = m_pControls->AddClone(pDataObject, nDropIndex);
		pClone->SetBeginGroup(bBeginGroup);
		pCommandBars->SetDragControl(pClone);

	}
	else
	{
		if (abs(ptDrag.x - ptDrop.x) > 4 || abs(ptDrag.y - ptDrop.y) > 4)
		{
			CXTPControl* pControl = GetControl(nDropIndex);
			ASSERT(pDataObject == pControl);
			pControl->SetBeginGroup(bDropAfter && nDropIndex != 0);
		}

		dropEffect = DROPEFFECT_CANCEL;
	}
	OnRecalcLayout();

	m_rcMarker.SetRectEmpty();
	Redraw();
}

void CXTPCommandBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl && pControl->OnRButtonDown(point))
		return;

#ifndef _XTP_EXCLUDE_CUSTOMIZATION
	if (pControl && m_bCustomizable && IsCustomizeMode())
	{
		if (pControl->IsVisible() && pControl->IsCustomizeMovable())
		{
			SetPopuped(-1);
			SetSelected(-1);

			CXTPCommandBars* pCommandBars = GetCommandBars();
			ASSERT(pCommandBars);
			CXTPCustomizeDropSource* pDropSource = pCommandBars->GetDropSource();
			ASSERT(pDropSource);

			pCommandBars->SetDragControl(pControl);

			if (pDropSource->GetSheet())
			{
				ClientToScreen(&point);
				pDropSource->ContextMenu(point);
			}
		}
		return;
	}
#endif

	CWnd::OnRButtonDown(nFlags, point);
}

void CXTPCommandBar::InvalidateParents()
{
	Redraw();
	CXTPCommandBar* pCommandBar = GetParentCommandBar();
	if (pCommandBar) pCommandBar->InvalidateParents();
}

void CXTPCommandBar::OnControlSelected(BOOL bSelected, CXTPControl* pControl)
{
	if (!pControl)
		return;

	GetSite()->SendMessage(WM_XTP_CONTROLSELECTED, bSelected ? (WPARAM)pControl : 0);

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
	{
		pCommandBars->m_nIDHelpTracking = bSelected ? pControl->GetHelpId() : 0;
	}


#ifndef _XTP_ACTIVEX
	if (pControl && (pControl->GetIndex() == m_nClickedControl))
	{
		if (!bSelected)
			m_nClickedControl = -1;
		return;
	}
	m_nClickedControl = -1;

	SetStatusText(bSelected ? pControl->GetDescription() : _T(""));
#endif
}

BOOL CXTPCommandBar::SetStatusText(const CString& strDescription)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	CWnd* pOwner = pCommandBars ? pCommandBars->GetSite() : GetSite();

	if (pCommandBars && !pCommandBars->GetCommandBarsOptions()->bFlyByStatus)
		return TRUE;

	if (strDescription.IsEmpty())
	{
		pOwner->SendMessage(WM_POPMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
	}
	else
	{
		pOwner->SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)(LPCTSTR)strDescription);
	}
	return TRUE;
}



void CXTPCommandBar::SetPaintManager(CXTPPaintManager* pPaintManager)
{
	if (m_pPaintManager)
		m_pPaintManager->InternalRelease();

	m_pPaintManager = pPaintManager;

	if (m_pPaintManager)
	{
		m_pPaintManager->RefreshMetrics();
	}
}

CXTPPaintManager* CXTPCommandBar::GetPaintManager() const
{
	if (m_pPaintManager)
		return m_pPaintManager;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
		return pCommandBars->GetPaintManager();

	CXTPCommandBar* pRoot = GetRootParent();
	if (pRoot->m_pPaintManager)
		return pRoot->m_pPaintManager;

	return XTPPaintManager();
}

void CXTPCommandBar::SetImageManager(CXTPImageManager* pImageManager)
{
	if (m_pImageManager)
		m_pImageManager->InternalRelease();

	m_pImageManager = pImageManager;

}

CXTPImageManager* CXTPCommandBar::GetImageManager() const
{
	if (m_pImageManager)
		return m_pImageManager;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
		return pCommandBars->GetImageManager();

	CXTPCommandBar* pRoot = GetRootParent();
	if (pRoot->m_pImageManager)
		return pRoot->m_pImageManager;

	return XTPImageManager();
}


void CXTPCommandBar::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive)
{
	m_barType = pCommandBar->m_barType;
	m_barPosition = pCommandBar->m_barPosition;
	m_nBarID = pCommandBar->m_nBarID;
	m_dwFlags = pCommandBar->m_dwFlags;
	m_dwStyle = pCommandBar->m_dwStyle;
	m_strTitle = pCommandBar->m_strTitle;
	m_bCustomizable = pCommandBar->m_bCustomizable;
	m_nMRUWidth = pCommandBar->m_nMRUWidth;
	m_szIcons = pCommandBar->m_szIcons;
	m_szButtons = pCommandBar->m_szButtons;
	m_pCommandBars = pCommandBar->m_pCommandBars;
	m_buttonStyle = pCommandBar->m_buttonStyle;

	m_pControls->Copy(pCommandBar->GetControls(), bRecursive);
}

#ifndef _XTP_ACTIVEX

UINT CXTPCommandBar::GetActiveDocTemplate() const
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return 0;

	return pCommandBars->GetActiveDocTemplate();
}
#endif

void CXTPCommandBar::UpdateDocTemplateControls()
{
#ifdef _XTP_ACTIVEX
	return;
#else
	UINT nIDResource = GetActiveDocTemplate();

	if (nIDResource == 0)
		return;

	BOOL bRecalcLayout = FALSE;
	BOOL bValue;

	for (int i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl || pControl->GetParent() != this)
			continue;

		if (pControl->m_mapDocTemplatesAssigned.GetCount() > 0)
		{
			BOOL bVisible = pControl->m_mapDocTemplatesAssigned.Lookup(nIDResource, bValue);
			if (pControl->SetHideFlag(xtpHideDocTemplate, !bVisible)) bRecalcLayout = TRUE;

		}
		if (pControl->m_mapDocTemplatesExcluded.GetCount() > 0)
		{
			BOOL bVisible = !pControl->m_mapDocTemplatesExcluded.Lookup(nIDResource, bValue);
			if (pControl->SetHideFlag(xtpHideDocTemplate, !bVisible)) bRecalcLayout = TRUE;
		}
	}

	if (bRecalcLayout)
		m_nIdleFlags |= xtpIdleLayout;
#endif
}

void CXTPCommandBar::SetFlags(DWORD dwFlagsAdd, DWORD dwFlagsRemove)
{
	m_dwFlags |= dwFlagsAdd;
	m_dwFlags &= ~dwFlagsRemove;
}


LRESULT CXTPCommandBar::OnHelpHitTest(WPARAM, LPARAM lParam)
{
	ASSERT_VALID(this);

	CXTPControl* pControl = m_pControls->HitTest(CPoint((DWORD)lParam));

	if (pControl != NULL)
		return HID_BASE_COMMAND + pControl->GetHelpId();

	UINT nID = _AfxGetDlgCtrlID(m_hWnd);
	return nID != 0 ? HID_BASE_CONTROL + nID : 0;
}

LRESULT CXTPCommandBar::OnNcHitTest(CPoint point)
{
	if (IsHelpMode())
	{
		ScreenToClient(&point);

		CXTPControl* pControl = m_pControls->HitTest(point);
		if (pControl && pControl->GetID() <= 0 && IsPopupControlType(pControl->GetType()))
			return HTMENU;

		return HTCLIENT;
	}

	return (LRESULT)CWnd::OnNcHitTest(point);

}

void CXTPCommandBar::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (IsHelpMode())
	{
		ScreenToClient(&point);
		OnLButtonDown(0, point);
		return;
	}
	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CXTPCommandBar::SetWidth(int nWidth)
{
	m_nMRUWidth = nWidth;
}

int CXTPCommandBar::GetWidth() const
{
	return m_nMRUWidth;
}

BOOL CXTPCommandBar::IsLayoutRTL() const
{
	return m_hWnd && GetExStyle() & (WS_EX_LAYOUTRTL | WS_EX_RTLREADING);
}

int CXTPCommandBar::OnMouseHitTest(CPoint pt)
{
	CXTPWindowRect rcWindow(this);
	if (!rcWindow.PtInRect(pt))
		return HTNOWHERE;

	if (GetPosition() != xtpBarPopup)
		return HTCLIENT;

	CRgn rgn;
	rgn.CreateRectRgn(0, 0, 0, 0);
	int nRegion = GetWindowRgn(rgn);
	if (nRegion == ERROR || nRegion == NULLREGION)
		return HTCLIENT;

	pt.Offset(-rcWindow.TopLeft());
	if (!::PtInRegion(rgn, pt.x, pt.y))
		return HTNOWHERE;

	return HTCLIENT;
}


//////////////////////////////////////////////////////////////////////////
//

CCmdTarget* CXTPCommandBar::GetAccessible()
{
	return this;
}

HRESULT CXTPCommandBar::GetAccessibleParent(IDispatch* FAR* ppdispParent)
{
	*ppdispParent = NULL;

	CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, this);
	if (pPopupBar)
	{
		if (pPopupBar->m_pControlPopup)
		{
			*ppdispParent = pPopupBar->m_pControlPopup->GetIDispatch(TRUE);
			return S_OK;
		}

	}
	else if (GetSafeHwnd())
	{
		return AccessibleObjectFromWindow(GetSafeHwnd(), OBJID_WINDOW, IID_IDispatch, (void**)ppdispParent);
	}
	return E_FAIL;
}

HRESULT CXTPCommandBar::GetAccessibleChildCount(long FAR* pChildCount)
{
	if (pChildCount == 0)
		return E_INVALIDARG;

	*pChildCount = GetControlCount();

	return S_OK;
}
HRESULT CXTPCommandBar::GetAccessibleChild(VARIANT varChild, IDispatch* FAR* ppdispChild)
{
	*ppdispChild = NULL;
	int nChild = GetChildIndex(&varChild);

	if (nChild > 0 && nChild <= GetControlCount())
	{
		CXTPControl* pControl = GetControl(nChild - 1);
		if (pControl)
		{
			*ppdispChild = pControl->GetIDispatch(TRUE);
		}
	}

	return S_OK;
}

HRESULT CXTPCommandBar::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	int nChild = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF || nChild == -1)
	{
		CString strCaption = GetTitle();
		*pszName = strCaption.AllocSysString();
		return S_OK;
	}

	if (nChild > 0 && nChild <= GetControlCount())
	{
		CXTPControl* pControl = GetControl(nChild - 1);
		CString strCaption = pControl->GetCaption();
		CXTPPaintManager::StripMnemonics(strCaption);

		if (!pControl->GetShortcutText().IsEmpty())
			strCaption = strCaption + _T('\t') + pControl->GetShortcutText();

		*pszName = strCaption.AllocSysString();
		return S_OK;
	}
	return E_INVALIDARG;
}

HRESULT CXTPCommandBar::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	pvarRole->vt = VT_EMPTY;
	int nChild = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = GetType() == xtpBarTypeMenuBar ? ROLE_SYSTEM_MENUBAR :
			GetType() == xtpBarTypePopup ? ROLE_SYSTEM_MENUPOPUP : ROLE_SYSTEM_TOOLBAR;
		return S_OK;
	}

	if (nChild > 0)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_MENUITEM;
		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPCommandBar::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	pvarState->vt = VT_I4;
	pvarState->lVal = 0;
	int nChild = GetChildIndex(&varChild);

	if (!IsVisible())
		pvarState->lVal |= STATE_SYSTEM_INVISIBLE;

	if (nChild > 0 && nChild <= GetControlCount())
	{
		pvarState->lVal |= STATE_SYSTEM_FOCUSABLE | STATE_SYSTEM_SELECTABLE |
			(m_nSelected == nChild - 1 ? STATE_SYSTEM_FOCUSED | STATE_SYSTEM_SELECTED | STATE_SYSTEM_HOTTRACKED : 0);
	}

	return S_OK;
}


HRESULT CXTPCommandBar::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	if (!GetSafeHwnd())
		return S_OK;

	CRect rc;
	GetWindowRect(&rc);

	int nChild = GetChildIndex(&varChild);

	if (nChild > 0 && nChild <= GetControlCount())
	{
		CXTPControl* pControl = GetControl(nChild - 1);
		CRect rcControl = pControl->GetRect();

		rc = CRect(CPoint(rc.left + rcControl.left, rc.top + rcControl.top), rcControl.Size());

	}

	*pxLeft = rc.left;
	*pyTop = rc.top;
	*pcxWidth = rc.Width();
	*pcyHeight = rc.Height();

	return S_OK;
}

HRESULT CXTPCommandBar::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (pvarID == NULL)
		return E_INVALIDARG;

	pvarID->vt = VT_EMPTY;

	if (!GetSafeHwnd())
		return S_FALSE;

	if (!CXTPWindowRect(this).PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	pvarID->vt = VT_I4;
	pvarID->lVal = 0;

	CPoint pt(xLeft, yTop);
	ScreenToClient(&pt);

	CXTPControl* pControl = GetControls()->HitTest(pt);
	if (pControl)
	{
		pvarID->vt = VT_DISPATCH;
		pvarID->pdispVal = pControl->GetIDispatch(TRUE);
	}

	return S_OK;
}


LRESULT CXTPCommandBar::OnGetObject(WPARAM wParam, LPARAM lParam)
{
	LONG dwObjId = (LONG) lParam;

	if (dwObjId > 0 && dwObjId < 0xFFFFFF)
	{
		CXTPControl* pControl = GetControls()->FindControl((int)lParam);
		if (pControl)
		{
			LPUNKNOWN lpUnknown = pControl->GetInterface(&IID_IAccessible);
			if (!lpUnknown)
				return E_FAIL;

			return LresultFromObject(IID_IAccessible, wParam, lpUnknown);
		}
	}

	if (dwObjId != OBJID_CLIENT && dwObjId != OBJID_MENU)
		return (LRESULT)Default();

	LPUNKNOWN lpUnknown = GetInterface(&IID_IAccessible);
	if (!lpUnknown)
		return E_FAIL;

	return LresultFromObject(IID_IAccessible, wParam, lpUnknown);
}

void CXTPCommandBar::FilterToolTipMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pToolTipContext)
	{
		m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}
}

BOOL CXTPCommandBar::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	FilterToolTipMessage(message, wParam, lParam);

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CXTPCommandBar::IsDwmEnabled() const
{
	if (GetPosition() != xtpBarTop)
		return FALSE;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return FALSE;

	return pCommandBars->IsDwmEnabled();
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPCommandBar, CWnd)
	DISP_PROPERTY_ID(CXTPCommandBar, "Title", dispidTitle, m_strTitle, VT_BSTR)
	DISP_FUNCTION_ID(CXTPCommandBar, "Controls", dispidControls, OleControls, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCommandBar, "Type", dispidType, OleGetType, VT_I4, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPCommandBar, "Visible", dispidVisible, OleGetVisible, OleSetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCommandBar, "Position", dispidPosition, OleGetPosition, OleSetPosition, VT_I4)
	DISP_FUNCTION_ID(CXTPCommandBar, "ShowPopup", dispidShowPopup, OleShowPopup, VT_I4, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPCommandBar, "SetIconSize", dispidSetIconSize, OleSetIconSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPCommandBar, "EnableDocking", dispidEnableDocking, OleEnableDocking, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCommandBar, "SetTearOffPopup", dispidSetTearOffPopup, OleSetTearOffPopup, VT_EMPTY, VTS_BSTR VTS_I4 VTS_I4)
	DISP_PROPERTY_ID(CXTPCommandBar, "BarID", dispidBarID, m_nBarID, VT_I4)
	DISP_FUNCTION_ID(CXTPCommandBar, "FindControl", dispidFindControl, OleFindControl, VT_DISPATCH, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPCommandBar, "Parent", dispidParent, OleParent, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCommandBar, "BuiltIn", dispidBuiltIn, OleBuiltIn, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCommandBar, "ModifyStyle", dispidModifyStyle, OleModifyStyle, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPCommandBar, "GetWindowRect", dispidGetWindowRect, OleGetWindowRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CXTPCommandBar, "SetFlags", dispidSetFlags, SetFlags, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPCommandBar, "ShowTextBelowIcons", 19, IsTextBelowIcons, ShowTextBelowIcons, VT_BOOL)
	DISP_FUNCTION_ID(CXTPCommandBar, "SetPopupToolBar", 20, OleSetPopupToolBar, VT_EMPTY, VTS_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCommandBar, "Width", 21, GetWidth, SetWidth, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPCommandBar, "Customizable", 23, OleGetCustomizable, OleSetCustomizable, VT_BOOL)
	DISP_FUNCTION_ID(CXTPCommandBar, "RedrawBar", 31, OleRedrawBar, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCommandBar, "RecalcLayout", 32, OleRecalcLayout, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPCommandBar, "SelectedControl", 34, OleGetSelectedControl, OleSetSelectedControl, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPCommandBar, "hWnd", DISPID_HWND, OleGetHandle, SetNotSupported, VT_HANDLE)

	DISP_PROPERTY_EX_ID(CXTPCommandBar, "CommandBars", 45, OleGetCommandBars, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPCommandBar, "EnableAnimation", 37, IsAnimationEnabled, EnableAnimation, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCommandBar, "ShowGripper", 38, GetShowGripper, SetShowGripper, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCommandBar, "DefaultButtonStyle", 39, m_buttonStyle, VT_I4)

	DISP_PROPERTY_ID(CXTPCommandBar, "TextOrientation", 40, m_nTextOrientation, VT_I4)

END_DISPATCH_MAP()

// {723CBB00-0690-4528-8656-E33E61F39377}
static const IID IID_ICommandBar =
{ 0x723cbb00, 0x690, 0x4528, { 0x86, 0x56, 0xe3, 0x3e, 0x61, 0xf3, 0x93, 0x77 } };


IMPLEMENT_OLETYPELIB_EX(CXTPCommandBar, IID_ICommandBar)

BOOL CXTPCommandBar::OleBuiltIn()
{
	CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, GetRootParent());
	if (pToolBar) return pToolBar->IsBuiltIn();
	return FALSE;
}

HWND CXTPCommandBar::OleGetHandle()
{
	return m_hWnd;
}

BOOL CXTPCommandBar::OleGetVisible()
{
	return IsVisible();
}

void CXTPCommandBar::OleSetVisible(BOOL bVisible)
{
	SetVisible(bVisible);
}


LPDISPATCH CXTPCommandBar::OleFindControl(const VARIANT& varType, const VARIANT& varId, const VARIANT& varVisible, const VARIANT& varRecursive)
{
	return m_pControls->OleFindControl(varType, varId, varVisible, varRecursive);
}

LPDISPATCH CXTPCommandBar::OleControls()
{
	return m_pControls->GetIDispatch(TRUE);
}

long CXTPCommandBar::OleGetType()
{
	return m_barType;
}
long CXTPCommandBar::OleGetPosition()
{
	return m_barPosition;
}

void CXTPCommandBar::OleSetPosition(long nPosition)
{
	SetPosition(XTPBarPosition(nPosition));
}

LPDISPATCH CXTPCommandBar::OleParent()
{
	CXTPPopupBar* pPopup = DYNAMIC_DOWNCAST(CXTPPopupBar, this);
	if (pPopup)
	{
		CXTPControl* pControl = pPopup->GetControlPopup();
		if (pControl)
			return pControl->GetIDispatch(TRUE);
	}
	return 0;
}

void CXTPCommandBar::OleModifyStyle(long dwRemove, long dwAdd)
{
	m_dwStyle |= dwAdd;
	m_dwStyle &= ~dwRemove;
}

void CXTPCommandBar::OleSetTearOffPopup(LPCTSTR strCaption, long nID, long nWidth)
{
	CXTPPopupBar* pPopup = DYNAMIC_DOWNCAST(CXTPPopupBar, this);
	if (pPopup)
	{
		pPopup->SetTearOffPopup(strCaption, nID, nWidth);
	}
}

void CXTPCommandBar::OleSetPopupToolBar(BOOL bToolBarType)
{
	CXTPPopupBar* pPopup = DYNAMIC_DOWNCAST(CXTPPopupBar, this);
	if (pPopup)
	{
		pPopup->SetPopupToolBar(bToolBarType);
	}

}

LPDISPATCH CXTPCommandBar::OleGetSelectedControl()
{
	if (m_nSelected != -1)
	{
		return GetControl(m_nSelected)->GetIDispatch(TRUE);
	}
	return NULL;
}

void CXTPCommandBar::OleSetSelectedControl(LPDISPATCH lpDispatch)
{
	if (lpDispatch == NULL)
	{
		SetSelected(-1);
		return;
	}

	CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, CXTPControl::FromIDispatch(lpDispatch));
	if (pControl)
	{
		SetSelected(pControl->GetIndex(), TRUE_KEYBOARD_NEXT);
	}
}

void CXTPCommandBar::OleSetIconSize(long cx, long cy)
{
	SetIconSize(CSize(cx, cy));
}
void CXTPCommandBar::OleEnableDocking(long dwFlags)
{
	m_dwFlags = dwFlags;
}

void CXTPCommandBar::OleGetWindowRect(long* pLeft, long* pTop, long* pRight, long* pBottom)
{
	if (pLeft == NULL || pTop == NULL || pRight == NULL || pBottom == NULL)
	{
		return;
	}
	if (!m_hWnd)
		return;

	CXTPWindowRect rc(this);

	OlePixelToUnits(rc);

	*pLeft = rc.left;
	*pRight = rc.right;
	*pTop = rc.top;
	*pBottom = rc.bottom;
}

BOOL CXTPCommandBar::OleGetCustomizable()
{
	return IsCustomizable();
}

void CXTPCommandBar::OleSetCustomizable(BOOL bCustomizable)
{
	EnableCustomization(bCustomizable);
}

void CXTPCommandBar::OleRedrawBar()
{
	Redraw();
}

void CXTPCommandBar::OleRecalcLayout()
{
	OnRecalcLayout();
}

void OlePixelToUnits(CWnd* pSite, CRect& rc);
void OleUnitsToPixel(CWnd* pSite, CPoint& pt);


void CXTPCommandBar::OlePixelToUnits(CRect& rc)
{
	::OlePixelToUnits(GetSite(), rc);
}


void CXTPCommandBar::OleUnitsToPixel(CPoint& pt)
{
	::OleUnitsToPixel(GetSite(), pt);
}


long CXTPCommandBar::OleShowPopup(const VARIANT& varFlags, const VARIANT& x, const VARIANT& y)
{
	CXTPPopupBar* pPopup = DYNAMIC_DOWNCAST(CXTPPopupBar, this);
	if (pPopup)
	{
		pPopup->m_rcExclude.SetRectEmpty();
		int nFlags = VariantToLong(&varFlags, 0);

		CPoint pt;
		pt.x = VariantToLong(&x, -1);
		pt.y = VariantToLong(&y, -1);

		if (pt.x != -1 && pt.y != -1)
		{
			OleUnitsToPixel(pt);
		}
		else GetCursorPos(&pt);

		return CXTPCommandBars::TrackPopupMenu(pPopup, nFlags, pt.x, pt.y, NULL);
	}
	return FALSE;

}

#endif


BEGIN_INTERFACE_MAP(CXTPCommandBar, CCmdTarget)
	INTERFACE_PART(CXTPCommandBar, IID_IAccessible, ExternalAccessible)
#ifdef _XTP_ACTIVEX
	INTERFACE_PART(CXTPCommandBar, IID_ICommandBar, Dispatch)
#endif
END_INTERFACE_MAP()
