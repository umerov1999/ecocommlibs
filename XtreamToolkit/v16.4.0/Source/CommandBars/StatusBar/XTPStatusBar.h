// XTPStatusBar.h : interface for the CXTPStatusBar class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSTATUSBAR_H__)
#define __XTPSTATUSBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPCommandBars;
class CXTPPaintManager;
class CXTPStatusBar;
class CXTPStatusBarPane;
class CXTPStatusBarPaintManager;
class CXTPImageManagerIcon;
class CXTPImageManager;
class CXTPToolTipContext;
class CXTPPopupBar;
class CXTPPropExchange;

class CXTPMarkupUIElement;
class CXTPMarkupContext;



//===========================================================================
// Summary:
//     CXTPStatusBar is a CControlBar derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBar : public CControlBar
{
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPStatusBar)
//}}AFX_CODEJOCK_PRIVATE

private:
	class CStatusCmdUI;
	class CControlCustomization;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBar object
	//-----------------------------------------------------------------------
	CXTPStatusBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a status bar (a child window) and associates it with the CStatusBar object.
	//     Also sets the initial font and sets the status bar's height to a default value.
	// Parameters:
	//     pParentWnd - Pointer to the CWnd object whose Windows window is the parent of the status bar.
	//     dwStyle    - The status-bar style. In addition to the standard Windows styles, these styles are supported
	//     nID        - The toolbar's child-window ID
	// Returns:
	//     Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, UINT nID = AFX_IDW_STATUS_BAR);
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets each indicator's ID to the value specified by the corresponding
	//     element of the array lpIDArray, loads the string resource specified
	//     by each ID, and sets the indicator's text to the string.
	// Parameters:
	//     lpIDArray - Pointer to an array of IDs.
	//     nIDCount  - Number of elements in the array pointed to by lpIDArray.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL SetIndicators (const UINT* lpIDArray, int nIDCount);

	CXTPCommandBars* GetCommandBars() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the command bar's object.
	// Parameters:
	//     pCommandBars - CXTPCommandBars to be set.
	//-----------------------------------------------------------------------
	void SetCommandBars(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the paint manager.
	// Returns:
	//     A pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the status bar paint manager.
	// Returns:
	//     A pointer to a CXTPStatusBarPaintManager object.
	//-----------------------------------------------------------------------
	CXTPStatusBarPaintManager* GetStatusBarPaintManager() const;

public:

	BOOL IsAllCaps() const;

	void SetAllCaps(BOOL bAllCaps);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the status bar to reposition pane
	//     item controls.
	//-----------------------------------------------------------------------
	void PositionControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a control to an indicator pane.
	// Parameters:
	//     pWnd        - Points to a control window.
	//     nID         - ID of the indicator pane.
	//     bAutoDelete - TRUE if the control is to be deleted when destroyed.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL AddControl(CWnd* pWnd, UINT nID, BOOL bAutoDelete = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the width for an indicator pane.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	//     cxText - New width for the indicator pane.
	//-----------------------------------------------------------------------
	void SetPaneWidth(int nIndex, int cxText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add an indicator pane to the status bar.
	// Parameters:
	//     nID    - ID of the indicator pane.
	//     nIndex - Index of the indicator pane.
	//     pPane  - New CXTPStatusBarPane object
	// Returns:
	//     CXTPStatusBarPane pointer if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane* AddIndicator(UINT nID, int nIndex = -1);
	CXTPStatusBarPane* AddIndicator(CXTPStatusBarPane* pPane, UINT nID, int nIndex = -1);  // <combine CXTPStatusBar::AddIndicator@UINT@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove an indicator pane from the status bar.
	// Parameters:
	//     nID - ID of the indicator pane.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveIndicator(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the number of panes that are created
	//     for the status bar.
	// Returns:
	//     An integer value that represents the number of panes for the status bar.
	//-----------------------------------------------------------------------
	int GetPaneCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates total number of visible panes.
	// Returns:
	//     Total number of panes currently visible.
	//-----------------------------------------------------------------------
	int GetVisiblePaneCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the pane information for the given index.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	// Returns:
	//     A pointer to an CXTPStatusBarPane class.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane* GetPane(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to find the pane for the given Identifier.
	// Parameters:
	//     nID - Identifier of the indicator pane.
	// Returns:
	//     A pointer to an CXTPStatusBarPane class.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane* FindPane(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove all indicators.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove an indicator pane from the status bar.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the image manager of the image manager.
	// Returns:
	//     A pointer to a CXTPImageManager object
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine which pane, if any, is at the
	//     specified screen position.
	// Parameters:
	//     pt - Point to be tested.
	//     lpRect - Returns rectangle of the item under point pt.
	// Returns:
	//     CXTPStatusBarPane pointer, or returns NULL if no pane is
	//     at the specified position.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane* HitTest(CPoint pt, CRect* lpRect = NULL) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable customization of the statusbar
	// Parameters:
	//     bEnable - TRUE to enable customization.
	//-----------------------------------------------------------------------
	void EnableCustomization(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Input:   lpszProfileName - Points to a null-terminated string that
	//                            specifies the name of a section in the
	//                            initialization file or a key in the Windows
	//                            registry where state information is stored.
	// Summary: Call this function to retrieve state information from the
	//          registry or .INI file.
	//-----------------------------------------------------------------------
	void LoadState(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Input:   lpszProfileName - Points to a null-terminated string that
	//                            specifies the name of a section in the
	//                            initialization file or a key in the Windows
	//                            registry where state information is stored.
	// Summary: Call this function to save the state information to the
	//          registry or .INI file
	//-----------------------------------------------------------------------
	void SaveState(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw disabled text.
	// Parameters:
	//     bDraw - TRUE to draw disabled text, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetDrawDisabledText(BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if paint manager draw disabled text.
	//-----------------------------------------------------------------------
	BOOL GetDrawDisabledText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get tooltip context pointer.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single pane of the status bar.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     nIndex - Index of the pane to draw.
	//     rcItem - Bounding rectangle of the pane
	//-----------------------------------------------------------------------
	virtual void DrawPaneEntry(CDC* pDC, int nIndex, CRect rcItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a status bar
	// Parameters:
	//     pParentWnd - Pointer to the CWnd object whose Windows window is the parent of the status bar.
	//     dwCtrlStyle - Additional styles for the creation of the embedded CStatusBarCtrl object.
	//     dwStyle - The status-bar style
	//     nID - The status bar's child-window ID.
	// Returns: Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = 0,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
		UINT nID = AFX_IDW_STATUS_BAR);


	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the indicator index for a given ID. The index of the first indicator is 0
	// Parameters:
	//      nIDFind - String ID of the indicator whose index is to be retrieved
	// Returns:
	//      The index of the indicator if successful; -1 if not successful.
	// See Also: GetItemID
	//-----------------------------------------------------------------------
	int CommandToIndex(UINT nIDFind) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the ID of the indicator specified by nIndex
	// Parameters:
	//     nIndex - Index of the indicator whose ID is to be retrieved.
	// Returns:
	//     The ID of the indicator specified by nIndex.
	// See Also: CommandToIndex
	//-----------------------------------------------------------------------
	UINT GetItemID(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies the coordinates of the indicator specified by nIndex into the structure pointed to by lpRect.
	//     Coordinates are in pixels relative to the upper-left corner of the status bar.
	// Parameters:
	//     nIndex - Index of the indicator whose rectangle coordinates are to be retrieved.
	//     lpRect - Points to aRECT structure or a CRect object that will receive the coordinates of the indicator specified by nIndex.
	// See Also: GetPaneInfo, CommandToIndex
	//-----------------------------------------------------------------------
	void GetItemRect(int nIndex, LPRECT lpRect) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the text that appears in a status-bar pane.
	//     The second form of this member function fills a CString object with the string text.
	// Parameters:
	//     nIndex  - Index of the pane whose text is to be retrieved
	//     rString - A reference to a CString object that contains the text to be retrieved.
	// See Also: SetPaneText
	//-----------------------------------------------------------------------
	CString GetPaneText(int nIndex) const;
	void GetPaneText(int nIndex, CString& rString) const; // <combine CXTPStatusBar::GetPaneText@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the pane text to the string pointed to by lpszNewText.
	// Parameters:
	//     nIndex - Index of the pane whose text is to be set.
	//     lpszNewText - Pointer to the new pane text
	//     bUpdate - If TRUE, the pane is invalidated after the text is set.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// See Also: GetPaneText
	//-----------------------------------------------------------------------
	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets nID, nStyle, and cxWidth to the ID, style, and width of the indicator
	//     pane at the location specified by nIndex.
	// Parameters:
	//     nIndex  - Index of the pane whose information is to be retrieved.
	//     nID     - Reference to a UINT that is set to the ID of the pane
	//     nStyle  - Reference to a UINT that is set to the style of the pane.
	//     cxWidth - Reference to an integer that is set to the width of the pane
	// See Also: SetPaneInfo, GetItemRect
	//-----------------------------------------------------------------------
	void GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the specified indicator pane to a new ID, style, and width.
	// Parameters:
	//     nIndex  - Index of the indicator pane whose style is to be set.
	//     nID     - New ID for the indicator pane
	//     nStyle  - New style for the indicator pane
	//     cxWidth - New width for the indicator pane
	// See Also: GetPaneInfo
	//-----------------------------------------------------------------------
	void SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the style of a status bar's pane. A pane's style determines how the pane appears.
	// Parameters:
	//     nIndex - Index of the pane whose style is to be retrieved.
	// Returns:
	//     The style of the status-bar pane specified by nIndex
	// See Also: SetPaneStyle
	//-----------------------------------------------------------------------
	UINT GetPaneStyle(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the style of a status bar's pane. A pane's style determines how the pane appears.
	// Parameters:
	//     nIndex - Index of the pane whose style is to be set.
	//     nStyle - Style of the pane whose style is to be set
	// See Also: GetPaneStyle
	//-----------------------------------------------------------------------
	void SetPaneStyle(int nIndex, UINT nStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function allows direct access to the underlying common control
	// Returns:
	//     Contains a reference to a CStatusBarCtrl object.
	//-----------------------------------------------------------------------
	CStatusBarCtrl& GetStatusBarCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set font for the status bar.
	// Parameters:
	//      pFont - Font to set
	//      bRedraw - TRUE to redraw status bar
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Call this method to get font for the status bar.
	// Returns: Font used for the status bar.
	//-----------------------------------------------------------------------
	CFont* GetFont();

	//-----------------------------------------------------------------------
	// Input:   nIndex - Position of the ribbon bar divider.
	// Summary: Sets the position of the ribbon divider index.  This specifies
	//          the panes that get the second color for the ribbon theme.
	// Remarks: RibbonDividerIndex is a zero based index and specifies the
	//          panes that get the second color for the ribbon theme.  All
	//          panes with an index larger than the divider index will get
	//          the second ribbon color applied.
	//-----------------------------------------------------------------------
	void SetRibbonDividerIndex(int nIndex);

	//-----------------------------------------------------------------------
	// Summary: Gets the position of the ribbon divider index
	// Returns: Position of the ribbon bar divider.
	//-----------------------------------------------------------------------
	int GetRibbonDividerIndex() const;

	//-----------------------------------------------------------------------
	// Input:   bEnableMarkup - TRUE to enable markup, FALSE to disable markup.
	// Summary: Allows the use of a XAML snippet to be used for the Text of
	//          panes in the Status Bar.
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Returns markup context.
	// Returns: Returns markup context.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary: Call this method to reposition all panes
	//-----------------------------------------------------------------------
	virtual void RecalcLayout();

//{{AFX_CODEJOCK_PRIVATE
	// Obsolete
	void UpdateAllPanes(BOOL bUpdateRects, BOOL bUpdateText);

#if _MSC_VER > 1100
	void SetBorders(LPCRECT lpRect);
	void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);
#endif
//}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the stretch gripper of the status bar.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcClient - Client rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawStretch(CDC* pDC, CRect& rcClient);

protected:
//{{AFX_CODEJOCK_PRIVATE
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	void CalcInsideRect(CRect& rect, BOOL bHorz) const;
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	void EnableDocking(DWORD dwDockStyle);
#endif

public:
	virtual void OnPaneClick(CXTPStatusBarPane* pPane);
	virtual void OnSwitchPaneClick(CXTPStatusBarPane* pPane, long Id);

	virtual CXTPPopupBar* GetStatusBarPopup();

//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//{{AFX_MSG(CXTPStatusBar)
	public:
	afx_msg void OnPaint();
	afx_msg void OnDraw(CDC* pDC);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg LRESULT OnIdleUpdateCmdUI (WPARAM wParam, LPARAM lParam);
	protected:
	afx_msg LRESULT OnNcHitTest(CPoint);
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg void OnNcPaint();
	afx_msg void OnWindowPosChanging(LPWINDOWPOS);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMinHeight(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void HandleMouseMove(CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL IsWin4();

private:
	DISABLE_WNDCREATE()
	DISABLE_WNDCREATEEX()

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_ENUM_VARIANT(CXTPStatusBar);
	long OleGetItemCount();
	LPDISPATCH OleGetItem(int nItem);

public:
	CString m_strIdleText;

// Implementation
protected:
	void OnFinalRelease ();

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPStatusBar);
	DECLARE_CONNECTION_EX(CXTPStatusBar)

	LPFONTDISP OleGetFont();
	void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg LPDISPATCH OleHitTest(OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y);
	afx_msg long OleGetMinHeight();
	afx_msg void OleSetMinHeight(int nMinHeight);
	afx_msg BOOL OleGetVisible();
	afx_msg void OleSetVisible(BOOL bNewValue);
	afx_msg void OleOnIdleTextChanged();
	afx_msg LPDISPATCH OleAddPane(long nID);
	afx_msg LPDISPATCH OleAddSwitchPane(long nID);
	afx_msg LPDISPATCH OleAddProgressPane(long nID);
	afx_msg LPDISPATCH OleAddSliderPane(long nID);
	afx_msg LPDISPATCH OleGetToolTipContext();
	afx_msg LPDISPATCH OleFindPane(long nID);
	afx_msg LPDISPATCH OleInsertPane(int nIndex, int nID);
	afx_msg void OleSetPaneText(long nIndex, LPCTSTR strText);
	afx_msg void OleSetPaneWidth(long nIndex, long nWidth);
	afx_msg void OleSetPaneStyle(long nIndex, long nStyle);

	afx_msg void OleSetShowSizeGripper(BOOL bGripper);
	afx_msg BOOL OleGetShowSizeGripper();

	afx_msg void OleSetEnableMarkup(BOOL bEnableMarkup);
	afx_msg BOOL OleGetEnableMarkup();
	afx_msg OLE_HANDLE OleGetHwnd();

	afx_msg void OleSetAllCaps(BOOL bAllCaps);
	afx_msg BOOL OleIsAllCaps();

public:
	void FireMouseDown(short nButton, short nShiftState, OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
		{ FireEvent(DISPID_MOUSEDOWN,
		EVENT_PARAM(VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS), nButton, nShiftState, x, y); }
	void FireMouseUp(short nButton, short nShiftState, OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
		{ FireEvent(DISPID_MOUSEUP, EVENT_PARAM(VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS),
		nButton, nShiftState, x, y); }
	void FirePaneDblClick(LPDISPATCH lpDispatch)
		{ FireEvent(1, EVENT_PARAM(VTS_DISPATCH), lpDispatch); }
	void FirePaneClick(LPDISPATCH lpDispatch)
		{ FireEvent(2, EVENT_PARAM(VTS_DISPATCH), lpDispatch); }
	void FireSwitchPaneClick(LPDISPATCH lpDispatch, long Id)
		{ FireEvent(3, EVENT_PARAM(VTS_DISPATCH VTS_I4), lpDispatch, Id); }
	void FireSliderPaneClick(LPDISPATCH lpDispatch, long Command, long Pos)
		{ FireEvent(4, EVENT_PARAM(VTS_DISPATCH VTS_I4 VTS_I4), lpDispatch, Command, Pos); }

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CArray <CXTPStatusBarPane*, CXTPStatusBarPane*> m_arrPanes;  // Array of panes
	int m_nMinHeight;                           // Minimum client height
	CFont m_fontStatus;                         // Font of status bar

	BOOL m_bShowSizeGripper;                    // TRUE to show size gripper
	BOOL m_bDrawDisabledText;                   // TRUE to draw disabled text
	BOOL m_bAllCaps;

	CXTPStatusBarPane* m_pHighlightedPane;      // Pane currently highlighted
	int m_nRibbonDividerIndex;                  // Position of dark part for Ribbon Theme
	CXTPMarkupContext* m_pMarkupContext;        // Markup Context

	int m_cxPaneSpacing;            // Gap between panes
	int m_bCustomizable;            // TRUE to allow customize status bar.
private:
	CXTPCommandBars* m_pCommandBars;
	CXTPToolTipContext* m_pToolTipContext;          // Tooltip Context.

	friend class CXTPStatusBarPane;
};

AFX_INLINE BOOL CXTPStatusBar::IsAllCaps() const
{
	return m_bAllCaps;
}

AFX_INLINE void CXTPStatusBar::SetAllCaps(BOOL bAllCaps)
{
	m_bAllCaps = bAllCaps;
}

AFX_INLINE void CXTPStatusBar::SetRibbonDividerIndex(int nIndex)
{
	m_nRibbonDividerIndex = nIndex;
}

AFX_INLINE int CXTPStatusBar::GetRibbonDividerIndex() const
{
	return m_nRibbonDividerIndex;
}

AFX_INLINE CXTPCommandBars* CXTPStatusBar::GetCommandBars() const
{
	return m_pCommandBars;
}

AFX_INLINE void CXTPStatusBar::SetCommandBars(CXTPCommandBars* pCommandBars)
{
	m_pCommandBars = pCommandBars;
}

AFX_INLINE int CXTPStatusBar::GetPaneCount() const {
	return (int)m_arrPanes.GetSize();
}
AFX_INLINE CStatusBarCtrl& CXTPStatusBar::GetStatusBarCtrl() const{
	return *(CStatusBarCtrl*)this;
}

#if _MSC_VER > 1100
AFX_INLINE void CXTPStatusBar::SetBorders(int cxLeft, int cyTop, int cxRight, int cyBottom){
	ASSERT(cyTop >= 2); CControlBar::SetBorders(cxLeft, cyTop, cxRight, cyBottom);
}AFX_INLINE void CXTPStatusBar::SetBorders(LPCRECT lpRect) {
	SetBorders(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}
#endif

#ifdef _DEBUG
// status bars do not support docking
AFX_INLINE void CXTPStatusBar::EnableDocking(DWORD) {
	ASSERT(FALSE);
}
#endif

AFX_INLINE void CXTPStatusBar::SetDrawDisabledText(BOOL bDraw) {
	m_bDrawDisabledText = bDraw;
}
AFX_INLINE BOOL CXTPStatusBar::GetDrawDisabledText() const {
	return m_bDrawDisabledText;
}
AFX_INLINE CXTPToolTipContext* CXTPStatusBar::GetToolTipContext() const {
	return m_pToolTipContext;
}
AFX_INLINE CXTPMarkupContext* CXTPStatusBar::GetMarkupContext() const {
	return m_pMarkupContext;
}


#endif //#if !defined(__XTPSTATUSBAR_H__)
