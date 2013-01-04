// XTPRibbonBar.cpp : implementation file
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
#include "Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPWinThemeWrapper.h"

#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManager.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlExt.h"
#include "CommandBars/XTPCommandBarsFrameHook.h"
#include "CommandBars/XTPCustomizeTools.h"
#include "CommandBars/XTPMouseManager.h"

#include "CommandBars/XTPPaintManager.h"

#include "XTPRibbonPopups.h"
#include "XTPRibbonTab.h"
#include "XTPRibbonPaintManager.h"
#include "XTPRibbonQuickAccessControls.h"
#include "XTPRibbonGroups.h"
#include "XTPRibbonGroup.h"
#include "XTPRibbonControlTab.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonSystemButton.h"
#include "XTPRibbonBackstageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonBar, CXTPMenuBar)

class CXTPRibbonBar::CControlQuickAccessCommand : public CXTPControlButton
{
public:
	CControlQuickAccessCommand(CXTPControls* pQuickAccessControls, CXTPControl* pControlOriginal)
	{
		m_pControlOriginal = pControlOriginal;
		m_pQuickAccessControls = pQuickAccessControls;
		m_pQuickAccessControl = 0;
	}

	void SetQuickAccessControl(CXTPControl* pQuickAccessControl)
	{
		m_pQuickAccessControl = pQuickAccessControl;
		SetChecked(TRUE);
	}

	void OnExecute()
	{
		CXTPControlButton::OnExecute();

		if (m_pQuickAccessControl)
		{
			m_pQuickAccessControls->Remove(m_pQuickAccessControl);
		}
		else
		{
			CXTPControl* pControl = m_pQuickAccessControls->AddClone(m_pControlOriginal, -1, TRUE);
			pControl->SetHideFlag(xtpHideCustomize, FALSE);
		}

		m_pQuickAccessControls->GetParent()->OnRecalcLayout();
	}

protected:
	CXTPControl* m_pControlOriginal;
	CXTPControl* m_pQuickAccessControl;
	CXTPControls* m_pQuickAccessControls;
};


class CXTPRibbonBarControlCaptionButton : public CXTPControlButton
{
	DECLARE_DYNAMIC(CXTPRibbonBarControlCaptionButton);
public:
	CXTPRibbonBarControlCaptionButton(CXTPCommandBarsFrameHook* pFrame)
	{
		m_pFrame = pFrame;
		m_bTemporary = TRUE;
		SetFlags(xtpFlagNoMovable | xtpFlagSkipFocus | xtpFlagManualUpdate);
	}
protected:
	void Draw(CDC* pDC)
	{
		((CXTPRibbonBar*)GetParent())->GetPaintManager()->GetFramePaintManager()->
			DrawFrameCaptionButton(pDC, GetRect(), GetID(), GetSelected(), GetPressed(), m_bEnabled && m_pFrame->IsFrameActive());
	}

	void OnExecute()
	{
		GetParent()->GetSite()->PostMessage(WM_SYSCOMMAND, GetID(), 0);
	}

	virtual BOOL IsCustomizeMovable() const
	{
		return FALSE;
	}

protected:
	CXTPCommandBarsFrameHook* m_pFrame;
};

IMPLEMENT_DYNAMIC(CXTPRibbonBarControlCaptionButton, CXTPControlButton);


class CXTPRibbonBarControlQuickAccessPopup: public CXTPControlPopup
{
	DECLARE_XTP_CONTROL(CXTPRibbonBarControlQuickAccessPopup)

public:
	CXTPRibbonBarControlQuickAccessPopup()
	{
		m_bTemporary = TRUE;
		SetFlags(xtpFlagNoMovable | xtpFlagManualUpdate);
	}
	CSize GetSize(CDC* /*pDC*/)
	{
		CXTPRibbonPaintManager* pPaintManager = m_pParent->GetPaintManager()->GetRibbonPaintManager();
		return CSize(pPaintManager->GetEditHeight() * 13 / 22, pPaintManager->GetEditHeight());
	}
	void Draw(CDC* pDC)
	{
		GetParent()->GetPaintManager()->GetRibbonPaintManager()->DrawRibbonQuickAccessButton(pDC, this);
	}

	BOOL OnSetPopup(BOOL bPopup)
	{
		if (bPopup)
		{
			if (m_pCommandBar)
				m_pCommandBar->InternalRelease();

			CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetParent()->GetRootParent();

			m_pCommandBar = pRibbonBar->CreateContextMenu(this);
		}

		return CXTPControlPopup::OnSetPopup(bPopup);
	}

	virtual BOOL IsCustomizeMovable() const
	{
		return FALSE;
	}
};

IMPLEMENT_XTP_CONTROL(CXTPRibbonBarControlQuickAccessPopup, CXTPControlPopup)


class CXTPRibbonBar::CControlQuickAccessMorePopup: public CXTPControlPopup
{
public:
	CControlQuickAccessMorePopup()
	{
		m_bTemporary = TRUE;
		SetFlags(xtpFlagNoMovable | xtpFlagManualUpdate);
	}
	CSize GetSize(CDC* /*pDC*/)
	{
		CXTPRibbonPaintManager* pPaintManager = m_pParent->GetPaintManager()->GetRibbonPaintManager();
		return CSize(pPaintManager->GetEditHeight() * 13 / 22, pPaintManager->GetEditHeight());
	}
	void Draw(CDC* pDC)
	{
		m_pParent->GetPaintManager()->GetRibbonPaintManager()->DrawRibbonQuickAccessMoreButton(pDC, this);
	}

	BOOL OnSetPopup(BOOL bPopup)
	{
		if (bPopup)
		{
			if (m_pCommandBar)
				m_pCommandBar->InternalRelease();

			m_pCommandBar = ((CXTPRibbonBar*)GetParent())->CreateMoreQuickAccessContextMenu();
		}

		return CXTPControlPopup::OnSetPopup(bPopup);
	}

};


/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBar

CXTPRibbonBar::CXTPRibbonBar()
{
	m_pQuickAccessControls = new CXTPRibbonQuickAccessControls;
	m_pQuickAccessControls->SetParent(this);

	m_pControls->InternalRelease();
	m_pControls = new CXTPRibbonControls;
	m_pControls->SetParent(this);

	m_bShowCaptionAlways = TRUE;

	m_pHighlightedGroup = NULL;

	m_dwStyle = 0;
	m_rcTabControl.SetRectEmpty();
	m_rcGroups.SetRectEmpty();
	m_rcCaption.SetRectEmpty();
	m_rcCaptionText.SetRectEmpty();
	m_rcHeader.SetRectEmpty();

	m_bMinimizeOnDblClick = FALSE;

	InitScrollableBar(this);

	m_pControlQuickAccess = m_pControls->Add(new CXTPRibbonBarControlQuickAccessPopup, XTP_ID_RIBBONCONTROLQUICKACCESS, NULL, -1, TRUE);
	m_pControlQuickAccessMore = m_pControls->Add(new CControlQuickAccessMorePopup, XTP_ID_RIBBONCONTROLQUICKACCESSMORE, NULL, -1, TRUE);

	m_pControlTab = (CXTPRibbonControlTab*)m_pControls->Add(new CXTPRibbonControlTab, XTP_ID_RIBBONCONTROLTAB, NULL, -1, TRUE);

	m_dwFlags = xtpFlagIgnoreSetMenuMessage | xtpFlagStretched;

	m_bCustomizable = FALSE;
	m_bGroupsVisible = TRUE;
	m_bTabsVisible = TRUE;

	m_nMinVisibleWidth = 250;
	m_nMinVisibleHeight = -1;

	m_bMinimized = FALSE;

	m_pControlSystemButton = NULL;

	m_bRibbonBarVisible = TRUE;
	m_bGroupReducedChanged = FALSE;

	m_bShowQuickAccessBelow = FALSE;
	m_bShowQuickAccess = TRUE;

	m_pContextHeaders = new CXTPRibbonTabContextHeaders;

	m_bMultiLine = TRUE;

	m_bAllowQuickAccessDuplicates = FALSE;
	m_bAllowQuickAccessCustomization = TRUE;

	m_nGroupsHeight = 0;
	m_nClientHeight = 0;
	m_nQuickAccessHeight = 0;

	m_nGroupsScrollPos = 0;

	m_bAllowMinimize = TRUE;

	m_nQuickAccessEmptyIconId = -1;

	EnableDoubleBuffer();
	EnableAnimation();
}

CXTPRibbonPaintManager* CXTPRibbonBar::GetRibbonPaintManager() const
{
	CXTPPaintManager* pPaintManager = GetPaintManager();

	return pPaintManager->GetRibbonPaintManager();
}


CXTPRibbonBar::~CXTPRibbonBar()
{
	if (m_pControlTab)
	{
		m_pControlTab->DeleteAllItems();
	}

	if (m_pQuickAccessControls)
	{
		m_pQuickAccessControls->RemoveAll();
	}

	CMDTARGET_RELEASE(m_pQuickAccessControls);
	SAFE_DELETE(m_pContextHeaders);
}

void CXTPRibbonBar::OnRemoved()
{
	GetCommandBars()->EnableFrameTheme(FALSE);

	if (m_pQuickAccessControls)
	{
		m_pQuickAccessControls->RemoveAll();
	}
	if (m_pControlTab)
	{
		m_pControlTab->DeleteAllItems();
	}

	CXTPMenuBar::OnRemoved();
}


CXTPControlPopup* CXTPRibbonBar::AddSystemButton(int nID)
{
	if (m_pControlSystemButton)
		return m_pControlSystemButton;

	m_pControlSystemButton = (CXTPRibbonControlSystemButton*)GetControls()->InsertAt(new CXTPRibbonControlSystemButton, 0);

	if (nID > 0) m_pControlSystemButton->SetID(nID);

	CXTPPopupBar* pCommandBar = new CXTPRibbonSystemPopupBar();
	pCommandBar->SetCommandBars(GetCommandBars());

	m_pControlSystemButton->SetCommandBar(pCommandBar);
	pCommandBar->InternalRelease();

	return m_pControlSystemButton;
}

CXTPCommandBarsFrameHook* CXTPRibbonBar::GetFrameHook() const
{
	return GetCommandBars()->GetFrameHook();
}


void CXTPRibbonBar::EnableFrameTheme(BOOL bEnable)
{
	if (!bEnable)
	{
		m_pControls->Remove(m_pControls->FindControl(SC_CLOSE));
		m_pControls->Remove(m_pControls->FindControl(SC_RESTORE));
		m_pControls->Remove(m_pControls->FindControl(SC_MAXIMIZE));
		m_pControls->Remove(m_pControls->FindControl(SC_MINIMIZE));

		GetCommandBars()->EnableFrameTheme(FALSE);

		OnRecalcLayout();
	}
	else
	{
		GetCommandBars()->EnableFrameTheme(TRUE);

		ASSERT(GetFrameHook()->m_pRibbonBar == this);
	}
}

BOOL CXTPRibbonBar::IsFrameThemeEnabled() const
{
	return GetCommandBars()->GetFrameHook() != NULL;
}

CXTPRibbonTab* CXTPRibbonBar::GetSelectedTab() const
{
	if (m_pControlTab == NULL)
		return NULL;

	return (CXTPRibbonTab*)m_pControlTab->GetSelectedItem();
}

BOOL CXTPRibbonBar::OnTabChanging(CXTPRibbonTab* pTab)
{
	NMXTPTABCHANGE nm;
	nm.pTab = pTab;

	return (BOOL)m_pControlTab->NotifySite(TCN_SELCHANGING, &nm);
}

void CXTPRibbonBar::OnTabChanged(CXTPRibbonTab* pTab)
{
	NMXTPTABCHANGE nm;
	nm.pTab = pTab;

	m_pControlTab->NotifySite(TCN_SELCHANGE, &nm);
}

CXTPRibbonTabPopupToolBar* CXTPRibbonBar::CreateTabPopupToolBar(CXTPRibbonTab* pTab)
{
	return new CXTPRibbonTabPopupToolBar(pTab);
}

int CXTPRibbonBar::GetCurSel() const
{
	return m_pControlTab->GetCurSel();
}

void CXTPRibbonBar::SetCurSel(int nIndex)
{
	m_pControlTab->SetCurSel(nIndex);

}

CXTPRibbonTab* CXTPRibbonBar::InsertTab(int nItem, int nID)
{
	CString strPrompt;
	VERIFY(strPrompt.LoadString(nID));

	CXTPRibbonTab* pRibbonTab = InsertTab(nItem, strPrompt, nID);
	if (!pRibbonTab)
		return pRibbonTab;

	if (strPrompt.Find('\n') != -1)
	{
		CString strCaption, strKeyboardTip, strToolTip;

		AfxExtractSubString(strToolTip, strPrompt, 1);
		AfxExtractSubString(strCaption, strPrompt, 2);
		AfxExtractSubString(strKeyboardTip, strPrompt, 3);

		pRibbonTab->SetCaption(strCaption.IsEmpty() ? strToolTip : strCaption);
		pRibbonTab->SetTooltip(strToolTip);
		pRibbonTab->SetKeyboardTip(strKeyboardTip);
	}

	return pRibbonTab;
}

