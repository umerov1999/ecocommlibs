// XTPChartTextPainter.cpp
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
#include <float.h>
#include <math.h>

#include "Common/XTPMarkupRender.h"

#include "../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"

#include "../XTPChartDefines.h"
#include "../XTPChartElement.h"
#include "../XTPChartContent.h"

#include "../Drawing/XTPChartDeviceContext.h"
#include "../Drawing/XTPChartDeviceCommand.h"
#include "../Drawing/XTPChartTextDeviceCommand.h"
#include "../Drawing/XTPChartTransformationDeviceCommand.h"

#include "XTPChartTextPainter.h"
#include "XTPChartMathUtils.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartTextPainterBase

CXTPChartTextPainterBase::CXTPChartTextPainterBase(CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider)
{
	m_strText = text;
	m_pTextProvider = pTextProvider;

	m_rcRoundedBounds.SetRectEmpty();

	m_pMarkupUIElement = NULL;

	CXTPMarkupContext* pMarkupContext = pTextProvider->GetContent()->GetMarkupContext();

	if (pMarkupContext)
	{
		m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, text);
	}

	if (m_pMarkupUIElement)
	{
		m_szTextSize = CXTPChartMarkupElementDeviceCommand::MeasureElement(pDC, m_pMarkupUIElement, pTextProvider->GetFont());
	}
	else
	{
		m_szTextSize = pDC->MeasureString(&text, pTextProvider->GetFont());
	}

	m_nHeight = (int)ceil(m_szTextSize.Height);
	m_nWidth = (int)ceil(m_szTextSize.Width);
}

CXTPChartTextPainterBase::~CXTPChartTextPainterBase()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

CRect CXTPChartTextPainterBase::GetRoundedBounds()
{
	if (m_rcRoundedBounds.IsRectNull())
	{
		m_rcBounds = CalculateBounds();
		m_rcRoundedBounds.SetRect((int)m_rcBounds.X, (int)m_rcBounds.Y, (int)m_rcBounds.GetRight(), (int)m_rcBounds.GetBottom());
	}
	return m_rcRoundedBounds;
}



//////////////////////////////////////////////////////////////////////////
// CXTPChartTextPainter

CXTPChartTextPainter::CXTPChartTextPainter(CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider)
	: CXTPChartTextPainterBase(pDC, text, pTextProvider)
{
	m_ptLocation = CXTPChartPointF(0, 0);
}

void CXTPChartTextPainter::SetLocation(const CXTPChartPointF& location)
{
	m_ptLocation = location;
}

CXTPChartDeviceCommand* CXTPChartTextPainter::CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartColor& color)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartRectF rect(m_ptLocation.X, m_ptLocation.Y, m_szTextSize.Width, m_szTextSize.Height);

	CXTPChartSaveStateDeviceCommand* pStateGraphicsCommand = new CXTPChartSaveStateDeviceCommand();

	CXTPChartDeviceCommand* pTextContainer = pStateGraphicsCommand->AddChildCommand(new CXTPChartTextAntialiasingDeviceCommand(m_pTextProvider->GetAntialiasing()));

	if (m_pMarkupUIElement)
		pTextContainer->AddChildCommand(new CXTPChartMarkupElementDeviceCommand(m_pMarkupUIElement, m_pTextProvider->GetFont(), color, rect));
	else
		pTextContainer->AddChildCommand(new CXTPChartBoundedTextDeviceCommand(m_strText, m_pTextProvider->GetFont(), color, rect));

	return pStateGraphicsCommand;
}

CXTPChartRectF CXTPChartTextPainter::CalculateBounds()
{
	return CXTPChartRectF(m_ptLocation.X, m_ptLocation.Y, m_szTextSize.Width, m_szTextSize.Height);
}


//////////////////////////////////////////////////////////////////////////
//  CXTPChartRotatedTextPainterBase

CXTPChartRotatedTextPainterBase::CXTPChartRotatedTextPainterBase(CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider, CPoint ptBase)
	: CXTPChartTextPainterBase(pDC, text, pTextProvider)
{
	m_ptBasePoint = ptBase;
	m_fAngle = 0;
}

void CXTPChartRotatedTextPainterBase::SetBasePoint(CPoint pt)
{
	m_ptBasePoint = pt;
}

CXTPChartRectF CXTPChartRotatedTextPainterBase::CalculateBounds()
{
	float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;

	float points[4][2];
	CalculatePoints(CalculateRotation(), GetInitialTextRect(), m_fAngle, points);

	for (int i = 0; i < 4; i++)
	{
		minX = min(minX, points[i][0]);
		maxX = max(maxX, points[i][0]);

		minY = min(minY, points[i][1]);
		maxY = max(maxY, points[i][1]);
	}

	return CXTPChartRectF(minX, minY, maxX - minX, maxY - minY);
}

