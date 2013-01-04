// XTPChartStackedAreaSeriesStyle.cpp
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

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"


#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../Point/XTPChartPointSeriesStyle.h"
#include "../Point/XTPChartPointSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "XTPChartAreaSeriesStyle.h"
#include "XTPChartAreaSeriesView.h"
#include "XTPChartStackedAreaSeriesStyle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartStackedAreaSeriesStyle, CXTPChartAreaSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartStackedAreaSeriesStyle::CXTPChartStackedAreaSeriesStyle()
{
	m_nTransparency = 135;
	m_dStackHeight = 0;
}

CXTPChartStackedAreaSeriesStyle::~CXTPChartStackedAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartStackedAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStackedAreaSeriesView(pSeries, pDiagramView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedAreaSeriesView

CXTPChartStackedAreaSeriesView::CXTPChartStackedAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartAreaSeriesView(pSeries,pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartStackedAreaSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartStackedAreaSeriesPointView(pPoint, pParentView);
}


void CXTPChartStackedAreaSeriesView::UpdateRange(CXTPChartDeviceContext* pDC)
{
	CXTPChartAreaSeriesView::UpdateRange(pDC);

	CXTPChartDiagramView* pDiagramView = GetDiagramView();


	for (int i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		if (DYNAMIC_DOWNCAST(CXTPChartStackedAreaSeriesStyle, pView->GetSeries()->GetStyle()))
		{
			if (pView != this)
				return;

			break;
		}
	}

	CMap<double, double, double, double> mapValues;

	for (int j = 0; j < pDiagramView->GetSeriesView()->GetCount(); j++)
	{
		CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(j);

		if (!DYNAMIC_DOWNCAST(CXTPChartStackedAreaSeriesStyle, pView->GetSeries()->GetStyle()))
			continue;

		CXTPChartStackedAreaSeriesView* pStackedAreaSeriesView = (CXTPChartStackedAreaSeriesView*)pView;

		for (int i = 0; i < pStackedAreaSeriesView->m_pPointsView->GetCount(); i++)
		{
			CXTPChartStackedAreaSeriesPointView* pPointView = (CXTPChartStackedAreaSeriesPointView*)pStackedAreaSeriesView->m_pPointsView->GetAt(i);

			double dValue = pPointView->GetPoint()->GetValue(0);

			pPointView->m_dValueFrom = mapValues[pPointView->GetPoint()->GetInternalArgumentValue()];
			pPointView->m_dInternalValue = pPointView->m_dValueTo = pPointView->m_dValueFrom + dValue;

			mapValues[pPointView->GetPoint()->GetInternalArgumentValue()] = pPointView->m_dValueTo;
		}
	}


	double dStackHeight = ((CXTPChartStackedAreaSeriesStyle*)GetStyle())->GetStackHeight();
	if (dStackHeight != 0)
	{
		for (int j = 0; j < pDiagramView->GetSeriesView()->GetCount(); j++)
		{
			CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pDiagramView->GetSeriesView()->GetAt(j);

			if (DYNAMIC_DOWNCAST(CXTPChartStackedAreaSeriesStyle, pView->GetSeries()->GetStyle()))
			{
				CXTPChartStackedAreaSeriesView* pStackedAreaSeriesView = (CXTPChartStackedAreaSeriesView*)pView;

				for (int i = 0; i < pStackedAreaSeriesView->m_pPointsView->GetCount(); i++)
				{
					CXTPChartStackedAreaSeriesPointView* pPointView = (CXTPChartStackedAreaSeriesPointView*)pStackedAreaSeriesView->m_pPointsView->GetAt(i);

					double dLimit = mapValues[pPointView->GetPoint()->GetInternalArgumentValue()];

					pPointView->m_dValueFrom = pPointView->m_dValueFrom * dStackHeight / dLimit;
					pPointView->m_dInternalValue = pPointView->m_dValueTo = pPointView->m_dValueTo * dStackHeight / dLimit;
				}
			}
		}

	}
}

CXTPChartDeviceCommand* CXTPChartStackedAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartStackedAreaSeriesStyle* pStyle = (CXTPChartStackedAreaSeriesStyle*)GetStyle();

	CXTPChartColor clrBorder = GetBorderActualColor();


	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartStackedAreaSeriesPointView* pPointView;

		CXTPChartPoints arrPoints;

		int i;

		for (i = 0; i < nCount; i++)
		{
			pPointView =  (CXTPChartStackedAreaSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
				pPointView->m_dValueTo);

			arrPoints.Add(point);
		}

		if (pStyle->GetBorder()->IsVisible())
			pCommand->AddChildCommand(new CXTPChartSolidPolylineDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));

		for (i = nCount - 1; i >= 0; i--)
		{
			pPointView =  (CXTPChartStackedAreaSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
				pPointView->m_dValueFrom);

			arrPoints.Add(point);
		}

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, GetActualColor(), GetActualColor2()));


	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}


CXTPChartStackedAreaSeriesPointView::CXTPChartStackedAreaSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesPointView(pPoint, pParentView)
{

}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartStackedAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartStackedAreaSeriesStyle, "StackHeight", 104, OleGetStackHeight, OleSetStackHeight, VT_R8)
END_DISPATCH_MAP()


// {383BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartStackedAreaSeriesStyle =
{ 0x383bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x98, 0x76 } };

BEGIN_INTERFACE_MAP(CXTPChartStackedAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	INTERFACE_PART(CXTPChartStackedAreaSeriesStyle, IID_IChartStackedAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStackedAreaSeriesStyle, IID_IChartStackedAreaSeriesStyle)

// {38490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartStackedAreaSeriesStyle, "Codejock.ChartStackedAreaSeriesStyle." _XTP_AXLIB_VERSION,
						0x38490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x54, 0x32);

double CXTPChartStackedAreaSeriesStyle::OleGetStackHeight()
{
	return GetStackHeight();
}

void CXTPChartStackedAreaSeriesStyle::OleSetStackHeight(double dStackHeight)
{
	SetStackHeight(dStackHeight);
}


#endif
