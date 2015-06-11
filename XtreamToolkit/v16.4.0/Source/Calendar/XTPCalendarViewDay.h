// XTPCalendarViewDay.h: interface for the CXTPCalendarViewDay class.
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
#if !defined(_XTPCALENDARVIEWDAY_H__)
#define _XTPCALENDARVIEWDAY_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

class CXTPCalendarView;
class CXTPCalendarViewDay;
class CXTPCalendarViewGroup;
class CXTPCalendarViewEvent;

class CXTPCalendarEvent;

struct XTP_CALENDAR_HITTESTINFO;

//===========================================================================
//
// Regular schema:
//    - CalendarView consists of CalendarView(s)Day
//    - CalendarViewDay consists of CalendarView(s)Resource
//    - CalendarViewGroup contains CalendarView(s)Event
//
// ViewGroup draws a single resource depending on the type of parent
// day view.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarViewGroup : public CXTPCalendarWMHandler
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPCalendarView;
	friend class CXTPCalendarViewDay;
	friend class CXTPCalendarViewEvent;
	friend class CXTPCalendarTheme;

	DECLARE_DYNAMIC(CXTPCalendarViewGroup)
	//}}AFX_CODEJOCK_PRIVATE

	typedef CXTPCalendarWMHandler TBase;    // Base class shortcut
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor. Construct CXTPCalendarViewGroup object.
	// See Also: ~CXTPCalendarViewGroup()
	//-----------------------------------------------------------------------
	CXTPCalendarViewGroup(CXTPCalendarViewDay* pViewDay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member item deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtain a pointer to collection of Calendar Resources.
	// Remarks:
	//     Use this method to retrieve a pointer to the Calendar Resources
	//     collection of this Group View.
	// Returns:
	//     CXTPCalendarResources pointer.
	// See Also:
	//     CXTPCalendarResources overview, SetResources, AddResource
	//-----------------------------------------------------------------------
	virtual CXTPCalendarResources* GetResources() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns data provider for this view group.
	// Returns:
	//     A pointer to the data provider object if it was previously set;
	//     otherwise, returns NULL.
	// See Also:
	//     CXTPCalendarData overview
	//-----------------------------------------------------------------------
	virtual CXTPCalendarData* GetDataProvider() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new Resource object into the collection of resources for
	//     this Group View.
	// Parameters:
	//     pResource   - Pointer to the added CXTPCalendarResource object.
	//     bWithAddRef - Set this value to TRUE to increment the reference
	//                   count of the inserted object,
	//                   FALSE to insert object without incrementing
	//                   reference count.
	// See Also:
	//     CXTPCalendarResources::Add
	//-----------------------------------------------------------------------
	virtual void AddResource(CXTPCalendarResource* pResource, BOOL bWithAddRef);

	//-----------------------------------------------------------------------
	// Summary:
	//     Assigns a new Resources collection to this Group View.
	// Parameters:
	//     pResources - Pointer to the new Resources collection to be copied
	//                  into internal collections of this Group View.
	// See Also:
	//     GetResources, AddResource
	//-----------------------------------------------------------------------
	virtual void SetResources(CXTPCalendarResources* pResources);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns default Schedule ID - the first existing one.
	// Returns:
	//     The first existing Schedule ID or
	//     XTP_CALENDAR_UNKNOWN_SCHEDULE_ID if not found.
	//-----------------------------------------------------------------------
	virtual UINT GetScheduleID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to populate the view with data for
	//     all items contained in the view.
	// Parameters:
	//     dtDayDate - A COleDateTime object that contains the day's date.
	// Remarks:
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	//-----------------------------------------------------------------------
	virtual void Populate(COleDateTime dtDayDate);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout depending
	//     on the provided rectangle and then calls AdjustLayout() for all
	//     of the sub-items.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     rcGroup - A CRect object that contains the rectangle coordinates
	//               that are used to draw the view.
	// Remarks:
	//     Call Populate(COleDateTime dtDayDate) prior to calling AdjustLayout().
	//     AdjustLayout2 is called by calendar control instead of AdjustLayout
	//     when theme is set.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcGroup);
	virtual void AdjustLayout2(CDC* pDC, const CRect& rcGroup); //<COMBINE AdjustLayout>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the view contents using
	//     the specified device context.
	// Parameters:
	//     pDC - A pointer to a valid device context.
	// Remarks:
	//     Call AdjustLayout() before calling Draw().
	// See Also: AdjustLayout(CRect rcGroup)
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at the specified position index, and returns additional
	//     info in an XTP_CALENDAR_HITTESTINFO struct.
	// Parameters:
	//     pt       - A CPoint object that contains the coordinates of
	//                the point to test.
	//     pHitTest - A pointer to an XTP_CALENDAR_HITTESTINFO struct that
	//                contains information about the point to test.
	// Remarks:
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at the specified position index, and returns additional
	//     info in the XTP_CALENDAR_HITTESTINFO template object.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point test.
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     Implements standard functionality for the HitTestEx method.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill the pHitTest structure
	//     members with the default values for the current day view object.
	// Parameters:
	//     pHitTest - A Pointer to a XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	//-----------------------------------------------------------------------
	virtual void FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the group's rectangle area.
	// Returns:
	//     A CRect that contains the rectangle coordinates of the group view
	//     in client coordinates of the parent's CXTPDatePickerControl object.
	// See Also:
	//     GetHeaderRect
	//-----------------------------------------------------------------------
	virtual CRect GetRect() const
	{
		return m_Layout.m_rcGroup;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the group's header rectangle
	//     area.
	// Returns:
	//     A CRect that contains the rectangle coordinates of the group view
	//     header part in client coordinates of the parent's
	//     CXTPDatePickerControl object.
	// See Also:
	//     GetRect
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderRect() const
	{
		return m_Layout.m_rcGroupHeader;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the bounding rectangle of
	//     the tooltip space (currently this is only "click to add new appointment").
	//     Tooltip rectangle is calculated basing on the current mouse position
	// Parameters:
	//     ptHit - a CPoint of mouse coordinates.
	//     hitInfo - a XTP_CALENDAR_HITTESTINFO object.
	// Returns:
	//     A CRect object with relative coordinates of the tooltip.
	// Remarks:
	//     This member function always returns an empty rectangle.
	//     Actual tooltip rectangle is calculated by ViewGroups of specific
	//     views (currently DayView and MonthView are supported).
	//-----------------------------------------------------------------------
	virtual CRect GetTooltipRect(const CPoint& ptHit, const XTP_CALENDAR_HITTESTINFO& hitInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the number of event views
	//     in the day view collection.
	// Remarks:
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// Returns:
	//     An int that contains the number of event views.
	//-----------------------------------------------------------------------
	int GetViewEventsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to a
	//     CXTPCalendarViewEvent object associated with the event's index.
	// Parameters:
	//     nIndex  - An int that contains the event view index in the day
	//               view collection.
	// Remarks:
	//     Index number starts with 0 and cannot be negative.
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// Returns:
	//     A pointer to a CXTPCalendarViewEvent object.
	// See Also: GetViewEventsCount()
	//-----------------------------------------------------------------------
	CXTPCalendarViewEvent* GetViewEvent_(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the containing
	//     CXTPCalendarViewDay object.
	// Returns:
	//     A pointer to the CXTPCalendarViewDay object.
	//-----------------------------------------------------------------------
	CXTPCalendarViewDay* GetViewDay_() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the index of the "this"
	//     pointer Group View in the Calendar Day View.
	// Returns:
	//     An int that contains the index of the "this" day in the Calendar
	//     Day View.
	//-----------------------------------------------------------------------
	virtual int GetGroupIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a group's view caption.
	// Returns:
	//     A CString object that contains the group's view caption.
	//-----------------------------------------------------------------------
	virtual CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the number of
	//     event views elements in this Group View.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array
	//     of event views inside this Group View.
	//     Because indexes are zero-based, the size is 1 greater than
	//     the largest index.
	// Returns:
	//     A count of event views elements.
	// See Also:
	//     GetChildHandlerAt
	//-----------------------------------------------------------------------
	virtual int GetChildHandlersCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a event view object at the
	//     specified numeric index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetChildHandlersCount.
	// Remarks:
	//     Returns the array element at the specified index.
	// Returns:
	//     The pointer to the descendant of CXTPCalendarWMHandler element
	//     currently at this index.
	// See Also:
	//     GetChildHandlersCount
	//-----------------------------------------------------------------------
	virtual CXTPCalendarWMHandler* GetChildHandlerAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer a CXTPCalendarViewEvent
	//     object that is identified by the corresponding event object.
	// Parameters:
	//     pEvent  - Pointer to a CXTPCalendarEvent object.
	// Returns:
	//     A pointer to a CXTPCalendarViewEvent object.
	// See Also: CXTPCalendarEvent, CXTPCalendarViewEvent
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEvent* GetViewEventByEvent_(CXTPCalendarEvent* pEvent) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain an event view object identified
	//     by its event place number.
	// Parameters:
	//     nEventPlace - An int that contains the ViewEvent place identifier.
	// Returns:
	//     A pointer to a CXTPCalendarViewEvent object.
	// See Also: CXTPCalendarEvent, CXTPCalendarViewEvent
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEvent* GetViewEventByPlace_(int nEventPlace) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the calendar control
	//     object.
	// Returns:
	//     A pointer to a CXTPCalendarControl object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarControl* GetCalendarControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to exchange event view positions.
	// Parameters:
	//     pViewEvent - A pointer to a CXTPCalendarViewEvent object.
	//     nNewPlace  - An int that contains the new event view position.
	// Remarks:
	//     Each event view has a unique position number for the day. In the
	//     simple case, this is the row number used to draw the event view.
	//     This function is used to avoid duplicating position numbers.
	// See Also: CXTPCalendarViewEvent::ChangeEventPlace,
	//           CXTPCalendarViewEvent::GetEventPlaceNumber,
	//           CXTPCalendarViewEvent::AddMultiDayEvent_Slave,
	//           CXTPCalendarViewEvent::AdjustLayout
	//-----------------------------------------------------------------------
	virtual void ExchangeEventPlace(CXTPCalendarViewEvent* pViewEvent, int nNewPlace);


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to create a timer event.
	// Parameters:
	//     uTimeOut_ms - A UINT that contains the duration of the timer in
	//                   milliseconds.
	// Returns:
	//     A UINT that is the identifier of the timer.
	//-----------------------------------------------------------------------
	virtual UINT SetTimer(UINT uTimeOut_ms);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to destroy a timer event.
	// Parameters:
	//     uTimerID - A UINT that specifies the identifier of the timer.
	//-----------------------------------------------------------------------
	virtual void KillTimer(UINT uTimerID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to allow some customization before
	//     show events.
	// Parameters:
	//     pEvents - A pointer to events array to draw.
	// Remarks:
	//     Default implementation send XTP_NC_CALENDAR_PREPOPULATE notification.
	// See Also: XTP_NC_CALENDAR_PREPOPULATE
	//-----------------------------------------------------------------------
	virtual void OnPrePopulate(CXTPCalendarEvents* pEvents);

public:
	virtual void AddViewEvent(CXTPCalendarEvent* pEvent);

protected:
	virtual CXTPCalendarViewEvent* CreateViewEvent(CXTPCalendarEvent* pEvent) = 0;

protected:
	XTP_VIEW_GROUP_LAYOUT& GetLayout_();

protected:
	XTP_VIEW_GROUP_LAYOUT m_Layout; // This group view client coordinates.

	CXTPCalendarPtrCollectionT<CXTPCalendarViewEvent>  m_arEvents; // Storage for events views.

	CXTPCalendarResources*  m_pResources;   // An associated resource object.

	CXTPCalendarViewDay* m_pViewDay; // Parent day view

#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarViewGroup);

	LPDISPATCH OleGetViewDay();
	LPDISPATCH OleGetViewEvents();
	LPDISPATCH OleGetMultipleResources();
	//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class represents the day view portion of the CalendarView.
// Remarks:
//     It represents a specific view of the CalendarView's associated events
//     data grouped by one day and provides basic functionality on this
//     data using user input through the keyboard and the mouse.
//
//          These are the predefined view implementations available
//          for the Calendar control:
//
//
//          * Day and work week day views - Using the CXTPCalendarDayViewDay class.
//          * Week day view               - Using the CXTPCalendarWeekViewDay class.
//          * Month day view              - Using the CXTPCalendarMonthViewDay class.
//
//
//          Furthermore, any user defined day view may also be implemented as
//          a descendant of the CXTPCalendarViewDay class.
//
//          A typical day view model consists of a collection of ViewEvent's for
//          the particular day, each of them represents a view of an CXTPCalendarEvent
//          object.
//
// See Also: CXTPCalendarViewDayT, CXTPCalendarDayViewDay,
//           CXTPCalendarWeekViewDay, CXTPCalendarMonthViewDay,
//           CXTPCalendarView, CXTPCalendarViewEvent
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarViewDay : public CXTPCalendarWMHandler
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPCalendarView;
	friend class CXTPCalendarViewEvent;
	friend class CXTPCalendarTheme;

	DECLARE_DYNAMIC(CXTPCalendarViewDay)

	typedef CXTPCalendarWMHandler TBase;
	//}}AFX_CODEJOCK_PRIVATE
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor. Construct CXTPCalendarViewDay object.
	// Parameters:
	//     pView - Pointer to CXTPCalendarView object.
	// See Also: ~CXTPCalendarViewDay()
	//-----------------------------------------------------------------------
	CXTPCalendarViewDay(const CXTPCalendarView* pView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles member item deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewDay();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to populate the view with data for
	//     all items contained in the view.
	// Parameters:
	//     dtDayDate - A COleDateTime object that contains the day's date.
	// Remarks:
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	//-----------------------------------------------------------------------
	virtual void Populate(COleDateTime dtDayDate);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout depending
	//     on the provided rectangle and then calls AdjustLayout() for all
	//     of the sub-items.
	// Parameters:
	//     pDC     - A pointer to a valid device context.
	//     rcDay   - A CRect object that contains the rectangle coordinates
	//               that are used to draw the view.
	// Remarks:
	//     Call Populate(COleDateTime dtDayDate) prior to calling AdjustLayout().
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcDay);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the view contents using
	//     the specified device context.
	// Parameters:
	//     pDC - A pointer to a valid device context.
	// Remarks:
	//     Call AdjustLayout() before calling Draw().
	// See Also: AdjustLayout(CRect rcDay)
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC)
	{
		UNREFERENCED_PARAMETER(pDC);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the number of
	//     resource group views elements in the collection.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array.
	//     Because indexes are zero-based, the size is 1 greater than
	//     the largest index.
	// Returns:
	//     A count of resource group views elements.
	//-----------------------------------------------------------------------
	virtual int GetChildHandlersCount() const { return GetViewGroupsCount(); }

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a resource group view object at the
	//     specified numeric index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetChildHandlersCount.
	// Remarks:
	//     Returns the array element at the specified index.
	// Returns:
	//     The pointer to the CXTPCalendarWMHandler element currently at this
	//     index.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarWMHandler* GetChildHandlerAt(int nIndex) const
	{
		ASSERT_KINDOF(CXTPCalendarWMHandler, GetViewGroup_(nIndex));
		return GetViewGroup_(nIndex);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at the specified position index, and returns additional
	//     info in an XTP_CALENDAR_HITTESTINFO struct.
	// Parameters:
	//     pt       - A CPoint object that contains the coordinates of
	//                the point to test.
	//     pHitTest - A pointer to an XTP_CALENDAR_HITTESTINFO struct that
	//                contains information about the point to test.
	// Remarks:
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at the specified position index, and returns additional
	//     info in the XTP_CALENDAR_HITTESTINFO template object.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point test.
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     Implements standard functionality for the HitTestEx method.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill the pHitTest structure
	//     members with the default values for the current day view object.
	// Parameters:
	//     pHitTest - A Pointer to a XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	//-----------------------------------------------------------------------
	virtual void FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to a
	//     CXTPCalendarView object.
	// Remarks:
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// Returns:
	//     A pointer to a CXTPCalendarView object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarView* GetView_() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to a
	//     CXTPCalendarControl object.
	// Returns:
	//     A pointer to a CXTPCalendarControl object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarControl* GetCalendarControl() const;

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
	//     This member function sets the new Resources collection for the view day.
	// Parameters:
	//     pResources - A pointer to the CXTPCalendarResources collection object.
	// Remarks:
	//     Call this member function to programmatically set the new
	//     Resources collection for the view day.
	//-----------------------------------------------------------------------
	virtual void SetResources(CXTPCalendarResources* pResources);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the number of event views
	//     in the day view collection.
	// Remarks:
	//     This function left for for compatibility with previous versions
	//     and only for singe resource group mode.
	// Returns:
	//     An int that contains the number of event views.
	//-----------------------------------------------------------------------
	virtual int GetViewEventsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to a
	//     CXTPCalendarViewEvent object associated with the event's index.
	// Parameters:
	//     nIndex  - An int that contains the event view index in the day
	//               view collection.
	// Remarks:
	//     Index number starts with 0 and cannot be negative.
	//     This function left for for compatibility with previous versions
	//     and only for singe resource group mode.
	// Returns:
	//     A pointer to a CXTPCalendarViewEvent object.
	// See Also: GetViewEventsCount()
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewEvent* GetViewEvent_(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the number of resource group
	//     views in the view day collection.
	// Remarks:
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// Returns:
	//     An int that contains the number of event views.
	//-----------------------------------------------------------------------
	int GetViewGroupsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to a
	//     CXTPCalendarViewGroup object associated with the group's index.
	// Parameters:
	//     nIndex  - An int that contains the group view index in the day
	//               view collection.
	// Remarks:
	//     Index number starts with 0 and cannot be negative.
	//     This is a pure virtual function. This function must be defined
	//     in the derived class.
	// Returns:
	//     A pointer to a CXTPCalendarViewGroup object.
	// See Also: GetViewGroupsCount()
	//-----------------------------------------------------------------------
	CXTPCalendarViewGroup* GetViewGroup_(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a day date value.
	// Returns:
	//     A COleDateTime object containing the day's date.
	// See Also: SetDayDate(COleDateTime dtDayDate)
	//-----------------------------------------------------------------------
	virtual COleDateTime GetDayDate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the day date value.
	// Parameters:
	//     dtDayDate   - A COleDateTime object that contains the new day's
	//                   date value.
	// Returns:
	//     A COleDateTime object that contains the previous day's date value.
	// See Also: GetDayDate()
	//-----------------------------------------------------------------------
	virtual COleDateTime SetDayDate(COleDateTime dtDayDate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if all day's event
	//     views are visible.
	// Remarks:
	//     This function is used for determining if the expanding sings are
	//     drawn.
	// Returns:
	//     A BOOL. TRUE if there is at least one event that is completely
	//     invisible. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL _NoAllEventsAreVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if all of the day's event
	//     views are not visible and to calculate the rectangle for the ExpandSign.
	// Returns:
	//     TRUE if there are invisible event views. FALSE otherwise.
	// See Also: CXTPCalendarViewDay::_NoAllEventsAreVisible
	//-----------------------------------------------------------------------
	virtual BOOL NoAllEventsAreVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the day view is selected.
	// Returns:
	//     A BOOL. TRUE if the day view is selected. FALSE otherwise.
	// See Also: SetSelected(bSet)
	//-----------------------------------------------------------------------
	virtual BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the day view as selected
	// See Also: IsSelected()
	//-----------------------------------------------------------------------
	virtual void SetSelected();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the day view rectangle coordinates.
	// Returns:
	//     A CRect object containing the day view rectangle coordinates.
	// See Also: SetDayRect(CRect rcDay)
	//-----------------------------------------------------------------------
	virtual CRect GetDayRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the day view rectangle coordinates.
	// Parameters:
	//     rcDay - A CRect object containing the new rectangle coordinates.
	// See Also: GetDayRect()
	//-----------------------------------------------------------------------
	virtual void SetDayRect(CRect rcDay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if there are event views
	//     invisible because the boundaries of the day view are hidden, and
	//     the day view should display the expand sign.
	// Returns:
	//     A BOOL. TRUE if there are invisible views. FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the expand sign rectangle.
	// Returns:
	//     A CRect object that contains the expand sign rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual CRect GetExpandSignRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to create a timer event.
	// Parameters:
	//     uTimeOut_ms - A UINT that contains the duration of the timer in
	//                   milliseconds.
	// Returns:
	//     A UINT that is the identifier of the timer.
	//-----------------------------------------------------------------------
	virtual UINT SetTimer(UINT uTimeOut_ms);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to destroy a timer event.
	// Parameters:
	//     uTimerID - A UINT that specifies the identifier of the timer.
	//-----------------------------------------------------------------------
	virtual void KillTimer(UINT uTimerID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the index of the "this"
	//     pointer day in the Calendar Day View.
	// Returns:
	//     An int that contains the index of the "this" day in the Calendar
	//     Day View.
	//-----------------------------------------------------------------------
	virtual int GetDayIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function used to process user action xtpCalendarUserAction_OnExpandDay.
	// Parameters:
	//      eButton - [in] a button from which this action comes.
	// Remarks:
	//      Implementation send a XTP_NC_CALENDAR_USERACTION notification.
	//
	// Returns:
	//      TRUE if action was handled by user and default processing must  be
	//      skipped, FALSE otherwise.
	// See Also: XTP_NC_CALENDAR_USERACTION
	//-----------------------------------------------------------------------
	virtual BOOL UserAction_OnExpandDay(XTPCalendarExpandDayButton eButton);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to allow some customization before
	//     show events.
	// Remarks:
	//     Default implementation send XTP_NC_CALENDAR_PREPOPULATE_DAY notification.
	// See Also: XTP_NC_CALENDAR_PREPOPULATE
	//-----------------------------------------------------------------------
	virtual void OnPrePopulateDay();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to crete new instance of group.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewGroup* CreateGroup() = 0;




public:
	const XTP_VIEW_DAY_LAYOUT& GetLayout_() const;
protected:
	//{{AFX_CODEJOCK_PRIVATE
	XTP_VIEW_DAY_LAYOUT& GetLayout_();
	//}}AFX_CODEJOCK_PRIVATE
protected:
	XTP_VIEW_DAY_LAYOUT m_Layout; // Layout data.

	COleDateTime m_dtDate;          // Day date.

	CXTPCalendarResources*  m_pResources; // Resources array

	CXTPCalendarPtrCollectionT<CXTPCalendarViewGroup> m_arViewGroups; // Storage for views of groups.

	CXTPCalendarView* m_pView; // Storage for a owner view.

protected:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarViewDay);

	DATE OleGetDate();
	LPDISPATCH OleGetView();
	LPDISPATCH OleGetViewGroups();

	LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();

	LPDISPATCH OleGetMultipleResources();
	void OleSetMultipleResources(LPDISPATCH pDispResources);

	DECLARE_ENUM_VARIANT(CXTPCalendarViewDay);

//}}AFX_CODEJOCK_PRIVATE
#endif

};

//===========================================================================
// Summary:
//     This class template customizes, overrides and implements some
//     functionality for the CXTPCalendarViewDay base class.
// Remarks:
//     It is used as a part of the calendar control framework to build
//     <b><i>ViewDay</i></b> layer of <b>View->DayView->ViewDay</b>
//     typed objects hierarchy.
//     These are template parameters:
//     _TView      - Type of owner View object.
//     _TViewGroup - Type of View Resource objects stored in View Day.
//     XTP_CALENDAR_HITTESTINFO   - Type of HitTest struct, used as parameter in the
//                   member functions.
//     _TPThis     - The last derived class type in the class hierarchy.
//
//          You must provide all of the above parameters.
// See Also: CXTPCalendarViewDay
//===========================================================================
template<class _TView, class _TViewGroup, class XTP_CALENDAR_HITTESTINFO, class _TPThis >
class CXTPCalendarViewDayT : public CXTPCalendarViewDay
{
public:
	//------------------------------------------------------------------------
	// Remarks:
	//     Owner view class type definition.
	//------------------------------------------------------------------------
	typedef _TView TView;


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pView - Pointer to owner view object.
	// See Also: ~CXTPCalendarViewDayT()
	//-----------------------------------------------------------------------
	CXTPCalendarViewDayT(const _TView* pView)
		: CXTPCalendarViewDay((CXTPCalendarView*)pView)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles class members deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewDayT()
	{
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill the pHitTest structure
	//     members with the default values for the current day view object.
	// Parameters:
	//     pHitTest - A Pointer to a XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	//-----------------------------------------------------------------------
	virtual void FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the owner view object.
	// Returns:
	//     A _TView template pointer to the owner view object.
	// See Also: GetView_
	//-----------------------------------------------------------------------
	_TView* GetView() const
	{
		return (TView*)GetView_();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the _TViewGroup
	//     object that corresponds to the resource's index.
	// Parameters:
	//     nIndex  - An int that contains the resource view index in the day
	//               view collection.
	// Returns:
	//     A pointer to a _TViewGroup template object.
	// Remarks:
	//     Index number starts with 0 and cannot be negative.
	// See Also:
	//     GetViewGroupsCount(), GetViewGroup_()
	//-----------------------------------------------------------------------
	virtual _TViewGroup* GetViewGroup(int nIndex) const
	{
		return (_TViewGroup*)GetViewGroup_(nIndex);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to crete new instance of group.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewGroup* CreateGroup()
	{
		return new _TViewGroup(GetPThis());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout depending
	//     on the provided rectangle and then calls AdjustLayout() for all
	//     of the sub-items.
	// Parameters:
	//     pDC     - A pointer to a valid device context.
	//     rcDay   - A CRect object that contains the rectangle coordinates
	//               that are used to draw the view.
	// Remarks:
	//     Call Populate(COleDateTime dtDayDate) prior to calling AdjustLayout().
	//-----------------------------------------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcDay)
	{
		CXTPCalendarViewDay::AdjustLayout(pDC, rcDay);

		int nCount = GetViewGroupsCount();
		for (int i = 0; i < nCount; i++)
		{
			CXTPCalendarViewGroup* pViewGroup = GetViewGroup_(i);
			ASSERT(pViewGroup);
			if (pViewGroup)
			{
				pViewGroup->AdjustLayout(pDC, rcDay);
			}
		}
	}


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the typed pointer to the
	//     last derived class in the class hierarchy.
	// Remarks:
	//     _TPThis - The last derived class type in the class hierarchy.
	//               This is a pure virtual function. This function must be defined
	//               in the derived class.
	// Returns:
	//     Pointer to this object as last derived class type.
	//-----------------------------------------------------------------------
	virtual _TPThis* GetPThis() = 0;


private:
};

////////////////////////////////////////////////////////////////////////////

AFX_INLINE COleDateTime CXTPCalendarViewDay::GetDayDate() const {
	return m_dtDate;
}
AFX_INLINE CRect CXTPCalendarViewDay::GetDayRect() const {
	return m_Layout.m_rcDay;
}
AFX_INLINE void CXTPCalendarViewDay::SetDayRect(CRect rcDay) {
	m_Layout.m_rcDay.CopyRect(rcDay);
}
AFX_INLINE BOOL CXTPCalendarViewDay::IsExpanded() const {
	return m_Layout.m_rcExpandSign.Width() > 0 ? TRUE : FALSE;
}
AFX_INLINE CRect CXTPCalendarViewDay::GetExpandSignRect() const {
	return m_Layout.m_rcExpandSign;
}

//===========================================================================
// Summary:
//     This class template customizes, overrides and implements some
//     functionality for the CXTPCalendarViewGroup base class.
// Remarks:
//     It is used as a part of the calendar control framework to build
//     <b><i>ViewGroup</i></b> layer of <b>View->DayView->ViewDay->ViewGroup</b>
//     typed objects hierarchy.
//     These are template parameters:
//     _TView      - Type of owner View object.
//     _TViewEvent - Type of View Event objects stored in Group View.
//     XTP_CALENDAR_HITTESTINFO   - Type of HitTest struct, used as parameter in the
//                   member functions.
//     _TPThis     - The last derived class type in the class hierarchy.
//
//          You must provide all of the above parameters.
// See Also: CXTPCalendarViewGroup
//===========================================================================
template<class _TViewDay, class _TViewEvent, class _TPThis>
class CXTPCalendarViewGroupT : public CXTPCalendarViewGroup
{
public:
	//------------------------------------------------------------------------
	// Remarks:
	//     Owner view class type definition.
	//------------------------------------------------------------------------
	typedef _TViewDay TViewDay;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pViewDay - Pointer to owner view Day object.
	// See Also: ~CXTPCalendarViewGroupT()
	//-----------------------------------------------------------------------
	CXTPCalendarViewGroupT(_TViewDay* pViewDay)
		: CXTPCalendarViewGroup(pViewDay)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles class members deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarViewGroupT()
	{
	};


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill the pHitTest structure
	//     members with the default values for the current group view object.
	// Parameters:
	//     pHitTest - A Pointer to a XTP_CALENDAR_HITTESTINFO structure.
	// Remarks:
	//     This is a pure virtual function.  This function must be defined
	//     in the derived class.
	//-----------------------------------------------------------------------
	virtual void FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain the owner view object.
	// Returns:
	//     A _TView template pointer to the owner view object.
	// See Also: GetView_
	//-----------------------------------------------------------------------
	_TViewDay* GetViewDay() const
	{
		return (_TViewDay*)GetViewDay_();
	}


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the _TViewEvent
	//     object that corresponds to the event's index.
	// Parameters:
	//     nIndex  - An int that contains the event view index in the day
	//               view collection.
	// Returns:
	//     A pointer to a _TViewEvent template object.
	// Remarks:
	//     Index number starts with 0 and cannot be negative.
	// See Also:
	//     GetViewEventsCount(), GetViewEvent_()
	//-----------------------------------------------------------------------
	virtual _TViewEvent* GetViewEvent(int nIndex) const
	{
		return (_TViewEvent*)GetViewEvent_(nIndex);
	}


	CXTPCalendarViewEvent* CreateViewEvent(CXTPCalendarEvent* pEvent)
	{
		return new _TViewEvent(pEvent, GetPThis());
	}

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the typed pointer to the
	//     last derived class in the class hierarchy.
	// Remarks: _TPThis - The last derived class type in the class hierarchy.
	//          This is a pure virtual function. This function must be defined
	//          in the derived class.
	// Returns:
	//     Pointer to this object as last derived class type.
	//-----------------------------------------------------------------------
	virtual _TPThis* GetPThis() = 0;


private:
};

////////////////////////////////////////////////////////////////////////////
AFX_INLINE XTP_VIEW_DAY_LAYOUT& CXTPCalendarViewDay::GetLayout_() {
	return m_Layout;
}
AFX_INLINE const XTP_VIEW_DAY_LAYOUT& CXTPCalendarViewDay::GetLayout_() const  {
	return m_Layout;
}
AFX_INLINE CXTPCalendarResources* CXTPCalendarViewGroup::GetResources() const {
	return m_pResources;
}
AFX_INLINE int CXTPCalendarViewGroup::GetChildHandlersCount() const {
	return GetViewEventsCount();
}
AFX_INLINE XTP_VIEW_GROUP_LAYOUT& CXTPCalendarViewGroup::GetLayout_() {
	return m_Layout;
}
AFX_INLINE CXTPCalendarControl* CXTPCalendarViewGroup::GetCalendarControl() const {
	if (GetViewDay_())
		return GetViewDay_()->GetCalendarControl();
	return NULL;
}


AFX_INLINE int CXTPCalendarViewGroup::GetViewEventsCount() const {
	return m_arEvents.GetCount();
}

AFX_INLINE CXTPCalendarViewEvent* CXTPCalendarViewGroup::GetViewEvent_(int nIndex) const
{
	ASSERT(this);
	if (!this)
	{
		return NULL;
	}

	int nCount = m_arEvents.GetCount();
	ASSERT(nIndex >= 0 && nIndex < nCount);

	return (nIndex >= 0 && nIndex < nCount) ? m_arEvents.GetAt(nIndex) : NULL;
}

AFX_INLINE int CXTPCalendarViewDay::GetViewGroupsCount() const {
	return m_arViewGroups.GetCount();
}
AFX_INLINE  CXTPCalendarViewGroup* CXTPCalendarViewDay::GetViewGroup_(int nIndex) const
{
	ASSERT(this);
	if (!this)
	{
		return NULL;
	}
	int nCount = m_arViewGroups.GetCount();
	ASSERT(nIndex >= 0 && nIndex < nCount);
	return (nIndex >= 0 && nIndex < nCount) ? m_arViewGroups.GetAt(nIndex) : NULL;
}

AFX_INLINE CXTPCalendarViewDay* CXTPCalendarViewGroup::GetViewDay_() const
{
	ASSERT(this);
	return this ? m_pViewDay : NULL;
}

AFX_INLINE CXTPCalendarView* CXTPCalendarViewDay::GetView_() const {
	ASSERT(this);
	return this ? m_pView : NULL;
}


////////////////////////////////////////////////////////////////////////////
#endif // !defined(_XTPCALENDARVIEWDAY_H__)
