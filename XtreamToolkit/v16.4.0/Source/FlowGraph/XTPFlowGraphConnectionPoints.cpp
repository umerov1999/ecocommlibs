// XTPFlowGraphConnectionPoints.cpp : implementation of the CXTPFlowGraphConnectionPoints class.
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
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

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

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
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

	// when adding a connection point with type = xtpFlowGraphPointInputAndOutput
	// 2 connection points are added actually, but there should only one undo task for it
	// when ondoing this task the second connection point will be remove either
	// see CXTPFlowGraphConnectionPoints::Remove

	XTPFlowGraphConnectionPointType type = pConnectionPoint->GetType();
	BOOL bAddUndoTask = TRUE;

	if ((type == xtpFlowGraphPointInput || type == xtpFlowGraphPointOutput) )
	{
		CXTPFlowGraphConnectionPoint *pSameNamePoint =
			m_pNode->GetConnectionPoints()->FindConnectionPoint(pConnectionPoint->GetName(), (type == xtpFlowGraphPointInput) ? xtpFlowGraphPointOutput : xtpFlowGraphPointInput );

		if (pSameNamePoint)
			bAddUndoTask = FALSE;
	}

	if (m_pNode->IsCustomMarkup())
		bAddUndoTask = FALSE; // not undo/redo operations for connection points in custom markup nodes

	if (bAddUndoTask)
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

	BOOL bSkipUndo = FALSE;

	for (int i = (int)m_arrConnectionPoints.GetSize() - 1; i >= 0; i--)
	{
		CXTPFlowGraphConnectionPoint* pConnectionPoint = m_arrConnectionPoints[i];

		// It will remove all Connections associated with Connection Point
		pConnectionPoint->OnRemoved();

		if ( (i-1) >=0 ) // there's a point before
		{
			CXTPFlowGraphConnectionPoint* pNextPoint = m_arrConnectionPoints[i-1];

			if (pNextPoint->GetName() == pConnectionPoint->GetName())
			{
				if (
					(pNextPoint->GetType() == xtpFlowGraphPointInput && pConnectionPoint->GetType() == xtpFlowGraphPointOutput)
					||
					(pNextPoint->GetType() == xtpFlowGraphPointOutput && pConnectionPoint->GetType() ==xtpFlowGraphPointInput ))
				{
					pConnectionPoint->m_bInputOutPair = TRUE;

					bSkipUndo = TRUE;
				}
			}
		}

		if (bSkipUndo && !pConnectionPoint->m_bInputOutPair)
		{
			bSkipUndo = FALSE;
		}
		else
		{
			if (m_pNode->IsCustomMarkup())
				bSkipUndo = TRUE; // not undo/redo operations for connection points in custom markup nodes

			pUndoManager->AddUndoCommand(new CXTPFlowGraphUndoDeleteConnectionPointCommand(pConnectionPoint));
		}

		pConnectionPoint->m_pNode = NULL;
		pConnectionPoint->m_nConnectionPointIndex = -1;

		pConnectionPoint->InternalRelease();
	}
	m_arrConnectionPoints.RemoveAll();

	pUndoManager->EndGroup();


	m_pNode->OnGraphChanged();
}

void CXTPFlowGraphConnectionPoints::Remove(CXTPFlowGraphConnectionPoint* pConnectionPoint, BOOL bRemovePair)
{
	if (!pConnectionPoint || pConnectionPoint->GetNode() != m_pNode)
		return;

	ASSERT(pConnectionPoint->m_nConnectionPointIndex != -1);

	if (pConnectionPoint->m_nConnectionPointIndex != -1)
	{
		//ASSERT(m_arrConnectionPoints[pConnectionPoint->m_nConnectionPointIndex] == pConnectionPoint);

		RemoveAt(pConnectionPoint->m_nConnectionPointIndex, bRemovePair);
	}
}

