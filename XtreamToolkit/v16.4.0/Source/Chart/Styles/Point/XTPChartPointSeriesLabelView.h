// XTPChartPointSeriesLabelView.h
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
#if !defined(__XTPCHARTPOINTSERIESLABELVIEW_H__)
#define __XTPCHARTPOINTSERIESLABELVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class abstracts the view of  the label of a point series, it is a kind of chart
//     series label view.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPointSeriesLabelView : public CXTPChartDiagram2DSeriesLabelView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPointSeriesLabelView object.
	// Parameters:
	//     pLabel - The pointer to chart series label object.
	//     pPointView - A pointer to chart series view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPointSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function calculates the anchor point of the label stem.
	// Returns:
	//     A CXTPChartPointF object denoting the anchor point.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartPointF GetAnchorPoint() const;
	virtual double GetAnchorAngle() const;
};



#endif //#if !defined(__XTPCHARTPOINTSERIESLABELVIEW_H__)
