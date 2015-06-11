// XTPCaptionButtonThemeOfficeXP.cpp: implementation of the CXTPCaptionButtonThemeOfficeXP class.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButtonTheme.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Button/Themes/XTPButtonThemeUltraFlat.h>
#include <Controls/Button/Themes/XTPButtonThemeOfficeXP.h>

#include <Controls/Static/XTPCaption.h>
#include <Controls/Static/XTPCaptionTheme.h>
#include <Controls/Static/Themes/XTPCaptionButtonThemeOfficeXP.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCaptionButtonThemeOfficeXP

CXTPCaptionButtonThemeOfficeXP::CXTPCaptionButtonThemeOfficeXP()
{
//  m_bAnimateIcon = FALSE;
//  m_bFadedIcon = FALSE;
	m_bOffsetHiliteText = FALSE;
}

COLORREF CXTPCaptionButtonThemeOfficeXP::GetTextColor(CXTPButton* pButton)
{
	if (!pButton->IsWindowEnabled())
		return GetXtremeColor(COLOR_GRAYTEXT);

	if (pButton->IsHighlighted() || (pButton->IsPushed() || pButton->GetChecked()))
		return m_crTextHilite;

	CXTPCaption* pCaption = ((CXTPCaptionButton*)pButton)->GetCaption();
	if (::IsWindow(pCaption->GetSafeHwnd()))
	{
		return pCaption->m_clrText;
	}

	return m_crText;
}

void CXTPCaptionButtonThemeOfficeXP::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	// define some temporary variables.
	CRect rcItem = pButton->GetButtonRect();

	// Paint the background.
	if (pButton->IsHighlighted() || (pButton->IsPushed() || pButton->GetChecked()))
	{
		pDC->FillSolidRect(rcItem, (pButton->IsPushed() || pButton->GetChecked()) ?
			GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED) : GetXtremeColor(XPCOLOR_HIGHLIGHT));

		pDC->Draw3dRect(rcItem, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER),
			GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	}
	else
	{
		CXTPCaption* pCaption = ((CXTPCaptionButton*)pButton)->GetCaption();
		if (::IsWindow(pCaption->GetSafeHwnd()))
		{
			m_crBack = pCaption->m_clrFace;
		}

		pDC->FillSolidRect(rcItem, m_crBack);

	}
}

void CXTPCaptionButtonThemeOfficeXP::DrawButtonText(CDC* pDC, CXTPButton* pButton)
{
	CXTPCaption* pCaption = ((CXTPCaptionButton*)pButton)->GetCaption();
	if (::IsWindow(pCaption->GetSafeHwnd()))
	{
		if (pCaption->IsAppCaption() && !pCaption->GetCaptionText().IsEmpty())
		{
			// select font into device context.
			CXTPFontDC fontDC(pDC, pButton->GetFont());
			pDC->SetTextColor(GetTextColor(pButton));

			CXTPClientRect rcItem(pButton);
			if (pButton->IsHighlighted() && m_bOffsetHiliteText)
				rcItem.OffsetRect(1, 1);

			// draw the text, and select the original font.
			pDC->DrawText(pCaption->GetCaptionText(), rcItem,
				DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
		}
	}
}
