// XTPChartRadarDiagramSeriesView.cpp
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


#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"
#include "../../XTPChartTitle.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesLabel.h"

#include "XTPChartRadarDiagram.h"
#include "XTPChartRadarDiagramView.h"
#include "XTPChartRadarDiagramSeriesView.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarDiagramSeriesView

CXTPChartRadarDiagramSeriesView::CXTPChartRadarDiagramSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartSeriesView(pSeries, pDiagramView)
{
	m_pAxisViewX = m_pAxisViewY = NULL;
}

CXTPChartPointF CXTPChartRadarDiagramSeriesView::GetScreenPoint(double x, double y) const
{
	CXTPChartRadarDiagramView* pDiagramView = (CXTPChartRadarDiagramView*)GetDiagramView();

	return pDiagramView->GetScreenPoint(this, x, y);
}

void CXTPChartRadarDiagramSeriesView::BeforeUpdateRange(CXTPChartDeviceContext* /*pDC*/)
{

}
void CXTPChartRadarDiagramSeriesView::AfterUpdateRange(CXTPChartDeviceContext* /*pDC*/)
{

}
