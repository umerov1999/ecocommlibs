// XTPRibbonBackstageSeparatorThemeResource.cpp : implementation file
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
#include <Ribbon/Backstage/XTPRibbonBackstageSeparatorThemeResource.h>

#ifndef _XTP_ACTIVEX
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageSeparatorThemeResource

CXTPRibbonBackstageSeparatorThemeResource::CXTPRibbonBackstageSeparatorThemeResource()
{
	const unsigned short _cb[] = {0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc};

	CBitmap bmp;
	bmp.CreateBitmap(8, 8, 1, 1, _cb);
	m_brush.CreatePatternBrush(&bmp);
}

void CXTPRibbonBackstageSeparatorThemeResource::DrawSeparator(CDC *pDC, CXTPRibbonBackstageSeparator *pSeparator)
{
	CXTPClientRect rc(pSeparator);

	if (pSeparator->IsVerticalStyle())
	{
		CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BACKSTAGEPANESEPARATOR"));
		if (pImage)
		{
			pImage->DrawImage(pDC, CRect(rc.right - 1, rc.top, rc.right, rc.bottom), pImage->GetSource(0, 1), CRect(0, 30, 0, 30));
		}

	}
	else
	{
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&m_brush);

		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(199, 200, 201));

		PatBlt(pDC->GetSafeHdc(), rc.left, rc.Height() / 2, rc.Width(), 1, PATCOPY);

		pDC->SelectObject(pOldBrush);
	}
}
