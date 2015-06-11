// XTPDockingPaneVisualStudio2005Beta1Theme.cpp : implementation of the CXTPDockingPaneVisualStudio2005Beta1Theme class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPSystemHelpers.h>

#include <TabManager/Includes.h>

#include <DockingPane/Resource.h>
#include <DockingPane/XTPDockingPaneDefines.h>
#include <DockingPane/XTPDockingPaneBase.h>
#include <DockingPane/XTPDockingPaneBaseContainer.h>
#include <DockingPane/XTPDockingPanePaintManager.h>
#include <DockingPane/XTPDockingPaneTabbedContainer.h>
#include <DockingPane/XTPDockingPane.h>
#include <DockingPane/XTPDockingPaneMiniWnd.h>
#include <DockingPane/XTPDockingPaneSplitterContainer.h>
#include <DockingPane/XTPDockingPaneSidePanel.h>

#include <DockingPane/Themes/XTPDockingPaneThemeDefault.h>
#include <DockingPane/Themes/XTPDockingPaneThemeGrippered.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta1.h>

#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2005.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2005Beta1Theme


CXTPDockingPaneVisualStudio2005Beta1Theme::CXTPDockingPaneVisualStudio2005Beta1Theme()
{
	CAppearanceSetPropertyPageFlat* pAppearanceFlat = (CAppearanceSetPropertyPageFlat*)
		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);

	m_pTabPaintManager->m_bDisableLunaColors = TRUE;
	pAppearanceFlat->m_bBlurPoints = FALSE;
	m_pTabPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2005(TRUE));
	m_pTabPaintManager->m_bHotTracking = FALSE;
	m_pTabPaintManager->m_clientFrame = xtpTabFrameNone;

	m_pPanelPaintManager->m_bDisableLunaColors = TRUE;
	pAppearanceFlat = (CAppearanceSetPropertyPageFlat*)m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	pAppearanceFlat->m_bBlurPoints = FALSE;
	m_pPanelPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2005(FALSE));
	m_pPanelPaintManager->m_bHotTracking = FALSE;
	m_pPanelPaintManager->SetLayout(xtpTabLayoutAutoSize);

	m_rcCaptionMargin.SetRect(0, 0, 0, 0);
	m_bGradientCaption = TRUE;
	m_bUseBoldCaptionFont = FALSE;
	m_dwSplitterStyle = xtpPaneSplitterGradient;

	m_systemTheme = xtpSystemThemeUnknown;
}

void CXTPDockingPaneVisualStudio2005Beta1Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2003Theme::RefreshMetrics();

	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTIONTEXT));

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
		m_clrSplitterGradient.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));
		m_clrNormalCaption.SetStandardValue(RGB(216, 215, 198), RGB(238, 239, 229));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;

	case xtpSystemThemeOlive:
		m_clrSplitterGradient.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));
		m_clrNormalCaption.SetStandardValue(RGB(216, 215, 198), RGB(238, 239, 229));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;

	case xtpSystemThemeSilver:
		m_clrSplitterGradient.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));
		m_clrNormalCaption.SetStandardValue(RGB(224, 224, 235), RGB(242, 242, 246));
		m_clrActiveCaption.SetStandardValue(RGB(160, 158, 186), RGB(224, 225, 235));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;
	}

}
