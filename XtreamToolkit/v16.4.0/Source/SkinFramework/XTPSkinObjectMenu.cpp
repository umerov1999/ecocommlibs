// XTPSkinObjectMDI.cpp: implementation of the CXTPSkinObjectMDI class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSynchro.h>

#include "XTPSkinObject.h"
#include "XTPSkinObjectFrame.h"
#include "XTPSkinObjectMenu.h"
#include "XTPSkinManager.h"
#include "XTPSkinManagerSchema.h"
#include "XTPSkinDrawTools.h"

#define TIMERID_MENUSHOW 0xACC
#define TIMERID_MENUHIDE 0xABB

#ifndef WM_UNINITMENUPOPUP
#define WM_MENURBUTTONUP                0x0122
#define WM_MENUDRAG                     0x0123
#define WM_MENUGETOBJECT                0x0124
#define WM_UNINITMENUPOPUP              0x0125
#define WM_MENUCOMMAND                  0x0126
#endif

#ifndef SPI_GETMENUSHOWDELAY
#define SPI_GETMENUSHOWDELAY       0x006A
#endif

#define TPM_SYSMENU 0x0200L

#ifndef HBMMENU_CALLBACK
	#define MIIM_STRING      0x00000040
	#define MIIM_BITMAP      0x00000080
	#define MIIM_FTYPE       0x00000100

	#define HBMMENU_CALLBACK            ((HBITMAP) -1)
	#define HBMMENU_SYSTEM              ((HBITMAP)  1)
	#define HBMMENU_MBAR_RESTORE        ((HBITMAP)  2)
	#define HBMMENU_MBAR_MINIMIZE       ((HBITMAP)  3)
	#define HBMMENU_MBAR_CLOSE          ((HBITMAP)  5)
	#define HBMMENU_MBAR_CLOSE_D        ((HBITMAP)  6)
	#define HBMMENU_MBAR_MINIMIZE_D     ((HBITMAP)  7)
	#define HBMMENU_POPUP_CLOSE         ((HBITMAP)  8)
	#define HBMMENU_POPUP_RESTORE       ((HBITMAP)  9)
	#define HBMMENU_POPUP_MAXIMIZE      ((HBITMAP) 10)
	#define HBMMENU_POPUP_MINIMIZE      ((HBITMAP) 11)
#endif

#define MENU_NOITEM (int)-1


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPSkinObjectApplicationFrame

IMPLEMENT_DYNCREATE(CXTPSkinObjectApplicationFrame, CXTPSkinObjectFrame)

CXTPSkinObjectApplicationFrame::CXTPSkinObjectApplicationFrame()
{
	m_rcMenuBar.SetRectEmpty();

	m_pPopupMenu = new CXTPSkinPopupMenu();
	m_pPopupMenu->m_bMenuBar = TRUE;
};



CXTPSkinObjectApplicationFrame::~CXTPSkinObjectApplicationFrame()
{
	if (m_pPopupMenu) m_pPopupMenu->InternalRelease();
};

BEGIN_MESSAGE_MAP(CXTPSkinObjectApplicationFrame, CXTPSkinObjectFrame)
	//{{AFX_MSG_MAP(CXTPSkinObjectMenu)
	ON_WM_NCCALCSIZE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTPSkinPopupMenu* CXTPSkinObjectApplicationFrame::LookUp(HMENU hMenu)
{
	CXTPSkinPopupMenu *pMenu = NULL;

	if ((NULL != m_pPopupMenu))
	{
		m_pPopupMenu->LookUp(hMenu);
	}

	return pMenu;
}

CXTPSkinPopupMenu* CXTPSkinPopupMenu::LookUp(HMENU hMenu)
{
	CXTPSkinPopupMenu *pMenu = NULL;

	if (hMenu == m_hMenu)
	{
		pMenu = this;
	}
	else
	{
		for (int nItem=0; nItem<m_arrItems.GetSize(); nItem++)
		{
			CXTPSkinPopupMenuItem *pItem = m_arrItems.GetAt(nItem);

			if (hMenu == pItem->GetPopupMenu())
			{
				pMenu = NULL;
			}
		}
	}


	return pMenu;
}

BOOL CXTPSkinObjectApplicationFrame::FrameHasMenuBar()
{
	if (GetStyle() & WS_CHILD)
		return FALSE;

	HMENU hMenu = ::GetMenu(m_hWnd);
	if (!hMenu)
		return FALSE;

	return GetMenuItemCount(hMenu) > 0;
}

void CXTPSkinObjectApplicationFrame::DrawFrame(CDC* pDC)
{
	if (m_bLockFrameDraw)
		return;

	if (!m_rcMenuBar.IsRectEmpty())
	{
		DrawFrameMenuBar(pDC);
	}

	CXTPSkinObjectFrame::DrawFrame(pDC);
}

void CXTPSkinObjectApplicationFrame::UpdateMenuBar()
{
	if (FrameHasMenuBar())
	{
		RebuildMenuItems();
	}
	else
	{
		if (m_pPopupMenu) m_pPopupMenu->m_hMenu = NULL;
		m_rcMenuBar.SetRectEmpty();
	}
}

void CXTPSkinObjectApplicationFrame::RedrawMenuBar()
{
	if (!m_rcMenuBar.IsRectEmpty())
	{
		CWindowDC dc(this);
		DrawFrameMenuBar(&dc);

		CXTPClientRect rect(this);
		if (rect.Height() <= 0)
			CXTPSkinObjectFrame::DrawFrame(&dc);
	}
}


void CXTPSkinObjectApplicationFrame::DrawFrameMenuBar(CDC* pDC)
{
	CXTPSkinManager* pSkinManager = XTPSkinManager();
	CXTPSkinManagerClass* pClassWindow = pSkinManager->GetSkinClass(this, _T("WINDOW"));

	CRect rc = m_rcMenuBar;
	CXTPBufferDC dc(*pDC, rc);

	dc.FillSolidRect(rc.left, rc.top, rc.Width(), rc.Height() - 1, GetColor(COLOR_3DFACE));
	dc.FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, GetColor(COLOR_WINDOW));

	CXTPFontDC font(&dc, &GetMetrics()->m_fntMenu);
	dc.SetBkMode(TRANSPARENT);

	int nCount = m_pPopupMenu->GetCount();


	for (int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		CXTPSkinPopupMenuItem* pItem = m_pPopupMenu->GetItem(nIndex);
		CRect rcItem = pItem->m_rcItem;
		BOOL bSelected = nIndex == m_pPopupMenu->m_nSelected;
		BOOL bEnabled = pItem->IsEnabled();

		HBITMAP hbm = pItem->GetItemBitmap();

		if (hbm == HBMMENU_SYSTEM)
		{
			HWND hwndItem = (HWND)pItem->GetItemData();
			if (hwndItem && IsWindow(hwndItem))
			{
				HICON hIcon = GetSchema()->GetFrameSmIcon(hwndItem, FALSE);

				if (hIcon)
				{
					rcItem = CRect(CPoint(rcItem.CenterPoint().x - 8, rcItem.CenterPoint().y - 8), CSize(16, 16));

					DrawIconEx(dc.m_hDC, rcItem.left, rcItem.top, hIcon,
						rcItem.Width(), rcItem.Height(), 0, NULL, DI_NORMAL);
				}
			}

			continue;
		}

		if (pItem->IsSeparator())
			continue;

		if (pItem->IsMDISysButton())
		{
			UINT nID = pItem->GetID();
			int nPartID = nID == SC_RESTORE ? WP_MDIRESTOREBUTTON : nID == SC_MINIMIZE ? WP_MDIMINBUTTON : WP_MDICLOSEBUTTON;
			int nState = !bEnabled ? SBS_DISABLED : bSelected ?  SBS_HOT : SBS_NORMAL;

			pClassWindow->DrawThemeBackground(&dc, nPartID, nState, &rcItem);

			continue;
		}

		CString strMenuText = m_pPopupMenu->GetMenuString(nIndex);

		if (bSelected)
		{
			dc.FillSolidRect(rcItem, GetColor(COLOR_HIGHLIGHT));
		}

		dc.SetTextColor(GetColor(!bEnabled ? COLOR_GRAYTEXT : bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));

		dc.DrawText(strMenuText, rcItem, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
}

void CXTPSkinObjectApplicationFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	CXTPSkinObjectFrame::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (FrameHasMenuBar())
	{
		m_rcMenuBar.SetRect(m_rcBorders.left, m_rcBorders.top, m_rcBorders.left + lpncsp[0].rgrc->right - lpncsp[0].rgrc->left, m_rcBorders.top);

		RebuildMenuItems();

		lpncsp[0].rgrc->top += m_rcMenuBar.Height();
	}
	else
	{
		if (m_pPopupMenu) m_pPopupMenu->m_hMenu = NULL;
		m_rcMenuBar.SetRectEmpty();
	}
}

BOOL CXTPSkinPopupMenuItem::IsMDISysButton() const
{
	int nID = GetID();
	if (nID == SC_CLOSE || nID == SC_RESTORE || nID == SC_MINIMIZE)
		return TRUE;

	return FALSE;
}

void CXTPSkinObjectApplicationFrame::RebuildMenuItems()
{
	m_pPopupMenu->RemoveAll();
	m_pPopupMenu->m_hMenu = NULL;

	if (GetStyle() & WS_CHILD)
		return;

	HMENU hMenu = ::GetMenu(m_hWnd);
	if (!hMenu)
		return;

	m_pPopupMenu->m_hMenu = hMenu;
	m_pPopupMenu->m_hWnd = m_hWnd;
	m_pPopupMenu->m_hWndNotify = m_hWnd;

	int nCount = ::GetMenuItemCount(hMenu);

	CWindowDC dc(this);

	CXTPFontDC font(&dc, &GetMetrics()->m_fntMenu);

	int nHeight = GetSystemMetrics(SM_CYMENUSIZE);

	//CRect rc = m_rcMenuBar;

	int x = m_rcMenuBar.left;
	int y = m_rcMenuBar.top;

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	int nIndex;

	for (nIndex = 0; nIndex < nCount; nIndex++ )
	{
		CXTPSkinPopupMenuItem* pItem = new CXTPSkinPopupMenuItem(m_pPopupMenu, nIndex);

		CSize sz(0, 0);

		HBITMAP hbm = pItem->GetItemBitmap();

		if ((UINT_PTR)hbm == 1)
		{
			sz.cx = 16 + 2 + 2;
			sz.cy = 16;
		}
		else if (pItem->IsSeparator())
		{
			sz.cx = 6;
		}
		else if (pItem->IsMDISysButton())
		{
			sz.cx = sz.cy = nHeight;
		}
		else
		{
			CString strMenuText = pItem->GetText();
			XTPDrawHelpers()->StripMnemonics(strMenuText);

			int nText = dc.GetTextExtent(strMenuText).cx;
			sz.cx = nText + (tm.tmAveCharWidth + 1) * 2;
			sz.cy = nHeight;
		}

		if (x + sz.cx > m_rcMenuBar.right && nIndex != 0)
		{
			y += nHeight;
			x = m_rcMenuBar.left;
		}

		CRect rcItem(x, y, x + sz.cx, y + nHeight);

		pItem->m_rcItem = rcItem;

		m_pPopupMenu->m_arrItems.Add(pItem);

		x = rcItem.right;
	}

	x = m_rcMenuBar.right;

	for (nIndex = nCount - 1; nIndex >= 0; nIndex--)
	{
		CXTPSkinPopupMenuItem* pItem = m_pPopupMenu->GetItem(nIndex);

		if ((pItem->GetType() & MFT_RIGHTJUSTIFY || pItem->IsMDISysButton()) && pItem->m_rcItem.top == y)
		{
			pItem->m_rcItem.OffsetRect(x - pItem->m_rcItem.right, 0);
			x -= pItem->m_rcItem.Width();
		}
		else
		{
			break;
		}
	}

	m_rcMenuBar.bottom = y + nHeight + 1;
}

int CXTPSkinObjectApplicationFrame::HitTestMenuItem(CPoint point)
{
	if (!m_rcMenuBar.PtInRect(point))
		return -1;

	for (int i = 0; i < m_pPopupMenu->GetCount(); i++)
	{
		if (::PtInRect(&m_pPopupMenu->GetItem(i)->m_rcItem, point))
			return i;
	}

	return -1;
}

BOOL CXTPSkinObjectApplicationFrame::HandleMouseMove(CPoint point)
{
	if (CXTPSkinObjectFrame::HandleMouseMove(point))
		return TRUE;

	if (m_rcMenuBar.IsRectEmpty())
		return FALSE;

	CPoint ptClient(point);
	ScreenToFrame(&ptClient);

	int nSelected = HitTestMenuItem(ptClient);

	if (nSelected != -1 && HandleNcHitTest(point) != HTMENU)
		nSelected = -1;

	if (m_pPopupMenu->m_nSelected != nSelected)
	{
		m_pPopupMenu->m_nSelected = nSelected;
		RedrawMenuBar();

		if (nSelected != -1)
			SetTimer (XTP_TID_MOUSELEAVE, 50, &CXTPSkinObjectFrame::OnTimerInternal);
		else
			CancelMouseLeaveTracking();

	}
	return TRUE;
}

LRESULT CXTPSkinObjectApplicationFrame::HandleNcHitTest(CPoint point)
{
	if (!m_rcMenuBar.IsRectEmpty())
	{
		CPoint ptMenu(point);
		ScreenToFrame(&ptMenu);

		CRect rcMenuBar(m_rcMenuBar);

		CXTPWindowRect rcWindow(this);
		rcWindow.OffsetRect(-rcWindow.TopLeft());
		rcWindow.DeflateRect(m_rcBorders);

		rcMenuBar.IntersectRect(rcMenuBar, rcWindow);
		rcMenuBar.top += 1;

		if (rcMenuBar.PtInRect(ptMenu))
			return HTMENU;
	}

	return CXTPSkinObjectFrame::HandleNcHitTest(point);
}

