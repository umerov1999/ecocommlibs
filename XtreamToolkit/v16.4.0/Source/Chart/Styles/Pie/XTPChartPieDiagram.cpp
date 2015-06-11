// XTPChartPieDiagram.cpp
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

#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "../../Types/XTPChartPie.h"


#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesLabel.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartPanel.h>
#include <Chart/XTPChartDiagram.h>
#include <Chart/XTPChartDiagramView.h>
#include <Chart/XTPChartSeriesPointView.h>

#include <Chart/Appearance/XTPChartAppearance.h>
#include <Chart/Appearance/XTPChartBorder.h>
#include <Common/XTPMathUtils.h>
#include <Chart/Utils/XTPChartTextPainter.h>
#include <Chart/Drawing/XTPChartDeviceCommand.h>

#include "XTPChartPieDiagram.h"
#include "XTPChartPieDiagramView.h"
#include "XTPChartPieSeriesLabel.h"
#include "XTPChartPieSeriesStyle.h"
#include "XTPChartPieSeriesPointView.h"
#include "XTPChartPieSeriesView.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieDiagram

IMPLEMENT_SERIAL(CXTPChartPieDiagram, CXTPChartDiagram, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPieDiagram::CXTPChartPieDiagram()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPChartDiagramView* CXTPChartPieDiagram::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPieDiagramView(this, pParent);
}

void CXTPChartPieDiagram::CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView)
{
	if (pView->GetCount() == 0)
		return;

	for (int i = 0; i < pView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartPieSeriesView* pSeriesView = (CXTPChartPieSeriesView*)pView->GetSeriesView()->GetAt(i);

		pSeriesView->CalculatePointLayout(pDC, pView->GetBounds());
		pSeriesView->CalculateLabelLayout(pDC);
	}
}







//////////////////////////////////////////////////////////////////////////
// CXTPChartPieDiagramView

CXTPChartPieDiagramView::CXTPChartPieDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent)
	: CXTPChartDiagramView(pDiagram, pParent)
{
	m_pSeriesView = new CXTPChartElementView(this);
	m_pLabelsView = new CXTPChartPieSeriesLabelsView(this);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartPieDiagram, CXTPChartDiagram)
END_DISPATCH_MAP()

// {786BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPieDiagram =
{ 0x786bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPieDiagram, CXTPChartDiagram)
	INTERFACE_PART(CXTPChartPieDiagram, IID_IChartPieDiagram, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPieDiagram, IID_IChartPieDiagram)

// {78690E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartPieDiagram, "Codejock.ChartPieDiagram." _XTP_AXLIB_VERSION,
						0x78690e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

#endif
