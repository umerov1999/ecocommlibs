// XTPTabPaintManager.cpp: implementation of the CXTPTabPaintManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPVC80Helpers.h"

#include "XTPTabManager.h"
#include "XTPTabPaintManager.h"
#include "XTPTabPaintManager.inl"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPTabPaintManagerColorSet

CXTPTabPaintManagerColorSet::CXTPTabPaintManagerColorSet()
{
	m_pPaintManager = NULL;

}

CXTPTabPaintManagerColorSet::~CXTPTabPaintManagerColorSet()
{
}

BOOL CXTPTabPaintManagerColorSet::IsAppThemed() const
{
	return FALSE;
}

void CXTPTabPaintManagerColorSet::RefreshMetrics()
{
	RefreshXtremeColors();

	m_clrFrameBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrHeaderFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrControlFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrPaneFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrBorderHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNHILIGHT));

	m_clrInActiveText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrNormalText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrSelectedText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrHighlightText.SetStandardValue(RGB(0, 0, 128));
	m_clrDisabledText.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));

	m_csStateButton.clrFace.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_csStateButton.clrBorder.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

	m_csPropertyPage2003.clrDarkShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csPropertyPage2003.clrFrameBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	m_csPropertyPage.clrHighlight.SetStandardValue(GetXtremeColor(COLOR_BTNHILIGHT));
	m_csPropertyPage.clrShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csPropertyPage.clrDarkShadow.SetStandardValue(GetXtremeColor(COLOR_3DDKSHADOW));

	m_csExcelSelected.clrRightInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrLeftInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrTopInnerBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csExcelSelected.clrLeftOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelSelected.clrRightOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelSelected.clrTopOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelNormal = m_csExcelSelected;
	m_csExcelNormal.clrLeftOuterBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_csExcelNormal.clrRightOuterBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrButtonNormal.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceExcel)
	{
		m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	}

	m_clrAutoHideFace = m_clrHeaderFace;
}

void CXTPTabPaintManagerColorSet::GradientFill(CDC* pDC, CRect rc, COLORREF clrLight, COLORREF clrDark, XTPTabPosition position)
{
	if (clrLight == XTP_TABMANAGER_COLOR_SHADED)
	{
		WORD bits[] =
		{
			0x0055, 0x00AA, 0x0055, 0x00AA,
			0x0055, 0x00AA, 0x0055, 0x00AA
		};

		CBitmap bitmap;
		bitmap.CreateBitmap(8, 8, 1, 1, &bits);

		CBrush brush;
		brush.CreatePatternBrush(&bitmap);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));

		pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);

		pDC->SelectObject(pOldBrush);
	}
	else
	{
		BOOL bHoriz = IsHorizontalTabPosition(position);
		if (!IsTopLeftTabPosition(position)) SwapColors(clrLight, clrDark);
		if (m_pPaintManager->m_bInvertGradient) SwapColors(clrLight, clrDark);

		XTPDrawHelpers()->GradientFill(pDC, rc, clrLight, clrDark, !bHoriz);
	}
}


void CXTPTabPaintManagerColorSet::FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
{
	COLORREF clrLight = m_clrHeaderFace.clrLight, clrDark = m_clrHeaderFace.clrDark;

	if (clrLight == clrDark && clrLight == GetXtremeColor(COLOR_3DFACE))
	{
		if (pTabManager->DrawParentBackground(pDC, rc))
			return;
	}

	GradientFill(pDC, rc, clrLight, clrDark, pTabManager->GetPosition());

}

COLORREF CXTPTabPaintManagerColorSet::FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
{
	CXTPTabManagerItem* pItem = pTabManager->GetSelectedItem();

	COLORREF clrClient = pItem ? GetItemColor(pItem) : m_clrButtonSelected;

	pDC->FillSolidRect(rc, clrClient);

	return clrClient;
}


void CXTPTabPaintManagerColorSet::SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
{
	pDC->SetTextColor(!pItem->IsEnabled() ? m_clrDisabledText: !pItem->GetTabManager()->IsActive() ? m_clrInActiveText :
		pItem->IsSelected() ? m_clrSelectedText: pItem->IsHighlighted() ? m_clrHighlightText : m_clrNormalText);
}

