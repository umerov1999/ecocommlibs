// XTPCalendarPaintManager.cpp : implementation file
//
// This file is a part of the XTREME CALENDAR MFC class library.
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
#include <math.h>


#include <Common/XTPResourceManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSmartPtrInternalT.h>
#include <Common/XTPVC80Helpers.h>

#include <Calendar/Resource.h>
#include <Calendar/XTPCalendarUtils.h>
#include <Calendar/XTPCalendarDefines.h>
#include <Calendar/XTPCalendarOptions.h>
#include <Calendar/XTPCalendarEvent.h>
#include <Calendar/XTPCalendarPtrCollectionT.h>
#include <Calendar/XTPCalendarEventLabel.h>
#include <Calendar/XTPCalendarView.h>
#include <Calendar/XTPCalendarViewDay.h>
#include <Calendar/XTPCalendarViewEvent.h>
#include <Calendar/XTPCalendarViewPart.h>
#include <Calendar/XTPCalendarWeekViewEvent.h>
#include <Calendar/XTPCalendarWeekViewDay.h>
#include <Calendar/XTPCalendarWeekView.h>
#include <Calendar/XTPCalendarMonthViewEvent.h>
#include <Calendar/XTPCalendarMonthView.h>
#include <Calendar/XTPCalendarDayViewEvent.h>
#include <Calendar/XTPCalendarDayViewDay.h>
#include <Calendar/XTPCalendarDayView.h>
#include <Calendar/XTPCalendarNotifications.h>
#include <Calendar/XTPCalendarTimeLineViewGroup.h>
#include <Calendar/XTPCalendarTimeLineViewTimeScale.h>
#include <Calendar/XTPCalendarTimeLineView.h>
#include <Calendar/XTPCalendarControl.h>
#include <Calendar/XTPCalendarPaintManager.h>
#include <Calendar/XTPCalendarShadowDrawer.h>
#include <Calendar/XTPCalendarData.h>

static const double dXTPMaxAlpha2003 = 75.;

static const LPCTSTR cszGlyphsFontName = _T("MS Outlook");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CXTPCalendarPaintManager::CControlPart::RefreshMetrics()
{
	m_clrTextColor.SetStandardValue(m_pPaintManager->m_clrButtonFaceText);
	m_clrBackground.SetStandardValue(m_pPaintManager->m_clrButtonFace);

	LOGFONT lfIcon;
	VERIFY(m_pPaintManager->GetIconLogFont(&lfIcon));

	m_fntText.SetStandardValue(&lfIcon);

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrTextColor = RGB(38, 38, 38);
	}
}

void CXTPCalendarPaintManager::CDayViewEventPart::RefreshMetrics()
{
	m_clrBackground.SetStandardValue(RGB(255, 255, 255));

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		//m_clrBackground.SetStandardValue(RGB(205, 230, 247));
	}
}

COLORREF GetDarkColor(COLORREF clrColor)
{
	DWORD dwHSLBackground = CXTPDrawHelpers::RGBtoHSL(clrColor);
	DWORD dwL = GetBValue(dwHSLBackground);

	DWORD dwDark = MulDiv(dwL, 2, 3);
	return CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwDark));
}

