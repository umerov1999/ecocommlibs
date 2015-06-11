// XTPRibbonBackstageButtonThemeOffice2013.cpp : implementation file
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
#include <Controls/Button/Themes/XTPButtonThemeOffice2013.h>

#include <Controls/Resize/XTPResizeRect.h>
#include <Controls/Resize/XTPResizePoint.h>
#include <Controls/Resize/XTPResize.h>
#include <Controls/Resize/XTPResizePropertyPage.h>

#include <Ribbon/Backstage/XTPRibbonBackstageButton.h>
#include <Ribbon/Backstage/XTPRibbonBackstageSeparator.h>
#include <Ribbon/Backstage/XTPRibbonBackstageButtonThemeOffice2013.h>




//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageButtonThemeOffice2013

CXTPRibbonBackstageButtonThemeOffice2013::CXTPRibbonBackstageButtonThemeOffice2013()
{
	m_bOffsetHiliteText = FALSE;
}


void CXTPRibbonBackstageButtonThemeOffice2013::DrawButtonBackground(CDC *pDC, CXTPButton *pButton)
{
	CXTPRibbonBackstageButton *pBackstageButton = (CXTPRibbonBackstageButton*)pButton;

	if (pBackstageButton->m_bTabStyle)
	{
		CWnd *pWndParent = pButton->GetParent();

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

					CRect rc(rcSeparatorScreen.right-1, rcSeparatorScreen.top, rcSeparatorScreen.right, rcSeparatorScreen.bottom);
					pDC->FillSolidRect(rc, RGB(225, 225, 225));
				}
			}
		}
	}

	CXTPButtonThemeOffice2013::DrawButtonBackground(pDC, pButton);
}
