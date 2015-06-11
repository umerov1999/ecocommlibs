// XTPChartAxisRange.cpp
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
#include <math.h>

#include <Common/XTPPropExchange.h>
#include <Common/XTPMathUtils.h>

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartSeriesPointView.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartErrorBar.h>

#include "XTPChartAxisRange.h"
#include "XTPChartAxis.h"
#include "XTPChartAxisView.h"

IMPLEMENT_DYNAMIC(CXTPChartAxisRange, CXTPChartElement)

CXTPChartAxisRange::CXTPChartAxisRange(CXTPChartAxis* pAxis)
{
	ASSERT(pAxis);

	m_pAxis = pAxis;
	m_pOwner = pAxis;


	m_nMinValue = 0;
	m_nMaxValue = 1;

	m_nViewMinValue = 0;
	m_nViewMaxValue = 1;

	m_bSideMargins = TRUE;
	m_bShowZeroLevel = FALSE;

	m_bAutoRange = TRUE;
	m_bViewAutoRange = TRUE;

	m_dZoomLimit = 0.1;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisRange::~CXTPChartAxisRange()
{

}

void CXTPChartAxisRange::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("AutoRange"), m_bAutoRange, TRUE);
	PX_Bool(pPX, _T("ViewAutoRange"), m_bViewAutoRange, TRUE);

	PX_Bool(pPX, _T("SideMargins"), m_bSideMargins, TRUE);
	PX_Bool(pPX, _T("ShowZeroLevel"), m_bShowZeroLevel, FALSE);

	PX_Double(pPX, _T("MinValue"), m_nMinValue, 0);
	PX_Double(pPX, _T("MaxValue"), m_nMaxValue, 1);

	PX_Double(pPX, _T("ViewMinValue"), m_nViewMinValue, 0);
	PX_Double(pPX, _T("ViewMaxValue"), m_nViewMaxValue, 1);

	PX_Double(pPX, _T("ZoomLimit"), m_dZoomLimit, 0.1);
}

void CXTPChartAxisRange::SetSideMargins(BOOL bSideMargins)
{
	m_bSideMargins = bSideMargins;
	OnChartChanged();
}

void CXTPChartAxisRange::SetShowZeroLevel(BOOL bShowZeroLevel)
{
	m_bShowZeroLevel = bShowZeroLevel;
	OnChartChanged();
}

void CXTPChartAxisRange::SetAutoRange(BOOL bAutRange)
{
	m_bAutoRange = bAutRange;
	OnChartChanged();
}

void CXTPChartAxisRange::SetViewAutoRange(BOOL bAutRange)
{
	if (m_bViewAutoRange != bAutRange)
	{
		m_bViewAutoRange = bAutRange;

		if (m_bViewAutoRange)
		{
			m_nViewMinValue = m_nMinValue;
			m_nViewMaxValue = m_nMaxValue;
		}
		m_pAxis->OnChartChanged(xtpChartUpdateLayout);
	}


}

void CXTPChartAxisRange::SetMaxValue(double dMaxValue)
{
	m_nMaxValue = dMaxValue;

	if (m_bViewAutoRange)
		m_nViewMaxValue = dMaxValue;

	if (m_pAxis) m_pAxis->OnChartChanged(xtpChartUpdateLayout);
}

void CXTPChartAxisRange::SetMinValue(double dMinValue)
{
	m_nMinValue = dMinValue;

	if (m_bViewAutoRange)
		m_nViewMinValue = dMinValue;

	if (m_pAxis) m_pAxis->OnChartChanged(xtpChartUpdateLayout);
}


void CXTPChartAxisRange::SetViewMaxValue(double dMaxValue)
{
	m_nViewMaxValue = dMaxValue;
	if (m_pAxis) m_pAxis->OnChartChanged(xtpChartUpdateLayout);
}

void CXTPChartAxisRange::SetViewMinValue(double dMinValue)
{
	m_nViewMinValue = dMinValue;
	if (m_pAxis) m_pAxis->OnChartChanged(xtpChartUpdateLayout);
}


void CXTPChartAxisRange::UpdateRange(CXTPChartAxisView* pView)
{
	if (!m_bAutoRange)
		return;

	BOOL bFirst = TRUE;

	double dMinValue = 0;
	double dMaxValue = 1;

	for (int i = 0; i < pView->m_arrSeries.GetSize(); i++)
	{
		CXTPChartSeriesView *pSeriesView = pView->m_arrSeries[i];
		CXTPChartSeries     *pSeries     = pSeriesView->GetSeries();

		if (NULL == pSeries)
			continue;

		for (int j = 0; j < pSeriesView->GetPointsView()->GetCount(); j++)
		{
			CXTPChartSeriesPointView *pPointView = (CXTPChartSeriesPointView*)pSeriesView->GetPointsView()->GetAt(j);
			CXTPChartErrorBarView *pErrorBarView = reinterpret_cast<CXTPChartErrorBarView*>(pSeriesView->GetErrorBarsView()->GetAt(j));

			CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

			double dValue = m_pAxis->IsValuesAxis() ? pPointView->GetInternalValue() :
				pPoint->GetInternalArgumentValue();

			if (CXTPMathUtils::IsNan(dValue))
				continue;

			double dCenter;
			double dPlus  = dValue;
			double dMinus = dValue;

			if (NULL != pErrorBarView && m_pAxis->IsValuesAxis())
			{
				pErrorBarView->GetValues(dCenter, dPlus, dMinus);
			}

			if (bFirst)
			{
				dMaxValue = dValue;
				dMinValue = dValue;

				dMaxValue = max(dMaxValue, dPlus);
				dMinValue = min(dMinValue, dMinus);
				bFirst = FALSE;
			}
			else
			{
				dMaxValue = max(dMaxValue, dValue);
				dMinValue = min(dMinValue, dValue);

				dMaxValue = max(dMaxValue, dPlus);
				dMinValue = min(dMinValue, dMinus);
			}

			if (m_pAxis->IsValuesAxis() && pPoint->GetValueLength() == 4) // Stock
			{
				double nValue = pPoint->GetValue(1);

				dMaxValue = max(dMaxValue, nValue);
				dMinValue = min(dMinValue, nValue);
			}

			if (m_pAxis->IsValuesAxis())
			{
				pPointView->UpdateMinMaxRange(dMinValue, dMaxValue);
			}
		}
	}

	m_nMinValue = dMinValue;
	m_nMaxValue = dMaxValue;

	Correct(pView);

	if (m_bViewAutoRange)
	{
		m_nViewMinValue = m_nMinValue;
		m_nViewMaxValue = m_nMaxValue;
	}
}

