// XTPChartRadarDiagramView.cpp
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
#include <math.h>

#include "../../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include "../../XTPChartPanel.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"
#include "../../XTPChartTitle.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesLabel.h"

#include "../Axis/XTPChartAxis.h"
#include "../Axis/XTPChartAxisView.h"
#include "../Axis/XTPChartRadarAxisView.h"
#include "../Axis/XTPChartRadarAxisXView.h"
#include "../Axis/XTPChartRadarAxisYView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartCircleDeviceCommand.h"
#include "../../Drawing/XTPChartRectangleDeviceCommand.h"
#include "../../Drawing/XTPChartTransformationDeviceCommand.h"
#include "../../Appearance/XTPChartFillStyle.h"
#include "../../Appearance/XTPChartAppearance.h"
#include "../../Appearance/XTPChartLineStyle.h"
#include <Common/XTPMathUtils.h>

#include "XTPChartRadarDiagram.h"
#include "XTPChartRadarDiagramView.h"
#include "XTPChartRadarDiagramSeriesStyle.h"
#include "XTPChartRadarDiagramSeriesView.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarDiagramView

CXTPChartRadarDiagramView::CXTPChartRadarDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent)
	: CXTPChartDiagramView(pDiagram, pParent)
{
	m_pAxisViews = NULL;

	m_rcLabelPadding.SetRectEmpty();
	m_rcBounds.SetRectEmpty();

	m_ptCenter = CXTPPoint2i(0, 0);
	m_nRadius = 0;

	m_pAxisXView = NULL;
	m_pAxisYView = NULL;
}

CXTPChartRadarAxisView* CXTPChartRadarDiagramView::AddAxisView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView, CXTPChartAxis* pAxis)
{
	CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)pAxis->CreateView(pDC, pParentView);

	pAxisView->CreateView(pDC);

	return pAxisView;
}

CXTPChartRadarAxisView* CXTPChartRadarDiagramView::GetAxisView(CXTPChartAxis* pAxis) const
{
	for (int i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)m_pAxisViews->GetAt(i);

		if (pAxisView->GetAxis() == pAxis)
			return pAxisView;
	}

	return NULL;
}

void CXTPChartRadarDiagramView::UpdateRange(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	int i;

	for (i = 0; i < GetSeriesView()->GetCount(); i++)
	{
		CXTPChartRadarDiagramSeriesView* pSeriesView = (CXTPChartRadarDiagramSeriesView*)GetSeriesView()->GetAt(i);

		pSeriesView->BeforeUpdateRange(pDC);
	}

	for (i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)m_pAxisViews->GetAt(i);

		for (int j = 0; j < GetSeriesView()->GetCount(); j++)
		{
			CXTPChartSeriesView* pSeriesView = (CXTPChartSeriesView*)GetSeriesView()->GetAt(j);
			if (!pSeriesView->GetSeries()->IsVisible())
				continue;

			CXTPChartRadarDiagramSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartRadarDiagramSeriesStyle, pSeriesView->GetSeries()->GetStyle());

			ASSERT(pStyle);
			if (!pStyle)
				continue;


			BOOL bAxisX = !pAxisView->GetAxis()->IsValuesAxis();

			pAxisView->m_arrSeries.Add(pSeriesView);

			if (bAxisX)
				((CXTPChartRadarDiagramSeriesView*)pSeriesView)->m_pAxisViewX = pAxisView;
			else
				((CXTPChartRadarDiagramSeriesView*)pSeriesView)->m_pAxisViewY = pAxisView;
		}
	}

	for (i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)m_pAxisViews->GetAt(i);

		pAxisView->GetAxis()->UpdateRange(pDC, pAxisView);

		pAxisView->AddLegendItems();
	}

	for (i = 0; i < GetSeriesView()->GetCount(); i++)
	{
		CXTPChartRadarDiagramSeriesView* pSeriesView = (CXTPChartRadarDiagramSeriesView*)GetSeriesView()->GetAt(i);

		pSeriesView->AfterUpdateRange(pDC);
	}
}

void CXTPChartRadarDiagramView::CheckLabelBounds(const CXTPChartRectF& rcBounds)
{
	if (rcBounds.GetTop() < m_rcBounds.top)
	{
		m_rcLabelPadding.top = max(m_rcLabelPadding.top, m_rcBounds.top - (LONG)rcBounds.GetTop());
	}

	if (rcBounds.GetRight() > m_rcBounds.right)
	{
		m_rcLabelPadding.right = max(m_rcLabelPadding.right, (LONG)rcBounds.GetRight() - m_rcBounds.right);
	}

	if (rcBounds.GetLeft() < m_rcBounds.left)
	{
		m_rcLabelPadding.left = max(m_rcLabelPadding.left, m_rcBounds.left - (LONG)rcBounds.GetLeft());
	}

	if (rcBounds.GetBottom() > m_rcBounds.bottom)
	{
		m_rcLabelPadding.bottom = max(m_rcLabelPadding.bottom, (LONG)rcBounds.GetBottom() - m_rcBounds.bottom);
	}
}

