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
#include <TabManager/ColorSets/XTPTabColorSetOffice2007.h>


void CXTPTabColorSetOffice2007::RefreshMetrics()
{
	CXTPTabColorSetOffice2003::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		m_clrTextHighlight.SetStandardValue(0);

		m_clrFrameBorder.SetStandardValue(pImages->GetImageColor(_T("TabManager"), _T("FrameBorder")));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);
		m_csPropertyPage.clrDarkShadow.SetStandardValue(pImages->GetImageColor(_T("TabManager"), _T("DarkShadow")));
		m_csPropertyPage2003.clrDarkShadow.SetStandardValue(m_csPropertyPage.clrDarkShadow);

		m_csPropertyPage.clrShadow.SetStandardValue(pImages->GetImageColor(_T("TabManager"), _T("LightShadow")));
		m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(m_csPropertyPage.clrShadow);
		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));

		m_clrHeaderFace.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("RibbonFace")));

		m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);

		m_clrButtonNormal.SetStandardValue(pImages->GetImageColor(_T("Window"), _T("ButtonFace")));
		m_clrButtonSelected.SetStandardValue(pImages->GetImageColor(_T("Window"), _T("HighlightChecked")));
		m_clrButtonHighlighted.SetStandardValue(pImages->GetImageColor(_T("Window"), _T("HighlightSelected")));
		m_clrNavigateButtonBorder.SetStandardValue(pImages->GetImageColor(_T("Window"), _T("WindowFrame")));

		m_clrTextHighlight.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText")));
		m_clrTextNormal.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText")));
		m_clrTextSelected.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabSelectedText")));

	}
	else
	{
		m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
		m_clrTextHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
		m_clrFrameBorder.SetStandardValue(RGB(59, 97, 156));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);
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
	}
	m_clrButtonText.SetStandardValue(m_clrTextNormal);

	m_bLunaTheme = TRUE;
}

void CXTPTabColorSetOffice2007::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	if (!pImages->IsValid())
	{
		CXTPTabColorSetDefault::FillNavigateButton(pDC, pButton, rc);
		return;
	}

	if (pButton->GetItem() == NULL) pDC->SetTextColor(m_clrButtonText);

	if (pButton->IsEnabled() && (pButton->IsPressed() || pButton->IsHighlighted()))
	{
		CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLBARBUTTONS22"));
		ASSERT(pImage);
		if (!pImage)
			return;

		int nState = pButton->IsPressed() ? 1 : 0;

		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 4), CRect(8, 8, 8, 8), COLORREF_NULL);

		pDC->SetTextColor(0);
	}
}
