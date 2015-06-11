// XTPRibbonBackstageViewThemeResource.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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

#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPKeyboardManager.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPPaintManager.h>

#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>
#include <Ribbon/XTPRibbonSystemButton.h>
#include <Ribbon/Backstage/XTPRibbonBackstageView.h>
#include <Ribbon/Backstage/XTPRibbonBackstageCommand.h>
#include <Ribbon/Backstage/XTPRibbonBackstageTab.h>
#include <Ribbon/Backstage/XTPRibbonBackstagePaintManager.h>
#include <Ribbon/Backstage/XTPRibbonBackstageViewThemeResource.h>

void CXTPRibbonBackstageViewThemeResource::DrawBackground(CDC *pDC, CXTPRibbonBackstageView *pView)
{
	CXTPClientRect rc(pView);

	pDC->FillSolidRect(rc, RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);

	COLORREF clrText = XTPResourceImages()->GetImageColor(_T("Ribbon"), _T("BackstageMenuTextColor"), 0);
	pDC->SetTextColor(clrText);


	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageTopBorder"));
	if (pImage)
	{
		pImage->DrawImage(pDC, CRect(0, 0, rc.Width(), 2), pImage->GetSource());
	}

	pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuBackground"));

	if (pImage)
	{
		pImage->DrawImage(pDC, CRect(0, 2, pView->GetMenuWidth(), rc.bottom), pImage->GetSource());
	}
}


void CXTPRibbonBackstageViewThemeResource::DrawControl(CDC *pDC, CXTPRibbonBackstageView *pView, CXTPControl *pControl)
{
	if (!pControl->IsVisible())
		return;

	BOOL bEnabled     = pControl->GetEnabled();
	BOOL bActiveTab   = pView->IsActiveTab(pControl);
	BOOL bHighlighted = pView->IsHighlightedControl(pControl);
	BOOL bFocused     = pView->IsFocusedControl(pControl);

	COLORREF clrText = XTPResourceImages()->GetImageColor(_T("Ribbon"), _T("BackstageMenuTextColor"), 0);

	if (!clrText)
		clrText = pDC->GetTextColor();

	if(bActiveTab)
		clrText = RGB(255, 255, 255);

	if (!bEnabled)
		clrText = GetXtremeColor(XPCOLOR_GRAYTEXT);

	pDC->SetTextColor(clrText);

	if (pView->IsCommand(pControl))
	{
		if (bHighlighted || bFocused)
		{
			CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuCommand"));
			if (pImage)
			{
				pImage->DrawImage(pDC, pControl->GetRect(), pImage->GetSource(!pControl->GetEnabled() ? 2 : 0, 3), CRect(2, 2, 2, 2));
			}
		}

		CXTPFontDC font(pDC, &pView->m_fntCommand);

		CRect rcText(pControl->GetRect());
		rcText.DeflateRect(m_nTextGap, 2, 2, 2);

		CXTPImageManagerIcon* pIcon = pView->GetImageManager()->GetImage(pControl->GetIconId());
		if (pIcon)
		{
			// Office 2010 respects this rule of 120 DPI
			CSize szIconScalled(XTPDpiHelper()->ScaleX(pIcon->GetWidth(), 120), XTPDpiHelper()->ScaleY(pIcon->GetHeight(), 120));

			CPoint ptIcon = CPoint(rcText.left, (rcText.top + rcText.bottom - szIconScalled.cy ) / 2);

			pIcon->Draw(pDC, ptIcon, pIcon->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled), szIconScalled);

			rcText.left += XTPDpiHelper()->ScaleX(pIcon->GetWidth(), 120) + m_nImageMargin;
		}

		pDC->DrawText(pControl->GetCaption(), rcText, DT_HIDEPREFIX | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if (bHighlighted || bActiveTab || bFocused)
		{
			CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuPage"));
			if (pImage)
			{
				pImage->DrawImage(pDC, pControl->GetRect(), pImage->GetSource(!pControl->GetEnabled() ? 3 : bActiveTab ? 1 : 0, 4), CRect(2, 2, 2, 2));
			}

			if (bActiveTab)
			{
				CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuPageGlyph"));
				if (pImage)
				{
					CRect rcSrc(pImage->GetSource());
					CRect rc(pControl->GetRect());
					CRect rcGlyph(CPoint(rc.right - rcSrc.Width(), (rc.top + rc.bottom - rcSrc.Height()) / 2), rcSrc.Size());

					pImage->DrawImage(pDC, rcGlyph, rcSrc);
				}
			}

		}

		CXTPFontDC font(pDC, &pView->m_fntTab);

		CRect rcText(pControl->GetRect());
		rcText.DeflateRect(m_nTextGap + m_nCommandGap, 2, 2, 2);

		CXTPImageManagerIcon* pIcon = pView->GetImageManager()->GetImage(pControl->GetIconId());
		if (pIcon)
		{
			pIcon->Draw(pDC, CPoint(rcText.left, (rcText.top + rcText.bottom - pIcon->GetHeight()) / 2), pIcon->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled));

			rcText.left += pIcon->GetWidth() + m_nImageMargin;
		}


		pDC->DrawText(pControl->GetCaption(), rcText, DT_HIDEPREFIX | DT_VCENTER | DT_SINGLELINE);
	}
}
