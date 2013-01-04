// XTPChartPieDeviceCommand.cpp
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

#include "stdafx.h"

#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"


#include "../Utils/XTPChartMathUtils.h"
#include "XTPChartDeviceContext.h"
#include "XTPChartDeviceCommand.h"

#include "../Types/XTPChartPie.h"
#include "XTPChartPieDeviceCommand.h"

#include <math.h>

using namespace Gdiplus;
using namespace Gdiplus::DllExports;


//////////////////////////////////////////////////////////////////////////
// CXTPChartPieDeviceCommand

CXTPChartPieDeviceCommand::CXTPChartPieDeviceCommand(const CXTPPoint3d& center, double dMajorSemiAxis, double dMinorSemiAxis, double dStartAngle, double dSweepAngle, double depth, double nHolePercent)
{
	m_ptCenter = center;
	m_dMajorSemiAxis = dMajorSemiAxis;
	m_dMinorSemiAxis = dMinorSemiAxis;
	m_dStartAngle = dStartAngle;
	m_dSweepAngle = dSweepAngle;
	m_dDepth = depth;
	m_dHolePercent = nHolePercent / 100.0;

	m_dFacetSize = 0;

	m_dInnerMajorSemiAxis = 0;
	m_dInnerMinorSemiAxis = 0;

	m_dCorrectedDepth = 0;

	m_dInnerRadius = 0;

	m_bDoughnut = nHolePercent > CXTPChartMathUtils::m_dEPS;

	if (m_bDoughnut)
	{
		double doughnutThickness = dMinorSemiAxis * (1.0 - m_dHolePercent);
		m_dInnerMajorSemiAxis = dMajorSemiAxis - doughnutThickness;
		m_dInnerMinorSemiAxis = dMinorSemiAxis - doughnutThickness;
	}

	if (depth > 0.0f)
	{
		m_dFacetSize = dMajorSemiAxis * 0.02f;
		m_dFacetSize = 0;

		if (m_dFacetSize > depth / 2.0f)
			m_dFacetSize = depth / 2.0f;
		m_dRadius = dMajorSemiAxis - m_dFacetSize;
		float diff = float(m_dRadius - m_dInnerMajorSemiAxis);
		if (diff < 0.0f) {
			m_dRadius -= diff;
			m_dFacetSize += diff;
		}

		m_dCorrectedDepth = depth - m_dFacetSize * 2;

		if (m_bDoughnut) {
			m_dInnerRadius = m_dInnerMajorSemiAxis;
			if (m_dInnerRadius > m_dRadius)
				m_dInnerRadius = m_dRadius;
		}

	}


}

void CXTPChartPieDeviceCommand::CalculateStartFinishPoints(const CXTPPoint3d& center, double majorSemiaxis, double minorSemiaxis, double dStartAngle, double dSweepAngle, CXTPChartPointF& startPoint, CXTPChartPointF& finishPoint) const
{
	CXTPChartEllipse ellipse(center, majorSemiaxis, minorSemiaxis);
	startPoint  = (CXTPChartPointF)ellipse.CalcEllipsePoint(-CXTPChartMathUtils::Degree2Radian(dStartAngle));
	finishPoint = (CXTPChartPointF)ellipse.CalcEllipsePoint(-CXTPChartMathUtils::Degree2Radian(dStartAngle + dSweepAngle));
}

CXTPChartElement* CXTPChartPieDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	GpPath* pGpPath = CreatePieGraphicsPath(m_ptCenter, m_dMajorSemiAxis, m_dMinorSemiAxis, m_dHolePercent, m_dStartAngle, m_dSweepAngle);

	BOOL bResult = FALSE;

	GdipIsVisiblePathPointI(pGpPath, point.x, point.y, NULL, &bResult);

	GdipDeletePath(pGpPath);

	return bResult ? pParent : NULL;
}

