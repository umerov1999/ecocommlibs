// XTPChartRadarSplineSeriesStyle.cpp
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
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Appearance/XTPChartLineStyle.h"
#include "../../Utils/XTPChartMathUtils.h"

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "XTPChartRadarPointSeriesStyle.h"
#include "XTPChartRadarPointSeriesView.h"
#include "XTPChartRadarLineSeriesStyle.h"
#include "XTPChartRadarLineSeriesView.h"

#include "XTPChartRadarSplineSeriesStyle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarSplineSeriesStyle

IMPLEMENT_SERIAL(CXTPChartRadarSplineSeriesStyle, CXTPChartRadarLineSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartRadarSplineSeriesStyle::CXTPChartRadarSplineSeriesStyle()
{
}

CXTPChartRadarSplineSeriesStyle::~CXTPChartRadarSplineSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartRadarSplineSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarSplineSeriesView(pSeries, pDiagramView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarSplineSeriesView

CXTPChartRadarSplineSeriesView::CXTPChartRadarSplineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarLineSeriesView(pSeries,pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartRadarSplineSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartRadarSplineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartRadarSplineSeriesStyle* pStyle = (CXTPChartRadarSplineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();

	CXTPChartPoints points;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartRadarPointSeriesPointView* pPointView =  (CXTPChartRadarPointSeriesPointView*)m_pPointsView->GetAt(i);
		if (CXTPChartMathUtils::IsNan(pPointView->m_dInternalValue))
		{
			if (points.GetSize() > 1)
			{
				pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateSplineDeviceCommand(points, GetColor()));
			}
			points.RemoveAll();
			continue;
		}

		CXTPChartPointF pointNext = pPointView->GetScreenPoint();
		points.Add(pointNext);
	}

	if (points.GetSize() > 1)
	{
		pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateSplineDeviceCommand(points, GetColor()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartRadarSplineSeriesStyle, CXTPChartRadarLineSeriesStyle)
END_DISPATCH_MAP()


// {AB7BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRadarSplineSeriesStyle =
{ 0xab7bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRadarSplineSeriesStyle, CXTPChartRadarLineSeriesStyle)
	INTERFACE_PART(CXTPChartRadarSplineSeriesStyle, IID_IChartRadarSplineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarSplineSeriesStyle, IID_IChartRadarSplineSeriesStyle)

// {AB790E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartRadarSplineSeriesStyle, "Codejock.ChartRadarSplineSeriesStyle." _XTP_AXLIB_VERSION,
0xab790e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
