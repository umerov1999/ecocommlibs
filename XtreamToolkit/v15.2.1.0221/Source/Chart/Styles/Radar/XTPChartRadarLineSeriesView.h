// XTPChartRadarLineSeriesView.h
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
#if !defined(__XTPCHARTRADARLINESERIESVIEW_H__)
#define __XTPCHARTRADARLINESERIESVIEW_H__
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
class _XTP_EXT_CLASS CXTPChartRadarLineSeriesView : public CXTPChartRadarDiagramSeriesView
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
	CXTPChartRadarLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

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
	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);
};


#endif //#if !defined(__XTPCHARTRADARLINESERIESVIEW_H__)
