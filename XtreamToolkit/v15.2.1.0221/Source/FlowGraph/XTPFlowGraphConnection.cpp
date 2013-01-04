// XTPFlowGraphConnection.cpp : implementation of the CXTPFlowGraphConnection class.
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
#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "Common/XTPPropExchange.h"

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphDrawContext.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnections.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphSelectedElements.h"
#include "XTPFlowGraphUndoManager.h"


IMPLEMENT_SERIAL(CXTPFlowGraphConnection, CXTPFlowGraphElement, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPFlowGraphConnection::CXTPFlowGraphConnection()
{
	m_pPage = NULL;

	m_pInputPoint = NULL;
	m_pOutputPoint = NULL;

	m_nInputId = m_nOutputId = 0;

	m_clrConnection = (COLORREF)-1;
	m_nStyle = -1;

	m_ptControlPoint = CPoint(-1, -1);

	m_rcBoundingRect.SetRectEmpty();

	m_pPath = NULL;

	m_pNextVisibleConnection = NULL;

	m_nInputPointConnectionIndex = -1;
	m_nOutputPointConnectionIndex = -1;
	m_nConnectionIndex = -1;

}

CXTPFlowGraphConnection::~CXTPFlowGraphConnection()
{
	CMDTARGET_RELEASE(m_pInputPoint);
	CMDTARGET_RELEASE(m_pOutputPoint);

	SAFE_DELETE(m_pPath);
}

CXTPFlowGraphControl* CXTPFlowGraphConnection::GetControl() const
{
	if (!m_pPage)
		return NULL;
	return m_pPage->GetControl();
}


void CXTPFlowGraphConnection::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));
	PX_DWord(pPX, _T("Color"), (DWORD)m_clrConnection, (COLORREF)-1);
	PX_Int(pPX, _T("Style"), m_nStyle, (int)-1);

	PX_Point(pPX, _T("ControlPoint"), m_ptControlPoint, CPoint(-1, -1));

	if (pPX->IsStoring())
	{
		m_nInputId = m_pInputPoint ? m_pInputPoint->GetID() : 0;
		PX_Int(pPX, _T("Input"), m_nInputId, 0);

		m_nOutputId = m_pOutputPoint ? m_pOutputPoint->GetID() : 0;
		PX_Int(pPX, _T("Output"), m_nOutputId, 0);
	}
	else
	{
		PX_Int(pPX, _T("Input"), m_nInputId, 0);
		PX_Int(pPX, _T("Output"), m_nOutputId, 0);
	}
}

void CXTPFlowGraphConnection::SetControlPoint(CPoint pt)
{
	if (m_ptControlPoint == pt)
		return;

	GetControl()->GetUndoManager()->AddUndoCommand(new CXTPFlowGraphUndoSetConnectionPositionCommand(this, m_ptControlPoint));

	m_ptControlPoint = pt;
	OnGraphChanged();
}

void CXTPFlowGraphConnection::RestoreConnection()
{
	ASSERT(m_pInputPoint == NULL && m_pOutputPoint == NULL);
	ASSERT(m_pPage != NULL);

	for (int i = 0; i < m_pPage->GetNodes()->GetCount(); i++)
	{
		CXTPFlowGraphNode* pNode = m_pPage->GetNodes()->GetAt(i);

		for (int j = 0; j < pNode->GetConnectionPoints()->GetCount(); j++)
		{
			CXTPFlowGraphConnectionPoint* pPoint =  pNode->GetConnectionPoints()->GetAt(j);

			if (pPoint->GetID() == m_nInputId)
			{
				SetInputPoint(pPoint);
			}

			if (pPoint->GetID() == m_nOutputId)
			{
				SetOutputPoint(pPoint);
			}
		}
	}
}


void CXTPFlowGraphConnection::RecalcLayout(CXTPFlowGraphDrawContext* pDC)
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (!pControl)
		return;

	pControl->GetPaintManager()->RecalcConnectionLayout(pDC, this);

	Gdiplus::Rect rcBounds;
	m_pPath->GetBounds(&rcBounds);

	m_rcBoundingRect.SetRect(rcBounds.GetLeft(), rcBounds.GetTop(), rcBounds.GetRight(), rcBounds.GetBottom());
}

