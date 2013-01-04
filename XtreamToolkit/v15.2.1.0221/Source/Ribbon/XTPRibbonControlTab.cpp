// XTPRibbonTab.cpp: implementation of the CXTPRibbonTab class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#include "Common/XTPPropExchange.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"

#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPMouseManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"

#include "XTPRibbonControlTab.h"
#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonTab.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_XTP_CONTROL(CXTPRibbonControlTab, CXTPControl)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPRibbonControlTab::CXTPRibbonControlTab()
{
	m_bAllowReorder = FALSE;
	m_bFocused = FALSE;
	m_bTemporary = TRUE;
	SetFlags(xtpFlagNoMovable);
}

CXTPRibbonControlTab::~CXTPRibbonControlTab()
{
}

CXTPRibbonTab* CXTPRibbonControlTab::FindTab(int nID) const
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		CXTPRibbonTab* pTab = (CXTPRibbonTab*)GetItem(i);
		if (pTab->GetID() == nID)
			return pTab;
	}
	return NULL;
}

CXTPRibbonTab* CXTPRibbonControlTab::GetTab(int nIndex) const
{
	return (CXTPRibbonTab*)GetItem(nIndex);
}

void CXTPRibbonControlTab::Reposition()
{
	if (!m_pParent->GetSafeHwnd())
		return;

	m_pParent->OnRecalcLayout();
}

BOOL CXTPRibbonControlTab::IsMouseLocked() const
{
	return GetRibbonBar()->GetCommandBars()->GetMouseManager()->IsMouseLocked();
}

CXTPTabPaintManager* CXTPRibbonControlTab::GetPaintManager() const
{
	return GetRibbonBar()->GetTabPaintManager();
}

void CXTPRibbonControlTab::ShowPopupBar(BOOL bKeyboard)
{
	if (!GetSelectedItem())
		return;

	CXTPRibbonBar* pRibbonBar = GetRibbonBar();
	if (!pRibbonBar->IsRibbonMinimized())
		return;

	if (pRibbonBar->m_nPopuped == m_nIndex)
	{
		pRibbonBar->m_nPopuped = -1;
		if (m_pCommandBar) m_pCommandBar->SetTrackingMode(FALSE);
	}

	pRibbonBar->SetTrackingMode(TRUE, FALSE);
	pRibbonBar->SetPopuped(m_nIndex, bKeyboard);
}

void CXTPRibbonControlTab::SetEnabled(BOOL /*bEnabled*/)
{

}

CString CXTPRibbonControlTab::GetItemTooltip(const CXTPTabManagerItem* pItem) const
{
	CString strToolTip = CXTPTabManager::GetItemTooltip(pItem);
	if (!strToolTip.IsEmpty())
		return strToolTip;

	return pItem->GetCaption();
}

void CXTPRibbonControlTab::OnItemClick(CXTPTabManagerItem* pItem)
{
	CXTPRibbonBar* pRibbonBar = GetRibbonBar();

	if (pRibbonBar->IsCustomizable() && pRibbonBar->IsCustomizeMode())
	{
		CXTPRibbonTab* pTab = (CXTPRibbonTab*)pItem;
		pRibbonBar->GetCommandBars()->SetDragControl(pTab);
	}

	CXTPTabManager::OnItemClick(pItem);
}

void CXTPRibbonControlTab::SetSelectedItem(CXTPTabManagerItem* pItem)
{
	if (!m_pParent->GetSafeHwnd())
	{
		CXTPTabManager::SetSelectedItem(pItem);
		return;
	}

	if (!IsFocused() && pItem && !GetRibbonBar()->IsRibbonMinimized())
		m_pParent->GetCommandBars()->ClosePopups();

	if (m_pSelected == pItem)
		return;

	if (m_pParent->GetCommandBars()->m_keyboardTips.nLevel > 1)
		m_pParent->GetCommandBars()->HideKeyboardTips();

	CXTPRibbonTab* pTab = (CXTPRibbonTab*)pItem;

	CXTPRibbonBar* pRibbonBar = GetRibbonBar();

	if (pRibbonBar->OnTabChanging(pTab))
		return;

	pRibbonBar->LockRedraw();

	pRibbonBar->RebuildControls(pTab);

	CXTPTabManager::SetSelectedItem(pTab);

	pRibbonBar->OnIdleUpdateCmdUI(0, 0);

	pRibbonBar->UnlockRedraw();

	pRibbonBar->OnTabChanged(pTab);

	NotifySite(CBN_XTP_EXECUTE);

	if (pItem)
	{
		AccessibleNotifyWinEvent(EVENT_OBJECT_FOCUS , GetParent()->GetSafeHwnd(), GetID(), pItem->GetIndex() + 1);
	}
}

