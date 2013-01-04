// XTPChartSeriesView.cpp
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

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include "XTPChartElementView.h"
#include "XTPChartLegend.h"
#include "XTPChartSeriesView.h"
#include "XTPChartSeries.h"
#include "XTPChartSeriesPoint.h"
#include "XTPChartSeriesStyle.h"
#include "XTPChartSeriesLabel.h"
#include "XTPChartDiagramView.h"

#include "Utils/XTPChartNumberFormat.h"

#include "Appearance/XTPChartAppearance.h"
#include "Appearance/XTPChartPalette.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesView

CXTPChartSeriesView::CXTPChartSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartElementView(pDiagramView->GetSeriesView())
{
	m_pSeries = pSeries;

	m_pDiagramView = pDiagramView;
	m_pPointsView = NULL;
	m_pLabelsView = NULL;
}

CXTPChartSeriesPointView* CXTPChartSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pPoint);
	UNREFERENCED_PARAMETER(pParentView);

	return NULL;
}

void CXTPChartSeriesView::CreatePointsView(CXTPChartDeviceContext* pDC)
{
	CXTPChartSeriesPointCollection*  pPoints = m_pSeries->GetPoints();

	m_pPointsView = new CXTPChartElementView(this);
	m_pLabelsView = m_pDiagramView->GetLabelsView();

	for (int i = 0; i < pPoints->GetCount(); i++)
	{
		CXTPChartSeriesPoint* pPoint = pPoints->GetAt(i);

		CXTPChartSeriesPointView* pPointView = CreateSeriesPointView(pDC, pPoint, m_pPointsView);

		if (pPointView && m_pSeries->GetStyle()->GetLabel() && m_pSeries->GetStyle()->GetLabel()->IsVisible())
		{
			m_pSeries->GetStyle()->GetLabel()->CreateView(pDC, pPointView, m_pLabelsView);
		}
	}
}

CXTPChartSeriesStyle* CXTPChartSeriesView::GetStyle() const
{
	return m_pSeries->GetStyle();
}

CXTPChartDiagram* CXTPChartSeriesView::GetDiagram() const
{
	return m_pDiagramView->GetDiagram();
}

CXTPChartColor CXTPChartSeriesView::GetColor() const
{

	if (m_pSeries->GetStyle()->GetColor().GetValue() != CXTPChartColor::Empty)
		return m_pSeries->GetStyle()->GetColor();

	return m_pSeries->GetAppearance()->GetPalette()->GetColor(m_pSeries->GetPaletteIndex());
}

CXTPChartColor CXTPChartSeriesView::GetColor2() const
{
	if (m_pSeries->GetStyle()->GetColor().GetValue() != CXTPChartColor::Empty)
		return CXTPChartAppearance::GetLightColor(m_pSeries->GetStyle()->GetColor());

	return m_pSeries->GetAppearance()->GetPalette()->GetColor2(m_pSeries->GetPaletteIndex());
}

void CXTPChartSeriesView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPChartSeriesView::UpdateRange(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}


CXTPChartString CXTPChartSeriesView::GetLegendName() const
{
	CXTPChartString strName = m_pSeries->GetLegendText();
	if (strName.IsEmpty())
		strName = m_pSeries->GetName();

	return strName;
}

CXTPChartDeviceCommand* CXTPChartSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* /*pDC*/, CRect /*rcBounds*/)
{
	return NULL;
}

void CXTPChartSeriesView::AddToLegend(CXTPChartLegendView* pView)
{
	if (!m_pSeries->IsLegendVisible())
		return;

	if (m_pSeries->GetStyle()->IsColorEach())
	{
		for (int i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartSeriesPointView* pPointView = (CXTPChartSeriesPointView*)m_pPointsView->GetAt(i);

			pView->AddItem(pPointView);
		}
	}
	else
	{
		pView->AddItem(this);
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesPointView

CXTPChartSeriesPointView::CXTPChartSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_pPoint = pPoint;

	m_dInternalValue = m_pPoint->GetValue(0);
}

CXTPChartColor CXTPChartSeriesPointView::GetColor() const
{
	CXTPChartSeriesView* pSeriesView = GetSeriesView();

	if (!m_pPoint->GetColor().IsEmpty())
		return m_pPoint->GetColor();

	if (pSeriesView->GetStyle()->IsColorEach())
		return pSeriesView->GetSeries()->GetAppearance()->GetPalette()->GetColor(m_pPoint->GetPaletteIndex());

	return pSeriesView->GetColor();
}



CXTPChartColor CXTPChartSeriesPointView::GetColor2() const
{
	CXTPChartSeriesView* pSeriesView = GetSeriesView();

	if (!m_pPoint->GetColor().IsEmpty())
		return CXTPChartAppearance::GetLightColor(m_pPoint->GetColor());

	if (pSeriesView->GetStyle()->IsColorEach())
		return pSeriesView->GetSeries()->GetAppearance()->GetPalette()->GetColor2(m_pPoint->GetPaletteIndex());

	return pSeriesView->GetColor2();
}


CXTPChartString CXTPChartSeriesPointView::GetLegendName() const
{
	CXTPChartString strText = m_pPoint->GetLegendText();
	if (!strText.IsEmpty())
		return strText;

	double dValue = m_pPoint->GetValue(m_pPoint->GetSeries()->GetStyle()->GetSeriesPointValueIndex());

	return m_pPoint->GetSeries()->GetPointLegendFormat()->FormatPoint(m_pPoint, dValue);
}

CXTPChartDeviceCommand* CXTPChartSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	return GetSeriesView()->CreateLegendDeviceCommand(pDC, rcBounds);
}
