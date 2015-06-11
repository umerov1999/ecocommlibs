#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsClassic.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsUx.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeOffice2000.h>

void CXTPScrollBarThemeOffice2000::DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar)
{
	CXTPScrollBarThemeWindowsUx::DrawScrollBar(pDC, pScrollBar);

	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pScrollBar->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pScrollBar->GetScrollBarPosInfo();

	if(!pSBInfo->fSizebox)
	{
		int nHtPressed = (NULL != pSBTrack) ? pSBInfo->ht : -1;

		if (pSBInfo->fVert)
		{
			XTPScrollMetricsV scroll;
			CalcVScroll(pScrollBar, &scroll);

			// Draw up button
			if (scroll.bEnabled && nHtPressed == XTP_HTSCROLLUP)
			{
				pDC->Draw3dRect(scroll.rcArrowUp, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
			}
			else
			{
				pDC->Draw3dRect(scroll.rcArrowUp, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
			}
			DrawArrowGlyph(pDC, scroll.rcArrowUp, xtpArrowGlyphUp, scroll.bEnabled);

			// Drawing down button
			if (scroll.bEnabled && nHtPressed == XTP_HTSCROLLDOWN)
			{
				pDC->Draw3dRect(scroll.rcArrowDown, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
			}
			else
			{
				pDC->Draw3dRect(scroll.rcArrowDown, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
			}
			DrawArrowGlyph(pDC, scroll.rcArrowDown, xtpArrowGlyphDown, scroll.bEnabled);

		}
		else
		{
			XTPScrollMetricsH scroll;
			CalcHScroll(pScrollBar, &scroll);

			// Draw left button
			if (scroll.bEnabled && nHtPressed == XTP_HTSCROLLLEFT)
			{
				pDC->Draw3dRect(scroll.rcArrowLeft, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
			}
			else
			{
				pDC->Draw3dRect(scroll.rcArrowLeft, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
			}
			DrawArrowGlyph(pDC, scroll.rcArrowLeft, xtpArrowGlyphLeft, scroll.bEnabled);

			// Draw right button
			if (scroll.bEnabled && nHtPressed == XTP_HTSCROLLRIGHT)
			{
				pDC->Draw3dRect(scroll.rcArrowRight, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
			}
			else
			{
				pDC->Draw3dRect(scroll.rcArrowRight, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
			}
			DrawArrowGlyph(pDC, scroll.rcArrowRight, xtpArrowGlyphRight, scroll.bEnabled);
		}
	}
}
