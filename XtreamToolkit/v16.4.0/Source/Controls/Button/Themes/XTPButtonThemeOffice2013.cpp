// XTPButtonThemeOffice2013.cpp: implementation of the CXTPButtonThemeOffice2013 class.
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

#ifndef _XTP_ACTIVEX_BUTTON
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#endif


CXTPButtonThemeOffice2013::CXTPButtonThemeOffice2013()
{
}

void CXTPButtonThemeOffice2013::RefreshMetrics(CXTPButton *pButton)
{
	CXTPButtonThemeUltraFlat::RefreshMetrics(pButton);

	m_crBack =         RGB(253, 253, 253);
	m_crBackHilite   = RGB(213, 225, 242);
	m_crBackPushed   = RGB(163, 189, 227);

	m_crBorderHilite = RGB(163, 189, 227);
	m_crBorderPushed = RGB(62,  109, 181);

	m_crBorderShadow = m_crBorder3DShadow = RGB(173,173,173);
	m_crBorder3DHilite = m_crBorder3DShadow;

	if (pButton && pButton->GetSafeHwnd())
	{
		DWORD dwButtonStyle = pButton->GetButtonStyle();

		if (dwButtonStyle == BS_CHECKBOX || dwButtonStyle == BS_AUTOCHECKBOX || dwButtonStyle == BS_AUTO3STATE // checkbox
			|| dwButtonStyle == BS_RADIOBUTTON || dwButtonStyle == BS_AUTORADIOBUTTON)  // radio button
		{
			m_crBorderShadow = m_crBorder3DShadow;
			m_crBorderHilite = RGB(163,189,227);

			m_crBack = RGB(253,253,253);
			m_crBackHilite = RGB(213,225,242);
		}
	}
}

void CXTPButtonThemeOffice2013::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
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
		COLORREF clrButtonBackground = bPressed ? m_crBackPushed :  m_crBackHilite;

		pDC->FillSolidRect(rc, clrButtonBackground);

		if (bPressed)
		{
			pDC->Draw3dRect(rc, m_crBorderPushed, m_crBorderPushed);
		}
		else
		{
			pDC->Draw3dRect(rc, m_crBorderHilite, m_crBorderHilite);
		}

		if (bChecked)
		{
			CXTPPenDC pen(pDC->GetSafeHdc(), m_crBorderPushed);
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
			pDC->Rectangle(rc);
		}
	}
	else
	{
		pDC->FillSolidRect(rc, m_crBack);
		pDC->Draw3dRect(rc, m_crBorderShadow, m_crBorderShadow);
	}
}

void CXTPButtonThemeOffice2013::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	// apply custom coloring to checkmark
	m_crCheckMark = pButton->IsHighlighted() ? RGB(58,62,66) : RGB(129, 129, 129);

	if (pButton->GetChecked() == 2)
		m_crCheckMark = GetXtremeColor(XPCOLOR_GRAYTEXT);

	CXTPButtonThemeUltraFlat::DrawCheckBoxMark(pDC, pButton);
}

void CXTPButtonThemeOffice2013::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	// apply custom coloring to checkmark
	m_crCheckMark = pButton->IsHighlighted() ? RGB(58,62,66) : RGB(129, 129, 129);

	CXTPButtonThemeUltraFlat::DrawRadioButtonMark(pDC, pButton);
}
