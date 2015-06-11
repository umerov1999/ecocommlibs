// XTPFlowGraphPages.cpp : implementation of the CXTPFlowGraphPages class.
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

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>
#include "Common/XTPPropExchange.h"

#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphPages.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphUndoManager.h"


CXTPFlowGraphPages::CXTPFlowGraphPages(CXTPFlowGraphControl* pControl)
{
	m_pControl = pControl;


#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

CXTPFlowGraphPages::~CXTPFlowGraphPages()
{
	RemoveAll();
}

void CXTPFlowGraphPages::RemoveAll()
{
	if (m_arrPages.GetSize() == 0)
		return;

	for (int i = 0; i < m_arrPages.GetSize(); i++)
	{
		m_arrPages[i]->OnRemoved();
		m_arrPages[i]->m_pControl = NULL;
		m_arrPages[i]->InternalRelease();
	}
	m_arrPages.RemoveAll();

	m_pControl->GetUndoManager()->Clear();
	m_pControl->OnGraphChanged();
}


CXTPFlowGraphPage* CXTPFlowGraphPages::AddPage(CXTPFlowGraphPage* pPage)
{
	m_arrPages.Add(pPage);
	pPage->m_pControl = m_pControl;

	return pPage;
}

CXTPFlowGraphPage* CXTPFlowGraphPages::FindPage(int nId) const
{
	for (int i = 0; i < m_arrPages.GetSize(); i++)
	{
		if (m_arrPages[i]->GetID() == nId)
			return m_arrPages[i];
	}
	return NULL;
}


void CXTPFlowGraphPages::Remove(CXTPFlowGraphPage* pPage)
{
	for (int i = 0; i < m_arrPages.GetSize(); i++)
	{
		if (m_arrPages[i] == pPage)
		{
			RemoveAt(i);
			return;
		}
	}
}

void CXTPFlowGraphPages::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrPages.GetSize())
		return;

	CXTPFlowGraphPage* pPage = m_arrPages[nIndex];

	m_arrPages.RemoveAt(nIndex);

	pPage->OnRemoved();
	pPage->m_pControl = NULL;
	pPage->InternalRelease();

	m_pControl->GetUndoManager()->Clear();
	m_pControl->OnGraphChanged();
}

void CXTPFlowGraphPages::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("Page")));

	if (pPX->IsStoring())
	{
		int nCount = (int)GetCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPFlowGraphPage* pPage = GetAt(i);
			ASSERT(pPage);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pPage, RUNTIME_CLASS(CXTPFlowGraphPage));

			BOOL bActive = m_pControl->GetActivePage() == pPage;
			PX_Bool(&sec, _T("Active"), bActive, FALSE);
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		CXTPFlowGraphPage* pActivePage = NULL;

		while (pos)
		{
			CXTPFlowGraphPage* pPage = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));

			if (!sec->ExchangeObjectInstance((CObject*&)pPage, RUNTIME_CLASS(CXTPFlowGraphPage)))
				AfxThrowArchiveException(CArchiveException::badClass);

			pPage->m_pControl = m_pControl;
			pPage->DoPropExchange(&sec);

			m_arrPages.Add(pPage);

			BOOL bActive = FALSE;
			PX_Bool(&sec, _T("Active"), bActive, FALSE);

			if (bActive)
				pActivePage = pPage;
		}

		m_pControl->SetActivePage(pActivePage);
	}
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPFlowGraphPages, CCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "Page", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphPages, "AddPage", 2, OleAddPage, VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphPages, "RemoveAll", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphPages, "FindPage", 5, OleFindPage, VT_DISPATCH, VTS_I4)
END_DISPATCH_MAP()

// {E4353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphPages =
{ 0xe4353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphPages, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphPages, IID_IFlowGraphPages, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphPages, IID_IFlowGraphPages)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphPages)


LPDISPATCH CXTPFlowGraphPages::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphPages::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPFlowGraphPages::OleAddPage()
{
	return XTPGetDispatch(AddPage(new CXTPFlowGraphPage()));
}

LPDISPATCH CXTPFlowGraphPages::OleFindPage(int nId)
{
	return XTPGetDispatch(FindPage(nId));
}

#endif
