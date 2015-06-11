// XTPChartBorder.cpp
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

#include "Common/XTPPropExchange.h"

#include "../Types/XTPChartTypes.h"

#include "../XTPChartDefines.h"
#include "../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>

#include "../Drawing/XTPChartDeviceCommand.h"
#include "../Drawing/XTPChartRectangleDeviceCommand.h"

#include "XTPChartBorder.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartBorder

CXTPChartBorder::CXTPChartBorder(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;

	m_bVisible = TRUE;
	m_clrColor = CXTPChartColor::Empty;
	m_nThickness = 1;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

CXTPChartBorder::~CXTPChartBorder()
{

}

CXTPChartDeviceCommand* CXTPChartBorder::CreateDeviceCommand(const CXTPChartRectF& rcBounds, const CXTPChartColor& color)
{
	if (m_bVisible && m_nThickness > 0)
	{
		return new CXTPChartBoundedRectangleDeviceCommand(rcBounds, color, m_nThickness);
	}

	return NULL;
}

CXTPChartDeviceCommand* CXTPChartBorder::CreateInnerBorderDeviceCommand(const CXTPChartRectF& rcBounds, const CXTPChartColor& color)
{
	if (m_bVisible && m_nThickness > 0)
	{
		return new CXTPChartInnerBorderDeviceCommand(rcBounds, color, m_nThickness);
	}

	return NULL;
}


void CXTPChartBorder::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Int(pPX, _T("Thickness"), m_nThickness, 1);
	PX_Color(pPX, _T("Color"), m_clrColor);
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartBorder, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPChartBorder, "Thickness", 1, GetThickness, SetThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartBorder, "Visible", 2, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartBorder, "Color", 3, OleGetColor, OleSetColor, VT_COLOR)
END_DISPATCH_MAP()


// {901BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartBorder =
{ 0x901bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartBorder, CCmdTarget)
	INTERFACE_PART(CXTPChartBorder, IID_IChartBorder, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartBorder, IID_IChartBorder)

OLE_COLOR CXTPChartBorder::OleGetColor()
{
	return m_clrColor.ToOleColor();
}

void CXTPChartBorder::OleSetColor(OLE_COLOR clr)
{
	SetColor(CXTPChartColor::FromOleColor(clr));
}

#endif