void CXTPCalendarPaintManager::CDayViewEventPart::OnDraw(CDC* pDC, CXTPCalendarDayViewEvent* pViewEvent)
{
	if (!pDC || !pViewEvent || !pViewEvent->GetEvent() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return;
	}

	// fill event background
	CXTPCalendarEventLabel* ptrLabel = pViewEvent->GetEvent()->GetLabel();
	COLORREF crBackground = ptrLabel ? ptrLabel->m_clrColor : GetBackgroundColor();

	BOOL bSelected = pViewEvent->IsSelected();
	int nMDEFlags = pViewEvent->GetMultiDayEventFlags();

	BOOL bMultiDay = (nMDEFlags & xtpCalendarMultiDayFMLmask) != 0;
//Start - End options
	DWORD dwAdditionalOpt = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
	BOOL bShowStartTime = dwAdditionalOpt & xtpCalendarOptDayViewShowStartTimeAlways;
	BOOL bShowEndTime = dwAdditionalOpt & xtpCalendarOptDayViewShowEndTimeAlways;

	CString strLoc = pViewEvent->GetItemTextLocation();
	CString strText = pViewEvent->GetItemTextSubject();
	if (strLoc.GetLength() > 0)
	{
		if (dwAdditionalOpt & xtpCalendarOptDayViewNoWordBreak)
			strText += _T("\n(") + strLoc + _T(")");
		else
			strText += _T(" (") + strLoc + _T(")");
	}
	CString strTime;

	if (!bMultiDay)
	{
		COleDateTime dtStart = pViewEvent->GetEvent()->GetStartTime();
		COleDateTime dtEnd = pViewEvent->GetEvent()->GetEndTime();

		DWORD dwTimeFlags = 0;
		if (bShowEndTime)
			dwTimeFlags = xtpCalendarItemText_EventStartTimeText;
		if (!CXTPCalendarUtils::IsEqual(dtStart, dtEnd) && bShowEndTime)
			dwTimeFlags = dwTimeFlags | xtpCalendarItemText_EventEndTimeText;

		strTime = pViewEvent->GetItemText(dwTimeFlags, pViewEvent->IsHideEventTime() ? _T("") : NULL);
	}

	CRect rcBorder(rcEvent);
	rcBorder.left += 1;

	if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
	{
		pDC->FillSolidRect(rcBorder, crBackground);

		pDC->FillSolidRect(rcBorder.left, rcBorder.top, rcBorder.Width(), 1, m_pPaintManager->m_clrWindowText);
		pDC->FillSolidRect(rcBorder.left - 1, rcBorder.bottom - (bMultiDay ? 1 : 0),
			rcBorder.Width() + 1, 1, m_pPaintManager->m_clrWindowText);

		if (!bMultiDay)
		{
			pDC->FillSolidRect(rcBorder.right, rcBorder.top, 1, rcBorder.Height()+1, m_pPaintManager->m_clrWindowText);
		}

		if (bSelected && bMultiDay)
		{
			pDC->FillSolidRect(rcEvent.left, rcEvent.top + 1, rcEvent.Width(), rcEvent.Height() - 2, m_pPaintManager->m_clrButtonFace);
			pDC->FillSolidRect(rcEvent.left, rcEvent.top - 1, rcEvent.Width()+1, 1, m_pPaintManager->m_clrWindowText);
			pDC->FillSolidRect(rcEvent.left, rcEvent.bottom, rcEvent.Width()+1, 1, m_pPaintManager->m_clrWindowText);
		}
	}
	else
	{
		CRect rcBigRect = rcBorder;
		rcBigRect.DeflateRect(3, 0, 0, 0);

		pDC->FillSolidRect(rcBigRect, crBackground);

		CRect rcBusyArea = rcBigRect;
		rcBusyArea.left = rcBusyArea.left - 7;
		rcBusyArea.right = rcBusyArea.left + 7;

		//////////////////////////////////////////////////////////////////////////

		if (pViewEvent->GetEvent()->GetBusyStatus() == xtpCalendarBusyStatusTentative)
		{
			// using ::SelectObject instead of CDC::SelectObject because VC6 can assert in print preview
			HGDIOBJ hOldBrush = ::SelectObject(pDC->GetSafeHdc(), ::GetStockObject(NULL_BRUSH));

			CPen outerPen;
			outerPen.CreatePen(PS_SOLID, 1, GetDarkColor(crBackground));
			CPen* pOldPen = pDC->SelectObject(&outerPen);

			pDC->Rectangle(rcBusyArea);

			::SelectObject(pDC->GetSafeHdc(), hOldBrush);
			pDC->SelectObject(pOldPen);

			rcBusyArea.DeflateRect(1, 1, 1, 1);

			CBrush br(HS_BDIAGONAL, GetDarkColor(crBackground));

			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->FillRect(rcBusyArea, &br);

		}
		else
		{
			CPen outerPen;
			outerPen.CreatePen(PS_SOLID, 1, GetDarkColor(crBackground));
			CPen* pOldPen = pDC->SelectObject(&outerPen);

			pDC->Rectangle(rcBusyArea);

			pDC->SelectObject(pOldPen);
		}
	}

	if (nMDEFlags & xtpCalendarMultiDayFirst)
	{
		CRect rcClockRect = pViewEvent->GetFirstClockRect();

		if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
			pDC->FillSolidRect(rcEvent.left, rcEvent.top, 1, rcEvent.Height(), m_pPaintManager->m_clrWindowText);
		else
		{
			// office 2013
			rcClockRect.OffsetRect(4, 0); // looks better
		}

		rcEvent.left++;
		if (rcClockRect.Width() > 0)
		{
			m_pPaintManager->DrawClock(pDC, pViewEvent->GetEvent()->GetStartTime(), rcClockRect, crBackground, xtpCalendarClockAlignLeft);
		}
	}
	if (nMDEFlags & xtpCalendarMultiDayLast)
	{
		if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
			pDC->FillSolidRect(rcEvent.right, rcEvent.top, 1, rcEvent.Height(), m_pPaintManager->m_clrWindowText);

		rcEvent.right -= 2;
		if (pViewEvent->GetLastClockRect().Width() > 0)
		{
			m_pPaintManager->DrawClock(
				pDC,
				pViewEvent->GetEvent()->GetEndTime(),
				pViewEvent->GetLastClockRect(),
				crBackground,
				xtpCalendarClockAlignLeft);
		}

	}

	UINT uAlign = 0;
	UINT uAlignWordBreak = 0;

	CRect rcText(pViewEvent->GetTextRect());
	CRect rcTime(rcText);
	BOOL bTimeOutOfBorders = FALSE;

	//  draw busy border
	if (!bMultiDay)
	{
		rcText.top += min(2, rcText.Height());
		rcTime.top = rcText.top;

		int nBusyStatus = pViewEvent->GetEvent()->GetBusyStatus();

		// fill busy area
		CRect rcBusyArea(rcEvent);
		//  draw border
		rcBusyArea.right = rcBusyArea.left + 7;
		//rcText.left = rcBusyArea.right + 3;

		if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
		{
			pDC->FillSolidRect(rcBusyArea, m_pPaintManager->m_clrWindowText);
			//  draw background
			rcBusyArea.DeflateRect(1, 1, 1, 0);
			pDC->FillSolidRect(rcBusyArea, m_pPaintManager->m_clrWindow);
		}
		else
		{
			if (pViewEvent->GetEvent()->GetBusyStatus() == xtpCalendarBusyStatusTentative)
			{
				// using ::SelectObject instead of CDC::SelectObject because VC6 can assert in print preview
				HGDIOBJ hOldBrush = ::SelectObject(pDC->GetSafeHdc(), ::GetStockObject(NULL_BRUSH));

				CPen outerPen;
				outerPen.CreatePen(PS_SOLID, 1, GetDarkColor(crBackground));
				CPen* pOldPen = pDC->SelectObject(&outerPen);

				pDC->Rectangle(rcBusyArea);

				::SelectObject(pDC->GetSafeHdc(), hOldBrush);
				pDC->SelectObject(pOldPen);

				rcBusyArea.DeflateRect(1, 1, 1, 1);

				CBrush br(HS_BDIAGONAL, GetDarkColor(crBackground));

				pDC->SetBkColor(RGB(255, 255, 255));
				pDC->FillRect(rcBusyArea, &br);

			}
			else
			{
				pDC->FillSolidRect(rcBusyArea, GetDarkColor(crBackground));
			}
		}

		if (pViewEvent->GetTimeframeRect().top == LONG_MAX ||
			pViewEvent->GetTimeframeRect().bottom == LONG_MAX)
		{
			POINT arBusyPoints[] =
			{
				{rcEvent.left, rcEvent.top},
				{rcEvent.left + 7, rcEvent.top},
				{rcEvent.left + 7, rcEvent.top + 7},
				{rcEvent.left, rcEvent.top}
			};
			const int cBPCount = sizeof(arBusyPoints) / sizeof(arBusyPoints[0]);
			CRgn rgnBusy;
			BOOL bRgn = rgnBusy.CreatePolygonRgn(arBusyPoints, cBPCount, ALTERNATE);
			ASSERT(bRgn);

			if (bRgn)
				m_pPaintManager->DrawBusyStatus(pDC, rgnBusy, nBusyStatus);
		}
		else
		{
			//  draw busy border
			rcBusyArea.top = pViewEvent->GetTimeframeRect().top;
			rcBusyArea.bottom = pViewEvent->GetTimeframeRect().bottom;

			//  draw busy color
			if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
			{
				pDC->FillSolidRect(rcBusyArea, m_pPaintManager->m_clrWindowText);

				rcBusyArea.DeflateRect(0, 1, 0, 1);

				m_pPaintManager->DrawBusyStatus(pDC, rcBusyArea, nBusyStatus);
			}
			else
			{

			}
		}

		// draw shadow
		if (!bSelected)
			m_pPaintManager->DrawShadow(pDC, pViewEvent->GetEventRect());

		// draw border
		if (bSelected)
		{
			if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
			{
				CBrush brushBorder(m_pPaintManager->m_clrWindowText);

				CRect rcBorder2(pViewEvent->GetEventRect());
				rcBorder2.right += 1;

				CRect rcBorderTop(rcBorder2);
				rcBorderTop.bottom = rcBorderTop.top + 1;
				rcBorderTop.top -= 5;

				m_pPaintManager->DrawBusyStatus(pDC, rcBorderTop, nBusyStatus);
				pDC->FrameRect(&rcBorderTop, &brushBorder);

				CRect rcBorderBottom(rcBorder2);
				rcBorderBottom.top = rcBorderBottom.bottom;
				rcBorderBottom.bottom += 6;

				m_pPaintManager->DrawBusyStatus(pDC, rcBorderBottom, nBusyStatus);
				pDC->FrameRect(&rcBorderBottom, &brushBorder);
			}
			else
			{
				CRect rcBorder2(pViewEvent->GetEventRect());

				// using ::SelectObject instead of CDC::SelectObject because VC6 can assert in print preview
				HGDIOBJ hOldBrush = ::SelectObject(pDC->GetSafeHdc(), ::GetStockObject(NULL_BRUSH));

				CPen outerPen;
				outerPen.CreatePen(PS_SOLID, 2, RGB(0,0,0));
				CPen* pOldPen = pDC->SelectObject(&outerPen);

				rcBorder2.InflateRect(-1, 1, 1, 2);

				pDC->Rectangle(rcBorder2);

				::SelectObject(pDC->GetSafeHdc(), hOldBrush);
				pDC->SelectObject(pOldPen);

				//rcBorder.DeflateRect(2, 2);
			}
		}
		rcText.right -= 4;

		uAlignWordBreak = dwAdditionalOpt & xtpCalendarOptDayViewNoWordBreak ? DT_WORD_ELLIPSIS : DT_WORDBREAK;
		uAlign |= DT_LEFT | uAlignWordBreak | DT_EDITCONTROL;

		if (!strTime.IsEmpty())
		{
			CString strTest = strTime;
			if (dwAdditionalOpt & xtpCalendarOptDayViewNoWordBreak)
				strTest += _T(" W...");

			CSize szTime = GetTextExtent(pDC, strTest);
			if (szTime.cx <= rcText.Width())
			{
				if (bShowStartTime || bShowEndTime)
					strText = strTime + _T(" ") + strText;
				strTime.Empty();
			}
			else if (bShowStartTime || bShowEndTime)
			{
				bTimeOutOfBorders = TRUE;
				rcTime.bottom = rcTime.top + szTime.cy;
				rcText.top = rcTime.bottom;
			}
			else
			{
				strTime.Empty();
			}
		}
	}
	else
	{
		rcText.left += 3;
		CSize szText = GetTextExtent(pDC, strText);
		if (pViewEvent->IsMultyDayTextAlign() || szText.cx + 2 > rcText.Width())
		{
			uAlign |= DT_LEFT;
		}
		else
		{
			uAlign |= DT_CENTER;
		}
		uAlign |= DT_SINGLELINE;
	}

	m_pPaintManager->DrawIcons(pDC, pViewEvent);

	// draw event time, if need
	if (!strTime.IsEmpty())
		DrawText(pDC, strTime, rcTime, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	// draw event text
	DrawText(pDC, strText, rcText, uAlign | DT_VCENTER);

	// update OutOfBorders flag
	int nX_need = 0, nY_need = 0;
	if (uAlign & DT_SINGLELINE)
	{
		CSize szText = GetTextExtent(pDC, strText);
		nX_need = szText.cx;
	}
	else
	{
		CRect rcNeeded(0, 0, rcText.Width(), 0);
		DrawText(pDC, strText, rcNeeded, DT_CALCRECT | uAlignWordBreak);
		nY_need = rcNeeded.Height();
	}

	BOOL bOutOfBorders = nY_need > rcText.Height() || nX_need > rcText.Width() || bTimeOutOfBorders;

	pViewEvent->SetTextOutOfBorders(bOutOfBorders);
}

void CXTPCalendarPaintManager::DrawShadow(CDC* pDC, const CRect& rcRect)
{
	ASSERT(pDC);
	if (!pDC || pDC->IsPrinting())
	{
		return;
	}

	CXTPCalendarShadowDrawer oShadow(pDC, dXTPMaxAlpha2003);

	CRect rcVShadow(rcRect);
	rcVShadow.left = rcVShadow.right;
	rcVShadow.right += 5;
	rcVShadow.top += 5;
	oShadow.DrawShadowRectPart(rcVShadow, CXTPCalendarShadowDrawer::xtpShadowLR);

	CRect rcHShadow(rcRect);
	rcHShadow.left += 7;
	rcHShadow.top = rcHShadow.bottom + 1;
	rcHShadow.bottom += 6;
	oShadow.DrawShadowRectPart(rcHShadow, CXTPCalendarShadowDrawer::xtpShadowTB);

	CRect rcTRShadow(rcVShadow);
	rcTRShadow.bottom = rcTRShadow.top;
	rcTRShadow.top -= 5;
	oShadow.DrawShadowRectPart(rcTRShadow, CXTPCalendarShadowDrawer::xtpShadowBR);

	CRect rcBRShadow(rcVShadow);
	rcBRShadow.top = rcBRShadow.bottom;
	rcBRShadow.bottom += 5;
	oShadow.DrawShadowRectPart(rcBRShadow, CXTPCalendarShadowDrawer::xtpShadowTR);

	CRect rcBLShadow(rcRect);
	rcBLShadow.right = rcBLShadow.left + 7;
	rcBLShadow.top = rcBLShadow.bottom + 1;
	rcBLShadow.bottom += 6;
	oShadow.DrawShadowRectPart(rcBLShadow, CXTPCalendarShadowDrawer::xtpShadowBL);
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::RefreshMetrics()
{
	m_nHourWidth = m_nMinWidth = 0;
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	STRCPY_S(lfIcon.lfFaceName, LF_FACESIZE, _T("Tahoma"));
	m_fntTimeText.SetStandardValue(&lfIcon);

	// apply colors

	m_clrBorder = m_pPaintManager->m_clr3DShadow;
	m_clrBackground.SetStandardValue(::GetSysColor(COLOR_3DFACE));

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 255));

		m_clrBorder = RGB(225, 225, 225);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::CalcWidth(CDC* pDC,
					const CString& strHour, const CString& strMin,
					int nHourHeight, int& nWidth)
{
	ASSERT(pDC);
	if (!pDC || !m_pPaintManager)
		return;

	CSize szMin(0, 0), szHour(0, 0);

	{
		CXTPFontDC fnt(pDC, &GetTextFont());
		szMin = pDC->GetTextExtent(strMin);
	}

	{
		CRect rcDefaultCell(0, 0, 100, nHourHeight);
		AdjustTimeFont(pDC, rcDefaultCell);

		CXTPFontDC fnt(pDC, m_fntTimeText);
		szHour = pDC->GetTextExtent(strHour);
		// check also widest hours number
		CSize szHour2 = pDC->GetTextExtent(_T("88"), 2);
		if (szHour2.cx > szHour.cx)
			szHour = szHour2;
	}

	m_nHourWidth = szHour.cx;
	m_nMinWidth = szMin.cx;
	nWidth = max(42, szHour.cx + szMin.cx + 17);
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::AdjustTimeFont(CDC* pDC, CRect rcCell)
{
	ASSERT(pDC);
	if (!pDC)
		return;

	rcCell.DeflateRect(5, 6, 1, 7);

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		if (rcCell.Height() > 10)
			rcCell.DeflateRect(0, rcCell.Height()/2 - 2, 0, 0); // this will make the font smaller as required in Outlook2013

		if (rcCell.Height() > 26)
			rcCell.DeflateRect(0, 0, 0, rcCell.Height() - 26); // set maximum font height

		if (rcCell.Height() < 14)
			rcCell.InflateRect( 0, 0, 0, 15 - rcCell.Height()); // set minimum font height
	}

	LOGFONT lfIcon;
	((CFont*)m_fntTimeText)->GetLogFont(&lfIcon);
	CSize szText;
	{
		CXTPFontDC fontBig(pDC, m_fntTimeText);
		szText = pDC->GetTextExtent(_T("88"), 2);
	}

	int nParam1 = rcCell.Width() * szText.cy;
	int nParam2 = rcCell.Height() * szText.cx;
	if (nParam1 > nParam2)
		lfIcon.lfHeight = ::MulDiv(lfIcon.lfHeight, rcCell.Height(), szText.cy);
	else
		lfIcon.lfHeight = ::MulDiv(lfIcon.lfHeight, rcCell.Width(), szText.cx);

	m_fntTimeText.SetStandardValue(&lfIcon);
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::DrawHourCell(CDC* pDC, CRect rc, CString strText, BOOL bFillBackground, BOOL bCurrentTime /*= FALSE*/)
{
	if (!pDC || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	if (bFillBackground)
		pDC->FillSolidRect(rc, GetBackgroundColor());

	pDC->FillSolidRect(rc.right - 1, rc.top, 1, rc.Height(), m_clrBorder);

	COLORREF clrOldTextColor = GetTextColor();

	if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
	{
		rc.DeflateRect(0, 0, 4, 0);

		DrawText(pDC, strText, rc, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
	}
	else
	{
		// office 2013

		if (bCurrentTime)
		{
			SetTextColor(RGB(42,141,212));
		}

		// Draw hours text
		CXTPFontDC fontBig(pDC, m_fntTimeText, GetTextColor());

		pDC->DrawText(strText, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}

	rc.DeflateRect(5, 0, 0, 0);
	pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, m_clrBorder);

	// restore text color
	SetTextColor(clrOldTextColor);
}

void CXTPCalendarPaintManager::CDayViewTimeScaleCellPart::DrawBigHourCell(CDC* pDC, CRect rc, CString strHour, CString strMin, int nRowPerHour, BOOL bFillBackground, BOOL bCurrentTime /*= FALSE*/)
{
	if (!pDC || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	// Fill background
	if (bFillBackground)
		pDC->FillSolidRect(rc, GetBackgroundColor());

	pDC->FillSolidRect(rc.right - 1, rc.top, 1, rc.Height(), m_clrBorder);

	// Calculate coordinates
	CRect rcMinutesText(rc);
	rcMinutesText.bottom = rcMinutesText.top + rc.Height() / nRowPerHour;
	rcMinutesText.left += m_nHourWidth + 10;

	CRect rcHoursText(rc);
	rcHoursText.right = rcMinutesText.left;
	rcHoursText.DeflateRect(0, 4, 4, 7);

	COLORREF clrOldTextColor = GetTextColor();
	COLORREF clrTextColor = GetTextColor();

	UINT dtHourTextFormat = DT_TOP | DT_RIGHT | DT_SINGLELINE | DT_NOCLIP;

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		if (bCurrentTime)
		{
			clrTextColor = RGB(42,141,212);

			SetTextColor(clrTextColor);
		}

		dtHourTextFormat = DT_TOP | DT_CENTER | DT_SINGLELINE | DT_NOCLIP;
		rcHoursText = rc;
		rcHoursText.DeflateRect(0, 2, 10, 0);

		rcMinutesText.left += 2;
		rcMinutesText.top -= 6;
	}

	// Draw hours text
	CXTPFontDC fontBig(pDC, m_fntTimeText, clrTextColor);

	pDC->DrawText(strHour, rcHoursText, dtHourTextFormat);

	if (strMin.GetLength())
	{
		// draw minutes text
		DrawText(pDC, strMin, rcMinutesText, DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOCLIP);
	}

	// draw small lines
	if (nRowPerHour > 2)
	{
		BOOL bShowMinutes = XTP_SAFE_GET3(m_pPaintManager, m_pControl, GetCalendarOptions(), bDayView_TimeScaleShowMinutes, FALSE);
		CString strMinEx;

		for (int i = 1; i < nRowPerHour; i++)
		{
			int y = rc.top + i * rc.Height() / nRowPerHour;
			pDC->FillSolidRect(rcMinutesText.left, y - 1, rcMinutesText.Width(), 1, m_clrBorder);

			if (bShowMinutes)
			{
				int nMinEx = 60 / nRowPerHour * i;
				strMinEx.Format(_T("%02d"), nMinEx);

				CRect rcMinEx = rcMinutesText;
				rcMinEx.top = y;
				rcMinEx.bottom = y + rc.Height();

				DrawText(pDC, strMinEx, rcMinEx, DT_TOP | DT_LEFT | DT_SINGLELINE | DT_NOCLIP);
			}
		}
	}

	// restore text color
	SetTextColor(clrOldTextColor);

	// draw line below
	CRect rcLine = rc;

	if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
	{
		rcLine.DeflateRect(5, 0, 4, 0);
	}
	else
	{
		rcLine.DeflateRect(5, 0, 0, 0);
	}

	pDC->FillSolidRect(rcLine.left, rcLine.bottom - 1, rcLine.Width(), 1, m_clrBorder);
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::RefreshMetrics()
{
	if (!m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	// apply colors

	m_clrBorder = m_pPaintManager->m_clr3DShadow;
	m_clrBackground.SetStandardValue(::GetSysColor(COLOR_3DFACE));

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 255));

		m_clrBorder = RGB(225, 225, 225);
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::OnDraw(CDC* pDC, CXTPCalendarDayView* /*pView*/, CRect rc, CString strText)
{
	if (!pDC || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	pDC->FillSolidRect(rc, GetBackgroundColor());
	pDC->FillSolidRect(rc.right - 1, rc.top, 1, rc.Height(), m_clrBorder);

	int nLeftDeflate = 4;

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		nLeftDeflate = 0;
	}

	rc.DeflateRect(1, 0, nLeftDeflate, 0);
	pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, m_clrBorder);

	rc.DeflateRect(1, 2);
	DrawText(pDC, strText, rc, DT_BOTTOM | DT_LEFT | DT_SINGLELINE);
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::DrawNowLine(CDC* pDC,
					CXTPCalendarDayView* pView, CRect rc, int y, BOOL bDrawBk)
{
	if (!pDC || !pView || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	int nHeight = CXTPCalendarUtils::GetTotalMinutes(pView->GetScaleInterval()) >= 60 ? 5 : 10;

	CRect rcNowLine = rc;
	rcNowLine.top = y - nHeight;
	rcNowLine.bottom = y + 1;
	rcNowLine.left += 5;
	rcNowLine.right -= 1;

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		CRect rcClient;
		GetCalendarControl()->GetClientRect(rcClient);

		nHeight = 3;

		rcNowLine.bottom = rcNowLine.top + nHeight;
		rcNowLine.right = rcClient.Width();

		pDC->FillSolidRect(&rcNowLine, RGB(177,214,240));
		rcNowLine.DeflateRect(0, 1, 0, 0);
		pDC->FillSolidRect(&rcNowLine, RGB(42,141,212));
		rcNowLine.DeflateRect(0, 1, 0, 0);
		pDC->FillSolidRect(&rcNowLine, RGB(177,214,240));
	}
	else
	{
		BOOL bXPTheme = m_pPaintManager->GetCurrentSystemTheme() != xtpSystemThemeUnknown;

		if (bDrawBk)
		{
			if (bXPTheme)
				XTPDrawHelpers()->GradientFill(pDC, &rcNowLine, m_pPaintManager->m_grclrToday, FALSE);
		}
		else
		{
			rcNowLine.top = rcNowLine.bottom - 1;
			pDC->FillSolidRect(&rcNowLine, m_pPaintManager->m_clrUnderLineHdr);
		}
	}
}

void CXTPCalendarPaintManager::CDayViewTimeScaleHeaderPart::DrawExpandSigns(CDC* pDC,
					CXTPCalendarDayView* pView, const CRect& rcTSHours)
{
	if (!pDC || !pView || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	// draw expand signs
	CRect rcSignUp(rcTSHours);
	CRect rcSignDown(rcTSHours);
	CSize szSign(m_pPaintManager->GetExpandSignSize());

	rcSignUp.left = rcTSHours.right - szSign.cx;

	rcSignDown.left = rcTSHours.right - szSign.cx;
	rcSignDown.top = rcSignDown.bottom - szSign.cy;

	CImageList* pilExpSgn = &m_pPaintManager->m_ilExpandSigns;

	if (pView->IsExpandUp())
		m_pPaintManager->DrawBitmap(pilExpSgn, pDC, rcSignUp, CXTPCalendarPaintManager::idxExpandSignUp);
	else if (pView->IsExpandDown())
		m_pPaintManager->DrawBitmap(pilExpSgn, pDC, rcSignDown, CXTPCalendarPaintManager::idxExpandSignDown);
}

void CXTPCalendarPaintManager::CDayViewCellPart::GetParams(CXTPCalendarDayViewGroup* pViewGroup,
		XTP_CALENDAR_DAYVIEWCELL_PARAMS* rCellParams)
{
	if (!pViewGroup || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	rCellParams->clrBackgroundColor =  rCellParams->bSelected ? m_pPaintManager->m_clrHighlight : GetBackgroundColor();
}

void CXTPCalendarPaintManager::CDayViewCellPart::RefreshMetrics()
{
	if (!m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}
}

void CXTPCalendarPaintManager::CDayViewCellPart::OnDraw(CDC* pDC,
														CXTPCalendarDayViewGroup* pViewGroup, CRect rc,
														const XTP_CALENDAR_DAYVIEWCELL_PARAMS& cellParams)
{
	if (!pDC || !pViewGroup || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	int iLeftMargin = 7;
	COLORREF clrBorder = m_pPaintManager->m_clrWindowText;

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		iLeftMargin = 1;

		clrBorder = m_clrHour; // gray color

		// TODO: remove vertical gray cell line in the left
	}

	COleDateTime dtTime = cellParams.dtBeginTime;

	CRect rcBusyStatus(rc);
	rcBusyStatus.right = rcBusyStatus.left + (iLeftMargin - 1);

	pDC->FillSolidRect(&rcBusyStatus, m_pPaintManager->m_clrWindow);
	int nDayBusyStatus = pViewGroup->GetBusyStatus(dtTime);
	if (nDayBusyStatus != xtpCalendarBusyStatusUnknown)
		m_pPaintManager->DrawBusyStatus(pDC, rcBusyStatus, nDayBusyStatus);

	if (XTP_SAFE_GET2(pViewGroup, GetViewDay(), GetDayIndex(), 0) != 0 || pViewGroup->GetGroupIndex() != 0)
		pDC->FillSolidRect(rc.left, rc.top, 1, rc.Height(), clrBorder);

	int nDayDeviderX = 0;//pViewGroup->GetGroupIndex() ? -2 : 0;

	pDC->FillSolidRect(rc.left + (iLeftMargin - 1) + nDayDeviderX, rc.top, 1, rc.Height(), clrBorder);

	rc.left += iLeftMargin + nDayDeviderX;

	if (m_pPaintManager->GetPaintTheme() >= xtpCalendarThemeOffice2003 ||
		cellParams.bSelected)
	{
		pDC->FillSolidRect(rc, cellParams.clrBackgroundColor);
	}
	else
	{
		WORD arBits[2] = {0xAA, 0x55};

		// Use the bit pattern to create a bitmap.
		CBitmap bm;
		bm.CreateBitmap(8,2,1,1, arBits);

		CBrush brush;
		brush.CreatePatternBrush(&bm);

		COLORREF clrTxt0 = pDC->SetTextColor(cellParams.clrBackgroundColor);
		COLORREF clrBk0 = pDC->SetBkColor(m_clrBackgroundSlave);

		pDC->FillRect(rc, &brush);

		pDC->SetTextColor(clrTxt0);
		pDC->SetBkColor(clrBk0);
	}

	if (dtTime.GetMinute() == 0 && dtTime.GetHour() != 0)
		pDC->FillSolidRect(rc.left, rc.top - 1, rc.Width(), 1, m_clrHour);

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		COleDateTime begin, end;
		pViewGroup->GetViewDay()->GetView()->GetSelection(&begin, &end);

		// this check doesn't look fine and pretty, but this is how Outlook2013 deals with selection
		// it means that we shouldn't draw dotted line for a the last selected cell (in selection)
		if (dtTime >= begin && dtTime <= end && (dtTime + pViewGroup->GetViewDay()->GetView()->GetCellDuration() >= end))
		{

		}
		else
		{
			COLORREF clrDottedLineColor = cellParams.bSelected ? RGB(255, 255, 255) : m_clrShadow;

			CPen outerPen;
			outerPen.CreatePen(PS_DOT, 1, clrDottedLineColor);
			CPen* pOldPen = pDC->SelectObject(&outerPen);

			pDC->MoveTo(rc.left, rc.bottom-1);
			pDC->LineTo(rc.right, rc.bottom-1);

			pDC->SelectObject(pOldPen);
		}
	}
	else
	{
		pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, m_clrShadow);
	}
}

void CXTPCalendarPaintManager::CDayViewAllDayEventsPart::RefreshMetrics()
{
	m_clrBackground.SetStandardValue(m_pPaintManager->m_clr3DShadow);

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 255));
	}
}

void CXTPCalendarPaintManager::CDayViewAllDayEventsPart::OnDraw(CDC* pDC,
				CXTPCalendarDayViewGroup* pViewGroup, CRect rc, BOOL bSelected)
{
	if (!pDC || !pViewGroup || !pViewGroup->GetViewDay() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	COLORREF clrBorder = RGB(0, 0, 0);

	if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
	{
		if (bSelected)
		{
			pDC->FillSolidRect(rc, m_pPaintManager->m_clrWindow);
			pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, m_pPaintManager->m_clr3DShadow);
			pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, m_pPaintManager->m_clr3DShadow);
		}
		else
		{
			pDC->FillSolidRect(rc, GetBackgroundColor());
		}
	}
	else
	{
		// Office 2013

		clrBorder = RGB(225,225,225);

		pDC->FillSolidRect(rc, bSelected ? RGB(153,200,233) : GetBackgroundColor());
	}

	if (pViewGroup->GetViewDay()->GetDayIndex() || pViewGroup->GetGroupIndex())
		pDC->FillSolidRect(rc.left, rc.top + 1, 1, rc.Height() - 2, clrBorder);

}

void CXTPCalendarPaintManager::CDayViewHeaderPart::RefreshMetrics()
{
	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground = RGB(225,242,250);
	}
}

