// XTPSplitterWnd.h : header file
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
#if !defined(__XTPSPLITTERWND_H__)
#define __XTPSPLITTERWND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPSplitterWndTheme;

// --------------------------------------------------------------------
// Summary:
//     CXTPSplitterWnd is a CSplitterWnd derived class. It
//     adds the ability to hide and show splitter panes based upon its
//     index.
// --------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CXTPSplitterWnd)

public:
	// ---------------------------------------------
	// Summary:
	//     Constructs a CXTPSplitterWnd object
	// ---------------------------------------------
	CXTPSplitterWnd();

	// ----------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSplitterWnd object, handles cleanup and
	//     deallocation
	// ----------------------------------------------------------------
	virtual ~CXTPSplitterWnd();

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
	BOOL SetTheme(CXTPSplitterWndTheme* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the currently seleted theme.
	// Returns:
	//     A pointer to a CXTPSplitterWndTheme object representing the currently selected theme.
	//-----------------------------------------------------------------------
	CXTPSplitterWndTheme* GetTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to show the column that was previously
	//     hidden.
	// See Also:
	//     HideColumn
	//-----------------------------------------------------------------------
	virtual void ShowColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will hide a column based upon its index.
	// Parameters:
	//     nColHide -  Index of the column to hide.
	// See Also:
	//     ShowColumn
	//-----------------------------------------------------------------------
	virtual void HideColumn(int nColHide);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to show the row that was previously
	//     hidden.
	// See Also:
	//     HideRow
	//-----------------------------------------------------------------------
	virtual void ShowRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will hide a row based upon its index.
	// Parameters:
	//     nRowHide -  Index of the row to hide.
	// See Also:
	//     ShowRow
	//-----------------------------------------------------------------------
	virtual void HideRow(int nRowHide);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to switch, or swap, a splitter view
	//     with another.
	// Parameters:
	//     nRow -      Specifies a row.
	//     nCol -      Specifies a column.
	//     pNewView -  Specifies the view to switch the specified pane with.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// See Also:
	//     ReplaceView
	//-----------------------------------------------------------------------
	virtual BOOL SwitchView(int nRow, int nCol, CView* pNewView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to replace an existing splitter view
	//     with another.
	// Parameters:
	//     nRow -        Specifies a row.
	//     nCol -        Specifies a column.
	//     pViewClass -  Specifies the runtime class to replace the specified pane with.
	//     pNewView -    Points to an already existing view.
	// Returns:
	//     A CView pointer to the view that was replaced, otherwise returns NULL.
	// See Also:
	//     SwitchView
	//-----------------------------------------------------------------------
	virtual CView* ReplaceView(int nRow, int nCol, CView* pNewView);
	virtual CView* ReplaceView(int nRow, int nCol, CRuntimeClass* pViewClass); //<COMBINE CXTPSplitterWnd::ReplaceView@int@int@CView*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable full drag windows.
	// Parameters:
	//     nFullDrag - 1 to enable full drag, 0 to disable, -1 to use system settings for full drag.
	// Remarks:
	//     This member function will enable / disable the display of windows contents for each child pane
	//     while the splitter is resized if nFullDrag is set to 1.
	//-----------------------------------------------------------------------
	void EnableFullDrag(int nFullDrag);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable dotted tracker display rather than the default.
	// Parameters:
	//     bEnable - TRUE to enable dotted tracker display.
	//-----------------------------------------------------------------------
	void EnableDotTracker(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to show/hide border of splitter
	// Parameters:
	//     bBorder - TRUE to show border; FALSE to hide
	//-----------------------------------------------------------------------
	void EnableBorder(BOOL bBorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if border display has been
	//     disabled for the splitter client area.
	// Returns:
	//     TRUE if border shown, otherwise FALSE if borders are hidden.
	//-----------------------------------------------------------------------
	BOOL IsBorderVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to disable resizing splitter pane windows.
	// Parameters:
	//     bEnable - TRUE to disable splitter resizing.
	//-----------------------------------------------------------------------
	void EnableNoSize(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the index of the column that is
	//     currently hidden.
	// Returns:
	//     The index of the hidden column or -1 if no columns are hidden.
	// See Also:
	//     GetHiddenRowIndex
	//-----------------------------------------------------------------------
	int GetHiddenColIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the index of the row that is
	//     currently hidden.
	// Returns:
	//     The index of the hidden row; or -1, if no rows are hidden.
	// See Also:
	//     GetHiddenColIndex
	//-----------------------------------------------------------------------
	int GetHiddenRowIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a pane to be the active one in the frame.
	// Remarks:
	//     This member function is called by the framework to set a pane as
	//     active when the user changes the focus to a pane within the frame
	//     window. You may explicitly call SetActivePane to change the focus
	//     to the specified view.<p/>
	//
	//     Specify pane by providing either row and column, or by providing
	//     <i>pWnd</i>.
	// Parameters:
	//     row  -  If pWnd is NULL, specifies the row in the pane that will
	//             be active.
	//     col  -  If pWnd is NULL, specifies the column in the pane that
	//             will be active.
	//     pWnd -  A pointer to a CWnd object. If NULL, the pane specified
	//             by row and col is set active.  If not NULL, specifies the
	//             pane that is set active.
	// See Also:
	//     CSplitterWnd::GetActivePane, CSplitterWnd::GetPane, CFrameWnd::SetActiveView
	//-----------------------------------------------------------------------
	virtual void SetActivePane(int row, int col, CWnd* pWnd = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check if full drag windows is enabled.
	// Returns:
	//     TRUE if full drag windows is supported, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL FullDragWindows() const;

//{{AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Note: The following functions are deprecated and could be removed
	// in a future release.
	//-----------------------------------------------------------------------

	void SetFullDrag(BOOL bFullDrag);
	virtual void SetSplitterStyle(DWORD dwxStyle);
	virtual DWORD GetSplitterStyle();
	virtual void EnableFlatLook(BOOL bFlatSplitter);

//}}AFX_CODEJOCK_PRIVATE

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh theme colors and redraw the control.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to draw the client area in the specified
	//     device context, most commonly in a printer device context.
	// Parameters:
	//     pDC - A pointer to the device context to draw in.
	//-----------------------------------------------------------------------
	void PrintClient(CDC* pDC);

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPSplitterWnd)
	virtual void SetSplitCursor(int ht);
	virtual void OnInvertTracker(const CRect& rect);
	virtual void StartTracking(int ht);
	virtual void StopTracking(BOOL bAccept);
	virtual void DrawTracker(const CRect& rect, CBrush* pBrush);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPSplitterWnd)
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnNcHitTest(CPoint point);
//}}AFX_CODEJOCK_PRIVATE

protected:

	int m_nHiddenCol;               // Index of the hidden column.
	int m_nHiddenRow;               // Index of the hidden row.
	int m_nFullDrag;                // 1 to enable full drag, 0 to disable, -1 to use system settings for full drag.
	BOOL m_bFullDrag;               // TRUE if full window dragging is enabled for the operating system.
	BOOL m_bDotTracker;             // TRUE to enable dotted tracker display.
	BOOL m_bNoSize;                 // TRUE to disable splitter resizing.
	BOOL m_bShowBorder;             // TRUE to show a border around client area.
	BOOL m_bClipStyles;             // Clip styles before dragging.
	BOOL m_bSubclassed;             // TRUE if the window was sub-classed.
	CPoint m_point;                 // Previous cursor position.
	CXTPSplitterWndTheme* m_pTheme; // Pointer to the current theme object.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTPSplitterWnd::GetHiddenColIndex() const {
	return m_nHiddenCol;
}
AFX_INLINE int CXTPSplitterWnd::GetHiddenRowIndex() const {
	return m_nHiddenRow;
}
AFX_INLINE CXTPSplitterWndTheme* CXTPSplitterWnd::GetTheme() const {
	return m_pTheme;
}
AFX_INLINE void CXTPSplitterWnd::EnableFullDrag(int nFullDrag) {
	m_nFullDrag = nFullDrag;
}
AFX_INLINE void CXTPSplitterWnd::EnableDotTracker(BOOL bEnable) {
	m_bDotTracker = bEnable;
}
AFX_INLINE void CXTPSplitterWnd::EnableNoSize(BOOL bEnable) {
	m_bNoSize = bEnable;
}
AFX_INLINE void CXTPSplitterWnd::EnableBorder(BOOL bBorder) {
	m_bShowBorder = bBorder;

	if (::IsWindow(m_hWnd))
		RedrawWindow();
}
AFX_INLINE BOOL CXTPSplitterWnd::IsBorderVisible() const {
	return m_bShowBorder;
}

// ---------------------------------------------------------------------
// Summary:
//     CXTPSplitterWndEx is a CXTPSplitterWnd derived class.
//     It can be used with the CXTPShortcutListBox to display a gap
//     between the top of the splitter and the toolbar area.
// ---------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPSplitterWndEx : public CXTPSplitterWnd
{
	DECLARE_DYNAMIC(CXTPSplitterWndEx)

public:

	// -----------------------------------------------
	// Summary:
	//     Constructs a CXTPSplitterWndEx object
	// -----------------------------------------------
	CXTPSplitterWndEx();

	// ------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSplitterWndEx object, handles cleanup and
	//     deallocation
	// ------------------------------------------------------------------
	virtual ~CXTPSplitterWndEx();

public:
	// ------------------------------------------------------------------------------
	// Summary:
	//     Call this member function to show a top border for the splitter
	//     window, similar to Outlook. Enabled by default.
	// Parameters:
	//     bShowTopBorder -  Call this member function to show a top border for the
	//                       splitter window, similar to Outlook. Enabled by default.
	//     cyTopBorderGap -  Call this member function to show a top border for the
	//                       splitter window, similar to Outlook. Enabled by default.
	// ------------------------------------------------------------------------------
	virtual void ShowTopBorder(bool bShowTopBorder = true, int cyTopBorderGap = 7);


protected:

	// -------------------------
	// Summary:
	//     Called to retrieve the client area minus the inside 3D borders.
	// Remarks:
	//     This member function is called by the splitter window to retrieve the
	//     size of the client area minus any 3D borders.
	// Parameters:
	//     rect -  Size of the client area minus borders.
	// -------------------------
	virtual void GetInsideRect(CRect& rect) const;

public:

	// -----------------
	// Summary:
	//     Call to redisplay the splitter window after adjusting row or column size.
	// Remarks:
	//     Call this member function to correctly redisplay the splitter window
	//     after you have adjusted row and column sizes with the SetRowInfo and
	//     SetColumnInfo member functions. If you change row and column sizes as
	//     part of the creation process before the splitter window is visible,
	//     it is not necessary to call this member function.<p/>
	//
	//     The framework calls this member function whenever the user resizes
	//     the splitter window or moves a split.
	// Example:
	//     See the example for CSplitterWnd::SetColumnInfo.
	// -----------------
	virtual void RecalcLayout();

	// ----------------------------
	// Summary:
	//     Renders an image of a split window.
	// Parameters:
	//     pDC     -  A pointer to the device context in which to draw. If pDC is NULL,
	//                then CWnd::RedrawWindow is called by the framework and no split
	//                window is drawn.
	//     nType   -  A value of the enum ESplitType. See remarks section for a list
	//                of values.
	//     rectArg -  A reference to a CRect object specifying the size and shape of
	//                the split windows.
	// Remarks:
	//     <i>nType</i> can be one of the following values:<p/>
	//
	//     * <b>splitBox</b>          The splitter drag box.
	//     * <b>splitBar</b>          The bar that appears between the two split windows.
	//     * <b>splitIntersection</b> The intersection of the split windows. This element
	//                                will not be called when running on Windows 95.
	//     * <b>splitBorder</b>       The split window borders.
	//
	//     This member function is called by the framework to draw and specify
	//     the exact characteristics of a splitter window.<p/>
	//
	//     Override OnDrawSplitter for advanced customization of the imagery for the
	//     various graphical components of a splitter window. The default imagery is
	//     similar to the splitter in Microsoft Works for Windows or Microsoft Windows
	//     95, in that the intersections of the splitter bars are blended together.<p/>
	//
	//     For more on dynamic splitter windows, see "Splitter Windows" in the
	//     articleMultiple Document Types, Views, and Frame Windows in Visual C++
	//     Programmer's Guide, Technical Note 29, and the CSplitterWnd class overview.
	// See Also:
	//      CSplitterWnd::OnInvertTracker
	// ----------------------------
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);

private:
	static void AFX_CDECL DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
		CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar);

	static  void AFX_CDECL LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
		int nMax, int nSize, int nSizeSplitter);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPSplitterWndEx)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	int  m_cyTopBorderGap; // Size in pixels of the top border.
	bool m_bShowTopBorder; // true to draw a top border line.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPSplitterWndEx::ShowTopBorder(bool bShowTopBorder, int cyTopBorderGap) {
	m_bShowTopBorder = bShowTopBorder; m_cyTopBorderGap = cyTopBorderGap;
}


//{{AFX_CODEJOCK_PRIVATE
const DWORD XTP_SPLIT_DOTTRACKER    = 0x0001; // deprecated see EnableDotTracker().
const DWORD XTP_SPLIT_NOFULLDRAG    = 0x0002; // deprecated see EnableFullDrag().
const DWORD XTP_SPLIT_NOBORDER      = 0x0004; // deprecated see EnableBorder().
const DWORD XTP_SPLIT_NOSIZE        = 0x0008; // deprecated see EnableNoSize().
//}}AFX_CODEJOCK_PRIVATE

#endif // !defined(__XTPSPLITTERWND_H__)
