// XTPChartSeriesStyle.cpp
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

#include <Chart/XTPChartDefines.h>
#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartSeriesLabel.h>
#include <Chart/XTPChartErrorBar.h>

#include <Chart/Appearance/XTPChartAppearance.h>



//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesStyle

IMPLEMENT_DYNAMIC(CXTPChartSeriesStyle, CXTPChartElement)


CXTPChartSeriesStyle::CXTPChartSeriesStyle()
	: m_pLabel   (NULL)
	, m_pErrorBar(NULL)
{
	m_clrColor = CXTPChartColor::Empty;

	m_bColorEach = FALSE;


#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartSeriesStyle::~CXTPChartSeriesStyle()
{
	SAFE_RELEASE(m_pLabel);
	SAFE_RELEASE(m_pErrorBar);
}

CXTPChartSeriesLabel* CXTPChartSeriesStyle::SetLabel(CXTPChartSeriesLabel *pLabel)
{
	SAFE_RELEASE(m_pLabel);

	m_pLabel = pLabel;

	if (NULL != m_pLabel)
	{
		m_pLabel->m_pOwner = this;
	}

	return m_pLabel;
}

CXTPChartErrorBar* CXTPChartSeriesStyle::SetErrorBar(CXTPChartErrorBar *pErrorBar)
{
	SAFE_RELEASE(m_pErrorBar);

	m_pErrorBar = pErrorBar;

	if (NULL != m_pErrorBar)
	{
		m_pErrorBar->m_pOwner = this;
	}

	return m_pErrorBar;
}

CXTPChartSeriesStyleAppearance* CXTPChartSeriesStyle::GetStyleAppearance() const
{
	return GetAppearance()->GetSeriesStyleAppearance();
}

BOOL CXTPChartSeriesStyle::IsStyleDiagram(CXTPChartDiagram* /*pDiagram*/) const
{
	return FALSE;
}

void CXTPChartSeriesStyle::CorrectAxisSideMargins(CXTPChartAxis* pAxis, double nMinValue, double nMaxValue, double& nCorrection)
{
	UNREFERENCED_PARAMETER(pAxis);
	UNREFERENCED_PARAMETER(nMinValue);
	UNREFERENCED_PARAMETER(nMaxValue);
	UNREFERENCED_PARAMETER(nCorrection);

}

void CXTPChartSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("Color"), m_clrColor);
	PX_Bool(pPX, _T("ColorEach"), m_bColorEach);

	if (pPX->IsLoading())
	{
		SAFE_RELEASE(m_pLabel);
	}

	PX_Object(pPX, _T("Label"), m_pLabel, RUNTIME_CLASS(CXTPChartSeriesLabel));

	if (pPX->IsLoading())
	{
		m_pLabel->m_pOwner = this;
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartSeriesStyle, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "Label", 1, OleGetLabel, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "ErrorBar", 20, OleGetErrorBar, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "ColorEach", 2, OleGetColorEach, OleSetColorEach, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesStyle, "Color", 3, OleGetColor, OleSetColor, VT_COLOR)
END_DISPATCH_MAP()


// {D31BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartSeriesStyle =
{ 0xd31bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartSeriesStyle, CXTPChartElement)
	INTERFACE_PART(CXTPChartSeriesStyle, IID_IChartSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeriesStyle, IID_IChartSeriesStyle)

void CXTPChartSeriesStyle::OleChartChanged()
{
	OnChartChanged();
}

LPDISPATCH CXTPChartSeriesStyle::OleGetLabel()
{
	return XTPGetDispatch(m_pLabel);
}

LPDISPATCH CXTPChartSeriesStyle::OleGetErrorBar()
{
	return XTPGetDispatch(m_pErrorBar);
}

BOOL CXTPChartSeriesStyle::OleGetColorEach()
{
	return IsColorEach();
}

void CXTPChartSeriesStyle::OleSetColorEach(BOOL bColorEach)
{
	SetColorEach(bColorEach);
}

OLE_COLOR CXTPChartSeriesStyle::OleGetColor()
{
	return m_clrColor.ToOleColor();
}

void CXTPChartSeriesStyle::OleSetColor(OLE_COLOR clr)
{
	SetColor(CXTPChartColor::FromOleColor(clr));
}


#endif
