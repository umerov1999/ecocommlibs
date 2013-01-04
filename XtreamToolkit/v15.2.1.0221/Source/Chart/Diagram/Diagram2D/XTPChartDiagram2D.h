// XTPChartDiagram2D.h
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
#if !defined(__XTPCHARTDIAGRAM2D_H__)
#define __XTPCHARTDIAGRAM2D_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartDiagram2DPane;
class CXTPChartDiagram2DPaneView;
class CXTPChartDiagram2DAppearance;

//===========================================================================
// Summary:
//     This class represents a chart 2D diagram, which is a kind of CXTPChartDiagram.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDiagram2D : public CXTPChartDiagram
{
	DECLARE_SERIAL(CXTPChartDiagram2D)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDiagram2D object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDiagram2D();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartDiagram2D object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartDiagram2D();

public:
	BOOL IsRotated() const;
	void SetRotated(BOOL bRotated);

	BOOL IsAllowZoom() const;
	void SetAllowZoom(BOOL bAllowZoom);

	BOOL IsAllowScroll() const;
	void SetAllowScroll(BOOL bAllowScroll);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the X axis associated with the 2D diagram.
	// Returns:
	//     A pointer to CXTPChartAxis object, the specific type is CXTPChartAxisX.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxisX() const;
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the Y axis associated with the 2D diagram.
	// Returns:
	//     A pointer to CXTPChartAxis object, the specific type is CXTPChartAxisY.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxisY() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the secondary X axis associated with the 2D
	//     diagram if any.
	// Returns:
	//     A pointer to CXTPChartAxis object, the specific type is CXTPChartAxisX.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetSecondaryAxisX() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the secondary Y axis associated with the 2D
	//     diagram if any.
	// Returns:
	//     A pointer to CXTPChartAxis object, the specific type is CXTPChartAxisX.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetSecondaryAxisY() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the pane to which this object is associated
	//     with.
	// Returns:
	//     A pointer to CXTPChartDiagram2DPane object.
	//-------------------------------------------------------------------------
	CXTPChartDiagram2DPane* GetPane() const;

	void SetPane(CXTPChartDiagram2DPane* pPane);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the diagram appearance object is associated
	//     with this object.
	// Returns:
	//     A pointer to CXTPChartDiagram2DAppearance object.
	//-------------------------------------------------------------------------
	CXTPChartDiagram2DAppearance* GetAppearance() const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of 2D diagram object.
	// Parameters:
	//     pDC     - A pointer to chart device context object.
	//     pParent - A pointer to the parent view.
	// Returns:
	//     A pointer to CXTPChartDiagramView object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagramView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent);

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to update the layout if there are some changes in
	//     the diagram.
	// Parameters:
	//     pDC      - A pointer to chart device context object.
	//     rcBounds - The rectangular diagram boundary.
	// Remarks:
	//-------------------------------------------------------------------------
	void UpdateLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView, CRect rcBounds);


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartDiagram2D);
	DECLARE_OLECREATE_EX(CXTPChartDiagram2D)
	LPDISPATCH OleGetAxisX();
	LPDISPATCH OleGetAxisY();
	LPDISPATCH OleGetSecondaryAxisX();
	LPDISPATCH OleGetSecondaryAxisY();



	OLE_COLOR OleGetBackgroundColor();
	void OleSetBackgroundColor(OLE_COLOR clr);
	OLE_COLOR OleGetBackgroundColor2();
	void OleSetBackgroundColor2(OLE_COLOR clr);
	OLE_COLOR OleGetBorderColor();
	void OleSetBorderColor(OLE_COLOR clr);
	LPDISPATCH OleGetBackgroundFillStyle();


//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartAxis* m_pAxisX;             //The pointer to X axis object.
	CXTPChartAxis* m_pAxisY;             //The pointer to Y axis object.
	CXTPChartDiagram2DPane* m_pPane;       //The pointer to diagram pane object.

	CXTPChartAxis* m_pSecondaryAxisX;    //The pointer to the secondary X axis object.
	CXTPChartAxis* m_pSecondaryAxisY;    //The pointer to the secondary Y axis object.

	BOOL m_bRotated;
	BOOL m_bAllowZoom;
	BOOL m_bAllowScroll;

	friend class CXTPChartDiagram2DView;
};


AFX_INLINE CXTPChartAxis* CXTPChartDiagram2D::GetAxisX() const {
	return m_pAxisX;
}
AFX_INLINE CXTPChartAxis* CXTPChartDiagram2D::GetAxisY() const {
	return m_pAxisY;
}
AFX_INLINE CXTPChartDiagram2DPane* CXTPChartDiagram2D::GetPane() const {
	return m_pPane;
}

AFX_INLINE CXTPChartAxis* CXTPChartDiagram2D::GetSecondaryAxisX() const {
	return m_pSecondaryAxisX;
}
AFX_INLINE CXTPChartAxis* CXTPChartDiagram2D::GetSecondaryAxisY() const {
	return m_pSecondaryAxisY;
}
AFX_INLINE BOOL CXTPChartDiagram2D::IsRotated() const {
	return m_bRotated;
}
AFX_INLINE void CXTPChartDiagram2D::SetRotated(BOOL bRotated) {
	m_bRotated = bRotated;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartDiagram2D::IsAllowZoom() const {
	return m_bAllowZoom;
}
AFX_INLINE void CXTPChartDiagram2D::SetAllowZoom(BOOL bAllowZoom) {
	m_bAllowZoom = bAllowZoom;
}
AFX_INLINE BOOL CXTPChartDiagram2D::IsAllowScroll() const {
	return m_bAllowScroll;
}
AFX_INLINE void CXTPChartDiagram2D::SetAllowScroll(BOOL bAllowScroll) {
	m_bAllowScroll = bAllowScroll;
	OnChartChanged();
}



#endif //#if !defined(__XTPCHARTDIAGRAM2D_H__)