CRect CXTPChartRotatedTextPainterBase::GetInitialTextRect()
{
	return CRect(CalculateLeftTopPoint(), CSize(m_nWidth, m_nHeight));
}

void CXTPChartRotatedTextPainterBase::CalculatePoints(XTPChartTextRotation rotation, CRect rect, float fDegree, float points[4][2])
{
	double fPI = CXTPChartMathUtils::m_dPI;
	double fRadial = fDegree * fPI / 180.0;

	float width = rect.Width() / 2.0f;
	float height = rect.Height() / 2.0f;
	float fsin = (float)sin(fRadial);
	float fcos = (float)cos(fRadial);


	points[0][0] = rect.left + height * fsin + width * (1 - fcos), points[0][1] = rect.top + height * (1 - fcos) - width * fsin;
	points[1][0] = rect.right + height * fsin - width * (1 - fcos), points[1][1] = rect.top + height * (1 - fcos) + width * fsin;
	points[2][0] = rect.right - height * fsin - width * (1 - fcos), points[2][1] = rect.bottom - height * (1 - fcos) + width * fsin;
	points[3][0] = rect.left - height * fsin + width * (1 - fcos), points[3][1] = rect.bottom - height * (1 - fcos) - width * fsin;

	float offset[2];
	switch (rotation)
	{
				case xtpChartTextLeftTop:
					offset[0] = -width * (1 - fcos) - height * fsin;
					offset[1] = width * fsin - height * (1 - fcos);
					break;
				case xtpChartTextCenterTop:
					offset[0] = -height * fsin, -height * (1 - fcos);
					break;
				case xtpChartTextRightTop:
					offset[0] = width * (1 - fcos) - height * fsin, offset[1] = -width * fsin - height * (1 - fcos);
					break;
				case xtpChartTextLeftCenter:
					offset[0] = -width * (1 - fcos), offset[1] = width * fsin;
					break;
				case xtpChartTextRightCenter:
					offset[0] = width * (1 - fcos), offset[1] = -width * fsin;
					break;
				case xtpChartTextLeftBottom:
					offset[0] = -width * (1 - fcos) + height * fsin, offset[1] = width * fsin + height * (1 - fcos);
					break;
				case xtpChartTextCenterBottom:
					offset[0] = height * fsin, offset[1] = height * (1 - fcos);
					break;
				case xtpChartTextRightBottom:
					offset[0] = width * (1 - fcos) + height * fsin, offset[1] = -width * fsin + height * (1 - fcos);
					break;
				default:
					return;
	}

	for (int i = 0; i < 4; i++)
	{
		points[i][0] += offset[0];
		points[i][0] += offset[1];
	}

}

