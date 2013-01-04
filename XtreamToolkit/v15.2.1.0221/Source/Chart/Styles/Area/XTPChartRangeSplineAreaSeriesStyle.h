// XTPChartRangeSplineAreaSeriesStyle.h
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
#if !defined(__XTPCHARTRANGESPLINEAREASERIESSTYLE_H__)
#define __XTPCHARTRANGESPLINEAREASERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;

//===========================================================================
// Summary:
//     CXTPChartRangeSplineAreaSeriesStyle specialized class from CXTPChartAreaSeriesStyle
//     class represents the area series style.
// Remarks:
//     Area series displays graphically the quantitative data in a chart.
//     it displays a series as a set of points connected by a line or spline
//     with all the area filled in below the line.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeSplineAreaSeriesStyle : public CXTPChartAreaSeriesStyle
{
	DECLARE_SERIAL(CXTPChartRangeSplineAreaSeriesStyle)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeSplineAreaSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeSplineAreaSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRangeSplineAreaSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRangeSplineAreaSeriesStyle();

public:

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to create a chart series view object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	// Returns:
	//     A pointer to CXTPChartSeriesView object newly created.
	// Remarks:
	//     CXTPChartSeriesView abstracts the view of a series.
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartRangeSplineAreaSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartRangeSplineAreaSeriesStyle)

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:

};

//===========================================================================
// Summary:
//     CXTPChartRangeSplineAreaSeriesView is a kind of CXTPChartAreaSeriesView, this class
//     represents the view of the area series of a chart.
// Remarks:
//     Area series displays graphically the quantitative data in a chart.
//     it displays a series as a set of points connected by a line or spline
//     with all the area filled in below the line.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeSplineAreaSeriesView : public CXTPChartAreaSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeSplineAreaSeriesView object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeSplineAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of an area series in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this polymorphic object handles
	//     the rendering of an element in the chart.Here it handles the drawing
	//     of the area series of the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

};


//===========================================================================
// Summary:
//     This class represents the view of a RangeBar series point,which is a kind of
//     CXTPChartPointSeriesPointView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeSplineAreaSeriesPointView : public CXTPChartPointSeriesPointView
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeBarSeriesPointView object.
	// Parameters:
	//     pPoint - A pointer to the chart series point object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeSplineAreaSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

public:
	virtual void UpdateMinMaxRange(double& nMinValue, double& nMaxValue) const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of an area series in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this polymorphic object handles
	//     the rendering of an element in the chart.Here it handles the drawing
	//     of the area series of the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

};



#endif //#if !defined(__XTPCHARTRANGESPLINEAREASERIESSTYLE_H__)
