// XTPFlowGraphPage.cpp : implementation of the CXTPFlowGraphPage class.
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
#include <math.h>

#include "Common/XTPPropExchange.h"

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphPages.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphNodeGroups.h"
#include "XTPFlowGraphNodeGroups.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnections.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphSelectedElements.h"
#include "XTPFlowGraphUndoManager.h"
#include "XTPFlowGraphMessages.h"

CLIPFORMAT CXTPFlowGraphPage::m_cfNode = (CLIPFORMAT)::RegisterClipboardFormat(_T("FlowGraphNode"));


IMPLEMENT_SERIAL(CXTPFlowGraphPage, CCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)


CXTPFlowGraphPage::CXTPFlowGraphPage()
{
	m_pControl = NULL;
	m_rcPage.SetRectEmpty();

	m_ptScrollOffset = CPoint(0, 0);
	m_nId = 0;

	m_pNodes = new CXTPFlowGraphNodes(this);

	m_pGroups = new CXTPFlowGraphNodeGroups(this);

	m_pConnections = new CXTPFlowGraphConnections(this);

	m_dZoomLevel = 1.0;

	m_pSelectedElements = new CXTPFlowGraphSelectedElements(this);

	m_pFirstVisibleNode = NULL;
	m_pFirstVisibleConnection = NULL;

}

CXTPFlowGraphPage::~CXTPFlowGraphPage()
{

	if (m_pSelectedElements)
	{
		m_pSelectedElements->Clear();
		m_pSelectedElements->InternalRelease();
		m_pSelectedElements = NULL;
	}


	if (m_pGroups)
	{
		m_pGroups->RemoveAll();
		m_pGroups->InternalRelease();
		m_pGroups = NULL;
	}

	if (m_pConnections)
	{
		m_pConnections->RemoveAll();
		m_pConnections->InternalRelease();
		m_pConnections = NULL;
	}

	if (m_pNodes)
	{
		m_pNodes->RemoveAll();
		m_pNodes->InternalRelease();
		m_pNodes = NULL;
	}
}

void CXTPFlowGraphPage::OnRemoved()
{
	if (m_pGroups)
	{
		m_pGroups->RemoveAll();
	}

	if (m_pConnections)
	{
		m_pConnections->RemoveAll();
	}

	if (m_pNodes)
	{
		m_pNodes->RemoveAll();
	}
}

void CXTPFlowGraphPage::OnGraphChanged()
{
	for (int i = 0; i < m_pNodes->GetCount(); i++)
		m_pNodes->GetAt(i)->m_bDirty = TRUE;

	m_pFirstVisibleNode = NULL;
	m_pFirstVisibleConnection = NULL;

	if (m_pControl) m_pControl->OnGraphChanged();
}

