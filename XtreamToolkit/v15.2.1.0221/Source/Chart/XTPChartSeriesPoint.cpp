// XTPChartSeriesPoint.cpp
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

#include "Common/XTPResourceManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPCustomHeap.h"

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include "XTPChartSeriesPoint.h"
#include "XTPChartSeries.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeries

IMPLEMENT_DYNAMIC(CXTPChartSeriesPoint, CXTPChartElement);

CXTPChartSeriesPoint::CXTPChartSeriesPoint()
{
	ZeroMemory(m_dValues, sizeof(m_dValues));
	m_nLength = 1;

	CommonConstruct();
}

CXTPChartSeriesPoint::~CXTPChartSeriesPoint()
{
}


CXTPChartSeriesPoint::CXTPChartSeriesPoint(const CXTPChartString& strArgument, double dValue)
{
	m_strArgument = strArgument;

	ZeroMemory(m_dValues, sizeof(m_dValues));
	m_dValues[0] = dValue;
	m_nLength = 1;

	CommonConstruct();
}

CXTPChartSeriesPoint::CXTPChartSeriesPoint(double dArgument, double dValue)
{
	m_dArgument = dArgument;

	ZeroMemory(m_dValues, sizeof(m_dValues));
	m_dValues[0] = dValue;
	m_nLength = 1;
	m_dInternalArgument = dArgument;

	CommonConstruct();
}

CXTPChartSeriesPoint::CXTPChartSeriesPoint(double dArgument, double dValue1, double dValue2)
{
	m_dArgument = dArgument;

	ZeroMemory(m_dValues, sizeof(m_dValues));
	m_dValues[0] = dValue1;
	m_dValues[1] = dValue2;
	m_nLength = 2;
	m_dInternalArgument = dArgument;

	CommonConstruct();
}

CXTPChartSeriesPoint::CXTPChartSeriesPoint(const CXTPChartString& strArgument, double dValue1, double dValue2)
{
	m_strArgument = strArgument;

	ZeroMemory(m_dValues, sizeof(m_dValues));
	m_dValues[0] = dValue1;
	m_dValues[1] = dValue2;
	m_nLength = 2;

	CommonConstruct();
}

CXTPChartSeriesPoint::CXTPChartSeriesPoint(const CXTPChartString& strArgument, double dValue1, double dValue2, double dValue3, double dValue4)
{
	m_strArgument = strArgument;

	ZeroMemory(m_dValues, sizeof(m_dValues));
	m_dValues[0] = dValue1;
	m_dValues[1] = dValue2;
	m_dValues[2] = dValue3;
	m_dValues[3] = dValue4;
	m_nLength = 4;

	CommonConstruct();
}

CXTPChartSeriesPoint::CXTPChartSeriesPoint(double dArgument, double dValue1, double dValue2, double dValue3, double dValue4)
{
	m_dArgument = dArgument;

	ZeroMemory(m_dValues, sizeof(m_dValues));
	m_dValues[0] = dValue1;
	m_dValues[1] = dValue2;
	m_dValues[2] = dValue3;
	m_dValues[3] = dValue4;
	m_nLength = 4;

	CommonConstruct();
}

void CXTPChartSeriesPoint::CommonConstruct()
{
	m_nPaletteIndex = 0;
	m_bSpecial= 0;

	if (m_strArgument.IsEmpty())
	{
		CString str;
		str.Format(_T("%g"), m_dArgument);
		m_strArgument = str;
	}
	else
	{
		m_dArgument = atof(XTP_CT2CA(m_strArgument));
		m_dInternalArgument = m_dArgument;

	}
	m_nIndex = -1;

	m_clrPoint = CXTPChartColor::Empty;


}

double CXTPChartSeriesPoint::GetValue(int nIndex) const
{
	if (nIndex >= m_nLength)
		return 0;

	return m_dValues[nIndex];
}

void CXTPChartSeriesPoint::SetValue(int nIndex, double dValue)
{
	if (nIndex >= m_nLength)
		return;

	m_dValues[nIndex] = dValue;
}

CXTPChartSeriesPoint* CXTPChartSeriesPoint::GetPreviousPoint() const
{
	if (!m_pOwner)
		return NULL;

	CXTPChartSeriesPointCollection* pOwner = (CXTPChartSeriesPointCollection*)m_pOwner;

	return pOwner->GetAt(m_nIndex - 1);
}

CXTPChartSeriesPoint* CXTPChartSeriesPoint::GetNextPoint() const
{
	if (!m_pOwner)
		return NULL;

	CXTPChartSeriesPointCollection* pOwner = (CXTPChartSeriesPointCollection*)m_pOwner;

	return pOwner->GetAt(m_nIndex + 1);
}