COLORREF CXTPTabPaintManagerColorSet::GetItemColor(CXTPTabManagerItem* pItem)
{
	if (m_pPaintManager->m_bOneNoteColors)
	{
		return pItem->GetColor();
	}
	return pItem->IsSelected() ? m_clrButtonSelected : m_clrButtonNormal;
}


COLORREF CXTPTabPaintManagerColorSet::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	COLORREF clr = GetItemColor(pItem);

	pDC->FillSolidRect(rcItem, clr);

	return clr;
}

void CXTPTabPaintManagerColorSet::FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	if (!pItem->IsSelected())
	{
		if (pItem->GetTabManager()->IsHorizontalPosition())
			pDC->Draw3dRect(rcItem.right, rcItem.top + 2, 2, rcItem.Height() - 4, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
		else
			pDC->Draw3dRect(rcItem.left + 2, rcItem.bottom, rcItem.Width() - 4, 2, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
	}
	else
	{
		if (m_pPaintManager->m_bOneNoteColors)
		{
			pDC->FillSolidRect(rcItem, GetItemColor(pItem));
		}
		else
		{
			GradientFill(pDC, rcItem, m_csStateButton.clrFace.clrLight, m_csStateButton.clrFace.clrDark, pItem->GetTabManager()->GetPosition());
		}

		pDC->Draw3dRect(rcItem, m_csStateButton.clrBorder.clrLight, m_csStateButton.clrBorder.clrDark);
	}
}

//////////////////////////////////////////////////////////////////////////
// CColorSetDefault

void CXTPTabPaintManager::CColorSetDefault::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_csStateButton.clrFace.SetStandardValue(XTP_TABMANAGER_COLOR_SHADED);
	m_csStateButton.clrBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));

	m_csExcelSelected.clrRightInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrLeftInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrTopInnerBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_csExcelSelected.clrLeftOuterBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_csExcelSelected.clrTopOuterBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csExcelSelected.clrRightOuterBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_csExcelNormal = m_csExcelSelected;
	m_csExcelNormal.clrLeftOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelNormal.clrRightOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
	m_csExcelNormal.clrTopOuterBorder.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceExcel)
	{
		m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrButtonNormal.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
		m_clrFrameBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	}


}

void CXTPTabPaintManager::CColorSetDefault::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	COLORREF clrFrameTopLeft = GetXtremeColor(COLOR_3DSHADOW), clrFrameBottomRight = GetXtremeColor(COLOR_BTNHIGHLIGHT);

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
	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
}

//////////////////////////////////////////////////////////////////////////
// CColorSetVisualStudio2003

void CXTPTabPaintManager::CColorSetVisualStudio2003::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	if (m_pPaintManager->GetAppearance() != xtpTabAppearanceExcel && m_pPaintManager->GetAppearance() != xtpTabAppearanceStateButtons)
		m_clrHeaderFace.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK));

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceVisualStudio)
	{
		m_clrInActiveText.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
		m_clrNormalText.SetStandardValue(GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT));
	}

	m_clrAutoHideFace.SetStandardValue(XTPDrawHelpers()->IsLowResolution() ? GetXtremeColor(COLOR_3DFACE) : m_clrHeaderFace);
}

void CXTPTabPaintManager::CColorSetVisualStudio2003::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceExcel || m_pPaintManager->GetAppearance() == xtpTabAppearanceStateButtons)
	{
		CColorSetDefault::FillNavigateButton(pDC, pButton, rc);
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

//////////////////////////////////////////////////////////////////////////
// CColorSetOffice2003

void CXTPTabPaintManager::CColorSetOffice2003::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();
	m_clrHeaderFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE), XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0xcd));

	m_csStateButton.clrFace.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_csStateButton.clrBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED_BORDER));

	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

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
void CXTPTabPaintManager::CColorSetOffice2003::Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush)
{
	pDC->FillSolidRect(rc, GetXtremeColor(nBrush));
	pDC->Draw3dRect(rc, GetXtremeColor(nPen), GetXtremeColor(nPen));
}


void CXTPTabPaintManager::CColorSetOffice2003::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
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


COLORREF CXTPTabPaintManager::CColorSetOffice2003::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
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


