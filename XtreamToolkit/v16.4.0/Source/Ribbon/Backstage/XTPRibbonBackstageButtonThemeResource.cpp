// XTPRibbonBackstageButtonThemeResource.cpp : implementation file
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

#include "StdAfx.h"

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Button/XTPButtonTheme.h>
#include <Controls/Button/Themes/XTPButtonThemeUltraFlat.h>
#include <Controls/Button/Themes/XTPButtonThemeResource.h>

#include <Controls/Resize/XTPResizeRect.h>
#include <Controls/Resize/XTPResizePoint.h>
#include <Controls/Resize/XTPResize.h>
#include <Controls/Resize/XTPResizePropertyPage.h>

#include <Ribbon/Backstage/XTPRibbonBackstageButton.h>
#include <Ribbon/Backstage/XTPRibbonBackstageLabel.h>
#include <Ribbon/Backstage/XTPRibbonBackstageSeparator.h>
#include <Ribbon/Backstage/XTPRibbonBackstageButtonThemeResource.h>


//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageButtonThemeResource

CXTPRibbonBackstageButtonThemeResource::CXTPRibbonBackstageButtonThemeResource()
{
	m_bOffsetHiliteText = FALSE;
}


void CXTPRibbonBackstageButtonThemeResource::DrawButtonBackground(CDC *pDC, CXTPButton *pButton)
{
	CXTPRibbonBackstageButton *pBackstageButton = (CXTPRibbonBackstageButton*)pButton;

	BOOL bPressed  = pBackstageButton->IsPushed();
	BOOL bSelected = pBackstageButton->IsHighlighted();
	BYTE bStyle    = pBackstageButton->GetButtonStyle();
	BOOL bEnabled  = pBackstageButton->IsWindowEnabled();
	BOOL bChecked  = pBackstageButton->GetChecked();
	BOOL bDefault  = bStyle == BS_DEFPUSHBUTTON;
	CRect rc(pButton->GetButtonRect());


	if (pBackstageButton->m_bTabStyle)
	{
		CWnd* pWndParent = pButton->GetParent();

		CXTPWindowRect rcButtonScreen(pButton);

		for (CWnd* pChildWnd = pWndParent->GetWindow(GW_CHILD); pChildWnd != NULL; pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT))
		{
#ifdef _XTP_ACTIVEX_BUTTON
			if (pChildWnd->IsKindOf(RUNTIME_CLASS(CBackstageSeparatorCtrl)))
#else
			if (pChildWnd->IsKindOf(RUNTIME_CLASS(CXTPRibbonBackstageSeparator)))
#endif
			{
				CXTPWindowRect rcSeparatorScreen(pChildWnd);

				CRect rcIntersect;
				if (rcIntersect.IntersectRect(rcButtonScreen, rcSeparatorScreen))
				{
					pButton->ScreenToClient(rcSeparatorScreen);

					CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEPANESEPARATOR"));
					if (pImage)
					{
						pImage->DrawImage(pDC, CRect(rcSeparatorScreen.right - 1, rcSeparatorScreen.top, rcSeparatorScreen.right, rcSeparatorScreen.bottom), pImage->GetSource(0, 1), CRect(0, 30, 0, 30));
					}
				}
			}
		}
	}

	if (pButton->GetFlatStyle() && !bSelected && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	if (pBackstageButton->m_bShowShadow && bEnabled)
	{
		CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTONSHADOW"));

		if (pImage)
		{
			pImage->DrawImage(pDC, CRect(rc.left, rc.bottom - 2, rc.right, rc.bottom), pImage->GetSource(), CRect(3, 0, 3, 0), 0xFF00FF);
			rc.bottom -= 2;
		}
	}



	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTON"));
	if (!pImage)
		pImage = XTPResourceImages()->LoadFile(_T("PUSHBUTTON"));

	if (!pImage)
	{
		CXTPButtonTheme::DrawButtonBackground(pDC, pButton);
		return;
	}

	int nState = bDefault ? 4 : 0;

	if (!bEnabled)
	{
		nState = 3;
	}
	else if (bChecked && !bSelected && !bPressed) nState = 2;
	else if (bChecked && bSelected && !bPressed) nState = 1;
	else if ((bSelected && bPressed) || pButton->IsPushed()) nState = 2;
	else if (bSelected || bPressed) nState = 1;

	if (nState != -1)
	{
		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 5), CRect(4, 4, 4, 4), 0xFF00FF);
	}

	if (bChecked && pBackstageButton->m_bTabStyle)
	{
		CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEBUTTONGLYPH"));

		if (pImage)
		{
			CRect rcSrc(pImage->GetSource());
			int y = rc.CenterPoint().y;
			pImage->DrawImage(pDC, CRect(rc.right - rcSrc.Width(), y - rcSrc.Height() / 2, rc.right, y - rcSrc.Height() / 2 + rcSrc.Height()), rcSrc);
		}
	}
}