CXTPChartDeviceCommand* CXTPChartRotatedTextPainterBase::CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartColor& color)
{
	UNREFERENCED_PARAMETER(pDC);

	CRect rect = GetInitialTextRect();
	CPoint offset;

	CPoint center(rect.left + rect.Width() / 2, rect.top + rect.Height() / 2);

	switch (CalculateRotation())
	{
				case xtpChartTextLeftTop:
					offset = CPoint(rect.left, rect.top);
					break;
				case xtpChartTextCenterTop:
					offset = CPoint(center.x, rect.top);
					break;
				case xtpChartTextRightTop:
					offset = CPoint(rect.right, rect.top);
					break;
				case xtpChartTextLeftCenter:
					offset = CPoint(rect.left, center.y);
					break;
				case xtpChartTextCenterCenter:
					offset = CPoint(center.x, center.y);
					break;
				case xtpChartTextRightCenter:
					offset = CPoint(rect.right, center.y);
					break;
				case xtpChartTextLeftBottom:
					offset = CPoint(rect.left, rect.bottom);
					break;
				case xtpChartTextCenterBottom:
					offset = CPoint(center.x, rect.bottom);
					break;
				case xtpChartTextRightBottom:
					offset = CPoint(rect.right, rect.bottom);
					break;
				default:
					offset = CPoint(0, 0);
					break;
	}


	rect.OffsetRect(-offset.x, -offset.y);

	CXTPChartSaveStateDeviceCommand* pStateGraphicsCommand = new CXTPChartSaveStateDeviceCommand();

	CXTPChartDeviceCommand* pCommand = pStateGraphicsCommand;

	pCommand = pCommand->AddChildCommand(new CXTPChartTranslateDeviceCommand((float)offset.x, (float)offset.y, 0));
	pCommand = pCommand->AddChildCommand(new CXTPChartRotateDeviceCommand(m_fAngle));

	CXTPChartDeviceCommand* pTextContainer = pCommand->AddChildCommand(new CXTPChartTextAntialiasingDeviceCommand(m_pTextProvider->GetAntialiasing()));

	if (m_pMarkupUIElement)
		pTextContainer->AddChildCommand(new CXTPChartMarkupElementDeviceCommand(m_pMarkupUIElement, m_pTextProvider->GetFont(), color, CXTPChartRectF(rect)));
	else
		pTextContainer->AddChildCommand(new CXTPChartBoundedTextDeviceCommand(m_strText, m_pTextProvider->GetFont(), color, CXTPChartRectF(rect)));

	return pStateGraphicsCommand;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRotatedTextPainterNearLine

CXTPChartRotatedTextPainterNearLine::CXTPChartRotatedTextPainterNearLine(CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider, CPoint ptBase, XTPChartNearTextPosition position, float fAngle)
	: CXTPChartRotatedTextPainterBase(pDC, text, pTextProvider, ptBase)
{
	m_nNearPosition = position;
	m_fAngle = fAngle;
}


CPoint CXTPChartRotatedTextPainterNearLine::CalculateLeftTopPoint()
{
	double fPI = acos(-1.0);
	double fRadial = m_fAngle * fPI / 180.0;
	int halfHeight = m_nHeight / 2;

	switch (m_nNearPosition)
	{
	case xtpChartTextNearBottom:
		if (m_fAngle == 0 || m_fAngle == 180)
			return CPoint(m_ptBasePoint.x - m_nWidth / 2, m_ptBasePoint.y);

		return m_fAngle < 180 ?
			CPoint(m_ptBasePoint.x, m_ptBasePoint.y - (int)(m_nHeight / 2 * sin(fRadial))) :
			CPoint(m_ptBasePoint.x - m_nWidth, m_ptBasePoint.y - (int)(m_nHeight / 2 * sin(fRadial - fPI)));


	case xtpChartTextNearLeft:
		if (m_fAngle == 90 || m_fAngle == 270)
			return CPoint(m_ptBasePoint.x - m_nWidth / 2 - halfHeight, m_ptBasePoint.y - halfHeight);
		return (m_fAngle < 90 || m_fAngle > 270) ?
			CPoint(m_ptBasePoint.x - (int)(m_nWidth + fabs(halfHeight * sin(fRadial))), m_ptBasePoint.y - halfHeight) :
		CPoint(m_ptBasePoint.x - (int)(fabs(halfHeight * cos(fRadial - fPI / 2))), m_ptBasePoint.y - halfHeight);

	case xtpChartTextNearRight:
		if (m_fAngle == 90 || m_fAngle == 270)
			return CPoint(m_ptBasePoint.x - m_nWidth / 2 + halfHeight, m_ptBasePoint.y - halfHeight);
		return (m_fAngle < 90 || m_fAngle > 270) ?
			CPoint(m_ptBasePoint.x + (int)(fabs(halfHeight * sin(fRadial))), m_ptBasePoint.y - halfHeight) :
		CPoint(m_ptBasePoint.x - m_nWidth + (int)(fabs(halfHeight * cos(fRadial - fPI / 2))), m_ptBasePoint.y - halfHeight);


	case xtpChartTextNearTop:
		if (m_fAngle == 0 || m_fAngle == 180)
			return CPoint(m_ptBasePoint.x - m_nWidth / 2, m_ptBasePoint.y - m_nHeight);
		return m_fAngle < 180 ?
			CPoint(m_ptBasePoint.x - m_nWidth, m_ptBasePoint.y - (int)(halfHeight + fabs(halfHeight * cos(fRadial)))) :
		CPoint(m_ptBasePoint.x, m_ptBasePoint.y - (int)(halfHeight + fabs(halfHeight * cos(fRadial - fPI))));

	}

	return CPoint(0, 0);

}

XTPChartTextRotation CXTPChartRotatedTextPainterNearLine::CalculateRotation()
{
	switch (m_nNearPosition)
	{
		case xtpChartTextNearBottom:
			if (m_fAngle == 0 || m_fAngle == 180)
				return xtpChartTextCenterCenter;
			return m_fAngle < 180 ? xtpChartTextLeftCenter : xtpChartTextRightCenter;

		case xtpChartTextNearLeft:
			if (m_fAngle == 90 || m_fAngle == 270)
				return xtpChartTextCenterCenter;
			return (m_fAngle < 90 || m_fAngle > 270) ? xtpChartTextRightCenter : xtpChartTextLeftCenter;

		case xtpChartTextNearRight:
			if (m_fAngle == 90 || m_fAngle == 270)
				return xtpChartTextCenterCenter;
			return (m_fAngle < 90 || m_fAngle > 270) ? xtpChartTextLeftCenter : xtpChartTextRightCenter;

		case xtpChartTextNearTop:
			if (m_fAngle == 0 || m_fAngle == 180)
				return xtpChartTextCenterCenter;
			return m_fAngle < 180 ? xtpChartTextRightCenter : xtpChartTextLeftCenter;

		default:
			return xtpChartTextCenterCenter;
	}
}
