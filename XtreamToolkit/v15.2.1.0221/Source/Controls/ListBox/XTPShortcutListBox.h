// XTPShortcutListBox.h interface for the CXTPShortcutListBox class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSHORTCUTLISTBOX_H__)
#define __XTPSHORTCUTLISTBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPShortcutListBoxTheme;
class CXTPImageManagerIcon;

//===========================================================================
// Summary:
//     CCXTPShortcutListBox is a CListBox derived class. It is used to implement
//     an Outlook bar style control. It can only be used with the LBS_OWNERDRAWVARIABLE
//     style bit set. This is a simpler version of CXTOutBarCtrl and does not
//     allow for shortcut folders.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutListBox : public CListBox
{
	DECLARE_DYNAMIC(CXTPShortcutListBox)

public:
	// ------------------------------------------------------------------
	// Summary:
	//     CONTENT_ITEM structure is used by the CXTPShortcutListBox
	//     class to maintain information about a particular menu item.
	// See Also:
	//     CXTPShortcutListBox
	// ------------------------------------------------------------------
	struct CONTENT_ITEM
	{
		//{{AFX_CODEJOCK_PRIVATE
		CONTENT_ITEM();
		~CONTENT_ITEM();
		//}}AFX_CODEJOCK_PRIVATE

		int           m_nIndex;   // Zero-based index for the menu item.
		BOOL          m_bEnabled; // TRUE if the menu item is enabled.
		CString       m_strText;  // Text label for the menu item.
		CXTPImageManagerIcon* m_pIcon;    // Icon handle for the menu item.
	};

	// ----------------------------------------------------------------------
	// Summary:
	//     List for maintaining CONTENT_ITEM structures.
	// Remarks:
	//     CList definition used by the CXTPShortcutListBox class to maintain
	//     a list of CONTENT_ITEM structures representing each menu item
	//     defined for the menu list box control.
	// See Also:
	//     CXTPShortcutListBox, CXTPShortcutListBox::InsertMenuItem, CONTENT_ITEM
	// ----------------------------------------------------------------------
	typedef CList<CONTENT_ITEM*, CONTENT_ITEM*> CContentItemList;


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutListBox object
	//-----------------------------------------------------------------------
	CXTPShortcutListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutListBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutListBox();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     nTheme - New visual theme. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     nStyle can be one of the following:
	//     * <b>xtpControlThemeDefault</b> Use default theme.
	//     * <b>xtpControlThemeOfficeXP</b> Use Office XP theme.
	//     * <b>xtpControlThemeOffice2003</b> Use Office 2003 theme.
	//-----------------------------------------------------------------------
	BOOL SetTheme(XTPControlTheme eTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the currently seleted theme.
	// Returns:
	//     A pointer to a CXTPShortcutListBoxTheme object representing the currently selected theme.
	//-----------------------------------------------------------------------
	CXTPShortcutListBoxTheme* GetTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates an Outlook Bar control.
	// Parameters:
	//     dwStyle    - Window style.
	//     rect       - The size and position of the window, in client coordinates of
	//                  'pParentWnd'.
	//     pParentWnd - The parent window.
	//     nID        - The ID of the child window.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text and background colors for
	//     the Outlook bar.
	// Parameters:
	//     clrText     - RGB value representing the text color.
	//     clrBack     - RGB value representing the background color.
	//     clrBackDark - RGB value representing the dark background color. If defined the
	//                   background will be drawn with a gradient effect from crBack to crBackDark.
	//-----------------------------------------------------------------------
	void SetColors(COLORREF clrText, COLORREF clrBack, COLORREF clrBackDark = COLORREF_NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function inserts a menu item into the Outlook bar. Unlike
	//     the AddMenuItem member function, InsertMenuItem does not cause an Outlook
	//     bar with the LBS_SORT style to be sorted.
	// Parameters:
	//     iIndex - Specifies the zero-based index of the position to insert the
	//     menu item. If this parameter is -1, the menu item is added to the
	//     end of the list.
	//     nIconID - Resource ID of the icon associated with this menu item.
	//     lpszText - Points to the null-terminated string for the menu item
	//     bEnabled - TRUE if the menu item is enabled.
	// Returns:
	//     The zero-based index of the position at which the menu item was inserted.
	//     The return value is LB_ERR if an error occurs. The return value is LB_ERRSPACE
	//     if insufficient space is available to store the new menu item.
	//-----------------------------------------------------------------------
	int InsertMenuItem(int iIndex, UINT nIconID, LPCTSTR lpszText, BOOL bEnabled = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a menu item to an Outlook bar. If
	//     the Outlook bar was not created with the LBS_SORT style, the menu item
	//     is added to the end of the Outlook bar. Otherwise, the menu item is
	//     inserted into the Outlook bar, and the Outlook bar is sorted. If the
	//     Outlook bar was created with the LBS_SORT style but not the LBS_HASSTRINGS
	//     style, the framework sorts the Outlook bar by one or more calls to
	//     the CompareItem member function. Use InsertMenuItem to insert a menu
	//     item into a specific location within the Outlook bar.
	// Parameters:
	//     nIconID - Resource ID of the icon associated with this menu item.
	//     lpszText - Points to the null-terminated string for the menu item.
	//     bEnabled - TRUE if the menu item is enabled.
	// Returns:
	//     The zero-based index to the menu item in the Outlook bar. The return
	//     value is LB_ERR if an error occurs. The return value is LB_ERRSPACE
	//     if insufficient space is available to store the new menu item.
	//-----------------------------------------------------------------------
	int AddMenuItem(UINT nIconID, LPCTSTR lpszText, BOOL bEnabled = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable the menu item
	//     specified by <i>iItem</i>.
	// Parameters:
	//     iItem - Index of the menu item to enable or disable.
	//     bEnabled - TRUE to enable the menu item, FALSE to disable.
	// Returns:
	//     Returns TRUE if the menu item's enabled state was successfully
	//     updated, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL EnableMenuItem(int iItem, BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return an CONTENT_ITEM object that
	//     represents the menu item specified by 'iItem'.
	// Parameters:
	//     iItem - Specifies the zero-based index of the menu item to retrieve.
	// Returns:
	//     An CONTENT_ITEM pointer.
	//-----------------------------------------------------------------------
	CONTENT_ITEM* GetMenuItem(int iItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the cursor, text and border attributes
	//     for a highlighted item.
	// Parameters:
	//     nIDCursor    - Resource id of the highlight cursor.
	//     bNoBorder    - True if not border is displayed when item is highlighted.
	//     bNoUnderline - True if text is not underlined when item is highlighted.
	//-----------------------------------------------------------------------
	void SetHilightCursor(UINT nIDCursor, bool bNoBorder = false, bool bNoUnderline = false);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the outlook bar to determine the
	//     string height for a multi-line text item.
	// Parameters:
	//     lspzItem - NULL terminated string.
	// Returns:
	//     The height in pixels of the specified string.
	// ----------------------------------------------------------------------
	int GetTextHeight(LPCTSTR lspzItem) const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh theme colors and redraw the control.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPShortcutListBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPShortcutListBox)
	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseLeave();
	//}}AFX_MSG
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam);
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void RedrawItem(int iIndex);
	void RedrawItem(CPoint point);

public:
	bool                m_bNoBorder;        // true to disable a border drawn around highlighted items.
	bool                m_bNoUnderline;     // true if the text is not drawn with a underline when no highlight border is displayed.

protected:
	int               m_nPrevIndex;     // Previously selected menu index.
	int               m_bHilightItemID;   // Index of the currently highlighted item, set to -1 if no selection.
	bool              m_bHilight;         // true when the menu item is selected.
	bool              m_bTimerActive;     // true if the redraw timer is active.
	CPoint            m_point;            // Holds the cursor position.
	CContentItemList  m_arContentItems;     // Array of CONTENT_ITEM structs that represent each item in the Outlook bar.
	HCURSOR           m_hCursor;          // Cursor display when item is hovered.
	CXTPShortcutListBoxTheme* m_pTheme;      // Pointer to the current theme object.
};

//{{AFX_CODEJOCK_PRIVATE
#define CXTOutlookBar CXTPShortcutListBox
//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPShortcutListBox::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

AFX_INLINE CXTPShortcutListBoxTheme* CXTPShortcutListBox::GetTheme() {
	return m_pTheme;
}

#endif // #if !defined(__XTPSHORTCUTLISTBOX_H__)
