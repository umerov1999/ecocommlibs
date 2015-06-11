// XTPOffice2003Theme.cpp : implementation of the CXTPOffice2003Theme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#include <Common/XTPImageManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/ScrollBar/XTPScrollBase.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPDockBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPDialogBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/Themes/XTPCommandBarsVisualStudio2005Theme.h>
#include <CommandBars/Themes/XTPCommandBarsVisualStudio2008Theme.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPVisualStudio2008Theme

CXTPVisualStudio2008Theme::CXTPVisualStudio2008Theme()
{
	m_nPopupBarTextPadding = 10;
	m_pShadowManager->SetShadowOptions(xtpShadowOfficeAlpha);
}

CXTPVisualStudio2008Theme::~CXTPVisualStudio2008Theme()
{
}

void CXTPVisualStudio2008Theme::RefreshMetrics()
{
	CXTPVisualStudio2005Theme::RefreshMetrics();

	m_clrMenuGripper.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 40), GetXtremeColor(COLOR_3DFACE));
	m_clrMenuSelection.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrMenuSelectionBorder.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrMenuSelectionText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
	case xtpSystemThemeRoyale:
		{
			m_clrMenuSelectionBorder.SetStandardValue(RGB(152, 181, 226));
			m_clrMenuSelection.SetStandardValue(RGB(239,243,250), RGB(193,210,238));
			m_clrMenuSelectionText.SetStandardValue(0);
			m_clrMenuGripper.SetStandardValue(RGB(241, 241, 241));
			m_clrMenuExpandedGripper.SetStandardValue(RGB(215, 227, 234));

			m_arrColor[XPCOLOR_LABEL].SetStandardValue(RGB(221, 231, 238));
		}
		break;

	case xtpSystemThemeAero:
		{
			m_clrMenuSelectionBorder.SetStandardValue(RGB(150,217,249));
			m_clrMenuSelection.SetStandardValue(RGB(241,248,251), RGB(186,228,246));
			m_clrMenuSelectionText.SetStandardValue(0);
			m_clrMenuGripper.SetStandardValue(RGB(244,247,252), RGB(212,216,230));

			m_arrColor[XPCOLOR_SEPARATOR].SetStandardValue(RGB(160,160,180));


			m_clrDockBar.SetStandardValue(RGB(233,236,250), RGB(244,247,252));
			m_clrToolbarShadow.SetStandardValue(RGB(181,190,206));
			m_clrCommandBar.SetStandardValue(RGB(250,250,253), RGB(196,203,219), 0.75f);

			m_clrFloatingGripper.SetStandardValue(RGB(128, 128, 128));
			m_clrPopupControl.SetStandardValue(RGB(255, 255, 255), RGB(118, 184, 219));
			m_clrToolbarExpand.SetStandardValue(RGB(195,202,218), RGB(233,236,250), 0.75f);

			m_clrStatusPane.SetStandardValue(RGB(160,160,180));
			m_pShadowManager->SetShadowColor(0);

			m_clrDisabledIcon.SetStandardValue(RGB(244,247,252), RGB(113,124,152));

			m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(RGB(240, 240, 240));

			INT nElements[] =
			{
				XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR, XPCOLOR_DISABLED, XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT
			};
			COLORREF clrValues[] =
			{
				RGB(160,160,180), RGB(160,160,180), RGB(128,128,128), RGB(240, 240, 240), RGB(160, 160, 160), RGB(182,228,247), RGB(41,153,255), RGB(41,153,255), RGB(48,127,177), RGB(206,237,250), RGB(206,237,250), RGB(233,236,250), 0
			};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_grcLunaChecked.SetStandardValue(RGB(237,248,253), RGB(129,192,224));
			m_grcLunaPushed.SetStandardValue(RGB(228,245,252), RGB(124,177,204));
			m_grcLunaSelected.SetStandardValue(RGB(237,248,253), RGB(129,192,224));

			m_bLunaTheme = TRUE;

		}
		break;

	case xtpSystemThemeOlive:
		{
			m_clrMenuSelectionBorder.SetStandardValue(RGB(147,160,112));
			m_clrMenuSelection.SetStandardValue(RGB(239,242,227), RGB(194,207,158));
			m_clrMenuSelectionText.SetStandardValue(0);
			m_clrMenuGripper.SetStandardValue(RGB(247,246,239), RGB(233,230,214));
		}
		break;

	case xtpSystemThemeSilver:
		{
			m_clrMenuSelectionBorder.SetStandardValue(RGB(255,192,111));
			m_clrMenuSelection.SetStandardValue(RGB(255,246,204), RGB(255,194,115));
			m_clrMenuSelectionText.SetStandardValue(0);
			m_clrMenuGripper.SetStandardValue(RGB(232,233,242), RGB(193,193,211));
			m_arrColor[XPCOLOR_SEPARATOR].SetStandardValue(RGB(110,109,143));
			m_clrPopupControl.SetStandardValue(RGB(232,233,241), RGB(192,191,209));
		}
		break;
	}
}

