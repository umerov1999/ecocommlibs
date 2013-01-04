// XTPShortcutListBoxTheme.cpp: implementation of the CXTPShortcutListBoxTheme class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPColorManager.h"
#include "../Util/XTPControlTheme.h"


#include "XTPShortcutListBox.h"
#include "XTPShortcutListBoxTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//===========================================================================
// CXTPShortcutListBoxTheme class
//===========================================================================

CXTPShortcutListBoxTheme::CXTPShortcutListBoxTheme()
{
}

CXTPShortcutListBoxTheme::~CXTPShortcutListBoxTheme()
{

}

void CXTPShortcutListBoxTheme::RefreshMetrics(CXTPShortcutListBox* /*pList*/)
{
	m_cxIcon = ::GetSystemMetrics(SM_CXICON);
	m_cyIcon = ::GetSystemMetrics(SM_CYICON);
	m_cyEdge = ::GetSystemMetrics(SM_CYEDGE)*2;

	// normal colors
	m_crgBack.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_crgItemBorder.SetStandardValue(GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
	m_crItemText.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_crItemTextDisabled.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));

	// hot, pressed colors
	m_crgItemBackHot.SetStandardValue(COLORREF_NULL, COLORREF_NULL);
	m_crgItemBackPressed.SetStandardValue(COLORREF_NULL, COLORREF_NULL);
}

void CXTPShortcutListBoxTheme::DrawItem(CXTPShortcutListBox* pMenuListBox, LPDRAWITEMSTRUCT lpDIS)
{
	// Define temporary variables.
	CDC*  pDC = CDC::FromHandle (lpDIS->hDC);
	CRect rcIcon = lpDIS->rcItem;
	CRect rcText = lpDIS->rcItem;
	int   iItemID = lpDIS->itemID;

	// if empty, draw focus rect.
	if (iItemID < 0)
	{
		rcIcon.bottom = rcIcon.top + 15;
		pDC->DrawFocusRect(rcIcon);
		return;
	}

	// Get the menu item.
	CXTPShortcutListBox::CONTENT_ITEM* pCI = pMenuListBox->GetMenuItem(iItemID);
	ASSERT(pCI);
	if (!pCI)
		return;

	// set the text foreground and background colors.
	pDC->SetTextColor(GetTextColor());

	// repaint the background.
	pDC->SetBkMode(TRANSPARENT);

	long iMargin = (rcIcon.Width()-m_cxIcon)/2;

	rcIcon.top += m_cyEdge;
	rcIcon.left += iMargin;
	rcIcon.right = rcIcon.left + m_cxIcon;
	rcIcon.bottom = rcIcon.top + m_cyIcon;

	int iOffset = 0;

	BOOL bSelected = FALSE;
	BOOL bPressed = FALSE;

	if ((lpDIS->itemState & ODS_SELECTED) && pCI->m_bEnabled)
	{
		bSelected = TRUE;

		CRect rcBorder(rcIcon);
		rcBorder.InflateRect(2, 2);

		if (::GetKeyState(VK_LBUTTON) < 0)
		{
			if (!m_crgItemBackPressed.IsNull())
				XTPDrawHelpers()->GradientFill(pDC, rcBorder, m_crgItemBackPressed, FALSE);

			pDC->Draw3dRect(&rcBorder,
				m_crgItemBorder.clrLight, m_crgItemBorder.clrDark);

			iOffset = 1;
			bPressed = TRUE;
		}
		else
		{
			if (!m_crgItemBackHot.IsNull())
				XTPDrawHelpers()->GradientFill(pDC, rcBorder, m_crgItemBackHot, FALSE);

			pDC->Draw3dRect(&rcBorder,
				m_crgItemBorder.clrDark, m_crgItemBorder.clrLight);

			iOffset = -1;
		}
	}

	LOGFONT lf;
	pMenuListBox->GetFont()->GetLogFont(&lf);

	if (bSelected && !pMenuListBox->m_bNoUnderline)
		lf.lfUnderline = 1;

	// select the font used by the button text.
	CFont font;
	font.CreateFontIndirect(&lf);
	CXTPFontDC fnt(pDC, &font);

	// get the text height.
	int iTextHeight = pMenuListBox->GetTextHeight(pCI->m_strText);

	// define the text rect.
	rcText.top = rcText.bottom - (iTextHeight + m_cyEdge);
	rcText.left += m_cyEdge;
	rcText.right -= m_cyEdge;

	if ((pMenuListBox->m_bNoBorder) && (iOffset != 0))
	{
		rcIcon.OffsetRect(iOffset, iOffset);
		rcText.OffsetRect(iOffset, iOffset);
	}

	if (pCI->m_pIcon)
	{
		DrawImage(pDC, rcIcon, pCI->m_pIcon, pCI->m_bEnabled, bSelected, bPressed);
	}

	DrawText(pDC, rcText, pCI);
}