void CXTPChartRadarDiagramView::CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(rcBounds);
	m_pDiagram->GetTitles()->CalculateView(pDC, rcBounds, m_pTitlesView);

	m_rcLabelPadding.SetRectEmpty();

	CRect rcDiagramBounds = rcBounds;

	for (int nUpdate = 0; nUpdate < 2; nUpdate++)
	{
		const int AXIS_GAP = 10;

		rcBounds = rcDiagramBounds;
		rcBounds.DeflateRect(m_rcLabelPadding);

		rcBounds.DeflateRect(AXIS_GAP, AXIS_GAP);

		int nSize = min(rcBounds.Width(), rcBounds.Height());

		m_ptCenter = CXTPPoint2i((rcBounds.right + rcBounds.left) / 2, (rcBounds.top + rcBounds.bottom) / 2);
		m_nRadius = nSize / 2;

		rcBounds.left = m_ptCenter.X - m_nRadius;
		rcBounds.right = rcBounds.left + m_nRadius * 2;

		rcBounds.top = m_ptCenter.Y - m_nRadius;
		rcBounds.bottom = rcBounds.top + m_nRadius * 2;

		m_rcBounds = rcBounds;

		m_rcLabelPadding.SetRectEmpty();

		CRect rcPane(rcBounds);

		int i;

		CXTPChartRadarDiagram* pDiagram = (CXTPChartRadarDiagram*)GetDiagram();

		pDiagram->UpdateLayout(pDC, this, rcBounds);



		for (i = 0; i < m_pAxisViews->GetCount(); i++)
		{
			CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)m_pAxisViews->GetAt(i);

			pAxisView->CalcSize(pDC, rcPane);

			pAxisView->SetBounds(pDC, rcBounds);
		}


		if (m_rcBounds.Width() < 1 || m_rcBounds.Height() < 1)
			break;

		pDiagram->CalculateSeriesLayout(pDC, this);

		for (i = 0; i < m_pLabelsView->GetCount(); i++)
		{
			CXTPChartSeriesLabelView* pLabelView = (CXTPChartSeriesLabelView*)m_pLabelsView->GetAt(i);

			pLabelView->CalculateLayout(pDC);
		}

		if( m_rcLabelPadding.IsRectNull())
			break;
	}
}

void CXTPChartRadarDiagramView::CreateView(CXTPChartDeviceContext* pDC)
{
	CXTPChartDiagramView::CreateView(pDC);

	CXTPChartRadarDiagram* pDiagram = (CXTPChartRadarDiagram*)GetDiagram();

	m_pSeriesView = new CXTPChartElementView(this);

	m_pAxisViews = new CXTPChartElementView(this);

	m_pAxisXView = (CXTPChartRadarAxisXView*)AddAxisView(pDC, m_pAxisViews, pDiagram->GetAxisX());

	m_pAxisYView = (CXTPChartRadarAxisYView*)AddAxisView(pDC, m_pAxisViews, pDiagram->GetAxisY());

	m_pLabelsView = new CXTPChartElementView(this);
}




CXTPChartDeviceCommand* CXTPChartRadarDiagramView::CreatePolygonStripDeviceCommand(CXTPChartDeviceContext* /*pDC*/, double dRadiusFrom, double dRadiusTo,
	const CXTPChartColor& color1, const CXTPChartColor& color2, CXTPChartFillStyle* pFillStyle)
{

	CXTPChartRadarAxisXView* pAxisXView = GetAxisXView();

	CXTPChartPoints arrFillPoints;
	CXTPChartPoints arrClipPoints;

	BOOL bClipPoints = CXTPMathUtils::Compare(dRadiusFrom, 0) != 0;

	for (int i = 0; i < pAxisXView->m_arrTicks.GetSize(); i++)
	{

		double dAngle = pAxisXView->ValueToAngle(pAxisXView->m_arrTicks[i].m_dValue);

		CXTPChartPointF pt(m_ptCenter.X + cos(dAngle) * dRadiusTo, m_ptCenter.Y - sin(dAngle) * dRadiusTo);

		arrFillPoints.Add(pt);

		if (bClipPoints)
		{
			pt = CXTPChartPointF(m_ptCenter.X + cos(dAngle) * dRadiusFrom, m_ptCenter.Y - sin(dAngle) * dRadiusFrom);

			arrClipPoints.Add(pt);

		}
	}

	if (arrClipPoints.GetSize() == 0)
		return pFillStyle->CreateDeviceCommand(arrFillPoints, color1, color2);
	else
	{
		CXTPChartDeviceCommand* pCommands = new CXTPChartClipRegionDeviceCommand(arrClipPoints);

		pCommands->AddChildCommand(pFillStyle->CreateDeviceCommand(arrFillPoints, color1, color2));

		return pCommands;
	}
}

