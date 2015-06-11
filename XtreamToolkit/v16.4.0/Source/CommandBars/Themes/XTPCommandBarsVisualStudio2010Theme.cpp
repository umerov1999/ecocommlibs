// XTPVisualStudio2010Theme.cpp : implementation of the CXTPVisualStudio2010Theme class.
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
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/Themes/XTPCommandBarsVisualStudio2005Theme.h>
#include <CommandBars/Themes/XTPCommandBarsVisualStudio2008Theme.h>
#include <CommandBars/Themes/XTPCommandBarsVisualStudio2010Theme.h>

// Status bar
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2010.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPVisualStudio2010Theme

CXTPVisualStudio2010Theme::CXTPVisualStudio2010Theme()
{
	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeVisualStudio2010(this);

	m_nPopupBarTextPadding = 10;
	m_pShadowManager->SetShadowOptions(xtpShadowOfficeAlpha);

	m_bFlatGripper = TRUE;
	m_bPlainSeparators = TRUE;

	m_bThemedStatusBar = TRUE;
	m_nToolbarSeparatorMargin = 2;
}

CXTPVisualStudio2010Theme::~CXTPVisualStudio2010Theme()
{
}

void CXTPVisualStudio2010Theme::RefreshMetrics()
{
	CXTPOffice2003Theme::RefreshMetrics();

	HWND hWnd = AfxGetMainWnd() ? AfxGetMainWnd()->GetSafeHwnd() : 0;
	m_themeStatusBar->OpenThemeData(hWnd, L"STATUS");


	m_clrCommandBar.SetStandardValue(RGB(188, 199, 216));
	m_clrDockBar.SetStandardValue(RGB(156,170,193));

	m_bLunaTheme = FALSE;

	m_arrColor[XPCOLOR_SPLITTER_FACE].SetStandardValue(XTPColorManager()->grcDockBar.clrDark);

	m_clrToolbarShadow.SetStandardValue(RGB(201, 210, 225));

	m_clrFloatingGripper.SetStandardValue(RGB(72, 92, 127));
	m_clrPopupControl.SetStandardValue(RGB(233, 236, 238));
	m_clrToolbarExpand.SetStandardValue(RGB(213,220,232));

	m_clrMenuExpandedGripper.SetStandardValue(RGB(203, 221, 246), RGB(121, 161, 220)); // TODO
	m_clrMenuExpand.SetStandardValue(0xFEEFE3, 0xE2A981); // TODO

	m_clrStatusPane.SetStandardValue(RGB(255, 255, 255));
	m_pShadowManager->SetShadowColor(RGB(0, 0, 0));
	m_clrTearOffGripper.SetStandardValue(RGB(169, 199, 240)); // TODO

	m_clrDisabledIcon.SetStandardValue(RGB(221, 227, 235), RGB(95, 100, 108));

	m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(RGB(133, 145, 162));
	m_arrColor[XPCOLOR_3DFACE].SetStandardValue(RGB(156, 170, 193));
	m_arrColor[XPCOLOR_3DSHADOW].SetStandardValue(RGB(96,114,140));
	m_arrColor[COLOR_APPWORKSPACE].SetStandardValue(RGB(47, 65, 95));
	m_arrColor[XPCOLOR_LABEL].SetStandardValue(RGB(233, 236, 238));

	INT nElements[] =
	{
		XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR, XPCOLOR_DISABLED,
		XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT,
		XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER,
		XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT
	};
	COLORREF clrValues[] =
	{
		RGB(97,115,140), RGB(133,145,162), RGB(128, 128, 128),
		RGB(221,226,232), RGB(155,167,183), RGB(255, 243, 206),
		RGB(231, 195, 99), RGB(229,195,101), RGB(229,195,101),
		RGB(255, 235, 165), RGB(255, 239, 189), RGB(188,199,216), 0
	};
	SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);



	m_arrColor[XPCOLOR_HIGHLIGHT_DISABLED_BORDER].SetStandardValue(RGB(128, 128, 128));
	m_arrColor[XPCOLOR_TOOLBAR_GRAYTEXT].SetStandardValue(RGB(128, 128, 128));
	m_arrColor[XPCOLOR_MENUBAR_GRAYTEXT].SetStandardValue(RGB(128, 128, 128));
	m_arrColor[XPCOLOR_FLOATBAR_BORDER].SetStandardValue(m_clrFloatingGripper);

	m_clrMenuGripper.SetStandardValue(RGB(233, 236, 238));

	m_bLunaTheme = XTPSystemVersion()->IsWinVistaOrGreater();
	m_grcLunaChecked.SetStandardValue(RGB(255,251,241), RGB(255,238,190));
	m_grcLunaPushed.SetStandardValue(RGB(255,232,166), RGB(255,232,166));
	m_grcLunaSelected.SetStandardValue(RGB(255,252,242), RGB(255,236,181));

	m_clrStatusPane.SetStandardValue(RGB(41, 57, 85));
	m_clrStatusTextColor = RGB(255, 255, 255);

	m_arrColor[XPCOLOR_TOOLBAR_TEXT].SetStandardValue(RGB(28, 42, 63));
	m_clrMenuSeparator.SetStandardValue(RGB(190, 195, 203));

	if (!m_bLunaTheme)
	{
		m_clrDockBar.SetStandardValue(RGB(173, 186, 206));
		m_clrCommandBar.SetStandardValue(RGB(189, 199, 222));
		m_clrToolbarShadow.SetStandardValue(RGB(206, 211, 231));

		m_arrColor[XPCOLOR_MENUBAR_FACE].SetStandardValue(RGB(239, 239, 239));
		m_arrColor[XPCOLOR_MENUBAR_BORDER].SetStandardValue(RGB(156, 166, 181));
		m_arrColor[XPCOLOR_TOOLBAR_GRIPPER].SetStandardValue(RGB(99, 113, 140));
		m_arrColor[XPCOLOR_SEPARATOR].SetStandardValue(RGB(132, 146, 165));

		m_arrColor[XPCOLOR_TOOLBAR_TEXT].SetStandardValue(RGB(24, 40, 57));

		m_clrMenuSeparator.SetStandardValue(RGB(189, 195, 206));
	}

	CreateGradientCircle();
}

