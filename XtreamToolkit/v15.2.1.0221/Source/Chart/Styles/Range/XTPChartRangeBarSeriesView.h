// XTPChartRangeBarSeriesView.h
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
#if !defined(__XTPCHARTRANGEBARSERIESVIEW_H__)
#define __XTPCHARTRANGEBARSERIESVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class represents the view of a RangeBar series ,which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeBarSeriesView : public CXTPChartBarSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeBarSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeBarSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the RangeBar series point.
	// Parameters:
	//     pDC    - A pointer to chart device context.
	//     pPoint - A pointer to the chart series point object.
	// Returns:
	//     A pointer to CXTPChartSeriesPointView, which refers to a newly created
	//     CXTPChartRangeBarSeriesPointView object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

protected:

	friend class CXTPChartRangeBarSeriesPointView;
};

//===========================================================================
// Summary:
//     This class represents the view of a RangeBar series point,which is a kind of
//     CXTPChartPointSeriesPointView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeBarSeriesPointView : public CXTPChartBarSeriesPointView
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeBarSeriesPointView object.
	// Parameters:
	//     pPoint - A pointer to the chart series point object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeBarSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

public:
	virtual CXTPChartRectF GetScreenRect() const;

	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

public:
	virtual void UpdateMinMaxRange(double& nMinValue, double& nMaxValue) const;

	CXTPChartPointF GetScreenPoint(BOOL bMinValue) const;


public:

};

#endif //#if !defined(__XTPCHARTRANGEBARSERIESVIEW_H__)
