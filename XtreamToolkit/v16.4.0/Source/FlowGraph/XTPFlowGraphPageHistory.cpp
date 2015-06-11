// XTPFlowGraphPageHistory.cpp : implementation of the CXTPFlowGraphPageHistory class.
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

#include "stdafx.h"

#include "Common/XTPPropExchange.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphPageHistory.h"

CXTPFlowGraphPageHistory::CXTPFlowGraphPageHistory(CXTPFlowGraphControl* pControl)
{
	m_pControl = pControl;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphPageHistory::~CXTPFlowGraphPageHistory()
{

}

BOOL CXTPFlowGraphPageHistory::CanGoBackward() const
{
	return m_arrPageUndo.GetSize() > 0;
}

BOOL CXTPFlowGraphPageHistory::CanGoForward() const
{
	return m_arrPageRedo.GetSize() > 0;
}

void CXTPFlowGraphPageHistory::AnimatePageTransition(CXTPFlowGraphNode* pNodeFrom, CXTPFlowGraphNode* pNodeTo)
{
	ASSERT(pNodeFrom && pNodeTo && pNodeFrom->GetPage() != pNodeTo->GetPage());

	if (!pNodeFrom || !pNodeTo)
		return;

	CXTPFlowGraphPageHistoryItem hi = {pNodeFrom, pNodeTo};

	m_arrPageUndo.Add(hi);
	m_arrPageRedo.RemoveAll();

	m_pControl->AnimatePageTransition(pNodeFrom, pNodeTo);
};

void CXTPFlowGraphPageHistory::GoBackward()
{
	if (m_arrPageUndo.GetSize() == 0)
		return;

	CXTPFlowGraphPageHistoryItem hi = m_arrPageUndo[m_arrPageUndo.GetSize() - 1];
	m_arrPageUndo.RemoveAt(m_arrPageUndo.GetSize() - 1);

	m_arrPageRedo.Add(hi);

	m_pControl->AnimatePageTransition(hi.pTo, hi.pFrom);
}

void CXTPFlowGraphPageHistory::GoForward()
{
	if (m_arrPageRedo.GetSize() == 0)
		return;

	CXTPFlowGraphPageHistoryItem hi = m_arrPageRedo[m_arrPageRedo.GetSize() - 1];
	m_arrPageRedo.RemoveAt(m_arrPageRedo.GetSize() - 1);

	m_arrPageUndo.Add(hi);

	m_pControl->AnimatePageTransition(hi.pFrom, hi.pTo);
}

void CXTPFlowGraphPageHistory::Clear()
{
	m_arrPageRedo.RemoveAll();
	m_arrPageUndo.RemoveAll();
}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphPageHistory, CCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphPageHistory, "GoBackward", 1000, GoBackward, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphPageHistory, "GoForward", 1001, GoForward, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphPageHistory, "CanGoBackward", 1002, CanGoBackward, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphPageHistory, "CanGoForward", 1003, CanGoForward, SetNotSupported, VT_BOOL)
	DISP_FUNCTION_ID(CXTPFlowGraphPageHistory, "Clear", 1004, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphPageHistory, "AnimatePageTransition", 1015, OleAnimatePageTransition, VT_EMPTY, VTS_DISPATCH VTS_DISPATCH)
END_DISPATCH_MAP()


// {13353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphPageHistory =
{ 0x13353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphPageHistory, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphPageHistory, IID_IFlowGraphPageHistory, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphPageHistory, IID_IFlowGraphPageHistory)

void CXTPFlowGraphPageHistory::OleAnimatePageTransition(LPDISPATCH lpDispFrom, LPDISPATCH lpDispTo)
{
	CXTPFlowGraphNode* pNodeFrom = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, CCmdTarget::FromIDispatch(lpDispFrom));
	CXTPFlowGraphNode* pNodeTo = DYNAMIC_DOWNCAST(CXTPFlowGraphNode, CCmdTarget::FromIDispatch(lpDispTo));

	AnimatePageTransition(pNodeFrom, pNodeTo);
}

#endif
