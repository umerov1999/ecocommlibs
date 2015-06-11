// XTPShortcutBarItem.cpp : implementation of the CXTPShortcutBar class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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
#include "Resource.h"
#include "Common/Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include <Common/ScrollBar/XTPScrollInfo.h>

#ifdef _XTP_INCLUDE_COMMANDBARS
#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPMouseManager.h"
#endif

#include "XTPShortcutBarPaintManager.h"
#include "XTPShortcutBarPane.h"
#include "XTPShortcutBar.h"
#include "XTPShortcutBarPopup.h"
#include "XTPShortcutBarAnimation.h"

#include <ShortcutBar/Themes/XTPShortcutBarOffice2000Theme.h>
#include <ShortcutBar/Themes/XTPShortcutBarOfficeXPTheme.h>
#include <ShortcutBar/Themes/XTPShortcutBarOffice2003Theme.h>
#include <ShortcutBar/Themes/XTPShortcutBarNativeWinXPTheme.h>
#include <ShortcutBar/Themes/XTPShortcutBarResourceTheme.h>
#include <ShortcutBar/Themes/XTPShortcutBarOffice2013Theme.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTP_ID_SHORTCUT_SELECTED 8000
#define XTP_ID_SHORTCUT_CUSTOMIZE 8001


CXTPShortcutBarItem::CXTPShortcutBarItem(CXTPShortcutBar* pShortcutBar, int nID, CWnd* pWnd)
{

#ifndef _XTP_ACTIVEX
	CString strCaption;
	if (nID > 0 && strCaption.LoadString((UINT)nID))
	{
		m_strCaption = m_strTooltip = strCaption;

		int nShortCutPos = strCaption.Find ('\n');
		if (nShortCutPos != -1)
		{
			m_strTooltip = strCaption.Mid(nShortCutPos + 1);
			m_strCaption = strCaption.Left (nShortCutPos);
		}
	}
#endif


	m_pShortcutBar = pShortcutBar;
	m_bVisible = TRUE;

	m_rcItem.SetRectEmpty();
	m_hwndChild = pWnd ? pWnd->GetSafeHwnd() : NULL;
	m_nID = nID;
	m_bExpandButton = FALSE;
	m_dwData = 0;
	m_bSelected = FALSE;
	m_nIconId = -1;
	m_bExpanded = FALSE;
	m_bHidden = FALSE;
	m_bEnabled = TRUE;
	m_bNavigateButton = FALSE;


	EnableAutomation();

#ifdef _XTP_ACTIVEX
	EnableTypeLib();
#endif

}


CXTPImageManagerIcon* CXTPShortcutBarItem::GetImage(int nWidth) const
{
	return m_pShortcutBar->GetImageManager()->GetImage(GetIconId(), nWidth);
}

void CXTPShortcutBarItem::SetClientWindow(CWnd* pWnd)
{
	if (m_hwndChild)
	{
		::ShowWindow(m_hwndChild, SW_HIDE);
	}

	m_hwndChild = pWnd->GetSafeHwnd();
	m_pShortcutBar->Reposition();
}


void CXTPShortcutBarItem::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);

	if (m_strCaption != strCaption)
	{
		m_strCaption = strCaption;
		m_pShortcutBar->RedrawControl();
	}
}

int CXTPShortcutBarItem::GetIconId() const
{
	return m_nIconId <= 0 ? m_nID: m_nIconId;
}

void CXTPShortcutBarItem::SetIconId(int nId)
{
	if (m_nIconId != nId)
	{
		m_nIconId = nId;
		m_pShortcutBar->RedrawControl();
	}
}

void CXTPShortcutBarItem::SetTooltip(LPCTSTR strTooltip)
{
	m_strTooltip = strTooltip;
	m_pShortcutBar->Reposition();
}

void CXTPShortcutBarItem::SetID(int nID)
{
	m_nID = nID;
}

int CXTPShortcutBarItem::GetID() const
{
	return m_nID;
}

BOOL CXTPShortcutBarItem::IsSelected() const
{
	if (m_pShortcutBar->IsSingleSelection())
		return m_pShortcutBar->GetSelectedItem() == this;

	return m_bSelected;
}

void CXTPShortcutBarItem::SetSelected(BOOL bSelected)
{
	if (m_pShortcutBar->IsSingleSelection())
	{
		if (bSelected)
		{
			m_pShortcutBar->SelectItem(this);
		}
	}
	else
	{
		m_bSelected = bSelected;
		m_pShortcutBar->RedrawControl();

	}
}

void CXTPShortcutBarItem::SetVisible(BOOL bVisible)
{
	if (m_bVisible != bVisible)
	{
		m_bVisible = bVisible;
		m_pShortcutBar->Reposition();
	}
}

BOOL CXTPShortcutBarItem::IsVisible() const
{
	return m_bVisible;
}

BOOL CXTPShortcutBarItem::IsHidden() const
{
	return m_bHidden;
}

BOOL CXTPShortcutBarItem::IsEnabled() const
{
	return m_bEnabled;
}