void CXTPVisualStudio2010Theme::AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical)
{
	CXTPOffice2003Theme::AdjustExcludeRect(rc, pControl, bVertical);
}

CRect CXTPVisualStudio2010Theme::GetCommandBarBorders(CXTPCommandBar* pBar)
{
	if (pBar->GetPosition() == xtpBarPopup && pBar->GetType() == xtpBarTypePopup)
		return CRect(3, 3, 3, 3);

	return CXTPOffice2003Theme::GetCommandBarBorders(pBar);
}

void CXTPVisualStudio2010Theme::DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy, BOOL bExpanded)
{
	CXTPOffice2003Theme::DrawPopupBarGripper(pDC, x, y, cx, cy, bExpanded);
}

void CXTPVisualStudio2010Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition)
{
	if (barPosition == xtpBarPopup && barType == xtpBarTypePopup && bSelected && !bChecked && !bPressed)
	{
		if (m_bLunaTheme)
		{
			XTPDrawHelpers()->GradientFill(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), m_grcLunaSelected, FALSE);
		}
		else
		{
			pDC->FillSolidRect(CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), GetXtremeColor(XPCOLOR_HIGHLIGHT));
		}

		COLORREF clrBorder = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
		CXTPVisualStudio2008Theme::AlphaBorder(pDC, rc, clrBorder);

		return;
	}

	if (barType == xtpBarTypeMenuBar && bSelected && !bChecked && !bPressed && bEnabled && !bPopuped)
	{
		COLORREF clrBorder = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
		CXTPVisualStudio2008Theme::AlphaBorder(pDC, rc, clrBorder);
		rc.DeflateRect(1, 1);
		ExcludeCorners(pDC, rc);

		if (m_bLunaTheme) XTPDrawHelpers()->GradientFill(pDC, rc, m_grcLunaSelected, FALSE);
		else pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT));

		return;
	}

	CXTPOffice2003Theme::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
}