void CXTPCalendarPaintManager::CDayViewHeaderPart::OnDraw(CDC* pDC,
					CXTPCalendarDayViewDay* pViewDay, CRect rc, CString strText)
{
	if (!pDC || !pViewDay || !pViewDay->GetView() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bIsToday = XTP_SAFE_GET1(GetCalendarControl(), IsToday(pViewDay->GetDayDate()), FALSE);

	pDC->FillSolidRect(rc, GetBackgroundColor());

	if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
	{
		CRect rcHeader(rc);

		m_pPaintManager->DrawHeader(pDC, rcHeader, FALSE, bIsToday);

		BOOL bFirstDay = pViewDay->GetDayIndex() == 0;
		BOOL bLastDay = pViewDay->GetDayIndex() == pViewDay->GetView()->GetViewDayCount() - 1;

		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, m_clrTopLeftBorder);
		pDC->FillSolidRect(rc.left, rc.top + (bFirstDay ? 0: 2), 1, rc.Height() -  (bFirstDay ? 0 : 3), m_clrTopLeftBorder);
		pDC->FillSolidRect(rc.right - 1, rc.top + (bLastDay ? 0: 2), 1, rc.Height() - (bLastDay ? 0 : 3), m_pPaintManager->m_clr3DShadow);

		rc.DeflateRect(2, 0, 2, 0);

		if (pViewDay->GetCalendarControl()->m_bFullDateMode)
		{
			CString sFmt = pViewDay->GetView()->GetDayHeaderFormat();
			if (!sFmt.IsEmpty())
			{
				if (sFmt.Find(_T("MM")) > -1)
				{
					sFmt = _T("%a, %b %d");

					pViewDay->GetView()->SetDayHeaderFormat(sFmt);
					strText = pViewDay->GetDayDate().Format(sFmt);
				}
			}
		}

		DrawLine_CenterLR(pDC, strText, rc, DT_VCENTER);
	}
	else
	{
		COLORREF clrRestore = pDC->GetTextColor();

		CFont &font = GetTextFont();
		CFont *pFontToSet = NULL;

		if (bIsToday)
		{
			pDC->FillSolidRect(rc, RGB(153,200,233));

			pDC->SetTextColor(RGB(255, 255, 255));

			LOGFONT lf;
			font.GetLogFont(&lf);
			lf.lfWeight = FW_BOLD;

			pFontToSet = new CFont();
			pFontToSet->CreateFontIndirect(&lf);
		}

		const int nStringLength = strText.GetLength();

		CharUpper(strText.GetBuffer(nStringLength));

		strText.ReleaseBuffer();

		rc.DeflateRect(10, 0, 0, 0);

		CFont* pOldFont = pDC->SelectObject(pFontToSet ? pFontToSet : &font);
		pDC->DrawText(strText, rc, DT_VCENTER | DT_LEFT | DT_NOPREFIX | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);

		pDC->SetTextColor(clrRestore);

		if (pFontToSet)
		{
			pFontToSet->DeleteObject();

			delete pFontToSet;
		}
	}
}

void CXTPCalendarPaintManager::CDayViewAllDayEventScrollIconsPart::OnDraw(CDC* pDC, CXTPCalendarDayViewGroup* pViewGroup, BOOL bDrawUp, BOOL bDrawDown,
										const CRect& rcIconUp, const CRect& rcIconDown, BOOL bHighlightUp, BOOL bHighlightDown)
{
	COLORREF clrIconColor = RGB(0,0,0);
	COLORREF clrIconColorInactive = XTPDrawHelpers()->LightenColor(130, RGB(0,0,0));

	if(bDrawUp)
	{
		CPoint pt1(rcIconUp.left, rcIconUp.bottom), pt2(rcIconUp.left + rcIconUp.Width() / 2, rcIconUp.top), pt3(rcIconUp.right, rcIconUp.bottom);
		XTPDrawHelpers()->Triangle(pDC, pt1, pt2, pt3, bHighlightUp ? clrIconColor : clrIconColorInactive);
	}

	if(bDrawDown)
	{
		CPoint pt1(rcIconDown.left, rcIconDown.top), pt2(rcIconDown.left + rcIconDown.Width() / 2, rcIconDown.bottom), pt3(rcIconDown.right, rcIconDown.top);
		XTPDrawHelpers()->Triangle(pDC, pt1, pt2, pt3, bHighlightDown ? clrIconColor : clrIconColorInactive);
	}
}

void CXTPCalendarPaintManager::CDayViewGroupHeaderPart::OnDraw(CDC* pDC,
				CXTPCalendarDayViewGroup* pViewGroup, CRect rc, CString strText)
{
	if (!pDC || !pViewGroup || !pViewGroup->GetViewDay() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	pDC->FillSolidRect(rc, GetBackgroundColor());

	CRect rcHeader(rc);

	m_pPaintManager->DrawHeader(pDC, rcHeader, FALSE, FALSE);

	CXTPCalendarDayViewDay* pViewDay = pViewGroup->GetViewDay();

	BOOL bFirstGroup = pViewDay->GetDayIndex() == 0 &&
		pViewGroup->GetGroupIndex() == 0;
	BOOL bLastGroup = pViewDay->GetDayIndex() == pViewDay->GetView()->GetViewDayCount() - 1 &&
		pViewGroup->GetGroupIndex() == pViewDay->GetViewGroupsCount() - 1;          ;

	pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, m_clrTopLeftBorder);
	pDC->FillSolidRect(rc.left, rc.top + (bFirstGroup ? 0: 2), 1, rc.Height() -  (bFirstGroup ? 0 : 3), m_clrTopLeftBorder);
	pDC->FillSolidRect(rc.right - 1, rc.top + (bLastGroup ? 0: 2), 1, rc.Height() - (bLastGroup ? 0 : 3), m_pPaintManager->m_clr3DShadow);

	rc.DeflateRect(2, 0, 2, 0);

	// Colors for schedules
	if (pViewDay->GetCalendarControl()->m_bMultiColorScheduleMode)
	{
		int ind = pViewGroup->GetGroupIndex();
		COLORREF clr = pDC->GetTextColor();
		if (ind == 1)
			SetTextColor(RGB(255, 0, 0));
		else if (ind == 2)
			SetTextColor(RGB(0, 0, 255));
		else if (ind == 3)
			SetTextColor(RGB(0, 255, 255));
		else
			SetTextColor(RGB((GetRValue(clr) + 64 * ind) % 255, (GetGValue(clr) + 64 * ind) % 255, (GetBValue(clr) + 64 * ind) % 255));
	}

	DrawLine_CenterLR(pDC, strText, rc, DT_VCENTER);
}

void CXTPCalendarPaintManager::DrawHeader(CDC* pDC, CRect& rcHeader,
		BOOL bIsSelected, BOOL bIsCurrent)
{
	DrawHeader(pDC, rcHeader, bIsSelected, bIsCurrent, m_grclrToday, m_clrUnderLineHdr);
}

void CXTPCalendarPaintManager::DrawHeader(CDC* pDC, CRect& rcHeader,
		BOOL bIsSelected, BOOL bIsCurrent,
		const CXTPPaintManagerColorGradient& grclr, COLORREF clrUnderLineHdr)
{
	ASSERT(pDC);
	if (!pDC)
	{
		return;
	}

	if (!bIsSelected)
		pDC->FillSolidRect(rcHeader, m_clrButtonFace);

	if (bIsCurrent)
	{
		if (grclr.clrDark != grclr.clrLight)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rcHeader, grclr, FALSE);
		}
		else
		{
			pDC->FillSolidRect(rcHeader, grclr.clrDark);
		}
		pDC->FillSolidRect(rcHeader.left, rcHeader.bottom -1 ,
			rcHeader.Width(), 1, clrUnderLineHdr);
	}

	// Office 2013 doesn't draw background of selected cell header
	if (bIsSelected && GetPaintTheme() != xtpCalendarThemeOffice2013)
	{
		pDC->FillSolidRect(rcHeader, m_clrHighlight);
	}
}

void CXTPCalendarPaintManager::CWeekViewPart::RefreshMetrics()
{
	m_clrTextColor.SetStandardValue(m_pPaintManager->m_clrButtonFaceText);
	m_clrTextHighLightColor = RGB(255, 255, 255);
	m_clrTextNormalColor = m_pPaintManager->m_clrButtonFaceText;
	m_clrHeaderBottomLine = m_pPaintManager->m_clr3DShadow;

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	m_fntText.SetStandardValue(&lfIcon);

	if (m_pPaintManager->GetPaintTheme() <= xtpCalendarThemeOffice2000 ||
		m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOfficeXP)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 255));
	}
	else if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2003)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 213));
	}
	else if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground = RGB(255, 255, 255);
		m_clrTextNormalColor = RGB(38, 38, 38);
		m_clrTextColor.SetStandardValue(RGB(38, 38, 38));
		m_clrTextHighLightColor = RGB(38, 38, 38);
		m_clrHeaderBottomLine = RGB(225, 225, 225);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CXTPCalendarPaintManager::CWeekViewPart::DrawDayHeader(CDC* pDC,
						CRect rcDay, int nHeaderHeight,
						CString strHeader, BOOL bIsCurrent, BOOL bIsSelected)
{
	if (!pDC || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2003)
		bIsCurrent = FALSE;

	CRect rcHeader(rcDay);
	rcHeader.DeflateRect(2, 2, 1, rcHeader.Height() - nHeaderHeight - 1);
	CRect rcText(rcHeader);
	rcText.DeflateRect(1, 0, 3, 1);

	CPen outerPen;
	outerPen.CreatePen(PS_SOLID, 1, m_clrHeaderBottomLine);
	CPen* pOldPen = pDC->SelectObject(&outerPen);

	// draw header top line
	m_pPaintManager->DrawHorizontalLine(pDC, rcDay.left + 1, rcDay.top, rcDay.Width()+1);

	pDC->SelectObject(pOldPen);

	// draw header contents
	CRect rcHeader2(rcHeader);
	rcHeader2.bottom -= 1;

	if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2013)
	{
		if (bIsCurrent && bIsSelected)
		{
			m_pPaintManager->DrawHeader(pDC, rcHeader2, FALSE, bIsCurrent);
			CRect rcSel = CRect(rcHeader2);
			CSize  szText = GetTextExtent(pDC, strHeader);
			rcSel.left = rcSel.right - (szText.cx + 7);
			rcSel.bottom--;
			m_pPaintManager->DrawHeader(pDC, rcSel, bIsSelected, FALSE);
		}
		else
		{
			m_pPaintManager->DrawHeader(pDC, rcHeader2, bIsSelected, bIsCurrent);
		}
	}
	else
	{
		pDC->FillSolidRect(rcHeader2, bIsSelected ? RGB(177, 214, 240) : RGB(225,242,250) );
	}

	// draw header bottom line
	if (!bIsCurrent)
	{
		CRect rcBottomLine(rcHeader);
		rcBottomLine.top = --rcBottomLine.bottom - 1;
		if (!bIsSelected)
			rcBottomLine.DeflateRect(2, 0, 2, 0);
		pDC->FillSolidRect(rcBottomLine, m_clrHeaderBottomLine);
	}

	// draw header text
	pDC->SetBkMode(TRANSPARENT);
	SetTextColor(bIsSelected ? m_clrTextHighLightColor : m_clrTextNormalColor);
	DrawText(pDC, strHeader, rcText, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
}

