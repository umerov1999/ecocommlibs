// XTPControl.cpp : implementation of the CXTPControl class.
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

#include <Common/Resource.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPShortcutManager.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPMouseManager.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/Customize/XTPCustomizeTools.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//////////////////////////////////////////////////////////////////////////
// CXTPControl

CXTPControl::CXTPControl()
{
	EnableAutomation();

#ifdef _XTP_ACTIVEX
	EnableTypeLib();
#endif

	m_nId = 0;
	m_nIconId = 0;
	m_nHelpId = 0;
	m_nCustomIconId = 0;
	m_nTag = 0;
	m_nIndex = 0;
	m_dwFlags = 0;
	m_rcControl.SetRectEmpty();
	m_rcRow.SetRectEmpty();

	m_pParent = 0;
	m_pControls = 0;
	m_bExpanded = FALSE;

	m_bChecked = FALSE;
	m_bEnabled = TRUE;
	m_bBeginGroup = FALSE;
	m_bEnabled = TRUE;
	m_bTemporary = FALSE;
	m_bSelected = FALSE;
	m_bPressed = FALSE;

	m_bWrap = FALSE;

	m_dwHideFlags = xtpNoHide;
	m_controlType = xtpControlError;

	m_bDefaultItem = FALSE;
	m_buttonStyle = xtpButtonAutomatic;
	m_buttonCustomStyle = xtpButtonUndefined;
	m_buttonRibbonStyle = xtpButtonAutomatic;

	m_bCloseSubMenuOnClick = TRUE;

	m_pRibbonGroup = NULL;

	m_pAction = 0;

	m_nWidth = 0;
	m_nHeight = 0;
	m_szIcon = CSize(0, 0);

	m_nExecuteOnPressInterval = 0;

	m_strCustomShortcutText = _T("");

	m_bLayoutRTL = FALSE;
}

CXTPControl::~CXTPControl()
{
	if (m_pAction)
	{
		m_pAction->RemoveControl(this);
		m_pAction = NULL;
	}
}

void CXTPControl::SetAction(CXTPControlAction* pAction)
{
	if (m_pAction)
	{
		m_pAction->RemoveControl(this);
		m_pAction = NULL;
	}

	if (pAction)
	{
		m_pAction = pAction;
		m_pAction->AddControl(this);
	}
}

void CXTPControl::OnRemoved()
{
	SetAction(NULL);
}

XTPButtonStyle CXTPControl::GetStyle() const
{
	return m_buttonCustomStyle != xtpButtonUndefined ? m_buttonCustomStyle :
		m_buttonStyle != xtpButtonAutomatic ? m_buttonStyle :
	m_buttonRibbonStyle != xtpButtonAutomatic || !m_pParent ? m_buttonRibbonStyle : m_pParent->GetDefaultButtonStyle();
}

CSize CXTPControl::GetSize(CDC* pDC)
{
	return GetPaintManager()->DrawControl(pDC, this, FALSE);
}

void CXTPControl::Draw(CDC* pDC)
{
	GetPaintManager()->DrawControl(pDC, this);
}

void CXTPControl::DelayRedrawParent()
{
	if (m_pParent && IsWindow(m_pParent->GetSafeHwnd()))
		((CXTPCommandBar*)m_pParent)->DelayRedraw();
}

void CXTPControl::DelayLayoutParent()
{
	if (m_pParent) m_pParent->DelayLayout();
}

void CXTPControl::RedrawParent(BOOL bAnimate)
{
	if (!IsVisible())
		return;

	if (m_pParent) m_pParent->Redraw(GetRect(), bAnimate);
}

void CXTPControl::ScreenToClient(CPoint* point)
{
	ASSERT(m_pParent);
	if (m_pParent)
		((CWnd*)m_pParent)->ScreenToClient(point);
}


BOOL CXTPControl::OnSetSelected(int bSelected)
{
	if (bSelected == m_bSelected)
		return FALSE;

	if (IsKeyboardSelected(bSelected) && m_pParent->m_nPopuped != GetIndex())
		m_pParent->SetPopuped(-1);

	m_bSelected = bSelected;

	m_pParent->OnControlSelected(bSelected, this);

	if (GetEnabled() || IsKeyboardSelected(bSelected) || !bSelected)
		RedrawParent(!bSelected);

	return TRUE;
}

void CXTPControl::OnMouseHover()
{
	if (m_pParent->SetPopuped(-1))
	{
		if (m_pParent->IsTrackingMode() == TRUE_POPUP && m_pParent->GetPosition() != xtpBarPopup)
			m_pParent->SetTrackingMode(FALSE);
	}
}

void CXTPControl::SetPrompt(LPCTSTR lpszPrompt)
{
	if (lpszPrompt == NULL || lpszPrompt[0] == _T('\0'))
		return;

	if (_tcschr(lpszPrompt, _T('\n')) != NULL)
	{
		AfxExtractSubString(m_strTooltipText, lpszPrompt, 1);
		AfxExtractSubString(m_strDescriptionText, lpszPrompt, 0);
	}
	else
	{
		m_strDescriptionText = m_strTooltipText = lpszPrompt;
	}

	if (m_strCaption.IsEmpty())
	{
		if (!AfxExtractSubString(m_strCaption, lpszPrompt, 2) || m_strCaption.IsEmpty())
		{
			m_strCaption = m_strTooltipText;
			OnCaptionChanged();
		}
	}

	if (m_strKeyboardTip.IsEmpty())
	{
		AfxExtractSubString(m_strKeyboardTip, lpszPrompt, 3);
	}
}

void CXTPControl::SetID(int nId)
{
	if (m_nId == nId)
		return;

	CString strPrompt;

#ifdef _XTP_ACTIVEX
	if (nId > 500)
#endif
	if (XTPResourceManager()->LoadString(&strPrompt, nId))
	{
		SetPrompt(strPrompt);
	}

	m_nId = nId;
}

void CXTPControl::OnActionChanged(int nProperty)
{
	if (nProperty == 3)
	{
		OnCaptionChanged();
	}

	if (nProperty == 0)
	{
		OnEnabledChanged();
	}
}

