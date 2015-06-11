// XTPDatePickerOffice2007Theme.h: interface for the CXTPDatePickerOffice2007Theme class.
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
#if !defined(_XTPDATEPICKEROFFICE2007THEME_H__)
#define _XTPDATEPICKEROFFICE2007THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------------------------------------------------------------------------
// Summary:
//     Class CXTPDatePickerThemeOffice2007 is derived from CXTPDatePickerTheme
//     is implement Office 2007 style for DatePicker control.
// Example:
// <code>
//      if (m_bThemeOffice2007)
//      {
//          CXTPDatePickerThemeOffice2007* pTheme2007 = new CXTPDatePickerThemeOffice2007;
//          m_wndDatePicker.SetTheme(pTheme2007);
//      }
//      else
//      {
//          m_wndDatePicker.SetTheme(NULL);
//      }
// </code>
// See Also:
//     CXTPDatePickerThemeOffice2007
//---------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPDatePickerThemeOffice2007 : public CXTPDatePickerPaintManager
{
public:
	CXTPDatePickerThemeOffice2007();
	virtual ~CXTPDatePickerThemeOffice2007();

public:
	virtual void RefreshMetrics();

	virtual void DrawButton(CDC* pDC, const CRect& rcButton, const CString& strButton, BOOL bIsDown, BOOL bIsHighLight);
protected:
	void SetControl(CXTPDatePickerControl*  pControl);


protected:
	COLORREF GetResourceColor(LPCTSTR pcszColorName, COLORREF clrDefault);
};

#endif // !defined(_XTPDATEPICKEROFFICE2007THEME_H__)