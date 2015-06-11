// XTPTabBase.h : header file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#ifndef __XTTABCTRLBASE_H__
#define __XTTABCTRLBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPTabCtrlButtons;
class CXTPTabBaseTheme;

// ----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine navigation button display for a
//     CXTPTabCtrl object.
// Remarks:
//     XTPNavBtnState type defines the constants used by the
//     CXTPTabCtrl class to determine which navigation buttons to display.
// See Also:
//     CXTPTabCtrl
//
// <KEYWORDS xtpNavBtnArrows, xtpNavBtnClose, xtpNavBtnShowAll>
// ----------------------------------------------------------------------
enum XTPNavBtnState
{
	xtpNavBtnArrows  = 1, // Display arrow buttons.
	xtpNavBtnClose   = 2, // Display close button.
	xtpNavBtnShowAll = 3  // Display arrow and close buttons.
};

//===========================================================================
// Summary:
//     CXTPTabBase is a stand alone base class. It is used to draw an XP
//     style tab control.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabBase
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabBase object
	//-----------------------------------------------------------------------
	CXTPTabBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPTabBase();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the associated tab control.
	// Remarks:
	//     This member function is called by the base class to manage a
	//     pointer for the derived class CTabCtrl object.
	// Returns:
	//     A pointer to a CTabCtrl object if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CTabCtrl* GetTabCtrlImpl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the tab control navigation button object.
	// Remarks:
	//     This member function is called to retrieve a to the tab control
	//     buttons used for navigating and closing tab windows.
	// Returns:
	//     A pointer to a CXTPTabCtrlButtons object if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPTabCtrlButtons* GetButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function copies the child coordinates of the CTabCtrl client
	//     area into the object referenced by 'rcChild'. The client coordinates
	//     specify the upper-left and lower-right corners of the client area.
	// Parameters:
	//     rcChild - A reference to a CRect object to receive the client coordinates.
	//-----------------------------------------------------------------------
	virtual void GetChildRect(CRect& rcChild) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set visibility of the navigation buttons. These
	//     buttons are used in place of the default forward and back buttons that are
	//     displayed when the tab control is not wide enough to display all tabs. You can
	//     also define a close button to be used to close the active tab. This will give
	//     the tab control a VS.NET style tabbed interface.
	// Parameters:
	//     dwFlags - The value can be one or more of the values listed in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//          * <b>xtpNavBtnArrows</b> To show arrow buttons.
	//          * <b>xtpNavBtnClose</b> To show close button.
	//          * <b>xtpNavBtnShowAll</b> To show arrow and close buttons.
	//-----------------------------------------------------------------------
	void ShowNavButtons(DWORD dwFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     eTheme - New visual theme. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     eTheme can be one of the following:
	//     * <b>xtpControlThemeDefault</b> Use default theme.
	//     * <b>xtpControlThemeOfficeXP</b> Use Office XP theme.
	//     * <b>xtpControlThemeOffice2003</b> Use Office 2003 theme.
	//-----------------------------------------------------------------------
	BOOL SetTheme(XTPControlTheme eTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the currently selected theme.
	// Returns:
	//     A pointer to a CXTPTabBaseTheme object representing the currently selected theme.
	//-----------------------------------------------------------------------
	CXTPTabBaseTheme* GetTheme();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh theme colors and redraw the control.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTPTabBase class to
	//     perform initialization when the window is created or sub-classed.
	// Returns:
	//     TRUE if the window was successfully initialized, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Init();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to associate the tab control with this object.
	// Parameters:
	//     pTabCtrl - Points to a valid tab control object.
	//-----------------------------------------------------------------------
	void ImplAttach(CTabCtrl* pTabCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the tab control to add padding to a
	//     tab label for use with XP style tabs.
	// Parameters:
	//     strLabelText - Tab label to add padding to.
	//-----------------------------------------------------------------------
	virtual void OnAddPadding(CString& strLabelText);

public:
	bool m_bBoldFont;       // true to set the selected tab font to bold.
	bool m_bXPBorder;       // true to draw an XP border around the tab child window.
	BOOL m_bAutoCondensing; // TRUE for auto-condensing tabs.

protected:
	CTabCtrl*           m_pTabCtrl;         // Pointer to the tab control associated with this object.
	CXTPTabCtrlButtons* m_pNavBtns;         // Arrow buttons.
	CXTPTabBaseTheme*   m_pTheme;           // Pointer to the current theme object.
	BOOL                m_bSubclassed;      // TRUE if the window was sub-classed.

//{{AFX_CODEJOCK_PRIVATE
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//}}AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam);
//}}AFX_CODEJOCK_PRIVATE
};

//---------------------------------------------------------------------------

AFX_INLINE CTabCtrl* CXTPTabBase::GetTabCtrlImpl() {
	return m_pTabCtrl;
}
AFX_INLINE CXTPTabCtrlButtons* CXTPTabBase::GetButtons() {
	return m_pNavBtns;
}
AFX_INLINE CXTPTabBaseTheme* CXTPTabBase::GetTheme() {
	return m_pTheme;
}

//===========================================================================

//{{AFX_CODEJOCK_PRIVATE
#define DECLATE_TABCTRL_BASE(ClassName, Tab, Base)\
class _XTP_EXT_CLASS ClassName : public Tab, public Base\
{\
protected:\
	virtual void PreSubclassWindow() {\
		Tab::PreSubclassWindow();\
		Base::PreSubclassWindow();\
	}   \
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) {\
		if (!Tab::PreCreateWindow(cs))\
			return FALSE;\
		return Base::PreCreateWindow(cs);\
	}   \
	afx_msg void OnPaint() {\
		Base::OnPaint();\
	}   \
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) {\
		return Base::OnEraseBkgnd(pDC);\
	}   \
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection) {\
		Base::OnSettingChange(uFlags, lpszSection);\
	}   \
	afx_msg void OnSysColorChange() {\
		Base::OnSysColorChange();\
	}   \
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct) {\
		if (Tab::OnCreate(lpCreateStruct) == -1)\
			return -1;\
		return Base::OnCreate(lpCreateStruct);\
	}   \
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam) {\
		return Base::OnSetTheme(wParam, lParam);\
	}   \
};