void CXTPChartSeriesPoint::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Argument"), m_strArgument, _T(""));
	PX_Double(pPX, _T("ArgumentValue"), m_dArgument, 0);

	PX_Int(pPX, _T("Length"), m_nLength, 1);
	PX_Bool(pPX, _T("Special"), m_bSpecial, FALSE);

	for (int i = 0; i < m_nLength; i++)
	{
		CString strValue;
		strValue.Format(_T("Value%d"), i);
		PX_Double(pPX, strValue, m_dValues[i]);
	}

	if (pPX->IsLoading())
	{
		m_dInternalArgument = m_dArgument;
	}
}

CXTPChartSeries* CXTPChartSeriesPoint::GetSeries() const
{
	CXTPChartSeriesPointCollection* pCollection = DYNAMIC_DOWNCAST(CXTPChartSeriesPointCollection, m_pOwner);
	if (!pCollection)
		return NULL;

	CXTPChartSeries* pSeries = DYNAMIC_DOWNCAST(CXTPChartSeries, pCollection->GetOwner());

	return pSeries;
}

void CXTPChartSeriesPoint::SetInternalArgument(double dInternalArgument)
{
	m_dInternalArgument = dInternalArgument;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesPointCollection

IMPLEMENT_DYNAMIC(CXTPChartSeriesPointCollection, CXTPChartElementCollection)

CXTPChartSeriesPointCollection::CXTPChartSeriesPointCollection()
{
}

CXTPChartSeriesPointCollection::~CXTPChartSeriesPointCollection()
{
}


CXTPChartSeriesPoint* CXTPChartSeriesPointCollection::Add(CXTPChartSeriesPoint* pPoint)
{
	pPoint->m_nIndex = GetCount();

	InsertAt(GetCount(), pPoint);

	return pPoint;
}

void CXTPChartSeriesPointCollection::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Point")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));


			CXTPChartSeriesPoint* pPoint = new CXTPChartSeriesPoint();
			Add(pPoint);

			pPoint->DoPropExchange(&pxItem);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Point")));
		POSITION pos = pEnumerator->GetPosition((int)m_arrElements.GetSize());

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			m_arrElements[i]->DoPropExchange(&pxItem);
		}

	}
}


XTP_DECLARE_HEAP_ALLOCATOR_(CXTPChartSeriesPointAllocator, XTP_EXPORT_PARAMS_NO)
XTP_IMPLEMENT_HEAP_ALLOCATOR(CXTPChartSeriesPointAllocator, FALSE)


XTP_DECLARE_BATCH_ALLOC_OBJ_DATA_(CXTPChartSeriesBatchPointData, XTP_EXPORT_PARAMS_NO);

class CXTPChartSeriesBatchPoint : public CXTPBatchAllocObjT<CXTPHeapObjectT<CXTPChartSeriesPoint, CXTPChartSeriesPointAllocator>,
	 CXTPChartSeriesBatchPointData>
{

};

XTP_IMPLEMENT_BATCH_ALLOC_OBJ_DATA(CXTPChartSeriesBatchPointData, CXTPChartSeriesBatchPoint, TRUE)


void CXTPChartSeriesPointCollection::AddPoints(int nCount)
{

	CXTPChartSeriesBatchPointData::m_nBlockSize = nCount;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartSeriesBatchPoint* pPoint = new CXTPChartSeriesBatchPoint();

		Add(pPoint);
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesVirtualPointCollection

CXTPChartSeriesVirtualPointCollection::CXTPChartSeriesVirtualPointCollection(double dMinRange, double dMaxRange, double dStep, GETPOINTVALUEPTR pCallbackPtr)
{
	m_dMinRange = dMinRange;
	m_dMaxRange = dMaxRange;
	m_dStep = dStep;

	if (dStep == 0)
		dStep = 1;

	m_nCount = int((m_dMaxRange - m_dMinRange) / dStep);

	m_pVirtualPoint = new CXTPChartSeriesPoint();
	m_pVirtualPoint->m_pOwner = this;
	m_pCallbackPtr = pCallbackPtr;
}

CXTPChartSeriesVirtualPointCollection::~CXTPChartSeriesVirtualPointCollection()
{
	SAFE_RELEASE(m_pVirtualPoint);
}

int CXTPChartSeriesVirtualPointCollection::GetCount() const
{
	return m_nCount;
}

CXTPChartSeriesPoint* CXTPChartSeriesVirtualPointCollection::GetAt(int nIndex) const
{
	double x = m_dMinRange + nIndex * m_dStep;

	m_pVirtualPoint->m_dArgument = m_pVirtualPoint->m_dInternalArgument = x;
	m_pVirtualPoint->m_nLength = 1;
	m_pVirtualPoint->m_dValues[0] = m_pCallbackPtr((CXTPChartSeries*)m_pOwner, x);

	return m_pVirtualPoint;
}

