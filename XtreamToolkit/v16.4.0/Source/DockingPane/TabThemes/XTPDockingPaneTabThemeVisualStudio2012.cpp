#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <TabManager/Includes.h>
#include <DockingPane/TabThemes/XTPDockingPaneTabThemeVisualStudio2012.h>


CXTPDockingPaneTabThemeVisualStudio2012::CXTPDockingPaneTabThemeVisualStudio2012()
	: CXTPTabThemeFlat()
{

}

void CXTPDockingPaneTabThemeVisualStudio2012::RefreshMetrics()
{
	CXTPTabThemeFlat::RefreshMetrics();

//  m_nButtonHeight = 29;
	m_rcHeaderMargin.SetRect(0, 0, 0, 0);
}

void CXTPDockingPaneTabThemeVisualStudio2012::DrawText(CDC *pDC, CXTPTabManager *pManager, const CString &str, LPRECT lpRect, UINT nFormat)
{
	CXTPTabThemeFlat::DrawText(pDC, pManager, str, lpRect, nFormat);
}

void CXTPDockingPaneTabThemeVisualStudio2012::DrawSingleButton(CDC *pDC, CXTPTabManagerItem *pItem)
{
	CXTPTabPaintManagerColorSet *pColorSet = GetColorSet();

	CRect rcItem(pItem->GetRect());
	CRect rcBorder(rcItem.right-1, rcItem.top, rcItem.right, rcItem.bottom);

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionTop:
		case xtpTabPositionBottom:
		{
			pDC->FillSolidRect(rcItem, pColorSet->GetItemColor(pItem));
			pDC->FillSolidRect(rcBorder, pColorSet->m_clrButtonBorder);
		}
		break;

		case xtpTabPositionLeft:
		{
			ASSERT(FALSE);
		}
		break;

		case xtpTabPositionRight:
		{
			ASSERT(FALSE);
		}
		break;
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rcItem, TRUE);
}
