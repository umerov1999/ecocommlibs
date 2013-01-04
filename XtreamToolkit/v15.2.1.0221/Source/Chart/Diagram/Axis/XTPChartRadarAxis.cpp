// XTPChartAxis.cpp
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

#include "../../XTPChartDefines.h"
#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../Appearance/XTPChartAppearance.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../RadarDiagram/XTPChartRadarDiagram.h"

#include "XTPChartAxis.h"
#include "XTPChartAxisView.h"
#include "XTPChartAxisRange.h"
#include "XTPChartRadarAxis.h"
#include "XTPChartRadarAxisView.h"
#include "XTPChartRadarAxisXView.h"
#include "XTPChartRadarAxisYView.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAxisX

CXTPChartRadarAxisX::CXTPChartRadarAxisX(CXTPChartRadarDiagram* pDiagram)
	: CXTPChartAxis(pDiagram)
{
	m_nMinorCount = 4;

	m_bInterlaced = TRUE;

	m_pRange->SetSideMargins(FALSE);
	m_pRange->SetShowZeroLevel(FALSE);
}


BOOL CXTPChartRadarAxisX::IsVertical() const
{
	return FALSE;
}

BOOL CXTPChartRadarAxisX::IsValuesAxis() const
{
	return FALSE;
}

double CXTPChartRadarAxisX::CalcSideMarginsValue(double nMinValue, double nMaxValue) const
{
	if (m_nScaleType == xtpChartScaleNumerical)
		return (nMaxValue - nMinValue) / 15.0;
	return 0.5;
}


CXTPChartAxisView* CXTPChartRadarAxisX::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartRadarAxisXView* pView = new CXTPChartRadarAxisXView(this, pParentView);

	return pView;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAxisY

CXTPChartRadarAxisY::CXTPChartRadarAxisY(CXTPChartRadarDiagram* pDiagram)
	: CXTPChartAxis(pDiagram)
{
	m_nMinorCount = 2;

	m_pRange->SetShowZeroLevel(TRUE);
}


BOOL CXTPChartRadarAxisY::IsVertical() const
{
	return TRUE;
}

BOOL CXTPChartRadarAxisY::IsValuesAxis() const
{
	return TRUE;
}

double CXTPChartRadarAxisY::CalcSideMarginsValue(double nMinValue, double nMaxValue) const
{
	if (m_nScaleType != xtpChartScaleNumerical)
		return 0.5;

	double delta;
	if (nMinValue > 0 && nMaxValue > 0)
		delta = nMaxValue;
	else if (nMinValue < 0 && nMaxValue < 0)
		delta = -nMinValue;
	else
		delta = nMaxValue - nMinValue;

	return delta / 10.0;
}

CXTPChartAxisView* CXTPChartRadarAxisY::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartRadarAxisYView* pView = new CXTPChartRadarAxisYView(this, pParentView);

	return pView;
}
