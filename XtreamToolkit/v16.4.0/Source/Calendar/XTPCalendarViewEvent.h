// XTPCalendarViewEvent.h: interface for the CXTPCalendarViewEvent class.
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
#if !defined(_XTP_CALENDARViewEvent_H__)
#define _XTP_CALENDARViewEvent_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPCalendarEvent;
class CXTPCalendarControl;
class CXTPCalendarViewGroup;
struct XTP_CALENDAR_HITTESTINFO;
class CXTPCalendarViewEventSubjectEditor;
class CXTPMarkupUIElement;

//===========================================================================
// Summary:
//     Enumerates allowable formats for event caption.
//===========================================================================
enum XTPCalendarEventCaptionFormat
{
	xtpCalendarCaptionFormatUnknown          = 0,   // Undefined format.
	xtpCalendarCaptionFormatSubject          = 1,   // Caption displays subject.
	xtpCalendarCaptionFormatStartSubject     = 2,   // Besides subject displays start time.
	xtpCalendarCaptionFormatStartEndSubject  = 3    // Besides subject displays start and end time.
};

//===========================================================================
// Summary:
//     Enumerates defined flags for multi day event states.
//===========================================================================
enum XTPCalendarMultiDayEventFlags
{
	xtpCalendarMultiDayUnknown     = 0,             // Undefined multi day event.

	xtpCalendarMultiDayNoMultiDay  = 0x01,          // Not a multi day event.

	xtpCalendarMultiDayFirst       = 0x02,          // First portion of a multi day event.
	                                                // Displayed in the first (by date) day view.
	xtpCalendarMultiDayMiddle      = 0x04,          // Middle portion of a multi day event.
	                                                // Displayed in one of a middle day view.
	xtpCalendarMultiDayLast        = 0x08,          // Last portion of a multi day event.
	                                                // Displayed in the last (by date) day view.

	xtpCalendarMultiDayFMLmask     = xtpCalendarMultiDayFirst | xtpCalendarMultiDayMiddle | xtpCalendarMultiDayLast, // Combined flag.

	xtpCalendarMultiDayMaster      = 0x10,          // First portion of a multi day event.
	                                                // Displayed in the first (by date) day view.
	xtpCalendarMultiDaySlave       = 0x20,          // Rest of the event views (besides Master).
	xtpCalendarMultiDayMSmask      = xtpCalendarMultiDayMaster | xtpCalendarMultiDaySlave    // Combines flag.
};