void CXTPShortcutBarItem::SetEnabled(BOOL bEnabled)
{
	if (m_bEnabled != bEnabled)
	{
		m_bEnabled = bEnabled;
		m_pShortcutBar->RedrawControl();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPShortcutBar

CXTPShortcutBar::CXTPShortcutBar()
{
	m_pPaintManager = new CXTPShortcutBarOffice2003Theme;
	m_pPaintManager->RefreshMetrics();
	m_paintTheme = xtpShortcutThemeOffice2003;

	m_nExpandedLines = 1;

	m_nDesiredExpandedLinesHeight = 0;

	m_szItem = CSize(22, 32);
	m_rcClient.SetRectEmpty();

	// Add expand button
	m_arrItems.Add(CreateConfigureButton());

	m_pNavigationPaneItem = CreateNavigationItem();


	m_hSizeCursor = AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_SIZENS));

	m_bTrackingSize = FALSE;
	m_pPressedItem = m_pSelectedItem = m_pHotItem = NULL;

	m_hwndClient = NULL;

	m_nMinClientHeight = 200;
	m_bAllowResize = TRUE;

	m_bClientPaneVisible = TRUE;
	m_pImageManager = NULL;
	m_bSingleSelection = TRUE;
	m_bShowActiveItemOnTop = FALSE;
	m_bAllowFreeResize = FALSE;

	m_bAllowCollapse = TRUE;
	m_bShowGripper = TRUE;

	m_hHandCursor = AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(32649));

	if (m_hHandCursor == 0)
		m_hHandCursor = XTPResourceManager()->LoadCursor(XTP_IDC_HAND);

	m_pToolTipContext = new CXTPToolTipContext;
	m_bPreSubclassWindow = TRUE;

	m_pAnimation = new CXTPShortcutBarAnimation(this);

	m_bAllowMinimize = FALSE;
	m_nMinimizedWidth = 32;
	m_bMinimized = FALSE;
	m_bShowMinimizeButton = FALSE;

	m_pActivePopup = NULL;


	RegisterWindowClass();
}

CXTPShortcutBar::~CXTPShortcutBar()
{
	CMDTARGET_RELEASE(m_pPaintManager);

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		m_arrItems[i]->InternalRelease();
	}

	CMDTARGET_RELEASE(m_pImageManager);

	CMDTARGET_RELEASE(m_pToolTipContext);

	CMDTARGET_RELEASE(m_pNavigationPaneItem);

	delete m_pAnimation;
}

BOOL CXTPShortcutBar::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance, _T("XTPShortcutBar"), 0);
}

void CXTPShortcutBar::RedrawControl(LPCRECT lpRect, BOOL bAnimate)
{
	if (m_hWnd)
	{
		m_pAnimation->RedrawRect(lpRect, bAnimate);
	}
}

CXTPShortcutBarItem* CXTPShortcutBar::CreateNavigationItem()
{
	CXTPShortcutBarItem* pItem = new CXTPShortcutBarItem(this, -1, 0);
	pItem->m_bNavigateButton = TRUE;

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&pItem->m_strCaption, XTP_IDS_SHORTCUT_NAVIGATIONPANE));

	return pItem;
}

CXTPShortcutBarItem* CXTPShortcutBar::CreateConfigureButton()
{
	CXTPShortcutBarItem* pItem = new CXTPShortcutBarItem(this, -1, 0);
	pItem->m_bExpandButton = TRUE;

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&pItem->m_strCaption, XTP_IDS_SHORTCUT_CONFIGURE));
	pItem->m_strTooltip = pItem->m_strCaption;

	return pItem;
}

void CXTPShortcutBar::ShowExpandButton(BOOL bShow)
{
	ASSERT(GetItemCount() > 0);
	ASSERT(GetItem(GetItemCount() - 1)->IsItemExpandButton());

	if (GetItemCount() > 0)
	{
		GetItem(GetItemCount() - 1)->SetVisible(bShow);
		Reposition();
	}
}


BOOL CXTPShortcutBar::IsExpandButtonVisible() const
{
	ASSERT(GetItemCount() > 0);
	ASSERT(GetItem(GetItemCount() - 1)->IsItemExpandButton());

	return GetItemCount() > 0 && GetItem(GetItemCount() - 1)->IsVisible();
}




CXTPImageManager* CXTPShortcutBar::GetImageManager() const
{
	return m_pImageManager ? m_pImageManager : XTPImageManager();
}

void CXTPShortcutBar::SetImageManager(CXTPImageManager* pImageManager)
{
	if (m_pImageManager)
		m_pImageManager->InternalRelease();

	m_pImageManager = pImageManager;

	Reposition();
}

BOOL CXTPShortcutBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CreateEx(0, _T("XTPShortcutBar"), NULL, dwStyle, rect, pParentWnd, nID))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CXTPShortcutBar::PreCreateWindow(CREATESTRUCT& cs)
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

void CXTPShortcutBar::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	if (m_bPreSubclassWindow) Reposition();
}

void CXTPShortcutBar::SetTheme(XTPShortcutBarPaintTheme paintTheme)
{
	CMDTARGET_RELEASE(m_pPaintManager);

	switch(paintTheme)
	{
	case xtpShortcutThemeOffice2000:
		m_pPaintManager = new CXTPShortcutBarOffice2000Theme();
		break;

	case xtpShortcutThemeOfficeXP:
		m_pPaintManager = new CXTPShortcutBarOfficeXPTheme();
		break;

	case xtpShortcutThemeOffice2003:
		m_pPaintManager = new CXTPShortcutBarOffice2003Theme();
		break;

	case xtpShortcutThemeOffice2007 |
		  xtpShortcutThemeOffice2010:
		m_pPaintManager = new CXTPShortcutBarResourceTheme();
		break;

	case xtpShortcutThemeNativeWinXP:
		m_pPaintManager = new CXTPShortcutBarNativeWinXPTheme();
		break;
	case xtpShortcutThemeOffice2013:
		m_pPaintManager = new CXTPShortcutBarOffice2013Theme();
		break;

	}

	m_paintTheme = paintTheme;
	m_pPaintManager->RefreshMetrics();

	if (m_hWnd)
	{
		Reposition();
		RedrawControl();
		SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	}
}

BOOL CXTPShortcutBar::IsShortcutBarMinimized() const
{
	return m_bMinimized;
}

