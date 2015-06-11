// XTPDatePickerOffice2013Theme.cpp: interface for the CXTPDatePickerOffice2013Theme class.
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

#include "Resource.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"

#include "XTPCalendarDefines.h"

#include "XTPDatePickerPaintManager.h"
#include "XTPDatePickerControl.h"
#include "XTPDatePickerOffice2013Theme.h"
#include "XTPDatePickerItemMonth.h"
#include "XTPDatePickerItemDay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CXTPDatePickerThemeOffice2013::CXTPDatePickerThemeOffice2013()
{
	// initialize default fonts
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
	lfIcon.lfHeight +=1;
	SetFontIndirect(&lfIcon);

	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
	lfIcon.lfWeight = FW_BOLD;
	m_fontHeader.DeleteObject();
	VERIFY(m_fontHeader.CreateFontIndirect(&lfIcon));
}

CXTPDatePickerThemeOffice2013::~CXTPDatePickerThemeOffice2013()
{

}

void CXTPDatePickerThemeOffice2013::RefreshMetrics()
{
	CXTPDatePickerPaintManager::RefreshMetrics();

	m_clrBackground = CXTPPaintManagerColorGradient(RGB(255, 255, 255), RGB(240, 240, 240));
	m_clrMonthHeaderBack = RGB(240, 240, 240);

	m_clrDayBack = RGB(255, 0, 255);

	m_clrSelectedDayBack = RGB(205,230,247);

	m_clrMonthHeaderText = RGB(68, 68, 68);
	m_clrDayText = RGB(68, 68, 68);
	m_clrNonMonthDayText = RGB(68, 68, 68);
	m_clrDaysOfWeekText = RGB(68, 68, 68);
	m_clrWeekNumbersText = RGB(68, 68, 68);

	m_clrHighlightToday = RGB(153,200,233);
}

void CXTPDatePickerThemeOffice2013::DrawBackground(CDC* pDC, CRect rcClient)
{
	COLORREF customColor = GetControlBackColor();

	if (customColor != GetSysColor(COLOR_WINDOW))
		pDC->FillSolidRect(rcClient, customColor);
	else
		XTPDrawHelpers()->GradientFill(pDC, &rcClient, m_clrBackground, FALSE);
}

void CXTPDatePickerThemeOffice2013::DrawWeekNumbers(CDC* /*pDC*/, CXTPDatePickerItemMonth* /*pMonth*/)
{
	// currently void
}

void CXTPDatePickerThemeOffice2013::MakeStringUpper(CString &strText)
{
	const int nStringLength = strText.GetLength();
	CharUpper(strText.GetBuffer(nStringLength));
	strText.ReleaseBuffer();
}

