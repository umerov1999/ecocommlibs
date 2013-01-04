// XTPChartGanttSeriesStyle.h
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
#if !defined(__XTPCHARTGANTTSERIESSTYLE_H__)
#define __XTPCHARTGANTTSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class represents a Gantt series style for the chart,which is a kind of
//     CXTPChartBarSeriesStyle.
// Remarks:
//    A Gantt chart or Gantt graph is a chart with rectangular Gantts with lengths
//    proportional to the values that they represent. Gantt charts are used for
//    comparing two or more values that were taken over time or on different
//    conditions, usually on small data sets.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGanttSeriesStyle : public CXTPChartRangeBarSeriesStyle
{
	DECLARE_SERIAL(CXTPChartGanttSeriesStyle)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGanttSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartGanttSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartGanttSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartGanttSeriesStyle();

public:


protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the Gantt series.
	// Parameters:
	//     pSeries      - A pointer to the chart series object, to which this object
	//                    is associated with.
	//     pDiagramView - A pointer to the chart diagram view object, to which this object
	//                    is associated with.
	// Returns:
	//     A pointer to CXTPChartSeriesView, refers a newly created CXTPChartGanttSeriesView
	//     object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

public:
	void DoPropExchange(CXTPPropExchange* pPX);
protected:

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartGanttSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartGanttSeriesStyle)
//}}AFX_CODEJOCK_PRIVATE
#endif
};
//===========================================================================
// Summary:
//     This class represents the view of a Gantt series ,which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGanttSeriesView : public CXTPChartRangeBarSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGanttSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartGanttSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the Gantt series point.
	// Parameters:
	//     pDC    - A pointer to chart device context.
	//     pPoint - A pointer to the chart series point object.
	// Returns:
	//     A pointer to CXTPChartSeriesPointView, which refers to a newly created
	//     CXTPChartGanttSeriesPointView object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

protected:

	friend class CXTPChartGanttSeriesPointView;
};

//===========================================================================
// Summary:
//     This class represents the view of a Gantt series point,which is a kind of
//     CXTPChartPointSeriesPointView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGanttSeriesPointView : public CXTPChartRangeBarSeriesPointView
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGanttSeriesPointView object.
	// Parameters:
	//     pPoint - A pointer to the chart series point object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartGanttSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

};


#endif //#if !defined(__XTPCHARTGANTTSERIESSTYLE_H__)
