// XTPPopupPaintManager.cpp: implementation of the CXTPPopupPaintManager class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include "stdafx.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPRichRender.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"

#include "../Defines.h"
#include "XTPPopupPaintManager.h"
#include "XTPPopupItem.h"
#include "XTPPopupControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPPopupPaintManager::CXTPPopupPaintManager()
	: m_iButtonBorderInflate(0)
{

}

CXTPPopupPaintManager::~CXTPPopupPaintManager()
{
}

void CXTPPopupPaintManager::RefreshMetrics()
{
	m_fntText.DeleteObject();

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	VERIFY(m_fntText.CreateFontIndirect(&lfIcon));

	m_clrText = GetXtremeColor(COLOR_BTNTEXT);
	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrFrame.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));

	m_clrButtonSelected = m_clrButtonPressed = (COLORREF)-1;
	m_clrButtonPressedBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
	m_clrButtonSelectedBorder.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
}

void CXTPPopupPaintManager::DrawBackground(CDC* pDC, CXTPPopupControl* pControl, CRect rcClient)
{
	if (pControl->GetBackgroundBitmap() > 0)
	{
		CXTPImageManagerIcon* pImage = pControl->GetImageManager()->GetImage(pControl->GetBackgroundBitmap(), 0);
		if (pImage)
		{
			pImage->Draw(pDC, CPoint(0, 0));
		}

	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, rcClient, m_clrBackground, TRUE);

		if (m_clrFrame.clrLight != (COLORREF)-1)
			pDC->Draw3dRect(rcClient, m_clrFrame.clrLight, m_clrFrame.clrDark);
	}
}

void CXTPPopupPaintManager::DrawButton(CDC* pDC, CXTPPopupItem* pItem)
{
	CRect rcButton = pItem->GetRect();

	if (m_iButtonBorderInflate)
		rcButton.InflateRect(m_iButtonBorderInflate, m_iButtonBorderInflate);

	if (pItem->IsPressed() && pItem->IsSelected())
	{
		if (m_clrButtonPressed != (COLORREF)-1)
			pDC->FillSolidRect(rcButton, m_clrButtonPressed);
		pDC->Draw3dRect(rcButton, m_clrButtonPressedBorder.clrLight, m_clrButtonPressedBorder.clrDark);
	}
	else if (pItem->IsSelected() || pItem->IsPressed())
	{
		if (m_clrButtonSelected != (COLORREF)-1)
			pDC->FillSolidRect(rcButton, m_clrButtonSelected);
		pDC->Draw3dRect(rcButton, m_clrButtonSelectedBorder.clrLight, m_clrButtonSelectedBorder.clrDark);
	}
}

void CXTPPopupPaintManager::DrawItemIcon(CDC* pDC, CXTPPopupItem* pItem)
{
	CXTPImageManagerIcon* pIcon = pItem->GetImage();
	CRect rc = pItem->GetRect();

	rc.OffsetRect(pItem->GetIconOffset());

	if (pItem->IsPressed() && pItem->IsSelected())
	{
		if (pItem->IsButton())
			DrawButton(pDC, pItem);

		pIcon->Draw(pDC, rc.TopLeft(), pIcon->GetCheckedIcon());
	}
	else if (pItem->IsSelected() || pItem->IsPressed())
	{
		if (pItem->IsButton())
			DrawButton(pDC, (CXTPPopupItem*)pItem);

		pIcon->Draw(pDC, rc.TopLeft(), pIcon->GetHotIcon());
	}
	else
	{
		pIcon->Draw(pDC, rc.TopLeft(), pIcon->GetIcon());
	}
}

void CXTPPopupPaintManager::SetFont(HFONT hFont)
{
	LOGFONT lf;
	CFont::FromHandle(hFont)->GetLogFont(&lf);

	m_fntText.DeleteObject();
	m_fntText.CreateFontIndirect(&lf);
}

void CXTPPopupPaintManager::DrawItem(CDC* pDC, CXTPPopupItem* pItem)
{
	CRect rc = pItem->GetRect();

	if (pItem->GetBackgroundColor() != (COLORREF)-1)
		pDC->FillSolidRect(rc, pItem->GetBackgroundColor());

	if (pItem->GetBorderColor() != (COLORREF)-1)
		pDC->Draw3dRect(rc, pItem->GetBorderColor(), pItem->GetBorderColor());

	CXTPImageManagerIcon* pIcon = pItem->GetImage();

	if (pIcon)
	{
		DrawItemIcon(pDC, pItem);

		return;
	}

	if (pItem->IsButton())
		DrawButton(pDC, (CXTPPopupItem*)pItem);

	if (!pItem->GetCaption().IsEmpty())
	{
		CXTPRichRender* pRichRender = pItem->m_pRichRender;

		CFont fntUnderline;
		CFont* pFont = pItem->GetTextFont();

		if ((pItem->IsSelected() && pItem->IsHyperLink() && pItem->IsUnderline()) || pItem->IsBold())
		{
			LOGFONT lpLogFont;
			pFont->GetLogFont(&lpLogFont);
			lpLogFont.lfUnderline = (pItem->IsSelected() && pItem->IsHyperLink() && pItem->IsUnderline());
			lpLogFont.lfWeight = pItem->IsBold() ? FW_BOLD : FW_NORMAL;

			VERIFY(fntUnderline.CreateFontIndirect(&lpLogFont));
			pFont = &fntUnderline;
		}
		CRect rcText(pItem->GetRect());

		CXTPFontDC font(pDC, pFont);

		if (pRichRender)
		{
			pRichRender->DrawText(pDC, rcText);
		}
		else
		{
			if ((pItem->GetTextAlignment() & (DT_WORDBREAK | DT_VCENTER)) == (DT_WORDBREAK | DT_VCENTER))
			{
				CRect rcTextHeight(rcText.left, 0, rcText.right, 0);
				pDC->DrawText(pItem->GetCaption(), rcTextHeight, pItem->GetTextAlignment() | DT_CALCRECT);
				rcText.top = (rcText.top + rcText.bottom - rcTextHeight.bottom) / 2;
			}

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(pItem->GetTextColor() == (COLORREF)-1 ? m_clrText : pItem->GetTextColor());

			pDC->DrawText(pItem->GetCaption(), rcText, pItem->GetTextAlignment());
		}
	}
}
