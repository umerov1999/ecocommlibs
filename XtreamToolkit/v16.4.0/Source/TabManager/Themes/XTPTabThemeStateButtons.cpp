#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/XTPTabPaintManager.h>
#include <TabManager/XTPTabPaintManager.inl>
#include <TabManager/XTPTabPaintManagerTheme.h>

#include <TabManager/Themes/XTPTabThemeStateButtons.h>


//////////////////////////////////////////////////////////////////////////
// CAppearanceSetStateButtons

CRect CAppearanceSetStateButtons::FillTabControl(CXTPTabManager* pTabManager, CDC* /*pDC*/, CRect rcControl)
{
	return GetHeaderRect(rcControl, pTabManager);
}


void CAppearanceSetStateButtons::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem = pItem->GetRect();

	GetColorSet()->FillStateButton(pDC, rcItem, pItem);

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
