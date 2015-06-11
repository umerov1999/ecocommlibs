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
#include "XTPChartRectangleDeviceCommand.h"
#include "XTPChartDeviceContext.h"


using namespace Gdiplus;
using namespace Gdiplus::DllExports;

CXTPChartRectangleDeviceCommand::CXTPChartRectangleDeviceCommand(const CXTPChartRectF& rect)
	: m_rect(rect)
{
}

CXTPChartElement* CXTPChartRectangleDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	CXTPChartElement *pElement = NULL;

	if (m_rect.Contains((REAL)point.x, (REAL)point.y))
	{
		pElement = pParent;
	}

	return pElement;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBoundedRectangleDeviceCommand

CXTPChartBoundedRectangleDeviceCommand::CXTPChartBoundedRectangleDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color, int thickness)
	: CXTPChartRectangleDeviceCommand(rect)
	, m_color(color)
	, m_thickness(thickness)
{
}


void CXTPChartBoundedRectangleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpPen* pGpPen = NULL;
	GdipCreatePen1(m_color.GetValue(), (REAL)m_thickness, UnitWorld, &pGpPen);
	GdipDrawRectangle(pDC->GetGraphics(), pGpPen, m_rect.X, m_rect.Y, m_rect.Width, m_rect.Height);
	GdipDeletePen(pGpPen);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSolidRectangleDeviceCommand

CXTPChartSolidRectangleDeviceCommand::CXTPChartSolidRectangleDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color)
	: CXTPChartRectangleDeviceCommand(rect)
	, m_color(color)
{
}

void CXTPChartSolidRectangleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);
	GdipFillRectangle(pDC->GetGraphics(), pGpBrush, m_rect.X, m_rect.Y, m_rect.Width, m_rect.Height);
	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGradientRectangleDeviceCommand

CXTPChartGradientRectangleDeviceCommand::CXTPChartGradientRectangleDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartLinearGradientMode nMode)
	: CXTPChartRectangleDeviceCommand(rect)
	, m_color(color)
	, m_color2(color2)
	, m_nMode(nMode)
{
}

void CXTPChartGradientRectangleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpLineGradient* pGpBrush = NULL;
	if (m_nMode == xtpChartLinearGradientModeCenterHorizontal)
	{
		GdipCreateLineBrushFromRect((RectF*)&m_rect, m_color.GetValue(), m_color.GetValue(), LinearGradientModeHorizontal, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = {m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(), m_color.GetValue()};
		REAL positions[] ={0, 0.45f, 0.55f, 1};
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else if (m_nMode == xtpChartLinearGradientModeCenterVertical)
	{
		GdipCreateLineBrushFromRect((RectF*)&m_rect, m_color.GetValue(), m_color.GetValue(), LinearGradientModeVertical, WrapModeTileFlipXY, &pGpBrush);

		Gdiplus::ARGB blend[] = {m_color.GetValue(), m_color2.GetValue(), m_color2.GetValue(), m_color.GetValue()};
		REAL positions[] ={0, 0.45f, 0.55f, 1};
		GdipSetLinePresetBlend(pGpBrush, blend, positions, 4);
	}
	else
	{
		GdipCreateLineBrushFromRect((RectF*)&m_rect, m_color.GetValue(), m_color2.GetValue(), (LinearGradientMode)m_nMode, WrapModeTileFlipXY, &pGpBrush);
	}

	GdipFillRectangle(pDC->GetGraphics(), pGpBrush, m_rect.X, m_rect.Y, m_rect.Width, m_rect.Height);

	GdipDeleteBrush(pGpBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartHatchRectangleDeviceCommand

CXTPChartHatchRectangleDeviceCommand::CXTPChartHatchRectangleDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartHatchStyle nStyle)
	: CXTPChartRectangleDeviceCommand(rect)
	, m_color(color)
	, m_color2(color2)
	, m_nStyle(nStyle)
{
}

void CXTPChartHatchRectangleDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GpHatch* pGpBrush = NULL;
	GdipCreateHatchBrush((GpHatchStyle)m_nStyle, m_color.GetValue(), m_color2.GetValue(), &pGpBrush);
	GdipFillRectangle(pDC->GetGraphics(), pGpBrush, m_rect.X, m_rect.Y, m_rect.Width, m_rect.Height);
	GdipDeleteBrush(pGpBrush);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartInnnerBorderDeviceCommand

CXTPChartInnerBorderDeviceCommand::CXTPChartInnerBorderDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color, int thickness)
	: CXTPChartBoundedRectangleDeviceCommand(rect, color, thickness)
{
	m_rect.X += thickness / 2.0f - 0.5f;
	m_rect.Y += thickness / 2.0f - 0.5f;

	m_rect.Width -= thickness - 0.5f;
	m_rect.Height -= thickness - 0.5f;
}


#ifdef _XTP_DEMOMODE

//////////////////////////////////////////////////////////////////////////
// CXTPChartWatermarkBackgroundDeviceCommand

CXTPChartWatermarkBackgroundDeviceCommand::CXTPChartWatermarkBackgroundDeviceCommand(const CXTPChartRectF& rect)
	: m_rect(rect)
{

}

void CXTPChartWatermarkBackgroundDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GdipSetTextRenderingHint(pDC->GetGraphics(), TextRenderingHintAntiAlias);

	GpSolidFill* pGpBrush = NULL;
	CXTPChartColor clrText(100, 102, 102, 102);
	GdipCreateSolidFill(clrText.GetValue(), &pGpBrush);

	GpFont* pGpFont = NULL;

	LOGFONT lf = {0};
	lf.lfHeight = LONG(m_rect.Width * 80 / 1000);

	lf.lfWeight = FW_BOLD;
	STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Myraid Pro"));

	GdipCreateFontFromLogfont(pDC->GetHDC(), &lf, &pGpFont);


	GpStringFormat* pGpStringFormat = NULL;
	GdipCreateStringFormat(0, 0, &pGpStringFormat);

	GdipSetStringFormatLineAlign(pGpStringFormat, StringAlignmentCenter);
	GdipSetStringFormatAlign(pGpStringFormat, StringAlignmentCenter);

	GdipDrawString(pDC->GetGraphics(), L"Codejock Chart Pro Trial", -1, pGpFont, (RectF*)&m_rect, pGpStringFormat, pGpBrush);

	GdipDeleteFont(pGpFont);
	GdipDeleteBrush(pGpBrush);
	GdipDeleteStringFormat(pGpStringFormat);

}
#endif
