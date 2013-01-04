// XTPFlowGraphElement.cpp : implementation of the CXTPFlowGraphElement class.
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
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphSelectedElements.h"

IMPLEMENT_DYNAMIC(CXTPFlowGraphElement, CCmdTarget)


CXTPFlowGraphElement::CXTPFlowGraphElement()
{
	m_bSelected = FALSE;
	m_bDirty = TRUE;

}

CXTPFlowGraphElement::~CXTPFlowGraphElement()
{

}


BOOL CXTPFlowGraphElement::IsSelected() const
{
	return m_bSelected;
}

void CXTPFlowGraphElement::OnGraphChanged()
{
	m_bDirty = TRUE;

	CXTPFlowGraphControl* pControl = GetControl();
	if (pControl) pControl->OnGraphChanged();
}

void CXTPFlowGraphElement::OnRemoved()
{
	if (IsSelected())
	{
		GetPage()->GetSelection()->Remove(this);
	}
}




