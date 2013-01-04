// XTPChartRadarAreaSeriesView.cpp
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

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "../Point/XTPChartPointSeriesLabel.h"
#include "XTPChartRadarPointSeriesStyle.h"
#include "XTPChartRadarAreaSeriesStyle.h"
#include "XTPChartRadarPointSeriesView.h"
#include "XTPChartRadarAreaSeriesView.h"
#include "../Point/XTPChartMarker.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAreaSeriesView

CXTPChartRadarAreaSeriesView::CXTPChartRadarAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarDiagramSeriesView(pSeries, pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartRadarAreaSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}


CXTPChartColor CXTPChartRadarAreaSeriesView::GetActualColor() const
{
	CXTPChartColor clr = GetColor();
	return CXTPChartColor((BYTE)((CXTPChartRadarAreaSeriesStyle*)GetStyle())->GetTransparency(), clr.GetR(), clr.GetG(), clr.GetB());
}

CXTPChartColor CXTPChartRadarAreaSeriesView::GetActualColor2() const
{
	CXTPChartColor clr = GetColor2();
	return CXTPChartColor((BYTE)((CXTPChartRadarAreaSeriesStyle*)GetStyle())->GetTransparency(), clr.GetR(), clr.GetG(), clr.GetB());
}


CXTPChartColor CXTPChartRadarAreaSeriesView::GetBorderActualColor() const
{
	CXTPChartRadarAreaSeriesStyle* pStyle = (CXTPChartRadarAreaSeriesStyle*)GetStyle();

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		return pStyle->GetBorder()->GetColor();

	CXTPChartColor clr = GetColor().GetDarkColor();
	return clr;
}


CXTPChartDeviceCommand* CXTPChartRadarAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartRadarAreaSeriesStyle* pStyle = (CXTPChartRadarAreaSeriesStyle*)GetStyle();

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

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, GetActualColor(), GetActualColor2()));

		if (pStyle->GetBorder()->IsVisible())
		{
			CXTPChartColor clrBorder = GetBorderActualColor();

			pCommand->AddChildCommand(new CXTPChartSolidPolylineDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}


CXTPChartDeviceCommand* CXTPChartRadarAreaSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();

	CXTPChartRadarAreaSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartRadarAreaSeriesStyle, m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.top + 4) ;
	int nSize = 8;


	CXTPChartPoints arrPoints;
	arrPoints.Add(CXTPChartPointF(rcBounds.left, rcBounds.bottom + 1));
	arrPoints.Add(CXTPChartPointF(ptCenter.X, float(rcBounds.top + 4)));
	arrPoints.Add(CXTPChartPointF(rcBounds.right, rcBounds.bottom + 1));

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, GetActualColor(), GetActualColor2()));


	CXTPChartColor clrBorder = GetBorderActualColor();
	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(arrPoints[0], arrPoints[1], clrBorder, 1));
	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(arrPoints[1], arrPoints[2], clrBorder, 1));

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, GetColor(), GetColor2(), GetColor().GetDarkColor()));

	return pCommand;
}
