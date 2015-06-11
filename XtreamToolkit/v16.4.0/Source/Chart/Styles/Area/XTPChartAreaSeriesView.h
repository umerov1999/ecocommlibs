// XTPChartAreaSeriesView.h
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
#if !defined(__XTPCHARTAREASERIESVIEW_H__)
#define __XTPCHARTAREASERIESVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;


//===========================================================================
// Summary:
//     CXTPChartAreaSeriesView is a kind of CXTPChartSeriesView, this class
//     represents the view of the area series of a chart.
// Remarks:
//     Area series displays graphically the quantitative data in a chart.
//     it displays a series as a set of points connected by a line or spline
//     with all the area filled in below the line.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAreaSeriesView : public CXTPChartDiagram2DSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAreaSeriesView object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create a chart area series point view object.
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

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual color of the series.
	// Returns:
	//     Returns the CXTPChartColor object representing an ARGB value.
	// Remarks:
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualColor() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the second actual color of the series.
	// Returns:
	//     Returns the CXTPChartColor object representing an ARGB value.
	// Remarks:
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualColor2() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual color of the series border.
	// Returns:
	//     Returns the CXTPChartColor object representing an ARGB value.
	// Remarks:
	//-------------------------------------------------------------------------
	CXTPChartColor GetBorderActualColor() const;

	CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);
};


#endif //#if !defined(__XTPCHARTAREASERIESVIEW_H__)
