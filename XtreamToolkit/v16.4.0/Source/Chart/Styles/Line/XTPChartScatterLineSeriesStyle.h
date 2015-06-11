// XTPChartScatterLineSeriesStyle.h
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
#if !defined(__XTPCHARTSCATTERLINESERIESSTYLE_H__)
#define __XTPCHARTSCATTERLINESERIESSTYLE_H__
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
class _XTP_EXT_CLASS CXTPChartScatterLineSeriesStyle : public CXTPChartLineSeriesStyle
{
	DECLARE_SERIAL(CXTPChartScatterLineSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartScatterLineSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartScatterLineSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartScatterLineSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartScatterLineSeriesStyle();


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

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartScatterLineSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartScatterLineSeriesStyle)
//}}AFX_CODEJOCK_PRIVATE
#endif


};


#endif //#if !defined(__XTPCHARTSCATTERLINESERIESSTYLE_H__)