void CXTPVisualStudio2008Theme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL /*bVertical*/)
{
	CXTPCommandBar* pParent = pControl->GetParent();

	if (pParent->GetType() == xtpBarTypePopup)
		rc.InflateRect(1, 0);
}

CRect CXTPVisualStudio2008Theme::GetCommandBarBorders(CXTPCommandBar* pBar)
{
	if (pBar->GetPosition() == xtpBarPopup && pBar->GetType() == xtpBarTypePopup)
		return CRect(4, 3, 4, 3);

	return CXTPVisualStudio2005Theme::GetCommandBarBorders(pBar);
}

void CXTPVisualStudio2008Theme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy, BOOL bExpanded)
{
	XTPDrawHelpers()->GradientFill(pDC, CRect(CPoint(x, y), CSize(cx, cy)),
		bExpanded ? m_clrMenuExpandedGripper : m_clrMenuGripper, TRUE);

	pDC->FillSolidRect(x + cx, y, 1, cy, GetXtremeColor(XPCOLOR_SEPARATOR));
	pDC->FillSolidRect(x + cx + 1, y, 1, cy, GetXtremeColor(COLOR_3DHILIGHT));
}

void CXTPVisualStudio2008Theme::AlphaPixel(CDC* pDC, int x, int y, COLORREF clr, int nAlpha)
{
	if (nAlpha == 255)
	{
		pDC->SetPixel(x, y, clr);
	}
	else
	{
		COLORREF clrOld = pDC->GetPixel(x, y);
		pDC->SetPixel(x, y, CXTPDrawHelpers::BlendColors(clr, clrOld, (double)nAlpha / 255));
	}
}

void CXTPVisualStudio2008Theme::AlphaBorder(CDC* pDC, CRect rc, COLORREF clrBorder)
{
	pDC->FillSolidRect(rc.left + 3, rc.top, rc.Width() - 6, 1, clrBorder);
	pDC->FillSolidRect(rc.left + 3, rc.bottom - 1, rc.Width() - 6, 1, clrBorder);
	pDC->FillSolidRect(rc.left, rc.top + 3, 1, rc.Height() - 6, clrBorder);
	pDC->FillSolidRect(rc.right - 1 , rc.top + 3, 1, rc.Height() - 6, clrBorder);

	AlphaPixel(pDC, rc.left + 1, rc.top + 1, clrBorder, 255);
	AlphaPixel(pDC, rc.left + 1, rc.top, clrBorder, 60);
	AlphaPixel(pDC, rc.left + 2, rc.top, clrBorder, 120);
	AlphaPixel(pDC, rc.left, rc.top + 1, clrBorder, 60);
	AlphaPixel(pDC, rc.left, rc.top + 2, clrBorder, 120);

	AlphaPixel(pDC, rc.right - 2, rc.top + 1, clrBorder, 255);
	AlphaPixel(pDC, rc.right - 3, rc.top, clrBorder, 120);
	AlphaPixel(pDC, rc.right - 2, rc.top, clrBorder, 60);
	AlphaPixel(pDC, rc.right - 1, rc.top + 1, clrBorder, 60);
	AlphaPixel(pDC, rc.right - 1, rc.top + 2, clrBorder, 120);

	AlphaPixel(pDC, rc.left + 1, rc.bottom - 2, clrBorder, 255);
	AlphaPixel(pDC, rc.left + 1, rc.bottom - 1, clrBorder, 60);
	AlphaPixel(pDC, rc.left + 2, rc.bottom - 1, clrBorder, 120);
	AlphaPixel(pDC, rc.left, rc.bottom - 2, clrBorder, 60);
	AlphaPixel(pDC, rc.left, rc.bottom - 3, clrBorder, 120);


	AlphaPixel(pDC, rc.right - 2, rc.bottom - 2, clrBorder, 255);
	AlphaPixel(pDC, rc.right - 3, rc.bottom - 1, clrBorder, 120);
	AlphaPixel(pDC, rc.right - 2, rc.bottom - 1, clrBorder, 60);
	AlphaPixel(pDC, rc.right - 1, rc.bottom - 2, clrBorder, 60);
	AlphaPixel(pDC, rc.right - 1, rc.bottom - 3, clrBorder, 120);
}

