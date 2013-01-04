// XTPChartFunnelDiagram.h
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
#if !defined(__XTPCHARTFUNNELDIAGRAM_H__)
#define __XTPCHARTFUNNELDIAGRAM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//===========================================================================
// Summary:
//     This class abstracts the pie diagram.This class is a kind of
//     CXTPChartDiagram.
// Remarks:
//     A pie chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pie chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelDiagram : public CXTPChartDiagram
{
	DECLARE_SERIAL(CXTPChartFunnelDiagram)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelDiagram object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFunnelDiagram();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the pie diagram.
	// Parameters:
	//     pDC           - A pointer to chart device context.
	//     pParent       - A pointer to the parent view.
	// Returns:
	//     A pointer to CXTPChartDiagramView object which points to a newly created
	//     CXTPChartFunnelDiagramView object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagramView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the pie series layout.
	// Parameters:
	//     pDC           - A pointer to chart device context.
	//     pView         - A pointer to the diagram view.
	//-------------------------------------------------------------------------
	virtual void CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView);


#ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartFunnelDiagram);
	DECLARE_OLECREATE_EX(CXTPChartFunnelDiagram)
	//}}AFX_CODEJOCK_PRIVATE
#endif

};

//===========================================================================
// Summary:
//     This class abstracts the view of a pie diagram.This class is a kind of
//     CXTPChartDiagramView.
// Remarks:
//     A pie chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pie chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelDiagramView : public CXTPChartDiagramView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelDiagramView object.
	// Parameters:
	//     pDiagram - A pointer to pie diagram object.
	//     pParent  - A pointer to the parent view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFunnelDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent);

protected:
};


#endif //#if !defined(__XTPCHARTFUNNELDIAGRAM_H__)