void CXTPCalendarPaintManager::CWeekViewPart::OnDraw(CDC* pDC, CXTPCalendarWeekView* pWeekView)
{
	//return;

	if (!pDC || !pWeekView || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	CRect rcView(pWeekView->GetViewRect());

	pDC->FillSolidRect(rcView, m_clrBackground);

	//draw grid
	int nDayWidth = pWeekView->GetDayWidth();

	COLORREF clrGridColor = m_clrHeaderBottomLine;

	CPen outerPen;
	outerPen.CreatePen(PS_SOLID, 1, clrGridColor);
	CPen* pOldPen = pDC->SelectObject(&outerPen);

	m_pPaintManager->DrawVerticalLine(pDC, rcView.left + nDayWidth - 1, rcView.top, rcView.bottom);

	pDC->SelectObject(pOldPen);

	// draw headers
	BOOL bIsSelected = FALSE;
	BOOL bIsCurrent = FALSE;
	COleDateTime dtCurrent = CXTPCalendarUtils::ResetTime(COleDateTime::GetCurrentTime());

	COleDateTime dtDay;

	int nDayHeaderHeight = pWeekView->GetDayHeaderHeight();
	for (int nDay = 0; nDay < 7; nDay++)
	{
		CXTPCalendarWeekViewDay* pWDay = pWeekView->GetViewDay(nDay);
		ASSERT(pWDay);
		if (!pWDay)
		{
			continue;
		}
		CString strDate = pWDay->GetCaption();

		dtDay = pWDay->GetDayDate();
		bIsSelected = pWDay->IsSelected();
		bIsCurrent = CXTPCalendarUtils::IsEqual(dtDay, dtCurrent);


		//-------------------------------------------------------------

		DrawDayHeader(pDC, pWDay->GetDayRect(), nDayHeaderHeight, strDate, bIsCurrent, bIsSelected);
	}
}
void CXTPCalendarPaintManager::CWeekViewEventPart::DrawBorder(CDC* pDC,
						CRect rcView, CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	if (!pDC || !pWeekViewEvent || !pWeekViewEvent->GetEvent() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	int nBorderWidth = pWeekViewEvent->IsSelected() ? 2 : 1;

	int nMDEFlags = pWeekViewEvent->GetMultiDayEventFlags();
	BOOL bAllDay = pWeekViewEvent->GetEvent()->IsAllDayEvent();

	if (pWeekViewEvent->IsSelected() &&
			(nMDEFlags & xtpCalendarMultiDayFirst ||
			nMDEFlags & xtpCalendarMultiDayLast ||
			nMDEFlags & xtpCalendarMultiDayMiddle))
	{
		rcView.top--;
		rcView.bottom++;
	}

	if (bAllDay ||
			nMDEFlags & xtpCalendarMultiDayFirst ||
			nMDEFlags & xtpCalendarMultiDayLast ||
			nMDEFlags & xtpCalendarMultiDayMiddle)
	{
		pDC->FillSolidRect(rcView.left, rcView.top , rcView.Width(), nBorderWidth, m_clrTextNormalColor);
		pDC->FillSolidRect(rcView.left, rcView.bottom - nBorderWidth, rcView.Width(), nBorderWidth, m_clrTextNormalColor);
	}
	if (nMDEFlags & xtpCalendarMultiDayFirst)
	{
		pDC->FillSolidRect(rcView.left, rcView.top, 1, rcView.Height(), m_clrTextNormalColor);
	}
	if (nMDEFlags & xtpCalendarMultiDayLast)
	{
		pDC->FillSolidRect(rcView.right, rcView.top, 1, rcView.Height(), m_clrTextNormalColor);
	}
}

int CXTPCalendarPaintManager::CWeekViewEventPart::DrawTimes(CDC* pDC, CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	if (!pDC || !pWeekViewEvent || !pWeekViewEvent->GetEvent() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return 0;
	}

	DWORD dwAdditionalOpt = XTP_SAFE_GET3(pWeekViewEvent, GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
	BOOL bShowStartTime = dwAdditionalOpt & xtpCalendarOptWeekViewShowStartTimeAlways;
	BOOL bShowEndTime = dwAdditionalOpt & xtpCalendarOptWeekViewShowEndTimeAlways;

	COLORREF clrBackground = pWeekViewEvent->IsSelected() ? m_pPaintManager->m_clrHighlight : m_clrBackground;

	CXTPCalendarEventLabel* ptrLabel = pWeekViewEvent->GetEvent()->GetLabel();
	COLORREF clrLabel = ptrLabel ? ptrLabel->m_clrColor : clrBackground;
	clrBackground = clrLabel;

	CRect rcStart(pWeekViewEvent->GetStartTimeRect());
	CRect rcEnd(pWeekViewEvent->GetEndTimeRect());
	COleDateTime dtStart(pWeekViewEvent->GetEvent()->GetStartTime());

	if (pWeekViewEvent->IsTimeAsClock())
	{
		if (bShowStartTime)
			m_pPaintManager->DrawClock(pDC, dtStart, rcStart, clrBackground, xtpCalendarClockAlignCenter);
	}
	else
	{
		CString strStartTime = pWeekViewEvent->GetItemTextStartTime();
		if (bShowStartTime)
			DrawLineEx(pDC, strStartTime, &rcStart, DT_RIGHT | DT_VCENTER, DT_LEFT | DT_VCENTER);
	}

	if (pWeekViewEvent->IsShowEndTime())
	{
		COleDateTime dtEnd(pWeekViewEvent->GetEvent()->GetEndTime());
		if (pWeekViewEvent->IsTimeAsClock())
		{
			if (bShowEndTime)
				m_pPaintManager->DrawClock(pDC, dtEnd, rcEnd, clrBackground, xtpCalendarClockAlignCenter);
		}
		else
		{
			CString strEndTime = pWeekViewEvent->GetItemTextEndTime();
			if (bShowEndTime)
				DrawLineEx(pDC, strEndTime, &rcEnd, DT_RIGHT | DT_VCENTER, DT_LEFT | DT_VCENTER);
		}
	}
	return 0;
}

void CXTPCalendarPaintManager::CWeekViewEventPart::DrawSubj(CDC* pDC,
									CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	if (!pDC || !pWeekViewEvent || !pWeekViewEvent->GetEvent() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	CRect rcText(pWeekViewEvent->GetTextRect());
	CString strLoc = pWeekViewEvent->GetItemTextLocation();
	CString strEventText = pWeekViewEvent->GetItemTextSubject();
	if (strLoc.GetLength() > 0)
	{
		strEventText += _T(" (") + strLoc + _T(")");
	}

	int nDayEventFlags = pWeekViewEvent->GetMultiDayEventFlags();
	int nAlign = DT_CENTER;
	if (pWeekViewEvent->IsMultyDayTextAlign() || GetTextExtent(pDC, strEventText).cx >= rcText.Width())
		nAlign = DT_LEFT;

	//nAlign
	if (pWeekViewEvent->GetEvent()->IsAllDayEvent() ||
			nDayEventFlags & xtpCalendarMultiDayFirst ||
			nDayEventFlags & xtpCalendarMultiDayMiddle ||
			nDayEventFlags & xtpCalendarMultiDayLast)
	{
		DrawText(pDC, strEventText, &rcText, nAlign | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		DrawText(pDC, strEventText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
}

void CXTPCalendarPaintManager::CWeekViewEventPart::OnDraw(CDC* pDC,
									CXTPCalendarWeekViewEvent* pWeekViewEvent)
{
	if (!pDC || !pWeekViewEvent || !pWeekViewEvent->GetEvent() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	CRect rcView(pWeekViewEvent->GetViewEventRect());
	CRect rcViewMax(rcView);
	CString strEventText;

	BOOL bSelected = pWeekViewEvent->IsSelected();
	int nDayEventFlags = pWeekViewEvent->GetMultiDayEventFlags();
	BOOL bMday = (nDayEventFlags & xtpCalendarMultiDayFMLmask) != 0;
	BOOL bAllDay = pWeekViewEvent->GetEvent()->IsAllDayEvent();

	CXTPCalendarEventLabel* ptrLabel = pWeekViewEvent->GetEvent()->GetLabel();
	BOOL bLabel = (ptrLabel && ptrLabel->m_nLabelID != XTP_CALENDAR_NONE_LABEL_ID);

	COLORREF clrBackground = m_clrBackground;
	SetTextColor(m_clrTextNormalColor);
	if (bMday || bAllDay)
		clrBackground = RGB(255, 255, 255);
	if (bLabel && ptrLabel)
		clrBackground = ptrLabel->m_clrColor;
	if (bSelected)
	{
		rcViewMax.CopyRect(pWeekViewEvent->GetViewEventRectMax());
		clrBackground = m_pPaintManager->m_clrHighlight;
		SetTextColor(m_clrTextHighLightColor);
	}

	if (bMday || bAllDay)
		pDC->FillSolidRect(rcView, clrBackground);
	else
		pDC->FillSolidRect(rcViewMax, clrBackground);

	// Draw border for long event
	DrawBorder(pDC, rcView, pWeekViewEvent);

	// Draw Clock for Long Events
	if (nDayEventFlags & xtpCalendarMultiDayFirst)
	{
		m_pPaintManager->DrawClock(pDC,
			pWeekViewEvent->GetEvent()->GetStartTime(),
			pWeekViewEvent->GetFirstClockRect(),
			clrBackground,
			xtpCalendarClockAlignLeft);
	}
	else if (nDayEventFlags & xtpCalendarMultiDayLast)
	{
		m_pPaintManager->DrawClock(pDC,
			pWeekViewEvent->GetEvent()->GetEndTime(),
			pWeekViewEvent->GetLastClockRect(),
			clrBackground,
			xtpCalendarClockAlignRight);
	}
	// 3 Draw time start/end and icons
	clrBackground = bSelected ? m_pPaintManager->m_clrHighlight : m_clrBackground;
	if (!bMday && !bAllDay)
		DrawTimes(pDC, pWeekViewEvent);

	m_pPaintManager->DrawIcons(pDC, pWeekViewEvent);

	// 5 Draw Text
	DrawSubj(pDC, pWeekViewEvent);
}

void CXTPCalendarPaintManager::CMonthViewGridPart::OnDrawGrid(CDC* pDC,
											CXTPCalendarMonthView* pMonthView)
{
	if (!pDC || !pMonthView || !pMonthView->GetGrid() ||
		!pMonthView->GetCalendarControl() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	COLORREF clrGridLine = RGB(0, 0, 0);
	BOOL bCompressWeD = pMonthView->GetCalendarControl()->MonthView_IsCompressWeekendDays();
	int nWeeksCount = pMonthView->GetGrid()->GetWeeksCount();

	if (pMonthView->GetCalendarControl()->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		clrGridLine = RGB(225, 225, 225);
	}

	COleDateTime dtNow = CXTPCalendarUtils::GetCurrentTime();
	int nCurrentMonth = dtNow.GetMonth();
	int nWhiteMonthColorIdx = nCurrentMonth % 2;

	for (int nWeek = 0; nWeek < nWeeksCount; nWeek++)
	{
		for (int nDayIdx = 0; nDayIdx < 7; nDayIdx++)
		{
			COleDateTime dtDay = pMonthView->GetGrid()->GetDayDate(nWeek, nDayIdx);
			CRect rcB = pMonthView->GetGrid()->GetDayRect(nWeek, nDayIdx, TRUE);

			//Draw Grid lines
			if (nWeek > 0)
			{
				pDC->FillSolidRect(rcB.left, rcB.top, rcB.Width(), 1, clrGridLine);
			}
			if (nWeek < nWeeksCount - 1)
			{
				pDC->FillSolidRect(rcB.left, rcB.bottom, rcB.Width(), 1, clrGridLine);
			}
			int nWDay = dtDay.GetDayOfWeek();
			BOOL bCompressedCol_Su = bCompressWeD && nDayIdx == 1 && nWDay == 1;
			if (nDayIdx > 0 && !bCompressedCol_Su)
			{
				pDC->FillSolidRect(rcB.left, rcB.top, 1, rcB.Height(), clrGridLine);
			}

			//Fill Grid cells background
			CRect rcDay = pMonthView->GetGrid()->GetDayRect(nWeek, nDayIdx, FALSE);

			int nMonth = dtDay.GetMonth();
			COLORREF clrBackground = nMonth % 2 == nWhiteMonthColorIdx ? m_clrBackground : m_clrBackground2;
			if (nDayIdx == 0)
			{
				rcDay.left--;
			}

			// disable changing of default background in Office2013
			if (pMonthView->GetCalendarControl()->GetPaintTheme() == xtpCalendarThemeOffice2013 )
			{
				clrBackground = m_clrBackground;
			}

			// Show Discrete Selection in non Office 2007 mode

			UINT uDay = (UINT) dtDay;
			BOOL bDiscreteSelected = pMonthView->IsInDiscreteSelection(uDay);

			if (bDiscreteSelected)
			{
				if (clrBackground == m_clrBackground)
					clrBackground = m_clrBackground2;
				else
					clrBackground = m_clrBackground;
			}
			else
			{
				// Show continue selection in non Office 2007 mode
				COleDateTime dBeg, dEnd;
				pMonthView->GetSelection(&dBeg, &dEnd);
				//if (dtDay >= dBeg && dtDay <= dEnd)
				if (dtDay >= dBeg && dtDay < dEnd)
				{
					if (clrBackground == m_clrBackground)
						clrBackground = m_clrBackground2;
					else
						clrBackground = m_clrBackground;
				}
			}
			pDC->FillSolidRect(&rcDay, clrBackground);
		}
	}
}

void CXTPCalendarPaintManager::CMonthViewHeaderPart::RefreshMetrics()
{
	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground = RGB(225,242,250);
	}

}

void CXTPCalendarPaintManager::CMonthViewHeaderPart::OnDraw(CDC* pDC,
									CXTPCalendarMonthView* pMonthView, CRect rc,
									int nCollIndex, CString strText)
{
	if (!pDC || !pMonthView || !pMonthView->GetGrid() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	pDC->FillSolidRect(rc, GetBackgroundColor());

	if (pMonthView->GetPaintManager()->GetPaintTheme() != xtpCalendarThemeOffice2013)
	{
		BOOL bFirstDay = nCollIndex == 0;
		BOOL bLastDay = nCollIndex == pMonthView->GetGrid()->GetColsCount() - 1;

		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, m_clrTopLeftBorder);

		pDC->FillSolidRect(rc.left, rc.top + (bFirstDay ? 0: 2), 1, rc.Height() -  (bFirstDay ? 1 : 3), m_clrTopLeftBorder);
		pDC->FillSolidRect(rc.right - 1, rc.top + (bLastDay ? 0: 2), 1, rc.Height() - (bLastDay ? 1 : 3), m_pPaintManager->m_clr3DShadow);

		pDC->FillSolidRect(rc.left, rc.bottom, rc.Width(), 1, m_pPaintManager->m_clr3DShadow);

		rc.DeflateRect(2, 0, 2, 0);

		DrawLine_CenterLR(pDC, strText, rc, DT_VCENTER);
	}
	else
	{
		const int nStringLength = strText.GetLength();

		CharUpper(strText.GetBuffer(nStringLength));

		strText.ReleaseBuffer();

		rc.DeflateRect(10, 0, 0, 0);

		DrawText(pDC, strText, rc, DT_VCENTER | DT_LEFT | DT_NOPREFIX | DT_SINGLELINE);
	}
}

void CXTPCalendarPaintManager::CMonthViewEventPart::RefreshMetrics()
{
	m_clrTextHighLightColor = RGB(255, 255, 255);
	m_clrMultiDayEventFrameColor = RGB(0, 0, 0);
	m_clrBackground.SetStandardValue(RGB(255, 255, 213));
	m_grclrClassicSelDay.SetStandardValue(m_clrBackground, RGB(217, 214, 202));
	m_clrClassicUnderline = RGB(10, 36, 106);

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{

	}
}

void CXTPCalendarPaintManager::CMonthViewEventPart::OnDrawDayDate(CDC* pDC,
											CRect rc, BOOL bToday,
											BOOL bSelected, BOOL bFirstMonthDay, CString strText)
{
	if (!pDC || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

	rc.DeflateRect(1, 1, 1, 1);

	BOOL bXPTheme = m_pPaintManager->GetCurrentSystemTheme() != xtpSystemThemeUnknown;

	const BOOL bToday2 = bToday;

	if (m_pPaintManager->GetPaintTheme() != xtpCalendarThemeOffice2003)
	{
		bXPTheme = bToday = FALSE;
	}

	if (bSelected && bToday)
	{
		CRect rcSel = CRect(rc);
		CRect rcUnSel = CRect(rc);
		CSize  szText = GetTextExtent(pDC, strText);
		rcSel.left = rcSel.right - (szText.cx + 7);
		rcUnSel.right = rcUnSel.left + rcUnSel.Width() - (szText.cx + 7);
		if (bXPTheme)
			m_pPaintManager->DrawHeader(pDC, rcUnSel, FALSE, bToday);
		else
			m_pPaintManager->DrawHeader(pDC, rcUnSel, FALSE, bToday,
										m_grclrClassicSelDay, m_clrClassicUnderline);

		m_pPaintManager->DrawHeader(pDC, rcSel, bSelected, FALSE);
	}
	else
	if (bSelected || bToday)
	{
		if (bXPTheme)
			m_pPaintManager->DrawHeader(pDC, rc, bSelected, bToday);
		else
			m_pPaintManager->DrawHeader(pDC, rc, bSelected, bToday,
										m_grclrClassicSelDay, m_clrClassicUnderline);

	}

	UINT dtTextFormat = DT_VCENTER | DT_SINGLELINE;

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		dtTextFormat |= DT_LEFT;
		rc.DeflateRect(5, 0, 0, 0);
	}
	else
	{
		dtTextFormat |= DT_RIGHT;

		rc.DeflateRect(0, 0, 5, 0);
	}

	COLORREF clrOldTextColor = pDC->GetTextColor();

	if (bSelected)
	{
		pDC->SetTextColor(m_clrTextHighLightColor);
	}

	CFont &textFont = GetTextFont();
	LOGFONT lfText;
	textFont.GetLogFont(&lfText);

	if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		if (bFirstMonthDay)
			lfText.lfWeight = FW_BOLD;

		if (bToday2)
		{
			lfText.lfWeight = FW_BOLD;

			if (!bSelected)
				pDC->SetTextColor(RGB(153,200,233));
		}

		// increase font size for Office 2013

		// lfText.lfHeight--;

		int nPointSize = ::MulDiv(-lfText.lfHeight, 72, GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY));

		lfText.lfHeight = -MulDiv(nPointSize + 1, GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 72);
	}

	CFont *pNewFont = CFont::FromHandle(::CreateFontIndirect(&lfText));

	CXTPFontDC fnt(pDC, pNewFont);
	pDC->DrawText(strText, rc, dtTextFormat);

	m_clrTextColor.SetDefaultValue();

	pNewFont->DeleteObject();

	pDC->SetTextColor(clrOldTextColor);
}

void CXTPCalendarPaintManager::CMonthViewEventPart::OnDrawEvent(CDC* pDC,
		CXTPCalendarMonthViewEvent* pViewEvent)
{
	if (!pDC || !pViewEvent || !pViewEvent->GetEvent() || !m_pPaintManager)
	{
		ASSERT(FALSE);
		return;
	}

//Start - End options
	DWORD dwAdditionalOpt = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
	BOOL bShowEndTime = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), MonthView_IsShowEndDate(), TRUE);
	BOOL bHideTimes = XTP_SAFE_GET2(m_pPaintManager->m_pControl, GetCalendarOptions(), bMonthView_HideTimes, FALSE);
	BOOL bDrawStartTime = (dwAdditionalOpt & xtpCalendarOptMonthViewShowStartTimeAlways) && !bHideTimes;
	BOOL bDrawEndTime = (dwAdditionalOpt & xtpCalendarOptMonthViewShowEndTimeAlways) && !bHideTimes;

	CRect rcEvent = pViewEvent->GetEventRect();
	CRect rcText = pViewEvent->GetTextRect();
	CXTPCalendarEvent* prtEvent = pViewEvent->GetEvent();

	BOOL bSelected = pViewEvent->IsSelected();
	int nMDEFlags = pViewEvent->GetMultiDayEventFlags();

	CRect rcClockFirst(0, 0, 0, 0);
	CRect rcClockLast(0, 0, 0, 0);
	COleDateTime dtClockFirst;
	COleDateTime dtClockLast;
	COLORREF clrBackground;
	COLORREF clrForeground = GetTextColor();
	if (nMDEFlags & xtpCalendarMultiDayFirst)
	{
		rcClockFirst.CopyRect(pViewEvent->GetFirstClockRect());
		dtClockFirst = pViewEvent->GetEvent()->GetStartTime();
	}
	if (nMDEFlags & xtpCalendarMultiDayLast)
	{
		rcClockLast.CopyRect(pViewEvent->GetLastClockRect());
		dtClockLast = pViewEvent->GetEvent()->GetEndTime();
	}

	if (bSelected)
	{
		if (nMDEFlags & xtpCalendarMultiDayFMLmask)
		{
			rcEvent.top--;
			rcEvent.bottom++;
		}
		else
		{
			rcEvent.left = pViewEvent->m_rcEventMax.left + 2;
			rcEvent.right = pViewEvent->m_rcEventMax.right - 2;
		}
		clrForeground = m_clrTextHighLightColor;
		clrBackground = m_pPaintManager->m_clrHighlight;
		pDC->FillSolidRect(&rcEvent, m_pPaintManager->m_clrHighlight);
		clrBackground = m_pPaintManager->m_clrHighlight;
	}
	else
	{
		CXTPCalendarEventLabel* ptrLabel = pViewEvent->GetEvent()->GetLabel();
		COLORREF clrLabel = ptrLabel ? ptrLabel->m_clrColor : RGB(255, 255, 255);

		BOOL bNoneLabel = ptrLabel ? (ptrLabel->m_nLabelID == XTP_CALENDAR_NONE_LABEL_ID) : TRUE;
		clrBackground = clrLabel;

		if (nMDEFlags & xtpCalendarMultiDayFMLmask || !bNoneLabel)
		{
			pDC->FillSolidRect(&rcEvent, clrLabel);
		}
	}

	UINT uAlign = DT_LEFT;

	CString strLoc = pViewEvent->GetItemTextLocation();
	CString strText = pViewEvent->GetItemTextSubject();
	if (strLoc.GetLength() > 0)
		//strText += _T(" (") + strLoc + _T(")");
		strText += _T(";"); //to have same separator in different themes

	if (nMDEFlags & xtpCalendarMultiDayFMLmask)
	{
		int nBY = bSelected ? 2 : 1;
		pDC->FillSolidRect(rcEvent.left, rcEvent.top, rcEvent.Width(), nBY, m_clrMultiDayEventFrameColor);
		pDC->FillSolidRect(rcEvent.left, rcEvent.bottom - nBY, rcEvent.Width(), nBY, m_clrMultiDayEventFrameColor);

		if (nMDEFlags & xtpCalendarMultiDayFirst)
		{
			pDC->FillSolidRect(rcEvent.left, rcEvent.top, 1, rcEvent.Height(), m_clrMultiDayEventFrameColor);
		}
		if (nMDEFlags & xtpCalendarMultiDayLast)
		{
			pDC->FillSolidRect(rcEvent.right, rcEvent.top, 1, rcEvent.Height(), m_clrMultiDayEventFrameColor);
		}

		//CSize szSpace = GetTextExtent(pDC, _T(" "));

		rcText.left += 1;//min(1, szSpace.cx/2);
		rcText.right -= 1;
		CSize szText = GetTextExtent(pDC, strText);

		if (pViewEvent->IsMultyDayTextAlign() || szText.cx > rcText.Width())
			uAlign = DT_LEFT;
		else
			uAlign = DT_CENTER;

	}

	// draw clocks for multyday events
	if ((nMDEFlags & xtpCalendarMultiDayFirst) || (nMDEFlags & xtpCalendarMultiDayLast))
	{
		if (rcClockFirst.Width() > 0)
			m_pPaintManager->DrawClock(
				pDC,
				dtClockFirst,
				rcClockFirst,
				clrBackground,
				xtpCalendarClockAlignLeft);

		if (rcClockLast.Width() > 0)
			m_pPaintManager->DrawClock(
				pDC,
				dtClockLast,
				rcClockLast,
				clrBackground,
				xtpCalendarClockAlignLeft);
	}

	//CRect rcText = pViewEvent->GetTextRect();
	if (nMDEFlags & xtpCalendarMultiDayNoMultiDay)
	{
		//IsTimeAsClock()
		COleDateTime dtStart(prtEvent->GetStartTime());
		COleDateTime dtEnd(prtEvent->GetEndTime());

		CString strStartTime = pViewEvent->GetItemTextStartTime();
		CString strEndTime = pViewEvent->GetItemTextEndTime();

		CRect rcStart(pViewEvent->GetStartTimeRect());
		CRect rcEnd(pViewEvent->GetEndTimeRect());

		if (pViewEvent->IsTimeAsClock())
		{
			if (bDrawStartTime)
				m_pPaintManager->DrawClock(pDC, dtStart, rcStart, clrBackground, xtpCalendarClockAlignCenter);
			else
				rcEnd.left -= rcStart.Width();
			if (bDrawEndTime && bShowEndTime)
				m_pPaintManager->DrawClock(pDC, dtEnd, rcEnd, clrBackground, xtpCalendarClockAlignCenter);
			else
				rcText.left -= rcEnd.Width();
		}
		else
		{
			if (bDrawStartTime)
				DrawLineEx(pDC, strStartTime, rcStart, DT_RIGHT | DT_VCENTER, DT_LEFT | DT_VCENTER );
			else
				rcEnd.left -= rcStart.Width();
			if (bDrawEndTime && bShowEndTime)
				DrawLineEx(pDC, strEndTime, rcEnd, DT_RIGHT | DT_VCENTER, DT_LEFT | DT_VCENTER );
			else
				rcText.left -= rcEnd.Width();
		}
	}

	rcText.left += pViewEvent->CalcIconsRect(rcText);

	COLORREF clrOldTextColor = pDC->SetTextColor(clrForeground);
	DrawText(pDC, strText, rcText, uAlign | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	pDC->SetTextColor(clrOldTextColor);

	m_pPaintManager->DrawIcons(pDC, pViewEvent);

	m_clrTextColor.SetDefaultValue();
}


/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarPaintManager

#define ADDVIEWPART(theClass, parent)\
	m_p##theClass = (C##theClass*)AddViewPart(new C##theClass(m_p##parent));



CXTPCalendarPaintManager::CXTPCalendarPaintManager()
{
	m_pGlyphsFont = NULL;

	m_szClockRect.cx = 15;
	m_szClockRect.cy = 15;

	m_szExpandSign.cx = m_szExpandSign.cy = 0;

	m_pControl = NULL;

	m_bEnableTheme = TRUE;

	m_dwAskItemTextFlags = 0;

	m_pControlPart = (CControlPart*)AddViewPart(new CControlPart());

	ADDVIEWPART(DayViewTimeScaleHeaderPart, ControlPart);
	ADDVIEWPART(DayViewTimeScaleCellPart, DayViewTimeScaleHeaderPart);
	ADDVIEWPART(DayViewCellPart, ControlPart);
	ADDVIEWPART(DayViewWorkCellPart, DayViewCellPart);
	ADDVIEWPART(DayViewNonworkCellPart, DayViewCellPart);
	ADDVIEWPART(DayViewHeaderPart, ControlPart);
	ADDVIEWPART(DayViewGroupHeaderPart, ControlPart);
	ADDVIEWPART(DayViewAllDayEventsPart, ControlPart);
	ADDVIEWPART(DayViewAllDayEventScrollIconsPart, ControlPart);
	ADDVIEWPART(DayViewEventPart, ControlPart);
	ADDVIEWPART(WeekViewPart, ControlPart);
	ADDVIEWPART(WeekViewEventPart, ControlPart);
	ADDVIEWPART(MonthViewGridPart, ControlPart);
	ADDVIEWPART(MonthViewHeaderPart, ControlPart);
	ADDVIEWPART(MonthViewEventPart, ControlPart);

	ADDVIEWPART(TimeLineViewPart, ControlPart);
	ADDVIEWPART(TimeLineViewTimeScalePart_Day, ControlPart);
	ADDVIEWPART(TimeLineViewTimeScalePart_Week, ControlPart);
	ADDVIEWPART(TimeLineViewTimeScalePart_Month, ControlPart);

	m_CurrSystemTheme = xtpSystemThemeUnknown;
	m_nPaintTheme = xtpCalendarThemeOffice2003;

	RefreshMetrics();
}

CXTPCalendarPaintManager::~CXTPCalendarPaintManager()
{
	while (!m_lstViewParts.IsEmpty())
	{
		delete m_lstViewParts.RemoveHead();
	}

	SAFE_DELETE(m_pGlyphsFont);
}

void CXTPCalendarPaintManager::UpdateEventLabelsDefaultColors()
{
	if (m_pControl)
	{
		COLORREF pclrDefaultLabelsColors[XTP_CALENDAR_EVENT_LABELS_COUNT] = {
			RGB(255, 255, 255), // XTP_IDS_CALENDAR_EVENT_LABEL_NONE
			RGB(255, 148, 132), // XTP_IDS_CALENDAR_EVENT_LABEL_IMPORTANT
			RGB(132, 156, 231), // XTP_IDS_CALENDAR_EVENT_LABEL_BUSINESS
			RGB(165, 222, 99), // XTP_IDS_CALENDAR_EVENT_LABEL_PERSONAL
			RGB(231, 231, 214), // XTP_IDS_CALENDAR_EVENT_LABEL_VACATION
			RGB(255, 181, 115), // XTP_IDS_CALENDAR_EVENT_LABEL_MUSTATTEND
			RGB(132, 239, 247), // XTP_IDS_CALENDAR_EVENT_LABEL_TRAVELREQUIRED
			RGB(214, 206, 132), // XTP_IDS_CALENDAR_EVENT_LABEL_NEEDSPREPARATION
			RGB(198, 165, 247), // XTP_IDS_CALENDAR_EVENT_LABEL_BIRTHDAY
			RGB(165, 206, 198), // XTP_IDS_CALENDAR_EVENT_LABEL_ANNIVERSARY
			RGB(255, 231, 115)  // XTP_IDS_CALENDAR_EVENT_LABEL_PHONECALL
		};

		if (GetPaintTheme() == xtpCalendarThemeOffice2013)
		{
			pclrDefaultLabelsColors[0] = RGB(205, 230, 247); // default label for an event (XTP_IDS_CALENDAR_EVENT_LABEL_NONE)
		}

		if (m_pControl)
			m_pControl->GetDataProvider()->UpdateEventLabels(pclrDefaultLabelsColors, XTP_CALENDAR_EVENT_LABELS_COUNT);
	}
}

void CXTPCalendarPaintManager::UpdateGlyphs()
{
	CBitmap bmpSignUp, bmpSignDown;
	CSize szSexpandSign = GetExpandSignSize();

	m_ilExpandSigns.DeleteImageList();
	m_ilGlyphs.DeleteImageList();

	VERIFY(m_ilExpandSigns.Create(szSexpandSign.cx, szSexpandSign.cy, ILC_COLOR24 | ILC_MASK, 0, 1));
	VERIFY(m_ilGlyphs.Create(16, 11, ILC_COLOR24 | ILC_MASK, 0, 1));

	if (m_nPaintTheme == xtpCalendarThemeOffice2013)
	{
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmpSignUp, XTP_IDB_CALENDAR_EXPANDSIGNUP_2013) );
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmpSignDown, XTP_IDB_CALENDAR_EXPANDSIGNDOWN_2013) );

		COLORREF clrMask = RGB(0, 0, 0);
		m_ilExpandSigns.Add(&bmpSignUp, clrMask);
		m_ilExpandSigns.Add(&bmpSignDown, clrMask);
	}
	else
	{
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmpSignUp, XTP_IDB_CALENDAR_EXPANDSIGNUP) );
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmpSignDown, XTP_IDB_CALENDAR_EXPANDSIGNDOWN) );

		COLORREF clrMask = RGB(255, 0, 255);
		m_ilExpandSigns.Add(&bmpSignUp, clrMask);
		m_ilExpandSigns.Add(&bmpSignDown, clrMask);
	}

	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_CALENDAR_EVENT_GLYPHS));
	m_ilGlyphs.Add(&bmp, RGB(255, 0, 255));
}