CXTPRibbonTab* CXTPRibbonBar::InsertTab(int nItem, CXTPRibbonTab* pTab)
{
	ASSERT(pTab);
	if (!pTab)
		return NULL;

	ASSERT(pTab->GetRibbonBar() == NULL);

	m_pControlTab->AddItem(nItem, pTab);

	pTab->m_pRibbonBar = this;
	pTab->m_pParent = this;


	if (nItem == 0 && GetCurSel() == -1)
	{
		SetCurSel(0);
	}

	m_bTrackOnHover = FALSE;
	m_barType = xtpBarTypeRibbon;

	return pTab;
}

CXTPRibbonTab* CXTPRibbonBar::InsertTab(int nItem, LPCTSTR lpszCaption, int nID)
{
	CXTPRibbonTab* pItem = InsertTab(nItem, new CXTPRibbonTab());

	pItem->SetCaption(lpszCaption);
	pItem->SetID(nID);

	return pItem;
}

CXTPRibbonTab* CXTPRibbonBar::AddTab(int nID)
{
	return InsertTab(m_pControlTab->GetItemCount(), nID);
}

CXTPRibbonTab* CXTPRibbonBar::AddTab(LPCTSTR lpszCaption)
{
	return InsertTab(m_pControlTab->GetItemCount(), lpszCaption, 0);
}

void CXTPRibbonBar::RebuildControls(CXTPRibbonTab* pSelected)
{
	m_nGroupsScrollPos = 0;

	if (IsRibbonMinimized())
		pSelected = NULL;

	for (int i = GetControlCount() - 1; i >= 0; i--)
	{
		CXTPControl* pControl = m_pControls->GetAt(i);
		CXTPRibbonGroup* pGroup = pControl->GetRibbonGroup();

		if ((pGroup != NULL) && (pGroup->GetParentTab() != pSelected || !pGroup->m_bVisible))
		{
			pControl->SetHideFlag(xtpHideRibbonTab, TRUE);
		}
		else
		{
			pControl->SetHideFlag(xtpHideRibbonTab, FALSE);
		}
	}
}


BEGIN_MESSAGE_MAP(CXTPRibbonBar, CXTPMenuBar)
	//{{AFX_MSG_MAP(CXTPRibbonBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND_RANGE(XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSBELOW, XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSABOVE, OnCustomizePlaceQuickAccess)
	ON_MESSAGE(WM_XTP_COMMAND, OnCustomizeCommand)
	ON_WM_NCHITTEST_EX()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBar message handlers

int CXTPRibbonBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPMenuBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetupHook();

	return 0;
}

void CXTPRibbonBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXTPMenuBar::OnShowWindow(bShow, nStatus);

	if (IsFrameThemeEnabled() && bShow != m_bRibbonBarVisible)
	{
		m_bRibbonBarVisible = bShow;
		GetFrameHook()->RecalcFrameLayout();
	}
}

void CXTPRibbonBar::SetVisible(BOOL bVisible)
{
	BOOL bOldVisible = IsVisible();

	CXTPMenuBar::SetVisible(bVisible);

	if (IsFrameThemeEnabled() && bOldVisible != IsVisible())
	{
		GetFrameHook()->RecalcFrameLayout();
	}
}

int CXTPRibbonBar::GetQuickAccessHeight() const
{
	return m_nQuickAccessHeight;

}

int CXTPRibbonBar::CalcQuickAccessHeight()
{
	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));

	int nQuickAccessHeight = GetButtonSize().cy + 4;

	for (int i = 0; i < m_pQuickAccessControls->GetCount(); i++)
	{
		CXTPControl* pControl = m_pQuickAccessControls->GetAt(i);
		if (!pControl->IsVisible())
			continue;

		int nHeight = pControl->GetSize(&dc).cy + 4;
		if (nHeight > nQuickAccessHeight)
			nQuickAccessHeight = nHeight;
	}
	return nQuickAccessHeight;
}

CSize CXTPRibbonBar::CalcDockingLayout(int nLength, DWORD dwMode, int /*nWidth*/)
{
	BOOL bRibbonBarVisible = nLength >= m_nMinVisibleWidth;
	m_bGroupReducedChanged = FALSE;

	CXTPWindowRect rc(GetSite());

	int nMinVisibleHeight = m_nMinVisibleHeight == -1 ? m_nMinVisibleWidth : m_nMinVisibleHeight;

	if (rc.Height() < nMinVisibleHeight)
		bRibbonBarVisible = FALSE;

	if (bRibbonBarVisible != m_bRibbonBarVisible)
	{
		m_bRibbonBarVisible =  bRibbonBarVisible;

		if (IsFrameThemeEnabled())
		{
			GetFrameHook()->DelayRecalcFrameLayout();
		}
	}

	if (!bRibbonBarVisible)
	{
		return CSize(nLength, 0);
	}

	int nCaptionHeight = GetCaptionHeight() + GetRibbonTopBorder();
	int nHeight = nCaptionHeight + GetTabsHeight() + GetGroupsHeight();

	m_nQuickAccessHeight = CalcQuickAccessHeight();

	if (IsQuickAccessBelowRibbon())
	{
		nHeight += m_nQuickAccessHeight + 1;
	}


	if (!IsGroupsVisible() && !IsQuickAccessBelowRibbon())
	{
		nHeight += 2;
	}

	if (dwMode & LM_COMMIT)
	{
		Reposition(nLength, nHeight);
	}



	if (m_bGroupReducedChanged)
	{
		OnUpdateCmdUI();
		m_bGroupReducedChanged = FALSE;
	}

	if ((m_nIdleFlags & xtpIdleLayout) && (dwMode & LM_COMMIT))
	{
		m_nIdleFlags &= ~xtpIdleLayout;
		Redraw();
	}

	return CSize(nLength, nHeight);
}

int CXTPRibbonBar::GetTabsHeight() const
{
	return m_bTabsVisible ? GetRibbonPaintManager()->m_nTabsHeight : 2;
}

int CXTPRibbonBar::CalcClientHeight()
{
	if (m_nClientHeight > 0)
		return m_nClientHeight;

	return GetRibbonPaintManager()->GetEditHeight() * 3;
}

int CXTPRibbonBar::CalcGroupsHeight()
{
	if (m_nGroupsHeight > 0)
		return m_nGroupsHeight;

	int nHeight = CalcClientHeight();
	int nCaptionHeight = GetRibbonPaintManager()->GetGroupCaptionHeight();

	return nHeight + nCaptionHeight + 10;
}

int CXTPRibbonBar::GetGroupsHeight()
{
	return IsGroupsVisible() ? CalcGroupsHeight() : 0;
}

BOOL CXTPRibbonBar::IsGroupsVisible() const
{
	return m_bGroupsVisible && GetSelectedTab() != NULL && !m_bMinimized;
}

void CXTPRibbonBar::SetGroupsVisible(BOOL bVisible)
{
	if (bVisible != m_bGroupsVisible)
	{
		m_bGroupsVisible = bVisible;
		OnRecalcLayout();
	}
}

BOOL CXTPRibbonBar::IsTabsVisible() const
{
	return m_bTabsVisible;
}

void CXTPRibbonBar::SetTabsVisible(BOOL bVisible)
{
	if (bVisible != m_bTabsVisible)
	{
		m_bTabsVisible = bVisible;
		OnRecalcLayout();
	}
}

BOOL CXTPRibbonBar::IsQuickAccessControl(CXTPControl* pControl) const
{
	if (!pControl)
		return FALSE;

	if (pControl == m_pControlQuickAccess)
		return TRUE;

	if (pControl == m_pControlQuickAccessMore)
		return TRUE;

	if (pControl->GetRibbonGroup())
		return FALSE;

	if (m_pQuickAccessControls == NULL)
		return FALSE;

	for (int i = 0; i < m_pQuickAccessControls->GetCount(); i++)
	{
		if (m_pQuickAccessControls->GetAt(i) == pControl)
			return TRUE;
	}
	return FALSE;
}

int CXTPRibbonBar::GetCaptionHeight() const
{
	if (!IsFrameThemeEnabled() && !m_bShowCaptionAlways)
		return 0;

	int nHeight = GetRibbonPaintManager()->GetRibbonCaptionHeight();

	if (!IsQuickAccessVisible() || IsQuickAccessBelowRibbon())
		return nHeight;

	int nQuickAccessHeight = GetQuickAccessHeight();
	nQuickAccessHeight = max(nQuickAccessHeight, GetTabsHeight() - 2);

	if (nHeight >= nQuickAccessHeight)
		return nHeight;

	return nQuickAccessHeight;
}