void CXTPControl::SetCaption(UINT nIDCaption)
{
	CString strCaption;
	if (strCaption.LoadString(nIDCaption))
	{
		SetCaption(strCaption);
	}
}

void CXTPControl::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);
	BOOL bDelayLayout = FALSE;

	int nShortCutPos = strCaption.Find ('\t');
	if (nShortCutPos != -1)
	{
		CString strShortcutTextAuto = strCaption.Mid(nShortCutPos + 1);

		if (m_strShortcutTextAuto != strShortcutTextAuto)
		{
			m_strShortcutTextAuto = strShortcutTextAuto;
			bDelayLayout = TRUE;
		}
		strCaption.ReleaseBuffer(nShortCutPos);
	}

	if (m_strCaption != strCaption)
	{
		m_strCaption = strCaption;
		OnCaptionChanged();
		bDelayLayout = TRUE;
	}

	if (bDelayLayout)
	{
		DelayLayoutParent();
	}
}

AFX_INLINE void NotifyExecute(CXTPControl* pControl, CWnd* pOwner)
{
	NMXTPCONTROL tagNMCONTROL;
#ifndef _XTP_ACTIVEX
	if (pControl->NotifySite(pOwner, CBN_XTP_EXECUTE, &tagNMCONTROL) == 0)
	{
		pOwner->SendMessage(WM_COMMAND, pControl->GetID());
	}
#else
	LRESULT lResult = pControl->NotifySite(pOwner, CBN_XTP_EXECUTE, &tagNMCONTROL);
	if (lResult == 0 || (pControl->GetParent() &&
		::IsWindow(pOwner->GetSafeHwnd()) && pControl->GetParent()->m_bAutoMenuLoad))
	{
		pOwner->SendMessage(WM_COMMAND, pControl->GetID());
	}
#endif
}

//Click Helper
void CXTPControl::ClickToolBarButton(CRect rcActiveRect)
{
	#define XTP_TID_CLICKTICK 0x10AD

	if (rcActiveRect.IsRectEmpty())
		rcActiveRect = m_rcControl;

	m_pParent->SetPopuped(-1);
	m_pParent->SetSelected(m_nIndex);
	m_bPressed = TRUE;
	RedrawParent();

	InternalAddRef();

	BOOL bExecuteOnTimer = m_nExecuteOnPressInterval > 0;
	CWnd* pOwner = m_pParent->GetOwnerSite();

	m_pParent->SetCapture();
	HWND hWndCapture = m_pParent->GetSafeHwnd();
	CPoint pt(0, 0);
	BOOL bClick = FALSE;

	if (bExecuteOnTimer)
	{
		m_pParent->SetTimer(XTP_TID_CLICKTICK, m_nExecuteOnPressInterval, NULL);
		NotifyExecute(this, pOwner);
	}

	while (::GetCapture() == hWndCapture)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (msg.message == WM_LBUTTONUP)
		{
			bClick = m_bSelected && ((!pt.x && !pt.y) || rcActiveRect.PtInRect(pt));
			break;
		}

		if (m_pParent == NULL)
			break;

		if (msg.message == WM_TIMER && msg.wParam == XTP_TID_CLICKTICK)
		{
			if (m_bSelected)
			{
				NotifyExecute(this, pOwner);
			}
		}

		if (msg.message == WM_MOUSEMOVE)
		{
			pt = CPoint(LOWORD(msg.lParam), HIWORD(msg.lParam));
			if (OnSetSelected(rcActiveRect.PtInRect(pt)))
			{
				RedrawParent();
			}
			continue;
		}

		DispatchMessage (&msg);
	}

	if (bExecuteOnTimer && m_pParent->GetSafeHwnd())
	{
		m_pParent->KillTimer(XTP_TID_CLICKTICK);
	}

	m_bPressed = bClick && !bExecuteOnTimer && NeedPressOnExecute() ? TRUE_KEYBOARD : FALSE;

	ReleaseCapture();

	if (!bExecuteOnTimer && m_bSelected && m_pParent)
	{
		m_pParent->SetSelected(-1);
		m_bSelected = FALSE;
		m_pParent->m_nClickedControl = GetIndex();
	}

	if (bClick && !bExecuteOnTimer)
	{
		OnExecute();

		if (m_bPressed)
		{
			m_bPressed = FALSE;
			RedrawParent();
		}
	}
	else if (bClick && m_pParent->GetType() == xtpBarTypePopup && m_bCloseSubMenuOnClick)
	{
		m_bPressed = FALSE;

		OnExecute();
	}
	else
	{
		RedrawParent();
	}

	InternalRelease();
}

LRESULT CXTPControl::NotifySite(UINT code)
{
	if (!m_pParent)
		return 0;

	NMXTPCONTROL tagNMCONTROL;
	return NotifySite(m_pParent->GetOwnerSite(), code, &tagNMCONTROL);
}

LRESULT CXTPControl::NotifySite(UINT code, NMXTPCONTROL* pNM)
{
	if (!m_pParent)
		return 0;

	return NotifySite(m_pParent->GetOwnerSite(), code, pNM);
}

LRESULT CXTPControl::NotifySite(CWnd* pSite, UINT code, NMXTPCONTROL* pNM)
{
	if (pSite == 0)
	{
		if (!m_pParent)
			return 0;

		pSite = m_pParent->GetOwnerSite();
	}

	pNM->hdr.code = code ;
	pNM->hdr.idFrom = GetID();
	pNM->hdr.hwndFrom = 0;
	pNM->pControl = this;

	LRESULT lResult = pSite->SendMessage(WM_XTP_COMMAND, GetID(), (LPARAM)pNM);

	if (lResult || !m_pParent)
		return lResult;

	AFX_NOTIFY notify;
	notify.pResult = &lResult;
	notify.pNMHDR = (NMHDR*)pNM;

	if (pSite->OnCmdMsg(GetID(), MAKELONG(code, WM_NOTIFY), &notify, NULL))
	{
		return lResult;
	}

	return 0;
}