BOOL CXTPDatePickerThemeOffice2013::DrawDay(CDC* pDC, CXTPDatePickerItemDay* pDay)
{
	if (!pDC || !pDay)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	// check this day item
	BOOL bThisMonthItem = FALSE;
	BOOL bNextMonthItem = FALSE;
	BOOL bPreviousMonthItem = FALSE;

	CXTPDatePickerControl* pControl = pDay->GetDatePickerControl();
	CXTPDatePickerItemMonth* pMonth = pDay->GetMonth();

	COleDateTime dtDay = pDay->GetDate();
	CRect rcDay = pDay->GetRect();

	if (dtDay.GetYear() > pMonth->GetMonth().GetYear())
		bNextMonthItem = TRUE;
	else if (dtDay.GetYear() < pMonth->GetMonth().GetYear())
		bPreviousMonthItem = TRUE;
	else // years the same
	{
		if (dtDay.GetMonth() > pMonth->GetMonth().GetMonth())
			bNextMonthItem = TRUE;
		else if (dtDay.GetMonth() < pMonth->GetMonth().GetMonth())
			bPreviousMonthItem = TRUE;
		else // months the same
			bThisMonthItem = TRUE;
	}

	if (bPreviousMonthItem && !pMonth->GetShowDaysBefore())
		return FALSE;

	if (bNextMonthItem && !pMonth->GetShowDaysAfter())
		return FALSE;

	// set default drawing metrics depending on item month position
	XTP_DAYITEM_METRICS* pMetrics = new XTP_DAYITEM_METRICS;
	pMetrics->clrBackground = bThisMonthItem ?
		GetDayBackColor() : GetNonMonthDayBackColor();
	pMetrics->clrForeground = bThisMonthItem ?
		GetDayTextColor() : GetNonMonthDayTextColor();
	pMetrics->SetFont(bThisMonthItem ?
		GetDayTextFont() : GetNonMonthDayTextFont());

	// call callback function for changing parameters if needed
	pControl->GetDayMetrics(dtDay, pMetrics);

	// change default drawing metrics if day is selected
	if (pControl->IsSelected(dtDay))
	{
		pMetrics->clrBackground = GetSelectedDayBackColor();
		pMetrics->clrForeground = GetSelectedDayTextColor();
	}

	if (pDay->IsToday(dtDay))
		pMetrics->clrForeground = RGB(255, 255, 255);

	// apply settings to the DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(pMetrics->clrForeground);

	// set previous and next month days to gray
	if (bNextMonthItem || bPreviousMonthItem)
	{
		pDC->SetTextColor(RGB(128, 128, 128));
	}

	if (pControl->IsSelected(dtDay))
		pDC->FillSolidRect(rcDay, pMetrics->clrBackground); // fill background

	BOOL bToday = FALSE;

	// highlight today item
	if (pControl->GetHighlightToday() && pDay->IsToday(dtDay))
	{
		pDC->FillSolidRect(rcDay, RGB(153,200,233) );
		bToday = TRUE;
	}

	// make a text for drawing
	CString strText;
	strText.Format(_T("%i"), dtDay.GetDay());

	// draw item content
#ifdef _XTP_ACTIVEX
	CPictureHolder* pPictureHolder = pMetrics->GetPictureHolder();
	if ((pPictureHolder != NULL) &&
		(pPictureHolder->GetType() != PICTYPE_NONE) &&
		(pPictureHolder->GetType() != PICTYPE_UNINITIALIZED))
	{
		// draw item picture
		pPictureHolder->Render(pDC, rcDay, rcDay);
	}
	else
#endif
	{
		// draw item text
		CXTPFontDC fntdc(pDC, pMetrics->GetFont());
		pDC->DrawText(strText, &rcDay, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP);
	}

	CRect rcFocus = rcDay;


	if (pControl->IsFocused(dtDay) && pControl->GetSafeHwnd() == ::GetFocus())
	{
		if (!bToday)
		{
			pDC->SetTextColor(0);
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->DrawFocusRect(&rcFocus);
		}
		else
		{
			COLORREF clrFocus = GetHighlightTodayColor();
			clrFocus = clrFocus ^ RGB(255, 128, 128);

			rcFocus.DeflateRect(0, 0, 1, 1);

			CPen pen1(PS_DOT, 1, clrFocus);
			CXTPPenDC autoPen(pDC, &pen1);

			pDC->MoveTo(rcFocus.TopLeft());
			pDC->LineTo(rcFocus.right, rcFocus.top);
			pDC->LineTo(rcFocus.right, rcFocus.bottom);
			pDC->LineTo(rcFocus.left, rcFocus.bottom);
			pDC->LineTo(rcFocus.left, rcFocus.top);
		}
	}

	pMetrics->InternalRelease();

	return TRUE;
}

void CXTPDatePickerThemeOffice2013::DrawDaysOfWeek(CDC* pDC, CXTPDatePickerItemMonth* pMonth)
{
	if (pMonth->m_rcDaysOfWeek.IsRectEmpty())
		return;

	CXTPDatePickerControl* pControl = pMonth->m_pControl;

	// draw days of week text
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetDaysOfWeekTextColor());
	CXTPFontDC fnt(pDC, GetDaysOfWeekTextFont());
	int nMaxX = 0;

	int nTwoLetterWidth = pDC->GetTextExtent(_T("XX"), 2).cx;
	int nWholeLetterWidth = 0;

	for (int i = 0; i < XTP_WEEK_DAYS; i++)
	{
		CString strText(pControl->GetDayOfWeekName(i));
		MakeStringUpper(strText);

		nWholeLetterWidth = max(nWholeLetterWidth, pDC->GetTextExtent(strText).cx);
	}

	for (int nDayDelta = 0; nDayDelta < XTP_WEEK_DAYS; nDayDelta++)
	{
		// calc item rect
		CXTPDatePickerItemDay* pDay = pMonth->GetDay(nDayDelta);
		CRect rcItem(pDay->GetRect());
		rcItem.top = pMonth->m_rcDaysOfWeek.top;
		rcItem.bottom = pMonth->m_rcDaysOfWeek.bottom - 2;
		nMaxX = rcItem.right;
		// get item text
		CString strText(pControl->GetDayOfWeekName(pDay->GetDate().GetDayOfWeek()));
		MakeStringUpper(strText);

		// Check if we can draw whole text
		if (nWholeLetterWidth + 4 <= rcItem.Width())
		{
			pDC->DrawText(strText, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}
		else if (nTwoLetterWidth + 4 <= rcItem.Width()) // Draw 2 letters
		{
			int nChrLen = (int)_tcsnbcnt(strText, 2);
			pDC->DrawText(strText, nChrLen, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}
		else // Draw only first letter
		{
			int nChrLen = (int)_tcsnbcnt(strText, 1);
			pDC->DrawText(strText, nChrLen, &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}
	}
}

void CXTPDatePickerThemeOffice2013::DrawBorder(CDC* pDC, const CXTPDatePickerControl* pControl, CRect& rcClient, BOOL bDraw)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(pControl);
	UNUSED_ALWAYS(rcClient);
	UNUSED_ALWAYS(bDraw);

	// do nothing
}
