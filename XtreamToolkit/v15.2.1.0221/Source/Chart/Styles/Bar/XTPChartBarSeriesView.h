// XTPChartBarSeriesView.h
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
#if !defined(__XTPCHARTBARSERIESVIEW_H__)
#define __XTPCHARTBARSERIESVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class represents the view of a bar series ,which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBarSeriesView : public CXTPChartDiagram2DSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBarSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBarSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the bar series point.
	// Parameters:
	//     pDC    - A pointer to chart device context.
	//     pPoint - A pointer to the chart series point object.
	// Returns:
	//     A pointer to CXTPChartSeriesPointView, which refers to a newly created
	//     CXTPChartBarSeriesPointView object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the layout of the bar series.
	// Parameters:
	//     pDC    - A pointer to chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	void BeforeUpdateRange(CXTPChartDeviceContext* pDC);

	CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);

protected:
	int m_nBarCount;        //The bar count in the series.
	int m_nBarIndex;        //The index of the bar series.

	BOOL m_bFirstStack;

	friend class CXTPChartBarSeriesPointView;
};

//===========================================================================
// Summary:
//     This class represents the view of a bar series point,which is a kind of
//     CXTPChartPointSeriesPointView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBarSeriesPointView : public CXTPChartSeriesPointView
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBarSeriesPointView object.
	// Parameters:
	//     pPoint - A pointer to the chart series point object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBarSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

public:
	virtual CXTPChartRectF GetScreenRect() const;

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a bar series point.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles the
	//     rendering of the bar series point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

public:
	CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);

};



#endif //#if !defined(__XTPCHARTBARSERIESVIEW_H__)
