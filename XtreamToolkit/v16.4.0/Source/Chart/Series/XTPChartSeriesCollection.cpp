// XTPChartSeries.cpp
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

#include <Common/XTPPropExchange.h>

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartTrendline.h>
#include <Chart/XTPChartContent.h>
#include <Chart/XTPChartPanel.h>
#include <Chart/XTPChartDiagram.h>
#include <Chart/Appearance/XTPChartAppearance.h>
#include <Chart/Appearance/XTPChartPalette.h>
#include <Chart/Utils/XTPChartNumberFormat.h>


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesCollection

CXTPChartSeriesCollection::CXTPChartSeriesCollection(CXTPChartContent* pOwner)
{
	m_pOwner = (CXTPChartElement*)pOwner;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

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


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartSeriesCollection, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "Add", 2, OleAdd, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "AddTrendline", 3, OleAddTrendline, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartSeriesCollection, "DeleteAt", 6, RemoveAt, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// {D87BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartSeriesCollection =
{ 0xd87bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartSeriesCollection, CXTPChartElement)
	INTERFACE_PART(CXTPChartSeriesCollection, IID_IChartSeriesCollection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeriesCollection, IID_IChartSeriesCollection)

IMPLEMENT_ENUM_VARIANT(CXTPChartSeriesCollection)

long CXTPChartSeriesCollection::OleGetItemCount()
{
	return (long)GetCount();
}

#pragma warning(push)
#pragma warning(disable : 4702)

LPDISPATCH CXTPChartSeriesCollection::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPChartSeries *pSeries = GetAt(nIndex);
		//return XTPGetDispatch(pSeries);
		return pSeries->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

#pragma warning(pop)

LPDISPATCH CXTPChartSeriesCollection::OleAdd(LPCTSTR lpszName)
{
	CXTPChartSeries *pSeries = new CXTPChartSeries();
	Add(pSeries);

	pSeries->SetName(lpszName);

	return XTPGetDispatch(pSeries);
}

LPDISPATCH CXTPChartSeriesCollection::OleAddTrendline(LPCTSTR lpszName)
{
	CXTPChartSeriesTrendline *pSeries = new CXTPChartSeriesTrendline();
	Add(pSeries);

	pSeries->SetName(lpszName);

	return XTPGetDispatch(pSeries);
}

#endif