COLORREF CXTPVisualStudio2010Theme::GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition)
{
	return CXTPOffice2003Theme::GetRectangleTextColor(bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
}

CSize CXTPVisualStudio2010Theme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (bDraw && pBar->GetType() == xtpBarTypePopup)
	{
		CRect rc;
		pBar->GetClientRect(&rc);
		CRect rcControl = pControl->GetRect();
		CRect rcRow = pControl->GetRowRect();

		if (!pControl->GetWrap())
			HorizontalLine(pDC, rcControl.left - 1 + GetPopupBarGripperWidth(pBar) + m_nPopupBarTextPadding,
				rcControl.top - 2, rcControl.right + 1, m_clrMenuSeparator);
		else
			VerticalLine(pDC, rcControl.left - 2, rcRow.top - 1, rcRow.bottom + 1, m_clrMenuSeparator);

		return 0;

	}
	return CXTPOffice2003Theme::DrawCommandBarSeparator(pDC, pBar, pControl, bDraw);
}

void CXTPVisualStudio2010Theme::FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CXTPClientRect rc((CWnd*)pBar);

	if (pBar->GetType() == xtpBarTypeMenuBar && pBar->GetPosition() != xtpBarFloating)
	{
		if (m_bLunaTheme)
			XTPDrawHelpers()->GradientFill(pDC, rc, RGB(200,209,224), RGB(174,185,205), IsVerticalPosition(pBar->GetPosition()));
		else
			pDC->FillSolidRect(rc, m_clrDockBar);
	}
	else if (pBar->GetPosition() == xtpBarTop || pBar->GetPosition() == xtpBarBottom)
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		pDC->FillSolidRect(rc, clr3DFace);

		CWnd* pWnd = pBar->GetParent();
		if (pWnd)
		{
			CXTPWindowRect rcFill((CWnd*)pWnd);
			pBar->ScreenToClient(rcFill);
			rcFill.right = rcFill.left + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2);
			XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrDockBar, TRUE, rc);
		}

		CRect rcFill = rc;
		rcFill.top += 1;
		rcFill.left += 2;

		pDC->FillSolidRect(rcFill.left + 1, rcFill.top, rcFill.Width() - 2, 1, m_clrCommandBar.clrDark);
		pDC->FillSolidRect(rcFill.left, rcFill.top + 1, rcFill.Width(), rcFill.Height() - 2, m_clrCommandBar.clrDark);
		pDC->FillSolidRect(rcFill.left + 1, rcFill.bottom - 1, rcFill.Width() - 2, 1, m_clrCommandBar.clrDark);

		CXTPVisualStudio2008Theme::AlphaBorder(pDC, rcFill, m_clrToolbarShadow);
	}
	else if (pBar->GetPosition() == xtpBarLeft || pBar->GetPosition() == xtpBarRight)
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		pDC->FillSolidRect(rc, clr3DFace);

		CWnd* pWnd = pBar->GetParent();
		if (pWnd)
		{
			if (pWnd->GetStyle() & CBRS_RIGHT)
				pDC->FillSolidRect(rc, m_clrDockBar.clrDark);
			else
			{
				CXTPWindowRect rcFill((CWnd*)pWnd);
				pBar->ScreenToClient(rcFill);
				rcFill.right = rcFill.left + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2);
				XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrDockBar, TRUE, rc);
			}
		}

		CRect rcFill = rc;
		rcFill.top += 2;
		rcFill.left += 1;

		pDC->FillSolidRect(rcFill.left + 1, rcFill.top, rcFill.Width() - 2, 1, m_clrCommandBar.clrDark);
		pDC->FillSolidRect(rcFill.left, rcFill.top + 1, rcFill.Width(), rcFill.Height() - 2, m_clrCommandBar.clrDark);
		pDC->FillSolidRect(rcFill.left + 1, rcFill.bottom - 1, rcFill.Width() - 2, 1, m_clrCommandBar.clrDark);

		CXTPVisualStudio2008Theme::AlphaBorder(pDC, rcFill, m_clrToolbarShadow);
	}
	else
	{
		CXTPOffice2003Theme::FillCommandBarEntry(pDC, pBar);
	}
}
