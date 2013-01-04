// XTPChartPieDiagram.h
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
#if !defined(__XTPCHARTPIEDIAGRAM_H__)
#define __XTPCHARTPIEDIAGRAM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartPie;
class CXTPChartDiagramDomain;

//===========================================================================
// Summary:
//     This class abstracts the pie diagram.This class is a kind of
//     CXTPChartDiagram.
// Remarks:
//     A pie chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pie chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPieDiagram : public CXTPChartDiagram
{
	DECLARE_SERIAL(CXTPChartPieDiagram)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPieDiagram object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPieDiagram();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the pie diagram.
	// Parameters:
	//     pDC           - A pointer to chart device context.
	//     pParent       - A pointer to the parent view.
	// Returns:
	//     A pointer to CXTPChartDiagramView object which points to a newly created
	//     CXTPChartPieDiagramView object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagramView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the pie series layout.
	// Parameters:
	//     pDC           - A pointer to chart device context.
	//     pView         - A pointer to the diagram view.
	//-------------------------------------------------------------------------
	virtual void CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView);


#ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartPieDiagram);
	DECLARE_OLECREATE_EX(CXTPChartPieDiagram)
	//}}AFX_CODEJOCK_PRIVATE
#endif

};

//===========================================================================
// Summary:
//     This class abstracts the view of a pie series.This class is a kind of
//     CXTPChartSeriesView.
// Remarks:
//     A pie chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pie chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPieSeriesView : public CXTPChartSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPieSeriesView object.
	// Parameters:
	//     pSeries       - Pointer to pie series object.
	//     pDiagramView  - Pointer to the pie diagram view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPieSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPieSeriesView object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPieSeriesView();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the pie series point layout.
	// Parameters:
	//     pDC      - Pointer to the chart device context.
	//     rcBounds - The rectangular boundary.
	//-------------------------------------------------------------------------
	void CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the pie series label layout.
	// Parameters:
	//     pDC      - Pointer to the chart device context.
	//     rcBounds - The rectangular boundary.
	//-------------------------------------------------------------------------
	void CalculateLabelLayout(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the inner bounds of the diagram.
	// Returns:
	//     A chart rect object denoting the rectangular boundary.
	//-------------------------------------------------------------------------
	CXTPChartRectF GetInnerBounds() const;

	CRect GetBounds() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the pie diagram domain object associated
	//     with this object.
	// Parameters:
	//     rcBounds - The rectangular boundary of the diagram.
	// Returns:
	//     A pointer to the chart diagram domain object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagramDomain* CreateDiagramDomain(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the pie diagram domain object associated with
	//     this object.
	// Returns:
	//     A pointer to the chart diagram domain object.
	//-------------------------------------------------------------------------
	CXTPChartDiagramDomain* GetDomain() const;

	int GetMaxLabelWidth() const;

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to create the pie series point view object.
	// Parameters:
	//     pDC    - Pointer to chart device context.
	//     pPoint - Pointer to chart series point.
	// Returns:
	//     A pointer to the chart series point view.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the pie series.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	//     rcBounds - The rectangular boundary of the legend.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the pie series.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//{{AFX_CODEJOCK_PRIVATE
	void CalculateValues();
	//}}AFX_CODEJOCK_PRIVATE


protected:
	CXTPChartDiagramDomain* m_pDomain;  //Pointer to the diagram domain.

	CRect m_rcBounds;

	int m_nMaxLabelWidth;
};

//===========================================================================
// Summary:
//     This class abstracts the view of a pie series point.This class is a kind of
//     CXTPChartSeriesPointView.
// Remarks:
//     A pie chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pie chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPieSeriesPointView : public CXTPChartSeriesPointView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPieSeriesPointView object.
	// Parameters:
	//     pPoint        - Pointer to chart series point.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPieSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPieSeriesPointView object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPieSeriesPointView();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to calculate the layout of the pie series point.
	// Parameters:
	//     dAngle - The swipe angle of the sector(pie).
	//-------------------------------------------------------------------------
	void CalculateLayout(double& dAngle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the pie for the point.
	// Returns:
	//     A pointer CXTPChartPie object which represents a pie.
	//-------------------------------------------------------------------------
	CXTPChartPie* GetPie() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the pie for the point.
	// Returns:
	//     A pointer CXTPChartPie object which represents a pie.
	//-------------------------------------------------------------------------
	CXTPChartPointF GetBasePoint() const;

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the pie series point.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the pie series point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);

public:
	double m_dValue;            //The value of the point.
	CXTPChartPie* m_pPie;        //The pie for this point.
	CXTPChartPointF m_basePoint; //The base point.

	friend class CXTPChartPieSeriesView;
};


AFX_INLINE CXTPChartPie* CXTPChartPieSeriesPointView::GetPie() const {
	return m_pPie;
}
AFX_INLINE CXTPChartPointF CXTPChartPieSeriesPointView::GetBasePoint() const {
	return m_basePoint;
}
AFX_INLINE CXTPChartDiagramDomain* CXTPChartPieSeriesView::GetDomain() const {
	return m_pDomain;
}
AFX_INLINE int CXTPChartPieSeriesView::GetMaxLabelWidth() const {
	return m_nMaxLabelWidth;
}
AFX_INLINE CRect CXTPChartPieSeriesView::GetBounds() const {
	return m_rcBounds;
}

#endif //#if !defined(__XTPCHARTPIEDIAGRAM_H__)
