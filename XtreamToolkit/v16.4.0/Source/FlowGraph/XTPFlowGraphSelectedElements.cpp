// XTPFlowGraphSelectedElements.cpp : implementation of the CXTPFlowGraphSelectedElements class.
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

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphSelectedElements.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphMessages.h"
#include "XTPFlowGraphPaintManager.h"

CXTPFlowGraphSelectedElements::CXTPFlowGraphSelectedElements(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

CXTPFlowGraphSelectedElements::~CXTPFlowGraphSelectedElements()
{
	Clear();
}

void CXTPFlowGraphSelectedElements::Clear()
{
	if (m_arrSelectedElements.GetSize() == 0)
		return;

	BOOL bRecalcLayout = TRUE; // always recalc layout
		//(m_pPage->GetControl()->GetPaintManager()->m_bRecalcConnectionLayoutWhenNeeded && !m_pPage->GetControl()->GetPaintManager()->m_bAlwaysDrawConnectionPoints);

	for (int i = 0; i < m_arrSelectedElements.GetSize(); i++)
	{
		m_arrSelectedElements[i]->m_bSelected = FALSE;
		m_arrSelectedElements[i]->InternalRelease();

		if (bRecalcLayout)
			m_arrSelectedElements[i]->m_bDirty = TRUE;
	}
	m_arrSelectedElements.RemoveAll();

	if (bRecalcLayout)
		m_pPage->GetControl()->Reposition();

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}


void CXTPFlowGraphSelectedElements::SetSelection(CXTPFlowGraphElement* pElement)
{
	if (m_arrSelectedElements.GetSize() == 1 && m_arrSelectedElements[0] == pElement)
		return;

	Clear();

	m_arrSelectedElements.Add(pElement);
	pElement->InternalAddRef();
	pElement->m_bSelected = TRUE;

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}

void CXTPFlowGraphSelectedElements::Remove(CXTPFlowGraphElement* pElement)
{
	if (!pElement)
		return;

	if (!pElement->IsSelected())
		return;

	for (int i = 0; i < m_arrSelectedElements.GetSize(); i++)
	{
		if (m_arrSelectedElements[i] == pElement)
		{

			RemoveAt(i);
			return;
		}
	}
}

void CXTPFlowGraphSelectedElements::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrSelectedElements.GetSize())
		return;

	CXTPFlowGraphElement* pElement = m_arrSelectedElements[nIndex];

	m_arrSelectedElements.RemoveAt(nIndex);

	pElement->m_bSelected = FALSE;
	pElement->InternalRelease();

	BOOL bRecalcLayout = TRUE; // always recalc layout

	if (bRecalcLayout)
	{
		pElement->m_bDirty = TRUE;

		m_pPage->GetControl()->Reposition();
	}

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}

void CXTPFlowGraphSelectedElements::AddSelection(CXTPFlowGraphElement* pElement)
{
	if (!pElement)
		return;

	if (pElement->IsSelected())
		return;

	BOOL bRecalcLayout = TRUE; // always recalc layout
		//(m_pPage->GetControl()->GetPaintManager()->m_bRecalcConnectionLayoutWhenNeeded && !m_pPage->GetControl()->GetPaintManager()->m_bAlwaysDrawConnectionPoints);

	m_arrSelectedElements.Add(pElement);
	pElement->InternalAddRef();
	pElement->m_bSelected = TRUE;

	if (bRecalcLayout)
	{
		pElement->m_bDirty = TRUE;

		m_pPage->GetControl()->Reposition();
	}

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPFlowGraphSelectedElements, CCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "Page", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "AddSelection", 2, OleAddSelection, VT_EMPTY, VTS_DISPATCH)

	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "RemoveAll", 3, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphSelectedElements, "Remove", 5, OleRemove, VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()

// {11353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphSelectedElements =
{ 0x11353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphSelectedElements, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphSelectedElements, IID_IFlowGraphSelectedElements, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphSelectedElements, IID_IFlowGraphSelectedElements)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphSelectedElements)


LPDISPATCH CXTPFlowGraphSelectedElements::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphSelectedElements::OleGetItemCount()
{
	return GetCount();
}

void CXTPFlowGraphSelectedElements::OleAddSelection(LPDISPATCH lpDisp)
{
	CXTPFlowGraphElement* pElement = DYNAMIC_DOWNCAST(CXTPFlowGraphElement, CCmdTarget::FromIDispatch(lpDisp));
	AddSelection(pElement);
}

void CXTPFlowGraphSelectedElements::OleRemove(LPDISPATCH lpDisp)
{
	CXTPFlowGraphElement* pElement = DYNAMIC_DOWNCAST(CXTPFlowGraphElement, CCmdTarget::FromIDispatch(lpDisp));
	Remove(pElement);
}


#endif
