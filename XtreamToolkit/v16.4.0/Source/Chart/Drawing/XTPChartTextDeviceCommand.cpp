// XTPChartTextDeviceCommand.cpp
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

#include "GraphicLibrary/GdiPlus/GdiPlus.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPResourceManager.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupDrawingContext.h"

#include "../Types/XTPChartTypes.h"

#include "XTPChartDeviceCommand.h"
#include "XTPChartTextDeviceCommand.h"
#include "XTPChartDeviceContext.h"

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartTextDeviceCommand

CXTPChartTextDeviceCommand::CXTPChartTextDeviceCommand(const CXTPChartString& strText, CXTPChartFont* pFont, const CXTPChartColor& color)
{
	m_strText = strText;
	m_pFont = pFont;
	m_color = color;
}

void CXTPChartTextDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	ExecuteInternal(pDC);
}

void CXTPChartTextDeviceCommand::ExecuteInternal(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartBoundedTextDeviceCommand

CXTPChartBoundedTextDeviceCommand::CXTPChartBoundedTextDeviceCommand(const CXTPChartString& strText, CXTPChartFont* pFont, const CXTPChartColor& color, const CXTPChartRectF& rectangle)
: CXTPChartTextDeviceCommand(strText, pFont, color)
{
	m_rect = rectangle;
}

CXTPChartElement* CXTPChartBoundedTextDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	if (m_rect.Contains((float)point.x, (float)point.y))
		return pParent;

	return NULL;
}

void CXTPChartBoundedTextDeviceCommand::ExecuteInternal(CXTPChartDeviceContext* pDC)
{
	GpSolidFill* pGpBrush = NULL;
	GdipCreateSolidFill(m_color.GetValue(), &pGpBrush);

	GpFont* pGpFont = NULL;
	if (m_pFont)
	{
		GdipCreateFontFromLogfont(pDC->GetHDC(), &m_pFont->m_lf, &pGpFont);
	}
	else
	{
		LOGFONT lf = {0};
		lf.lfHeight = -12;
		lf.lfWeight = FW_NORMAL;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Tahoma"));

		GdipCreateFontFromLogfont(pDC->GetHDC(), &lf, &pGpFont);
	}

	GpStringFormat* pGpStringFormat = NULL;
	GdipCreateStringFormat(0, 0, &pGpStringFormat);

	GdipSetStringFormatLineAlign(pGpStringFormat, StringAlignmentCenter);
	GdipSetStringFormatAlign(pGpStringFormat, StringAlignmentCenter);

	GdipDrawString(pDC->GetGraphics(), XTP_CT2CW(m_strText), -1, pGpFont, (RectF*)&m_rect, pGpStringFormat, pGpBrush);

	GdipDeleteFont(pGpFont);
	GdipDeleteBrush(pGpBrush);
	GdipDeleteStringFormat(pGpStringFormat);

}


//////////////////////////////////////////////////////////////////////////
// CXTPChartTextAntialiasingDeviceCommand

CXTPChartTextAntialiasingDeviceCommand::CXTPChartTextAntialiasingDeviceCommand(BOOL bAntialias /* = TRUE */)
{
	m_bAntiAlias = bAntialias;
	m_nOldTextRenderingHint = 0;
}

void CXTPChartTextAntialiasingDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	GdipGetTextRenderingHint(pDC->GetGraphics(), (TextRenderingHint*)&m_nOldTextRenderingHint);

	GdipSetTextRenderingHint(pDC->GetGraphics(), m_bAntiAlias ? TextRenderingHintAntiAlias : TextRenderingHintSystemDefault);
}

void CXTPChartTextAntialiasingDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	GdipSetTextRenderingHint(pDC->GetGraphics(), (TextRenderingHint)m_nOldTextRenderingHint);

}

//////////////////////////////////////////////////////////////////////////
// CXTPChartMarkupElementDeviceCommand

CXTPChartMarkupElementDeviceCommand::CXTPChartMarkupElementDeviceCommand(CXTPMarkupUIElement* pMarkupUIElement, CXTPChartFont* pFont, const CXTPChartColor& color, const CXTPChartRectF& rectangle)
{
	m_pMarkupUIElement = pMarkupUIElement;

	m_pMarkupUIElement->AddRef();

	m_pFont = pFont;
	m_color = color;
	m_rect = rectangle;
}

CXTPChartMarkupElementDeviceCommand::~CXTPChartMarkupElementDeviceCommand()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPChartMarkupElementDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	if (!m_pMarkupUIElement)
		return;

	CRect rc((int)floor(m_rect.GetLeft()), (int)floor(m_rect.GetTop()), (int)ceil(m_rect.GetRight()), (int)ceil(m_rect.GetBottom()));

	m_pMarkupUIElement->GetMarkupContext()->SetDefaultFont(&m_pFont->m_lf);
	m_pMarkupUIElement->GetMarkupContext()->SetDefaultTextColor(RGB(m_color.GetR(), m_color.GetG(), m_color.GetB()));

	CXTPMarkupDrawingContext dc(m_pMarkupUIElement->GetMarkupContext(), pDC->GetGraphics());
	m_pMarkupUIElement->Arrange(rc);
	m_pMarkupUIElement->Render(&dc);
}

CXTPChartSizeF AFX_CDECL CXTPChartMarkupElementDeviceCommand::MeasureElement(CXTPChartDeviceContext* pDC, CXTPMarkupUIElement* pMarkupUIElement, CXTPChartFont* pFont)
{
	if (!pMarkupUIElement)
		return CXTPChartSizeF(0, 0);

	pMarkupUIElement->GetMarkupContext()->SetDefaultFont(&pFont->m_lf);

	CXTPMarkupDrawingContext dc(pMarkupUIElement->GetMarkupContext(), pDC->GetGraphics());
	pMarkupUIElement->Measure(&dc, CSize(INT_MAX, INT_MAX));

	CSize sz = pMarkupUIElement->GetDesiredSize();

	return CXTPChartSizeF((float)sz.cx, (float)sz.cy);
}

CXTPChartElement* CXTPChartMarkupElementDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	if (m_rect.Contains((float)point.x, (float)point.y))
		return pParent;

	return NULL;
}
