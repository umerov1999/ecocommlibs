// XTPChartSeriesLabel.cpp
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

#include <Common/XTPPropExchange.h>

#include <Chart/Types/XTPChartTypes.h>

#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>

#include <Chart/XTPChartSeriesLabel.h>
#include <Chart/XTPChartSeriesPointView.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesStyle.h>


#include <Common/XTPMathUtils.h>
#include "Utils/XTPChartNumberFormat.h"
#include "Common/Base/Types/XTPPoint3.h"
#include <Chart/Appearance/XTPChartBorder.h>
#include <Chart/Appearance/XTPChartFillStyle.h>
#include <Chart/Appearance/XTPChartAppearance.h>

#include <Chart/Drawing/XTPChartDeviceCommand.h>
#include <Chart/Drawing/XTPChartLineDeviceCommand.h>


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesLabel

IMPLEMENT_SERIAL(CXTPChartSeriesLabel, CXTPChartTextElement, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartSeriesLabel::CXTPChartSeriesLabel()
{
	m_pFont = CXTPChartFont::GetTahoma8();
	m_clrTextColor = CXTPChartColor::Empty;
	m_clrBackColor = CXTPChartColor::Empty;
	m_clrLineColor = CXTPChartColor::Empty;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillSolid);

	m_bAntialiasing = FALSE;
	m_bVisible = TRUE;
	m_bShowLines = TRUE;
	m_nLineLength = 10;

	m_nTransparency = 255;

	m_pFormat = new CXTPChartNumberFormat(this);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartSeriesLabel::~CXTPChartSeriesLabel()
{
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pBorder);
	SAFE_RELEASE(m_pFillStyle);
	SAFE_RELEASE(m_pFormat);
}

void CXTPChartSeriesLabel::SetFont(CXTPChartFont* pFont)
{
	SAFE_RELEASE(m_pFont);
	m_pFont = pFont;
	OnChartChanged();
}

void CXTPChartSeriesLabel::SetFormat(LPCTSTR lpszFormat)
{
	m_pFormat->SetPattern(lpszFormat);
}

CXTPChartString CXTPChartSeriesLabel::GetPointLabel(CXTPChartSeriesPoint* pPoint, int nValueIndex) const
{
	if (!pPoint->GetLabelText().IsEmpty())
		return pPoint->GetLabelText();

	double dValue = pPoint->GetValue(nValueIndex == - 1 ? pPoint->GetSeries()->GetStyle()->GetSeriesPointValueIndex() : nValueIndex);


	return m_pFormat->FormatPoint(pPoint, dValue);
}

CXTPChartColor CXTPChartSeriesLabel::GetActualBackColor() const
{
	CXTPChartColor clrBackColor = m_clrBackColor;

	if (clrBackColor.IsEmpty())
	{
		CXTPChartSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartSeriesStyle, GetOwner());
		ASSERT(pStyle);

		clrBackColor = pStyle->GetStyleAppearance()->GetLabelAppearance()->BackgroundColor;
	}

	if (m_nTransparency != 255)
	{
		return CXTPChartColor((BYTE)m_nTransparency, clrBackColor.GetR(), clrBackColor.GetG(), clrBackColor.GetB());
	}

	return clrBackColor;
}

CXTPChartColor CXTPChartSeriesLabel::GetActualConnectorColor() const
{
	CXTPChartColor clrLineColor = GetLineColor();
#if 0
	if (clrLineColor.IsEmpty())
	{
		CXTPChartSeriesStyle* pStyle = (CXTPChartSeriesStyle*)GetOwner();

		clrLineColor = pStyle->GetStyleAppearance()->GetLabelAppearance()->ConnectorColor;

		if (clrLineColor.IsEmpty())
			clrLineColor = m_pPointView->GetColor().GetDarkColor();
	}

	int nTransparency = GetTransparency();

	if (nTransparency != 255)
	{
		return CXTPChartColor((BYTE)nTransparency, clrLineColor.GetR(), clrLineColor.GetG(), clrLineColor.GetB());
	}
#endif
	return clrLineColor;
}


void CXTPChartSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("Antialiasing"), m_bAntialiasing, FALSE);
	PX_Bool(pPX, _T("ShowLines"), m_bShowLines, TRUE);
	PX_Int(pPX, _T("LineLength"), m_nLineLength, 10);

	CXTPPropExchangeSection secFormat(pPX->GetSection(_T("Format")));
	m_pFormat->DoPropExchange(&secFormat);

	PX_Font(pPX, _T("Font"), m_pFont);

	PX_Color(pPX, _T("BackColor"), m_clrBackColor);
	PX_Color(pPX, _T("TextColor"), m_clrTextColor);
	PX_Color(pPX, _T("LineColor"), m_clrLineColor);

	if (pPX->GetSchema() >= _XTP_SCHEMA_1500)
		PX_Int(pPX, _T("Transparency"), m_nTransparency, 255);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesLabelConnectorPainterBase

CXTPChartSeriesLabelConnectorPainterBase::CXTPChartSeriesLabelConnectorPainterBase(CXTPPoint2f startPoint, CXTPChartPointF finishPoint, CXTPChartRectF bounds)
{
	m_startPoint = startPoint;
	m_finishPoint = finishPoint;
	m_bounds = bounds;
}

enum XTPChartAngleOrientation
{
	xtpChartAngleQuadrant1,
	xtpChartAngleQuadrant2,
	xtpChartAngleQuadrant3,
	xtpChartAngleQuadrant4,
	xtpChartAngle0,
	xtpChartAngle90,
	xtpChartAngle180,
	xtpChartAngle270,
};

static XTPChartAngleOrientation GetAngleOrientation(double angle, double deadAngle)
{
	angle = CXTPMathUtils::NormalizeRadian(angle);
	angle = angle < 0 ? 2 * CXTPMathUtils::m_dPI + angle : angle;
	XTPChartAngleOrientation disposition;
	if (CXTPMathUtils::Compare(angle, 0, deadAngle) == 0)
		disposition = xtpChartAngle0;
	else if (CXTPMathUtils::Compare(angle, CXTPMathUtils::m_dPI / 2.0, deadAngle) == 0)
		disposition = xtpChartAngle90;
	else if (CXTPMathUtils::Compare(angle, CXTPMathUtils::m_dPI, deadAngle) == 0)
		disposition = xtpChartAngle180;
	else if (CXTPMathUtils::Compare(angle, 3 * CXTPMathUtils::m_dPI / 2.0, deadAngle) == 0)
		disposition = xtpChartAngle270;
	else if (angle > 0 && angle < CXTPMathUtils::m_dPI / 2.0)
		disposition = xtpChartAngleQuadrant1;
	else if (angle > CXTPMathUtils::m_dPI / 2.0 && angle < CXTPMathUtils::m_dPI)
		disposition = xtpChartAngleQuadrant2;
	else if (angle > CXTPMathUtils::m_dPI && angle < 3 * CXTPMathUtils::m_dPI / 2.0)
		disposition = xtpChartAngleQuadrant3;
	else
		disposition = xtpChartAngleQuadrant4;
	return disposition;
}


CXTPChartRectF CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(const CXTPPoint3d& point, double angle, const CXTPChartSizeF& textSize, int borderThickness, CXTPChartRectF& innerBounds)
{
	const double DeadAngle = CXTPMathUtils::m_dPI / 180.0 * 10;

	CXTPChartRectF bounds(0, 0, (float)floor(textSize.Width + 2 * borderThickness), (float)floor(textSize.Height + 2 * borderThickness));
	CXTPPoint3d topLeft = point;

	XTPChartAngleOrientation disposition = GetAngleOrientation(angle, DeadAngle);
	switch (disposition)
	{
				case xtpChartAngle0:
					topLeft.Offset(0, -bounds.Height / 2.0);
					break;
				case xtpChartAngle90:
					topLeft.Offset(-bounds.Width / 2.0, -bounds.Height);
					break;
				case xtpChartAngle180:
					topLeft.Offset(-bounds.Width, -bounds.Height / 2.0);
					break;
				case xtpChartAngle270:
					topLeft.Offset(-bounds.Width / 2.0, 0);
					break;
				case xtpChartAngleQuadrant1:
					topLeft.Offset(0, -bounds.Height);
					break;
				case xtpChartAngleQuadrant2:
					topLeft.Offset(-bounds.Width, -bounds.Height);
					break;
				case xtpChartAngleQuadrant3:
					topLeft.Offset(-bounds.Width, 0);
					break;
				case xtpChartAngleQuadrant4:
					break;
	}
	bounds.Offset(topLeft);
	bounds.Round();

	innerBounds = bounds;
	innerBounds.Inflate((float)-borderThickness, (float)-borderThickness);
	return bounds;

}

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesLabelLineConnectorPainter

CXTPChartSeriesLabelLineConnectorPainter::CXTPChartSeriesLabelLineConnectorPainter(CXTPChartPointF startPoint, CXTPChartPointF finishPoint, double angle, CXTPChartRectF bounds)
	: CXTPChartSeriesLabelConnectorPainterBase(startPoint, finishPoint, bounds)
{
	m_angle = angle;
}

CXTPChartDeviceCommand* CXTPChartSeriesLabelLineConnectorPainter::CreateDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartSeriesLabel* pLabel, const CXTPChartColor& color)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pLabel);

	CXTPChartDeviceCommand* pContaier = new CXTPChartPolygonAntialiasingDeviceCommand();
	pContaier->AddChildCommand(new CXTPChartSolidLineDeviceCommand(m_startPoint, m_finishPoint, color, 1));

	return pContaier;
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartSeriesLabel, CXTPChartElement)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartSeriesLabel, "Visible", 1, m_bVisible, OleChartChanged, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "ShowLines", 2, GetShowLines, SetShowLines, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "LineLength", 3, GetLineLength, SetLineLength, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "Antialiasing", 4, GetAntialiasing, SetAntialiasing, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "Format", 5, OleGetFormat, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "Font", DISPID_FONT, OleGetFont, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "Border", 7, OleGetBorder, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "FillStyle", 8, OleGetFillStyle, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "TextColor", 9, OleGetTextColor, OleSetTextColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "BackColor", 10, OleGetBackColor, OleSetBackColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "LineColor", 11, OleGetLineColor, OleSetLineColor, VT_COLOR)

	DISP_PROPERTY_EX_ID(CXTPChartSeriesLabel, "Transparency", 12, GetTransparency, SetTransparency, VT_I4)
END_DISPATCH_MAP()


// {CDDBCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartSeriesLabel =
{ 0xcddbcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartSeriesLabel, CXTPChartElement)
	INTERFACE_PART(CXTPChartSeriesLabel, IID_IChartSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeriesLabel, IID_IChartSeriesLabel)

void CXTPChartSeriesLabel::OleChartChanged()
{
	OnChartChanged();
}

LPDISPATCH CXTPChartSeriesLabel::OleGetFormat()
{
	return XTPGetDispatch(m_pFormat);
}

LPDISPATCH CXTPChartSeriesLabel::OleGetFont()
{
	return XTPGetDispatch(m_pFont);
}

LPDISPATCH CXTPChartSeriesLabel::OleGetBorder()
{
	return XTPGetDispatch(m_pBorder);
}

LPDISPATCH CXTPChartSeriesLabel::OleGetFillStyle()
{
	return XTPGetDispatch(m_pFillStyle);
}

OLE_COLOR CXTPChartSeriesLabel::OleGetTextColor()
{
	return m_clrTextColor.ToOleColor();
}

void CXTPChartSeriesLabel::OleSetTextColor(OLE_COLOR clr)
{
	SetTextColor(CXTPChartColor::FromOleColor(clr));
}

OLE_COLOR CXTPChartSeriesLabel::OleGetBackColor()
{
	return m_clrBackColor.ToOleColor();
}

void CXTPChartSeriesLabel::OleSetBackColor(OLE_COLOR clr)
{
	SetBackColor(CXTPChartColor::FromOleColor(clr));
}

OLE_COLOR CXTPChartSeriesLabel::OleGetLineColor()
{
	return m_clrLineColor.ToOleColor();
}

void CXTPChartSeriesLabel::OleSetLineColor(OLE_COLOR clr)
{
	SetLineColor(CXTPChartColor::FromOleColor(clr));
}

#endif