void CXTPRibbonBar::Reposition(int cx, int cy)
{
	CClientDC dc(this);
	int nTabsHeight = GetTabsHeight();
	int nGroupsHeight = GetGroupsHeight();
	BOOL bFrameThemeEnabled = IsFrameThemeEnabled();
	BOOL bShowCaption = bFrameThemeEnabled || m_bShowCaptionAlways;

	int nQuickAccessHeight = m_nQuickAccessHeight;

	int i;

	CRect rcTabControl(2, 0, cx - 2, nTabsHeight);
	CRect rcRibbonGroups(0, nTabsHeight, cx, nTabsHeight + nGroupsHeight);

	int nFrameBorder = 4;

	if (bShowCaption)
	{
		CXTPCommandBarsFrameHook* pFrameHook = GetFrameHook();

		nFrameBorder = pFrameHook ? pFrameHook->GetFrameBorder() : 0;
		int nRibbonTopBorder = GetRibbonTopBorder();
		int nCaptionOffset = nRibbonTopBorder - nFrameBorder;

		m_rcCaption.SetRect(-nFrameBorder, nCaptionOffset, cx + nFrameBorder, GetCaptionHeight() + nRibbonTopBorder);
		rcTabControl.OffsetRect(0, m_rcCaption.Height() + nCaptionOffset);
		rcRibbonGroups.OffsetRect(0, m_rcCaption.Height() + nCaptionOffset);

		m_rcCaptionText.SetRect(0, 0, cx, m_rcCaption.bottom);
		m_rcHeader.SetRect(0, 0, cx, m_rcCaption.bottom);

		if (!pFrameHook) m_rcCaptionText.SetRectEmpty();
	}
	else
	{
		m_rcCaption.SetRectEmpty();
		m_rcCaptionText.SetRectEmpty();
		m_rcHeader.SetRectEmpty();
	}

	CArray<CXTPControl*, CXTPControl*> arrFreeControls;
	CArray<CXTPControl*, CXTPControl*> arrQuickAccessControls;

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);

		if (IsQuickAccessControl(pControl))
		{
			arrQuickAccessControls.Add(pControl);
		}
		else
		{
			if (!pControl->IsVisible())
				continue;

			if (pControl->GetRibbonGroup() != NULL)
				continue;

			if (pControl->GetID() >= SC_MINIMIZE && pControl->GetID() <= SC_RESTORE)
				continue;

			if (pControl == m_pControlSystemButton || pControl == m_pControlScrollGroupsLeft ||
				pControl == m_pControlScrollGroupsRight)
				continue;

			if (pControl == m_pControlTab)
				continue;

			arrFreeControls.Add(pControl);
		}
	}

	int nCorner = nQuickAccessHeight + nTabsHeight - 3;

	if (!m_pControlSystemButton || m_pControlSystemButton->IsSimpleButton())
	{
		nCorner = 10 + GetSystemMetrics(SM_CXSMICON);
	}

	if (m_bShowQuickAccess && m_bShowQuickAccessBelow)
	{
		m_rcQuickAccess.SetRect(0, rcRibbonGroups.bottom, cx, rcRibbonGroups.bottom + nQuickAccessHeight);
	}
	else
	{
		m_rcQuickAccess.SetRect(3 + nCorner, m_rcCaption.top + nFrameBorder - 2, m_rcCaption.right, m_rcCaption.top + nFrameBorder - 2 + nQuickAccessHeight - 1);
	}

	int nLeft = 2;
	int nRight = cx - 2;

	if (m_pControlSystemButton)
	{
		CSize szControl(nCorner, nCorner - 1);
		int nTop = m_rcCaption.top + nFrameBorder - 1;
		nLeft = 0;
		int nGap = 0;

		if (m_pControlSystemButton->IsSimpleButton())
		{
			szControl = m_pControlSystemButton->GetSize(&dc);
			szControl.cy = nTabsHeight;

			nTop = rcTabControl.top + (nTabsHeight - szControl.cy) / 2;
			nGap = 3;

			CRect rcMargin = GetRibbonPaintManager()->m_rcFileButtonMargin;

			nTop += rcMargin.top;
			szControl.cy -= rcMargin.top;

			nLeft += rcMargin.left;
			nGap += rcMargin.right;

		}
		else if (!bShowCaption)
		{
			szControl = m_pControlSystemButton->GetSize(&dc);
			nTop = rcTabControl.top + (nTabsHeight - szControl.cy) / 2;
		}
		else if (!m_bTabsVisible)
		{
			szControl = m_pControlSystemButton->GetSize(&dc);
			nTop = m_rcCaption.top + (m_rcCaption.Height() - szControl.cy) / 2;
			if (!m_bShowQuickAccessBelow) m_rcQuickAccess.left = 3 + szControl.cx + 12;
		}

		CRect rcControl(nLeft, nTop, nLeft + szControl.cx, nTop + szControl.cy);
		nLeft += szControl.cx + nGap;

		m_pControlSystemButton->SetHideWrap(!bShowCaption && !m_bTabsVisible);
		m_pControlSystemButton->SetRect(rcControl);
		m_pControlSystemButton->SetWrap(TRUE);
	}

	for (i = 0; i < (int)arrFreeControls.GetSize(); i++)
	{
		CXTPControl* pControl = arrFreeControls.GetAt(i);

		if (pControl->GetFlags() & xtpFlagRightAlign)
			continue;

		pControl->SetHideFlag(xtpHideDockingPosition, !m_bTabsVisible);
		if (!pControl->IsVisible())
			continue;

		CSize szControl = pControl->GetSize(&dc);
		int nTop = rcTabControl.top + (nTabsHeight - szControl.cy) / 2;

		if (pControl->GetBeginGroup())
			nLeft += 6;

		CRect rcControl(nLeft, nTop, nLeft + szControl.cx, nTop + szControl.cy);
		nLeft += szControl.cx;

		pControl->SetHideWrap(FALSE);
		pControl->SetRect(rcControl);
		pControl->SetRowRect(CRect(2, rcTabControl.top, cy - 2, rcTabControl.bottom));
		pControl->SetWrap(FALSE);
	}

	int nLeftTab = nLeft;
	BOOL bQuickAccessNearTabs = m_bShowQuickAccess && !m_bShowQuickAccessBelow && !bShowCaption;

	if (bQuickAccessNearTabs)
	{
		nLeft += 5;
		m_rcQuickAccess.left = nLeft;
		m_rcQuickAccess.top = rcTabControl.top;
		m_rcQuickAccess.bottom = rcTabControl.bottom;
	}
	else
	{
		nLeft = m_rcQuickAccess.left + (m_bShowQuickAccessBelow ? 2 : 0);
	}

	nRight = m_bShowQuickAccess && m_bShowQuickAccessBelow ? cx - 2 - 13 :
		IsDwmEnabled() ? cx - 115 : cx - m_rcCaption.Height() * 3;

	if (m_pControlQuickAccessMore) m_pControlQuickAccessMore->SetHideFlag(xtpHideGeneric, TRUE);
	BOOL bQuickAccessMoreVisible = FALSE;
	BOOL bQuickAccessFound = FALSE;

	BOOL bQuickAccessNearControls = (m_pControlSystemButton && m_pControlSystemButton->IsSimpleButton());

	for (i = 0; i < (int)arrQuickAccessControls.GetSize(); i++)
	{
		CXTPControl* pControl = arrQuickAccessControls.GetAt(i);

		if (!m_bShowQuickAccess)
			pControl->SetHideFlag(xtpHideWrap, TRUE);
		else if (pControl != m_pControlQuickAccessMore)
			pControl->SetHideFlag(xtpHideWrap, bQuickAccessMoreVisible);
		if (!bShowCaption && !m_bTabsVisible)
			pControl->SetHideFlag(xtpHideWrap, TRUE);

		if (pControl == m_pControlQuickAccess && bQuickAccessFound &&
			!bQuickAccessNearTabs && !m_bShowQuickAccessBelow)
		{
			if (!bQuickAccessNearControls)
				nLeft += 12;
		}

		if (!pControl->IsVisible())
			continue;

		BOOL bSpecial = pControl == m_pControlQuickAccessMore || pControl == m_pControlQuickAccess;

		CSize szControl = pControl->GetSize(&dc);
		int nTop = m_rcQuickAccess.top + (m_rcQuickAccess.Height() - szControl.cy + 1) / 2;

		if (!m_bShowQuickAccessBelow && !bQuickAccessNearTabs)
		{
			if (bQuickAccessNearControls)
				nTop += 1;
			else if (!bSpecial)
				nTop += 2;
		}
		if (!bSpecial)
		{
			bQuickAccessFound = TRUE;
		}

		if (pControl->GetBeginGroup())
			nLeft += 6;

		if (!bSpecial && (nLeft + szControl.cx > nRight))
		{
			bQuickAccessMoreVisible = TRUE;
			pControl->SetHideFlag(xtpHideWrap, bQuickAccessMoreVisible);
			m_pControlQuickAccessMore->SetHideFlag(xtpHideGeneric, FALSE);
			continue;
		}

		CRect rcControl(nLeft, nTop, nLeft + szControl.cx, nTop + szControl.cy);

		if (bQuickAccessNearTabs || !bSpecial)
			nLeft += szControl.cx;

		pControl->SetHideWrap(FALSE);
		pControl->SetRect(rcControl);
		pControl->SetRowRect(m_rcQuickAccess);
		pControl->SetWrap(FALSE);
	}
	if (!m_bShowQuickAccessBelow) m_rcQuickAccess.right = nLeft;

	if (!bQuickAccessNearTabs) nLeft = nLeftTab;
	nRight = cx;

	for (i = (int)arrFreeControls.GetSize() - 1; i >= 0; i--)
	{
		CXTPControl* pControl = arrFreeControls.GetAt(i);

		if ((pControl->GetFlags() & xtpFlagRightAlign) == 0)
			continue;

		pControl->SetHideFlag(xtpHideDockingPosition, !m_bTabsVisible);
		if (!pControl->IsVisible())
			continue;

		CSize szControl = pControl->GetSize(&dc);
		int nTop = rcTabControl.top + (nTabsHeight - szControl.cy) / 2;

		CRect rcControl = CRect(nRight - szControl.cx, nTop, nRight, nTop + szControl.cy);
		nRight -= szControl.cx;

		pControl->SetHideWrap(FALSE);
		pControl->SetRect(rcControl);
		pControl->SetRowRect(CRect(2, rcTabControl.top, cy - 2, rcTabControl.bottom));
		pControl->SetWrap(TRUE);
		pControl->SetBeginGroup(FALSE);
	}

	if (!m_bTabsVisible) rcTabControl.bottom = rcTabControl.top;


	if (nLeft + 6 > nRight || !m_bTabsVisible)
		m_rcTabControl = CRect(0, rcTabControl.top, 0, rcTabControl.bottom + 1);
	else
		m_rcTabControl = CRect(nLeft, rcTabControl.top, nRight, rcTabControl.bottom + 1);

	if (m_pControlTab)
	{
		m_pControlTab->SetHideFlag(xtpHideGeneric, !m_bTabsVisible);

		m_pControlTab->SetRect(m_rcTabControl);
		GetTabPaintManager()->RepositionTabControl(m_pControlTab, &dc, m_rcTabControl);
	}

	RepositionGroups(&dc, rcRibbonGroups);

	RepositionCaptionButtons();

	if (bShowCaption)
	{
		RepositionContextHeaders();
	}
	else
	{
		m_pContextHeaders->RemoveAll();
	}
}



void CXTPRibbonBar::AddCaptionButton(int nId, BOOL bAdd, BOOL bEnabled, CRect& rcCaption)
{
	CXTPControl* pButton = m_pControls->FindControl(nId);

	if (!pButton && bAdd)
	{
		pButton = m_pControls->Add(new CXTPRibbonBarControlCaptionButton(GetFrameHook()), nId, NULL, -1, TRUE);

		CString strCaption;
		CMenu* pMenu = GetSite()->GetSystemMenu(FALSE);
		if (pMenu)
		{
			pMenu->GetMenuString(nId, strCaption, MF_BYCOMMAND);
			int nIndex = strCaption.Find(_T('\t'));
			if (nIndex > 0)
			{
				strCaption = strCaption.Left(nIndex);
			}
		}
		if (pButton->GetAction())
		{
			pButton->GetAction()->SetCaption(_T(""));
			pButton->GetAction()->SetDescription(NULL);
		}

		pButton->SetDescription(NULL);
		pButton->SetCaption(_T(""));
		pButton->SetTooltip(strCaption);
	}
	else if (pButton && !bAdd)
	{
		m_pControls->Remove(pButton);
	}

	if (bAdd)
	{
		CRect rcButton(rcCaption.right - rcCaption.Height(), rcCaption.top, rcCaption.right, rcCaption.bottom);
		pButton->SetRect(rcButton);
		rcCaption.right -= rcButton.Width();
		pButton->SetEnabled(bEnabled);
	}
}

BOOL CXTPRibbonBar::ShrinkContextHeaders(int nLeft, int nRight)
{
	int i, nCount = m_pContextHeaders->GetCount();

	for (i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader =  m_pContextHeaders->GetHeader(i);
		if (pHeader->m_rcRect.left < nLeft)
		{
			pHeader->m_rcRect.left = nLeft;
			if (pHeader->m_rcRect.Width()  < 40)
			{
				pHeader->m_rcRect.right = nLeft + 40;
			}
			nLeft = pHeader->m_rcRect.right;
		}
		else break;
	}
	for (i = nCount - 1; i >= 0; i--)
	{
		CXTPRibbonTabContextHeader* pHeader =  m_pContextHeaders->GetHeader(i);
		if (pHeader->m_rcRect.right > nRight)
		{
			pHeader->m_rcRect.right = nRight;
			if (pHeader->m_rcRect.Width()  < 40)
			{
				pHeader->m_rcRect.left = nRight - 40;
			}
			nRight = pHeader->m_rcRect.left;

			if (nRight < nLeft)
				return FALSE;
		}
		else break;
	}

	return TRUE;
}

BOOL CXTPRibbonBar::IsCaptionVisible() const
{
	return IsFrameThemeEnabled() || m_bShowCaptionAlways;
}

void CXTPRibbonBar::RepositionContextHeaders()
{
	m_pContextHeaders->RemoveAll();

	CXTPRibbonTabContextHeader* pPreviousHeader = NULL;

	if (!IsCaptionVisible())
		return;

	int nCount = GetTabCount();
	if (nCount == 0)
		return;

	if (!IsQuickAccessBelowRibbon() && IsQuickAccessVisible() && !GetFrameHook())
		m_rcHeader.left = m_rcQuickAccess.right + (m_pControlQuickAccess ? m_pControlQuickAccess->GetRect().Width() + 2 : 12);

	int i;
	for (i = 0; i < nCount; i++)
	{
		CXTPRibbonTab* pTab = GetTab(i);
		pTab->m_pContextHeader = NULL;
		if (!pTab->IsVisible())
			continue;

		if (pTab->GetContextColor() == xtpRibbonTabContextColorNone || pTab->GetContextCaption().IsEmpty())
		{
			pPreviousHeader = NULL;
			continue;
		}
		if (pPreviousHeader && pPreviousHeader->m_color == pTab->GetContextColor() &&
			pPreviousHeader->m_strCaption == pTab->GetContextCaption())
		{
			pPreviousHeader->m_rcRect.right = pTab->GetRect().right;
			pPreviousHeader->m_pLastTab = pTab;
			pTab->m_pContextHeader = pPreviousHeader;
		}
		else
		{
			CXTPRibbonTabContextHeader* pHeader = new CXTPRibbonTabContextHeader(pTab);

			pHeader->m_rcRect.SetRect(pTab->GetRect().left, m_rcCaption.top + 4, pTab->GetRect().right, m_rcCaption.bottom);

			m_pContextHeaders->Add(pHeader);

			pPreviousHeader = pHeader;
		}
	}

	nCount = m_pContextHeaders->GetCount();
	if (nCount == 0)
		return;

	if (!ShrinkContextHeaders(m_rcHeader.left, m_rcHeader.right))
	{
		m_pContextHeaders->RemoveAll();
		return;
	}

	CRect rcHeaders(m_pContextHeaders->GetHeader(0)->m_rcRect.left, m_rcCaption.top + 6,
		m_pContextHeaders->GetHeader(nCount - 1)->m_rcRect.right, m_rcCaption.bottom);

	int nCaptionLength = m_rcCaptionText.Width();
	CRect rcCaptionText(m_rcCaptionText);

	if (CRect().IntersectRect(rcCaptionText, rcHeaders))
	{
		if (rcCaptionText.CenterPoint().x > rcHeaders.CenterPoint().x)
		{
			if (m_rcHeader.right - rcHeaders.right < nCaptionLength)
			{
				m_rcCaptionText.left = rcHeaders.right;
				m_rcCaptionText.right = m_rcHeader.right;
			}
			else
			{
				m_rcCaptionText.left = rcHeaders.right;
				m_rcCaptionText.right = rcHeaders.right + nCaptionLength;
			}
		}
		else
		{
			if (rcHeaders.left - m_rcHeader.left < nCaptionLength)
			{
				m_rcCaptionText.left = m_rcHeader.left;
				m_rcCaptionText.right = rcHeaders.left;
			}
			else
			{
				m_rcCaptionText.left = rcHeaders.left - nCaptionLength;
				m_rcCaptionText.right = rcHeaders.left;
			}
		}
	}
}

int CXTPRibbonBar::GetRibbonTopBorder() const
{
	CXTPCommandBarsFrameHook* pFrameHook = GetFrameHook();

	return pFrameHook && pFrameHook->IsDwmEnabled() ? pFrameHook->GetFrameBorder() : 0;
}

