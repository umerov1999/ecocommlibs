// XTPFlowGraphNode.h: interface for the CXTPFlowGraphNode class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
class CXTPFlowGraphSelectionRect;

class CXTPMarkupUIElement;
class CXTPMarkupObject;
class CXTPMarkupUIElementCollection;
class CXTPMarkupVisual;
class CXTPMarkupTextBlock;

#define XAML_MARKUP_PROPERTY_CPNAME             L"CPName"
#define XAML_MARKUP_PROPERTY_CPTYPE             L"CPType"
#define XAML_MARKUP_PROPERTY_CANSETCOLOR        L"CanSetColor"
#define XAML_MARKUP_PROPERTY_CPOINTS_IN         L"ConnectionPointsIn"
#define XAML_MARKUP_PROPERTY_RESIZABLE          L"Resizable"

#define XAML_MARKUP_TAG_ITEMSCOLLECTION         L"nodeItemsCollection"
#define XAML_MARKUP_TAG_CUSTOMCONTROL           L"nodeCustomControl"
#define XAML_MARKUP_TAG_NODECAPTION             L"nodeCaption"
#define XAML_MARKUP_TAG_NODECAPTIONBORDER       L"nodeCaptionBorder"
#define XAML_MARKUP_TAG_NODE_ITEM_CAPTION       L"nodeItemCaption"
#define XAML_MARKUP_TAG_NODE_ITEM_IMAGE         L"nodeItemImage"
#define XAML_MARKUP_TAG_NODEIMAGE               L"nodeImage"
#define XAML_MARKUP_TAG_NODECONTENT             L"nodeContent"

#define XAML_MARKUP_CPNAME_PLACEHOLDER          _T("%CPNAME%")
#define XAML_MARKUP_CPCAPTION_PLACEHOLDER       _T("%CPCAPTION%")

#define XAML_MARKUP_RESOURCENAME_NODE            _T("FLOWGRAPH_XAML_NODE")
#define XAML_MARKUP_RESOURCENAME_NODE_ITEM       _T("FLOWGRAPH_XAML_NODE_ITEM")
#define XAML_MARKUP_RESOURCENAME_NODE_ITEM_IN    _T("FLOWGRAPH_XAML_NODE_ITEM_IN")
#define XAML_MARKUP_RESOURCENAME_NODE_ITEM_OUT   _T("FLOWGRAPH_XAML_NODE_ITEM_OUT")
#define XAML_MARKUP_RESOURCENAME_NODE_ITEM_INOUT _T("FLOWGRAPH_XAML_NODE_ITEM_INOUT")

// ------------------------------------------------------------------------------
// Summary:
//     Node Connection Point types (input\\output).
// Remarks:
//     The xtpFlowGraphConnectionPointType is used to specify whether a
//     CXTPFlowGraphConnection will have any connection points.
//     Connection Points are used to make connections between a
//     connection in different nodes.
// See Also:
//     CXTPFlowGraphConnectionPoint::SetType@XTPFlowGraphConnectionPointType
//
// ------------------------------------------------------------------------------
enum XTPFlowGraphConnectionPointType
{
	xtpFlowGraphPointNone = 0, // Connection has no connection points.
	xtpFlowGraphPointInput = 1, // Connection only has an input point.
	xtpFlowGraphPointOutput = 2, // Connection only has an output point.

	// Connection has both input and output points. For the default node type.
	// When iterating connection points you will never find a connection point of this type
	// It's used to add a table row with actual 2 connection points: xtpFlowGraphPointInput and xtpFlowGraphPointOutput
	xtpFlowGraphPointInputAndOutput = 3,

