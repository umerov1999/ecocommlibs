#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <TabManager/Includes.h>
#include <DockingPane/TabThemes/XTPDockingPanePanelThemeVisualStudio2012.h>


CXTPDockingPanePanelThemeVisualStudio2012::CXTPDockingPanePanelThemeVisualStudio2012()
	: CXTPTabThemeFlat()
{

}

void CXTPDockingPanePanelThemeVisualStudio2012::RefreshMetrics()
{
	CXTPTabThemeFlat::RefreshMetrics();

	m_nGripperWidth = 6;
	m_nButtonHeight = 29;
	m_rcHeaderMargin.SetRect(0, 0, 0, 0);
}

void CXTPDockingPanePanelThemeVisualStudio2012::DrawText(CDC *pDC, CXTPTabManager *pManager, const CString &str, LPRECT lpRect, UINT nFormat)
{
	CXTPTabThemeFlat::DrawText(pDC, pManager, str, lpRect, nFormat);
}

void CXTPDockingPanePanelThemeVisualStudio2012::DrawSingleButton(CDC *pDC, CXTPTabManagerItem *pItem)
{
	CXTPTabPaintManagerColorSet *pColorSet = GetColorSet();

	CRect rcItem(pItem->GetRect());

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionTop:
		{
			pDC->FillSolidRect(rcItem, pColorSet->m_clrAutoHideFace);

			CRect rcGripper(rcItem.left, rcItem.bottom-m_nGripperWidth, rcItem.right, rcItem.bottom);
			pDC->FillSolidRect(rcGripper, pColorSet->GetItemColor(pItem));

			rcItem.right  += 5;
			rcItem.left   -= 5;
			rcItem.bottom -= m_nGripperWidth;
		}
		break;

		case xtpTabPositionBottom:
		{
			pDC->FillSolidRect(rcItem, pColorSet->m_clrAutoHideFace);

			CRect rcGripper(rcItem.left, rcItem.top, rcItem.right, rcItem.top+m_nGripperWidth);
			pDC->FillSolidRect(rcGripper, pColorSet->GetItemColor(pItem));

			rcItem.right  += 5;
			rcItem.left   -= 5;
			rcItem.top    += m_nGripperWidth;
		}
		break;

		case xtpTabPositionLeft:
		{
			pDC->FillSolidRect(rcItem, pColorSet->m_clrAutoHideFace);

			CRect rcGripper(rcItem.right - m_nGripperWidth, rcItem.top, rcItem.right, rcItem.bottom - 10);
			pDC->FillSolidRect(rcGripper, pColorSet->GetItemColor(pItem));

			rcItem.right  -= m_nGripperWidth;
			rcItem.top    -= 5;
			rcItem.bottom -= 5;
		}
		break;

		case xtpTabPositionRight:
		{
			pDC->FillSolidRect(rcItem, pColorSet->m_clrAutoHideFace);

			CRect rcGripper(rcItem.left, rcItem.top, rcItem.left + m_nGripperWidth, rcItem.bottom - 10);
			pDC->FillSolidRect(rcGripper, pColorSet->GetItemColor(pItem));

			rcItem.left   += m_nGripperWidth;
			rcItem.top    -= 5;
			rcItem.bottom -= 5;
		}
		break;
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rcItem, TRUE);
}
