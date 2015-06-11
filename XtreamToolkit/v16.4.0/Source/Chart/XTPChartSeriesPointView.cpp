// XTPChartSeriesPointView.cpp
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

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "XTPChartElementView.h"
#include "XTPChartLegend.h"
#include <Chart/XTPChartLegendView.h>
#include <Chart/XTPChartSeriesPointView.h>
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

	return pSeriesView->m_pSeries->GetColor();
}



CXTPChartColor CXTPChartSeriesPointView::GetColor2() const
{
	CXTPChartSeriesView* pSeriesView = GetSeriesView();

	if (!m_pPoint->GetColor().IsEmpty())
		return CXTPChartAppearance::GetLightColor(m_pPoint->GetColor());

	if (pSeriesView->GetStyle()->IsColorEach())
		return pSeriesView->GetSeries()->GetAppearance()->GetPalette()->GetColor2(m_pPoint->GetPaletteIndex());

	return pSeriesView->m_pSeries->GetColor2();
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
