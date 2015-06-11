// XTPTaskPanelShortcutBarOffice2007Theme.cpp : implementation of the CXTPTaskPanelShortcutBarOffice2007Theme class.
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
#include "XTPTaskPanelListViewTheme.h"
#include "XTPTaskPanelListViewOfficeXPTheme.h"
#include "XTPTaskPanelListViewOffice2003Theme.h"
#include "XTPTaskPanelShortcutBarOffice2003Theme.h"
#include "XTPTaskPanelShortcutBarOffice2007Theme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelShortcutBarOffice2007Theme

CXTPTaskPanelShortcutBarOffice2007Theme::CXTPTaskPanelShortcutBarOffice2007Theme()
{
	m_nCaptionTextFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
	m_bBoldCaption = TRUE;
}

void CXTPTaskPanelShortcutBarOffice2007Theme::RefreshMetrics()
{
	CXTPTaskPanelShortcutBarOffice2003Theme::RefreshMetrics();

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC fnt(&dc, &m_fntCaption);
	m_nCaptionHeight = max(22, dc.GetTextExtent(_T(" "), 1).cy + 3);

	m_groupNormal.clrClient.SetStandardValue(0xFFFFFF);

	m_groupNormal.clrHead.SetStandardValue(RGB(227, 239, 255), RGB(192, 219, 255), 0.25f);

	CXTPResourceImages* pImages = XTPResourceImages();

	m_bPlainStyle = pImages->GetImageInt(_T("ShortcutBar"), _T("FlatStyle"), FALSE);
	m_clrBackground.SetStandardValue(0xFFFFFF);

	if (m_bPlainStyle)
	{
		m_clrBackground.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("FlatBackground")));
		m_groupNormal.clrClient.SetStandardValue(m_clrBackground);
	}

	m_groupSpecial = m_groupNormal;

	CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARITEMS"));
	if (pImage)
	{
		m_clrShortcutItemShadow = pImages->GetImageColor(_T("ShortcutBar"), _T("FrameBorder"));

		COLORREF clrText = pImages->GetImageColor(_T("ShortcutBar"), _T("NormalText"));
		m_groupNormal.clrHeadTextNormal.SetStandardValue(clrText);
		m_groupNormal.clrHeadTextHot.SetStandardValue(clrText);
	}
	else
	{
		m_clrShortcutItemShadow = RGB(101, 147, 207);

		m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(32, 77, 137));
		m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(32, 77, 137));
	}
	m_groupNormal.clrClientText.SetStandardValue(m_groupNormal.clrHeadTextNormal);
	m_groupNormal.clrClientLink.SetStandardValue(m_groupNormal.clrHeadTextNormal);
	m_groupNormal.clrClientLinkHot.SetStandardValue(m_groupNormal.clrHeadTextNormal);

	m_grcPushed.SetStandardValue(RGB(232, 127, 8), RGB(247, 218, 124));
	m_grcHot.SetStandardValue(RGB(255, 255, 220), RGB(247, 192, 91));
	m_groupNormal.clrHead.SetStandardValue(RGB(227, 239, 255), RGB(192, 219, 255), 0.25f);

	m_clrHighlightHot.SetStandardValue(RGB(255, 231, 162));
	m_clrHighlightPressed.SetStandardValue(RGB(255, 171, 63));
	m_clrHighlightSelected.SetStandardValue(RGB(255,189,105));
	m_clrHighlightBorder.SetStandardValue(RGB(255, 171, 63));

	m_groupSpecial = m_groupNormal;
}

CRect CXTPTaskPanelShortcutBarOffice2007Theme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	CXTPResourceImages* pImages = XTPResourceImages();


	CXTPResourceImage* pImage = 0;

	if (m_bPlainStyle)
		pImage = pImages->LoadFile(_T("SHORTCUTBARSMALLITEMS"));

	if (!pImage)
		pImage = pImages->LoadFile(_T("SHORTCUTBARITEMS"));

	if (!pImage)
		return CXTPTaskPanelShortcutBarOffice2003Theme::DrawGroupCaptionBackground(pDC, pGroup, rc);

	if (m_bPlainStyle)
	{
		if (pGroup->IsItemPressed())
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(3, 4), CRect(2, 2, 2, 2));
		}
		else if (pGroup->IsItemHot())
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(2, 4), CRect(2, 2, 2, 2));
		}
	}
	else
	{
		if (pGroup->IsItemPressed())
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(3, 4), CRect(2, 2, 2, 2));
		}
		else if (pGroup->IsItemHot())
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(2, 4), CRect(2, 2, 2, 2));
		}
		else
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(0, 4), CRect(2, 2, 2, 2));
		}
		pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, m_clrShortcutItemShadow);
	}


	return rc;
}

void CXTPTaskPanelShortcutBarOffice2007Theme::FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTPDrawHelpers()->GradientFill(pDC, rc, GetGroupColors(pGroup)->clrClient, FALSE);
}

void CXTPTaskPanelShortcutBarOffice2007Theme::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	DrawGroupClientItems(pDC, pGroup, rc);

	if (pGroup->GetParentItems()->GetLastVisibleItem() != pGroup)
		pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, m_clrShortcutItemShadow);
}

void CXTPTaskPanelShortcutBarOffice2007Theme::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);
	rc.bottom = max(rc.bottom, pTaskPanel->GetClientHeight());

	XTPDrawHelpers()->GradientFill(pDC, rc, m_clrBackground, FALSE);
}

void CXTPTaskPanelShortcutBarOffice2007Theme::DrawGroupItemFrame(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(rc.Height() > 22 ? _T("TOOLBARBUTTONS50") : _T("TOOLBARBUTTONS22"));
	if (!pImage)
	{
		CXTPTaskPanelShortcutBarOffice2003Theme::DrawGroupItemFrame(pDC, pItem, rc);
		return;
	}

	int nState = -1;

	if (pItem->IsItemSelected() && !pItem->IsItemHot() && !pItem->IsItemPressed()) nState = 2;
	else if (pItem->IsItemSelected() && pItem->IsItemHot() && !pItem->IsItemPressed())  nState = 3;
	else if (pItem->IsItemHot() && pItem->IsItemPressed()) nState = 1;
	else if (pItem->IsItemHot() || pItem->IsItemPressed()) nState = 0;

	if (nState != -1)
	{
		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 4), CRect(8, 8, 8, 8), COLORREF_NULL);
	}
}