void CXTPShortcutBar::SetCustomTheme(CXTPShortcutBarPaintManager* pPaintManager)
{
	if (!pPaintManager)
		return;

	CMDTARGET_RELEASE(m_pPaintManager);

	m_pPaintManager = pPaintManager;
	m_pPaintManager->RefreshMetrics();

	Reposition();
	RedrawControl();
}


void CXTPShortcutBar::SetExpandedLinesCount(int nLinesCount)
{
	m_nDesiredExpandedLinesHeight = nLinesCount * m_szItem.cy;
	Reposition();
}

void CXTPShortcutBar::SetExpandedLinesHeight(int nHeight)
{
	m_nDesiredExpandedLinesHeight = nHeight;
	Reposition();
}


CXTPShortcutBarItem* CXTPShortcutBar::AddItem(UINT nID, CWnd* pWnd)
{
	CXTPShortcutBarItem* pItem = new CXTPShortcutBarItem(this, nID, pWnd);
	m_arrItems.InsertAt(m_arrItems.GetSize() - 1, pItem);

	Reposition();

	return pItem;
}



int CXTPShortcutBar::GetVisibleItemsCount() const
{
	int nCount = 0;
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->m_bVisible) nCount++;

	}
	return nCount;
}

int CXTPShortcutBar::GetCollapsedItemsCount() const
{
	int nCount = 0;
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->m_bVisible && !m_arrItems[i]->m_bHidden && !m_arrItems[i]->m_bExpanded)
			nCount++;

	}
	return nCount;
}


