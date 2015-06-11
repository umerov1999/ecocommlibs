// XTPChartRadarPointSeriesStyle.cpp
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

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "../Point/XTPChartMarker.h"

#include "XTPChartRadarPointSeriesStyle.h"
#include "XTPChartRadarPointSeriesView.h"
#include "XTPChartRadarPointSeriesLabel.h"

IMPLEMENT_SERIAL(CXTPChartRadarPointSeriesStyle, CXTPChartRadarDiagramSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesStyle

CXTPChartRadarPointSeriesStyle::CXTPChartRadarPointSeriesStyle()
{
	m_pMarker = new CXTPChartMarker(this);

	SetLabel(new CXTPChartRadarPointSeriesLabel());
}

CXTPChartRadarPointSeriesStyle::~CXTPChartRadarPointSeriesStyle()
{
	SAFE_RELEASE(m_pMarker);
}

void CXTPChartRadarPointSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartRadarDiagramSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection secMarker(pPX->GetSection(_T("Marker")));
	m_pMarker->DoPropExchange(&secMarker);
}

CXTPChartSeriesView* CXTPChartRadarPointSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarPointSeriesView(pSeries, pDiagramView);
}


#ifdef _XTP_ACTIVEX



BEGIN_DISPATCH_MAP(CXTPChartRadarPointSeriesStyle, CXTPChartSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartRadarPointSeriesStyle, "Marker", 4, OleGetMarker, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {A34BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRadarPointSeriesStyle =
{ 0xa34bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRadarPointSeriesStyle, CXTPChartSeriesStyle)
	INTERFACE_PART(CXTPChartRadarPointSeriesStyle, IID_IChartRadarPointSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarPointSeriesStyle, IID_IChartRadarPointSeriesStyle)

// {A3490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartRadarPointSeriesStyle, "Codejock.ChartRadarPointSeriesStyle." _XTP_AXLIB_VERSION,
	0xa3490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


LPDISPATCH CXTPChartRadarPointSeriesStyle::OleGetMarker()
{
	return XTPGetDispatch(m_pMarker);
}

#endif
