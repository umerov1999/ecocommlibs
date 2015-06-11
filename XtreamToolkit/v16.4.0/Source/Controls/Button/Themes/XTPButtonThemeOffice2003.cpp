// XTPButtonThemeOffice2003.cpp: implementation of the CXTPButtonThemeOffice2003 class.
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

#include "StdAfx.h"
#include "math.h"

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPImageManager.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Button/XTPButtonTheme.h>
#include <Controls/Button/Themes/XTPButtonThemeUltraFlat.h>
#include <Controls/Button/Themes/XTPButtonThemeOfficeXP.h>
#include <Controls/Button/Themes/XTPButtonThemeOffice2003.h>

#ifndef _XTP_ACTIVEX_BUTTON
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPButtonThemeOffice2003

CXTPButtonThemeOffice2003::CXTPButtonThemeOffice2003()
{
	m_nTheme = xtpControlThemeOffice2003;
	m_bLunaTheme = FALSE;
}


void CXTPButtonThemeOffice2003::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonThemeOfficeXP::RefreshMetrics(pButton);

	XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

	if (systemTheme != xtpSystemThemeUnknown)
	{
		m_crBackHilite.SetStandardValue(RGB(255, 238, 194));
		m_crBackPushed.SetStandardValue(RGB(254, 128, 62));
		m_crBackChecked.SetStandardValue(RGB(255, 192, 111));
		m_crText.SetStandardValue(0);
		m_crTextHilite.SetStandardValue(0);
		m_crTextPushed.SetStandardValue(0);
	}

	switch (systemTheme)
	{
	case xtpSystemThemeBlue:
	case xtpSystemThemeRoyale:
	case xtpSystemThemeAero:
		m_crBack.SetStandardValue(RGB(169, 199, 240));
		m_crBorderShadow.SetStandardValue(RGB(127, 157, 185));
		m_crBorderHilite.SetStandardValue(RGB(0, 0, 128));
		break;

	case xtpSystemThemeOlive:
		m_crBack.SetStandardValue(RGB(197, 212, 159));
		m_crBorderShadow.SetStandardValue(RGB(164, 185, 127));
		m_crBorderHilite.SetStandardValue(RGB(63, 93, 56));
		break;

	case xtpSystemThemeSilver:
		m_crBack.SetStandardValue(RGB(192, 192, 211));
		m_crBorderShadow.SetStandardValue(RGB(165, 172, 178));
		m_crBorderHilite.SetStandardValue(RGB(75, 75, 11));
		break;
	}

	m_bLunaTheme = systemTheme != xtpSystemThemeUnknown;
}

void CXTPButtonThemeOffice2003::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	if (!m_bLunaTheme)
	{
		CXTPButtonThemeOfficeXP::DrawButtonBackground(pDC, pButton);
		return;
	}
	BOOL bPressed = pButton->IsPushed();
	BOOL bHot = pButton->IsHighlighted();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bHot && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (bHot || bPressed || bChecked)
	{
		if (bPressed)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rc,
				XTPColorManager()->grcLunaPushed, FALSE);
		}
		else if (bHot)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rc,
				XTPColorManager()->grcLunaSelected, FALSE);
		}
		else if (bChecked)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rc,
				XTPColorManager()->grcLunaChecked, FALSE);
		}

		pDC->Draw3dRect(rc, m_crBorderHilite,
			m_crBorderHilite);

	}
	else
	{
		pDC->FillSolidRect(rc, m_crBack);
		pDC->Draw3dRect(rc, m_crBorderShadow, m_crBorderShadow);
	}
}
