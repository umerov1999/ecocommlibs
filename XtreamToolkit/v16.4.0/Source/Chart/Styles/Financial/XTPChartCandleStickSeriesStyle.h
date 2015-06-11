// XTPChartCandleStickSeriesStyle.h
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
#if !defined(__XTPCHARTCANDLESTICKSERIESSTYLE_H__)
#define __XTPCHARTCANDLESTICKSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


//===========================================================================
// Summary:
//     This class represents a candle stick series style, which is a kind of
//     CXTPChartHighLowSeriesStyle.
// Remarks:
//     A candlestick chart is a style of bar-chart used primarily to describe
//     price movements of a security (finance), derivative, or currency over time.
//     It is a combination of a line-chart and a bar-chart, in that each bar
//     represents the range of price movement over a given time interval. It is
//     most often used in technical analysis of equity and currency price patterns.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartCandleStickSeriesStyle : public CXTPChartHighLowSeriesStyle
{
	DECLARE_SERIAL(CXTPChartCandleStickSeriesStyle)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartCandleStickSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartCandleStickSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBubbleSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartCandleStickSeriesStyle();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the candlestick series.
	// Returns:
	//     Newly created the CXTPChartCandleStickSeriesView pointer, which is
	//     a kind of CXTPChartSeriesView*.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartCandleStickSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartCandleStickSeriesStyle)
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class represents the view of a candle stick series, which is a kind
//     of  CXTPChartSeriesView.
// Remarks:
//     A candlestick chart is a style of bar-chart used primarily to describe
//     price movements of a security (finance), derivative, or currency over time.
//     It is a combination of a line-chart and a bar-chart, in that each bar
//     represents the range of price movement over a given time interval. It is
//     most often used in technical analysis of equity and currency price patterns.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartCandleStickSeriesView : public CXTPChartDiagram2DSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartCandleStickSeriesView object.
	// Parameters
	//     pSeries - The chart series object pointer to which this object
	//               is associated with.
	//     pDiagramView - The pointer to the diagram view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartCandleStickSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a new candlestick series point view object.
	// Parameters
	//     pDC - The pointer to the chart device context.
	//               is associated with.
	//     pPoint - The pointer to the chart series point object.
	// Returns:
	//     A pointer to CXTPChartCandleStickSeriesPointView , which is a kind of
	//     CXTPChartSeriesPointView.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);
};

//===========================================================================
// Summary:
//     This class represents the view of a candle stick series point,which is
//     a kind of  CXTPChartSeriesPointView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartCandleStickSeriesPointView : public CXTPChartSeriesPointView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartCandleStickSeriesPointView object.
	// Parameters
	//     pPoint - A pointer to chart series point object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartCandleStickSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create CXTPChartDeviceCommand object, this object
	//     handles the rendering of the candlestick series point.
	// Parameters:
	//     pDC - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of a candlestick series point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the screen point from a series index.
	// Parameters:
	//     nIndex - An integer value denoting the index.
	// Returns:
	//     Returns a screen point object CXTPChartPointF.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartPointF GetScreenPoint(int nIndex);

};


#endif //#if !defined(__XTPCHARTCANDLESTICKSERIESSTYLE_H__)
