// XTPChartPalette.cpp
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

#include "../Types/XTPChartTypes.h"
#include "../XTPChartElement.h"

#include "XTPChartPalette.h"


CXTPChartPalette::CXTPChartPalette(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartPalette::~CXTPChartPalette()
{
}

CXTPChartColor CXTPChartPalette::GetColor(int Index) const
{
	if (m_arrEntries.GetSize() == 0)
		return CXTPChartColor(CXTPChartColor::White);

	return m_arrEntries[Index % m_arrEntries.GetSize()].Color;
}

CXTPChartColor CXTPChartPalette::GetColor2(int Index) const
{
	if (m_arrEntries.GetSize() == 0)
		return CXTPChartColor(CXTPChartColor::Black);

	return m_arrEntries[Index % m_arrEntries.GetSize()].Color2;
}


void CXTPChartPalette::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection pxItems(pPX->GetSection(_T("Items")));

	if (pPX->IsLoading())
	{
		m_arrEntries.RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pxItems->GetEnumerator(_T("Item")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			CXTPChartPaletteEntry entry;
			PX_Color(&pxItem, _T("Color"), entry.Color);

			CXTPChartColor clr2;
			PX_Color(&pxItem, _T("Color2"), entry.Color2);

			m_arrEntries.Add(entry);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pxItems->GetEnumerator(_T("Item")));
		POSITION pos = pEnumerator->GetPosition((int)m_arrEntries.GetSize());

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPChartPaletteEntry& entry = m_arrEntries[i];

			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			PX_Color(&pxItem, _T("Color"), entry.Color);

			PX_Color(&pxItem, _T("Color2"), entry.Color2);
		}

	}

}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPalette, CXTPChartElement)
	DISP_PROPERTY_PARAM_ID(CXTPChartPalette, "Color", 1, OleGetColor, OleSetColor, VT_COLOR, VTS_I4)
	DISP_PROPERTY_PARAM_ID(CXTPChartPalette, "Color2", 2, OleGetColor2, OleSetColor2, VT_COLOR, VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPChartPalette, "Count", 3, OleGetCount, SetNotSupported, VT_I4)
	DISP_FUNCTION_ID(CXTPChartPalette, "DeleteAll", 4, OleDeleteAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartPalette, "AddEntry", 5, OleAddEntry, VT_EMPTY, VTS_COLOR VTS_COLOR)
END_DISPATCH_MAP()

// {A67BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPalette =
{ 0xa67bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPalette, CXTPChartElement)
	INTERFACE_PART(CXTPChartPalette, IID_IChartPalette, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPalette, IID_IChartPalette)

OLE_COLOR CXTPChartPalette::OleGetColor(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrEntries.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	return m_arrEntries[nIndex].Color.ToOleColor();
}


OLE_COLOR CXTPChartPalette::OleGetColor2(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrEntries.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	return m_arrEntries[nIndex].Color2.ToOleColor();
}

void CXTPChartPalette::OleSetColor(int nIndex, OLE_COLOR clr)
{
	if (nIndex < 0 || nIndex >= m_arrEntries.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	m_arrEntries[nIndex].Color = CXTPChartColor::FromOleColor(clr);
}

void CXTPChartPalette::OleSetColor2(int nIndex, OLE_COLOR clr)
{
	if (nIndex < 0 || nIndex >= m_arrEntries.GetSize())
		AfxThrowOleException(E_INVALIDARG);

	m_arrEntries[nIndex].Color2 = CXTPChartColor::FromOleColor(clr);
}

void CXTPChartPalette::OleDeleteAll()
{
	m_arrEntries.RemoveAll();
}

int CXTPChartPalette::OleGetCount()
{
	return m_arrEntries.GetSize();
}

void CXTPChartPalette::OleAddEntry(OLE_COLOR clr, OLE_COLOR clr2)
{
	CXTPChartPaletteEntry entry;
	entry.Color = CXTPChartColor::FromOleColor(clr);
	entry.Color2 = CXTPChartColor::FromOleColor(clr2);

	m_arrEntries.Add(entry);
}


#endif
