// XTPChartSplineAreaSeriesStyle.h
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
#if !defined(__XTPCHARTSPLINEAREASERIESSTYLE_H__)
#define __XTPCHARTSPLINEAREASERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;

//===========================================================================
// Summary:
//     This class represents a spline area series style, which is a kind of
//     CXTPChartAreaSeriesStyle.
// Remarks:
//     Spline area series displays graphically the quantitative data in a chart.
//     it displays a series as a set of points connected by a spline with all
//     the area filled in below the line.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSplineAreaSeriesStyle : public CXTPChartAreaSeriesStyle
{
	DECLARE_SERIAL(CXTPChartSplineAreaSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSplineAreaSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSplineAreaSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartSplineAreaSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSplineAreaSeriesStyle();

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to create a spline area series view object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	// Returns:
	//     A pointer to CXTPChartSeriesView which refers a newly created
	//     CXTPChartSplineAreaSeriesView.
	// Remarks:
	//     CXTPChartSplineAreaSeriesView class abstracts the view of a spine area
	//     series.
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartSplineAreaSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartSplineAreaSeriesStyle)
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:

};

//===========================================================================
// Summary:
//     CXTPChartSplineAreaSeriesView is a kind of CXTPChartSeriesView, this class
//     represents the view of the area series of a chart.
// Remarks:
//     Spline area series displays graphically the quantitative data in a chart.
//     it displays a series as a set of points connected by a spline with all
//     the area filled in below the line.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSplineAreaSeriesView : public CXTPChartAreaSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSplineAreaSeriesView object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSplineAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create a spline area series point view object.
	// Parameters:
	//     pDC      - Pointer to a chart device context object.
	//     pPoint   - Pointer to a chart series object.
	// Returns:
	//     A pointer to CXTPChartSeriesPointView which refers a  newly created.
	//     CXTPChartPointSeriesPointView object.
	// Remarks:
	//     CXTPChartSeriesPointView object abstracts the view of a point in a series.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a spline area series in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     A CXTPChartDeviceCommand object, this polymorphic object handles
	//     the rendering of an element in the chart.Here it handles the drawing
	//     of the spline area series of the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

};


#endif //#if !defined(__XTPCHARTSPLINEAREASERIESSTYLE_H__)
