// XTPChartRadarAxis.h
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
#if !defined(__XTPCHARTRADARAXIS_H__)
#define __XTPCHARTRADARAXIS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartRadarDiagram;


//===========================================================================
// Summary:
//     CXTPChartRadarAxisY specialized class from CXTPChartAxis class represents the X axis
//     of a chart.
// Remarks:It includes the features, specific to X axis.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisX : public CXTPChartAxis
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisX object.
	// Parameters:
	//     pDiagram     - Pointer to a CXTPChartDiagram2D object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarAxisX(CXTPChartRadarDiagram* pDiagram);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This is the override of base the class member, it tells that whether X axis is vertical.
	// Returns:
	//     Returns TRUE if the X axis is vertical, FALSE if the X axis is not vertical.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsVertical() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This is the override of base the class member,tells that whether the X axis is value type.
	// Returns:
	//     Returns TRUE if the X axis is value type, FALSE if the X axis is not value type.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsValuesAxis() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This override returns the side margin value of the chart X axis.
	// Parameters:
	//     nMinValue - The minimum value of the chart X axis.
	//     nMaxValue - The maximum value of the chart X axis.
	// Returns:
	//     Returns a double value, represents the margin.
	// See Also:
	//-------------------------------------------------------------------------
	virtual double CalcSideMarginsValue(double nMinValue, double nMaxValue) const;

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartAxisView)of the axis.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pParentView - The parent view of the axis.
	// Returns:
	//     Returns an pointer to CXTPChartAxisView object newly created.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartAxisView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);

};

//===========================================================================
// Summary:
//     CXTPChartRadarAxisY specialized class from CXTPChartAxis class represents the Y axis
//     of a chart.
// Remarks:It includes the features, specific to Y axis.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisY : public CXTPChartAxis
{
public:
	CXTPChartRadarAxisY(CXTPChartRadarDiagram* pDiagram);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This is the override of base the class member, it tells that whether Y axis is vertical.
	// Returns:
	//     Returns TRUE if the Y axis is vertical, FALSE if the Y axis is not vertical.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsVertical() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This is the override of base the class member,tells that whether the Y axis is value type.
	// Returns:
	//     Returns TRUE if the Y axis is value type, FALSE if the Y axis is not value type.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsValuesAxis() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This override returns the side margin value of the chart Y axis.
	// Parameters:
	//     nMinValue - The minimum value of the chart Y axis.
	//     nMaxValue - The maximum value of the chart Y axis.
	// Returns:
	//     Returns a double value, represents the margin.
	// See Also:
	//-------------------------------------------------------------------------
	virtual double CalcSideMarginsValue(double nMinValue, double nMaxValue) const;

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartAxisView)of the axis.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pParentView - The parent view of the axis.
	// Returns:
	//     Returns an pointer to CXTPChartAxisView object newly created.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartAxisView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);

};

#endif //#if !defined(__XTPCHARTRADARAXIS_H__)
