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

#include <TabManager/Themes/XTPTabThemeFlat.h>


//////////////////////////////////////////////////////////////////////////
// CXTPTabThemeFlat

void CXTPTabThemeFlat::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem(pItem->GetRect());

	InflateRectEx(rcItem, CRect(0, 0, 1, 0), pItem->GetTabManager()->GetPosition());

	GetColorSet()->FillPropertyButton(pDC, rcItem, pItem);

	if (pItem->IsSelected())
		InflateRectEx(rcItem, CRect(0, 0, 0, 1), pItem->GetTabManager()->GetPosition());

	pDC->Draw3dRect(rcItem, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);


	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}


CRect CXTPTabThemeFlat::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{

	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);

	CRect rcClient = GetClientRect(rcControl, pTabManager);

	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(), GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}


	return rcHeader;
}
