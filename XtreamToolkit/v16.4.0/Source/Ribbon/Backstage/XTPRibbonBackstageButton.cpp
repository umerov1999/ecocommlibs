// XTPRibbonBackstageButton.cpp : implementation file
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Button/XTPButtonTheme.h>
#include <Controls/Button/Themes/XTPButtonThemeUltraFlat.h>
#include <Controls/Button/Themes/XTPButtonThemeResource.h>
#include <Controls/Button/Themes/XTPButtonThemeOffice2013.h>

#include <Ribbon/Backstage/XTPRibbonBackstageButton.h>
#include <Ribbon/Backstage/XTPRibbonBackstageButtonThemeResource.h>
#include <Ribbon/Backstage/XTPRibbonBackstageButtonThemeOffice2013.h>

#ifndef _XTP_ACTIVEX_BUTTON
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


//////////////////////////////////////////////////////////////////////////
//  CXTPRibbonBackstageButton

#ifndef _XTP_ACTIVEX_BUTTON
IMPLEMENT_DYNCREATE(CXTPRibbonBackstageButton, CXTPButton)
#endif

CXTPRibbonBackstageButton::CXTPRibbonBackstageButton()
{
	SetTheme(xtpControlThemeResource);
}

CXTPRibbonBackstageButton::~CXTPRibbonBackstageButton()
{
}

BOOL CXTPRibbonBackstageButton::SetTheme(XTPControlTheme theme)
{
	switch(theme)
	{
	case xtpControlThemeResource:
		CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeResource());
		break;
	case xtpControlThemeOffice2013:
		CXTPButton::SetTheme(new CXTPRibbonBackstageButtonThemeOffice2013());
		break;
	default:
		CXTPButton::SetTheme(theme);
		break;
	}

	return TRUE;
}

void CXTPRibbonBackstageButton::SetTabStyle(BOOL bTabStyle)
{
	m_bTabStyle = bTabStyle;
}

BEGIN_MESSAGE_MAP(CXTPRibbonBackstageButton, CXTPButton)
//{{AFX_MSG_MAP(CXTPRibbonBackstageSeparator)
	ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPRibbonBackstageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPButton::OnLButtonDown(nFlags, point);

}
