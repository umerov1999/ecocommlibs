// XTPChartRangeAreaSeriesLabel.h
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
#if !defined(__XTPCHARTRANGEAREASERIESLABEL_H__)
#define __XTPCHARTRANGEAREASERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


//===========================================================================
// Summary:
//     This class abstracts the label of a bar series, it is a kind of point
//     series label.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeAreaSeriesLabel : public CXTPChartPointSeriesLabel
{
	DECLARE_SERIAL(CXTPChartRangeAreaSeriesLabel);
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeAreaSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeAreaSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRangeAreaSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRangeAreaSeriesLabel();

public:

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function create the view of the bar series label.
	// Parameters:
	//     pDC -        The device context.
	//     pPointView - A pointer to the series point view object.
	// Returns:
	//     A pointer to CXTPChartElementView object, which a polymorphic to CXTPChartRangeAreaSeriesLabelView.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);


	void DoPropExchange(CXTPPropExchange* pPX);

protected:

};

//===========================================================================
// Summary:
//     This class abstracts the view of the label of a bar series, it is a kind of point
//     series label view.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeAreaSeriesLabelView : public CXTPChartPointSeriesLabelView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeAreaSeriesLabel object.
	// Parameters:
	//     pLabel     - Pointer chart series label.
	//     pPointView - Pointer to chart series point view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeAreaSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function calculates the anchor point of the label stem.
	// Returns:
	//     A CXTPChartPointF object denoting the anchor point.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartPointF GetAnchorPoint(BOOL bMinValue) const;


protected:
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
	void CalculateLayout(CXTPChartDeviceContext* pDC);
};

#endif //#if !defined(__XTPCHARTRANGEAREASERIESLABEL_H__)
