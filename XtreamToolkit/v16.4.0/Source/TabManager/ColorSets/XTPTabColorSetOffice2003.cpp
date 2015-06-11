#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

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


//////////////////////////////////////////////////////////////////////////
// CXTPTabColorSetOffice2003

void CXTPTabColorSetOffice2003::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();
	m_clrHeaderFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE), XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0xcd));

	m_csStateButton.clrFace.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_csStateButton.clrBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED_BORDER));

	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrTextHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_clrAutoHideFace = m_clrHeaderFace;

	m_bLunaTheme = FALSE;
	if (!m_pPaintManager->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_clrFrameBorder.SetStandardValue(RGB(59, 97, 156));
			m_csPropertyPage2003.clrFrameBorder.SetStandardValue(RGB(59, 97, 156));
			m_csPropertyPage.clrDarkShadow.SetStandardValue(RGB(0, 53, 154));
			m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(0, 53, 154));
			m_csPropertyPage.clrShadow.SetStandardValue(RGB(117, 166, 241));
			m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(117, 166, 241));
			m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));

			m_csStateButton.clrFace.SetStandardValue(RGB(255, 192, 111));
			m_csStateButton.clrBorder.SetStandardValue(RGB(0, 0, 128));

			m_clrHeaderFace.SetStandardValue(RGB(196, 218, 250), RGB(252, 253, 254));

			m_clrButtonNormal.SetStandardValue(RGB(162, 192, 246));
			m_clrButtonSelected.SetStandardValue(RGB(255, 194, 115));
			m_clrButtonHighlighted.SetStandardValue(RGB(255, 240, 201));

			m_clrAutoHideFace.SetStandardValue(RGB(158, 190, 245), RGB(196, 218, 250));

			m_clrNavigateButtonBorder.SetStandardValue(RGB(0, 0, 128));

			m_bLunaTheme = TRUE;

			break;

		case xtpSystemThemeOlive:
			m_clrFrameBorder.SetStandardValue(RGB(96, 128, 88));
			m_csPropertyPage2003.clrFrameBorder.SetStandardValue(RGB(96, 128, 88));
			m_csPropertyPage.clrDarkShadow.SetStandardValue(RGB(96, 119, 107));
			m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(96, 119, 107));
			m_csPropertyPage.clrShadow.SetStandardValue(RGB(176, 194, 140));
			m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(176, 194, 140));
			m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));

			m_csStateButton.clrFace.SetStandardValue(RGB(255, 192, 111));
			m_csStateButton.clrBorder.SetStandardValue(RGB(63, 93, 56));

			m_clrHeaderFace.SetStandardValue(RGB(242, 241, 228), RGB(252, 254, 254));

			m_clrButtonNormal.SetStandardValue(RGB(208, 222, 170));
			m_clrButtonSelected.SetStandardValue(RGB(255, 194, 115));
			m_clrButtonHighlighted.SetStandardValue(RGB(255, 240, 199));

			m_clrAutoHideFace.SetStandardValue(RGB(217, 217, 167), RGB(242, 241, 228));

			m_clrNavigateButtonBorder.SetStandardValue(RGB(63, 93, 56));

			m_bLunaTheme = TRUE;
			break;

		case xtpSystemThemeSilver:
			m_clrFrameBorder.SetStandardValue(RGB(124, 124, 148));
			m_csPropertyPage2003.clrFrameBorder.SetStandardValue(RGB(124, 124, 148));
			m_csPropertyPage.clrDarkShadow.SetStandardValue(RGB(118, 116, 146));
			m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(118, 116, 146));
			m_csPropertyPage.clrShadow.SetStandardValue(RGB(186, 185, 206));
			m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(186, 185, 206));
			m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));

			m_csStateButton.clrFace.SetStandardValue(RGB(255, 192, 111));
			m_csStateButton.clrBorder.SetStandardValue(RGB(75, 75, 111));

			m_clrHeaderFace.SetStandardValue(RGB(243, 243, 247), RGB(252, 254, 254));

			m_clrButtonNormal.SetStandardValue(RGB(217, 217, 230));
			m_clrButtonSelected.SetStandardValue(RGB(255, 194, 115));
			m_clrButtonHighlighted.SetStandardValue(RGB(255, 238, 194));

			m_clrAutoHideFace.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));

			m_clrNavigateButtonBorder.SetStandardValue(RGB(75, 75, 111));
			m_bLunaTheme = TRUE;

			break;

		default:
			m_clrButtonSelected.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
		}
	}

	if (m_bLunaTheme)
	{
		m_clrButtonPressed.SetStandardValue(RGB(254, 142, 75));
	}

	m_csExcelSelected.clrRightInnerBorder.SetStandardValue(m_clrFrameBorder);
	m_csExcelSelected.clrLeftInnerBorder.SetStandardValue(m_clrFrameBorder);
	m_csExcelSelected.clrTopInnerBorder.SetStandardValue(m_clrFrameBorder);
	m_csExcelSelected.clrTopOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelSelected.clrRightOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelSelected.clrLeftOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelNormal = m_csExcelSelected;

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceStateButtons)
	{
		m_clrAutoHideFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrHeaderFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	}

}
void CXTPTabColorSetOffice2003::Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush)
{
	pDC->FillSolidRect(rc, GetXtremeColor(nBrush));
	pDC->Draw3dRect(rc, GetXtremeColor(nPen), GetXtremeColor(nPen));
}


void CXTPTabColorSetOffice2003::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	int nColor = COLOR_BTNTEXT;

	if (pButton->IsEnabled())
	{
		if ((pButton->IsHighlighted() && pButton->IsPressed()))
		{
			if (!m_bLunaTheme)
			{
				Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
				nColor = XPCOLOR_PUSHED_TEXT;
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(254, 142, 75), RGB(255, 207, 139), FALSE);
				pDC->Draw3dRect(rc, m_clrNavigateButtonBorder, m_clrNavigateButtonBorder);
			}
		}
		else if (pButton->IsPressed() || pButton->IsHighlighted())
		{
			if (!m_bLunaTheme)
			{
				Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT);
				nColor = XPCOLOR_HIGHLIGHT_TEXT;
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(255, 242, 200), RGB(255, 212, 151), FALSE);
				pDC->Draw3dRect(rc, m_clrNavigateButtonBorder, m_clrNavigateButtonBorder);
			}
		}
	}
	pDC->SetTextColor(GetXtremeColor(nColor));

}


COLORREF CXTPTabColorSetOffice2003::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	COLORREF clrItem = GetItemColor(pItem);

	if (pItem->IsPressed())
	{
		clrItem = m_clrButtonPressed;
	}
	else if (pItem->IsHighlighted())
	{
		clrItem = m_clrButtonHighlighted;
	}

	GradientFill(pDC, rcItem, XTPColorManager()->LightColor(clrItem, RGB(255, 255, 255), 300),
		clrItem, pItem->GetTabManager()->GetPosition());

	return clrItem;
}
