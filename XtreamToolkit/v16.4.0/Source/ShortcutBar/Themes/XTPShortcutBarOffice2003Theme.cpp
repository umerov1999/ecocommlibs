// XTPShortcutBarPaintManager.cpp : implementation of the CXTPShortcutBarPaintManager class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>

#include <ShortcutBar/Resource.h>
#include <ShortcutBar/XTPShortcutBarPaintManager.h>
#include <ShortcutBar/XTPShortcutBar.h>
#include <ShortcutBar/XTPShortcutBarPane.h>
#include <ShortcutBar/Themes/XTPShortcutBarOffice2003Theme.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarOffice2003Theme

CXTPShortcutBarOffice2003Theme::CXTPShortcutBarOffice2003Theme()
{
	m_bShowTopLeftCaptionShadow = FALSE;
	m_bLunaColorDisabled = FALSE;
}

void CXTPShortcutBarOffice2003Theme::RefreshMetrics()
{
	CXTPShortcutBarPaintManager::RefreshMetrics();

	m_grcSelected.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_grcHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_grcPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_grcShortcutItem.SetStandardValue(GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DFACE));
	m_clrShortcutItemShadow.SetStandardValue(GetXtremeColor(XPCOLOR_FRAME));
	m_grcShortcutBarGripper.SetStandardValue(XTPColorManager()->grcShortcutBarGripper.clrLight,
		XTPColorManager()->grcShortcutBarGripper.clrDark);

	m_grcCaption.SetStandardValue(XTPColorManager()->grcCaption);
	m_clrCaptionText.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_grcItemCaption.SetStandardValue(XTPColorManager()->grcDockBar.clrDark, XTPColorManager()->grcDockBar.clrLight);
	m_grcItemHotCaption.SetStandardValue(m_grcItemCaption);
	m_clrItemCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	if (XTPColorManager()->IsLowResolution())
	{
		m_grcShortcutItem.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrShortcutItemShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
		m_grcShortcutBarGripper.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	}

	if (m_bLunaColorDisabled)
	{
		m_grcShortcutItem.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_grcShortcutBarGripper.SetStandardValue(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DSHADOW));
		m_clrShortcutItemShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	}
	else if (!XTPColorManager()->IsLunaColorsDisabled())
	{

		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_grcShortcutItem.SetStandardValue(RGB(203, 225, 252), RGB(125, 165, 224));
			m_clrAppSplitterFace = RGB(216, 231, 252);
			break;

		case xtpSystemThemeOlive:
			m_grcShortcutItem.SetStandardValue(RGB(234, 240, 207), RGB(177, 192, 140));
			m_clrAppSplitterFace = RGB(226, 231, 191);
			break;

		case xtpSystemThemeSilver:
			m_grcShortcutItem.SetStandardValue(RGB(225, 226, 236), RGB(149, 147, 177));
			m_clrAppSplitterFace = RGB(223, 223, 234);
			break;
		}

		if (systemTheme != xtpSystemThemeUnknown)
		{
			m_grcSelected.SetStandardValue(RGB(251, 230, 148), RGB(238, 149, 21));
			m_grcPushed.SetStandardValue(RGB(232, 127, 8), RGB(247, 218, 124));
			m_grcHot.SetStandardValue(RGB(255, 255, 220), RGB(247, 192, 91));

			m_clrPushedText.SetStandardValue(0);
		}
	}
	m_clrItemCaptionTop.SetStandardValue(XTPColorManager()->grcDockBar.clrLight);
	m_clrItemCaptionShadow.SetStandardValue(m_clrShortcutItemShadow);

	m_grcItemHotCaption.SetStandardValue(m_grcItemCaption);


	m_clrPopupFrame.SetStandardValue(m_clrShortcutItemShadow);
	m_clrPopupBackground.SetStandardValue(m_grcShortcutItem);

}


int CXTPShortcutBarOffice2003Theme::DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_fntPaneCaption);

		CXTPClientRect rc(pPane);
		rc.bottom = rc.top + 24;
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcCaption, FALSE);

		if (m_bShowTopLeftCaptionShadow)
		{
			pDC->FillSolidRect(0, rc.top, rc.Width(), 1, RGB(0xFF, 0xFF, 0xFF));
			pDC->FillSolidRect(0, rc.top, 1, rc.Height(), RGB(0xFF, 0xFF, 0xFF));
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clrCaptionText);

		CRect rcText(rc);
		rcText.DeflateRect(m_rcCaptionPadding);
		pDC->DrawText(pPane->GetCaption(), rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));
	}
	return 24;
}

