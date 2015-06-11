// XTPResourceTheme.cpp : implementation of the CXTPResourceTheme class.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPVC80Helpers.h>
// ScrollBar
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPPaintManager.h>

#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/Themes/XTPCommandBarsResourceTheme.h>

#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/Slider/XTPSliderThemeResource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPSliderResourceTheme

void CXTPSliderResourceTheme::RefreshMetrics()
{
	CXTPSliderPaintManager::RefreshMetrics();

	CXTPResourceImage* pImage = ((CXTPResourceTheme*)m_pPaintManager)->LoadImage(_T("SLIDERUP"));

	if (pImage)
	{
		m_cyHScroll = pImage->GetSource(0, 3).Height();
		m_cxHScroll = pImage->GetSource(0, 3).Width();
	}

	m_cThumb = 11;
}

void CXTPSliderResourceTheme::DrawScrollBar(CDC* pDC, CXTPScrollBase* pGallery)
{
	if(!XTPResourceImages()->IsValid())
			return;

	#define GETPARTSTATE4(ht) (nPressedHt == ht ? 2 : nHotHt == ht ? 1 : 0)

	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pGallery->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pGallery->GetScrollBarPosInfo();

	ASSERT(!pSBInfo->fSizebox && !pSBInfo->fVert);

	BOOL nPressedHt = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1) : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;


	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
	{
		return;
	}

	BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0) && pGallery->IsScrollBarEnabled();

	int nBtnTrackSize =   pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	CXTPResourceTheme* pPaintManager = (CXTPResourceTheme*)m_pPaintManager;

	if (!pSBInfo->fVert)
	{
		CRect rcHScroll(pSBInfo->rc);

		CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
		CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);
		CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);
		CRect rcBtnTrack(rcTrack.left + nBtnTrackPos, rcTrack.top, rcTrack.left + nBtnTrackPos + nBtnTrackSize, rcTrack.bottom);

		CXTPResourceImage* pImage =  pPaintManager->LoadImage(_T("SLIDERTRACK"));
		ASSERT(pImage);
		if (!pImage)
			return;

		CRect rcTrackDest(CPoint(rcTrack.left, (rcTrack.top + rcTrack.bottom - pImage->GetHeight()) / 2), CSize(rcTrack.Width(), pImage->GetHeight()));
		pImage->DrawImage(pDC, rcTrackDest, pImage->GetSource());

		pImage =  pPaintManager->LoadImage(_T("SLIDERTICK"));

		XTP_SLIDERTICKS* pTicks = pGallery->GetTicks();
		if (!pTicks || (pSBInfo->posMax <= pSBInfo->posMin))
		{
			CRect rcTrackTickDest(CPoint((rcTrackDest.left + rcTrackDest.right - pImage->GetWidth()) / 2,
				(rcTrackDest.top + rcTrackDest.bottom - pImage->GetHeight()) / 2), pImage->GetExtent());

			pImage->DrawImage(pDC, rcTrackTickDest, pImage->GetSource(), CRect(0, 0, 0, 0), 0xFF00FF);
		}
		else
		{
			rcTrackDest.DeflateRect(6, 0);

			for (int i = 0; i < pTicks->nCount; i++)
			{
				double dTick = pTicks->pTicks[i];
				double dPos = (dTick - pSBInfo->posMin) * rcTrackDest.Width() / (pSBInfo->posMax - pSBInfo->posMin);

				if (dPos >= 0 && dPos <= rcTrackDest.Width())
				{
					CRect rcTrackTickDest(CPoint(rcTrackDest.left + (int)dPos - pImage->GetWidth() / 2,
						(rcTrackDest.top + rcTrackDest.bottom - pImage->GetHeight()) / 2), pImage->GetExtent());

					pImage->DrawImage(pDC, rcTrackTickDest, pImage->GetSource(), CRect(0, 0, 0, 0), 0xFF00FF);
				}
			}
		}

		pImage = pPaintManager->LoadImage(_T("SLIDERUP"));
		pImage->DrawImage(pDC, rcArrowLeft, pImage->GetSource(GETPARTSTATE4(XTP_HTSCROLLUP), 3));

		pImage = pPaintManager->LoadImage(_T("SLIDERDOWN"));
		pImage->DrawImage(pDC, rcArrowRight, pImage->GetSource(GETPARTSTATE4(XTP_HTSCROLLDOWN), 3));

		if (bEnabled)
		{
			pImage = pPaintManager->LoadImage(_T("SLIDERTHUMB"));

			CRect rcImgSrc = pImage->GetSource(
				nPressedHt == XTP_HTSCROLLTHUMB  ? 2 : nPressedHt == XTP_HTSCROLLUPPAGE || nPressedHt == XTP_HTSCROLLDOWNPAGE ||
					nHotHt == XTP_HTSCROLLUPPAGE || nHotHt == XTP_HTSCROLLDOWNPAGE || nHotHt == XTP_HTSCROLLTHUMB ? 1 : 0, 3);

			CRect rcBtnTrackDest(CPoint((rcBtnTrack.left + rcBtnTrack.right - rcImgSrc.Width()) / 2,
				(rcBtnTrack.top + rcBtnTrack.bottom - rcImgSrc.Height()) / 2), rcImgSrc.Size());
			pImage->DrawImage(pDC, rcBtnTrackDest, rcImgSrc);
		}
	}
}
