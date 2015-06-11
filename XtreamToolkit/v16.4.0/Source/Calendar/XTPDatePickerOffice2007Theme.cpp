// XTPDatePickerOffice2007Theme.cpp: interface for the CXTPDatePickerOffice2007Theme class.
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
#include "XTPDatePickerOffice2007Theme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CXTPDatePickerThemeOffice2007::CXTPDatePickerThemeOffice2007()
{
	m_nPaintTheme = xtpCalendarThemeResource;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPDatePickerThemeOffice2007::~CXTPDatePickerThemeOffice2007()
{
}

void CXTPDatePickerThemeOffice2007::RefreshMetrics()
{
	CXTPDatePickerPaintManager::RefreshMetrics();

	m_clrMonthBorder.SetStandardValue(GetResourceColor(_T("MonthBorder"), RGB(154, 198,255)));
	m_clrMonthHeaderBack.SetStandardValue(GetResourceColor(_T("MonthHeaderBack"), RGB(191, 219, 255)));
	m_clrMonthHeaderText.SetStandardValue(GetResourceColor(_T("MonthHeaderText"), RGB(0, 0, 0)));
	m_clrMonthHeaderButton.SetStandardValue(GetResourceColor(_T("MonthHeaderButton"), RGB(111, 157, 217)));

	m_clrControlBack.SetStandardValue(GetResourceColor(_T("ControlBack"), RGB(255, 255, 255)));

	m_clrHighlightToday.SetStandardValue(GetResourceColor(_T("HighlightToday"), RGB(187, 85, 3)));

	m_clrDayBack.SetStandardValue(m_clrControlBack);
	m_clrDayText.SetStandardValue(GetResourceColor(_T("DayText"), RGB(0, 0, 0)));

	m_clrSelectedDayBack.SetStandardValue(GetResourceColor(_T("SelectedDayBack"), RGB(251, 200, 79)));
	m_clrSelectedDayText.SetStandardValue(GetResourceColor(_T("SelectedDayText"), m_clrDayText));

	m_clrNonMonthDayBack.SetStandardValue(m_clrDayBack);
	m_clrNonMonthDayText.SetStandardValue(GetResourceColor(_T("NonMonthDayText"), m_clr3DShadow));

	m_clrDaysOfWeekBack.SetStandardValue(m_clrDayBack);
	m_clrDaysOfWeekText.SetStandardValue(m_clrDayText);

	m_clrWeekNumbersBack.SetStandardValue(m_clrDayBack);
	m_clrWeekNumbersText.SetStandardValue(m_clrDayText);

	m_clrButtonText.SetStandardValue(RGB(0, 0, 0));
}

void CXTPDatePickerThemeOffice2007::DrawButton(CDC* pDC, const CRect& rcButton, const CString& strButton, BOOL bIsDown, BOOL bIsHighLight)
{
	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("PUSHBUTTON"));
	if (!pImage)
	{
		CXTPDatePickerPaintManager::DrawButton(pDC, rcButton, strButton, bIsDown, bIsHighLight);
		return;
	}

	pImage->DrawImage(pDC, rcButton, pImage->GetSource(bIsDown ? 2 : bIsHighLight ? 1 : 0, 5), CRect(4, 4, 4, 4), 0xFF00FF);

	pDC->SetBkMode(TRANSPARENT);

	if (!strButton.IsEmpty())
	{
		CXTPFontDC autoFont(pDC, &m_fontButton, m_clrButtonText);

		CRect rcText(rcButton);
		rcText.DeflateRect(1, 1);

		pDC->DrawText(strButton, rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
}



COLORREF CXTPDatePickerThemeOffice2007::GetResourceColor(LPCTSTR pcszColorName, COLORREF clrDefault)
{
	if (this == NULL)
		return clrDefault;

	COLORREF clrColor = COLORREF_NULL;

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		clrColor = pImages->GetImageColor(_T("DatePicker"), pcszColorName);
	}

	if (clrColor == COLORREF_NULL)
	{
		clrColor = clrDefault;
	}

	return clrColor;
}