	// a new connection point type for other shapes
	xtpFlowGraphPointBoth = 4

};

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
	virtual void SetColor(COLORREF clr, BYTE bAlpha = 255);
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
	virtual void SetWindowHandle(HWND hWnd);
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
	virtual void SetWindowSize(CSize sz);
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

	// ---------------------------------------------------------------------
	//
	// Summary:
	//     Specifies whether the node is visible.
	// Parameters:
	//     bVisible :  True if the nodes are to be visible, False otherwise
	// Remarks:
	//     If False, any connections to/from this node become invisible as well
	// ---------------------------------------------------------------------
	void SetVisible(BOOL bVisible = TRUE);

	// ----------------------------------------------------------------
	// Summary:
	//     Gets whether the node and its connections are visible.
	// Returns:
	//     True if the node is visible, False otherwise
	// ----------------------------------------------------------------
	BOOL IsVisible() const;

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
	//     Sets the width\\height of the node (node's markup!)
	// Parameters:
	//     sz :  New size for the node.
	// ---------------------------------------
	virtual void SetSize(CSize sz);

	// ---------------------------------------
	// Summary:
	//     Sets the width\\height of the node.
	// Parameters:
	//     sz :  New size for the node.
	// ---------------------------------------
	void SetSizeCore(CSize sz);

	// --------------------------------------
	// Summary:
	//     Gets the current size of the node.
	// Returns:
	//     The size of the node.
	// --------------------------------------
	CSize GetSize() const;

	// --------------------------------------
	// Summary:
	//     Gets the current size of the node (including user resizing)
	// Returns:
	//     The size of the node.
	// --------------------------------------
	CSize GetUserSize() const;

	// --------------------------------------
	// Summary:
	//     Returns if the node is resizable or not
	// --------------------------------------
	BOOL IsResizable() const;

	// --------------------------------------
	// Summary:
	//     Gets minimum size of the node, calculated in CalcMinSize method
	// Returns:
	//     The minimum size of the node
	// --------------------------------------
	CSize GetMinSize() const;

	// --------------------------------------
	// Summary:
	//     Sets minimum size of the node
	// --------------------------------------
	void SetMinSize(CSize sz);

	// ---------------------------------------------
	// Summary:
	//     Gets the screen coordinates for the node.
	// Returns:
	//     The screen coordinates for the node.
	// ---------------------------------------------
	CRect GetScreenRect(BOOL bIncludeConnectionPointsEllipses = TRUE) const;

	virtual CRect GetBoundingRectangle(BOOL bIncludeConnectionPointsEllipses = TRUE) const;

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
	virtual void Select(BOOL bSelect = TRUE);

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

	// ----------------------------------------------------------------------
	// Remarks:
	//     Store/Load node common properties (for both default and derived nodes)
	// Remarks:
	//     This functions is called automatically in default node and custom
	// ----------------------------------------------------------------------
	void DoCommonPropExchange(CXTPPropExchange* pPX);

public:
	// ----------------------------------------------------------------------
	// Remarks:
	//     Recalculates the layout of the Node.
	// Remarks:
	//     Call this functions to correctly reposition the connections within
	//     the node. This also will readjust the layout when the size or
	//     caption has changed.
	// ----------------------------------------------------------------------
	virtual void RecalLayout();
	// ---------------------------------------------
	// Summary:
	//     This method is called to draw the node.
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// ---------------------------------------------
	virtual void Draw(CXTPFlowGraphDrawContext* pDC);

	CRect GetSelectionRect() const;

	// ---------------------------------------------
	// Summary:
	//     This virtual method is called to draw a node's selection rectangle
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// ---------------------------------------------
	virtual void DrawSelectionRect(CXTPFlowGraphDrawContext* pDC);

	// ---------------------------------------------
	// Summary:
	//     A virtual method that draws node's connection points (circles) in their placeholders
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// ---------------------------------------------
	virtual void DrawConnectionPoints(CXTPFlowGraphDrawContext* pDC);

	// ---------------------------------------------
	// Summary:
	//     A virtual method that calculates minimum size of a node (used in resizing mechanism)
	// ---------------------------------------------
	virtual void CalcMinSize();

// Default Markup support:

