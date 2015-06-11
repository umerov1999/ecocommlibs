// XTPCalendarControlPaintManager.h: interface for the CXTPCalendarControlPaintManager class.
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
#if !defined(_XTPCALENDARPAINTMANAGER_H__)
#define _XTPCALENDARPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCalendarTimeLineViewTimeScale;
class CXTPCalendarTimeLineViewGroup;
class CXTPCalendarTimeLineViewEvent;
class CXTPCalendarTimeLineView;
class CXTPResourceImage;
class CXTPCalendarControl;
class CXTPCalendarDayView;
class CXTPCalendarDayViewDay;
class CXTPCalendarDayViewGroup;
class CXTPCalendarMonthView;
class CXTPCalendarWeekView;
class CXTPCalendarDayViewEvent;
class CXTPCalendarMonthViewEvent;
class CXTPCalendarWeekViewEvent;
class CXTPCalendarViewEvent;
class CXTPCalendarDayViewTimeScale;
class CXTPCalendarFlagsSet_imp;
class CXTPCalendarViewPartBrushValue;

struct XTP_CALENDAR_DAYVIEWCELL_PARAMS;


//===========================================================================
// Summary:
//     This helper macro is used to declare paint manager drawing parts.
//         <b>theClass</b> - Part name.
//         <b>parentClass</b> - Parent part name.
//     There are two paint manager methods available to access declared
//          part object:
//          <b> CPartName* GetPartName();           </b>
//          <b> void SetPartName(CPartName* pPart); </b>
//              Where PartName is the name of the declared part.
//
// Example:
// <code>
// declaration in the paint manager class
// BEGIN_VIEW_PART(PartClass1, ParentPartClass1)
// ...
// END_VIEW_PART(PartClass1)
//
// // in the implementation
// CPaintManagerClassName::CPartClass1* pPart1 = pPainManObj->GetPartClass1();
// </code>
// See Also: END_VIEW_PART, CXTPCalendarPaintManager
//===========================================================================
#define BEGIN_VIEW_PART(theClass, parentClass)
//{{AFX_CODEJOCK_PRIVATE
#undef BEGIN_VIEW_PART
#define BEGIN_VIEW_PART(theClass, parentClass)\
	class C##theClass  : public C##parentClass\
	{\
	public:\
		typedef C##parentClass TBase; \
		C##theClass(CXTPCalendarViewPart* pParentPart = NULL)\
			: C##parentClass(pParentPart)\
		{\
			_Init(); \
		}
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This helper macro is used to declare paint manager drawing parts.
//         theClass    - Part name.
//
// See Also: BEGIN_VIEW_PART, CXTPCalendarPaintManager
//===========================================================================
#define END_VIEW_PART(theClass)
//{{AFX_CODEJOCK_PRIVATE
#undef END_VIEW_PART
#define END_VIEW_PART(theClass)\
	};\
	friend class C##theClass;\
	protected:\
		C##theClass* m_p##theClass;\
	public:\
		C##theClass* Get##theClass() const {\
			return m_p##theClass;\
		}\
		void Set##theClass(C##theClass* p##theClass)\
		{\
			POSITION pos = m_lstViewParts.Find(m_p##theClass);\
			ASSERT(pos);\
			m_lstViewParts.RemoveAt(pos);\
			delete m_p##theClass;\
			m_p##theClass = p##theClass;\
			m_lstViewParts.AddTail(p##theClass);\
			p##theClass->m_pPaintManager = this;\
		}
//}}AFX_CODEJOCK_PRIVATE


/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarPaintManager command target

//===========================================================================
// Summary:
//     Utility class, handles most of the drawing activities.
// Remarks:
//     It stores all settings, needed by control to perform drawing operations :
//     fonts, colors, styles for all others classes of control. It also implements
//     all functions for drawing typical graphical primitives, functions that
//     directly work with device context.
//     Can be overridden to provide another look and feel for the control.
//     Thus, you have an easy way to change the "skin" of your control. Just provide
//     your own implementation of CXTPCalendarControlPaintManager and don't
//     touch the functionality of any of the other control classes.
//
//          Create a CXTPCalendarControlPaintManager by calling its constructor.
//          Furthermore, you can call get and set functions to change
//          settings as needed.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarPaintManager : public CXTPCmdTarget
{
	friend class CXTPCalendarViewPart;
public:
	//{{AFX_CODEJOCK_PRIVATE
	//-----------------------------------------------------------------------
	// Summary:
	//     Paint manager's ControlPart is used as parent for the
	//     other parts of the control.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(ControlPart, XTPCalendarViewPart)
		//-------------------------------------------------------------------
		// Summary:
		//     Performs refreshing of graphical related parameters from
		//     system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();
	END_VIEW_PART(ControlPart)

	//-----------------------------------------------------------------------
	//## Day View related drawing.
	//-----------------------------------------------------------------------

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw the time scale header,
	//     'Now' line, and the expand signs for the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewTimeScaleHeaderPart, XTPCalendarViewPart)

		COLORREF m_clrBorder;

		//-------------------------------------------------------------------
		// Summary:
		//     Performs refreshing of graphical related parameters from
		//     system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the time scale header.
		// Parameters:
		//     pDC     - Pointer to a valid device context.
		//     pView   - A CXTPCalendarDayView pointer that contains the Day
		//               View object.
		//     rc      - The time scale header's bounding rectangle.
		//     strText - A CString object that contains the header text.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarDayView* pView, CRect rc, CString strText);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the 'Now' line.
		// Parameters:
		//     pDC       - Pointer to a valid device context.
		//     pView     - A CXTPCalendarDayView pointer to the Day View object.
		//     rc        - A CRect that contains the Time scale bounding rectangle.
		//     y         - An int that contains the 'Now' line position.
		//     bDrawBk   - If FALSE, then draw only the line. If TRUE,
		//                 then draw the line and the gradient fill.
		//-------------------------------------------------------------------
		virtual void DrawNowLine(CDC* pDC, CXTPCalendarDayView* pView, CRect rc,
								 int y, BOOL bDrawBk = FALSE);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw expand signs.
		// Parameters:
		//     pDC       - Pointer to a valid device context.
		//     pView     - A CXTPCalendarDayView pointer to the Day View object.
		//     rcTSHours - A CRect that contains the Time scale hours rectangle.
		//-------------------------------------------------------------------
		virtual void DrawExpandSigns(CDC* pDC, CXTPCalendarDayView* pView, const CRect& rcTSHours);
	END_VIEW_PART(DayViewTimeScaleHeaderPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw time scale time cells
	//     for the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewTimeScaleCellPart, DayViewTimeScaleHeaderPart)
		CXTPCalendarViewPartFontValue m_fntTimeText; // Time text font.
		int m_nHourWidth; // Width of hour part of the text in pixels.
		int m_nMinWidth; // Width of minutes (am/pm) part of the text in pixels.

		COLORREF m_clrBorder;

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to refresh the graphical
		// related parameters using system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to adjust the time text font size.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     rcCell - A CRect object that contains the time scale hour cell
		//              bounding rectangle.
		//-------------------------------------------------------------------
		void AdjustTimeFont(CDC* pDC, CRect rcCell);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to adjust the time text font size.
		// Parameters:
		//     pDC     - Pointer to a valid device context.
		//     strHour - A CString object that contains the Hour text.
		//     strMin  - A CString object that contains the Minutes text.
		//     nHourHeight - A hour big text height.
		//     nWidth  - An int that returns the width of time scale with
		//               provided hour and minutes texts.
		//-------------------------------------------------------------------
		void CalcWidth(CDC* pDC, const CString& strHour, const CString& strMin, int nHourHeight, int& nWidth);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw background of the time scale header
		// Parameters:
		//     pDC             - Pointer to a valid device context.
		//     rc              - A CRect object that contains bounding rectangle.
		//-------------------------------------------------------------------
		//virtual void DrawBackground(CDC* pDC, CRect rc);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the hour cell.
		// Parameters:
		//     pDC             - Pointer to a valid device context.
		//     rc              - A CRect object that contains the time scale
		//                       hour cell bounding rectangle.
		//     strText         - A CString object that contains the time text.
		//     bFillBackground - A BOOL. If TRUE, then fill the background before drawing.
		//                       If FALSE, then do not fill the background before drawing.
		//-------------------------------------------------------------------
		virtual void DrawHourCell(CDC* pDC, CRect rc, CString strText, BOOL bFillBackground = TRUE, BOOL bCurrentTime = FALSE);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the big hour cell.
		// Parameters:
		//     pDC             - Pointer to a valid device context.
		//     rc              - A CRect object that contains the time scale
		//                       hour cell bounding rectangle.
		//     strHour         - A CString object that contains the Hour text.
		//     strMin          - A CString object that contains the Minutes text.
		//     nRowPerHour     - An int that contains the Rows per hour.
		//     bFillBackground - A BOOL. If TRUE, then fill the background before drawing.
		//                       If FALSE, then do not fill the background before drawing.
		//-------------------------------------------------------------------
		virtual void DrawBigHourCell(CDC* pDC, CRect rc, CString strHour, CString strMin,
									 int nRowPerHour, BOOL bFillBackground = TRUE, BOOL bCurrentTime = FALSE);
	END_VIEW_PART(DayViewTimeScaleCellPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used as the base class for the parts
	//     which draw different kinds of no all-day-cells (work/non-work)
	//     for the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewCellPart, XTPCalendarViewPart)
		CXTPPaintManagerColor m_clrBackgroundSlave; //Background secondary color.
		CXTPPaintManagerColor m_clrShadow; // Cell background color.
		CXTPPaintManagerColor m_clrHour;   // Cell border color.

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to refresh the graphical
		// related parameters using system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to fill out cell parameters with
		//     default values.
		// Parameters:
		//     pViewGroup  - [in] A CXTPCalendarDayViewGroup pointer to the Day View Group object.
		//     rCellParams - [in, out] A XTP_CALENDAR_DAYVIEWCELL_PARAMS object that contains
		//                   the cell's parameters.
		//-------------------------------------------------------------------
		virtual void GetParams(CXTPCalendarDayViewGroup* pViewGroup, XTP_CALENDAR_DAYVIEWCELL_PARAMS* rCellParams);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the day view with no
		//     all-day-cells.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     pViewGroup  - A CXTPCalendarDayViewGroup pointer to the Day View Group object.
		//     rc          - A CRect object that contains the cell's bounding rectangle.
		//     cellParams  - A XTP_CALENDAR_DAYVIEWCELL_PARAMS object that contains
		//                   the cell's parameters.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarDayViewGroup* pViewGroup,
							CRect rc, const XTP_CALENDAR_DAYVIEWCELL_PARAMS& cellParams);
	END_VIEW_PART(DayViewCellPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw a day view that contains
	//     no all day work time cells.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewWorkCellPart, DayViewCellPart)
		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to perform the refreshing of
		//     graphical related parameters from system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();
	END_VIEW_PART(DayViewWorkCellPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw "no all day non-work"
	//     time cells for the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewNonworkCellPart, DayViewCellPart)
		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to perform refreshing of
		//     graphical related parameters from system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();
	END_VIEW_PART(DayViewNonworkCellPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw days headers for
	//     the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayMonthViewHeaderPart, XTPCalendarViewPart)
		CXTPPaintManagerColor m_clrTopLeftBorder;   // Top and Left borders color.

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to perform refreshing of
		//     graphical related parameters from system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics()
		{
			m_clrTopLeftBorder.SetStandardValue(RGB(255, 255, 255));
		}
	END_VIEW_PART(DayMonthViewHeaderPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw days headers for
	//     the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewHeaderPart, DayMonthViewHeaderPart)
		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the day header.
		// Parameters:
		//     pDC      - Pointer to a valid device context.
		//     pViewDay - A CXTPCalendarDayViewDay pointer to the Day View
		//                object.
		//     rc       - A CRect object that contains the coordinates of
		//                the Header's bounding rectangle.
		//     strText  - A CString object that contains the Header text.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarDayViewDay* pViewDay, CRect rc,
							CString strText);

		virtual void RefreshMetrics();
	END_VIEW_PART(DayViewHeaderPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw Groups headers for
	//     the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewGroupHeaderPart, DayMonthViewHeaderPart)
		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the day header.
		// Parameters:
		//     pDC      - Pointer to a valid device context.
		//     pViewGroup - A CXTPCalendarDayViewGroup pointer to the Group
		//                  object.
		//     rc       - A CRect object that contains the coordinates of
		//                the Header's bounding rectangle.
		//     strText  - A CString object that contains the Header text.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarDayViewGroup* pViewGroup,
							CRect rc, CString strText);
	END_VIEW_PART(DayViewGroupHeaderPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw all day events area for
	//     the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewAllDayEventsPart, XTPCalendarViewPart)
		//-------------------------------------------------------------------
		// Summary:
		//     This member functions is used to perform refreshing of graphical
		//      related parameters from system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the all-day events area.
		// Parameters:
		//     pDC       - Pointer to a valid device context.
		//     pViewGroup - A CXTPCalendarDayViewGroup pointer to the Group object.
		//     rc        - A CRect object that contains the all-day events
		//                 area bounding rectangle coordinates.
		//     bSelected - A BOOL. TRUE if the draw area is selected. FALSE otherwise.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarDayViewGroup* pViewGroup,
							CRect rc, BOOL bSelected);
	END_VIEW_PART(DayViewAllDayEventsPart)

		//-----------------------------------------------------------------------
		// Summary:
		//     This paint manager part is used to draw all day events scroll buttons if there are too many
		//     all day events.
		//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewAllDayEventScrollIconsPart, XTPCalendarViewPart)
			//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw scroll icons in all day events area.
		// Parameters:
		//     pDC       - Pointer to a valid device context.
		//     pViewGroup - A CXTPCalendarDayViewGroup pointer to the Group object.
		//     bDrawUp - selects show\hide up scroll icon
		//     bDrawDown - selects show\hide down scroll icon
		//     rcIconUp - scroll up icon area
		//     rcIconDown - scroll down icon area
		//     bHighlightUp - selects if draw the icon in highlighted color
		//     bHighlightDown - selects if draw the icon in highlighted color
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarDayViewGroup* pViewGroup, BOOL bDrawUp, BOOL bDrawDown,
			const CRect& rcIconUp, const CRect& rcIconDown, BOOL bHighlightUp, BOOL bHighlightDown);
	END_VIEW_PART(DayViewAllDayEventScrollIconsPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw events for
	//     the Day View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(DayViewEventPart, XTPCalendarViewPart)
		//-------------------------------------------------------------------
		// Summary:
		//     Performs refreshing of graphical related parameters from
		//      system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();
		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw an event.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     pViewEvent  - A CXTPCalendarDayViewEvent pointer to the Day
		//                   Event View object.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarDayViewEvent* pViewEvent);
	END_VIEW_PART(DayViewEventPart)

	//-----------------------------------------------------------------------
	//## Month View related drawing
	//-----------------------------------------------------------------------

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw days grid and to fill
	//     days cells background for the Month View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(MonthViewGridPart, XTPCalendarViewPart)
		CXTPPaintManagerColor m_clrBackground2; // Second background color which is used to fill days background.

		//-------------------------------------------------------------------
		// Summary:
		//     Performs refreshing of graphical related parameters from
		//      system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the days grid and to fill
		//     the days cells background.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     pMonthView  - A CXTPCalendarMonthView pointer to the Month View object.
		//-------------------------------------------------------------------
		virtual void OnDrawGrid(CDC* pDC, CXTPCalendarMonthView* pMonthView);
	END_VIEW_PART(MonthViewGridPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw week days headers for
	//     the Month View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(MonthViewHeaderPart, DayMonthViewHeaderPart)
		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the week day header.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     pMonthView  - A CXTPCalendarMonthView pointer to the Month
		//                   View object.
		//     rc          - A CRect object that contains the Week Day header's
		//                   bounding rectangle coordinates.
		//     nCollIndex  - An int that contains the Week Day column index in the grid.
		//     strText     - A CString object that contains the formatted Week Day name.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarMonthView* pMonthView,
							CRect rc, int nCollIndex, CString strText);

		virtual void RefreshMetrics();

	END_VIEW_PART(MonthViewHeaderPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw events and day dates
	//     for the Month View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(MonthViewEventPart, XTPCalendarViewPart)
		COLORREF m_clrTextHighLightColor;       // Text color for the selected event.
		COLORREF m_clrMultiDayEventFrameColor;  // Multi-day event frame color.
		COLORREF m_clrClassicUnderline;         // Underline color for today day.

		CXTPPaintManagerColorGradient m_grclrClassicSelDay;// Gradient color for today day.

		//-------------------------------------------------------------------
		// Summary:
		//     Performs refreshing of graphical related parameters from
		//     system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw an event.
		// Parameters:
		//     pDC        - Pointer to a valid device context.
		//     pViewEvent - A CXTPCalendarMonthViewEvent pointer to the Month Event View object.
		//-------------------------------------------------------------------
		virtual void OnDrawEvent(CDC* pDC, CXTPCalendarMonthViewEvent* pViewEvent);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the week day header.
		// Parameters:
		//     pDC       - Pointer to a valid device context.
		//     rc        - A CRect object that contains the Day header's bounding
		//                 rectangle coordinates.
		//     bToday    - TRUE for today day, otherwise FALSE.
		//     bSelected - Is day selected.
		//     strText   - Formatted day date string.
		//-------------------------------------------------------------------
		virtual void OnDrawDayDate(CDC* pDC, CRect rc, BOOL bToday, BOOL bSelected, BOOL bFirstMonthDay, CString strText);
	END_VIEW_PART(MonthViewEventPart)

	//-----------------------------------------------------------------------
	//## Week view related drawing
	//-----------------------------------------------------------------------

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw week days grid and
	//     headers for the Week View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(WeekViewPart, XTPCalendarViewPart)
		COLORREF m_clrTextHighLightColor;   // Text color for the selected day header.
		COLORREF m_clrTextNormalColor;      // Text color for the day header and some other elements.
		COLORREF m_clrHeaderBottomLine;     // Text color for the day header bottom line.

		//-------------------------------------------------------------------
		// Summary:
		//     Performs refreshing of graphical related parameters from
		//     system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw an event.
		// Parameters:
		//     pDC       - Pointer to a valid device context.
		//     pWeekView - A CXTPCalendarWeekView pointer to the Week Event View object.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarWeekView* pWeekView);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the day header.
		// Parameters:
		//     pDC             - Pointer to a valid device context.
		//     rcDay           - A CRect object that contains the day's bounding
		//                       rectangle coordinates.
		//     nHeaderHeight   - An int that contains the header height value.
		//     strHeader       - A CString object that contains the header text.
		//     bIsCurrent      - A BOOL. TRUE if this is the current day for today. FALSE otherwise.
		//     bIsSelected     - A BOOL. TRUE if the day is selected.  FALSE otherwise.
		//-------------------------------------------------------------------
		virtual void DrawDayHeader(CDC* pDC, CRect rcDay, int nHeaderHeight, CString strHeader,
							BOOL bIsCurrent = FALSE, BOOL bIsSelected = FALSE);
	END_VIEW_PART(WeekViewPart)

	//-----------------------------------------------------------------------
	// Summary:
	//     This paint manager part is used to draw events for the
	//     Week View.
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(WeekViewEventPart, XTPCalendarViewPart)
		COLORREF m_clrTextHighLightColor;   // Text color for the selected event.
		COLORREF m_clrTextNormalColor;      // Text color for the event.

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to perform refreshing of
		//     graphical related parameters from system settings.
		//-------------------------------------------------------------------
		virtual void RefreshMetrics()
		{
			m_clrBackground.SetStandardValue(m_pPaintManager->GetWeekViewPart()->GetBackgroundColor()); // RGB(255, 255, 213));
			m_clrTextColor.SetStandardValue(m_pPaintManager->m_clrButtonFaceText);
			m_clrTextHighLightColor = RGB(255, 255, 255);
			m_clrTextNormalColor = m_pPaintManager->m_clrButtonFaceText;

			LOGFONT lfIcon;
			VERIFY(m_pPaintManager->GetIconLogFont(&lfIcon));
			m_fntText.SetStandardValue(&lfIcon);
		}

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the specified event.
		// Parameters:
		//     pDC             - Pointer to a valid device context.
		//     pWeekViewEvent  - A CXTPCalendarWeekViewEvent pointer to the Week Event View object.
		//-------------------------------------------------------------------
		virtual void OnDraw(CDC* pDC, CXTPCalendarWeekViewEvent* pWeekViewEvent);

		//-------------------------------------------------------------------
		// Summary:
		//     This member functions is used to draw the event border.
		// Parameters:
		//     pDC             - Pointer to a valid device context.
		//     rcView          - A CRect that contains the day view's bounding rectangle coordinates.
		//     pWeekViewEvent  - A CXTPCalendarWeekViewEvent pointer to the Week Event View object.
		//-------------------------------------------------------------------
		virtual void DrawBorder(CDC* pDC, CRect rcView, CXTPCalendarWeekViewEvent* pWeekViewEvent);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the event's times texts or icons.
		// Parameters:
		//     pDC             - Pointer to a valid device context.
		//     pWeekViewEvent  - A CXTPCalendarWeekViewEvent pointer to the Week Event View object.
		//-------------------------------------------------------------------
		virtual int DrawTimes(CDC* pDC, CXTPCalendarWeekViewEvent* pWeekViewEvent);

		//-------------------------------------------------------------------
		// Summary:
		//     This member function is used to draw the event subject.
		// Parameters:
		//     pDC             - Pointer to a valid device context.
		//     pWeekViewEvent  - A CXTPCalendarWeekViewEvent pointer to the Week Event View object.
		//-------------------------------------------------------------------
		virtual void DrawSubj(CDC* pDC, CXTPCalendarWeekViewEvent* pWeekViewEvent);
	END_VIEW_PART(WeekViewEventPart)

	//-----------------------------------------------------------------------
	// Summary: This paint manager part is used to draw time scale for the
	//          Time Line view
	//-----------------------------------------------------------------------
	BEGIN_VIEW_PART(TimeLineViewTimeScalePart_Day, XTPCalendarTimeLineViewTimeScalePart)
		virtual void Draw(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView);
	END_VIEW_PART(TimeLineViewTimeScalePart_Day)

	BEGIN_VIEW_PART(TimeLineViewTimeScalePart_Week, XTPCalendarTimeLineViewTimeScalePart)
		virtual void Draw(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView);
	END_VIEW_PART(TimeLineViewTimeScalePart_Week)

	BEGIN_VIEW_PART(TimeLineViewTimeScalePart_Month, XTPCalendarTimeLineViewTimeScalePart)
		virtual void Draw(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView);
	END_VIEW_PART(TimeLineViewTimeScalePart_Month)

	BEGIN_VIEW_PART(TimeLineViewTimeScalePart_WorkWeek, XTPCalendarTimeLineViewTimeScalePart)
		virtual void Draw(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineView* pView);
	END_VIEW_PART(TimeLineViewTimeScalePart_WorkWeek)

	BEGIN_VIEW_PART(TimeLineViewPart, XTPCalendarTimeLineViewPart)

		CXTPPaintManagerColor m_clrEventBar;            //Event bar color.
		CXTPPaintManagerColor m_clrEventBarLine;        //Event bar line color.

		CXTPPaintManagerColor m_clrTimeScaleBackground; //Time scale background color.
		CXTPPaintManagerColor m_clrTimeScaleHightlite;  //Time scale highlight color.
		CXTPPaintManagerColor m_clrTimeScaleText;       //The time scale text color.

		CXTPPaintManagerColor m_clrSelectedBackground;  //The selected background color.
		CXTPPaintManagerColor m_clrSelectedText;        //The selected text color.

		CXTPPaintManagerColor m_clrBackground;          //The background color.
		CXTPPaintManagerColor m_clrText;                //The text color.

		//-----------------------------------------------------------------------
		// Summary:
		//      Call this function to draw the time line view group.
		// Parameters:
		//      pDC     - The device context.
		//      rcRect  - Rectangular drawing area.
		//      pGroup  - The calendar time line view group.
		//-----------------------------------------------------------------------
		virtual void DrawGroup(CDC* pDC, const CRect& rcRect, CXTPCalendarTimeLineViewGroup* pGroup);

		//-----------------------------------------------------------------------
		// Summary:
		//      Call this function to calculate the time line view event size.
		// Parameters:
		//      pDC         - The device context.
		//      pEventView  - The event view.
		// Returns:
		//      A CSize object denoting the size of the time line view event.
		//-----------------------------------------------------------------------
		virtual CSize CalcEventSize(CDC* c, CXTPCalendarTimeLineViewEvent* pEventView);

		//-----------------------------------------------------------------------
		// Summary:
		//      Call this function to draw the event.
		// Parameters:
		//      pDC         - The device context.
		//      rcEvents    - The rectangular bounds.
		//      pEventView  - The event view.
		//-----------------------------------------------------------------------
		virtual void DrawEvent(CDC* pDC, const CRect& rcEvents, CXTPCalendarTimeLineViewEvent* pEventView);

		//-----------------------------------------------------------------------
		// Summary:
		//      Call this function to refresh the metrics
		//-----------------------------------------------------------------------
		virtual void RefreshMetrics();
		//CBitmap  m_bmpEvent;
		CXTPResourceImage* m_pimgEvent;   //The event image.

		//-----------------------------------------------------------------------
		// Summary:
		//      Call this function to do the initializations
		//-----------------------------------------------------------------------
		virtual void _Init();

		//-----------------------------------------------------------------------
		// Summary:
		//      Destroys the CTimeLineViewPart object, does the cleanup.
		//-----------------------------------------------------------------------
		virtual ~CTimeLineViewPart();

	END_VIEW_PART(TimeLineViewPart)
//}}AFX_CODEJOCK_PRIVATE

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor.
	// Remarks:
	//     Handles initial initialization.
	// See Also: RefreshMetrics()
	//-----------------------------------------------------------------------
	CXTPCalendarPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarPaintManager();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to update event label default colors
	//-----------------------------------------------------------------------
	virtual void UpdateEventLabelsDefaultColors();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain current paint theme.
	// Returns:
	//     A paint theme ID from enum XTPCalendarTheme.
	//-----------------------------------------------------------------------
	virtual XTPCalendarTheme GetPaintTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set current paint theme.
	// Parameters:
	//     ePaintTheme - A paint theme ID from enum XTPCalendarTheme.
	//-----------------------------------------------------------------------
	virtual void SetPaintTheme(XTPCalendarTheme ePaintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to initialize all drawing defaults.
	// Remarks:
	//     Initializes all drawings defaults (fonts, colors, etc.).
	//     most of defaults are system defaults.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	void UpdateGlyphs();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw an arbitrary line.
	// Parameters:
	//     pDC  - Pointer to a valid device context.
	//     xPos - An int that contains the Horizontal coordinate of the beginning of line.
	//     yPos - An int that contains the Vertical coordinate of the beginning of line.
	//     cx   - An int that contains the Horizontal coordinate of the end of line.
	//     cy   - An int that contains the Vertical coordinate of the end of line.
	// Remarks:
	//     This member function is used anywhere a simple arbitrary line is needed.
	//-----------------------------------------------------------------------
	virtual void DrawLine(CDC* pDC, int xPos, int yPos, int cx, int cy);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw a Horizontal line.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     xPos   - An int that contains the Horizontal coordinate of the beginning of the line.
	//     yPos   - An int that contains the Vertical coordinate of the beginning of the line.
	//     cx  - An int that contains the Length of the line.
	// Remarks:
	//     This member function is used anywhere a simple horizontal line is needed.
	//-----------------------------------------------------------------------
	virtual void DrawHorizontalLine(CDC* pDC, int xPos, int yPos, int cx);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw a Vertical line.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     xPos   - An int that contains the Horizontal coordinates of the beginning of the line.
	//     yPos   - An int that contains the Vertical coordinates of the beginning of the line.
	//     cy  - An int that contains the Length of the line.
	//     pPen - A CPen pointer to the pen object used to draw the line.
	// Remarks:
	//     This member function is used anywhere a simple vertical line is needed.
	//-----------------------------------------------------------------------
	virtual void DrawVerticalLine(CDC* pDC, int xPos, int yPos, int cy);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the time as clock.
	// Parameters:
	//     pDC           - Pointer to a valid device context.
	//     dtClockTime   - A COleDateTime object that contains the time to draw.
	//     rcView        - A CRect object that contains the bounding rectangle
	//                     coordinates of where the clock is drawn.
	//     clrBackground - A COLORREF object that contains the background color of the clock.
	//     cafAlign      - A XTPCalendarClockAlignFlags alignment flag, appropriate values are determined by
	//                     enum XTPCalendarClockAlignFlags.
	// Remarks:
	//     This member function is used anywhere a time clock is needed.
	// Returns:
	//      Returns the size (width) of the square where clock icon was drawn inside.
	//      Zero when clock icon wasn't successfully drawn.
	// See Also: enum XTPCalendarClockAlignFlags determines alignment flags.
	//-----------------------------------------------------------------------
	virtual int DrawClock(CDC* pDC, COleDateTime dtClockTime, CRect rcView, COLORREF clrBackground, XTPCalendarClockAlignFlags cafAlign);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw a bitmap from the provided ImageList.
	// Parameters:
	//     pImageList  - A CImageList pointer to the ImageList.
	//     pDC         - Pointer to a valid device context.
	//     rcView      - A CRect object that contains the bounding rectangle
	//                   coordinates to draw the bitmap.
	//     iIcon       - An int that contains the image index.
	//     uFlags      - Additional drawing options. Should be 0 or DT_VCENTER.
	// Remarks:
	//     Use this function to draw a bitmap in the view's bounding rectangle.
	//     If the provided bounding rectangle is too small, then the bitmap is not drawn.
	// Returns:
	//     An int that contains the width of the drawn bitmap.
	//-----------------------------------------------------------------------
	virtual int DrawBitmap(CImageList* pImageList, CDC* pDC, CRect rcView,
							int iIcon, UINT uFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the bitmap using its resource id.
	// Parameters:
	//     nIDResource - A UINT that contains the integer resource id of the bitmap.
	//     pDC         - Pointer to a valid device context.
	//     rcBitmap    - A CRect that contains the bounding rectangle coordinates
	//                   used to draw the bitmap.
	// Remarks:
	//     Use this function to draw a bitmap in the view's bounding rectangle.
	//     If the provided bounding rectangle is too small, then the bitmap is not drawn.
	// Returns:
	//     An int that contains the width of the drawn bitmap.
	//-----------------------------------------------------------------------
	virtual int DrawBitmap(UINT nIDResource, CDC* pDC,  CRect rcBitmap);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the Glyph symbol using special
	//     font.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - A CRect that contains the bounding rectangle coordinates
	//                used to draw the symbol.
	//     ch       - A char symbol (from the font) to draw.
	// Remarks:
	//     Use this function to draw the Glyph symbol in the center
	//     of the provided bounding rectangle.
	// Returns:
	//     An int that contains the width of the drawn symbol.
	//-----------------------------------------------------------------------
	virtual int DrawIconChar(CDC* pDC, CRect rc, TCHAR ch);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the horizontal and vertical
	//     size of the bitmap.
	// Parameters:
	//     nIDResource :  A UINT that contains the integer resource id of the
	//                    bitmap.
	// Returns:
	//     A CSize object that contains the size of the bitmap.
	// ----------------------------------------------------------------------
	virtual CSize GetBitmapSize(UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the horizontal and vertical
	//     size of the Expand Sign bitmap.
	// Returns:
	//     A CSize object that contains the bitmap size.
	//-----------------------------------------------------------------------
	virtual const CSize GetExpandSignSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the horizontal and vertical
	//     size of the Clock.
	// Returns:
	//     A CSize object that contains the bitmap size.
	//-----------------------------------------------------------------------
	virtual CSize GetClockSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the CXTPCalendarControl object
	//     for the CXTPCalendarPaintManager.
	// Parameters:
	//     pControl - A CXTPCalendarControl pointer to the CXTPCalendarControl object
	//-----------------------------------------------------------------------
	virtual void SetControl(CXTPCalendarControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the shadow regions around the
	//     rectangle.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcRect - A CRect that contains the rectangle coordinates of where
	//              to draw the shadow regions.
	//-----------------------------------------------------------------------
	virtual void DrawShadow(CDC* pDC, const CRect& rcRect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw icons corresponding to the event
	//     state.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//      pViewEvent - A CXTPCalendarViewEvent pointer to the event view object.
	//-----------------------------------------------------------------------
	virtual int DrawIcons(CDC* pDC, CXTPCalendarViewEvent* pViewEvent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the Header.
	// Parameters:
	//      pDC     - Pointer to a valid device context.
	//      rcHeader    - A CRect that contains the bounding rectangle coordinates
	//                    used to draw the header.
	//      bIsSelected - A BOOL. TRUE if the header is selected. FALSE otherwise.
	//      bIsCurrent  - A BOOL. TRUE if the header represents the current time.
	//                    FALSE otherwise.
	//     grclr           - A CXTPPaintManagerColorGradient object that specifies the Gradient color.
	//     clrUnderLineHdr - A COLORREF object that specifies the color of header's underline.
	//-----------------------------------------------------------------------
	virtual void DrawHeader(CDC* pDC, CRect& rcHeader, BOOL bIsSelected,
							BOOL bIsCurrent);
	virtual void DrawHeader(CDC* pDC, CRect& rcHeader, BOOL bIsSelected,
		BOOL bIsCurrent, const CXTPPaintManagerColorGradient& grclr, COLORREF clrUnderLineHdr); // <combine CXTPCalendarPaintManager::DrawHeader@CDC*@CRect&@BOOL@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill the area using the corresponding
	//     busy color or pattern.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rcRect      - A CRect object that contains the rectangle coordinates
	//                   used to fill the corresponding busy color or pattern.
	//     eBusyStatus - An int that contains the Event Busy Status value from
	//                   enum XTPCalendarEventBusyStatus.
	//     rgnBusy     - A CRgn that contains the region area used to fill with the
	//                   corresponding busy color or pattern and draw the region
	//                   border using m_clrWindowText color.
	// See Also: enum XTPCalendarEventBusyStatus determines Event Busy Status,
	//           CRgn.
	//-----------------------------------------------------------------------
	virtual void DrawBusyStatus(CDC* pDC, CRect& rcRect, int eBusyStatus);
	virtual void DrawBusyStatus(CDC* pDC, CRgn& rgnBusy, int eBusyStatus); // <combine CXTPCalendarPaintManager::DrawBusyStatus@CDC*@CRect&@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to enable a new theme.
	// Parameters:
	//     bEnableTheme - A BOOL. TRUE if the theme enabled.  FALSE otherwise.
	//-----------------------------------------------------------------------
	void EnableTheme(BOOL bEnableTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the status of the theme.
	// Returns:
	//     A BOOL. TRUE if theme is enabled. FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsThemeEnabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the current Windows XP
	//     theme that is in use.
	// Returns:
	//     A XTPCurrentSystemTheme enumeration that represents the
	//     current Windows theme in use, can be one of the following
	//     values:
	//     xtpSystemThemeUnknown - No known theme in use, or Windows Classic theme.
	//     xtpSystemThemeBlue    - Blue theme in use.
	//     xtpSystemThemeOlive   - Olive theme in use.
	//     xtpSystemThemeSilver  - Silver theme in use.
	//-----------------------------------------------------------------------
	XTPCurrentSystemTheme GetCurrentSystemTheme();

public:

private:
	CSize m_szExpandSign;   // Cached size of the expand sign

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add the specified ViewPart object
	//     to the parts list.
	// Parameters:
	//     pPart - Pointer to a 'View Part' object.
	// Returns:
	//     Pointer to a 'View Part' object.
	//-----------------------------------------------------------------------
	CXTPCalendarViewPart* AddViewPart(CXTPCalendarViewPart* pPart);

public:
	CXTPPaintManagerColor m_clrButtonFace;     // Stores standard Button Face color.
	CXTPPaintManagerColor m_clrButtonFaceText; // Stores standard color to display text.
	CXTPPaintManagerColor m_clrWindow;         // Stores standard windows color.
	CXTPPaintManagerColor m_clrWindowText;     // Stores standard color to display window text.
	CXTPPaintManagerColor m_clr3DShadow;       // Stores standard color to display shadow items.
	CXTPPaintManagerColor m_clrHighlight;      // Stores standard color to display highlighted items.

	CXTPPaintManagerColorGradient  m_grclrToday;         // Gradient color of today's header.

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this member function to set custom brush to draw event busy status.
	// Parameters:
	//      nBusyStatus      - Busy status value.
	//      pBrush           - Pointer to a brush object.
	//      bAutodeleteBrush - This parameter indicates should be a brush object
	//                         deleted when destroy or other brush is set.
	//                         If TRUE - brush object will be deleted automatically.
	// Example:
	// <code>
	//
	//  m_wndCalendar.GetPaintManager()->SetBusyStatusBrush(
	//      xtpCalendarBusyStatusFree, new CBrush(RGB(240, 255, 250)), TRUE);
	//
	//  m_wndCalendar.GetPaintManager()->SetBusyStatusBrush(
	//      xtpCalendarBusyStatusTentative, new CBrush(RGB(240, 120, 250)), TRUE);
	//
	//  m_wndCalendar.GetPaintManager()->SetBusyStatusBrush(
	//      100, new CBrush(RGB(240, 120, 250)), TRUE);
	//
	// </code>
	//-----------------------------------------------------------------------
	void SetBusyStatusBrush(int nBusyStatus, CBrush* pBrush, BOOL bAutodeleteBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this member function to get a brush to draw event busy status.
	// Parameters:
	//      nBusyStatus      - Busy status value.
	// Returns:
	//     Pointer to a brush object.
	//-----------------------------------------------------------------------
	CBrush* GetBusyStatusBrush(int nBusyStatus);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns set of flags which define calendar items
	//     to send XTP_NC_CALENDAR_GETITEMTEXT notification.
	// Returns:
	//     Set of flags from enums XTPCalendarGetItemText, XTPCalendarGetItemTextEx.
	// See Also:
	//     XTPCalendarGetItemText, XTPCalendarGetItemTextEx,
	//     SetAskItemTextFlags, CXTPCalendarControl::GetAskItemTextFlags,
	//     CXTPCalendarTheme::GetAskItemTextFlags,
	//     CXTPCalendarTheme::SetAskItemTextFlags, XTP_NC_CALENDAR_GETITEMTEXT
	//-----------------------------------------------------------------------
	DWORD GetAskItemTextFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set flags which define calendar items
	//     to send XTP_NC_CALENDAR_GETITEMTEXT notification.
	// Parameters:
	//      dwFlags - Set of flags from enums XTPCalendarGetItemText,
	//                XTPCalendarGetItemTextEx.
	// See Also:
	//     XTPCalendarGetItemText, XTPCalendarGetItemTextEx,
	//     GetAskItemTextFlags, CXTPCalendarControl::GetAskItemTextFlags,
	//     CXTPCalendarTheme::GetAskItemTextFlags,
	//     CXTPCalendarTheme::SetAskItemTextFlags, XTP_NC_CALENDAR_GETITEMTEXT
	//-----------------------------------------------------------------------
	void SetAskItemTextFlags(DWORD dwFlags);

	BOOL GetIconLogFont(LOGFONT* plf);

	CUIntArray m_arStartCol;    //hold start column in each virtual page
	CUIntArray m_arStartRow;    //hold start row in each virtual page
	int m_PrintPageWidth;       //Print page width.

protected:

	CList<CXTPCalendarViewPart*, CXTPCalendarViewPart*> m_lstViewParts; // Collection to store ViewParts objects.


	//-----------------------------------------------------------------------
	// Summary: Enumerates types of expand signs drawn on a day view.
	//-----------------------------------------------------------------------
	enum XTPEnumExpandSigns
	{
		idxExpandSignUp = 0,    // Expand sign: arrow up
		idxExpandSignDown = 1   // Expand sign: arrow down
	};

	CImageList  m_ilExpandSigns; // Stores standard Expand Signs icons.

	CImageList  m_ilGlyphs;     // Stores standard glyphs for displaying events. Used when glyphs font is not installed.
	CFont*      m_pGlyphsFont;  // Stores standard glyphs font for displaying events.

	CBitmap     m_bmpTentativePattern;  // Stores standard bitmap pattern to draw Tentative event busy status.
	CBrush      m_brushTentative;       // Stores standard brush to draw Tentative event busy status.

	CXTPCalendarTypedPtrAutoDeleteArray<CPtrArray, CXTPCalendarViewPartBrushValue*>
				m_arBusyStatusBrushes; // Stores customizable brush values to draw Event Busy Status in the Day View.

	//-----------------------------------------------------------------------
	// Summary: Returns drawing brush value for a specific busy status.
	// Parameters:
	//     eBusyStatus - Busy status enumeration.
	//     bGrow       - Append a new brush value if needed or not.
	// Returns:
	//     Pointer to CXTPCalendarViewPartBrushValue array member.
	// See Also:
	//     XTPCalendarEventBusyStatus
	//-----------------------------------------------------------------------
	CXTPCalendarViewPartBrushValue* _GetAtGrow_BusyStatusBrushValue(int eBusyStatus, BOOL bGrow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Set default colors/patterns to fill Busy Status area.
	//-----------------------------------------------------------------------
	virtual void InitBusyStatusDefaultColors();

	CSize m_szClockRect;                     // Stores dimensions for a rectangle used to display the time as a clock glyph.

	CXTPCalendarControl* m_pControl;         // Stores a pointer to a CXTPCalendarControl object.

	BOOL m_bEnableTheme;                     // True to enable Luna theme.
	COLORREF m_clrUnderLineHdr;              // Color of header's underline.
	XTPCurrentSystemTheme m_CurrSystemTheme; // Current theme flag.

	XTPCalendarTheme m_nPaintTheme;          // Store the Paint Theme;

	DWORD m_dwAskItemTextFlags;              // Store flag set to send XTP_NC_CALENDAR_GETITEMTEXT notification.

protected:
	#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	friend class CCalendarControlCtrl;
	CXTPCalendarFlagsSet_imp* AxGetAskItemTextFlags();
	//}}AFX_CODEJOCK_PRIVATE
	#endif
};
/////////////////////////////////////////////////////////////////////////////
AFX_INLINE XTPCalendarTheme CXTPCalendarPaintManager::GetPaintTheme() const
{
	return m_nPaintTheme;
}

AFX_INLINE void CXTPCalendarPaintManager::SetPaintTheme(XTPCalendarTheme ePaintTheme)
{
	m_nPaintTheme = min(xtpCalendarThemeOffice2013, ePaintTheme);
	RefreshMetrics();
}

AFX_INLINE XTPCurrentSystemTheme CXTPCalendarPaintManager::GetCurrentSystemTheme(){
	return m_CurrSystemTheme;
}

AFX_INLINE CSize CXTPCalendarPaintManager::GetClockSize(){
	return m_szClockRect;
}
AFX_INLINE BOOL CXTPCalendarPaintManager::IsThemeEnabled(){
	return m_bEnableTheme;
}

AFX_INLINE void CXTPCalendarPaintManager::EnableTheme(BOOL bEnableTheme){
	m_bEnableTheme = bEnableTheme;
	RefreshMetrics();
}

AFX_INLINE DWORD CXTPCalendarPaintManager::GetAskItemTextFlags() const
{
	return m_dwAskItemTextFlags;
}

AFX_INLINE void CXTPCalendarPaintManager::SetAskItemTextFlags(DWORD dwFlags)
{
	m_dwAskItemTextFlags = dwFlags;
}


/////////////////////////////////////////////////////////////////////////////
#endif // !defined(_XTPCALENDARPAINTMANAGER_H__)
