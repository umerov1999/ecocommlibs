// XTPChartScatterLineSeriesStyle.cpp
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

#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesStyle.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../Point/XTPChartPointSeriesStyle.h"

#include "XTPChartLineSeriesStyle.h"
#include "XTPChartScatterLineSeriesStyle.h"



IMPLEMENT_SERIAL(CXTPChartScatterLineSeriesStyle, CXTPChartLineSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesStyle

CXTPChartScatterLineSeriesStyle::CXTPChartScatterLineSeriesStyle()
{
}

CXTPChartScatterLineSeriesStyle::~CXTPChartScatterLineSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartScatterLineSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartLineSeriesView(pSeries, pDiagramView, FALSE);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartScatterLineSeriesStyle, CXTPChartLineSeriesStyle)
END_DISPATCH_MAP()


// {ED9BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartScatterLineSeriesStyle =
{ 0xed9bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartScatterLineSeriesStyle, CXTPChartLineSeriesStyle)
	INTERFACE_PART(CXTPChartScatterLineSeriesStyle, IID_IChartScatterLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartScatterLineSeriesStyle, IID_IChartScatterLineSeriesStyle)

// {ED990E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartScatterLineSeriesStyle, "Codejock.ChartScatterLineSeriesStyle." _XTP_AXLIB_VERSION,
0xed990e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