GpPath* CXTPChartPieDeviceCommand::CreatePieGraphicsPath(const CXTPPoint3d& center, double majorSemiaxis, double minorSemiaxis, double holePercent, double dStartAngle, double dSweepAngle)  const
{
	GpPath* pGpPath = NULL;
	GdipCreatePath(FillModeAlternate, &pGpPath);

	CRect rect(int(center.X - majorSemiaxis), int(center.Y - minorSemiaxis), int(center.X + majorSemiaxis), int(center.Y + minorSemiaxis));

	BOOL bShouldAddLines = CXTPChartMathUtils::Compare(dSweepAngle, -360.0) != 0;
	float dInnerMajorSemiaxis = (float)(majorSemiaxis * holePercent);
	float dInnerMinorSemiaxis = (float)(minorSemiaxis * holePercent);
	if (dInnerMajorSemiaxis >= 1.0f && dInnerMinorSemiaxis >= 1.0f)
	{
		CRect innerRect(int(center.X - dInnerMajorSemiaxis),  int(center.Y - dInnerMinorSemiaxis), int(center.X + dInnerMajorSemiaxis), int(center.Y + dInnerMinorSemiaxis));

		CXTPChartPointF startPoint, finishPoint, innerStartPoint, innerFinishPoint;

		if (bShouldAddLines)
		{
			CalculateStartFinishPoints(center, majorSemiaxis, minorSemiaxis, dStartAngle, dSweepAngle, startPoint, finishPoint);
			CalculateStartFinishPoints(center, dInnerMajorSemiaxis, dInnerMinorSemiaxis, dStartAngle, dSweepAngle, innerStartPoint, innerFinishPoint);
		}

		GdipAddPathArc(pGpPath, (REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(), (REAL)rect.Height(), (REAL)dStartAngle, (REAL)dSweepAngle);

		if (bShouldAddLines)
		{
			GdipAddPathLine(pGpPath, finishPoint.X, finishPoint.Y, innerFinishPoint.X, innerFinishPoint.Y);
		}
		GdipAddPathArc(pGpPath, (REAL)innerRect.left, (REAL)innerRect.top, (REAL)innerRect.Width(), (REAL)innerRect.Height(), (REAL)(dStartAngle + dSweepAngle), (REAL)(-dSweepAngle));

		if (bShouldAddLines)
			GdipAddPathLine(pGpPath, innerStartPoint.X, innerStartPoint.Y, startPoint.X, startPoint.Y);

	}
	else if (bShouldAddLines)
	{
		GdipAddPathPie(pGpPath, (REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(), (REAL)rect.Height(), (REAL)dStartAngle, (REAL)dSweepAngle);

	}
	else
	{
		GdipAddPathEllipse(pGpPath, (REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(), (REAL)rect.Height());

	}

	return pGpPath;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartGradientPieDeviceCommand

CXTPChartGradientPieDeviceCommand::CXTPChartGradientPieDeviceCommand(const CXTPPoint3d& center, double dMajorSemiAxis, double dMinorSemiAxis,
	double dStartAngle, double dSweepAngle, double depth, double holePercent, const CXTPChartRectF& gradientBounds, const CXTPChartColor& color, const CXTPChartColor& color2)
	: CXTPChartPieDeviceCommand(center, dMajorSemiAxis, dMinorSemiAxis, dStartAngle, dSweepAngle, depth, holePercent)
{
	m_rcGradientBounds = gradientBounds;
	m_color = color;
	m_color2 = color2;
}


GpBrush* CXTPChartGradientPieDeviceCommand::CreateBrush()
{
	GpLineGradient* pGpBrush = NULL;

	PointF ptGradient[2];
	ptGradient[0] = PointF((REAL)m_rcGradientBounds.X, (REAL)m_ptCenter.Y);
	ptGradient[1] = PointF((REAL)(m_rcGradientBounds.X + m_rcGradientBounds.Width), (REAL)m_ptCenter.Y);


	GdipCreateLineBrush(&ptGradient[0], &ptGradient[1], m_color.GetValue(), m_color2.GetValue(), WrapModeTile, &pGpBrush);

	return pGpBrush;
}

void CXTPChartGradientPieDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{

	GpBrush* pGpBrush = CreateBrush();


	GpPath* pGpPath = CreatePieGraphicsPath(m_ptCenter, m_dMajorSemiAxis, m_dMinorSemiAxis, m_dHolePercent, m_dStartAngle, m_dSweepAngle);

	GdipFillPath(pDC->GetGraphics(), pGpBrush, pGpPath);

	GdipDeletePath(pGpPath);

	GdipDeleteBrush(pGpBrush);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartBoundedPieDeviceCommand


CXTPChartBoundedPieDeviceCommand::CXTPChartBoundedPieDeviceCommand(const CXTPPoint3d& center, double dMajorSemiAxis, double dMinorSemiAxis, double dStartAngle, double dSweepAngle, double depth, int holePercent, const CXTPChartColor& color, int nThickness)
	: CXTPChartPieDeviceCommand(center, dMajorSemiAxis, dMinorSemiAxis, dStartAngle, dSweepAngle, depth, holePercent)
{
	m_color = color;
	m_nThickness = nThickness;
}

void CXTPChartBoundedPieDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_nThickness, UnitWorld, &pGpPen);


	GpPath* pGpPath = CreatePieGraphicsPath(m_ptCenter, m_dMajorSemiAxis, m_dMinorSemiAxis, m_dHolePercent, m_dStartAngle, m_dSweepAngle);

	GdipDrawPath(pDC->GetGraphics(), pGpPen, pGpPath);

	GdipDeletePath(pGpPath);

	GdipDeletePen(pGpPen);
}
