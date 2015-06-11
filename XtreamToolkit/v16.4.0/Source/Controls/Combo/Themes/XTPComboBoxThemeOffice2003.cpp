// XTPComboBoxTheme2003.cpp: implementation of the CXTPComboBoxThemeOffice2003 class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE E2003RESSED WRITTEN
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

#include "StdAfx.h"
#include "math.h"

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Combo/XTPComboBox.h>
#include <Controls/Combo/XTPComboBoxTheme.h>

#include "XTPComboBoxThemeOffice2003.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeOffice2003::CXTPComboBoxThemeOffice2003()
	: m_bHighlight (FALSE)
{

}

CXTPComboBoxThemeOffice2003::~CXTPComboBoxThemeOffice2003()
{

}

void CXTPComboBoxThemeOffice2003::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	m_clrBorderHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));

	m_clrButtonHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));

	m_clrButtonArrowHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_clrButtonArrowPressed.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));

	XTPColorManager()->RefreshColors();

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();
	switch (systemTheme)
	{
	case xtpSystemThemeBlue:
	case xtpSystemThemeRoyale:
	case xtpSystemThemeAero:
		m_clrBorderNormal.SetStandardValue(RGB(147, 181, 231));
		m_clrBorderHot.SetStandardValue(RGB(0, 0, 128));
		m_grcButton.SetStandardValue(RGB(227, 239, 255), RGB(147, 181, 231));
		break;

	case xtpSystemThemeOlive:
		m_clrBorderNormal.SetStandardValue(RGB(194, 206, 159));
		m_clrBorderHot.SetStandardValue(RGB(63, 93, 56));
		m_grcButton.SetStandardValue(RGB(236, 240, 213), RGB(194, 206, 159));
		break;

	case xtpSystemThemeSilver:
		m_clrBorderNormal.SetStandardValue(RGB(186, 185, 205));
		m_clrBorderHot.SetStandardValue(RGB(75, 75, 11));
		m_grcButton.SetStandardValue(RGB(233, 231, 241), RGB(186, 185, 205));
		break;

	default:
		m_clrBorderNormal.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
		m_clrBorderHot.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
		m_grcButton.SetStandardValue(
			XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0x14a),
			XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0x320));
		break;
	}

	m_grcButtonHilite.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
	m_grcButtonPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));

	if (systemTheme != xtpSystemThemeUnknown)
	{
		m_clrButtonHot.SetStandardValue(RGB(255, 238, 194));
		m_clrButtonPressed.SetStandardValue(RGB(254, 128, 62));
		m_clrButtonArrowPressed.SetStandardValue(0);
		m_clrButtonArrowHot.SetStandardValue(0);
	}
}

void CXTPComboBoxThemeOffice2003::DrawBackground(CDC *pDC, CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::DrawBackground(pDC, pComboBox);

	CRect rc = pComboBox->GetComboBoxRect();

	BOOL bPressed = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
	BOOL bEnabled = pComboBox->IsWindowEnabled();

	COLORREF clrBorders = bSelected? m_clrBorderHot : m_clrBorderNormal;

	if (!bSelected && !bPressed && bEnabled && pComboBox->GetFlatStyle())
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		pDC->Draw3dRect(rc, bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW), bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW));
	}
}

void CXTPComboBoxThemeOffice2003::DrawButton(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	CRect rc = pComboBox->GetComboBoxRect();
	HBRUSH hBrush = pComboBox->GetClientBrush(pDC);

	BOOL bPressed = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
	BOOL bEnabled = pComboBox->IsWindowEnabled();

	COLORREF clrBorders = bSelected? m_clrBorderHot : m_clrBorderNormal;

	if (!bSelected && !bPressed && bEnabled && pComboBox->GetFlatStyle())
	{
		XTPDrawHelpers()->GradientFill(pDC, &rcBtn, m_grcButton, FALSE);

		DrawFrame(pDC->GetSafeHdc(), rcBtn, 1, hBrush);
	}
	else
	{
		if (bEnabled)
		{
			CXTPPaintManagerColorGradient& clrButton = bPressed ? m_grcButtonPushed : bSelected ? m_grcButtonHilite : m_grcButton;

			if (m_bHighlight && !bPressed && bSelected)
			{
				CRect rc[2];

				rc[0] = rcBtn;
				rc[0].bottom = rc[0].top + rcBtn.Height()/2;
				pDC->FillSolidRect(&rc[0], m_grcButtonHilite.clrLight);

				rc[1] = rcBtn;
				rc[1].top += rcBtn.Height()/2;
				pDC->FillSolidRect(&rc[1], m_grcButtonHilite.clrDark);
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, &rcBtn, clrButton, FALSE);
			}
		}
		else
		{
			COLORREF clrButton = bPressed ? m_clrButtonPressed : bSelected ? m_clrButtonHot : m_clrButtonNormal;
			pDC->FillSolidRect(rcBtn, clrButton);
		}

		CXTPPenDC pen(pDC->GetSafeHdc(), bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW));

		pDC->MoveTo(rcBtn.TopLeft());
		pDC->LineTo(rcBtn.left, rcBtn.bottom);
	}
}
