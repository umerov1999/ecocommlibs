// XTPSkinObjectMenu.h: interface for the CXTPSkinObjectMenu class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSKINOBJECTMENU_H__)
#define __XTPSKINOBJECTMENU_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPSkinPopupMenu;
class CXTPSkinManagerSchema;
class CXTPSkinPopupMenuState;

class CXTPSkinPopupMenuItem
{
public:
	CXTPSkinPopupMenuItem(CXTPSkinPopupMenu *pPopupParent, int nItem);

public:
	HMENU GetPopupMenu() const;
	DWORD GetState() const;
	CRect GetScreenPos() const;
	BOOL IsSeparator() const;
	BOOL IsOwnerDraw() const;
	UINT GetID() const;
	BOOL IsEnabled() const;
	BOOL IsChecked() const;
	UINT GetType() const;
	BOOL IsDefault() const;

	BOOL IsMenuBreak() const;

	CString GetText() const;
	DWORD_PTR GetItemData() const;

	void GetMenuItemInfo(UINT fMask, LPMENUITEMINFO lpMII) const;

	HBITMAP GetItemBitmap() const;

	BOOL IsMDISysButton() const;
public:
	CRect m_rcItem;
	int m_nItem;
	HMENU m_hMenu;

	CXTPSkinPopupMenu *m_pPopupParent;
	CXTPSkinPopupMenu *m_pPopupChild;
};

class CXTPSkinPopupMenu : public CWnd
{
public:
	CXTPSkinPopupMenu();
	~CXTPSkinPopupMenu();

public:
	CString GetMenuString(int nIndex);
	int GetCount();
	CXTPSkinPopupMenuItem* GetItem(int nIndex) const;

	void RebuildItems();

	void RemoveAll();

	CSize RecalcLayout();
	void PositionSysMenu();

	BOOL Create(HWND hwndParent);
	BOOL IsMenuThemed();

	BOOL IsSysMenuBar() const {
		return m_bMenuBar && m_bSysMenu;
	}
	void OnFinalRelease();

	void OnScrollUp();
	void OnScrollDown();
	void EnsureVisible(int nItem);

protected:

	void RecalcItemsSize(CDC *pDC);
	int RecalcItemsHeight(int nScreenArea);

	int GetColumnCount() const;

	BOOL IsMultiColumn() const
	{
		return GetColumnCount() > 1;
	}

	void Scroll(int nOffsetY);
	void ScrollUp();
	void ScrollDown();

	void ShowScrollUp(BOOL bShow);
	void ShowScrollDown(BOOL bShow);

protected:
	DECLARE_MESSAGE_MAP();
	//{{AFX_MSG(CXTPPopupBar)
	afx_msg void OnPaint();
	//}}AFX_MSG

public:

	CXTPSkinPopupMenu* LookUp(HMENU hMenu);

	HMENU m_hMenu;

	CArray<CXTPSkinPopupMenuItem*, CXTPSkinPopupMenuItem*> m_arrItems;
	int m_nSelected;
	int m_nDropped;
	BOOL m_bMenuBar;
	BOOL m_bSysMenu;
	BOOL m_bToggle;
	BOOL m_bDropNextPopup;
	BOOL m_bAboutToHide;
	BOOL m_bSendUninit;
	BOOL m_bDroppedLeft;

	HWND m_hWndNotify;

	int m_nMaxWidthColumn;
	int m_nGripperWidth;
	int m_nHeight;

	CXTPSkinPopupMenuState* m_pState;

	UINT_PTR m_nHideTimer;
	UINT_PTR m_nShowTimer;

	CXTPSkinPopupMenu* m_pNextPopup;
	CXTPSkinPopupMenu* m_pPrevPopup;

	CRect m_rcBorder;
	XTP_SCROLLINFO m_scrollInfo;
};

class CXTPSkinPopupMenuState
{
public:
	enum MenuFocus
	{
		focusMouse,
		focusKeyboard
	};

public:
	CXTPSkinPopupMenuState(HWND hWndNotify);

public:
	BOOL HandleMenuMessage(LPMSG lpMsg);
	BOOL StartMenu(MenuFocus nFocus);
	void InitMenu(CXTPSkinPopupMenu* pPopupMenu);
	void RunLoop(LPARAM lParam);

	int ItemHitTest(CPoint point, CXTPSkinPopupMenu* pPopupMenu);
	CXTPSkinPopupMenu* FindMenu(CPoint point, CXTPSkinPopupMenu* pPopupMenu, int* pnItem);

	void EndState();

	BOOL TrackPopupMenu(HMENU hMenu, UINT dwFlags, int x, int y, HWND hWnd, CONST RECT *prcRect);

public:
	void OnButtonDown(CXTPSkinPopupMenu* pMenu, int nItem, BOOL bClick);
	void OnButtonDblClick(CXTPSkinPopupMenu* pPopupMenu, int nItem);
	void OnButtonUp(CXTPSkinPopupMenu* pMenu, int nItem);
	void OnMouseMove(CPoint point);
	void OnKeyDown(CXTPSkinPopupMenu* pMenu, WPARAM nKey);
	void OnChar(CXTPSkinPopupMenu* pPopupMenu, WPARAM nKey);

