// XTPRibbonBackstageViewThemeOffice2013.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPKeyboardManager.h>
#include <CommandBars/XTPCommandBar.h>

#include <Ribbon/Backstage/XTPRibbonBackstageView.h>
#include <Ribbon/Backstage/XTPRibbonBackstageCommand.h>
#include <Ribbon/Backstage/XTPRibbonBackstageTab.h>
#include <Ribbon/Backstage/XTPRibbonBackstagePaintManager.h>
#include <Ribbon/Backstage/XTPRibbonBackstageViewThemeOffice2013.h>


CXTPRibbonBackstageViewThemeOffice2013::CXTPRibbonBackstageViewThemeOffice2013()
{
	m_nCommandGap = 0;
}

CXTPRibbonBackstageViewThemeOffice2013::~CXTPRibbonBackstageViewThemeOffice2013()
{
}

void CXTPRibbonBackstageViewThemeOffice2013::DrawBackground(CDC *pDC, CXTPRibbonBackstageView *pView)
{
	CXTPClientRect rc(pView);

	pDC->FillSolidRect(rc, RGB(43, 87, 154));
}


void CXTPRibbonBackstageViewThemeOffice2013::DrawControl(CDC *pDC, CXTPRibbonBackstageView *pView, CXTPControl *pControl)
{
	if (!pControl->IsVisible())
		return;

//  BOOL bEnabled     = pControl->GetEnabled();
	BOOL bActiveTab   = pView->IsActiveTab(pControl);
	BOOL bHighlighted = pView->IsHighlightedControl(pControl);
//  BOOL bFocused     = pView->IsFocusedControl(pControl);

	COLORREF m_clrNormal   = RGB(43, 87, 154);
	COLORREF m_clrSelected = RGB(62, 109, 181);
	COLORREF m_clrHighlighted = RGB(25, 71, 138);
//  COLORREF m_clrPressed = RGB(25, 71, 138);

	CRect rcControl(pControl->GetRect());

	if (bHighlighted)
	{
		pDC->FillSolidRect(rcControl, m_clrHighlighted);
	}
	else if (bActiveTab)
	{
		pDC->FillSolidRect(rcControl, m_clrSelected);
	}
	else
	{
		pDC->FillSolidRect(rcControl, m_clrNormal);
	}

	CRect rcText(rcControl);
	rcText.DeflateRect(m_nTextGap + m_nCommandGap, 2, 2, 2);

	CXTPFontDC font(pDC, &pView->m_fntCommand);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->DrawText(pControl->GetCaption(), rcText, DT_HIDEPREFIX | DT_VCENTER | DT_SINGLELINE);
}
