// XTPRibbonTabTheme.cpp: implementation of the CXTPRibbonTabTheme class.
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
#include <Ribbon/XTPRibbonPaintManager.h>
#include <Ribbon/XTPRibbonTab.h>
#include <Ribbon/XTPRibbonTabTheme.h>
#include <Ribbon/XTPRibbonControlTab.h>

void CXTPRibbonTabTheme::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	m_pRibbonTheme->FillRibbonTabControl(pDC, ((CXTPRibbonControlTab*)pTabManager)->GetRibbonBar(), rcControl);

	CAppearanceSetPropertyPageFlat::DrawTabControl(pTabManager, pDC, rcControl);
}

void CXTPRibbonTabTheme::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	m_pRibbonTheme->DrawRibbonTab(pDC, (CXTPRibbonTab*)pItem);

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect() , TRUE);
}

void CXTPRibbonTabTheme::DrawText(CDC* pDC, CXTPTabManager* pManager, const CString& str, LPRECT lpRect, UINT nFormat)
{
	CXTPRibbonControlTab* pControlTab = (CXTPRibbonControlTab*)pManager;
	CXTPRibbonBar* pRibbonBar = pControlTab->GetRibbonBar();

	if (nFormat & DT_CALCRECT)
		pDC->DrawText(str, lpRect, nFormat);
	else
	{
		BOOL bComposite =  IsCompositeRect(pRibbonBar, lpRect);
		CXTPPaintManager::DrawTextApi(pDC, str, lpRect, nFormat, bComposite);
	}
}

int CXTPRibbonTabTheme::GetButtonHeight(const CXTPTabManager *pTabManager)
{
	UNREFERENCED_PARAMETER(pTabManager);
	return m_pRibbonTheme->m_nTabsHeight + 1;
}