void CXTPShortcutBar::Reposition()
{
	if (!m_hWnd)
		return;

	static BOOL bReposition = FALSE;

	if (bReposition)
		return;

	bReposition = TRUE;


	CXTPClientRect rcShortcutBar(this);
	int nWidth = rcShortcutBar.Width();
	int nHeight = rcShortcutBar.Height();
	int nGripperHeight = m_bShowGripper ? m_pPaintManager->DrawGripper(NULL, this, FALSE) : 0;

	BOOL bMinimized = m_bAllowMinimize && rcShortcutBar.Width() <= m_nMinimizedWidth;

	if (bMinimized != m_bMinimized)
	{
		m_bMinimized = bMinimized;

		if (m_bMinimized && m_hwndClient)
			::ShowWindow(m_hwndClient, SW_HIDE);

		if (!m_bMinimized && m_hwndClient)
			::ShowWindow(m_hwndClient, SW_SHOW);

	}

	for (int nItem = 0; nItem < m_arrItems.GetSize(); nItem++)
		m_arrItems[nItem]->m_bHidden = FALSE;

	int nVisibleItemsCount = GetVisibleItemsCount();

	BOOL bShowActiveItemOnTop = m_bShowActiveItemOnTop && m_bClientPaneVisible;

	int nExpandedItem = 0;

	if (!m_bClientPaneVisible)
	{
		m_nExpandedLines = nVisibleItemsCount;
		nGripperHeight = 0;

		m_rcGripper.SetRectEmpty();
		m_rcClient.SetRectEmpty();

		nHeight = min(nHeight, m_nExpandedLines * m_szItem.cy);
		if (nHeight - m_nExpandedLines * m_szItem.cy < 0)
		{
			m_nExpandedLines = nHeight/ m_szItem.cy;
			nHeight = m_nExpandedLines * m_szItem.cy;
		}
		nHeight--;
	}
	else
	{

		int nTopItemHeight = 0;
		if (bShowActiveItemOnTop)
		{
			for (; nExpandedItem < m_arrItems.GetSize(); nExpandedItem++)
			{

				CXTPShortcutBarItem* pItem = GetItem(nExpandedItem);
				if (!pItem->m_bVisible)
					continue;

				pItem->m_bExpanded = TRUE;
				pItem->m_rcItem.SetRect(0, nTopItemHeight, nWidth, nTopItemHeight + m_szItem.cy);

				nVisibleItemsCount--;

				nTopItemHeight += m_szItem.cy;

				if (m_pSelectedItem == pItem)
					break;

			}
		}


		int nClientHeight = 0;

		int nMinClientHeight = m_nMinClientHeight;
		CXTPShortcutBarPane* pPaneClient = DYNAMIC_DOWNCAST(CXTPShortcutBarPane, CWnd::FromHandle(m_hwndClient));
		if (pPaneClient) nMinClientHeight = max(nMinClientHeight, pPaneClient->GetMinimumClientHeight());

		if (bShowActiveItemOnTop && m_hwndClient == NULL)
		{
			nGripperHeight = 0;
			nClientHeight = nTopItemHeight;
			nMinClientHeight = 0;

			m_nExpandedLines = (nHeight - nClientHeight - nGripperHeight) / m_szItem.cy;

			if (m_nExpandedLines > nVisibleItemsCount)
			{
				m_nExpandedLines = nVisibleItemsCount;
			}

			if ((m_nExpandedLines < nVisibleItemsCount) && !m_bAllowCollapse)
			{
				nClientHeight = nHeight - nGripperHeight - m_szItem.cy * nVisibleItemsCount;
				m_nExpandedLines = nVisibleItemsCount;
			}

			if (m_nExpandedLines < 1)
			{
				nClientHeight = nHeight - nGripperHeight - m_szItem.cy;
				m_nExpandedLines = 1;
			}
		}
		else if (m_bAllowFreeResize)
		{
			nClientHeight = nHeight - m_nDesiredExpandedLinesHeight - nGripperHeight;

			if (nClientHeight - nTopItemHeight < nMinClientHeight)
			{
				nClientHeight = nMinClientHeight + nTopItemHeight;
			}

			m_nExpandedLines = (nHeight - nClientHeight - nGripperHeight) / m_szItem.cy;

			if (m_nExpandedLines > nVisibleItemsCount)
			{
				m_nExpandedLines = nVisibleItemsCount;
			}

			if ((m_nExpandedLines < nVisibleItemsCount) && !m_bAllowCollapse)
			{
				nClientHeight = nHeight - nGripperHeight - m_szItem.cy * nVisibleItemsCount;
				m_nExpandedLines = nVisibleItemsCount;
			}

			if (m_nExpandedLines < 1)
			{
				nClientHeight = nHeight - nGripperHeight - m_szItem.cy;
				m_nExpandedLines = 1;
			}

		}
		else
		{
			m_nExpandedLines = m_nDesiredExpandedLinesHeight / m_szItem.cy;

			if (nHeight - nTopItemHeight - m_nExpandedLines * m_szItem.cy - nGripperHeight < nMinClientHeight)
			{
				m_nExpandedLines = (nHeight - nTopItemHeight - nGripperHeight - nMinClientHeight) / m_szItem.cy;

			}

			if (m_nExpandedLines > nVisibleItemsCount || !m_bAllowCollapse)
				m_nExpandedLines = nVisibleItemsCount;

			if (m_nExpandedLines < 1)
				m_nExpandedLines = 1;

			nClientHeight = nHeight - m_nExpandedLines * m_szItem.cy - nGripperHeight;
		}


		m_rcGripper.SetRect(0, nClientHeight, nWidth, nClientHeight + nGripperHeight);
		m_rcClient.SetRect(0, nTopItemHeight, nWidth, nClientHeight);

		if (bShowActiveItemOnTop && nVisibleItemsCount == 0)
		{
			m_rcClient.SetRect(0, nTopItemHeight, nWidth, nHeight);
			m_rcGripper.SetRectEmpty();
		}
	}


	BOOL bExpandButtonVisible = IsExpandButtonVisible();


	int nExpandedLines = m_nExpandedLines;

	for (; nExpandedItem < m_arrItems.GetSize(); nExpandedItem++)
	{

		CXTPShortcutBarItem* pItem = GetItem(nExpandedItem);
		if (!pItem->m_bVisible)
			continue;

		if (m_pSelectedItem == pItem && bShowActiveItemOnTop)
			continue;

		if ((!(nExpandedLines == 1 && !bExpandButtonVisible && nVisibleItemsCount == 1))
			&& (nExpandedLines < 2)) break;

		pItem->m_bExpanded = TRUE;
		int nItemTop = m_rcGripper.bottom + (m_nExpandedLines - nExpandedLines) * m_szItem.cy;
		pItem->m_rcItem.SetRect(0, nItemTop, nWidth, nItemTop +  m_szItem.cy);

		nExpandedLines--;
		nVisibleItemsCount--;
	}

	int nCollapsedCount = min(nVisibleItemsCount, nWidth / m_szItem.cx) - 1;
	int nCollapsedPos = nWidth - m_szItem.cx * (nCollapsedCount + 1);

	for (int nCollapsedItem = nExpandedItem; nCollapsedItem < m_arrItems.GetSize(); nCollapsedItem++)
	{
		CXTPShortcutBarItem* pItem = GetItem(nCollapsedItem);
		if (!pItem->m_bVisible)
			continue;

		if (m_pSelectedItem == pItem && bShowActiveItemOnTop)
			continue;

		pItem->m_bExpanded = FALSE;

		if (pItem->IsItemExpandButton())
			pItem->m_rcItem.SetRect(nWidth - m_szItem.cx, rcShortcutBar.Height() - m_szItem.cy, nWidth, rcShortcutBar.Height());
		else if ((nCollapsedCount > 0) || ((nCollapsedCount == 0) && !bExpandButtonVisible))
			pItem->m_rcItem.SetRect(nCollapsedPos, rcShortcutBar.Height() - m_szItem.cy, nCollapsedPos + m_szItem.cx, rcShortcutBar.Height());
		else
			pItem->m_bHidden = TRUE;

		nCollapsedPos += m_szItem.cx;
		nCollapsedCount--;
	}


	if (m_hwndClient && !m_bMinimized)
	{
		::MoveWindow(m_hwndClient, m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), TRUE);
	}

	if (m_bMinimized)
	{
		m_pNavigationPaneItem->m_rcItem = m_rcClient;
		m_pNavigationPaneItem->m_bVisible = TRUE;
	}
	else
	{
		m_pNavigationPaneItem->m_rcItem.SetRectEmpty();
		m_pNavigationPaneItem->m_bVisible = FALSE;
	}

	RedrawControl();

	bReposition = FALSE;

	OnRepositionDone();
}

void CXTPShortcutBar::OnRepositionDone()
{
#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireEvent(5, EVENT_PARAM(VTS_NONE));
#endif
}

INT_PTR CXTPShortcutBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{

	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

#if !defined(_XTP_ACTIVEX) && defined(_XTP_INCLUDE_COMMANDBARS)
	if (XTPMouseManager()->IsTrackedLock(NULL) || XTPMouseManager()->IsMouseLocked())
		return -1;
#endif

	CXTPShortcutBarItem* pItem = HitTest(point);
	if (!pItem)
		return -1;

	CClientDC dc(CWnd::FromHandle(m_hWnd));

	BOOL bShowToolTip = !pItem->IsExpanded() ||
		GetPaintManager()->IsShortcutItemTruncated(&dc, pItem);

	if (bShowToolTip)
	{
		nHit = pItem->GetID();

		if (pItem->IsItemExpandButton())
			nHit = XTP_IDS_SHORTCUT_CONFIGURE;

		CString strTip = pItem->GetTooltip();
		if (strTip.GetLength() == 0)
			return -1;

		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, pItem->GetItemRect(),
			nHit, strTip, pItem->GetCaption(), strTip, GetImageManager());

		return nHit;
	}

	return -1;
}