void CXTPFlowGraphConnectionPoints::RemoveAt(int nIndex, BOOL bRemovePair /*= TRUE*/, BOOL bSkipUndoTask /*= FALSE*/)
{
	if (nIndex < 0 || nIndex >= m_arrConnectionPoints.GetSize())
		return;

	CXTPFlowGraphConnectionPoint* pConnectionPoint = m_arrConnectionPoints[nIndex];

	CXTPFlowGraphUndoManager* pUndoManager = m_pNode->GetControl()->GetUndoManager();

	if (m_pNode->IsCustomMarkup())
		bSkipUndoTask = TRUE; // not undo/redo operations for connection points in custom markup nodes

	if (!bSkipUndoTask)
		pUndoManager->StartGroup();

	//////////////////////////////////////////////////////////////////////////

	XTPFlowGraphConnectionPointType type = pConnectionPoint->GetType();

	CXTPFlowGraphConnectionPoint *pSameNamePoint = NULL;

	// if the node has both input and ouput points with the same name,
	// we should remove the second point as well
	if (bRemovePair && (type == xtpFlowGraphPointInput || type == xtpFlowGraphPointOutput) )
	{
		pSameNamePoint =
			FindConnectionPoint(pConnectionPoint->GetName(), (type == xtpFlowGraphPointInput) ? xtpFlowGraphPointOutput : xtpFlowGraphPointInput );

		if (pSameNamePoint)
			pConnectionPoint->m_bInputOutPair = TRUE; // for undo command!
	}

	//////////////////////////////////////////////////////////////////////////

	pConnectionPoint->OnRemoved();

	m_arrConnectionPoints.RemoveAt(nIndex);

	for (int i = nIndex; i < (int)m_arrConnectionPoints.GetSize(); i++)
	{
		m_arrConnectionPoints[i]->m_nConnectionPointIndex = i;
	}

	if (!bSkipUndoTask)
	{
		pUndoManager->AddUndoCommand(new CXTPFlowGraphUndoDeleteConnectionPointCommand(pConnectionPoint));

		pUndoManager->EndGroup();
	}

	pConnectionPoint->m_pNode = NULL;
	pConnectionPoint->m_nConnectionPointIndex = -1;

	pConnectionPoint->InternalRelease();

	m_pNode->OnGraphChanged();

	//////////////////////////////////////////////////////////////////////////

	if (pSameNamePoint)
	{
		ASSERT(pSameNamePoint->m_nConnectionPointIndex != -1);

		if (pSameNamePoint->m_nConnectionPointIndex != -1)
		{
			RemoveAt(pSameNamePoint->m_nConnectionPointIndex, FALSE, TRUE); // Skip Undo TASK!!!
		}
	}
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

CXTPFlowGraphConnectionPoint* CXTPFlowGraphConnectionPoints::FindConnectionPoint(LPCTSTR lpszName, XTPFlowGraphConnectionPointType type /*= xtpFlowGraphPointNone*/) const
{
	for (int i = 0; i < m_arrConnectionPoints.GetSize(); i++)
	{
		if (m_arrConnectionPoints[i]->GetName() == lpszName)
		{
			if (type != xtpFlowGraphPointNone) // check type
			{
				if (m_arrConnectionPoints[i]->GetType() != type)
					continue;
			}

			return m_arrConnectionPoints[i];
		}
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

CXTPFlowGraphConnectionPoint* CXTPFlowGraphConnectionPoints::FindConnectionPointByCaption(LPCTSTR lpszCaption, XTPFlowGraphConnectionPointType type /*= xtpFlowGraphPointNone*/) const
{
	for (int i = 0; i < m_arrConnectionPoints.GetSize(); i++)
	{
		if (m_arrConnectionPoints[i]->GetName() == lpszCaption)
		{
			if (type != xtpFlowGraphPointNone) // check type
			{
				if (m_arrConnectionPoints[i]->GetType() != type)
					continue;
			}

			return m_arrConnectionPoints[i];
		}
	}
	return NULL;
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPFlowGraphConnectionPoints, CCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoints, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoints, "ConnectionPoint", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoints, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoints, "RemoveAll", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoints, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)

	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoints, "FindConnectionPoint", 5, OleFindConnectionPoint, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoints, "FindConnectionPointByCaption", 6, OleFindConnectionPointByCaption, VT_DISPATCH, VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPFlowGraphConnectionPoints, "FindConnectionPointByName", 7, OleFindConnectionPointByName, VT_DISPATCH, VTS_BSTR VTS_VARIANT)

END_DISPATCH_MAP()

// {04353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphConnectionPoints =
{ 0x04353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphConnectionPoints, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphConnectionPoints, IID_IFlowGraphConnectionPoints, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphConnectionPoints, IID_IFlowGraphConnectionPoints)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphConnectionPoints)


LPDISPATCH CXTPFlowGraphConnectionPoints::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphConnectionPoints::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPFlowGraphConnectionPoints::OleFindConnectionPoint(int nId)
{
	return XTPGetDispatch(FindConnectionPoint(nId));
}

LPDISPATCH CXTPFlowGraphConnectionPoints::OleFindConnectionPointByCaption(LPCTSTR lpszCaption, const VARIANT& vtType)
{
	XTPFlowGraphConnectionPointType nType = xtpFlowGraphPointNone;

	// more info at
	// http://support.microsoft.com/kb/154039

	if (vtType.scode != DISP_E_PARAMNOTFOUND && vtType.vt != VT_ERROR) // additional parameter
		nType = (XTPFlowGraphConnectionPointType)vtType.uintVal;

	LPDISPATCH test = XTPGetDispatch(FindConnectionPointByCaption(lpszCaption, nType));

	return XTPGetDispatch(FindConnectionPointByCaption(lpszCaption, nType));
}

LPDISPATCH CXTPFlowGraphConnectionPoints::OleFindConnectionPointByName(LPCTSTR lpszName, const VARIANT& vtType)
{
	XTPFlowGraphConnectionPointType nType = xtpFlowGraphPointNone;

	if (vtType.scode != DISP_E_PARAMNOTFOUND && vtType.vt != VT_ERROR) // additional parameter
		nType = (XTPFlowGraphConnectionPointType)vtType.uintVal;

	return XTPGetDispatch(FindConnectionPoint(lpszName, nType));
}

#endif