//===========================================================================
// Summary:
//     This class represents an event view portion of the CalendarViewGroup.
// Remarks:
//     It represents a specific view of the event associated data according
//     to various view types and display settings and provides basic
//     functionality on this data using user input through keyboard and mouse.
//
//          These are the predefined view implementations available
//          for the Calendar control:
//
//
//          * Day and work week event views - Using the CXTPCalendarDayViewEvent class.
//          * Week event view               - Using the CXTPCalendarWeekViewEvent class.
//          * Month view view               - Using the CXTPCalendarMonthViewEvent class.
//
//
//          Furthermore, any type of user defined event view may also be implemented as
//          a descendant of the CXTPCalendarViewEvent class.
//
// See Also: CXTPCalendarDayViewEvent, CXTPCalendarWeekViewEvent,
//              CXTPCalendarMonthViewEvent,
//          CXTPCalendarView, CXTPCalendarViewDay, CXTPCalendarViewGroup
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarViewEvent : public CXTPCalendarWMHandler
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPCalendarView;
	friend class CXTPCalendarViewDay;
	friend class CXTPCalendarViewGroup;

	DECLARE_DYNAMIC(CXTPCalendarViewEvent)
	typedef CXTPCalendarWMHandler TBase;
	//}}AFX_CODEJOCK_PRIVATE

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pViewGroup    - A pointer to a CXTPCalendarViewGroup object.
	//     pEvent      - A pointer to a CXTPCalendarEvent object.
	// See Also: ~CXTPCalendarViewEvent()
	//-----------------------------------------------------------------------
	CXTPCalendarViewEvent(CXTPCalendarEvent* pEvent, CXTPCalendarViewGroup* pViewGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewEvent();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at the specified position index, and returns additional
	//     info in the XTP_CALENDAR_HITTESTINFO struct.
	// Parameters:
	//     pt          - A CPoint that contains the coordinates of the point to test.
	//     pHitTest    - A pointer to an XTP_CALENDAR_HITTESTINFO structure.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the containing
	//     CXTPCalendarViewDay object.
	// Returns:
	//     A pointer to the CXTPCalendarViewDay object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewGroup* GetViewGroup_() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the corresponding read only
	//     event object without increasing the object's reference counter.
	// Returns:
	//     A pointer to a CXTPCalendarEvent object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarEvent* GetEvent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the calendar control
	//     object.
	// Returns:
	//     A pointer to a CXTPCalendarControl object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarControl* GetCalendarControl() const;

	CXTPCalendarView* GetView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if this event view is
	//     visible on the screen.
	// Remarks:
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// Returns:
	//     A BOOL. TRUE if the event view is visible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsVisible() const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the given event view
	//     is selected.
	// Returns:
	//     A BOOL. TRUE if the event view is selected. FALSE otherwise.
	// See Also: SetSelected(bSet)
	//-----------------------------------------------------------------------
	virtual BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the day view as select or
	//     unselected.
	// Parameters:
	//     bSet    - A BOOL. If TRUE, then set the flag as selected.
	//               Otherwise, set the flag as unselected.
	// See Also: IsSelected()
	//-----------------------------------------------------------------------
	virtual void SetSelected(BOOL bSet = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the given event view
	//     is a multi-day event.
	// Returns:
	//     A BOOL. TRUE if the event view is a multi-day event. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsMultidayEvent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to create and to show the subject
	//     editor.
	// Remarks:
	//     Subject is created only for the Master event if the event is a
	//     multi day event. If this member function is called for a slave
	//     event, then the editing process is passed to the master event.
	// Returns:
	//     A pointer to a CXTPCalendarViewEvent object for which editor is created.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEvent* StartEditSubject();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain an edited subject.
	// Returns:
	//     A CString object containing the edited subject.
	//-----------------------------------------------------------------------
	virtual CString GetEditingSubjectFromEditor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to end the editing project, delete
	//     the subject editor, and to obtain the edited text.
	// Returns:
	//     A CString object containing the edited subject text.
	//-----------------------------------------------------------------------
	virtual CString EndEditSubject();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to show a ToolTip rectangle.
	//-----------------------------------------------------------------------
	virtual void ShowToolTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to hide the ToolTip rectangle.
	//-----------------------------------------------------------------------
	virtual void HideToolTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the event's subject
	//     is currently being edited.
	// Returns:
	//     A BOOL. TRUE if the subject is being edited. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsEditingSubject() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the subject editor
	//     rectangle coordinates.
	// Returns:
	//     A CRect object containing the subject editor rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual CRect GetSubjectEditorRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the event's subject
	//     editor window.
	// Returns:
	//     A pointer to the CWnd or NULL.
	//-----------------------------------------------------------------------
	virtual CWnd* GetSubjectEditor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the event's tooltip
	//     is visible.
	// Returns:
	//     A BOOL. TRUE if the subject is visible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsShowingToolTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to format the event's time as a
	//     string with a predefined format.
	// Parameters:
	//     dtTime  - A COleDateTime time object that contains the value
	//               to format.
	// Returns:
	//     A CString object that contains the formatted time.
	//-----------------------------------------------------------------------
	virtual CString FormatEventTime(COleDateTime dtTime) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     used for drawing a clock in the beginning of a multi day event.
	// Returns:
	//     A CRect object containing the clocks rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual CRect GetFirstClockRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     used for drawing a clock in the end of multi day event.
	// Returns:
	//     A CRect object containing the clocks rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetLastClockRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     used for displaying the event's subject.
	// Returns:
	//     A CRect object containing the text rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetTextRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     used for displaying a clock to show the start event time for
	//     single day events.
	// Returns:
	//     A CRect object containing the clocks rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetStartTimeRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     for displaying a clock to show the end event time for single day events.
	// Returns:
	//     A CRect object containing the clocks rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetEndTimeRect()const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     for the event's rectangle.
	// Returns:
	//     A CRect object containing the event's rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetViewEventRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     for the event's maximum rectangle dimensions.
	// Returns:
	//     A CRect object containing the event's maximum rectangle dimensions.
	//-----------------------------------------------------------------------
	virtual CRect GetViewEventRectMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     for displaying the remainder icon.
	// Returns:
	//     A CRect object containing the glyph rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetReminderGlyphRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     used for displaying the recurrence icon.
	// Returns:
	//     A CRect object containing the glyph rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetRecurrenceGlyphRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the rectangle coordinates for
	//     displaying the meeting icon.
	// Returns:
	//     A CRect object containing the glyph rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetMeetingGlyphRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the rectangle coordinates
	//     used for displaying the private icon.
	// Returns:
	//     A CRect object containing the glyph rectangle.
	//-----------------------------------------------------------------------
	virtual CRect GetPrivateGlyphRect() const;
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the flag to determine if
	//     multi day text align mode is on.
	// Remarks:
	//     Multi day text align mode means that all event items align to the
	//     center.
	// Returns:
	//     A BOOL. TRUE if multi day text align mode is on. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsMultyDayTextAlign() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the multi day event state flags.
	// Returns:
	//     An int that contains the event state flags.
	// See Also: XTPCalendarMultiDayEventFlags
	//-----------------------------------------------------------------------
	virtual int GetMultiDayEventFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the multi day event Master/Slave state.
	// Parameters:
	//     bMaster     - A BOOL. Used to indicate Master or Slave state.
	//                   TRUE if in the Master state.
	//                   FALSE if in the Slave state.
	//     pMasterView - A CXTPCalendarViewEvent pointer to the Master event view object.
	// Remarks:
	//     Master/Slave states are used to draw a multi day event. Such
	//     event are drawn not as a few events for a few days, but as one big
	//     event for a few days, or a few big events for a few days. For this
	//     purposes, the first event view in the chain is marked as <b>Master</b>
	//     and other events views in this chain are marked as <b>Slave</b>.
	//     Event view's chain is the set of events views which can be
	//     drawn continuously in the one row.
	//     On adjust layout process width of the Master event view rect
	//     is extended to cover last Slave event view rect in the chain
	//     and only master event view are drawn because it covers Slave
	//     evens views too. Slave events views look hidden and they
	//     are used for adjusting process (to reserve place on the screen
	//     and for other purposes).One multi day event can have few Master
	//     events. This depends on screen positions of days event parts.
	// See Also: AddMultiDayEvent_Slave
	//-----------------------------------------------------------------------
	virtual void SetMultiDayEvent_MasterSlave(BOOL bMaster, CXTPCalendarViewEvent* pMasterView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to add a Slave event view to the
	//     Master event view.
	// Parameters:
	//     pSlaveView        - A CXTPCalendarViewEvent pointer to the Slave
	//                         event view object.
	//     nMasterEventPlace - An int that contains the Master event place number.
	// Remarks:
	//     For multi day event only.
	// See Also: SetMultiDayEvent_MasterSlave
	//-----------------------------------------------------------------------
	virtual void AddMultiDayEvent_Slave(CXTPCalendarViewEvent* pSlaveView, int nMasterEventPlace);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set a flag to determine if the
	//     text size exceeds the event view bounds.
	// Parameters:
	//     bOutOfBorders - A BOOL. TRUE if the text is out of the borders.
	//                     FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void SetTextOutOfBorders(BOOL bOutOfBorders);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the text size
	//     exceeds the event view bounds.
	// Returns:
	//     A BOOL. TRUE if text size exceeds the event view bounds. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsTextOutOfBorders() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the place number of the
	//     event view.
	// Returns:
	//     An int that contains the event's view place number.
	//-----------------------------------------------------------------------
	virtual int GetEventPlaceNumber() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the place number of the
	//     event in Timeline view.
	// Returns:
	//     An int that contains the event's place number in Timeline view.
	//-----------------------------------------------------------------------
	virtual int GetEventTimelinePlaceNumber() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the event view's bounding
	//     rectangle.
	// Remarks:
	//     Use this member function to obtain the event view's bounding
	//     rectangle coordinates.
	// Returns:
	//     A CRect object with the bounding rectangle's coordinates.
	//-----------------------------------------------------------------------
	CRect GetEventRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout
	//     depending on the provided rectangle and calls AdjustLayout()
	//     for all sub-items.
	// Parameters:
	//     pDC               - A pointer to a valid device context.
	//     rcEventMax        - A CRect that contains the rectangle coordinates
	//                         used to draw the view.
	//     nEventPlaceNumber - An int that contains the sequential place number.
	// Remarks:
	//     Call Populate(COleDateTime dtDayDate) prior to adjust layout.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout when theme is set
	//     depending on the provided rectangle and calls AdjustLayout()
	//     for all sub-items.
	// Parameters:
	//     pDC               - A pointer to a valid device context.
	//     rcEventMax        - A CRect that contains the rectangle coordinates
	//                         used to draw the view.
	//     nEventPlaceNumber - An int that contains the sequential place number.
	// -----------------------------------------------------------------------
	virtual void AdjustLayout2(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber) = 0;

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
	//     This member function is used to draw the view content using
	//     the specified device context.
	// Parameters:
	//     pDC - A pointer to a valid device context.
	// Remarks:
	//     Call AdjustLayout() before calling Draw().
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// See Also: AdjustLayout()
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to perform adjustments of the subject
	//     editor area.
	// Remarks:
	//     Call this member function to perform adjustment of subject editor
	//     rectangle.
	//-----------------------------------------------------------------------
	virtual void AdjustSubjectEditor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the item subject.
	// Returns:
	//      A CString denoting the subject.
	// Remarks:
	//      This is a virtual function.
	//-----------------------------------------------------------------------
	virtual CString GetItemTextSubject() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the item location.
	// Returns:
	//      A CString denoting the item location.
	// Remarks:
	//      This is a virtual function.
	//-----------------------------------------------------------------------
	virtual CString GetItemTextLocation() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the item body.
	// Returns:
	//      A CString denoting the item body.
	// Remarks:
	//      This is a virtual function.
	//-----------------------------------------------------------------------
	virtual CString GetItemTextBody() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the item start time.
	// Returns:
	//      A CString denoting the item start time.
	// Remarks:
	//      This is a virtual function.
	//-----------------------------------------------------------------------
	virtual CString GetItemTextStartTime() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the item end time.
	// Returns:
	//      A CString denoting the item end time.
	// Remarks:
	//      This is a virtual function.
	//-----------------------------------------------------------------------
	virtual CString GetItemTextEndTime() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the item text.
	// parameters:
	//     dwItemFlag       - [in] Flags from enum XTPCalendarGetItemText
	//                        which specify event item.
	//     pcszDefaultText  - [in] Pointer to default item text or NULL to
	//                        generate default text automatically.
	// Returns:
	//      A CString denoting the item text.
	// Remarks:
	//      This is a virtual function.
	//-----------------------------------------------------------------------
	virtual CString GetItemText(DWORD dwItemFlag, LPCTSTR pcszDefaultText = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member functions are used to obtain markup template text.
	// Remarks:
	//     It internally call GetItemText to replace markup template parameters:
	//  [ul]
	//  [li] /Calendar/Events/Event/Subject
	//  [li] /Calendar/Events/Event/Location
	//  [li] /Calendar/Events/Event/Body
	//  [li] /Calendar/Events/Event/StartTime
	//  [li] /Calendar/Events/Event/EndTime
	// Returns:
	//     String with prepared markup text.
	//-----------------------------------------------------------------------
	virtual CString GetMarkupText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member functions are used to get stored CXTPMarkupUIElement object.
	// Returns:
	//     A pointer to CXTPMarkupUIElement object.
	//-----------------------------------------------------------------------
	virtual CXTPMarkupUIElement* GetMarkupUIElement();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at a specified position index, and returns additional
	//     info in a XTP_CALENDAR_HITTESTINFO template parameter.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point to test.
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO. Template parameter specifies
	//                the type of HitTest info structure.
	// Returns:
	//     TRUE if item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const = 0;


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the icons rectangle used
	//     to draw the event view icons.
	// Parameters:
	//     rc  - A CRect that contains the coordinates used to draw the view.
	// Returns:
	//     An integer denoting the width of the icon.
	// Remarks:
	//     Call this member function to calculate rectangles used to draw event
	//     view icons depending on what glyphs must be drawn and based on
	//     the event view rect.
	//-----------------------------------------------------------------------
	virtual int CalcIconsRect(CRect rc);
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust a multi day view's layout
	//     depending on the provided rectangle coordinates.
	// Parameters:
	//     rcEventMax        - A CRect that contains the rectangle coordinates
	//                         used to draw the view.
	//     nEventPlaceNumber - An int that contains the sequential place number.
	// Remarks:
	//     Call this member function to perform special adjustment of
	//     multi day events that must be drawn together through a number
	//     of sequential days in some views.
	//     Call Populate(COleDateTime dtDayDate) prior to adjust layout.
	//-----------------------------------------------------------------------
	virtual void AdjustMultidayEvent(CRect rcEventMax, int nEventPlaceNumber);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain tooltip text for the
	//     event view.
	// Returns:
	//     A CString object containing the tooltip text.
	//-----------------------------------------------------------------------
	virtual CString GetToolText();

private:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process mouse events by markup if enabled.
	// Parameters:
	//     nMessage - A mouse window message identifier to be processed by markup.
	//     nFlags  - An int that indicates whether various virtual keys are down.
	//     point   - A CPoint object that specifies the x- and y- coordinates of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Returns:
	//     TRUE if the message is handled properly, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL MarkupHandleMouseEvent(UINT nMessage, UINT nFlags, const CPoint& point);

public:

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
	//     point   - A CPoint that specifies the x- and y- coordinate of the cursor.
	//               These coordinates are always relative to the
	//               upper-left corner of the window.
	// Remarks:
	//     This method is called by the CalendarViewGroup when the user
	//     moves the mouse cursor or stylus.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

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
	//     This member function is used to create a timer event.
	// Parameters:
	//     uTimeOut_ms - A UINT that specifies the duration of the timer
	//                   in milliseconds.
	// Returns:
	//     The identifier of the timer.
	//-----------------------------------------------------------------------
	virtual UINT SetTimer(UINT uTimeOut_ms);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to destroy a timer event.
	// Parameters:
	//     uTimerID - A UINT that specifies the identifier of the timer.
	// Returns:
	//     For the derived classes. This member function should return
	//     TRUE if successful. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void KillTimer(UINT uTimerID);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function after each interval
	//     specified in the SetTimer member function.
	// Parameters:
	//     uTimerID - A UINT that specifies the identifier of the timer.
	// Returns:
	//     A BOOL. TRUE if successful. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL OnTimer(UINT_PTR uTimerID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the timeout value for
	//     displaying a tooltip.
	// Returns:
	//     A UINT that contains the value of timeout in milliseconds.
	//-----------------------------------------------------------------------
	virtual UINT GetShowToolTipTimeOut() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the styles of the editor
	//     window.
	// Returns:
	//     A DWORD that contains the value of the window styles.
	//-----------------------------------------------------------------------
	virtual DWORD GetSubjectEditorStyles() const; // like multi-line

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a pointer to the editor
	//     window font.
	// Returns:
	//     A pointer to a CFont object that contains the editor window font.
	//-----------------------------------------------------------------------
	virtual CFont* GetSubjectEditorFont() const;

	// ------------------------------------------------------------
	// Summary:
	//     This function is used to change the event place number.
	// Parameters:
	//     nNewPlace :  new event place number.
	// ------------------------------------------------------------
	virtual void ChangeEventPlace(int nNewPlace);

	//{{AFX_CODEJOCK_PRIVATE
	virtual void _ChangeEventPlace(int nNewPlace, int nYbasePrev, int nYbaseNew);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to move the rectangle vertically.
	// Parameters:
	//     nYbasePrev  - An int that contains the previous position.
	//     nYbaseNew   - An int that contains the new position.
	//     rRC         - A CRect that contains the rectangle coordinates.
	//-----------------------------------------------------------------------
	static void _MoveRectY(int nYbasePrev, int nYbaseNew, CRect& rRC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework before the destroy() function is called.
	//-----------------------------------------------------------------------
	virtual void OnBeforeDestroy();

public:
	CRect m_rcEvent;                    // Event view rectangle.
	CRect m_rcEventMax;                 // Event view rectangle margins.

	CRect m_rcText;                     // Rectangle to display event text in event view.
	CSize m_szText;                     // Sizes of displayed text.

	CRect m_rcFirstClock;               // Rectangle to draw clock in the beginning of the first day of multi day event.
	CRect m_rcLastClock;                // Rectangle to draw clock at the end of the last day of multi day event.
	CRect m_rcStartTime;                // Rectangle to display start time day event view.
	CRect m_rcEndTime;                  // Rectangle to display end time event view.

	int m_nMultiDayEventFlags;          // Multi day event flags packed into integer by bitwise operations.
//protected:
	CRect m_rcReminderGlyph;            // Rectangle to draw reminder glyph event view.
	CRect m_rcRecurrenceGlyph;          // Rectangle to draw recurrence glyph event view.
	CRect m_rcMeetingGlyph;             // Rectangle to draw meeting glyph event view.
	CRect m_rcPrivateGlyph;             // Rectangle to draw private glyph event view.

	CRect m_rcSubjectEditor;            // Subject editor rectangle.
	CRect m_rcToolTip;                  // Tool tip rectangle.


	CXTPMarkupUIElement* m_pUIElement;  // Store pointer to CXTPMarkupUIElement object.
	CString        m_strUIElementText;  // Store last markup text for which m_pUIElement object was updated.

protected:
	CXTPCalendarEvent*    m_ptrEvent;   // Pointer to the displayed event object.
	BOOL m_bSelected;                   // Event view selected flag.
	CXTPCalendarViewEvent* m_pMasterView;   // Pointer to Master event view object.
	int m_nEventPlaceNumber;                // Event view place number (sequential identifier).
	int m_nEventTimelinePlaceNumber;        // Event view place number in Timeline view.

	UINT m_nTimerID_ShowToolTip;        // Identifier of timer event to show tooltip with delay.

	CXTPCalendarViewEventSubjectEditor* m_pSubjectEditor;            // Pointer to subject editor.

	BOOL m_bMultyDayTextAlign;          // Flag indicates special text alignment for multi day event.

	BOOL m_bTextOutOfBorders;           // Flag indicates text out of event borders.
private:
	BOOL m_bShowingToolTip;             // Whether we are currently showing event tooltip window.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarViewEvent);

	LPDISPATCH OleGetEvent();
	BOOL OleGetSelected();
	void OleSetSelected(BOOL bSelected);
	LPDISPATCH OleGetViewDay();
	LPDISPATCH OleGetViewGroup();

//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class used as collection of Event View object.
// See Also: CXTPCalendarPtrCollectionT
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarViewEvents : public CXTPCalendarPtrCollectionT<CXTPCalendarViewEvent>
{
public:
	//{{AFX_CODEJOCK_PRIVATE
	typedef CXTPCalendarPtrCollectionT<CXTPCalendarViewEvent> TBase;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarViewEvents();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewEvents();

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function finds first event view index in the collection
	//      using event object.
	// Parameters:
	//      pEvent - A pointer to a CXTPCalendarEvent object.
	// Returns:
	//      -1 if event view is not found, otherwise an integer index that is
	//      greater than or equal to 0 and less than the value returned
	//      by GetCount.
	//-----------------------------------------------------------------------
	virtual int Find(const CXTPCalendarEvent* pEvent) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function finds all event views for event object
	//      and removes them from the array.
	// Parameters:
	//      pEvent - A pointer to a CXTPCalendarEvent object.
	//-----------------------------------------------------------------------
	virtual void Remove(CXTPCalendarEvent* pEvent);

#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarViewEvents);
	DECLARE_ENUM_VARIANT(CXTPCalendarViewEvents)

	//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class is used to build the ViewEvent layer of the DayView and
//     ViewEvent hierarchies.
// Remarks:
//     This class template customizes, overrides and implements some
//     functionality for the CXTPCalendarViewEvent base class.
//     It is used as a part of the calendar control framework to build
//     <b><i>ViewEvent</i></b> layer of <b>View->DayView->ViewEvent</b>
//     typed objects hierarchy.
//     These are the template parameters:
//     _TViewGroup   - Type of View Day objects stored in View
//
//          All of the above parameters are required.
//
// See Also: CXTPCalendarViewEvent
//===========================================================================
template<class _TViewGroup>
class CXTPCalendarViewEventT : public CXTPCalendarViewEvent
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pEvent      - Pointer to CXTPCalendarEvent object.
	//     pViewGroup    - Template parameter specifies type of VIew Day object.
	// See Also: ~CXTPCalendarViewEventT()
	//-----------------------------------------------------------------------
	CXTPCalendarViewEventT(CXTPCalendarEvent* pEvent, _TViewGroup* pViewGroup) :
		CXTPCalendarViewEvent(pEvent, (CXTPCalendarViewGroup*)pViewGroup)
	{
		m_pViewGroup = pViewGroup;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewEventT()
	{
	};


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a base CXTPCalendarViewGroup*
	//     pointer to a corresponding DayView object.
	// Remarks:
	//     Call this member function to obtain a pointer to a DayView object
	//     which stores the ViewEvent.
	// Returns:
	//     A pointer to a base CXTPCalendarViewGroup object.
	// See also:
	//     GetViewGroup()
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewGroup* GetViewGroup_() const
	{
		ASSERT(this);
		return this ? (CXTPCalendarViewGroup*)m_pViewGroup : NULL;
	}


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a pointer to a DayView object.
	// Remarks:
	//     Call this member function to obtain a pointer to a DayView object
	//     which stores the ViewEvent. The _TViewGroup template parameter is
	//     used to determine the type of the returned pointer.
	// Returns:
	//     A pointer to a DayView object, the type of the returned pointer
	//     is determined by the _TViewGroup template parameter.
	// See also:
	//     GetViewGroup_()
	//-----------------------------------------------------------------------
	_TViewGroup* GetViewGroup() const
	{
		ASSERT(this);
		return this ? m_pViewGroup : NULL;
	}


private:
	_TViewGroup* m_pViewGroup;
};

////////////////////////////////////////////////////////////////////////////

//===========================================================================
// Summary:
//      This class is used to edit event subject.
// Remarks:
//      All massages received by this window are sent to the parent calendar
//      control before they are processed. Some of them are handled by parent
//      calendar control object.
// See Also: CXTPCalendarControl::OnWndMsg_Children()
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarViewEventSubjectEditor : public CEdit
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pOwner    - A pointer to a owner calendar control object.
	// See Also: ~CXTPCalendarViewEventSubjectEditor()
	//-----------------------------------------------------------------------
	CXTPCalendarViewEventSubjectEditor(CXTPCalendarControl* pOwner);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewEventSubjectEditor();

protected:
	CXTPCalendarControl* m_pOwner; // Stored pointer to the owner calendar control object.

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg virtual UINT OnGetDlgCode();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//      This class is used to edit event subject in Office 2007 Theme.
// Remarks:
//      This class is derived from CXTPCalendarViewEventSubjectEditor and
//      draw editor background in Office 2007 Theme event object style
//      or use no background (transparent background).
// See Also:
//      CXTPCalendarViewEventSubjectEditor
//===========================================================================
class _XTP_EXT_CLASS  CXTPCalendarViewEventSubjectEditorColored : public CXTPCalendarViewEventSubjectEditor
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pOwner     - A pointer to an owner calendar control object.
	//     pViewEvent - A pointer to an editing event view object.
	//     pTheme2007 - A pointer to an owner theme.
	// See Also: ~CXTPCalendarViewEventSubjectEditor2007()
	//-----------------------------------------------------------------------
	CXTPCalendarViewEventSubjectEditorColored(CXTPCalendarControl* pOwner, CXTPCalendarViewEvent* pViewEvent, COLORREF clrBackground);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewEventSubjectEditorColored();

protected:
	CXTPCalendarViewEvent* m_pViewEvent;        // Stored pointer to the editing event object.
	CXTPPaintManagerColorGradient m_grclrBk;    // Subject Editor background color (or gradient colors).

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg virtual BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg virtual HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg virtual void OnChange();
	//}}AFX_CODEJOCK_PRIVATE
};

////////////////////////////////////////////////////////////////////////////
AFX_INLINE void CXTPCalendarViewEvent::SetTextOutOfBorders(BOOL bOutOfBorders) {
	m_bTextOutOfBorders = bOutOfBorders;
}
AFX_INLINE  BOOL CXTPCalendarViewEvent::IsTextOutOfBorders() const {
	return m_bTextOutOfBorders;
}

AFX_INLINE CXTPCalendarEvent* CXTPCalendarViewEvent::GetEvent() const {
	return m_ptrEvent;
}
AFX_INLINE int CXTPCalendarViewEvent::GetMultiDayEventFlags() const {
	return m_nMultiDayEventFlags;
}
AFX_INLINE int CXTPCalendarViewEvent::GetEventPlaceNumber() const {
	return m_nEventPlaceNumber;
}
AFX_INLINE int CXTPCalendarViewEvent::GetEventTimelinePlaceNumber() const {
	return m_nEventTimelinePlaceNumber;
}
AFX_INLINE BOOL CXTPCalendarViewEvent::IsEditingSubject() const {
	return m_pSubjectEditor != NULL;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetSubjectEditorRect() const {
	return m_rcSubjectEditor;
}
AFX_INLINE CWnd* CXTPCalendarViewEvent::GetSubjectEditor() const {
	return m_pSubjectEditor;
}
AFX_INLINE UINT CXTPCalendarViewEvent::GetShowToolTipTimeOut() const {
	return XTP_CALENDAR_SHOW_TOOLTIP_TIMEOUT_MS;
}
AFX_INLINE DWORD CXTPCalendarViewEvent::GetSubjectEditorStyles() const {
	return ES_NOHIDESEL | ES_MULTILINE;
}
AFX_INLINE CFont* CXTPCalendarViewEvent::GetSubjectEditorFont() const {
	return NULL;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetFirstClockRect() const {
	return m_rcFirstClock;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetLastClockRect() const {
	return m_rcLastClock;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetTextRect() const {
	return m_rcText;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetStartTimeRect() const {
	return m_rcStartTime;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetEndTimeRect() const {
	return m_rcEndTime;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetViewEventRect() const {
	return m_rcEvent;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetViewEventRectMax() const {
	return m_rcEventMax;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetReminderGlyphRect() const {
	return m_rcReminderGlyph;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetRecurrenceGlyphRect() const {
	return m_rcRecurrenceGlyph;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetMeetingGlyphRect() const {
	return m_rcMeetingGlyph;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetPrivateGlyphRect() const {
	return m_rcPrivateGlyph;
}
AFX_INLINE BOOL CXTPCalendarViewEvent::IsMultyDayTextAlign() const {
	return m_bMultyDayTextAlign;
}
AFX_INLINE CRect CXTPCalendarViewEvent::GetEventRect() const {
	return m_rcEvent;
}

//////////////////////////////////////////////////////////////////////////
#endif // !defined(_XTP_CALENDARViewEvent_H__)
