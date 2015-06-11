// XTPTaskPanelOffice2003Theme.cpp : implementation of the CXTPTaskPanelOffice2003Theme class.
//
// This file is a part of the XTREME TASKPANEL MFC class library.
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
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>

#include <TaskPanel/Resource.h>
#include <TaskPanel/XTPTaskPanelDefines.h>
#include <TaskPanel/XTPTaskPanelItem.h>
#include <TaskPanel/XTPTaskPanelGroup.h>
#include <TaskPanel/XTPTaskPanelGroupItem.h>
#include <TaskPanel/XTPTaskPanelItems.h>
#include <TaskPanel/XTPTaskPanelPaintManager.h>

#include "XTPTaskPanelOffice2003Theme.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelOffice2003Theme

CXTPTaskPanelOffice2003Theme::CXTPTaskPanelOffice2003Theme()
{
	m_bOfficeHighlight = XTP_TASKPANEL_HIGHLIGHT_OFFICE2003;
	m_rcControlMargins = CRect(8, 8, 0, 8);
	m_nGroupSpacing = 15;
	m_eGripper = xtpTaskPanelGripperNone;
}

void CXTPTaskPanelOffice2003Theme::RefreshMetrics()
{
	CXTPTaskPanelPaintManager::RefreshMetrics();

	if (m_bUseStandardCaptionFont)
	{
		m_fntCaption.DeleteObject();

		LOGFONT lfIcon;
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

		lfIcon.lfUnderline = FALSE;
		lfIcon.lfWeight = FW_BOLD;
		lfIcon.lfHeight = 18;

		CString strOfficeFont(_T("Tahoma"));
		if (CXTPDrawHelpers::FontExists(strOfficeFont))
		{
			STRCPY_S(lfIcon.lfFaceName, LF_FACESIZE, strOfficeFont);
		}
		m_fntCaption.CreateFontIndirect(&lfIcon);

		CWindowDC dc(CWnd::GetDesktopWindow());
		CXTPFontDC fnt(&dc, &m_fntCaption);
		m_nCaptionHeight = max(20, dc.GetTextExtent(_T(" "), 1).cy);
	}


	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:

			m_clrBackground.SetStandardValue(RGB(221, 236, 254), RGB(74, 122, 201));
			m_groupNormal.clrHead.SetStandardValue(RGB(196, 219, 249), RGB(101, 143, 224));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(0, 45, 134));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(0, 45, 134));
			m_groupNormal.clrClient.SetStandardValue(RGB(221, 236, 254));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 61, 178));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 61, 178));
			break;

		case xtpSystemThemeOlive:

			m_clrBackground.SetStandardValue(RGB(243, 242, 231), RGB(190, 198, 152));
			m_groupNormal.clrHead.SetStandardValue(RGB(210, 223, 174), RGB(161, 176, 128));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(90, 107, 70));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(90, 107, 70));
			m_groupNormal.clrClient.SetStandardValue(RGB(243, 242, 231));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 61, 178));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 61, 178));
			break;

		case xtpSystemThemeSilver:

			m_clrBackground.SetStandardValue(RGB(238, 238, 244), RGB(177, 176, 195));
			m_groupNormal.clrHead.SetStandardValue(RGB(207, 207, 222), RGB(169, 168, 191));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(92, 91, 121));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(92, 91, 121));
			m_groupNormal.clrClient.SetStandardValue(RGB(238, 238, 244));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 61, 178));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 61, 178));
			break;

		default:
			m_clrBackground.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 50), GetXtremeColor(COLOR_3DFACE));

			m_groupNormal.clrHead.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 39),
				XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 70));

			m_groupNormal.clrHeadTextHot.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupNormal.clrClient.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 49));

			m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 0, 0xFF));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 0, 0xFF));
	}
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));



	m_groupSpecial = m_groupNormal;


	m_bLeftRoundedCorner = TRUE;
	m_bRightRoundedCorner = FALSE;
	m_bOfficeBorder = TRUE;
}
