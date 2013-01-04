// XTPChartSeriesStyle.cpp
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

#include "Types/XTPChartTypes.h"

#include "XTPChartElement.h"
#include "XTPChartElementView.h"
#include "XTPChartSeriesStyle.h"
#include "XTPChartSeriesLabel.h"

#include "Appearance/XTPChartAppearance.h"



//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesStyle

IMPLEMENT_DYNAMIC(CXTPChartSeriesStyle, CXTPChartElement)


CXTPChartSeriesStyle::CXTPChartSeriesStyle()
{
	m_pLabel = NULL;
	m_clrColor = CXTPChartColor::Empty;

	m_bColorEach = FALSE;


}

CXTPChartSeriesStyle::~CXTPChartSeriesStyle()
{
	SAFE_RELEASE(m_pLabel);
}

CXTPChartSeriesLabel* CXTPChartSeriesStyle::SetLabel(CXTPChartSeriesLabel* pLabel)
{
	SAFE_RELEASE(m_pLabel);

	m_pLabel = pLabel;

	if (m_pLabel)
	{
		m_pLabel->m_pOwner = this;
	}

	return m_pLabel;
}

CXTPChartSeriesStyleAppearance* CXTPChartSeriesStyle::GetStyleAppearance() const
{
	return GetAppearance()->GetSeriesStyleAppearance();
}

BOOL CXTPChartSeriesStyle::IsStyleDiagram(CXTPChartDiagram* /*pDiagram*/) const
{
	return FALSE;
}

void CXTPChartSeriesStyle::CorrectAxisSideMargins(CXTPChartAxis* pAxis, double nMinValue, double nMaxValue, double& nCorrection)
{
	UNREFERENCED_PARAMETER(pAxis);
	UNREFERENCED_PARAMETER(nMinValue);
	UNREFERENCED_PARAMETER(nMaxValue);
	UNREFERENCED_PARAMETER(nCorrection);

}

void CXTPChartSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("Color"), m_clrColor);
	PX_Bool(pPX, _T("ColorEach"), m_bColorEach);

	if (pPX->IsLoading())
	{
		SAFE_RELEASE(m_pLabel);
	}

	PX_Object(pPX, _T("Label"), m_pLabel, RUNTIME_CLASS(CXTPChartSeriesLabel));

	if (pPX->IsLoading())
	{
		m_pLabel->m_pOwner = this;
	}
}