void CXTPTabPaintManager::CColorSetOffice2007::RefreshMetrics()
{
	CColorSetOffice2003::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		m_clrHighlightText.SetStandardValue(0);

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

		m_clrHighlightText.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText")));
		m_clrNormalText.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText")));
		m_clrSelectedText.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabSelectedText")));

	}
	else
	{
		m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
		m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
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
	m_clrButtonText.SetStandardValue(m_clrNormalText);

	m_bLunaTheme = TRUE;
}

void CXTPTabPaintManager::CColorSetOffice2007::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	if (!pImages->IsValid())
	{
		CColorSetDefault::FillNavigateButton(pDC, pButton, rc);
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

//////////////////////////////////////////////////////////////////////////
//

void CXTPTabPaintManager::CColorSetOffice2007Access::RefreshMetrics()
{
	CColorSetOffice2007::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		m_clrSelectedText.SetStandardValue(0);
		m_clrNormalText.SetStandardValue(0);
		m_clrHighlightText.SetStandardValue(0);

		m_clrFrameBorder.SetStandardValue(pImages->GetImageColor(_T("TabManager"), _T("AccessBorder")));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);

		m_clrHeaderFace.SetStandardValue(pImages->GetImageColor(_T("TabManager"), _T("AccessHeader")));
		m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);

		m_clrButtonText.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText")));
	}
}

//////////////////////////////////////////////////////////////////////////
// CColorSetWinNative

CXTPTabPaintManager::CColorSetWinNative::CColorSetWinNative()
{
	m_themeTabButton = new CXTPWinThemeWrapper();
	m_themeToolbar = new CXTPWinThemeWrapper();
}

CXTPTabPaintManager::CColorSetWinNative::~CColorSetWinNative()
{
	SAFE_DELETE(m_themeTabButton);
	SAFE_DELETE(m_themeToolbar);
}


void CXTPTabPaintManager::CColorSetWinNative::RefreshMetrics()
{
	CColorSetDefault::RefreshMetrics();

	m_themeTabButton->OpenThemeData(0, L"TAB");

	if (m_themeTabButton->IsAppThemed())
	{
		m_csPropertyPage.clrHighlight.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
		m_csPropertyPage.clrShadow.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
		m_csPropertyPage.clrDarkShadow.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);


		COLORREF clrClientFace = m_clrPaneFace;
		if (SUCCEEDED(m_themeTabButton->GetThemeColor(TABP_PANE, 0, TMT_FILLCOLORHINT, &clrClientFace)))
		{
			m_clrPaneFace.SetStandardValue(clrClientFace);
		}

		COLORREF clrText;
		if (SUCCEEDED(m_themeTabButton->GetThemeColor(TABP_TOPTABITEM, TIS_NORMAL, TMT_TEXTCOLOR, &clrText)))
		{
			m_clrNormalText.SetStandardValue(clrText);
		}

		if (SUCCEEDED(m_themeTabButton->GetThemeColor(TABP_TOPTABITEM, TIS_SELECTED, TMT_TEXTCOLOR, &clrText)))
		{
			m_clrSelectedText.SetStandardValue(clrText);
		}

		m_clrHighlightText = m_clrNormalText;

		if (SUCCEEDED(m_themeTabButton->GetThemeColor(TABP_TOPTABITEM, TIS_HOT, TMT_TEXTCOLOR, &clrText)))
		{
			m_clrHighlightText.SetStandardValue(clrText);
		}

		m_clrInActiveText = m_clrNormalText;
	}

	m_themeToolbar->OpenThemeData(0, L"TOOLBAR");
}

BOOL CXTPTabPaintManager::CColorSetWinNative::IsAppThemed() const
{
	return m_themeTabButton->IsAppThemed();
}

void CXTPTabPaintManager::CColorSetWinNative::DrawRotatedBitsBottom(int cx, int cy, UINT* pSrcBits, UINT* pDestBits)
{
	UINT* pSrc = &pSrcBits[cx * (cy - 1)];
	for (int i = 0; i < cy; i++)
	{
		pSrcBits = pSrc;
		for (int j = 0; j < cx; j++)
		{
			*pDestBits++ = *pSrcBits++;
		}
		pSrc -= cx;
	}
}

