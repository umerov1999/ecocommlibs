// XTPFlowGraphNodes.cpp : implementation of the CXTPFlowGraphNodes class.
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

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphConnections.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphUndoManager.h"


CXTPFlowGraphNodes::CXTPFlowGraphNodes(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;

}

CXTPFlowGraphNodes::~CXTPFlowGraphNodes()
{
	RemoveAll();
}


void CXTPFlowGraphNodes::RemoveAll()
{
	if (m_arrNodes.GetSize() == 0)
		return;

	CXTPFlowGraphUndoManager* pUndoManager = m_pPage->GetControl()->GetUndoManager();

	pUndoManager->StartGroup();

	m_pPage->GetConnections()->RemoveAll();

	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		CXTPFlowGraphNode* pNode = m_arrNodes[i];

		pUndoManager->AddUndoCommand(new CXTPFlowGraphUndoDeleteNodeCommand(pNode));

		pNode->OnRemoved();
		pNode->m_pPage = NULL;
		pNode->InternalRelease();
	}
	m_arrNodes.RemoveAll();

	pUndoManager->EndGroup();

	m_pPage->OnGraphChanged();
}

CXTPFlowGraphNode* CXTPFlowGraphNodes::AddNode(CXTPFlowGraphNode* pNode)
{
	m_arrNodes.Add(pNode);
	pNode->m_pPage = m_pPage;

	m_pPage->GetControl()->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoAddNodeCommand(pNode));

	m_pPage->OnGraphChanged();

	return pNode;
}

void CXTPFlowGraphNodes::MoveToFront(CXTPFlowGraphNode* pNode)
{
	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		if (m_arrNodes[i] == pNode)
		{
			m_arrNodes.RemoveAt(i);
			m_arrNodes.Add(pNode);

			m_pPage->OnGraphChanged();
			return;
		}
	}
}

void CXTPFlowGraphNodes::Remove(CXTPFlowGraphNode* pNode)
{
	if (pNode->GetPage() != m_pPage)
		return;

	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		if (m_arrNodes[i] == pNode)
		{
			RemoveAt(i);
			return;
		}
	}
}

CXTPFlowGraphNode* CXTPFlowGraphNodes::FindNode(int nId) const
{
	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		if (m_arrNodes[i]->GetID() == nId)
			return m_arrNodes[i];
	}
	return NULL;
}

void CXTPFlowGraphNodes::RemoveAt(int nIndex)
{
	CXTPFlowGraphNode* pNode = GetAt(nIndex);
	if (!pNode)
		return;

	CXTPFlowGraphUndoManager* pUndoManager = m_pPage->GetControl()->GetUndoManager();

	pUndoManager->StartGroup();

	m_arrNodes.RemoveAt(nIndex);
	pNode->OnRemoved();

	pUndoManager->AddUndoCommand(new CXTPFlowGraphUndoDeleteNodeCommand(pNode));
	pUndoManager->EndGroup();


	pNode->m_pPage = NULL;
	pNode->InternalRelease();

	m_pPage->OnGraphChanged();
}

void CXTPFlowGraphNodes::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("Node")));

	if (pPX->IsStoring())
	{
		int nCount = (int)GetCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPFlowGraphNode* pNode = GetAt(i);
			ASSERT(pNode);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pNode, RUNTIME_CLASS(CXTPFlowGraphNode));
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		while (pos)
		{
			CXTPFlowGraphNode* pNode = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));

			if (!sec->ExchangeObjectInstance((CObject*&)pNode, RUNTIME_CLASS(CXTPFlowGraphNode)))
				AfxThrowArchiveException(CArchiveException::badClass);

			pNode->m_pPage = m_pPage;
			pNode->DoPropExchange(&sec);

			m_arrNodes.Add(pNode);
		}
	}

}


