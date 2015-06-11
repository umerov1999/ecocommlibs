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

#include <TabManager/Themes/XTPTabThemeOffice2000.h>
#include <TabManager/Themes/XTPTabThemePropertyPageSelected.h>

//////////////////////////////////////////////////////////////////////////
// CAppearanceSetPropertyPageSelected

void CAppearanceSetPropertyPageSelected::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	if (pItem->IsSelected() || pItem->IsHighlighted())
	{
		CXTPTabThemeOffice2000::DrawSingleButton(pDC, pItem);
	}
	else
	{
		m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
	}
}
