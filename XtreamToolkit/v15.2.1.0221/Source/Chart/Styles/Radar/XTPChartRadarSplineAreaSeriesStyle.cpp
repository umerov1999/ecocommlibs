// XTPChartRadarSplineAreaSeriesStyle.cpp
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
#include "Common/Base/Types/XTPPoint3.h"

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartDiagramView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisRange.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "XTPChartRadarPointSeriesStyle.h"
#include "XTPChartRadarPointSeriesView.h"
#include "XTPChartRadarAreaSeriesStyle.h"
#include "XTPChartRadarAreaSeriesView.h"

#include "XTPChartRadarAreaSeriesStyle.h"
#include "XTPChartRadarAreaSeriesView.h"
#include "XTPChartRadarSplineAreaSeriesStyle.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarSplineAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartRadarSplineAreaSeriesStyle, CXTPChartRadarAreaSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRadarSplineAreaSeriesStyle::CXTPChartRadarSplineAreaSeriesStyle()
{
}

CXTPChartRadarSplineAreaSeriesStyle::~CXTPChartRadarSplineAreaSeriesStyle()
{

}

CXTPChartSeriesView* CXTPChartRadarSplineAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarSplineAreaSeriesView(pSeries, pDiagramView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarSplineAreaSeriesView

CXTPChartRadarSplineAreaSeriesView::CXTPChartRadarSplineAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarAreaSeriesView(pSeries,pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartRadarSplineAreaSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartRadarSplineAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartRadarSplineAreaSeriesStyle* pStyle = (CXTPChartRadarSplineAreaSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();

	if (nCount > 1)
	{
		CXTPChartPoints arrPoints;

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartRadarPointSeriesPointView* pPointView =  (CXTPChartRadarPointSeriesPointView*)m_pPointsView->GetAt(i);
			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			arrPoints.Add(pointNext);
		}

		CXTPChartPointF ptFirst = arrPoints[0];
		arrPoints.Add(ptFirst);

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateSplineDeviceCommand(arrPoints, GetActualColor(), GetActualColor2(), 2));

		if (pStyle->GetBorder()->IsVisible())
		{
			CXTPChartColor clrBorder = GetBorderActualColor();

			pCommand->AddChildCommand(new CXTPChartSolidSplineDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartRadarSplineAreaSeriesStyle, CXTPChartRadarAreaSeriesStyle)
END_DISPATCH_MAP()


// {AA5BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRadarSplineAreaSeriesStyle =
{ 0xaa5bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRadarSplineAreaSeriesStyle, CXTPChartRadarAreaSeriesStyle)
	INTERFACE_PART(CXTPChartRadarSplineAreaSeriesStyle, IID_IChartRadarSplineAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarSplineAreaSeriesStyle, IID_IChartRadarSplineAreaSeriesStyle)

// {AA590E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartRadarSplineAreaSeriesStyle, "Codejock.ChartRadarSplineAreaSeriesStyle." _XTP_AXLIB_VERSION,
0xaa590e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
