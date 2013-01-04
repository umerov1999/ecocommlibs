// XTPFlowGraphConnectionPoints.cpp : implementation of the CXTPFlowGraphConnectionPoints class.
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
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphConnections.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphUndoManager.h"
#include "XTPFlowGraphControl.h"


CXTPFlowGraphConnectionPoints::CXTPFlowGraphConnectionPoints(CXTPFlowGraphNode* pNode)
{
	m_pNode = pNode;

}

CXTPFlowGraphConnectionPoints::~CXTPFlowGraphConnectionPoints()
{
	RemoveAll();
}

CXTPFlowGraphConnectionPoint* CXTPFlowGraphConnectionPoints::AddConnectionPoint(CXTPFlowGraphConnectionPoint* pConnectionPoint, int nIndex)
{
	ASSERT (pConnectionPoint->m_nConnectionPointIndex == -1 && pConnectionPoint->m_pNode == NULL);

	if (pConnectionPoint->m_nConnectionPointIndex != -1)
		return pConnectionPoint;

	if (nIndex == -1 || nIndex > m_arrConnectionPoints.GetSize())
		nIndex = (int)m_arrConnectionPoints.GetSize();

	m_arrConnectionPoints.InsertAt(nIndex, pConnectionPoint);

	for (; nIndex < (int)m_arrConnectionPoints.GetSize(); nIndex++)
	{
		m_arrConnectionPoints[nIndex]->m_nConnectionPointIndex = nIndex;
	}

	pConnectionPoint->m_pNode = m_pNode;

	m_pNode->GetControl()->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoAddConnectionPointCommand(pConnectionPoint));

	m_pNode->OnGraphChanged();

	return pConnectionPoint;
}

void CXTPFlowGraphConnectionPoints::RemoveAll()
{
	if (m_arrConnectionPoints.GetSize() == 0)
		return;

	CXTPFlowGraphUndoManager* pUndoManager = m_pNode->GetControl()->GetUndoManager();

	pUndoManager->StartGroup();

	for (int i = (int)m_arrConnectionPoints.GetSize() - 1; i >= 0; i--)
	{
		CXTPFlowGraphConnectionPoint* pConnectionPoint = m_arrConnectionPoints[i];

		// It will remove all Connections associated with Connection Point
		pConnectionPoint->OnRemoved();

		pUndoManager->AddUndoCommand(new CXTPFlowGraphUndoDeleteConnectionPointCommand(pConnectionPoint));

		pConnectionPoint->m_pNode = NULL;
		pConnectionPoint->m_nConnectionPointIndex = -1;

		pConnectionPoint->InternalRelease();
	}
	m_arrConnectionPoints.RemoveAll();

	pUndoManager->EndGroup();


	m_pNode->OnGraphChanged();
}

void CXTPFlowGraphConnectionPoints::Remove(CXTPFlowGraphConnectionPoint* pConnectionPoint)
{
	if (!pConnectionPoint || pConnectionPoint->GetNode() != m_pNode)
		return;

	ASSERT(pConnectionPoint->m_nConnectionPointIndex != -1);
	ASSERT(m_arrConnectionPoints[pConnectionPoint->m_nConnectionPointIndex] == pConnectionPoint);

	RemoveAt(pConnectionPoint->m_nConnectionPointIndex);
}

void CXTPFlowGraphConnectionPoints::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrConnectionPoints.GetSize())
		return;

	CXTPFlowGraphConnectionPoint* pConnectionPoint = m_arrConnectionPoints[nIndex];

	CXTPFlowGraphUndoManager* pUndoManager = m_pNode->GetControl()->GetUndoManager();

	pUndoManager->StartGroup();

	pConnectionPoint->OnRemoved();

	m_arrConnectionPoints.RemoveAt(nIndex);

	for (int i = nIndex; i < (int)m_arrConnectionPoints.GetSize(); i++)
	{
		m_arrConnectionPoints[i]->m_nConnectionPointIndex = i;
	}

	pUndoManager->AddUndoCommand(new CXTPFlowGraphUndoDeleteConnectionPointCommand(pConnectionPoint));
	pUndoManager->EndGroup();

	pConnectionPoint->m_pNode = NULL;
	pConnectionPoint->m_nConnectionPointIndex = -1;

	pConnectionPoint->InternalRelease();

	m_pNode->OnGraphChanged();
}

void CXTPFlowGraphConnectionPoints::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeEnumeratorPtr pEnumRecords(pPX->GetEnumerator(_T("ConnectionPoint")));

	if (pPX->IsStoring())
	{
		int nCount = (int)GetCount();
		POSITION pos = pEnumRecords->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPFlowGraphConnectionPoint* pConnectionPoint = GetAt(i);
			ASSERT(pConnectionPoint);

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));
			PX_Object(&sec, pConnectionPoint, RUNTIME_CLASS(CXTPFlowGraphConnectionPoint));
		}
	}
	else
	{
		RemoveAll();

		POSITION pos = pEnumRecords->GetPosition();

		while (pos)
		{
			CXTPFlowGraphConnectionPoint* pConnectionPoint = NULL;

			CXTPPropExchangeSection sec(pEnumRecords->GetNext(pos));

			if (!sec->ExchangeObjectInstance((CObject*&)pConnectionPoint, RUNTIME_CLASS(CXTPFlowGraphConnectionPoint)))
				AfxThrowArchiveException(CArchiveException::badClass);

			pConnectionPoint->m_pNode = m_pNode;
			pConnectionPoint->m_nConnectionPointIndex = (int)m_arrConnectionPoints.Add(pConnectionPoint);

			pConnectionPoint->DoPropExchange(&sec);
		}
	}

}

CXTPFlowGraphConnectionPoint* CXTPFlowGraphConnectionPoints::FindConnectionPoint(LPCTSTR lpszCaption) const
{
	for (int i = 0; i < m_arrConnectionPoints.GetSize(); i++)
	{
		if (m_arrConnectionPoints[i]->GetCaption() == lpszCaption)
			return m_arrConnectionPoints[i];
	}
	return NULL;
}

CXTPFlowGraphConnectionPoint* CXTPFlowGraphConnectionPoints::FindConnectionPoint(int nId) const
{
	for (int i = 0; i < m_arrConnectionPoints.GetSize(); i++)
	{
		if (m_arrConnectionPoints[i]->GetID() == nId)
			return m_arrConnectionPoints[i];
	}
	return NULL;
}