CXTPChartDeviceCommand* CXTPChartRadarDiagramView::CreatePolygonLineDeviceCommand(CXTPChartDeviceContext* /*pDC*/, double dRadius,
	const CXTPChartColor& color, CXTPChartLineStyle* pLineStyle)
{

	CXTPChartRadarAxisXView* pAxisXView = GetAxisXView();

	CXTPChartPoints arrPoints;


	for (int i = 0; i < pAxisXView->m_arrTicks.GetSize(); i++)
	{

		double dAngle = pAxisXView->ValueToAngle(pAxisXView->m_arrTicks[i].m_dValue);

		CXTPChartPointF pt(m_ptCenter.X + cos(dAngle) * dRadius, m_ptCenter.Y - sin(dAngle) * dRadius);

		arrPoints.Add(pt);
	}

	if (arrPoints.GetSize() == 0)
		return NULL;

	CXTPChartPointF pt = arrPoints[0];
	arrPoints.Add(pt);

	return pLineStyle->CreateDeviceCommand(arrPoints, color);
}



CXTPChartDeviceCommand* CXTPChartRadarDiagramView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	if (m_nRadius <= 1)
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartDeviceCommand* pContainer = pCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand(TRUE));

	CXTPChartRadarDiagram* pDiagram = (CXTPChartRadarDiagram*)GetDiagram();

	CXTPChartFillStyle* pFillStyle = pDiagram->GetActualFillStyle();

	if (pDiagram->GetStyle() == xtpChartRadarDiagramCircle)
	{
		pContainer->AddChildCommand(pFillStyle->CreateCircleDeviceCommand(m_ptCenter, m_nRadius,
			pDiagram->GetActualBackgroundColor(), pDiagram->GetActualBackgroundColor2()));

		pContainer->AddChildCommand(new CXTPChartBoundedCircleDeviceCommand(m_ptCenter, m_nRadius,
			pDiagram->GetActualBorderColor(), 1));
	}
	else
	{
		pContainer->AddChildCommand(CreatePolygonStripDeviceCommand(pDC, 0, m_nRadius,
			pDiagram->GetActualBackgroundColor(), pDiagram->GetActualBackgroundColor2(), pDiagram->GetActualFillStyle()));

		CXTPChartLineStyle line(pDiagram);
		pContainer->AddChildCommand(CreatePolygonLineDeviceCommand(pDC, m_nRadius, pDiagram->GetActualBorderColor(), &line));

	}


	CXTPChartElementView* pAxisViews =  m_pAxisViews;
	int i;

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)pAxisViews->GetAt(i);
		pContainer->AddChildCommand(pAxisView->CreateInterlacedDeviceCommand(pDC));
	}

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)pAxisViews->GetAt(i);
		pContainer->AddChildCommand(pAxisView->CreateStripsDeviceCommand(pDC));
	}

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)pAxisViews->GetAt(i);
		pContainer->AddChildCommand(pAxisView->CreateGridLinesDeviceCommand(pDC));
	}

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)pAxisViews->GetAt(i);
		pContainer->AddChildCommand(pAxisView->CreateConstantLinesDeviceCommand(pDC, TRUE));
	}

	pCommand->AddChildCommand(CXTPChartDiagramView::CreateDeviceCommand(pDC));

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartRadarAxisView* pAxisView = (CXTPChartRadarAxisView*)pAxisViews->GetAt(i);
		pContainer->AddChildCommand(pAxisView->CreateConstantLinesDeviceCommand(pDC, FALSE));
	}

	return pCommand;
}


CXTPChartPointF CXTPChartRadarDiagramView::GetScreenPoint(const CXTPChartSeriesView* /*pView*/, double x, double y) const
{
	double dAngle = m_pAxisXView->ValueToAngle(x);
	double dRadius = m_pAxisYView->ValueToRadius(y);

	return CXTPChartPointF(m_ptCenter.X + cos(dAngle) * dRadius, m_ptCenter.Y - sin(dAngle) * dRadius);
}
