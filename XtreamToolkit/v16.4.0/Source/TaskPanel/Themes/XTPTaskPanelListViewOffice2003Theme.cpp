// XTPTaskPanelListViewOffice2003Theme.cpp : implementation of the CXTPTaskPanelListViewOffice2003Theme class.
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
#include "XTPTaskPanelToolboxTheme.h"
#include "XTPTaskPanelListViewTheme.h"
#include "XTPTaskPanelListViewOfficeXPTheme.h"
#include "XTPTaskPanelListViewOffice2003Theme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelListViewOffice2003Theme

CXTPTaskPanelListViewOffice2003Theme::CXTPTaskPanelListViewOffice2003Theme()
{
	m_bOfficeHighlight = XTP_TASKPANEL_HIGHLIGHT_OFFICE2003;
	m_bEmbossedDisabledText = FALSE;
}

void CXTPTaskPanelListViewOffice2003Theme::RefreshMetrics()
{
	CXTPTaskPanelListViewOfficeXPTheme::RefreshMetrics();

	m_grcPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_grcHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_clrBackground.SetStandardValue(RGB(216, 231, 252));
			m_groupNormal.clrClient.SetStandardValue(RGB(216, 231, 252));
			break;
		case xtpSystemThemeOlive:
			m_clrBackground.SetStandardValue(RGB(226, 231, 191));
			m_groupNormal.clrClient.SetStandardValue(RGB(226, 231, 191));
			break;
		case xtpSystemThemeSilver:
			m_clrBackground.SetStandardValue(RGB(223, 223, 234));
			m_groupNormal.clrClient.SetStandardValue(RGB(223, 223, 234));
			break;
		}

		if (systemTheme != xtpSystemThemeUnknown)
		{
			m_grcPushed.SetStandardValue(RGB(232, 127, 8), RGB(247, 218, 124));
			m_grcHot.SetStandardValue(RGB(255, 255, 220), RGB(247, 192, 91));
		}
	}
	m_groupNormal.clrHead.SetStandardValue(XTPColorManager()->grcDockBar.clrDark, XTPColorManager()->grcDockBar.clrLight);
	m_groupSpecial = m_groupNormal;
}

CRect CXTPTaskPanelListViewOffice2003Theme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (pGroup->IsItemHot() || pGroup->IsItemPressed())
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, pGroup->IsItemPressed() ? m_grcPushed : m_grcHot, FALSE);
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, GetGroupColors(pGroup)->clrHead, FALSE);
	}

	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(XPCOLOR_FRAME));

	return rc;
}
