// XTPFlowGraphNode.h: interface for the CXTPFlowGraphNode class.
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
#if !defined(__XTPFLOWGRAPHNODE_H__)
#define __XTPFLOWGRAPHNODE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPFlowGraphPage;
class CXTPFlowGraphDrawContext;
class CXTPFlowGraphControl;
class CXTPFlowGraphConnectionPoints;
class CXTPFlowGraphConnectionPoint;
class CXTPFlowGraphNodeGroup;
class CXTPPropExchange;
class CXTPFlowGraphImage;
class CXTPMarkupUIElement;

// -----------------------------------------------------------------------
//
// Summary:
//     Represents a node object.
//
// Remarks:
//     Nodes are the main objects that are displayed in the flow graph. In
//     terms of a database they can be thought of as the tables in the
//     database. The nodes contain many connection points, which are like
//     the fields in a table. Finally there are connections that are used
//     to draw a relationship between the field of two tables.
// -----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphNode : public CXTPFlowGraphElement
{
	DECLARE_SERIAL(CXTPFlowGraphNode);
public:
	// ------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphNode object.
	// ------------------------------------------
	CXTPFlowGraphNode();
	// ------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphNode object, handles cleanup and
	//     deallocation.
	// ------------------------------------------------------------
	virtual ~CXTPFlowGraphNode();

public:
	// -------------------------------------------------------------------
	// Summary:
	//     Sets the caption that is displayed for this node.
	// Parameters:
	//     lpszCaption :  Caption to displayed for this node.
	//
	// Remarks:
	//     The node's caption will be displayed at the top of the node and
	//     its background color is the same color as the node, which is by
	//     default a different color than the connection points that get
	//     added to the node.
	// -------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);
	// -------------------------------------------------------------------
	// Summary:
	//     Gets the caption that is displayed for this node.
	// Remarks:
	//     The node's caption will be displayed at the top of the node and
	//     its background color is the same color as the node, which is by
	//     default a different color than the connection points that get
	//     added to the node.
	// Returns:
	//     The caption that is displayed for this node.
	// -------------------------------------------------------------------
	CString GetCaption() const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip for this node.
	// Parameters:
	//     lpszTooltip :  Tooltip for this node.
	// Remarks:
	//     This is the text that is displayed when the mouse is hovered over
	//     the node.
	// ---------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);
	// ---------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip for this node.
	// Remarks:
	//     This is the text that is displayed when the mouse is hovered over
	//     the node.
	// Returns:
	//     The tooltip for this node.
	// ---------------------------------------------------------------------
	CString GetTooltip() const;

	// ----------------------------------
	// Summary:
	//     Sets the color of the node.
	// Parameters:
	//     clr :  Color to make the node.
	//
	// ----------------------------------
	void SetColor(COLORREF clr);
	// -----------------------------------
	// Summary:
	//     Gets the color of the node.
	// Returns:
	//     \Returns the color of the node.
	// -----------------------------------
	COLORREF GetColor() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Reserved for future use. Currently can be used as a "tag" property
	//     if needed.
	// ----------------------------------------------------------------------
	void SetStyle(int nStyle);
	// ----------------------------------------------------------------------
	// Summary:
	//     Reserved for future use. Currently can be used as a "tag" property
	//     if needed.
	// ----------------------------------------------------------------------
	int GetStyle() const;

	// ---------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the node.
	// Parameters:
	//     dwTag :  Contains the new value to associate with the node.
	//
	// Remarks:
	//     This is used to store any arbitrary value needed to be associated
	//     with the node.
	// ---------------------------------------------------------------------
	void SetTag(DWORD_PTR dwTag);
	// ---------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the node.
	// Remarks:
	//     The Tag is used to store any arbitrary value needed to be
	//     associated with the node.
	// Returns:
	//     The 32-bit value associated with the node.
	// ---------------------------------------------------------------
	DWORD_PTR GetTag() const;

	// -------------------------------------------------------
	// Summary:
	//     Sets the ID used to identify this node.
	// Parameters:
	//     nId :  New id for this node.
	//
	// -------------------------------------------------------
	void SetID(int nId);
	// -------------------------------------------------------
	// Summary:
	//     Gets the ID used to identify this node.
	// Returns:
	//     The ID used to identify this node.
	// -------------------------------------------------------
	int GetID() const;

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member to associate an object with the node. This will
	//     tell the node what to display. I.e. The HWND of a dialog.
	// Parameters:
	//     hWnd :  HWND of object that should be displayed in this node.
	// --------------------------------------------------------------------
	void SetWindowHandle(HWND hWnd);
	// ----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the HWND of the object associated with the
	//     node. This is the HWND of the object that is displayed in the
	//     node.
	// Returns:
	//     HWND of object associated with the node.
	// ----------------------------------------------------------------------
	HWND GetWindowHandle() const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Sets the size of the window for the external control that is
	//     attached to the node (if applicable).
	// Parameters:
	//     sz :  New size of the window.
	// Remarks:
	//     This will set the size for the attached external control (i.e. if
	//     a tree control is attached to the node). You would attach the
	//     external control using the SetWindowHandle method.
	// ---------------------------------------------------------------------
	void SetWindowSize(CSize sz);
	// ---------------------------------------------------------------------
	// Summary:
	//     Gets the size of the window for the external control that is
	//     attached to the node (if applicable).
	// Remarks:
	//     This will get the size for the attached external control (i.e. if
	//     a tree control is attached to the node). This is the size of the
	//     window of the control that was attached using the SetWindowHandle
	//     method.
	// Returns:
	//     Size of the window for the external control that is attached to
	//     the node .
	// ---------------------------------------------------------------------
	CSize GetWindowSize() const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Sets the ID of the Image to display next to the caption text of
	//     the node.
	// Parameters:
	//     nImageIndex :  The ID of the Image that will be used to display
	//                    next to the caption text of the node.
	// Remarks:
	//     Use the Image Index to specify the index of the previously loaded
	//     image to display next to the caption text of the node.
	// ---------------------------------------------------------------------
	void SetImageIndex(int nImageIndex);
	// -------------------------------------------------------------------
	// Summary:
	//     Gets the ID of the Image to display next to the caption text of
	//     the node.
	// Returns:
	//     The ID of the Image to display next to the caption text of the
	//     node.
	// -------------------------------------------------------------------
	int GetImageIndex() const;

	// ---------------------------------------------------------------------
	//
	// Summary:
	//     Specifies whether the node is locked.
	// Parameters:
	//     bLocked :  True if the nodes are to be locked, False
	//                if they can accept connections.
	// Remarks:
	//     If True, the connection's points can't accept a connection line
	//     and any connection lines that were added can't be removed.
	// ---------------------------------------------------------------------
	void SetLocked(BOOL bLocked = TRUE);
	// ----------------------------------------------------------------
	// Summary:
	//     Gets whether the connection's points are locked.
	// Returns:
	//     True if the node is locked, False if the node can
	//     be moved\\edited.
	// ----------------------------------------------------------------
	BOOL IsLocked() const;

public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Sets the location of the node.
	// Parameters:
	//     ptLocation :  A point containing the x\\y coordinate location of the
	//                   node.
	// ------------------------------------------------------------------------
	void SetLocation(CPoint ptLocation);
	// ----------------------------------------------------------------
	// Summary:
	//     Gets the x\\y coordinate location of the node.
	// Returns:
	//     A point containing the x\\y coordinate location of the node.
	// ----------------------------------------------------------------
	CPoint GetLocation() const;

	// ---------------------------------------
	// Summary:
	//     Sets the width\\height of the node.
	// Parameters:
	//     sz :  New size for the node.
	// ---------------------------------------
	void SetSize(CSize sz);

	// --------------------------------------
	// Summary:
	//     Gets the current size of the node.
	// Returns:
	//     The size of the node.
	// --------------------------------------
	CSize GetSize() const;
	CSize GetUserSize() const;

	// ---------------------------------------------
	// Summary:
	//     Gets the screen coordinates for the node.
	// Returns:
	//     The screen coordinates for the node.
	// ---------------------------------------------
	CRect GetScreenRect() const;

public:
	// -------------------------------------------------------------------
	// Summary:
	//     Gets a reference to the page that this node belongs
	//     to.
	// Returns:
	//     A reference to the page that this node belongs to.
	// -------------------------------------------------------------------
	CXTPFlowGraphPage* GetPage() const;
	// ---------------------------------------------------------------------
	// Summary:
	//     Reference to the group this node is a part of (if any).
	// Remarks:
	//     A group is just a "box" that is used to visually group nodes
	//     together. By default the group will only be shown when a node in
	//     the group is selected, but you can choose to always show the
	//     groups with ShowNodeGroupsAlways.
	// Returns:
	//     Reference to the group this node is a part of (if any), otherwise
	//     \returns null.
	// ---------------------------------------------------------------------
	CXTPFlowGraphNodeGroup* GetGroup() const;
	// -------------------------------------------------------------------
	// Summary:
	//     Gets the ID of the Image to display next to the caption text of
	//     the node.
	// Returns:
	//     The ID of the Image to display next to the caption text of the
	//     node.
	//
	//
	// -------------------------------------------------------------------
	CXTPFlowGraphImage* GetImage() const;
	// ---------------------------------------------------------------------
	// Summary:
	//     Reference to the connection points collection for this Node.
	// Returns:
	//     Reference to the connection points collection for this Node.
	//
	// ---------------------------------------------------------------------
	CXTPFlowGraphConnectionPoints* GetConnectionPoints() const;
	// -----------------------------------------------------
	// Summary:
	//     Gets a reference to the parent FlowGraph control.
	// Returns:
	//     A reference to the parent FlowGraph control.
	// -----------------------------------------------------
	CXTPFlowGraphControl* GetControl() const;

public:
	// ------------------------------------------
	// Summary:
	//     Removes this node from the flow graph.
	// ------------------------------------------
	void Remove();

public:

	// ------------------------------------------------------------------
	// Summary:
	//     Determines if the specified point is within the node.
	// Parameters:
	//     point :  Point to test.
	// Returns:
	//     True if the point is located within the node, False otherwise.
	// ------------------------------------------------------------------
	virtual CXTPFlowGraphConnectionPoint* HitTestConnectionPoint(CPoint point);

	// ------------------------------------------------------------------
	// Summary:
	//     Call this method to update the window position of the attached
	//     external control.
	// Remarks:
	//     This is also called internally when the doe is drawn.
	// ------------------------------------------------------------------
	virtual void UpdateWindowPosition();

	// --------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the content of the node.
	// Parameters:
	//     pDC :  A pointer to the CXTPFlowGraphDrawContext context object.
	// --------------------------------------------------------------------
	virtual void DrawWindowContent(CXTPFlowGraphDrawContext* pDC);

	// -----------------------------------------------------
	// Summary:
	//     Specifies whether the node is currently selected.
	// -----------------------------------------------------
	virtual void Select();

public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a node using the specified
	//     data object.
	// Parameters:
	//     pPX :  Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load node data to or from
	//     a storage.
	// ------------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	// ----------------------------------------------------------------------
	// Remarks:
	//     Recalculates the layout of the Node.
	// Remarks:
	//     Call this functions to correctly reposition the connections within
	//     the node. This also will readjust the layout when the size or
	//     caption has changed.
	// ----------------------------------------------------------------------
	virtual void RecalLayout(CXTPFlowGraphDrawContext* pDC);
	// ---------------------------------------------
	// Summary:
	//     This method is called to draw the node.
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// ---------------------------------------------
	virtual void Draw(CXTPFlowGraphDrawContext* pDC);

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Call this method to get Markup element that renders caption of the
	//     control.
	// Returns:
	//     Pointer to CXTPMarkupUIElement element.
	// ----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

protected:
	// ---------------------------------------------------
	// Summary:
	//     This method is called when the node is removed.
	// ---------------------------------------------------
	virtual void OnRemoved();

protected:
	CXTPFlowGraphPage* m_pPage;  // Pointer to the page this node is displayed on.

	CXTPFlowGraphNode* m_pNextVisibleNode; // Pointer to the next visible node.

	CString m_strCaption;  // Caption displayed for this node.
	CXTPMarkupUIElement* m_pMarkupUIElement;  // Markup element that renders caption.

	CPoint m_ptLocation;  // Point that contains the coordinates of the node.

	CSize m_szUserSize; // Current size of the node.  Including user resizing.
	CSize m_szActualSize; // Real size of the node before the user might have resized it.

	CString m_strTooltip; // Tooltip of the item.
	COLORREF m_clrNode; // Color of the node.
	int m_nStyle; // Reserved for future use. Currently can be used as a "tag" property if needed.
	DWORD_PTR m_dwTag; // The 32-bit value associated with the item.
	int m_nId; // Id of the node.

	HWND m_hWnd; // Handle of the node.
	CSize m_szWindow; // Size of the window of the attached control (if applicable).  For example, if a tree control was attached to the node, this would be it's window size.

	int m_nGroupId;  // Id of the group this node belongs to (if any).

	int m_nImageIndex; // Index of an image to display next to the caption text of the node.

	BOOL m_bLocked; // Specifies whether the node is locked.  True if the node if locked, False otherwise.

	CXTPFlowGraphConnectionPoints* m_pConnectionPoints;  // Collection of connection points that are displayed for this node.
	CXTPFlowGraphNodeGroup* m_pGroup;  // Pointer to the group this node belongs to (if any).

	friend class CXTPFlowGraphNodes;
	friend class CXTPFlowGraphNodeGroup;
	friend class CXTPFlowGraphPaintManager;
	friend class CXTPFlowGraphPage;
	friend class CXTPFlowGraphControl;

//{{AFX_CODEJOCK_PRIVATE
private:
	int m_nArrangeIndex;
	int m_nArrangeLevel;
	int m_nArrangeComponent;
//}}AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE
// Paint Manager Properties:
public:
	CRect m_rcCaption;
	CRect m_rcWindow;
//}}AFX_CODEJOCK_PRIVATE


};


