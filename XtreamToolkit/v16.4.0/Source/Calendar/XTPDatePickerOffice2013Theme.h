// XTPDatePickerOffice2013Theme.h: interface for the CXTPDatePickerOffice2013Theme class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(_XTPDATEPICKEROFFICE2013THEME_H__)
#define _XTPDATEPICKEROFFICE2013THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPDatePickerThemeOffice2013 : public CXTPDatePickerPaintManager
{
public:
	CXTPDatePickerThemeOffice2013();
	virtual ~CXTPDatePickerThemeOffice2013();

protected:

	CXTPPaintManagerColorGradient m_clrBackground;

	static void MakeStringUpper(CString &str);

public:
	virtual void RefreshMetrics();
	virtual void DrawBackground(CDC* pDC, CRect rcClient);
	virtual void DrawWeekNumbers(CDC* pDC, CXTPDatePickerItemMonth* pMonth);
	virtual void DrawDaysOfWeek(CDC* pDC, CXTPDatePickerItemMonth* pMonth);

	virtual void DrawBorder(CDC* pDC, const CXTPDatePickerControl* pControl, CRect& rcClient, BOOL bDraw);

	virtual BOOL DrawDay(CDC* pDC, CXTPDatePickerItemDay* pDay);
};

#endif // !defined(_XTPDATEPICKEROFFICE2013THEME_H__)