void CXTPTabPaintManager::CColorSetWinNative::DrawRotatedBitsInvertRight(int cx, int cy, UINT* pSrcBits, UINT* pDestBits)
{
	UINT* pSrc = pSrcBits;
	UINT* pDest = pDestBits + cx - 1;

	for (int i = 0; i < cx; i++)
	{
		pSrcBits = pSrc;
		pDestBits = pDest;
		for (int j = 0; j < cy; j++)
		{
			*pDestBits = *pSrcBits;
			pSrcBits++;
			pDestBits += cx;
		}
		pSrc += cy;
		pDest -= 1;
	}
}

void CXTPTabPaintManager::CColorSetWinNative::DrawRotatedBitsRight(int cx, int cy, UINT* pSrcBits, UINT* pDestBits)
{
	UINT* pSrc = pSrcBits;
	UINT* pDest = pDestBits + cx * cy - cx;
	for (int i = 0; i < cy; i++)
	{
		pSrcBits = pSrc;
		pDestBits = pDest;
		for (int j = 0; j < cx; j++)
		{
			*pDestBits++ = *pSrcBits;
			pSrcBits += cy;
		}
		pSrc += 1;
		pDest -= cx;
	}
}

void CXTPTabPaintManager::CColorSetWinNative::DrawRotatedBitsLeft(int cx, int cy, UINT* pSrcBits, UINT* pDestBits)
{
	UINT* pSrc = pSrcBits;
	pDestBits += cx * cy - 1;
	for (int i = 0; i < cy; i++)
	{
		pSrcBits = pSrc;
		for (int j = 0; j < cx; j++)
		{
			*pDestBits-- = *pSrcBits;
			pSrcBits += cy;
		}
		pSrc += 1;
	}
}


void CXTPTabPaintManager::CColorSetWinNative::DrawRotatedButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem, BOOL bSwap, LPFNDRAWROTATEDBITS pfnRotatedProc)
{
	const int cx = rcItem.Width();
	const int cy = rcItem.Height();

	UINT* pSrcBits = NULL;
	HBITMAP hbmSrc = CXTPImageManager::Create32BPPDIBSection(NULL, bSwap ? cy : cx, bSwap ? cx : cy, (LPBYTE*)&pSrcBits);
	if (!pSrcBits)
		return;

	UINT* pDestBits = NULL;
	HBITMAP hbmDest = CXTPImageManager::Create32BPPDIBSection(NULL, cx, cy, (LPBYTE*)&pDestBits);
	if (!pDestBits)
		return;

	CDC dc;
	dc.CreateCompatibleDC(NULL);
	HGDIOBJ hbmpOld = ::SelectObject(dc, hbmSrc);

	CRect rcDraw(0, 0, bSwap ? cy: cx, bSwap ? cx : cy);

	dc.FillSolidRect(rcDraw, GetXtremeColor(COLOR_3DFACE));

	m_themeTabButton->DrawThemeBackground(dc.GetSafeHdc(), pItem->GetIndex() == 0 ? TABP_TOPTABITEMLEFTEDGE :
				pItem->GetIndex() == pItem->GetTabManager()->GetItemCount() - 1 && !pItem->IsSelected() ? TABP_TOPTABITEMRIGHTEDGE : TABP_TOPTABITEM,
		pItem->IsSelected() ? TIS_SELECTED: pItem->IsHighlighted() ? TIS_HOT : TIS_NORMAL, &rcDraw, 0);

	(*pfnRotatedProc)(cx, cy, pSrcBits, pDestBits);

	pDC->DrawState(rcItem.TopLeft(), rcItem.Size(), hbmDest, DST_BITMAP);

	::SelectObject(dc, hbmpOld);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);
}


