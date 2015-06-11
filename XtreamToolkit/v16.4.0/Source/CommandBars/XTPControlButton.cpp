// XTPControlButton.cpp : implementation of the CXTPControlButton class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"

#include "XTPCommandBarsDefines.h"
#include "XTPControl.h"
#include "XTPControlButton.h"
#include "XTPCommandBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPControlButton

IMPLEMENT_XTP_CONTROL(CXTPControlButton, CXTPControl)

CXTPControlButton::CXTPControlButton()
{
	EnableAutomation();

	m_controlType = xtpControlButton;
}

CXTPControlButton::~CXTPControlButton()
{
}

void CXTPControlButton::OnLButtonUp(CPoint /*point*/)
{
	if (GetEnabled() && (m_pParent->GetType() == xtpBarTypePopup || m_pParent->GetPosition() == xtpBarPopup))
	{
		OnExecute();
	}
}

void CXTPControlButton::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!GetEnabled()) return;

	if (IsCustomizeMode())
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(-1);
		CustomizeStartDrag(pt);
		return;
	}

	if (!bKeyboard)
	{
		if (m_pParent->GetType() != xtpBarTypePopup || m_nExecuteOnPressInterval > 0)
			ClickToolBarButton();
	}
	else
	{
		OnExecute();
	}
}

void CXTPControlButton::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT(DYNAMIC_DOWNCAST(CXTPControlButton, pControl));

	CXTPControl::Copy(pControl, bRecursive);
}






#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPControlButton, CXTPControl)
END_DISPATCH_MAP()

// {324B467B-1E0F-446d-B92B-BEDBEF77302F}
static const GUID IID_ICommandBarButton =
{ 0x324b467b, 0x1e0f, 0x446d, { 0xb9, 0x2b, 0xbe, 0xdb, 0xef, 0x77, 0x30, 0x2f } };

BEGIN_INTERFACE_MAP(CXTPControlButton, CXTPControl)
	INTERFACE_PART(CXTPControlButton, IID_ICommandBarButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlButton, IID_ICommandBarButton)

#endif
