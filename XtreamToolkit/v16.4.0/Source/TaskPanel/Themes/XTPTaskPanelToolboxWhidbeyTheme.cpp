// XTPTaskPanelToolboxWhidbeyTheme.cpp : implementation of the CXTPTaskPanelToolboxWhidbeyTheme class.
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
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include <TaskPanel/Resource.h>
#include <TaskPanel/XTPTaskPanelDefines.h>
#include <TaskPanel/XTPTaskPanel.h>
#include <TaskPanel/XTPTaskPanelItem.h>
#include <TaskPanel/XTPTaskPanelGroup.h>
#include <TaskPanel/XTPTaskPanelGroupItem.h>
#include <TaskPanel/XTPTaskPanelItems.h>
#include <TaskPanel/XTPTaskPanelPaintManager.h>

#include "XTPTaskPanelPaintManagerPlain.h"
#include "XTPTaskPanelToolboxTheme.h"
#include "XTPTaskPanelToolboxWhidbeyTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelToolboxWhidbeyTheme

CXTPTaskPanelToolboxWhidbeyTheme::CXTPTaskPanelToolboxWhidbeyTheme()
{
	m_bOfficeHighlight = TRUE;
	m_bBoldCaption = TRUE;
	m_bInvertDragRect = FALSE;
	m_bCaptionScrollButton = FALSE;
}

void CXTPTaskPanelToolboxWhidbeyTheme::RefreshMetrics()
{
	CXTPTaskPanelToolboxTheme::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
		m_groupNormal.clrHead.SetStandardValue(RGB(222, 220, 203), RGB(196, 193, 176));
		m_clrBackground.SetStandardValue(RGB(243, 241, 230), RGB(228, 225, 208));
		break;

	case xtpSystemThemeOlive:
		m_groupNormal.clrHead.SetStandardValue(RGB(223, 228, 209), RGB(222, 227, 206));
		m_clrBackground.SetStandardValue(RGB(243, 241, 230), RGB(228, 225, 208));
		m_clrHighlightHot.SetStandardValue(RGB(182, 198, 141));
		m_clrHighlightPressed.SetStandardValue(RGB(147, 160, 112));
		m_clrHighlightSelected.SetStandardValue(RGB(194, 207, 158));
		break;

	case xtpSystemThemeSilver:
		m_groupNormal.clrHead.SetStandardValue(RGB(233, 232, 237), RGB(223, 224, 227));
		m_clrBackground.SetStandardValue(RGB(234, 233, 238), RGB(215, 213, 218));
		m_clrHighlightHot.SetStandardValue(RGB(255, 238, 194));
		m_clrHighlightPressed.SetStandardValue(RGB(254, 128, 062));
		m_clrHighlightSelected.SetStandardValue(RGB(255, 192, 111));
		m_clrHighlightBorder.SetStandardValue(RGB(75, 75, 111));
		break;

	default:
		m_groupNormal.clrHead.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DFACE));
	}

	m_groupNormal.clrClient.SetStandardValue(m_clrBackground);

	m_groupSpecial = m_groupNormal;

}


CRect  CXTPTaskPanelToolboxWhidbeyTheme::DrawExpandButton(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);
	COLORREF clrFace = pGroupColors->clrHead.clrLight;

	CRect rcButton(rc.left + 6, rc.CenterPoint().y - 6, rc.left + 6 + 9, rc.CenterPoint().y + 3);

	COLORREF clrButton = clrFace;
	COLORREF clrFrame = GetXtremeColor(COLOR_3DSHADOW);
	pDC->FillSolidRect(rcButton, clrButton);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, rcButton.Width() - 2, rcButton.Height() - 2, GetXtremeColor(COLOR_3DFACE));
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top, rcButton.Width() - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.bottom - 1, rcButton.Width() - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left, rcButton.top + 1, 1, rcButton.Height() - 2, clrFrame);
	pDC->FillSolidRect(rcButton.right - 1, rcButton.top + 1, 1, rcButton.Height() - 2, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, rcButton.Width() - 2, 3, 0xFFFFFF);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 4, rcButton.Width() - 4, 2, 0xFFFFFF);

	pDC->FillSolidRect(rcButton.left + 2, rcButton.top + 4, rcButton.Width() - 4, 1, 0);
	if (!pGroup->IsExpanded())
		pDC->FillSolidRect(rcButton.left + 4, rcButton.top + 2, 1, rcButton.Height() - 4, 0);

	CRect rcText(rc);
	rcText.DeflateRect(19, 0, 0, 2);

	return rcText;
}

CRect CXTPTaskPanelToolboxWhidbeyTheme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);
	COLORREF clrFace = pGroupColors->clrHead.clrLight;

	if (pGroup->IsItemFocused())
	{
		if (!pGroup->IsItemHot() && !pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightSelected);
		else if (pGroup->IsItemHot() && !pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pGroup->IsItemHot() && pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pGroup->IsItemHot() || pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightHot);

	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC,
			CRect(rc.left, rc.top, rc.right, rc.bottom - 2), pGroupColors->clrHead, FALSE);

		pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, clrFace);
	}

	return DrawExpandButton(pDC, pGroup, rc);
}

void CXTPTaskPanelToolboxWhidbeyTheme::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);
	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrBackground, TRUE);
}

void CXTPTaskPanelToolboxWhidbeyTheme::FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTPDrawHelpers()->GradientFill(pDC, rc, GetGroupColors(pGroup)->clrClient, TRUE);
}

void CXTPTaskPanelToolboxWhidbeyTheme::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	DrawGroupClientItems(pDC, pGroup, rc);
}
