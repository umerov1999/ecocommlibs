// XTPReportHyperlink.cpp : implementation of the CXTPReportHyperlink class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPCustomHeap.h"

#include "XTPReportDefines.h"
#include "XTPReportHyperlink.h"

#pragma warning(disable: 4097) // TBase' used as synonym for class-name


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CXTPReportHyperlinkStyle::CXTPReportHyperlinkStyle(COLORREF color, XTPReportTextDecoration textDecoration)
	: m_color(color)
	, m_textDecoration(textDecoration)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

COLORREF CXTPReportHyperlinkStyle::GetColor() const
{
	return m_color;
}

void CXTPReportHyperlinkStyle::SetColor(COLORREF color)
{
	m_color = color;
}

XTPReportTextDecoration CXTPReportHyperlinkStyle::GetTextDecoration() const
{
	return m_textDecoration;
}

void CXTPReportHyperlinkStyle::SetTextDecoration(XTPReportTextDecoration textDecoration)
{
	m_textDecoration = textDecoration;
}


IMPLEMENT_DYNAMIC(CXTPReportHyperlink, CCmdTarget)
IMPLEMENT_DYNAMIC(CXTPReportHyperlinks, CCmdTarget)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPReportHyperlink::CXTPReportHyperlink(int nHyperTextBegin, int nHyperTextLen)
	: m_nHyperTextBegin(nHyperTextBegin),
	m_nHyperTextLen(nHyperTextLen),
	m_rcHyperSpot(0, 0, 0, 0)
{

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPReportHyperlink::~CXTPReportHyperlink()
{

}

void CXTPReportHyperlink::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Int(pPX, _T("HyperTextBegin"), m_nHyperTextBegin);
	PX_Int(pPX, _T("HyperTextLen"), m_nHyperTextLen);
}

CXTPReportHyperlinks::CXTPReportHyperlinks()
{

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPReportHyperlinks::~CXTPReportHyperlinks()
{
	RemoveAll();
}

void CXTPReportHyperlinks::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)GetSize())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPReportHyperlink* pItem = GetAt(nIndex);
	if (pItem)
		pItem->InternalRelease();

	TBase::RemoveAt(nIndex);
}

void CXTPReportHyperlinks::RemoveAll()
{
	for (int nItem = (int)GetSize() - 1; nItem >= 0; nItem--)
	{
		CXTPReportHyperlink* pItem = GetAt(nItem);
		if (pItem)
			pItem->InternalRelease();
	}
	TBase::RemoveAll();
}

void CXTPReportHyperlinks::DoPropExchange(CXTPPropExchange* pPX)
{
	int nCount = (int)GetSize();

	CXTPPropExchangeEnumeratorPtr pEnumItems(pPX->GetEnumerator(_T("Hyperlink")));

	if (pPX->IsStoring())
	{
		POSITION posItem = pEnumItems->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPReportHyperlink* pItem = GetAt(i);
			ASSERT(pItem);
			if (pItem)
			{
				CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
				pItem->DoPropExchange(&secItem);
			}
		}
	}
	else
	{
		POSITION posItem = pEnumItems->GetPosition();
		while (posItem)
		{
			CXTPReportHyperlink* pItem = new CXTPReportHyperlink();
			if (!pItem)
			{
				return;
			}
			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			pItem->DoPropExchange(&secItem);

			Add(pItem);
		}
	}

}

void CXTPReportHyperlinks::CopyFrom(CXTPReportHyperlinks* pSrc)
{
	if (pSrc == this)
		return;

	RemoveAll();

	if (!pSrc)
		return;

	int nCount = pSrc->GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CXTPReportHyperlink* pHlnk = pSrc->GetAt(i);
		if (pHlnk)
		{
			pHlnk->InternalAddRef();
			Add(pHlnk);
		}
	}
}

#ifdef _XTP_ACTIVEX


// {253A9CA8-51EA-4c67-AB9B-E6B1A446953D}
static const GUID IID_IReportHyperlink =
{ 0x253a9ca8, 0x51ea, 0x4c67, { 0xab, 0x9b, 0xe6, 0xb1, 0xa4, 0x46, 0x95, 0x3d } };

BEGIN_INTERFACE_MAP(CXTPReportHyperlink, CCmdTarget)
	INTERFACE_PART(CXTPReportHyperlink, IID_IReportHyperlink, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportHyperlink, IID_IReportHyperlink)

BEGIN_DISPATCH_MAP(CXTPReportHyperlink, CCmdTarget)
	DISP_PROPERTY_ID(CXTPReportHyperlink, "HyperTextBegin", 1, m_nHyperTextBegin,   VT_I4)
	DISP_PROPERTY_ID(CXTPReportHyperlink, "HyperTextLen",   2, m_nHyperTextLen,     VT_I4)
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////

// {E45E24E1-0C85-41dd-8A5B-DDF486AF03F8}
static const GUID IID_IReportHyperlinks =
{ 0xe45e24e1, 0xc85, 0x41dd, { 0x8a, 0x5b, 0xdd, 0xf4, 0x86, 0xaf, 0x3, 0xf8 } };

BEGIN_INTERFACE_MAP(CXTPReportHyperlinks, CCmdTarget)
	INTERFACE_PART(CXTPReportHyperlinks, IID_IReportHyperlinks, Dispatch)
	//INTERFACE_PART(CXTPReportHyperlinks, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportHyperlinks, IID_IReportHyperlinks)
IMPLEMENT_ENUM_VARIANT(CXTPReportHyperlinks)

BEGIN_DISPATCH_MAP(CXTPReportHyperlinks, CCmdTarget)
	DISP_FUNCTION_ID(CXTPReportHyperlinks, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportHyperlinks, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPReportHyperlinks, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPReportHyperlinks, "AddHyperlink",  2, OleAddHyperlink, VT_DISPATCH, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPReportHyperlinks, "Remove",        3, OleRemove,       VT_EMPTY,    VTS_I4)
	DISP_FUNCTION_ID(CXTPReportHyperlinks, "RemoveAll",     4, OleRemoveAll,    VT_EMPTY,    VTS_NONE)
END_DISPATCH_MAP()

LPDISPATCH CXTPReportHyperlinks::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetSize())
	{
		CXTPReportHyperlink* pItem = GetAt(nIndex);
		return pItem ? pItem->GetIDispatch(TRUE) : NULL;
	}
	AfxThrowOleException(DISP_E_BADINDEX);
	return NULL;
}

LPDISPATCH CXTPReportHyperlinks::OleAddHyperlink(long nHyperTextBegin, long nHyperTextLen)
{
	CXTPReportHyperlink* pItem = new CXTPReportHyperlink(nHyperTextBegin, nHyperTextLen);
	if (!pItem)
		return NULL;

	Add(pItem);
	return pItem->GetIDispatch(TRUE);
}

#endif
