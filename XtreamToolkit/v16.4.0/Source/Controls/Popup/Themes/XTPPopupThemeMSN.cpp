// XTPPopupThemeMSN.cpp: implementation of the CXTPPopupThemeMSN class.
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
#include "Common/XTPColorManager.h"

#include "../../Defines.h"

#include "../XTPPopupControl.h"
#include "../XTPPopupPaintManager.h"
#include "XTPPopupThemeOffice2000.h"
#include "XTPPopupThemeMSN.h"

//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeMSN

CXTPPopupThemeMSN::CXTPPopupThemeMSN()
{
}

CXTPPopupThemeMSN::~CXTPPopupThemeMSN()
{
}

void CXTPPopupThemeMSN::RefreshMetrics()
{
	CXTPPopupThemeOffice2000::RefreshMetrics();


	m_clrButtonSelected = RGB(194, 238, 255);
	m_clrButtonSelectedBorder.SetStandardValue(RGB(114, 142, 184));

	m_clrButtonPressed = RGB(134, 162, 224);
	m_clrButtonPressedBorder.SetStandardValue(RGB(114, 142, 184));

}

void CXTPPopupThemeMSN::DrawBackground(CDC* pDC, CXTPPopupControl* pControl, CRect rcClient)
{
	if (pControl->GetBackgroundBitmap() > 0)
	{
		CXTPPopupPaintManager::DrawBackground(pDC, pControl, rcClient);
		return;
	}

	pDC->Draw3dRect(rcClient, RGB(166, 180, 207), RGB(69, 86, 144));
	rcClient.DeflateRect(1, 1);
	pDC->Draw3dRect(rcClient, RGB(255, 255, 255), RGB(207, 222, 244));
	rcClient.DeflateRect(1, 1);

	CRect rcBackground(rcClient.left, rcClient.top, rcClient.right, rcClient.top + 12);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(207, 215, 236), RGB(255, 255, 255) , FALSE);

	rcBackground = CRect(rcClient.left, rcBackground.bottom, rcClient.right, rcClient.top + 23);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(255, 255, 255), RGB(207, 221, 244), FALSE);

	rcBackground = CRect(rcClient.left, rcBackground.bottom, rcClient.right, rcClient.top + 40);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(207, 221, 244), RGB(255, 255, 255), FALSE);

	rcBackground = CRect(rcClient.left, rcBackground.bottom, rcClient.right, rcClient.bottom);
	XTPDrawHelpers()->GradientFill(pDC, rcBackground, RGB(255, 255, 255), RGB(207, 221, 244), FALSE);

	CRect rcFrame(rcClient.left, rcClient.top + 22, rcClient.right, rcClient.bottom);
	pDC->Draw3dRect(rcFrame, RGB(114, 142, 184), RGB(185, 201, 239));
}
