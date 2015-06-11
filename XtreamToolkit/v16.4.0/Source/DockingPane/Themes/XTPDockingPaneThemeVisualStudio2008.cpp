// XTPDockingPaneVisualStudio2008Theme.cpp : implementation of the CXTPDockingPaneVisualStudio2008Theme class.
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
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta2.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2008.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2008Theme

CXTPDockingPaneVisualStudio2008Theme::CXTPDockingPaneVisualStudio2008Theme()
{

}

void CXTPDockingPaneVisualStudio2008Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2005Theme::RefreshMetrics();

	if (m_themeRebar->IsAppThemed())
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pPanelPaintManager->SetColor(xtpTabColorWinNative);
		m_pPanelPaintManager->m_bHotTracking = TRUE;

		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pTabPaintManager->SetColor(xtpTabColorWinNative);
		m_pTabPaintManager->m_bHotTracking = TRUE;
	}
}