CXTPSkinPopupMenuState* CXTPSkinObjectApplicationFrame::StartMenuState(UINT /*nID*/, LPARAM /*lParam*/)
{
	if (m_pPopupMenu->m_hMenu == 0)
	{
		HMENU hMenu = ::GetSystemMenu(m_hWnd, FALSE);
		if (!hMenu)
			return NULL;

		::SetMenuDefaultItem(hMenu, SC_CLOSE, MF_BYCOMMAND);

		m_pPopupMenu->m_hMenu = hMenu;
		m_pPopupMenu->m_bSysMenu = TRUE;
		m_pPopupMenu->m_hWnd = m_hWnd;
		m_pPopupMenu->m_hWndNotify = m_hWnd;

		m_pPopupMenu->PositionSysMenu();
	}

	CXTPSkinPopupMenuState* pState = new CXTPSkinPopupMenuState(m_hWnd);
	pState->InitMenu(m_pPopupMenu);

	return pState;
}

void CXTPSkinPopupMenuState::PlayEventSound(UINT nID)
{
	XTPSoundManager()->PlaySystemSound((XTPSoundManagerState)nID);
}

void CXTPSkinPopupMenuState::EndState()
{
	if (m_pAlternatePopup)
	{
		if (!m_pAlternatePopup->IsSysMenuBar())
		{
			CXTPSkinPopupMenu* pAlternatePopup = m_pAlternatePopup;
			m_pAlternatePopup = m_pRootPopup;
			m_pRootPopup = pAlternatePopup;
		}

		m_pAlternatePopup->InternalRelease();
	}
	else if (m_pRootPopup && m_pRootPopup->m_bSysMenu)
	{
		m_pRootPopup->m_bSysMenu = FALSE;
		m_pRootPopup->m_hMenu = NULL;
		m_pRootPopup->m_hWnd = NULL;
	}

	if (m_pRootPopup)
	{
		m_pRootPopup->InternalRelease();
	}

	if (NULL != m_hWndCaptureOld)
	{
		::SetCapture(m_hWndCaptureOld);
	}

	delete this;
}

void CXTPSkinPopupMenuState::FilterMenuKey(LPARAM lParam)
{
#ifdef _XTP_ACTIVEX
	SAFE_MANAGE_STATE(XTPSkinManager()->m_pModuleState);
#endif


	if (m_bButtonDown)
		return;

	if (!StartMenu(focusKeyboard))
		return;

	m_bInsideMenuLoop = TRUE;

	switch (lParam)
	{
		case 0:
			SelectItem(m_pRootPopup, 0);
			RunLoop(lParam);
			return;

		case '-':
			if ((GetWindowLong(m_pRootPopup->m_hWndNotify, GWL_STYLE) & WS_CHILD) == 0)
			{
				break;
			}
			// else fall through.

		case ' ':
			if (!m_pAlternatePopup && !m_pRootPopup->m_bSysMenu)
			{
				Dismiss();
				return;
			}

			CloseHierarchy(m_pRootPopup);

			if (!m_pRootPopup->m_bSysMenu)
				SwitchToAlternateMenu();

			SelectItem(m_pRootPopup, 0);
			OpenHierarchy(m_pRootPopup);
			m_pRootPopup->m_bToggle = FALSE;
			RunLoop(lParam);
			return;

	}

	if (m_pRootPopup->m_bSysMenu && m_pRootPopup->m_pState == NULL)
	{
		// ALT + key
		OnChar(m_pRootPopup, (UINT)lParam);
	}
	else
	{
		OnChar(m_pRootPopup, (UINT)lParam);
	}
	if (m_pRootPopup->m_nSelected != -1)
	{
		RunLoop(lParam);
	}
	else
	{
		Dismiss();
	}
}

BOOL CXTPSkinObjectApplicationFrame::HandleSysCommand(UINT nID, LPARAM lParam)
{
	if ((GetSkinManager()->GetApplyOptions() & xtpSkinApplyMenus) == 0)
		return CXTPSkinObjectFrame::HandleSysCommand(nID, lParam);

	UINT nCmd = (nID & 0xFFF0);

	if (nCmd == SC_DEFAULT)
	{
		return TRUE;
	}
	if (nCmd == SC_MOUSEMENU)
	{
		CXTPSkinPopupMenuState* pState = StartMenuState(nID, lParam);
		if (pState)
		{
			pState->RunLoop(lParam);

			pState->EndState();
		}
		return TRUE;
	}

	if (nCmd == SC_KEYMENU)
	{
		CXTPSkinPopupMenuState* pState = StartMenuState(nID, lParam);
		if (pState)
		{
			pState->FilterMenuKey(lParam);

			pState->EndState();
		}
		else
		{
			CXTPSkinObjectFrame::HandleSysCommand(nID, lParam);
		}
		return TRUE;
	}

	return FALSE;
}



//////////////////////////////////////////////////////////////////////////
// CXTPSkinPopupMenuItem

CXTPSkinPopupMenuItem::CXTPSkinPopupMenuItem(CXTPSkinPopupMenu *pPopupParent, int nItem)
	: m_pPopupParent(pPopupParent)
	, m_pPopupChild (NULL)
	, m_hMenu       (pPopupParent->m_hMenu)
	, m_nItem       (nItem)
{
	m_rcItem.SetRectEmpty();
}

CRect CXTPSkinPopupMenuItem::GetScreenPos() const
{
	CRect rcItem(m_rcItem);

	CXTPSkinPopupMenuState::WindowToScreen(m_pPopupParent->m_hWnd, rcItem);

	return rcItem;
}

HMENU CXTPSkinPopupMenuItem::GetPopupMenu() const
{
	if (m_pPopupParent->IsSysMenuBar())
	{
		return m_hMenu;
	}

	return ::GetSubMenu(m_hMenu, m_nItem);
}

DWORD CXTPSkinPopupMenuItem::GetState() const
{
	if (m_pPopupParent->IsSysMenuBar())
		return MF_ENABLED;

	MENUITEMINFO mii;
	GetMenuItemInfo(MIIM_STATE, &mii);

	return mii.fState;
}

void CXTPSkinPopupMenuItem::GetMenuItemInfo(UINT fMask, LPMENUITEMINFO lpMII) const
{
	ZeroMemory(lpMII, sizeof(MENUITEMINFO));
	lpMII->cbSize = sizeof(MENUITEMINFO);
	lpMII->fMask = fMask;

	::GetMenuItemInfo(m_hMenu, m_nItem, TRUE, lpMII);
}

UINT CXTPSkinPopupMenuItem::GetID() const
{
	MENUITEMINFO mii;
	GetMenuItemInfo(MIIM_ID, &mii);

	return mii.wID;
}

BOOL CXTPSkinPopupMenuItem::IsEnabled() const
{
	return (GetState() & MFS_GRAYED) == 0;
}

BOOL CXTPSkinPopupMenuItem::IsChecked() const
{
	return (GetState() & MFS_CHECKED);
}

BOOL CXTPSkinPopupMenuItem::IsDefault() const
{
	return (GetState() & MFS_DEFAULT);
}


DWORD_PTR CXTPSkinPopupMenuItem::GetItemData() const
{
	MENUITEMINFO mii;
	GetMenuItemInfo(MIIM_DATA, &mii);

	return mii.dwItemData;
}

CString CXTPSkinPopupMenuItem::GetText() const
{
	CString strText;
	::GetMenuString(m_hMenu, m_nItem, strText.GetBuffer(256), 256, MF_BYPOSITION);
	strText.ReleaseBuffer();
	return strText;
}

BOOL CXTPSkinPopupMenuItem::IsSeparator() const
{
	MENUITEMINFO mii;
	GetMenuItemInfo(MIIM_TYPE, &mii);

	return mii.fType & MFT_SEPARATOR;
}

BOOL CXTPSkinPopupMenuItem::IsOwnerDraw() const
{
	MENUITEMINFO mii;
	GetMenuItemInfo(MIIM_TYPE, &mii);

	return mii.fType & MFT_OWNERDRAW;
}

UINT CXTPSkinPopupMenuItem::GetType() const
{
	MENUITEMINFO mii;
	GetMenuItemInfo(MIIM_TYPE, &mii);

	return mii.fType;
}

BOOL CXTPSkinPopupMenuItem::IsMenuBreak() const
{
	return (GetType() & (MF_MENUBREAK | MF_MENUBARBREAK));
}

#ifndef MIIM_BITMAP
#define MIIM_BITMAP      0x00000080
#endif

HBITMAP CXTPSkinPopupMenuItem::GetItemBitmap() const
{
	struct MENUITEMINFO98
	{
		UINT    cbSize;
		UINT    fMask;
		UINT    fType;          // used if MIIM_TYPE (4.0) or MIIM_FTYPE (>4.0)
		UINT    fState;         // used if MIIM_STATE
		UINT    wID;            // used if MIIM_ID
		HMENU   hSubMenu;       // used if MIIM_SUBMENU
		HBITMAP hbmpChecked;    // used if MIIM_CHECKMARKS
		HBITMAP hbmpUnchecked;  // used if MIIM_CHECKMARKS
		DWORD   dwItemData;     // used if MIIM_DATA
		LPWSTR  dwTypeData;     // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
		UINT    cch;            // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
		HBITMAP hbmpItem;       // used if MIIM_BITMAP
	};

	if (XTPSystemVersion()->IsWin95() || XTPSystemVersion()->IsWinNT4())
		return NULL;

	MENUITEMINFO98 mii;
	ZeroMemory(&mii, sizeof(MENUITEMINFO98));
	mii.cbSize = sizeof(MENUITEMINFO98);
	mii.fMask = MIIM_BITMAP;

	::GetMenuItemInfo(m_hMenu, m_nItem, TRUE, (MENUITEMINFO*)&mii);

	return mii.hbmpItem;
}



//////////////////////////////////////////////////////////////////////////
// CXTPSkinPopupMenu

#define XTP_SKINPOPUPMENU_SCROLL 22


CXTPSkinPopupMenu::CXTPSkinPopupMenu()
	: m_rcBorder(3,3,3,3)
{
	m_bMenuBar = FALSE;
	m_bSendUninit = FALSE;
	m_bSysMenu = FALSE;
	m_hWnd = 0;
	m_nSelected = -1;
	m_nDropped = -1;
	m_hMenu = NULL;
	m_pNextPopup = NULL;
	m_pPrevPopup = NULL;
	m_bToggle = FALSE;
	m_bDropNextPopup = FALSE;
	m_hWndNotify = 0;
	m_bAboutToHide = FALSE;
	m_bDroppedLeft = FALSE;

	m_pState = NULL;

	m_nHideTimer = 0;
	m_nShowTimer = 0;

	m_nGripperWidth = 0;

	m_scrollInfo.Init(this, XTP_TID_HOVERUP, XTP_TID_HOVERDOWN);
}

CXTPSkinPopupMenu::~CXTPSkinPopupMenu()
{
	m_hWnd = NULL;
	RemoveAll();

	m_scrollInfo.btnUp.KillTimer();
	m_scrollInfo.btnDown.KillTimer();
}

void CXTPSkinPopupMenu::OnFinalRelease()
{
	delete this;
}

void CXTPSkinPopupMenu::RemoveAll()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		delete m_arrItems[i];
	}
	m_arrItems.RemoveAll();
}

CString CXTPSkinPopupMenu::GetMenuString(int nIndex)
{
	CString strMenuText;

	if (m_hMenu)
	{
		::GetMenuString(m_hMenu, nIndex, strMenuText.GetBuffer(256), 256, MF_BYPOSITION);
		strMenuText.ReleaseBuffer();
	}
	return strMenuText;
}


void CXTPSkinPopupMenu::RebuildItems()
{
	RemoveAll();

	int nCount = !IsSysMenuBar() ? GetMenuItemCount(m_hMenu) : 1;

	for (int i = 0; i < nCount; i++)
	{
		m_arrItems.Add(new CXTPSkinPopupMenuItem(this, i));
	}
}


int CXTPSkinPopupMenu::GetCount()
{
	int nCount = !IsSysMenuBar() ? GetMenuItemCount(m_hMenu) : 1;
	if (nCount != m_arrItems.GetSize())
	{
		RebuildItems();
	}

	return nCount;
}

CXTPSkinPopupMenuItem* CXTPSkinPopupMenu::GetItem(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrItems.GetSize() ? m_arrItems.GetAt(nIndex) : NULL;
}

BEGIN_MESSAGE_MAP(CXTPSkinPopupMenu, CWnd)
	//{{AFX_MSG_MAP(CXTPSkinObjectMenu)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPSkinPopupMenu::IsMenuThemed()
{
	CXTPSkinManager* pSkinManager = XTPSkinManager();
	CXTPSkinManagerClass* pClass = pSkinManager->GetSkinClass(pSkinManager->Lookup(m_hWndNotify), _T("MENU"));

	BOOL bTheme = pClass->GetThemeInt(XTP_MP_POPUPBACKGROUND, 0, TMT_BORDERSIZE, 0) != 0;
	if (!bTheme)
		return FALSE;

	int nCount = ::GetMenuItemCount(m_hMenu);

	for (int i = 0; i < nCount; i++)
	{
		MENUITEMINFO mii;
		ZeroMemory(&mii, sizeof(MENUITEMINFO));
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_TYPE;
		::GetMenuItemInfo(m_hMenu, i, TRUE, &mii);

		if (mii.fType & MFT_OWNERDRAW)
			return FALSE;
	}

	return TRUE;

}

void CXTPSkinPopupMenu::PositionSysMenu()
{
	RebuildItems();

	ASSERT(GetCount() == 1);

	CXTPSkinPopupMenuItem* pItem = GetItem(0);

	HWND hWnd = m_hWndNotify;
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	CRect rcBorders = XTPSkinManager()->GetSchema()->CalcFrameBorders(dwStyle, dwExStyle);

	pItem->m_rcItem.SetRect(rcBorders.left, rcBorders.left, rcBorders.top, rcBorders.top);
}