void CXTPVisualStudio2008Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition)
{
	if (barPosition == xtpBarPopup && barType == xtpBarTypePopup && bSelected && !bChecked && !bPressed)
	{
		XTPDrawHelpers()->GradientFill(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), m_clrMenuSelection, FALSE);

		COLORREF clrBorder = m_clrMenuSelectionBorder;
		AlphaBorder(pDC, rc, clrBorder);
		return;
	}

	if (barType == xtpBarTypeMenuBar && bSelected && !bChecked && !bPressed && bEnabled)
	{
		COLORREF clrShadow = GetXtremeColor(COLOR_WINDOW);
		COLORREF clrBorder = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);

		pDC->FillSolidRect(rc.left + 1, rc.top, rc.Width() - 2, 1, clrShadow);
		pDC->FillSolidRect(rc.left + 1, rc.bottom - 1, rc.Width() - 2, 1, clrShadow);
		pDC->FillSolidRect(rc.left, rc.top + 1, 1, rc.Height() - 2, clrShadow);
		pDC->FillSolidRect(rc.right - 1 , rc.top + 1, 1, rc.Height() - 2, clrShadow);
		rc.DeflateRect(1, 1);

		pDC->Draw3dRect(rc, clrBorder, clrBorder);
		COLORREF clrPixel = CXTPDrawHelpers::BlendColors(clrBorder, clrShadow, 0.5);
		pDC->SetPixel(rc.left, rc.top, clrPixel);
		pDC->SetPixel(rc.left, rc.bottom - 1, clrPixel);
		pDC->SetPixel(rc.right - 1, rc.top, clrPixel);
		pDC->SetPixel(rc.right - 1, rc.bottom - 1, clrPixel);

		rc.DeflateRect(1, 1);

		if (!bPopuped)
		{
			if (m_bLunaTheme) XTPDrawHelpers()->GradientFill(pDC, rc, m_grcLunaSelected, FALSE);
			else pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT));
		}
		else
			XTPDrawHelpers()->GradientFill(pDC, rc, m_clrPopupControl, FALSE);

		return;
	}
	if (barType != xtpBarTypePopup && bPopuped && bEnabled)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, m_clrPopupControl, FALSE);
		Draw3dRect(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_BORDER);
		return;
	}

	CXTPVisualStudio2005Theme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
}

COLORREF CXTPVisualStudio2008Theme::GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition)
{
	if (barPosition == xtpBarPopup && barType == xtpBarTypePopup && bSelected && !bPressed && bEnabled)
	{
		return m_clrMenuSelectionText;
	}

	return CXTPVisualStudio2005Theme::GetRectangleTextColor(bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
}

CSize CXTPVisualStudio2008Theme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw || pBar->GetType() != xtpBarTypePopup || pBar->GetPosition() != xtpBarPopup)
		return CXTPVisualStudio2005Theme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);

	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CRect rcRow = pControl->GetRowRect();

	if (!pControl->GetWrap())
	{
		HorizontalLine(pDC, rcControl.left - 1 + GetPopupBarGripperWidth(pBar) + m_nPopupBarTextPadding, rcControl.top - 2, rcControl.right + 1, GetXtremeColor(XPCOLOR_SEPARATOR));
		HorizontalLine(pDC, rcControl.left - 1 + GetPopupBarGripperWidth(pBar) + m_nPopupBarTextPadding, rcControl.top - 1, rcControl.right + 1, GetXtremeColor(COLOR_3DHILIGHT));
	}
	else
	{
		VerticalLine(pDC, rcControl.left - 2, rcRow.top - 1, rcRow.bottom + 1, GetXtremeColor(XPCOLOR_SEPARATOR));
		VerticalLine(pDC, rcControl.left - 1, rcRow.top - 1, rcRow.bottom + 1, GetXtremeColor(COLOR_3DHILIGHT));
	}

	return 0;
}
