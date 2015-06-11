// XTPChartSeriesPoint.cpp
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

#include "Common/XTPResourceManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPCustomHeap.h"

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "XTPChartSeriesPoint.h"
#include "XTPChartSeries.h"
#include <Common/Math/XTPMathStats.h>

#include <math.h>

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

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

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

//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesPointCollection

IMPLEMENT_DYNAMIC(CXTPChartSeriesPointCollection, CXTPChartElementCollection)

CXTPChartSeriesPointCollection::CXTPChartSeriesPointCollection()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
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


double CXTPChartSeriesPointCollection::GetArithmeticMean(int nValue) const
{
	return GetMovingAverage(nValue, 0, GetCount()+1);
}


double CXTPChartSeriesPointCollection::GetVariance(int nValue) const
{
	double dArithmeticMean = GetArithmeticMean(nValue);
	CXTPMathVariance variance;

	for (int nPoint=0; nPoint<GetCount(); nPoint++)
	{
		const CXTPChartSeriesPoint *pPoint = GetAt(nPoint);
		variance.Add(pPoint->m_dValues[nValue], dArithmeticMean);
	}

	return variance;
}


double CXTPChartSeriesPointCollection::GetStandardDeviation(int nValue) const
{
	return sqrt(GetVariance(nValue));
}


double CXTPChartSeriesPointCollection::GetMovingAverage(int nValue, int nFrom, int nTo) const
{
	CXTPMathArithmeticMean mean;

	for (int nPoint=nFrom; nPoint<=nTo; nPoint++)
	{
		const CXTPChartSeriesPoint *pPoint = GetAt(nPoint);

		if (NULL != pPoint)
		{
			mean.Add(pPoint->m_dValues[nValue]);
		}
	}

	return mean;
}

double CXTPChartSeriesPointCollection::GetMinimum(int nValue) const
{
	double dMinimum = 0;

	for (int nPoint=0; nPoint<GetCount(); nPoint++)
	{
		const CXTPChartSeriesPoint *pPoint = GetAt(nPoint);
		ASSERT(NULL != pPoint);

		const double dValue = pPoint->m_dValues[nValue];

		if (0 == nPoint)
		{
			dMinimum = dValue;
		}
		else
		{
			dMinimum = min(dMinimum, dValue);
		}
	}

	return dMinimum;
}

double CXTPChartSeriesPointCollection::GetMaximum(int nValue) const
{
	double dMaximum = 0;

	for (int nPoint=0; nPoint<GetCount(); nPoint++)
	{
		const CXTPChartSeriesPoint *pPoint = GetAt(nPoint);
		ASSERT(NULL != pPoint);

		const double dValue = pPoint->m_dValues[nValue];

		if (0 == nPoint)
		{
			dMaximum = dValue;
		}
		else
		{
			dMaximum = max(dMaximum, dValue);
		}
	}

	return dMaximum;
}

void CXTPChartSeriesPointCollection::GetSlopeQualitative(int nValue, double &dSlope, double &dIntercept) const
{
	int nPoint;

	CXTPMathArithmeticMean meanX;

	for (nPoint=0; nPoint<GetCount(); nPoint++)
	{
		meanX.Add(nPoint+1);
	}

	CXTPMathVariance varianceX;

	for (nPoint=0; nPoint<GetCount(); nPoint++)
	{
		varianceX.Add(nPoint+1, meanX);
	}

	double dMeanY = GetArithmeticMean(nValue);

	CXTPMathCovariance covariance;

	for (nPoint=0; nPoint<GetCount(); nPoint++)
	{
		const CXTPChartSeriesPoint *pPoint = GetAt(nPoint);

		if (NULL != pPoint)
		{
			covariance.Add(nPoint+1, meanX, pPoint->m_dValues[nValue], dMeanY);
		}
	}

	dSlope = (covariance / varianceX);
	dIntercept = dMeanY-dSlope*meanX;
}