void CXTPFlowGraphPage::Reposition(CXTPFlowGraphDrawContext* pDC, CRect rcPage)
{
	m_rcPage = rcPage;

	m_rcWorkRect.SetRect(0, 0, 0, 0);
	CXTPFlowGraphPaintManager* pPaintManager = m_pControl->GetPaintManager();

	int i;
	int nNodesCount = m_pNodes->GetCount();

	for (i = 0; i < nNodesCount; i++)
	{
		CXTPFlowGraphNode* pNode = m_pNodes->GetAt(i);


		if (pNode->m_bDirty)
		{
			pNode->RecalLayout(pDC);
		}

		CRect rc = pPaintManager->GetNodeBoundingRectangle(pNode);

		if (i == 0)
		{
			m_rcWorkRect = rc;
		}

		m_rcWorkRect.right = max(m_rcWorkRect.right, rc.right);
		m_rcWorkRect.bottom = max(m_rcWorkRect.bottom, rc.bottom);

		m_rcWorkRect.left = min(m_rcWorkRect.left, rc.left);
		m_rcWorkRect.top = min(m_rcWorkRect.top, rc.top);
	}

	for (i = 0; i < m_pGroups->GetCount(); i++)
	{
		CXTPFlowGraphNodeGroup* pGroup = m_pGroups->GetAt(i);
		CRect rcWorkRect(0, 0, 0, 0);

		for (int j = 0; j < pGroup->GetCount(); j++)
		{
			CXTPFlowGraphNode* pNode = pGroup->GetAt(j);

			CRect rc = pPaintManager->GetNodeBoundingRectangle(pNode);

			if (j == 0)
			{
				rcWorkRect = rc;
			}

			rcWorkRect.right = max(rcWorkRect.right, rc.right);
			rcWorkRect.bottom = max(rcWorkRect.bottom, rc.bottom);

			rcWorkRect.left = min(rcWorkRect.left, rc.left);
			rcWorkRect.top = min(rcWorkRect.top, rc.top);
		}
		pGroup->m_rcWorkRect = rcWorkRect;
	}

	for (i = 0; i < m_pConnections->GetCount(); i++)
	{
		CXTPFlowGraphConnection* pConnection = GetConnections()->GetAt(i);

		if (pConnection->m_bDirty || (pConnection->GetInputPoint() && pConnection->GetInputPoint()->GetNode()->m_bDirty) ||
			(pConnection->GetOutputPoint() && pConnection->GetOutputPoint()->GetNode()->m_bDirty))
		{
			pConnection->RecalcLayout(pDC);
			pConnection->m_bDirty = FALSE;
		}
	}


	for (i = 0; i < nNodesCount; i++)
	{
		CXTPFlowGraphNode* pNode = m_pNodes->GetAt(i);
		pNode->m_bDirty = FALSE;
	}

	m_pFirstVisibleNode = NULL;
	CRect rcBounds = ScreenToPage(rcPage);

	for (i = 0; i < nNodesCount; i++)
	{
		CXTPFlowGraphNode* pNode = m_pNodes->GetAt(i);
		CRect rcNodeBounds = pPaintManager->GetNodeBoundingRectangle(pNode);

		pNode->m_pNextVisibleNode = m_pFirstVisibleNode;

		if (rcBounds.right < rcNodeBounds.left || rcBounds.left > rcNodeBounds.right || rcBounds.top > rcNodeBounds.bottom || rcBounds.bottom < rcNodeBounds.top)
			continue;

		m_pFirstVisibleNode = pNode;
	}

	m_pFirstVisibleConnection = NULL;

	for (i = 0; i < m_pConnections->GetCount(); i++)
	{
		CXTPFlowGraphConnection* pConnection = GetConnections()->GetAt(i);
		pConnection->m_pNextVisibleConnection = m_pFirstVisibleConnection;

		CRect rcConnectionBounds = pConnection->GetBoundingRect();

		pConnection->m_pNextVisibleConnection = m_pFirstVisibleConnection;

		if (rcBounds.right < rcConnectionBounds.left || rcBounds.left > rcConnectionBounds.right ||
			rcBounds.top > rcConnectionBounds.bottom || rcBounds.bottom < rcConnectionBounds.top)
			continue;

		m_pFirstVisibleConnection = pConnection;
	}
}

CRect CXTPFlowGraphPage::PageToScreen(LPCRECT lpRect) const
{
	CRect rc(lpRect);
	rc.OffsetRect(GetPageRect().TopLeft());
	rc.OffsetRect(-GetScrollOffset());

	double dZoomLevel = GetZoomLevel();

	return CRect(int(rc.left * dZoomLevel), int(rc.top * dZoomLevel), int(rc.right * dZoomLevel), int(rc.bottom * dZoomLevel));
}

void CXTPFlowGraphPage::ScreenToPage(double& x, double& y) const
{
	double dZoomLevel = GetZoomLevel();

	x /= dZoomLevel;
	x += GetScrollOffset().x;

	y /= dZoomLevel;
	y += GetScrollOffset().y;
}

CPoint CXTPFlowGraphPage::ScreenToPage(CPoint point) const
{
	double dZoomLevel = GetZoomLevel();

	point.x = int(point.x / dZoomLevel);
	point.x += GetScrollOffset().x;

	point.y = int(point.y / dZoomLevel);
	point.y += GetScrollOffset().y;

	return point;
}

