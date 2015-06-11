// XTPButtonThemeVisualStudio2012.cpp: implementation of the CXTPButtonThemeVisualStudio2012 class.
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
#include <Controls/Button/Themes/XTPButtonThemeOffice2013.h>
#include <Controls/Button/Themes/XTPButtonThemeVisualStudio2012.h>

#ifndef _XTP_ACTIVEX_BUTTON
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPButtonThemeVisualStudio2012

CXTPButtonThemeVisualStudio2012::CXTPButtonThemeVisualStudio2012(BOOL bLight)
	:m_bLight(bLight)
{
	if (m_bLight)
		m_nTheme = xtpControlThemeVisualStudio2012Light;
	else
		m_nTheme = xtpControlThemeVisualStudio2012Dark;

	m_nBorderWidth = 2;
	m_bOffsetHiliteText = FALSE;
}

void CXTPButtonThemeVisualStudio2012::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonThemeUltraFlat::RefreshMetrics(pButton);

	if (m_bLight)
	{
		//from CXTButtonTheme
		m_crBack.SetStandardValue(RGB(246, 246, 246));// RGB value for nomral background
		m_crText.SetStandardValue(RGB(0, 0, 0));// RGB value for normal text

		m_crTextDisabled.SetStandardValue(RGB(113, 113, 133)); // RGB value for disabled text color.

		m_crBorderShadow = m_crBorder3DShadow = m_crBorderHilite = RGB(219,221,230);

		//from CXTPButtonUltraFlatTheme
		m_crBackHilite.SetStandardValue(RGB(254, 254, 254));  // RGB value for highlighted background color.
		m_crTextHilite.SetStandardValue(RGB(255, 255, 255));  // RGB value for highlighted text color

		m_crBackPushed.SetStandardValue(RGB(0, 122, 204));  // RGB value for pushed background color.
		m_crBorderPushed.SetStandardValue(RGB(145,196,248)); // RGB value for border pushed color.
		m_crTextPushed.SetStandardValue(RGB(255, 255, 255));  // RGB value for pushed text color.

		m_crCheckMark.SetStandardValue(RGB(37, 37, 37));
	}
	else
	{
		//from CXTButtonTheme
		m_crBack.SetStandardValue(RGB(37, 37, 38));// RGB value for nomral background
		m_crText.SetStandardValue(RGB(255, 255, 255));// RGB value for normal text

		m_crTextDisabled.SetStandardValue(RGB(153, 153, 153)); // RGB value for disabled text color.

		m_crBorderShadow = m_crBorder3DShadow = m_crBorderHilite = RGB(219,221,230);

		//from CXTPButtonUltraFlatTheme
		m_crBackHilite.SetStandardValue(RGB(62, 62, 64));  // RGB value for highlighted background color.
		m_crTextHilite.SetStandardValue(RGB(255, 255, 255));  // RGB value for highlighted text color

		m_crBackPushed.SetStandardValue(RGB(0, 122, 204));  // RGB value for pushed background color.
		m_crBorderPushed.SetStandardValue(RGB(51,153,255)); // RGB value for border pushed color.
		m_crTextPushed.SetStandardValue(RGB(255, 255, 255));  // RGB value for pushed text color.

		m_crCheckMark.SetStandardValue(RGB(241, 241, 241));
	}
}

COLORREF CXTPButtonThemeVisualStudio2012::GetTextColor(CXTPButton* pButton)
{
	if (pButton->IsPushed()) return m_crTextPushed;
	if (!pButton->IsWindowEnabled()) return m_crTextDisabled;

	return m_crText;
}

void CXTPButtonThemeVisualStudio2012::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	CXTPButtonThemeUltraFlat::DrawCheckBoxMark(pDC, pButton);
}

void CXTPButtonThemeVisualStudio2012::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	CXTPButtonThemeUltraFlat::DrawRadioButtonMark(pDC, pButton);
}
