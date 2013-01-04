// XTPFlowGraphNode.cpp : implementation of the CXTPFlowGraphNode class.
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
#include "Common/XTPMarkupRender.h"

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnections.h"
#include "XTPFlowGraphSelectedElements.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphUndoManager.h"
#include "XTPFlowGraphMessages.h"
#include "XTPFlowGraphImage.h"

IMPLEMENT_SERIAL(CXTPFlowGraphNode, CXTPFlowGraphElement, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)


CXTPFlowGraphNode::CXTPFlowGraphNode()
{
	m_pPage = NULL;
	m_ptLocation = CPoint(0, 0);

	m_szUserSize = CSize(0, 0);
	m_szActualSize = CSize(0, 0);

	m_clrNode = COLORREF(-1);
	m_nStyle = -1;

	m_dwTag = 0;
	m_nId = 0;
	m_hWnd = NULL;
	m_szWindow = CSize(0, 0);

	m_rcCaption.SetRectEmpty();

	m_rcWindow.SetRectEmpty();

	m_pGroup = NULL;
	m_nGroupId = NULL;

	m_nArrangeIndex = -1;
	m_nArrangeLevel = -1;
	m_nArrangeComponent = -1;

	m_nImageIndex = -1;

	m_bLocked = FALSE;

	m_pConnectionPoints = new CXTPFlowGraphConnectionPoints(this);

	m_pNextVisibleNode = NULL;

	m_pMarkupUIElement = NULL;

}

CXTPFlowGraphNode::~CXTPFlowGraphNode()
{
	if (m_pConnectionPoints)
	{
		m_pConnectionPoints->RemoveAll();

		m_pConnectionPoints->InternalRelease();
		m_pConnectionPoints = NULL;
	}

	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

CXTPFlowGraphControl* CXTPFlowGraphNode::GetControl() const
{
	if (!m_pPage)
		return NULL;

	return m_pPage->GetControl();
}


void CXTPFlowGraphNode::RecalLayout(CXTPFlowGraphDrawContext* pDC)
{
	CXTPFlowGraphPaintManager* pPaintManager = GetControl()->GetPaintManager();

	pPaintManager->RecalcNodeLayout(pDC, this);
}

void CXTPFlowGraphNode::UpdateWindowPosition()
{
	if (!m_hWnd)
		return;

	CRect rc(m_rcWindow);
	rc.OffsetRect(m_ptLocation);
	rc = m_pPage->PageToScreen(rc);

	::SetWindowPos(m_hWnd, 0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
}


void CXTPFlowGraphNode::Draw(CXTPFlowGraphDrawContext* pDC)
{
	CXTPFlowGraphPaintManager* pPaintManager = GetControl()->GetPaintManager();

	pPaintManager->DrawNode(pDC, this);

	UpdateWindowPosition();
}

CRect CXTPFlowGraphNode::GetScreenRect() const
{
	CRect rc(m_ptLocation, GetSize());

	return m_pPage->PageToScreen(rc);
}

CXTPFlowGraphImage* CXTPFlowGraphNode::GetImage() const
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (!pControl)
		return NULL;

	return pControl->GetImages()->GetAt(m_nImageIndex);
}


void CXTPFlowGraphNode::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));
	PX_Point(pPX, _T("Location"), m_ptLocation, CPoint(0, 0));
	PX_DWord(pPX, _T("Color"), (DWORD)m_clrNode, (COLORREF)-1);
	PX_Int(pPX, _T("Style"), m_nStyle, (int)-1);
	PX_Int(pPX, _T("ImageIndex"), m_nImageIndex, (int)-1);
	PX_Size(pPX, _T("UserSize"), m_szUserSize, CSize(0, 0));
	PX_Bool(pPX, _T("Locked"), m_bLocked, FALSE);

	DWORD dwTag = (DWORD)m_dwTag;
	PX_DWord(pPX, _T("Tag"), (DWORD)dwTag, 0);
	if (pPX->IsLoading())
		m_dwTag = dwTag;

	PX_Int(pPX, _T("Id"), m_nId, 0);

	if (pPX->IsLoading())
	{
		PX_Int(pPX, _T("GroupId"), m_nGroupId, 0);
	}
	else
	{
		m_nGroupId = m_pGroup ? (int)(INT_PTR)m_pGroup : 0;
		PX_Int(pPX, _T("GroupId"), m_nGroupId, 0);
	}

	m_pConnectionPoints->DoPropExchange(pPX);

	if (pPX->IsLoading())
	{
		CXTPFlowGraphControl* pControl = GetControl();
		if (pControl)
		{
			m_pMarkupUIElement = XTPMarkupParseText(pControl->GetMarkupContext(), m_strCaption);
		}
	}
}