	int FindChar(CXTPSkinPopupMenu* pPopupMenu, WPARAM nKey, int nStart);

	CXTPSkinPopupMenuItem* SelectItem(CXTPSkinPopupMenu* pPopupMenu, int nItem);
	void SendMenuSelect(CXTPSkinPopupMenu* pPopupMenu);
	HWND OpenHierarchy(CXTPSkinPopupMenu* pPopupMenu);
	void CloseHierarchy(CXTPSkinPopupMenu* pPopupMenu);
	BOOL HideNextHierarchy(CXTPSkinPopupMenu* pPopupMenu);
	BOOL SetTimerToCloseHierarchy(CXTPSkinPopupMenu* pPopupMenu);
	BOOL SetTimerToOpenHierarchy(CXTPSkinPopupMenu* pPopupMenu);
	void RedrawPopup(CXTPSkinPopupMenu* pPopupMenu);

	CXTPSkinPopupMenu* GetNextPopup(CXTPSkinPopupMenu* pPopupMenu) const;
	CXTPSkinPopupMenu* GetActivePopup() const;

	BOOL ContinueLoop() const;
	void Dismiss();
	void Notify(UINT nMessage, UINT nCommand, LPARAM lParam);
	void DismissNotify(CXTPSkinPopupMenu* pPopupMenu, int nItem);
	void DismissNotify(HMENU hMenu, int nItem);
	void EndMenuLoop();

	CXTPSkinPopupMenu* FindPopup(HWND hWnd) const;
	BOOL RemoveMessage(UINT nMessage);

	int FindNextItem(CXTPSkinPopupMenu* pPopupMenu, int nItem, BOOL bForward, UINT nFlags) const;
	void FilterMenuKey(LPARAM lParam);

	BOOL SwitchToAlternateMenu();
	void AdjustMonitorRect(CXTPSkinPopupMenu* pPopupMenu, CPoint& pt, CSize sz, UINT dwFlags, LPCRECT prcExclude);
	CPoint PositionHierarchy(CXTPSkinPopupMenu* pSubMenu, CXTPSkinPopupMenu* pPopupMenu, CRect rcItem, CSize size);

	static void AFX_CDECL WindowToScreen(HWND hWnd, CRect& rcItem);
	void PlayEventSound(UINT nID);
	int GetMenuFlags(HMENU hMenu);
public:

public:
	MenuFocus m_nFocus;
	BOOL m_bButtonDown;
	BOOL m_bInsideMenuLoop;
	BOOL m_bDismiss;
	HWND m_hWndCapture;
	HWND m_hWndCaptureOld; // The handle of the window that had previously captured the mouse

	CXTPSkinPopupMenu* m_pRootPopup;
	CXTPSkinPopupMenu* m_pAlternatePopup;
	BOOL m_bNofyByPos;
	CPoint m_ptMouseLast;
	BOOL m_bMenuStarted;

	int m_nLastCommand;
	BOOL m_bFirstClick;

	DWORD m_dwMenuShowDelay;
	CXTPSkinManagerSchema* m_pSchema;

	struct TrackPopupMenuFlags
	{
		TrackPopupMenuFlags()
			: bRightButton(FALSE)
			, bNoNotify   (FALSE)
			, bReturnCmd  (FALSE)
		{
		}

		BOOL bRightButton;      // TPM_RIGHTBUTTON
		BOOL bNoNotify;         // TPM_NONOTIFY
		BOOL bReturnCmd;            // TPM_RETURNCMD
	};

	TrackPopupMenuFlags m_flags;
};

class _XTP_EXT_CLASS CXTPSkinObjectApplicationFrame : public CXTPSkinObjectFrame
{
	DECLARE_DYNCREATE(CXTPSkinObjectApplicationFrame)

public:
	CXTPSkinObjectApplicationFrame();
	virtual ~CXTPSkinObjectApplicationFrame();

	CXTPSkinPopupMenu* LookUp(HMENU hMenu);

protected:
	void DrawFrame(CDC* pDC);
	BOOL FrameHasMenuBar();
	void DrawFrameMenuBar(CDC* pDC);
	void RedrawMenuBar();
	void UpdateMenuBar();

protected:
	int HitTestMenuItem(CPoint point);

	void RebuildMenuItems();

	CXTPSkinPopupMenuState* StartMenuState(UINT nID, LPARAM lParam);


public:
	static BOOL AFX_CDECL TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, HWND hWnd, CONST RECT *prcRect);


protected:
	DECLARE_MESSAGE_MAP();

	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg LRESULT HandleNcHitTest(CPoint point);
	afx_msg BOOL HandleMouseMove(CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

	BOOL HandleSysCommand(UINT nID, LPARAM lParam);

protected:
	CRect m_rcMenuBar;
	CXTPSkinPopupMenu* m_pPopupMenu;
};

#endif // !defined(__XTPSKINOBJECTMENU_H__)
