// XTPFlowGraphElement.cpp : implementation of the CXTPFlowGraphElement class.
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

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphSelectedElements.h"

IMPLEMENT_DYNAMIC(CXTPFlowGraphElement, CCmdTarget)


CXTPFlowGraphElement::CXTPFlowGraphElement()
{
	m_bSelected = FALSE;
	m_bDirty = TRUE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
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




#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPFlowGraphElement, CCmdTarget)
END_DISPATCH_MAP()

// {12353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphElement =
{ 0x12353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphElement, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphElement, IID_IFlowGraphElement, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphElement, IID_IFlowGraphElement)

#endif
