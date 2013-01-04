// XTPChartSeries.cpp
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

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include "XTPChartSeries.h"
#include "XTPChartSeriesStyle.h"
#include "XTPChartSeriesPoint.h"
#include "XTPChartContent.h"
#include "XTPChartPanel.h"
#include "XTPChartDiagram.h"

#include "Utils/XTPChartNumberFormat.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeries

IMPLEMENT_DYNAMIC(CXTPChartSeries, CXTPChartElement)

CXTPChartSeries::CXTPChartSeries()
{
	m_pStyle = NULL;
	m_pDiagram = NULL;

	m_pPoints = new CXTPChartSeriesPointCollection();
	m_pPoints->m_pOwner = this;

	m_nArgumentScaleType = xtpChartScaleQualitative;
	m_nValueScaleType = xtpChartScaleNumerical;

	m_nPaletteIndex = 0;
	m_bVisible = TRUE;

	m_bLegendVisible = TRUE;

	m_pPointLegendFormat = new CXTPChartNumberFormat(this);
	m_pPointLegendFormat->SetPattern(_T("{A}: {V}"));

}

CXTPChartSeries::~CXTPChartSeries()
{
	SAFE_RELEASE(m_pStyle);
	SAFE_RELEASE(m_pPoints);
	SAFE_RELEASE(m_pPointLegendFormat);

	SetDiagram(NULL);
}

CXTPChartSeriesStyle* CXTPChartSeries::SetStyle(CXTPChartSeriesStyle* pStyle)
{
	if (!pStyle)
		return pStyle;

	SAFE_RELEASE(m_pStyle);

	m_pStyle = pStyle;
	m_pStyle->m_pOwner = this;

	GetContent()->OnSeriesStyleChanged(this);

	OnChartChanged();

	return pStyle;
}

BOOL CXTPChartSeries::SetDiagram(CXTPChartDiagram* pDiagram)
{
	if (pDiagram == m_pDiagram)
		return TRUE;

	if (pDiagram)
	{
		if (!m_pStyle)
			return FALSE;

		if (!m_pStyle->IsStyleDiagram(pDiagram))
			return FALSE;
	}

	if (m_pDiagram)
	{
		m_pDiagram->OnSeriesRemoved(this);

		CMDTARGET_RELEASE(m_pDiagram);
	}


	if (pDiagram)
	{
		if (!pDiagram->OnSeriesAdded(this))
			return FALSE;

		pDiagram->InternalAddRef();
	}

	m_pDiagram = pDiagram;

	OnChartChanged();

	return TRUE;
}

void CXTPChartSeries::SetPointLegendFormat(LPCTSTR lpszFormat)
{
	m_pPointLegendFormat->SetPattern(lpszFormat);
}

void CXTPChartSeries::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("LegendText"), m_strLegendText);
	PX_String(pPX, _T("Name"), m_strName);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Enum(pPX, _T("ArgumentScaleType"), m_nArgumentScaleType, xtpChartScaleQualitative);
	PX_Enum(pPX, _T("ValueScaleType"), m_nValueScaleType, xtpChartScaleNumerical);

	PX_Bool(pPX, _T("LegendVisible"), m_bLegendVisible, TRUE);

	CXTPPropExchangeSection secPointLegendFormat(pPX->GetSection(_T("PointLegendFormat")));
	m_pPointLegendFormat->DoPropExchange(&secPointLegendFormat);

	CXTPPropExchangeSection secPoints(pPX->GetSection(_T("Points")));
	m_pPoints->DoPropExchange(&secPoints);

	PX_Object(pPX, _T("Style"), m_pStyle, RUNTIME_CLASS(CXTPChartSeriesStyle));

	if (pPX->IsLoading())
	{
		m_pStyle->m_pOwner = this;
	}

	CXTPChartContent* pContent = GetContent();
	ASSERT (pContent);

	if (pPX->IsStoring())
	{
		int nIndex = 0;
		for (; nIndex < pContent->GetPanels()->GetCount(); nIndex++)
		{
			if (pContent->GetPanels()->GetAt(nIndex) == m_pDiagram)
			{
				break;
			}
		}
		PX_Int(pPX, _T("Diagram"), nIndex, -1);
	}
	else
	{
		int nIndex;
		PX_Int(pPX, _T("Diagram"), nIndex, -1);

		if (nIndex >= 0 && nIndex < pContent->GetPanels()->GetCount())
		{
			CXTPChartDiagram* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram, pContent->GetPanels()->GetAt(nIndex));

			SetDiagram(pDiagram);
		}
	}
}

void CXTPChartSeries::SetPoints(CXTPChartSeriesPointCollection* pPoints)
{
	ASSERT(pPoints);
	if (!pPoints)
		return;

	SAFE_RELEASE(m_pPoints);

	m_pPoints = pPoints;
	m_pPoints->m_pOwner = this;

	OnChartChanged();
}

void CXTPChartSeries::Release()
{
	SetDiagram(NULL);

	CXTPChartElement::Release();
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesCollection

CXTPChartSeriesCollection::CXTPChartSeriesCollection(CXTPChartContent* pOwner)
{
	m_pOwner = (CXTPChartElement*)pOwner;


}

CXTPChartSeriesCollection::~CXTPChartSeriesCollection()
{
}

CXTPChartSeries* CXTPChartSeriesCollection::Add(CXTPChartSeries* pSeries)
{
	InsertAt(GetCount(), pSeries);

	return pSeries;
}


void CXTPChartSeriesCollection::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Series")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));


			CXTPChartSeries* pSeries = new CXTPChartSeries();
			Add(pSeries);

			pSeries->DoPropExchange(&pxItem);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Series")));
		POSITION pos = pEnumerator->GetPosition((int)m_arrElements.GetSize());

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			m_arrElements[i]->DoPropExchange(&pxItem);
		}

	}
}



