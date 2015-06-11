// XTPTaskPanelExplorerTheme.cpp : implementation of the CXTPTaskPanelExplorerTheme class.
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

#include "XTPTaskPanelExplorerTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelExplorerTheme

CXTPTaskPanelExplorerTheme::CXTPTaskPanelExplorerTheme()
{
	m_bOfficeHighlight = XTP_TASKPANEL_HIGHLIGHT_OFFICE2003;
	m_eGripper = xtpTaskPanelGripperBitmap;
	m_bExplorerTheme = FALSE;
}

void CXTPTaskPanelExplorerTheme::RefreshMetrics()
{
	CXTPTaskPanelPaintManager::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

	m_bExplorerTheme = FALSE;

	if (!m_bEmulateEplorerTheme && (systemTheme == xtpSystemThemeUnknown))
	{
		m_eGripper = xtpTaskPanelGripperPlain;
		return ;
	}
	m_bExplorerTheme = TRUE;

	m_bRightRoundedCorner = m_bLeftRoundedCorner = TRUE;
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupSpecial.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
		{
			m_clrBackground.SetStandardValue(RGB(123, 162, 231), RGB(99, 117, 214));

			m_groupSpecial.clrHead.SetStandardValue(RGB(0, 73, 181), RGB(41, 93, 206));
			m_groupSpecial.clrHeadTextNormal.SetStandardValue(RGB(255, 255, 255));
			m_groupSpecial.clrHeadTextHot.SetStandardValue(RGB(66, 142, 255));
			m_groupSpecial.clrClient.SetStandardValue(RGB(239, 243, 255));

			m_groupNormal.clrHead.SetStandardValue(RGB(255, 255, 255), RGB(198, 211, 247));
			m_groupNormal.dHeadGradientFactor = 0.5;
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(33, 93, 198));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(66, 142, 255));
			m_groupNormal.clrClient.SetStandardValue(RGB(214, 223, 247));

			m_groupSpecial.clrClientLink.SetStandardValue(RGB(33, 93, 198));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(33, 93, 198));
			m_groupSpecial.clrClientLinkHot.SetStandardValue(RGB(66, 142, 255));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(66, 142, 255));

			break;

		}
		case xtpSystemThemeOlive:
		{
			m_clrBackground.SetStandardValue(RGB(204, 217, 173), RGB(165, 189, 132));

			m_groupSpecial.clrHead.SetStandardValue(RGB(119, 140, 64), RGB(150, 168, 103));
			m_groupSpecial.clrHeadTextNormal.SetStandardValue(RGB(255, 255, 255));
			m_groupSpecial.clrHeadTextHot.SetStandardValue(RGB(224, 231, 184));
			m_groupSpecial.clrClient.SetStandardValue(RGB(246, 246, 236));

			m_groupNormal.clrHead.SetStandardValue(RGB(255, 252, 236), RGB(224, 231, 184));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(86, 102, 45));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(114, 146, 29));
			m_groupNormal.clrClient.SetStandardValue(RGB(246, 246, 236));

			m_groupSpecial.clrClientLink.SetStandardValue(RGB(86, 102, 45));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(86, 102, 45));
			m_groupSpecial.clrClientLinkHot.SetStandardValue(RGB(114, 146, 29));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(114, 146, 29));

			break;
		}
		case xtpSystemThemeSilver:
		{
			m_clrBackground.SetStandardValue(RGB(196, 200, 212), RGB(177, 179, 200));

			m_groupSpecial.clrHead.SetStandardValue(RGB(119, 119, 146), RGB(180, 182, 199));
			m_groupSpecial.clrHeadTextNormal.SetStandardValue(RGB(255, 255, 255));
			m_groupSpecial.clrHeadTextHot.SetStandardValue(RGB(230, 230, 230));
			m_groupSpecial.clrClient.SetStandardValue(RGB(240, 241, 245));

			m_groupNormal.clrHead.SetStandardValue(RGB(255, 255, 255), RGB(214, 215, 224));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(63, 61, 61));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(126, 124, 124));
			m_groupNormal.clrClient.SetStandardValue(RGB(240, 241, 245));

			m_groupSpecial.clrClientLink.SetStandardValue(RGB(63, 61, 61));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(63, 61, 61));
			m_groupSpecial.clrClientLinkHot.SetStandardValue(RGB(126, 124, 124));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(126, 124, 124));

			break;
		}
		default:
		{
			m_clrBackground.SetStandardValue(
				BlendColor(GetXtremeColor(COLOR_INACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 128),
				BlendColor(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 192));

			COLORREF clrClient = BlendColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 24);

			m_groupSpecial.clrHead.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION), BlendColor(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 92));

			m_groupSpecial.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
			m_groupSpecial.clrHeadTextHot.SetStandardValue(BlendColor(GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_HIGHLIGHT), 128));
			m_groupSpecial.clrClient.SetStandardValue(clrClient);


			m_groupNormal.clrHead.SetStandardValue(GetXtremeColor(COLOR_WINDOW), BlendColor(GetXtremeColor(COLOR_INACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 92));

			m_groupNormal.clrHeadTextNormal.SetStandardValue(BlendColor(GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_BTNTEXT), 192));
			m_groupNormal.clrHeadTextHot.SetStandardValue(BlendColor(GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_BTNTEXT), 240));
			m_groupNormal.clrClient.SetStandardValue(BlendColor(clrClient, GetXtremeColor(COLOR_ACTIVECAPTION), 230));

			m_groupSpecial.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupSpecial.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
			m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

		}
	}

	CreateGripperBitmaps();

}
