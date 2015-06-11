#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>

#include <TabManager/Includes.h>

#include <DockingPane/XTPDockingPaneDefines.h>
#include <DockingPane/XTPDockingPaneBase.h>
#include <DockingPane/XTPDockingPaneBaseContainer.h>
#include <DockingPane/XTPDockingPaneTabbedContainer.h>
#include <DockingPane/XTPDockingPanePaintManager.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2005.h>


CXTPDockingPaneTabColorSetVisualStudio2005::CXTPDockingPaneTabColorSetVisualStudio2005(BOOL bHighlightSelected)
	: CXTPDockingPaneTabColorSetVisualStudio2003(bHighlightSelected)
{
	m_bTransparentButton = FALSE;
}

void CXTPDockingPaneTabColorSetVisualStudio2005::RefreshMetrics()
{
	CXTPDockingPaneTabColorSetVisualStudio2003 ::RefreshMetrics();

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeSilver:
		m_bGradientButton = TRUE;
		m_clrButtonNormal.SetStandardValue(RGB(242, 242, 247));
		m_clrHeaderFace.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));

		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));
		m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(145, 155, 156));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(RGB(145, 155, 156));
		m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(190, 190, 216));
		break;

	case xtpSystemThemeBlue:
		m_bGradientButton = TRUE;
		m_clrHeaderFace.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));

		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));
		break;

	case xtpSystemThemeOlive:
		m_bGradientButton = TRUE;
		m_clrHeaderFace.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));

		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));
		break;
	}

	m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);
}
