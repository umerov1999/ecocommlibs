// XTPFlowGraphNodeGroup.cpp : implementation of the XTPFlowGraphNodeGroup class.
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

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphNodeGroups.h"
#include "XTPFlowGraphPage.h"

IMPLEMENT_DYNAMIC(CXTPFlowGraphNodeGroup, CXTPFlowGraphElement)

CXTPFlowGraphNodeGroup::CXTPFlowGraphNodeGroup()
{
	m_pGroups = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
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

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPFlowGraphNodeGroup, CXTPFlowGraphElement)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "Node", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNodeGroup, "Caption", DISPID_CAPTION, OleGetCaption, OleSetCaption, VT_BSTR)

	DISP_PROPERTY_EX_ID(CXTPFlowGraphNodeGroup, "Page", 11, OleGetPage, SetNotSupported, VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "AddNode", 2, OleAddNode, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPFlowGraphNodeGroup, "RemoveNode", 3, OleRemoveNode, VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()

// {03353C2D-B341-4329-0123-97B0BF66B90E}
static const GUID IID_IFlowGraphNodeGroup =
{ 0x03353c2d, 0xb341, 0x4329, { 0x01, 0x23, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphNodeGroup, CXTPFlowGraphElement)
	INTERFACE_PART(CXTPFlowGraphNodeGroup, IID_IFlowGraphNodeGroup, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphNodeGroup, IID_IFlowGraphNodeGroup)

void CXTPFlowGraphNodeGroup::OleSetCaption(LPCTSTR lpszCaption)
{
	SetCaption(lpszCaption);
}

BSTR CXTPFlowGraphNodeGroup::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}

LPDISPATCH CXTPFlowGraphNodeGroup::OleGetPage()
{
	return XTPGetDispatch(GetPage());
}

LPDISPATCH CXTPFlowGraphNodeGroup::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphNodeGroup)

void CXTPFlowGraphNodeGroup::OleAddNode(LPDISPATCH Node)
{
	CXTPFlowGraphNode* pNode = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, CXTPFlowGraphNode::FromIDispatch(Node));

	if (!pNode)
		return;

	AddNode(pNode);
}

void CXTPFlowGraphNodeGroup::OleRemoveNode(LPDISPATCH Node)
{
	CXTPFlowGraphNode* pNode = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, CXTPFlowGraphNode::FromIDispatch(Node));

	if (!pNode)
		return;

	RemoveNode(pNode);
}

int CXTPFlowGraphNodeGroup::OleGetItemCount()
{
	return GetCount();
}


#endif