CRect CXTPFlowGraphConnection::GetBoundingRect()
{
	return m_rcBoundingRect;

}

BOOL CXTPFlowGraphConnection::PtInRect(POINT pt)
{
	if (!m_pPath)
		return FALSE;

	double x = pt.x;
	double y = pt.y;
	m_pPage->ScreenToPage(x, y);

	Gdiplus::Pen* pPen = new Gdiplus::Pen(0xFF000000, (float)3);

	BOOL res = m_pPath->IsOutlineVisible((float)x, (float)y, pPen);

	delete pPen;

	return res;
}

void CXTPFlowGraphConnection::Remove()
{
	if (!m_pPage)
		return;

	m_pPage->GetConnections()->Remove(this);
}

void CXTPFlowGraphConnection::OnRemoved()
{
	SAFE_DELETE(m_pPath);

	SetInputPoint(NULL);
	SetOutputPoint(NULL);

	CXTPFlowGraphElement::OnRemoved();
}

void CXTPFlowGraphConnection::SetInputPoint(CXTPFlowGraphConnectionPoint* pPoint)
{
	if (m_pInputPoint == pPoint)
		return;

	if (m_pInputPoint)
	{
		CArray<CXTPFlowGraphConnection*, CXTPFlowGraphConnection*>& arrInputConnections = m_pInputPoint->m_arrInputConnections;

		int nCount = (int)arrInputConnections.GetSize();

		ASSERT(m_nInputPointConnectionIndex != -1);
		ASSERT(arrInputConnections[m_nInputPointConnectionIndex] == this);

		if (m_nInputPointConnectionIndex != nCount - 1)
		{
			arrInputConnections[m_nInputPointConnectionIndex] = arrInputConnections[nCount - 1];

			arrInputConnections[m_nInputPointConnectionIndex]->m_nInputPointConnectionIndex = m_nInputPointConnectionIndex;
		}

		arrInputConnections.SetSize(nCount - 1);

		m_nInputPointConnectionIndex = -1;
		CMDTARGET_RELEASE(m_pInputPoint);
	}

	m_pInputPoint = pPoint;

	if (pPoint)
	{
		m_nInputPointConnectionIndex = (int)pPoint->m_arrInputConnections.Add(this);

		CMDTARGET_ADDREF(pPoint);
	}

	OnGraphChanged();
}

void CXTPFlowGraphConnection::SetOutputPoint(CXTPFlowGraphConnectionPoint* pPoint)
{
	if (m_pOutputPoint == pPoint)
		return;

	if (m_pOutputPoint)
	{
		CArray<CXTPFlowGraphConnection*, CXTPFlowGraphConnection*>& arrOutputConnections = m_pOutputPoint->m_arrOutputConnections;

		int nCount = (int)arrOutputConnections.GetSize();

		ASSERT(m_nOutputPointConnectionIndex != -1);
		ASSERT(arrOutputConnections[m_nOutputPointConnectionIndex] == this);

		if (m_nOutputPointConnectionIndex != nCount - 1)
		{
			arrOutputConnections[m_nOutputPointConnectionIndex] = arrOutputConnections[nCount - 1];

			arrOutputConnections[m_nOutputPointConnectionIndex]->m_nOutputPointConnectionIndex = m_nOutputPointConnectionIndex;
		}

		arrOutputConnections.SetSize(nCount - 1);

		m_nOutputPointConnectionIndex = -1;
		CMDTARGET_RELEASE(m_pOutputPoint);
	}

	m_pOutputPoint = pPoint;

	if (pPoint)
	{
		m_nOutputPointConnectionIndex = (int)pPoint->m_arrOutputConnections.Add(this);

		CMDTARGET_ADDREF(pPoint);
	}

	OnGraphChanged();
}

CXTPFlowGraphNode* CXTPFlowGraphConnection::GetInputNode() const
{
	if (m_pInputPoint)
		return m_pInputPoint->GetNode();
	return NULL;
}

CXTPFlowGraphNode* CXTPFlowGraphConnection::GetOutputNode() const
{
	if (m_pOutputPoint)
		return m_pOutputPoint->GetNode();
	return NULL;
}