void CXTPRibbonBar::RepositionCaptionButtons()
{
	if (!IsFrameThemeEnabled())
	{
		return;
	}

	CRect rcQuickAccess = m_rcQuickAccess;
	if (m_bShowQuickAccessBelow || !m_bShowQuickAccess)
	{
		if (!m_pControlSystemButton)
			m_rcHeader.left += 30;
		else
			m_rcHeader.left += 70;

		if (m_pControlSystemButton)
			rcQuickAccess = m_pControlSystemButton->GetRect();
		else
			rcQuickAccess.SetRectEmpty();
	}
	else
	{
		rcQuickAccess.right += (m_pControlQuickAccess ? m_pControlQuickAccess->GetRect().Width() + 2 : 12);
		m_rcHeader.left = rcQuickAccess.right;
	}

	CWnd* pSite = GetSite();
	DWORD dwStyle = pSite->GetStyle();
	DWORD dwExStyle = pSite->GetExStyle();
	BOOL bToolWindow = (dwExStyle & WS_EX_TOOLWINDOW) == WS_EX_TOOLWINDOW;

	int nFrameBorder = GetRibbonTopBorder();

	m_rcHeader.DeflateRect(0, nFrameBorder, 0, 3);

	CXTPCommandBarsFrameHook* pFrameHook = GetFrameHook();
	BOOL bDwmEnabled = pFrameHook->IsDwmEnabled();

	BOOL bEnableClose = TRUE;
	BOOL bMaximized = (dwStyle & WS_MAXIMIZE) == WS_MAXIMIZE;
	BOOL bSysMenu = (dwStyle & WS_SYSMENU);
	BOOL bEnabledMaximize = ((dwStyle & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX);
	BOOL bEnabledMinimize = ((dwStyle & WS_MINIMIZEBOX) == WS_MINIMIZEBOX);
	BOOL bShowMinMaxButtons = !bToolWindow && bSysMenu && (bEnabledMaximize || bEnabledMinimize);
	BOOL bShowCloseButton = bSysMenu;
	if (bSysMenu && !bToolWindow)
	{
		CMenu* pMenu = GetSite()->GetSystemMenu(FALSE);
		if (pMenu && pMenu->GetMenuState(SC_CLOSE, MF_BYCOMMAND) & (MF_DISABLED | MF_GRAYED)) bEnableClose = FALSE;
	}
	if (bMaximized && (dwStyle & WS_CHILD))
	{
		bShowMinMaxButtons = bShowCloseButton = FALSE;
	}

	CRect rcHeader(m_rcHeader);

	AddCaptionButton(SC_CLOSE, !bDwmEnabled && bShowCloseButton, bEnableClose, m_rcHeader);
	AddCaptionButton(SC_RESTORE, !bDwmEnabled && bShowMinMaxButtons && bMaximized, bEnabledMaximize, m_rcHeader);
	AddCaptionButton(SC_MAXIMIZE, !bDwmEnabled && bShowMinMaxButtons && !bMaximized, bEnabledMaximize, m_rcHeader);
	AddCaptionButton(SC_MINIMIZE, !bDwmEnabled && bShowMinMaxButtons, bEnabledMinimize, m_rcHeader);

	if (bDwmEnabled)
		m_rcHeader.right -= 100;

	m_rcCaptionText.SetRect(m_rcCaptionText.left + (rcHeader.right - m_rcHeader.right), m_rcHeader.top, m_rcHeader.right, m_rcHeader.bottom);


	CClientDC dc(this);
	CXTPFontDC font(&dc, &GetPaintManager()->GetFramePaintManager()->m_fontFrameCaption);

	GetSite()->GetWindowText(m_strCaptionText);

	int nCaptionLength = dc.GetTextExtent(m_strCaptionText).cx + 20;
	nCaptionLength = min(m_rcCaptionText.Width(), nCaptionLength);

	CRect rcCaptionText(m_rcCaptionText);
	rcCaptionText.left += (rcCaptionText.Width() - nCaptionLength) / 2;
	if (rcCaptionText.left < m_rcHeader.left)
		rcCaptionText.left = m_rcHeader.left;

	rcCaptionText.right = rcCaptionText.left + nCaptionLength;

	if (CRect().IntersectRect(rcQuickAccess, rcCaptionText))
	{
		nCaptionLength = min(m_rcHeader.Width(), nCaptionLength);
		rcCaptionText.left = m_rcHeader.left + (m_rcHeader.Width() - nCaptionLength) / 2;
		rcCaptionText.right = rcCaptionText.left + nCaptionLength;
	}
	if (rcCaptionText.right > m_rcHeader.right) rcCaptionText.right = m_rcHeader.right;

	m_rcCaptionText = rcCaptionText;

}

void CXTPRibbonBar::RepositionGroups(CDC* pDC, CRect rcGroups)
{
	m_rcGroups = rcGroups;

	CXTPRibbonTab* pTab = GetSelectedTab();
	if (!pTab)
		return;

	if (IsBackstageViewVisible())
		return;


	CXTPRibbonGroups* pGroups = pTab->GetGroups();

	CXTPRibbonPaintManager* pPaintManager = GetRibbonPaintManager();

	int nMargin = 0;

	if (pPaintManager->m_nGroupSpacing > 0)
		nMargin += 4;

	pGroups->CalcDynamicSize(pDC, rcGroups.Width(), LM_COMMIT, CRect(rcGroups.left + nMargin, rcGroups.top + 3, 4 + nMargin, 3));
}

void CXTPRibbonBar::EnsureVisible(CXTPControl* pControl)
{
	if (pControl->GetRibbonGroup())
	{
		if (pControl->GetRibbonGroup()->GetParentTab() != GetSelectedTab())
		{
			pControl->GetRibbonGroup()->GetParentTab()->Select();
		}
	}

	if (GetSelectedTab() && pControl->GetRibbonGroup() &&
		(m_pControlScrollGroupsLeft->IsVisible() || m_pControlScrollGroupsRight->IsVisible()))
	{
		ShowScrollableRect(GetSelectedTab()->GetGroups(), pControl->GetRect());
	}
}

void CXTPRibbonBar::OnGroupsScroll(BOOL bScrollLeft)
{
	if (!GetSelectedTab())
		return;

	GetCommandBars()->ClosePopups();
	HideKeyboardTips();

	int nGroupsScrollPos = GetSelectedTab()->GetGroups()->m_nGroupsScrollPos;

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


CXTPTabPaintManager* CXTPRibbonBar::GetTabPaintManager() const
{
	return GetRibbonPaintManager()->GetTabPaintManager();
}

void CXTPRibbonBar::DrawCommandBar(CDC* pDC, CRect rcClipBox)
{
#if 0
	_int64 nPerfomanceEnd;
	_int64 nPerfomanceStart;

	QueryPerformanceCounter((LARGE_INTEGER*)&nPerfomanceStart);
#endif

	pDC->SetBkMode(TRANSPARENT);

	CXTPPaintManager* pPaintManager = GetPaintManager();

	CXTPFontDC font(pDC, pPaintManager->GetCommandBarFont(this));

	GetRibbonPaintManager()->FillRibbonBar(pDC, this);

	if (IsFrameThemeEnabled() && CRect().IntersectRect(m_rcCaption, rcClipBox))
	{
		GetRibbonPaintManager()->DrawRibbonFrameCaption(pDC, this, GetFrameHook()->m_bActive);
	}
	else if (m_bShowCaptionAlways && CRect().IntersectRect(m_rcCaption, rcClipBox))
	{
		GetRibbonPaintManager()->DrawRibbonFrameCaptionBar(pDC, this);
	}

	CXTPRibbonTab* pSlectedTab = GetSelectedTab();

	if (IsGroupsVisible())
	{
		GetRibbonPaintManager()->FillGroupRect(pDC, pSlectedTab, m_rcGroups);
	}

	GetTabPaintManager()->m_bDrawTextHidePrefix = !IsKeyboardCuesVisible();

	if (m_pControlTab && CRect().IntersectRect(rcClipBox, m_rcTabControl) && m_bTabsVisible)
	{
		GetTabPaintManager()->DrawTabControl(m_pControlTab, pDC, m_rcTabControl);
	}

	int i;

	if (pSlectedTab && IsGroupsVisible())
	{
		CXTPRibbonGroups* pGroups = pSlectedTab->GetGroups();
		for (i = 0; i < pGroups->GetCount(); i++)
		{
			CXTPRibbonGroup* pGroup = pGroups->GetAt(i);

			if (pGroup->IsVisible() && CRect().IntersectRect(rcClipBox, pGroup->GetRect()))
			{
				pGroup->Draw(pDC, rcClipBox);
			}
		}
	}

	BOOL bFirst = TRUE;

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl->IsVisible())
			continue;

		if (pControl->GetRibbonGroup() != NULL)
			continue;

		if (pControl->GetBeginGroup() && !bFirst)
		{
			pPaintManager->DrawCommandBarSeparator(pDC, this, pControl);
		}
		bFirst = FALSE;
		CRect rc(pControl->GetRect());

		if (CRect().IntersectRect(rcClipBox, rc))
		{
			pControl->Draw(pDC);
		}
	}


	CXTPCommandBars* pCommandBars = GetCommandBars();

	if (pCommandBars && pCommandBars->IsCustomizeMode())
	{
		CXTPControl* pDragControl = DYNAMIC_DOWNCAST(CXTPControl, pCommandBars->GetDragControl());

		if (pDragControl && pDragControl->GetParent() == this && pDragControl->IsVisible())
		{
			CRect rc = pDragControl->GetRect();
			pDC->Draw3dRect(rc, 0, 0);
			rc.DeflateRect(1, 1);
			pDC->Draw3dRect(rc, 0, 0);
		}

		CXTPRibbonTab* pDragTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, pCommandBars->GetDragControl());

		if (pDragTab && pDragTab->GetParent() == this && pDragTab->IsVisible())
		{
			CRect rc = pDragTab->GetRect();
			pDC->Draw3dRect(rc, 0, 0);
			rc.DeflateRect(1, 1);
			pDC->Draw3dRect(rc, 0, 0);
		}

		CXTPRibbonGroup* pDragGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, pCommandBars->GetDragControl());

		if (pDragGroup && pDragGroup->GetRibbonBar() == this && pDragGroup->IsVisible())
		{
			CRect rc = pDragGroup->GetRect();
			pDC->Draw3dRect(rc, 0, 0);
			rc.DeflateRect(1, 1);
			pDC->Draw3dRect(rc, 0, 0);
		}

		if (!m_rcMarker.IsRectEmpty())
		{
			CustomizeDrawMarker(pDC);
		}
	}
#if 0
	QueryPerformanceCounter((LARGE_INTEGER*)&nPerfomanceEnd);
	TRACE(_T("TotalCounter = %i \n"), int(nPerfomanceEnd - nPerfomanceStart));
#endif
}

void CXTPRibbonBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int nHit = HitTestCaption(point);

	if ((nHit == HTNOWHERE) && IsFrameThemeEnabled() && m_pContextHeaders->HitTest(point))
		nHit = HTCAPTION;

	if (nHit == HTCAPTION || nHit == HTSYSMENU)
	{
		CWnd* pSite = GetSite();
		ClientToScreen(&point);

		::DefWindowProc(pSite->GetSafeHwnd(), WM_NCLBUTTONDBLCLK, nHit, MAKELPARAM(point.x, point.y));
		return;
	}

	CXTPTabManagerItem* pItem = m_pControlTab->HitTest(point);
	if (pItem && pItem == GetSelectedTab() && m_bAllowMinimize)
	{
		if (IsRibbonMinimized())
		{
			SetRibbonMinimized(FALSE);
		}
		else if (m_bMinimizeOnDblClick)
		{
			SetRibbonMinimized(TRUE);
		}
		m_bMinimizeOnDblClick = TRUE;
	}

	CXTPMenuBar::OnLButtonDblClk(nFlags, point);
}

int CXTPRibbonBar::HitTestCaption(CPoint point) const
{
	if (IsCustomizeMode())
		return HTNOWHERE;

	if (!IsFrameThemeEnabled())
		return HTNOWHERE;

	if (m_rcCaption.PtInRect(point) && m_pControls->HitTest(point) == 0)
	{
		if (m_pContextHeaders->HitTest(point))
			return HTNOWHERE;

		DWORD dwStyle = GetSite()->GetStyle();

		if ((point.y < m_rcCaption.top + 5) && (dwStyle & WS_SIZEBOX) && ((dwStyle & WS_MAXIMIZE) == 0))
			return HTTOP;

		if ((!m_pControlSystemButton || m_pControlSystemButton->IsSimpleButton()) &&
			(point.x < 7 + GetSystemMetrics(SM_CXSMICON)) && (dwStyle & WS_SYSMENU))
			return HTSYSMENU;

		return HTCAPTION;
	}

	if (m_bTabsVisible && m_pControlTab && GetRibbonPaintManager()->m_bFlatFrame && m_rcTabControl.PtInRect(point) &&
		m_pControlTab->HitTest(point) == NULL && m_pControlTab->HitTestNavigateButton(point, TRUE) == NULL)
	{
		return HTCAPTION;
	}
	return HTNOWHERE;
}

BOOL CXTPRibbonBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	int nHit = HitTestCaption(pt);

	if (nHit >= HTSIZEFIRST && nHit <= HTSIZELAST)
	{
		CWnd* pSite = GetSite();
		pSite->SendMessage(WM_SETCURSOR, (WPARAM)pSite->m_hWnd, MAKELPARAM(nHit, message));
		return TRUE;

	}

	return CXTPMenuBar::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CXTPRibbonBar::IsBackstageViewVisible() const
{
	return m_pControlSystemButton && m_pControlSystemButton->GetPopuped() &&
		m_pControlSystemButton->GetCommandBar() && m_pControlSystemButton->GetCommandBar()->IsTrackingMode() &&
		m_pControlSystemButton->GetCommandBar()->IsKindOf(RUNTIME_CLASS(CXTPRibbonBackstageView));
}

void CXTPRibbonBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMinimizeOnDblClick = m_pControlTab->HitTest(point) == GetSelectedTab();

	if (m_bMinimizeOnDblClick && IsRibbonMinimized() && !m_pControlTab->IsPopupBarTracking())
		m_bMinimizeOnDblClick = FALSE;

	if (m_bMinimizeOnDblClick && IsCustomizeMode())
		m_bMinimizeOnDblClick = FALSE;

	CXTPRibbonTabContextHeader* pContextHeader = m_pContextHeaders->HitTest(point);
	if (pContextHeader)
	{
		pContextHeader->m_pFirstTab->Select();
		return;
	}

	int nHit = HitTestCaption(point);

	if (nHit != HTNOWHERE)
	{
		CWnd* pSite = GetSite();
		ClientToScreen(&point);

		GetCommandBars()->ClosePopups();
		UpdateWindow();

		if (nHit == HTSYSMENU)
		{
			point.x = 0;
			point.y = m_rcCaption.bottom;
			ClientToScreen(&point);
			pSite->SendMessage(0x0313, (WPARAM)pSite->GetSafeHwnd(), MAKELPARAM(point.x, point.y));

			return;
		}

		if (nHit == HTCAPTION)
		{
			::DefWindowProc(pSite->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		}
		else if (nHit >= HTSIZEFIRST && nHit <= HTSIZELAST)
		{
			if (pSite->GetStyle() & WS_MAXIMIZE)
				return;

			::DefWindowProc(pSite->GetSafeHwnd(), WM_NCLBUTTONDOWN, nHit, MAKELPARAM(point.x, point.y));
		}

		return;
	}

	if (m_rcTabControl.PtInRect(point))
	{
		GetCommandBars()->ClosePopups();

		if (IsBackstageViewVisible())
		{
			SetTrackingMode(FALSE);

			if (IsRibbonMinimized())
				return;
		}
	}

	if (m_bMinimizeOnDblClick)
	{

		CXTPMenuBar::OnLButtonDown(nFlags, point);
		return;
	}

	if (m_pControlTab->PerformClick(m_hWnd, point))
	{
		if (IsRibbonMinimized())
		{
			m_pControlTab->ShowPopupBar(FALSE);
			m_bMinimizeOnDblClick = TRUE;

		}
		return;
	}

	if (IsCustomizable() && IsCustomizeMode() && m_pControls->HitTest(point) == NULL && GetSelectedTab())
	{
		m_pCommandBars->ClosePopups();

		CXTPRibbonTab* pTab = GetSelectedTab();
		CXTPRibbonGroup* pGroup = pTab->GetGroups()->HitTest(point);
		if (pGroup)
		{
			m_pCommandBars->SetDragControl(pGroup);
			return;
		}
	}

	CXTPMenuBar::OnLButtonDown(nFlags, point);
}

BOOL CXTPRibbonBar::IsRibbonMinimized() const
{
	return m_bMinimized;
}

void CXTPRibbonBar::SetRibbonMinimized(BOOL bMinimized)
{
	if (m_bMinimized != bMinimized)
	{
		m_bMinimized = bMinimized;

		if (!m_bMinimized && GetSelectedTab() == NULL)
		{
			m_pControlTab->SetSelectedItem(m_pControlTab->FindNextFocusable(-1, +1));
		}

		RebuildControls(GetSelectedTab());

		OnRecalcLayout();

		GetCommandBars()->ClosePopups();

		GetSite()->SendMessage(WM_XTP_RIBBONMINIMIZE, (WPARAM)this);
	}
}

void CXTPRibbonBar::OnPopupRButtonUp(CXTPCommandBar* pCommandBar, CPoint point)
{
	if (!m_bShowQuickAccess)
		return;

	CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, pCommandBar);
	if (pPopupBar && pPopupBar->GetControlPopup())
	{
		if (pPopupBar->GetControlPopup()->GetID() == XTP_ID_RIBBONCONTROLQUICKACCESS ||
			pPopupBar->GetControlPopup()->GetID() == XTP_ID_RIBBONCONTROLQUICKACCESSMORE)
			return;
	}

	pCommandBar->SetPopuped(-1, FALSE);

	CXTPControl* pControl = pCommandBar->GetControls()->HitTest(point);
	if (pControl)
	{
		ShowContextMenu(point, pControl);
		return;
	}

	CXTPRibbonScrollableBar* pScrollable = GetScrollableBar(pCommandBar);
	if (pScrollable)
	{
		CXTPRibbonGroup* pGroup = pScrollable->HitTestGroup(point);
		if (pGroup && pGroup->GetRect().bottom - GetRibbonPaintManager()->m_nGroupCaptionHeight < point.y)
		{
			ShowContextMenu(point, pGroup->GetControlGroupPopup());
			return;
		}
	}
}

void CXTPRibbonBar::OnRButtonUp(UINT nFlags, CPoint point)
{
	int nHit = HitTestCaption(point);

	if (nHit == HTCAPTION || nHit == HTSYSMENU)
	{
		GetCommandBars()->ClosePopups();
		UpdateWindow();

		CWnd* pSite = GetSite();
		ClientToScreen(&point);
		pSite->SendMessage(0x0313, (WPARAM)pSite->GetSafeHwnd(), MAKELPARAM(point.x, point.y));
		return;
	}

	if (IsCustomizeMode())
		return;

	CXTPControl* pControl = m_pControls->HitTest(point);

	if (pControl && GetSite()->SendMessage(WM_XTP_CONTROLRBUTTONUP, (WPARAM)(LPPOINT)&point, (LPARAM)pControl) == TRUE)
		return;

	if (pControl && pControl->OnRButtonUp(point))
		return;

	if (pControl)
	{
		ShowContextMenu(point, pControl);
		return;
	}

	CXTPRibbonGroup* pGroup = HitTestGroup(point);
	if (pGroup && pGroup->GetRect().bottom - GetRibbonPaintManager()->m_nGroupCaptionHeight < point.y)
	{
		ShowContextMenu(point, pGroup->GetControlGroupPopup());
		return;
	}

	if (m_rcQuickAccess.PtInRect(point))
	{
		ShowContextMenu(point, m_pControlTab);
		return;
	}

	CWnd::OnRButtonUp(nFlags, point);
}


void CXTPRibbonBar::OnSysColorChange()
{
	CXTPMenuBar::OnSysColorChange();

	GetRibbonPaintManager()->RefreshMetrics();
	m_pControlTab->Reposition();
}

void CXTPRibbonBar::RefreshSysButtons()
{
	CXTPMenuBar::RefreshSysButtons();

	if (IsFrameThemeEnabled())
	{
		CString strWindowText;
		GetSite()->GetWindowText(strWindowText);

		if (strWindowText != m_strCaptionText)
			DelayLayout();
	}
}

INT_PTR CXTPRibbonBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	INT_PTR nHit = m_pControlTab ? m_pControlTab->PerformToolHitTest(m_hWnd, point, pTI) : -1;
	if (nHit != -1)
		return nHit;

	nHit = CXTPMenuBar::OnToolHitTest(point, pTI);
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

CXTPRibbonGroup* CXTPRibbonBar::HitTestGroup(CPoint point) const
{
	CXTPRibbonTab* pTab = GetSelectedTab();
	if (!pTab)
		return NULL;

	if (!IsGroupsVisible())
		return NULL;

	if (!m_rcGroups.PtInRect(point))
		return NULL;

	if (m_pControlScrollGroupsLeft && m_pControlScrollGroupsLeft->IsVisible() &&
		m_pControlScrollGroupsLeft->GetRect().PtInRect(point))
		return NULL;

	if (m_pControlScrollGroupsRight && m_pControlScrollGroupsRight->IsVisible() &&
		m_pControlScrollGroupsRight->GetRect().PtInRect(point))
		return NULL;

	return pTab->GetGroups()->HitTest(point);
}

BOOL CXTPRibbonBar::IsDwmEnabled() const
{
	CXTPCommandBarsFrameHook* pFrameHook = GetFrameHook();

	return pFrameHook && pFrameHook->IsDwmEnabled();
}

LRESULT CXTPRibbonBar::OnNcHitTest(CPoint point)
{
	if (IsDwmEnabled())
	{
		LRESULT lResult = 0;
		CXTPWinDwmWrapper().DefWindowProc(GetSite()->GetSafeHwnd(), WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y), &lResult);

		if (lResult == HTMINBUTTON || lResult == HTMAXBUTTON || lResult == HTCLOSE || lResult == HTHELP)
		{
			return HTTRANSPARENT;
		}
	}

	return CXTPMenuBar::OnNcHitTest(point);
}

void CXTPRibbonBar::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCommandBars* pCommandBars = GetCommandBars ();
	CXTPMouseManager* pMouseManager = pCommandBars->GetMouseManager();

	BOOL bMouseLocked = pMouseManager->IsMouseLocked() || !pMouseManager->IsTopParentActive(m_hWnd)
		|| pMouseManager->IsTrackedLock(this) || m_nPopuped != -1;

	CXTPRibbonGroup* pHighlightedGroup = bMouseLocked || !GetRibbonPaintManager()->m_bHotTrackingGroups ?
		NULL : HitTestGroup(point);

	if (pHighlightedGroup != m_pHighlightedGroup)
	{
		HighlightGroup(pHighlightedGroup);

		if (m_pHighlightedGroup)
		{
			pMouseManager->TrackMouseLeave(*this);
		}
	}

	if (!bMouseLocked && m_pControlTab)
	{
		m_pControlTab->PerformMouseMove(m_hWnd, point);
	}

	CXTPMenuBar::OnMouseMove(nFlags, point);
}

void CXTPRibbonBar::OnMouseLeave()
{
	if (m_pControlTab) m_pControlTab->PerformMouseMove(m_hWnd, CPoint(-1, -1));

	HighlightGroup(NULL);

	CXTPMenuBar::OnMouseLeave();
}

CXTPRibbonTab* CXTPRibbonBar::GetTab(int nIndex) const
{
	return (CXTPRibbonTab*)m_pControlTab->GetItem(nIndex);
}

int CXTPRibbonBar::GetTabCount() const
{
	return m_pControlTab->GetItemCount();
}



BOOL CXTPRibbonBar::PreviewAccel(UINT chAccel)
{
	if (GetKeyState(VK_CONTROL) < 0)
		return FALSE;

	CXTPCommandBars* pCommandBars = GetCommandBars();

	if (chAccel == (UINT)VK_MENU && GetTabCount() > 0)
	{
		pCommandBars->ClosePopups();
		pCommandBars->ShowKeyboardCues(TRUE);
		pCommandBars->m_keyboardTips.nLevel = 1;

		if (IsRibbonMinimized())
		{
			SetTrackingMode(TRUE, TRUE, TRUE);
		}
		else
		{
			SetTrackingMode(TRUE, FALSE, TRUE);

			SetSelected(m_bTabsVisible ? m_pControlTab->GetIndex() : 0, TRUE);
			m_pControlTab->SetFocused(TRUE);
		}

		return TRUE;
	}

	if (chAccel > '0' && chAccel <= '9')
	{
		int nIndex = chAccel - '0' - 1;
		CXTPControl* pControl = m_pQuickAccessControls->GetVisibleAt(nIndex);

		if (pControl && pControl->GetEnabled())
		{
			nIndex = pControl->GetIndex();

			pCommandBars->ClosePopups();
			pCommandBars->ShowKeyboardCues(TRUE);
			pCommandBars->m_keyboardTips.nLevel = 2;

			SetTrackingMode(TRUE, FALSE, TRUE);
			SetSelected(nIndex, TRUE);

			pControl->OnUnderlineActivate();

			return TRUE;
		}
	}

	for (int i = 0; i < GetTabCount(); i++)
	{
		CXTPRibbonTab* pTab = GetTab(i);

		CString strCaption = pTab->GetCaption();
		if (!strCaption.IsEmpty() && pTab->IsEnabled() && pTab->IsVisible())
		{
			int nAmpIndex = CXTPShortcutManager::FindAccelPos(strCaption);
			BOOL bFound = nAmpIndex > -1 && nAmpIndex < strCaption.GetLength() - 1;

			if (bFound && CXTPShortcutManager::CompareAccelKey(strCaption[nAmpIndex + 1], chAccel))
			{

				if (!IsTrackingMode())
					pCommandBars->ClosePopups();

				pCommandBars->ShowKeyboardCues(TRUE);

				m_bKeyboardSelect = TRUE;
				pCommandBars->m_keyboardTips.nLevel = 1;
				SetCurSel(i);

				SetTrackingMode(TRUE, FALSE, TRUE);
				SetSelected(m_pControlTab->GetIndex(), TRUE);
				m_pControlTab->SetFocused(TRUE);

				if (!IsRibbonMinimized()) pCommandBars->ShowKeyboardTips(this, 2);

				m_pControlTab->ShowPopupBar(TRUE);

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CXTPRibbonBar::IsKeyboardCuesVisible() const
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return TRUE;

	if (pCommandBars->GetCommandBarsOptions()->bShowKeyboardTips)
		return FALSE;

	return pCommandBars->m_bKeyboardCuesVisible;
}

BOOL CXTPRibbonBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	m_pControlTab->PerformMouseMove(m_hWnd, CPoint(-1, -1));

	if (m_bTracking || bMode) HighlightGroup(NULL);


	BOOL bResult = CXTPMenuBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard);

	if (!bMode && m_pControlSystemButton && m_pControlSystemButton->GetPopuped())
	{
		m_pControlSystemButton->OnSetPopup(FALSE);
	}


	return bResult;
}

BOOL CXTPRibbonBar::IsAllowQuickAccessControl(CXTPControl* pControl)
{
	if (!pControl)
		return FALSE;

	if (!m_bAllowQuickAccessDuplicates)
	{
		CXTPControl* pControlQuickAccess = m_pQuickAccessControls->FindDuplicate(pControl);
		if (pControlQuickAccess)
		{
			return FALSE;
		}
	}

	if (pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonGroupControlPopup)))
	{
		return pControl->GetID() > 0;
	}

	if (pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonGroupOption)))
	{
		return pControl->GetID() > 0;
	}

	if ((pControl->GetFlags() & xtpFlagNoMovable) == 0)
		return TRUE;

	return FALSE;
}