COLORREF CXTPTabPaintManager::CColorSetWinNative::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	if (!m_themeTabButton->IsAppThemed())
		return CColorSetDefault::FillPropertyButton(pDC, rcItem, pItem);

	if (pItem->IsSelected())
	{
		switch (pItem->GetTabManager()->GetPosition())
		{
			case xtpTabPositionTop:
				rcItem.InflateRect(0, 0, 0, 2);
				break;

			case xtpTabPositionLeft:
				rcItem.InflateRect(0, 0, 1, 0);
				break;
		}
	}

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionTop:
			rcItem.InflateRect(1, 1, 1, 0);
			m_themeTabButton->DrawThemeBackground(pDC->GetSafeHdc(),
				pItem->GetIndex() == 0 ? TABP_TOPTABITEMLEFTEDGE :
				pItem->GetIndex() == pItem->GetTabManager()->GetItemCount() - 1 && !pItem->IsSelected() ? TABP_TOPTABITEMRIGHTEDGE : TABP_TOPTABITEM,
				pItem->IsSelected() ? TIS_SELECTED : pItem->IsHighlighted() ? TIS_HOT : TIS_NORMAL, &rcItem, 0);
			break;

		case xtpTabPositionBottom:
			rcItem.InflateRect(1, 0, 1, 2);
			DrawRotatedButton(pDC, rcItem, pItem, FALSE, DrawRotatedBitsBottom);
			break;

		case xtpTabPositionLeft:
			rcItem.InflateRect(1, 1, 0, 1);
			DrawRotatedButton(pDC, rcItem, pItem, TRUE, DrawRotatedBitsLeft);
			break;

		case xtpTabPositionRight:
			rcItem.InflateRect(0, 1, 2, 1);
			DrawRotatedButton(pDC, rcItem, pItem, TRUE, DrawRotatedBitsRight);
			break;
	}

	return XTP_TABMANAGER_COLOR_NONE;
}

COLORREF CXTPTabPaintManager::CColorSetWinNative::FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
{
	if (!m_themeTabButton->IsAppThemed())
		return CColorSetDefault::FillClient(pDC, rc, pTabManager);

	switch (pTabManager->GetPosition())
	{
		case xtpTabPositionBottom:
			rc.InflateRect(0, 0, 0, 1);
			break;

		case xtpTabPositionRight:
			rc.InflateRect(0, 0, 1, 0);
			break;
	}

	m_themeTabButton->DrawThemeBackground(pDC->GetSafeHdc(), TABP_PANE, 0, &rc, 0);

	return 0;
}

void CXTPTabPaintManager::CColorSetWinNative::SelectClipRgn(CDC* pDC, CRect rcClient, CXTPTabManager* pTabManager)
{
	if (!m_themeTabButton->IsAppThemed())
	{
		CColorSetDefault::SelectClipRgn(pDC, rcClient, pTabManager);
		return;
	}

	CRect rc = m_pPaintManager->GetAppearanceSet()->GetClientRect(rcClient, pTabManager);

	switch (pTabManager->GetPosition())
	{
	case xtpTabPositionBottom:
		pDC->ExcludeClipRect(rc.left, rc.bottom - 2, rc.left + 1, rc.bottom - 1);
		break;

	case xtpTabPositionTop:
	case xtpTabPositionLeft:
		pDC->ExcludeClipRect(rc.left, rc.top, rc.left + 1, rc.top + 1);
		break;

	case xtpTabPositionRight:
		pDC->ExcludeClipRect(rc.right - 2, rc.top, rc.right - 1, rc.top + 1);
		break;
	}
}

void CXTPTabPaintManager::CColorSetWinNative::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	if (!m_themeToolbar->IsAppThemed())
	{
		CColorSetDefault::FillNavigateButton(pDC, pButton, rc);
		return;
	}

	if (pButton->IsEnabled())
	{
		m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON,
			pButton->IsPressed() ? TS_PRESSED :
			pButton->IsHighlighted() ? TS_HOT : TS_NORMAL, &rc, 0);
	}

	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

}

void CXTPTabPaintManager::CColorSetWinNative::FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	if (!m_themeToolbar->IsAppThemed() || !pItem->IsSelected())
	{
		CColorSetDefault::FillStateButton(pDC, rcItem, pItem);
		return;
	}

	m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON,
		pItem->IsHighlighted() ? TS_HOTCHECKED : TS_CHECKED, &rcItem, 0);
}


//////////////////////////////////////////////////////////////////////////
// void CXTPTabPaintManager::CColorSetVisualStudio2005

CXTPTabPaintManager::CColorSetVisualStudio2005::CColorSetVisualStudio2005()
{
	m_bGradientButton = FALSE;
}

