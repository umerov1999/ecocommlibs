#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsClassic.h>


void CXTPScrollBarThemeWindowsClassic::RefreshMetrics()
{
	CXTPScrollBarPaintManager::RefreshMetrics();
}

void CXTPScrollBarThemeWindowsClassic::DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar)
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pScrollBar->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pScrollBar->GetScrollBarPosInfo();

	if(!pSBInfo->fSizebox)
	{
		int nHtPressed = (NULL != pSBTrack) ? pSBInfo->ht : -1;

		if (pSBInfo->fVert)
		{
			XTPScrollMetricsV scroll;
			CalcVScroll(pScrollBar, &scroll);

			::DrawFrameControl(pDC->GetSafeHdc(), &scroll.rcArrowUp,   DFC_SCROLL, DFCS_SCROLLUP   | (!scroll.bEnabled ? DFCS_INACTIVE : 0) | (nHtPressed == XTP_HTSCROLLUP   ? DFCS_PUSHED : 0));
			::DrawFrameControl(pDC->GetSafeHdc(), &scroll.rcArrowDown, DFC_SCROLL, DFCS_SCROLLDOWN | (!scroll.bEnabled ? DFCS_INACTIVE : 0) | (nHtPressed == XTP_HTSCROLLDOWN ? DFCS_PUSHED : 0));

			CWnd* pParent = pScrollBar->GetParentWindow();
			HBRUSH hbrRet = (HBRUSH)DefWindowProc(pParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pParent->GetSafeHwnd());

			::FillRect(pDC->GetSafeHdc(), &scroll.rcTrack, hbrRet);

			if (nHtPressed == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &scroll.rcLowerTrack);
			}

			if (!scroll.rcTrack.IsRectEmpty() && !scroll.rcBtnTrack.IsRectEmpty())
			{
				pDC->FillSolidRect(scroll.rcBtnTrack, GetXtremeColor(COLOR_3DFACE));
				DrawEdge(pDC->GetSafeHdc(), &scroll.rcBtnTrack, EDGE_RAISED, (UINT)(BF_ADJUST | BF_RECT));
			}

		}
		else
		{
			XTPScrollMetricsH scroll;
			CalcHScroll(pScrollBar, &scroll);

			::DrawFrameControl(pDC->GetSafeHdc(), &scroll.rcArrowLeft, DFC_SCROLL, DFCS_SCROLLLEFT | (!scroll.bEnabled ? DFCS_INACTIVE : 0) | (nHtPressed == XTP_HTSCROLLUP ? DFCS_PUSHED : 0));
			::DrawFrameControl(pDC->GetSafeHdc(), &scroll.rcArrowRight, DFC_SCROLL, DFCS_SCROLLRIGHT | (!scroll.bEnabled ? DFCS_INACTIVE : 0)  | (nHtPressed == XTP_HTSCROLLDOWN ? DFCS_PUSHED : 0));

			CWnd* pParent = pScrollBar->GetParentWindow();
			HBRUSH hbrRet = (HBRUSH)DefWindowProc(pParent->GetSafeHwnd(), WM_CTLCOLORSCROLLBAR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pParent->GetSafeHwnd());

			::FillRect(pDC->GetSafeHdc(), &scroll.rcTrack, hbrRet);

			if (nHtPressed == XTP_HTSCROLLUPPAGE)
			{
				::InvertRect(pDC->GetSafeHdc(), &scroll.rcLowerTrack);
			}

			if (!scroll.rcTrack.IsRectEmpty() && !scroll.rcBtnTrack.IsRectEmpty())
			{
				pDC->FillSolidRect(scroll.rcBtnTrack, GetXtremeColor(COLOR_3DFACE));
				DrawEdge(pDC->GetSafeHdc(), &scroll.rcBtnTrack, EDGE_RAISED, (UINT)(BF_ADJUST | BF_RECT));
			}
		}
	}
	else
	{
		::DrawFrameControl(pDC->GetSafeHdc(), &pSBInfo->rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	}
}
