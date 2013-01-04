// XTPChartAxisCustomLabels.cpp
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

#include "XTPChartAxisCustomLabels.h"
#include "XTPChartAxis.h"

IMPLEMENT_DYNCREATE(CXTPChartAxisCustomLabel, CXTPChartTextElement)

//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisCustomLabel

CXTPChartAxisCustomLabel::CXTPChartAxisCustomLabel()
{
	m_dAxisValue = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisCustomLabel::~CXTPChartAxisCustomLabel()
{
}

void CXTPChartAxisCustomLabel::SetText(const CXTPChartString& lpszCustomLabel)
{
	m_strText = lpszCustomLabel;
	OnChartChanged();
}

void CXTPChartAxisCustomLabel::DoPropExchange(CXTPPropExchange* pPX)
{

	PX_String(pPX, _T("Text"), m_strText, _T(""));

	PX_String(pPX, _T("AxisValue"), m_strAxisValue, _T(""));
	PX_Double(pPX, _T("AxisValueInternal"), m_dAxisValue, 0);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisCustomLabels


CXTPChartAxisCustomLabels::CXTPChartAxisCustomLabels(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisCustomLabels::~CXTPChartAxisCustomLabels()
{
}

CXTPChartAxisCustomLabel* CXTPChartAxisCustomLabels::Add(CXTPChartAxisCustomLabel* pCustomLabel)
{
	InsertAt(GetCount(), pCustomLabel);

	return pCustomLabel;
}

void CXTPChartAxisCustomLabels::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("CustomLabel")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));


			CXTPChartAxisCustomLabel* pElement = new CXTPChartAxisCustomLabel();
			Add(pElement);

			pElement->DoPropExchange(&pxItem);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("CustomLabel")));
		POSITION pos = pEnumerator->GetPosition((int)m_arrElements.GetSize());

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			m_arrElements[i]->DoPropExchange(&pxItem);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisCustomLabel, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisCustomLabel, "Text", DISPID_VALUE, OleGetText, OleSetText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPChartAxisCustomLabel, "AxisValue", 1, OleGetAxisValue, OleSetAxisValue, VT_VARIANT)

END_DISPATCH_MAP()


BSTR CXTPChartAxisCustomLabel::OleGetText()
{
	return m_strText.AllocSysString();
}

void CXTPChartAxisCustomLabel::OleSetText(LPCTSTR lpszText)
{
	SetText(lpszText);
}

VARIANT CXTPChartAxisCustomLabel::OleGetAxisValue()
{
	VARIANT vt;
	if (!m_strAxisValue.IsEmpty())
	{
		vt.vt = VT_BSTR;
		vt.bstrVal = m_strAxisValue.AllocSysString();
	}
	else
	{
		vt.vt = VT_R8;
		vt.dblVal = m_dAxisValue;
	}
	return vt;
}

void CXTPChartAxisCustomLabel::OleSetAxisValue(const VARIANT& var)
{
	if (IsStringVariant(&var))
		SetAxisValue(VariantToString(&var));
	else
		SetAxisValue(VariantToDoubleEx(&var));
}


// {E9ABCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAxisCustomLabel =
{ 0xe9abcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAxisCustomLabel, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisCustomLabel, IID_IChartAxisCustomLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisCustomLabel, IID_IChartAxisCustomLabel)

//////////////////////////////////////////////////////////////////////////
//

BEGIN_DISPATCH_MAP(CXTPChartAxisCustomLabels, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartAxisCustomLabels, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartAxisCustomLabels, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPChartAxisCustomLabels, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPChartAxisCustomLabels, "Add", 2, OleAdd, VT_DISPATCH, VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPChartAxisCustomLabels, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// {E9BBCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAxisCustomLabels =
{ 0xe9bbcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAxisCustomLabels, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxisCustomLabels, IID_IChartAxisCustomLabels, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisCustomLabels, IID_IChartAxisCustomLabels)


long CXTPChartAxisCustomLabels::OleGetItemCount()
{
	return (long)GetCount();
}

LPDISPATCH CXTPChartAxisCustomLabels::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPChartAxisCustomLabel* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

LPDISPATCH CXTPChartAxisCustomLabels::OleAdd(LPCTSTR lpszCustomLabel, const VARIANT& val)
{
	CXTPChartAxisCustomLabel* pCustomLabel = Add(new CXTPChartAxisCustomLabel());

	pCustomLabel->SetText(lpszCustomLabel);
	pCustomLabel->OleSetAxisValue(val);

	return XTPGetDispatch(pCustomLabel);
}

IMPLEMENT_ENUM_VARIANT(CXTPChartAxisCustomLabels)


#endif