int CXTPShortcutBarOffice2003Theme::DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_fntPaneItemCaption);

		CRect rc = pItem->GetCaptionRect();
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcItemCaption, FALSE);

		pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, m_clrItemCaptionShadow);
		pDC->FillSolidRect(0, rc.top, rc.Width(), 1, m_clrItemCaptionTop);

		if (m_bShowTopLeftCaptionShadow)
		{
			pDC->FillSolidRect(0, rc.top + 1, rc.Width(), 1, RGB(0xFF, 0xFF, 0xFF));
			pDC->FillSolidRect(0, rc.top + 1, 1, rc.Height() - 2, RGB(0xFF, 0xFF, 0xFF));
		}


		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clrItemCaptionText);

		CRect rcText(rc);
		rcText.DeflateRect(m_rcItemCaptionPadding);
		pDC->DrawText(pItem->GetCaption(), rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));

	}

	return m_nPaneItemHeight;
}


void CXTPShortcutBarOffice2003Theme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPClientRect rc(pShortcutBar);

	int nItemHeight = pShortcutBar->GetItemSize().cy;

	if (pShortcutBar->IsClientPaneVisible())
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));

		int nTopExpandedLines = pShortcutBar->GetGripperRect().bottom +
			(pShortcutBar->GetExpandedLinesCount() - 1) * nItemHeight;

		XTPDrawHelpers()->GradientFill(pDC, CRect(0, nTopExpandedLines, rc.Width(), rc.bottom),
			m_grcShortcutItem, FALSE);
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, CRect(0, (pShortcutBar->GetExpandedLinesCount() - 1) * nItemHeight - 1, rc.Width(),
			rc.Height()) , m_grcShortcutItem, FALSE);
	}
}

void CXTPShortcutBarOffice2003Theme::DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPWindowRect rc(pShortcutBar);
	rc.OffsetRect(-rc.TopLeft());
	pDC->Draw3dRect(rc, m_clrShortcutItemShadow, m_clrShortcutItemShadow);
}

void CXTPShortcutBarOffice2003Theme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc = pItem->GetItemRect();
	CXTPClientRect rcShortcutBar(pShortcutBar);

	if (pItem->IsSelected() &&
		(pShortcutBar->GetHotItem() != pItem || (pShortcutBar->GetPressedItem() != NULL && pShortcutBar->GetPressedItem() != pItem)))
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcSelected, FALSE);
		pDC->SetTextColor(m_clrSelectedText);
	}
	else if (pItem->IsSelected() ||
		(pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem))
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcPushed, FALSE);
		pDC->SetTextColor(m_clrPushedText);
	}
	else if ((pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL) ||
		pShortcutBar->GetPressedItem() == pItem)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcHot, FALSE);
		pDC->SetTextColor(m_clrHotText);
	}
	else if (pItem->IsExpanded())
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcShortcutItem, FALSE);
	}

	if (pItem->IsExpanded() && rc.bottom != rcShortcutBar.bottom)
	{
		pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, m_clrShortcutItemShadow);
	}
}

void CXTPShortcutBarOffice2003Theme::FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc = pItem->GetItemRect();

	if (pItem->IsExpanded())
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcSelected, TRUE);
	}
	else if (pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcPushed, TRUE);
		pDC->SetTextColor(m_clrPushedText);
	}
	else if (pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcHot, TRUE);
		pDC->SetTextColor(m_clrHotText);
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrItemCaptionTop);
	}

}


int CXTPShortcutBarOffice2003Theme::DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pShortcutBar->GetGripperRect();
		XTPDrawHelpers()->GradientFill(pDC, rc, m_grcShortcutBarGripper, FALSE);

		int nRight = max (2, (rc.Width() - 4 * 9) / 2);
		for (int i = 0; i < 9; i++)
		{
			pDC->FillSolidRect(nRight, rc.top + 1, 2, 2, RGB(40, 50, 71));
			pDC->FillSolidRect(nRight + 1, rc.top + 2, 2, 2, RGB(249, 249, 251));
			pDC->FillSolidRect(nRight + 1, rc.top + 2, 1, 1, RGB(97, 116, 152));

			nRight += 4;
		}
	}
	return 7;
}


void CXTPShortcutBarOffice2003Theme::DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bEnabled)
{
	if (!bEnabled) pImage->Draw(pDC, pt, pImage->GetDisabledIcon(), sz);
	else pImage->Draw(pDC, pt, bSelected || bPressed ? pImage->GetHotIcon(): bChecked ? pImage->GetCheckedIcon() : pImage->GetIcon(), sz);
}
