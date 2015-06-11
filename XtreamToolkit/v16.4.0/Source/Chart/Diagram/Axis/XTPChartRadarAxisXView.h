// XTPChartRadarAxisXView.h
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
#if !defined(__XTPCHARTRADARAXISXVIEW_H__)
#define __XTPCHARTRADARAXISXVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartRadarAxisXView;
class CXTPChartAxisRange;
class CXTPChartAxis;
class CXTPChartSeriesView;

//===========================================================================
// Summary:
//     This class represents a tick mark in the axis.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisXViewTick
{
public:
	double m_dValue;            //The value of the tick mark.
	CXTPChartString m_strLabel;  //The tick mark lablel.
	CSize m_szBounds;           //The boundary.
	CXTPChartSizeF m_szLabel;    //The size of the label.
	CXTPChartPointF m_ptLocation;
};
//===========================================================================
// Summary:
//     This class abstracts the view of a chart axis and its child items in
//     a bounding rectangle context.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisXView : public CXTPChartRadarAxisView
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarAxisXView object.
	// Parameters:
	//     pAxis     - Pointer to a CXTPChartAxis object.
	//     pParentView - Pointer to a CXTPChartElementView object.
	// Remarks:
	// See Also:
	//-----------------------------------------------------------------------
	CXTPChartRadarAxisXView(CXTPChartAxis* pAxis, CXTPChartElementView* pParentView);
	~CXTPChartRadarAxisXView();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Override this function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of an axis in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of an axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the grid lines the chart axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     handles the rendering of an element in the chart.Here it handles the drawing
	//     of the grid lines in an axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateGridLinesDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the interlaced strips in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     handles the rendering of an element in the chart.Here it handles the drawing
	//     of the interlaced strips in the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateInterlacedDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the tick marks in the axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     handles the rendering of an element in the chart.Here it handles the drawing
	//     of the tick marks in the axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateTickMarksDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the labels in the axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     handles the rendering of an element in the chart.Here it handles the drawing
	//     of the labels in the axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateLabelsDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the contant lines perpendicular to an axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     handles the rendering of an element in the chart.Here it handles the drawing
	//     of the contant lines perpendicular to an axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateConstantLinesDeviceCommand(CXTPChartDeviceContext* pDC, BOOL bBehind);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the strips parallel to an axis.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     rcPane  - The bounding rectange.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     the rendering of an element in the chart.Here it handles the drawing
	//     handles of the strips parallel to an axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create chart axis view.
	// Parameters:
	//     pDC     - The chart device context object.
	// Remarks:
	//     This function create the view of an axis and its contituent elemets
	//     like contant lines and strips.
	// See Also:
	//-------------------------------------------------------------------------
	void CreateView(CXTPChartDeviceContext* pDC);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function calculates the size of axis, including the constituent elements
	//     like, tick marks, labels amd axis title etc.
	// Parameters:
	//     pDC       - The chart device context object.
	//     rcDiagram - The rectangle of the diagram.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void CalcSize(CXTPChartDeviceContext* pDC, CRect rcDiagram);
	//-------------------------------------------------------------------------
	// Summary:
	//     This function the bounds for the axis.
	// Parameters:
	//     pDC       - The chart device context object.
	//     rcBounds  - The bounding rectangle.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetBounds(CXTPChartDeviceContext* pDC, CRect rcBounds);

	double ValueToAngle(double nValue) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns the size of the axis.
	// Returns:
	//     x - A point within the axis range.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	int GetSize() const;


protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create the axis tick marks.
	// Parameters:
	//     pDC -       The device context.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void CreateTickMarks(CXTPChartDeviceContext* pDC);


public:
	CArray<CXTPChartRadarAxisXViewTick, CXTPChartRadarAxisXViewTick&> m_arrTicks;         //The collection of tick marks.

	CArray<double, double&> m_arrMinorTicks;

protected:
	CRect m_rcBounds;           //The diagram boundary.
	int m_nSize;                //The axis size;

	double m_dMinValue;
	double m_dMaxValue;

	CXTPPoint2i m_ptCenter;
	int m_nRadius;
};

AFX_INLINE int CXTPChartRadarAxisXView::GetSize() const {
	return m_nSize;
}

#endif //#if !defined(__XTPCHARTRADARAXISXVIEW_H__)
