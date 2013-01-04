// XTPFlowGraphNodeGroup.cpp : implementation of the XTPFlowGraphNodeGroup class.
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
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphNodeGroups.h"
#include "XTPFlowGraphPage.h"

IMPLEMENT_DYNAMIC(CXTPFlowGraphNodeGroup, CXTPFlowGraphElement)

CXTPFlowGraphNodeGroup::CXTPFlowGraphNodeGroup()
{
	m_pGroups = NULL;

}

CXTPFlowGraphNodeGroup::~CXTPFlowGraphNodeGroup()
{
	RemoveAll();
}

void CXTPFlowGraphNodeGroup::RemoveAll()
{
	for (int i = 0; i < m_arrNodes.GetSize(); i++)
		m_arrNodes[i]->m_pGroup = NULL;

	m_arrNodes.RemoveAll();
}

void CXTPFlowGraphNodeGroup::Remove()
{
	if (!m_pGroups)
		return;

	m_pGroups->Remove(this);
}

void CXTPFlowGraphNodeGroup::AddNode(CXTPFlowGraphNode* pNode)
{
	if (!pNode)
		return;

	if (pNode->GetGroup() == this)
		return;

	if (pNode->GetGroup() != NULL)
	{
		pNode->GetGroup()->RemoveNode(pNode);
	}

	m_arrNodes.Add(pNode);
	pNode->m_pGroup = this;

	OnGraphChanged();
}

void CXTPFlowGraphNodeGroup::RemoveNode(CXTPFlowGraphNode* pNode)
{
	if (!pNode || pNode->GetGroup() != this)
		return;

	for (int i = 0; i < m_arrNodes.GetSize(); i++)
	{
		if (m_arrNodes[i] == pNode)
		{
			m_arrNodes.RemoveAt(i);
			pNode->m_pGroup = NULL;

			OnGraphChanged();
			return;
		}
	}
}

CXTPFlowGraphControl* CXTPFlowGraphNodeGroup::GetControl() const
{
	if (m_pGroups == NULL)
		return NULL;

	return m_pGroups->GetPage()->GetControl();
}

CXTPFlowGraphPage* CXTPFlowGraphNodeGroup::GetPage() const
{
	if (m_pGroups == NULL)
		return NULL;

	return m_pGroups->GetPage();
}

