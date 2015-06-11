// XTPChartLineSeriesStyle.cpp
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

#include "StdAfx.h"

#include <Common/XTPPropExchange.h>
#include <Common/Base/Types/XTPPoint2.h>
#include <Common/Base/Types/XTPPoint3.h>
#include <Common/XTPMathUtils.h>

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesPointView.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartErrorBar.h>

#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h>

#include <Chart/Drawing/XTPChartDeviceCommand.h>
#include <Chart/Drawing/XTPChartLineDeviceCommand.h>
#include <Chart/Appearance/XTPChartLineStyle.h>

#include <Chart/Styles/Point/XTPChartPointSeriesStyle.h>
#include <Chart/Styles/Point/XTPChartPointSeriesView.h>
#include <Chart/Styles/Point/XTPChartMarker.h>

#include "XTPChartLineSeriesStyle.h"
#include "XTPChartLineSeriesErrorBar.h"

IMPLEMENT_SERIAL(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesStyle

CXTPChartLineSeriesStyle::CXTPChartLineSeriesStyle()
{
	m_pLineStyle = new CXTPChartLineStyle(this);
	m_pLineStyle->SetThickness(2);

	SetErrorBar(new CXTPChartLineSeriesErrorBar());
}

CXTPChartLineSeriesStyle::~CXTPChartLineSeriesStyle()
{
	SAFE_RELEASE(m_pLineStyle);

}

CXTPChartSeriesView* CXTPChartLineSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartLineSeriesView(pSeries, pDiagramView);
}

void CXTPChartLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPointSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection sec(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&sec);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartLineSeriesStyle, "LineStyle", 100, OleGetLineStyle, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {D33BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartLineSeriesStyle =
{ 0xd33bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle)
INTERFACE_PART(CXTPChartLineSeriesStyle, IID_IChartLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartLineSeriesStyle, IID_IChartLineSeriesStyle)

// {14490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartLineSeriesStyle, "Codejock.ChartLineSeriesStyle." _XTP_AXLIB_VERSION,
	0x14490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

LPDISPATCH CXTPChartLineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}


#endif