CRect CXTPFlowGraphPage::ScreenToPage(LPCRECT lpRect) const
{
	double dZoomLevel = GetZoomLevel();

	return CRect(int(lpRect->left / dZoomLevel + GetScrollOffset().x), int(lpRect->top / dZoomLevel + GetScrollOffset().y),
		(int)ceil(lpRect->right / dZoomLevel + GetScrollOffset().x), (int)ceil(lpRect->bottom / dZoomLevel + GetScrollOffset().y));


}

void CXTPFlowGraphPage::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Double(pPX, _T("ZoomLevel"), m_dZoomLevel, 1.0);

	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_Int(pPX, _T("Id"), m_nId, 0);

	PX_Point(pPX, _T("ScrollOffset"), m_ptScrollOffset, CPoint(0, 0));

	m_pNodes->DoPropExchange(pPX);

	m_pConnections->DoPropExchange(pPX);

	if (pPX->IsLoading())
	{
		m_pGroups->RemoveAll();

		CMap<int, int, CXTPFlowGraphNodeGroup*, CXTPFlowGraphNodeGroup*>  mapGroups;

		for (int i = 0; i < m_pNodes->GetCount(); i++)
		{
			CXTPFlowGraphNode* pNode = m_pNodes->GetAt(i);

			if (pNode->m_nGroupId != 0)
			{
				CXTPFlowGraphNodeGroup* pGroup = NULL;
				if (!mapGroups.Lookup(pNode->m_nGroupId, pGroup))
				{
					pGroup = m_pGroups->AddGroup();
					mapGroups[pNode->m_nGroupId] = pGroup;
				}

				pGroup->AddNode(pNode);
			}
		}
	}
}

void CXTPFlowGraphPage::Copy()
{
	try
	{
		int nCount = 0, i;
		for (i = 0; i < GetSelection()->GetCount(); i++)
		{
			if (DYNAMIC_DOWNCAST(CXTPFlowGraphNode, GetSelection()->GetAt(i)))
			{
				nCount++;
			}
		}
		if (nCount == 0)
			return;

		CSharedFile sharedFile;
		CArchive ar (&sharedFile, CArchive::store);

		ar.WriteCount(nCount);

		CXTPPropExchangeArchive px(ar);

		for (i = 0; i < GetSelection()->GetCount(); i++)
		{
			CXTPFlowGraphNode* pNode = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, GetSelection()->GetAt(i));
			if (pNode)
			{
				CRuntimeClass* pClass = pNode->GetRuntimeClass();
				ASSERT (pClass != NULL);

				ar.WriteClass (pClass);
				pNode->DoPropExchange(&px);
			}
		}

		ar.Close();

		if (!m_pControl->OpenClipboard())
			return;

		if (!::EmptyClipboard())
		{
			::CloseClipboard();
			return;
		}

		HGLOBAL hGlobal = sharedFile.Detach();

		if (hGlobal)
		{
			SetClipboardData(m_cfNode, hGlobal);
		}

		::CloseClipboard();
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::PrepareDrag. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::PrepareDrag. Archive exception\r\n"));
		pEx->Delete ();
	}

}

void CXTPFlowGraphPage::Paste()
{
	COleDataObject data;

	if (!data.AttachClipboard())
	{
		return;
	}

	if (!data.IsDataAvailable(m_cfNode))
		return;

	CFile* pFile = data.GetFileData(m_cfNode);
	if (!pFile)
		return;

	GetSelection()->Clear();
	m_pControl->GetUndoManager()->StartGroup();

	try
	{
		CArchive ar (pFile, CArchive::load);
		CXTPPropExchangeArchive px(ar);

		int nCount = (int)ar.ReadCount();
		for (int i = 0; i < nCount; i++)
		{
			CRuntimeClass* pClass = ar.ReadClass ();

			if (pClass != NULL)
			{
				CXTPFlowGraphNode* pNode = (CXTPFlowGraphNode*)pClass->CreateObject();

				if (pNode != NULL)
				{
					pNode->DoPropExchange(&px);
				}

				CPoint pt = pNode->GetLocation();
				pNode->SetLocation(pt + CPoint(10, 10));

				m_pNodes->AddNode(pNode);

				GetSelection()->AddSelection(pNode);
			}
		}

		ar.Close ();
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. Archive exception\r\n"));
		pEx->Delete ();
	}
	catch (CNotSupportedException *pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. \"Not Supported\" exception\r\n"));
		pEx->Delete ();
	}

	m_pControl->GetUndoManager()->EndGroup();


	delete pFile;
}