void CXTPRibbonBar::ShowContextMenu(CPoint point, CXTPControl* pSelectedControl)
{
	if (!pSelectedControl)
		return;

	pSelectedControl->GetParent()->ClientToScreen(&point);

	CXTPPopupBar* pPopupBar = CreateContextMenu(pSelectedControl);
	if (!pPopupBar)
		return;

	CWnd* pSite = m_bShowQuickAccess ? this : GetOwnerSite();

	CXTPMouseManager* pMouseManager = XTPMouseManager();

	if (pPopupBar->GetControls()->GetVisibleCount() > 0)
	{
		BOOL bLocked = pMouseManager->IsMouseLocked();
		if (bLocked) pMouseManager->UnlockMouseMove();

		CXTPCommandBars::TrackPopupMenu(pPopupBar, TPM_RIGHTBUTTON | (pSelectedControl->GetParent() == this ? 0 : TPM_RECURSE),
			point.x, point.y, pSite, NULL, pSite);

		if (bLocked) pMouseManager->LockMouseMove();
	}

	pPopupBar->InternalRelease();
}

CXTPPopupBar* CXTPRibbonBar::CreateMoreQuickAccessContextMenu()
{
	CXTPPopupBar* pPopupBar = new CXTPRibbonBarMorePopupToolBar(this);

	for (int i = 0; i < m_pQuickAccessControls->GetCount(); i++)
	{
		CXTPControl* pControl = m_pQuickAccessControls->GetAt(i);
		if (pControl->GetHideFlags() & xtpHideWrap)
		{
			CXTPControl* pClone = pPopupBar->GetControls()->AddClone(pControl);
			pClone->SetHideFlag(xtpHideWrap, FALSE);
		}
	}

	CXTPControl* pClone = pPopupBar->GetControls()->AddClone(m_pControlQuickAccess);
	pClone->SetHideFlag(xtpHideWrap, FALSE);

	pPopupBar->SetWidth(300);

	return pPopupBar;
}


CXTPPopupBar* CXTPRibbonBar::CreateContextMenu(CXTPControl* pSelectedControl)
{
	if (!m_bShowQuickAccess)
	{
		CXTPPopupBar* pPopupBar = GetCommandBars()->GetToolbarsPopup();
		GetSite()->SendMessage(WM_XTP_TOOLBARCONTEXTMENU, (WPARAM)this, (LPARAM)pPopupBar);
		return pPopupBar;
	}

	if (!pSelectedControl)
		return NULL;

	if (pSelectedControl->GetType() == xtpControlGallery &&
		((CXTPControlPopup*)pSelectedControl)->GetCommandBar() == NULL &&
		DYNAMIC_DOWNCAST(CXTPPopupBar, pSelectedControl->GetParent()))
	{
		pSelectedControl = ((CXTPPopupBar*)pSelectedControl->GetParent())->GetControlPopup();
	}
	CXTPCommandBars* pCommandBars = GetCommandBars();

	BOOL bMoreMenu = pSelectedControl->GetID() == XTP_ID_RIBBONCONTROLQUICKACCESS;

	CMenu menu;
	if (!XTPResourceManager()->LoadMenu(&menu, bMoreMenu ? XTP_IDR_RIBBONCUSTOMIZEMENUMORE : XTP_IDR_RIBBONCUSTOMIZEMENU))
		return NULL;

	CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(pCommandBars);
	pPopupBar->SetOwner(this);
	pPopupBar->LoadMenu(menu.GetSubMenu(0));

	CXTPControl* pControlToolbars = pPopupBar->GetControls()->InsertAt(new CXTPControlToolbars, bMoreMenu ? 0 : 2);
	pControlToolbars->SetBeginGroup(TRUE);

	BOOL bQuickAccess = IsQuickAccessControl(pSelectedControl);

	BOOL bAllow = IsAllowQuickAccessControl(pSelectedControl);
	BOOL bSystem = ((pSelectedControl->GetFlags() & xtpFlagNoMovable) && !bAllow) || pSelectedControl->IsTemporary();

	for (int i = 0; i < pPopupBar->GetControlCount(); i++)
	{
		CXTPControl* pControl = pPopupBar->GetControl(i);

		if (pControl->GetID() == XTP_ID_RIBBONCUSTOMIZE_MORE)
			pControl->SetID(XTP_ID_RIBBONCUSTOMIZE);

		if (pControl->GetID() == XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSBELOW_MORE)
			pControl->SetID(XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSBELOW);

		if (pControl->GetID() == XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSABOVE_MORE)
			pControl->SetID(XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSABOVE);

		switch (pControl->GetID())
		{
			case XTP_ID_RIBBONCUSTOMIZE_REMOVE:
				pControl->SetVisible(bQuickAccess && !bSystem && m_bAllowQuickAccessCustomization);
				pControl->SetTag((DWORD_PTR)pSelectedControl);
				break;

			case XTP_ID_RIBBONCUSTOMIZE_ADD:
				pControl->SetVisible(!bQuickAccess && !bSystem && m_bAllowQuickAccessCustomization);
				pControl->SetEnabled(bAllow);
				pControl->SetTag((DWORD_PTR)pSelectedControl);
				break;

			case XTP_ID_RIBBONCUSTOMIZE:
				if (!m_bAllowQuickAccessCustomization)
				{
					if (pCommandBars->IsCustomizeAvail())
						pControl->SetCaption(XTP_IDS_CUSTOMIZE);
					else
						pControl->SetVisible(FALSE);
				}
				break;

			case XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSBELOW:
				pControl->SetVisible(m_bShowQuickAccess && !m_bShowQuickAccessBelow);
				break;

			case XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSABOVE:
				pControl->SetVisible(m_bShowQuickAccess && m_bShowQuickAccessBelow);
				break;

			case XTP_ID_RIBBONCUSTOMIZE_LABEL:
				pControl = pPopupBar->GetControls()->SetControlType(i, xtpControlLabel);
				pControl->SetFlags(xtpFlagSkipFocus);
				pControl->SetItemDefault(TRUE);
				pControl->SetVisible(m_bAllowQuickAccessCustomization && GetQuickAccessControls()->GetOriginalControls());
				break;

			case XTP_ID_RIBBONCUSTOMIZE_MINIMIZE:
				pControl->SetVisible(m_bAllowMinimize);
				pControl->SetChecked(m_bMinimized);
				if (GetTabCount() == 0)
					pControl->SetEnabled(FALSE);
				break;

			case XTP_ID_RIBBONCUSTOMIZE_COMMANDS:
				{
					pControl->SetHideFlags(xtpHideGeneric);
					CXTPControls* pCommands = GetQuickAccessControls()->GetOriginalControls();
					if (!pCommands)
						continue;

					if (!m_bAllowQuickAccessCustomization)
						continue;

					for (int j = 0; j < pCommands->GetCount(); j++)
					{
						CXTPControl* pControlOriginal = pCommands->GetAt(j);

						CControlQuickAccessCommand* pControlCommand = new CControlQuickAccessCommand(GetQuickAccessControls(), pControlOriginal);
						pPopupBar->GetControls()->Add(pControlCommand, 0, NULL, i, TRUE);
						pControlCommand->SetCaption(pControlOriginal->GetCaption());

						CXTPControl* pControlQuickAccess = GetQuickAccessControls()->FindDuplicate(pControlOriginal);
						if (pControlQuickAccess)
						{
							pControlCommand->SetQuickAccessControl(pControlQuickAccess);
						}

						i++;
					}

				}
				break;

			default:
				continue;
		}
		pControl->SetFlags(pControl->GetFlags() | xtpFlagManualUpdate);
	}

	GetSite()->SendMessage(WM_XTP_TOOLBARCONTEXTMENU, (WPARAM)this, (LPARAM)pPopupBar);

	return pPopupBar;
}

void CXTPRibbonBar::SelectNextTab(BOOL bNext)
{
	m_pControlTab->PerformKeyDown(m_hWnd, !bNext ? VK_LEFT : VK_RIGHT);
}

int CXTPRibbonBar::OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult)
{
	if (IsTrackingMode() && nMessage == WM_CONTEXTMENU)
	{
		if (lParam != -1)
			return TRUE;

		CXTPControl* pControl = GetControl(m_nSelected);
		if (!pControl)
			return TRUE;

		ShowContextMenu(pControl->GetRect().TopLeft(), pControl);
		return TRUE;
	}

	if (IsTrackingMode() && nMessage == WM_MOUSEWHEEL && !GetCommandBars()->GetMouseManager()->IsMouseLocked()
		&& !IsKeyboardTipsVisible())
	{
		SelectNextTab((short)HIWORD(wParam) <= 0);
		return TRUE;
	}

	if (IsTrackingMode() && nMessage == WM_KEYDOWN && wParam == VK_F10)
	{
		SetTrackingMode(FALSE);
		return TRUE;
	}

	return CXTPMenuBar::OnHookMessage(hWnd, nMessage, wParam, lParam, lResult);
}


void CXTPRibbonBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars) return;

	if (IsCustomizeMode())
	{
		CXTPCommandBar::OnRButtonDown(nFlags, point);
		return;
	}

	SetTrackingMode(FALSE);

	int nHit = HitTestCaption(point);
	if (nHit != HTNOWHERE)
		return;

	CXTPControl* pControl = m_pControls->HitTest(point);

	if (pControl && pControl->OnRButtonDown(point))
		return;
}

void CXTPRibbonBar::OnCustomizePlaceQuickAccess(UINT nCommand)
{
	GetCommandBars()->ClosePopups();
	ShowQuickAccessBelowRibbon(nCommand == XTP_ID_RIBBONCUSTOMIZE_QUICKACCESSBELOW);
}

LRESULT CXTPRibbonBar::OnCustomizeCommand(WPARAM wParam, LPARAM lParam)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)lParam;
	CXTPControl* pControl = (CXTPControl*)tagNMCONTROL->pControl->GetTag();

	if (wParam == XTP_ID_RIBBONCUSTOMIZE_ADD)
	{
		GetCommandBars()->ClosePopups();

		m_bInRecalcLayout = TRUE;
		pControl = m_pQuickAccessControls->AddClone(pControl, -1, TRUE);

		pControl->SetBeginGroup(FALSE);
		pControl->SetVisible(TRUE);
		pControl->SetFlags(pControl->GetFlags() & ~xtpFlagNoMovable);

		XTPControlType nType = pControl->GetType();

		if (nType == xtpControlGallery || nType == xtpControlPopup)
			pControl->SetStyle(xtpButtonIcon);
		else
			pControl->SetStyle(xtpButtonAutomatic);


		if (m_nQuickAccessEmptyIconId != -1 &&
			(nType == xtpControlGallery || nType == xtpControlPopup || nType == xtpControlButton || nType == xtpControlButtonPopup || nType == xtpControlSplitButtonPopup))
		{
			CXTPImageManagerIcon* pIcon = pControl->GetImage();
			if (pIcon == NULL)
			{
				pControl->SetIconId(m_nQuickAccessEmptyIconId);

			}
		}

		m_bInRecalcLayout = FALSE;

		OnRecalcLayout();

		return TRUE;
	}

	if (wParam == XTP_ID_RIBBONCUSTOMIZE_REMOVE)
	{
		m_pQuickAccessControls->Remove(pControl);
		OnRecalcLayout();

		return TRUE;
	}

	if (wParam == XTP_ID_RIBBONCUSTOMIZE)
	{
		GetCommandBars()->ClosePopups();
		CWnd* pOwner = GetOwnerSite();

		if (pOwner->SendMessage(WM_XTP_COMMAND, wParam, lParam) == 0)
		{
			pOwner->SendMessage(WM_COMMAND, wParam);
		}

		return TRUE;
	}

	if (wParam == XTP_ID_RIBBONCUSTOMIZE_MINIMIZE)
	{
		SetRibbonMinimized(!IsRibbonMinimized());
		return TRUE;
	}

	if ((int)wParam < 9000 || (int)wParam > 10000)
	{
		tagNMCONTROL->pControl->GetParent()->GetRootParent()->OnTrackLost();

		if (m_pCommandBars->GetSite()->SendMessage(WM_XTP_COMMAND, wParam, lParam) == 0)
		{
			m_pCommandBars->GetSite()->SendMessage(WM_COMMAND, wParam);
		}
		return TRUE;
	}




	return FALSE;
}

BOOL CXTPRibbonBar::ShouldSerializeBar()
{
	return TRUE;
}

void CXTPRibbonBar::GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam)
{
	if (IsCustomizable())
	{
		CXTPMenuBar::GenerateCommandBarList(nID, pCommandBarList, pParam);
	}
	else
	{
		if (!pCommandBarList->Lookup(this))
		{
			pCommandBarList->Add(this);
			InternalAddRef();

			ASSERT(m_nBarID != 0);

			m_pQuickAccessControls->GenerateCommandBarList(nID, pCommandBarList, pParam);
		}
	}
}

