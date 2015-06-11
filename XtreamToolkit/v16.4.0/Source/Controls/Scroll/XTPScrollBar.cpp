// XTPScrollBar.cpp : implementation of the CXTPScrollBar class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include <Common/XTPColorManager.h>

// Scroll bar
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Scroll/XTPScrollBar.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPScrollBar, CXTPScrollBarCtrl)

CXTPScrollBar::CXTPScrollBar()
	: m_nTheme(xtpControlThemeDefault)
	, m_bUseVisualStyle(TRUE)
{
	SetTheme(m_nTheme);
}

CXTPScrollBar::~CXTPScrollBar()
{
}

void CXTPScrollBar::SetCommandBars(void*)
{
	TRACE(_T("WARNING: CXTPScrollBar renamed to CXTPCommandBarScrollBarCtrl\n"));
}

void CXTPScrollBar::SetTheme(XTPControlTheme nTheme)
{
	switch(nTheme)
	{
	case xtpControlThemeDefault:
		if (m_bUseVisualStyle)
		{
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeWindowsDefault);
		}
		else
		{
			CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeWindowsClassic);
		}
		break;
	case xtpControlThemeFlat:
		break;
	case xtpControlThemeUltraFlat:
		break;
	case xtpControlThemeOffice2000:
		CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeOffice2000);
		break;
	case xtpControlThemeOfficeXP:
		CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeOfficeXP);
		break;
	case xtpControlThemeResource:
		break;
	case xtpControlThemeVisualStudio2012Light:
		CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeVisualStudio2012Light);
		break;
	case xtpControlThemeVisualStudio2012Dark:
		CXTPScrollBarCtrl::SetTheme(xtpScrollBarThemeVisualStudio2012Dark);
		break;
	}

	RedrawScrollBar();
}

void CXTPScrollBar::SetUseVisualStyle(BOOL bUseVisualStyle)
{
	m_bUseVisualStyle = bUseVisualStyle;
	SetTheme(m_nTheme);
}
