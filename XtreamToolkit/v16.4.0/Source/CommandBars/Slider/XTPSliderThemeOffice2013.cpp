// XTPSliderThemeOffice2013.cpp : implementation of the CXTPSliderThemeOffice2013 class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include <Common/XTPColorManager.h>

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/Slider/XTPSliderThemeOffice2013.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPSliderThemeOffice2013::CXTPSliderThemeOffice2013(CXTPPaintManager *pPaintManager)
	: CXTPSliderPaintManager(pPaintManager)
{
}

void CXTPSliderThemeOffice2013::RefreshMetrics()
{
	CXTPSliderPaintManager::RefreshMetrics();
}

void CXTPSliderThemeOffice2013::DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar)
{
	const int nGlyphThickness = 2;

	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pScrollBar->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pScrollBar->GetScrollBarPosInfo();

	ASSERT(!pSBInfo->fSizebox && !pSBInfo->fVert);

	int nHtPressed = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1) : -1;
	int nHtHot     = pSBTrack ? -1 : pSBInfo->ht;

	COLORREF clrNormal = pScrollBar->IsSlider() ? RGB(171,171,171) : RGB(255,255,255);
	COLORREF clrHot    = RGB(152,178,217);

	if (!pSBInfo->fVert)
	{
		XTPScrollMetricsH metricsH;
		CalcHScroll(pScrollBar, &metricsH);

		BOOL bHtLeft  = (nHtPressed == XTP_HTSCROLLLEFT  || nHtHot == XTP_HTSCROLLLEFT);
		BOOL bHtRight = (nHtPressed == XTP_HTSCROLLRIGHT || nHtHot == XTP_HTSCROLLRIGHT);

		BOOL bHtTrack =
			(nHtPressed == XTP_HTSCROLLUPPAGE || nHtHot == XTP_HTSCROLLUPPAGE) ||
			(nHtPressed == XTP_HTSCROLLTHUMB  || nHtHot == XTP_HTSCROLLTHUMB)  ||
			(nHtPressed == XTP_HTSCROLLDOWNPAGE || nHtHot == XTP_HTSCROLLDOWNPAGE);


		// -
		CRect rcMinus(metricsH.rcArrowLeft);
		rcMinus.left   += 5;
		rcMinus.top    += ((rcMinus.Height() / 2) - (nGlyphThickness / 2));
		rcMinus.right  -= 5;
		rcMinus.bottom  = rcMinus.top + nGlyphThickness;
		pDC->FillSolidRect(rcMinus, bHtLeft ? clrHot : clrNormal);

		// +
		CRect rcPlusH(metricsH.rcArrowRight);
		rcPlusH.left   += 4;
		rcPlusH.top    += ((rcPlusH.Height() / 2) - (nGlyphThickness / 2));
		rcPlusH.right  -= 4;
		rcPlusH.bottom  = rcPlusH.top + nGlyphThickness;

		CRect rcPlusV(metricsH.rcArrowRight);
		rcPlusV.left   += ((rcPlusV.Width() / 2) - (nGlyphThickness /2));
		rcPlusV.top    += 4;
		rcPlusV.right   = rcPlusV.left + nGlyphThickness;
		rcPlusV.bottom -= 4;

		pDC->FillSolidRect(rcPlusH, bHtRight ? clrHot : clrNormal);
		pDC->FillSolidRect(rcPlusV, bHtRight ? clrHot : clrNormal);

		// Track
		CRect rcTrack(metricsH.rcTrack);
		rcTrack.top   += (rcTrack.Height() / 2) - 1;
		rcTrack.bottom = rcTrack.top + 1;
		pDC->FillSolidRect(rcTrack, clrNormal);

		CRect rcBtnTrack(metricsH.rcBtnTrack);
		rcBtnTrack.DeflateRect(4,2,4,4);
		pDC->FillSolidRect(rcBtnTrack, bHtTrack ? clrHot : clrNormal);
	}
}
