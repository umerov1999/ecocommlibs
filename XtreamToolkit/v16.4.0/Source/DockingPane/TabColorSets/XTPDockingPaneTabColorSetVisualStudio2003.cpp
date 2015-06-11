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

CXTPDockingPaneTabColorSetVisualStudio2003::CXTPDockingPaneTabColorSetVisualStudio2003(BOOL bHighlightSelected)
{
	m_bHighlightSelected = bHighlightSelected;
	m_bTransparentButton = TRUE;
	m_bGradientButton    = FALSE;
}

void CXTPDockingPaneTabColorSetVisualStudio2003::RefreshMetrics()
{
	CXTPTabColorSetOffice2003::RefreshMetrics();

	if (m_bHighlightSelected)
	{
		m_clrButtonSelected.SetStandardValue(XTPColorManager()->LightColor(
			GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_WINDOW), 0x02));

		m_clrTextSelected.SetStandardValue(GetSysColor(COLOR_WINDOWTEXT));
	}
	else
	{
		m_clrTextSelected.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
	}
	m_clrHeaderFace.SetStandardValue(GetSysColor(COLOR_3DFACE), XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_WINDOW), 0xcd));
	m_clrTextNormal.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
	m_clrButtonNormal.SetStandardValue(RGB(242, 242, 247));

	m_bGradientButton = FALSE;
	m_bLunaTheme = FALSE;

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeSilver:
	case xtpSystemThemeBlue:
	case xtpSystemThemeOlive:
		m_clrTextNormal.SetStandardValue(RGB(113, 111, 100));
		if (!m_bHighlightSelected) m_clrTextSelected.SetStandardValue(RGB(113, 111, 100));
		break;
	}

	m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);
}

void CXTPDockingPaneTabColorSetVisualStudio2003::XTPFillFramePartRect(CDC* pDC, CRect rc, CWnd* pWnd, CWnd* pSite)
{
	::XTPFillFramePartRect(pDC, rc, pWnd, pSite, m_clrHeaderFace, TRUE);
}


void CXTPDockingPaneTabColorSetVisualStudio2003::FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
{
	XTPFillFramePartRect(pDC, rc, (CXTPDockingPaneTabbedContainer*)pTabManager, ((CXTPDockingPaneTabbedContainer*)pTabManager)->GetDockingSite());
}

COLORREF CXTPDockingPaneTabColorSetVisualStudio2003::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	if (m_bTransparentButton && (!m_bHighlightSelected || !pItem->IsSelected()))
		return COLORREF_NULL;

	if (m_bGradientButton)
		return CXTPTabColorSetOffice2003::FillPropertyButton(pDC, rcItem, pItem);

	return CXTPTabColorSetOffice2003::FillPropertyButton(pDC, rcItem, pItem);
}