void CXTPFlowGraphPage::Remove()
{
	if (!m_pControl)
		return;

	m_pControl->GetPages()->Remove(this);

}

BOOL CXTPFlowGraphPage::CanPaste()
{
	return IsClipboardFormatAvailable(m_cfNode);
}

void CXTPFlowGraphPage::Cut()
{
	Copy();

	m_pControl->GetUndoManager()->StartGroup();

	for (int i = GetSelection()->GetCount() - 1; i >= 0; i--)
	{
		CXTPFlowGraphNode* pNode = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, GetSelection()->GetAt(i));
		if (pNode)
		{
			pNode->Remove();
			continue;
		}

		CXTPFlowGraphConnection* pConnection = DYNAMIC_DOWNCAST(CXTPFlowGraphConnection, GetSelection()->GetAt(i));

		if (pConnection)
		{
			pConnection->Remove();
		}
	}

	m_pControl->GetUndoManager()->EndGroup();
}

void CXTPFlowGraphPage::OnDraw(CXTPFlowGraphDrawContext* pDC)
{
	int i;

	CXTPFlowGraphPaintManager* pPaintManager = GetControl()->GetPaintManager();


	if (pPaintManager->m_bShowNodeGroupsAlways)
	{
		for (i = 0; i < GetGroups()->GetCount(); i++)
		{
			CXTPFlowGraphNodeGroup* pGroup = GetGroups()->GetAt(i);
			if (pGroup->GetCount() > 1 && (!pGroup->GetAt(0)->IsSelected()))
			{
				pPaintManager->DrawNodeGroup(pDC, pGroup);
			}
		}
	}

	for (i = 0; i < GetConnections()->GetCount(); i++)
	{
		CXTPFlowGraphConnection* pConnection = GetConnections()->GetAt(i);
		if (!pConnection->IsSelected())
			pPaintManager->DrawConnection(pDC, pConnection);
	}

	for (i = 0; i < GetNodes()->GetCount(); i++)
	{
		CXTPFlowGraphNode* pNode = GetNodes()->GetAt(i);
		if (!pNode->IsSelected())
			pNode->Draw(pDC);
	}

	for (i = 0; i < GetConnections()->GetCount(); i++)
	{
		CXTPFlowGraphConnection* pConnection = GetConnections()->GetAt(i);
		if (pConnection->IsSelected())
			pPaintManager->DrawConnection(pDC, pConnection);
	}

	for (i = 0; i < GetGroups()->GetCount(); i++)
	{
		CXTPFlowGraphNodeGroup* pGroup = GetGroups()->GetAt(i);
		if (pGroup->GetCount() > 1 && pGroup->GetAt(0)->IsSelected())
		{
			pPaintManager->DrawNodeGroup(pDC, pGroup);
		}
	}

	for (i = 0; i < GetNodes()->GetCount(); i++)
	{
		CXTPFlowGraphNode* pNode = GetNodes()->GetAt(i);
		if (pNode->IsSelected())
			pNode->Draw(pDC);
	}

}

