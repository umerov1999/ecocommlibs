// XTPChartRangeBarSeriesLabel.cpp
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

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesLabel.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"


#include "../../Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"


#include "../Bar/XTPChartBarSeriesView.h"

#include "XTPChartRangeBarSeriesLabel.h"
#include "XTPChartRangeBarSeriesView.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeBarSeriesLabel
IMPLEMENT_SERIAL(CXTPChartRangeBarSeriesLabel, CXTPChartDiagram2DSeriesLabel, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRangeBarSeriesLabel::CXTPChartRangeBarSeriesLabel()
{
	m_nPosition = xtpChartRangeBarLabelOutside;
	m_nType = xtpChartRangeBarTwoLabels;

	m_nLineLength = 5;
}

CXTPChartRangeBarSeriesLabel::~CXTPChartRangeBarSeriesLabel()
{

}

void CXTPChartRangeBarSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesLabel::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Position"), m_nPosition, xtpChartRangeBarLabelOutside);
	PX_Enum(pPX, _T("Type"), m_nType, xtpChartRangeBarTwoLabels);
}

CXTPChartElementView* CXTPChartRangeBarSeriesLabel::CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRangeBarSeriesLabelView(this, pPointView, pParentView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeBarSeriesLabelView

CXTPChartRangeBarSeriesLabelView::CXTPChartRangeBarSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
	: CXTPChartDiagram2DSeriesLabelView(pLabel, pPointView, pParentView)
{

}

double CXTPChartRangeBarSeriesLabelView::GetAnchorAngle(BOOL bMinValue) const
{
	CXTPChartRangeBarSeriesLabel* pLabel = (CXTPChartRangeBarSeriesLabel*)m_pLabel;

	CXTPChartDiagram2DSeriesView* pView = (CXTPChartDiagram2DSeriesView*)m_pPointView->GetSeriesView();

	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)pView->GetDiagram();

	BOOL bTop = TRUE;
	if (bMinValue)
		bTop = !bTop;

	if (pView->GetAxisViewY()->GetAxis()->IsReversed())
		bTop = !bTop;

	if (pLabel->GetPosition() == xtpChartRangeBarLabelInside)
		bTop = !bTop;

	if (m_pPointView->GetPoint()->GetValue(0) > m_pPointView->GetPoint()->GetValue(1))
		bTop = !bTop;


	if (pDiagram->IsRotated())
	{
		if (!bTop)
			return 180;
		return 0;
	}
	else
	{
		if (!bTop)
			return 270;
		return 90;
	}
}

CXTPChartPointF CXTPChartRangeBarSeriesLabelView::GetAnchorPoint(BOOL bMinValue) const
{
	CXTPChartRangeBarSeriesLabel* pLabel = (CXTPChartRangeBarSeriesLabel*)m_pLabel;

	if (pLabel->GetType() == xtpChartRangeBarOneLabel)
	{
		CXTPChartRectF rc(((CXTPChartRangeBarSeriesPointView*)m_pPointView)->GetScreenRect());
		return rc.GetCenter();
	}
	else
	{
		return ((CXTPChartRangeBarSeriesPointView*)m_pPointView)->GetScreenPoint(bMinValue);
	}
}




void CXTPChartRangeBarSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CXTPChartRangeBarSeriesLabel* pLabel = (CXTPChartRangeBarSeriesLabel*)m_pLabel;

	if (pLabel->GetType() == xtpChartRangeBarOneLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0) + _T(", ") + m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(0), strText);
	}
	else if (pLabel->GetType() == xtpChartRangeBarTwoLabels)
	{

		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(TRUE), GetAnchorAngle(TRUE), strText);

		strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(FALSE), GetAnchorAngle(FALSE), strText);
	}
	else if (pLabel->GetType() == xtpChartRangeBarMinValueLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(TRUE), GetAnchorAngle(TRUE), strText);
	}
	else  if (pLabel->GetType() == xtpChartRangeBarMaxValueLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint(FALSE), GetAnchorAngle(FALSE), strText);
	}
}


CXTPChartDeviceCommand* CXTPChartRangeBarSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartRangeBarSeriesLabel* pLabel = (CXTPChartRangeBarSeriesLabel*)m_pLabel;

	if (pLabel->GetType() == xtpChartRangeBarOneLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0) + _T(", ") + m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(0), strText);
	}

	if (pLabel->GetType() == xtpChartRangeBarTwoLabels)
	{
		CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
		pCommand->AddChildCommand(CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(TRUE), GetAnchorAngle(TRUE), strText));

		strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		pCommand->AddChildCommand(CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(FALSE), GetAnchorAngle(FALSE), strText));

		return pCommand;
	}

	if (pLabel->GetType() == xtpChartRangeBarMinValueLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 0);
		return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(TRUE), GetAnchorAngle(TRUE), strText);
	}

	if (pLabel->GetType() == xtpChartRangeBarMaxValueLabel)
	{
		CString strText = m_pLabel->GetPointLabel(m_pPointView->GetPoint(), 1);
		return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint(FALSE), GetAnchorAngle(FALSE), strText);
	}

	return 0;
}





#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRangeBarSeriesLabel, CXTPChartDiagram2DSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartRangeBarSeriesLabel, "Position", 106, m_nPosition, OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartRangeBarSeriesLabel, "Type", 107, m_nType, OleChartChanged, VT_I4)
END_DISPATCH_MAP()


// {89ABCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRangeBarSeriesLabel =
{ 0x89abcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRangeBarSeriesLabel, CXTPChartDiagram2DSeriesLabel)
	INTERFACE_PART(CXTPChartRangeBarSeriesLabel, IID_IChartRangeBarSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRangeBarSeriesLabel, IID_IChartRangeBarSeriesLabel)


#endif
