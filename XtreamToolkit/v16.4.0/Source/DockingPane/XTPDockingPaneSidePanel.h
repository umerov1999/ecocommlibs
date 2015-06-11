// XTPDockingPaneSidePanel.h : interface for the CXTPDockingPaneSidePanel class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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
#if !defined(__XTPDOCKINGPANESIDEPANEL_H__)
#define __XTPDOCKINGPANESIDEPANEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPDockingPaneLayout;
class CXTPDockingPaneSidePanel;

//===========================================================================
// Summary:
//     CXTPDockingPaneSidePanel is a multiple inheritance class derived from
//     CWnd and CXTPDockingPaneBase. It is used internally as an auto-hide container
//     for CXTPDockingPaneBase derived classes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneSidePanel : public CMiniFrameWnd, public CXTPDockingPaneBaseContainer
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneAutoHidePanel object
	//     Protected constructor. Internally constructed only.
	// Parameters:
	//     pLayout - Points to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneSidePanel(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneAutoHidePanel object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPDockingPaneSidePanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a CXTPDockingPaneTabbedContainer object that points to the parent container for this container.
	// Returns:
	//     CXTPDockingPaneTabbedContainer object that points to the parent container for this container.
	//-----------------------------------------------------------------------
	CXTPDockingPaneTabbedContainer* GetTopContainer() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the position of the
	//     auto-hide panel.
	// Returns:
	//     The panel's direction.
	//-----------------------------------------------------------------------
	XTPDockingPaneDirection GetDirection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get child pane state.
	// Returns:
	//     TRUE if a child pane is active.
	//-----------------------------------------------------------------------
	BOOL IsActive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the title of the pane.
	// Returns:
	//     The title of the pane.
	//-----------------------------------------------------------------------
	CString GetTitle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the currently selected pane in the side panel.
	// Returns:
	//     Pointer to a CXTPDockingPane object that is the currently selected pane.
	//-----------------------------------------------------------------------
	CXTPDockingPane* GetSelectedPane() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to collapse frame.
	// Parameters:
	//     bDelay - TRUE to collapse after small delay with animation.
	//-----------------------------------------------------------------------
	void Collapse(BOOL bDelay = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if frame is collapsed
	// Returns:
	//     TRUE if collapsed, FALSE if visible.
	//-----------------------------------------------------------------------
	BOOL IsCollapsed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to expand collapsed frame.
	//-----------------------------------------------------------------------
	void Expand();

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the side panel to the desired location.
	// Parameters:
	//     direction   - The panel's direction.
	//     rect - Position of SidePanel.
	//-----------------------------------------------------------------------
	void MovePanel(XTPDockingPaneDirection direction, CRect rect);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneSidePanel object
	//     Protected constructor. Internally constructed only.
	// Parameters:
	//     pPane   - Child pane.
	//     direction - Direction to dock.
	//     rc - Position of SidePanel.
	//-----------------------------------------------------------------------
	BOOL Init(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates frame window.
	// Parameters:
	//     rc - Specifies the size and position of the window.
	// Returns:
	//     Nonzero if initialization is successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL Create(CRect rc);

	//{{AFX_CODEJOCK_PRIVATE
	static void AFX_CDECL OnSizeParentEx(CXTPDockingPaneManager* pManager, CWnd* pParent, CRect rect);
protected:
	void DeletePane();
	void OnFinalRelease();
	void PostNcDestroy();
	void OnFocusChanged();

	void RecalcLayout(BOOL bNotify = TRUE);
	void RemovePane(CXTPDockingPaneBase* pPane);
	void OnChildContainerChanged(CXTPDockingPaneBase* pContainer);
	void CreateContainer();
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies a CXTPDockingPaneSidePanel object
	// Parameters:
	//     pClone  - Points to a CXTPDockingPaneSidePanel object
	//     pMap - Points to a CXTPPaneToPaneMap object
	//     dwIgnoredOptions - Options that must be skipped.
	// See Also:
	//     CXTPPaneToPaneMap
	//-----------------------------------------------------------------------
	void Copy(CXTPDockingPaneBase* pClone, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert a pane into a collection
	// Parameters:
	//     pPane - Points to a CXTPDockingPaneBase object
	//-----------------------------------------------------------------------
	void _InsertPane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to fill the pMinMaxInfo structure with the
	//     minimum and maximum width and height of the splitter container.
	//     This will look at the MinMaxInfo for each pane in the splitter
	//     container and set the appropriate minimum and maximum width
	//     and height for the splitter container.
	// Parameters:
	//     pMinMaxInfo - Pointer to MINMAXINFO structure
	//-----------------------------------------------------------------------
	void GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the slide panel orientation
	//     is horizontal.
	// Returns:
	//     TRUE if the slide panel orientation is horizontal, FALSE
	//     otherwise.
	//-----------------------------------------------------------------------
	BOOL IsHorizontal() const;

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y-coordinate of the cursor. These
	//                  coordinates are always relative to the upper-left
	//                  corner of the window.
	//          pTI - A pointer to a TOOLINFO structure.
	// Summary: The framework calls this member function to determine whether
	//          a point is in the bounding rectangle of the specified tool.
	// Returns: If the tooltip control was found, the window control ID. If
	//          the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	//{{AFX_CODEJOCK_PRIVATE
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Points to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a need to redraw pane.
	// Parameters:
	//     bSelectionChanged - TRUE if selected pane was changed
	//-----------------------------------------------------------------------
	virtual void InvalidatePane(BOOL bSelectionChanged);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves safe window handle.
	// Returns: Safe window handle.
	//-----------------------------------------------------------------------
	HWND GetPaneHwnd() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to animate collapsed frame
	// Parameters:
	//     bActivate - TRUE to activate frame.
	//-----------------------------------------------------------------------
	void DoSlideStep(BOOL bActivate = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the "pin" caption button is clicked.
	//-----------------------------------------------------------------------
	void OnPinButtonClick();


private:
	typedef CArray<CXTPDockingPaneSidePanel*, CXTPDockingPaneSidePanel*> CSidePanelArray;

	static void AFX_CDECL OnSizeParentEx(CSidePanelArray& arrSide, CWnd* pParent, CRect rect);
	static int _cdecl CompareLength(const void *arg1, const void *arg2);

	struct LENGTH;
	static void SortLength(LENGTH* pLength, int nFirstIndex, int nLastIndex);

private:
	CXTPDockingPaneCaptionButton* HitTestCaptionButton(CPoint point) const;
	void OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton);
	void OnCaptionLButtonDown(CPoint point);
	void _RestoreFocus();

	BOOL IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton);
	void MovePanel(LENGTH* pLength, CRect rect);

	int GetMinHeight();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPDockingPaneSidePanel)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPDockingPaneSidePanel)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnMouseHover(WPARAM, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint pt);
	afx_msg void OnRButtonUp(UINT /*nFlags*/, CPoint point);
	//}}AFX_MSG
	BOOL IsResizable(int nHit);