void CXTPShortcutListBoxTheme::DrawImage(CDC* pDC, CRect& rcIcon, CXTPImageManagerIcon* pImage, BOOL bEnabled, BOOL /*bSelected*/, BOOL /*bPressed*/)
{
	CPoint pt(rcIcon.TopLeft());

	if (bEnabled)
	{
		pImage->Draw(pDC, pt);
	}
	else
	{
		if (pImage->HasDisabledIcon() || pImage->IsAlpha())
		{
			pImage->Draw(pDC, pt, pImage->GetDisabledIcon());
		}
		else
		{
			pDC->DrawState(pt, rcIcon.Size(), pImage->GetIcon(), DST_ICON | DSS_DISABLED, (HBRUSH)NULL);
		}
	}
}

void CXTPShortcutListBoxTheme::DrawText(CDC* pDC, CRect& rcText, CXTPShortcutListBox::CONTENT_ITEM* pCI)
{
	if (!pCI->m_bEnabled)
	{
		CRect rc = rcText;
		rc.OffsetRect(1, 1);

		pDC->SetTextColor(GetXtremeColor(COLOR_WINDOW));
		pDC->DrawText(pCI->m_strText, rc,
			DT_CENTER | DT_TOP | DT_END_ELLIPSIS);

		pDC->SetTextColor(GetXtremeColor(COLOR_3DSHADOW));
	}

	pDC->DrawText(pCI->m_strText, rcText,
		DT_CENTER | DT_TOP | DT_END_ELLIPSIS);
}

COLORREF CXTPShortcutListBoxTheme::GetBackColor()
{
	return m_crgBack;
}

COLORREF CXTPShortcutListBoxTheme::GetTextColor()
{
	return m_crItemText;
}

//////////////////////////////////////////////////////////////////////////
// CXTPShortcutListBoxThemeOfficeXP

CXTPShortcutListBoxThemeOfficeXP::CXTPShortcutListBoxThemeOfficeXP()
{
	m_nTheme = xtpControlThemeOfficeXP;
}

void CXTPShortcutListBoxThemeOfficeXP::RefreshMetrics(CXTPShortcutListBox* pList)
{
	CXTPShortcutListBoxTheme::RefreshMetrics(pList);

	// normal colors
	m_crgBack.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_crItemText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_crgItemBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_crItemTextDisabled.SetStandardValue(GetXtremeColor(XPCOLOR_GRAYTEXT));

	// hot, pressed colors
	m_crgItemBackHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT), GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_crgItemBackPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED), GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
}

void CXTPShortcutListBoxThemeOfficeXP::DrawImage(CDC* pDC, CRect& rcIcon, CXTPImageManagerIcon* pImage, BOOL bEnabled, BOOL bSelected, BOOL bPressed)
{
	ASSERT(pImage);
	if (!pImage)
		return;
	CPoint pt(rcIcon.TopLeft());

	if (bEnabled)
	{
		if (!bSelected && !bPressed)
		{
			pImage->Draw(pDC, pt, pImage->GetFadedIcon());
		}
		else if (bSelected && !bPressed)
		{
			pImage->Draw(pDC, CPoint(pt.x + 1, pt.y + 1), pImage->GetShadowIcon());
			pImage->Draw(pDC, CPoint(pt.x - 1, pt.y - 1), pImage->GetHotIcon());
		}
		else
		{
			pImage->Draw(pDC, rcIcon.TopLeft());
		}
	}
	else
	{
		if (pImage->HasDisabledIcon() || pImage->IsAlpha())
		{
			pImage->Draw(pDC, pt, pImage->GetDisabledIcon());
		}
		else
		{
			pImage->DrawMono(pDC, pt, pImage->GetIcon(), 0, m_crItemTextDisabled);
		}
	}
}

