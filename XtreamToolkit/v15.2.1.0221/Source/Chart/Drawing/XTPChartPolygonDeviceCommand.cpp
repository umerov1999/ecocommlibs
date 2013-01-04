// XTPChartDeviceCommand.cpp
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

#include "Common/XTPVC80Helpers.h"

#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "../Types/XTPChartTypes.h"
#include "../XTPChartDefines.h"

#include "XTPChartDeviceCommand.h"
#include "XTPChartPolygonDeviceCommand.h"
#include "XTPChartDeviceContext.h"

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartPolygonDeviceCommand

CXTPChartPolygonDeviceCommand::CXTPChartPolygonDeviceCommand(const CXTPChartPoints& points)
{
	m_points.Copy(points);

	if (points.GetSize() != 0)
	{
		double xMax = points[0].X, xMin = points[0].X;
		double yMax = points[0].Y, yMin = points[0].Y;

		for (int i = 1; i < points.GetSize(); i++)
		{
			xMax = max(xMax, points[i].X);
			yMax = max(yMax, points[i].Y);
			xMin = min(xMin, points[i].X);
			yMin = min(yMin, points[i].Y);
		}

		m_bounds.X = (REAL)xMin;
		m_bounds.Y = (REAL)yMin;
		m_bounds.Width = (REAL)(xMax - xMin);
		m_bounds.Height = (REAL)(yMax - yMin);
	}
}

CXTPChartElement* CXTPChartPolygonDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	if (point.x < m_bounds.GetLeft() || point.x > m_bounds.GetRight())
		return NULL;

	if (point.y < m_bounds.GetTop() || point.y > m_bounds.GetBottom())
		return NULL;

	GpPath* pGpPath;
	GdipCreatePath(FillModeAlternate, &pGpPath);
	GdipAddPathPolygon(pGpPath, (PointF*)m_points.GetData(), (int)m_points.GetSize());

	BOOL bResult = FALSE;
	GdipIsVisiblePathPointI(pGpPath, point.x, point.y, 0, &bResult);

	GdipDeletePath(pGpPath);

	if (bResult)
		return pParent;

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSolidPolygonDeviceCommand

CXTPChartSolidPolygonDeviceCommand::CXTPChartSolidPolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color)
	: CXTPChartPolygonDeviceCommand(points)
{
	m_color = color;
}

void CXTPChartSolidPolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);

	GdipFillPolygon(pDC->GetGraphics(), pGpBrush, (PointF*)m_points.GetData(), (int)m_points.GetSize(), FillModeAlternate);

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGradientPolygonDeviceCommand



CXTPChartGradientPolygonDeviceCommand::CXTPChartGradientPolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartLinearGradientMode nMode)
	: CXTPChartPolygonDeviceCommand(points)
{
	m_color = color;
	m_color2 = color2;
	m_nMode = nMode;

}

void CXTPChartGradientPolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpLineGradient* pGpBrush = NULL;

	if (m_nMode == xtpChartLinearGradientModeCenterHorizontal)
	{
		GdipCreateLineBrushFromRect((RectF*)&m_bounds, m_color.GetValue(), m_color.GetValue(), LinearGradientModeHorizontal, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = {m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(), m_color.GetValue()};
		REAL positions[] ={0, 0.45f, 0.55f, 1};
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else if (m_nMode == xtpChartLinearGradientModeCenterVertical)
	{
		GdipCreateLineBrushFromRect((RectF*)&m_bounds, m_color.GetValue(), m_color.GetValue(), LinearGradientModeVertical, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = {m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(), m_color.GetValue()};
		REAL positions[] ={0, 0.45f, 0.55f, 1};
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else
	{
		GdipCreateLineBrushFromRect((RectF*)&m_bounds, m_color.GetValue(), m_color2.GetValue(), (LinearGradientMode)m_nMode, WrapModeTileFlipXY, &pGpBrush);
	}

	GdipFillPolygon(pDC->GetGraphics(), pGpBrush, (PointF*)m_points.GetData(), (int)m_points.GetSize(), FillModeAlternate);


	GdipDeleteBrush(pGpBrush);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartHatchPolygonDeviceCommand

CXTPChartHatchPolygonDeviceCommand::CXTPChartHatchPolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartHatchStyle nStyle)
	: CXTPChartPolygonDeviceCommand(points)
{
	m_color = color;
	m_color2 = color2;
	m_nStyle = nStyle;
}

void CXTPChartHatchPolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpHatch* pGpBrush = NULL;
	GdipCreateHatchBrush((GpHatchStyle)m_nStyle, m_color.GetValue(), m_color2.GetValue(), &pGpBrush);

	GdipFillPolygon(pDC->GetGraphics(), pGpBrush, (PointF*)m_points.GetData(), (int)m_points.GetSize(), FillModeAlternate);


	GdipDeleteBrush(pGpBrush);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSolidSplinePolygonDeviceCommand

CXTPChartSolidSplinePolygonDeviceCommand::CXTPChartSolidSplinePolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color, BOOL bTwoSides)
{
	m_color = color;

	GpPath* pGpPath = 0;
	GdipCreatePath(FillModeAlternate, &pGpPath);
	GdipStartPathFigure(pGpPath);

	int nCount = (int)points.GetSize();

	if (bTwoSides == 2)
	{
		GdipAddPathCurve(pGpPath, (PointF*)points.GetData(), nCount);
	}
	else if (!bTwoSides)
	{
		GdipAddPathLine(pGpPath, points[0].X, points[0].Y, points[1].X, points[1].Y);
		GdipAddPathCurve(pGpPath, (PointF*)points.GetData() + 1, nCount - 2);
		GdipAddPathLine(pGpPath, points[nCount - 2].X, points[nCount - 2].Y, points[nCount - 1].X, points[nCount - 1].Y);
	}
	else
	{

		GdipAddPathCurve(pGpPath, (PointF*)points.GetData(), nCount / 2);
		GdipAddPathLine(pGpPath, points[nCount / 2].X, points[nCount / 2].Y, points[nCount / 2 + 1].X, points[nCount / 2 + 1].Y);

		GdipAddPathCurve(pGpPath, (PointF*)points.GetData() + nCount / 2, nCount / 2);

		GdipAddPathLine(pGpPath, points[nCount - 1].X, points[nCount - 1].Y, points[0].X, points[0].Y);
	}

	GdipClosePathFigures(pGpPath);

	m_pGpPath = pGpPath;

}

CXTPChartSolidSplinePolygonDeviceCommand::~CXTPChartSolidSplinePolygonDeviceCommand()
{
	GdipDeletePath(m_pGpPath);
}

CXTPChartElement* CXTPChartSolidSplinePolygonDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	BOOL bResult = FALSE;
	GdipIsVisiblePathPointI(m_pGpPath, point.x, point.y, 0, &bResult);

	if (bResult)
		return pParent;

	return NULL;
}


void CXTPChartSolidSplinePolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);

	GdipFillPath(pDC->GetGraphics(), pGpBrush, m_pGpPath);

	GdipDeleteBrush(pGpBrush);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartBoundedPoygonDeviceCommand

CXTPChartBoundedPolygonDeviceCommand::CXTPChartBoundedPolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color, int thickness)
{
	m_points.Copy(points);
	m_color = color;
	m_thickness = thickness;
}


void CXTPChartBoundedPolygonDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_thickness, UnitWorld, &pGpPen);

	GdipDrawPolygon(pDC->GetGraphics(), pGpPen, (PointF*)m_points.GetData(), (int)m_points.GetSize());


	GdipDeletePen(pGpPen);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartPolygonAntialiasingDeviceCommand

CXTPChartPolygonAntialiasingDeviceCommand::CXTPChartPolygonAntialiasingDeviceCommand(BOOL bAntiAlias)
{
	m_bAntiAlias = bAntiAlias;
}

CXTPChartPolygonAntialiasingDeviceCommand::~CXTPChartPolygonAntialiasingDeviceCommand()
{
}

void CXTPChartPolygonAntialiasingDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	GdipGetSmoothingMode(pDC->GetGraphics(), (SmoothingMode*)&m_bOldAntiAlias);
	GdipSetSmoothingMode(pDC->GetGraphics(), m_bAntiAlias ? SmoothingModeHighQuality : SmoothingModeDefault);
}

void CXTPChartPolygonAntialiasingDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	GdipSetSmoothingMode(pDC->GetGraphics(), (SmoothingMode)m_bOldAntiAlias);
}