void CXTPSkinPopupMenu::RecalcItemsSize(CDC *pDC)
{
	CXTPSkinManagerMetrics *pMetrics = m_pState->m_pSchema->GetMetrics();
	CXTPFontDC font(pDC, &pMetrics->m_fntMenu);

	static const int MENUBREAK_SPACING = 4;

	m_nGripperWidth = IsMenuThemed() ? 33 : 17;

	INT_PTR nItem;
	INT_PTR nCount = m_arrItems.GetSize();

	int x = m_rcBorder.left;
	int y = m_rcBorder.top;

	m_nMaxWidthColumn = 0;
	int nFirstColumn = 0;
	int nMaxHeight = 0;

	for (nItem = 0; nItem<nCount; nItem++ )
	{
		CXTPSkinPopupMenuItem *pItem = m_arrItems[nItem];

		///////////////////////////////////////////////////////////////////////
		// Calculate gripper width
		///////////////////////////////////////////////////////////////////////

		MENUITEMINFO mii;
		ZeroMemory(&mii, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask  = MIIM_STATE | MIIM_ID;

		::GetMenuItemInfo(m_hMenu, (UINT)nItem, TRUE, &mii);

		if (!pItem->IsOwnerDraw() && !pItem->IsSeparator())
		{
			HBITMAP hbm = pItem->GetItemBitmap();

			if (hbm && HIWORD(hbm) != 0)
			{
				BITMAP bmpInfo;
				::GetObject(hbm, sizeof(BITMAP), &bmpInfo);

				m_nGripperWidth = max(m_nGripperWidth, bmpInfo.bmWidth + 4 + (IsMenuThemed() ? 5 + 3 : 0));
			}
		}

		///////////////////////////////////////////////////////////////////////

		CString strMenuText = pItem->GetText();

		CSize szText(0, 0);

		if (pItem->IsOwnerDraw())
		{
			MEASUREITEMSTRUCT mis;
			mis.CtlType    = ODT_MENU;
			mis.CtlID      = 0;
			mis.itemID     = pItem->GetID();
			mis.itemWidth  = 0;
			mis.itemHeight = (UINT) pDC->GetTextExtent(_T(" "), 1).cy;
			mis.itemData   = pItem->GetItemData();

			::SendMessage(m_hWndNotify, WM_MEASUREITEM, 0, (LPARAM)&mis);

			szText.cx = mis.itemWidth + 12;
			szText.cy = mis.itemHeight;

		}
		else if (pItem->IsSeparator())
		{
			szText.cy = IsMenuThemed() ? 6 : 9;
		}
		else
		{
			BOOL bDefault = pItem->IsDefault();
			CFont fontDefault;

			if (bDefault)
			{
				LOGFONT lf;
				pMetrics->m_fntMenu.GetLogFont(&lf);
				lf.lfWeight  = FW_BOLD;
				fontDefault.CreateFontIndirect(&lf);
				font.SetFont(&fontDefault);
			}

			int nIndex = strMenuText.Find(_T('\t'));

			CSize szShortcut(0, 0);

			if (nIndex != -1)
			{
				CString strShortcutText = strMenuText.Mid(nIndex);
				strMenuText.ReleaseBuffer(nIndex);

				szShortcut = pDC->GetTextExtent(strShortcutText);
			}

			szText = pDC->GetTextExtent(strMenuText);

			if (IsMenuThemed())
			{
				szText.cx = m_nGripperWidth + szText.cx + szShortcut.cx + 55;
				szText.cy = max(szText.cy, 22);
			}
			else
			{
				szText.cx = m_nGripperWidth + szText.cx + szShortcut.cx + 44;
				szText.cy = max(szText.cy, 17);
			}

			if (bDefault)
			{
				font.SetFont(&pMetrics->m_fntMenu);
			}
		}

		// Menu break
		if (pItem->IsMenuBreak() && (nItem != nFirstColumn))
		{
			for (; nFirstColumn < nItem; nFirstColumn++ )
			{
				CXTPSkinPopupMenuItem* pItem = m_arrItems[nFirstColumn];
				pItem->m_rcItem.right = pItem->m_rcItem.left + m_nMaxWidthColumn;
			}

			x += m_nMaxWidthColumn + MENUBREAK_SPACING;
			nMaxHeight = max(nMaxHeight, y);
			y = m_rcBorder.top;
			m_nMaxWidthColumn = 0;
		}

		if (!pItem->IsOwnerDraw() && !pItem->IsSeparator())
		{
			HBITMAP hbm = pItem->GetItemBitmap();

			if (hbm && HIWORD(hbm) != 0)
			{
				BITMAP bmpInfo;
				::GetObject(hbm, sizeof(BITMAP), &bmpInfo);

				szText.cy = max(szText.cy, bmpInfo.bmHeight + 4);
			}
		}


		CRect rcItem(x, y, x + szText.cx, y + szText.cy);
		m_nMaxWidthColumn = max(m_nMaxWidthColumn, szText.cx);

		pItem->m_rcItem = rcItem;

		y = rcItem.bottom;
	} // for

	// Adjust width for all items
	for (nItem = nFirstColumn; nItem < nCount; nItem++ )
	{
		CXTPSkinPopupMenuItem *pItem = m_arrItems[nItem];
		pItem->m_rcItem.right = pItem->m_rcItem.left + m_nMaxWidthColumn;
	}

	font.ReleaseFont();
}

int CXTPSkinPopupMenu::GetColumnCount() const
{
	int nCount = 1;

	for (int nItem=0; nItem<m_arrItems.GetSize(); nItem++)
	{
		const CXTPSkinPopupMenuItem *pItem = m_arrItems[nItem];

		if (pItem->IsMenuBreak())
		{
			nCount++;
		}
	}

	return nCount;
}


int CXTPSkinPopupMenu::RecalcItemsHeight(int nScreenArea)
{
	int nColumnHeightMax = 0; // Maximum column height
	int nColumnHeight    = 0; // Current column height
	int nColumnArea      = nScreenArea;

	for (int nItem=0; nItem<m_arrItems.GetSize(); nItem++)
	{
		const CXTPSkinPopupMenuItem *pItem = m_arrItems[nItem];
		const int nHeight = pItem->m_rcItem.Height();

		if (pItem->IsMenuBreak())
		{
			nColumnHeightMax = max(nColumnHeightMax, nColumnHeight);
			nColumnHeight    = 0;
			nColumnArea      = nScreenArea;
		}

		if (nColumnArea > nHeight)
		{
			nColumnHeight += nHeight;
		}

		nColumnArea -= nHeight;
	}

	nColumnHeightMax = max(nColumnHeightMax, nColumnHeight);

	return nColumnHeightMax;
}



CSize CXTPSkinPopupMenu::RecalcLayout()
{
	ASSERT(m_pState);
	ASSERT(!m_bMenuBar);

	//CXTPSkinManagerSchema *pSchema = m_pState->m_pSchema;

	CDC dc;
	dc.Attach(::GetWindowDC(m_hWnd));

	RebuildItems();
	RecalcItemsSize(&dc);

	::ReleaseDC(m_hWnd, dc.Detach());

	LONG nBorderHor = m_rcBorder.left + m_rcBorder.right;
	LONG nBorderVer = m_rcBorder.top  + m_rcBorder.bottom;


	int nWorkArea = XTPMultiMonitor()->GetWorkArea().Height();
	m_nHeight = RecalcItemsHeight(INT_MAX);

	if ((m_nHeight > nWorkArea) && !IsMultiColumn())
	{
		m_nHeight = RecalcItemsHeight(nWorkArea-XTP_SKINPOPUPMENU_SCROLL+nBorderVer) + XTP_SKINPOPUPMENU_SCROLL+nBorderVer;

		m_scrollInfo.bScroll = TRUE;

		/*
		*/
		ShowScrollDown(TRUE);
	}

	m_scrollInfo.nScrollFirst = 0;
	m_scrollInfo.nScrollLast  = GetCount() - 1;

	return CSize(
		nBorderHor + (GetColumnCount() * m_nMaxWidthColumn) + ((GetColumnCount()-1)*4),
		nBorderVer + m_nHeight);
}

void CXTPSkinPopupMenu::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint);

	if (!m_pState)
		return;

	CXTPSkinManager* pSkinManager = XTPSkinManager();
	CXTPSkinManagerMetrics* pMetrics = m_pState->m_pSchema->GetMetrics();

	CXTPClientRect rcClient(this);

	CXTPSkinManagerClass* pClass = pSkinManager->GetSkinClass(pSkinManager->Lookup(m_hWndNotify), _T("MENU"));

	BOOL bTheme = IsMenuThemed();

	if (bTheme)
	{
		CRect rcBackground(rcClient);
		pClass->DrawThemeBackground(&dc, XTP_MP_POPUPBORDERS, 0, &rcBackground);
		rcBackground.DeflateRect(3, 3);
		pClass->DrawThemeBackground(&dc, XTP_MP_POPUPBACKGROUND, 0, &rcBackground);

		rcBackground.right = rcBackground.left + m_nGripperWidth - 5;
		pClass->DrawThemeBackground(&dc, XTP_MP_POPUPGUTTER, 0, &rcBackground);

	}
	else
	{
		dc.FillSolidRect(rcClient, pMetrics->GetColor(COLOR_MENU));
		dc.Draw3dRect(rcClient, pMetrics->GetColor(COLOR_3DSHADOW), pMetrics->GetColor(COLOR_3DSHADOW));
	}

	CXTPFontDC font(&dc, &pMetrics->m_fntMenu);
	dc.SetBkMode(TRANSPARENT);



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	LONG nClipTop    = rcClient.top;
	LONG nClipBottom = rcClient.bottom;

	if (m_scrollInfo.bScroll)
	{
		if (!m_scrollInfo.btnUp.m_rc.IsRectEmpty())
		{
			CPoint pt = m_scrollInfo.btnUp.m_rc.CenterPoint();
			XTPDrawHelpers()->Triangle(&dc, CPoint(pt.x - 3, pt.y), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y - 3), pMetrics->GetColor(COLOR_MENUTEXT));
			nClipTop = m_scrollInfo.btnUp.m_rc.bottom;
		}

		if (!m_scrollInfo.btnDown.m_rc.IsRectEmpty())
		{
			CPoint pt = m_scrollInfo.btnDown.m_rc.CenterPoint();
			XTPDrawHelpers()->Triangle(&dc, CPoint(pt.x - 3, pt.y), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y + 3), pMetrics->GetColor(COLOR_MENUTEXT));
			nClipBottom = m_scrollInfo.btnDown.m_rc.top;
		}
	}



	int nCount = GetCount();

	for (int nIndex=m_scrollInfo.nScrollFirst; nIndex < nCount; nIndex++)
	{
		const CXTPSkinPopupMenuItem *pItem = GetItem(nIndex);
		ASSERT(NULL != pItem);

		MENUITEMINFO mii;
		ZeroMemory(&mii, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask  = MIIM_STATE | MIIM_CHECKMARKS | MIIM_ID;

		::GetMenuItemInfo(m_hMenu, nIndex, TRUE, &mii);

		BOOL bSeparator = pItem->IsSeparator();
		BOOL bEnabled = pItem->IsEnabled();
		BOOL bSelected = (nIndex == m_nSelected) && !bSeparator;


		CRect rcItem(pItem->m_rcItem);

		if (rcItem.top < nClipTop)
			continue;

		if (rcItem.bottom > nClipBottom)
			break;

		m_scrollInfo.nScrollLast = nIndex;

		if ((pItem->GetType() & MF_MENUBARBREAK) && (nIndex != 0) && !bTheme)
		{
			dc.FillSolidRect(rcItem.left - 3, rcClient.top + 3, 1, rcClient.Height() - 6, pMetrics->GetColor(COLOR_3DSHADOW));
			dc.FillSolidRect(rcItem.left - 2, rcClient.top + 3, 1, rcClient.Height() - 6, pMetrics->GetColor(COLOR_3DHIGHLIGHT));
		}

		if (pItem->IsOwnerDraw())
		{
			dc.SetTextColor(pMetrics->GetColor(COLOR_MENUTEXT));
			dc.SetBkColor  (pMetrics->GetColor(COLOR_MENU));

			DWORD dwState = mii.fState;

			DRAWITEMSTRUCT dis;
			dis.CtlType = ODT_MENU;
			dis.CtlID = 0;
			dis.itemID = mii.wID;
			dis.itemAction = ODA_DRAWENTIRE;
			dis.itemState   =
				((dwState & MF_GRAYED)       ? ODS_GRAYED    : 0) |
				((dwState & MFS_DEFAULT)     ? ODS_DEFAULT   : 0) |
				((dwState & MFS_CHECKED)     ? ODS_CHECKED   : 0) |
				((dwState & MFS_DISABLED)    ? ODS_DISABLED  : 0) |
				(bSelected                   ? ODS_SELECTED  : 0);

			dis.hwndItem = (HWND)m_hMenu;
			dis.hDC      = dc.GetSafeHdc();
			dis.rcItem   = pItem->m_rcItem;
			dis.itemData = pItem->GetItemData();

			if (m_hWndNotify)
			{
				::SendMessage(m_hWndNotify, WM_DRAWITEM, 0, (LPARAM)&dis);
			}

			if (pItem->GetPopupMenu())
			{
				CPoint ptCenter(rcItem.right - (bTheme ? 10 : 5), rcItem.CenterPoint().y);
				XTPDrawHelpers()->Triangle(&dc, ptCenter, CPoint(ptCenter.x - 3, ptCenter.y - 3),
					CPoint(ptCenter.x - 3, ptCenter.y + 3), pMetrics->GetColor(!bEnabled ? COLOR_GRAYTEXT : bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));
			}
			continue;
		}
		if (bSeparator)
		{
			if (!bTheme)
			{
				dc.FillSolidRect(rcItem.left + 1, rcItem.CenterPoint().y, rcItem.Width() - 2, 1, pMetrics->GetColor(COLOR_3DSHADOW));
				dc.FillSolidRect(rcItem.left + 1, rcItem.CenterPoint().y + 1, rcItem.Width() - 2, 1, pMetrics->GetColor(COLOR_3DHIGHLIGHT));
			}
			else
			{
				CRect rcSeparator(rcItem.left + m_nGripperWidth - 5, rcItem.top, rcItem.right, rcItem.bottom);
				pClass->DrawThemeBackground(&dc, XTP_MP_POPUPSEPARATOR, 0, &rcSeparator);

			}
			continue;
		}

		if (bTheme)
		{
			if (bSelected)
			{
				pClass->DrawThemeBackground(&dc, XTP_MP_POPUPITEM, 2, &rcItem);
			}

			dc.SetTextColor(pMetrics->GetColor(!bEnabled ? COLOR_GRAYTEXT : COLOR_MENUTEXT));
		}
		else
		{
			if (bSelected)
			{
				dc.FillSolidRect(rcItem, pMetrics->GetColor(COLOR_HIGHLIGHT));
			}

			dc.SetTextColor(pMetrics->GetColor(!bEnabled ? COLOR_GRAYTEXT : bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));
		}

		BOOL bDefault = pItem->IsDefault();
		CFont fontDefault;

		if (bDefault)
		{
			LOGFONT lf;
			pMetrics->m_fntMenu.GetLogFont(&lf);
			lf.lfWeight  = FW_BOLD;
			fontDefault.CreateFontIndirect(&lf);
			font.SetFont(&fontDefault);
		}

		CString strMenuText = pItem->GetText();
		int nFind = strMenuText.Find(_T('\t'));

		CSize szShortcut(0, 0);

		if (nFind != -1)
		{
			CString strShortcutText = strMenuText.Mid(nFind + 1);
			strMenuText.ReleaseBuffer(nFind);

			CRect rcShortcut(rcItem);
			rcShortcut.right -= 20;

			dc.DrawText(strShortcutText, rcShortcut, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
		}

		CRect rcText(rcItem);
		rcText.left += m_nGripperWidth;

		dc.DrawText(strMenuText, rcText, DT_VCENTER | DT_LEFT | DT_SINGLELINE);

		if (pItem->GetPopupMenu())
		{
			if (bTheme)
			{
				CRect rcPopup(rcText.right - 16, rcText.top, rcText.right - 7, rcText.bottom);
				pClass->DrawThemeBackground(&dc, XTP_MP_POPUPSUBMENU, bEnabled ? 1 : 2, &rcPopup);
			}
			else
			{
				CPoint ptCenter(rcText.right - (bTheme ? 10 : 5), rcText.CenterPoint().y);
				XTPDrawHelpers()->Triangle(&dc, ptCenter, CPoint(ptCenter.x - 3, ptCenter.y - 3),
					CPoint(ptCenter.x - 3, ptCenter.y + 3), dc.GetTextColor());
			}
		}

		if (bDefault)
		{
			font.SetFont(&pMetrics->m_fntMenu);
		}

		BOOL bChecked = pItem->IsChecked();

		HBITMAP hbm = bChecked ? mii.hbmpChecked : mii.hbmpUnchecked;

		if (hbm)
		{
			BITMAP bmpInfo;
			::GetObject(hbm, sizeof(BITMAP), &bmpInfo);

			CXTPCompatibleDC dcBitmap(&dcPaint, hbm);

			dc.SetTextColor(0);
			dc.SetBkColor(0xFFFFFF);

			dc.BitBlt((bTheme ? 16 : 11) -  bmpInfo.bmWidth / 2 , (rcItem.top + rcItem.bottom - bmpInfo.bmHeight) / 2, bmpInfo.bmWidth, bmpInfo.bmHeight,
				&dcBitmap, 0, 0, SRCAND);
		}
		else if (bChecked)
		{
			if (bTheme)
			{
				CRect rcCheck(rcItem.left, rcItem.top, rcItem.left + 22, rcItem.bottom);

				pClass->DrawThemeBackground(&dc, XTP_MP_POPUPCHECKBACKGROUND, bEnabled ? 2 : 1, rcCheck);
				pClass->DrawThemeBackground(&dc, XTP_MP_POPUPCHECK,           bEnabled ? 1 : 2, rcCheck);
			}
			else
			{
				CXTPPenDC pen (dc, dc.GetTextColor());
				CXTPBrushDC brush (dc, dc.GetTextColor());

				CPoint pt(rcItem.left + 5, rcItem.CenterPoint().y);
				CPoint pts[] = {pt, CPoint(pt.x + 2, pt.y + 2), CPoint(pt.x + 6, pt.y - 2),
					CPoint(pt.x + 6, pt.y), CPoint(pt.x + 2, pt.y + 4), CPoint(pt.x, pt.y + 2)};
				dc.Polygon(pts, _countof(pts));
			}
		}
		else
		{
			hbm = pItem->GetItemBitmap();

			int nSysBitmap = 0;

			switch ((UINT_PTR)hbm)
			{
				case (UINT_PTR)HBMMENU_POPUP_RESTORE:  nSysBitmap = XTP_MP_SYSTEMRESTORE;  break;
				case (UINT_PTR)HBMMENU_POPUP_MINIMIZE: nSysBitmap = XTP_MP_SYSTEMMINIMIZE; break;
				case (UINT_PTR)HBMMENU_POPUP_MAXIMIZE: nSysBitmap = XTP_MP_SYSTEMMAXIMIZE; break;
				case (UINT_PTR)HBMMENU_POPUP_CLOSE:    nSysBitmap = XTP_MP_SYSTEMCLOSE;    break;
			}

			if (nSysBitmap != 0)
			{
				if (bTheme)
				{
					CSize sz(20, 20);
					CRect rcBitmap(CPoint((bTheme ? 16 : 11) -  sz.cx / 2 , (rcItem.top + rcItem.bottom - sz.cy) / 2), sz);

					pClass->DrawThemeBackground(&dc, nSysBitmap, bEnabled ? 1 : 2, rcBitmap);
				}
				else
				{
					CSize sz(20, 20);
					CRect rcBitmap(CPoint((bTheme ? 16 : 11) -  sz.cx / 2 , (rcItem.top + rcItem.bottom - sz.cy) / 2 + 1), sz);

					CFont fontMarlett;
					fontMarlett.CreatePointFont(MulDiv(80, 96, dc.GetDeviceCaps(LOGPIXELSX)), _T("Marlett"));                   CFont* pFont = dc.SelectObject(&fontMarlett);
					dc.DrawText(nSysBitmap == XTP_MP_SYSTEMMINIMIZE ? _T("0") :
						nSysBitmap == XTP_MP_SYSTEMRESTORE ?  _T("2") :
						nSysBitmap == XTP_MP_SYSTEMMAXIMIZE ?  _T("1") :_T("r"),
						1, rcBitmap, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					dc.SelectObject(pFont);
				}
			}
			else if (hbm)
			{
				BITMAP bmpInfo;
				::GetObject(hbm, sizeof(BITMAP), &bmpInfo);

				int nCenter = 3 + (m_nGripperWidth - (bTheme ? 3 + 5 : 0)) / 2;

				if (bmpInfo.bmBitsPixel == 32 && bmpInfo.bmBits && XTPSystemVersion()->IsWinVistaOrGreater())
				{
					CXTPImageManagerIcon::DrawAlphaBitmap(&dc, hbm, CPoint(nCenter -  bmpInfo.bmWidth / 2 , (rcItem.top + rcItem.bottom - bmpInfo.bmHeight) / 2),
						CSize(bmpInfo.bmWidth, bmpInfo.bmHeight));
				}
				else
				{
					CXTPCompatibleDC dcBitmap(&dcPaint, hbm);

					dc.SetTextColor(0);
					dc.SetBkColor(0xFFFFFF);

					dc.BitBlt(nCenter -  bmpInfo.bmWidth / 2 , (rcItem.top + rcItem.bottom - bmpInfo.bmHeight) / 2, bmpInfo.bmWidth, bmpInfo.bmHeight,
						&dcBitmap, 0, 0, SRCAND);
				}
			}
		}
	}

}

void CXTPSkinPopupMenu::EnsureVisible(int nItem)
{
	if ((MENU_NOITEM == nItem) || (0 == m_scrollInfo.nScrollLast))
		return;

	int nScrollFirst = m_scrollInfo.nScrollFirst;
	int nScrollLast  = m_scrollInfo.nScrollLast;

	if (nItem < nScrollFirst)       // Scroll up
	{
		for (int nCount=nScrollFirst-nItem; nCount>0; nCount--)
		{
			ScrollUp();
		}
	}

	if (nItem > nScrollLast)        // Scroll down
	{
		for (int nCount=nItem-nScrollLast; nCount>0; nCount--)
		{
			ScrollDown();
		}
	}
}

void CXTPSkinPopupMenu::OnScrollUp()
{
	if (m_scrollInfo.nScrollFirst > 0)
	{
		ScrollUp();
		Invalidate();
	}
}

void CXTPSkinPopupMenu::OnScrollDown()
{
	if (m_scrollInfo.nScrollLast < GetCount()-1)
	{
		ScrollDown();
		Invalidate();
	}
}

void CXTPSkinPopupMenu::ScrollUp()
{
	const CXTPSkinPopupMenuItem *pItem = GetItem(m_scrollInfo.nScrollFirst-1);
	Scroll(+pItem->m_rcItem.Height());
	m_scrollInfo.nScrollFirst--; // Scroll one item up

	if (0 == m_scrollInfo.nScrollFirst)
	{
		ShowScrollUp(FALSE);
	}

	if (m_scrollInfo.btnDown.m_rc.IsRectEmpty())
	{
		ShowScrollDown(TRUE);
	}
}

void CXTPSkinPopupMenu::ScrollDown()
{
	const CXTPSkinPopupMenuItem *pItem = GetItem(m_scrollInfo.nScrollFirst);
	Scroll(-pItem->m_rcItem.Height());
	m_scrollInfo.nScrollFirst++; // Scroll one item down

	pItem = GetItem(GetCount()-1); // Get last item
	if (pItem->m_rcItem.bottom <= m_scrollInfo.btnDown.m_rc.bottom)
	{
		ShowScrollDown(FALSE);
	}

	if (m_scrollInfo.btnUp.m_rc.IsRectEmpty())
	{
		ShowScrollUp(TRUE);
		ScrollDown();
	}
}

void CXTPSkinPopupMenu::Scroll(int nOffsetY)
{
	for (int nItem=0; nItem<GetCount(); nItem++)
	{
		CXTPSkinPopupMenuItem *pItem = GetItem(nItem);

		pItem->m_rcItem.OffsetRect(0, nOffsetY);
	}
}

void CXTPSkinPopupMenu::ShowScrollUp(BOOL bShow)
{
	if (bShow && m_scrollInfo.btnUp.m_rc.IsRectEmpty())
	{
		Scroll(XTP_SKINPOPUPMENU_SCROLL);
		m_scrollInfo.btnUp.m_rc.left   = m_rcBorder.left;
		m_scrollInfo.btnUp.m_rc.top    = m_rcBorder.top;
		m_scrollInfo.btnUp.m_rc.right  = m_nMaxWidthColumn;
		m_scrollInfo.btnUp.m_rc.bottom = m_rcBorder.top + XTP_SKINPOPUPMENU_SCROLL;
	}

	if (!bShow && !m_scrollInfo.btnUp.m_rc.IsRectEmpty())
	{
		Scroll(-XTP_SKINPOPUPMENU_SCROLL);
		m_scrollInfo.btnUp.m_rc.SetRectEmpty();
	}
}

void CXTPSkinPopupMenu::ShowScrollDown(BOOL bShow)
{
	if (bShow && m_scrollInfo.btnDown.m_rc.IsRectEmpty())
	{
		m_scrollInfo.btnDown.m_rc.left   = 0;
		m_scrollInfo.btnDown.m_rc.top    = m_nHeight - m_rcBorder.bottom - XTP_SKINPOPUPMENU_SCROLL;
		m_scrollInfo.btnDown.m_rc.right  = m_nMaxWidthColumn;
		m_scrollInfo.btnDown.m_rc.bottom = m_nHeight - m_rcBorder.bottom;
	}

	if (!bShow && !m_scrollInfo.btnDown.m_rc.IsRectEmpty())
	{
		m_scrollInfo.btnDown.m_rc.SetRectEmpty();
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPSkinPopupMenuState

CXTPSkinPopupMenuState::CXTPSkinPopupMenuState(HWND hWndNotify)
{
	m_bInsideMenuLoop = FALSE;
	m_bButtonDown = 0;
	m_nFocus = focusMouse;
	m_hWndCapture = 0;
	m_bDismiss = FALSE;
	m_bNofyByPos = FALSE;
	m_pRootPopup = NULL;
	m_ptMouseLast = CPoint(0);
	m_nLastCommand = 0;

	m_bFirstClick = FALSE;
	m_bMenuStarted = FALSE;
	m_pAlternatePopup = NULL;

	CXTPSkinObject* pSkinObject = XTPSkinManager()->Lookup(hWndNotify);
	m_pSchema = pSkinObject ? pSkinObject->GetSchema() : XTPSkinManager()->GetSchema();

	m_dwMenuShowDelay = 400;
	SystemParametersInfo(SPI_GETMENUSHOWDELAY, 0, &m_dwMenuShowDelay, 0);
}

void CXTPSkinPopupMenuState::InitMenu(CXTPSkinPopupMenu* pPopupMenu)
{
	m_pRootPopup = pPopupMenu;
	m_pRootPopup->InternalAddRef();

	m_bButtonDown = ((GetKeyState(VK_LBUTTON) & 0x8000) != 0);

	pPopupMenu->m_bDropNextPopup = FALSE;

	if (m_pRootPopup->m_hWndNotify && !m_flags.bNoNotify)
	{
		SendMessage(m_pRootPopup->m_hWndNotify, WM_ENTERMENULOOP, 0, 0);
	}
}

#ifndef MNS_NOTIFYBYPOS
#define MNS_NOTIFYBYPOS     0x08000000
#define MIM_STYLE 0x00000010
#endif

int CXTPSkinPopupMenuState::GetMenuFlags(HMENU hMenu)
{
	struct XTP_MENUINFO
	{
		DWORD   cbSize;
		DWORD   fMask;
		DWORD   dwStyle;
		UINT    cyMax;
		HBRUSH  hbrBack;
		DWORD   dwContextHelpID;
		ULONG_PTR dwMenuData;
	};

	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (!hLib)
		return 0;

	typedef BOOL (WINAPI *PFNGETMENUINFO) (HMENU hmenu, XTP_MENUINFO* lpcmi);

	PFNGETMENUINFO pfnGetMenuInfo = (PFNGETMENUINFO)::GetProcAddress(hLib, "GetMenuInfo");
	if (!pfnGetMenuInfo)
		return 0;

	XTP_MENUINFO mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_STYLE;

	if (!(*pfnGetMenuInfo)(hMenu, &mi))
		return FALSE;

	return mi.dwStyle;
}

BOOL CXTPSkinPopupMenuState::StartMenu(MenuFocus nFocus)
{
	m_bMenuStarted = TRUE;

	m_nFocus = nFocus;

	m_pRootPopup->m_bToggle = FALSE;
	m_pRootPopup->m_bAboutToHide = FALSE;

	HWND hWndNotify = m_pRootPopup->m_hWndNotify;

	// Capture mouse
	m_hWndCapture    = hWndNotify;
	m_hWndCaptureOld = ::SetCapture(m_hWndCapture);

	::SendMessage(hWndNotify, WM_SETCURSOR, (WPARAM)hWndNotify, MAKELONG(MSGF_MENU, 0));

	if (m_pRootPopup->m_bMenuBar)
	{
		// TODO GetSysMenu from Child xxxGetInitMenuParam
		BOOL bSystemMenu = m_pRootPopup->m_bSysMenu;

		if (!bSystemMenu)
		{
			HMENU hMenu = ::GetSystemMenu(hWndNotify, FALSE);
			if (hMenu)
			{
				::SetMenuDefaultItem(hMenu, SC_CLOSE, MF_BYCOMMAND);
				m_pAlternatePopup = new CXTPSkinPopupMenu();
				m_pAlternatePopup->m_hMenu = hMenu;
				m_pAlternatePopup->m_hWnd = hWndNotify;
				m_pAlternatePopup->m_hWndNotify = hWndNotify;
				m_pAlternatePopup->m_bSysMenu = TRUE;
				m_pAlternatePopup->m_bMenuBar = TRUE;
				m_pAlternatePopup->PositionSysMenu();
				m_pAlternatePopup->m_pState = this;
			}
		}
	}

	int nFlags = GetMenuFlags(m_pRootPopup->m_hMenu);

	if (nFlags & MNS_NOTIFYBYPOS)
		m_bNofyByPos = TRUE;

	if (!m_flags.bNoNotify && hWndNotify)
	{
		::SendMessage(hWndNotify, WM_INITMENU, (WPARAM)m_pRootPopup->m_hMenu, 0);
	}

	// TODO: Add EVENT_SYSTEM_MENUSTART WinEvent.

	return TRUE;
}

int CXTPSkinPopupMenuState::ItemHitTest(CPoint point, CXTPSkinPopupMenu* pPopupMenu)
{
	for (int i = 0; i < pPopupMenu->GetCount(); i++)
	{
		CXTPSkinPopupMenuItem* pItem = pPopupMenu->m_arrItems.GetAt(i);

		CRect rcItem(pItem->m_rcItem);

		WindowToScreen(pPopupMenu->m_hWnd, rcItem);

		if (rcItem.PtInRect(point))
			return i;
	}
	return MENU_NOITEM;
}

void CXTPSkinPopupMenuState::WindowToScreen(HWND hWnd, CRect& rcItem)
{
	CXTPWindowRect rcWindow(hWnd);

	if (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL )
	{
		rcItem.OffsetRect(rcWindow.right - rcItem.right - rcItem.left, rcWindow.top);
	}
	else
	{
		rcItem.OffsetRect(rcWindow.TopLeft());
	}
}

CXTPSkinPopupMenu* CXTPSkinPopupMenuState::FindMenu(CPoint point, CXTPSkinPopupMenu* pPopupMenu, int* pnItem)
{
	*pnItem = 0;

	if (pPopupMenu->m_pNextPopup)
	{
		CXTPSkinPopupMenu* lResult = FindMenu(point, pPopupMenu->m_pNextPopup, pnItem);
		if (lResult)
		{
			return lResult;
		}
	}

	if (pPopupMenu->m_bMenuBar)
	{
		HWND hWnd = pPopupMenu->m_hWndNotify;

		if (pPopupMenu->m_bSysMenu)
		{
			HICON hIcon = m_pSchema->GetFrameSmIcon(hWnd);
			if (!hIcon)
				return NULL;

			CXTPWindowRect rcWindow(hWnd);

			if (GetWindowLong(hWnd, GWL_STYLE) & WS_MINIMIZE)
			{
				if (rcWindow.PtInRect(point))
					return pPopupMenu;
				return NULL;
			}

			CRect rcIcon(pPopupMenu->GetItem(0)->m_rcItem);
			WindowToScreen(hWnd, rcIcon);

			if (rcIcon.PtInRect(point))
			{
				*pnItem = 0;
				return pPopupMenu;
			}

			if (m_pAlternatePopup && m_pAlternatePopup != pPopupMenu)
			{
				int nHitTest = CXTPSkinPopupMenuState::ItemHitTest(point, m_pAlternatePopup);
				if (nHitTest != MENU_NOITEM)
				{
					*pnItem = nHitTest;
					return m_pAlternatePopup;
				}
			}

			return NULL;
		}
		else
		{
			if (GetWindowLong(hWnd, GWL_STYLE) & WS_MINIMIZE)
				return NULL;
		}

	}
	else
	{
		CXTPWindowRect rc(pPopupMenu->m_hWnd);
		if (!rc.PtInRect(point))
			return NULL;
	}

	int nHitTest = CXTPSkinPopupMenuState::ItemHitTest(point, pPopupMenu);

	if (pPopupMenu->m_bMenuBar)
	{
		if (nHitTest == MENU_NOITEM)
		{
			if (m_pAlternatePopup && m_pAlternatePopup != pPopupMenu)
			{
				CXTPSkinPopupMenu* pAlternatePopup = FindMenu(point, m_pAlternatePopup, pnItem);

				if (pAlternatePopup)
					return pAlternatePopup;
			}
			return NULL;
		}
	}

	*pnItem = nHitTest;
	return pPopupMenu;
}

BOOL CXTPSkinPopupMenuState::SetTimerToCloseHierarchy(CXTPSkinPopupMenu* pPopupMenu)
{
	CXTPSkinPopupMenu* pNextPopup = GetNextPopup(pPopupMenu);

	if (!pNextPopup)
		return FALSE;

	if (pPopupMenu->m_nHideTimer)
		return TRUE;

	if (!SetTimer(pPopupMenu->m_hWnd, TIMERID_MENUHIDE, m_dwMenuShowDelay, NULL))
		return FALSE;

	pPopupMenu->m_nHideTimer = TIMERID_MENUHIDE;

	pNextPopup->m_bAboutToHide = TRUE;

	return TRUE;
}

BOOL CXTPSkinPopupMenuState::SetTimerToOpenHierarchy(CXTPSkinPopupMenu* pPopupMenu)
{
	ASSERT(!pPopupMenu->m_bMenuBar);

	CXTPSkinPopupMenuItem* pItem = pPopupMenu->GetItem(pPopupMenu->m_nSelected);
	if (!pItem)
		return FALSE;

	if (pPopupMenu->m_nShowTimer || pPopupMenu->m_nSelected == pPopupMenu->m_nDropped)
		return 1;

	if (!SetTimer(pPopupMenu->m_hWnd, TIMERID_MENUSHOW, m_dwMenuShowDelay, NULL))
		return FALSE;

	pPopupMenu->m_nShowTimer = TIMERID_MENUSHOW;

	return TRUE;
}

void CXTPSkinPopupMenuState::RedrawPopup(CXTPSkinPopupMenu* pPopupMenu)
{
	if (pPopupMenu->m_bMenuBar) ::SendMessage(pPopupMenu->m_hWnd, WM_NCPAINT, 0, 0); else ::InvalidateRect(pPopupMenu->m_hWnd, 0, FALSE);
}

void CXTPSkinPopupMenuState::SendMenuSelect(CXTPSkinPopupMenu* pPopupMenu)
{
	UINT dwFlags = 0;
	UINT nCommand = 0;
	int nIndex = -1;

	CXTPSkinPopupMenuItem* pItem = pPopupMenu->GetItem(pPopupMenu->m_nSelected);
	if (pItem)
	{
		dwFlags = pItem->GetType() | pItem->GetState();
		nIndex = pItem->m_nItem;

		if (pItem->GetPopupMenu())
			dwFlags |= MF_POPUP;

		if (dwFlags & MF_POPUP)
			nCommand = nIndex;
		else
			nCommand = pItem->GetID();

		if (m_nFocus == focusMouse)
			dwFlags |= MF_MOUSESELECT;

		if (pPopupMenu->m_bSysMenu)
			dwFlags |= MF_SYSMENU;

	}

	::SendMessage(pPopupMenu->m_hWndNotify, WM_MENUSELECT, (DWORD)MAKELONG(nCommand, dwFlags), (LPARAM)pPopupMenu->m_hMenu);

}

CXTPSkinPopupMenuItem* CXTPSkinPopupMenuState::SelectItem(CXTPSkinPopupMenu* pPopupMenu, int nItem)
{
	if (pPopupMenu->m_nSelected == nItem)
	{
		return pPopupMenu->GetItem(nItem);
	}

	if (pPopupMenu->m_nShowTimer)
	{
		KillTimer(pPopupMenu->m_hWnd, pPopupMenu->m_nShowTimer);
		pPopupMenu->m_nShowTimer = 0;
	}

	if (pPopupMenu->m_bAboutToHide && pPopupMenu->m_pPrevPopup)
	{
		CXTPSkinPopupMenu* pPrevPopup = pPopupMenu->m_pPrevPopup;

		if (pPrevPopup->m_nHideTimer)
		{
			::KillTimer(pPrevPopup->m_hWnd, pPrevPopup->m_nHideTimer);
			pPrevPopup->m_nHideTimer = NULL;
		}

		if (pPrevPopup->m_nShowTimer)
		{
			::KillTimer(pPrevPopup->m_hWnd, pPrevPopup->m_nShowTimer);
			pPrevPopup->m_nShowTimer = NULL;
		}

		if (pPrevPopup->m_nSelected != pPrevPopup->m_nDropped)
		{
			pPrevPopup->m_nSelected = pPrevPopup->m_nDropped;
			RedrawPopup(pPrevPopup);
			SendMenuSelect(pPrevPopup);
		}

		pPopupMenu->m_bAboutToHide = FALSE;
	}

	if (pPopupMenu->m_nSelected != MENU_NOITEM)
	{
		if (GetNextPopup(pPopupMenu))
		{
			if (pPopupMenu->m_bMenuBar)
			{
				CloseHierarchy(pPopupMenu);
			}
			else
			{
				SetTimerToCloseHierarchy(pPopupMenu);
			}
		}
	}

	pPopupMenu->m_nSelected = nItem;
	pPopupMenu->EnsureVisible(nItem);
	RedrawPopup(pPopupMenu);

	SendMenuSelect(pPopupMenu);

	return pPopupMenu->GetItem(nItem);
}

CPoint CXTPSkinPopupMenuState::PositionHierarchy(CXTPSkinPopupMenu* pSubMenu, CXTPSkinPopupMenu* pPopupMenu, CRect rcItem, CSize sz)
{
	CPoint pt(pPopupMenu->m_bMenuBar ? rcItem.left : rcItem.right - 3, pPopupMenu->m_bMenuBar ? rcItem.bottom : rcItem.top - 3);
	CRect rcMonitor(XTPMultiMonitor()->GetScreenArea(pt));

	if (pPopupMenu->m_bMenuBar)
	{
		BOOL bMinimized = (GetWindowLong(pPopupMenu->m_hWnd, GWL_STYLE) & WS_MINIMIZE) != 0;

		if (bMinimized)
		{
			pt.y = rcItem.top - sz.cy;
			if (pt.y < rcMonitor.top) pt.y = rcItem.bottom;
		}

		if (GetWindowLong(pPopupMenu->m_hWnd, GWL_EXSTYLE) & (WS_EX_LAYOUTRTL | WS_EX_RIGHT))
		{
			pt.x = rcItem.right - sz.cx;
			pSubMenu->m_bDroppedLeft = TRUE;
		}
		pt.x = min(pt.x, rcMonitor.right - sz.cx);
	}
	else
	{
		if (pPopupMenu->m_bDroppedLeft)
		{
			int x = rcItem.left + 3 - sz.cx;
			if (x >= rcMonitor.left)
			{
				pt.x = x;
				pSubMenu->m_bDroppedLeft = TRUE;
			}
		}

		if (pt.x + sz.cx > rcMonitor.right)
		{
			pt.x = rcItem.left + 3 - sz.cx;
			pSubMenu->m_bDroppedLeft = TRUE;
		}
	}

	if (pt.y + sz.cy > rcMonitor.bottom)
	{
		pt.y -= sz.cy;

		if (pPopupMenu->m_bMenuBar)
		{
			pt.y -= GetSystemMetrics(SM_CYMENUSIZE);
		}
		else
		{
			pt.y += rcItem.Height() + 3;
		}

		if ((pt.y < rcMonitor.top) || (pt.y + sz.cy > rcMonitor.bottom))
			pt.y = rcMonitor.bottom - sz.cy;
	}

	pt.x = max(pt.x, rcMonitor.left);
	pt.y = max(pt.y, rcMonitor.top);

	CRect rcPopup(pt.x, pt.y, pt.x + sz.cx, pt.y + sz.cy);
	rcPopup.DeflateRect(3, 3);

	if (CRect().IntersectRect(rcItem, rcPopup))
	{
		if ((rcItem.right + sz.cx) <= rcMonitor.right)
			pt.x = rcItem.right;
		else if ((rcItem.left - sz.cx) >= rcMonitor.left)
			pt.x = rcItem.left - sz.cx;
	}



	return pt;
}

HWND CXTPSkinPopupMenuState::OpenHierarchy(CXTPSkinPopupMenu* pPopupMenu)
{
	if (pPopupMenu->m_nSelected < 0 || pPopupMenu->m_nSelected >= pPopupMenu->GetCount())
		return NULL;

	if (GetNextPopup(pPopupMenu))
	{
		if (pPopupMenu->m_nHideTimer)
		{
			CloseHierarchy(pPopupMenu);
		}
		else
		{
			return NULL;
		}
	}

	if (pPopupMenu->m_nShowTimer)
	{
		KillTimer(pPopupMenu->m_hWnd, pPopupMenu->m_nShowTimer);
		pPopupMenu->m_nShowTimer = 0;
	}

	CXTPSkinPopupMenuItem* pItem = pPopupMenu->GetItem(pPopupMenu->m_nSelected);

	HMENU hSubMenu = pItem->GetPopupMenu();
	if (!hSubMenu)
		return NULL;

	BOOL bSendNotify = FALSE;

	HWND hwndResult = NULL;

	TRY
	{
		if (pPopupMenu->m_hWndNotify && !m_flags.bNoNotify)
		{
			SendMessage(pPopupMenu->m_hWndNotify, WM_INITMENUPOPUP, (WPARAM)hSubMenu, MAKELONG(pPopupMenu->m_nSelected, pPopupMenu->m_bSysMenu ? 1 : 0));

			bSendNotify = TRUE;
		}

		if (!m_bInsideMenuLoop)
			AfxThrowMemoryException();

		if (pPopupMenu->m_nSelected < 0 || pPopupMenu->m_nSelected >= pPopupMenu->GetCount())
			AfxThrowMemoryException();

		pItem = pPopupMenu->GetItem(pPopupMenu->m_nSelected);

		if (!pItem->IsEnabled() || !pItem->GetPopupMenu() || GetMenuItemCount(pItem->GetPopupMenu()) == 0)
			AfxThrowMemoryException();



		pItem->m_pPopupChild = new CXTPSkinPopupMenu();
		pItem->m_pPopupChild->m_pState = this;

		if (!pItem->m_pPopupChild->Create(pPopupMenu->m_hWndNotify))
		{
			pItem->m_pPopupChild->InternalRelease();
			AfxThrowMemoryException();
		}

		pItem->m_pPopupChild->m_hMenu      = hSubMenu;
		pItem->m_pPopupChild->m_hWndNotify = pPopupMenu->m_hWndNotify;
		pItem->m_pPopupChild->m_bSysMenu   = pPopupMenu->m_bSysMenu;
		pItem->m_pPopupChild->m_bSendUninit = TRUE;

		bSendNotify = FALSE;

		pPopupMenu->m_pNextPopup = pItem->m_pPopupChild;
		pItem->m_pPopupChild->m_pPrevPopup = pPopupMenu;

		if (pPopupMenu->m_bMenuBar)
		{
			pPopupMenu->m_bDropNextPopup = TRUE;
		}

		if (pPopupMenu->m_hWndNotify)
		{
			UpdateWindow(pPopupMenu->m_hWndNotify);
		}

		CSize size = pItem->m_pPopupChild->RecalcLayout();

		CRect rcWorkArea = XTPMultiMonitor()->GetWorkArea();

		CRect rcItem = pItem->GetScreenPos();

		CPoint pt = PositionHierarchy(pItem->m_pPopupChild, pPopupMenu, rcItem, size);

		PlayEventSound(xtpSoundMenuPopup);

		::SetWindowPos(pItem->m_pPopupChild->m_hWnd, HWND_TOPMOST, pt.x, pt.y, size.cx, size.cy, SWP_SHOWWINDOW | SWP_NOOWNERZORDER | SWP_NOACTIVATE);

		if (m_nFocus == focusKeyboard)
		{
			SelectItem(pItem->m_pPopupChild, 0);
		}

		pPopupMenu->m_nDropped = pPopupMenu->m_nSelected;

		UpdateWindow(pItem->m_pPopupChild->m_hWnd);

		hwndResult = pItem->m_pPopupChild->m_hWnd;

	}
	CATCH (CMemoryException, e)
	{
	}
	END_CATCH

	if (bSendNotify)
	{
		SendMessage(pPopupMenu->m_hWndNotify, WM_UNINITMENUPOPUP, (WPARAM)hSubMenu, MAKELONG(0, pPopupMenu->m_bSysMenu ? 1 : 0));
	}


	return hwndResult;
}

void CXTPSkinPopupMenuState::CloseHierarchy(CXTPSkinPopupMenu* pPopupMenu)
{
	if (pPopupMenu->m_nHideTimer)
	{
		::KillTimer(pPopupMenu->m_hWnd, pPopupMenu->m_nHideTimer);
		pPopupMenu->m_nHideTimer = 0;
	}

	CXTPSkinPopupMenu* pNextPopup = GetNextPopup(pPopupMenu);

	if (pNextPopup)
	{
		CloseHierarchy(pNextPopup);

		if (!pNextPopup->m_bMenuBar)
		{
			if (pNextPopup->m_bSendUninit && !m_flags.bNoNotify)
			{
				SendMessage(pNextPopup->m_hWndNotify, WM_UNINITMENUPOPUP,
					(WPARAM)pNextPopup->m_hMenu, MAKELONG(0, pNextPopup->m_bSysMenu ? MF_SYSMENU: 0));
			}

			pNextPopup->DestroyWindow();
			pNextPopup->InternalRelease();
		}

		pPopupMenu->m_pNextPopup = NULL;
		pPopupMenu->m_nDropped = MENU_NOITEM;
	}

	if (pPopupMenu->m_nSelected != MENU_NOITEM)
	{
		SendMenuSelect(pPopupMenu);
	}

}


CXTPSkinPopupMenu* CXTPSkinPopupMenuState::GetNextPopup(CXTPSkinPopupMenu* pPopupMenu) const
{
	return pPopupMenu->m_pNextPopup;
}

CXTPSkinPopupMenu* CXTPSkinPopupMenuState::GetActivePopup() const
{
	CXTPSkinPopupMenu* pActivePopup = m_pRootPopup;

	while (pActivePopup->m_pNextPopup != NULL)
	{
		pActivePopup = pActivePopup->m_pNextPopup;
	}

	return pActivePopup;
}

BOOL CXTPSkinPopupMenuState::HideNextHierarchy(CXTPSkinPopupMenu* pPopupMenu)
{
	CXTPSkinPopupMenu* pNextPopup = GetNextPopup(pPopupMenu);
	if (pNextPopup)
	{
		if (GetNextPopup(pPopupMenu) != GetActivePopup())
			CloseHierarchy(pNextPopup);

		SelectItem(pNextPopup, -1);
		return TRUE;
	}

	return FALSE;
}

void CXTPSkinPopupMenuState::DismissNotify(CXTPSkinPopupMenu* pPopupMenu, int nItem)
{
	DismissNotify(pPopupMenu->m_hMenu, nItem);
}

void CXTPSkinPopupMenuState::DismissNotify(HMENU hMenu, int nItem)
{
	if (nItem < 0 || nItem >= GetMenuItemCount(hMenu))
		return;

	UINT nMessage = 0;
	UINT nCommand = 0;
	LPARAM lParam = 0;

	if (m_pRootPopup->m_bSysMenu)
	{
		nMessage = WM_SYSCOMMAND;
		nCommand = ::GetMenuItemID(hMenu, nItem);
	}
	else if (m_bNofyByPos)
	{
		nMessage = WM_MENUCOMMAND;
		nCommand = nItem;
		lParam = (LPARAM)hMenu;
	}
	else
	{
		nMessage = WM_COMMAND;
		nCommand =  ::GetMenuItemID(hMenu, nItem);
	}

	Notify(nMessage, nCommand, lParam);
}

void CXTPSkinPopupMenuState::OnButtonUp(CXTPSkinPopupMenu* pPopupMenu, int nItem)
{
	if (!m_bButtonDown)
		return;

	if (nItem == MENU_NOITEM)
		return;

	if (nItem != pPopupMenu->m_nSelected)
		return;

	if (pPopupMenu->m_bMenuBar)
	{
		if (GetNextPopup(pPopupMenu))
		{
			if (!pPopupMenu->m_bToggle)
				return;

			pPopupMenu->m_bToggle = FALSE;
			Dismiss();
			return;
		}

	}
	else if (pPopupMenu->m_nShowTimer)
	{
		pPopupMenu->m_bToggle = FALSE;
		OpenHierarchy(pPopupMenu);
		return;
	}

	CXTPSkinPopupMenuItem* pItem = pPopupMenu->GetItem(pPopupMenu->m_nSelected);
	if (!pItem)
		return;

	if (!pItem->IsSeparator() && pItem->IsEnabled() && !pItem->GetPopupMenu())
	{
		DismissNotify(pPopupMenu, pPopupMenu->m_nSelected);
	}
}

void CXTPSkinPopupMenuState::OnButtonDblClick(CXTPSkinPopupMenu* pPopupMenu, int nItem)
{
	CXTPSkinPopupMenuItem* pItem = pPopupMenu->GetItem(nItem);
	if (!pItem)
		return;

	if (!pItem->IsEnabled())
		return;

	HMENU hMenu = pItem->GetPopupMenu();
	if (hMenu)
	{
		nItem = GetMenuDefaultItem(hMenu, MF_BYPOSITION, 0);
		if (nItem == -1)
			return;
	}
	else
	{
		hMenu = pPopupMenu->m_hMenu;
	}

	DismissNotify(hMenu, nItem);
}

void CXTPSkinPopupMenuState::OnButtonDown(CXTPSkinPopupMenu* pPopupMenu, int nItem, BOOL bClick)
{
	if (pPopupMenu->m_nSelected != nItem)
	{
		BOOL bOpenPopup = FALSE;

		if (bClick)
		{
			bOpenPopup = TRUE;
			pPopupMenu->m_bToggle = FALSE;

		}
		else
		{
			bOpenPopup = pPopupMenu->m_bDropNextPopup;
		}

		CXTPSkinPopupMenuItem* pItem = SelectItem(pPopupMenu, nItem);

		if (pItem && pItem->GetPopupMenu() && bOpenPopup)
		{
			if (!OpenHierarchy(pPopupMenu))
				return;
		}
	}
	else
	{
		if (bClick) pPopupMenu->m_bToggle = TRUE;

		if (!HideNextHierarchy(pPopupMenu) && bClick && OpenHierarchy(pPopupMenu))
			pPopupMenu->m_bToggle = FALSE;
	}

	if (bClick)
	{
		m_bButtonDown = TRUE;
	}
}

void CXTPSkinPopupMenuState::Dismiss()
{
	Notify(0, 0, 0);
}

void CXTPSkinPopupMenuState::Notify(UINT nMessage, UINT nCommand, LPARAM lParam)
{
	if (m_bDismiss)
		return;

	m_bDismiss = TRUE;
	m_bInsideMenuLoop = FALSE;
	m_bButtonDown = FALSE;

	CloseHierarchy(m_pRootPopup);
	SelectItem(m_pRootPopup, MENU_NOITEM);

	ReleaseCapture();

	HWND hWndNotify = m_pRootPopup->m_hWndNotify;

	if (hWndNotify == 0)
		return;

	if (!m_pRootPopup->m_bMenuBar)
	{
		CloseHierarchy(m_pRootPopup);
		m_pRootPopup->DestroyWindow();
	}

	::SendMessage(hWndNotify, WM_MENUSELECT, (DWORD)MAKELONG(0, 0xFFFF), 0); // Send WM_MENUSELECT with flags = 0xFFFF

	if (!m_flags.bNoNotify)
	{
		SendMessage(hWndNotify, WM_EXITMENULOOP, 0, 0);
	}

	m_nLastCommand = nCommand;

	if (nMessage != 0)
	{
		PlayEventSound(xtpSoundMenuCommand);

		if (!m_flags.bReturnCmd)
		{
			if (nMessage == WM_COMMAND)
				PostMessage(hWndNotify, nMessage, nCommand, lParam);
			else
				SendMessage(hWndNotify, nMessage, nCommand, lParam);

			if (nMessage == WM_SYSCOMMAND && IsWindow(hWndNotify))
			{
				::SendMessage(hWndNotify, WM_NCPAINT, 0, 0);
			}
		}
	}
}

void CXTPSkinPopupMenuState::OnMouseMove(CPoint point)
{
   if (point == m_ptMouseLast)
		return;

	m_ptMouseLast = point;


	int nItem;
	CXTPSkinPopupMenu* pMenuHit = FindMenu(point, m_pRootPopup, &nItem);

	if (m_nFocus == focusKeyboard)
	{
		if (pMenuHit == NULL)
			return;

		m_nFocus = focusMouse;
	}

	if (pMenuHit == m_pAlternatePopup)
	{
		if (m_bButtonDown)
		{
			SwitchToAlternateMenu();
		}
		else
		{
			pMenuHit = NULL;
		}

	}

	if (pMenuHit)
	{
		if (pMenuHit->m_scrollInfo.bScroll)
		{
			CPoint ptClient = point;
			pMenuHit->ScreenToClient(&ptClient);

			if (pMenuHit->m_scrollInfo.btnUp.OnMouseMove(ptClient) ||
				 pMenuHit->m_scrollInfo.btnDown.OnMouseMove(ptClient))
			{
				return;
			}
		}

		if (pMenuHit->m_bMenuBar)
		{
			OnButtonDown(pMenuHit, nItem, FALSE);
		}
		else
		{
			CXTPSkinPopupMenuItem* pItem = SelectItem(pMenuHit, nItem);
			if (pItem && pItem->GetPopupMenu() && pItem->IsEnabled())
			{
				if (!SetTimerToOpenHierarchy(pMenuHit))
				{
					HideNextHierarchy(pMenuHit);
				}
			}
		}
	}
	else
	{
		SelectItem(GetActivePopup(), MENU_NOITEM);
	}

}

CXTPSkinPopupMenu* CXTPSkinPopupMenuState::FindPopup(HWND hWnd) const
{
	CXTPSkinPopupMenu* pPopupMenu = m_pRootPopup;
	while (pPopupMenu)
	{
		if (pPopupMenu->m_hWnd == hWnd)
			return pPopupMenu;

		pPopupMenu = GetNextPopup(pPopupMenu);
	}

	return NULL;
}

BOOL CXTPSkinPopupMenuState::RemoveMessage(UINT nMessage)
{
	MSG msg;
	if (!PeekMessage(&msg, NULL, 0, 0, PM_NOYIELD | PM_NOREMOVE))
	{
		return FALSE;
	}

	if (msg.message == nMessage)
	{
		PeekMessage(&msg, NULL, msg.message, msg.message, PM_REMOVE);
		return TRUE;
	}

	return FALSE;
}

int CXTPSkinPopupMenuState::FindNextItem(CXTPSkinPopupMenu* pPopupMenu, int nItem, BOOL bForward, UINT nFlags) const
{
	int nCount = pPopupMenu->GetCount();
	int nStart = nItem;

	if ((nItem < 0) && bForward)
		// going forward from beginning -- stop after last menu item
		nItem = nStart = nCount;
	else if ((nItem >= nCount) && (!bForward))
		// going backward from end -- stop after first menu item
		nItem = nStart = -1;


	if (!nCount)
		return MENU_NOITEM;

	BOOL bFirst = TRUE;

	for (;;)
	{
		nItem += bForward ? +1 : -1;

		if (nItem == nStart && !bFirst)
			return MENU_NOITEM;

		if (nItem >= nCount)
		{
			if (nFlags)
				return MENU_NOITEM;

			nItem = -1;
			continue;
		}
		else if (nItem < 0)
		{
			if (nFlags)
				return MENU_NOITEM;

			nItem = nCount;
			continue;
		}

		bFirst = FALSE;

		CXTPSkinPopupMenuItem* pItem = pPopupMenu->GetItem(nItem);

		if (pItem->IsSeparator())
			continue;

		if (pPopupMenu->m_bMenuBar && !pPopupMenu->m_bSysMenu && pItem->IsMDISysButton())
			continue;

		break;
	}

	return nItem;
}

void CXTPSkinPopupMenuState::OnKeyDown(CXTPSkinPopupMenu* pPopupMenu, WPARAM nKey)
{
	if (m_bButtonDown)
		return;

	switch (nKey)
	{
		case VK_MENU:
		case VK_F10:
			Dismiss();
			return;

		case VK_ESCAPE:

			if (pPopupMenu->m_bMenuBar || pPopupMenu == m_pRootPopup || pPopupMenu->m_pPrevPopup == NULL)
			{
				Dismiss();
			}
			else
			{
				CXTPSkinPopupMenu* pPrevPopup = pPopupMenu->m_pPrevPopup;
				CloseHierarchy(pPrevPopup);

				pPrevPopup->m_bDropNextPopup = FALSE;
			}
			return;

		case VK_UP:
		case VK_DOWN:

			if (pPopupMenu->m_bMenuBar)
			{
				OpenHierarchy(pPopupMenu);
			}
			else
			{
				int nItem = FindNextItem(pPopupMenu, pPopupMenu->m_nSelected, (nKey == VK_UP ? FALSE : TRUE), 0);
				SelectItem(pPopupMenu, nItem);
			}

			return;

		case VK_LEFT:
		case VK_RIGHT:
			{
				int nScreenKey = GetWindowLong(pPopupMenu->m_hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL ? (nKey == VK_LEFT ? VK_RIGHT : VK_LEFT) : (int)nKey;

				if (!pPopupMenu->m_bMenuBar && nScreenKey == VK_RIGHT && !GetNextPopup(pPopupMenu))
				{
					if (OpenHierarchy(pPopupMenu))
						return;
				}
				BOOL bHierarchyWasDropped = FALSE;

				if (GetNextPopup(pPopupMenu))
				{
					bHierarchyWasDropped = TRUE;

					if (nScreenKey == VK_LEFT && !pPopupMenu->m_bMenuBar)
					{
						CloseHierarchy(pPopupMenu);
						return;
					}
				}
				else
				{
					if (pPopupMenu->m_bDropNextPopup) bHierarchyWasDropped = TRUE;
				}


				if (pPopupMenu->m_pPrevPopup)
				{
					OnKeyDown(pPopupMenu->m_pPrevPopup, nKey);
					return;
				}

				if (pPopupMenu->m_bMenuBar)
				{
					int nItem = FindNextItem(pPopupMenu, pPopupMenu->m_nSelected, (nScreenKey == VK_RIGHT  ? TRUE : FALSE), 1);

					if (nItem == MENU_NOITEM)
					{
						if (m_pAlternatePopup)
						{
							if (SwitchToAlternateMenu())
							{
								pPopupMenu = m_pRootPopup;
								nItem = FindNextItem(pPopupMenu, MENU_NOITEM, (nScreenKey == VK_RIGHT ? TRUE : FALSE), 0);
							}
						}

					}

					if (nItem != MENU_NOITEM)
					{
						if (GetNextPopup(pPopupMenu))
						{
							CloseHierarchy(pPopupMenu);
						}

						SelectItem(pPopupMenu, nItem);

						if (bHierarchyWasDropped)
						{
							OpenHierarchy(pPopupMenu);
						}
					}
				}
			}
			return;

		case VK_RETURN:
			{
				CXTPSkinPopupMenuItem* pItem = pPopupMenu->GetItem(pPopupMenu->m_nSelected);
				if (!pItem)
					return;

				if (!pItem->IsEnabled())
				{
					Dismiss();
					return;
				}

				if (pItem->GetPopupMenu())
				{
					OpenHierarchy(pPopupMenu);
					return;
				}

				DismissNotify(pPopupMenu, pPopupMenu->m_nSelected);

			}
			return;
	}
}

int CXTPSkinPopupMenuState::FindChar(CXTPSkinPopupMenu* pPopupMenu, WPARAM nKey, int nItem)
{
	if (!nKey)
		return MENU_NOITEM;

	CString sKey((TCHAR)nKey);
	sKey.MakeLower();

	int nResult = MENU_NOITEM;
	int nFirst = MENU_NOITEM;

	int nStart = nItem;

	if (nStart < 0)
		nStart = FindNextItem(pPopupMenu, -1, FALSE, 0);

	do
	{
		int nPrev = nItem;

		nItem = FindNextItem(pPopupMenu, nItem, TRUE, 0);
		if (nItem == MENU_NOITEM || nItem == nFirst)
			break;

		if (nFirst == MENU_NOITEM)
			nFirst = nItem;

		CString str = pPopupMenu->GetMenuString(nItem);
		if (!str.IsEmpty())
		{
			str.MakeLower();

			int nIndex = str.Find(_T('&'));
			if (nIndex == -1)
			{
				if (str[0] == sKey[0] && nResult == MENU_NOITEM)
					nResult = nItem;
			}
			else
			{
				if (str[nIndex + 1] == sKey[0])
				{
					return nItem;
				}
			}
		}

		if (nItem == nPrev)
			break;

	} while (nItem != nStart);

	return nResult;
}

void CXTPSkinPopupMenuState::OnChar(CXTPSkinPopupMenu* pPopupMenu, WPARAM nKey)
{
	int nItem = FindChar(pPopupMenu, nKey, pPopupMenu->m_nSelected);
	BOOL bExecute = FALSE;

	if (nItem != MENU_NOITEM)
	{
		int nFirstItem = nItem;

		while (!pPopupMenu->GetItem(nItem)->IsEnabled())
		{
			nItem = FindChar(pPopupMenu, nKey, nItem);

			if (nItem == nFirstItem)
			{
				Dismiss();
				return;
			}
		}
		int nItemExecute = nItem;

		do
		{
			nItem = FindChar(pPopupMenu, nKey, nItem);

		} while (!pPopupMenu->GetItem(nItem)->IsEnabled() && (nItem != nFirstItem));

		if (nFirstItem == nItem || nItem == nItemExecute)
			bExecute = TRUE;

		nItem = nItemExecute;
	}

	if (nItem == MENU_NOITEM && pPopupMenu->m_bMenuBar && nKey == ' ')
	{
		if (pPopupMenu->m_bSysMenu)
		{
			nItem = 0;
			bExecute = TRUE;
		}
		else if (m_pAlternatePopup)
		{
			if (SwitchToAlternateMenu())
			{
				OnChar(m_pRootPopup, nKey);
			}
			return;
		}
	}

#if 0
	if (nItem == MENU_NOITEM && pPopupMenu->m_bMenuBar && m_pAlternatePopup && m_pAlternatePopup != pPopupMenu)
	{
		nItem = FindChar(m_pAlternatePopup, nKey, 0);
		if (nItem != MENU_NOITEM)
		{
			if (SwitchToAlternateMenu())
			{
				OnChar(m_pRootPopup , nKey);
			}
			return;
		}
	}
#endif

	if (nItem == MENU_NOITEM && pPopupMenu->m_hWndNotify && !m_flags.bNoNotify)
	{
		int nFlag = (pPopupMenu->m_bSysMenu ? MF_SYSMENU : 0) | (pPopupMenu->m_bMenuBar ? 0 : MF_POPUP);

		LRESULT lResult = ::SendMessage(pPopupMenu->m_hWndNotify, WM_MENUCHAR, MAKELONG((WORD)nKey, (WORD)nFlag), (LPARAM)pPopupMenu->m_hMenu);

		switch (HIWORD(lResult))
		{
		case MNC_IGNORE:
			MessageBeep(0);

			if (nFlag & MF_POPUP)
				return;

			Dismiss();
			return;

		case MNC_CLOSE:
			Dismiss();
			return;

		case MNC_EXECUTE:
			bExecute = TRUE;

		case MNC_SELECT:
			nItem = (short)LOWORD(lResult);
			if (nItem < 0 || nItem < pPopupMenu->GetCount())
				return;

			break;
		}
	}

	if (nItem != MENU_NOITEM)
	{
		SelectItem(pPopupMenu, nItem);

		if (bExecute)
		{
			OnKeyDown(pPopupMenu, VK_RETURN);
		}
	}
}

BOOL CXTPSkinPopupMenuState::SwitchToAlternateMenu()
{
	if (!m_pRootPopup->m_bMenuBar || !m_pAlternatePopup)
		return FALSE;

	SelectItem(m_pRootPopup, MENU_NOITEM);

	CXTPSkinPopupMenu* pAlternatePopup = m_pAlternatePopup;
	m_pAlternatePopup = m_pRootPopup;
	m_pRootPopup = pAlternatePopup;

	m_pRootPopup->m_bDropNextPopup = m_pAlternatePopup->m_bDropNextPopup;

	return TRUE;
}

BOOL CXTPSkinPopupMenuState::HandleMenuMessage(LPMSG lpMsg)
{
	switch (lpMsg->message)
	{
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			if (!m_flags.bRightButton)
			{
				m_nFocus = CXTPSkinPopupMenuState::focusMouse;
				CPoint point;
				GetCursorPos(&point);

				int nItem = 0;
				CXTPSkinPopupMenu* pMenuHit = FindMenu(point, m_pRootPopup, &nItem);

				if (!pMenuHit)
				{
					Dismiss();
					return TRUE;
				}

				RemoveMessage(lpMsg->message);
				return TRUE;
			}

		case WM_LBUTTONDBLCLK:
		case WM_NCLBUTTONDBLCLK:
		{
			CPoint point;
			GetCursorPos(&point);

			int nItem = 0;
			CXTPSkinPopupMenu* pMenuHit = FindMenu(point, m_pRootPopup, &nItem);

			if (!pMenuHit)
			{
				Dismiss();
				return TRUE;
			}

			OnButtonDblClick(pMenuHit, nItem);
		}
		return TRUE;

		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		{
			m_nFocus = CXTPSkinPopupMenuState::focusMouse;
			CPoint point;
			GetCursorPos(&point);

			int nItem = 0;
			CXTPSkinPopupMenu* pMenuHit = FindMenu(point, m_pRootPopup, &nItem);

			if (!pMenuHit && !nItem)
			{
				Dismiss();
				return TRUE;
			}

			if (pMenuHit == m_pAlternatePopup)
			{
				SwitchToAlternateMenu();
			}

			if (pMenuHit)
			{
				OnButtonDown(pMenuHit, nItem, TRUE);
			}

			RemoveMessage(lpMsg->message);

			return TRUE;
		}
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
		{
			if (!m_flags.bRightButton)
			{
				if (m_bButtonDown)
				{
					RemoveMessage(lpMsg->message);
					return TRUE;
				}

				CPoint point;
				GetCursorPos(&point);

				int nItem = 0;
				CXTPSkinPopupMenu *pMenuHit = FindMenu(point, m_pRootPopup, &nItem);
				if(NULL != pMenuHit)
				{
					MSG msg;
					ZeroMemory(&msg, sizeof(msg));

					msg.hwnd    = lpMsg->hwnd;
					msg.message = WM_MENURBUTTONUP;
					msg.wParam  = WPARAM(nItem);
					msg.lParam  = LPARAM(pMenuHit->m_hMenu);

					DispatchMessage(&msg);

					return TRUE;
				}

				break;
			}
		}

		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			{
				if (!m_bButtonDown)
					return TRUE;


				CPoint point;
				GetCursorPos(&point);

				int nItem = 0;
				CXTPSkinPopupMenu* pMenuHit = FindMenu(point, m_pRootPopup, &nItem);


				if (m_pRootPopup->m_bMenuBar)
				{
					if (pMenuHit == NULL)
					{
						Dismiss();
						return TRUE;
					}
				}
				else
				{
					if (pMenuHit == NULL)
					{
						if (!m_bFirstClick)
						{
							Dismiss();
							return TRUE;
						}
					}
					m_bFirstClick = FALSE;
				}

				if (pMenuHit)
				{
					OnButtonUp(pMenuHit, nItem);
				}

				m_bButtonDown = FALSE;

				return TRUE;
			}

		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			{
				CPoint point;
				GetCursorPos(&point);

				OnMouseMove(point);
				return TRUE;
			}

		case WM_TIMER:
		{
			CXTPSkinPopupMenu *pPopupMenu = FindPopup(lpMsg->hwnd);
			if (pPopupMenu)
			{
				if (TIMERID_MENUSHOW == lpMsg->wParam && pPopupMenu->m_nShowTimer == lpMsg->wParam)
				{
					pPopupMenu->m_bToggle = FALSE;
					OpenHierarchy(pPopupMenu);
				}
				if (TIMERID_MENUHIDE == lpMsg->wParam && pPopupMenu->m_nHideTimer == lpMsg->wParam)
				{
					pPopupMenu->m_bToggle = FALSE;
					CloseHierarchy(pPopupMenu);
				}
				if (XTP_TID_HOVERUP == lpMsg->wParam)
				{
					pPopupMenu->OnScrollUp();
				}
				if (XTP_TID_HOVERDOWN == lpMsg->wParam)
				{
					pPopupMenu->OnScrollDown();
				}

				return TRUE;
			}

			return FALSE;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			{
				if (m_bButtonDown)
					return TRUE;

				m_nFocus = focusKeyboard;

				switch (lpMsg->wParam)
				{
					case VK_UP:
					case VK_DOWN:
					case VK_LEFT:
					case VK_RIGHT:
					case VK_RETURN:
					case VK_CANCEL:
					case VK_ESCAPE:
					case VK_MENU:
					case VK_F10:
					case VK_F1:
						OnKeyDown(GetActivePopup(), lpMsg->wParam);
						break;

					case VK_TAB:
						if (m_pRootPopup->m_bMenuBar && !GetNextPopup(m_pRootPopup))
						{
							Dismiss();
							return TRUE;
						}

					default:
						TranslateMessage(lpMsg);
						break;
				}
				return TRUE;
			}

		case WM_CHAR:
		case WM_SYSCHAR:
			OnChar(GetActivePopup(), lpMsg->wParam);
			return TRUE;

		case WM_SYSKEYUP:
			return TRUE;

		case WM_KEYUP:
			TranslateMessage(lpMsg);
			return TRUE;
	}

	return FALSE;
}

BOOL CXTPSkinPopupMenuState::ContinueLoop() const
{
	return (::GetCapture() == m_hWndCapture) && (m_pRootPopup != NULL) && m_bInsideMenuLoop;
}

void CXTPSkinPopupMenuState::EndMenuLoop()
{
	Dismiss();
}

void CXTPSkinPopupMenuState::RunLoop(LPARAM lParam)
{
#ifdef _XTP_ACTIVEX
	SAFE_MANAGE_STATE(XTPSkinManager()->m_pModuleState);
#endif

	CPoint pt(0, 0);

	m_bInsideMenuLoop = TRUE;
	m_bDismiss = FALSE;
	BOOL bSendIdle = TRUE;

	if (!m_bMenuStarted)
	{
		if (!StartMenu(focusMouse))
		{
			EndMenuLoop();
			return;
		}

		if (m_pRootPopup->m_bMenuBar)
		{
			MSG msg;
			msg.message = WM_LBUTTONDOWN;
			msg.wParam = MK_LBUTTON;
			msg.lParam = lParam;
			msg.hwnd = m_pRootPopup->m_hWnd;
			HandleMenuMessage(&msg);
		}
	}

	// get messages until capture lost or canceled/accepted
	while (ContinueLoop())
	{
		MSG msg;

		BOOL bPeek = PeekMessage(&msg, NULL, 0, 0, PM_NOYIELD | PM_NOREMOVE);

		if (bPeek)
		{
			BOOL bQueue = (msg.message == WM_LBUTTONDOWN ||
				msg.message == WM_RBUTTONDOWN ||
				msg.message == WM_NCLBUTTONDOWN ||
				msg.message == WM_NCRBUTTONDOWN);

			if (!bQueue)
			{
				PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
			}

			if (!HandleMenuMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}


			if (!ContinueLoop())
				break;

			if ((msg.message == WM_TIMER) || (msg.message == WM_PAINT))
				continue;

			bSendIdle = TRUE;
		}
		else
		{
			if (!ContinueLoop())
				break;

			if (bSendIdle && !m_flags.bNoNotify && m_pRootPopup->m_hWndNotify)
			{
				SendMessage(m_pRootPopup->m_hWndNotify, WM_ENTERIDLE, MSGF_MENU, (LPARAM)GetActivePopup()->m_hWnd);

			}
			bSendIdle = FALSE;

			WaitMessage();
		}
	} // while (ContinueLoop())

	m_bInsideMenuLoop = FALSE;
	m_hWndCapture = 0;

	EndMenuLoop();
	ReleaseCapture();

}


void CXTPSkinObjectApplicationFrame::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	if ((GetSkinManager()->GetApplyOptions() & xtpSkinApplyMenus) == 0)
	{
		CXTPSkinObjectFrame::OnContextMenu(pWnd, pos);
		return;
	}

	if (pWnd && pWnd->m_hWnd != m_hWnd)
	{
		CXTPSkinObjectFrame::OnContextMenu(pWnd, pos);
		return;
	}

	LRESULT nHit = SendMessage(WM_NCHITTEST, 0, MAKELONG(pos.x, pos.y));

	if (nHit == HTCAPTION || nHit == HTSYSMENU || nHit == HTMINBUTTON  || nHit == HTMAXBUTTON || nHit == HTCLOSE)
	{
		HMENU hMenu = ::GetSystemMenu(m_hWnd, FALSE);
		if (hMenu)
		{
			int nDefault = SC_CLOSE;

			if (nHit != HTSYSMENU)
			{
				if (GetStyle() & (WS_MAXIMIZE | WS_MINIMIZE)) nDefault = SC_RESTORE; else nDefault = SC_MAXIMIZE;
			}

			::SetMenuDefaultItem(hMenu, nDefault, MF_BYCOMMAND);

			CXTPSkinObjectApplicationFrame::TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_SYSMENU, pos.x, pos.y, m_hWnd, 0);

			return;
		}
	}
	else if (nHit == HTMENU)
		return;

	CXTPSkinObjectFrame::OnContextMenu(pWnd, pos);
}

BOOL CXTPSkinObjectApplicationFrame::TrackPopupMenu(HMENU hMenu, UINT dwFlags, int x, int y, HWND hWnd, CONST RECT *prcRect)
{
	CXTPSkinPopupMenuState* pState = new CXTPSkinPopupMenuState(hWnd);
	BOOL bResult = pState->TrackPopupMenu(hMenu, dwFlags, x, y, hWnd, prcRect);
	pState->EndState();

	return bResult;
}

#define CS_DROPSHADOW       0x00020000

#ifndef SPI_GETDROPSHADOW
#define SPI_GETDROPSHADOW     0x1024
#endif

BOOL CXTPSkinPopupMenu::Create(HWND hwndParent)
{
	UINT nClassStyle = CS_SAVEBITS | CS_DBLCLKS;

	BOOL bDropShadow = FALSE;

	if (XTPSystemVersion()->IsWinXPOrGreater() && SystemParametersInfo(SPI_GETDROPSHADOW, 0, &bDropShadow, 0))
	{
		if (bDropShadow)
		{
			nClassStyle |= CS_DROPSHADOW;
		}
	}

	XTPDrawHelpers()->RegisterWndClass(0, _T("XTPSkinManagerMenu"), nClassStyle);

	UINT nWindowStyle = WS_EX_TOOLWINDOW;

	if (GetWindowLong(hwndParent, GWL_EXSTYLE) & (WS_EX_LAYOUTRTL | WS_EX_RIGHT))
	{
		nWindowStyle |= WS_EX_LAYOUTRTL;
	}

	HWND hWnd = ::CreateWindowEx(nWindowStyle, _T("XTPSkinManagerMenu"),
		NULL, WS_POPUP, 0, 0, 100, 100, hwndParent, 0, XTPGetInstanceHandle(), 0);

	if (!hWnd)
		return FALSE;

	SubclassWindow(hWnd);

	return TRUE;
}

void CXTPSkinPopupMenuState::AdjustMonitorRect(CXTPSkinPopupMenu* pPopupMenu, CPoint& pt, CSize sz, UINT wFlags, LPCRECT prcExclude)
{
	CRect rcExclude(pt.x, pt.y, pt.x, pt.y);
	CRect rcMonitor(XTPMultiMonitor()->GetScreenArea(pt));

	if (prcExclude)
	{
		rcExclude.IntersectRect(prcExclude, rcMonitor);
	}

	if (pt.x + sz.cx > rcMonitor.right)
	{
		if ((wFlags & TPM_CENTERALIGN) || (pt.x - sz.cx < rcMonitor.left) || (pt.x >= rcMonitor.right))
			pt.x = rcMonitor.right - sz.cx - 1;
		else
			pt.x -= sz.cx - 1;

		pPopupMenu->m_bDroppedLeft = TRUE;

		if (!rcExclude.IsRectEmpty())
		{
			if (CRect().IntersectRect(rcExclude, CRect(pt, sz)))
			{
				pt.x = rcExclude.left - sz.cx;
			}
		}
	}

	if (pt.x < rcMonitor.left)
	{
		pt.x += sz.cx;
		if ((wFlags & TPM_CENTERALIGN) || (pt.x >= rcMonitor.right) || (pt.x < rcMonitor.left))
			pt.x = rcMonitor.left;
	}


	if (pt.y + sz.cy > rcMonitor.bottom)
	{
		if ((wFlags & TPM_VCENTERALIGN) || (pt.y - sz.cy < rcMonitor.top) || (pt.y >= rcMonitor.bottom))
			pt.y = rcMonitor.bottom - sz.cy - 1;
		else
			pt.y -= sz.cy - 1;

		if (!rcExclude.IsRectEmpty())
		{
			if (CRect().IntersectRect(rcExclude, CRect(pt, sz)))
			{
				pt.y = rcExclude.top - sz.cy;
			}
		}
	 }

	if (pt.y < rcMonitor.top)
	{
		pt.y += sz.cy;
		if ((wFlags & TPM_VCENTERALIGN) || (pt.y >= rcMonitor.bottom) || (pt.y < rcMonitor.top))
			pt.y = rcMonitor.top;
	 }
}

BOOL CXTPSkinPopupMenuState::TrackPopupMenu(HMENU hMenu, UINT dwFlags, int x, int y, HWND hWnd, CONST RECT* prcRect)
{
#ifdef _XTP_ACTIVEX
	SAFE_MANAGE_STATE(XTPSkinManager()->m_pModuleState);
#endif

	if (!hWnd)
		return FALSE;

	if (!hMenu)
		return FALSE;

	if (GetMenuItemCount(hMenu) == 0)
		return FALSE;

	BOOL bButtonDown = (GetKeyState(dwFlags & TPM_RIGHTBUTTON ? VK_RBUTTON : VK_LBUTTON) & 0x8000) != 0;

	CXTPSkinPopupMenu* pSubMenu = new CXTPSkinPopupMenu();
	pSubMenu->m_pState = this;

	if (!pSubMenu->Create(hWnd))
	{
		pSubMenu->InternalRelease();
		return 0;
	}

	pSubMenu->m_hMenu = hMenu;
	pSubMenu->m_hWndNotify = hWnd;
	pSubMenu->m_bSysMenu =  ((dwFlags & TPM_SYSMENU) != 0);

	m_flags.bRightButton = ((dwFlags & TPM_RIGHTBUTTON) != 0);
	m_flags.bNoNotify    = ((dwFlags & TPM_NONOTIFY)    != 0);
	m_flags.bReturnCmd   = ((dwFlags & TPM_RETURNCMD));

	InitMenu(pSubMenu);

	if (!StartMenu(CXTPSkinPopupMenuState::focusMouse))
	{
		pSubMenu->InternalRelease();
		return 0;
	}

	m_bFirstClick = bButtonDown;


	if (!m_flags.bNoNotify)
	{
		SendMessage(hWnd, WM_INITMENUPOPUP, (WPARAM)hMenu, MAKELONG(0, (pSubMenu->m_bSysMenu ? 1 : 0)));
	}

	pSubMenu->m_bSendUninit = TRUE;

	CSize size = pSubMenu->RecalcLayout();

	if ((GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL) && ((dwFlags & TPM_CENTERALIGN) == 0))
		dwFlags ^= TPM_RIGHTALIGN;

	CPoint pt(x, y);

	if (dwFlags & TPM_RIGHTALIGN)
	{
		pt.x -= size.cx;
		pSubMenu->m_bDroppedLeft = TRUE;
	}
	else if (dwFlags & TPM_CENTERALIGN)
	{
		pt.x -= size.cx / 2;
	}

	if (dwFlags & TPM_BOTTOMALIGN)
	{
		pt.y -= size.cy;
	}
	else if (dwFlags & TPM_VCENTERALIGN)
	{
		pt.y -= size.cy / 2;
	}

	AdjustMonitorRect(pSubMenu, pt, size, dwFlags, prcRect);

	PlayEventSound(xtpSoundMenuPopup);

	::SetWindowPos(pSubMenu->m_hWnd, HWND_TOPMOST, pt.x, pt.y, size.cx, size.cy, SWP_SHOWWINDOW | SWP_NOOWNERZORDER | SWP_NOACTIVATE);

	m_bButtonDown = bButtonDown;

	RunLoop(0);


	CloseHierarchy(pSubMenu);
	if (pSubMenu->m_bSendUninit && !m_flags.bNoNotify)
	{
		SendMessage(pSubMenu->m_hWndNotify, WM_UNINITMENUPOPUP,
			(WPARAM)pSubMenu->m_hMenu, MAKELONG(0, pSubMenu->m_bSysMenu ? MF_SYSMENU: 0));
	}

	pSubMenu->DestroyWindow();
	pSubMenu->InternalRelease();

	return m_flags.bReturnCmd ? m_nLastCommand : TRUE;
}
