// XTPRibbonTabColorSet.cpp: implementation of the CXTPRibbonTabColorSet class.
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

#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPSystemMetrics.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/Hook/XTPWinEventHook.h>

#include <TabManager/Includes.h>
#include <TabManager/ColorSets/XTPTabColorSetDefault.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2003.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Frame/XTPFrameHook.h>
#include <CommandBars/Frame/XTPFramePaintManager.h>

#include <Ribbon/XTPRibbonGroup.h>
#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>
#include <Ribbon/XTPRibbonTab.h>
#include <Ribbon/XTPRibbonControlTab.h>
#include <Ribbon/XTPRibbonTabColorSet.h>

void CXTPRibbonTabColorSet::RefreshMetrics()
{
	CXTPTabColorSetOffice2003::RefreshMetrics();
}

void CXTPRibbonTabColorSet::SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CXTPRibbonTab *pRibbonTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, pItem);
	CXTPRibbonBar* pRibbonBar = pRibbonTab->GetRibbonBar();

	BOOL bSelected = pItem->IsSelected();
	BOOL bHighlighted = pItem->IsHighlighted();
	BOOL bFocused = pItem->IsFocused();

	if (bSelected && pRibbonBar->IsRibbonMinimized())
	{
		CXTPRibbonControlTab* pControlTab =  (CXTPRibbonControlTab*)pItem->GetTabManager();
		if (pControlTab->GetCommandBar() && pControlTab->GetCommandBar()->IsTrackingMode())
		{
			bFocused = bHighlighted = FALSE;
		}
		else if (pControlTab->GetSelected() && bFocused)
		{
			bFocused = bSelected = FALSE;
			bHighlighted = TRUE;
		}
		else
		{
			bFocused = bSelected = FALSE;
		}
	}

	if (bSelected && pRibbonBar->IsBackstageViewVisible())
		bSelected = FALSE;


	pDC->SetTextColor(!pItem->IsEnabled() ? m_clrTextDisabled: bSelected ? m_clrTextSelected: bHighlighted ? m_clrTextHighlight : m_clrTextNormal);
}
