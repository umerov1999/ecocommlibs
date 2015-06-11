// XTPChartBarSeriesErrorBar.cpp
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
#include <math.h>

#include <Common/XTPPropExchange.h>
#include <Common/XTPMathUtils.h>

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeriesLabel.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartSeriesPointView.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartPanel.h>
#include <Chart/XTPChartDiagram.h>
#include <Chart/Drawing/XTPChartDeviceCommand.h>
#include <Chart/Appearance/XTPChartLineStyle.h>

#include <Chart/XTPChartErrorBar.h>
#include <Chart/Diagram/Axis/XTPChartAxis.h>
#include <Chart/Diagram/Axis/XTPChartAxisView.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2D.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h>
#include <Chart/Styles/Bar/XTPChartBarSeriesErrorBar.h>
#include <Chart/Styles/Bar/XTPChartBarSeriesView.h>


//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesErrorBar
IMPLEMENT_SERIAL(CXTPChartBarSeriesErrorBar, CXTPChartErrorBar, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartBarSeriesErrorBar::CXTPChartBarSeriesErrorBar()
{
}

CXTPChartBarSeriesErrorBar::~CXTPChartBarSeriesErrorBar()
{
}

void CXTPChartBarSeriesErrorBar::DoPropExchange(CXTPPropExchange *pPX)
{
	UNREFERENCED_PARAMETER(pPX);
}

CXTPChartElementView* CXTPChartBarSeriesErrorBar::CreateView(CXTPChartDeviceContext *pDC, CXTPChartSeriesPointView *pPointView, CXTPChartElementView *pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartBarSeriesErrorBarView(this, pPointView, pParentView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesErrorBarView

CXTPChartBarSeriesErrorBarView::CXTPChartBarSeriesErrorBarView(CXTPChartErrorBar *pErrorBar, CXTPChartSeriesPointView *pPointView, CXTPChartElementView *pParentView)
	: CXTPChartErrorBarView(pErrorBar, pPointView, pParentView)
{
}

void CXTPChartBarSeriesErrorBarView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

#ifdef _XTP_ACTIVEX

// {7F1F3222-16AE-488a-8C98-C0E7286A78F0}
static const GUID IID_IChartBarSeriesErrorBar =
{ 0x7f1f3222, 0x16ae, 0x488a, { 0x8c, 0x98, 0xc0, 0xe7, 0x28, 0x6a, 0x78, 0xf0 } };

BEGIN_DISPATCH_MAP(CXTPChartBarSeriesErrorBar, CXTPChartErrorBar)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartBarSeriesErrorBar, CXTPChartErrorBar)
	INTERFACE_PART(CXTPChartBarSeriesErrorBar, IID_IChartBarSeriesErrorBar, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartBarSeriesErrorBar, IID_IChartBarSeriesErrorBar)

#endif