void CXTPCalendarPaintManager::RefreshMetrics()
{
	UpdateGlyphs();

	UpdateEventLabelsDefaultColors();

	RefreshXtremeColors(FALSE);

	m_clrButtonFace.SetStandardValue(GetSysColor(COLOR_BTNFACE));
	m_clrButtonFaceText.SetStandardValue(GetSysColor(COLOR_BTNTEXT));
	m_clrWindow.SetStandardValue(GetSysColor(COLOR_WINDOW));
	m_clrWindowText.SetStandardValue(GetSysColor(COLOR_WINDOWTEXT));
	m_clr3DShadow.SetStandardValue(GetSysColor(COLOR_3DSHADOW));
	m_clrHighlight.SetStandardValue(GetSysColor(COLOR_HIGHLIGHT));

	InitBusyStatusDefaultColors();

	POSITION pos = m_lstViewParts.GetHeadPosition();
	while (pos)
	{
		CXTPCalendarViewPart* pPart = m_lstViewParts.GetNext(pos);
		ASSERT(pPart);
		if (pPart)
		{
			pPart->RefreshMetrics();
		}
	}

	m_grclrToday.SetStandardValue(m_clrButtonFace, m_clrButtonFace);

	m_clrUnderLineHdr = m_clrHighlight;

	if (m_nPaintTheme >= xtpCalendarThemeOffice2003)
	{
		m_CurrSystemTheme = xtpSystemThemeUnknown;

		if (m_bEnableTheme && !XTPColorManager()->IsLunaColorsDisabled())
		{
			m_CurrSystemTheme = XTPColorManager()->GetCurrentSystemTheme();

			if (m_CurrSystemTheme != xtpSystemThemeUnknown)
			{
				m_grclrToday.SetStandardValue(m_clrButtonFace, RGB(250, 203, 91));
				m_clrUnderLineHdr = RGB(187, 85, 3);
			}
		}
	}

	if (m_nPaintTheme == xtpCalendarThemeOffice2013)
	{
		m_clrHighlight.SetStandardValue(RGB(153, 200, 233));
	}

	SAFE_DELETE(m_pGlyphsFont);

	BOOL bFontExists = XTPDrawHelpers()->FontExists(cszGlyphsFontName);
	if (bFontExists)
	{
		m_pGlyphsFont = new CFont();
		if (m_pGlyphsFont)
		{
			LOGFONT lfGlupth;
			::ZeroMemory(&lfGlupth, sizeof(lfGlupth));

			STRCPY_S(lfGlupth.lfFaceName, LF_FACESIZE, cszGlyphsFontName);
			lfGlupth.lfCharSet = SYMBOL_CHARSET;
			lfGlupth.lfHeight = -11;
			lfGlupth.lfWeight = FW_NORMAL;

			if (!m_pGlyphsFont->CreateFontIndirect(&lfGlupth))
			{
				SAFE_DELETE(m_pGlyphsFont);
			}
		}
	}

	if (!m_pGlyphsFont)
	{
		TRACE(_T("WARNING! Calendar PaintManeger: cannot create 'MS Outlook' font. Standard bitmaps are used. \n"));
	}
}

