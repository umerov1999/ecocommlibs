// Workspace.h : header file
//
// This file is a part of the XTREME CALENDAR MFC class library.
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
#if !defined(__XTPTABCLIENTWND_WORKSPACE_H__)
#define __XTPTABCLIENTWND_WORKSPACE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//-----------------------------------------------------------------------
// Summary:
//     CWorkspace is a CXTPTabManager derived class. internal used.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTabClientWnd::CWorkspace : public CXTPTabManager
{

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CWorkspace object
	//-----------------------------------------------------------------------
	CWorkspace();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CWorkspace object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CWorkspace();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the paint manager.
	// Returns:
	//     A pointer to a CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a tab navigation button is clicked.
	// Parameters:
	//     pButton - the tab navigation button that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a tab button is clicked.  This
	//     will select the tab that was clicked.
	// Parameters:
	//     pItem - Pointer to the CXTPTabManagerItem tab that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnItemClick(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before item click
	// Parameters:
	//     pItem - Pointer to item is about to be clicked
	// Returns:
	//     TRUE if cancel process
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeItemClick(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member recalculates the layout of the tab manager and
	//     then repositions itself.  This member must be overridden in
	//     derived classes.
	//-----------------------------------------------------------------------
	virtual void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the icon of the tab needs to be
	//     drawn.
	// Parameters:
	//     pDC    - Pointer to the destination device context.
	//     pt     - Specifies the location of the image.
	//     pItem  - CXTPTabManagerItem object to draw icon on.
	//     bDraw  - TRUE if the icon needs to be drawn, I.e. the icon size
	//              changed.  FALSE if the icon does not need to be
	//              drawn or redrawn.
	//     szIcon - Size of the tab icon.
	// Remarks:
	//     For example, on mouseover.  This member is overridden by its
	//     descendants.  This member must be overridden in
	//     derived classes.
	// Returns:
	//     TRUE if the icon was successfully drawn, FALSE if the icon
	//     was not drawn.
	//-----------------------------------------------------------------------
	virtual BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Initiates redrawing of the control
	// Remarks:
	//     Call this member function if you want to initialize redrawing
	//     of the control. The control will be redrawn taking into account
	//     its latest state.
	// Parameters:
	//     lpRect - The rectangular area of the window that is invalid.
	//     bAnimate - TRUE to animate changes in bounding rectangle.
	//-----------------------------------------------------------------------
	virtual void RedrawControl(LPCRECT lpRect, BOOL bAnimate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select the specified item.
	// Parameters:
	//     pItem - Points to a CXTPTabManagerItem object.
	//-----------------------------------------------------------------------
	virtual void SetSelectedItem(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a CXTPTabManagerItem is clicked
	//     and dragged within the tab header.  This will reorder the
	//     selected tab to the location it is dragged to.
	// Parameters:
	//     hWnd  - Handle to the CWnd object beneath the mouse cursor.
	//     pt - CPoint object specifies xy coordinates.
	//     pItem - Points to a CXTPTabManagerItem object
	//-----------------------------------------------------------------------
	virtual void ReOrder(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks to see if the mouse is locked.
	// Returns:
	//     TRUE if locked; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsMouseLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the tooltip associated with the specified item.
	// Parameters:
	//     pItem - Points to a CXTPTabManagerItem object.
	// Returns:
	//     Associated tooltip
	//-----------------------------------------------------------------------
	virtual CString GetItemTooltip(const CXTPTabManagerItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns whether or not reorder is allowed.
	// Returns:
	//     TRUE when reorder is allowed; FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowReorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows/disallows reorder.
	// Parameters:
	//     bAllowReorder - TRUE for allowing reorder; FALSE for disallowing.
	//-----------------------------------------------------------------------
	virtual void SetAllowReorder(BOOL bAllowReorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if frame must be drawn.
	//-----------------------------------------------------------------------
	virtual BOOL IsDrawStaticFrame() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get window handle of workspace
	//-----------------------------------------------------------------------
	virtual CWnd* GetWindow() const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds the tab item that corresponds to the specified window
	// Parameters:
	//     hWnd - Handle of MDI child window to find.
	// Returns:
	//     Returns a pointer to the specified tab if it was found.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* FindItem(const HWND hWnd) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new tab item for the specified window
	// Parameters:
	//     pChildWnd - Pointer to MDI child window to add.
	// Returns:
	//     Returns a pointer to the newly added tab.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* AddItem(const CWnd* pChildWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the index for the current icon.
	// Parameters:
	//     pItem - Pointer to a CXTPTabManagerItem.
	// Returns:
	//     An icon index for the current item.
	//-----------------------------------------------------------------------
	HICON GetItemIcon(const CXTPTabManagerItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the RGB value for the referenced
	//     item text or (COLORREF)-1, if the color was not set.
	// Parameters:
	//     pItem - Pointer to a CXTPTabManagerItem.
	// Returns:
	//     The RGB value for the referenced item text, or (COLORREF)-1,
	//     if the color was not set
	//-----------------------------------------------------------------------
	COLORREF GetItemColor(const CXTPTabManagerItem* pItem) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to recalculate layout of tab client
	//-------------------------------------------------------------------------
	virtual void OnRecalcLayout();

protected:

	//-----------------------------------------------------------------------
	// Summary: Returns markup context.
	// Returns: Returns markup context.
	//-----------------------------------------------------------------------
	virtual CXTPMarkupContext* GetMarkupContext() const;

	virtual void AddNewWindowItem();

	BOOL DrawParentBackground(CDC* pDC, CRect rc);

protected:
	CXTPTabClientWnd* m_pTabClientWnd;  // Parent tabbed client.
	double m_dHeight;                   // Height of the workspace
	CRect m_rcSplitter;                 // Splitter position.
	CXTPTabManagerItem* m_pNewWindowItem;

	friend class CXTPTabClientWnd;
};

//{{AFX_CODEJOCK_PRIVATE
#endif //#if !defined(__XTPTABCLIENTWND_WORKSPACE_H__)
//}}AFX_CODEJOCK_PRIVATE