public:

	// ---------------------------------------------
	// Summary:
	//     Adds a table row to the node (of default type), with none, one or two real connection points
	// ----------------------------------------------------------------------
	// Parameters:
	//     szName :  unique name
	//     szCaption :  caption of table row. When not specified - uses szName
	//     type : type
	// Remarks:
	//     xtpFlowGraphPointNone             - no actual connection points
	//     xtpFlowGraphPointInput            - 1 connection point:  in the left of the row
	//     xtpFlowGraphPointOutput           - 1 connection point:  in the right of the row
	//     xtpFlowGraphPointInputAndOutput   - 2 connection points: in the left & right of the row
	// ---------------------------------------------
	virtual void AddNamedConnectionPoints(LPCTSTR szName, XTPFlowGraphConnectionPointType type = xtpFlowGraphPointNone, LPCTSTR szCaption = NULL);

	// ---------------------------------------------
	// Summary:
	//     An important common method used to get a rectangle of any visual markup element
	// Remarks:
	//    A rectangle coordinates are relative to markup root element (<Border> for example)
	// Parameters:
	//     pElementVisual :  a pointer to a valid visual element
	// ---------------------------------------------
	CRect GetMarkupVisualElementRect(const CXTPMarkupVisual *pElementVisual) const;

	// ---------------------------------------------
	// Summary:
	//     Returns an index of the table row in the list (to insert a new one)
	// Parameters:
	//     pPoint    :  Pointer to a connection point that is located in this row
	//     pChildren :  Pointer to a connection point that is located in this rot
	// ---------------------------------------------
	int GetMarkupTableRowIndex(CXTPFlowGraphConnectionPoint *pPoint, CXTPMarkupUIElementCollection *pChildren);

	// ---------------------------------------------
	// Summary:
	//     Removes a table row (with it's connection points) of a default node
	// Returns:
	//      an index in the table rows list
	// Parameters:
	//     pPoint    :  Pointer to a connection point that is located in the table row
	// ----------------------------------------------------------------------
	int RemoveMarkupTableRow(CXTPFlowGraphConnectionPoint *pPoint);

	// ---------------------------------------------
	// Summary:
	//     Gets XAML TextBlock element for a table row of a default node
	// Returns:
	//      a pointer to CXTPMarkupTextBlock element
	// Parameters:
	//     strConnectionPointName :  a connection point's name, this connection point is supposed to be in this table row
	// ----------------------------------------------------------------------
	CXTPMarkupTextBlock *GetTableRowTextBlock(const CString &strConnectionPointName) const;

	// ---------------------------------------------
	// Summary:
	//     Gets XAML TextBlock element's rectangle of a table row of a default node
	// Returns:
	//      a rectangle of CXTPMarkupTextBlock element. Coordinates are relative to node's location!
	// Parameters:
	//     strConnectionPointCaption :  a connection point's name, this connection point is supposed to be in this table row
	// ----------------------------------------------------------------------
	CRect GetTableRowCaptionRect(CXTPFlowGraphConnectionPoint *pPoint) const;

	// ---------------------------------------------
	// Summary:
	//     Sets default XAML markup for a default node with zero table rows
	// ----------------------------------------------------------------------
	virtual void SetDefaultMarkup();

	// ---------------------------------------------
	// Summary:
	//     updates the markup point caption (when changed or restoring using DoPropExchange)
	// ----------------------------------------------------------------------
	void UpdateMarkupPointCaption(CXTPFlowGraphConnectionPoint *pPoint);

	void UpdateMarkupPointName(CXTPFlowGraphConnectionPoint *pPoint);

	// ---------------------------------------------
	// Summary:
	//     updates the markup point color (when changed or restoring using DoPropExchange)
	// ----------------------------------------------------------------------
	virtual void UpdateMarkupPointColor(CXTPFlowGraphConnectionPoint *pPoint);

	// ---------------------------------------------
	// Summary:
	//     updates the markup point image (when changed or restoring using DoPropExchange)
	// ----------------------------------------------------------------------
	void UpdateMarkupPointImage(CXTPFlowGraphConnectionPoint *pPoint);

	// ----------------------------------------------------------------
	// Summary:
	//     Sets markup for the node (for custom node)
	// Returns:
	//     True if the markup is valid and the operation is success
	// ----------------------------------------------------------------
	BOOL SetMarkupText(LPCTSTR szMarkupText);

	// ----------------------------------------------------------------
	// Summary:
	//     A virtual function that returns TRUE only for custom nodes (nodes with custom XAML markup)
	// ----------------------------------------------------------------
	virtual BOOL IsCustomMarkup() const { return FALSE; }

	// ----------------------------------------------------------------
	// Summary:
	//     A virtual method that finds and updates real connection points in the markup
	// Remarks:
	//     Implementation of this method for default nodes is recursive.
	//     Rectangle for a connection point is defined by <Rectangle/> element in XAML for default nodes.
	//     Implementation of this methods for custom nodes uses absolutely different approach (see CXTPFlowGraphNodeCustom::UpdateConnectionPoints)
	// Parameters:
	//     pRootElement: an XAML element used to search for connection points in its children
	// ----------------------------------------------------------------
	virtual void UpdateConnectionPoints(CXTPMarkupObject *pRootElement);

	// ----------------------------------------------------------------
	// Summary:
	//     A recursive method to update/set color of a node
	// Parameters:
	//     pRootElement: an XAML element used to search for XAML elements that can have a color property (see XAML_MARKUP_PROPERTY_CANSETCOLOR)
	// ----------------------------------------------------------------
	virtual void UpdateColorRecursive(CXTPMarkupObject *pRootElement, COLORREF clrColor);

	// ----------------------------------------------------------------
	// Summary:
	//     Updates foreground of CXTPMarkupTextBlock element of node's caption
	// ----------------------------------------------------------------
	virtual void UpdateCaptionColor();

	// ----------------------------------------------------------------
	// Summary:
	//     Updates node's image, image index is defined as m_nImageIndex
	// ----------------------------------------------------------------
	virtual void UpdateMarkupImage();

	// ----------------------------------------------------------------
	// Summary:
	//     Updates node's caption <TextBlock/> element with node's caption text (m_strCaption)
	// Returns:
	//     a pointer to CXTPMarkupTextBlock element
	// ----------------------------------------------------------------
	virtual CXTPMarkupTextBlock* UpdateMarkupCaption();

	// ---------------------------------------------
	// Summary:
	//     Get node's caption <TextBlock> elements
	// Returns:
	//     A pointer to CXTPMarkupTextBlock element
	// ---------------------------------------------
	CXTPMarkupTextBlock* GetCaptionTextBlock() const;

	// ----------------------------------------------------------------
	// Summary:
	//     Get node's caption <TexbBlock> rectangle
	// Returns:
	//     a rectangle of the TextBlock element. Coordinates are relative to node's location!
	// ----------------------------------------------------------------
	CRect GetCaptionRect() const;

	// ----------------------------------------------------------------
	// Summary:
	//     Updates the default font define in Paint Manager, used in Markup drawing
	// ----------------------------------------------------------------
	virtual void UpdateMarkupFont();

