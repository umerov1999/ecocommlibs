#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <TabManager/Includes.h>
#include <TabManager/Themes/XTPTabThemeVisualStudio2012.h>


CXTPTabThemeVisualStudio2012::CXTPTabThemeVisualStudio2012()
	: CXTPTabThemeFlat()
{
}

void CXTPTabThemeVisualStudio2012::RefreshMetrics()
{
	CXTPTabThemeFlat::RefreshMetrics();
}

void CXTPTabThemeVisualStudio2012::DrawSingleButton(CDC *pDC, CXTPTabManagerItem *pItem)
{
	CRect rcItem(pItem->GetRect());

	CXTPTabPaintManagerColorSet *pColorSet = GetColorSet();

	pColorSet->FillPropertyButton(pDC, rcItem, pItem);
	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
