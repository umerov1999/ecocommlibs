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

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphNodeGroups.h"
#include "XTPFlowGraphPage.h"


CXTPFlowGraphNodeGroups::CXTPFlowGraphNodeGroups(CXTPFlowGraphPage* pPage)
{
	m_pPage = pPage;

}

CXTPFlowGraphNodeGroups::~CXTPFlowGraphNodeGroups()
{
	RemoveAll();
}

void CXTPFlowGraphNodeGroups::RemoveAll()
{
	if (m_arrGroups.GetSize() == 0)
		return;

	for (int i = 0; i < m_arrGroups.GetSize(); i++)
	{
		m_arrGroups[i]->RemoveAll();
		m_arrGroups[i]->InternalRelease();
	}
	m_arrGroups.RemoveAll();
}

void CXTPFlowGraphNodeGroups::Remove(CXTPFlowGraphNodeGroup* pGroup)
{
	for (int i = 0; i < m_arrGroups.GetSize(); i++)
	{
		if (m_arrGroups[i] == pGroup)
		{
			pGroup->RemoveAll();
			pGroup->InternalRelease();
			m_arrGroups.RemoveAt(i);
			return;
		}
	}
}

CXTPFlowGraphNodeGroup* CXTPFlowGraphNodeGroups::AddGroup()
{
	CXTPFlowGraphNodeGroup* pGroup = new CXTPFlowGraphNodeGroup();
	pGroup->m_pGroups = this;

	m_arrGroups.Add(pGroup);

	return pGroup;
}

