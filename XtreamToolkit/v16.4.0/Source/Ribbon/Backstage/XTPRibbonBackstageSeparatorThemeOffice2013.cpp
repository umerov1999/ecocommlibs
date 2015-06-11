// XTPRibbonBackstageSeparatorThemeOffice2013.cpp : implementation file
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
#include <Common/XTPMarkupRender.h>

#include <Controls/Util/XTPControlTheme.h>

#include <Ribbon/Backstage/XTPRibbonBackstageSeparator.h>
#include <Ribbon/Backstage/XTPRibbonBackstageSeparatorPaintManager.h>
#include <Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeOffice2013.h>

#ifndef _XTP_ACTIVEX
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageSeparatorThemeOffice2013

CXTPRibbonBackstageSeparatorThemeOffice2013::CXTPRibbonBackstageSeparatorThemeOffice2013()
{
}

void CXTPRibbonBackstageSeparatorThemeOffice2013::DrawSeparator(CDC *pDC, CXTPRibbonBackstageSeparator *pSeparator)
{
	CXTPClientRect rc(pSeparator);

	if (pSeparator->IsVerticalStyle())
	{
		pDC->FillSolidRect(CRect(rc.right - 1, rc.top, rc.right, rc.bottom), RGB(225, 225, 225));
	}
	else
	{
		pDC->FillSolidRect(CRect(rc.left, rc.Height() / 2, rc.right, rc.Height() / 2 + 1), RGB(225, 225, 225));
	}
}