BOOL CXTPControl::NeedPressOnExecute() const
{
	if (m_pParent && m_pParent->GetPosition() == xtpBarPopup)
		return FALSE;

	return TRUE;
}

void CXTPControl::OnExecute()
{
	XTPSoundManager()->PlaySystemSound(xtpSoundMenuCommand);

	m_bPressed = NeedPressOnExecute() ? TRUE_KEYBOARD : FALSE;

	CXTPCommandBar* pCommandBar = m_pParent->GetRootParent();
	BOOL bRecursePopup = pCommandBar->m_bRecursePopup;

	CWnd* pOwner = m_pParent->GetOwnerSite();
	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();

	if (pCommandBars)
	{
		if (m_nId) pCommandBars->SetCommandUsed(m_nId);

		CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, m_pParent);
		if (pPopupBar && pPopupBar->GetControlPopup() && ((CXTPControl*)pPopupBar->GetControlPopup())->GetID())
		{
			pCommandBars->SetCommandUsed(((CXTPControl*)pPopupBar->GetControlPopup())->GetID());
		}
	}

	if (pCommandBar->m_pReturnCmd)
	{
		if (bRecursePopup)
			pCommandBar->OnTrackLost();
		else if (pCommandBars)
			pCommandBars->ClosePopups();
		else
			XTPMouseManager()->SendTrackLost();

		*pCommandBar->m_pReturnCmd = m_nId;
		return;
	}

#ifdef _XTP_ACTIVEX
		m_pParent->m_bAutoMenuLoad = pCommandBar->m_bAutoMenuLoad;
#endif

	InternalAddRef();

	if (m_bCloseSubMenuOnClick)
	{
		if (bRecursePopup)
			pCommandBar->OnTrackLost();
		else if (pCommandBars)
			pCommandBars->ClosePopups();
		else
			XTPMouseManager()->SendTrackLost();
	}

	if (m_nId == 0)
	{
		InternalRelease();
		return;
	}

	NotifyExecute(this, pOwner);

	if ((!m_bCloseSubMenuOnClick || m_bPressed) && m_pParent)
	{
		m_bPressed = FALSE;

		m_pParent->OnIdleUpdateCmdUI(0, 0);
		RedrawParent();
	}

	InternalRelease();
}

BOOL CXTPControl::IsCursorOver() const
{
	CPoint pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);
	return m_rcControl.PtInRect(pt);
}

void CXTPControl::Copy(CXTPControl* pControl, BOOL /*bRecursive*/)
{
	m_nId = pControl->m_nId;
	SetAction(pControl->GetAction());

	m_nTag = pControl->m_nTag;
	m_dwFlags = pControl->m_dwFlags;
	m_controlType = pControl->m_controlType;
	m_strCaption = pControl->m_strCaption;
	m_strShortcutText = pControl->m_strShortcutText;
	m_strShortcutTextAuto = pControl->m_strShortcutTextAuto;
	m_strTooltipText = pControl->m_strTooltipText;
	m_strDescriptionText = pControl->m_strDescriptionText;
	m_strParameter = pControl->m_strParameter;
	m_strKeyboardTip = pControl->m_strKeyboardTip;
	m_strCustomShortcutText = pControl->m_strCustomShortcutText;
	m_nCustomIconId = pControl->m_nCustomIconId;
	m_nIconId = pControl->m_nIconId;
	m_nHelpId = pControl->m_nHelpId;
	m_bTemporary = pControl->m_bTemporary;
	m_strCustomCaption = pControl->m_strCustomCaption;
	m_strCategory = pControl->m_strCategory;
	m_dwHideFlags = pControl->m_dwHideFlags & ~(xtpHideWrap | xtpHideScroll | xtpHideDockingPosition | xtpHideExpand);
	m_bDefaultItem = pControl->m_bDefaultItem;
	m_bEnabled = pControl->m_bEnabled;
	m_bChecked = pControl->m_bChecked;
	m_buttonCustomStyle = pControl->m_buttonCustomStyle;
	m_buttonStyle = pControl->m_buttonStyle;

	m_nWidth = max(GetCustomizeMinWidth(), pControl->m_nWidth);
	m_nHeight = pControl->m_nHeight;

	m_szIcon = pControl->m_szIcon;

	m_bBeginGroup = pControl->m_bBeginGroup;

	m_bCloseSubMenuOnClick = pControl->m_bCloseSubMenuOnClick;
	m_nExecuteOnPressInterval = pControl->m_nExecuteOnPressInterval;

	m_mapDocTemplatesAssigned.Copy(pControl->m_mapDocTemplatesAssigned);
	m_mapDocTemplatesExcluded.Copy(pControl->m_mapDocTemplatesExcluded);

	OnCaptionChanged();
}

BOOL CXTPControl::Compare(CXTPControl* pOther)
{
	if (GetStyle() != pOther->GetStyle())
		return FALSE;

	if (GetType() != pOther->GetType())
		return FALSE;

	if (GetID() != pOther->GetID())
		return FALSE;

	if (GetFlags() != pOther->GetFlags())
		return FALSE;

	if (GetBeginGroup() != pOther->GetBeginGroup())
		return FALSE;

	if (GetIconId() != pOther->GetIconId())
		return FALSE;

	if (m_strCaption != pOther->m_strCaption)
		return FALSE;

	if (m_strTooltipText != pOther->m_strTooltipText)
		return FALSE;

	if (m_strDescriptionText != pOther->m_strDescriptionText)
		return FALSE;

	if (m_strCustomCaption != pOther->m_strCustomCaption)
		return FALSE;

	if (m_buttonCustomStyle != pOther->m_buttonCustomStyle)
		return FALSE;

	if (m_strParameter != pOther->m_strParameter)
		return FALSE;

	if ((m_dwHideFlags & xtpHideCustomize) != (pOther->m_dwHideFlags & xtpHideCustomize))
		return FALSE;

	if (m_nWidth != pOther->m_nWidth)
		return FALSE;

	return TRUE;
}

IMPLEMENT_XTP_CONTROL(CXTPControl, CCmdTarget)


