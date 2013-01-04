// XTPChartSplineAreaSeriesStyle.cpp
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

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisRange.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../Point/XTPChartPointSeriesStyle.h"
#include "../Point/XTPChartPointSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "XTPChartAreaSeriesStyle.h"
#include "XTPChartAreaSeriesView.h"
#include "XTPChartSplineAreaSeriesStyle.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartSplineAreaSeriesStyle::CXTPChartSplineAreaSeriesStyle()
{
}

CXTPChartSplineAreaSeriesStyle::~CXTPChartSplineAreaSeriesStyle()
{

}

CXTPChartSeriesView* CXTPChartSplineAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartSplineAreaSeriesView(pSeries, pDiagramView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineAreaSeriesView

CXTPChartSplineAreaSeriesView::CXTPChartSplineAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartAreaSeriesView(pSeries,pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartSplineAreaSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartSplineAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartSplineAreaSeriesStyle* pStyle = (CXTPChartSplineAreaSeriesStyle*)GetStyle();

	CXTPChartAxis* pAxisY = GetAxisViewY()->GetAxis();

	CXTPChartColor clrBorder = GetBorderActualColor();

	double dZero = max(0, pAxisY->GetRange()->GetMinValue());

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointSeriesPointView* pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(0);

		CXTPChartPoints arrPoints;
		CXTPChartPointF bottomPoint(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		arrPoints.Add(bottomPoint);

		for (int i = 0; i < nCount; i++)
		{
			pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);
			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			arrPoints.Add(pointNext);
		}


		pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(nCount - 1);
		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateSplineDeviceCommand(arrPoints, GetActualColor(), GetActualColor2()));

		arrPoints.RemoveAt(0);
		arrPoints.RemoveAt(arrPoints.GetSize() - 1);
		pCommand->AddChildCommand(new CXTPChartSolidSplineDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle)
END_DISPATCH_MAP()


// {393BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartSplineAreaSeriesStyle =
{ 0x393bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle)
INTERFACE_PART(CXTPChartSplineAreaSeriesStyle, IID_IChartSplineAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSplineAreaSeriesStyle, IID_IChartSplineAreaSeriesStyle)

// {39490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartSplineAreaSeriesStyle, "Codejock.ChartSplineAreaSeriesStyle." _XTP_AXLIB_VERSION,
						0x39490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