void CXTPRibbonBar::RestoreCommandBarList(CXTPCommandBarList* pCommandBarList)
{
	if (IsCustomizable())
	{
		CXTPMenuBar::RestoreCommandBarList(pCommandBarList);
	}
	else
	{
		m_pQuickAccessControls->RestoreCommandBarList(pCommandBarList);
	}
}

void CXTPRibbonBar::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		ASSERT(pPX->m_dwData != 0);
		if (pPX->m_dwData) SetCommandBars(((XTP_COMMANDBARS_PROPEXCHANGE_PARAM*)pPX->m_dwData)->pCommandBars);
	}

	PX_Bool(pPX, _T("ShowQuickAccessBelow"), m_bShowQuickAccessBelow, FALSE);

	if (pPX->GetSchema() > _XTP_SCHEMA_103)
		PX_Bool(pPX, _T("Minimized"), m_bMinimized, FALSE);

	if (pPX->GetSchema() > _XTP_SCHEMA_110)
		PX_Bool(pPX, _T("Customizable"), m_bCustomizable, FALSE);

	if (IsCustomizable())
	{
		CXTPMenuBar::DoPropExchange(pPX);

		CXTPPropExchangeSection secSystemButton(pPX->GetSection(_T("SystemButton")));
		BOOL bSystemButton = m_pControlSystemButton != NULL;
		PX_Bool(&secSystemButton, _T("SystemButton"), bSystemButton, TRUE);

		if (bSystemButton && pPX->IsLoading())
		{
			GetControls()->Remove(m_pControlSystemButton);
			m_pControlSystemButton = NULL;
			PX_Object(&secSystemButton, m_pControlSystemButton, RUNTIME_CLASS(CXTPRibbonControlSystemButton));
			GetControls()->InsertAt(m_pControlSystemButton, 0);
		}
		else if (bSystemButton && pPX->IsStoring())
		{
			PX_Object(&secSystemButton, m_pControlSystemButton, RUNTIME_CLASS(CXTPRibbonControlSystemButton));
		}

		CXTPPropExchangeSection secTabs(pPX->GetSection(_T("Tabs")));
		m_pControlTab->DoPropExchange(&secTabs);

	}

	CXTPPropExchangeSection secControls(pPX->GetSection(_T("QuickAccessControls")));
	m_pQuickAccessControls->DoPropExchange(&secControls);

}

void CXTPRibbonBar::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive /*= FALSE*/)
{
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pCommandBar;

	m_bShowQuickAccessBelow = pRibbonBar->m_bShowQuickAccessBelow;

	m_bMinimized = pRibbonBar->m_bMinimized;

	if (m_bMinimized)
	{
		m_pControlTab->SetSelectedItem(NULL);
	}

	if (pRibbonBar->IsCustomizable())
	{
		CXTPMenuBar::Copy(pCommandBar, bRecursive);

		m_pControlTab->Copy(pRibbonBar->m_pControlTab);

		GetControls()->Remove(m_pControlSystemButton);
		m_pControlSystemButton = NULL;

		if (pRibbonBar->m_pControlSystemButton)
		{
			m_pControlSystemButton = (CXTPRibbonControlSystemButton*)GetControls()->AddClone(pRibbonBar->m_pControlSystemButton,
				0, TRUE);
		}
		RebuildControls(GetSelectedTab());
	}

	m_pQuickAccessControls->RemoveAll();

	for (int i = 0; i < pRibbonBar->GetQuickAccessControls()->GetCount(); i++)
	{
		m_pQuickAccessControls->AddClone(pRibbonBar->GetQuickAccessControls()->GetAt(i), -1, FALSE);
	}
}

void CXTPRibbonBar::MergeToolBar(CXTPCommandBar* pCommandBar, BOOL /*bSilent*/)
{
	Copy(pCommandBar, FALSE);
}

CXTPRibbonTab* CXTPRibbonBar::FindTab(int nId) const
{
	return m_pControlTab->FindTab(nId);
}

CXTPRibbonGroup* CXTPRibbonBar::FindGroup(int nId) const
{
	for (int i = 0; i < GetTabCount(); i++)
	{
		CXTPRibbonTab* pTab = GetTab(i);
		CXTPRibbonGroup* pGroup = pTab->FindGroup(nId);

		if (pGroup)
			return pGroup;
	}
	return NULL;
}

void CXTPRibbonBar::ShowQuickAccessBelowRibbon(BOOL bBelow)
{
	m_bShowQuickAccessBelow = bBelow;

	OnRecalcLayout();

	CXTPCommandBarsFrameHook* pFrameHook = GetFrameHook();

	if (pFrameHook != NULL)
	{
		if (IsDwmEnabled())
		{
			UpdateWindow();
			pFrameHook->UpdateFrameRegion();
		}
		else
		{
			pFrameHook->GetSite()->SendMessage(WM_NCPAINT);
		}
	}
}

BOOL CXTPRibbonBar::IsQuickAccessBelowRibbon() const
{
	return m_bShowQuickAccessBelow;
}

void CXTPRibbonBar::RemoveAllTabs()
{
	m_pControlTab->DeleteAllItems();
}

void CXTPRibbonBar::RemoveTab(int nIndex)
{
	CXTPRibbonTab* pTab = GetTab(nIndex);
	if (!pTab)
		return;

	pTab->Remove();
}

CSize CXTPRibbonBar::GetButtonSize() const
{
	if (m_szButtons != CSize(0)) return m_szButtons;

	int nHeight = GetPaintManager()->GetControlHeight();
	return CSize(nHeight, nHeight);
}

void CXTPRibbonBar::SetFontHeight(int nFontHeight)
{
	GetRibbonPaintManager()->SetFontHeight(nFontHeight);

	OnRecalcLayout();

	CXTPCommandBarsFrameHook* pFrameHook = GetFrameHook();

	if (pFrameHook && IsDwmEnabled())
	{
		UpdateWindow();
		pFrameHook->UpdateFrameRegion();
	}

	if (pFrameHook)
	{
		pFrameHook->RedrawFrame();
	}
}

int CXTPRibbonBar::GetFontHeight() const
{
	return GetRibbonPaintManager()->GetFontHeight();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CXTPRibbonBar::OnKeyboardTip(CXTPCommandBarKeyboardTip* pTip)
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
					GetCommandBars()->ShowKeyboardTips(pPopupToolBar);

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

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, pTip->m_pOwner);
	if (pTab)
	{
		SetCurSel(pTab->GetIndex());

		SetTrackingMode(TRUE, FALSE, TRUE);
		SetSelected(m_pControlTab->GetIndex(), TRUE);
		m_pControlTab->SetFocused(TRUE);

		m_pControlTab->ShowPopupBar(TRUE);

		if (!IsRibbonMinimized()) GetCommandBars()->ShowKeyboardTips(this, 2);

		return;
	}
}

void CXTPRibbonScrollableBar::CreateControlKeyboardTips(CXTPControl* pControl, LPCTSTR lpszPrefix, BOOL bVisible)
{
	if (!pControl || (pControl->GetFlags() & xtpFlagSkipFocus))
		return;

	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
	CXTPRibbonPaintManager* pPaintManager = pCommandBars->GetPaintManager()->GetRibbonPaintManager();
	CRect rcGroups = GetGroupsRect();

	if (pControl->GetCaption().IsEmpty() && pControl->GetKeyboardTip().IsEmpty())
		return;

	CXTPRibbonGroup* pGroup = pControl->GetRibbonGroup();
	ASSERT(pGroup);
	if (!pGroup)
		return;

	CRect rc(pControl->GetRect());

	CPoint pt(rc.left + 21, pControl->GetRect().bottom - 4);
	DWORD dwAlign = DT_TOP;

	CRect rcGroup = pGroup->GetRect();
	CRect rcCaption(rcGroup.left, rcGroup.bottom - pPaintManager->GetGroupCaptionHeight() + 2, rcGroup.right, rcGroup.bottom);
	rcGroup.bottom = rcCaption.top;

	if (CRect().IntersectRect(rc, rcCaption))
	{
		pt = CPoint(rc.CenterPoint().x, rcGroups.bottom - 6);
		dwAlign = DT_TOP | DT_CENTER;
	}
	else if (pControl->GetStyle() == xtpButtonIconAndCaptionBelow && rc.Height() > rcGroup.Height() / 2)
	{
		pt = CPoint(rc.CenterPoint().x, rcGroup.bottom - 2);
		dwAlign = DT_CENTER | DT_VCENTER;
	}
	else if (pControl->GetType() == xtpControlGallery)
	{
		pt = CPoint(rc.right, rcGroup.bottom - 2);
		dwAlign = DT_CENTER | DT_VCENTER;
	}
	else if (rc.CenterPoint().y < rcGroup.top + rcGroup.Height() * 1 / 3)
	{
		pt = CPoint(rc.left + 11, rcGroup.top + 2);
		dwAlign = DT_LEFT | DT_VCENTER;
	}
	else if (rc.CenterPoint().y > rcGroup.top + rcGroup.Height() * 2 / 3)
	{
		pt = CPoint(rc.left + 11, rcGroup.bottom - 2);
		dwAlign = DT_LEFT | DT_VCENTER;
	}
	else
	{
		pt = CPoint(rc.left + 11, rcGroup.CenterPoint().y - 1);
		dwAlign = DT_LEFT | DT_VCENTER;
	}
	if (pControl->GetType() == xtpControlCheckBox || pControl->GetType() == xtpControlRadioButton)
	{
		pt.x = rc.left + 6;
	}
	if ((pControl->GetType() == xtpControlComboBox || pControl->GetType() == xtpControlEdit)
		&& pControl->GetStyle() == xtpButtonAutomatic)
	{
		pt.x = rc.CenterPoint().x;
		dwAlign |= DT_CENTER;
	}

	CXTPCommandBarKeyboardTip* pWnd = new CXTPCommandBarKeyboardTip(pCommandBars, pControl, pControl->GetCaption(), pt, dwAlign, pControl->GetEnabled());

	if (!pControl->GetKeyboardTip().IsEmpty())
	{
		pWnd->m_strTip = pControl->GetKeyboardTip();
	}

	if (lpszPrefix)
	{
		pWnd->m_strPrefix = lpszPrefix;
	}

	CRect rcIntersect; rcIntersect.IntersectRect(rcGroups, rc);
	pWnd->m_bVisible = bVisible && (rcIntersect == rc) && (pControl->IsVisible());

	pCommandBars->m_keyboardTips.arr.Add(pWnd);
}

void CXTPRibbonScrollableBar::CreateGroupKeyboardTips(CXTPRibbonTab* pSelectedTab)
{
	for (int i = 0; i < pSelectedTab->GetGroups()->GetCount(); i++)
	{
		CXTPRibbonGroup* pGroup = pSelectedTab->GetGroups()->GetAt(i);
		BOOL bVisible = pGroup->IsVisible();

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

void CXTPRibbonBar::CreateKeyboardTips()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return;

	if (IsBackstageViewVisible())
		return;

	if (pCommandBars->m_keyboardTips.nLevel == 1)
	{
		int i;


		int k = 1;
		for (i = 0; i < m_pQuickAccessControls->GetCount(); i++)
		{
			CXTPControl* pControl = m_pQuickAccessControls->GetAt(i);
			if (!pControl->IsVisible())
				continue;

			CPoint pt(pControl->GetRect().CenterPoint().x, pControl->GetRect().bottom - 11);
			CString strCaption;
			if (k < 10)
			{
				strCaption.Format(_T("%i"), k);
			}
			else if (k < 19)
			{
				strCaption.Format(_T("0%i"), k - 9);
			}
			else break;
			k++;

			CXTPCommandBarKeyboardTip* pWnd = new CXTPCommandBarKeyboardTip(pCommandBars, pControl, strCaption, pt, DT_CENTER | DT_TOP, pControl->GetEnabled());

			pCommandBars->m_keyboardTips.arr.Add(pWnd);
		}

		if (m_bTabsVisible)
		{
			for (i = 0; i < GetTabCount(); i++)
			{
				CXTPRibbonTab* pTab = GetTab(i);

				CString strCaption = pTab->GetCaption();
				if (!strCaption.IsEmpty() && pTab->IsEnabled() && pTab->IsVisible())
				{
					CPoint pt(pTab->GetRect().CenterPoint().x, pTab->GetRect().bottom - 9);

					CXTPCommandBarKeyboardTip* pWnd = new CXTPCommandBarKeyboardTip(pCommandBars, pTab, strCaption, pt, DT_CENTER | DT_TOP, TRUE);

					if (!pTab->GetKeyboardTip().IsEmpty())
					{
						pWnd->m_strTip = pTab->GetKeyboardTip();
					}

					pCommandBars->m_keyboardTips.arr.Add(pWnd);
				}
			}
		}

		if (m_pControlSystemButton)
		{
			CPoint pt(m_pControlSystemButton->GetRect().CenterPoint().x, m_pControlSystemButton->GetRect().CenterPoint().y);

			CXTPCommandBarKeyboardTip* pWnd = new CXTPCommandBarKeyboardTip(pCommandBars,
				m_pControlSystemButton, m_pControlSystemButton->GetCaption(), pt, DT_VCENTER | DT_CENTER, m_pControlSystemButton->GetEnabled());

			if (!m_pControlSystemButton->GetKeyboardTip().IsEmpty())
			{
				pWnd->m_strTip = m_pControlSystemButton->GetKeyboardTip();
			}

			pCommandBars->m_keyboardTips.arr.Add(pWnd);

		}

		if (GetTabCount() == 0) // Acts as simple menu
		{
			for (i = 0; i < m_pControls->GetCount(); i++)
			{
				CXTPControl* pControl = GetControl(i);

				if (IsQuickAccessControl(pControl) || pControl == m_pControlSystemButton || pControl->GetRibbonGroup() != 0)
					continue;

				if (!pControl->IsVisible() || pControl->GetCaption().IsEmpty() || (pControl->GetFlags() & xtpFlagSkipFocus))
					continue;

				if (pControl->GetType() == xtpControlGallery)
					continue;

				CPoint pt(pControl->GetRect().left + 12, pControl->GetRect().bottom - 11);

				CXTPCommandBarKeyboardTip* pWnd = new CXTPCommandBarKeyboardTip(pCommandBars, pControl, pControl->GetCaption(), pt, DT_TOP, pControl->GetEnabled());

				if (!pControl->GetKeyboardTip().IsEmpty())
				{
					pWnd->m_strTip = pControl->GetKeyboardTip();
				}

				pCommandBars->m_keyboardTips.arr.Add(pWnd);
			}
		}
	}

	if (pCommandBars->m_keyboardTips.nLevel == 2)
	{
		CreateGroupKeyboardTips(GetSelectedTab());
	}
}

CSize CXTPRibbonBar::GetIconSize() const
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	return m_szIcons != CSize(0) ? m_szIcons:
		(pCommandBars->GetCommandBarsOptions()->szIcons != CSize(0) ?
		pCommandBars->GetCommandBarsOptions()->szIcons : GetAutoIconSize(FALSE));
}