#define ON_TABCTRL_REFLECT \
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme) \
	ON_WM_PAINT() \
	ON_WM_ERASEBKGND() \
	ON_WM_SETTINGCHANGE() \
	ON_WM_SYSCOLORCHANGE() \
	ON_WM_CREATE
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPTabExBase is a stand alone base class. It is used to draw an XP
//     style tab control.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabExBase : public CXTPTabBase
{
private:
	struct ITEMANDWIDTH;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabExBase object
	//-----------------------------------------------------------------------
	CXTPTabExBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabExBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPTabExBase();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable the control from
	//     being redrawn or updated.
	// Parameters:
	//     bRedraw - Specifies the state of the redraw flag. If this parameter
	//               is TRUE, the redraw flag is set; if FALSE, the flag is cleared.
	//-----------------------------------------------------------------------
	void EnableRedraw(BOOL bRedraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to initialize the font for the tab control
	//     associated with this view.
	//-----------------------------------------------------------------------
	virtual void InitializeFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the handle of the tooltip control associated
	//     with the tab control. The tab control creates a tooltip control if
	//     it has the TCS_TOOLTIPS style. You can also assign a tooltip control
	//     to a tab control by using the SetToolTips member function.
	// Returns:
	//     The handle of the tooltip control if successful; otherwise returns NULL.
	//-----------------------------------------------------------------------
	virtual CToolTipCtrl* GetTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to assign a tooltip control to the tab control.
	//     You can associate the tooltip control with a tab control by making
	//     a call to GetToolTips.
	// Parameters:
	//     pWndTip - Pointer to a tooltip control.
	//-----------------------------------------------------------------------
	virtual void SetTips(CToolTipCtrl* pWndTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to register a tab with the tooltip control so
	//     that the information stored in the tooltip is displayed when the cursor
	//     is on the tab.
	// Parameters:
	//     nIDTab   - Index of the tab.
	//     lpszText - Pointer to the text for the tool.
	//-----------------------------------------------------------------------
	virtual void AddToolTip(UINT nIDTab, LPCTSTR lpszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to update the tooltip text for the specified tab.
	// Parameters:
	//     nIDTab - Index of the tab.
	//     pViewClass - CRuntimeClass associated with the tab.
	//     lpszText - Pointer to the text for the tool.
	//-----------------------------------------------------------------------
	virtual void UpdateToolTip(int nIDTab, LPCTSTR lpszText);
	virtual void UpdateToolTip(CRuntimeClass* pViewClass, LPCTSTR lpszText); // <combine CXTPTabExBase::UpdateToolTip@int@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to reset the values for the tooltip
	//     control based upon the information stored for each tab.
	//-----------------------------------------------------------------------
	virtual void ResetToolTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable tooltip usage.
	// Parameters:
	//     bEnable - TRUE to enable tooltip usage.
	// Returns:
	//     TRUE if the tooltip control was found and updated, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL EnableToolTipsEx(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a view to the tab control associated
	//     with this view.
	// Parameters:
	//     lpszLabel  - Pointer to the text for the tab associated with the view.
	//     pView      - An existing view to be added to the tab control.
	//     pViewClass - CView runtime class associated with the tab.
	//     pDoc       - CDocument associated with the view.
	//     pContext   - Create context for the view.
	//     iIndex     - -1 to add to the end.
	//     iIconIndex - Icon index for the tab. If -1, 'iIndex' is used to determine the index.
	//     lParam     - Application defined parameter
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL AddView(LPCTSTR lpszLabel, CView* pView, int iIndex = -1, int iIconIndex = -1, LPARAM lParam = 0);
	virtual BOOL AddView(LPCTSTR lpszLabel, CRuntimeClass* pViewClass, CDocument* pDoc = NULL, CCreateContext* pContext = NULL, int iIndex = -1, int iIconIndex = -1, LPARAM lParam = 0); // <combine CXTPTabExBase::AddView@LPCTSTR@CView*@int@int@LPARAM>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to add a control to the tab control
	//     associated with this view.
	// Parameters:
	//     lpszLabel  - Pointer to the text for the tab associated with the view.
	//     pWnd       - CWnd object associated with the tab.
	//     iIndex     - Tab index of where to insert the new view. Default is -1 to add to
	//                  the end.
	//     iIconIndex - Icon index for the tab. If -1, 'iIndex' is used to determine the index.
	//     lParam     - Application defined parameter
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL AddControl(LPCTSTR lpszLabel, CWnd* pWnd, int iIndex = -1, int iIconIndex = -1, LPARAM lParam = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns a pointer to a view from the specified
	//     runtime class.
	// Parameters:
	//     nView - Tab index.
	//     pViewClass - CView runtime class associated with the tab.
	// Returns:
	//     A pointer to a CView object, otherwise returns NULL.
	//-----------------------------------------------------------------------
	virtual CWnd* GetView(int nView);
	virtual CWnd* GetView(CRuntimeClass* pViewClass); // <combine CXTPTabExBase::GetView@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns a pointer to the active view associated
	//     with the selected tab.
	// Returns:
	//     A pointer to the active view, otherwise returns NULL.
	//-----------------------------------------------------------------------
	virtual CWnd* GetActiveView();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to activate the specified view and deactivate
	//     all remaining views.
	// Parameters:
	//     pTabView - CWnd object to make active.
	//-----------------------------------------------------------------------
	virtual void ActivateView(CWnd* pTabView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set a view active based on the specified
	//     runtime class.
	// Parameters:
	//     nActiveTab - Tab index.
	//     pTabView - CWnd object to make active.
	//     pViewClass - CView runtime class associated with the tab.
	//-----------------------------------------------------------------------
	virtual void SetActiveView(int nActiveTab);
	virtual void SetActiveView(CWnd* pTabView); // <combine CXTPTabExBase::SetActiveView@int>
	virtual void SetActiveView(CRuntimeClass* pViewClass); // <combine CXTPTabExBase::SetActiveView@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will remove a view based on the specified
	//     tab index.
	// Parameters:
	//     nView - Tab index of the view.
	//     pView       - Points to the CWnd object associated with the tab.
	//     pViewClass - CView runtime class associated with the tab.
	//     bDestroyWnd - TRUE to destroy the list item.
	//-----------------------------------------------------------------------
	virtual void DeleteView(int nView, BOOL bDestroyWnd = TRUE);
	virtual void DeleteView(CWnd* pView, BOOL bDestroyWnd = TRUE); // <combine CXTPTabExBase::DeleteView@int@BOOL>
	virtual void DeleteView(CRuntimeClass* pViewClass, BOOL bDestroyWnd = TRUE); // <combine CXTPTabExBase::DeleteView@int@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the name for a view based on the tab
	//     index.
	// Parameters:
	//     nView - Tab index of the view.
	//     pViewClass - CView runtime class associated with the tab.
	// Returns:
	//     A NULL terminated string that represents the tab item text.
	//-----------------------------------------------------------------------
	virtual LPCTSTR GetViewName(int nView);
	virtual LPCTSTR GetViewName(CRuntimeClass* pViewClass); // <combine CXTPTabExBase::GetViewName@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when the tab control is resized.
	//     It is responsible for updating internal structures that are
	//     dependent on the control's size
	//-----------------------------------------------------------------------
	virtual void RecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Helper methods
	// Parameters:
	//     hDWP  - Handle to a multiple-window - position structure that contains
	//             size and position information for one or more windows.
	//     pView - A pointer to a CWnd object to be resized.
	//-----------------------------------------------------------------------
	virtual BOOL Defer(HDWP& hDWP, CWnd* pView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to resize the tab view specified by 'pView'.
	// Parameters:
	//     pView - A pointer to a CWnd object to be resized.
	//-----------------------------------------------------------------------
	virtual void ResizeTabView(CWnd* pView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used by the tab control bar to remove an item
	//     from the tab view list.
	// Parameters:
	//     pos         - The POSITION value of the item to be removed.
	//     bDestroyWnd - TRUE to destroy the list item.
	//-----------------------------------------------------------------------
	virtual void RemoveListItem(POSITION pos, BOOL bDestroyWnd = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove all the tabs, including all associated
	//     views.
	// Parameters:
	//     bDestroyWnd - TRUE to destroy the window associated with the tab item.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL RemoveAllTabs(BOOL bDestroyWnd = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the tab index from the current cursor
	//     position.
	// Parameters:
	//     point - Pointer to a CPoint object that contains the cursor screen coordinates.
	//             Use default for current cursor position.
	// Returns:
	//     An integer based index of the tab; or -1, if no tab is at the specified 'point'.
	//-----------------------------------------------------------------------
	virtual int GetTabFromPoint(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to see if the specified CWnd object is a
	//     child of the tab control.
	// Parameters:
	//     pView - A pointer to a CWnd object.
	// Returns:
	//     TRUE if the specified CWnd object is a child of the tab control, otherwise
	//     returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsChildView(CWnd* pView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text for the specified tab.
	// Parameters:
	//     nTab      - Index of the tab.
	//     pView     - CWnd object associated with the tab.
	//     pViewClass - CRuntimeClass of the CWnd associated with the tab.
	//     lpszLabel - New text for the tab label.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetTabText(int nTab, LPCTSTR lpszLabel);
	BOOL SetTabText(CWnd* pView, LPCTSTR lpszLabel); // <combine CXTPTabExBase::SetTabText@int@LPCTSTR>
	BOOL SetTabText(CRuntimeClass* pViewClass, LPCTSTR lpszLabel); // <combine CXTPTabExBase::SetTabText@int@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to activate the next view in the tab
	//     control.
	// Returns:
	//     A CWnd pointer to the newly activated view.
	//-----------------------------------------------------------------------
	CWnd* NextView();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to activate the previous view in the
	//     tab control.
	// Returns:
	//     A CWnd pointer to the newly activated view.
	//-----------------------------------------------------------------------
	CWnd* PrevView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable the tab auto-condensing
	//     mode. Auto-condensing mode affects the tab control's behavior when
	//     there is not enough room to fit all tabs. Without auto-condensation,
	//     the CXTPTabCtrl control behaves like a standard tab control (i.e. it
	//     will display a slider control that allows the user to pan between tabs).
	//     With the auto-condensing mode enabled, CXTPTabCtrl attempts to fit all
	//     tabs in the available space by trimming the tab label text. This behavior
	//     is similar to the behavior displayed by Visual C++'s Workspace View.
	//     For instance, you can see the FileView tab shrink if you shrink the
	//     Workspace View.
	// Parameters:
	//     bEnable - TRUE to enable auto-condense mode.
	//-----------------------------------------------------------------------
	void SetAutoCondense(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the state of the tab control's auto-condense
	//     mode. See SetAutoCondense() for a full explanation of this mode.
	// Returns:
	//     TRUE if auto-condense is enabled, or FALSE if it is disabled.
	//-----------------------------------------------------------------------
	BOOL GetAutoCondense();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will modify the style for the tab control associated
	//     with this view and set the appropriate font depending on the tab's
	//     orientation.
	// Parameters:
	//     dwRemove - Specifies window styles to be removed during style modification.
	//     dwAdd    - Specifies window styles to be added during style modification.
	//     nFlags   - Flags to be passed to SetWindowPos, or zero if SetWindowPos should
	//                not be called. The default is zero. See CWnd::ModifyStyle for more
	//                details.
	// Returns:
	//     Nonzero if the style was successfully modified, otherwise returns zero.
	//-----------------------------------------------------------------------
	virtual BOOL ModifyTabStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to get the last known view that belongs
	//     to the frame.
	// Returns:
	//     A CView pointer to the last known view.
	//-----------------------------------------------------------------------
	CView* GetLastKnownChildView();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the resource ID for the popup menu
	//     used by the tab control.
	// Parameters:
	//     popupMenuID - ID for the tab control popup menu.
	//     nPos        - Index position in the menu resource.
	//-----------------------------------------------------------------------
	virtual void SetMenuID(UINT popupMenuID, int nPos = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the menu resource associated with the
	//     tab control.
	// Returns:
	//     The resource ID of the menu associated with the tab control.
	//-----------------------------------------------------------------------
	virtual UINT GetMenuID();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to allow WM_INITIALUPATE message to be sent
	//     to views after creation.
	// Parameters:
	//     bInitialUpdate - TRUE to send initial update message.
	//-----------------------------------------------------------------------
	virtual void SendInitialUpdate(BOOL bInitialUpdate);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is called whenever the theme has changed.
	// See Also:
	//      CXTPControlThemeManagerStyleHost::OnThemeChanged(), CXTPHeaderCtrl::OnThemeChanged()
	//-----------------------------------------------------------------------
	virtual void OnThemeChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to handle a TCN_SELCHANGING event in
	//     your derived class to provide additional functionality.
	// See Also:
	//     OnSelChange, TCN_XTP_SELCHANGE, TCN_XTP_SELCHANGING
	//-----------------------------------------------------------------------
	virtual void OnSelChanging();

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to handle a TCN_SELCHANGE event in
	//     your derived class to provide additional functionality.
	// See Also:
	//     OnSelChanging, TCN_XTP_SELCHANGE, TCN_XTP_SELCHANGING
	//-----------------------------------------------------------------------
	virtual void OnSelChange();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the tooltip and tab text for
	//     the specified tab.
	// Parameters:
	//     nTab      - Index of the tab.
	//     pMember   - Address of an CXTPTcbItem struct associated with the tab.
	//     lpszLabel - NULL terminated string that represents the new tab label.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL UpdateTabLabel(int nTab, CXTPTcbItem* pMember, LPCTSTR lpszLabel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates the CWnd object that is associated
	//     with a tab control item.
	// Parameters:
	//     pViewClass - CView runtime class to be created.
	//     pDocument  - CDocument associated with view.
	//     pContext   - Create context for the view.
	// Returns:
	//     A pointer to the newly created CWnd object, otherwise returns NULL.
	//-----------------------------------------------------------------------
	virtual CWnd* CreateTabView(CRuntimeClass *pViewClass, CDocument *pDocument, CCreateContext* pContext);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used internally by the tab control to calculate
	//     the width of a tab based on its label text.
	// Parameters:
	//     pDC      - Points to the current device context.
	//     sLabel   - Represents the tab label text.
	//     bHasIcon - Set to true if the tab item has an icon.
	// Returns:
	//     An integer value that represents the width of a tab.
	//-----------------------------------------------------------------------
	int CalculateTabWidth(CDC* pDC, CString& sLabel, bool bHasIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used internally by the tab control to shrink,
	//     or un-shrink, tabs based on the control's width and the state of the
	//     auto-condensation mode. See SetAutoCondense() for more information.
	//-----------------------------------------------------------------------
	void Condense();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTPTabExBase class to
	//     perform initialization when the window is created or sub-classed.
	// Returns:
	//     TRUE if the window was successfully initialized, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Init();

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnPreWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	virtual void OnPostWindowPosChanged();
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_CODEJOCK_PRIVATE

public:
	CList <CXTPTcbItem*, CXTPTcbItem*> m_tcbItems; // Template list containing tab information.

protected:
	int         m_nPos;             // Index of the popup menu contained in the menu.
	UINT        m_popupMenuID;      // Popup menu resource ID.
	BOOL        m_bInitialUpdate;   // TRUE to send initial update to views when created.
	CWnd*       m_pParentWnd;       // Points to the parent and will equal 'm_pParentFrame' in non-dialog applications.
	CView*      m_pLastActiveView;  // Points to the last active view that belongs to the main frame window.
	CFrameWnd*  m_pParentFrame;     // Points to the parent frame.

private:
	int        m_nOldIndex;
};

//---------------------------------------------------------------------------

AFX_INLINE CWnd* CXTPTabExBase::GetActiveView() {
	return GetView(m_pTabCtrl->GetCurSel());
}
AFX_INLINE CView* CXTPTabExBase::GetLastKnownChildView() {
	return m_pLastActiveView;
}
AFX_INLINE void CXTPTabExBase::SetMenuID(UINT popupMenuID, int nPos) {
	m_popupMenuID = popupMenuID; m_nPos = nPos;
}
AFX_INLINE UINT CXTPTabExBase::GetMenuID() {
	ASSERT(::IsWindow(m_pTabCtrl->GetSafeHwnd())); return m_popupMenuID;
}
AFX_INLINE void CXTPTabExBase::SendInitialUpdate(BOOL bInitialUpdate) {
	m_bInitialUpdate = bInitialUpdate;
}

//===========================================================================

//{{AFX_CODEJOCK_PRIVATE
class _XTP_EXT_CLASS CXTPTabCtrlBaseEx : public CXTPTabExBase // CXTPTabCtrlBaseEx deprecated, use CXTPTabExBase instead (included for backward compatibility).
{
public:
	virtual BOOL EnableToolTipsImpl(BOOL bEnable) {
		return CXTPTabExBase::EnableToolTipsEx(bEnable);
	}
protected:
	void OnRButtonDownImpl(UINT nFlags, CPoint point) {
		CXTPTabExBase::OnRButtonDown(nFlags, point);
	}
	int OnCreateImpl_Post(LPCREATESTRUCT lpCreateStruct) {
		return CXTPTabExBase::OnCreate(lpCreateStruct);
	}
	void OnDestroyImpl_Pre() {
		CXTPTabExBase::OnDestroy();
	}
	BOOL OnSelchangeImpl(NMHDR* pNMHDR, LRESULT* pResult) {
		return CXTPTabExBase::OnSelchange(pNMHDR, pResult);
	}
	BOOL OnSelchangingImpl(NMHDR* pNMHDR, LRESULT* pResult) {
		return CXTPTabExBase::OnSelchanging(pNMHDR, pResult);
	}
	void OnWindowPosChangedImpl_Pre(WINDOWPOS FAR* lpwndpos) {
		CXTPTabExBase::OnPreWindowPosChanged(lpwndpos);
	}
	void OnWindowPosChangedImpl_Post(WINDOWPOS FAR*) {
		CXTPTabExBase::OnPostWindowPosChanged();
	}
	BOOL PreTranslateMessageImpl(MSG* pMsg) {
		return CXTPTabExBase::PreTranslateMessage(pMsg);
	}
	void PreSubclassWindowImpl_Post() {
		CXTPTabExBase::PreSubclassWindow();
	}
	BOOL OnCmdMsgImpl_Pre(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
		return CXTPTabExBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	LRESULT OnInitializeImpl(WPARAM, LPARAM) {
		ASSERT(0); return 0; // this is deprecated, tab is self initializing via Init().
	}
	void OnInitialUpdateImpl() {
		CXTPTabExBase::Init();
	}
};
//{{AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE
#define DECLATE_TABCTRLEX_BASE(ClassName, Tab, Base)\
class _XTP_EXT_CLASS ClassName : public Tab, public Base \
{ \
protected: \
	virtual void PreSubclassWindow() { \
		Tab::PreSubclassWindow(); \
		Base::PreSubclassWindow(); \
	} \
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) {\
		if (!Tab::PreCreateWindow(cs))\
			return FALSE;\
		return Base::PreCreateWindow(cs);\
	}   \
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) { \
		if (Base::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) \
			return TRUE; \
		return Tab::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo); \
	} \
	virtual BOOL PreTranslateMessage(MSG* pMsg) { \
		if (Base::PreTranslateMessage(pMsg)) \
			return TRUE; \
		return Tab::PreTranslateMessage(pMsg); \
	} \
	afx_msg void OnPaint() { \
		Base::OnPaint(); \
	} \
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) { \
		return Base::OnEraseBkgnd(pDC); \
	} \
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection) { \
		Tab::OnSettingChange(uFlags, lpszSection); \
		Base::OnSettingChange(uFlags, lpszSection); \
	} \
	afx_msg void OnSysColorChange() { \
		Tab::OnSysColorChange(); \
		Base::OnSysColorChange(); \
	} \
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct) { \
		if (Tab::OnCreate(lpCreateStruct) == -1) \
			return -1; \
		return Base::OnCreate(lpCreateStruct); \
	} \
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point) { \
		Base::OnRButtonDown(nFlags, point); \
	} \
	afx_msg void OnDestroy() { \
		Base::OnDestroy(); \
		Tab::OnDestroy(); \
	} \
	afx_msg BOOL OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) { \
		return Base::OnSelchange(pNMHDR, pResult); \
	} \
	afx_msg BOOL OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) { \
		return Base::OnSelchanging(pNMHDR, pResult); \
	} \
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) { \
		Base::OnPreWindowPosChanged(lpwndpos); \
		Tab::OnWindowPosChanged(lpwndpos); \
		Base::OnPostWindowPosChanged(); \
	} \
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam) {\
		return Base::OnSetTheme(wParam, lParam);\
	} \
};

#define ON_TABCTRLEX_REFLECT \
	ON_WM_CREATE() \
	ON_WM_DESTROY() \
	ON_WM_RBUTTONDOWN() \
	ON_WM_WINDOWPOSCHANGED() \
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnSelchange) \
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGING, OnSelchanging) \
	ON_TABCTRL_REFLECT
//}}AFX_CODEJOCK_PRIVATE

#endif // __XTTABCTRLBASE_H__