void CXTPChartAxisRange::Correct(CXTPChartAxisView* pView)
{
	BOOL bLogarithmic = m_pAxis->IsLogarithmic();

	double dZero = bLogarithmic ? 1 : 0;

	if (m_bSideMargins)
	{
		double nCorrection = m_pAxis->CalcSideMarginsValue(m_nMinValue, m_nMaxValue);


		CArray<CXTPChartSeriesView*, CXTPChartSeriesView*>& arrSeries = pView->m_arrSeries;

		for (int i = 0; i < arrSeries.GetSize(); i++)
		{
			CXTPChartSeriesView* pSeriesView = arrSeries.GetAt(i);
			pSeriesView->GetSeries()->GetStyle()->CorrectAxisSideMargins(m_pAxis, m_nMinValue, m_nMaxValue, nCorrection);
		}


		if (m_nMinValue > dZero - CXTPMathUtils::m_dEPS && m_bShowZeroLevel)
			m_nMinValue = dZero;
		else
		{
			if (!bLogarithmic || m_nMinValue - nCorrection > CXTPMathUtils::m_dEPS)
				m_nMinValue -= nCorrection;
		}

		m_nMaxValue += nCorrection;
	}

	if (m_bShowZeroLevel)
	{
		if (m_nMinValue > dZero)
			m_nMinValue = dZero;

		if (m_nMaxValue < dZero)
			m_nMaxValue = dZero;
	}

	if (fabs(m_nMinValue - m_nMaxValue) < CXTPMathUtils::m_dEPS)
	{
		double nValue = max(0.5, fabs(m_nMinValue) / 10);
		m_nMaxValue += nValue;
		m_nMinValue -= nValue;
	}
}


#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisRange

BEGIN_DISPATCH_MAP(CXTPChartAxisRange, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisRange, "ShowZeroLevel", 1, OleGetShowZeroLevel, OleSetShowZeroLevel, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisRange, "SideMargins", 2, OleGetSideMargins, OleSetSideMargins, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartAxisRange, "MinValue", 3, OleGetMinValue, OleSetMinValue, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartAxisRange, "MaxValue", 4, OleGetMaxValue, OleSetMaxValue, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartAxisRange, "AutoRange", 5, OleGetAutoRange, OleSetAutoRange, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartAxisRange, "ViewMinValue", 6, GetViewMinValue, SetViewMinValue, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartAxisRange, "ViewMaxValue", 7, GetViewMaxValue, SetViewMaxValue, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartAxisRange, "ViewAutoRange", 8, IsViewAutoRange, SetViewAutoRange, VT_BOOL)
	DISP_PROPERTY_ID(CXTPChartAxisRange, "ZoomLimit", 9, m_dZoomLimit, VT_R8)
END_DISPATCH_MAP()

// {F34BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAxisRange =
{ 0xf34bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAxisRange, CXTPChartElement)
INTERFACE_PART(CXTPChartAxisRange, IID_IChartAxisRange, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisRange, IID_IChartAxisRange)


BOOL CXTPChartAxisRange::OleGetShowZeroLevel()
{
	return GetShowZeroLevel();
}

void CXTPChartAxisRange::OleSetShowZeroLevel(BOOL bSet)
{
	SetShowZeroLevel(bSet);
}


BOOL CXTPChartAxisRange::OleGetSideMargins()
{
	return GetSideMargins();
}

void CXTPChartAxisRange::OleSetSideMargins(BOOL bSet)
{
	SetSideMargins(bSet);
}


BOOL CXTPChartAxisRange::OleGetAutoRange()
{
	return m_bAutoRange;
}

void CXTPChartAxisRange::OleSetAutoRange(BOOL bSet)
{
	SetAutoRange(bSet);
}


double CXTPChartAxisRange::OleGetMinValue()
{
	return m_nMinValue;
}

void CXTPChartAxisRange::OleSetMinValue(double bSet)
{
	SetMinValue(bSet);
}

double CXTPChartAxisRange::OleGetMaxValue()
{
	return m_nMaxValue;
}

void CXTPChartAxisRange::OleSetMaxValue(double bSet)
{
	SetMaxValue(bSet);
}

#endif
