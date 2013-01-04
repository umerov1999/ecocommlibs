// XTPChartPointSeriesStyle.h
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
#if !defined(__XTPCHARTPOINTSERIESSTYLE_H__)
#define __XTPCHARTPOINTSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartMarker;


//===========================================================================
// Summary:
//     This class represents a line series style, which is a kind of
//     CXTPChartSeriesStyle.
// Remarks:
//     A point chart is a type of graph, which displays information as a
//     series of data points.It is a basic type of chart common in many
//     fields.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPointSeriesStyle : public CXTPChartDiagram2DSeriesStyle
{
	DECLARE_SERIAL(CXTPChartPointSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPointSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPointSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPointSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPointSeriesStyle();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the marker object associated with this
	//     CXTPChartPointSeriesStyle object.
	// Returns:
	//     A pointer to CXTPChartMarker object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartMarker* GetMarker() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the point series.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     A pointer to CXTPChartPointSeriesView object which is a kind of
	//     CXTPChartSeriesView object.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartPointSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartPointSeriesStyle)
	LPDISPATCH OleGetMarker();

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartMarker* m_pMarker;
};

AFX_INLINE CXTPChartMarker* CXTPChartPointSeriesStyle::GetMarker() const {
	return m_pMarker;
}

#endif //#if !defined(__XTPCHARTPOINTSERIESSTYLE_H__)