void CXTPCalendarPaintManager::CDayViewWorkCellPart::RefreshMetrics()
{
	if (m_pPaintManager->GetPaintTheme() <= xtpCalendarThemeOffice2000 ||
		m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOfficeXP)
	{
		if (m_pPaintManager->GetPaintTheme() <= xtpCalendarThemeOffice2000)
			m_clrBackground.SetStandardValue(RGB(255, 255, 255));
		else
			m_clrBackground.SetStandardValue(RGB(255, 255, 0));

		m_clrBackgroundSlave.SetStandardValue(RGB(255, 255, 255));
		m_clrShadow.SetStandardValue(RGB(128, 128, 128));
		m_clrHour.SetStandardValue(RGB(128, 128, 128));
	}
	else if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2003)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 213));
		m_clrShadow.SetStandardValue(RGB(243, 228, 177));
		m_clrHour.SetStandardValue(RGB(234, 208, 152));
	}
	else if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 255));
		m_clrShadow.SetStandardValue(RGB(225, 225, 225));
		m_clrHour.SetStandardValue(RGB(225, 225, 225));
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CXTPCalendarPaintManager::CDayViewNonworkCellPart::RefreshMetrics()
{
	if (m_pPaintManager->GetPaintTheme() <= xtpCalendarThemeOffice2000 ||
		m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOfficeXP)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 0));
		m_clrBackgroundSlave.SetStandardValue(RGB(128, 128, 128));

		if (m_pPaintManager->GetPaintTheme() <= xtpCalendarThemeOffice2000)
		{
			m_clrBackground.SetStandardValue(RGB(212, 208, 200));
			m_clrBackgroundSlave.SetStandardValue(RGB(212, 208, 200));
		}

		m_clrShadow.SetStandardValue(RGB(128, 128, 128));
		m_clrHour.SetStandardValue(RGB(128, 128, 128));
	}
	else if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2003)
	{
		m_clrBackground.SetStandardValue(RGB(255, 244, 188));
		m_clrBackgroundSlave.SetStandardValue(RGB(255, 244, 188));
		m_clrShadow.SetStandardValue(RGB(243, 228, 177));
		m_clrHour.SetStandardValue(RGB(234, 208, 152));
	}
	else if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground.SetStandardValue(RGB(240, 240, 240));
		m_clrBackgroundSlave.SetStandardValue(RGB(255, 244, 188)); // ??? TODO:
		m_clrShadow.SetStandardValue(RGB(225, 225, 225));
		m_clrHour.SetStandardValue(RGB(225, 225, 225));
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CXTPCalendarPaintManager::CMonthViewGridPart::RefreshMetrics()
{
	if (m_pPaintManager->GetPaintTheme() <= xtpCalendarThemeOffice2000 ||
		m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOfficeXP)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 255));
		m_clrBackground2.SetStandardValue(RGB(215, 211, 203));
	}
	else if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2003)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 213));
		m_clrBackground2.SetStandardValue(RGB(255, 244, 188));
	}
	else if (m_pPaintManager->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		m_clrBackground.SetStandardValue(RGB(255, 255, 255));
		m_clrBackground2.SetStandardValue(RGB(153,200,233));
	}
	else
	{
		ASSERT(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarViewPart* CXTPCalendarPaintManager::AddViewPart(CXTPCalendarViewPart* pPart)
{
	ASSERT(pPart);
	if (pPart)
	{
		m_lstViewParts.AddTail(pPart);
		pPart->m_pPaintManager = this;
	}

	return pPart;
}

void CXTPCalendarPaintManager::DrawLine(CDC* pDC, int x, int y, int cx, int cy)
{
	ASSERT(pDC);
	if (!pDC)
	{
		return;
	}

	pDC->MoveTo(x, y);
	pDC->LineTo(x + cx, y + cy);
}

void CXTPCalendarPaintManager::DrawHorizontalLine(CDC* pDC, int x, int y, int cx)
{
	DrawLine(pDC, x, y, cx, 0);
}
void CXTPCalendarPaintManager::DrawVerticalLine(CDC* pDC, int x, int y, int cy)
{
	DrawLine(pDC, x, y, 0, cy);
}

int CXTPCalendarPaintManager::DrawBitmap(CImageList* pImageList, CDC* pDC,
		CRect rcView, int iIcon, UINT uFlags)
{
	ASSERT(pImageList && pDC);
	if (!pImageList || !pDC)
	{
		return 0;
	}

	int nImageBound = rcView.left;

	if (rcView.left >= rcView.right - 2)
		return nImageBound;

	POINT ptIcon;
	IMAGEINFO imgInf;

	if (!pImageList->GetImageInfo(iIcon, &imgInf))
		return nImageBound;

	CSize szImage(imgInf.rcImage.right - imgInf.rcImage.left, imgInf.rcImage.bottom - imgInf.rcImage.top);
	CSize szView(rcView.Size());
	ptIcon.x = rcView.left;
	ptIcon.y = rcView.top;

	szImage.cx = min(szImage.cx, szView.cx);

	if (uFlags & DT_VCENTER)
	{
		ptIcon.y = rcView.top + (szView.cy - szImage.cy) /2 + 1;
	}

	IMAGELISTDRAWINDIRECT_S(pImageList, pDC, iIcon, ptIcon, szImage);

	return szImage.cx;
}

int CXTPCalendarPaintManager::DrawBitmap(UINT nIDResource, CDC* pDC, CRect rcBitmap)
{
	ASSERT(pDC);
	if (!pDC)
	{
		return 0;
	}

	CBitmap bmpIcon;

	if (!XTPResourceManager()->LoadBitmap(&bmpIcon, nIDResource))
		return 0;

	BITMAP bmpInfo;
	bmpIcon.GetBitmap(&bmpInfo);

	CImageList ilBitmap;
	VERIFY(ilBitmap.Create(bmpInfo.bmWidth, bmpInfo.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 1));
	ilBitmap.Add(&bmpIcon, RGB(255, 0 , 255));

	CRect rcBitmap2 = rcBitmap;
	rcBitmap2.bottom = rcBitmap2.top + bmpInfo.bmHeight;

	DrawBitmap(&ilBitmap, pDC, rcBitmap2, 0);

	return bmpInfo.bmWidth;
}

CSize CXTPCalendarPaintManager::GetBitmapSize(UINT nIDResource)
{
	CBitmap bmpIcon;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmpIcon, nIDResource));

	BITMAP bmpInfo;
	bmpIcon.GetBitmap(&bmpInfo);
	CSize szIcon(bmpInfo.bmWidth, bmpInfo.bmHeight);
	return szIcon;
}

const CSize CXTPCalendarPaintManager::GetExpandSignSize()
{
	// if sign size is not initialized, initialize it
	if (!(m_szExpandSign.cx | m_szExpandSign.cy))
	{
		m_szExpandSign = GetBitmapSize(XTP_IDB_CALENDAR_EXPANDSIGNDOWN);
	}
	return m_szExpandSign;
}

int CXTPCalendarPaintManager::DrawClock(CDC* pDC, COleDateTime dtClockTime,
	CRect rcView, COLORREF clrBackground, XTPCalendarClockAlignFlags cafAlign)
{
	ASSERT(pDC);
	if (!pDC)
	{
		return 0;
	}

	static const CPoint arrHours[] =
		{
			CPoint(0, -6),
			CPoint(3, -5),
			CPoint(5, -3),
			CPoint(6, 0),
			CPoint(5, 3),
			CPoint(3, 5),
			CPoint(0, 6),
			CPoint(-3, 5),
			CPoint(-5, 3),
			CPoint(-6, 0),
			CPoint(-5, -3),
			CPoint(-3, -5),
			CPoint(0, -6)
		};

	static const CPoint arrHalfHours[] =
		{
			CPoint(1, -5),
			CPoint(3, -3),
			CPoint(4, -1),
			CPoint(5, 1),
			CPoint(3, 3),
			CPoint(1, 5),
			CPoint(-1, 5),
			CPoint(-3, 3),
			CPoint(-5, 1),
			CPoint(-5, -1),
			CPoint(-3, -3),
			CPoint(-1, -5),
			CPoint(1, -5)
		};

	if (rcView.Width() < m_szClockRect.cx || rcView.Height() < m_szClockRect.cy)
		return 0;

	CPoint ptClock(rcView.left, rcView.top);

	if (cafAlign == xtpCalendarClockAlignLeft)
		ptClock.Offset(1, (rcView.Height() - m_szClockRect.cy)/2 + 1);
	else if (cafAlign == xtpCalendarClockAlignRight)
		ptClock.Offset(rcView.Width() - m_szClockRect.cx - 1,
			(rcView.Height() - m_szClockRect.cy)/2 + 1);
	else if (cafAlign == xtpCalendarClockAlignCenter)
		ptClock.Offset((rcView.Width() - m_szClockRect.cx)/2,
			(rcView.Height() - m_szClockRect.cy)/2 + 1);

	CRect rcClock(0, 0, 0, 0);
	rcClock.SetRect(ptClock.x, ptClock.y, ptClock.x + m_szClockRect.cx, ptClock.y + m_szClockRect.cy);

	int nHour = dtClockTime.GetHour();
	int nMin = dtClockTime.GetMinute() /5;

	COLORREF clrFill;
	COLORREF clrItems;
	if (nHour >= 12)
	{
		clrFill = RGB(0, 0, 0);
		clrItems = clrBackground;
	}
	else
	{
		clrFill = clrBackground;
		clrItems = RGB(0, 0, 0);
	}


	nHour = nHour > 12 ? nHour - 12 : nHour;

	CXTPBrushDC brushClock(pDC->m_hDC, clrFill);
	CPoint ptCenter(rcClock.CenterPoint());
	pDC->Ellipse(rcClock);
	CXTPPenDC penItems(pDC->m_hDC, clrItems);
	int x = 0;
	int y = 0;

	for (int i = 0; i <= 12; i++)
	{
		pDC->SetPixel(ptCenter.x + arrHours[i].x, ptCenter.y + arrHours[i].y, clrItems);

		if (nHour == i)
		{
			if (nMin >= 6)
			{
				x = arrHalfHours[i].x;
				y = arrHalfHours[i].y;
			}
			else
			{
				x = arrHours[i].x;
				y = arrHours[i].y;
				x = x > 0 ? x -1 : x == 0 ? 0 : x + 1;
				y = y > 0 ? y -1 : y == 0 ? 0 : y + 1;
			}
			pDC->MoveTo(ptCenter);
			pDC->LineTo(ptCenter.x+ x, ptCenter.y + y);
		}
		if (nMin == i)
		{
			pDC->MoveTo(ptCenter);
			pDC->LineTo(ptCenter.x + arrHours[i].x,
						ptCenter.y + arrHours[i].y);
		}
	}

	return m_szClockRect.cx;

}

void CXTPCalendarPaintManager::SetControl(CXTPCalendarControl* pControl)
{
	m_pControl = pControl;
}

int CXTPCalendarPaintManager::DrawIconChar(CDC* pDC, CRect rc, TCHAR ch)
{
	if (!pDC || !m_pGlyphsFont || !m_pGlyphsFont->m_hObject)
	{
		ASSERT(FALSE);
		return 0;
	}

	CFont* pPrevFnt = pDC->SelectObject(m_pGlyphsFont);
	pDC->SetTextColor(GetControlPart()->GetTextColor());
	TCHAR cszCh[2] = {ch, 0};

	UINT uFlags = DT_VCENTER | DT_LEFT; // DT_CENTER;

	//CSize szChar = pDC->GetTextExtent(szCh, 1);
	CRect rcNeed = rc;
	pDC->DrawText(cszCh, 1, rcNeed, uFlags | DT_CALCRECT);
	rc.bottom = rc.top + rcNeed.Height();

	pDC->DrawText(cszCh, 1, rc, uFlags);

	pDC->SelectObject(pPrevFnt);

	//return szChar.cx;
	return rcNeed.Width();
}

int CXTPCalendarPaintManager::DrawIcons(CDC* pDC, CXTPCalendarViewEvent* pViewEvent)
{
	if (!pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return 0;
	}


	BOOL bUseFont = XTP_SAFE_GET3(pViewEvent, GetCalendarControl(),
						GetCalendarOptions(), bUseOutlookFontGlyphs, FALSE);
	if (m_pGlyphsFont == NULL)
		bUseFont = FALSE;

	int nWidth = 0;

	if (pViewEvent->GetEvent()->IsReminder() &&
		!pViewEvent->GetReminderGlyphRect().IsRectEmpty())
	{
		if (bUseFont)
		{
			nWidth += DrawIconChar(pDC, pViewEvent->GetReminderGlyphRect(), 0x41);
		}
		else
		{
			nWidth += DrawBitmap(&m_ilGlyphs, pDC,
							pViewEvent->GetReminderGlyphRect(), 0, DT_VCENTER);
		}
	}

	int nRecurrenceState = pViewEvent->GetEvent()->GetRecurrenceState();
	if (!pViewEvent->GetRecurrenceGlyphRect().IsRectEmpty())
	{
		if ((nRecurrenceState == xtpCalendarRecurrenceMaster) || (nRecurrenceState == xtpCalendarRecurrenceOccurrence))
		{
			if (bUseFont)
			{
				nWidth += DrawIconChar(pDC, pViewEvent->GetRecurrenceGlyphRect(), 0x43);
			}
			else
			{
				nWidth += DrawBitmap(&m_ilGlyphs, pDC,
					pViewEvent->GetRecurrenceGlyphRect(), 1, DT_VCENTER);
			}
		}
		else if (nRecurrenceState == xtpCalendarRecurrenceException)
		{
			if (bUseFont)
			{
				nWidth += DrawIconChar(pDC, pViewEvent->GetRecurrenceGlyphRect(), 0x44);
			}
			else
			{
				nWidth += DrawBitmap(&m_ilGlyphs, pDC,
					pViewEvent->GetRecurrenceGlyphRect(), 2, DT_VCENTER);
			}
		}
	}

	BOOL bMeeting = pViewEvent->GetEvent()->IsMeeting();
	CRect rcMeeting = pViewEvent->GetMeetingGlyphRect();

	if (bMeeting && !pViewEvent->GetMeetingGlyphRect().IsRectEmpty())
	{
		if (bUseFont)
		{
			nWidth += DrawIconChar(pDC, rcMeeting, 0x45);
		}
		else
		{
			nWidth += DrawBitmap(&m_ilGlyphs, pDC, rcMeeting, 3, DT_VCENTER);
		}
	}

	if (pViewEvent->GetEvent()->IsPrivate() &&
		!pViewEvent->GetPrivateGlyphRect().IsRectEmpty())
	{
		if (bUseFont)
		{
			nWidth += DrawIconChar(pDC, pViewEvent->GetPrivateGlyphRect(), 0x42);
		}
		else
		{
			nWidth += DrawBitmap(&m_ilGlyphs, pDC,
							pViewEvent->GetPrivateGlyphRect(), 4, DT_VCENTER);
		}
	}

	return nWidth;
}

