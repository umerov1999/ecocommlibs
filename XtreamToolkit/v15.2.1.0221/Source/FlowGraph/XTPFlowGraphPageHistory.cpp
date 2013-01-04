// XTPFlowGraphPageHistory.cpp : implementation of the CXTPFlowGraphPageHistory class.
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
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphPageHistory.h"

CXTPFlowGraphPageHistory::CXTPFlowGraphPageHistory(CXTPFlowGraphControl* pControl)
{
	m_pControl = pControl;

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



