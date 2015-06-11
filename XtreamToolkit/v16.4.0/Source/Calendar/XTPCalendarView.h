// XTPCalendarView.h: interface for the CXTPCalendarView class.
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
#if !defined(_XTPCALENDARVIEW_H__)
#define _XTPCALENDARVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCalendarControl;
class CXTPCalendarPaintManager;
class CXTPCalendarTheme;

class CXTPCalendarViewDay;
class CXTPCalendarViewEvent;
class CXTPCalendarViewEvents;
class CXTPCalendarEvent;
class CXTPCalendarEvents;
class CXTPCalendarViewPart;
class CXTPCalendarResources;
class CXTPCalendarData;
class CXTPCalendarOptions;

struct XTP_CALENDAR_HITTESTINFO;
typedef DWORD XTP_NOTIFY_CODE;

XTP_DEFINE_SMART_PTR_INTERNAL(CXTPCalendarEvent)
XTP_DEFINE_SMART_PTR_INTERNAL(CXTPCalendarEvents)
XTP_DEFINE_SMART_PTR_INTERNAL(CXTPCalendarViewEvent)
XTP_DEFINE_SMART_PTR_INTERNAL(CXTPCalendarViewEvents)


////////////////////////////////////////////////////////////////////////////

//===========================================================================
// Remarks:
//     Timeout before appearing in the event subject editor window.
//===========================================================================
#define XTP_CALENDAR_START_EDIT_SUBJECT_TIMEOUT_MS 400

//===========================================================================
// Remarks:
//     Calendar events clipboard format name.
//===========================================================================
static const LPCTSTR XTPCALENDARCTRL_CF_EVENT = _T("XTPCalendar_CF_Event");

//{{AFX_CODEJOCK_PRIVATE
enum XTPEnumCalendarUpdateResult
{
	xtpCalendar_Skip        = 0,
	xtpCalendar_Redraw      = 1,
	xtpCalendar_Populate    = 2
};
//===========================================================================
// use project settings to define _TRACE_EDITING_ON
#ifdef _TRACE_EDITING_ON
#define TRACE_MOUSE         TRACE
#define TRACE_KEYBOARD      TRACE
#define TRACE_DRAGGING      TRACE
#define TRACE_EDIT_SUBJECT  TRACE
#else
#define TRACE_MOUSE
#define TRACE_KEYBOARD
#define TRACE_DRAGGING
#define TRACE_EDIT_SUBJECT
#endif
//===========================================================================
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This class helps to handle Windows messages (WM_*) processing portion
//     of the Calendar control views.
// Remarks:
//     All calendar views are derived from this class. When a message is
//     handled by main Calendar control window (CXTPCalendarControl class),
//     it is then forwarded down to views by hierarchy.
//     control -> day/week/month -> single day -> resource group -> event
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarWMHandler : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarWMHandler)
	typedef CCmdTarget TBase;
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarWMHandler();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the number of
	//     CXTPCalendarWMHandler elements in the descendant handlers collection.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array.
	//     Because indexes are zero-based, the size is 1 greater than
	//     the largest index.
	//
	//     This number shows a count of single day views for a day/week/month
	//     view, a number of resource groups for a single day view, a number
	//     of event views for a resource group view, and so on.
	// Returns:
	//     A count of descendant handlers.
	// See Also:
	//     CXTPCalendarWMHandler overview
	//-----------------------------------------------------------------------
	virtual int GetChildHandlersCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a handler object at the specified numeric
	//     index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetChildHandlersCount.
	// Remarks:
	//     Returns the array element at the specified index.
	// Returns:
	//     The pointer to the CXTPCalendarWMHandler element currently at this
	//     index.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarWMHandler* GetChildHandlerAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process left mouse button down events.
	// Parameters:
	//     nFlags  - An int that indicates whether various virtual keys are down.
	//     point   - A CPoint object that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	// Remarks:
	//     This method is called by the CalendarView when the user
	//     presses the left mouse button.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process left mouse button up events.
	// Parameters:
	//     nFlags  - An int that indicates whether various virtual keys are down.
	//     point   - A CPoint object that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	// Remarks:
	//     This method is called by the CalendarView when the user
	//     releases the left mouse button.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process left mouse button
	//     double click events.
	// Parameters:
	//     nFlags  - An int that indicates whether various virtual keys are down.
	//     point   - A CPoint object that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Remarks:
	//     This method is called by the CalendarView when the user
	//     double clicks the left mouse button.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDblClk(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process mouse movement events.
	// Parameters:
	//     nFlags  - A UINT that indicates whether various virtual keys are down.
	//     point   - A CPoint object that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Remarks:
	//     This method is called by the CalendarView when the user
	//     moves the mouse cursor or stylus.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to processes keyboard events.
	// Parameters:
	//     nChar   - A UINT that contains the character code value of the key.
	//     nRepCnt - A UINT that is used to count the number of times the
	//               keystroke is repeated as a result of the user holding
	//               down the key.
	//     nFlags  - A UINT that contains the Scan code, key-transition code,
	//               previous key state, and the context code.
	// Remarks:
	//     This method is called by the CalendarControl when a keystroke
	//     translates to a nonsystem character.
	//-----------------------------------------------------------------------
	virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process key-down keyboard events.
	// Parameters:
	//     nChar   - A UINT that specifies the virtual key code of the given key.
	//     nRepCnt - A UINT that specifies the number of times the keystroke
	//               is repeated as a result of the user holding down the key.
	//     nFlags  - A UINT that specifies the scan code, key-transition code,
	//               previous key state, and the context code.
	// Remarks:
	//     This method is called by the CalendarView when the user
	//     presses keys on the keyboard.
	//-----------------------------------------------------------------------
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process key-up keyboard events.
	// Parameters:
	//     nChar   - A UINT that specifies the virtual key code of the given key.
	//     nRepCnt - A UINT that specifies the number of times the keystroke
	//               is repeated as a result of the user holding down the key.
	//     nFlags  - A UINT that specifies the scan code, key-transition code,
	//               previous key state, and the context code.
	// Remarks:
	//     This method is called by the CalendarView when the user
	//     releases keys on the keyboard.
	//-----------------------------------------------------------------------
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process key-up keyboard events.
	// Parameters:
	//     nChar   - A UINT that specifies the virtual key code of the given key.
	//     nRepCnt - A UINT that specifies the number of times the keystroke
	//               is repeated as a result of the user holding down the key.
	//     nFlags  - A UINT that specifies the scan code, key-transition code,
	//               previous key state, and the context code.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	// Remarks:
	//     This method is called by the CalendarView when the user
	//     presses keys on the keyboard.
	//-----------------------------------------------------------------------
	virtual BOOL OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function after each interval
	//     specified in the SetTimer member function.
	// Parameters:
	//     uTimerID    - A UINT that is used to specify the identifier
	//                   of the timer.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	//-----------------------------------------------------------------------
	virtual BOOL OnTimer(UINT_PTR uTimerID);

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is called by framework after default message processing.
	// Parameters:
	//      message - Specifies the message to be sent.
	//      wParam - Specifies additional message-dependent information.
	//      lParam - Specifies additional message-dependent information.
	//      pResult - The return value of WindowProc. Depends on the message; may be NULL.
	// Returns:
	//      TRUE if the message is handled properly, FALSE else.
	//-----------------------------------------------------------------------
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform additional adjustments
	//     in some kinds of views.
	// Remarks:
	//     Call this member function to perform additional adjustments after
	//     all adjustment activities are completed.
	//-----------------------------------------------------------------------
	virtual void OnPostAdjustLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the framework before destroying
	//     the view.
	//-----------------------------------------------------------------------
	virtual void OnBeforeDestroy();

	//{{AFX_CODEJOCK_PRIVATE
	virtual void OnFinalRelease();
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     This class represents the view portion of the Calendar control.
// Remarks:
//     It represents a specific view of the Control's associated events
//     data and provides basic functionality on the data using user
//     input through the keyboard and mouse.
//
//          The following predefined view implementations are available
//          for the Calendar control:
//
//
//          * Day and work week views - using CXTPCalendarDayView class.
//          * Week view               - using CXTPCalendarWeekView class.
//          * Month view              - using CXTPCalendarMonthView class.
//
//
//          Furthermore, any type of user defined view can also be implemented as
//          a descendant of the CXTPCalendarView class.
//
//          A typical Calendar View model consists of a collection of
//          corresponding ViewDay's, which realizes the contents on a square
//          of one day. ViewDay in its turn contains a collection of ViewEvent's
//          for the particular day, each of them represents a view of an
//          CXTPCalendarEvent object.
//
// See Also: CXTPCalendarDayView, CXTPCalendarWeekView, CXTPCalendarMonthView,
//          CXTPCalendarViewDay, CXTPCalendarViewEvent
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarView : public CXTPCalendarWMHandler
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPCalendarControl;
	friend class CXTPCalendarViewDay;
	friend class CXTPCalendarViewEvent;
	friend class CXTPCalendarResource;
	friend class CXTPCalendarTheme;
	friend class CXTPCalendarDayViewGroup;

	DECLARE_DYNAMIC(CXTPCalendarView)

	typedef CXTPCalendarWMHandler TBase;
	//}}AFX_CODEJOCK_PRIVATE
private:
	CUIntArray m_DiscreteSelection;
