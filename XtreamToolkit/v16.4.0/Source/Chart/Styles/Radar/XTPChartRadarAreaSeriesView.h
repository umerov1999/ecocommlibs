// XTPChartRadarAreaSeriesView.h
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
#if !defined(__XTPCHARTRADARAREASERIESVIEW_H__)
#define __XTPCHARTRADARAREASERIESVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartMarker;



//===========================================================================
// Summary:
//     This class represents the view of a point series ,which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAreaSeriesView : public CXTPChartRadarDiagramSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPointSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the series points.
	// Parameters:
	//     pDC    - A pointer to chart device context.
	//     pPoint - A pointer to the chart series point object.
	// Returns:
	//     A pointer to CXTPChartPointSeriesPointView object which is a kind of
	//     CXTPChartSeriesPointView object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);


public:
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

public:
	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);
};


#endif //#if !defined(__XTPCHARTRADARAREASERIESVIEW_H__)
