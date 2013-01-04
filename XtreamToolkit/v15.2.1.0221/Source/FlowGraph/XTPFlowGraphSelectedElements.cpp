// XTPFlowGraphSelectedElements.cpp : implementation of the CXTPFlowGraphSelectedElements class.
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

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphSelectedElements.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphMessages.h"


CXTPFlowGraphSelectedElements::CXTPFlowGraphSelectedElements(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;


}

CXTPFlowGraphSelectedElements::~CXTPFlowGraphSelectedElements()
{
	Clear();
}

void CXTPFlowGraphSelectedElements::Clear()
{
	if (m_arrSelectedElements.GetSize() == 0)
		return;

	for (int i = 0; i < m_arrSelectedElements.GetSize(); i++)
	{
		m_arrSelectedElements[i]->m_bSelected = FALSE;
		m_arrSelectedElements[i]->InternalRelease();
	}
	m_arrSelectedElements.RemoveAll();

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

	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}

void CXTPFlowGraphSelectedElements::AddSelection(CXTPFlowGraphElement* pElement)
{
	if (!pElement)
		return;

	if (pElement->IsSelected())
		return;

	m_arrSelectedElements.Add(pElement);
	pElement->InternalAddRef();
	pElement->m_bSelected = TRUE;


	m_pPage->GetControl()->RedrawControl();

	m_pPage->GetControl()->SendNotifyMessage(XTP_FGN_SELECTIONCHANGED);
}


