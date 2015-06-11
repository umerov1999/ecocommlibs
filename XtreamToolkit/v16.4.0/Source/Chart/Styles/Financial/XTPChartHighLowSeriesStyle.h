// XTPChartHighLowSeriesStyle.h
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
#if !defined(__XTPCHARTHIGHLOWSERIESSTYLE_H__)
#define __XTPCHARTHIGHLOWSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


//===========================================================================
// Summary:
//     This class represents the stock series style in the chart.The objects
//     of this class is a kind of series style of a chart.
// Remarks:
//     Stock charts are used primarily to describe price movements of a security
//     (finance), derivative, or currency over time.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartHighLowSeriesStyle : public CXTPChartDiagram2DSeriesStyle
{
	DECLARE_SERIAL(CXTPChartHighLowSeriesStyle);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartHighLowSeriesStyle object.
	//-----------------------------------------------------------------------
	CXTPChartHighLowSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartHighLowSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartHighLowSeriesStyle();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the line thickness of stock ticks.
	// Parameters:
	//     nLineThickness - The thickness of the tick lines.
	//-----------------------------------------------------------------------
	void SetLineThickness(int nLineThickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the line thickness of stock ticks.
	// Returns:
	//     An integer value for the thickness of the tick lines currently using.
	//-----------------------------------------------------------------------
	int GetLineThickness() const;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the user defined up day color of stock ticks.
	// Returns:
	//     An CXTPChartColor object repeseting a 32 bit ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetCustomUpColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the user defined down day color of stock ticks.
	// Returns:
	//     An CXTPChartColor object repeseting a 32 bit ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetCustomDownColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the up day color of stock ticks.
	// Returns:
	//     An CXTPChartColor object repeseting a 32 bit ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetUpColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the down day color of stock ticks.
	// Returns:
	//     An CXTPChartColor object repeseting a 32 bit ARGB value.
	//-----------------------------------------------------------------------
	CXTPChartColor GetDownColor() const;

protected:

	// Summary:
	//     Call this function to create the view of the stock series.
	// Returns:
	//     A pointer to CXTPChartHighLowSeriesView object, which is a kind of
	//     CXTPChartSeriesView.
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);



protected:

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartHighLowSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartHighLowSeriesStyle)

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	int m_nLineThickness;                // The stock tick line thickness.
	CXTPChartColor m_clrDownColor;  // User defined down day color.
	CXTPChartColor m_clrUpColor;    // User defined up day color.
};

//===========================================================================
// Summary:
//     This class abstracts the view of the stock series of the chart.The objects
//     of this class is a kind of series view object of a chart.
// Remarks:
//     Stock charts are used primarily to describe price movements of a security
//     (finance), derivative, or currency over time.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartHighLowSeriesView : public CXTPChartDiagram2DSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartHighLowSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to chart series object.
	//     pDiagramView - A pointer to chart diagram view object.
	//-----------------------------------------------------------------------
	CXTPChartHighLowSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create stock series point view object.
	// Parameters:
	//     pDC    - A pointer to chart device context.
	//     pPoint - A pointer to chart series point object.
	// Returns:
	//     A pointer to stock chart series point view object.
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);
};

//===========================================================================
// Summary:
//     This class abstracts the view of the stock series point of the chart.The objects
//     of this class is a kind of series point view object of a chart.
// Remarks:
//     Stock charts are used primarily to describe price movements of a security
//     (finance), derivative, or currency over time.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartHighLowSeriesPointView : public CXTPChartSeriesPointView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartHighLowSeriesPointView object.
	// Parameters:
	//     pPoint - A pointer to chart series point object.
	//-----------------------------------------------------------------------
	CXTPChartHighLowSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a stock chart series point.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the stock chart series point.
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the screen point from a point index.
	// Parameters:
	//     nIndex - A zero based index of the point in the series.
	// Returns:
	//     A CXTPChartPointF object specifying the coordinates of the point
	//     in the chosen index of the series.
	//-----------------------------------------------------------------------
	CXTPChartPointF GetScreenPoint(int nIndex);

};

AFX_INLINE int CXTPChartHighLowSeriesStyle::GetLineThickness() const {
	return m_nLineThickness;
}
AFX_INLINE void CXTPChartHighLowSeriesStyle::SetLineThickness(int nLineThickness) {
	m_nLineThickness = nLineThickness;
	OnChartChanged();
}
AFX_INLINE CXTPChartColor CXTPChartHighLowSeriesStyle::GetCustomUpColor() const {
	return m_clrUpColor;
}
AFX_INLINE CXTPChartColor CXTPChartHighLowSeriesStyle::GetCustomDownColor() const {
	return m_clrDownColor;
}


#endif //#if !defined(__XTPCHARTHIGHLOWSERIESSTYLE_H__)