void CXTPRibbonBar::OnCustomizeDrop(CXTPControl* pDataObject, DROPEFFECT& dropEffect, CPoint ptDrop, CPoint ptDrag)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	ASSERT(pCommandBars);
	if (!pCommandBars)
		return;

	CRect rcMarker;
	int nDropIndex;
	BOOL bDropAfter;

	CustomizeFindDropIndex (pDataObject, ptDrop, rcMarker, nDropIndex, bDropAfter);

	CXTPControl* pDropControl = nDropIndex != -1 ? GetControl(nDropIndex) : NULL;

	if (nDropIndex == -1 || pDataObject != pDropControl)
	{
		CXTPRibbonGroup* pGroup = pDropControl ? pDropControl->GetRibbonGroup() : NULL;

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


		CXTPControl* pClone = NULL;

		if (pGroup != NULL)
		{
			pClone = pGroup->AddClone(pDataObject, pGroup->IndexOf(pDropControl) + (bDropAfter ? 1 : 0));
		}
		else
		{
			pClone = m_pControls->AddClone(pDataObject, nDropIndex);

		}

		pClone->SetBeginGroup(bBeginGroup);
		pCommandBars->SetDragControl(pClone);

		if (m_pControlTab && nDropIndex == m_pControlTab->GetIndex() + 1 && bDropAfter)
			pClone->SetFlags(pClone->GetFlags() | xtpFlagRightAlign);

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

void CXTPRibbonBar::CustomizeFindDropIndex(CXTPControl* pDataObject, const CPoint& point, CRect& rcMarker, int& nDropIndex, BOOL& bDropAfter)
{
	CXTPMenuBar::CustomizeFindDropIndex(pDataObject, point, rcMarker, nDropIndex, bDropAfter);

	if (nDropIndex != -1)
	{
		if (m_pControlSystemButton && nDropIndex == m_pControlSystemButton->GetIndex())
			rcMarker.SetRectEmpty();

		if (IsQuickAccessControl(GetControl(nDropIndex)))
			rcMarker.SetRectEmpty();
	}

	if (IsCustomizable() && rcMarker.IsRectEmpty() && GetSelectedTab())
	{
		CXTPRibbonGroup* pGroup = GetSelectedTab()->GetGroups()->HitTest(point);

		if (pGroup && GetControls()->HitTest(point) == NULL)
		{
			CRect rc(pGroup->GetRect());
			rcMarker.SetRect(rc.left + 2, rc.top + 2, rc.left + 8, rc.top + 22);

			nDropIndex =  pGroup->GetControlGroupOption()->GetIndex();
			bDropAfter = FALSE;
			m_bVerticalMarker = FALSE;
		}
	}
}

#define xtpKeyDown xtpKeyPopup
#define xtpKeyUp xtpKeyBack

CRect CXTPRibbonScrollableBar::_GetBoundRect(CXTPControl* pControl, BOOL bSelected, XTPSpecialKey key)
{
	CRect rc(pControl->GetRect());

	if (pControl->GetRibbonGroup() && DYNAMIC_DOWNCAST(CXTPRibbonGroupOption, pControl))
	{
		rc = pControl->GetRibbonGroup()->GetCaptionRect();
		rc.DeflateRect(4, 0, 4, 0);
	}
	else if (!bSelected && (key == xtpKeyDown || key == xtpKeyUp) && !pControl->GetRibbonGroup() && DYNAMIC_DOWNCAST(CXTPRibbonBarControlQuickAccessPopup, pControl))
	{
		rc.right = CXTPClientRect(pControl->GetParent()).right;
	}
	else if (bSelected && key == xtpKeyUp && DYNAMIC_DOWNCAST(CXTPRibbonControlTab, pControl))
	{
		if (((CXTPRibbonControlTab*)pControl)->GetSelectedItem())
		{
			rc = ((CXTPRibbonControlTab*)pControl)->GetSelectedItem()->GetRect();
		}
	}

	return rc;
}

BOOL CXTPRibbonBar::ProcessSpecialKey(XTPSpecialKey key)
{
	if (key != xtpKeyEscape)
		HideKeyboardTips();

	if (CXTPRibbonScrollableBar::ProcessSpecialKey(key))
		return TRUE;

	return CXTPMenuBar::ProcessSpecialKey(key);
}

BOOL CXTPRibbonScrollableBar::ProcessSpecialKey(XTPSpecialKey key)
{
	CXTPControl* pSelected = m_pParent->GetSelected();

	if (!pSelected)
		return FALSE;

	if (key == xtpKeyReturn || key == xtpKeySpace)
	{
		if (pSelected->IsFocused())
			pSelected->OnClick(TRUE);
		else
			pSelected->OnUnderlineActivate();

		return TRUE;
	}

	if (key != xtpKeyDown && key != xtpKeyUp && key != xtpKeyLeft && key != xtpKeyRight)
		return FALSE;

	CRect rcCondidate(0, 0, 0, 0);
	CRect rcSelected = _GetBoundRect(pSelected, TRUE, key);
	int nSelCenter = rcSelected.CenterPoint().y;
	CXTPControl* pCondidate = NULL;
	CXTPControl* pFirst = NULL;
	int nCount = m_pParent->GetControlCount();

	if (key == xtpKeyRight)
	{
		for (int i = 0; i < nCount; i++)
		{
			CXTPControl* pControl = m_pParent->GetControl(i);
			if (pControl == pSelected || !pControl->IsVisible(xtpHideScroll) || (pControl->GetFlags() & xtpFlagSkipFocus))
				continue;

			CRect rc = _GetBoundRect(pControl, FALSE, key);

			if (((rc.top + 1 >= rcSelected.top && rc.top < rcSelected.bottom) || (rcSelected.top + 1 >= rc.top && rcSelected.top < rc.bottom)))
			{
				if ((rc.left >= rcSelected.right) && ((pCondidate == NULL) || (rc.left < rcCondidate.left) ||
					(rc.left == rcCondidate.left && abs(rc.CenterPoint().y - nSelCenter) < abs(rcCondidate.CenterPoint().y - nSelCenter))))
				{
					pCondidate = pControl;
					rcCondidate = rc;
				}

				if (pFirst == NULL || rc.left < pFirst->GetRect().left ||
					(rc.left == pFirst->GetRect().left && abs(rc.CenterPoint().y - nSelCenter) < abs(pFirst->GetRect().CenterPoint().y - nSelCenter)))
				{
					pFirst = pControl;
				}
			}
		}
	}
	else if (key == xtpKeyLeft)
	{
		for (int i = 0; i < nCount; i++)
		{
			CXTPControl* pControl = m_pParent->GetControl(i);
			if (pControl == pSelected || !pControl->IsVisible(xtpHideScroll) || (pControl->GetFlags() & xtpFlagSkipFocus))
				continue;

			CRect rc = _GetBoundRect(pControl, FALSE, key);

			if (((rc.top + 1 >= rcSelected.top && rc.top < rcSelected.bottom) || (rcSelected.top + 1 >= rc.top && rcSelected.top < rc.bottom)))
			{
				if ((rc.right <= rcSelected.left) && ((pCondidate == NULL) || (rc.right > rcCondidate.right) ||
					(rc.right == rcCondidate.right && abs(rc.CenterPoint().y - nSelCenter) < abs(rcCondidate.CenterPoint().y - nSelCenter))))
				{
					pCondidate = pControl;
					rcCondidate = rc;
				}

				if (pFirst == NULL || rc.right >= pFirst->GetRect().right ||
					(rc.right == pFirst->GetRect().right && abs(rc.CenterPoint().y - nSelCenter) < abs(pFirst->GetRect().CenterPoint().y - nSelCenter)))
				{
					pFirst = pControl;
				}
			}
		}
	}
	else if (key == xtpKeyDown)
	{
		nSelCenter = rcSelected.CenterPoint().x;

		for (int i = 0; i < nCount; i++)
		{
			CXTPControl* pControl = m_pParent->GetControl(i);
			if (pControl == pSelected || !pControl->IsVisible(xtpHideScroll) || (pControl->GetFlags() & xtpFlagSkipFocus))
				continue;

			CRect rc = _GetBoundRect(pControl, FALSE, key);

			if (((rc.left + 1 >= rcSelected.left && rc.left < rcSelected.right) || (rcSelected.left + 1 >= rc.left && rcSelected.left < rc.right)))
			{
				if ((rc.top >= rcSelected.bottom) && ((pCondidate == NULL) || (rc.top < rcCondidate.top) ||
					(rc.top == rcCondidate.top && abs(rc.CenterPoint().x - nSelCenter) < abs(rcCondidate.CenterPoint().x - nSelCenter))))
				{
					pCondidate = pControl;
					rcCondidate = rc;
				}
			}
		}
	}
	else if (key == xtpKeyUp)
	{
		nSelCenter = rcSelected.CenterPoint().x;

		for (int i = 0; i < nCount; i++)
		{
			CXTPControl* pControl = m_pParent->GetControl(i);
			if (pControl == pSelected || !pControl->IsVisible(xtpHideScroll) || (pControl->GetFlags() & xtpFlagSkipFocus))
				continue;

			CRect rc = _GetBoundRect(pControl, FALSE, key);

			if (((rc.left + 1 >= rcSelected.left && rc.left < rcSelected.right) || (rcSelected.left + 1 >= rc.left && rcSelected.left < rc.right)))
			{
				if ((rc.bottom <= rcSelected.top) && ((pCondidate == NULL) || (rc.bottom > rcCondidate.bottom) ||
					(rc.bottom == rcCondidate.bottom && abs(rc.CenterPoint().x - nSelCenter) < abs(rcCondidate.CenterPoint().x - nSelCenter))))
				{
					pCondidate = pControl;
					rcCondidate = rc;
				}
			}
		}
	}

	if (pCondidate)
	{
		m_pParent->SetSelected(pCondidate->GetIndex(), key == xtpKeyRight || key == xtpKeyDown ? TRUE_KEYBOARD_NEXT : TRUE_KEYBOARD_PREV);
	}
	else if (pFirst)
	{
		m_pParent->SetSelected(pFirst->GetIndex(), key == xtpKeyRight || key == xtpKeyDown? TRUE_KEYBOARD_NEXT : TRUE_KEYBOARD_PREV);
	}


	return TRUE;
}

void CXTPRibbonBar::Reset(BOOL /*bShowWarningMessage*/)
{
	CWnd* pWnd = GetSite();
	if (pWnd && ::IsWindow(pWnd->m_hWnd)) pWnd->SendMessage(WM_XTP_CUSTOMIZATION_RESETTOOLBAR, 0, (LPARAM)this);

	RefreshMenu();
	OnInitialUpdate();
	OnRecalcLayout();
}

BOOL CXTPRibbonBar::OnFrameMouseWheel(BOOL bForward)
{
	CPoint pt;
	GetCursorPos(&pt);

	CXTPWindowRect rc(this);
	if (!rc.PtInRect(pt))
		return FALSE;

	HWND hWnd = ::WindowFromPoint(pt);
	if (hWnd != m_hWnd && !::IsChild(m_hWnd, hWnd))
		return FALSE;

	CWnd* pWnd = GetSite();
	if (!pWnd->IsWindowEnabled())
		return FALSE;

	SelectNextTab(bForward);
	return TRUE;
}

BOOL CXTPRibbonBar::IsControlEnabled(const CXTPControl* pControl) const
{
	if (!IsBackstageViewVisible())
		return CXTPCommandBar::IsControlEnabled(pControl);

	if (pControl == m_pControlQuickAccess || pControl == m_pControlQuickAccessMore)
		return FALSE;

	if (pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonGroupControlPopup)))
		return FALSE;

	if ((pControl == m_pControlSystemButton) || pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonBarControlCaptionButton)) || (pControl->GetFlags() & xtpFlagManualUpdate))
		return -1; // Default for control

	return FALSE;
}

