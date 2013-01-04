// XTPChartPie.h
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
#if !defined(__XTPCHARTPIE_H__)
#define __XTPCHARTPIE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartPie;
class CXTPChartDeviceCommand;
class CXTPChartBounds;

//===========================================================================
// Summary:
//     This class abstracts an ellipse.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartEllipse
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartEllipse object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartEllipse();

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a CXTPChartSplineSeriesView object.
	// Parameters:
	//     ptCenter       - The center point of the ellipse.
	//     dMajorSemiaxis - The length of the major semi axis.
	//     dMinorSemiaxis - The length of the minor semi axis.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartEllipse(const CXTPPoint3d& ptCenter, double dMajorSemiaxis, double dMinorSemiaxis);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the area of the ellipse.
	// Returns:
	//     A double value specifying the area of the ellipse.
	// Remarks:
	//-----------------------------------------------------------------------
	double GetArea() const;
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the center of the ellipse.
	// Returns:
	//     A CXTPPoint3d value specifying the center point of the ellipse.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint3d GetCenter() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the finish angle, from start angle and area
	//     of a sector.
	// Returns:
	//     A double value specifying the finish angle.
	// Remarks:
	//-----------------------------------------------------------------------
	double CalcEllipseSectorFinishAngle(double areaSector, double startAngle) const;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to covert polar coordinates to Cartesian co-ordinates.
	// Parameters:
	//     angle  - The angle in degree.
	//     radius - The radius of the point.
	// Returns:
	//     A CXTPPoint3d specifying the Cartesian co-ordinate.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint3d Polar2Cartesian(double angle, double radius) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the radius of the ellipse from
	//     the given angle
	// Parameters:
	//     angle  - The angle in degree.
	// Returns:
	//     A double value specifying the radius of the ellipse.
	// Remarks:
	//-----------------------------------------------------------------------
	double CalcEllipseRadius(double angle) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the ellipse Cartesian point
	//     on a given angle
	// Parameters:
	//     angle  - The angle in degree.
	// Returns:
	//     A CXTPPoint3d denoting the Cartesian coordinate.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint3d CalcEllipsePoint(double angle) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the sector area from a start angle
	//     and finish angle.
	// Parameters:
	//     startAngle  - The start angle in degree.
	//     finishAngle - The finish angle in degree.
	// Returns:
	//     A double value specifying the sector area.
	// Remarks:
	//-----------------------------------------------------------------------
	double CalcEllipseSectorArea(double startAngle, double finishAngle) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the ellipse angle from circle angle.
	// Parameters:
	//     angle  - The circle angle.
	// Returns:
	//     A double value ellipse angle.
	// Remarks:
	//-----------------------------------------------------------------------
	double CalcEllipseAngleFromCircleAngle(double angle) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to apply the center point of the ellipse.
	// Parameters:
	//     point  - The center point.
	// Returns:
	//     A CXTPPoint3d value specifying the new center point.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint3d ApplyCenterPoint(const CXTPPoint3d& point) const;

	//{{AFX_CODEJOCK_PRIVATE
	double AtanMulTan(double multiplier, double tanAngle) const;
	//}}AFX_CODEJOCK_PRIVATE


public:
	CXTPPoint3d m_ptCenter;        //The center of the ellipse.
	double m_dMajorSemiaxis;                //The major semi axis of the ellipse.
	double m_dMinorSemiaxis;                //The minor semi axis of the ellipse.

	double m_dArea;                         //The area of the ellipse.

	friend class CXTPChartPie;
};

//===========================================================================
// Summary:
//     This class abstracts an pie chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPie
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPie object.
	// Parameters:
	//     dStartAngle      - The start angle of the pie.
	//     dFinishAngle     - The finish angle of the pie.
	//     ellipse          - The ellipse associated with the pie.
	//     nDepthPercent    - The depth percentage, in case of 3D chart.
	//     nHolePercent     - The hole percentage.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPie(double dStartAngle, double dFinishAngle, const CXTPChartEllipse& ellipse, int nDepthPercent, int nHolePercent);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the ceter of the pie.
	// Parameters:
	//     basePoint - The base point in the diagram.
	// Returns:
	//     A CXTPPoint3d object denoting the center point of the pie.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint3d CalculateCenter(const CXTPChartPointF& basePoint) const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the pie.
	// Parameters:
	//     color      - The first color used for gradient pie.
	//     color2     - The second color used for gradient pie.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the pie.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreatePieDeviceCommand(const CXTPChartColor color, const CXTPChartColor color2, const CXTPChartPointF& basePoint);
	CXTPChartDeviceCommand* CreateBoundedPieDeviceCommand(const CXTPChartColor color, int nThickness, const CXTPChartPointF& basePoint);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the half angle of the pie slice.
	// Returns:
	//     A double specifying the half angle.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	double GetHalfAngle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the start angle of the pie slice.
	// Returns:
	//     A double specifying the start angle.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	double GetStartAngle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the finish point of the pie slice.
	// Returns:
	//     A CXTPChartPointF object specifying the finish point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartPointF GetFinishPoint() const;
	CXTPChartPointF GetStartPoint() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the center point of the pie slice.
	// Returns:
	//     A CXTPChartPointF object specifying the center point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartPointF GetCenterPoint() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the bounding rectangle the pie chart.
	// Returns:
	//     A CXTPChartRectF object specifying the bounds.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartRectF GetBounds() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the hole percentage of the pie chart.
	// Returns:
	//     An integer specifying the percentage of hole in the pie chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	int GetHolePercent() const;

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to update the bounds of  the pie when there is
	//     a change in the start angle and/or sweep angle.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void UpdateBounds();


	//{{AFX_CODEJOCK_PRIVATE
	void UpdateBounds(CXTPChartBounds* bounds, CXTPChartEllipse& ellipse);
	//}}AFX_CODEJOCK_PRIVATE


protected:
	CXTPChartEllipse m_ellipse;      //The ellipse associated with the pie chart.
	double m_dStartAngle;           //The start angle of the slice.
	double m_dSweepAngle;           //The sweep angle of the slice.
	double m_depth;                 //The depth of the 3D pie.
	int m_nHolePercent;             //The hole percentage.
	CXTPChartRectF m_anchorBounds;   //The anchor bounds.
};


AFX_INLINE double CXTPChartEllipse::GetArea() const {
	return m_dArea;
}

AFX_INLINE CXTPPoint3d CXTPChartEllipse::GetCenter() const {
	return m_ptCenter;
}
AFX_INLINE double CXTPChartPie::GetStartAngle() const {
	return m_dStartAngle;
}
AFX_INLINE int CXTPChartPie::GetHolePercent() const {
	return m_nHolePercent;
}

#endif //#if !defined(__XTPCHARTPIE_H__)
