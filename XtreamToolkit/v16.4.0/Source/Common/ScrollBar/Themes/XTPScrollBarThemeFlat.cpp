#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsClassic.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsUx.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeFlat.h>


void CXTPScrollBarThemeFlat::RefreshMetrics()
{
	CXTPScrollBarThemeWindowsUx::RefreshMetrics();
	m_crBorderHilite = GetXtremeColor(COLOR_3DHIGHLIGHT);
	m_crBorder = GetXtremeColor(COLOR_3DSHADOW);
}

void CXTPScrollBarThemeFlat::DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar)
{
	CXTPScrollBarThemeWindowsUx::DrawScrollBar(pDC, pScrollBar);

	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pScrollBar->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pScrollBar->GetScrollBarPosInfo();

	if(!pSBInfo->fSizebox)
	{
		int nHtPressed  = (NULL != pSBTrack) ? pSBInfo->ht : -1;

		if (pSBInfo->fVert)
		{
			XTPScrollMetricsV scroll;
			CalcVScroll(pScrollBar, &scroll);

			pDC->FillSolidRect(scroll.rcArrowUp, m_crBack);
			if (scroll.bEnabled && nHtPressed == XTP_HTSCROLLUP)
				pDC->Draw3dRect(scroll.rcArrowUp, m_crBorder, m_crBorderHilite);
			else
				pDC->Draw3dRect(scroll.rcArrowUp, m_crBorderHilite, m_crBorder);

			DrawArrowGlyph(pDC, scroll.rcArrowUp, xtpArrowGlyphUp, scroll.bEnabled);

			pDC->FillSolidRect(scroll.rcArrowDown, m_crBack);
			if (scroll.bEnabled && nHtPressed == XTP_HTSCROLLDOWN)
				pDC->Draw3dRect(scroll.rcArrowDown, m_crBorder, m_crBorderHilite);
			else
				pDC->Draw3dRect(scroll.rcArrowDown, m_crBorderHilite, m_crBorder);
			DrawArrowGlyph(pDC, scroll.rcArrowDown, xtpArrowGlyphDown, scroll.bEnabled);


			CWnd* pParent = pScrollBar->GetParentWindow();
			HBRUSH hbrRet = (HBRUSH)DefWindowProc(pParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pParent->GetSafeHwnd());

			::FillRect(pDC->GetSafeHdc(), &scroll.rcTrack, hbrRet);

			if (nHtPressed == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &scroll.rcLowerTrack);
			}

			if (!scroll.rcTrack.IsRectEmpty() && !scroll.rcBtnTrack.IsRectEmpty())
			{
				pDC->FillSolidRect(scroll.rcBtnTrack, m_crBack);
				pDC->Draw3dRect(scroll.rcBtnTrack, m_crBorderHilite, m_crBorder);
			}
		}
		else
		{
			XTPScrollMetricsH scroll;
			CalcHScroll(pScrollBar, &scroll);

			pDC->FillSolidRect(scroll.rcArrowLeft, m_crBack);
			if (scroll.bEnabled && nHtPressed == XTP_HTSCROLLUP)
			{
				pDC->Draw3dRect(scroll.rcArrowLeft, m_crBorder, m_crBorderHilite);
			}
			else
			{
				pDC->Draw3dRect(scroll.rcArrowLeft, m_crBorderHilite, m_crBorder);
			}

			DrawArrowGlyph(pDC, scroll.rcArrowLeft, xtpArrowGlyphLeft, scroll.bEnabled);

			pDC->FillSolidRect(scroll.rcArrowRight, m_crBack);
			if (scroll.bEnabled && nHtPressed == XTP_HTSCROLLDOWN)
			{
				pDC->Draw3dRect(scroll.rcArrowRight,m_crBorder, m_crBorderHilite);
			}
			else
			{
				pDC->Draw3dRect(scroll.rcArrowRight, m_crBorderHilite, m_crBorder);
			}
			DrawArrowGlyph(pDC, scroll.rcArrowRight, xtpArrowGlyphRight, scroll.bEnabled);


			CWnd* pParent = pScrollBar->GetParentWindow();
			HBRUSH hbrRet = (HBRUSH)DefWindowProc(pParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pParent->GetSafeHwnd());

			::FillRect(pDC->GetSafeHdc(), &scroll.rcTrack, hbrRet);

			if (nHtPressed == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &scroll.rcLowerTrack);
			}

			if (!scroll.rcTrack.IsRectEmpty() && !scroll.rcBtnTrack.IsRectEmpty())
			{
				pDC->FillSolidRect(scroll.rcBtnTrack, m_crBack);
				pDC->Draw3dRect(scroll.rcBtnTrack, m_crBorderHilite, m_crBorder);
			}
		}
	}
	else
	{
		pDC->FillSolidRect(&pSBInfo->rc, GetXtremeColor(COLOR_3DFACE));
	}
}
