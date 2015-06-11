// XTPTaskPanelExplorerThemePlain.cpp : implementation of the CXTPTaskPanelExplorerThemePlain class.
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

#include "XTPTaskPanelPaintManagerPlain.h"
#include "XTPTaskPanelExplorerThemePlain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelExplorerThemePlain

CXTPTaskPanelExplorerThemePlain::CXTPTaskPanelExplorerThemePlain()
{
	m_bOfficeHighlight = XTP_TASKPANEL_HIGHLIGHT_OFFICE2003;
	m_eGripper = xtpTaskPanelGripperBitmap;
}

CRect CXTPTaskPanelExplorerThemePlain::DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (!m_bExplorerTheme)
		return DrawCaptionGripper(pDC, pGroup, rc);

	if (!pGroup->IsExpandable())
		return rc;

	if (rc.Width() < 20 + 6)
		return rc;


	return DrawCaptionGripperBitmap(pDC, pGroup, pGroup->IsExpanded(), pGroup->IsItemHot(), rc);
}

void CXTPTaskPanelExplorerThemePlain::RefreshMetrics()
{

	CXTPTaskPanelPaintManagerPlain::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

	m_bExplorerTheme = FALSE;
	if (!m_bEmulateEplorerTheme && (systemTheme == xtpSystemThemeUnknown))
	{
		m_eGripper = xtpTaskPanelGripperPlain;
		return ;
	}
	m_bExplorerTheme = TRUE;

	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupSpecial.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
		{
			m_clrBackground.SetStandardValue(RGB(123, 162, 231), RGB(99, 117, 214));
			m_groupNormal.clrClient.SetStandardValue(RGB(214, 223, 247));

			m_groupNormal.clrClientLink.SetStandardValue(RGB(33, 93, 198));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(66, 142, 255));
			break;

		}
		case xtpSystemThemeOlive:
		{
			m_clrBackground.SetStandardValue(RGB(204, 217, 173), RGB(165, 189, 132));
			m_groupNormal.clrClient.SetStandardValue(RGB(246, 246, 236));

			m_groupNormal.clrClientLink.SetStandardValue(RGB(86, 102, 45));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(114, 146, 29));

			break;
		}
		case xtpSystemThemeSilver:
		{
			m_clrBackground.SetStandardValue(RGB(196, 200, 212), RGB(177, 179, 200));
			m_groupNormal.clrClient.SetStandardValue(RGB(240, 241, 245));

			m_groupNormal.clrClientLink.SetStandardValue(RGB(63, 61, 61));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(126, 124, 124));

			break;
		}
		default:
		{
			m_clrBackground.SetStandardValue(
				BlendColor(GetXtremeColor(COLOR_INACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 128),
				BlendColor(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 192));


			COLORREF clrClient = BlendColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 24);
			m_groupNormal.clrClient.SetStandardValue(BlendColor(clrClient, GetXtremeColor(COLOR_ACTIVECAPTION), 230));

			m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

		}
	}
	m_groupSpecial = m_groupNormal;

	CreateGripperBitmaps();
}