AFX_INLINE  CString CXTPFlowGraphNode::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE CPoint CXTPFlowGraphNode::GetLocation() const {
	return m_ptLocation;
}
AFX_INLINE CXTPFlowGraphConnectionPoints* CXTPFlowGraphNode::GetConnectionPoints() const {
	return m_pConnectionPoints;
}
AFX_INLINE void CXTPFlowGraphNode::SetTooltip(LPCTSTR lpszTooltip) {
	m_strTooltip = lpszTooltip;
}
AFX_INLINE CString CXTPFlowGraphNode::GetTooltip() const {
	return m_strTooltip;
}
AFX_INLINE void CXTPFlowGraphNode::SetColor(COLORREF clr) {
	m_clrNode = clr;
	OnGraphChanged();
}
AFX_INLINE COLORREF CXTPFlowGraphNode::GetColor() const {
	return m_clrNode;
}
AFX_INLINE void CXTPFlowGraphNode::SetStyle(int nStyle) {
	m_nStyle = nStyle;
	OnGraphChanged();
}
AFX_INLINE int CXTPFlowGraphNode::GetStyle() const {
	return m_nStyle;
}
AFX_INLINE CXTPFlowGraphPage* CXTPFlowGraphNode::GetPage() const {
	return m_pPage;
}
AFX_INLINE void CXTPFlowGraphNode::SetTag(DWORD_PTR dwTag) {
	m_dwTag = dwTag;
}
AFX_INLINE DWORD_PTR CXTPFlowGraphNode::GetTag() const {
	return m_dwTag;
}
AFX_INLINE void CXTPFlowGraphNode::SetID(int nId) {
	m_nId = nId;
}
AFX_INLINE int CXTPFlowGraphNode::GetID() const {
	return m_nId;
}
AFX_INLINE void CXTPFlowGraphNode::SetWindowHandle(HWND hWnd) {
	m_hWnd = hWnd;
	OnGraphChanged();
}
AFX_INLINE HWND CXTPFlowGraphNode::GetWindowHandle() const {
	return m_hWnd;
}
AFX_INLINE void CXTPFlowGraphNode::SetWindowSize(CSize sz) {
	m_szWindow = sz;
	OnGraphChanged();
}
AFX_INLINE CSize CXTPFlowGraphNode::GetWindowSize() const{
	return m_szWindow;
}
AFX_INLINE CXTPFlowGraphNodeGroup* CXTPFlowGraphNode::GetGroup() const {
	return m_pGroup;
}
AFX_INLINE void CXTPFlowGraphNode::SetImageIndex(int nImageIndex) {
	m_nImageIndex = nImageIndex;
}
AFX_INLINE int CXTPFlowGraphNode::GetImageIndex() const {
	return m_nImageIndex;
}
AFX_INLINE void CXTPFlowGraphNode::SetLocked(BOOL bLocked) {
	m_bLocked = bLocked;
}
AFX_INLINE BOOL CXTPFlowGraphNode::IsLocked() const {
	return m_bLocked;
}
AFX_INLINE CXTPMarkupUIElement* CXTPFlowGraphNode::GetMarkupUIElement() const {
	return m_pMarkupUIElement;
}


#endif //#if !defined(__XTPFLOWGRAPHNODE_H__)
