// XTPChartBubbleSeriesLabel.cpp
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

#include "../../Types/XTPChartTypes.h"

#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeriesLabel.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"


#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"


#include "XTPChartPointSeriesView.h"
#include "XTPChartBubbleSeriesLabel.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartBubbleSeriesLabel
IMPLEMENT_SERIAL(CXTPChartBubbleSeriesLabel, CXTPChartSeriesLabel, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartBubbleSeriesLabel::CXTPChartBubbleSeriesLabel()
{
}

CXTPChartBubbleSeriesLabel::~CXTPChartBubbleSeriesLabel()
{

}

CXTPChartElementView* CXTPChartBubbleSeriesLabel::CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartBubbleSeriesLabelView(this, pPointView, pParentView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBubbleSeriesLabelView

CXTPChartBubbleSeriesLabelView::CXTPChartBubbleSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
	: CXTPChartDiagram2DSeriesLabelView(pLabel, pPointView, pParentView)
{

}


CXTPChartDeviceCommand* CXTPChartBubbleSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartPointF anchorPoint = ((CXTPChartPointSeriesPointView*)m_pPointView)->GetScreenPoint();

	return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, anchorPoint);
}

void CXTPChartBubbleSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CXTPChartPointF anchorPoint = ((CXTPChartPointSeriesPointView*)m_pPointView)->GetScreenPoint();

	CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, anchorPoint);
}