//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
private:
	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPDockingPaneSidePanel);
	DECLARE_ENUM_VARIANT(CXTPDockingPaneSidePanel);
	DECLARE_INTERFACE_MAP()
	LPDISPATCH OleGetDispatch(BOOL /*bAddRef*/);
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetContainer();
	afx_msg int OleGetType();
	afx_msg LPDISPATCH OleGetPane(int nIndex);
	afx_msg HWND OleGetHwnd();
	afx_msg BOOL OleIsEmpty();
	afx_msg int OleGetPosition();
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	XTPDockingPaneDirection m_direction;                          // Location of side panel
	int m_nLengthIndex;

	int m_nStepsCount;          // Total steps of animation process
	int m_nSlideStep;           // Current step of animation process
	int m_nDeactivationCount;   // Deactivation counter.

	BOOL m_bCollapsed;          // Pane is collapsed
	BOOL m_bExpanded;           // Pane is collapsed but expanded now
	BOOL m_bSlideOut;           // Pane is currently expanding.


	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPaneLayout;

};

AFX_INLINE HWND CXTPDockingPaneSidePanel::GetPaneHwnd() const {
	return CWnd::GetSafeHwnd();
}
AFX_INLINE XTPDockingPaneDirection CXTPDockingPaneSidePanel::GetDirection() const {
	return m_direction;
}
AFX_INLINE BOOL CXTPDockingPaneSidePanel::IsActive() const {
	return m_bActive;
}

#endif // #if !defined(__XTPDOCKINGPANESIDEPANEL_H__)
