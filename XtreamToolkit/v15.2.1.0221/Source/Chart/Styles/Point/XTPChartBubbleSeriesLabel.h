// XTPChartBubbleSeriesLabel.h
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
#if !defined(__XTPCHARTBUBBLESERIESLABEL_H__)
#define __XTPCHARTBUBBLESERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     CXTPChartBubbleSeriesLabel is a kind of CXTPChartSeriesLabel, this class
//     abstracts the label of a bubble series.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesLabel : public CXTPChartSeriesLabel
{
	DECLARE_SERIAL(CXTPChartBubbleSeriesLabel);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBubbleSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartBubbleSeriesLabel();

public:

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartBubbleSeriesLabelView)of the
	//     bubble series.
	// Parameters:
	//     pDC        - The chart device context object pointer.
	//     pPointView - The seris point view.
	// Returns:
	//     Returns an pointer to CXTPChartBubbleSeriesLabelView which is a kind of
	//     CXTPChartElementView object newly created.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);
protected:
};

//===========================================================================
// Summary:
//     CXTPChartBubbleSeriesLabelView is a kind of CXTPChartSeriesLabelView, this class
//     abstracts the view of a bubble series label.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesLabelView : public CXTPChartDiagram2DSeriesLabelView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesLabelView object.
	// Parameters:
	//     pLabel     - A pointer to the chart series label object.
	//     pPointView - A pointer to chart series point view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a bubble series label.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the bubble series label.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	void CalculateLayout(CXTPChartDeviceContext* pDC);
};

#endif //#if !defined(__XTPCHARTBUBBLESERIESLABEL_H__)
