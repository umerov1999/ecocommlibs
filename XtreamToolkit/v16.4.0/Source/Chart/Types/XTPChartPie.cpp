// XTPChartPie.cpp
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

#include "stdafx.h"
#include <math.h>

#include "../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"

#include <Common/XTPMathUtils.h>
#include "../Drawing/XTPChartDeviceCommand.h"
#include "../Drawing/XTPChartPieDeviceCommand.h"

#include "XTPChartPie.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartEllipse

CXTPChartEllipse::CXTPChartEllipse()
{
	m_dMajorSemiaxis = m_dMinorSemiaxis = 0;
	m_dArea = 0;
}

CXTPChartEllipse::CXTPChartEllipse(const CXTPPoint3d& ptCenter, double dMajorSemiaxis, double dMinorSemiaxis)
{
	m_ptCenter = ptCenter;
	m_dMajorSemiaxis = dMajorSemiaxis;
	m_dMinorSemiaxis = dMinorSemiaxis;

	m_dArea = CXTPMathUtils::m_dPI * m_dMajorSemiaxis * m_dMinorSemiaxis;
}

double CXTPChartEllipse::CalcEllipseSectorFinishAngle(double areaSector, double startAngle) const
{
	double val = AtanMulTan(m_dMajorSemiaxis / m_dMinorSemiaxis, startAngle);
	double angle = 2 * areaSector / (m_dMajorSemiaxis * m_dMinorSemiaxis) + val;
	return AtanMulTan(m_dMinorSemiaxis / m_dMajorSemiaxis, angle);
}

double CXTPChartEllipse::AtanMulTan(double multiplier, double tanAngle) const
{
	double angle = CXTPMathUtils::NormalizeRadian(tanAngle);
	double pi = CXTPMathUtils::m_dPI;

	double result = tanAngle;
	if (CXTPMathUtils::Compare(fabs(angle), pi / 2) != 0 && CXTPMathUtils::Compare(fabs(angle), pi / 2 * 3) != 0)
	{
		result = atan(multiplier * tan(angle)) + tanAngle - angle;
		if (CXTPMathUtils::Compare(fabs(angle), pi / 2) > 0 && CXTPMathUtils::Compare(fabs(angle), pi / 2 * 3) < 0)
			result += CXTPMathUtils::Sign(tanAngle) * pi;
		else if (CXTPMathUtils::Compare(fabs(angle), pi / 2 * 3) > 0)
			result += CXTPMathUtils::Sign(tanAngle) * 2 * pi;
	}
	return result;
}

CXTPPoint3d CXTPChartEllipse::Polar2Cartesian(double angle, double radius) const
{
	return CXTPPoint3d(radius * cos(angle), -radius * sin(angle));
}


CXTPPoint3d CXTPChartEllipse::ApplyCenterPoint(const CXTPPoint3d& point) const
{
	CXTPPoint3d result = point;
	result.Offset(m_ptCenter.X, m_ptCenter.Y);
	return result;
}

double CXTPChartEllipse::CalcEllipseRadius(double angle) const
{
	return m_dMajorSemiaxis * m_dMinorSemiaxis / sqrt(pow(m_dMajorSemiaxis* sin(angle), 2.0) + pow(m_dMinorSemiaxis * cos(angle), 2.0));
}


CXTPPoint3d CXTPChartEllipse::CalcEllipsePoint(double angle) const
{
	CXTPPoint3d point = Polar2Cartesian(angle, CalcEllipseRadius(angle));
	return ApplyCenterPoint(point);
}

double CXTPChartEllipse::CalcEllipseSectorArea(double startAngle, double finishAngle) const
{
	double val1 = AtanMulTan(m_dMajorSemiaxis / m_dMinorSemiaxis, finishAngle);
	double val2 = AtanMulTan(m_dMajorSemiaxis / m_dMinorSemiaxis, startAngle);
	return m_dMajorSemiaxis *  m_dMinorSemiaxis / 2.0 * (val1 - val2);
}

double CXTPChartEllipse::CalcEllipseAngleFromCircleAngle(double angle) const
{
	return AtanMulTan(m_dMinorSemiaxis / m_dMajorSemiaxis, angle);
}



//////////////////////////////////////////////////////////////////////////
// CXTPChartPie

CXTPChartPie::CXTPChartPie(double dStartAngle, double dFinishAngle, const CXTPChartEllipse& ellipse, int nDepthPercent, int nHolePercent)
{
	m_ellipse = ellipse;

	m_dStartAngle = dStartAngle;
	m_dSweepAngle = dFinishAngle - m_dStartAngle;

	m_depth = m_ellipse.m_dMajorSemiaxis * nDepthPercent / 50.0;

	m_nHolePercent = nHolePercent;

	UpdateBounds();
}

//===========================================================================
// Summary:
//     This class represents the bounds of a pie.
// Remarks:
//===========================================================================
class CXTPChartBounds
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs and initialize CXTPChartBounds object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBounds()
	{
		xMin = INT_MAX, xMax = -INT_MAX, yMin = INT_MAX, yMax = -INT_MAX;
	}

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to update bounds of the pie.
	// Parameters:
	//     point - CXTPPoint3d object.
	// Remarks:
	//-----------------------------------------------------------------------
	void Update(const CXTPPoint3d& point)
	{
		xMax = max(xMax, point.X);
		yMax = max(yMax, point.Y);
		xMin = min(xMin, point.X);
		yMin = min(yMin, point.Y);
	}

