// XTPChartLineSeriesView.cpp
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

#include <Common/XTPPropExchange.h>
#include <Common/Base/Types/XTPPoint2.h>
#include <Common/Base/Types/XTPPoint3.h>
#include <Common/XTPMathUtils.h>

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesPointView.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartSeriesStyle.h>

#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h>

#include <Chart/Drawing/XTPChartDeviceCommand.h>
#include <Chart/Drawing/XTPChartLineDeviceCommand.h>
#include <Chart/Appearance/XTPChartLineStyle.h>

#include <Chart/Styles/Point/XTPChartPointSeriesStyle.h>
#include <Chart/Styles/Point/XTPChartPointSeriesView.h>
#include <Chart/Styles/Point/XTPChartMarker.h>

#include "XTPChartLineSeriesStyle.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesView

CXTPChartLineSeriesView::CXTPChartLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView, BOOL bSortPoints)
	: CXTPChartDiagram2DSeriesView(pSeries,pDiagramView, bSortPoints)
{

}
CXTPChartSeriesPointView* CXTPChartLineSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartLineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartLineSeriesStyle* pStyle = (CXTPChartLineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointF pointPrev;
		BOOL bValidPrev = FALSE;

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartPointSeriesPointView* pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);

			if (CXTPMathUtils::IsNan(pPointView->m_dInternalValue))
			{
				bValidPrev = FALSE;
				continue;
			}

			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			if (bValidPrev)
			{
				pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateDeviceCommand(pointPrev, pointNext, m_pSeries->GetColor()));
			}

			pointPrev = pointNext;
			bValidPrev = TRUE;
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartLineSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle, m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = 10;

	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.left,ptCenter.Y),
		CXTPPoint3d(rcBounds.right,ptCenter.Y), m_pSeries->GetColor(), m_pSeries->GetLegendItem()->GetActualLineThickness()));
	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(), m_pSeries->GetColor2(), m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}