void CXTPRibbonControlTab::AdjustExcludeRect(CRect& rc, BOOL /*bVertical*/)
{
	rc.bottom--;
}


BOOL CXTPRibbonControlTab::DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) const
{
	if (!pItem)
		return GetPaintManager()->m_bShowIcons;

	if (pItem->GetImageIndex() == -1 || GetPaintManager()->m_bShowIcons == FALSE)
		return FALSE;

	CXTPImageManagerIcon* pImage = GetImageManager()->GetImage(pItem->GetImageIndex(), szIcon.cx);

	if (!pImage)
		return FALSE;

	if (!bDraw)
	{
		return TRUE;
	}

	pItem->DrawImage(pDC, CRect(pt, szIcon), pImage);

	return TRUE;
}

void CXTPRibbonControlTab::RedrawControl(LPCRECT lpRect, BOOL bAnimate)
{
	m_pParent->Redraw(lpRect, bAnimate);
}

void CXTPRibbonControlTab::OnUnderlineActivate()
{
	SetFocused(TRUE);
}


BOOL CXTPRibbonControlTab::HeaderHasFocus() const
{
	return IsFocused() && m_pParent->IsTrackingMode();
}

BOOL  CXTPRibbonControlTab::OnSetSelected(int bSelected)
{
	if (bSelected == m_bSelected)
		return FALSE;

	if (IsKeyboardSelected(bSelected) && !GetRibbonBar()->IsRibbonMinimized())
		m_pParent->SetPopuped(-1);

	m_bSelected = bSelected;

	m_pParent->OnControlSelected(bSelected, this);

	if (IsKeyboardSelected(bSelected))
	{
		SetFocused(TRUE);
	}

	if (!bSelected)
	{
		SetFocused(FALSE);
		PerformMouseMove(m_pParent->GetSafeHwnd(), CPoint(-1, -1));
	}
	else
	{
		AccessibleNotifyWinEvent(EVENT_OBJECT_FOCUS , GetParent()->GetSafeHwnd(), GetID(), GetCurSel() + 1);
	}

	return TRUE;
}

BOOL CXTPRibbonControlTab::IsFocusable() const
{
	return TRUE;
}


BOOL CXTPRibbonControlTab::IsFocused() const
{
	return m_bFocused;
}

void CXTPRibbonControlTab::Draw(CDC* /*pDC*/)
{

}

BOOL CXTPRibbonControlTab::IsPopupBarTracking() const
{
	return m_pCommandBar && m_pCommandBar->IsTrackingMode();
}

void CXTPRibbonControlTab::SetFocused(BOOL bFocused)
{
	if (m_bFocused == bFocused)
		return;

	m_bFocused = bFocused;

	if (m_bFocused && GetSelectedItem() == NULL)
	{
		SetSelectedItem(FindNextFocusable(-1, +1));
	}

	RedrawParent();
}

BOOL CXTPRibbonControlTab::OnHookKeyDown(UINT nChar, LPARAM /*lParam*/)
{
	if (!IsFocused())
		return FALSE;

	UINT nCharLayout(nChar);
	CXTPDrawHelpers::KeyToLayout(m_pParent, nCharLayout);

	if (nCharLayout == VK_UP || nCharLayout == VK_DOWN || nCharLayout == VK_LEFT || nCharLayout == VK_RIGHT)
	{
		m_pParent->HideKeyboardTips();
	}

	if (nCharLayout == VK_LEFT && FindNextFocusable(GetCurSel(), -1))
	{
		if (PerformKeyDown(m_pParent->GetSafeHwnd(), nChar))
			return TRUE;
	}

	if (nCharLayout == VK_RIGHT && FindNextFocusable(GetCurSel(), +1))
	{
		if (PerformKeyDown(m_pParent->GetSafeHwnd(), nChar))
			return TRUE;
	}


	if (nCharLayout == VK_DOWN)
	{
		m_pParent->SetSelected(m_pControls->GetNext(CXTPControl::GetIndex(), +1), TRUE_KEYBOARD_NEXT);
		return TRUE;
	}

	if (nCharLayout == VK_RETURN || nCharLayout == VK_SPACE && GetRibbonBar()->IsRibbonMinimized())
	{
		ShowPopupBar(TRUE);
		return TRUE;
	}
	return FALSE;
}

