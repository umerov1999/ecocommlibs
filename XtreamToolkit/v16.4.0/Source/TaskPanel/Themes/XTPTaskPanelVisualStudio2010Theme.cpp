// XTPTaskPanelVisualStudio2010Theme.cpp : implementation of the CXTPTaskPanelVisualStudio2010Theme class.
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
#include "XTPTaskPanelVisualStudio2010Theme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelVisualStudio2010Theme

CXTPTaskPanelVisualStudio2010Theme::CXTPTaskPanelVisualStudio2010Theme()
{
	m_bOfficeHighlight = TRUE;
	m_bBoldCaption = FALSE;
	m_bInvertDragRect = FALSE;
	m_bCaptionScrollButton = FALSE;

	m_themeTreeExplorer = new CXTPWinThemeWrapper();
}

CXTPTaskPanelVisualStudio2010Theme::~CXTPTaskPanelVisualStudio2010Theme()
{
	SAFE_DELETE(m_themeTreeExplorer);
}

void CXTPTaskPanelVisualStudio2010Theme::RefreshMetrics()
{
	CXTPTaskPanelToolboxWhidbeyTheme::RefreshMetrics();

	m_clrBackground.SetStandardValue(RGB(255, 255, 255));

	m_groupNormal.clrHead.SetStandardValue(RGB(240,240,240), RGB(240,240,240));
	m_clrBackground.SetStandardValue(RGB(255, 255, 255));

	m_groupNormal.clrClient.SetStandardValue(m_clrBackground);

	m_groupSpecial = m_groupNormal;

	m_clrHighlightBorder.SetStandardValue(RGB(229, 195, 101));


	m_clrHighlightSelected.SetStandardValue(RGB(255,239,187));
	m_clrHighlightPressed = m_clrHighlightSelected;
	m_clrHighlightHot = m_clrHighlightSelected;


	m_themeTreeExplorer->CloseThemeData();

	if (XTPSystemVersion()->IsWinVistaOrGreater())
	{
		CWnd wnd;
		if (wnd.CreateEx(0, AfxRegisterWndClass(0), NULL, WS_POPUP, CRect(0, 0, 0, 0), 0, 0))
		{
			CXTPWinThemeWrapper().SetWindowTheme(wnd, L"EXPLORER", NULL);

			m_themeTreeExplorer->OpenThemeData(wnd, L"TREEVIEW");
		}
	}
}

CRect CXTPTaskPanelVisualStudio2010Theme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTP_TASKPANEL_GROUPCOLORS* pGroupColors = GetGroupColors(pGroup);

	CRect rcText(rc);
	if (pGroup->IsItemFocused())
	{
		if (XTPSystemVersion()->IsWinVistaOrGreater())
		{
			XTPDrawHelpers()->GradientFill(pDC,
				CRect(rc.left, rc.top, rc.right, rc.bottom), RGB(255, 250, 239), RGB(255, 242, 203), FALSE);

			pDC->Draw3dRect(rc, m_clrHighlightBorder, m_clrHighlightBorder);
		}
		else
		{
			if (!pGroup->IsItemHot() && !pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightSelected);
			else if (pGroup->IsItemHot() && !pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
			else if (pGroup->IsItemHot() && pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
			else if (pGroup->IsItemHot() || pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightHot);
		}
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC,
			CRect(rc.left, rc.top, rc.right, rc.bottom), pGroupColors->clrHead, FALSE);
		pDC->Draw3dRect(rc, RGB(206,212,223), RGB(206,212,223));
	}

	return DrawExpandButton(pDC, pGroup, rc);
}

void CXTPTaskPanelVisualStudio2010Theme::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);
	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrBackground, TRUE);
}

void CXTPTaskPanelVisualStudio2010Theme::FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTPDrawHelpers()->GradientFill(pDC, rc, GetGroupColors(pGroup)->clrClient, TRUE);
}

void CXTPTaskPanelVisualStudio2010Theme::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	DrawGroupClientItems(pDC, pGroup, rc);
}


CRect  CXTPTaskPanelVisualStudio2010Theme::DrawExpandButton(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (m_themeTreeExplorer->IsAppThemed())
	{
		CRect rcButton(rc.left + 1, rc.CenterPoint().y - 8, rc.left + 1 + 16, rc.CenterPoint().y + 8);

		m_themeTreeExplorer->DrawThemeBackground(*pDC, TVP_GLYPH, pGroup->IsExpanded() ? GLPS_OPENED : GLPS_CLOSED, &rcButton, 0);

		CRect rcText(rc);
		rcText.DeflateRect(16, 0, 0, 2);

		return rcText;
	}

	return CXTPTaskPanelToolboxWhidbeyTheme::DrawExpandButton(pDC, pGroup, rc);
}
