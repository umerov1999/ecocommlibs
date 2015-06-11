// XTPChartRectangleDeviceCommand.cpp
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

#include "Common/XTPVC80Helpers.h"

#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "../Types/XTPChartTypes.h"
#include "../XTPChartDefines.h"

#include "XTPChartDeviceCommand.h"
#include "XTPChartCircleDeviceCommand.h"
#include "XTPChartDeviceContext.h"


using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartCircleDeviceCommand

CXTPChartCircleDeviceCommand::CXTPChartCircleDeviceCommand(const CXTPChartPointF& center, double radius)
{
	m_center = center;
	m_radius = radius;
}


CXTPChartElement* CXTPChartCircleDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	if (point.x < m_center.X - m_radius || point.x > m_center.X + m_radius)
		return NULL;

	if (point.y < m_center.Y - m_radius || point.y > m_center.Y + m_radius)
		return NULL;

	if ((point.x - m_center.X) * (point.x - m_center.X) + (point.y - m_center.Y) * (point.y - m_center.Y) <= m_radius * m_radius)
		return pParent;

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSolidCircleDeviceCommand

CXTPChartSolidCircleDeviceCommand::CXTPChartSolidCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color)
	: CXTPChartCircleDeviceCommand(center, radius)
{
	m_color = color;
}

void CXTPChartSolidCircleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	RectF rect((REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);

	GdipFillEllipse(pDC->GetGraphics(), pGpBrush, (REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GdipDeleteBrush(pGpBrush);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartGradientCircleDeviceCommand

CXTPChartGradientCircleDeviceCommand::CXTPChartGradientCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartLinearGradientMode nMode)
	: CXTPChartCircleDeviceCommand(center, radius)
{
	m_color = color;
	m_color2 = color2;
	m_nMode = nMode;
}

void CXTPChartGradientCircleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	RectF rect((REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GpLineGradient* pGpBrush = NULL;
	if (m_nMode == xtpChartLinearGradientModeCenterHorizontal)
	{
		GdipCreateLineBrushFromRect((RectF*)&rect, m_color.GetValue(), m_color.GetValue(), LinearGradientModeHorizontal, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = {m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(), m_color.GetValue()};
		REAL positions[] ={0, 0.45f, 0.55f, 1};
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else if (m_nMode == xtpChartLinearGradientModeCenterVertical)
	{
		GdipCreateLineBrushFromRect((RectF*)&rect, m_color.GetValue(), m_color.GetValue(), LinearGradientModeVertical, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = {m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(), m_color.GetValue()};
		REAL positions[] ={0, 0.45f, 0.55f, 1};
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else
	{
		GdipCreateLineBrushFromRect((RectF*)&rect, m_color.GetValue(), m_color2.GetValue(), (LinearGradientMode)m_nMode, WrapModeTileFlipXY, &pGpBrush);
	}

	GdipFillEllipse(pDC->GetGraphics(), pGpBrush, (REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartHatchCircleDeviceCommand

CXTPChartHatchCircleDeviceCommand::CXTPChartHatchCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartHatchStyle nStyle)
	: CXTPChartCircleDeviceCommand(center, radius)
{
	m_color = color;
	m_color2 = color2;
	m_nStyle = nStyle;
}

void CXTPChartHatchCircleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	RectF rect((REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GpHatch* pGpBrush = NULL;
	GdipCreateHatchBrush((GpHatchStyle)m_nStyle, m_color.GetValue(), m_color2.GetValue(), &pGpBrush);

	GdipFillEllipse(pDC->GetGraphics(), pGpBrush, (REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBoundedCircleDeviceCommand

CXTPChartBoundedCircleDeviceCommand::CXTPChartBoundedCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color, int thickness)
{
	m_center = center;
	m_radius = radius;
	m_color = color;
	m_thickness = thickness;
}

void CXTPChartBoundedCircleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	if (m_thickness == 0)
		return;

	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_thickness, UnitWorld, &pGpPen);

	GdipDrawEllipse(pDC->GetGraphics(), pGpPen, (REAL)(m_center.X - m_radius), (REAL)(m_center.Y - m_radius), (REAL)(m_radius * 2), (REAL)(m_radius * 2));

	GdipDeletePen(pGpPen);
}