void CXTPCalendarPaintManager::InitBusyStatusDefaultColors()
{
	m_brushTentative.DeleteObject();
	m_bmpTentativePattern.DeleteObject();

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&m_bmpTentativePattern, XTP_IDB_CALENDAR_TENATIVE_PATTERN));
	VERIFY(m_brushTentative.CreatePatternBrush(&m_bmpTentativePattern));


	CXTPCalendarViewPartBrushValue* pBVal_Free = _GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusFree, TRUE);
	CXTPCalendarViewPartBrushValue* pBVal_Tentative = _GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusTentative, TRUE);
	CXTPCalendarViewPartBrushValue* pBVal_Busy = _GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusBusy, TRUE);
	CXTPCalendarViewPartBrushValue* pBVal_OutOO = _GetAtGrow_BusyStatusBrushValue(xtpCalendarBusyStatusOutOfOffice, TRUE);

	if (pBVal_Free)
	{
		pBVal_Free->SetStandardValue(new CBrush(RGB(0xFF, 0xFF, 0xFF)), TRUE);
	}
	if (pBVal_Tentative)
	{
		pBVal_Tentative->SetStandardValue(&m_brushTentative, FALSE);
	}
	if (pBVal_Busy)
	{
		pBVal_Busy->SetStandardValue(new CBrush(RGB(0, 0, 0xFF)), TRUE);
	}
	if (pBVal_OutOO)
	{
		pBVal_OutOO->SetStandardValue(new CBrush(RGB(132, 0, 132)), TRUE);
	}
}

void CXTPCalendarPaintManager::SetBusyStatusBrush(int eBusyStatus, CBrush* pBrush, BOOL bAutodeleteBrush)
{
	if (eBusyStatus < 0 || !pBrush)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarViewPartBrushValue* pBVal = _GetAtGrow_BusyStatusBrushValue(eBusyStatus, TRUE);
	ASSERT(pBVal);
	if (pBVal)
	{
		pBVal->SetCustomValue(pBrush, bAutodeleteBrush);
	}
}

CBrush* CXTPCalendarPaintManager::GetBusyStatusBrush(int nBusyStatus)
{
	CXTPCalendarViewPartBrushValue* pBVal = _GetAtGrow_BusyStatusBrushValue(nBusyStatus, TRUE);
	ASSERT(pBVal);
	if (pBVal)
	{
		CBrush* pBrush = pBVal->GetValue();
		return pBrush;
	}

	return NULL;
}

CXTPCalendarViewPartBrushValue* CXTPCalendarPaintManager::_GetAtGrow_BusyStatusBrushValue(
	int nBusyStatus, BOOL bGrow)
{
	if (nBusyStatus < 0)
	{
		//ASSERT(FALSE);
		nBusyStatus = 0;
	}

	int nCount = (int)m_arBusyStatusBrushes.GetSize();

	if (nBusyStatus >= nCount || m_arBusyStatusBrushes[nBusyStatus] == NULL)
	{
		if (!bGrow)
		{
			return NULL;
		}
		m_arBusyStatusBrushes.SetAtGrow(nBusyStatus, new CXTPCalendarViewPartBrushValue);
	}
	CXTPCalendarViewPartBrushValue* pBVal = m_arBusyStatusBrushes[nBusyStatus];
	return pBVal;
}