CXTPShortcutBarItem* CXTPShortcutBar::HitTest(CPoint pt) const
{
	if (m_pNavigationPaneItem->m_bVisible && m_pNavigationPaneItem->m_rcItem.PtInRect(pt))
		return m_pNavigationPaneItem;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (pItem->m_bVisible && pItem->IsEnabled() && !pItem->m_bHidden && pItem->m_rcItem.PtInRect(pt))
			return m_arrItems[i];
	}
	return NULL;
}

void CXTPShortcutBar::SetHotItem(CXTPShortcutBarItem* pItem)
{
	if (m_pHotItem == pItem)
		return;

	CXTPShortcutBarItem* pHotItem = m_pHotItem;
	m_pHotItem = pItem;

	if (pHotItem)
		RedrawControl(pHotItem->GetItemRect(), TRUE);

	if (m_pHotItem)
		RedrawControl(m_pHotItem->GetItemRect(), FALSE);

	if (m_pHotItem)
	{
		TRACKMOUSEEVENT tme =
		{
			sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0
		};
		_TrackMouseEvent(&tme);
	}

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireEvent(4, EVENT_PARAM(VTS_NONE));
#endif

}

void CXTPShortcutBar::SelectItem(CXTPShortcutBarItem* pItem)
{
	if (!pItem)
		return;

	if (pItem->IsItemExpandButton())
	{
		OnExpandButtonDown(pItem);
		return;
	}

	if (pItem->m_bNavigateButton)
	{
		OnNavigateButtonDown(pItem);
		return;
	}


	if ((m_pSelectedItem == pItem) && m_bSingleSelection)
		return;

	if (GetOwner()->SendMessage(XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_SELECTION_CHANGING, (LPARAM)pItem) == -1)
		return;


	if (pItem->m_hwndChild != m_hwndClient)
	{
		if (m_hwndClient)
		{
			::ShowWindow(m_hwndClient, SW_HIDE);
		}

		m_hwndClient = pItem->m_hwndChild;

		if (m_hwndClient && !m_bMinimized)
		{
			::ShowWindow(m_hwndClient, SW_SHOW);
		}
	}

	if (m_bSingleSelection)
	{
		m_pSelectedItem = pItem;
	}
	else
	{
		pItem->m_bSelected = !pItem->m_bSelected;
	}


	Reposition();

	GetOwner()->SendMessage(XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_SELECTION_CHANGED, (LPARAM)pItem);
}

void CXTPShortcutBar::OnNavigateButtonDown(CXTPShortcutBarItem* pNavigateButton)
{
	if (m_pActivePopup)
		return;

	SetHotItem(NULL);

	CXTPShortcutBarItem* pItem = GetSelectedItem();
	if (!pItem)
		return;

	HWND hwndChild = pItem->m_hwndChild;
	if (!hwndChild)
		return;

	pNavigateButton->m_bExpanded = TRUE;
	RedrawControl();

	m_pActivePopup = new CXTPShortcutBarPopup(this, hwndChild);

	m_pActivePopup->DoModal();

	SAFE_DELETE(m_pActivePopup);

	pNavigateButton->m_bExpanded = FALSE;
	RedrawControl();

}

void CXTPShortcutBar::OnExpandButtonDown(CXTPShortcutBarItem* pExpandButton)
{

#ifdef _XTP_INCLUDE_COMMANDBARS

	CXTPPopupBar* pPopup = CXTPPopupBar::CreatePopupBar(NULL);

	if (m_bClientPaneVisible)
	{
		pPopup->GetControls()->Add(xtpControlButton, XTP_ID_SHORTCUT_SHOW_MORE);
		pPopup->GetControls()->Add(xtpControlButton, XTP_ID_SHORTCUT_SHOW_FEWER);
	}

	CXTPControlPopup* pControlAddOrRemove = (CXTPControlPopup*)pPopup->GetControls()->Add(xtpControlPopup, XTP_ID_CUSTOMIZE_ADDORREMOVE);
	pControlAddOrRemove->SetFlags(xtpFlagManualUpdate);

	for (int j = 0; j < m_arrItems.GetSize() - 1; j++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[j];

		CXTPControl* pControl = pControlAddOrRemove->GetCommandBar()->GetControls()->Add(xtpControlButton, XTP_ID_SHORTCUT_CUSTOMIZE);
		pControl->SetIconId(pItem->GetID());
		pControl->SetCaption(pItem->GetCaption());
		pControl->SetTag((DWORD_PTR)pItem);
		pControl->SetChecked(pItem->m_bVisible);
		pControl->SetFlags(xtpFlagManualUpdate);
	}



	BOOL bBeginGroup = TRUE;
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (pItem->m_bVisible && pItem->m_bHidden)
		{
			CXTPControl* pControl = pPopup->GetControls()->Add(xtpControlButton, XTP_ID_SHORTCUT_SELECTED);
			pControl->SetIconId(pItem->GetID());
			pControl->SetCaption(pItem->GetCaption());
			pControl->SetTag((DWORD_PTR)pItem);

			pControl->SetFlags(xtpFlagManualUpdate);

			pControl->SetBeginGroup(bBeginGroup);

			bBeginGroup = FALSE;
		}
	}

	CPoint pt(pExpandButton->GetItemRect().right, pExpandButton->GetItemRect().CenterPoint().y);
	ClientToScreen(&pt);

	CXTPCommandBars::TrackPopupMenu(pPopup, 0, pt.x, pt.y, this);

	pPopup->InternalRelease();

	SetHotItem(NULL);

