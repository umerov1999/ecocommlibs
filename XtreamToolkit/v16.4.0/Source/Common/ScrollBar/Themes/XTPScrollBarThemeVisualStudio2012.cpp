#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeVisualStudio2012.h>

void CXTPScrollBarThemeVisualStudio2012::RefreshMetrics()
{
	CXTPScrollBarPaintManager::RefreshMetrics();
}

void CXTPScrollBarThemeVisualStudio2012::DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar)
{
	XTPScrollBarStyle style =  pScrollBar->GetScrollBarStyle();

	#define GETPARTSTATE(ht, pressed, hot, normal, disabled) \
		(!bEnabled ? disabled : nPressedHt == ht ? pressed : nHotHt == ht ? hot : normal)

	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pScrollBar->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pScrollBar->GetScrollBarPosInfo();

	BOOL nPressedHt = pSBTrack ? (pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1) : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;


	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);

	if (cWidth <= 0)
	{
		return;
	}

	BOOL bEnabled = (pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0) && pScrollBar->IsScrollBarEnabled();

	int nBtnTrackSize =   pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	const int nMargin = 5;

	COLORREF clrBackground;

	COLORREF clrArrowDisabled;
	COLORREF clrArrowEnabled;
	COLORREF clrArrowHot;
	COLORREF clrArrowPressed;

	COLORREF clrBtnTrackEnabled;
	COLORREF clrBtnTrackHot;
	COLORREF clrBtnTrackPressed;

	switch(style)
	{
		case xtpScrollStyleVisualStudio2012Dark:
		{
			clrBackground = RGB(62, 62, 66);

			clrArrowDisabled = RGB(85, 85, 88);
			clrArrowEnabled = RGB(153, 153, 153);
			clrArrowHot = RGB(28, 151, 234);
			clrArrowPressed = RGB(0, 122, 204);

			clrBtnTrackEnabled = RGB(104, 104, 104);
			clrBtnTrackHot = RGB(158, 158, 158);
			clrBtnTrackPressed = RGB(239, 235, 239);
		}
		break;

		default:
		case xtpScrollStyleVisualStudio2012Light:
		{
			clrBackground = RGB(232, 232, 236);

			clrArrowDisabled = RGB(202, 203, 211);
			clrArrowEnabled = RGB(134, 137, 153);
			clrArrowHot = RGB(28, 151, 234);
			clrArrowPressed = RGB(0, 122, 204);

			clrBtnTrackEnabled = RGB(208, 209, 215);
			clrBtnTrackHot = RGB(136, 136, 136);
			clrBtnTrackPressed = RGB(106, 106, 106);
		}
		break;
	}


	if(!pSBInfo->fSizebox)
	{
		if (pSBInfo->fVert)
		{
			XTPScrollMetricsV scroll;
			CalcVScroll(pScrollBar, &scroll);

			//drawing top and bottom buttons
			pDC->FillSolidRect(scroll.rcArrowUp, clrBackground);
			pDC->FillSolidRect(scroll.rcArrowDown, clrBackground);

			if (bEnabled)
			{
				COLORREF clrText;

				//drawing up button
				clrText = clrArrowEnabled;
				if (nHotHt == XTP_HTSCROLLUP)
					clrText = clrArrowHot;
				if (bEnabled && nPressedHt == XTP_HTSCROLLUP)
					clrText = clrArrowPressed;
				DrawArrowGlyph(pDC, scroll.rcArrowUp, xtpArrowGlyphUp, bEnabled, clrText);

				//drawing down button
				clrText = clrArrowEnabled;
				if (nHotHt == XTP_HTSCROLLDOWN)
					clrText = clrArrowHot;
				if (bEnabled && nPressedHt == XTP_HTSCROLLDOWN)
					clrText = clrArrowPressed;
				DrawArrowGlyph(pDC, scroll.rcArrowDown, xtpArrowGlyphDown, bEnabled, clrText);
			}
			else
			{
				DrawArrowGlyph(pDC, scroll.rcArrowUp, xtpArrowGlyphUp, bEnabled, clrArrowDisabled);
				DrawArrowGlyph(pDC, scroll.rcArrowDown, xtpArrowGlyphDown, bEnabled, clrArrowDisabled);
			}

			//drawing central part of scrollbar with slider
			pDC->FillSolidRect(scroll.rcTrack, clrBackground);
			scroll.rcBtnTrack.DeflateRect(nMargin, 0, nMargin, 0);
			pDC->FillSolidRect(scroll.rcBtnTrack, nPressedHt == XTP_HTSCROLLTHUMB ? clrBtnTrackPressed : nHotHt == XTP_HTSCROLLTHUMB ? clrBtnTrackHot : clrBtnTrackEnabled);
		}
		else
		{
			//horizontal scrollbar

			CRect rcHScroll(pSBInfo->rc);

			CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
			CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);
			CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);

			CRect rcLowerTrack(rcTrack.left, rcTrack.top, rcTrack.left + nBtnTrackPos, rcTrack.bottom);
			CRect rcBtnTrack(rcLowerTrack.right, rcTrack.top, rcLowerTrack.right + nBtnTrackSize, rcTrack.bottom);
			CRect rcUpperTrack(rcBtnTrack.right, rcTrack.top, rcTrack.right, rcTrack.bottom);

			//drawing top and bottom buttons
			pDC->FillSolidRect(rcArrowLeft, clrBackground);
			pDC->FillSolidRect(rcArrowRight, clrBackground);

			if (bEnabled)
			{
				COLORREF clrText;

				//drawing up button
				clrText = clrArrowEnabled;
				if (nHotHt == XTP_HTSCROLLUP)
					clrText = clrArrowHot;
				if (bEnabled && nPressedHt == XTP_HTSCROLLUP)
					clrText = clrArrowPressed;
				DrawArrowGlyph(pDC, rcArrowLeft, xtpArrowGlyphLeft, bEnabled, clrText);

				//drawing down button
				clrText = clrArrowEnabled;
				if (nHotHt == XTP_HTSCROLLDOWN)
					clrText = clrArrowHot;
				if (bEnabled && nPressedHt == XTP_HTSCROLLDOWN)
					clrText = clrArrowPressed;
				DrawArrowGlyph(pDC, rcArrowRight, xtpArrowGlyphRight, bEnabled, clrText);
			}
			else
			{
				DrawArrowGlyph(pDC, rcArrowLeft, xtpArrowGlyphLeft, bEnabled, clrArrowDisabled);
				DrawArrowGlyph(pDC, rcArrowRight, xtpArrowGlyphRight, bEnabled, clrArrowDisabled);
			}

			//drawing central part of scrollbar with slider
			pDC->FillSolidRect(rcTrack, clrBackground);
			rcBtnTrack.DeflateRect(0, nMargin, 0, nMargin);
			pDC->FillSolidRect(rcBtnTrack, nPressedHt == XTP_HTSCROLLTHUMB ? clrBtnTrackPressed : nHotHt == XTP_HTSCROLLTHUMB ? clrBtnTrackHot : clrBtnTrackEnabled);
		}
	}
	else
	{
		pDC->FillSolidRect(&pSBInfo->rc, clrBackground);
	}
}