public:
	double xMin;    //The minumum value in x co-ordinate
	double xMax;    //The maximum value in x co-ordinate
	double yMin;    //The minumum value in y co-ordinate
	double yMax;    //The maximum value in y co-ordinate
};

void CXTPChartPie::UpdateBounds(CXTPChartBounds* bounds, CXTPChartEllipse& ellipse)
{
	bounds->Update(ellipse.CalcEllipsePoint(m_dStartAngle));
	bounds->Update(ellipse.CalcEllipsePoint(m_dStartAngle + m_dSweepAngle));

	double normalizedStartAngle = CXTPMathUtils::NormalizeRadian(m_dStartAngle);
	double normalizedFinishAngle = normalizedStartAngle + m_dSweepAngle;
	if (normalizedFinishAngle < normalizedStartAngle) {
		double temp = normalizedStartAngle;
		normalizedStartAngle = normalizedFinishAngle;
		normalizedFinishAngle = temp;
	}
	double beginAngle = -4 * CXTPMathUtils::m_dPI;
	double endAngle = 4 * CXTPMathUtils::m_dPI;
	for (int multiplier = 0;; multiplier++) {
		double angle = beginAngle + multiplier * CXTPMathUtils::m_dPI / 2;
		if (CXTPMathUtils::Compare(angle, endAngle) > 0)
			break;
		if (CXTPMathUtils::Compare(angle, normalizedStartAngle) > 0 && CXTPMathUtils::Compare(angle, normalizedFinishAngle) < 0)
			bounds->Update(ellipse.CalcEllipsePoint(angle));
	}
}

void CXTPChartPie::UpdateBounds()
{
	CXTPChartBounds bounds;
	UpdateBounds(&bounds, m_ellipse);

	if (m_nHolePercent > 0)
	{
		CXTPChartEllipse innerEllipse(m_ellipse.GetCenter(),
			m_ellipse.m_dMajorSemiaxis * m_nHolePercent / 100.0, m_ellipse.m_dMinorSemiaxis * m_nHolePercent / 100.0);
		UpdateBounds(&bounds, innerEllipse);
	}
	else
	{
		bounds.Update(m_ellipse.GetCenter());

	}

	m_anchorBounds = CXTPChartRectF((float)bounds.xMin, (float)bounds.yMin, (float)(bounds.xMax - bounds.xMin), (float)(bounds.yMax - bounds.yMin));
}


CXTPPoint3d CXTPChartPie::CalculateCenter(const CXTPChartPointF& basePoint) const
{
	CXTPPoint3d center = m_ellipse.GetCenter();
	center.X += basePoint.X;
	center.Y += basePoint.Y;
	return center;
}

double CXTPChartPie::GetHalfAngle() const
{
	return m_ellipse.CalcEllipseSectorFinishAngle(m_ellipse.CalcEllipseSectorArea(m_dStartAngle, m_dSweepAngle + m_dStartAngle) / 2, m_dStartAngle);
}

CXTPChartPointF CXTPChartPie::GetFinishPoint() const
{
	return m_ellipse.CalcEllipsePoint(m_dSweepAngle + m_dStartAngle);
}

CXTPChartPointF CXTPChartPie::GetStartPoint() const
{
	return m_ellipse.CalcEllipsePoint(m_dStartAngle);
}

CXTPChartPointF CXTPChartPie::GetCenterPoint() const
{
	return m_ellipse.GetCenter();
}
CXTPChartRectF CXTPChartPie::GetBounds() const
{
	return CXTPChartRectF((float)(m_ellipse.m_ptCenter.X - m_ellipse.m_dMajorSemiaxis), (float)(m_ellipse.m_ptCenter.Y - m_ellipse.m_dMinorSemiaxis),
		(float)(2 * m_ellipse.m_dMajorSemiaxis), (float)(2 * m_ellipse.m_dMinorSemiaxis));
}

CXTPChartDeviceCommand* CXTPChartPie::CreatePieDeviceCommand(const CXTPChartColor color, const CXTPChartColor color2, const CXTPChartPointF& basePoint)
{
	CXTPPoint3d center = CalculateCenter(basePoint);

	CXTPChartRectF bounds(m_anchorBounds);
	bounds.Offset(basePoint);

	return new CXTPChartGradientPieDeviceCommand(center, m_ellipse.m_dMajorSemiaxis, m_ellipse.m_dMinorSemiaxis,
		-CXTPMathUtils::Radian2Degree(m_dStartAngle), -CXTPMathUtils::Radian2Degree(m_dSweepAngle), m_depth, m_nHolePercent, bounds, color2, color);
}

CXTPChartDeviceCommand* CXTPChartPie::CreateBoundedPieDeviceCommand(const CXTPChartColor color, int nThickness, const CXTPChartPointF& basePoint)
{
	if (nThickness == 0)
		return 0;

	CXTPPoint3d center = CalculateCenter(basePoint);

	CXTPChartRectF bounds(m_anchorBounds);
	bounds.Offset(basePoint);

	return new CXTPChartBoundedPieDeviceCommand(center, m_ellipse.m_dMajorSemiaxis, m_ellipse.m_dMinorSemiaxis,
		-CXTPMathUtils::Radian2Degree(m_dStartAngle), -CXTPMathUtils::Radian2Degree(m_dSweepAngle), m_depth, m_nHolePercent, color, nThickness);
}
