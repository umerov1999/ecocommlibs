// XTPChartLineSeriesStyle.h
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
#if !defined(__XTPCHARTFASTLINESERIESSTYLE_H__)
#define __XTPCHARTFASTLINESERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartLineStyle;

//===========================================================================
// Summary:
//     This class represents a line series style,which is a kind of
//     CXTPChartPointSeriesStyle.
// Remarks:
//     A line chart or line graph is a type of graph, which displays information
//     as a series of data points connected by straight line segments.It is a
//     basic type of chart common in many fields.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFastLineSeriesStyle : public CXTPChartDiagram2DSeriesStyle
{
	DECLARE_SERIAL(CXTPChartFastLineSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFastLineSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFastLineSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFastLineSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartFastLineSeriesStyle();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the line style of the line series.
	// Returns:
	//     A pointer to chart line style object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartLineStyle* GetLineStyle() const;

	void SetAntialiasing(BOOL bAntialiased);
	BOOL GetAntialiasing() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the line series.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     A pointer to CXTPChartLineSeriesView object which is a kind of
	//     CXTPChartSeriesView.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

	void DoPropExchange(CXTPPropExchange* pPX);
#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartFastLineSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartFastLineSeriesStyle)
	LPDISPATCH OleGetLineStyle();
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartLineStyle* m_pLineStyle;        //The line style.
	BOOL m_bAntialiasing;

};

//===========================================================================
// Summary:
//     This class represents the view of a line series ,which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFastLineSeriesView : public CXTPChartDiagram2DSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartLineSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFastLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the line series point.
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
	//     represents the rendering of a line series.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the line series.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);

	void CreatePointsView(CXTPChartDeviceContext* pDC);

};


AFX_INLINE CXTPChartLineStyle* CXTPChartFastLineSeriesStyle::GetLineStyle() const {
	return m_pLineStyle;
}
AFX_INLINE BOOL CXTPChartFastLineSeriesStyle::GetAntialiasing() const {
	return m_bAntialiasing;
}
AFX_INLINE void CXTPChartFastLineSeriesStyle::SetAntialiasing(BOOL bAntialiasing)  {
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}



#endif //#if !defined(__XTPCHARTFASTLINESERIESSTYLE_H__)
