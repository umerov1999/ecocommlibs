// XTPTaskPanelVisualStudio2012Theme.cpp : implementation of the CXTPTaskPanelVisualStudio2012Theme class.
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
#include "XTPTaskPanelVisualStudio2012Theme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelVisualStudio2012Theme

CXTPTaskPanelVisualStudio2012Theme::CXTPTaskPanelVisualStudio2012Theme(BOOL bLight)
	:m_bLight(bLight)
{
	m_bOfficeHighlight = TRUE;
	m_bBoldCaption = FALSE;
	m_bInvertDragRect = FALSE;
	m_bCaptionScrollButton = FALSE;
}

CXTPTaskPanelVisualStudio2012Theme::~CXTPTaskPanelVisualStudio2012Theme()
{

}

void CXTPTaskPanelVisualStudio2012Theme::RefreshMetrics()
{
	CXTPTaskPanelToolboxWhidbeyTheme::RefreshMetrics();

	m_rcItemInnerMargins.left = 23;
	m_rcItemInnerMargins.right = 2;
	m_rcItemInnerMargins.top = m_rcItemInnerMargins.bottom = 2;

	if (m_bLight)
	{
		m_clrBackground.SetStandardValue(RGB(246, 246, 246));
		m_clrHighlightSelected.SetStandardValue(RGB(51, 153, 255));
		m_clrHighlightHot.SetStandardValue(RGB(254, 254, 254));
		m_clrSelectedWithoutFocus.SetStandardValue(RGB(204, 206, 219));
		m_clrText.SetStandardValue(RGB(0, 0, 0));
		m_clrSelectedText.SetStandardValue(RGB(255, 255, 255));
		m_clrSelectedTextWithoutFocus.SetStandardValue(RGB(0, 0, 0));

		m_clrTriangle.SetStandardValue(RGB(30, 30, 30));
	}
	else
	{
		m_clrBackground.SetStandardValue(RGB(37, 37, 38));
		m_clrHighlightSelected.SetStandardValue(RGB(51, 153, 255));
		m_clrHighlightHot.SetStandardValue(RGB(64, 64, 64));
		m_clrSelectedWithoutFocus.SetStandardValue(RGB(63, 63, 70));
		m_clrText.SetStandardValue(RGB(241, 241, 241));
		m_clrSelectedText.SetStandardValue(RGB(255, 255, 255));
		m_clrSelectedTextWithoutFocus.SetStandardValue(RGB(255, 255, 255));

		m_clrTriangle.SetStandardValue(RGB(241, 241, 241));
	}

	m_clrTriangleFocused.SetStandardValue(RGB(255,255,255));
	m_clrTriangleHot.SetStandardValue(RGB(0,122,204));

	//set colors for groups
	m_groupNormal.clrHead.SetStandardValue(m_clrBackground);
	m_groupNormal.clrClient.SetStandardValue(m_clrBackground);
	m_groupSpecial = m_groupNormal;

	//selected, pressed and border
	m_clrHighlightBorder = m_clrHighlightSelected;
	m_clrHighlightPressed = m_clrHighlightSelected;
}

void CXTPTaskPanelVisualStudio2012Theme::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);
	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	Rectangle(pDC, rcFill, m_clrBackground, m_clrBackground);
}

void CXTPTaskPanelVisualStudio2012Theme::FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	Rectangle(pDC, rc, GetGroupColors(pGroup)->clrClient, GetGroupColors(pGroup)->clrClient);
}

void CXTPTaskPanelVisualStudio2012Theme::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	DrawGroupClientItems(pDC, pGroup, rc);
}

void CXTPTaskPanelVisualStudio2012Theme::Triangle(CDC* pDC, POINT *pPoints, COLORREF clr, BOOL bFilled)
{
	CXTPPenDC pen (*pDC, clr);

	HBRUSH hBrush = bFilled ? (::CreateSolidBrush(clr)) : (static_cast<HBRUSH>(GetStockObject(NULL_BRUSH)));

	HGDIOBJ hOldBrush = pDC->SelectObject(hBrush);

	pDC->Polygon(pPoints, 3);

	pDC->SelectObject(hOldBrush);

	::DeleteObject(hBrush); // It is not necessary (but it is not harmful) to delete stock objects by calling DeleteObject.
}

