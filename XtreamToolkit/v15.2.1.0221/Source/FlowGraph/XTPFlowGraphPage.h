// XTPFlowGraphPage.h: interface for the CXTPFlowGraphPage class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPFLOWGRAPHPAGE_H__)
#define __XTPFLOWGRAPHPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphControl;
class CXTPFlowGraphNodes;
class CXTPFlowGraphNodeGroups;
class CXTPFlowGraphConnections;
class CXTPFlowGraphDrawContext;
class CXTPPropExchange;
class CXTPFlowGraphNode;
class CXTPFlowGraphSelectedElements;
class CXTPFlowGraphConnection;

// ----------------------------------------------------------------------
//
// Summary:
//     Represents a page in the flow graph.
//
// Remarks:
//     A page is what contains all the elements in the flow graph. The
//     flow graph can contain many pages, each of which can contain
//     nodes, node groups and connections. A page is the first thing that
//     you must add to the flow graph, then you can add elements to the
//     pages.
// ----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphPage : public CXTPCmdTarget
{
	DECLARE_SERIAL(CXTPFlowGraphPage);
public:
	// ------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphPage object.
	// ------------------------------------------
	CXTPFlowGraphPage();

protected:
	// ------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphPage object, handles cleanup and
	//     deallocation.
	// ------------------------------------------------------------
	virtual ~CXTPFlowGraphPage();

public:
	// -----------------------------------------------------------------------
	// Summary:
	//     Gets a collection of all nodes that have been added to this page.
	// Remarks:
	//     Nodes are the main objects that are displayed in the flow graph. In
	//     terms of a database they can be thought of as the tables in the
	//     database. The nodes contain many connection points, which are like
	//     the fields in a table. Finally there are connections that are used
	//     to draw a relationship between the field of two tables.
	// Returns:
	//     A collection of all nodes that have been added to this page.
	// -----------------------------------------------------------------------
	CXTPFlowGraphNodes* GetNodes() const;
	// ----------------------------------------------------------------------
	// Summary:
	//     Gets a collection of all groups that have been added to this page.
	// Remarks:
	//     A group is just a "box" that is used to visually group nodes
	//     together. By default the group will only be shown when a node in
	//     the group is selected, but you can choose to always show the
	//     groups with ShowNodeGroupsAlways.
	// Returns:
	//     A collection of all groups that have been added to this page.
	// ----------------------------------------------------------------------
	CXTPFlowGraphNodeGroups* GetGroups() const;
	// -----------------------------------------------------
	// Summary:
	//     Gets a reference to the parent FlowGraph control.
	// Returns:
	//     A reference to the parent FlowGraph control.
	// -----------------------------------------------------
	CXTPFlowGraphControl* GetControl() const;
	// ----------------------------------------------------------------------
	// Summary:
	//     Gets a collection of all connections that have been added to this
	//     page.
	// Returns:
	//     A collection of all connections that have been added to this page.
	// ----------------------------------------------------------------------
	CXTPFlowGraphConnections* GetConnections() const;

public:
	// --------------------------------
	// Summary:
	//     Sets the ID for this page.
	// Parameters:
	//     nId :  New ID for this page.
	// --------------------------------
	void SetID(int nId);
	// ------------------------------
	// Summary:
	//     Gets the ID for this page.
	// Returns:
	//     The ID for this page.
	// ------------------------------
	int GetID() const;

	// ---------------------------------------------
	// Summary:
	//     Sets the caption of this page.
	// Parameters:
	//     lpszCaption :  New caption for this page.
	// ---------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);
	// ----------------------------------
	// Summary:
	//     Gets the caption of this page.
	// Returns:
	//     The caption of this page.
	// ----------------------------------
	CString GetCaption() const;

public:
	// -----------------------------------------------------------------
	// Summary:
	//     This method is called when the flow graph layout has changed.
	// Remarks:
	//     This is called when the page zoom level, page scroll offset,
	//     window\\node size, window handle, node style, node color, or
	//     connection style is changed.
	// -----------------------------------------------------------------
	virtual void OnGraphChanged();

	// ------------------------------------------------------------------
	// Summary:
	//     This method is called to contents of the page to their correct
	//     positions.
	// Parameters:
	//     pDC :     Pointer to a valid device context.
	//     rcPage :  Bounding rectangle of the page.
	// ------------------------------------------------------------------
	virtual void Reposition(CXTPFlowGraphDrawContext* pDC, CRect rcPage);
	// -------------------------------------------------
	// Summary:
	//     This method is called when the page is drawn.
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// -------------------------------------------------
	virtual void OnDraw(CXTPFlowGraphDrawContext* pDC);


	// ----------------------------------------------------------------------
	// Summary:
	//     Converts the page coordinates of a given point or rectangle on the
	//     display to screen coordinates.
	// Parameters:
	//     lpRect :  Points to a RECT structure or CRect object that contains
	//               the page coordinates to be converted.
	//
	// Remarks:
	//     The PageToScreen member function uses the page coordinates in the
	//     POINT or RECT structure or the CPoint or CRect object pointed to
	//     by lpPoint or lpRect to compute new screen coordinates. It then
	//     replaces the coordinates in the structure with the new
	//     coordinates. The new screen coordinates are relative to the
	//     upper-left corner of the system display.
	//
	//     The PageToScreen member function assumes that the given point or
	//     rectangle is in page coordinates.
	// ----------------------------------------------------------------------
	CRect PageToScreen(LPCRECT lpRect) const;
	// ---------------------------------------------------------------------
	// Summary:
	//     Converts the screen coordinates of a given x\\y coordinate on the
	//     display to page coordinates.
	// Parameters:
	//     y :  y coordinate that contains the y coordinate to be converted.
	//     x :  x coordinate that contains the x coordinate to be converted.
	// Remarks:
	//     The ScreenToPage member function replaces the screen coordinates
	//     given in x\\y coorninates. The new
	//     coordinates are relative to the upper-left corner of the page
	//     area.
	// ---------------------------------------------------------------------
	void ScreenToPage(double& x, double& y) const;
	// -----------------------------------------------------------------------
	// Summary:
	//     Converts the screen coordinates of a given point or rectangle on
	//     the display to page coordinates.
	// Parameters:
	//     point :  Points to a CPoint object or POINT structure that contains
	//              the screen coordinates to be converted.
	// Remarks:
	//     The ScreenToPage member function replaces the screen coordinates
	//     given in lpPoint or lpRect with page coordinates. The new
	//     coordinates are relative to the upper-left corner of the page
	//     area.
	// -----------------------------------------------------------------------
	CPoint ScreenToPage(CPoint point) const;
	// --------------------------------------------------------------------
	// Summary:
	//     Converts the screen coordinates of a given point or rectangle on
	//     the display to page coordinates.
	// Parameters:
	//     lpRect :  Points to a CPoint object or POINT structure that
	//               contains the screen coordinates to be converted.
	// Remarks:
	//     The ScreenToPage member function replaces the screen coordinates
	//     given in lpPoint or lpRect with page coordinates. The new
	//     coordinates are relative to the upper-left corner of the page
	//     area.
	//
	// --------------------------------------------------------------------
	CRect ScreenToPage(LPCRECT lpRect) const;


	// -------------------------------------------------------------------
	// Summary:
	//     Call this method to copy all the currently selected elements to
	//     the flow graphs clipboard.
	// -------------------------------------------------------------------
	virtual void Copy();
	// -------------------------------------------------------------
	// Summary:
	//     Call this method to paste the contents of the flow graphs
	//     clipboard to the currently active page.
	// -------------------------------------------------------------
	virtual void Paste();
	// ----------------------------------------------------------------------
	// Summary:
	//     Specifies whether the flow control can perform a paste operation.
	// Remarks:
	//     There must be something in the control's clipboard from a previous
	//     copy\\cut before a paste operation can be performed.
	// Returns:
	//     True if the flow control can perform a paste operation.
	// ----------------------------------------------------------------------
	virtual BOOL CanPaste();
	// -------------------------------------------------------------------
	// Summary:
	//     Call this method to copy all the currently selected elements to
	//     the flow graphs clipboard and then remove the selected contents
	//     from the active page.
	// -------------------------------------------------------------------
	virtual void Cut();

	// ------------------------------------------
	// Summary:
	//     Removes this page from the flow graph.
	// ------------------------------------------
	void Remove();

	// ------------------------------------------------------------------
	// Summary:
	//     Auto-Arranges the nodes in this page.
	// Remarks:
	//     This will left-align all nodes in this page, nodes with
	//     connections will be placed to the right of the connected node.
	// ------------------------------------------------------------------
	virtual void Arrange();

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Gets a reference to the collection of selected elements in the
	//     Flow Graph in this page.
	// Remarks:
	//     Selected elements will include Nodes, Connections and Connection
	//     Points. This collection can be used to add or remove elements
	//     to\\from the selection. Elements can be selected with the mouse or
	//     using methods in code.
	// Returns:
	//     A reference to the collection of selected elements in the Flow
	//     Graph in this page.
	// ----------------------------------------------------------------------
	CXTPFlowGraphSelectedElements* GetSelection() const;

public:
	// ----------------------------------------------
	// Summary:
	//     Gets the bounding rectangle for this page.
	// Returns:
	//     The bounding rectangle for this page.
	// ----------------------------------------------
	CRect GetPageRect() const;
	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the scroll offset (how much the page has been scrolled). This
	//     \returns the X/Y coordinates - left/right offset.
	// Returns:
	//     The scroll offset (how much the page has been scrolled). This
	//     \returns a point that contains the X/Y coordinates - left/right
	//     \offset.
	// ----------------------------------------------------------------------
	CPoint GetScrollOffset() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the scroll offset (how much the page has been scrolled). This
	//     \returns the X coordinate - left/right offset.
	// Parameters:
	//     ptScrollOffset :  Point to scroll to.
	// ----------------------------------------------------------------------
	void SetScrollOffset(CPoint ptScrollOffset);

	// --------------------------------------------
	// Summary:
	//     Gets the current zoom level of the page.
	// Remarks:
	//     0.0 is 0 zoom and 1.0 is 100% zoom.
	// Returns:
	//     The current zoom level of the page.
	//
	//
	// --------------------------------------------
	double GetZoomLevel() const;
	// ------------------------------------------------
	// Summary:
	//     Sets the current zoom level of the page.
	// Parameters:
	//     dZoomLevel :  Zoom level to set the page to.
	//
	// Remarks:
	//     0.0 is 0 zoom and 1.0 is 100% zoom.
	// ------------------------------------------------
	void SetZoomLevel(double dZoomLevel);

protected:
	// ---------------------------------------------------
	// Summary:
	//     This method is called when the page is removed.
	// ---------------------------------------------------
	virtual void OnRemoved();

public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a page using the specified
	//     data object.
	// Parameters:
	//     pPX :  Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load page data to or from
	//     a storage.
	// ------------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

private:
//{{AFX_CODEJOCK_PRIVATE
	void SetArrangeLevel(int v, CArray<CDWordArray*, CDWordArray*>& graph);
	int SetArrangeLocation(int v, int x, int y, int nLevelWidth, CArray<CDWordArray*, CDWordArray*>& graph);
//}}AFX_CODEJOCK_PRIVATE

public:
	static CLIPFORMAT m_cfNode; // Clipboard format.

protected:
	CXTPFlowGraphControl* m_pControl; // Pointer to the parent flow graph control.
	CXTPFlowGraphNodes* m_pNodes; // Collection of nodes that are displayed on this page.
	CXTPFlowGraphNodeGroups* m_pGroups; // Collection of groups that are displayed on this page.
	CXTPFlowGraphConnections* m_pConnections; // Collection of connections that are used in this page.
	CXTPFlowGraphSelectedElements* m_pSelectedElements; // Colleciton of elements that are used in this page.

	CXTPFlowGraphNode* m_pFirstVisibleNode; // Pointer to the first visible node in the page.
	CXTPFlowGraphConnection* m_pFirstVisibleConnection; // Pointer to the first visible connection in the page.

	CRect m_rcPage; // Bounding rectangle for this page.

	CRect m_rcWorkRect; // Rectangle for the currently visible portion of the page (it might be zoomed or scrolled).

	CPoint m_ptScrollOffset; // The scroll offset (how much the page has been scrolled).  This returns the X and Y coordinates - up/down offset.
	double m_dZoomLevel; // Current zoom level of the page.

	int m_nId; // Id of the page.
	CString m_strCaption; // Caption of the page.

	friend class CXTPFlowGraphPages;
	friend class CXTPFlowGraphControl;


};


