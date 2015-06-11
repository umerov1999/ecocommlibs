// XTPRibbonBackstageTab.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include <Common/XTPSystemHelpers.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <Ribbon/Backstage/XTPRibbonBackstageTab.h>

IMPLEMENT_XTP_CONTROL(CXTPRibbonBackstageTab, CXTPControl)

CXTPRibbonBackstageTab::CXTPRibbonBackstageTab(HWND hwndControl)
{
	m_szControlPaneMinSize = CSize(0, 0);

	m_hwndControl = hwndControl;
}

CWnd* CXTPRibbonBackstageTab::GetControlPane() const
{
	return CWnd::FromHandle(m_hwndControl);
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonBackstageTab, CXTPControl)
END_DISPATCH_MAP()

// {66BE0F75-CB1F-40fd-B073-4BC151A5205F}
static const GUID IID_IRibbonBackstageTab =
{ 0x66be0f75, 0xcb1f, 0x40fd, { 0xb0, 0x73, 0x4b, 0xc1, 0x51, 0xa5, 0x20, 0x5f } };

BEGIN_INTERFACE_MAP(CXTPRibbonBackstageTab, CXTPControl)
	INTERFACE_PART(CXTPRibbonBackstageTab, IID_IRibbonBackstageTab, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonBackstageTab, IID_IRibbonBackstageTab)

#endif // _XTP_ACTIVEX
