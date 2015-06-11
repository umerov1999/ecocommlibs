// XTPFlowGraphNodes.cpp : implementation of the CXTPFlowGraphNodes class.
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPResourceManager.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodeCustom.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphConnections.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphUndoManager.h"
#include "XTPFlowGraphSelectedElements.h"

CXTPFlowGraphNodes::CXTPFlowGraphNodes(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
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

	m_pPage->GetSelection()->Clear();
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

	pNode->SetDefaultMarkup();

	m_pPage->GetControl()->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoAddNodeCommand(pNode));

	m_pPage->OnGraphChanged();

	return pNode;
}

CXTPFlowGraphNodeCustom* CXTPFlowGraphNodes::AddCustomNode(CXTPFlowGraphNodeCustom* pNode /*= NULL */)
{
	if (!pNode)
		pNode = new CXTPFlowGraphNodeCustom();

	return static_cast<CXTPFlowGraphNodeCustom *>(AddNode(pNode));
}

CXTPFlowGraphNodeCustom* CXTPFlowGraphNodes::AddResourceNode(UINT uResShapeID)
{
	if (m_pPage)
		return m_pPage->GetControl()->AddCustomNodeFromToolkitResource(uResShapeID);

	return NULL;
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
			m_pPage->GetSelection()->Remove(pNode); // should delete from selection first

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

			pNode->SetDefaultMarkup();

			pNode->DoPropExchange(&sec);

			m_arrNodes.Add(pNode);
		}
	}

}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPFlowGraphNodes, CCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "Node", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "AddNode", 2, OleAddNode, VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "RemoveAll", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "FindNode", 5, OleFindNode, VT_DISPATCH, VTS_I4)

	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "AddCustomNode", 10, OleAddCustomNode, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodes, "AddResourceNode", 11, OleAddResourceNode, VT_DISPATCH, VTS_I4)

END_DISPATCH_MAP()

// {02353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphNodes =
{ 0x02353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphNodes, CCmdTarget)
INTERFACE_PART(CXTPFlowGraphNodes, IID_IFlowGraphNodes, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphNodes, IID_IFlowGraphNodes)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphNodes)


LPDISPATCH CXTPFlowGraphNodes::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphNodes::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPFlowGraphNodes::OleAddNode()
{
	return XTPGetDispatch(AddNode(new CXTPFlowGraphNode()));
}

LPDISPATCH CXTPFlowGraphNodes::OleAddCustomNode()
{
	return XTPGetDispatch(AddCustomNode(new CXTPFlowGraphNodeCustom()));
};

LPDISPATCH CXTPFlowGraphNodes::OleAddResourceNode(int nResShapeID)
{
	return XTPGetDispatch(AddResourceNode(nResShapeID));
};

LPDISPATCH CXTPFlowGraphNodes::OleFindNode(int nId)
{
	return XTPGetDispatch(FindNode(nId));
}


#endif
