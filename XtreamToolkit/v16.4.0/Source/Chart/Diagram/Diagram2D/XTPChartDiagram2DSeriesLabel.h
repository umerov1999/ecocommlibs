// XTPChartDiagram2DSeriesLabel.h
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
#if !defined(__XTPCHARTDIAGRAM2DSERIESLABEL_H__)
#define __XTPCHARTDIAGRAM2DSERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


//===========================================================================
// Summary:
//     This class abstracts the label of a point series, it is a kind of chart
//     series label.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDiagram2DSeriesLabel : public CXTPChartSeriesLabel
{
	DECLARE_DYNAMIC(CXTPChartDiagram2DSeriesLabel);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDiagram2DSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDiagram2DSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartDiagram2DSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartDiagram2DSeriesLabel();

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
};

//===========================================================================
// Summary:
//     This class abstracts the view of  the label of a point series, it is a kind of chart
//     series label view.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDiagram2DSeriesLabelView : public CXTPChartSeriesLabelView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDiagram2DSeriesLabelView object.
	// Parameters:
	//     pLabel - The pointer to chart series label object.
	//     pPointView - A pointer to chart series view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDiagram2DSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the chart series label.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     handles the rendering of an element in the chart.Here it handles the drawing
	//     of the chart series label.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartPointF ptCenterPoint);
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartPointF ptCenterPoint, const CXTPChartString& text);
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartPointF& ptAnchorPoint, double dAnchorAngle, const CXTPChartString& text);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function calculates the anchor point of the label stem.
	// Returns:
	//     A CXTPChartPointF object denoting the anchor point.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartPointF GetAnchorPoint() const;
	virtual double GetAnchorAngle() const;

	virtual void CalculateLayout(CXTPChartDeviceContext* pDC);
	void CalculateLayout(CXTPChartDeviceContext* pDC, CXTPChartPointF ptCenterPoint);

	void CalculateLayout(CXTPChartDeviceContext* pDC, const CXTPChartPointF& ptAnchorPoint, double dAnchorAngle, const CXTPChartString& text);
	void CalculateLayout(CXTPChartDeviceContext* pDC, CXTPChartPointF ptCenterPoint, const CXTPChartString& text);


protected:
	CRect GetPaneRect() const;
};


#endif //#if !defined(__XTPCHARTDIAGRAM2DSERIESLABEL_H__)