void CXTPFlowGraphPage::SetArrangeLevel(int v, CArray<CDWordArray*, CDWordArray*>& graph)
{
	CXTPFlowGraphNode* pRootNode = m_pNodes->GetAt(v);
	ASSERT(pRootNode->m_nArrangeIndex == v);
	pRootNode->m_nArrangeIndex = -1;

	int nLevel = pRootNode->m_nArrangeLevel;

	for (int i = 0; i < graph[v]->GetSize(); i++)
	{
		int t = graph[v]->GetAt(i);
		CXTPFlowGraphNode* pNode = GetNodes()->GetAt(t);

		if ((pNode->m_nArrangeIndex != -1) &&
			(pNode->m_nArrangeLevel == -1 || pNode->m_nArrangeLevel < nLevel + 1))
		{
			pNode->m_nArrangeLevel = nLevel + 1;
			SetArrangeLevel(t, graph);
		}
	}

	pRootNode->m_nArrangeIndex = v;
}

int CXTPFlowGraphPage::SetArrangeLocation(int v, int x, int y, int nLevelWidth, CArray<CDWordArray*, CDWordArray*>& graph)
{
	CXTPFlowGraphNode* pRootNode = m_pNodes->GetAt(v);
	pRootNode->m_nArrangeLevel = -1;

	pRootNode->SetLocation(CPoint(x, y));

	int nBottom = y + pRootNode->GetSize().cy + 10;

	for (int i = 0; i < graph[v]->GetSize(); i++)
	{
		int t = graph[v]->GetAt(i);
		CXTPFlowGraphNode* pNode = GetNodes()->GetAt(t);
		if (pNode->m_nArrangeLevel == -1)
			continue;

		int nChildBottom = SetArrangeLocation(t, x + nLevelWidth, y, nLevelWidth, graph);

		y = nChildBottom;
	}

	nBottom = max(nBottom, y);

	return nBottom;
}

