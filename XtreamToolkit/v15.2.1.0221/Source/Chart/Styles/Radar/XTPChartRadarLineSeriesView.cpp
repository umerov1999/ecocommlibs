// XTPChartRadarLineSeriesView.cpp
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
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include "../../Utils/XTPChartMathUtils.h"
#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "../../Appearance/XTPChartLineStyle.h"

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "../Point/XTPChartPointSeriesLabel.h"
#include "XTPChartRadarPointSeriesStyle.h"
#include "XTPChartRadarLineSeriesStyle.h"
#include "XTPChartRadarPointSeriesView.h"
#include "XTPChartRadarLineSeriesView.h"
#include "../Point/XTPChartMarker.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarLineSeriesView

CXTPChartRadarLineSeriesView::CXTPChartRadarLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarDiagramSeriesView(pSeries, pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartRadarLineSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartRadarLineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartRadarLineSeriesStyle* pStyle = (CXTPChartRadarLineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointF pointPrev;
		BOOL bValidPrev = FALSE;

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartRadarPointSeriesPointView* pPointView =  (CXTPChartRadarPointSeriesPointView*)m_pPointsView->GetAt(i);

			if (CXTPChartMathUtils::IsNan(pPointView->m_dInternalValue))
			{
				bValidPrev = FALSE;
				continue;
			}

			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			if (bValidPrev)
			{
				pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateDeviceCommand(pointPrev, pointNext, GetColor()));
			}

			pointPrev = pointNext;
			bValidPrev = TRUE;
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartRadarLineSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartRadarPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartRadarPointSeriesStyle, m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = 10;

	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.left,ptCenter.Y),
		CXTPPoint3d(rcBounds.right,ptCenter.Y), GetColor(), 1));
	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, GetColor(), GetColor2(), GetColor().GetDarkColor()));

	return pCommand;
}
