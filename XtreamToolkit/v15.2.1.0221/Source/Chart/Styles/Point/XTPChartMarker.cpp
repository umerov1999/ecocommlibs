// XTPChartMarker.cpp
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
#include <math.h>

#include <Common/XTPPropExchange.h>

#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartSeriesStyle.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartCircleDeviceCommand.h"
#include "../../Drawing/XTPChartRectangleDeviceCommand.h"
#include "../../Drawing/XTPChartPolygonDeviceCommand.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "XTPChartMarker.h"


IMPLEMENT_DYNAMIC(CXTPChartMarker, CXTPChartElement)

CXTPChartMarker::CXTPChartMarker(CXTPChartSeriesStyle* pOwner)
{
	m_pOwner = pOwner;

	m_nSize = 8;
	m_bVisible = TRUE;

	m_nType = xtpChartMarkerCircle;
	m_clrColor = CXTPChartColor::Empty;
	m_clrBorderColor = CXTPChartColor::Empty;

	m_bBorderVisible = TRUE;

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillGradient);
	m_pFillStyle->SetGradientAngle(xtpChartGradientAngle315);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartMarker::~CXTPChartMarker()
{
	SAFE_RELEASE(m_pFillStyle);
}


CXTPChartDeviceCommand* CXTPChartMarker::CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartPointF& point, CXTPChartColor color, CXTPChartColor color2, CXTPChartColor colorBorder)
{
	return CreateDeviceCommand(pDC, point, m_nSize, color, color2, colorBorder);
}

CXTPChartDeviceCommand* CXTPChartMarker::CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartPointF& point, int nWidth, CXTPChartColor color, CXTPChartColor color2, CXTPChartColor colorBorder)
{
	if (!m_bVisible)
		return NULL;

	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();

	if (m_nType == xtpChartMarkerCircle)
	{
		pCommand->AddChildCommand(m_pFillStyle->CreateCircleDeviceCommand(point, nWidth / 2, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(new CXTPChartBoundedCircleDeviceCommand(point, nWidth / 2, colorBorder, 1));
		}
	}
	else if (m_nType == xtpChartMarkerSquare)
	{
		CXTPChartPoints arrPoints;

		arrPoints.Add(CXTPChartPointF(point.X - nWidth / 2, point.Y - nWidth / 2));
		arrPoints.Add(CXTPChartPointF(point.X + nWidth / 2, point.Y - nWidth / 2));
		arrPoints.Add(CXTPChartPointF(point.X + nWidth / 2, point.Y + nWidth / 2));
		arrPoints.Add(CXTPChartPointF(point.X - nWidth / 2, point.Y + nWidth / 2));

		pCommand->AddChildCommand(m_pFillStyle->CreateDeviceCommand(arrPoints, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(new CXTPChartBoundedPolygonDeviceCommand(arrPoints, colorBorder, 1));
		}
	}
	else if (m_nType == xtpChartMarkerDiamond)
	{
		CXTPChartPoints arrPoints;

		float d = nWidth / (float)sqrt(2.0) - 1;

		arrPoints.Add(CXTPChartPointF(point.X - d, point.Y));
		arrPoints.Add(CXTPChartPointF(point.X, point.Y - d));
		arrPoints.Add(CXTPChartPointF(point.X + d, point.Y));
		arrPoints.Add(CXTPChartPointF(point.X, point.Y + d));

		pCommand->AddChildCommand(m_pFillStyle->CreateDeviceCommand(arrPoints, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(new CXTPChartBoundedPolygonDeviceCommand(arrPoints, colorBorder, 1));
		}
	}
	else if (m_nType == xtpChartMarkerTriangle || m_nType == xtpChartMarkerPentagon || m_nType == xtpChartMarkerHexagon)
	{
		int nSide = m_nType == xtpChartMarkerTriangle ? 3 : m_nType == xtpChartMarkerPentagon ? 5 : 6;

		CXTPChartPoints arrPoints;
		double d = nWidth / (float)sqrt(2.0) - 1;
		const double PI = acos(-1.0);

		for (int i = 0; i < nSide; i++)
		{
			arrPoints.Add(CXTPChartPointF(point.X + d * cos(PI / 2 + i * 2 * PI / nSide), point.Y - d * sin(PI / 2 + i * 2 * PI / nSide)));
		}

		pCommand->AddChildCommand(m_pFillStyle->CreateDeviceCommand(arrPoints, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(new CXTPChartBoundedPolygonDeviceCommand(arrPoints, colorBorder, 1));
		}

	}
	else if (m_nType == xtpChartMarkerStar)
	{
		int nSide = 5;

		CXTPChartPoints arrPoints;
		double d = nWidth / (float)sqrt(2.0) - 1;
		const double PI = acos(-1.0);

		double angle = 2 * PI / nSide;

		for (int i = 0; i < nSide; i++)
		{
			arrPoints.Add(CXTPChartPointF(point.X + d * cos(PI / 2 + i * angle), point.Y - d * sin(PI / 2 + i * angle)));

			arrPoints.Add(CXTPChartPointF(point.X + d / 2 * cos(PI / 2 + i * angle + angle/ 2), point.Y - d / 2 * sin(PI / 2 + i * angle + angle / 2)));
		}

		pCommand->AddChildCommand(m_pFillStyle->CreateDeviceCommand(arrPoints, color, color2));

		if (m_bBorderVisible)
		{
			pCommand->AddChildCommand(new CXTPChartBoundedPolygonDeviceCommand(arrPoints, colorBorder, 1));
		}

	}
	return pCommand;
}

void CXTPChartMarker::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("BorderVisible"), m_bBorderVisible, TRUE);
	PX_Color(pPX, _T("BorderColor"), m_clrBorderColor);
	PX_Color(pPX, _T("Color"), m_clrColor);
	PX_Enum(pPX, _T("Type"), m_nType, xtpChartMarkerCircle);
	PX_Int(pPX, _T("Size"), m_nSize, 0);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartMarker, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "Size", 1, GetSize, SetSize, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "Type", 2, GetType, SetType, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "FillStyle", 3, OleGetFillStyle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "BorderVisible", 4, GetBorderVisible, SetBorderVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartMarker, "Visible", 5, GetVisible, SetVisible, VT_BOOL)
END_DISPATCH_MAP()


// {456BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartMarker =
{ 0x456bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartMarker, CXTPChartElement)
	INTERFACE_PART(CXTPChartMarker, IID_IChartMarker, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartMarker, IID_IChartMarker)

LPDISPATCH CXTPChartMarker::OleGetFillStyle()
{
	return XTPGetDispatch(m_pFillStyle);
}

#endif
