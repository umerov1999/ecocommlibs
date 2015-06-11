#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/ColorSets/XTPTabColorSetDefault.h>
#include <TabManager/ColorSets/XTPTabColorSetVisualStudio2003.h>


//////////////////////////////////////////////////////////////////////////
// CColorSetVisualStudio2003

void CColorSetVisualStudio2003::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	if (m_pPaintManager->GetAppearance() != xtpTabThemeExcel2003 && m_pPaintManager->GetAppearance() != xtpTabAppearanceStateButtons)
		m_clrHeaderFace.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK));

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceVisualStudio)
	{
		m_clrTextInactive.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
		m_clrTextNormal.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
	}

	m_clrAutoHideFace.SetStandardValue(XTPDrawHelpers()->IsLowResolution() ? GetXtremeColor(COLOR_3DFACE) : m_clrHeaderFace);
}

void CColorSetVisualStudio2003::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	if (m_pPaintManager->GetAppearance() == xtpTabThemeExcel2003 || m_pPaintManager->GetAppearance() == xtpTabAppearanceStateButtons)
	{
		CXTPTabColorSetDefault::FillNavigateButton(pDC, pButton, rc);
		return;
	}

	COLORREF clrFrameTopLeft = GetXtremeColor(COLOR_BTNTEXT), clrFrameBottomRight = GetXtremeColor(COLOR_3DFACE);

	if (pButton->IsEnabled())
	{
		if (pButton->IsPressed())
		{
			pDC->Draw3dRect(rc, clrFrameTopLeft, clrFrameBottomRight);
			rc.OffsetRect(1, 1);
		}
		else if (pButton->IsHighlighted())
			pDC->Draw3dRect(rc, clrFrameBottomRight, clrFrameTopLeft);
	}
	pDC->SetTextColor(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
}
