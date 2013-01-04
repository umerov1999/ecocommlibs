// XTPChartRadarPointSeriesStyle.cpp
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
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include "../../Utils/XTPChartMathUtils.h"
#include "../../Drawing/XTPChartDeviceCommand.h"

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "../Point/XTPChartPointSeriesLabel.h"
#include "XTPChartRadarPointSeriesStyle.h"
#include "XTPChartRadarPointSeriesView.h"
#include "../Point/XTPChartMarker.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesView

CXTPChartRadarPointSeriesView::CXTPChartRadarPointSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartRadarDiagramSeriesView(pSeries,pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartRadarPointSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartRadarPointSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1, 2, 2);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartRadarPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartRadarPointSeriesStyle, m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, GetColor(), GetColor2(), GetColor().GetDarkColor()));

	return pCommand;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesPointView

CXTPChartRadarPointSeriesPointView::CXTPChartRadarPointSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{

}

CXTPChartPointF CXTPChartRadarPointSeriesPointView::GetScreenPoint() const
{
	CXTPChartRadarPointSeriesView* pView = (CXTPChartRadarPointSeriesView*)GetSeriesView();

	return pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_dInternalValue);
}


CXTPChartDeviceCommand* CXTPChartRadarPointSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartColor color = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartColor colorBorder = GetColor().GetDarkColor();

	if (CXTPChartMathUtils::IsNan(m_dInternalValue))
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pPoint);

	CXTPChartPointF point = GetScreenPoint();

	pCommand->AddChildCommand(((CXTPChartRadarPointSeriesStyle*)GetSeriesView()->GetStyle())->GetMarker()->CreateDeviceCommand(pDC, point, color, color2, colorBorder));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartRadarPointSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1, 2, 2);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartRadarPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartRadarPointSeriesStyle, GetSeriesView()->GetSeries()->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, GetColor(), GetColor2(), GetColor().GetDarkColor()));

	return pCommand;

}
