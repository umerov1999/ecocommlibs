// XTPChartStepAreaSeriesStyle.cpp
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
#
#include "../../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesStyle.h"

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisRange.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"
#include <Common/XTPMathUtils.h>

#include "../Point/XTPChartPointSeriesStyle.h"
#include "../Point/XTPChartPointSeriesView.h"
#include "../Point/XTPChartMarker.h"

#include "XTPChartAreaSeriesStyle.h"
#include "XTPChartAreaSeriesView.h"
#include "XTPChartStepAreaSeriesStyle.h"


IMPLEMENT_SERIAL(CXTPChartStepAreaSeriesStyle, CXTPChartAreaSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepAreaSeriesStyle

CXTPChartStepAreaSeriesStyle::CXTPChartStepAreaSeriesStyle()
{
	m_bInvertedStep = FALSE;
}

CXTPChartStepAreaSeriesStyle::~CXTPChartStepAreaSeriesStyle()
{

}

CXTPChartSeriesView* CXTPChartStepAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStepAreaSeriesView(pSeries, pDiagramView);
}

void CXTPChartStepAreaSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartAreaSeriesStyle::DoPropExchange(pPX);

	PX_Bool(pPX, _T("InvertedStep"), m_bInvertedStep, FALSE);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepAreaSeriesView

CXTPChartStepAreaSeriesView::CXTPChartStepAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartAreaSeriesView(pSeries, pDiagramView)
{

}


CXTPChartDeviceCommand* CXTPChartStepAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartStepAreaSeriesStyle* pStyle = (CXTPChartStepAreaSeriesStyle*)GetStyle();

	CXTPChartAxis* pAxisY = GetAxisViewY()->GetAxis();

	CXTPChartColor clrBorder = GetBorderActualColor();

	double dZero = max(0, pAxisY->GetRange()->GetMinValue());

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointSeriesPointView* pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(0);
		CXTPChartPointSeriesPointView* pPointPrev = pPointView;

		CXTPChartPointF pointPrev = pPointView->GetScreenPoint();

		CXTPChartPoints arrPoints;
		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		arrPoints.Add(pointPrev);

		for (int i = 1; i < nCount; i++)
		{
			pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			if (pPointPrev)
			{
				CXTPChartPointF pointMiddle;
				if (!pStyle->GetInvertedStep())
				{
					pointMiddle= GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
						pPointPrev->m_dInternalValue);
				}
				else
				{
					pointMiddle = GetScreenPoint(pPointPrev->GetPoint()->GetInternalArgumentValue(),
						pPointView->m_dInternalValue);

				}

				arrPoints.Add(pointMiddle);
				arrPoints.Add(pointNext);
			}


			pointPrev = pointNext;
			pPointPrev = pPointView;
		}

		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, GetActualColor(), GetActualColor2()));


		arrPoints.RemoveAt(0);
		arrPoints.RemoveAt(arrPoints.GetSize() - 1);

		if (pStyle->GetBorder()->IsVisible())
			pCommand->AddChildCommand(new CXTPChartSolidPolylineDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartStepAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartStepAreaSeriesStyle, "InvertedStep", 200, GetInvertedStep, SetInvertedStep, VT_BOOL)
END_DISPATCH_MAP()


// {4B4BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartStepAreaSeriesStyle =
{ 0x4b4bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartStepAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	INTERFACE_PART(CXTPChartStepAreaSeriesStyle, IID_IChartStepAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStepAreaSeriesStyle, IID_IChartStepAreaSeriesStyle)

// {4B490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartStepAreaSeriesStyle, "Codejock.ChartStepAreaSeriesStyle." _XTP_AXLIB_VERSION,
0x4b490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