void CXTPFlowGraphPage::Arrange()
{
	m_dZoomLevel = 1;
	m_ptScrollOffset = CPoint(0, 0);

	CXTPFlowGraphControl* pControl = GetControl();
	pControl->GetUndoManager()->StartGroup();

	int i;

	for (i = 0; i < GetConnections()->GetCount(); i++)
	{
		CXTPFlowGraphConnection* pConnection = GetConnections()->GetAt(i);

		pConnection->SetControlPoint(CPoint(-1, -1));
	}

	int nMaxWidth = 0;
	int nNodesCount = GetNodes()->GetCount();

	for (i = 0; i < nNodesCount; i++)
	{
		CXTPFlowGraphNode* pNode = GetNodes()->GetAt(i);

		nMaxWidth = max(nMaxWidth, pNode->GetSize().cx);

		pNode->m_nArrangeIndex = i;
		pNode->m_nArrangeLevel = -1;
		pNode->m_nArrangeComponent = -1;
	}

	CArray<CDWordArray*, CDWordArray*> graph;
	graph.SetSize(nNodesCount);

	CArray<CDWordArray*, CDWordArray*> tgraph;
	tgraph.SetSize(nNodesCount);


	for (i = 0; i < nNodesCount; i++)
	{
		graph[i] = new CDWordArray();
		tgraph[i] = new CDWordArray();
	}

	for (i = 0; i < GetConnections()->GetCount(); i++)
	{
		CXTPFlowGraphConnection* pConnection = GetConnections()->GetAt(i);

		if (!pConnection->GetInputNode() || !pConnection->GetOutputNode())
			continue;

		int nFrom = pConnection->GetOutputNode()->m_nArrangeIndex;
		int nTo = pConnection->GetInputNode()->m_nArrangeIndex;

		graph[nFrom]->Add(nTo);
		tgraph[nTo]->Add(nFrom);
	}

	for (i = 0; i < nNodesCount; i++)
	{
		if (tgraph[i]->GetSize() == 0)
		{
			CXTPFlowGraphNode* pNode = GetNodes()->GetAt(i);

			pNode->m_nArrangeLevel = 0;
			SetArrangeLevel(i, graph);
		}
	}
	for (i = 0; i < nNodesCount; i++)
	{
		CXTPFlowGraphNode* pNode = GetNodes()->GetAt(i);
		if (pNode->m_nArrangeLevel == -1)
		{
			pNode->m_nArrangeLevel = 0;
			SetArrangeLevel(i, graph);
		}
	}

	int nCompCount = 0;

	for (i = 0; i < nNodesCount; i++)
	{
		if (m_pNodes->GetAt(i)->m_nArrangeComponent == -1)
		{
			m_pNodes->GetAt(i)->m_nArrangeComponent = nCompCount;

			CDWordArray stack;
			stack.Add(i);

			while (stack.GetSize())
			{
				int v = stack[stack.GetSize() - 1], j;
				stack.RemoveAt(stack.GetSize() - 1);

				for (j = 0; j < graph[v]->GetSize(); j++)
				{
					int t = graph[v]->GetAt(j);
					if (m_pNodes->GetAt(t)->m_nArrangeComponent == -1)
					{
						m_pNodes->GetAt(t)->m_nArrangeComponent = nCompCount;
						stack.Add(t);
					}
				}

				for (j = 0; j < tgraph[v]->GetSize(); j++)
				{
					int t = tgraph[v]->GetAt(j);
					if (m_pNodes->GetAt(t)->m_nArrangeComponent == -1)
					{
						m_pNodes->GetAt(t)->m_nArrangeComponent = nCompCount;
						stack.Add(t);
					}
				}
			}

			nCompCount++;
		}
	}

	for (i = 0; i < nNodesCount; i++)
	{
		CDWordArray* pConnections = graph[i];
		int nSize = (int)pConnections->GetSize();

		for (int j = 0; j < nSize; j++)
		{
			for (int k = j + 1; k < nSize; k++)
			{
				CXTPFlowGraphNode* pj = m_pNodes->GetAt(pConnections->GetAt(j));
				CXTPFlowGraphNode* pk = m_pNodes->GetAt(pConnections->GetAt(k));

				if ((pj->m_nArrangeLevel > pk->m_nArrangeLevel) ||
					(pj->m_nArrangeLevel == pk->m_nArrangeLevel && pj->m_nArrangeIndex > pk->m_nArrangeIndex))
				{
					int t = (*pConnections)[j];
					(*pConnections)[j] = (*pConnections)[k];
					(*pConnections)[k] = t;
				}
			}
		}
	}

	CDWordArray order;

	for (i = 0; i < nNodesCount; i++)
	{
		if (m_pNodes->GetAt(i)->m_nArrangeLevel == 0)
			order.Add(i);
	}

	for (i = 0; i < order.GetSize(); i++)
	{
		for (int j = i + 1; j < order.GetSize(); j++)
		{
			CDWordArray& pi = *graph[order[i]];
			CDWordArray& pj = *graph[order[j]];

			BOOL bOrder = TRUE;

			if (m_pNodes->GetAt(order[i])->m_nArrangeComponent != m_pNodes->GetAt(order[j])->m_nArrangeComponent)
			{
				if (m_pNodes->GetAt(order[i])->m_nArrangeComponent > m_pNodes->GetAt(order[j])->m_nArrangeComponent)
				{
					bOrder = FALSE;
				}
			}
			else
			{
				for (int k = 0; ; k++)
				{
					if (pi.GetSize() <= k)
						break;
					if (pj.GetSize() <= k)
					{
						bOrder = FALSE;
						break;
					}
					if (pi[k] != pj[k])
					{
						bOrder = pi[k] < pj[k];
						break;
					}
				}
			}
			if (!bOrder)
			{
				int t = order[i];
				order[i] = order[j];
				order[j] = t;
			}
		}
	}

	int x = 10, y = 10;

	for (i = 0; i < order.GetSize(); i++)
	{
		y = SetArrangeLocation(order[i], x, y, 50 + nMaxWidth, graph);
	}

	for (i = 0; i < nNodesCount; i++)
	{
		delete graph[i];
		delete tgraph[i];
	}


	pControl->GetUndoManager()->EndGroup();

	pControl->SendNotifyMessage(XTP_FGN_PAGESCROLLOFFSETCHANGED);
	pControl->SendNotifyMessage(XTP_FGN_PAGEZOOMLEVELCHANGED);

}