AFX_INLINE CXTPFlowGraphSelectedElements* CXTPFlowGraphPage::GetSelection() const {
	return m_pSelectedElements;
}

AFX_INLINE CXTPFlowGraphNodes* CXTPFlowGraphPage::GetNodes() const {
	return m_pNodes;
}
AFX_INLINE CXTPFlowGraphNodeGroups* CXTPFlowGraphPage::GetGroups() const {
	return m_pGroups;
}
AFX_INLINE CXTPFlowGraphConnections* CXTPFlowGraphPage::GetConnections() const {
	return m_pConnections;
}
AFX_INLINE CXTPFlowGraphControl* CXTPFlowGraphPage::GetControl() const {
	return m_pControl;
}
AFX_INLINE CRect CXTPFlowGraphPage::GetPageRect() const {
	return m_rcPage;
}
AFX_INLINE CPoint CXTPFlowGraphPage::GetScrollOffset() const {
	return m_ptScrollOffset;
}
AFX_INLINE void CXTPFlowGraphPage::SetScrollOffset(CPoint ptScrollOffset) {
	m_ptScrollOffset = ptScrollOffset;
	OnGraphChanged();
}
AFX_INLINE void CXTPFlowGraphPage::SetID(int nId) {
	m_nId = nId;
}
AFX_INLINE int CXTPFlowGraphPage::GetID() const {
	return m_nId;
}
AFX_INLINE  CString CXTPFlowGraphPage::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE  void CXTPFlowGraphPage::SetCaption(LPCTSTR lpszCaption) {
	m_strCaption = lpszCaption;
}
AFX_INLINE  double CXTPFlowGraphPage::GetZoomLevel() const {
	return m_dZoomLevel;
}
AFX_INLINE  void CXTPFlowGraphPage::SetZoomLevel(double dZoomLevel) {
	m_dZoomLevel = dZoomLevel;
	OnGraphChanged();
}



#endif //#if !defined(__XTPFLOWGRAPHPAGE_H__)
