#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>

#include <TabManager/Includes.h>
#include <TabManager/ColorSets/XTPTabColorSetDefault.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2003.h>

#include <DockingPane/XTPDockingPaneDefines.h>
#include <DockingPane/XTPDockingPaneBase.h>
#include <DockingPane/XTPDockingPaneBaseContainer.h>
#include <DockingPane/XTPDockingPaneTabbedContainer.h>
#include <DockingPane/XTPDockingPanePaintManager.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetOfficePanel.h>

CXTPDockingPaneTabColorSetOfficePanel::CXTPDockingPaneTabColorSetOfficePanel()
	: CXTPDockingPaneTabColorSetVisualStudio2003(FALSE)
{
}

void CXTPDockingPaneTabColorSetOfficePanel::RefreshMetrics()
{
	CXTPDockingPaneTabColorSetVisualStudio2003::RefreshMetrics();

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceFlat)
	{
		m_clrTextSelected.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
		m_clrTextNormal.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
	}
}
