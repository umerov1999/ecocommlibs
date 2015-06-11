// XTPChartRadarDiagramView.h
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
#if !defined(__XTPCHARTRADARDIAGRAMVIEW_H__)
#define __XTPCHARTRADARDIAGRAMVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartDiagram2DAppearance;
class CXTPChartRadarAxisView;
class CXTPChartSeriesView;
class CXTPChartRadarAxisXView;
class CXTPChartRadarAxisYView;
class CXTPChartLineStyle;
class CXTPChartFillStyle;

//===========================================================================
// Summary:
//     This class represents the view of a chart 2D diagram, which is a kind of
//     CXTPChartDiagramView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarDiagramView :public CXTPChartDiagramView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarDiagramView object.
	// Parameters:
	//     pDiagram     - A pointer to the chart diagram object.
	//     pParent      - A pointer to the parent view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the 2D diagram.
	// Parameters:
	//     pDC      - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	void CreateView(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the layout based on a rectangular
	//     boundary.
	// Parameters:
	//     pDC      - A pointer to the chart device context.
	//     rcBounds - The diagram boundary.
	// Remarks:
	//-----------------------------------------------------------------------
	void CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds);

	void UpdateRange(CXTPChartDeviceContext* pDC);

	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns Axis View
	// Parameters:
	//     pAxis - Axis
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarAxisView* GetAxisView(CXTPChartAxis* pAxis) const;

public:
	void CheckLabelBounds(const CXTPChartRectF& rcBounds);

	CXTPChartPointF GetScreenPoint(const CXTPChartSeriesView* pView, double x, double y) const;


	CXTPChartRadarAxisXView* GetAxisXView() const;
	CXTPChartRadarAxisYView* GetAxisYView() const;


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to add an axis view to the diagram view.
	//     boundary.
	// Parameters:
	//     pDC          - A pointer to the chart device context.
	//     pParentView  - The parent view of the axis view.
	//     pAxis        - A pointer to the axis object, whose view is to be
	//                    added.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarAxisView* AddAxisView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView, CXTPChartAxis* pAxis);

public:
	CXTPChartDeviceCommand* CreatePolygonStripDeviceCommand(CXTPChartDeviceContext* /*pDC*/, double dRadiusFrom, double dRadiusTo,
		const CXTPChartColor& color1, const CXTPChartColor& color2, CXTPChartFillStyle* pFillStyle);

	CXTPChartDeviceCommand* CreatePolygonLineDeviceCommand(CXTPChartDeviceContext* /*pDC*/, double dRadius,
		const CXTPChartColor& color, CXTPChartLineStyle* pLineStyle);


protected:

	CXTPChartElementView* m_pAxisViews;      //The axis view.

	CXTPChartRadarAxisXView* m_pAxisXView;
	CXTPChartRadarAxisYView* m_pAxisYView;

	CRect m_rcLabelPadding;

	CXTPPoint2i m_ptCenter;
	int m_nRadius;
};


AFX_INLINE CXTPChartRadarAxisXView* CXTPChartRadarDiagramView::GetAxisXView() const {
	return m_pAxisXView;
}
AFX_INLINE CXTPChartRadarAxisYView* CXTPChartRadarDiagramView::GetAxisYView() const {
	return m_pAxisYView;
}


#endif //#if !defined(__XTPCHARTRADARDIAGRAMVIEW_H__)