#else

#ifdef _XTP_ACTIVEX
	BOOL bCancel = FALSE;
	((COleControl*)GetParent())->FireEvent(2, EVENT_PARAM(VTS_PBOOL), &bCancel);

	if (bCancel)
		return;
#endif

	CMenu menu;
	menu.CreatePopupMenu();


	if (m_bClientPaneVisible)
	{
		CString str;
		XTPResourceManager()->LoadString(&str, XTP_ID_SHORTCUT_SHOW_MORE);
		menu.AppendMenu(MF_STRING, XTP_ID_SHORTCUT_SHOW_MORE, str);

		if (GetVisibleItemsCount() == m_nExpandedLines)
			menu.EnableMenuItem(XTP_ID_SHORTCUT_SHOW_MORE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);


		XTPResourceManager()->LoadString(&str, XTP_ID_SHORTCUT_SHOW_FEWER);

		menu.AppendMenu(MF_STRING, XTP_ID_SHORTCUT_SHOW_FEWER, str);
		if (m_nExpandedLines <= 1)
			menu.EnableMenuItem(XTP_ID_SHORTCUT_SHOW_FEWER, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}


	BOOL bBeginGroup = m_bClientPaneVisible;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (pItem->m_bVisible && pItem->m_bHidden)
		{
			if (bBeginGroup)
			{
				menu.AppendMenu(MF_SEPARATOR);
			}
			menu.AppendMenu(MF_STRING, i + 1, pItem->GetCaption());
			bBeginGroup = FALSE;
		}
	}

	CPoint pt(pExpandButton->GetItemRect().right, pExpandButton->GetItemRect().CenterPoint().y);
	ClientToScreen(&pt);

	int nCmd = menu.TrackPopupMenu(TPM_RETURNCMD, pt.x, pt.y, this);

	if (nCmd > 0)
	{
		if (nCmd > m_arrItems.GetSize())
			SendMessage(WM_COMMAND, nCmd);
		else
		{
			SelectItem(m_arrItems[nCmd - 1]);
		}
	}

#endif
}


BEGIN_MESSAGE_MAP(CXTPShortcutBar, CWnd)
	//{{AFX_MSG_MAP(CXTPShortcutBar)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(XTP_ID_SHORTCUT_SHOW_MORE, OnShowMoreButtons)
	ON_COMMAND(XTP_ID_SHORTCUT_SHOW_FEWER, OnShowFewerButtons)
	ON_UPDATE_COMMAND_UI(XTP_ID_SHORTCUT_SHOW_MORE, OnUpdateShowMoreButtons)
	ON_UPDATE_COMMAND_UI(XTP_ID_SHORTCUT_SHOW_FEWER, OnUpdateShowFewerButtons)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP

#ifdef _XTP_INCLUDE_COMMANDBARS
	ON_MESSAGE(WM_XTP_COMMAND, OnExpandPopupExecute)
#endif

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPShortcutBar message handlers

void CXTPShortcutBar::OnPaint()
{
	CPaintDC paintDC(this);
	m_pAnimation->OnPaint(paintDC);
}

LRESULT CXTPShortcutBar::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		OnDraw(pDC, CXTPClientRect(this));
	}

	return TRUE;
}

void CXTPShortcutBar::OnDraw(CDC* pDC, CRect /*rcClipBox*/)
{
	CXTPClientRect rc(this);
	m_pPaintManager->FillShortcutBar(pDC, this);

	if (m_bClientPaneVisible && m_bShowGripper && !m_rcGripper.IsRectEmpty())
		m_pPaintManager->DrawGripper(pDC, this, TRUE);

	if (m_pNavigationPaneItem->m_bVisible)
	{
		m_pPaintManager->DrawNavigationItem(pDC, m_pNavigationPaneItem);

	}

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (!pItem->m_bVisible || pItem->m_bHidden)
			continue;
		m_pPaintManager->DrawShortcutItem(pDC, pItem);
	}
}

BOOL CXTPShortcutBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPShortcutBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Reposition();
}

void CXTPShortcutBar::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case XTP_TID_ANIMATION:
		m_pAnimation->OnAnimate();
		return;
	}

	CWnd::OnTimer(nIDEvent);
}

void CXTPShortcutBar::OnDestroy()
{
	m_pAnimation->OnDestroy();

	CWnd::OnDestroy();
}

int CXTPShortcutBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Reposition();

	return 0;
}

void CXTPShortcutBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
{
	if (m_pPaintManager->m_bShowBorder)
	{
		RECT& rc = lpncsp->rgrc[0];
		rc.left   += 1;
		rc.top    += 1;
		rc.right  -= 1;
		rc.bottom -= 1;
	}
}

void CXTPShortcutBar::OnNcPaint()
{
	CWindowDC dc(this); // device context for painting

	if (m_pPaintManager->m_bShowBorder)
	{
		m_pPaintManager->DrawShortcutBarFrame(&dc, this);
	}
}