public:

	// -----------------------
	// Summary:
	//     This member function is used to get number of selected events.
	// Returns:
	//     number of selected events
	// -----------------------
	UINT GetDiscreteSelectionCount() const;

	// -----------------------
	// Summary:
	//     This member function is used to get selected event by ordinal number.
	// Parameters:
	//     id :  event ordinal number
	// Returns:
	//     COleDateTime selected event
	// -----------------------
	COleDateTime GetDiscreteSelectionValue(UINT id) const;

	// -----------------------
	// Summary:
	//     This member function is used to add event to selection by event ordinal number.
	// Parameters:
	//     id :  event ordinal number
	// Returns:
	//     TRUE if success other case FALSE
	// -----------------------
	BOOL AddToDiscreteSelection(UINT id);

	// -----------------------
	// Summary:
	//     This member function is used to check is event in selection
	// Parameters:
	//     id :  event ordinal number
	// Returns:
	//     BOOL flag (TRUE if present in selection, FALSE if not)
	// -----------------------
	BOOL IsInDiscreteSelection(UINT id) const;

	// -----------------------
	// Summary:
	//     This member function is used to Clear Discrete Selection
	// -----------------------
	void ClearDiscreteSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pCalendarControl - A Pointer to CXTPCalendarControl object.
	// See Also: ~CXTPCalendarEvent()
	//-----------------------------------------------------------------------
	CXTPCalendarView(CXTPCalendarControl* pCalendarControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the view type flag.
	// Remarks:
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	// Returns:
	//     A XTPCalendarViewType view type flag.
	// See Also: XTPCalendarViewType
	//-----------------------------------------------------------------------
	virtual XTPCalendarViewType GetViewType() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to populate the view with data
	//     for all items contained in the view.
	//-----------------------------------------------------------------------
	virtual void Populate();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout
	//     depending on the window client size and calls AdjustLayout()
	//     for all sub-items.
	// Parameters:
	//     pDC                     - Pointer to a valid device context.
	//     rcView                  - A CRect that contains the coordinates
	//                               for drawing the view.
	//     bCallPostAdjustLayout   - A BOOL. This is a flag that is used to
	//                               call for additional adjustments. If TRUE,
	//                               then additional adjustments are performed
	//                               for multi day events. If FALSE, then no
	//                               additional adjustments are made.
	// Remarks:
	//     Call Populate() prior adjusting layout.
	//     AdjustLayout2 is called by calendar control instead of AdjustLayout
	//     when theme is set.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcView, BOOL bCallPostAdjustLayout = TRUE);
	virtual void AdjustLayout2(CDC* pDC, const CRect& rcView, BOOL bCallPostAdjustLayout = TRUE);//<COMBINE AdjustLayout>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform the redrawing of the
	//     control's layout.
	// Remarks:
	//     Call AdjystLayout() prior to redrawing the control.
	//-----------------------------------------------------------------------
	virtual void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to show the specified day in the
	//     calendar view.
	// Parameters:
	//     date - A COleDateTime object that contains the date to show.
	//     bSelect - TRUE to select the specified day after showing,
	//               FALSE otherwise. TRUE is the default value.
	// Remarks:
	//     Call this member function to show only the specified day in the
	//     calendar view.
	//-----------------------------------------------------------------------
	virtual void ShowDay(const COleDateTime& date, BOOL bSelect = TRUE) = 0;


	//-----------------------------------------------------------------------
	// Summary:
	//     Scroll vertically.
	// Parameters:
	//     nPos     - An int that contains the position value.
	//     nPos_raw - An int that contains the raw position value.
	// Remarks:
	//     This member function is used to vertical scroll the contents
	//     of the view.
	//-----------------------------------------------------------------------
	virtual void ScrollV(int nPos, int nPos_raw)
	{
		UNREFERENCED_PARAMETER(nPos);
		UNREFERENCED_PARAMETER(nPos_raw);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Scroll horizontally.
	// Parameters:
	//     nPos     - An int that contains the position value.
	//     nPos_raw - An int that contains the raw position value.
	// Remarks:
	//     This member function is used to horizontal scroll the contents
	//     of the view.
	//-----------------------------------------------------------------------
	virtual void ScrollH(int nPos, int nPos_raw)
	{
		UNREFERENCED_PARAMETER(nPos);
		UNREFERENCED_PARAMETER(nPos_raw);
	}

	// -------------------------
	// Summary:
	//     This member function is used to force scroll to the end of scroll range
	// Parameters:
	//     nBar :  standard 0 - 1 values (for vertical or horizontal scrollbar)
	// -------------------------
	virtual void OnScrollEnd(int nBar)
	{
		UNREFERENCED_PARAMETER(nBar);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to retrieve the information about
	//     a scroll bar state.
	// Parameters:
	//     pSI - A SCROLLINFO pointer that contains information about the
	//           current state of the scroll bar.
	// Returns:
	//     A BOOL. The base class definition always returns FALSE.
	//     The derived class definition should return TRUE if the function
	//     is successful. Return FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL GetScrollBarInfoV(SCROLLINFO* pSI) const
	{
		UNREFERENCED_PARAMETER(pSI);
		return FALSE;
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to retrieve the information about
	//     a scroll bar state.
	// Parameters:
	//     pSI - A SCROLLINFO pointer that contains information about the
	//           current state of the scroll bar.
	//     pnScrollStep - A pointer to int to receive scroll step. Can be NULL.
	// Returns:
	//     A BOOL. The base class definition always returns FALSE.
	//     The derived class definition should return TRUE if the function
	//     is successful. Return FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL GetScrollBarInfoH(SCROLLINFO* pSI, int* pnScrollStep = NULL) const
	{
		UNREFERENCED_PARAMETER(pSI); UNREFERENCED_PARAMETER(pnScrollStep);
		return FALSE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function performs all of the drawing logic of the
	//     calendar view.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the view content using the
	//     specified device context.
	// Parameters:
	//     pDC - A pointer to a valid device context.
	// Remarks:
	//     Call AdjustLayout() before calling Draw().
	//     Draw2 is called from OnDraw instead of Draw when theme
	//     is set.
	// See Also: OnDraw, AdjustLayout
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC)
	{
		UNREFERENCED_PARAMETER(pDC);
	}

	//<COMBINE Draw>
	virtual void Draw2(CDC* pDC)
	{
		UNREFERENCED_PARAMETER(pDC);
		//Draw(pDC);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to processes left mouse button events.
	// Parameters:
	//     nFlags  - A UINT that is used to indicates whether various virtual
	//               keys are down.
	//     point   - A CPoint that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	// Remarks:
	//     This method is called by the CalendarControl when the user
	//     presses the left mouse button.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to processes left mouse button events.
	// Parameters:
	//     nFlags  - A UINT that is used to indicate whether various virtual keys are down.
	//     point   - A CPoint that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	// Remarks:
	//     This method is called by the CalendarControl when the user
	//     releases the left mouse button.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonUp(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to processes mouse movement events.
	// Parameters:
	//     nFlags  - A UINT that is used to indicate whether various virtual
	//               keys are down.
	//     point   - A CPoint that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Remarks:
	//     This method is called by the CalendarControl when the user
	//     moves the mouse cursor or stylus.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process mouse double-click events.
	// Parameters:
	//     nFlags  - A UINT that is used to indicate whether various virtual
	//               keys are down.
	//     point   - A CPoint that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	// Remarks:
	//     This method is called by the CalendarControl when the user
	//     presses, releases, and then presses the left mouse button again
	//     within the system's double-click time limit.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDblClk(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process keyboard events.
	// Parameters:
	//     nChar   - A UINT that contains the character code value of the key.
	//     nRepCnt - A UINT that is used to count the number of times the
	//               keystroke is repeated as a result of the user holding
	//               down the key.
	//     nFlags  - A UINT that contains the Scan code, key-transition code,
	//               previous key state, and the context code.
	// Remarks:
	//     This method is called by the CalendarControl when a keystroke
	//     translates to a nonsystem character.
	//-----------------------------------------------------------------------
	virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to processes keyboard events.
	// Parameters:
	//     nChar   - A UINT that is used to identify the virtual key code
	//               of the given key.
	//     nRepCnt - A UINT that is used to count the number of times the
	//               keystroke is repeated as a result of the user holding
	//               down the key.
	//     nFlags  - A UINT that contains the Scan code, key-transition code,
	//               previous key state, and the context code.
	// Remarks:
	//     This method is called by the CalendarControl when the user
	//     presses keys on the keyboard.
	//-----------------------------------------------------------------------
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to processes system keyboard events.
	// Parameters:
	//     nChar   - A UINT that specifies the virtual key code of the given key.
	//     nRepCnt - A UINT that contains the number of times the keystroke is
	//               repeated as a result of the user holding down the key.
	//     nFlags  - A UINT that contains the scan code, key-transition code,
	//               previous key state, and the context code.
	// Returns:
	//     TRUE if the message is handled properly, FALSE else.
	// Remarks:
	//     This method is called by the CalendarControl when the user
	//     presses keys on the keyboard.
	//-----------------------------------------------------------------------
	virtual BOOL OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the view's bounding rectangle.
	// Returns:
	//     A CRect object that contains the view's bounding rectangle coordinates.
	//-----------------------------------------------------------------------
	CRect GetViewRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the number of
	//     CXTPCalendarWMHandler elements in the descendant handlers collection.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array.
	//     Because indexes are zero-based, the size is 1 greater than
	//     the largest index.
	// Returns:
	//     A count of descendant handlers.
	// See Also:
	//     CXTPCalendarWMHandler overview
	//-----------------------------------------------------------------------
	virtual int GetChildHandlersCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a handler object at the specified numeric
	//     index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetChildHandlersCount.
	// Remarks:
	//     Returns the array element at the specified index.
	// Returns:
	//     The pointer to the CXTPCalendarWMHandler element currently at this
	//     index.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarWMHandler* GetChildHandlerAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the number of "day views"
	//     in the view collection.
	// Remarks:
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	// Returns:
	//     An int that contains the the number of "day views" in the view
	//     collection.
	//-----------------------------------------------------------------------
	virtual int GetViewDayCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the date of the day view by
	//     the day's index.
	// Parameters:
	//     nIndex  - An int that contains the day view index in the view collection.
	// Remarks:
	//     Indexed number starts from 0 and cannot be negative.
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	// Returns:
	//     A COleDateTime object that contains the day view date and time.
	// See Also: GetViewDayCount()
	//-----------------------------------------------------------------------
	virtual COleDateTime GetViewDayDate(int nIndex) const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the CXTPCalendarViewDay object
	//     by the day's index.
	// Parameters:
	//     nIndex  - An int that is used as the day view index in the view collection.
	//     dtDay   - A day date to find.
	// Remarks:
	//     Index numbers start with 0 and cannot be negative.
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	// Returns:
	//     A pointer to a CXTPCalendarViewDay object.
	// See Also: GetViewDayCount()
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewDay* GetViewDay_(int nIndex) const;

	virtual CXTPCalendarViewDay* _GetViewDay(const COleDateTime& dtDay) const; //<COMBINE CXTPCalendarView::GetViewDay_@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determines which view item,
	//     if any, is at a specified position index, and returns additional
	//     information in the XTP_CALENDAR_HITTESTINFO argument.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point to test.
	//     pHitTest - A pointer a XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at a specified position index, and returns additional
	//     info in a XTP_CALENDAR_HITTESTINFO object.
	// Parameters:
	//     pt       - A CPoint object that contains the coordinates of the
	//                point to test.
	//     pHitTest - A pointer to the template parameter XTP_CALENDAR_HITTESTINFO
	//                object that contains information on the point
	//                to test.
	// Returns:
	//     TRUE if item found; FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the current dragging mode.
	// Returns:
	//     An XTPCalendarDraggingModeFlag object that contains the current
	//     dragging mode.
	// See Also: XTPCalendarDraggingMode
	//-----------------------------------------------------------------------
	virtual XTPCalendarDraggingMode GetDraggingMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the PaintManager object.
	// Returns:
	//     A pointer to the CXTPCalendarPaintManager object.
	// See Also: CXTPCalendarPaintManager, GetTheme
	//-----------------------------------------------------------------------
	virtual CXTPCalendarPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the associated theme
	//     object.
	//
	// Remarks:
	//     It calls CXTPCalendarControl::GetTheme() method.
	// Returns:
	//     A CXTPCalendarTheme pointer to the associated theme object or NULL.
	// See Also:
	//     CXTPCalendarControl::GetTheme, GetPaintManager
	//-----------------------------------------------------------------------
	virtual CXTPCalendarTheme* GetTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the Calendar
	//     Control object.
	// Returns:
	//     A pointer to the CXTPCalendarControl object.
	// See Also: CXTPCalendarControl
	//-----------------------------------------------------------------------
	CXTPCalendarControl* GetCalendarControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this member function to get direct access to calendar options
	//      data.
	// Remarks:
	//      This member function wraps CXTPCalendarData::GetCalendarOptions()
	//      method. If you read or change returned structure members
	//      no additional checks are made and no notifications are sent.
	//      The preferred way is to use CXTPCalendarControl options methods
	//      like GetWorkWeekMask(), SetWorkWeekMask(), GetFirstDayOfWeek(),
	//      SetFirstDayOfWeek(), ... etc.
	// Returns:
	//      A pointer to calendar options data class CXTPCalendarOptions.
	// See Also:
	//      CXTPCalendarOptions,  CXTPCalendarData::GetCalendarOptions().
	//-----------------------------------------------------------------------
	CXTPCalendarOptions* GetCalendarOptions() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the associated
	//     Calendar Resources collection object.
	// Returns:
	//     A pointer to the CXTPCalendarResources collection object.
	// See Also:
	//     CXTPCalendarResources overview, CXTPCalendarResource overview
	//-----------------------------------------------------------------------
	virtual CXTPCalendarResources* GetResources() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the new Resources collection for the view.
	// Parameters:
	//     pResources - A pointer to the CXTPCalendarResources collection object.
	// Remarks:
	//     Call this member function to programmatically set the new
	//     Resources collection for the view.
	//-----------------------------------------------------------------------
	virtual void SetResources(CXTPCalendarResources* pResources);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to un-select all of the events of the
	//     current view.
	//-----------------------------------------------------------------------
	virtual void UnselectAllEvents();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to select or un-select the provided event.
	// Parameters:
	//     pEvent  - A pointer to the CXTPCalendarEvent object.
	//     bSelect - A BOOL. If TRUE, then select the event.
	//               If FALSE, then un-select the event.
	// Remarks:
	//     This method doesn't redraw control's window in order to avoid
	//     blinking when executed in groups (like selecting a number of events).
	//     So far, you'll have to call Calendar.RedrawControl after that.
	// See Also: SelectViewEvent(), CXTPCalendarEvent
	//-----------------------------------------------------------------------
	virtual void SelectEvent(CXTPCalendarEvent* pEvent, BOOL bSelect = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to select or un-select the provided event.
	// Parameters:
	//     pViewEvent   - A pointer to the CXTPCalendarViewEvent object.
	//     bSelect      - A BOOL. If TRUE, then select the event.
	//                    If FALSE, then un-select the event.
	// Remarks:
	//     This method doesn't redraw control's window in order to avoid
	//     blinking when executed in groups (like selecting a number of events).
	//     So far, you'll have to call Calendar.RedrawControl after that.
	// See Also: SelectEvent(), CXTPCalendarViewEvent
	//-----------------------------------------------------------------------
	void SelectViewEvent(CXTPCalendarViewEvent* pViewEvent, BOOL bSelect  = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to select or un-select the provided
	//     day, including all of the day's events.
	// Parameters:
	//     pDay         - A pointer to a CXTPCalendarViewDay object.
	//     nGroupIndex  - A group index to select.
	// See Also: CXTPCalendarViewDay, SelectDay(COleDateTime dtSelDay, BOOL bSelect)
	//-----------------------------------------------------------------------
	virtual void SelectDay(CXTPCalendarViewDay* pDay, int nGroupIndex = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to a selected
	//     event's views.
	// Remarks:
	//     Tims method may return few event views instances for a one event.
	//     (multi day event has a separate view event object for each day).
	// Returns:
	//     A pointer to a CXTPCalendarViewEvents object that contains the
	//     selected event views.
	// See Also: CXTPCalendarViewEvents, CXTPCalendarViewDay,
	//           SelectDay(COleDateTime dtSelDay, BOOL bSelect),
	//           GetSelectedEvents()
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEvents* GetSelectedViewEvents() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to a selected
	//     event's views collection.
	// Remarks:
	//      Tims method returns one event view instance for a one event.
	//     (multi day event has a separate view event object for each day).
	// Returns:
	//     A smart pointer to a CXTPCalendarEvents object that contains the
	//     selected events.
	// See Also: GetSelectedViewEvents(), CXTPCalendarViewEvents
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEventsPtr GetSelectedEvents() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the last selected event view.
	// Returns:
	//     A pointer to last selected event view or NULL.
	// See Also: CXTPCalendarViewEvent, GetSelectedViewEvents().
	//-----------------------------------------------------------------------
	CXTPCalendarViewEvent* GetLastSelectedViewEvent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if there are selected event view
	// Parameters:
	//     TRUE if the view event selected, FALSE else.
	// See Also: GetSelectedViewEvents
	//-----------------------------------------------------------------------
	virtual BOOL HasSelectedViewEvent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to determine is just one event is selected.
	// Returns:
	//      TRUE - if only one event is selected, FALSE - if no events are selected or
	//      if more than one event is selected.
	// See Also: GetSelectedViewEvents(), SelectViewEvent(), SelectEvent().
	//-----------------------------------------------------------------------
	BOOL IsSingleEventSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the last selected day index.
	// Returns:
	//     Index of the last selected day of -1.
	// See Also: GetLastSelectedDate()
	//-----------------------------------------------------------------------
	int FindLastSelectedDay() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the last selected day date.
	// Returns:
	//     Date of the last selected day or the middle view day date.
	// See Also: FindLastSelectedDay(), GetSelection(), SetSelection().
	//-----------------------------------------------------------------------
	COleDateTime GetLastSelectedDate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the view's selection.
	// Parameters:
	//     pBegin                 - A pointer to the COleDateTime object
	//                              that contains the beginning of the selection.
	//     pEnd                   - A pointer to the COleDateTime object
	//                              that contains the ending of the selection.
	//     pbAllDayEvent          - A pointer to the BOOL variable
	//                              that contains the AllDayEvent attribute of
	//                              the selection (used in the day view only).
	//     pnGroupIndex           - A pointer to int to receive group index.
	//     pspSelectionResolution - A pointer to the COleDateTimeSpan
	//                              object that contains the selection unit
	//                              length. (i.e. 1 day, 1 Hour, ... 5 min)
	// Remarks:
	//     Call this member function to fill the provided COleDateTime
	//     objects with the values of the selection beginning, ending,
	//     and selection unit length.
	// Returns:
	//     A BOOL. TRUE if the selection is returned successfully.
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL GetSelection(COleDateTime* pBegin = NULL, COleDateTime* pEnd = NULL,
							  BOOL* pbAllDayEvent = NULL, int* pnGroupIndex = NULL,
							  COleDateTimeSpan* pspSelectionResolution = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the new selection for the view.
	// Parameters:
	//     dtBegin      - Selection beginning time.
	//     dtEnd        - Selection ending time.
	//     bAllDayEvent - Selection AllDayEvent attribute (used in the day view only).
	//     nGroupIndex  - A group index.
	// Remarks:
	//     Call this member function to programmatically set the new
	//     selection beginning and ending for the view.
	//-----------------------------------------------------------------------
	void SetSelection(const COleDateTime& dtBegin, const COleDateTime& dtEnd,
					  BOOL bAllDayEvent = FALSE, int nGroupIndex = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function resets the current selection - set it empty.
	// Remarks:
	//     Call this member function to programmatically clear the current
	//     selection.
	//-----------------------------------------------------------------------
	void ResetSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if a date time value
	//     is contained inside the selection.
	// Parameters:
	//     date         - A COleDateTime object that contains a date time value.
	//     nGroupIndex  - A group index or -1 for any group.
	//     pbAllDaySelection - A pointer to BOOL to receive is selection All Day.
	// Returns:
	//     TRUE - If date is contained inside the selection.
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL SelectionContains(COleDateTime date, int nGroupIndex = -1, BOOL* pbAllDaySelection = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to select or un-select the day
	//     identified by the provided date.
	// Parameters:
	//     dtSelDay     - A COleDateTime object that contains a date time value.
	//     nGroupIndex  - A group index to select.
	// See Also: CXTPCalendarViewDay, SelectDay
	//-----------------------------------------------------------------------
	virtual void SelectDay(COleDateTime dtSelDay, int nGroupIndex = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the row height required
	//     to display the event.
	// Returns:
	//     An int that contains the height of the row required to display a
	//     single event.
	//-----------------------------------------------------------------------
	virtual int GetRowHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the collection of the day's
	//     events.
	// Parameters:
	//     pRC   - A pointer to calendar resource object.
	//     dtDay - A COleDateTime object that contains the day's date.
	// Returns:
	//     A pointer to a CXTPCalendarEvents object that contains the
	//     collection of the day's events.
	// See Also: CXTPCalendarEvents
	//-----------------------------------------------------------------------
	virtual CXTPCalendarEventsPtr RetrieveDayEvents(CXTPCalendarResource* pRC, COleDateTime dtDay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the subject is in an
	//     editable mode.
	// Returns:
	//     A BOOL. TRUE if the subject is ready for editing. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsEditingSubject() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the bounding rectangle of
	//     the subject editor.
	// Returns:
	//     A CRect object that contains the current coordinates of the
	//     subject editor.
	//-----------------------------------------------------------------------
	virtual CRect GetSubjectEditorRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the bounding rectangle of
	//     the tooltip space (currently this is only "click to add new appointment").
	//     Tooltip rectangle is calculated basing on the current mouse position
	// Parameters:
	//      ptHit - CPoint& of mouse position
	//      hitInfo - XTP_CALENDAR_HITTESTINFO
	// Returns:
	//     A CRect object with relative coordinates of the tooltip.
	//-----------------------------------------------------------------------
	virtual CRect GetTooltipRect(const CPoint& ptHit, const XTP_CALENDAR_HITTESTINFO& hitInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to show/hide a ToolTip rectangle.
	// Parameters:
	//     bShow :  TRUE to show a tooltip, FALSE to hide it.
	//-----------------------------------------------------------------------
	virtual void ShowToolTip(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the timeout value for
	//     displaying a tooltip.
	// Returns:
	//     A UINT that contains the value of timeout in milliseconds.
	//-----------------------------------------------------------------------
	virtual UINT GetShowToolTipTimeOut() const;

	// -----------------------------------------------------------------
	// Summary:
	//     Call this member function to set the new subject text for the
	//     editing event.
	// Parameters:
	//     strNewSubject :  New subject value.
	// -----------------------------------------------------------------
	virtual void OnUpdateEditingSubject(CString strNewSubject);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if you can perform an
	//     undo operation for the subject editor.
	// Returns:
	//     A BOOL. TRUE if there is the possibility to perform an undo
	//     operation. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL CanUndo() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if a redo operation is
	//     possible for the subject editor.
	// Returns:
	//     A BOOL. TRUE if it is possible to perform a redo operation on
	//     the subject-editor. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL CanRedo() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform an Undo operation.
	// Returns:
	//     A BOOL. TRUE if the undo operation is successful. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Undo();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform a Redo operation.
	// Returns:
	//     A BOOL. TRUE if the redo operation is successful. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Redo();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to erases the undo buffer.
	//-----------------------------------------------------------------------
	virtual void EmptyUndoBuffer();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to format the time using the provided
	//     format.
	// Parameters:
	//     dtTime : Date-time value to format time part.
	// Returns:
	//     A CString object containing the formatted time string.
	// ----------------------------------------------------------------------
	virtual CString FormatEventTime(COleDateTime dtTime) const;

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set custom time format for view.
	//     Events will use this format to show their times
	// Parameters:
	//     strFormat : format string specification.
	//     h        Hours with no leading zero for single-digit hours; 12-hour clock
	//     hh       Hours with leading zero for single-digit hours; 12-hour clock
	//     H        Hours with no leading zero for single-digit hours; 24-hour clock
	//     HH       Hours with leading zero for single-digit hours; 24-hour clock
	//     m        Minutes with no leading zero for single-digit minutes
	//     mm       Minutes with leading zero for single-digit minutes
	//     s        Seconds with no leading zero for single-digit seconds
	//     ss       Seconds with leading zero for single-digit seconds
	//     t        One character time marker string, such as A or P
	//     tt       Multi-character time marker string, such as AM or PM
	// Returns:
	//     none.
	// ----------------------------------------------------------------------
	void SetCustomTimeFormat(const CString& strFormat);

	// ----------------------------------------------------------------------
	// Summary:
	//     Reset custom time format to default
	// Parameters:
	//     None
	// Returns:
	//     None
	// ----------------------------------------------------------------------
	void ResetCustomTimeFormat();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the current format string.
	// Returns:
	//     A CString object that contains the format string.
	//-----------------------------------------------------------------------
	virtual CString GetDayHeaderFormat() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the day header format.
	// Parameter:
	//     sFmt - A CString object that contains the format string.
	//-----------------------------------------------------------------------
	virtual void SetDayHeaderFormat(LPCTSTR sFmt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if it is possible to
	//     perform a Cut operation.
	// Remarks:
	//     This member function is used to determine the possibility of
	//     performing a Cut operation in the subject editor.
	// Returns:
	//     A BOOL. TRUE if the cut operation is possible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL CanCut() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if it is possible to
	//     perform a Copy operation.
	// Remarks:
	//     This member function is used to determine the possibility of
	//     performing a Copy operation in the subject editor.
	// Returns:
	//     A BOOL. TRUE if the copy operation is possible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL CanCopy() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if it is possible to
	//     perform a Paste operation.
	// Remarks:
	//     This member function is used to determine the possibility of
	//     performing a Paste operation in the subject editor.
	// Returns:
	//     A BOOL. TRUE if the paste operation is possible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL CanPaste() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform a Cut operation.
	// Remarks:
	//     Call this member function to perform a Cut operation in the subject
	//     editor's window.
	// Returns:
	//     A BOOL. TRUE if the cut operation is successful. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Cut();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform a Copy operation.
	// Remarks:
	//     Call this member function to perform a Copy operation in the
	//     subject editor's window.
	// Returns:
	//     A BOOL. TRUE if the copy operation is successful. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Copy();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform a Paste operation.
	// Remarks:
	//     Call this member function to perform a Paste operation in the
	//     subject editor's window.
	// Returns:
	//     A BOOL. TRUE if the paste operation is successful. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Paste();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the event object before
	//     the start of the event editing (dragging).
	// Returns:
	//     A pointer to a CXTPCalendarEvent object before the start the
	//     event editing or NULL.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarEvent* GetDraggingEventOrig() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the event object that
	//     is currently being edited. (i.e. dragging)
	// Returns:
	//     A pointer to a CXTPCalendarEvent object or NULL.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarEvent* GetDraggingEventNew() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to prepare the event to event editing
	//     (dragging) operation.
	// Returns:
	//     A BOOL. TRUE if the event editing is successful. FALSE otherwise.
	// Parameters:
	//     pEventOrig : Pointer to an initial event for the dragging.
	// ----------------------------------------------------------------------
	virtual BOOL SetDraggingEvent(CXTPCalendarEvent* pEventOrig);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to commit the event editing
	//     (dragging) operation.
	// Returns:
	//     A BOOL. TRUE if the event editing is successful. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL CommitDraggingEvent();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to cancel the event editing
	//     (dragging) operation.
	//-----------------------------------------------------------------------
	virtual void CancelDraggingEvent();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to start the event editing (dragging).
	// Parameters:
	//     point    - A CPoint that contains the mouse position with the
	//                coordinates to start the event editing (dragging).
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO struct that
	//                contains the HitTest results from a specified point.
	//-----------------------------------------------------------------------
	virtual void OnStartDragging(CPoint point, XTP_CALENDAR_HITTESTINFO* pHitTest);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform the event editing
	//     (dragging) operation step.
	// Parameters:
	//     point    - A CPoint that contains the mouse position coordinates.
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO struct that
	//                contains the HitTest results from the specified point.
	// Returns:
	//     TRUE if the operation is successful FALSE else.
	//-----------------------------------------------------------------------
	virtual BOOL OnDragging(CPoint point, XTP_CALENDAR_HITTESTINFO* pHitTest);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to finish the event editing
	//     (dragging) operation.
	// Parameters:
	//     point :     A CPoint that contains the mouse position coordinates.
	//     pHitInfo :  A pointer to a XTP_CALENDAR_HITTESTINFO struct that contains
	//                 the HitTest results from the specified point.
	// Returns:
	//     TRUE if the operation is successful FALSE else.
	// ----------------------------------------------------------------------
	virtual BOOL OnEndDragging(CPoint point, XTP_CALENDAR_HITTESTINFO* pHitInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to start the event subject editing.
	// Parameters:
	//     pViewEvent  - A pointer to a CXTPCalendarViewEvent event view
	//                   that is used to edit the subject.
	//-----------------------------------------------------------------------
	virtual void StartEditSubject(CXTPCalendarViewEvent* pViewEvent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to finish the event subject editing.
	// Parameters:
	//     eAction        - An XTPCalendarEndEditSubjectAction object that
	//                      contains the value to commit or cancel the operation.
	//                      See enum XTPCalendarEndEditSubjectAction.
	//     bUpdateControl - A BOOL. If TRUE, then redraw the control immediately,
	//                      otherwise perform only the specified action
	//                      without redrawing.
	//-----------------------------------------------------------------------
	virtual void EndEditSubject(XTPCalendarEndEditSubjectAction eAction, BOOL bUpdateControl = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the time value after
	//     a mouse click event in the subject editing window.
	// Returns:
	//     A UINT that contains the timeout value in milliseconds before
	//     the start event subject editing.
	//-----------------------------------------------------------------------
	virtual UINT GetStartEditSubjectTimeOut() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to create a new event and start
	//      subject editing.
	// Parameters:
	//      pcszInitialSubject - A pointer to the string which contains initial
	//                           subject value for new event (generally initial
	//                           char).
	//-----------------------------------------------------------------------
	virtual void StartEditNewEventInplace(LPCTSTR pcszInitialSubject);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the event view object
	//     for the specified event and day.
	// Parameters:
	//     pEvent - A CXTPCalendarEvent object that contains the value
	//              to search for.
	//     dtDay  - A COleDateTime object that contains the day that
	//              corresponds to the event object.
	// Returns:
	//     A CXTPCalendarViewEvent object that contains the view object or NULL.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEvent* FindViewEvent(CXTPCalendarEvent* pEvent,
												 COleDateTime dtDay) const;


	virtual CXTPCalendarViewEvent* FindViewEventByGroup(CXTPCalendarEvent* pEvent,
												 COleDateTime dtDay, int nScheduleID) const;
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the next event view
	//     object used for editing events by pressing the TAB key.
	// Parameters:
	//     dtMinStart  - A COleDateTime object that contains the first
	//                   date and time to start the search.
	//     bReverse    - If TRUE the reverse order is used, otherwise normal is used.
	//     pAfterEvent - A CXTPCalendarEvent object that contains the
	//                   last event object to search for.
	// Returns:
	//     A CXTPCalendarViewEvent object that contains the view object or NULL.
	// See Also: GetNextTimeEditByTAB, UpdateNextTimeEditByTAB
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEvent* FindEventToEditByTAB(COleDateTime dtMinStart,
								BOOL bReverse, CXTPCalendarEvent* pAfterEvent = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the next date and time
	//     to start the find event view for editing events using the TAB key.
	// Returns:
	//     A COleDateTime object that contains the next date and time.
	// See Also: FindEventToEditByTAB, UpdateNextTimeEditByTAB
	//-----------------------------------------------------------------------
	virtual COleDateTime GetNextTimeEditByTAB() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the next date and time to
	//     start the find event view for editing events using the TAB key.
	// Parameters:
	//     dtNext   - A COleDateTime object that contains the next date and
	//                time to start the find event view.
	//     bReverse - If TRUE the reverse order is used, otherwise normal is used.
	//     bReset   - If TRUE the next time is set to the start (or end)
	//                iteration position.
	// See Also: GetNextTimeEditByTAB, FindEventToEditByTAB
	//-----------------------------------------------------------------------
	virtual void UpdateNextTimeEditByTAB(COleDateTime dtNext, BOOL bReverse,
										 BOOL bReset = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process the days selection that
	//     are selected with the mouse.
	// Parameters:
	//     pInfo  - A pointer to a XTP_CALENDAR_HITTESTINFO struct that contains
	//              HitTest information about the current mouse position.
	//     nFlags - A UINT that contains the current state of the additional
	//              mouse and keyboard buttons. Like MK_SHIFT, ...
	// See Also: SelectDays
	//-----------------------------------------------------------------------
	virtual void ProcessDaySelection(XTP_CALENDAR_HITTESTINFO* pInfo, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to change the days selection depending
	//     on the previous state of the selection.
	// Parameters:
	//     dtNewSel - A COleDateTime object that contains the day date where
	//                the selection has moved.
	//-----------------------------------------------------------------------
	virtual void SelectDays(COleDateTime dtNewSel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process events that are selected
	//     with the mouse.
	// Parameters:
	//     pInfo  - A pointer to an XTP_CALENDAR_HITTESTINFO struct that contains
	//              HitTest information about current mouse position.
	//     nFlags - A UINT that contains the current state of the additional
	//              mouse and keyboard buttons. Like MK_SHIFT, ...
	//-----------------------------------------------------------------------
	virtual void ProcessEventSelection(XTP_CALENDAR_HITTESTINFO* pInfo, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function(s) is used to disable or enable vertical or
	//      horizontal scrolling of the view (by date or time).
	// Parameters:
	//     bEnable - Set TRUE to enable scrolling, FALSE to disable.
	// Remarks:
	//      Vertical and horizontal scrolling are enabled by default.
	//-----------------------------------------------------------------------
	virtual void EnableVScroll(BOOL bEnable = TRUE);
	virtual void EnableHScroll(BOOL bEnable = TRUE); // <COMBINE CXTPCalendarView::EnableVScroll>

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function(s) is used to obtain status of the vertical or
	//      horizontal scrolling features of the view (by date or time).
	// Returns:
	//     TRUE if scrolling enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsHScrollEnabled() const;
	virtual BOOL IsVScrollEnabled() const;// <COMBINE CXTPCalendarView::IsHScrollEnabled>

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function(s) is used to set date format string for the
	//      day view.
	// Parameters:
	//     pcszCustomFormat - Date format string.
	// Remarks:
	//      Long, Middle, Short and Shortest format strings are used for
	//      different header widths. Calendar control select the longest format
	//      which can be drawn for active day header width.
	//      By default these format strings are build using locale settings
	//      and SetDayHeaderFormatXXX methods should be used to customize
	//      default settings.
	//-----------------------------------------------------------------------
	virtual void SetDayHeaderFormatLong(LPCTSTR pcszCustomFormat);
	// <COMBINE SetDayHeaderFormatLong>
	virtual void SetDayHeaderFormatMiddle(LPCTSTR pcszCustomFormat);
	// <COMBINE SetDayHeaderFormatLong>
	virtual void SetDayHeaderFormatShort(LPCTSTR pcszCustomFormat);
	// <COMBINE SetDayHeaderFormatLong>
	virtual void SetDayHeaderFormatShortest(LPCTSTR pcszCustomFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine is Today day visible.
	// Returns:
	//     A BOOL. TRUE if Today day is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsTodayVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the calendar framework to retrieve
	//     event time string with maximum length (to reserve space).
	// Returns:
	//     A maximum time string.
	//-----------------------------------------------------------------------
	virtual CString GetItemTextEventTimeMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates the day header format string.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     nWidth    - An int that contains the day header with in pixels.
	//     pTextFont - A pointer to a CFont object that is
	//                 used to calculate the text dimensions.
	// Remarks:
	//     Call this member function to create day header format
	//     string based on the current locale.
	//-----------------------------------------------------------------------
	virtual void CalculateHeaderFormat(CDC* pDC, int nWidth, CFont* pTextFont);

	COleDateTime GetDateTimeDay(const COleDateTime& dt, BOOL bWholeDayEvent) const;
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to create a timer event.
	// Parameters:
	//     uTimeOut_ms - A UINT that contains the duration of the timer
	//                   event in milliseconds.
	// Returns:
	//     A UINT that contains the identifier of the timer.
	//-----------------------------------------------------------------------
	virtual UINT SetTimer(UINT uTimeOut_ms);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to destroy a timer event.
	// Parameters:
	//     uTimerID - A UINT that is used to specify the identifier of the timer.
	//-----------------------------------------------------------------------
	virtual void KillTimer(UINT uTimerID);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function after each interval
	//     specified in the SetTimer member function.
	// Parameters:
	//     uTimerID    - A UINT that is used to specify the identifier
	//                   of the timer.
	//-----------------------------------------------------------------------
	virtual BOOL OnTimer(UINT_PTR uTimerID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform additional adjustments
	//     in some kinds of views.
	// Remarks:
	//     Call this member function to perform additional adjustments after
	//     all adjustment activities are completed.
	//-----------------------------------------------------------------------
	//virtual void OnPostAdjustLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the framework before destroying
	//     the view.
	//-----------------------------------------------------------------------
	virtual void OnBeforeDestroy();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the framework view is activated
	//     or deactivated.
	// Parameters:
	//     bActivate     - Indicates whether the view is being activated or
	//                     deactivated.
	//     pActivateView - Points to the view object that is being activated.
	//     pInactiveView - Points to the view object that is being deactivated.
	// Remarks:
	//     Override this member function to handle activated/deactivated
	//     events.
	// See Also:
	//     CXTPCalendarControl::SwitchActiveView(XTPCalendarViewType eView),
	//     CXTPCalendarControl::SetActiveView(CXTPCalendarView* pView)
	//-----------------------------------------------------------------------
	virtual void OnActivateView(BOOL bActivate, CXTPCalendarView* pActivateView,
								CXTPCalendarView* pInactiveView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to send a notification from the
	//     parent Calendar control.
	// Parameters:
	//     EventCode - A XTP_NOTIFY_CODE that contains the specific
	//                 code of the event.
	//     wParam -  A DWORD that contains the first custom parameter.
	//               Depends on the event type. See specific event description
	//               for details.
	//     lParam -  A DWORD that contains the second custom parameter.
	//               Depends on the event type. See specific event description
	//               for details.
	// Remarks:
	//     This member function is called internally from inside the
	//     class when a notification is sent to all listeners.
	// See Also: XTP_NOTIFY_CODE
	//-----------------------------------------------------------------------
	virtual void SendNotification(XTP_NOTIFY_CODE EventCode, WPARAM wParam = 0, LPARAM lParam = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to create the time format string.
	// Remarks:
	//     Call this member function to create the time format string
	//     based on the current locale.
	//-----------------------------------------------------------------------
	virtual void _CalculateEventTimeFormat();

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is used to calculates the max width of the date
	//     used in the specified format string.
	// Parameters:
	//     pDC       - Pointer to a valid device context.
	//     strFormat - A CString that contains the date format string.
	//     bMonth    - A BOOL. If TRUE, then calculate the max width for
	//                 all the months in a year, otherwise calculate the
	//                 max width for all week days.
	// Remarks:
	//     Use this member function to calculate the header format width.
	// Returns:
	//     A CString containing the name of the month or the name of the
	//     day of the week that has the maximum width.
	//-----------------------------------------------------------------------
	virtual CString _GetMaxWidthFormat(CDC* pDC, CString strFormat, BOOL bMonth) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to calculate, in pixels, the max
	//     date width for the specified format string.
	// Parameters:
	//     pDC       - A pointer to a valid device context.
	//     strFormat - A CString that contains the date format string.
	// Remarks:
	//     Use this member function to calculate the maximum width of the
	//     date string.
	// Returns:
	//     An int that contains the maximum date width, in pixels.
	//-----------------------------------------------------------------------
	virtual int _GetMaxWidth(CDC* pDC, CString strFormat) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to remove the year part from the
	//     specified format string.
	// Parameters:
	//     strFormat - [IN/OUT] A CString object that contains the date
	//                          format string reference.
	// Remarks:
	//     This member function is used to calculate the header format.
	//-----------------------------------------------------------------------
	virtual void _RemoveYearPart(CString& strFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to build day header date format
	//      default strings.
	// Remarks:
	//      This member function read locale settings and fill
	//      m_strDayHeaderFormatDefaultXXX members.
	// See Also:
	//      CalculateHeaderFormat(),
	//      m_strDayHeaderFormatDefaultLong, m_strDayHeaderFormatDefaultMiddle,
	//      m_strDayHeaderFormatDefaultShort, m_strDayHeaderFormatDefaultShortest.
	//-----------------------------------------------------------------------
	virtual void _ReadDefaultHeaderFormats();

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function get custom or default day header date format
	//      string.
	// Parameters:
	//      nLevel - Format width level:
	//                  0 - Long,
	//                  1 - Middle,
	//                  2 - Short,
	//                  3 - Shortest.
	// Remarks:
	//      Returns m_strDayHeaderFormatXXX member value if is not empty,
	//      otherwise  m_strDayHeaderFormatDefaultXXX member value is returned.
	// Returns:
	//      Active header date format string for the specified width level.
	// See Also:
	//      m_strDayHeaderFormatDefaultLong, m_strDayHeaderFormatDefaultMiddle,
	//      m_strDayHeaderFormatDefaultShort, m_strDayHeaderFormatDefaultShortest,
	//      m_strDayHeaderFormatLong, m_strDayHeaderFormatMiddle,
	//      m_strDayHeaderFormatShort, m_strDayHeaderFormatShortest.
	//-----------------------------------------------------------------------
	virtual CString _GetDayHeaderFormat(int nLevel) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the specified
	//     editing operation command is possible for the event subject
	//     editor window.
	// Parameters:
	//     idEditCmd - A UINT that contains the editing operation command ID:
	//                 ID_EDIT_UNDO, ID_EDIT_CUT, ID_EDIT_COPY, or ID_EDIT_PASTE.
	// Remarks:
	//     This function works correctly even when there is no subject editing mode.
	// Returns:
	//     A BOOL. TRUE - if the subject editing mode and specified editing
	//     operation are possible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL _CanSubjectEditor(UINT idEditCmd) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a pointer to the event
	//     subject editor window.
	// Remarks:
	//     Works correctly even when not using a subject editing mode.
	// Returns:
	//     Pointer to the CWnd or NULL.
	//-----------------------------------------------------------------------
	virtual CWnd* _GetSubjectEditorWnd() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to copy selected events and CXTPCalendarEvent objects,
	//     to a COleDataSource object.
	// Parameters:
	//     dwCopyFlags - A DWORD that contains a set of flags from the enum
	//                   XTPCalendarClipboardCopyFlags or user defined
	//                   values.
	// Remarks:
	//     COleDataSource->SetClipboard method must be called to place
	//     data into the clipboard. Keep in mind - SetClipboard calls
	//     InternalRelease.
	// Returns:
	//     If there are selected events, then the function returns a pointer
	//     to a new COleDataSource object. If there are no selected events,
	//     then the function returns NULL.
	// See Also: _ReadEventsFromClipboard, XTPCalendarClipboardCopyFlags,
	//           XTPCALENDARCTRL_CF_EVENT, COleDataSource
	//-----------------------------------------------------------------------
	virtual COleDataSource* _CopySelectedEvents(DWORD dwCopyFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to read events and CXTPCalendarEvent
	//     objects, from a COleDataSource object.
	// Parameters:
	//     pData        - A COleDataObject pointer to the data object.
	//     pdwCopyFlags - A pointer to a DWORD that contains the CopyFlags.
	//                    These flags are set in the _CopySelectedEvents call.
	//                    The flag values are from the XTPCalendarClipboardCopyFlags
	//                    enum or are user defined values.
	// Remarks:
	//     COleDataSource->AttachClipboard method must be called to
	//     retrieve data from the clipboard.
	// Returns:
	//     A pointer to a CXTPCalendarEvents object. NULL - If there are no events
	//     in the clipboard. Otherwise, a pointer to a new CXTPCalendarEvents objects
	//     collection.
	// See Also: _CopySelectedEvents, XTPCalendarClipboardCopyFlags,
	//           XTPCALENDARCTRL_CF_EVENT, COleDataSource
	//-----------------------------------------------------------------------
	virtual CXTPCalendarEventsPtr _ReadEventsFromClipboard(COleDataObject* pData,
													DWORD* pdwCopyFlags = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the minimum event duration
	//     visible in the current view.
	// Returns:
	//     A COleDateTimeSpan object that contains the minimum duration
	//     of the event.
	//-----------------------------------------------------------------------
	virtual COleDateTimeSpan GetEventDurationMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform an undo/redo operation
	//     for the specified event.
	// Parameters:
	//     pEvent - A pointer to a CXTPCalendarEvent object that contains
	//              the undo/redo action.
	// Returns:
	//     A BOOL. TRUE if the undo operation is successful. FALSE otherwise.
	// See Also:
	//     Undo, Redo, m_UndoBuffer, m_eUndoMode, XTPCalendarUndoMode
	//-----------------------------------------------------------------------
	virtual BOOL _Undo(CXTPCalendarEvent* pEvent = NULL);

	//{{AFX_CODEJOCK_PRIVATE
	void _ReSelectSelectEvents();
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function handles changing of an event in the associated
	//     data provider. It performs all the necessary validation of this
	//     action and returns an update result.
	// Parameters:
	//     nfCode - Notification code of the data provider update operation.
	//     pEvent - A pointer to a CXTPCalendarEvent object that was changed.
	// Returns:
	//     A code of the update action to be performed in the view.
	// See Also:
	//     XTP_NOTIFY_CODE overview, XTPEnumCalendarUpdateResult overview
	//-----------------------------------------------------------------------
	virtual XTPEnumCalendarUpdateResult OnEventChanged_InDataProvider(XTP_NOTIFY_CODE nfCode, CXTPCalendarEvent* pEvent);

protected:
	//=======================================================================
	// Summary:
	//     This helper class implements Undo/Redo events editing
	//     functionality.
	//=======================================================================
	class CUndoBuffer;

		//-----------------------------------------------------------------------
	// Summary:
	//      This method is used to determine Copy or Move dragging mode.
	// Parameters:
	//      eDragMode - A value from enum XTPCalendarDraggingMode.
	// Returns:
	//      TRUE if eDragMode == xtpCalendaDragModeCopy or eDragMode == xtpCalendaDragModeMove.
	//-----------------------------------------------------------------------
	BOOL _IsDragModeCopyMove(int eDragMode) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is used to determine is specified editing operation
	//      disabled.
	// Parameters:
	//      eOperation - A value from enum XTPCalendarEditOperation.
	//      pEventView - A pointer to event view.
	// Returns:
	//      TRUE if specified editing operation disabled, otherwise FALSE.
	// See Also:
	//      XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
	//      XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION, XTPCalendarEditOperation.
	//-----------------------------------------------------------------------
	BOOL IsEditOperationDisabledNotify(XTPCalendarEditOperation eOperation, CXTPCalendarViewEvent* pEventView) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is used to customize or disable specified editing
	//      operation.
	// Parameters:
	//      eOperation - A value from enum XTPCalendarEditOperation.
	//      pEventView - A pointer to event view.
	// Returns:
	//      TRUE if specified editing operation disabled or handled,
	//      otherwise FALSE.
	// See Also:
	//      XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
	//      XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION, XTPCalendarEditOperation.
	//-----------------------------------------------------------------------
	BOOL OnBeforeEditOperationNotify(XTPCalendarEditOperation eOperation,
		CXTPCalendarViewEvent* pEventView);

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is used to customize or disable specified editing
	//      operation.
	// Parameters:
	//      eOperation - A value from enum XTPCalendarEditOperation.
	//      pEventViews - A pointer to event views.
	// Returns:
	//      TRUE if specified editing operation disabled or handled,
	//      otherwise FALSE.
	// See Also:
	//      XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
	//      XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION, XTPCalendarEditOperation.
	//-----------------------------------------------------------------------
	BOOL OnBeforeEditOperationNotify(XTPCalendarEditOperation eOperation,
		CXTPCalendarViewEvents* pEventViews);

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is used to customize or disable paste editing
	//      operation.
	// Parameters:
	//      pEvent                        - A pointer to event prepared for paste.
	//      bPasteWillChangeExistingEvent - If this parameter is TRUE, the new
	//                                      event will not be added, just
	//                                      existing event will be updated.
	//                                      Used for creating recurrence event
	//                                      exceptions by cut operation.
	// Returns:
	//      TRUE if paste editing operation disabled or handled,
	//      otherwise FALSE.
	// See Also:
	//      XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
	//      XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION, XTPCalendarEditOperation.
	//-----------------------------------------------------------------------
	BOOL OnBeforePasteNotify(CXTPCalendarEvent* pEvent, BOOL bPasteWillChangeExistingEvent);

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is used to customize or disable xtpCalendarEO_InPlaceCreateEvent
	//      editing operation.
	// Parameters:
	//      pcszInitialSubject - A pointer to the string which contains initial
	//                           subject value for new event (generally initial
	//                           char).
	// Returns:
	//      TRUE if In-place create event editing operation disabled or handled,
	//      otherwise FALSE.
	// See Also:
	//      XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
	//      XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION, XTPCalendarEditOperation.
	//-----------------------------------------------------------------------
	BOOL OnInPlaceCreateEvent(LPCTSTR pcszInitialSubject);

	//=======================================================================
	// Summary:
	//      Class CSelectionChangedContext used to avoid multiple sending of
	//      XTP_NC_CALENDAR_SELECTION_CHANGED notification for one selection
	//      changing operation.
	//=======================================================================
	class CSelectionChangedContext
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//     Constructs a CSelectionChangedContext object.
		// Parameters:
		//     pView    - Pointer to parent CXTPCalendarView object.
		//     eSelType - Value from enum XTPCalendarSelectionChanged.
		//                Changed selection type.
		// Remarks:
		//     eSelType = xtpCalendarSelectionUnknown only lock sending notifications.
		// See Also: XTP_NC_CALENDAR_SELECTION_CHANGED
		//-------------------------------------------------------------------
		CSelectionChangedContext(CXTPCalendarView* pView, int eSelType = xtpCalendarSelectionUnknown);

		//-------------------------------------------------------------------
		// Summary:
		//      Destroys a CSelectionChangedContext object and send notification(s)
		//      if this is last locker object.
		//-------------------------------------------------------------------
		virtual ~CSelectionChangedContext();

	private:
		CXTPCalendarView* m_pView;
		int m_eSelType;
	};


protected:

	//=======================================================================
	// Summary: This class defines an internal structure used as a counter on
	//          selection changes, which is needed to redraw a view when
	//          necessary.
	//=======================================================================
	class CSelectionChanged_ContextData
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//      Default object constructor.
		//-------------------------------------------------------------------
		CSelectionChanged_ContextData();

		int m_nLockCount;         // Selection change notification global Lock counter.
		int m_nLockCount_Day;     // Days selection change notification Lock counter.
		int m_nLockCount_Event;   // Events selection change notification Lock counter.

		BOOL m_bRequest_Day;      // Days selection change send notification request.
		BOOL m_bRequest_Event;    // Events selection change send notification request.
	};

	CSelectionChanged_ContextData   m_cntSelChanged; // Counter on selection changes,
	                                                 // is needed to redraw a view when necessary.

	//{{AFX_CODEJOCK_PRIVATE
	friend class CSelectionChangedContext;
	friend class CXTPCalendarViewSelection;
	//}}AFX_CODEJOCK_PRIVATE

protected:
	int         m_eUndoMode;        // Undo mode, see XTPCalendarUndoMode.
	BOOL        m_bResetUndoBuffer; // Clean undo buffer before add next undo action.

	CUndoBuffer* m_pPrevUndoBuffer;   // Undo buffer data after last undo buffer clean.
	CUndoBuffer* m_pUndoBuffer;       // Undo buffer object.

private:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL _EditOperationNotify(XTP_NOTIFY_CODE ncEvent, XTPCalendarEditOperation eOperation,
							  CXTPCalendarViewEvent* pEventView, CXTPCalendarViewEvents* pEventViews = NULL) const;

	XTPCalendarDraggingMode _GetDraggingMode(XTP_CALENDAR_HITTESTINFO* pHitTest) const;
	XTPCalendarEditOperation    _DragMod2Operation(int eDragMode);

	virtual CXTPCalendarData* _GetDataProviderBySelection(UINT* puScheduleID = NULL);
	virtual CXTPCalendarData* _GetDataProviderByConnStr(LPCTSTR pcszConnStr, BOOL bCompareNoCase = TRUE);
	virtual BOOL _IsScheduleVisible(CXTPCalendarData* pDataProvider, UINT uScheduleID);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	XTP_VIEW_LAYOUT& GetLayout_();
	const XTP_VIEW_LAYOUT& GetLayout_() const;
	virtual BOOL IsUseCellAlignedDraggingInTimeArea() const;
	//}}AFX_CODEJOCK_PRIVATE
protected:

	XTP_VIEW_LAYOUT m_Layout; // Layout data.

	//===========================================================================
	// Remarks:
	//     This helper struct is used to group information which is
	//     necessary to format Start/End events times.
	// See Also: FormatEventTime, _CalculateEventTimeFormat.
	//===========================================================================
	struct SEventTimeFormatInfo
	{
		CString strFormat;  // Time format string.
		CString strAM;      // AM symbol.
		CString strPM;      // PM symbol.
		BOOL b24_HFormat;   // Is 24 hours time format.
		BOOL isCustom;

		//-----------------------------------------------------------------------
		// Summary:
		//     Default object constructor.
		//-----------------------------------------------------------------------
		SEventTimeFormatInfo() {b24_HFormat = FALSE; isCustom = FALSE;}
	};

	SEventTimeFormatInfo m_EventTimeFormat; // Information used to format events times.

	CString m_strHeaderFormat;              // Day header active date format string.

	CString m_strDayHeaderFormatLong;       // Day header Long date format custom string.
	CString m_strDayHeaderFormatMiddle;     // Day header Middle date format custom string.
	CString m_strDayHeaderFormatShort;      // Day header Short date format custom string.
	CString m_strDayHeaderFormatShortest;   // Day header Shortest date format custom string.

	CString m_strDayHeaderFormatDefaultLong;        // Day header Long date format default string.
	CString m_strDayHeaderFormatDefaultMiddle;      // Day header Middle date format default string.
	CString m_strDayHeaderFormatDefaultShort;       // Day header Short date format default string.
	CString m_strDayHeaderFormatDefaultShortest;    // Day header Shortest date format default string.

	CRect m_rcView;                  // Client rect.
	CRect m_rcTooltip;               // Last shown tooltip rect (while GetTooltipRect returns calculated
	                                 // currently required tooltip rect)

protected:
	CXTPCalendarEventPtr    m_ptrDraggingEventOrig; // Event object before start event editing.
	CXTPCalendarEventPtr    m_ptrDraggingEventNew;  // Editing event object.

	COleDateTimeSpan        m_spDraggingStartOffset;// Start dragging operation offset from the beginning of the event.
	CPoint                  m_ptStartDragging;      // Request to start dragging operation from this point.

	XTPCalendarDraggingMode m_eDraggingMode;        // Editing event mode.
	BOOL m_bStartedClickInside;                     // Is OnLButtonDown was inside view rect.

	CXTPCalendarViewEventPtr m_ptrEditingViewEvent; // Currently editing subject event view or event view requested to edit subject by timeout.

	UINT m_nTimerID_StartEditSubject;   // Timer ID of edit event subject request.
	UINT m_nTimerID_ShowToolTip;        // Timer ID of start show tooltip request.

	CXTPCalendarControl* m_pControl; // Pointer to the owner control object.

	CXTPCalendarViewEvents* m_pSelectedEvents;  //Selected Events;

	XTP_CALENDAR_HITTESTINFO  m_LastHitInfo;    // Last HitInfo data. It is used in CXTPCalendarView::OnMouseMove() for processing selecting, dragging and other operations.

	UINT m_nKeyStateTimerID;                // Timer ID to check Keys state (like Control).

	XTP_CALENDAR_VIEWSELECTION m_selectedBlock; // Current selection.

	BOOL m_bScrollV_Disabled;               // Is vertical scrolling disabled for a view.
	BOOL m_bScrollH_Disabled;               // Is horizontal scrolling disabled for a view.

	CXTPCalendarResources*  m_pResources; // Resources array

	CPoint m_ptLBtnUpMousePos;
	CPoint m_ptLBtnDownMousePos;

protected:
	mutable CXTPCalendarPtrCollectionT< CXTPCalendarViewDay> m_arDays; // Stores the view day collection.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarView);
protected:
	void OleResetSelection();
	void OleSetSelection(DATE Begin, DATE End, BOOL bAllDay);
	BOOL OleGetSelection(DATE* pBegin, DATE* pEnd, VARIANT_BOOL* pvbAllDay);
	BOOL OleGetSelectionV(VARIANT* pBegin, VARIANT* pEnd, VARIANT* pvbAllDay);
	LPDISPATCH OleSelection();

	LPDISPATCH OleHitTest();
	LPDISPATCH OleHitTestEx(long X, long Y);

	void OleShowDay(DATE dt, const VARIANT& Select);
	int OleGetItemCount();
	LPDISPATCH OleGetItem(long nIndex);

	LPDISPATCH OleGetSelectedEvents();
	void OleSelectViewEvent(LPDISPATCH pViewEventDisp, BOOL bSelect);
	void OleUnselectAllEvents();

	void OleEnableVScroll(BOOL bEnable);
	void OleEnableHScroll(BOOL bEnable);

	LPDISPATCH OleGetMultipleResources();
	void OleSetMultipleResources(LPDISPATCH pDispResources);
	BOOL OleCanStartEdit();

	DECLARE_ENUM_VARIANT(CXTPCalendarView)

	private:
		CXTPCalendarViewSelection* m_pSelOleWrapper;
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This template class represents a view portion of the Calendar control.
// Remarks:
//     It represents a specific view of the Control's associated events
//     data and provides basic functionality on this data using user
//     input through the keyboard and the mouse.
//     It is used as a part of the calendar control framework to build
//     <b><i>View</i></b> layer of <b>View->DayView->ViewEvent</b>
//     typed objects hierarchy.
//     These are the template parameters:
//     _TViewDay   - Type of View Day objects stored in View.
//     _TViewEvent - Type of View Event objects stored in View Day.
//
//          You must provide all of the above parameters.
//
//          These are the predefined view implementations available
//          for the Calendar control:
//
//
//          * Day and work week views - Using the CXTPCalendarDayView class.
//          * Week view               - Using the CXTPCalendarWeekView class.
//          * Month view              - Using the CXTPCalendarMonthView class.
//
//
//          Furthermore, any type of user defined view may also be implemented as
//          a descendant of the CXTPCalendarView class.
//
//          A typical Calendar View model consists of a collection of
//          corresponding ViewDay's, which realizes the contents on a square
//          of one day. ViewDay in turn contains a collection of ViewEvent's
//          for the particular day, each of them represents a view of an
//          CXTPCalendarEvent object.
//
// See Also: CXTPCalendarDayView, CXTPCalendarWeekView, CXTPCalendarMonthView,
//          CXTPCalendarViewDay, CXTPCalendarViewEvent
//===========================================================================
template<class _TViewDay>
class CXTPCalendarViewT : public CXTPCalendarView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pCalendarControl - A pointer to a CXTPCalendarControlT object.
	//     nViewType        - An XTPCalendarViewType object that contains the
	//                        view type identifier.
	// See Also: XTPCalendarViewType, ~CXTPCalendarEventT()
	//-----------------------------------------------------------------------
	CXTPCalendarViewT (CXTPCalendarControl* pCalendarControl, XTPCalendarViewType nViewType) :
		CXTPCalendarView(pCalendarControl)
	{
		m_nViewType = nViewType;
	}

	//-----------------------------------------------------------------------
	// Summary:
		//     Default class destructor.
	// Remarks:
		//     Handles member item deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewT()
	{
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the view type flag.
	// Returns:
	//     An XTPCalendarViewType object that contains the view type flag.
	// See Also: XTPCalendarViewType
	//-----------------------------------------------------------------------
	virtual XTPCalendarViewType GetViewType() const
	{
		return m_nViewType;
	}


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a _TViewDay object corresponding
	//     with the day's index.
	// Parameters:
	//     nIndex  - An int that contains the day view index in the view collection.
	// Returns:
	//     If the day's index is valid, then the function returns a pointer to a
	//     _TViewDay object that corresponds with the day's index.  Otherwise,
	//     the function returns NULL.
	// Remarks:
	//     Index numbers start with 0 and cannot be negative.
	// See Also: GetViewDayCount()
	//-----------------------------------------------------------------------
	virtual _TViewDay*  GetViewDay(int nIndex) const
	{
		return (_TViewDay*)GetViewDay_(nIndex);
	}


private:
	XTPCalendarViewType m_nViewType;

};


AFX_INLINE CXTPCalendarControl* CXTPCalendarView::GetCalendarControl() const {
	return this ? m_pControl : NULL;
}
AFX_INLINE CRect CXTPCalendarView::GetViewRect() const {
	return m_rcView;
}
AFX_INLINE XTPCalendarDraggingMode CXTPCalendarView::GetDraggingMode() const {
	return m_eDraggingMode;
}
AFX_INLINE int CXTPCalendarView::GetRowHeight() const {
	return m_Layout.m_nRowHeight;
}
AFX_INLINE UINT CXTPCalendarView::GetStartEditSubjectTimeOut() const {
	return XTP_CALENDAR_START_EDIT_SUBJECT_TIMEOUT_MS;
}
AFX_INLINE BOOL CXTPCalendarView::IsEditingSubject() const {
	return m_ptrEditingViewEvent != NULL;
}
AFX_INLINE CString CXTPCalendarView::GetDayHeaderFormat() const {
	return m_strHeaderFormat;
}
AFX_INLINE void CXTPCalendarView::SetDayHeaderFormat(LPCTSTR sFmt) {
	m_strHeaderFormat = sFmt;
}
AFX_INLINE BOOL CXTPCalendarView::_IsDragModeCopyMove(int eDragMode) const {
	return eDragMode == xtpCalendaDragModeCopy || eDragMode == xtpCalendaDragModeMove;
}
AFX_INLINE XTPCalendarEditOperation CXTPCalendarView::_DragMod2Operation(int eDragMode) {
	return (XTPCalendarEditOperation)eDragMode;
}
AFX_INLINE void CXTPCalendarView::EnableVScroll(BOOL bEnable) {
	m_bScrollV_Disabled = !bEnable;
}
AFX_INLINE void CXTPCalendarView::EnableHScroll(BOOL bEnable) {
	m_bScrollH_Disabled = !bEnable;
}
AFX_INLINE BOOL CXTPCalendarView::IsHScrollEnabled() const {
	return !m_bScrollH_Disabled;
}
AFX_INLINE BOOL CXTPCalendarView::IsVScrollEnabled() const {
	return !m_bScrollV_Disabled;
}
AFX_INLINE void CXTPCalendarView::SetDayHeaderFormatLong(LPCTSTR pcszCustomFormat) {
	m_strDayHeaderFormatLong = pcszCustomFormat;
}
AFX_INLINE void CXTPCalendarView::SetDayHeaderFormatShortest(LPCTSTR pcszCustomFormat) {
	m_strDayHeaderFormatShortest = pcszCustomFormat;
}
AFX_INLINE void CXTPCalendarView::SetDayHeaderFormatMiddle(LPCTSTR pcszCustomFormat) {
	m_strDayHeaderFormatMiddle = pcszCustomFormat;
}
AFX_INLINE void CXTPCalendarView::SetDayHeaderFormatShort(LPCTSTR pcszCustomFormat) {
	m_strDayHeaderFormatShort = pcszCustomFormat;
}
AFX_INLINE int CXTPCalendarView::GetChildHandlersCount() const {
	return GetViewDayCount();
}
AFX_INLINE XTP_VIEW_LAYOUT& CXTPCalendarView::GetLayout_() {
	return m_Layout;
}
AFX_INLINE const XTP_VIEW_LAYOUT& CXTPCalendarView::GetLayout_() const {
	return m_Layout;
}
AFX_INLINE BOOL CXTPCalendarView::IsUseCellAlignedDraggingInTimeArea() const {
	return FALSE;
}
AFX_INLINE UINT CXTPCalendarView::GetShowToolTipTimeOut() const {
	return XTP_CALENDAR_SHOW_BUTTON_TIMEOUT_MS;
}
AFX_INLINE BOOL CXTPCalendarView::AddToDiscreteSelection(UINT uDay) {
	BOOL bAdd(TRUE);

	for (int id = 0; id < (int)m_DiscreteSelection.GetSize(); id++)
	{
		if (uDay == m_DiscreteSelection.GetAt(id))
		{
			bAdd = FALSE;
			break;
		}
	}
	if (bAdd)
		m_DiscreteSelection.Add(uDay);
	return bAdd;
}
AFX_INLINE BOOL CXTPCalendarView::IsInDiscreteSelection(UINT uDay) const {
	BOOL bFound = FALSE;

	for (int id = 0; id < (int)m_DiscreteSelection.GetSize(); id++)
	{
		if (uDay == m_DiscreteSelection.GetAt(id))
		{
			bFound = TRUE;
			break;
		}
	}
	return bFound;
}
AFX_INLINE void CXTPCalendarView::ClearDiscreteSelection() {
	m_DiscreteSelection.RemoveAll();
}
AFX_INLINE UINT CXTPCalendarView::GetDiscreteSelectionCount() const {
	return (UINT)m_DiscreteSelection.GetSize();
}
AFX_INLINE COleDateTime CXTPCalendarView::GetDiscreteSelectionValue(UINT id) const
{
	COleDateTime dt;

	if (id < (UINT) m_DiscreteSelection.GetSize())
		dt = COleDateTime((double) m_DiscreteSelection.GetAt(id));
	return dt;
}

AFX_INLINE int CXTPCalendarView::GetViewDayCount() const {
	return m_arDays.GetCount();
}

AFX_INLINE CXTPCalendarViewDay* CXTPCalendarView::GetViewDay_(int nIndex) const {
	ASSERT(this);
	if (!this)
		return NULL;

	int nCount = m_arDays.GetCount();
	ASSERT(nIndex >= 0 && nIndex < nCount);

	return (nIndex >= 0 && nIndex < nCount) ? m_arDays.GetAt(nIndex) : NULL;
}

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(_XTPCALENDARVIEW_H__)
