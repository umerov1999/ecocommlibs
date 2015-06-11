// XTPCaptionButtonThemeOffice2003.cpp: implementation of the CXTPCaptionButtonThemeOffice2003 class.
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
#include <Controls/Button/Themes/XTPButtonThemeOffice2003.h>

#include <Controls/Static/XTPCaption.h>
#include <Controls/Static/XTPCaptionTheme.h>
#include <Controls/Static/Themes/XTPCaptionButtonThemeOffice2003.h>

/////////////////////////////////////////////////////////////////////////////
// CXTPCaptionButtonThemeOffice2003

void CXTPCaptionButtonThemeOffice2003::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	if (!::IsWindow(pButton->GetSafeHwnd()))
		return;

	CRect rc = pButton->GetButtonRect();

	CXTPCaption* pCaption = ((CXTPCaptionButton*)pButton)->GetCaption();
	if (::IsWindow(pCaption->GetSafeHwnd()))
	{
		BOOL bHilite = pButton->IsHighlighted();
		BOOL bSelected = (pButton->IsPushed() || pButton->GetChecked());

		// Paint the background.
		if (bSelected || bHilite)
		{
			XTPDrawHelpers()->GradientFill(pDC, rc, bSelected ?
				XTPColorManager()->grcLunaPushed: XTPColorManager()->grcLunaSelected, FALSE);

			pDC->Draw3dRect(&rc, GetMSO2003Color(XPCOLOR_HIGHLIGHT_BORDER),
				GetMSO2003Color(XPCOLOR_HIGHLIGHT_BORDER));
		}
		else
		{
			CXTPWindowRect rButton(pButton);

			CXTPClientRect rcCaption(pCaption);
			pCaption->ClientToScreen(rcCaption);
			pButton->ScreenToClient(rcCaption);

			XTPDrawHelpers()->GradientFill(pDC, rcCaption, pCaption->IsAppCaption() ?
				XTPColorManager()->grcCaption: XTPColorManager()->grcToolBar, FALSE, rc);
		}

		return;
	}

	CXTPButtonThemeOffice2003::DrawButtonBackground(pDC, pButton);
}

COLORREF CXTPCaptionButtonThemeOffice2003::GetTextColor(CXTPButton* pButton)
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