CXTPFlowGraphConnectionPoint* CXTPFlowGraphNode::HitTestConnectionPoint(CPoint point)
{
	point = m_pPage->ScreenToPage(point);
	point.Offset(-m_ptLocation);

	for (int i = 0; i < m_pConnectionPoints->GetCount(); i++)
	{
		CXTPFlowGraphConnectionPoint* pPoint = m_pConnectionPoints->GetAt(i);

		if (pPoint->GetRect().PtInRect(point))
			return pPoint;
	}

	return NULL;
}

void CXTPFlowGraphNode::Remove()
{
	if (!m_pPage)
		return;

	m_pPage->GetNodes()->Remove(this);
}

void CXTPFlowGraphNode::OnRemoved()
{
	m_pConnectionPoints->RemoveAll();

	if (m_pGroup)
	{
		m_pGroup->RemoveNode(this);
	}

	CXTPFlowGraphElement::OnRemoved();
}

void CXTPFlowGraphNode::SetLocation(CPoint ptLocation)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl)
	{
		pControl->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoSetNodePositionCommand(this, m_ptLocation));
	}

	m_ptLocation = ptLocation;
	OnGraphChanged();

	if (pControl)
	{
		XTP_NM_FLOWGRAPH_NODECHANGED nc;
		nc.pNode = this;

		pControl->SendNotifyMessage(XTP_FGN_NODELOCATIONCHANGED, &nc.hdr);
	}
}

void CXTPFlowGraphNode::SetSize(CSize sz)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl)
	{
		pControl->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoSetNodeSizeCommand(this, m_szUserSize));
	}

	m_szUserSize = sz;
	OnGraphChanged();

	if (pControl)
	{
		XTP_NM_FLOWGRAPH_NODECHANGED nc;
		nc.pNode = this;

		pControl->SendNotifyMessage(XTP_FGN_NODELOCATIONCHANGED, &nc.hdr);
	}
}

CSize CXTPFlowGraphNode::GetSize() const
{
	return m_szActualSize;
}

CSize CXTPFlowGraphNode::GetUserSize() const
{
	return m_szUserSize;
}

void CXTPFlowGraphNode::SetCaption(LPCTSTR lpszCaption)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl)
	{
		pControl->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoSetNodeCaptionCommand(this, m_strCaption));
	}

	m_strCaption = lpszCaption;

	XTPMarkupReleaseElement(m_pMarkupUIElement);

	if (pControl)
	{
		m_pMarkupUIElement = XTPMarkupParseText(pControl->GetMarkupContext(), lpszCaption);
	}

	OnGraphChanged();
}

void CXTPFlowGraphNode::DrawWindowContent(CXTPFlowGraphDrawContext* /*pDC*/)
{

}


void CXTPFlowGraphNode::Select()
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (!pControl)
		return;

	CXTPFlowGraphSelectedElements* pSelectedElements = m_pPage->GetSelection();

	if (GetGroup() != NULL)
	{
		for (int j = 0; j < GetGroup()->GetCount(); j++)
		{
			CXTPFlowGraphNode* pNode = GetGroup()->GetAt(j);
			pSelectedElements->AddSelection(pNode);
		}
	}
	else
	{
		pSelectedElements->AddSelection(this);
	}
}