BOOL CXTPShortcutBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if ((m_bTrackingSize || m_rcGripper.PtInRect(pt)) && m_bAllowResize && m_bShowGripper)
	{
		::SetCursor(m_hSizeCursor);
		return TRUE;
	}

	if (m_pHotItem != NULL && m_hHandCursor)
	{
		::SetCursor(m_hHandCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}



void CXTPShortcutBar::OnMouseMove(UINT nFlags, CPoint point)
{
#ifdef _XTP_INCLUDE_COMMANDBARS
	if (XTPMouseManager()->IsTrackedLock(0))
		return;
#endif

	if (m_bTrackingSize)
	{
		CXTPClientRect rc(this);

		SetExpandedLinesHeight(rc.Height() - point.y);
	}
	else
	{
		CXTPShortcutBarItem* pItem = HitTest(point);
		if (pItem != m_pHotItem)
		{
			SetHotItem(pItem);
		}

	}

	CWnd::OnMouseMove(nFlags, point);
}


void CXTPShortcutBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcGripper.PtInRect(point) && m_bAllowResize)
	{
		m_bTrackingSize = TRUE;
		SetCapture();
		return;
	}

	CXTPShortcutBarItem* pItem = HitTest(point);

	if (pItem)
	{
		m_pPressedItem = pItem;
		RedrawControl();
		SetCapture();
		return;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPShortcutBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	m_bTrackingSize = FALSE;

	CXTPShortcutBarItem* pPressedItem = m_pPressedItem;
	m_pPressedItem = NULL;
	ReleaseCapture();


#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseUp(VK_LBUTTON, AxShiftState(), point.x, point.y);
#endif

	if (pPressedItem)
	{
		if (m_pHotItem == pPressedItem)
		{
			SelectItem(pPressedItem);
		}
		else
		{
			RedrawControl();
		}
	}
}

void CXTPShortcutBar::OnCaptureChanged(CWnd* pWnd)
{
	m_bTrackingSize = FALSE;

	if (m_pPressedItem)
	{
		m_pPressedItem = NULL;
		RedrawControl();
	}

	CWnd::OnCaptureChanged(pWnd);
}

void CXTPShortcutBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnMouseMove(nFlags, point);

	GetOwner()->SendMessage(XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_RCLICK, MAKELONG(point.x, point.y));

	GetCursorPos(&point);
	ScreenToClient(&point);
	OnMouseMove(nFlags, point);

	CWnd::OnRButtonDown(nFlags, point);
}

void CXTPShortcutBar::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseUp(VK_RBUTTON, AxShiftState(), point.x, point.y);
#endif
}


void CXTPShortcutBar::OnMouseLeave()
{
#ifdef _XTP_INCLUDE_COMMANDBARS
	if (!XTPMouseManager()->IsTrackedLock(0))
#endif
		SetHotItem(NULL);
}


void CXTPShortcutBar::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	m_pPaintManager->RefreshMetrics();
	Reposition();

}

CXTPShortcutBarItem* CXTPShortcutBar::FindItem(int nID) const
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->GetID() == nID)
			return m_arrItems[i];
	}
	return NULL;
}

void CXTPShortcutBar::LoadState(LPCTSTR lpszProfileName)
{
	m_nDesiredExpandedLinesHeight = AfxGetApp()->GetProfileInt(lpszProfileName, _T("DesiredExpandedLinesHeight"), m_nDesiredExpandedLinesHeight);

	CDWordArray arrHiddenItems;

	LPBYTE lpbData;
	UINT dwDataSize;
	if (AfxGetApp()->GetProfileBinary(lpszProfileName, _T("Hidden"), &lpbData, &dwDataSize))
	{
		try
		{
			CMemFile file(lpbData, dwDataSize);
			CArchive ar (&file, CArchive::load);
			arrHiddenItems.Serialize(ar);

			for (int i = 0; i < arrHiddenItems.GetSize(); i++)
			{
				CXTPShortcutBarItem* pItem = FindItem(arrHiddenItems[i]);
				if (pItem && !pItem->IsItemExpandButton()) pItem->m_bVisible = FALSE;
			}

		}
		catch (CMemoryException* pEx)
		{
			pEx->Delete ();
		}
		catch (CArchiveException* pEx)
		{
			pEx->Delete ();
		}

		delete[] lpbData;
	}

	Reposition();
}

void CXTPShortcutBar::SaveState(LPCTSTR lpszProfileName)
{
	AfxGetApp()->WriteProfileInt(lpszProfileName, _T("DesiredExpandedLinesHeight"), m_nDesiredExpandedLinesHeight);

	CDWordArray arrHiddenItems;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (!m_arrItems[i]->IsVisible() && !m_arrItems[i]->IsItemExpandButton())
			arrHiddenItems.Add(m_arrItems[i]->GetID());
	}

	try
	{
		CMemFile file;
		CArchive ar (&file, CArchive::store);
		arrHiddenItems.Serialize(ar);
		ar.Flush ();

		DWORD dwDataSize = (DWORD)file.GetPosition();
		LPBYTE lpbData = file.Detach();

		AfxGetApp()->WriteProfileBinary(lpszProfileName, _T("Hidden"), lpbData, dwDataSize);

		ar.Close();
		file.Close();
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
	}

}
int CXTPShortcutBar::GetItemCount() const
{
	return (int)m_arrItems.GetSize();
}

CXTPShortcutBarItem* CXTPShortcutBar::GetItem(int nIndex) const
{
	if (nIndex >= 0 && nIndex < m_arrItems.GetSize())
		return m_arrItems[nIndex];
	return NULL;
}

void CXTPShortcutBar::RemoveItem(int nID)
{
	m_pHotItem = NULL;
	m_pPressedItem = NULL;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (pItem->GetID() == nID)
		{
			if (pItem == m_pSelectedItem) m_pSelectedItem = NULL;

			m_arrItems.RemoveAt(i);
			pItem->InternalRelease();
			Reposition();
			return;
		}
	}
}
void CXTPShortcutBar::RemoveAllItems()
{
	m_pHotItem = NULL;
	m_pPressedItem = NULL;
	m_pSelectedItem = NULL;

	for (int i = (int)m_arrItems.GetSize() - 1; i >= 0 ; i--)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];

		if (!pItem->IsItemExpandButton())
		{
			m_arrItems.RemoveAt(i);
			pItem->InternalRelease();
		}
	}

	Reposition();
}