void CXTPChartSeriesPointCollection::GetSlopeNumerical(int nValue, double &dSlope, double &dIntercept) const
{
	int nPoint;

	CXTPMathArithmeticMean meanX;

	for (nPoint=0; nPoint<GetCount(); nPoint++)
	{
		const CXTPChartSeriesPoint *pPoint = GetAt(nPoint);
		meanX.Add(pPoint->GetArgumentValue());
	}

	CXTPMathVariance varianceX;

	for (nPoint=0; nPoint<GetCount(); nPoint++)
	{
		const CXTPChartSeriesPoint *pPoint = GetAt(nPoint);
		varianceX.Add(pPoint->GetArgumentValue(), meanX);
	}

	double dMeanY = GetArithmeticMean(nValue);

	CXTPMathCovariance covariance;

	for (nPoint=0; nPoint<GetCount(); nPoint++)
	{
		const CXTPChartSeriesPoint *pPoint = GetAt(nPoint);

		if (NULL != pPoint)
		{
			covariance.Add(pPoint->GetArgumentValue(), meanX, pPoint->m_dValues[nValue], dMeanY);
		}
	}

	dSlope = (covariance / varianceX);
	dIntercept = dMeanY-dSlope*meanX;
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

IMPLEMENT_DYNAMIC(CXTPChartSeriesVirtualPointCollection, CXTPChartSeriesPointCollection);

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

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartSeriesPoint, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartSeriesPoint, "GetPreviousPoint", 1, OleGetPreviousPoint, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesPoint, "GetNextPoint", 2, OleGetNextPoint, VT_DISPATCH, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPChartSeriesPoint, "InternalArgumentValue", 7, GetInternalArgumentValue, SetNotSupported, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesPoint, "ArgumentValue", 5, GetArgumentValue, OleSetArgumentValue, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesPoint, "ArgumentValueString", 6, OleGetArgumentString, OleSetArgumentString, VT_BSTR)

	DISP_PROPERTY_EX_ID(CXTPChartSeriesPoint, "LegendText", 9, OleGetLegendText, OleSetLegendText, VT_BSTR)



	DISP_PROPERTY_PARAM_ID(CXTPChartSeriesPoint, "Value", DISPID_VALUE, GetValue, SetValue, VT_R8, VTS_I4)

	DISP_PROPERTY_EX_ID(CXTPChartSeriesPoint, "Special", 8, OleGetSpecial, OleSetSpecial, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartSeriesPoint, "Color", 10, OleGetColor, OleSetColor, VT_COLOR)

	DISP_PROPERTY_ID(CXTPChartSeriesPoint, "TooltipText", 11, m_strTooltipText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPChartSeriesPoint, "LabelText", 12, OleGetLabelText, OleSetLabelText, VT_BSTR)

END_DISPATCH_MAP()


// {CD8BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartSeriesPoint =
{ 0xcd8bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartSeriesPoint, CXTPChartElement)
INTERFACE_PART(CXTPChartSeriesPoint, IID_IChartSeriesPoint, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeriesPoint, IID_IChartSeriesPoint)


BSTR CXTPChartSeriesPoint::OleGetArgumentString()
{
	return m_strArgument.AllocSysString();
}

void CXTPChartSeriesPoint::OleSetArgumentString(LPCTSTR lpszText)
{
	m_strArgument = lpszText;
	OnChartChanged();
}


BSTR CXTPChartSeriesPoint::OleGetLabelText()
{
	return m_strLabelText.AllocSysString();
}

void CXTPChartSeriesPoint::OleSetLabelText(LPCTSTR lpszText)
{
	SetLabelText(lpszText);
}


BSTR CXTPChartSeriesPoint::OleGetLegendText()
{
	return m_strLegentText.AllocSysString();
}

void CXTPChartSeriesPoint::OleSetLegendText(LPCTSTR lpszText)
{
	SetLegentText(lpszText);
}

void CXTPChartSeriesPoint::OleSetArgumentValue(double dNewValue)
{
	SetArgumentValue(dNewValue);
}

OLE_COLOR CXTPChartSeriesPoint::OleGetColor()
{
	return m_clrPoint.ToOleColor();
}

void CXTPChartSeriesPoint::OleSetColor(OLE_COLOR clr)
{
	SetColor(CXTPChartColor::FromOleColor(clr));
}

