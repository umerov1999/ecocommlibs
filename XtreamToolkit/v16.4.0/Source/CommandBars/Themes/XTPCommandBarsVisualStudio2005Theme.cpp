// XTPOffice2003Theme.cpp : implementation of the CXTPOffice2003Theme class.
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

#include <Common/XTPVC80Helpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/ScrollBar/XTPScrollBase.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPDockBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPDialogBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/Themes/XTPCommandBarsVisualStudio2005Theme.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPVisualStudio2005Theme

IMPLEMENT_DYNAMIC(CXTPVisualStudio2005Theme, CXTPOffice2003Theme)

CXTPVisualStudio2005Theme::CXTPVisualStudio2005Theme()
{
	m_systemTheme = xtpSystemThemeUnknown;
	m_bPlainSeparators = TRUE;
}

CXTPVisualStudio2005Theme::~CXTPVisualStudio2005Theme()
{

}

void CXTPVisualStudio2005Theme::RefreshMetrics()
{
	CXTPOffice2003Theme::RefreshMetrics();

	m_clrToolbarExpand.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DFACE), 550),
		GetXtremeColor(COLOR_3DSHADOW));

	if (XTPColorManager()->IsLowResolution())
	{
		m_clrToolbarExpand.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	}

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
		m_clrCommandBar.SetStandardValue(RGB(250, 249, 245), RGB(192, 192, 168), 0.75f);
		m_clrToolbarExpand.SetStandardValue(RGB(238, 237, 234), RGB(152, 152, 126), 0.75f);
		m_clrDockBar.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));
		m_clrToolbarShadow.SetStandardValue(RGB(163, 163, 124));
		m_clrMenuExpandedGripper.SetStandardValue(RGB(204, 199, 186));
		m_clrMenuExpand.SetStandardValue(m_clrCommandBar);
		m_clrDisabledIcon.SetStandardValue(RGB(247, 245, 249), RGB(122, 121, 153));
		break;

	case xtpSystemThemeOlive:
		m_clrCommandBar.SetStandardValue(RGB(250, 249, 245), RGB(192, 192, 168), 0.75f);
		m_clrToolbarExpand.SetStandardValue(RGB(238, 237, 234), RGB(176, 172, 158), 0.75f);
		m_clrDockBar.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));
		m_clrToolbarShadow.SetStandardValue(RGB(163, 163, 124));
		m_clrMenuExpandedGripper.SetStandardValue(RGB(204, 199, 186));
		m_clrMenuExpand.SetStandardValue(m_clrCommandBar);
		m_clrDisabledIcon.SetStandardValue(RGB(236, 233, 216), RGB(172, 168, 153));

		m_arrColor[XPCOLOR_HIGHLIGHT].SetStandardValue(RGB(182, 198, 141));
		m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED].SetStandardValue(RGB(147, 160, 112));
		m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED].SetStandardValue(RGB(182, 198, 141));
		m_arrColor[XPCOLOR_PUSHED_TEXT].SetStandardValue(RGB(255, 255, 255));
		break;

	case xtpSystemThemeSilver:
		m_clrCommandBar.SetStandardValue(RGB(243, 244, 250), RGB(153, 151, 181), 0.75f);
		m_clrToolbarExpand.SetStandardValue(RGB(179, 178, 200), RGB(118, 116, 146), 0.75f);
		m_clrDockBar.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));
		m_clrMenuExpandedGripper.SetStandardValue(RGB(215, 215, 226), RGB(133, 131, 162));
		m_clrMenuExpand.SetStandardValue(0xF1E8E8, 0xC5AEB0);
		m_clrToolbarShadow.SetStandardValue(RGB(124, 124, 148));
		m_clrDisabledIcon.SetStandardValue(RGB(247, 245, 249), RGB(122, 121, 153));

		m_arrColor[XPCOLOR_TOOLBAR_GRAYTEXT].SetStandardValue(RGB(141, 141, 141));
		m_arrColor[XPCOLOR_MENUBAR_GRAYTEXT].SetStandardValue(RGB(141, 141, 141));
		m_arrColor[XPCOLOR_SEPARATOR].SetStandardValue(RGB(110, 109, 143));
		m_arrColor[XPCOLOR_TOOLBAR_GRIPPER].SetStandardValue(RGB(84, 84, 117));
		m_arrColor[XPCOLOR_TOOLBAR_FACE].SetStandardValue(RGB(219, 218, 228));
		m_clrTearOffGripper.SetStandardValue(RGB(192, 192, 211));
		m_clrFloatingGripper.SetStandardValue(RGB(122, 121, 153));
		m_arrColor[XPCOLOR_FLOATBAR_BORDER].SetStandardValue(RGB(122, 121, 153));

		INT nElements[] =
		{
			XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_PUSHED_TEXT
		};
		COLORREF clrValues[] =
		{
			RGB(255, 238, 194), RGB(75, 75, 111), RGB(75, 75, 111), RGB(75, 75, 111), RGB(254, 128, 62), RGB(255, 192, 111), 0
		};
		SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

		break;
	}

	m_clrMenuGripper.SetStandardValue(m_clrCommandBar);

	CreateGradientCircle();
}