CXTPPaintManagerColor CXTPTaskPanelVisualStudio2012Theme::GetTriangleColorAndState(CXTPTaskPanelGroup* pGroup, BOOL &bFilled)
{
	CXTPPaintManagerColor clrTriangle = m_clrTriangle;

	if (pGroup->IsItemFocused())
		clrTriangle = m_clrTriangleFocused;
	else if (pGroup->IsItemHot())
		clrTriangle = m_clrTriangleHot;

	if (pGroup->IsExpanded())
	{
		if (pGroup->IsItemHot() && pGroup->IsItemFocused())
			bFilled = FALSE;
		else
			bFilled = TRUE;
	}
	else
	{
		if (pGroup->IsItemHot() && pGroup->IsItemFocused())
			bFilled = TRUE;
		else
			bFilled = FALSE;
	}

	return clrTriangle;
}

CRect CXTPTaskPanelVisualStudio2012Theme::DrawExpandButton(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	static int nExpandButtonHalfWidth = 10;
	static int nExpandButtonHalfHeight = 10;

	CRect rcButton(rc.left, rc.CenterPoint().y - nExpandButtonHalfHeight,
		rc.left + nExpandButtonHalfWidth*2, rc.CenterPoint().y + nExpandButtonHalfHeight);

	POINT arPoints[3];

	BOOL bFilled = FALSE;
	CXTPPaintManagerColor clrTriangle = GetTriangleColorAndState(pGroup, bFilled);

	if (pGroup->IsExpanded())
	{
		POINT ptLTGlyph = {8, 13};
		SIZE szGlyph = {6, 6};

		arPoints[0].x = rcButton.left + ptLTGlyph.x;
		arPoints[0].y = rcButton.top + ptLTGlyph.y;

		arPoints[1].x = arPoints[0].x + szGlyph.cx;
		arPoints[1].y = arPoints[0].y;

		arPoints[2].x = arPoints[1].x;
		arPoints[2].y = arPoints[1].y - szGlyph.cy;
	}
	else
	{
		POINT ptLTGlyph = {9, 5};
		SIZE szGlyph = {5, 10};

		arPoints[0].x = rcButton.left + ptLTGlyph.x;
		arPoints[0].y = rcButton.top + ptLTGlyph.y;

		arPoints[1].x = arPoints[0].x;
		arPoints[1].y = arPoints[0].y + szGlyph.cy;

		arPoints[2].x = arPoints[1].x + szGlyph.cx;
		arPoints[2].y = arPoints[1].y - szGlyph.cy / 2;
	}

	Triangle(pDC, arPoints, clrTriangle, bFilled);

	CRect rcText(rc);
	rcText.DeflateRect(nExpandButtonHalfWidth*2, 0, 0, 0);
	return rcText;
}

void CXTPTaskPanelVisualStudio2012Theme::DrawGroupItemFrame(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc)
{
	if (pItem->IsItemHot() && !pItem->IsItemSelected() && !pItem->IsItemPressed())
	{
		Rectangle(pDC, rc, m_clrHighlightHot, m_clrHighlightHot);
		return;
	}

	if ((pItem->GetTaskPanel()->GetSafeHwnd() != GetFocus()) && pItem->IsItemSelected())
	{
		Rectangle(pDC, rc, m_clrSelectedWithoutFocus, m_clrSelectedWithoutFocus);
		return;
	}

	CXTPTaskPanelPaintManager::DrawGroupItemFrame(pDC, pItem, rc);
}

CRect CXTPTaskPanelVisualStudio2012Theme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (pGroup->IsItemFocused())
	{
		if (pGroup->GetTaskPanel()->GetSafeHwnd() != GetFocus())
			Rectangle(pDC, rc, m_clrSelectedWithoutFocus, m_clrSelectedWithoutFocus);
		else
			Rectangle(pDC, rc, m_clrHighlightSelected, m_clrHighlightSelected);
	}
	return DrawExpandButton(pDC, pGroup, rc);
}


void CXTPTaskPanelVisualStudio2012Theme::DrawItemCaption(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rcText, UINT nFormat)
{
	if ((pItem->IsGroup() && pItem->IsItemFocused()) || (!pItem->IsGroup() && static_cast<CXTPTaskPanelGroupItem*>(pItem)->IsItemSelected()))
	{
		if (pItem->GetTaskPanel()->GetSafeHwnd() == GetFocus())
			pDC->SetTextColor(m_clrSelectedText);
		else
			pDC->SetTextColor(m_clrSelectedTextWithoutFocus);
	}
	else
		pDC->SetTextColor(m_clrText);

	pDC->SetBkMode(TRANSPARENT);
	pItem->DrawCaptionText(pDC, rcText, nFormat);
	pItem->SetTextRect(pDC, rcText, nFormat);
}