protected:
	// ---------------------------------------------------
	// Summary:
	//     This method is called when the node is removed.
	// ---------------------------------------------------
	virtual void OnRemoved();

	static LRESULT CALLBACK ControlWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // new window procedure

	void SetCustomWndProc();
	void ResetCustomWndProc();

protected:
	CXTPFlowGraphPage* m_pPage;  // Pointer to the page this node is displayed on.

	CXTPFlowGraphNode* m_pNextVisibleNode; // Pointer to the next visible node.

	CString m_strCaption;  // Caption displayed for this node.

	CString m_strMarkup;

	CXTPMarkupUIElement *m_pMarkup; // Markup that is responsible for drawing this node

	CPoint m_ptLocation;  // Point that contains the coordinates of the node.

	CSize m_szUserSize; // Current size of the node.  Including user resizing.
	CSize m_szActualSize; // Real size of the node before the user might have resized it.
	CSize m_szMinSize;
	CSize m_szUserMinSize; // manually set minimum size

	BOOL m_bResizable;

	CString m_strTooltip; // Tooltip of the item.
	COLORREF m_clrNode; // Color of the node.

	BYTE m_bAlpha;   // alpha channel 0-255

	int m_nStyle; // Reserved for future use. Currently can be used as a "tag" property if needed.
	DWORD_PTR m_dwTag; // The 32-bit value associated with the item.
	int m_nId; // Id of the node.

	HWND m_hWnd; // Handle of the window inside the node
	WNDPROC m_pOriginalWindowProc; // Window procedure pointer
	CSize m_szWindow; // Size of the window of the attached control (if applicable).  For example, if a tree control was attached to the node, this would be it's window size.

	static CMap<HWND, HWND, CXTPFlowGraphNode*, CXTPFlowGraphNode *> m_WindowToNodeHash; // used in the custom window 'ControlWindowProc'

	CXTPFlowGraphSelectionRect *m_pSelectionRect;

	int m_nGroupId;  // Id of the group this node belongs to (if any).

	int m_nImageIndex; // Index of an image to display next to the caption text of the node.

	BOOL m_bLocked; // Specifies whether the node is locked.  True if the node if locked, False otherwise.

	BOOL m_bVisible; // Specifies whether the node is visible.  True if the node if visible, False otherwise.

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
	CRect m_rcWindow;