void CXTPTabPaintManager::CColorSetVisualStudio2005::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrSelectedText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_bLunaTheme = FALSE;
	m_bGradientButton = FALSE;

	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	if (!m_pPaintManager->IsLunaColorsDisabled())
	{
		CColorSetOffice2003::RefreshMetrics();
	}
	else
	{
		switch (XTPColorManager()->GetCurrentSystemTheme())
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeOlive:
			m_clrHeaderFace.SetStandardValue(RGB(228, 226, 213));
			m_bGradientButton = TRUE;
			break;

		case xtpSystemThemeSilver:
			m_clrButtonNormal.SetStandardValue(RGB(245, 245, 248));
			m_clrHeaderFace.SetStandardValue(RGB(238, 238, 238));
			m_bGradientButton = TRUE;
			break;
		}
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
		m_clrFrameBorder.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
	}
}

COLORREF CXTPTabPaintManager::CColorSetVisualStudio2005::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	COLORREF clrItem = GetItemColor(pItem);

	if (pItem->IsHighlighted())
	{
		clrItem = m_clrButtonHighlighted;
	}

	if (m_bGradientButton)
	{
		GradientFill(pDC, rcItem, XTPColorManager()->LightColor(clrItem, RGB(255, 255, 255), 300),
			clrItem, pItem->GetTabManager()->GetPosition());

	}
	else
	{
		pDC->FillSolidRect(rcItem, clrItem);
	}

	return clrItem;
}


//////////////////////////////////////////////////////////////////////////
// void CXTPTabPaintManager::CColorSetVisualStudio2008

CXTPTabPaintManager::CColorSetVisualStudio2008::CColorSetVisualStudio2008()
{
	m_bGradientButton = FALSE;
}

void CXTPTabPaintManager::CColorSetVisualStudio2008::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrSelectedText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_bLunaTheme = FALSE;
	m_bGradientButton = FALSE;

	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_csPropertyPage2003.clrFrameBorder.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
	m_clrFrameBorder.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));

	m_clrButtonSelected.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clrButtonHighlighted.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrButtonNormal.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	m_clrHeaderFace.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	if (!m_pPaintManager->IsLunaColorsDisabled())
	{
		CColorSetOffice2003::RefreshMetrics();
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

COLORREF CXTPTabPaintManager::CColorSetVisualStudio2008::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
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


//////////////////////////////////////////////////////////////////////////
// void CXTPTabPaintManager::CColorSetVisualStudio2005

CXTPTabPaintManager::CColorSetVisualStudio2010::CColorSetVisualStudio2010(BOOL bPaneColor /* = FALSE */)
{
	m_bGradientButton = FALSE;
	m_bPaneColor = bPaneColor;

	const DWORD _cb[] =
	{
		0x293956, 0x2d3f5c, 0x2d3f5c, 0x2d3f5c,
		0x35496a, 0x2d3f5c, 0x2d3f5c, 0x2d3f5c,
		0x2d3f5c, 0x2d3f5c, 0x293956, 0x2d3f5c,
		0x2d3f5c, 0x2d3f5c, 0x35496a, 0x2d3f5c,
	};

	CBitmap bmp;
	LPBYTE lpBits;
	bmp.Attach(CXTPImageManager::Create32BPPDIBSection(0, 4, 4, &lpBits));

	if (lpBits)
	{
		MEMCPY_S(lpBits, _cb, 4 * 4 * sizeof(DWORD));

		m_brushHeader.CreatePatternBrush(&bmp);
	}
}

void CXTPTabPaintManager::CColorSetVisualStudio2010::FillHeader(CDC* pDC, CRect rc, CXTPTabManager* /*pTabManager*/)
{
	if (m_brushHeader.GetSafeHandle() == 0)
		pDC->FillSolidRect(rc, m_clrHeaderFace);
	else
		FillRect(pDC->GetSafeHdc(), rc, m_brushHeader);
}

void CXTPTabPaintManager::CColorSetVisualStudio2010::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_bLunaTheme = XTPSystemVersion()->IsWinVistaOrGreater();
	m_bGradientButton = TRUE;

	m_clrSelectedText.SetStandardValue(0);
	m_clrHighlightText.SetStandardValue(RGB(255, 255, 255));
	m_clrNormalText.SetStandardValue(RGB(255, 255, 255));
	m_clrInActiveText.SetStandardValue(RGB(206, 212, 221));
	m_clrDisabledText.SetStandardValue(RGB(206, 212, 221));


	m_clrButtonNormal.SetStandardValue(RGB(74, 93, 128));
	m_clrButtonSelected.SetStandardValue(RGB(255, 232, 166));
	m_clrButtonHighlighted.SetStandardValue(RGB(95, 107, 117));
	m_clrButtonPressed.SetStandardValue(RGB(255, 232, 166));

	m_clrHeaderFace.SetStandardValue(RGB(45, 63, 92));

	m_clrAutoHideFace.SetStandardValue(RGB(45, 63, 92));

	m_clrNavigateButtonBorder = RGB(229, 195, 101);

	m_csPropertyPage2003.clrFrameBorder.SetStandardValue(RGB(47, 65, 95));
	m_clrFrameBorder.SetStandardValue(RGB(47, 65, 95));
	m_clrFrameHighlightBorder.SetStandardValue(RGB(155, 167, 183));

	m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(41,58,86));
	m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(41,58,86));
	m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(155, 167, 183));

	if (!m_bLunaTheme)
	{
		m_clrButtonHighlighted.SetStandardValue(RGB(74, 93, 115));
		m_clrButtonNormal.SetStandardValue(RGB(57, 81, 115));
		m_clrButtonSelected.SetStandardValue(RGB(255, 243, 206));
	}

	if (m_bPaneColor)
	{
		m_clrButtonSelected.SetStandardValue(RGB(255, 255, 255));
	}
}

