// XTPChartRangeAreaSeriesLabel.cpp
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
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"


#include "../../Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"

#include "../Point/XTPChartPointSeriesLabel.h"
#include "../Point/XTPChartPointSeriesLabelView.h"

#include "XTPChartRangeAreaSeriesLabel.h"
#include "XTPChartAreaSeriesView.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeAreaSeriesLabel
IMPLEMENT_SERIAL(CXTPChartRangeAreaSeriesLabel, CXTPChartPointSeriesLabel, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRangeAreaSeriesLabel::CXTPChartRangeAreaSeriesLabel()
{
}

CXTPChartRangeAreaSeriesLabel::~CXTPChartRangeAreaSeriesLabel()
{

}

void CXTPChartRangeAreaSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPointSeriesLabel::DoPropExchange(pPX);
}

CXTPChartElementView* CXTPChartRangeAreaSeriesLabel::CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRangeAreaSeriesLabelView(this, pPointView, pParentView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeAreaSeriesLabelView

CXTPChartRangeAreaSeriesLabelView::CXTPChartRangeAreaSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesLabelView(pLabel, pPointView, pParentView)
{

}


CXTPChartPointF CXTPChartRangeAreaSeriesLabelView::GetAnchorPoint(BOOL bMinValue) const
{
	CXTPChartAreaSeriesView* pView = (CXTPChartAreaSeriesView*)m_pPointView->GetSeriesView();
	CXTPChartSeriesPoint* pPoint = m_pPointView->GetPoint();

	return pView->GetScreenPoint(pPoint->GetInternalArgumentValue(), pPoint->GetValue(bMinValue ? 0 : 1));
}


void CXTPChartRangeAreaSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
	CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(TRUE), GetAnchorAngle(), strText);

	strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
	CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(FALSE), GetAnchorAngle(), strText);
}


CXTPChartDeviceCommand* CXTPChartRangeAreaSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
	pCommand->AddChildCommand(CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(TRUE), GetAnchorAngle(), strText));

	strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
	pCommand->AddChildCommand(CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(FALSE), GetAnchorAngle(), strText));

	return pCommand;
}