BOOL CXTPControl::IsCustomizeMode() const
{
	return m_pParent ? m_pParent->IsCustomizeMode() : FALSE;
}

void CXTPControl::OnInvertTracker(CDC* pDC, CRect rect)
{
	ASSERT(!rect.IsRectEmpty());

	pDC->InvertRect(CRect(rect.left, rect.top, rect.right, rect.top + 2));
	pDC->InvertRect(CRect(rect.left, rect.bottom - 2, rect.right, rect.bottom));
	pDC->InvertRect(CRect(rect.left, rect.top + 2, rect.left + 2, rect.bottom - 2));
	pDC->InvertRect(CRect(rect.right - 2, rect.top + 2, rect.right, rect.bottom - 2));

}

void CXTPControl::OnCustomizeMouseMove(CPoint point)
{
	if (IsCustomizeResizeAllow() && m_rcControl.PtInRect(point) && ((point.x - m_rcControl.left <= 2) || (m_rcControl.right - point.x <= 2)))
	{
		::SetCursor(XTPResourceManager()->LoadCursor(XTP_IDC_VRESIZE));
	}
}

BOOL CXTPControl::CustomizeStartResize(CPoint point)
{
	if (m_rcControl.PtInRect(point) && ((point.x - m_rcControl.left <= 2) || (m_rcControl.right - point.x <= 2)))
	{
		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		ASSERT(pCommandBars);
		ASSERT(pCommandBars->m_pDragSelected == this);
		pCommandBars->m_pDragSelected = NULL;
		m_pParent->Redraw();
		m_pParent->UpdateWindow();

		CRect rectTracker = m_rcControl;
		m_pParent->ClientToScreen(rectTracker);

		m_pParent->SetCapture();
		::SetCursor(XTPResourceManager()->LoadCursor(XTP_IDC_VRESIZE));

		CDC* pDC = 0;

		CWnd* pWnd = CWnd::GetDesktopWindow();
		if (pWnd->LockWindowUpdate())
			pDC = pWnd->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
		else
			pDC = pWnd->GetDCEx(NULL, DCX_WINDOW | DCX_CACHE);

		OnInvertTracker(pDC, rectTracker);
		int nMinWidth = GetCustomizeMinWidth();
		BOOL bLeftAnchor = (point.x - m_rcControl.left <= 2);

		LONG& lTrackerAnchor = bLeftAnchor ? rectTracker.left : rectTracker.right;
		int nOffset = bLeftAnchor ? m_rcControl.left - point.x : m_rcControl.right - point.x;

		BOOL bAccept = FALSE;
		while (CWnd::GetCapture() == m_pParent)
		{
			MSG msg;
			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			if (msg.message == WM_MOUSEMOVE)
			{
				point = CPoint(msg.lParam);
				m_pParent->ClientToScreen(&point);
				point.x += nOffset;

				point.x = bLeftAnchor ? min(point.x, rectTracker.right - nMinWidth) :
					max(point.x, rectTracker.left + nMinWidth);

				if (lTrackerAnchor != point.x)
				{
					OnInvertTracker(pDC, rectTracker);
					lTrackerAnchor = point.x;
					OnInvertTracker(pDC, rectTracker);
				}
			}
			else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) break;
			else if (msg.message == WM_LBUTTONUP)
			{
				bAccept = TRUE;
				break;
			}
			else  ::DispatchMessage(&msg);
		}

		OnInvertTracker(pDC, rectTracker);

		if (CWnd::GetCapture() == m_pParent) ReleaseCapture();

		pWnd->UnlockWindowUpdate();
		if (pDC != NULL)
		{
			pWnd->ReleaseDC(pDC);
			pDC = NULL;
		}

		pCommandBars->m_pDragSelected = this;

		int nWidth = rectTracker.Width();

		if (bAccept && rectTracker.Width() != m_rcControl.Width())
		{
			if (GetParent()->GetType() == xtpBarTypePopup)
				nWidth -= GetPaintManager()->GetPopupBarGripperWidth(GetParent());

			SetWidth(nWidth);
			m_pParent->OnRecalcLayout();
		}
		m_pParent->Redraw();

		return TRUE;

	}

	return FALSE;
}

void CXTPControl::CustomizeStartDrag(CPoint pt)
{
	ASSERT(m_pParent);
	if (!m_pParent)
		return;

	if (m_pParent->m_nPopuped != m_nIndex)
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(-1);
	}

	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
	ASSERT(pCommandBars);
	if (!pCommandBars)
		return;

	if ((pCommandBars->m_pDragSelected == this) && IsCustomizeResizeAllow() &&
		CustomizeStartResize(pt))
	{
		return;
	}

	pCommandBars->SetDragControl(this);

	if (GetFlags() & xtpFlagNoMovable)
		return;

	CXTPCustomizeDropSource* pDropSource = pCommandBars->GetDropSource();
	DROPEFFECT dropEffect = pDropSource->DoDragDrop(this);

	if (dropEffect == DROPEFFECT_NONE || dropEffect == DROPEFFECT_MOVE)
	{
		CXTPCommandBar* pParent = m_pParent;
		pParent->SetPopuped(-1);
		pParent->SetSelected(-1);

		if (pCommandBars->GetDragControl() == this)
			pCommandBars->SetDragControl(NULL);


		if (GetBeginGroup() && m_nIndex < m_pControls->GetCount() - 1)
			m_pControls->GetAt(m_nIndex + 1)->SetBeginGroup(TRUE);

		if (m_nIndex == 0 && m_pControls->GetCount() > 1)
			m_pControls->GetAt(1)->SetBeginGroup(FALSE);

		m_pControls->Remove(this);

		pParent->OnRecalcLayout();
	}
	pCommandBars->SetDragControl(pCommandBars->GetDragControl());
}

void CXTPControl::OnCustomizeDragOver(CXTPControl* /*pDataObject*/, CPoint /*point*/, DROPEFFECT& /*dropEffect*/)
{
	m_pParent->SetPopuped(-1);
	m_pParent->SetSelected(-1);
}
BOOL CXTPControl::IsCustomizeDragOverAvail(CXTPCommandBar* /*pCommandBar*/, CPoint /*point*/, DROPEFFECT& /*dropEffect*/)
{
	return TRUE;
}