COLORREF CXTPTabPaintManager::CColorSetVisualStudio2010::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	COLORREF clrItem = GetItemColor(pItem);

	if (pItem->IsSelected())
	{
		if (m_bLunaTheme)
		{
			pDC->FillSolidRect(rcItem, m_clrButtonSelected);

			if (!m_bPaneColor)
			{
				CRect rTop = rcItem;

				int nHalf = IsHorizontalTabPosition(pItem->GetTabManager()->GetPosition()) ? rcItem.Height() / 2 : rcItem.Width() / 2;
				CXTPTabPaintManagerAppearanceSet::DeflateRectEx(rTop, CRect(0, 0, 0, nHalf), pItem->GetTabManager()->GetPosition());

				GradientFill(pDC, rTop, RGB(255, 252, 242), RGB(255, 243, 207), pItem->GetTabManager()->GetPosition());
			}
		}
		else
		{
			pDC->FillSolidRect(rcItem, m_clrButtonSelected);
		}

		clrItem = m_clrButtonHighlighted;
	}
	else if (pItem->IsHighlighted())
	{
		GradientFill(pDC, rcItem, RGB(109, 117, 118), m_clrButtonHighlighted, pItem->GetTabManager()->GetPosition());
		clrItem = m_clrButtonHighlighted;
	}
	else
	{
		pDC->FillSolidRect(rcItem, clrItem);
	}

	return clrItem;
}

void CXTPTabPaintManager::CColorSetVisualStudio2010::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	COLORREF clrColor = pButton->GetItem() ?
		(pButton->GetItem()->IsSelected() ? RGB(117, 99, 61) :
	pButton->GetItem()->IsHighlighted() ? RGB(206, 212, 221) : RGB(41, 56, 82)) : RGB(206, 212, 221);

	if (pButton->IsEnabled())
	{
		if ((pButton->IsHighlighted() && pButton->IsPressed()))
		{
			pDC->FillSolidRect(rc, RGB(255, 232, 166));
			pDC->Draw3dRect(rc, m_clrNavigateButtonBorder, m_clrNavigateButtonBorder);
			clrColor = RGB(0, 0, 0);
		}
		else if (pButton->IsPressed() || pButton->IsHighlighted())
		{
			pDC->FillSolidRect(rc, RGB(255, 252, 244));
			pDC->Draw3dRect(rc, m_clrNavigateButtonBorder, m_clrNavigateButtonBorder);
			clrColor = RGB(0, 0, 0);
		}
		else
		{

		}
	}
	pDC->SetTextColor(clrColor);
}

void CXTPTabPaintManager::CColorSetVisualStudio2010::SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
{
	pDC->SetTextColor(!pItem->IsEnabled() ? m_clrDisabledText:
		pItem->IsSelected() ? m_clrSelectedText: pItem->IsHighlighted() ? m_clrHighlightText : m_clrNormalText);
}