//}}AFX_CODEJOCK_PRIVATE


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphNode)

	afx_msg void OleSetCaption(LPCTSTR lpszCaption);
	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetTooltip(LPCTSTR lpszTooltip);
	afx_msg BSTR OleGetTooltip();
	afx_msg void OleSetColor(OLE_COLOR clr);
	afx_msg OLE_COLOR OleGetColor();
	afx_msg void OleSetStyle(int nStyle);
	afx_msg int OleGetStyle();
	afx_msg LPDISPATCH OleGetConnectionPoints();

	afx_msg int OleGetLeft();
	afx_msg void OleSetLeft(int nLeft);
	afx_msg int OleGetTop();
	afx_msg void OleSetTop(int nTop);
	afx_msg int OleGetWidth();
	afx_msg int OleGetHeight();

	afx_msg LPDISPATCH OleGetPage();
	afx_msg BOOL OleGetSelected();
	afx_msg void OleSetSelected(BOOL bSelected);

	afx_msg LPDISPATCH OleGetGroup();
	afx_msg LPDISPATCH OleHitTestConnectionPoint(long x, long y);

	afx_msg void OleAddNamedConnectionPoints(LPCTSTR lpszName, long type);
	afx_msg void OleAddNamedConnectionPointsWithCaption(LPCTSTR lpszName, long type, LPCTSTR lpszCaption);

	afx_msg void OleSetWindowHandle(HWND hWnd);
	afx_msg HWND OleGetWindowHandle();

	afx_msg int OleGetWindowWidth();
	afx_msg void OleSetWindowWidth(int nWidth);

	afx_msg int OleGetWindowHeight();
	afx_msg void OleSetWindowHeight(int nHeight);

	afx_msg void OleSetWidth(int nWidth);
	afx_msg void OleSetHeight(int nHeight);

//}}AFX_CODEJOCK_PRIVATE
#endif
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
AFX_INLINE HWND CXTPFlowGraphNode::GetWindowHandle() const {
	return m_hWnd;
}
AFX_INLINE CSize CXTPFlowGraphNode::GetWindowSize() const{
	return m_szWindow;
}
AFX_INLINE CXTPFlowGraphNodeGroup* CXTPFlowGraphNode::GetGroup() const {
	return m_pGroup;
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
AFX_INLINE BOOL CXTPFlowGraphNode::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE BOOL CXTPFlowGraphNode::IsResizable() const {
	return m_bResizable;
}

#endif //#if !defined(__XTPFLOWGRAPHNODE_H__)