BOOL CXTPControl::IsCustomizeResizeAllow() const
{
	return FALSE;
}

CXTPImageManager* CXTPControl::GetImageManager() const
{
	if (m_pParent)
		return m_pParent->GetImageManager();

	if (m_pControls)
	{
		CXTPCommandBars* pCommandBars = m_pControls->GetCommandBars();
		if (pCommandBars)
			return pCommandBars->GetImageManager();
	}
	return XTPImageManager();
}
CXTPPaintManager* CXTPControl::GetPaintManager() const
{
	if (m_pParent)
		return m_pParent->GetPaintManager();

	if (m_pControls)
	{
		CXTPCommandBars* pCommandBars = m_pControls->GetCommandBars();
		if (pCommandBars)
			return pCommandBars->GetPaintManager();
	}
	return XTPPaintManager();

}
CXTPImageManagerIcon* CXTPControl::GetImage(int nWidth) const
{
	if (GetIconId() != 0)
		return GetImageManager()->GetImage(GetIconId(), nWidth);
	return NULL;

}

void CXTPControl::SetExpanded(BOOL bExpanded)
{
	m_bExpanded = bExpanded;
	if (!bExpanded) SetHideFlags(GetHideFlags() & ~xtpHideExpand);
}

BOOL CXTPControl::IsItemDefault() const
{
	return m_bDefaultItem;
}

void CXTPControl::SetItemDefault(BOOL bDefault)
{
	if (bDefault != m_bDefaultItem)
	{
		m_bDefaultItem = bDefault;
		DelayLayoutParent();
	}
}

void CXTPControl::SetFlags(DWORD dwFlags)
{
	m_dwFlags = dwFlags;
}

DWORD CXTPControl::GetFlags() const
{
	return m_dwFlags;
}

BOOL CXTPControl::IsCustomizeMovable() const
{
	if (m_pParent && m_pParent->IsCustomizable() && ((GetFlags() & xtpFlagNoMovable) == 0))
		return TRUE;

	if (!m_pParent)
		return FALSE;
	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();

	if (pCommandBars && pCommandBars->m_bDesignerMode)
		return TRUE;

	return FALSE;
}

void CXTPControl::CDocTemplateMap::Copy(CDocTemplateMap& map)
{
	RemoveAll();

	UINT nIDResource;
	BOOL bValue;
	POSITION pos = map.GetStartPosition();
	while (pos)
	{
		map.GetNextAssoc(pos, nIDResource, bValue);
		SetAt(nIDResource, bValue);
	}
}

CXTPControl* CXTPControl::FromUI(CCmdUI* pCmdUI)
{
	CXTPCommandBar* pCommandBar = DYNAMIC_DOWNCAST(CXTPCommandBar, pCmdUI->m_pOther);
	if (!pCommandBar)
		return NULL;

	CXTPControls* pControls = pCommandBar->GetControls();
	if ((int)pCmdUI->m_nIndex < pControls->GetCount())
		return pControls->GetAt(pCmdUI->m_nIndex);

	return NULL;

}

void CXTPControl::OnLButtonDown(CPoint point)
{
	if (!m_pParent->IsCustomizeMode() || IsCustomizeMovable())
		OnClick(FALSE, point);
}

BOOL CXTPControl::OnLButtonDblClk(CPoint point)
{
	OnClick(FALSE, point);
	return TRUE;
}

void CXTPControl::Reset()
{
	if (m_nCustomIconId != 0 || !m_strCustomCaption.IsEmpty() || m_buttonCustomStyle != xtpButtonUndefined)
	{
		m_nCustomIconId = 0;
		m_buttonCustomStyle = xtpButtonUndefined;

		if (!m_strCustomCaption.IsEmpty())
		{
			m_strCustomCaption = _T("");
			OnCaptionChanged();
		}
		GetParent()->OnRecalcLayout();
	}
}

void CXTPControl::SetCustomIcon(HICON hIcon)
{
	CXTPImageManager* pImageManager = GetImageManager();
	if (pImageManager)
	{
		m_nCustomIconId = pImageManager->AddCustomIcon(hIcon);
	}
}

CSize CXTPControl::GetButtonSize() const
{
	return m_pParent->GetButtonSize();
}

CSize CXTPControl::GetIconSize() const
{
	if (m_szIcon != CSize(0, 0))
		return m_szIcon;

	return m_pParent->GetIconSize();
}


BOOL CXTPControl::GetEnabled() const
{
	BOOL bEnabled = m_pParent ? m_pParent->IsControlEnabled(this) : -1;
	if (bEnabled != -1)
		return bEnabled;

	return m_bEnabled == -1 && m_pAction ? m_pAction->GetEnabled() : m_bEnabled;
}


#ifndef _XTP_INCLUDE_RIBBON
BOOL CXTPControl::HasDwmCompositedRect() const
{
	if (!GetParent())
		return FALSE;

	if (GetParent()->IsDwmEnabled())
		return TRUE;

	return FALSE;
}
#endif

void CXTPControl::OnUnderlineActivate()
{
	OnExecute();
}

BOOL CXTPControl::IsCaptionVisible() const
{
	if (GetParent()->GetType() == xtpBarTypePopup)
		return TRUE;

	if (GetCaption().IsEmpty())
		return FALSE;

	XTPButtonStyle buttonStyle = GetStyle();

	if ((buttonStyle == xtpButtonCaption) || (buttonStyle == xtpButtonIconAndCaption) || (buttonStyle == xtpButtonIconAndCaptionBelow))
		return TRUE;

	if (buttonStyle == xtpButtonIcon)
		return FALSE;

	switch (GetType())
	{
		case xtpControlPopup:
		case xtpControlLabel:
		case xtpControlCheckBox:
		case xtpControlRadioButton:
			return TRUE;

		case xtpControlButton:
		case xtpControlButtonPopup:
		case xtpControlSplitButtonPopup:
			CXTPImageManagerIcon* pImage = GetImage(0);
			return pImage == NULL;
	}
	return FALSE;
}