void CXTPRibbonControlTab::OnClick(BOOL /*bKeyboard*/, CPoint /*pt*/)
{
	// Do Nothing
}

BOOL CXTPRibbonControlTab::OnSetPopup(BOOL bPopup)
{
	m_bPopup = bPopup;
	if (bPopup)
	{
		ASSERT(GetSelectedItem());
		CXTPRibbonTab* pSelectedTab = (CXTPRibbonTab*)GetSelectedItem();
		if (!pSelectedTab || !GetRibbonBar()->IsRibbonMinimized())
		{
			m_bPopup = FALSE;
			return FALSE;
		}

		if (m_pCommandBar)
			m_pCommandBar->InternalRelease();

		CXTPCommandBars* pCommandBars = GetRibbonBar()->GetCommandBars();
		ASSERT(pCommandBars);

		m_pCommandBar = GetRibbonBar()->CreateTabPopupToolBar(pSelectedTab);
		m_pCommandBar->SetCommandBars(pCommandBars);

		m_pCommandBar->EnableCustomization(FALSE);
		m_pCommandBar->Popup(this, IsKeyboardSelected(GetSelected()));
	}
	else if (m_pCommandBar)
	{
		ASSERT(m_pCommandBar);
		m_pCommandBar->SetTrackingMode(FALSE);
	}
	RedrawParent();
	return TRUE;
}

void CXTPRibbonControlTab::Copy(CXTPControl* pControl, BOOL /*bRecursive = FALSE*/)
{
	DeleteAllItems();

	CXTPRibbonControlTab* pControlTab = (CXTPRibbonControlTab*)pControl;

	for (int i = 0; i < pControlTab->GetItemCount(); i++)
	{
		CXTPRibbonTab* pTab = pControlTab->GetTab(i);

		CXTPRibbonTab* pTabClone = (CXTPRibbonTab*)pTab->GetRuntimeClass()->CreateObject();

		pTabClone->m_pRibbonBar = (CXTPRibbonBar*)GetParent();
		pTabClone->m_pParent = (CXTPRibbonBar*)GetParent();

		AddItem(i, pTabClone);

		pTabClone->Copy(pTab);
	}
}

