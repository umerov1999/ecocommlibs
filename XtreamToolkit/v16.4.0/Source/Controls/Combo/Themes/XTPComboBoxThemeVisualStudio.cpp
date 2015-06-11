// XTPComboBoxThemeVisualStudio.cpp: implementation of the CXTPComboBoxThemeVisualStudio class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE E2003RESSED WRITTEN
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
#include "math.h"

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Combo/XTPComboBox.h>
#include <Controls/Combo/XTPComboBoxTheme.h>

#include "XTPComboBoxThemeOffice2003.h"
#include "XTPComboBoxThemeVisualStudio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeVisualStudio::CXTPComboBoxThemeVisualStudio()
{

}

CXTPComboBoxThemeVisualStudio::~CXTPComboBoxThemeVisualStudio()
{

}

void CXTPComboBoxThemeVisualStudio::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	m_clrButtonHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT)); // XPCOLOR_HIGHLIGHT_BORDER
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));

	m_clrButtonArrowHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_clrButtonArrowPressed.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));

	m_grcButton.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 40), GetXtremeColor(COLOR_3DFACE));

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
	case xtpSystemThemeRoyale:
		{
			m_grcButtonHilite.SetStandardValue(RGB(193, 210, 238));
			m_grcButtonPushed.SetStandardValue(RGB(152, 181, 226));
		}
		break;

	case xtpSystemThemeAero:
		{
			m_grcButtonHilite.SetStandardValue(RGB(228, 245, 252), RGB(124, 177, 205));
			m_grcButtonPushed.SetStandardValue(RGB(237, 248, 253), RGB(129, 192, 224));
		}
		break;

	case xtpSystemThemeOlive:
		{
			m_grcButtonHilite.SetStandardValue(RGB(182, 198, 141));
			m_grcButtonPushed.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
		}
		break;

	case xtpSystemThemeSilver:
		{
			m_grcButtonHilite.SetStandardValue(RGB(255, 242, 200), RGB(255, 210, 148));
			m_grcButtonPushed.SetStandardValue(RGB(254, 149, 82), RGB(255, 207, 139));
		}
		break;

	default:
		{
			m_grcButtonHilite.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_WINDOW), 30));
			m_grcButtonPushed.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_WINDOW), 50));
		}
		break;
	}


}