void CXTPShortcutListBoxThemeOfficeXP::DrawText(CDC* pDC, CRect& rcText, CXTPShortcutListBox::CONTENT_ITEM* pCI)
{
	pDC->SetTextColor(pCI->m_bEnabled ? m_crItemText : m_crItemTextDisabled);
	pDC->DrawText(pCI->m_strText, rcText, DT_CENTER | DT_TOP | DT_END_ELLIPSIS);
}

//////////////////////////////////////////////////////////////////////////
//  CXTPShortcutListBoxThemeOffice2003

CXTPShortcutListBoxThemeOffice2003::CXTPShortcutListBoxThemeOffice2003()
{
	m_nTheme = xtpControlThemeOffice2003;
}

void CXTPShortcutListBoxThemeOffice2003::RefreshMetrics(CXTPShortcutListBox* pList)
{
	CXTPShortcutListBoxThemeOfficeXP::RefreshMetrics(pList);

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			// normal colors
			m_crgBack.SetStandardValue(RGB(0xdd, 0xec, 0xfe), RGB(0x4a, 0x7a, 0xc9));
			m_crgItemBorder.SetStandardValue(RGB(0x00, 0x00, 0x80), RGB(0x00, 0x00, 0x80));
			m_crItemText.SetStandardValue(RGB(0x00, 0x00, 0x00));
			m_crItemTextDisabled.SetStandardValue(RGB(0x6d, 0x96, 0xd0));

			// hot, pressed colors
			m_crgItemBackHot.SetStandardValue(RGB(0xff, 0xf0, 0xc5), RGB(0xff, 0xd6, 0x9b));
			m_crgItemBackPressed.SetStandardValue(RGB(0xfe, 0x90, 0x4d), RGB(0xfe, 0xc9, 0x85));
			break;

		case xtpSystemThemeOlive:
			// normal colors
			m_crgBack.SetStandardValue(RGB(0xf3, 0xf2, 0xe7), RGB(0x9f, 0xab, 0x80));
			m_crgItemBorder.SetStandardValue(RGB(0x3f, 0x5d, 0x38), RGB(0x3f, 0x5d, 0x38));
			m_crItemText.SetStandardValue(RGB(0x00, 0x00, 0x00));
			m_crItemTextDisabled.SetStandardValue(RGB(0x9f, 0xae, 0x7a));

			// hot, pressed colors
			m_crgItemBackHot.SetStandardValue(RGB(0xff, 0xf0, 0xc5), RGB(0xff, 0xd6, 0x9b));
			m_crgItemBackPressed.SetStandardValue(RGB(0xfe, 0x90, 0x4d), RGB(0xfe, 0xc9, 0x85));
			break;

		case xtpSystemThemeSilver:
			// normal colors
			m_crgBack.SetStandardValue(RGB(0xee, 0xee, 0xf4), RGB(0xa2, 0xa2, 0xb5));
			m_crgItemBorder.SetStandardValue(RGB(0x4b, 0x4b, 0x6f), RGB(0x4b, 0x4b, 0x6f));
			m_crItemText.SetStandardValue(RGB(0x00, 0x00, 0x00));
			m_crItemTextDisabled.SetStandardValue(RGB(0xa8, 0xa7, 0xbe));

			// hot, pressed colors
			m_crgItemBackHot.SetStandardValue(RGB(0xff, 0xf0, 0xc5), RGB(0xff, 0xd6, 0x9b));
			m_crgItemBackPressed.SetStandardValue(RGB(0xfe, 0x90, 0x4d), RGB(0xfe, 0xc9, 0x85));
			break;

		default:
			// normal colors
			m_crgBack.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE),
				GetXtremeColor(COLOR_WINDOW), 0xcd), GetXtremeColor(COLOR_3DFACE));
			break;
		}
	}
}

void CXTPShortcutListBoxThemeOffice2003::DrawImage(CDC* pDC, CRect& rcIcon, CXTPImageManagerIcon* pImage, BOOL bEnabled, BOOL /*bSelected*/, BOOL /*bPressed*/)
{
	CPoint pt(rcIcon.TopLeft());

	if (bEnabled)
	{
		pImage->Draw(pDC, pt);
	}
	else
	{
		if (pImage->HasDisabledIcon() || pImage->IsAlpha())
		{
			pImage->Draw(pDC, pt, pImage->GetDisabledIcon());
		}
		else
		{
			pImage->DrawMono(pDC, pt, pImage->GetIcon(), 0, m_crItemTextDisabled);
		}
	}
}
