// XTPChartRadarSplineSeriesStyle.h
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
#if !defined(__XTPCHARTRADARSPLINESERIESSTYLE_H__)
#define __XTPCHARTRADARSPLINESERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartLineStyle;


//===========================================================================
// Summary:
//     This class represents the spline series style in the chart.The objects
//     of this class is a kind of point series style of a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarSplineSeriesStyle : public CXTPChartRadarLineSeriesStyle
{
	DECLARE_SERIAL(CXTPChartRadarSplineSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarSplineSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarSplineSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRadarSplineSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRadarSplineSeriesStyle();

public:

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the spline series.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     A pointer to CXTPChartRadarSplineSeriesView object which is a kind of
	//     CXTPChartSeriesView.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);


protected:
#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartRadarSplineSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartRadarSplineSeriesStyle)
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:

};

//===========================================================================
// Summary:
//     This class represents the view of a spline series ,which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarSplineSeriesView : public CXTPChartRadarLineSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarSplineSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarSplineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the spline series point.
	// Parameters:
	//     pDC    - A pointer to chart device context.
	//     pPoint - A pointer to the chart series point object.
	// Returns:
	//     A pointer to CXTPChartPointSeriesPointView object which is a kind of
	//     CXTPChartSeriesPointView object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a spline series.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the spline series.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

};


#endif //#if !defined(__XTPCHARTRADARSPLINESERIESSTYLE_H__)