CCmdTarget* CXTPControl::GetAccessible()
{
	return this;
}

HRESULT CXTPControl::GetAccessibleParent(IDispatch* FAR* ppdispParent)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	*ppdispParent = NULL;

	if (m_pParent)
	{
		*ppdispParent = m_pParent->GetIDispatch(TRUE);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CXTPControl::GetAccessibleChildCount(long FAR* pChildCount)
{
	if (pChildCount == 0)
		return E_INVALIDARG;

	*pChildCount = GetCommandBar() ? 1 : 0;

	return S_OK;
}
HRESULT CXTPControl::GetAccessibleChild(VARIANT varChild, IDispatch* FAR* ppdispChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	*ppdispChild = NULL;

	if (GetChildIndex(&varChild) == 1)
	{
		CXTPCommandBar* pCommandBar = GetCommandBar();
		if (pCommandBar)
		{
			*ppdispChild = pCommandBar->GetIDispatch(TRUE);
		}
	}

	return S_OK;
}

HRESULT CXTPControl::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	CString strCaption = GetCaption();
	CXTPPaintManager::StripMnemonics(strCaption);

	if (!GetShortcutText().IsEmpty())
		strCaption = strCaption + _T('\t') + GetShortcutText();

	if (strCaption.IsEmpty())
	{
		strCaption = GetTooltip();
	}

	*pszName = strCaption.AllocSysString();
	return S_OK;
}

HRESULT CXTPControl::GetAccessibleKeyboardShortcut(VARIANT varChild, BSTR* pszKeyboardShortcut)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	CString strCaption = GetCaption();
	if (strCaption.IsEmpty())
	{
		*pszKeyboardShortcut = SysAllocString(L"");
	}
	else
	{
		int nIndex = CXTPShortcutManager::FindAccelPos(strCaption);
		CString strShortcut = CString(strCaption[nIndex + 1]);

		*pszKeyboardShortcut = strShortcut.AllocSysString();
	}


	return S_OK;

}

HRESULT CXTPControl::GetAccessibleDescription(VARIANT varChild, BSTR* pszDescription)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	*pszDescription = GetDescription().AllocSysString();
	return S_OK;
}

HRESULT CXTPControl::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	pvarRole->vt = VT_EMPTY;

	if (m_pParent && m_pParent->GetSafeHwnd() && GetChildIndex(&varChild) == CHILDID_SELF)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = m_pParent->GetType() == xtpBarTypeNormal || m_pParent->GetType() == xtpBarTypeRibbon ?
			GetCommandBar() ? ROLE_SYSTEM_BUTTONMENU: ROLE_SYSTEM_PUSHBUTTON : ROLE_SYSTEM_MENUITEM;
		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPControl::AccessibleSelect(long /*flagsSelect*/, VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->SetTrackingMode(TRUE, FALSE);
		m_pParent->SetSelected(m_nIndex, TRUE);
	}

	return S_OK;
}

#ifndef STATE_SYSTEM_HASPOPUP
#define STATE_SYSTEM_HASPOPUP   (0x40000000)
#endif //STATE_SYSTEM_HASPOPUP


HRESULT CXTPControl::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	pvarState->vt = VT_I4;
	pvarState->lVal = STATE_SYSTEM_FOCUSABLE |
		(m_bSelected ? STATE_SYSTEM_FOCUSED | STATE_SYSTEM_SELECTED | STATE_SYSTEM_HOTTRACKED : 0);

	if (!m_pParent->IsVisible() || !IsVisible())
		pvarState->lVal |= STATE_SYSTEM_INVISIBLE;

	if (!GetEnabled())
		pvarState->lVal |= STATE_SYSTEM_UNAVAILABLE;

	if (GetChecked())
		pvarState->lVal |= STATE_SYSTEM_CHECKED;

	if (GetPressed())
		pvarState->lVal |= STATE_SYSTEM_PRESSED;

	if (GetCommandBar())
		pvarState->lVal |= STATE_SYSTEM_HASPOPUP;

	return S_OK;
}

HRESULT CXTPControl::GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	*pszDefaultAction = SysAllocString(L"Click");

	return S_OK;
}

HRESULT CXTPControl::AccessibleDoDefaultAction(VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	OnClick(TRUE);

	return S_OK;
}


HRESULT CXTPControl::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	if (!m_pParent->GetSafeHwnd())
		return S_OK;

	if (!IsVisible())
		return S_OK;

	CRect rcControl = GetRect();
	m_pParent->ClientToScreen(&rcControl);

	*pxLeft = rcControl.left;
	*pyTop = rcControl.top;
	*pcxWidth = rcControl.Width();
	*pcyHeight = rcControl.Height();

	return S_OK;
}

HRESULT CXTPControl::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (pvarID == NULL)
		return E_INVALIDARG;

	pvarID->vt = VT_EMPTY;

	if (!m_pParent->GetSafeHwnd())
		return S_FALSE;

	if (!CXTPWindowRect(m_pParent).PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	pvarID->vt = VT_I4;
	pvarID->lVal = 0;

	CPoint pt(xLeft, yTop);
	m_pParent->ScreenToClient(&pt);

	if (!GetRect().PtInRect(pt))
		return S_FALSE;

	return S_OK;
}

void CXTPControl::SetVisible(BOOL bVisible)
{
	DWORD dwHideFlags = m_dwHideFlags;
	if (!bVisible) SetHideFlags(m_dwHideFlags | xtpHideGeneric); else SetHideFlags(m_dwHideFlags & ~xtpHideGeneric);
	if (dwHideFlags != m_dwHideFlags) DelayLayoutParent();
}

BOOL CXTPControl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == CN_EVENT && nID == XTP_CN_REDRAWPARENT)
	{
		RedrawParent(FALSE);
		return TRUE;
	}
	return CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CXTPControl::IsLayoutRTL() const
{
	return m_pParent && m_pParent->GetExStyle() & (WS_EX_LAYOUTRTL | WS_EX_RTLREADING);
}

