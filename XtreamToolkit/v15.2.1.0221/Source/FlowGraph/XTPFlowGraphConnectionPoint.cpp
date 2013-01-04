// XTPFlowGraphConnectionPoint.cpp : implementation of the CXTPFlowGraphConnectionPoint class.
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
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphImage.h"
#include "XTPFlowGraphUndoManager.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnectionPoints.h"

IMPLEMENT_SERIAL(CXTPFlowGraphConnectionPoint, CCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)


CXTPFlowGraphConnectionPoint::CXTPFlowGraphConnectionPoint()
{
	m_nStyle = -1;
	m_clrPoint = (COLORREF)-1;

	m_nId = (int)(INT_PTR)this;

	m_nType = xtpFlowGraphPointInput;

	m_bLocked = FALSE;

	m_nImageIndex = -1;

	m_pNode = NULL;

	m_nMaxConnections = -1;

	m_pMarkupUIElement = NULL;

	m_rcPoint.SetRectEmpty();

	m_nConnectionPointIndex = -1;

}

CXTPFlowGraphConnectionPoint::~CXTPFlowGraphConnectionPoint()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPFlowGraphConnectionPoint::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));
	PX_DWord(pPX, _T("Color"), (DWORD)m_clrPoint, (COLORREF)-1);
	PX_Int(pPX, _T("Style"), m_nStyle, (int)-1);
	PX_Enum(pPX, _T("Type"), m_nType, xtpFlowGraphPointInput);

	PX_Int(pPX, _T("Id"), m_nId, 0);

	PX_Bool(pPX, _T("Locked"), m_bLocked, FALSE);

	PX_Int(pPX, _T("MaxConnections"), m_nMaxConnections, -1);

	PX_Int(pPX, _T("ImageIndex"), m_nImageIndex, -1);

	if (pPX->IsLoading())
	{
		CXTPFlowGraphControl* pControl = GetControl();
		if (pControl)
		{
			m_pMarkupUIElement = XTPMarkupParseText(pControl->GetMarkupContext(), m_strCaption);
		}
	}
}

CXTPFlowGraphControl* CXTPFlowGraphConnectionPoint::GetControl() const
{
	if (m_pNode)
		return m_pNode->GetControl();
	return NULL;
}

CXTPFlowGraphPage* CXTPFlowGraphConnectionPoint::GetPage() const
{
	if (m_pNode)
		return m_pNode->GetPage();
	return NULL;
}

void CXTPFlowGraphConnectionPoint::Remove()
{
	if (!m_pNode)
		return;

	m_pNode->GetConnectionPoints()->Remove(this);
}

BOOL CXTPFlowGraphConnectionPoint::HitTestConnectionArea(CPoint point) const
{
	CXTPFlowGraphPaintManager* pPaintManager = GetControl()->GetPaintManager();

	return pPaintManager->HitTestConnectionArea(this, point);
}

CXTPFlowGraphImage* CXTPFlowGraphConnectionPoint::GetImage() const
{
	CXTPFlowGraphControl* pControl = GetControl();
	if (!pControl)
		return NULL;

	return pControl->GetImages()->GetAt(m_nImageIndex);
}

void CXTPFlowGraphConnectionPoint::OnGraphChanged()
{
	if (m_pNode) m_pNode->OnGraphChanged();
}

void CXTPFlowGraphConnectionPoint::SetCaption(LPCTSTR lpszCaption)
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


void CXTPFlowGraphConnectionPoint::OnRemoved()
{
	int i;

	for (i = (int)m_arrInputConnections.GetSize() - 1; i >= 0; i--)
	{
		m_arrInputConnections[i]->Remove();
	}

	for (i = (int)m_arrOutputConnections.GetSize() - 1; i >= 0; i--)
	{
		m_arrOutputConnections[i]->Remove();
	}

	// Removed Connections should update them
	ASSERT(m_arrInputConnections.GetSize() == 0);
	ASSERT(m_arrOutputConnections.GetSize() == 0);

	CXTPFlowGraphElement::OnRemoved();
}



