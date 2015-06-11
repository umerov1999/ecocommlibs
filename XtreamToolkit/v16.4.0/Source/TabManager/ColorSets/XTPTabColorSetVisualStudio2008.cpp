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
#include <TabManager/ColorSets/XTPTabColorSetOffice2003.h>
#include <TabManager/ColorSets/XTPTabColorSetVisualStudio2008.h>


//////////////////////////////////////////////////////////////////////////
// void CXTPTabColorSetVisualStudio2008

CXTPTabColorSetVisualStudio2008::CXTPTabColorSetVisualStudio2008()
{
	m_bGradientButton = FALSE;
}

void CXTPTabColorSetVisualStudio2008::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrTextSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_bLunaTheme = FALSE;
	m_bGradientButton = FALSE;

	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrTextHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_csPropertyPage2003.clrFrameBorder.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
	m_clrFrameBorder.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));

	m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrButtonNormal.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	m_clrHeaderFace.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	if (!m_pPaintManager->IsLunaColorsDisabled())
	{
		CXTPTabColorSetOffice2003::RefreshMetrics();
	}
	else
	{
		switch (XTPColorManager()->GetCurrentSystemTheme())
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
			m_clrButtonSelected.SetStandardValue(RGB(193, 210, 238));
			m_clrButtonHighlighted.SetStandardValue(RGB(167, 191, 227));
			m_clrFrameBorder.SetStandardValue(RGB(152, 181, 226));
			m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);
			m_bGradientButton = TRUE;
			break;

		case xtpSystemThemeAero:
			m_clrButtonSelected.SetStandardValue(RGB(210, 230, 250));
			m_clrButtonNormal.SetStandardValue(RGB(152, 180, 210));
			m_clrButtonHighlighted.SetStandardValue(RGB(129, 208, 241));
			m_clrFrameBorder.SetStandardValue(RGB(105, 161, 191));
			m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);
			m_bGradientButton = TRUE;
			break;


		case xtpSystemThemeOlive:
			m_clrButtonSelected.SetStandardValue(RGB(182, 198, 141));
			m_clrButtonHighlighted.SetStandardValue(RGB(175, 175, 142));
			m_clrFrameBorder.SetStandardValue(RGB(147, 160, 112));
			m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);
			m_bGradientButton = TRUE;
			break;

		case xtpSystemThemeSilver:
			m_clrButtonSelected.SetStandardValue(RGB(225, 226, 236));
			m_clrButtonHighlighted.SetStandardValue(RGB(255, 201, 128));
			m_clrFrameBorder.SetStandardValue(RGB(147, 145, 176));
			m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);
			m_bGradientButton = TRUE;
			break;
		}
	}
}

COLORREF CXTPTabColorSetVisualStudio2008::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	COLORREF clrItem = GetItemColor(pItem);

	if (pItem->IsHighlighted() && !pItem->IsSelected())
	{
		clrItem = m_clrButtonHighlighted;
	}

	if (m_bGradientButton || pItem->IsSelected())
	{
		GradientFill(pDC, rcItem, RGB(253, 253, 253), clrItem, pItem->GetTabManager()->GetPosition());

	}
	else
	{
		pDC->FillSolidRect(rcItem, clrItem);
	}

	return clrItem;
}