BOOL CXTPControl::IsLayoutChanged()
{
	if ( m_bLayoutRTL != IsLayoutRTL())
	{
		m_bLayoutRTL = IsLayoutRTL();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlAction, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPControlAction, "Caption", dispidCaption, OleGetCaption, OleSetCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlAction, "DescriptionText", dispidDescriptionText, m_strDescriptionText, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlAction, "Id", dispidId, m_nId, VT_I4)
	DISP_PROPERTY_ID(CXTPControlAction, "ShortcutText", dispidShortcutText, m_strShortcutText, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlAction, "TooltipText", dispidTooltipText, m_strTooltipText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControlAction, "Visible", dispidVisible, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlAction, "Enabled", dispidEnabled, GetEnabled, SetEnabled, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlAction, "Checked", dispidChecked, GetChecked, SetChecked, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlAction, "IconId", dispidIconId, GetIconId, SetIconId, VT_I4)
	DISP_PROPERTY_ID(CXTPControlAction, "Category", dispidCategory, m_strCategory, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlAction, "Tag", dispidParameter, m_oleTag, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPControlAction, "EditHint", 55, OleGetEditHint, SetEditHint, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlAction, "KeyboardTip", 56, m_strKeyboardTip, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlAction, "Key", 57, m_strKey, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlAction, "CustomShortcutText", 58, m_strCustomShortcutText, VT_BSTR)

END_DISPATCH_MAP()

// {DEC5479A-9E07-4079-924A-4CB1C11FC700}
static const GUID IID_ICommandBarAction =
{ 0xdec5479a, 0x9e07, 0x4079, { 0x92, 0x4a, 0x4c, 0xb1, 0xc1, 0x1f, 0xc7, 0x0 } };

IMPLEMENT_OLETYPELIB_EX(CXTPControlAction, IID_ICommandBarAction)

void CXTPControlAction::OleSetCaption(LPCTSTR lpszCaption)
{
	SetCaption(CString(lpszCaption));
}

BSTR CXTPControlAction::OleGetCaption()
{
	return GetCaption().AllocSysString();
}

BSTR CXTPControlAction::OleGetEditHint()
{
	return GetEditHint().AllocSysString();
}


//////////////////////////////////////////////////////////////////////////


BEGIN_DISPATCH_MAP(CXTPControl, CCmdTarget)
	DISP_FUNCTION_ID(CXTPControl, "Controls", dispidControls, OleControls, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControl, "Parent", dispidParent, OleParent, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPControl, "Caption", dispidCaption, OleGetCaption, OleSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControl, "BeginGroup", dispidBeginGroup, GetBeginGroup, SetBeginGroup, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControl, "DescriptionText", dispidDescriptionText, OleGetDescription, OleSetDescription, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControl, "Id", dispidId, m_nId, VT_I4)
	DISP_PROPERTY_ID(CXTPControl, "Parameter", dispidParameter, m_strParameter, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControl, "ShortcutText", dispidShortcutText, OleGetShortcutText, OleSetShortcutText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControl, "TooltipText", dispidTooltipText, OleGetTooltipText, OleSetTooltipText, VT_BSTR)
	DISP_FUNCTION_ID(CXTPControl, "Index", dispidIndex, OleGetIndex, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControl, "Type", dispidType, OleGetType, VT_I4, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPControl, "Visible", dispidVisible, OleGetVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControl, "Enabled", dispidEnabled, GetEnabled, SetEnabled, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControl, "Checked", dispidChecked, GetChecked, SetChecked, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControl, "IconId", dispidIconId, GetIconId, SetIconId, VT_I4)
	DISP_FUNCTION_ID(CXTPControl, "Delete", dispidDelete, OleDelete, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPControl, "DefaultItem", dispidDefaultItem, IsItemDefault, SetItemDefault, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControl, "Flags", dispidFalgs, GetFlags, SetFlags, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControl, "Category", dispidCategory, OleGetCategory, OleSetCategory, VT_BSTR)
	DISP_FUNCTION_ID(CXTPControl, "Copy", dispidCopy, OleCopy, VT_DISPATCH, VTS_DISPATCH VTS_VARIANT)

	DISP_FUNCTION_ID(CXTPControl, "SetFocus", dispidSetFocus, OleSetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControl, "Execute", dispidExecute, OleExecute, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_ID(CXTPControl, "HideFlags", 23, m_dwHideFlags, VT_I4)
	DISP_PROPERTY_ID(CXTPControl, "CloseSubMenuOnClick", 24, m_bCloseSubMenuOnClick, VT_BOOL)

	DISP_FUNCTION_ID(CXTPControl, "GetRect", 25, OleGetRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_PROPERTY_EX_ID(CXTPControl, "RibbonGroup", 26, OleGetRibbonGroup, SetNotSupported, VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPControl, "Reset", 27, Reset, VT_EMPTY, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPControl, "Style", 28, OleGetStyle, OleSetStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControl, "Action", 29, OleGetAction, OleSetAction, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPControl, "ExecuteOnPressInterval", 30, m_nExecuteOnPressInterval, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControl, "Width", 31, OleGetWidth, OleSetWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControl, "Height", 32, OleGetHeight, OleSetHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControl, "Top", 33, OleGetTop, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControl, "Left", 34, OleGetLeft, SetNotSupported, VT_I4)
	DISP_FUNCTION_ID(CXTPControl, "Clone", 35, OleClone, VT_DISPATCH, VTS_VARIANT)
	DISP_PROPERTY_ID(CXTPControl, "KeyboardTip", 36, m_strKeyboardTip, VT_BSTR)
	DISP_FUNCTION_ID(CXTPControl, "SetIconSize", 37, OleSetIconSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPControl, "CommandBar", 50, OleGetCommandBar, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPControl, "CustomShortcutText", 90, OleGetCustomShortcutText, OleSetCustomShortcutText, VT_BSTR)

END_DISPATCH_MAP()

// {1FC5479A-9E07-4079-924A-4CB1C11FC700}
static const GUID IID_ICommandBarControl =
{ 0x1fc5479a, 0x9e07, 0x4079, { 0x92, 0x4a, 0x4c, 0xb1, 0xc1, 0x1f, 0xc7, 0x0 } };

IMPLEMENT_OLETYPELIB_EX(CXTPControl, IID_ICommandBarControl)

int CXTPControl::OleGetWidth()
{
	return GetWidth() > 0 ? GetWidth() : IsVisible() ? m_rcControl.Width() : 0;
}

void CXTPControl::OleSetWidth(int nWidth)
{
	SetWidth(nWidth);
}

int CXTPControl::OleGetHeight()
{
	return GetHeight() > 0 ? GetHeight() : IsVisible() ? m_rcControl.Height() : 0;
}

void CXTPControl::OleSetHeight(int nHeight)
{
	SetHeight(nHeight);
}

int CXTPControl::OleGetTop()
{
	return IsVisible() ? m_rcControl.top : 0;
}

int CXTPControl::OleGetLeft()
{
	return IsVisible() ? m_rcControl.left : 0;
}

BOOL CXTPControl::OleGetVisible()
{
	return IsVisible();
}

void CXTPControl::OleSetCaption(LPCTSTR lpszCaption)
{
	SetCaption(CString(lpszCaption));
}

BSTR CXTPControl::OleGetCaption()
{
	return GetCaption().AllocSysString();
}

void CXTPControl::OleSetDescription(LPCTSTR lpszCaption)
{
	SetDescription(lpszCaption);
}

LPDISPATCH CXTPControl::OleGetAction()
{
	return XTPGetDispatch(m_pAction);
}

void CXTPControl::OleSetAction(LPDISPATCH lpDisp)
{
	CXTPControlAction* pAction = DYNAMIC_DOWNCAST(CXTPControlAction, CCmdTarget::FromIDispatch(lpDisp));
	SetAction(pAction);
}

BSTR CXTPControl::OleGetDescription()
{
	return GetDescription().AllocSysString();
}

void CXTPControl::OleSetTooltipText(LPCTSTR lpszCaption)
{
	SetTooltip(lpszCaption);
}


BSTR CXTPControl::OleGetCategory()
{
	return GetCategory().AllocSysString();
}

void CXTPControl::OleSetCategory(LPCTSTR lpszCaption)
{
	SetCategory(lpszCaption);
}
BSTR CXTPControl::OleGetTooltipText()
{
	return GetTooltip().AllocSysString();
}

void CXTPControl::OleSetShortcutText(LPCTSTR lpszCaption)
{
	SetShortcutText(lpszCaption);
}

BSTR CXTPControl::OleGetShortcutText()
{
	return GetShortcutText().AllocSysString();
}

void CXTPControl::OleSetCustomShortcutText(LPCTSTR lpszCustomShortcutText)
{
	SetCustomShortcutText(lpszCustomShortcutText);
}

BSTR CXTPControl::OleGetCustomShortcutText()
{
	return GetCustomShortcutText().AllocSysString();
}

LPDISPATCH CXTPControl::OleControls()
{
	return XTPGetDispatch(m_pControls);
}

LPDISPATCH CXTPControl::OleParent()
{
	return XTPGetDispatch(m_pParent);
}

long CXTPControl::OleGetIndex()
{
	return m_nIndex + 1;
}
long CXTPControl::OleGetType()
{
	return m_controlType;
}
void CXTPControl::OleDelete()
{
	if (m_pControls) m_pControls->Remove(this);
}

void CXTPControl::OleSetFocus()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->SetTrackingMode(TRUE, FALSE);
		m_pParent->SetSelected(m_nIndex, TRUE);
	}
}

void CXTPControl::OleExecute()
{
	OnClick(TRUE);
}

LPDISPATCH CXTPControl::OleClone(const VARIANT& varRecursive)
{
	BOOL bRecursive = VariantToBool(&varRecursive, FALSE);

	CXTPControl* pControl = Clone(bRecursive);

	return XTPGetDispatch(pControl, FALSE);
}

LPDISPATCH CXTPControl::OleCopy(LPDISPATCH lpDispBar, const VARIANT& varBefore)
{
	CXTPCommandBar* pCommandBar = CXTPCommandBar::FromDispatch(lpDispBar);
	if (!pCommandBar)
		return NULL;

	CXTPControl* pControl = Clone(FALSE);

	int nBefore = VariantToLong(&varBefore) - 1;

	pCommandBar->GetControls()->Add(pControl, pControl->GetID(), pControl->GetParameter(), nBefore);


	return XTPGetDispatch(pControl);

}

LPDISPATCH CXTPControl::OleGetRibbonGroup()
{
	return XTPGetDispatch((CCmdTarget*)m_pRibbonGroup);

}

void CXTPControl::OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom)
{
	if (pLeft == NULL || pTop == NULL || pRight == NULL || pBottom == NULL)
	{
		return;
	}
	if (!m_pParent || !m_pParent->m_hWnd)
		return;

	CRect rc(m_rcControl);

	m_pParent->OlePixelToUnits(rc);

	*pLeft = rc.left;
	*pRight = rc.right;
	*pTop = rc.top;
	*pBottom = rc.bottom;
}

int CXTPControl::OleGetStyle()
{
	return GetStyle();
}

void CXTPControl::OleSetStyle(int nStyle)
{
	SetStyle((XTPButtonStyle)nStyle);
}

void CXTPControl::OleSetIconSize(long cx, long cy)
{
	SetIconSize(CSize(cx, cy));
}


LPDISPATCH CXTPControl::OleGetCommandBar()
{
	return XTPGetDispatch(GetCommandBar());
}

#endif


BEGIN_INTERFACE_MAP(CXTPControl, CCmdTarget)
	INTERFACE_PART(CXTPControl, IID_IAccessible, ExternalAccessible)
#ifdef _XTP_ACTIVEX
	INTERFACE_PART(CXTPControl, IID_ICommandBarControl, Dispatch)
#endif
END_INTERFACE_MAP()