void CXTPRibbonControlTab::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsStoring())
	{
		long nCount = (long)m_arrItems.GetSize();
		int i;

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Tab")));
		POSITION posEnum = pEnumerator->GetPosition(nCount, FALSE);

		for (i = 0; i < nCount; i++)
		{
			CXTPRibbonTab* pTab = (CXTPRibbonTab*)GetItem(i);

			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
			PX_Object(&secItem, pTab, RUNTIME_CLASS(CXTPRibbonTab));
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Tab")));
		POSITION posEnum = pEnumerator->GetPosition(0, FALSE);

		DeleteAllItems();

		while (posEnum)
		{
			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
			secItem->m_pOwner = this;

			CXTPRibbonTab* pTab = NULL;
			PX_Object(&secItem, pTab, RUNTIME_CLASS(CXTPRibbonTab));

			if (!pTab)
				AfxThrowArchiveException(CArchiveException::badClass);

			AddItem(GetItemCount(), pTab);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Accessible


HRESULT CXTPRibbonControlTab::GetAccessibleChildCount(long FAR* pChildCount)
{
	if (pChildCount == 0)
		return E_INVALIDARG;

	*pChildCount = GetItemCount();

	return S_OK;
}
HRESULT CXTPRibbonControlTab::GetAccessibleChild(VARIANT /*varChild*/, IDispatch* FAR* ppdispChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	*ppdispChild = NULL;
	return S_FALSE;
}

HRESULT CXTPRibbonControlTab::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	int nIndex = GetChildIndex(&varChild);

	if (nIndex == CHILDID_SELF)
		return CXTPControl::GetAccessibleName(varChild, pszName);

	CXTPTabManagerItem* pItem = GetItem(nIndex - 1);
	if (!pItem)
		return E_INVALIDARG;

	CString strCaption = pItem->GetCaption();
	CXTPDrawHelpers::StripMnemonics(strCaption);

	*pszName = strCaption.AllocSysString();
	return S_OK;
}

HRESULT CXTPRibbonControlTab::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	pvarRole->vt = VT_I4;
	pvarRole->lVal = GetChildIndex(&varChild) == CHILDID_SELF ? ROLE_SYSTEM_PAGETABLIST : ROLE_SYSTEM_PAGETAB;
	return S_OK;
}

HRESULT CXTPRibbonControlTab::AccessibleSelect(long flagsSelect, VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	int nIndex = GetChildIndex(&varChild);
	if (nIndex == CHILDID_SELF)
		return CXTPControl::AccessibleSelect(flagsSelect, varChild);

	SetCurSel(nIndex - 1);

	return S_OK;
}

HRESULT CXTPRibbonControlTab::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	int nIndex = GetChildIndex(&varChild);

	if (nIndex == CHILDID_SELF)
	{
		pvarState->vt = VT_I4;
		pvarState->lVal = STATE_SYSTEM_FOCUSABLE;

		if (!m_pParent->IsVisible() || !IsVisible())
			pvarState->lVal |= STATE_SYSTEM_INVISIBLE;
		return S_OK;
	}

	CXTPTabManagerItem* pItem = GetItem(nIndex - 1);
	if (!pItem)
		return E_INVALIDARG;

	pvarState->vt = VT_I4;
	pvarState->lVal = STATE_SYSTEM_FOCUSABLE | STATE_SYSTEM_SELECTABLE;

	if (pItem->IsSelected())
		pvarState->lVal |= STATE_SYSTEM_SELECTED | STATE_SYSTEM_FOCUSED;

	if (!pItem->IsVisible())
		pvarState->lVal |= STATE_SYSTEM_INVISIBLE;

	return S_OK;
}

HRESULT CXTPRibbonControlTab::GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
{
	if (GetChildIndex(&varChild) == CHILDID_SELF)
			return CXTPControl::GetAccessibleDefaultAction(varChild, pszDefaultAction);

	*pszDefaultAction = SysAllocString(L"Switch");

	return S_OK;
}

HRESULT CXTPRibbonControlTab::AccessibleDoDefaultAction(VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	int nIndex = GetChildIndex(&varChild);

	if (nIndex == CHILDID_SELF)
		return CXTPControl::AccessibleDoDefaultAction(varChild);

	CXTPTabManagerItem* pItem = GetItem(nIndex - 1);
	if (!pItem)
		return E_INVALIDARG;

	pItem->Select();
	return S_OK;
}


HRESULT CXTPRibbonControlTab::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	int nIndex = GetChildIndex(&varChild);

	if (!m_pParent->GetSafeHwnd())
		return S_OK;

	if (!IsVisible())
		return S_OK;

	CRect rcControl = GetRect();

	if (nIndex != CHILDID_SELF)
	{
		rcControl = GetItem(nIndex - 1)->GetRect();
	}

	m_pParent->ClientToScreen(&rcControl);

	*pxLeft = rcControl.left;
	*pyTop = rcControl.top;
	*pcxWidth = rcControl.Width();
	*pcyHeight = rcControl.Height();

	return S_OK;
}

HRESULT CXTPRibbonControlTab::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (pvarID == NULL)
		return E_INVALIDARG;

	pvarID->vt = VT_EMPTY;

	if (!m_pParent->GetSafeHwnd())
		return S_FALSE;

	if (!CXTPWindowRect(m_pParent).PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	pvarID->vt = VT_I4;
	pvarID->lVal = CHILDID_SELF;

	CPoint pt(xLeft, yTop);
	m_pParent->ScreenToClient(&pt);

	if (!GetRect().PtInRect(pt))
		return S_FALSE;

	CXTPTabManagerItem* pItem = HitTest(pt);
	if (pItem)
	{
		pvarID->lVal = pItem->GetIndex() + 1;
	}

	return S_OK;
}