void CXTPShortcutBar::OnShowMoreButtons()
{
	SetExpandedLinesCount(m_nExpandedLines + 1);
}
void CXTPShortcutBar::OnShowFewerButtons()
{
	SetExpandedLinesCount(m_nExpandedLines - 1);
}

void CXTPShortcutBar::EnableAnimation(BOOL bEnable)
{
	m_pAnimation->m_bAnimation = bEnable;
	RedrawControl();
}

void CXTPShortcutBar::OnUpdateShowMoreButtons(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetVisibleItemsCount() != m_nExpandedLines);

}
void CXTPShortcutBar::OnUpdateShowFewerButtons(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nExpandedLines > 1);
}

CXTPToolTipContext* CXTPShortcutBar::GetToolTipContext() const
{
	return m_pToolTipContext;
}

BOOL CXTPShortcutBar::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_pToolTipContext)
	{
		m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

LRESULT CXTPShortcutBar::OnExpandPopupExecute(WPARAM wParam, LPARAM lParam)
{
#ifdef _XTP_INCLUDE_COMMANDBARS
	if (wParam == XTP_ID_SHORTCUT_SELECTED)
	{
		CXTPControl* pControl = ((NMXTPCONTROL*)lParam)->pControl;
		SelectItem((CXTPShortcutBarItem*)pControl->GetTag());
		return 1;
	}
	if (wParam == XTP_ID_SHORTCUT_CUSTOMIZE)
	{
		CXTPControl* pControl = ((NMXTPCONTROL*)lParam)->pControl;
		CXTPShortcutBarItem* pItem = (CXTPShortcutBarItem*)pControl->GetTag();

		pItem->m_bVisible = !pItem->m_bVisible;
		Reposition();


		return 1;
	}
#else
	wParam;
	lParam;
#endif
	return 0;
}

void CXTPShortcutBar::SetLayoutRTL(BOOL bRightToLeft)
{
	if (!XTPSystemVersion()->IsLayoutRTLSupported())
		return;

	if (!m_hWnd)
		return;

	ModifyStyleEx(bRightToLeft ? 0 : WS_EX_LAYOUTRTL, !bRightToLeft ? 0 : WS_EX_LAYOUTRTL);
	GetImageManager()->DrawReverted(bRightToLeft);

	Reposition();
}

void CXTPShortcutBar::OnMinimizeButtonClicked()
{
	SetFocus();
	GetOwner()->PostMessage(XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_MINIMIZEBUTTONCLICKED, 0);
}

void CXTPShortcutBar::OnSetFocus(CWnd* pWnd)
{
	CWnd::OnSetFocus(pWnd);

	if (m_pActivePopup)
	{
		m_pActivePopup->Close();
	}
}


#ifdef _XTP_ACTIVEX

// {F428621B-9CD0-4dbc-8BDD-91A73D4BB4F6}
static const GUID IID_ShortcutBarItem =
{ 0xf428621b, 0x9cd0, 0x4dbc, { 0x8b, 0xdd, 0x91, 0xa7, 0x3d, 0x4b, 0xb4, 0xf6 } };



BEGIN_INTERFACE_MAP(CXTPShortcutBarItem, CCmdTarget)
	INTERFACE_PART(CXTPShortcutBarItem, IID_ShortcutBarItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPShortcutBarItem, IID_ShortcutBarItem)

BEGIN_DISPATCH_MAP(CXTPShortcutBarItem, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "Caption", dispidCaption, OleGetCaption, SetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "Tooltip", dispidTooltip, OleGetTooltip, SetTooltip, VT_BSTR)

	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "Id", dispidId, GetID, SetID, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "Visible", dispidVisible, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "IconHandle", dispidIconHandle, OleGetIconHandle, OleSetIconHandle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "Hidden", 7, IsHidden, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "Selected", 8, IsSelected, SetSelected, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "hWnd", 9, OleGetHandle, OleSetHandle, VT_HANDLE)

	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "IconId", 10, GetIconId, SetIconId, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarItem, "Enabled", 11, IsEnabled, SetEnabled, VT_BOOL)

END_DISPATCH_MAP()



BSTR CXTPShortcutBarItem::OleGetCaption()
{
	return m_strCaption.AllocSysString();

}
BSTR CXTPShortcutBarItem::OleGetTooltip()
{
	return m_strTooltip.AllocSysString();
}
void CXTPShortcutBarItem::OleSetIconHandle(int nIcon)
{
	m_pShortcutBar->GetImageManager()->SetIcon((HICON)nIcon, m_nID);
	m_pShortcutBar->Reposition();
}
int CXTPShortcutBarItem::OleGetIconHandle()
{
	return 0;
}

OLE_HANDLE CXTPShortcutBarItem::OleGetHandle()
{
	return (OLE_HANDLE)m_hwndChild;
}

void CXTPShortcutBarItem::OleSetHandle(OLE_HANDLE hWnd)
{
	BOOL bRefresh = m_pShortcutBar->m_hwndClient == m_hwndChild && ::IsWindowVisible(m_hwndChild);

	m_hwndChild = (HWND)hWnd;

	if (bRefresh)
	{
		if (m_pShortcutBar->m_hwndClient)
		{
			::ShowWindow(m_pShortcutBar->m_hwndClient, SW_HIDE);
		}

		m_pShortcutBar->m_hwndClient = m_hwndChild;

		if (m_pShortcutBar->m_hwndClient  && !m_pShortcutBar->m_bMinimized)
		{
			::ShowWindow(m_pShortcutBar->m_hwndClient, SW_SHOW);
		}
		m_pShortcutBar->Reposition();
	}
}


#endif
