// XTPChartRadarLineSeriesStyle.cpp
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

#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesPoint.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include <Common/XTPMathUtils.h>
#include "../../Drawing/XTPChartDeviceCommand.h"

#include "../../Appearance/XTPChartLineStyle.h"

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "XTPChartRadarPointSeriesStyle.h"
#include "XTPChartRadarLineSeriesStyle.h"
#include "XTPChartRadarLineSeriesView.h"

IMPLEMENT_SERIAL(CXTPChartRadarLineSeriesStyle, CXTPChartRadarPointSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarLineSeriesStyle

CXTPChartRadarLineSeriesStyle::CXTPChartRadarLineSeriesStyle()
{
	m_pLineStyle = new CXTPChartLineStyle(this);
	m_pLineStyle->SetThickness(2);
}

CXTPChartRadarLineSeriesStyle::~CXTPChartRadarLineSeriesStyle()
{
	SAFE_RELEASE(m_pLineStyle);

}

void CXTPChartRadarLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartRadarPointSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection sec(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&sec);
}

CXTPChartSeriesView* CXTPChartRadarLineSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarLineSeriesView(pSeries, pDiagramView);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartRadarLineSeriesStyle, CXTPChartRadarPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartRadarLineSeriesStyle, "LineStyle", 100, OleGetLineStyle, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {A54BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRadarLineSeriesStyle =
{ 0xa54bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRadarLineSeriesStyle, CXTPChartRadarPointSeriesStyle)
	INTERFACE_PART(CXTPChartRadarLineSeriesStyle, IID_IChartRadarLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarLineSeriesStyle, IID_IChartRadarLineSeriesStyle)

// {A5490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartRadarLineSeriesStyle, "Codejock.ChartRadarLineSeriesStyle." _XTP_AXLIB_VERSION,
	0xa5490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


LPDISPATCH CXTPChartRadarLineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}

#endif
