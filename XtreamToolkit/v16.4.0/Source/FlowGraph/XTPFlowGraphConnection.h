// XTPFlowGraphConnection.h: interface for the CXTPFlowGraphConnection class.
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
#if !defined(__XTPFLOWGRAPHCONNECTION_H__)
#define __XTPFLOWGRAPHCONNECTION_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphPage;
class CXTPFlowGraphConnectionPoint;
class CXTPFlowGraphDrawContext;
class CXTPFlowGraphNode;

namespace Gdiplus
{
	class GraphicsPath;
};


// -------------------------------------------------------------------
// Summary:
//     Represents a connection. Connections are the lines that connect
//     two connection points.
// -------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphConnection : public CXTPFlowGraphElement
{
	DECLARE_SERIAL(CXTPFlowGraphConnection);

public:
	// ------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphConnection object.
	// ------------------------------------------------
	CXTPFlowGraphConnection();
	// ------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphConnection object, handles cleanup and
	//     deallocation.
	// ------------------------------------------------------------------
	virtual ~CXTPFlowGraphConnection();

public:
	// --------------------------------------------------------------------
	// Summary:
	//     Sets the input ConnectionPoint this connection line is connected
	//     to.
	// Parameters:
	//     pPoint :  Reference to the ConnectionPoint to connect to.
	// --------------------------------------------------------------------
	virtual void SetInputPoint(CXTPFlowGraphConnectionPoint* pPoint);
	// --------------------------------------------------------------------
	// Summary:
	//     Gets the input ConnectionPoint this connection line is connected
	//     to.
	// Returns:
	//     Reference to the input ConnectionPoint this connection line is
	//     connected to.
	// --------------------------------------------------------------------
	CXTPFlowGraphConnectionPoint* GetInputPoint() const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Sets the output ConnectionPoint this connection line is connected
	//     to.
	// Parameters:
	//     pPoint :  Reference to the ConnectionPoint to connect to.
	// ---------------------------------------------------------------------
	virtual void SetOutputPoint(CXTPFlowGraphConnectionPoint* pPoint);
	// ---------------------------------------------------------------------
	// Summary:
	//     Gets the output ConnectionPoint this connection line is connected
	//     to.
	// Returns:
	//     Reference to the output ConnectionPoint this connection line is
	//     connected to.
	// ---------------------------------------------------------------------
	CXTPFlowGraphConnectionPoint* GetOutputPoint() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the middle point of the connection. (This is also what
	//     happens when a connection is dragged by the mouse).
	// Parameters:
	//     pt :  Point to use as the middle point in the connection.
	// Remarks:
	//     This sets the point were the little circle glyph is drawn along
	//     the spline\\line (connection). When a connection is clicked and
	//     dragged with the mouse a small circle "point" is drawn to indicate
	//     where the line was clicked and how much it has been dragged.
	// ----------------------------------------------------------------------
	void SetControlPoint(CPoint pt);
	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the middle point for the connection. (This is also what
	//     happens when a connection is dragged by the mouse).
	// Remarks:
	//     This is the point were the little circle glyph is drawn along the
	//     spline\\line (connection). When a connection is clicked and
	//     dragged with the mouse a small circle "point" is drawn to indicate
	//     where the line was clicked and how much it has been dragged.
	// Returns:
	//     The middle point for the connection.
	// ----------------------------------------------------------------------
	CPoint GetControlPoint() const;

	// -------------------------------------------------------------------
	// Summary:
	//     Sets the caption of the connection.
	// Parameters:
	//     lpszCaption :  Caption of the connection.
	//
	//     <b>Remarks</b>
	//     The connection's caption is not visible in the graph, it can be
	//     used like a tag property.
	// -------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);
	// -------------------------------------------------------------------
	// Summary:
	//     Gets the caption of the connection.
	// Remarks:
	//     The connection's caption is not visible in the graph, it can be
	//     used like a tag property.
	// Returns:
	//     The caption of the connection.
	// -------------------------------------------------------------------
	CString GetCaption() const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip for this connection.
	// Parameters:
	//     lpszTooltip :  Tooltip for this connection.
	// Remarks:
	//     This is the text that is displayed when the mouse is hovered over
	//     the connection.
	// ---------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);
	// ---------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip for this connection.
	// Remarks:
	//     This is the text that is displayed when the mouse is hovered over
	//     the connection.
	// Returns:
	//     The tooltip for this connection.
	// ---------------------------------------------------------------------
	CString GetTooltip() const;

	void SetTag(LPCTSTR lpszTag);

	CString GetTag() const;

	// ------------------------------------------
	// Summary:
	//     Sets the color of the connection line.
	// Parameters:
	//     clr :  Color of the connection line.
	// ------------------------------------------
	void SetColor(COLORREF clr);
	// ------------------------------------------
	// Summary:
	//     Gets the color of the connection line.
	// Returns:
	//     The color of the connection line.
	//
	//
	// ------------------------------------------
	COLORREF GetColor() const;

	// --------------------------------------------------------------------
	// Summary:
	//     Sets the style of the connector.
	// Parameters:
	//     nStyle :  Style for the connector, this will be a value from the
	//               XTPFlowGraphConnectorType enumeration.
	// Remarks:
	//     Use a style from the a value from the XTPFlowGraphConnectorType
	//     enumeration to set the style of the connector.
	// --------------------------------------------------------------------
	void SetStyle(int nStyle);
	// ---------------------------------------------------------------
	// Summary:
	//     Gets the style of the connector.
	// Remarks:
	//     This will return a value from the XTPFlowGraphConnectorType
	//     enumeration.
	// Returns:
	//     The style of the connector.
	// ---------------------------------------------------------------
	int GetStyle() const;

public:
	// -------------------------------------------------
	// Summary:
	//     Gets the bounding rectangle for this element.
	// Returns:
	//     The bounding rectangle for this element.
	// -------------------------------------------------
	CRect GetBoundingRect();
	// ---------------------------------------------------------------
	// Summary:
	//     Determines if the specified point is within the connection.
	// Parameters:
	//     pt :  Point to test.
	//
	// Returns:
	//     True if the point is located within the connection, False
	//     \otherwise.
	// ---------------------------------------------------------------
	virtual BOOL PtInRect(POINT pt);

public:
	// ---------------------------------------------------------------------
	// Summary:
	//     Gets the input Node this connection line is connected to.
	// Returns:
	//     Reference to the input Node this connection line is connected to.
	// ---------------------------------------------------------------------
	CXTPFlowGraphNode* GetInputNode() const;
	// ----------------------------------------------------------------------
	// Summary:
	//     Gets the output Node this connection line is connected to.
	// Returns:
	//     Reference to the output Node this connection line is connected to.
	// ----------------------------------------------------------------------
	CXTPFlowGraphNode* GetOutputNode() const;
	// -----------------------------------------------------
	// Summary:
	//     Gets a reference to the parent FlowGraph control.
	// Returns:
	//     A reference to the parent FlowGraph control.
	// -----------------------------------------------------
	CXTPFlowGraphControl* GetControl() const;
	// -----------------------------------------------------------------
	// Summary:
	//     Gets a reference to the page that this connection belongs to.
	// Returns:
	//     A reference to the page that this connection belongs to.
	// -----------------------------------------------------------------
	CXTPFlowGraphPage* GetPage() const;

public:
	// --------------------------------------------------------------
	// Summary:
	//     Call this method to recalculate the size and layout of the
	//     connection.
	// Parameters:
	//     pDC :  Reference to a valid device context.
	// --------------------------------------------------------------
	virtual void RecalcLayout(CXTPFlowGraphDrawContext* pDC);

public:
	// --------------------------------------------------------
	// Summary:
	//     Remove\\delete this connection from its parent node.
	// --------------------------------------------------------
	void Remove();
	// ---------------------------------------------------------
	// Summary:
	//     This method is called when the connection is removed.
	// ---------------------------------------------------------
	virtual void OnRemoved();

	void SetPathReversed(BOOL bReversed);

	BOOL IsPathReversed() const;

public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a connection using the
	//     specified data object.
	// Parameters:
	//     pPX :  Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load connection data to
	//     \or from a storage.
	// ------------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);


protected:
	// ----------------------------------------------------------------
	// Summary:
	//     Used internally to restore the connection when loaded from a
	//     resource. This is used in DoPropExchange.
	// ----------------------------------------------------------------
	virtual void RestoreConnection();

	int GetConnectionPointIndex(CXTPFlowGraphConnectionPoint *pPoint) const;

protected:
	CXTPFlowGraphPage* m_pPage; // The page that this connection belongs to.
	CXTPFlowGraphConnectionPoint* m_pInputPoint; // The input ConnectionPoint this connection line is connected to.
	CXTPFlowGraphConnectionPoint* m_pOutputPoint; // The output ConnectionPoint this connection line is connected to.
	CXTPFlowGraphConnection* m_pNextVisibleConnection; // Reference to the next visible connection.

	CPoint m_ptInputPoint; //  Stores the coordinates of the Input connection point (if any), also is used internally for drag\drop.
	CPoint m_ptOutputPoint; //  Stores the coordinates of the Output connection point (if any), also is used internally for drag\drop.
	CPoint m_ptControlPoint; // Middle point for the connection.  This is the point that is set when a connection is clicked and dragged by a mouse.

	CString m_strCaption; // The caption of the connection.
	CString m_strTooltip; // Tooltip text for the item.
	CString m_strTag;     // A tag property to define a connection

	COLORREF m_clrConnection; // Color of the connection.
	int m_nStyle;  // Style of the connection. It uses a value from the XTPFlowGraphConnectorType enumeration.

	int m_nInputId;  // Used internally to store the ID of the Input connection point.
	int m_nOutputId; // Used internally to store the ID of the Output connection point.

	int m_nInputPointIndex;
	int m_nOutputPointIndex;

	int m_nInputPointConnectionIndex;
	int m_nOutputPointConnectionIndex;
	int m_nConnectionIndex;

	CRect m_rcBoundingRect;  // The bounding rectangle for this element.

	Gdiplus::GraphicsPath* m_pPath;  // Path to the GDI Plus graphics used to draw the conneciton.
	BOOL m_bPathReversed;

	friend class CXTPFlowGraphPaintManager;
	friend class CXTPFlowGraphConnections;
	friend class CXTPFlowGraphControl;
	friend class CXTPFlowGraphPage;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphConnection)

	afx_msg void OleSetCaption(LPCTSTR lpszCaption);
	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetTooltip(LPCTSTR lpszTooltip);
	afx_msg BSTR OleGetTooltip();

	afx_msg void OleSetTag(LPCTSTR lpszTag);
	afx_msg BSTR OleGetTag();

	afx_msg void OleSetColor(OLE_COLOR clr);
	afx_msg OLE_COLOR OleGetColor();
	afx_msg void OleSetStyle(int nStyle);
	afx_msg int OleGetStyle();
	afx_msg LPDISPATCH OleGetOutputPoint();
	afx_msg void OleSetOutputPoint(LPDISPATCH lpDisp);
	afx_msg LPDISPATCH OleGetInputPoint();
	afx_msg void OleSetInputPoint(LPDISPATCH lpDisp);

//}}AFX_CODEJOCK_PRIVATE
#endif
};


AFX_INLINE CXTPFlowGraphConnectionPoint* CXTPFlowGraphConnection::GetInputPoint() const {
	return m_pInputPoint;
}

AFX_INLINE CXTPFlowGraphConnectionPoint* CXTPFlowGraphConnection::GetOutputPoint() const {
	return m_pOutputPoint;
}
AFX_INLINE CString CXTPFlowGraphConnection::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE void CXTPFlowGraphConnection::SetTooltip(LPCTSTR lpszTooltip) {
	m_strTooltip = lpszTooltip;
}
AFX_INLINE CString CXTPFlowGraphConnection::GetTooltip() const {
	return m_strTooltip;
}
AFX_INLINE void CXTPFlowGraphConnection::SetTag(LPCTSTR lpszTag)
{
	m_strTag = lpszTag;
}
AFX_INLINE CString CXTPFlowGraphConnection::GetTag() const
{
	return m_strTag;
}
AFX_INLINE void CXTPFlowGraphConnection::SetColor(COLORREF clr) {
	m_clrConnection = clr;
}
AFX_INLINE COLORREF CXTPFlowGraphConnection::GetColor() const {
	return m_clrConnection;
}
AFX_INLINE void CXTPFlowGraphConnection::SetStyle(int nStyle) {
	m_nStyle = nStyle;
	OnGraphChanged();
}
AFX_INLINE int CXTPFlowGraphConnection::GetStyle() const {
	return m_nStyle;
}

AFX_INLINE void CXTPFlowGraphConnection::SetPathReversed(BOOL bReversed)
{
	m_bPathReversed = bReversed;
}

AFX_INLINE BOOL CXTPFlowGraphConnection::IsPathReversed() const
{
	return m_bPathReversed;
}

AFX_INLINE CPoint CXTPFlowGraphConnection::GetControlPoint() const {
	return m_ptControlPoint;
}
AFX_INLINE CXTPFlowGraphPage* CXTPFlowGraphConnection::GetPage() const {
	return m_pPage;
}

#endif //#if !defined(__XTPFLOWGRAPHCONNECTION_H__)