void CXTPCalendarPaintManager::DrawBusyStatus(CDC* pDC, CRect& rcRect, int eBusyStatus)
{
	CBrush* pBrush = GetBusyStatusBrush(eBusyStatus);
	if (!pBrush || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	pDC->FillRect(&rcRect, pBrush);
}

void CXTPCalendarPaintManager::DrawBusyStatus(CDC* pDC, CRgn& rgnBusy, int eBusyStatus)
{
	CBrush* pBrush = GetBusyStatusBrush(eBusyStatus);
	if (!pBrush || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	CBrush brBorder(m_clrWindowText);

	VERIFY(pDC->FillRgn(&rgnBusy, pBrush));
	VERIFY(pDC->FrameRgn(&rgnBusy, &brBorder, 1, 1));
}

//class CXTPCalendarTimeLineViewTimeScalePart: public CXTPCalendarViewPart

/*
int CXTPCalendarPaintManager::CTimeLineViewTimeScalePart::_CalcHeaderHeigt(CDC* pDC)
{

}

int CXTPCalendarPaintManager::CTimeLineViewTimeScalePart::_CalcScaleHeigt(CDC* pDC)
{
	return 0;
}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart::DrawUpperScale(
								CDC* pDC, const CRect& rc,
								CXTPCalendarTimeLineViewTimeScale* pTimeScale)
{

}
*/

CXTPCalendarTimeLineViewTimeScalePart::CXTPCalendarTimeLineViewTimeScalePart(CXTPCalendarViewPart* pParentPart)
	: CXTPCalendarViewPart(pParentPart)
{
	m_nHeaderHeight = 0;
}

void CXTPCalendarTimeLineViewTimeScalePart::RefreshMetrics()
{
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
	lfIcon.lfHeight = -16 * 11 / 8;
	m_fntScaleHeaderText.SetStandardValue(&lfIcon);
}

int CXTPCalendarTimeLineViewTimeScalePart::CalcHeigt(CDC* pDC)
{
	CXTPFontDC fnt(pDC, m_fntScaleHeaderText);
	CSize sizeText = pDC->GetTextExtent(_T("W1"));

	m_nHeaderHeight = sizeText.cy * 38 / 27;

	return m_nHeaderHeight + 30;
}

void CXTPCalendarTimeLineViewTimeScalePart::Draw(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView)
{
	if (!pView)
	{
		ASSERT(FALSE);
		return;
	}
	CRect rcHeader = rcRect;
	rcHeader.bottom = rcHeader.top + m_nHeaderHeight;

	pDC->FillSolidRect(&rcHeader, pView->m_clrTimeScaleBackground);
	CRect rcBorder = rcHeader;
	rcBorder.top = rcBorder.bottom - 1;

	pDC->FillSolidRect(&rcBorder, RGB(0, 0, 0));
}

CString CXTPCalendarTimeLineViewTimeScalePart::GetHeaderDateFormat(int nLabelInterval)
{
	//LOCALE_IDATE  -  short date format ordering
	// 0 Month-Day-Year
	// 1 Day-Month-Year
	// 2 Year-Month-Day
	int nDateOrdering = CXTPCalendarUtils::GetLocaleLong(LOCALE_ILDATE);
	CString strFormat = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE);
	CString strD = _T("dd"), strM = _T("M"), strY = _T("Y");

	if (strFormat.Find(_T("dd")) < 0)
		strD = _T("d");

	if (strFormat.Find(_T("MMMM")) >= 0)
		strM = _T("MMMM");
	else if (strFormat.Find(_T("MMM")) >= 0)
		strM = _T("MMM");
	else if (strFormat.Find(_T("MM")) >= 0)
		strM = _T("MM");

	if (strFormat.Find(_T("yyyy")) >= 0)
		strY = _T("yyyy");
	else if (strFormat.Find(_T("yy")) >= 0)
		strY = _T("yy");

	if (nLabelInterval == xtpDTScale_Day)
	{
		if (nDateOrdering == 0)
			strFormat.Format(_T("%s %s %s"), (LPCTSTR)strM, (LPCTSTR)strD, (LPCTSTR)strY);
		else if (nDateOrdering == 1)
			strFormat.Format(_T("%s %s %s"), (LPCTSTR)strD, (LPCTSTR)strM, (LPCTSTR)strY);
		else// if (nDateOrdering == 2)
			strFormat.Format(_T("%s %s %s"), (LPCTSTR)strY, (LPCTSTR)strM, (LPCTSTR)strD);
	}
	else if (nLabelInterval == xtpDTScale_Month)
	{
		if (nDateOrdering == 2)
			strFormat.Format(_T("%s %s"), (LPCTSTR)strY, (LPCTSTR)strM);
		else
			strFormat.Format(_T("%s %s"), (LPCTSTR)strM, (LPCTSTR)strY);
	}

	return strFormat;
}


void CXTPCalendarTimeLineViewTimeScalePart::DrawHeader(CDC* pDC, const CRect& rcRect,
																	CXTPCalendarTimeLineView* pView, int nLabelInterval)
{
	if (!XTP_SAFE_GET2(pView, GetTimeScale(), GetTimeScaleParams(), NULL))
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
	CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();

	CRect rcHeader = rcRect;
	rcHeader.bottom = rcHeader.top + m_nHeaderHeight;

	CRect rcBorder = rcHeader;
	rcBorder.top = rcBorder.bottom - 1;

	pDC->FillSolidRect(&rcHeader, pView->m_clrTimeScaleBackground);
	pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

	pDC->SetBkMode(TRANSPARENT);

	COleDateTime dtStart = pTimeScale->GetStartViewDate();


	double dTimePerPixel = (double)pTSParams->GetTimePerPixel();

	//CXTPFontDC autoFont(pDC, m_fntScaleHeaderText, GetTextColor());
	CXTPFontDC autoFont(pDC, m_fntScaleHeaderText, pView->m_clrTimeScaleText);

	CString strFormat = GetHeaderDateFormat(nLabelInterval);
	COleDateTimeSpan spDay(1, 0, 0, 0);


	COleDateTime dtDay = CXTPCalendarUtils::ResetTime(dtStart);
	for (int nP = 0; nP < 100; nP++)
	{
		COleDateTime dtDayNext = dtDay;
		if (nLabelInterval == xtpDTScale_Month)
		{
			CXTPCalendarUtils::ShiftDate_Month(dtDayNext, 1);
		}
		else
		{
			ASSERT(nLabelInterval == xtpDTScale_Day);
			dtDayNext = dtDay + spDay;
		}

		double dDay_tshift = double(dtDay - dtStart);
		int nDay_xshift = int(dDay_tshift / dTimePerPixel);

		if (nDay_xshift > rcHeader.Width())
			break;

		double dNextDay_tshift = double(dtDayNext - dtStart);
		int nNextDay_xshift = int(dNextDay_tshift / dTimePerPixel);

		CRect rcDayLabel = rcHeader;
		rcDayLabel.left += 5;
		rcDayLabel.right = rcHeader.left + nNextDay_xshift - 15;

		if (nDay_xshift > 0)
			rcDayLabel.left += nDay_xshift;

		CString strLabel = CXTPCalendarUtils::GetDateFormat(dtDay, strFormat);
		CSize szLabel = pDC->GetTextExtent(strLabel);

		UINT nFormat = DT_VCENTER | DT_SINGLELINE;

		CPoint ptTR(rcDayLabel.left + szLabel.cx + 4, rcDayLabel.bottom - 4 - 5);

		if (szLabel.cx >= rcDayLabel.Width())
		{
			nFormat |= DT_RIGHT;
			rcDayLabel.left = rcHeader.left;
			ptTR.x = rcDayLabel.right;
		}

		pDC->DrawText(strLabel, &rcDayLabel, nFormat);

		POINT arTRPts[] ={ {ptTR.x, ptTR.y}, {ptTR.x + 9, ptTR.y}, {ptTR.x + 4, ptTR.y + 5} };
		CRgn rgnTR;
		BOOL bRgn = rgnTR.CreatePolygonRgn(arTRPts, _countof(arTRPts), ALTERNATE);
		ASSERT(bRgn);

		if (bRgn)
		{
			//CBrush brTR(RGB(0, 0, 0));
			//CBrush brTR(pView->m_clrTimeScaleBorder);
			CBrush brTR(pView->m_clrTimeScaleText);
			VERIFY(pDC->FillRgn(&rgnTR, &brTR));
		}

		dtDay = dtDayNext;
	}
}

//int CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_Day::CalcHeigt(CDC* pDC)
//{
//  CXTPFontDC fnt(pDC, m_fntScaleHeaderText);
//  CSize sizeText = GetTextExtent(pDC, _T("W1"));
//
//  int nHeight = TBase::CalcHeigt(CDC* pDC) 20 ;
//
//  return nHeight;
//}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_Day::Draw(CDC* pDC, const CRect& rcRect,
																	CXTPCalendarTimeLineView* pView)
{
	if (!pView || !pView->GetTimeScale())
	{
		ASSERT(FALSE);
		return;
	}

	pDC->SetBkMode(TRANSPARENT);

	CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
	pDC->FillSolidRect(&rcRect, pView->m_clrTimeScaleBackground);

	CRect rcBorder = rcRect;
	rcBorder.top = rcBorder.bottom - 1;
	pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

	CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
	if (!pTSParams)
	{
		ASSERT(FALSE);
		return;
	}

	COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
	int nSInterval_px = pTSParams->GetScaleIntervalWidth_px();

	COleDateTime dtStart = pTimeScale->GetStartViewDate();
	COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

	double dFirstStroke_Shift = (double)(dtBegin - dtStart);
	double dTimePerPixel = (double)pTSParams->GetTimePerPixel(); //(DATE)((double)spSInterval / nSInterval_px);

	int nFirstStroke_Xshift = (int)(dFirstStroke_Shift / dTimePerPixel);

	CXTPFontDC autoFont(pDC, m_fntScaleHeaderText, GetTextColor());

	DrawHeader(pDC, rcRect, pView, xtpDTScale_Day);

	CRect rcHeader = rcRect;
	rcHeader.bottom = rcHeader.top + m_nHeaderHeight;

	TBase::Draw(pDC, rcRect, pView);

	CString strLDFormat = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE);

	COleDateTime dtDay = CXTPCalendarUtils::ResetTime(dtStart);
	COleDateTimeSpan spDay(1, 0, 0, 0);
	for (int nP = 0; nP < 100; nP++, dtDay += spDay)
	{
		double dDay_tshift = double(dtDay - dtStart);
		int nDay_xshift = int(dDay_tshift / dTimePerPixel);

		if (nDay_xshift > rcHeader.Width())
			break;

		double dNextDay_tshift = double(dtDay + spDay - dtStart);
		int nNextDay_xshift = int(dNextDay_tshift / dTimePerPixel);

		CRect rcDayLabel = rcHeader;
		rcDayLabel.left += 5;
		rcDayLabel.right = rcHeader.left + nNextDay_xshift - 15;

		if (nDay_xshift > 0)
			rcDayLabel.left += nDay_xshift;

		CString strLabel = CXTPCalendarUtils::GetDateFormat(dtDay, strLDFormat);
		CSize szLabel = pDC->GetTextExtent(strLabel);

		UINT nFormat = DT_VCENTER | DT_SINGLELINE;

		CPoint ptTR(rcDayLabel.left + szLabel.cx + 4, rcDayLabel.bottom - 4 - 5);

		if (szLabel.cx >= rcDayLabel.Width())
		{
			nFormat |= DT_RIGHT;
			rcDayLabel.left = rcHeader.left;
			ptTR.x = rcDayLabel.right;
		}

		pDC->DrawText(strLabel, &rcDayLabel, nFormat);

		POINT arTRPts[] ={ {ptTR.x, ptTR.y}, {ptTR.x + 9, ptTR.y}, {ptTR.x + 4, ptTR.y + 5} };
		CRgn rgnTR;
		BOOL bRgn = rgnTR.CreatePolygonRgn(arTRPts, _countof(arTRPts), ALTERNATE);
		ASSERT(bRgn);

		if (bRgn)
		{
			//CBrush brTR(RGB(0, 0, 0));
			//CBrush brTR(pView->m_clrTimeScaleBorder);
			CBrush brTR(pView->m_clrTimeScaleText);
			VERIFY(pDC->FillRgn(&rgnTR, &brTR));
		}

	}
	autoFont.SetFont(&GetTextFont());

	CString strHour;
	int nCount = (rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + 1;
	for (int i = 0; i < nCount; i++)
	{
		COleDateTimeSpan spFirstStroke((double)spSInterval * i);
		COleDateTime dtStroke = dtBegin + spFirstStroke;
		BOOL bBold = dtStroke.GetMinute() == 0;
		CRect rcStroke;

		rcStroke.left = rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
		rcStroke.right = rcStroke.left + (bBold ? 2 : 1);

		rcStroke.bottom = rcRect.bottom;
		rcStroke.top = rcStroke.bottom - (bBold ? 10 : 8);

		pDC->FillSolidRect(&rcStroke, pView->m_clrTimeScaleBorder);

		if (bBold)
		{
			CRect rcLabel = rcRect;
			rcLabel.top += m_nHeaderHeight + 7;
			rcLabel.left = rcStroke.left+1;
			rcLabel.right = rcLabel.left + nSInterval_px;

			rcLabel.bottom = rcStroke.top - 1;

			strHour.Format(_T("%d"), dtStroke.GetHour());

			COleDateTime dtNow = COleDateTime::GetCurrentTime();
			if (dtStroke.GetYear() == dtNow.GetYear() &&
				dtStroke.GetMonth() == dtNow.GetMonth() &&
				dtStroke.GetDay() == dtNow.GetDay() &&
				dtStroke.GetHour() == dtNow.GetHour())
				pDC->FillSolidRect(&rcLabel, pView->m_clrSelectedBackground);

			pDC->DrawText(strHour, &rcLabel, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
		}
	}
}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_Week::Draw(CDC* pDC, const CRect& rcRect,
																	CXTPCalendarTimeLineView* pView)
{
	if (!pView || !pView->GetTimeScale())
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
	pDC->FillSolidRect(&rcRect, pView->m_clrTimeScaleBackground);

	DrawHeader(pDC, rcRect, pView, xtpDTScale_Month);

	CRect rcBorder = rcRect;
	rcBorder.top = rcBorder.bottom - 1;
	pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

	CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
	if (!pTSParams)
	{
		ASSERT(FALSE);
		return;
	}

	COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
	int nSInterval_px = pTSParams->GetScaleIntervalWidth_px();

	COleDateTime dtStart = pTimeScale->GetStartViewDate();
	COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

	//COleDateTimeSpan spFirstStroke_Shift = dtBegin - dtStart;
	double dFirstStroke_Shift = (double)(dtBegin - dtStart);
	double dTimePerPixel = (double)pTSParams->GetTimePerPixel(); //(DATE)((double)spSInterval / nSInterval_px);

	int nFirstStroke_Xshift = (int)(dFirstStroke_Shift / dTimePerPixel);

	CString strLabel;
	int nCount = (rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + 1;
	for (int i = 0; i < nCount; i++)
	{
		COleDateTimeSpan spFirstStroke((double)spSInterval * i);
		COleDateTime dtStroke = dtBegin + spFirstStroke;

		CRect rcStroke;
		rcStroke.left = rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
		rcStroke.right = rcStroke.left + 1;

		rcStroke.bottom = rcRect.bottom;
		rcStroke.top = rcStroke.bottom - 8;

		pDC->FillSolidRect(&rcStroke, pView->m_clrTimeScaleBorder);

		CRect rcLabel = rcRect;
		rcLabel.top += m_nHeaderHeight + 7;
		rcLabel.left = rcStroke.left+1;
		rcLabel.right = rcLabel.left + nSInterval_px;

		rcLabel.bottom = rcStroke.top - 1;

		int nWD = dtStroke.GetDayOfWeek() - 1;
		if (nWD == 0)
			nWD = 7;
		CString strWD = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVDAYNAME1 + nWD - 1);
		strLabel.Format(_T("%s %d"), (LPCTSTR)strWD, dtStroke.GetDay());

		pDC->SetBkColor(pView->m_clrTimeScaleBackground);

		COleDateTime dtNow = COleDateTime::GetCurrentTime();
		if (dtStroke.GetYear() == dtNow.GetYear() &&
			dtStroke.GetMonth() == dtNow.GetMonth() &&
			dtStroke.GetDay() == dtNow.GetDay())
			pDC->FillSolidRect(&rcLabel, pView->m_clrSelectedBackground);

		DrawText(pDC, strLabel, &rcLabel, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_Month::Draw(CDC* pDC, const CRect& rcRect,
																	CXTPCalendarTimeLineView* pView)
{
	if (!pView || !pView->GetTimeScale())
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
	pDC->FillSolidRect(&rcRect, pView->m_clrTimeScaleBackground);

	DrawHeader(pDC, rcRect, pView, xtpDTScale_Month);

	CRect rcBorder = rcRect;
	rcBorder.top = rcBorder.bottom - 1;
	pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

	CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
	if (!pTSParams)
	{
		ASSERT(FALSE);
		return;
	}

	COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
	int nSInterval_px = pTSParams->GetScaleIntervalWidth_px();

	COleDateTime dtStart = pTimeScale->GetStartViewDate();
	COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

	//COleDateTimeSpan spFirstStroke_Shift = dtBegin - dtStart;
	double dFirstStroke_Shift = (double)(dtBegin - dtStart);
	double dTimePerPixel = (double)pTSParams->GetTimePerPixel(); //(DATE)((double)spSInterval / nSInterval_px);

	int nFirstStroke_Xshift = (int)(dFirstStroke_Shift / dTimePerPixel);

	CString strLabel;
	int nCount = (rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + 1;
	for (int i = 0; i < nCount; i++)
	{
		COleDateTimeSpan spFirstStroke((double)spSInterval * i);
		COleDateTime dtStroke = dtBegin + spFirstStroke;
		BOOL bBold = dtStroke.GetDay() == 1;
		CRect rcStroke;

		rcStroke.left = rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
		rcStroke.right = rcStroke.left + (bBold ? 2 : 1);

		rcStroke.bottom = rcRect.bottom;
		rcStroke.top = rcStroke.bottom - (bBold ? 10 : 8);

		pDC->FillSolidRect(&rcStroke, pView->m_clrTimeScaleBorder);

		CRect rcLabel = rcRect;
		rcLabel.top += m_nHeaderHeight + 7;
		rcLabel.left = rcStroke.left+1;
		rcLabel.right = rcLabel.left + nSInterval_px;

		rcLabel.bottom = rcStroke.top - 1;

		int nWD = dtStroke.GetDayOfWeek() - 1;
		if (nWD == 0)
			nWD = 7;
		CString strWD = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVDAYNAME1 + nWD - 1);

		//  Microsoft BUG: Unless the user has manually tweaked the "iFirstDayOfWeek" registry key,
		//  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, ...) always returns
		//  zero, which means Sunday, even though the locale on the system indicates
		//  otherwise.
		// Ask the system for the first day of the week
		//TCHAR sz[2];
		//::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, sz, 2);
		//::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IFIRSTDAYOFWEEK, sz, 2);
		//CString strWD = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVDAYNAME1 + nWD - 1);
		TCHAR szWD1[8] = {0};
		STRNCPY_S(szWD1, 4, strWD, 1);
		strLabel.Format(_T("%s %d"), szWD1, dtStroke.GetDay());

		pDC->SetBkColor(pView->m_clrTimeScaleBackground);

		if (nWD % 7 == 0)
			pDC->FillSolidRect(&rcStroke, RGB(255, 0, 0));

		COleDateTime dtNow = COleDateTime::GetCurrentTime();
		if (dtStroke.GetYear() == dtNow.GetYear() &&
			dtStroke.GetMonth() == dtNow.GetMonth() &&
			dtStroke.GetDay() == dtNow.GetDay())
			pDC->FillSolidRect(&rcLabel, pView->m_clrSelectedBackground);

		DrawText(pDC, strLabel, &rcLabel, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewTimeScalePart_WorkWeek::Draw(CDC* pDC, const CRect& rcRect,
																	CXTPCalendarTimeLineView* pView)
{
	if (!pView || !pView->GetTimeScale())
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
	pDC->FillSolidRect(&rcRect, pView->m_clrTimeScaleBackground);

	DrawHeader(pDC, rcRect, pView, xtpDTScale_Month);

	CRect rcBorder = rcRect;
	rcBorder.top = rcBorder.bottom - 1;
	pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

	CXTPCalendarTLV_TimeScaleParams* pTSParams = pTimeScale->GetTimeScaleParams();
	if (!pTSParams)
	{
		ASSERT(FALSE);
		return;
	}

	COleDateTimeSpan spSInterval = pTSParams->GetScaleInterval();
	int nSInterval_px = pTSParams->GetScaleIntervalWidth_px();

	COleDateTime dtStart = pTimeScale->GetStartViewDate();
	COleDateTime dtBegin = pTSParams->CalcScaleIntervalBegin(dtStart);

	//COleDateTimeSpan spFirstStroke_Shift = dtBegin - dtStart;
	double dFirstStroke_Shift = (double)(dtBegin - dtStart);
	double dTimePerPixel = (double)pTSParams->GetTimePerPixel(); //(DATE)((double)spSInterval / nSInterval_px);

	int nFirstStroke_Xshift = (int)(dFirstStroke_Shift / dTimePerPixel);

	CString strLabel;
	int nCount = (rcRect.Width() + abs(nFirstStroke_Xshift)) / nSInterval_px + 1;
	for (int i = 0; i < nCount; i++)
	{
		COleDateTimeSpan spFirstStroke((double)spSInterval * i);
		COleDateTime dtStroke = dtBegin + spFirstStroke;

		CRect rcStroke;
		rcStroke.left = rcRect.left + nFirstStroke_Xshift + i * nSInterval_px;
		rcStroke.right = rcStroke.left + 1;

		rcStroke.bottom = rcRect.bottom;
		rcStroke.top = rcStroke.bottom - 8;

		pDC->FillSolidRect(&rcStroke, pView->m_clrTimeScaleBorder);

		CRect rcLabel = rcRect;
		rcLabel.top += m_nHeaderHeight + 7;
		rcLabel.left = rcStroke.left+1;
		rcLabel.right = rcLabel.left + nSInterval_px;

		rcLabel.bottom = rcStroke.top - 1;

		int nWD = dtStroke.GetDayOfWeek() - 1;
		if (nWD == 0)
			nWD = 7;
		CString strWD = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVDAYNAME1 + nWD - 1);
		strLabel.Format(_T("%s %d"), (LPCTSTR)strWD, dtStroke.GetDay());

		pDC->SetBkColor(pView->m_clrTimeScaleBackground);

		COleDateTime dtNow = COleDateTime::GetCurrentTime();
		if (dtStroke.GetYear() == dtNow.GetYear() &&
			dtStroke.GetMonth() == dtNow.GetMonth() &&
			dtStroke.GetDay() == dtNow.GetDay())
			pDC->FillSolidRect(&rcLabel, pView->m_clrSelectedBackground);

		DrawText(pDC, strLabel, &rcLabel, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewPart::DrawGroup(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineViewGroup* pGroup)
{
	// TODO: draw group header if need.

	int nCount = pGroup->GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarTimeLineViewEvent* pViewEv = pGroup->GetViewEvent(i);
		DrawEvent(pDC, rcRect, pViewEv);
	}
}

void CXTPCalendarPaintManager::CTimeLineViewPart::DrawEvent(CDC* pDC, const CRect& rcEvents, CXTPCalendarTimeLineViewEvent* pEventView)
{
	CRect rcEvent = pEventView->GetEventRect();

	if (!pDC->IsPrinting())
	{
		int t = rcEvents.top;
		if (rcEvent.top < t)
			return; //to hide not fully visible event
		rcEvent.top -= (t - 70);
		rcEvent.bottom -= (t - 70);
	}

	rcEvent.bottom -= 2;
	rcEvent.right -= 2;

	pEventView->SetLastVisibleRect(rcEvent);

	if (pEventView->IsSelected())
		pDC->FillSolidRect(rcEvent, m_clrSelectedBackground);

	CRect rcB1 = rcEvent;
	rcB1.top += 1;
	rcB1.bottom = rcB1.top + 7;
	rcB1.left += 1;
	rcB1.right = rcB1.left + pEventView->GetEventDurationInPixels();

	pDC->FillSolidRect(rcB1, pEventView->GetView()->m_clrEventBar);

	rcB1.bottom = rcB1.top + 1;

	pDC->FillSolidRect(rcB1, pEventView->GetView()->m_clrEventBarLine);

	CSize szIcon(0, 0);
	if (m_pimgEvent)
	{
		szIcon = m_pimgEvent->GetExtent();

		CRect rcIcon = rcEvent;
		rcIcon.left += 1;
		rcIcon.bottom -= 5;
		rcIcon.top = rcIcon.bottom - szIcon.cy;
		rcIcon.right = rcIcon.left + szIcon.cx;

		m_pimgEvent->DrawImage(pDC, rcIcon, m_pimgEvent->GetSource());
	}

	COLORREF clrText = GetTextColor();
	if (pEventView->IsSelected())
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_HIGHLIGHTTEXT));

		pDC->DrawFocusRect(rcEvent);
		clrText = m_clrSelectedText;
	}

	pDC->SetBkMode(TRANSPARENT);

	CXTPFontDC autoFont(pDC, &GetTextFont(), clrText);

	CRect rcText = rcEvent;
	rcText.left = rcEvent.left + szIcon.cx + 3;
	rcText.right = rcEvent.right + szIcon.cx + 3;
	rcText.bottom -= 3;

	CString sText = pEventView->GetEvent()->GetSubject();
//extend text draw size in NonCompact mode
	BOOL bCompact = TRUE;
	CXTPCalendarOptions* pOptions = pEventView->GetCalendarControl()->GetCalendarOptions();
	if (pOptions)
		bCompact = pOptions->bTimeLineCompact;
	if (!bCompact)
	{
		CRect rcCl;
		pEventView->GetCalendarControl()->GetClientRect(&rcCl);
		if (rcText.left < rcCl.left && rcText.right > rcCl.right)
		{
			rcText.left = rcCl.left;
			rcText.right = rcCl.right;
		}
		else if (rcText.left <= rcCl.left && rcText.right > rcCl.left)
		{
			rcText.left = rcCl.left; //keep right side
		}
		else if (rcText.left > rcCl.left)
		{
			rcText.right = rcCl.right; //unrestricted length if start visible
		}
	}
	pDC->DrawText(sText, rcText, DT_SINGLELINE | DT_BOTTOM | DT_NOCLIP | DT_END_ELLIPSIS);
}

CSize CXTPCalendarPaintManager::CTimeLineViewPart::CalcEventSize(CDC* pDC,
									CXTPCalendarTimeLineViewEvent* pEventView)
{
	CSize szEvent = m_pimgEvent->GetExtent();

	CSize szText = GetTextExtent(pDC, pEventView->GetItemTextSubject());

	int iMaxPixels = pEventView->GetView()->m_nTimelineMaxPixelsForEvent;

	int nScaleID = pEventView->GetView()->GetTimeScaleID();
	if (nScaleID == 1)
		iMaxPixels = pEventView->GetView()->m_nTimelineMaxPixelsForEvent_ScaleDay;
	else if (nScaleID == 2)
		iMaxPixels = pEventView->GetView()->m_nTimelineMaxPixelsForEvent_ScaleWeek;

	szEvent.cx = szEvent.cx + min(szText.cx, iMaxPixels) + 10;
	szEvent.cy = max(szEvent.cy, szText.cy) + 10;

	return szEvent;
}

CXTPCalendarPaintManager::CTimeLineViewPart::~CTimeLineViewPart()
{
	CMDTARGET_RELEASE(m_pimgEvent);
}

void CXTPCalendarPaintManager::CTimeLineViewPart::_Init()
{
	CXTPResourceImages* pImages = XTPResourceImages();

	m_pimgEvent = new CXTPResourceImage(pImages);
}

void CXTPCalendarPaintManager::CTimeLineViewPart::RefreshMetrics()
{
	LPCTSTR pcszBmpName = _T("XTP_IDB_CALENDAR_TLV_EVENT");

	HMODULE hModule = XTPResourceManager()->GetResourceHandle();
	HRSRC hRc = NULL;

	if (hModule)
		hRc = ::FindResource(hModule, pcszBmpName, _T("PNG"));

	if (!hRc)
		hModule = AfxFindResourceHandle(pcszBmpName, _T("PNG"));

	if (hModule)
		VERIFY(m_pimgEvent->LoadFile(hModule, pcszBmpName));

	m_clrEventBar.SetStandardValue(::GetSysColor(COLOR_ACTIVECAPTION));
	m_clrEventBarLine.SetStandardValue(RGB(255, 0, 0));

	m_clrSelectedText.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	m_clrSelectedBackground.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHT));

	m_clrBackground.SetStandardValue(::GetSysColor(COLOR_WINDOW));
	m_clrText.SetStandardValue(::GetSysColor(COLOR_WINDOWTEXT));

	m_clrTimeScaleBackground.SetStandardValue(::GetSysColor(COLOR_WINDOW));
	m_clrTimeScaleHightlite.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHT));
	m_clrTimeScaleText.SetStandardValue(::GetSysColor(COLOR_WINDOWTEXT));
}

BOOL CXTPCalendarPaintManager::GetIconLogFont(LOGFONT* plf)
{
	return CXTPDrawHelpers::GetIconLogFont(plf);
}

//////////////////////////////////////////////////////////////////////////
#ifdef _XTP_ACTIVEX

CXTPCalendarFlagsSet_imp* CXTPCalendarPaintManager::AxGetAskItemTextFlags()
{
	CXTPCalendarFlagsSet_imp* pFlagsWrapper = new CXTPCalendarFlagsSet_imp();
	if (pFlagsWrapper)
	{
		pFlagsWrapper->SetDataRef(&m_dwAskItemTextFlags, this);
	}
	return pFlagsWrapper;
}

#endif