LPDISPATCH CXTPChartSeriesPoint::OleGetPreviousPoint()
{
	if (!m_pOwner)
		return NULL;

	CXTPChartSeriesPointCollection* pOwner = (CXTPChartSeriesPointCollection*)m_pOwner;

	CXTPChartSeriesPoint* pItem = (m_nIndex > 0 ? pOwner->GetAt(m_nIndex - 1) : NULL);
	if (!pItem)
		return NULL;

	return pItem->GetIDispatch(TRUE);
}

LPDISPATCH CXTPChartSeriesPoint::OleGetNextPoint()
{
	if (!m_pOwner)
		return NULL;

	CXTPChartSeriesPointCollection* pOwner = (CXTPChartSeriesPointCollection*)m_pOwner;

	CXTPChartSeriesPoint* pItem =  (m_nIndex < pOwner->GetCount() ? pOwner->GetAt(m_nIndex + 1) : NULL);
	if (!pItem)
		return NULL;

	return pItem->GetIDispatch(TRUE);
}

BOOL CXTPChartSeriesPoint::OleGetSpecial()
{
	return m_bSpecial;
}

void CXTPChartSeriesPoint::OleSetSpecial(BOOL bSpecial)
{
	m_bSpecial = bSpecial;
	OnChartChanged();
}

//////////////////////////////////////////////////////////////////////////
//

BEGIN_DISPATCH_MAP(CXTPChartSeriesPointCollection, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "Add", 2, OleAdd, VT_DISPATCH, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "Add4", 300, OleAdd4, VT_DISPATCH, VTS_VARIANT VTS_R8 VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "Add2", 301, OleAdd2, VT_DISPATCH, VTS_VARIANT VTS_R8 VTS_R8)

	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "DeleteAt", 6, RemoveAt, VT_EMPTY, VTS_I4)

	DISP_FUNCTION_ID(CXTPChartSeriesPointCollection, "AddPoints", 7, AddPoints, VT_EMPTY, VTS_I4)

END_DISPATCH_MAP()

// {CD9BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartSeriesPointCollection =
{ 0xcd9bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartSeriesPointCollection, CXTPChartElement)
	INTERFACE_PART(CXTPChartSeriesPointCollection, IID_IChartSeriesPointCollection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeriesPointCollection, IID_IChartSeriesPointCollection)


long CXTPChartSeriesPointCollection::OleGetItemCount()
{
	return (long)GetCount();
}

#pragma warning(push)
#pragma warning(disable : 4702)

LPDISPATCH CXTPChartSeriesPointCollection::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPChartSeriesPoint* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

#pragma warning(pop)

LPDISPATCH CXTPChartSeriesPointCollection::OleAdd(const VARIANT& varArgument, const VARIANT& varValue)
{

	CXTPChartSeriesPoint* pPoint;

	if (IsStringVariant(&varArgument))
		pPoint = Add(new CXTPChartSeriesPoint(VariantToString(&varArgument), VariantToDoubleEx(&varValue)));
	else
		pPoint = Add(new CXTPChartSeriesPoint(VariantToDouble(&varArgument), VariantToDoubleEx(&varValue)));

	return XTPGetDispatch(pPoint);
}


LPDISPATCH CXTPChartSeriesPointCollection::OleAdd4(const VARIANT& varArgument, double varValue1, double varValue2, double varValue3, double varValue4)
{
	CXTPChartSeriesPoint* pPoint;

	if (IsStringVariant(&varArgument))
		pPoint = Add(new CXTPChartSeriesPoint(VariantToString(&varArgument), varValue1, varValue2, varValue3, varValue4));
	else
		pPoint = Add(new CXTPChartSeriesPoint(VariantToDouble(&varArgument), varValue1, varValue2, varValue3, varValue4));


	return XTPGetDispatch(pPoint);
}

LPDISPATCH CXTPChartSeriesPointCollection::OleAdd2(const VARIANT& varArgument, double varValue1, double varValue2)
{
	CXTPChartSeriesPoint* pPoint;

	if (IsStringVariant(&varArgument))
		pPoint = Add(new CXTPChartSeriesPoint(VariantToString(&varArgument), varValue1, varValue2));
	else
		pPoint = Add(new CXTPChartSeriesPoint(VariantToDouble(&varArgument), varValue1, varValue2));


	return XTPGetDispatch(pPoint);
}


IMPLEMENT_ENUM_VARIANT(CXTPChartSeriesPointCollection)


#endif
