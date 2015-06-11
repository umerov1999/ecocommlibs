#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsClassic.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsUx.h>

CXTPScrollBarThemeWindowsUx::CXTPScrollBarThemeWindowsUx()
{
	m_themeScrollBar = new CXTPWinThemeWrapper();
}

CXTPScrollBarThemeWindowsUx::~CXTPScrollBarThemeWindowsUx()
{
	SAFE_DELETE(m_themeScrollBar);
}

void CXTPScrollBarThemeWindowsUx::RefreshMetrics()
{
	CXTPScrollBarPaintManager::RefreshMetrics();

	m_themeScrollBar->CloseThemeData();
	m_themeScrollBar->OpenTheme(0, L"SCROLLBAR");

	if(!m_themeScrollBar->IsAppThemeReady())
	{
		CXTPScrollBarThemeWindowsClassic::RefreshMetrics();
	}
}

#define GETPARTSTATE(ht, pressed, hot, normal, disabled) \
	(!scroll.bEnabled ? disabled : nHtPressed == ht ? pressed : nHtHot == ht ? hot : normal)

void CXTPScrollBarThemeWindowsUx::DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar)
{
	if(m_themeScrollBar->IsAppThemeReady())
	{
		XTP_SCROLLBAR_TRACKINFO *pSBTrack = pScrollBar->GetScrollBarTrackInfo();
		XTP_SCROLLBAR_POSINFO   *pSBInfo  = pScrollBar->GetScrollBarPosInfo();

		if(!pSBInfo->fSizebox)
		{
			int nHtHot      = (NULL != pSBTrack) ?          -1 : pSBInfo->ht;
			int nHtPressed  = (NULL != pSBTrack) ? pSBInfo->ht :          -1;

			if (pSBInfo->fVert)
			{
				XTPScrollMetricsV scroll;
				CalcVScroll(pScrollBar, &scroll);

				m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLUP, ABS_UPPRESSED, ABS_UPHOT, ABS_UPNORMAL, ABS_UPDISABLED),  scroll.rcArrowUp, NULL);
				m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLDOWN, ABS_DOWNPRESSED, ABS_DOWNHOT, ABS_DOWNNORMAL, ABS_DOWNDISABLED),  scroll.rcArrowDown, NULL);

				if (!scroll.rcTrack.IsRectEmpty())
				{
					if (!scroll.rcLowerTrack.IsRectEmpty())
						m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_LOWERTRACKVERT, GETPARTSTATE(XTP_HTSCROLLUPPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  scroll.rcLowerTrack, NULL);

					if (!scroll.rcBtnTrack.IsRectEmpty())
					{
						m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_THUMBBTNVERT, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  scroll.rcBtnTrack, NULL);
						if (scroll.rcBtnTrack.Height() > 13)
							m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_GRIPPERVERT, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  scroll.rcBtnTrack, NULL);
					}

					if (!scroll.rcUpperTrack.IsRectEmpty())
						m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_UPPERTRACKVERT, GETPARTSTATE(XTP_HTSCROLLDOWNPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED),  scroll.rcUpperTrack, NULL);
				}
			}
			else
			{
				XTPScrollMetricsH scroll;
				CalcHScroll(pScrollBar, &scroll);

				m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLUP, ABS_LEFTPRESSED, ABS_LEFTHOT, ABS_LEFTNORMAL, ABS_LEFTDISABLED), scroll.rcArrowLeft, NULL);
				m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLDOWN, ABS_RIGHTPRESSED, ABS_RIGHTHOT, ABS_RIGHTNORMAL, ABS_RIGHTDISABLED), scroll.rcArrowRight, NULL);

				if (!scroll.rcTrack.IsRectEmpty())
				{
					if (!scroll.rcLowerTrack.IsRectEmpty())
						m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_LOWERTRACKHORZ, GETPARTSTATE(XTP_HTSCROLLUPPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED), scroll.rcLowerTrack, NULL);

					if (!scroll.rcBtnTrack.IsRectEmpty())
					{
						m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_THUMBBTNHORZ, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED), scroll.rcBtnTrack, NULL);
						if (scroll.rcBtnTrack.Width() > 13)
						{
							m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_GRIPPERHORZ, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED), scroll.rcBtnTrack, NULL);
						}
					}

					if (!scroll.rcUpperTrack.IsRectEmpty())
					{
						m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_UPPERTRACKHORZ, GETPARTSTATE(XTP_HTSCROLLDOWNPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED), scroll.rcUpperTrack, NULL);
					}
				}
			}
		}
		else
		{
			COLORREF crBackground = m_themeScrollBar->GetThemeSysColor(COLOR_BTNFACE);
			pDC->FillSolidRect(&pSBInfo->rc, crBackground);
			m_themeScrollBar->DrawThemeBackground(pDC->GetSafeHdc(), SBP_SIZEBOX, 0, &pSBInfo->rc, NULL);
		}
	}
	else
	{
		CXTPScrollBarThemeWindowsClassic::DrawScrollBar(pDC, pScrollBar);
	}
}
