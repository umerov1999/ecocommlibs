// XTPDatePickerPaintManager.h: interface for the CXTPDatePickerPaintManager class.
//
// This file is a part of the XTREME CALENDAR MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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
#if !defined(_XTPDATEPICKERPAINTMANAGER_H__)
#define _XTPDATEPICKERPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPDatePickerControl;
class CXTPDatePickerItemMonth;
class CXTPDatePickerItemDay;
class CXTPWinThemeWrapper;

// ----------------------------------------------------------------------
// Summary:
//     Class CXTPDatePickerPaintManager handles most of the drawing
//     activities for the DatePicker control.
// Remarks:
//     Utility class, handles most of the drawing activities. It stores
//     all settings needed by the control to perform drawing operations :
//     fonts, colors, styles for all others classes of control. It also
//     implements all functions for drawing typical graphical primitives,
//     functions that directly work with the device context. Can be
//     \overridden to provide another look and feel for the control.
//     Thus, you have an easy way to change the "skin" of your control.
//     Provide your own implementation of CXTPDatePickerPaintManager and do
//     not change the functionality all other classes of the control.<p/>
// Create a DatePickerPaintManager by calling its constructor.
//     Furthermore, call the "get" and "set" functions to change the
//     settings as needed.
// ----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPDatePickerPaintManager : public CXTPCmdTarget
{
	friend class CXTPDatePickerControl;
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also: RefreshMetrics()
	//-----------------------------------------------------------------------
	CXTPDatePickerPaintManager();

	// -------------------------------------
	// Summary:
	//     Default paint manager destructor.
	// Remarks:
	//     Handles member item deallocation.
	// -------------------------------------
	virtual ~CXTPDatePickerPaintManager();

	// -------------------------------------------------------------------
	// Summary:
	//     This member function performs initialization for the Paint
	//     Manager.
	// Remarks:
	//     Initializes all drawing defaults (fonts, colors, etc.). Most of
	//     defaults are system defaults.
	// -------------------------------------------------------------------
	virtual void RefreshMetrics();

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function is called to fill the control's background.
	// Parameters:
	//     pDC :   Pointer to a valid device context.
	// rcClient :  A CRect that contains the control's client area rectangle
	//                 coordinates.
	// ---------------------------------------------------------------------
	virtual void DrawBackground(CDC* pDC, CRect rcClient);

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function draws a border around the control.
	// Parameters:
	//      pDC :       Pointer to a valid device context.
	//  pControl :  Pointer to a CXTPDatePickerControl object.
	//      rcClient :  A CRect that contains the control's client area rectangle
	//                 coordinates.
	//      bDraw    -  TRUE to draw else Adjust client rectangle.
	// ---------------------------------------------------------------------
	virtual void DrawBorder(CDC* pDC, const CXTPDatePickerControl* pControl, CRect& rcClient, BOOL bDraw);

	// ----------------------------------------------------------------------------------
	// Summary:
	//     This member function draws scrolling triangles on the highest
	//     bounded month headers.
	// Parameters:
	//  pDC : Pointer to a valid device context.
	//  rcSpot : A CRect object that contains the coordinates of the triangle spot area.
	//  bLeftDirection : A BOOL that is used to determine the triangle direction. TRUE
	//                  the left scrolling triangle is drawn.FALSE the right
	//                  scrolling triangle is drawn.
	//  clrColor : The triangle color
	//  bYears : BOOL flag to show or hide arrow to scroll by years
	// ----------------------------------------------------------------------------------
	virtual void DrawScrollTriangle(CDC* pDC, CRect rcSpot, BOOL bLeftDirection,
		COLORREF clrColor = 0, BOOL bYears = FALSE);

	// -------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the control's background
	//     color.
	// Returns:
	//     Color used for filling the controls client rectangle.
	// -------------------------------------------------------------------
	virtual COLORREF GetControlBackColor();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the background color of the
	//     standard day item.
	// Returns:
	//     A COLORREF object that contains the background color of the
	//     standard day item.
	// ----------------------------------------------------------------------
	virtual COLORREF GetDayBackColor();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the color of the standard
	//     day item.
	// Returns:
	//     A COLORREF object that contains the color of the standard day
	//     item.
	// --------------------------------------------------------------------
	virtual COLORREF GetDayTextColor();


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a pointer to the font for
	//     the standard day item.
	// Example:
	// <code>
	// // Get the font and extract a LOGGFONT structure.
	// LOGFONT lf;
	// GetDayTextFont()-\>GetLogFont(&lf);
	// // Create new italic font.
	// lf.lfItalic = 1;
	// CFont fontItalic;
	// Italic.CreateFontIndirect(&lf);
	// </code>
	// Returns:
	//     A pointer to a CFont object that contains the standard day item
	//     font.
	//-----------------------------------------------------------------------
	virtual CFont* GetDayTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a pointer to the font for
	//     the bolded day item.
	// Returns:
	//     A pointer to a CFont object that contains the bold day item font.
	//-----------------------------------------------------------------------
	virtual CFont* GetDayTextFontBold();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the background color of the
	//     non-month day item.
	// Returns:
	//     A COLORREF object that contains the background color of the non
	//     month day item.
	// ----------------------------------------------------------------------
	virtual COLORREF GetNonMonthDayBackColor();

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the color of the non-month
	//     day item.
	// Returns:
	//     A COLORREF object that contains the color of the non-month day
	//     item.
	// ---------------------------------------------------------------------
	virtual COLORREF GetNonMonthDayTextColor();

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the font of the non-month
	//     day item.
	// Example:
	// <code>
	// // Get the font and extract a LOGGFONT structure.
	// LOGFONT lf;
	// GetNonMonthDayTextFont-\>GetLogFont(&lf);
	// // Create new old font
	// lf.lfWeight = FW_BOLD;
	// CFont fontBold;
	// fntBold.CreateFontIndirect(&lf);
	// </code>
	// Returns:
	//     Pointer to a CFont object that contains the font of the non-month
	//     day item.
	// ---------------------------------------------------------------------
	CFont* GetNonMonthDayTextFont();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the background color of the
	//     selected day item.
	// Returns:
	//     A COLORREF object that contains the background color of the
	//     selected day item.
	// ----------------------------------------------------------------------
	virtual COLORREF GetSelectedDayBackColor();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function obtains the color of the selected day item.
	// Returns:
	//     A COLORREF object that contains the color of the selected day
	//     item.
	// --------------------------------------------------------------------
	virtual COLORREF GetSelectedDayTextColor();

	// ------------------------------------------------------------------
	// Summary:
	//     This member function obtains the background color of the month
	//     header area.
	// Returns:
	//     A COLORREF object containing the background color of the month
	//     header area.
	// ------------------------------------------------------------------
	virtual COLORREF GetMonthHeaderBackColor();

	// -------------------------------------------------------------------
	// Summary:
	//     This member function obtains the text color of the month header
	//     area.
	// Returns:
	//     A COLORREF object containing the text color of the month header
	//     area.
	// -------------------------------------------------------------------
	virtual COLORREF GetMonthHeaderTextColor();

	// ------------------------------------------------------------------
	// Summary:
	//     This member function obtains the font of the month header area
	//     text.
	// Returns:
	//     A CFont object that contains the font of the month header area
	//     text.
	// ------------------------------------------------------------------
	CFont* GetHeaderTextFont();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function returns the background color of the days of
	//     weeks area.
	// Returns:
	//     A COLORREF object containing the background color of the days of
	//     weeks area.
	// --------------------------------------------------------------------
	virtual COLORREF GetDaysOfWeekBackColor();

	// --------------------------------------------------------------------------
	// Summary:
	//     This member function obtains the text color of the days of weeks area.
	// Returns:
	//     A COLORREF object containing the text color of the days of weeks
	//     area.
	// --------------------------------------------------------------------------
	virtual COLORREF GetDaysOfWeekTextColor();

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function obtains the font of the days of weeks area
	//     text.
	// Returns:
	//     A pointer to a CFont object that contains the font of the days of
	//     weeks area text.
	// ---------------------------------------------------------------------
	CFont* GetDaysOfWeekTextFont();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function obtains the background color of the week
	//     numbers area.
	// Returns:
	//     A COLORREF object that contains the background color of the week
	//     numbers area.
	// --------------------------------------------------------------------
	virtual COLORREF GetWeekNumbersBackColor();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the text color of the week numbers
	//     items.
	// Returns:
	//     A COLORREF object that contains the text color of the week numbers
	//     items.
	// ----------------------------------------------------------------------
	virtual COLORREF GetWeekNumbersTextColor();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function obtains the font of the week numbers items.
	// Returns:
	//     A pointer to a CFont object that contains the font of the week
	//     numbers items.
	// --------------------------------------------------------------------
	CFont* GetWeekNumbersTextFont();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the background color of the pop-up
	//     month list control.
	// Returns:
	//     A COLORREF object that contains the background color of the pop-up
	//     month list control.
	// ----------------------------------------------------------------------
	virtual COLORREF GetListControlBackColor();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the text color of the pop-up month
	//     list control.
	// Returns:
	//     A COLORREF object that contains the text color of the pop-up month
	//     list control.
	// ----------------------------------------------------------------------
	virtual COLORREF GetListControlTextColor();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function obtains the font of the pop-up month list
	//     control.
	// Returns:
	//     A pointer to a CFont object that contains the font of the pop-up
	//     month list control.
	// --------------------------------------------------------------------
	CFont* GetListControlTextFont();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the highlighting color of the "Today"
	//     item.
	// Returns:
	//     A COLORREF object that contains the highlighting color of the
	//     "Today" item.
	// ----------------------------------------------------------------------
	virtual COLORREF GetHighlightTodayColor();

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function sets the button's font.
	// Parameters:
	//     fontButton :  A CFont pointer to the button's font object.
	// Remarks:
	//     Call this function to set the new button's font from the provided
	//     CFont object.
	// See Also:
	//     SetButtonFontIndirect(LOGFONT*), GetButtonFont()
	// ---------------------------------------------------------------------
	void SetButtonFont(CFont* fontButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the button's font using a LOGFONT
	//     structure.
	// Parameters:
	//     lFont - Pointer to the LOGFONT structure.
	// Remarks:
	//     This member function creates and sets the new button's font
	//     from the given LOGFONT structure.
	// See Also: SetButtonFont(CFont*), GetButtonFont()
	//-----------------------------------------------------------------------
	void SetButtonFontIndirect(LOGFONT* lFont);

	// --------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the button's font object.
	// Returns:
	//     A pointer to the button's CFont object.
	// See Also:
	//     SetButtonFontIndirect(LOGFONT* lFont), SetButtonFont(CFont*
	//     fontButton)
	// --------------------------------------------------------------------
	CFont* GetButtonFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the control's default font
	//     using a LOGFONT structure.
	// Parameters:
	//     lFont - Pointer to the LOGFONT structure.
	// Remarks:
	//     This member function creates and sets the control's new default
	//     font from the given LOGFONT structure.
	//-----------------------------------------------------------------------
	void SetFontIndirect(LOGFONT* lFont);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the control's buttons.
	// Parameters:
	//     pDC :           Pointer to a valid device context.
	//     rcButton :      A CRect that contains the button area bounding
	//                     rectangle coordinates.
	//     strButton :     A CString that contains the string of the button's
	//                     text.
	//     bIsDown :       A BOOL that determines if the button is down or up.
	//     bIsHighLight :  A BOOL that determines if the button is highlighted.
	// Remarks:
	//     Use this member function to draw buttons in the appropriate state.
	//     The state depends on the bIsDown parameter, the button may be
	//     drawn pressed or released. Furthermore, depending on the
	//     bIsHighLight, the button may be drawn with highlighted borders.
	// Example:
	// <code>
	// for (int i = 0; i \< GetButtonCount(); i++)
	// {
	//     CXTPDatePickerButton* pButton = GetButton(i);
	//     if (pButton-\>m_bVisible)
	//     DrawButton(pDC, pButton-\>m_rcButton, pButton-\>m_strCaption,
	//     pButton-\>m_bPressed, pButton-\>m_bHighlight);
	// }
	// </code>
	// ----------------------------------------------------------------------
	virtual void DrawButton(CDC* pDC, const CRect& rcButton, const CString& strButton,
							BOOL bIsDown, BOOL bIsHighLight);

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw a single month day in the month
	//     area.
	// Parameters:
	//     pDC      : Pointer to a valid device context.
	//     pDay     : Pointer to day object.
	// Returns:
	//     TRUE if the drawing is successful, FALSE else.
	// Remarks:
	//     It is called from CXTPDatePickerItemDay implementation.
	//     You can override it to customize drawing.
	// --------------------------------------------------------------------
	virtual BOOL DrawDay(CDC* pDC, CXTPDatePickerItemDay* pDay);

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the month header in the month
	//     area.
	// Parameters:
	//     pDC      : Pointer to a valid device context.
	//     pMonth   : Pointer to month object.
	// Remarks:
	//     It is called from CXTPDatePickerItemMonth implementation.
	//     You can override it to customize drawing.
	// --------------------------------------------------------------------
	virtual void DrawMonthHeader(CDC* pDC, CXTPDatePickerItemMonth* pMonth);

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the week numbers in the month
	//     area.
	// Parameters:
	//     pDC      : Pointer to a valid device context.
	//     pMonth   : Pointer to month object.
	// Remarks:
	//     It is called from CXTPDatePickerItemMonth implementation.
	//     You can override it to customize drawing.
	// --------------------------------------------------------------------
	virtual void DrawWeekNumbers(CDC* pDC, CXTPDatePickerItemMonth* pMonth);

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the days of the week in the
	//     month header.
	// Parameters:
	//     pDC      : Pointer to a valid device context.
	//     pMonth   : Pointer to month object.
	// Remarks:
	//     It is called from CXTPDatePickerItemMonth implementation.
	//     You can override it to customize drawing.
	// --------------------------------------------------------------------
	virtual void DrawDaysOfWeek(CDC* pDC, CXTPDatePickerItemMonth* pMonth);

	// ----------------------------------------------------------------
	// Summary:
	//     This member function is used to calculate the day's bounding
	//     rectangle coordinates.
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// Remarks:
	//     This member function calculates the day's bounding rectangle
	//     coordinates for the given device context current day's font.
	// Returns:
	//     A CSize object containing the day's bounding rectangle
	//     coordinates.
	// ----------------------------------------------------------------
	virtual CSize CalcDayRect(CDC* pDC);

	// -------------------------------------------------------------------
	// Summary:
	//     This member function calculates the month's header bounding
	//     rectangle coordinates.
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// Remarks:
	//     This member function calculates the month's header bounding
	//     rectangle coordinates for the given device context's current
	//     header font.
	// Returns:
	//     A CSize object containing the month's header bounding rectangle
	//     coordinates.
	// -------------------------------------------------------------------
	virtual CSize CalcMonthHeaderRect(CDC* pDC);

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function calculates the "day of week names" bounding
	//     rectangle coordinates.
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// Remarks:
	//     Calculates the "day of week names" bounding rectangle coordinates
	//     for the given device context's current header font.
	// Returns:
	//     A CSize object that contains the "day of week names" rectangle
	//     coordinates.
	// ---------------------------------------------------------------------
	virtual CSize CalcDayOfWeekRect(CDC* pDC);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function calculates the "week numbers" bounding
	//     rectangle coordinates.
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// Remarks:
	//     Calculates the "week numbers" bounding rectangle coordinates for
	//     the given device context's current header font.
	// Returns:
	//     A CSize object that contains the "week numbers" bounding rectangle
	//     coordinates.
	// ----------------------------------------------------------------------
	virtual CSize CalcWeekNumbersRect(CDC* pDC);

	// ------------------------------------------------------------------
	// Summary:
	//     This member function calculates the month's bounding rectangle
	//     coordinates.
	// Parameters:
	//     pDC :  Pointer to a valid device context.
	// Remarks:
	//     Calculates the Month's bounding rectangle coordinates for the
	//     given device context's current header font.
	// Returns:
	//     A CSize object that contains the month's bounding rectangle
	//     coordinates.
	// ------------------------------------------------------------------
	virtual CSize CalcMonthRect(CDC* pDC);

	// --------------------------------------------------------------------------------
	// Summary:
	//     This member function is used to switch "Themes".
	// Parameters:
	//     bEnableTheme :  A BOOL that is used as a flag to indicate if themes are
	//                     enabled. Set to TRUE if Themes are enabled, FALSE Themes not enabled.
	// --------------------------------------------------------------------------------
	virtual void EnableTheme(BOOL bEnableTheme);

	// --------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the back cursor used for
	//     weeks selection.
	// Returns:
	//     A handle of the loaded cursor.
	// --------------------------------------------------------------------
	virtual HCURSOR GetCursorBack();

protected:
	// --------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw a black triangle.
	// Parameters:
	//     pDC : Pointer to a valid device context.
	//     pt0 : Apex coordinate.
	//     pt1 : Apex coordinate.
	//     pt2 : Apex coordinate.
	//     clrColor : The triangle color
	// --------------------------------------------------------------------
	virtual void Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clrColor = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain current paint theme.
	// Returns:
	//     A paint theme ID from enum XTPCalendarTheme.
	//-----------------------------------------------------------------------
	XTPCalendarTheme GetPaintTheme() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set current paint theme.
	// Parameters:
	//     ePaintTheme - A paint theme ID from enum XTPCalendarTheme.
	//-----------------------------------------------------------------------
	void SetPaintTheme(XTPCalendarTheme ePaintTheme);

public:
	CSize m_szDayMargin;

	CXTPPaintManagerColor m_clrControlBack;     // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // background color of the control.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrHighlightToday;  // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // color for highlighting the "Today" item.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrSelectedDayBack; // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // background color of the selected day item.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrMonthHeaderBack; // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // background color of the month header area.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrMonthHeaderText; // ------------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the text
	                                            // color of the month header area.
	                                            // ------------------------------------------------------------------

	CXTPPaintManagerColor m_clrSelectedDayText; // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // color of the selected day item.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrMonthBorder;
	CXTPPaintManagerColor m_clrMonthHeaderButton;


	CXTPPaintManagerColor m_clrDayBack;         // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // background color of the normal day item.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrDayText;         // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // color of the usual day item.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrNonMonthDayBack; // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // background color of the non month day item.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrNonMonthDayText; // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // color of the non month day item.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrDaysOfWeekBack;  // --------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used for the
	                                            // background color of the days of weeks area.
	                                            // --------------------------------------------------------------

	CXTPPaintManagerColor m_clrDaysOfWeekText;  // ------------------------------------------------------------------
	                                            // This member variable is a COLORFEF object that is used as the text
	                                            // color of the days of weeks area.
	                                            // ------------------------------------------------------------------

	CXTPPaintManagerColor m_clrWeekNumbersBack; // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // background color of the week numbers area.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrWeekNumbersText; // ------------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the text
	                                            // color of the week numbers items.
	                                            // ------------------------------------------------------------------

	CXTPPaintManagerColor m_clrButtonText;      // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // color for the button text.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrButtonFace;      // -------------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the basic
	                                            // color for buttons.
	                                            // -------------------------------------------------------------------


	CXTPPaintManagerColor m_clrListControlBack; // -------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the
	                                            // background color of the pop-up month list control.
	                                            // -------------------------------------------------------------

	CXTPPaintManagerColor m_clrListControlText; // ------------------------------------------------------------------
	                                            // This member variable is a COLORREF object that is used as the text
	                                            // color of the pop-up month list control.
	                                            // ------------------------------------------------------------------

protected:



	COLORREF m_clrStaticBorder;     // Static border color
	COLORREF m_clr3DShadow;         // -----------------------------------------------------------------
	                                // This member variable is a COLORREF object that is used to specify
	                                // the color for shadow areas.
	                                // -----------------------------------------------------------------
	COLORREF m_clr3DLight;          // -----------------------------------------------------------------
	                                // This member variable is a COLORREF object that is used to specify
	                                // the color for light areas
	                                // -----------------------------------------------------------------

	COLORREF m_clr3DFace;           // -------------------------------------------------------------
	                                // This member variable is a COL    ORREF object that is used as the
	                                // color for the button text.
	                                // -------------------------------------------------------------

	COLORREF m_clr3DText;           // -------------------------------------------------------------------
	                                // This member variable is a COLORREF object that is used as the basic
	                                // color for buttons.
	                                // -------------------------------------------------------------------


	CFont m_fontDay;                // ----------------------------------------------------------------
	                                // This member variable is a CFont object that is used for the font
	                                // \of the usual day item.
	                                // ----------------------------------------------------------------

	CFont m_fontDayBold;            // ----------------------------------------------------------------
	                                // This member variable is a CFont object that is used for the font
	                                // \of the bold day item.
	                                // ----------------------------------------------------------------

	CFont m_fontNonMonthDay;        // ---------------------------------------------------------------
	                                // This member variable is a CFont object that is used as the font
	                                // for the non month day item.
	                                // ---------------------------------------------------------------
	CFont m_fontHeader;             // ---------------------------------------------------------------
	                                // This member variable is a CFont object that is used as the font
	                                // for the header text.
	                                // ---------------------------------------------------------------
	CFont m_fontDaysOfWeek;         // ---------------------------------------------------------------
	                                // This member variable is a CFont object that is used as the font
	                                // for the days of weeks text.
	                                // ---------------------------------------------------------------
	CFont m_fontWeekNumbers;        // ---------------------------------------------------------------
	                                // This member variable is a CFont object that is used as the font
	                                // for the week number items text.
	                                // ---------------------------------------------------------------
	CFont m_fontListControl;        // ---------------------------------------------------------------
	                                // This member variable is a CFont object that is used as the font
	                                // for the pop-up month list control.
	                                // ---------------------------------------------------------------
	CFont m_fontButton;             // ---------------------------------------------------------------
	                                // This member variable is a CFont object that is used as the font
	                                // for the button text.
	                                // ---------------------------------------------------------------

	BOOL m_bEnableTheme;            // ---------------------------------------------------------------
	                                // This member variable is a BOOL that is used to indicate if Luna
	                                // colors are enabled. The valid values are TRUE to enable Luna
	                                // colors, otherwise FALSE.
	                                // ---------------------------------------------------------------

	CXTPWinThemeWrapper* m_themeButton; // -----------------------------------------------------------------
									   // This member variable is a CXTPWinThemeWrapper object that is used
									   // as the theme wrapper for the Paint Manager.
									   // -----------------------------------------------------------------



	HCURSOR m_hCursorBack;          // Back cursor for week selection

	XTPCalendarTheme m_nPaintTheme; // Store the Paint Theme;

protected:

	friend class CXTPDatePickerControl;
};




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


AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetControlBackColor() {
	return m_clrControlBack;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetDayBackColor() {
	return m_clrDayBack;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetDayTextColor() {
	return m_clrDayText;
}

AFX_INLINE CFont* CXTPDatePickerPaintManager::GetDayTextFont() {
	return &m_fontDay;
}

AFX_INLINE CFont* CXTPDatePickerPaintManager::GetDayTextFontBold() {
	return &m_fontDayBold;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetNonMonthDayBackColor() {
	return m_clrNonMonthDayBack;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetNonMonthDayTextColor() {
	return m_clrNonMonthDayText;
}

AFX_INLINE CFont* CXTPDatePickerPaintManager::GetNonMonthDayTextFont() {
	return &m_fontNonMonthDay;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetSelectedDayBackColor() {
	return m_clrSelectedDayBack;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetSelectedDayTextColor() {
	return m_clrSelectedDayText;
}


AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetMonthHeaderBackColor() {
	return m_clrMonthHeaderBack;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetMonthHeaderTextColor() {
	return m_clrMonthHeaderText;
}

AFX_INLINE CFont* CXTPDatePickerPaintManager::GetHeaderTextFont() {
	return &m_fontHeader;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetDaysOfWeekBackColor() {
	return m_clrDaysOfWeekBack;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetDaysOfWeekTextColor() {
	return m_clrDaysOfWeekText;
}

AFX_INLINE CFont* CXTPDatePickerPaintManager::GetDaysOfWeekTextFont() {
	return &m_fontDaysOfWeek;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetWeekNumbersBackColor() {
	return m_clrWeekNumbersBack;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetWeekNumbersTextColor() {
	return m_clrWeekNumbersText;
}

AFX_INLINE CFont* CXTPDatePickerPaintManager::GetWeekNumbersTextFont() {
	return &m_fontWeekNumbers;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetListControlBackColor() {
	return m_clrListControlBack;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetListControlTextColor() {
	return m_clrListControlText;
}

AFX_INLINE CFont* CXTPDatePickerPaintManager::GetListControlTextFont() {
	return &m_fontListControl;
}

AFX_INLINE COLORREF CXTPDatePickerPaintManager::GetHighlightTodayColor() {
	return m_clrHighlightToday;
}

AFX_INLINE CFont* CXTPDatePickerPaintManager::GetButtonFont(){
	return &m_fontButton;
}

AFX_INLINE void CXTPDatePickerPaintManager::EnableTheme(BOOL bEnableTheme) {
	m_bEnableTheme = bEnableTheme;
	RefreshMetrics();
}

AFX_INLINE HCURSOR CXTPDatePickerPaintManager::GetCursorBack() {
	return m_hCursorBack;
}

AFX_INLINE XTPCalendarTheme CXTPDatePickerPaintManager::GetPaintTheme() const {
	return m_nPaintTheme;
}

#endif // !defined(_XTPDATEPICKERPAINTMANAGER_H__)
