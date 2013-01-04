// XTPChartRadarDiagram.cpp
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

#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include "../../Appearance/XTPChartAppearance.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../Axis/XTPChartAxis.h"
#include "../Axis/XTPChartRadarAxis.h"
#include "XTPChartRadarDiagram.h"
#include "XTPChartRadarDiagramView.h"


IMPLEMENT_SERIAL(CXTPChartRadarDiagram, CXTPChartDiagram, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRadarDiagram::CXTPChartRadarDiagram()
{
	m_pAxisX = new CXTPChartRadarAxisX(this);

	m_pAxisY = new CXTPChartRadarAxisY(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillEmpty);

	m_nStartAngle = 0;

	m_nStyle = xtpChartRadarDiagramCircle;
}

CXTPChartRadarDiagram::~CXTPChartRadarDiagram()
{
	SAFE_RELEASE(m_pAxisX);
	SAFE_RELEASE(m_pAxisY);
	SAFE_RELEASE(m_pFillStyle);
}

CXTPChartDiagramView* CXTPChartRadarDiagram::CreateView(CXTPChartDeviceContext* /*pDC*/, CXTPChartElementView* pParent)
{
	return new CXTPChartRadarDiagramView(this, pParent);
}

CXTPChartDiagram2DAppearance* CXTPChartRadarDiagram::GetAppearance() const
{
	return CXTPChartElement::GetAppearance()->GetDiagram2DAppearance();
}

void CXTPChartRadarDiagram::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram::DoPropExchange(pPX);

	PX_Color(pPX, _T("BackgroundColor"), m_clrBackgroundColor);
	PX_Color(pPX, _T("BackgroundColor2"), m_clrBackgroundColor2);
	PX_Color(pPX, _T("BorderColor"), m_clrBorderColor);

	PX_Int(pPX, _T("StartAngle"), m_nStartAngle);
	PX_Enum(pPX, _T("Style"), m_nStyle, xtpChartRadarDiagramCircle);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("BackgroundFillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);

	CXTPPropExchangeSection secAxisX(pPX->GetSection(_T("AxisX")));
	m_pAxisX->DoPropExchange(&secAxisX);

	CXTPPropExchangeSection secAxisY(pPX->GetSection(_T("AxisY")));
	m_pAxisY->DoPropExchange(&secAxisY);
}

void CXTPChartRadarDiagram::UpdateLayout(CXTPChartDeviceContext* /*pDC*/, CXTPChartDiagramView* /*pView*/, CRect /*rcBounds*/)
{
}

CXTPChartColor CXTPChartRadarDiagram::GetBackgroundColor() const
{
	return m_clrBackgroundColor;
}

CXTPChartColor CXTPChartRadarDiagram::GetBackgroundColor2() const
{
	return m_clrBackgroundColor2;
}

CXTPChartColor CXTPChartRadarDiagram::GetBorderColor() const
{
	return m_clrBorderColor;
}

CXTPChartColor CXTPChartRadarDiagram::GetActualBackgroundColor() const
{
	if (!m_clrBackgroundColor.IsEmpty())
		return m_clrBackgroundColor;

	return GetAppearance()->BackgroundColor;
}

CXTPChartColor CXTPChartRadarDiagram::GetActualBackgroundColor2() const
{
	if (!m_clrBackgroundColor2.IsEmpty())
		return m_clrBackgroundColor2;

	return GetAppearance()->BackgroundColor2;
}

CXTPChartColor CXTPChartRadarDiagram::GetActualBorderColor() const
{
	if (!m_clrBorderColor.IsEmpty())
		return m_clrBorderColor;

	return GetAppearance()->BorderColor;
}


CXTPChartFillStyle* CXTPChartRadarDiagram::GetActualFillStyle() const
{
	if (m_pFillStyle->GetFillMode() != xtpChartFillEmpty)
		return m_pFillStyle;

	return GetAppearance()->BackgroundFillStyle;
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarDiagram, CXTPChartDiagram)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "AxisX", 1, OleGetAxisX, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "AxisY", 2, OleGetAxisY, SetNotSupported, VT_DISPATCH)


	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "BackgroundColor", 7, OleGetBackgroundColor, OleSetBackgroundColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "BackgroundColor2", 8, OleGetBackgroundColor2, OleSetBackgroundColor2, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "BorderColor", 9, OleGetBorderColor, OleSetBorderColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "BackgroundFillStyle", 10, OleGetBackgroundFillStyle, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "StartAngle", 11, GetStartAngle, SetStartAngle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartRadarDiagram, "Style", 12, GetStyle, SetStyle, VT_I4)
END_DISPATCH_MAP()

// {56ABCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRadarDiagram =
{ 0x56abcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRadarDiagram, CXTPChartDiagram)
	INTERFACE_PART(CXTPChartRadarDiagram, IID_IChartRadarDiagram, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarDiagram, IID_IChartRadarDiagram)

// {56A90E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartRadarDiagram, "Codejock.ChartRadarDiagram." _XTP_AXLIB_VERSION,
0x56a90e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


LPDISPATCH CXTPChartRadarDiagram::OleGetAxisX()
{
	return XTPGetDispatch(GetAxisX());
}

LPDISPATCH CXTPChartRadarDiagram::OleGetAxisY()
{
	return XTPGetDispatch(GetAxisY());
}

OLE_COLOR CXTPChartRadarDiagram::OleGetBackgroundColor()
{
	return GetBackgroundColor().ToOleColor();
}

void CXTPChartRadarDiagram::OleSetBackgroundColor(OLE_COLOR clr)
{
	SetBackgroundColor(CXTPChartColor::FromOleColor(clr));
}

OLE_COLOR CXTPChartRadarDiagram::OleGetBackgroundColor2()
{
	return GetBackgroundColor2().ToOleColor();
}

void CXTPChartRadarDiagram::OleSetBackgroundColor2(OLE_COLOR clr)
{
	SetBackgroundColor2(CXTPChartColor::FromOleColor(clr));
}

LPDISPATCH CXTPChartRadarDiagram::OleGetBackgroundFillStyle()
{
	return XTPGetDispatch(GetFillStyle());
}


OLE_COLOR CXTPChartRadarDiagram::OleGetBorderColor()
{
	return GetBorderColor().ToOleColor();
}

void CXTPChartRadarDiagram::OleSetBorderColor(OLE_COLOR clr)
{
	SetBorderColor(CXTPChartColor::FromOleColor(clr));
}


#endif
