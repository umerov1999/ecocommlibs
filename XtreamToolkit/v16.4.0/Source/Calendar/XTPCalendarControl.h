// XTPCalendarControl.h: interface for the CXTPCalendarControl class.
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
#if !defined(_XTPCALENDARCONTROL_H__)
#define _XTPCALENDARCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCalendarView;
class CXTPCalendarPaintManager;
class CXTPCalendarData;
class CXTPCalendarDayView;
class CXTPCalendarWeekView;
class CXTPCalendarMonthView;
class CXTPCalendarTimeLineView;
class CXTPCalendarOptions;
class CXTPNotifyConnection;
class CXTPNotifySink;
class CXTPCalendarRemindersManager;
class CXTPCalendarResources;
class CXTPCalendarResourcesNf;
class CXTPCalendarTheme;

class CXTPMarkupContext;
class CXTPMarkupUIElement;
class CXTPCalendarTip;

typedef DWORD_PTR XTP_CONNECTION_ID;

//===========================================================================
// Summary:
//     Class CXTPCalendarControl implements the calendar's view interface.
// Remarks:
//     A CXTPCalendarControl object implements a calendar view interface
//     similar to Outlook 2003 interface.
//
//     There are 2 main separated concepts: Calendar data contents
//     and Calendar views representing this contents.
//
//     It can show 3 types of following predefined views. Also any type
//     of user view could also be implemented as a descendant of CXTPCalendarView class.
//
//     * Day and work week views - using CXTPCalendarDayView class.
//     * Week view - using CXTPCalendarWeekView class.
//     * Month view - using CXTPCalendarMonthView class.
//
//     One of the other main Control's concepts is Calendar Resources.
//     Resource is the source of Calendar data contents with some additional
//     properties. This concept is designed to represent a schedule of a
//     separate person, or a shared resource (sic!) like a conference room,
//     a projector, and any similar entity. Each Resource has its own Data
//     Provider, therefore the Control can show different data sources on the
//     same screen. For example, you can configure Control to show your own
//     Outlook events in "your" resource (using MAPI Data Provider), then
//     you can use some shared resource data using Database Data Provider, etc.
//
//     Note that each event now has standard "Schedule" property, defining
//     which user's schedule it belongs to. And each resource has a collection
//     of Schedule ID's to represent on it. For example, you can have only
//     one common Data Provider which stores events data for all members of
//     your organization. Every member has a corresponding schedule (i.e.
//     Jeff - ID 1, Matt - ID 2, John - ID 3, Room 1 - ID 10, Room 2 - ID 11, etc).
//     You can configure a number of resources to show data for every team
//     member separately, and one resource to show events for both room 1 and
//     room 2 together. Or for example, you want to show a common view of all
//     events for a specified team including only some of employees. You will
//     add their schedule ID's to the corresponding Resource collection, and
//     see all of them. Of course, when you open event details, you'll see
//     who it belongs to.
//
//
//     There are 2 regular models of using your custom data source with Calendar control.
//
//     a) Firstly, you can use built-in Memory data provider, which will handle
//        all the data in itself. If you do not specify neither XML nor Binary
//        filename, it will not be automatically loaded or saved. In this case,
//        you'll have 2 entry points in your application:
//        -- Loading all data from your data source (could be done on form load
//           for example). Your data access code would transfer CalendarEvents
//           records from the data storage to objects and add them into
//           corresponding Calendar's collection.
//        -- Saving data to your data storage. You'll have to iterate all CalendarEvents,
//           transfer them into your data records and upload into the database.
//           This could be implemented on pressing Save button for example.
//        This is the most easy way.
//
//     b) Second idea is to handle basic data storage operations by yourself.
//        This is called "custom data provider model". It could be needed when
//        you have a very lot of CalendarEvents records and do not want to store
//        them all in memory because of a huge size. Another possible need of
//        using this model is having all the information always updated if your
//        data storage is shared.
//
//
//     For some predefined examples of this functionality look Calendar Samples,
//     and menu items: MultiSchedules configuration and Load sample configurations...
//
// See Also: CXTPCalendarView, CXTPCalendarData, CXTPCalendarResource
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarControl : public CWnd
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPCalendarView;
	friend class CXTPCalendarViewDay;
	friend class CXTPCalendarViewGroup;
	friend class CXTPCalendarViewEvent;
	friend class CCalendarControlCtrl;
	friend class CXTPCalendarControlView;
	friend class CXTPCalendarResource;
	friend class CXTPCalendarThemePart;
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function constructs a CXTPCalendarControl object.
	// Remarks:
	//     Construct a CXTPCalendarControl object in two steps.
	//     First, call the constructor CXTPCalendarControl and then
	//     call Create method, which initializes the window.
	//
	// Example:
	// <code>
	// Declare a local CXTPCalendarControl object.
	// CXTPCalendarControl myCalendar;
	//
	// // Declare a dynamic CXTPCalendarControl object.
	// CXTPCalendarControl* pMyCalendar = new CXTPCalendarControl();
	//
	// // Create a window
	// if (!myCalendar.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rcRect, this, ID_CALENDAR_CONTROL))
	// {
	//     TRACE(_T("Failed to create calendar control window\n"));
	// }
	// </code>
	// See Also: Create
	//-----------------------------------------------------------------------
	CXTPCalendarControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function destroys a CXTPCalendarControl object and
	//     handles cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function handles control window creation.
	// Parameters:
	//     dwStyle - Specifies the window style attributes.
	//     rect - The size and position of the window, in client coordinates of pParentWnd.
	//     pParentWnd - The parent window.
	//     nID - The ID of the child window.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function populates the active control view with
	//     events data.
	// Remarks:
	//     Creates a new events views collection for the view, populating it
	//     with the data objects taken from the associated data provider.
	// See Also:
	//-----------------------------------------------------------------------
	virtual void Populate();

// Operations
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
	//     This member function is used to obtain current theme ID. Basically
	//     this can be used to check if 2010 of 2007 theme is used.
	// Returns:
	//     A theme version ID from enum XTPCalendarThemeVersion.
	//-----------------------------------------------------------------------
	XTPCalendarThemeVersion GetThemeVersion() const;

	/////////////////////////////////////////////////////////////////////////
	// drawing related

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the associated paint
	//     manager.
	// Remarks:
	//     Call this member function to obtain the paint manager object used
	//     for drawing a calendar control window.
	// Returns:
	//     A CXTPCalendarPaintManager pointer to the associated paint manager object.
	// See Also:
	//-----------------------------------------------------------------------
	CXTPCalendarPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the associated theme
	//     object.
	//
	// Remarks:
	//     Themes are used for drawing a calendar control window instead of
	//     a Paint manager.
	//     This is a new feature of Calendar control which will be further
	//     developed.
	//     Some new drawing features will be added for themes only
	//     (no for Paint manager). But the Paint manager will also be used
	//     for compatibility.
	//
	//     Initial version. Themes will be expanded in the feature.
	// Returns:
	//     A CXTPCalendarTheme pointer to the associated theme object or NULL.
	// See Also:
	//     CXTPCalendarView::GetTheme, GetPaintManager
	//-----------------------------------------------------------------------
	CXTPCalendarTheme* GetTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets (or disable) the new control drawing
	//     theme.
	// Parameters:
	//     pTheme - A CXTPCalendarTheme pointer to the theme object.
	//              If pTheme is NULL - themes are disabled and the paint
	//              manager is used.
	// Remarks:
	//     Themes are used for drawing a calendar control window instead of
	//     a Paint manager.
	//     This is a new feature of Calendar control which will be further
	//     developed.
	//     Some new drawing features will be added for themes only
	//     (no for Paint manager). But the Paint manager will also be used
	//     for compatibility.
	//
	//     Initial version. Themes will be expanded in the feature.
	// See Also: GetPaintManager
	//-----------------------------------------------------------------------
	void SetTheme(CXTPCalendarTheme* pTheme);

	/////////////////////////////////////////////////////////////////////////
	// resources related

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtain a pointer to collection of Calendar Resources.
	// Remarks:
	//     Use this method to retrieve a pointer to the Calendar Resources
	//     collection of this Calendar control.
	// Returns:
	//     CXTPCalendarResources pointer.
	// See Also:
	//     CXTPCalendarResources overview
	//-----------------------------------------------------------------------
	CXTPCalendarResources* GetResources() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the new Resources collection for the control.
	// Parameters:
	//     pResources - A pointer to the CXTPCalendarResources collection object.
	//     pOptionsDataProvider - A pointer to the Data Provider object for
	//                            using on Calendar Options.
	// Remarks:
	//     Call this member function to programmatically set the new
	//     Resources collection for the control.
	//-----------------------------------------------------------------------
	void SetResources(CXTPCalendarResources* pResources, CXTPCalendarData* pOptionsDataProvider = NULL);

	/////////////////////////////////////////////////////////////////////////
	// data provider related

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the custom data provider for the control.
	// Parameters:
	//     lpszConnectionString - A text Calendar connection string.
	// Remarks:
	//     Call this member function to set the custom data provider
	//     that is currently used by this calendar control. Note that
	//     custom data provider must be a descendant of CXTPCalendarData.
	// See Also:
	//     CXTPCalendarData, XTPCalendarDataProvider, GetDataProvider
	//-----------------------------------------------------------------------
	void SetDataProvider(LPCTSTR lpszConnectionString);
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the custom data provider for the control.
	// Parameters:
	//     pDataProvider - Pointer to the custom data provider object.
	//     bCloseDataProviderWhenDestroy - BOOL value shows whether data
	//                                     provider should be closed with the
	//                                     control's destruction.
	// Remarks:
	//     Call this member function to set the custom data provider
	//     that is currently used by this calendar control. Note that
	//     custom data provider must be a descendant of CXTPCalendarData.
	// See Also:
	//     CXTPCalendarData, XTPCalendarDataProvider, GetDataProvider
	//-----------------------------------------------------------------------
	void SetDataProvider(CXTPCalendarData* pDataProvider, BOOL bCloseDataProviderWhenDestroy = TRUE);
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the custom data provider for the control.
	// Parameters:
	//     eDataProvider - Data provider type.
	//     lpszConnectionString - A text Calendar connection string.
	// Remarks:
	//     Call this member function to set the custom data provider
	//     that is currently used by this calendar control. Note that
	//     custom data provider must be a descendant of CXTPCalendarData.
	// See Also:
	//     CXTPCalendarData, XTPCalendarDataProvider, GetDataProvider
	//-----------------------------------------------------------------------
	void SetDataProvider(XTPCalendarDataProvider eDataProvider, LPCTSTR lpszConnectionString = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns common data provider if it was previously set.
	// Returns:
	//     A pointer to the common data provider if it was previously set;
	//     otherwise, returns NULL.
	//-----------------------------------------------------------------------
	CXTPCalendarData* GetDataProvider() const;

	/////////////////////////////////////////////////////////////////////////
	// view related

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the current calendar view.
	// Remarks:
	//     Call this member function to obtain the calendar view object
	//     currently used in this calendar control.
	// Returns:
	//     A CXTPCalendarView pointer to the associated calendar view object.
	// See Also:
	//     CXTPCalendarView overview, SetActiveView,
	//     GetViewDay, GetWeekView, GetMonthView
	//-----------------------------------------------------------------------
	virtual CXTPCalendarView* GetActiveView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the Day calendar view.
	// Remarks:
	//     Call this member function to obtain Day calendar view object
	//     regardless of its visibility.
	// Returns:
	//     A CXTPCalendarView pointer to the associated calendar view object.
	// See Also:
	//     CXTPCalendarView overview, GetActiveView, GetWeekView, GetMonthView
	//-----------------------------------------------------------------------
	CXTPCalendarDayView* GetDayView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the Week calendar view.
	// Remarks:
	//     Call this member function to obtain Week calendar view object
	//     regardless of its visibility.
	// Returns:
	//     A CXTPCalendarView pointer to the associated calendar view object.
	// See Also:
	//     CXTPCalendarView overview, GetActiveView, GetDayView, GetMonthView
	//-----------------------------------------------------------------------
	CXTPCalendarWeekView* GetWeekView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the Month calendar view.
	// Remarks:
	//     Call this member function to obtain Month calendar view object
	//     regardless of its visibility.
	// Returns:
	//     A CXTPCalendarView pointer to the associated calendar view object.
	// See Also:
	//     CXTPCalendarView overview, GetActiveView, GetWeekView, GetDayView
	//-----------------------------------------------------------------------
	CXTPCalendarMonthView* GetMonthView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a pointer to the TimeLine calendar view.
	// Remarks:
	//     Call this member function to obtain TimeLine calendar view object
	//     regardless of its visibility.
	// Returns:
	//     A CXTPCalendarView pointer to the associated calendar view object.
	// See Also:
	//     CXTPCalendarView overview, GetActiveView, GetWeekView, GetDayView, GetMonthView
	//------------------------------------------------------------------------------------
	CXTPCalendarTimeLineView* GetTimeLineView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets a predefined calendar view for the control.
	// Parameters:
	//     eView - Calendar view type.
	// Remarks:
	//     Call this member function to set the predefined calendar view
	//     that is currently used by this calendar control.
	// See Also: CXTPCalendarView overview, XTPCalendarViewType, GetAvtiveView
	//-----------------------------------------------------------------------
	virtual void SwitchActiveView(XTPCalendarViewType eView);

// Implementation
public:
	/////////////////////////////////////////////////////////////////////////
	// drawing related

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set right-to-left mode.
	// Parameters:
	//     bRightToLeft - TRUE to set right-to-left reading-order properties.
	//-----------------------------------------------------------------------
	void SetLayoutRTL(BOOL bRightToLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if right-to-left mode is active
	// Returns:
	//     TRUE if right-to-left mode is enabled and FALSE otherwise
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get current TimeLine mode.
	// Returns:
	//     An integer representing one of the following values.
	//
	//     xtpTSPID_Day      = 1,
	//     xtpTSPID_Week     = 2,
	//     xtpTSPID_Month    = 3,
	//     xtpTSPID_WorkWeek = 4,
	//-----------------------------------------------------------------------
	int GetTimeLineScale() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function initiates the Calendar control's redrawing.
	// Parameters:
	//     bForce : Set this parameter to TRUE if you'd like to force
	//              control's redrawing even if update is locked.
	// Remarks:
	//     Call this member function if you want to force the control's
	//     redrawing. The control will be redrawn to reflect its latest state.
	// See Also: CXTPCalendarControl::CUpdateContext, AdjustLayout, AdjustScrollBar,
	//           LockUpdate, UnlockUpdate.
	//-----------------------------------------------------------------------
	virtual void RedrawControl(BOOL bForce = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adjusts the main control parts depending
	//     on the current control's size.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcView   - A rectangle for calendar control.
	// See Also:
	//     CXTPCalendarControl::CUpdateContext, AdjustScrollBar, RedrawControl
	//-----------------------------------------------------------------------
	virtual void AdjustLayout();
	virtual void AdjustLayout(CDC* pDC, const CRect& rcView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adjusts the scroll bar to reflect the current
	//     control's state.
	// See Also:
	//     CXTPCalendarControl::CUpdateContext, AdjustLayout, RedrawControl
	// Parameters:
	//     nSBType : Specifies the scroll bar type to be adjusted.
	//               This parameter can be either of the following:
	//               * SB_HORZ - Horizontal scroll bar.
	//               * SB_VERT - Vertical scroll bar.
	// ----------------------------------------------------------------------
	virtual void AdjustScrollBar(int nSBType = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function switch calendar to timeline mode
	//-----------------------------------------------------------------------
	void OnCalendarTimeline();

private:
//{{AFX_CODEJOCK_PRIVATE
	// Always updates the screen on exit block
	void BeginUpdate();
	void EndUpdate();

	// Update the screen on exit block only when something has changed
	void LockUpdate();
	void UnlockUpdate();

	// Adjusts layout on exit block
	void BeginAdjust(BOOL bWithScrollBar = FALSE);
	void EndAdjust(BOOL bWithScrollBar = FALSE);
//}}AFX_CODEJOCK_PRIVATE

public:

	/////////////////////////////////////////////////////////////////////////
	// managing calendar settings

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the bitwise mask of the work week.
	// Returns:
	//     An integer containing the bitwise day mask.
	// See Also: SetWorkWeekMask, XTPCalendarWeekDay
	//-----------------------------------------------------------------------
	int GetWorkWeekMask() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the bitwise mask for the work week.
	// Parameters:
	//     nMask - An int that contains the new bitwise mask for the work week.
	// See Also: GetWorkWeekMask, XTPCalendarWeekDay
	//-----------------------------------------------------------------------
	void SetWorkWeekMask(const int nMask);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the first day of the week.
	// Returns:
	//     First day of the week represented by an integer from
	//     range between 1 and 7, where 1 = Sunday, 2 = Monday, and so on.
	// See Also: SetFirstDayOfWeek, XTPCalendarWeekDay
	//-----------------------------------------------------------------------
	int GetFirstDayOfWeek() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function give the first week of year days.
	// Returns:
	//     An integer denoting the first week of year days,
	//-----------------------------------------------------------------------
	int GetFirstWeekOfYearDays() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the first day of the week.
	// Parameters:
	//     nFirstDayOfWeek - An int that contains the new first day of the
	//                       week. An integer from range between 1 and 7,
	//                       where 1 = Sunday, 2 = Monday, and so on.
	// See Also: GetFirstDayOfWeek, XTPCalendarWeekDay
	//-----------------------------------------------------------------------
	void SetFirstDayOfWeek(const int nFirstDayOfWeek);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the first week of year days.
	// Parameter:
	//     nFirstWeekOfYearDays - An integer denoting the first week of year days,
	//-----------------------------------------------------------------------
	void SetFirstWeekOfYearDays(const int nFirstWeekOfYearDays);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the begin time of the working day.
	// Parameters:
	//     dtTime - A reference to the COleDateTime value to store the result.
	//     nHour - An int. A reference to the hour part of the result.
	//     nMin  - An int. A reference to the minutes part of the result.
	//     nSec  - An int. A reference to the seconds part of the result.
	// See Also: SetWorkDayStartTime, GetWorkDayEndTime, SetWorkDayEndTime
	//-----------------------------------------------------------------------
	void GetWorkDayStartTime(COleDateTime& dtTime) const;
	void GetWorkDayStartTime(int& nHour, int& nMin, int& nSec) const; // <COMBINE CXTPCalendarControl::GetWorkDayStartTime@COleDateTime&@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains the end time of the working day.
	// Parameters:
	//     dtTime - A reference to the COleDateTime value to store the result.
	//     nHour - An int. A reference to the hour part of the result.
	//     nMin  - An int. A reference to the minutes part of the result.
	//     nSec  - An int. A reference to the seconds part of the result.
	// See Also: SetWorkDayEndTime, GetWorkDayStartTime, SetWorkDayStartTime
	//-----------------------------------------------------------------------
	void GetWorkDayEndTime(COleDateTime& dtTime) const;
	void GetWorkDayEndTime(int& nHour, int& nMin, int& nSec) const; // <COMBINE CXTPCalendarControl::GetWorkDayEndTime@COleDateTime&@const>

	//-----------------------------------------------------------------------
	// Summary:
	//      set format for Cell's Day Header in Month View
	// Parameters:
	//      pcszCustomFormat - An string sd custom format.
	//-----------------------------------------------------------------------
	void SetLongDayHeaderFormat(LPCTSTR pcszCustomFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//      set format for Cell's Day Header in Month View
	// Parameters:
	//      pcszCustomFormat - An string sd custom format.
	//-----------------------------------------------------------------------
	void SetSmallDayHeaderFormat(LPCTSTR pcszCustomFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the begin time for the working day.
	// Parameters:
	//     dtTime - A new begin time value.
	// See Also: GetWorkDayStartTime, GetWorkDayEndTime, SetWorkDayEndTime
	//-----------------------------------------------------------------------
	void SetWorkDayStartTime(const COleDateTime& dtTime);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the begin time for the working day.
	// Parameters:
	//     nHour - An int. The hour part of the new time value.
	//     nMin  - An int. The minutes part of the new time value.
	//     nSec  - An int. The seconds part of the new time value.
	// See Also: GetWorkDayStartTime, GetWorkDayEndTime, SetWorkDayEndTime
	//-----------------------------------------------------------------------
	void SetWorkDayStartTime(int nHour, int nMin, int nSec);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets an end time for the working day.
	// Parameters:
	//     dtTime - A new end time value.
	// See Also: GetWorkDayEndTime, GetWorkDayStartTime, SetWorkDayStartTime
	//-----------------------------------------------------------------------
	void SetWorkDayEndTime(const COleDateTime& dtTime);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets an end time for the working day.
	// Parameters:
	//     nHour - An int. The hour part of the new time value.
	//     nMin  - An int. The minutes part of the new time value.
	//     nSec  - An int. The seconds part of the new time value.
	// See Also: GetWorkDayEndTime, GetWorkDayStartTime, SetWorkDayStartTime
	//-----------------------------------------------------------------------
	void SetWorkDayEndTime(int nHour, int nMin, int nSec);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds custom time interval for an event to choose in the event properties dialog
	// Parameters:
	//     nMin                  - An int. Custom reminder interval in minutes
	//     szCustomMeaning       - A string that describes this interval (e.g. hour, day). Default string is CXTPCalendarUtils::FormatTimeDuration(nMin)
	//     bIgnoreDefaultValues  - Tells the event properties dialog to ignore default reminder time intervals (1,5,10,15 minutes)
	//-----------------------------------------------------------------------
	void AddCustomTimeIntervalForReminder(int nMin, LPCTSTR szCustomMeaning = NULL, BOOL bIgnoreDefaultValues = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the AutoResetBusyFlag is set.
	// Returns:
	//     Value of AutoResetBusyFlag.
	// See Also: MonthView_SetCompressWeekendDays
	//-----------------------------------------------------------------------
	BOOL DayView_IsAutoResetBusyFlag() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the AutoResetBusyFlag.
	// Parameters:
	//     bAutoResetBusyFlag - A BOOL.
	//                          TRUE to set the AutoResetBusyFlag.
	//                          FALSE otherwise.
	// See Also: MonthView_SetCompressWeekendDays
	//-----------------------------------------------------------------------
	void DayView_SetAutoResetBusyFlag(BOOL bAutoResetBusyFlag = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine whether weekend days are
	//     drawn compressed in month view or not.
	// Returns:
	//     TRUE if weekend days are drawn compressed. FALSE otherwise.
	// See Also: MonthView_SetCompressWeekendDays
	//-----------------------------------------------------------------------
	BOOL MonthView_IsCompressWeekendDays() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to specify whether weekend days are
	//     drawn compressed in the month view.
	// Parameters:
	//     bCompress - A BOOL. TRUE if weekend days are drawn compressed.
	//                 FALSE otherwise.
	// See Also: MonthView_IsCompressWeekendDays
	//-----------------------------------------------------------------------
	void MonthView_SetCompressWeekendDays(BOOL bCompress = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine whether the end date is
	//     shown for events in the month view.
	// Returns:
	//     TRUE if the end date is shown. FALSE otherwise.
	// See Also: MonthView_SetShowEndDate
	//-----------------------------------------------------------------------
	BOOL MonthView_IsShowEndDate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to specify whether the end date is
	//     shown in the month view.
	// Parameters:
	//     bShowEnd - A BOOL. TRUE if the end date is shown in month view.
	//                FALSE otherwise.
	// See Also: MonthView_IsShowEndDate
	//-----------------------------------------------------------------------
	void MonthView_SetShowEndDate(BOOL bShowEnd = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the time is
	//     shown as a graphical clock for events in the month view.
	// Returns:
	//     TRUE for time shown as graphical clock. FALSE otherwise.
	// See Also: MonthView_SetShowTimeAsClocks
	//-----------------------------------------------------------------------
	BOOL MonthView_IsShowTimeAsClocks() const;

	// --------------------------------------------------------------------------
	// Summary:
	//     Call this method to specify if the events start and end time is
	//     shown as a graphical clock in the month view.
	// Parameters:
	//     bShowClocks :  A BOOL. TRUE if the time is shown as a graphical clock.
	//                    FALSE otherwise.
	// See Also:
	//     MonthView_IsShowTimeAsClocks
	// --------------------------------------------------------------------------
	void MonthView_SetShowTimeAsClocks(BOOL bShowClocks= TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the end date is
	//     shown for events in the week view.
	// Returns:
	//     TRUE if the end date is shown. FALSE otherwise.
	// See Also: WeekView_SetShowEndDate
	//-----------------------------------------------------------------------
	BOOL WeekView_IsShowEndDate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to specify if the end date is
	//     shown in week view.
	// Parameters:
	//     bShowEnd - A BOOL. TRUE if the end date is shown in week view.
	//                FALSE otherwise.
	// See Also: WeekView_IsShowEndDate
	//-----------------------------------------------------------------------
	void WeekView_SetShowEndDate(BOOL bShowEnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the time is
	//     shown as a graphical clock for events in the week view.
	// Returns:
	//     TRUE for time shown as graphical clocks, FALSE otherwise.
	// See Also: WeekView_SetShowTimeAsClocks
	//-----------------------------------------------------------------------
	BOOL WeekView_IsShowTimeAsClocks() const;

	// -------------------------------------------------------------------------------
	// Summary:
	//     Call this method to specify if the events start and end time is
	//     shown as a graphical clock in week view.
	// Parameters:
	//     bShowClocks :  A BOOL. TRUE for time to be shown as graphical clocks, FALSE
	//                    otherwise.
	// See Also:
	//     WeekView_IsShowTimeAsClocks
	// -------------------------------------------------------------------------------
	void WeekView_SetShowTimeAsClocks(BOOL bShowClocks);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the new control drawing PaintManager.
	// Parameters:
	//     pPaintManager - A CXTPCalendarPaintManager pointer to the
	//     new paint manager object.
	// Remarks:
	//     Call this member function to set the paint manager object used
	//     for drawing a calendar control window.
	// See Also: GetPaintManager
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPCalendarPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function updates the mouse cursor in relation with
	//     the control's state.
	// Returns:
	//     A BOOL. TRUE if the mouse cursor was updated to one of the known modes,
	//     FALSE if the mode is unknown and the cursor was set to default arrow.
	//-----------------------------------------------------------------------
	virtual BOOL UpdateMouseCursor();

	//---- Time Management functions ----------------------------------------

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the provided date is "today"
	//     The today date may be overriden with SetTodayDateOverride() call or with custom time provider
	// Returns:
	//     A BOOL. TRUE if the provided date is "today" for this calendar control instance, FALSE otherwise
	//-----------------------------------------------------------------------
	virtual BOOL IsToday(const COleDateTime& dtDate) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine the "today" date
	//     The today date may be overriden with SetTodayDateOverride() call or with custom time provider
	// Returns:
	//     COleDateTime with "today" date for this calendar control instance
	//-----------------------------------------------------------------------
	COleDateTime GetTodayDate() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to override the "today" date. Calendar control will assume the date provided as "today"
	//     The today date is used to draw highlight border for day views (mark them as today)
	// Parameters:
  //     COleDateTime object with date which you want to highlight in calendar as "today"
	//-----------------------------------------------------------------------
	void SetTodayDateOverride(const COleDateTime& dtTodayOverride);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns override for "today" date is it was set with SetTodayDateOverride() call.
	//     If the GetState() member function of COleDateTime returns invalid flag for returned date, then no override was applied
	// Returns:
	//     COleDateTime object with override today date
	//-----------------------------------------------------------------------
	COleDateTime GetTodayDateOverride() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function resets override for "today" date to the current date. If no override is applied the calls to GetTodayDate()
	//     and IsToday() will use the CXTPCalendarUtils::GetCurrentTime() to determine the current time on user machine
	//-----------------------------------------------------------------------
	void ResetTodayDateOverride();

public:
	//{{AFX_CODEJOCK_PRIVATE
	// for compatibility with previous versions
	// use xtp_wm_UserAction for the feature
	//
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function queues the calendar view while changing
	//     to the day view for the specified day.
	// Parameters:
	//     dtDate - A specified date to change to.
	// Remarks:
	//     Call this member function for changing calendar view
	//     to the day view for the specified day.
	// See Also: CXTPCalendarDayView overview, XTPCalendarViewType,
	//          GetAvtiveView, SetAvtiveView
	//-----------------------------------------------------------------------
	virtual void QueueDayViewSwitch(DATE dtDate);
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Register the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of resource where control is located
	// Returns:
	//     TRUE if the window class was successfully registered.  FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);
protected:


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function performs all of the drawing logic of the
	//     calendar control window.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// See Also: OnPaint
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the custom calendar view for the control.
	// Parameters:
	//     pView - Pointer to the custom calendar view object.
	// Remarks:
	//     Call this member function to set the custom calendar view
	//     currently used by the calendar control. Note that
	//     custom calendar view must be descendant of CXTPCalendarView.
	// See Also: CXTPCalendarView overview, GetAvtiveView
	//-----------------------------------------------------------------------
	virtual void SetActiveView(CXTPCalendarView* pView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates a timer event.
	// Parameters:
	//     uTimeOut_ms - Duration of timer in milliseconds.
	// Returns:
	//     The identifier of the timer.
	//-----------------------------------------------------------------------
	virtual UINT SetTimer(UINT uTimeOut_ms);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the framework before the
	//     view is destroyed.
	//-----------------------------------------------------------------------
	virtual void OnBeforeDestroy();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adjusts the scroll bar to reflect the current
	//     control's state.
	// See Also:
	//     CXTPCalendarControl::CUpdateContext, AdjustLayout, RedrawControl
	// Parameters:
	//     nSBType : Specifies the scroll bar type to be adjusted.
	//               This parameter can be either of the following:
	//               * SB_HORZ - Horizontal scroll bar.
	//               * SB_VERT - Vertical scroll bar.
	// ----------------------------------------------------------------------
	virtual void AdjustScrollBarEx(int nSBType);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function internally sets the custom data provider
	//     for the control.
	// Parameters:
	//     pDataProvider - Pointer to the custom data provider object.
	//     bCloseDataProviderWhenDestroy - BOOL value shows whether data
	//                                     provider should be closed with the
	//                                     control's destruction.
	// Remarks:
	//     This method is used by SetDataProvider().
	// See Also:
	//     SetDataProvider
	//-----------------------------------------------------------------------
	virtual void _SetDataProvider(CXTPCalendarData* pDataProvider,
								  BOOL bCloseDataProviderWhenDestroy = TRUE);


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Obtain a pointer to the notification connection object.
	// Remarks:
	//     Used to subscribe (Advice) for notification events from the
	//     control.
	// Returns:
	//     Connection object pointer.
	// See Also: CXTPNotifyConnection overview,
	//           IXTPNotificationSink overview
	//-----------------------------------------------------------------------
	CXTPNotifyConnection* GetConnection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sends a notification to all control's
	//     event subscribers.
	// Parameters:
	//     EventCode - A specific code identifying the event.
	//     wParam - First custom parameter. Depends on the event type.
	//               See specific event description for details.
	//     lParam - Second custom parameter. Depends on the event type.
	//               See specific event description for details.
	// Remarks:
	//     This member function is called internally from inside the
	//     control when a notification is sent to all notification
	//     listeners.
	//     SendNotification depends on EnableSendNotifications flag.
	//     SendNotificationAlways - Performs send in any case.
	// See Also: XTP_NOTIFY_CODE, GetConnection
	//-----------------------------------------------------------------------
	virtual void SendNotification(XTP_NOTIFY_CODE EventCode, WPARAM wParam , LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function always sends a notification to all control's
	//     event subscribers.
	// Parameters:
	//     EventCode - A specific code identifying the event.
	//     wParam - First custom parameter. Depends on the event type.
	//               See specific event description for details.
	//     lParam - Second custom parameter. Depends on the event type.
	//               See specific event description for details.
	// Remarks:
	//     This member function is called internally from inside the
	//     control when a notification is sent to all notification
	//     listeners.
	//     SendNotification depends on EnableSendNotifications flag.
	//     SendNotificationAlways - Performs send in any case.
	// See Also: XTP_NOTIFY_CODE, GetConnection
	//-----------------------------------------------------------------------
	virtual void SendNotificationAlways(XTP_NOTIFY_CODE EventCode, WPARAM wParam , LPARAM lParam); // <COMBINE SendNotification>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function enables or disables sending a notification
	//     to all control's event subscribers.
	// Parameters:
	//     bEnable - TRUE to allow sending notifications,
	//               FALSE to disallow.
	// See Also:
	//     SendNotification
	//-----------------------------------------------------------------------
	virtual void EnableSendNotifications(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Obtain an associated Reminders Manager object.
	// Remarks:
	//     Use this method to retrieve a pointer to the Reminders Manager
	//     object associated to this Calendar control..
	// Returns:
	//     Calendar Reminders Manager.
	// See Also:
	//     CXTPCalendarRemindersManager overview
	//-----------------------------------------------------------------------
	virtual CXTPCalendarRemindersManager* GetRemindersManager() const;


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process the command with
	//     specified ID.
	// Parameters:
	//     uCommandID - Command ID.  Specifies which command to process.
	// See Also: OnUndo, OnCut, OnCopy, OnPaste
	//-----------------------------------------------------------------------
	virtual void OnEditCommand(UINT uCommandID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process undo commands.
	// See Also: OnCommand, OnCut, OnCopy, OnPaste
	//-----------------------------------------------------------------------
	afx_msg virtual void OnUndo();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process the "Cut" command.
	// See Also: OnCommand, OnUndo, OnCopy, OnPaste
	//-----------------------------------------------------------------------
	afx_msg virtual void OnCut();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process the "Copy" command.
	// See Also: OnCommand, OnUndo, OnCut, OnPaste
	//-----------------------------------------------------------------------
	afx_msg virtual void OnCopy();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to process the "Paste" command.
	// See Also: OnCommand, OnUndo, OnCut, OnCopy
	//-----------------------------------------------------------------------
	afx_msg virtual void OnPaste();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to update the user interface
	//     object represented by pCmdUI.
	// Parameters:
	//     pCmdUI - pointer to the CCmdUI object.
	// See Also: OnUpdateUndo, OnUpdateCut, OnUpdateCopy, OnUpdatePaste
	//-----------------------------------------------------------------------
	afx_msg virtual void OnUpdateCmdUI(CCmdUI* pCmdUI);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to update the user interface
	//     object represented by pCmdUI for the "Undo" command only.
	// Parameters:
	//     pCmdUI - pointer to the CCmdUI object.
	// See Also: OnUpdateUndo, OnUpdateCut, OnUpdateCopy, OnUpdatePaste
	//-----------------------------------------------------------------------
	afx_msg virtual void OnUpdateUndo(CCmdUI* pCmdUI);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to update the user interface object
	//     represented by pCmdUI for the "Cut" command only.
	// Parameters:
	//     pCmdUI - pointer to the CCmdUI object.
	// See Also: OnUpdateUndo, OnUpdateUndo, OnUpdateCopy, OnUpdatePaste
	//-----------------------------------------------------------------------
	afx_msg virtual void OnUpdateCut(CCmdUI* pCmdUI);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to update the user interface object
	//     represented by pCmdUI for the "Copy" command only.
	// Parameters:
	//     pCmdUI - pointer to the CCmdUI object.
	// See Also: OnUpdateUndo, OnUpdateUndo, OnUpdateCut, OnUpdatePaste
	//-----------------------------------------------------------------------
	afx_msg virtual void OnUpdateCopy(CCmdUI* pCmdUI);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to update the user interface
	//     object represented by pCmdUI for the "Paste" command only.
	// Parameters:
	//     pCmdUI - pointer to the CCmdUI object.
	// See Also: OnUpdateUndo, OnUpdateUndo, OnUpdateCut, OnUpdateCopy
	//-----------------------------------------------------------------------
	afx_msg virtual void OnUpdatePaste(CCmdUI* pCmdUI);

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is called by WindowProc, or is called during
	//      message reflection.
	// Parameters:
	//      message - Specifies the message to be sent.
	//      wParam - Specifies additional message-dependent information.
	//      lParam - Specifies additional message-dependent information.
	//      pResult - The return value of WindowProc. Depends on the message; may be NULL.
	// Returns:
	//      TRUE if the message is handled properly, FALSE else.
	//-----------------------------------------------------------------------
	afx_msg virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to catch messages before child
	//     windows (like Subject editor).
	// Remarks:
	//     The child windows must call OnWndMsg_Children() for the messages
	//     which are interesting for other calendar control parts (main control
	//     window, view, day view, event view). The OnWndMsg() method may be
	//     overridden for this purposes. This mechanism is used instead of
	//     PreTranslateMessage() for ActiveX builds.
	// Parameters:
	//      message - Specifies the message to be sent.
	//      wParam  - Specifies additional message-dependent information.
	//      lParam  - Specifies additional message-dependent information.
	//      pResult - The return value of WindowProc. Depends on the message;
	//                may be NULL.
	// Returns:
	//      TRUE if message was handled; otherwise FALSE.
	// See Also: CXTPCalendarViewEventSubjectEditor, OnWndMsg().
	//-----------------------------------------------------------------------
	virtual BOOL OnWndMsg_Children(UINT message, WPARAM wParam, LPARAM lParam,
									LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function deletes specified event from the calendar.
	// Parameters:
	//      pViewEvent  - View of the event to delete.
	// Remarks:
	//     Call member function if you want to delete an event basing
	//     on its View object.
	// Returns:
	//      TRUE if event was deleted successfully; otherwise FALSE.
	// See Also:
	//-----------------------------------------------------------------------
	virtual BOOL DoDeleteEvent(CXTPCalendarViewEvent* pViewEvent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function deletes selected events from the calendar.
	// Parameters:
	//      pViewEvent  - View of the last event to delete.
	// Remarks:
	//     Call member function if you want to delete all selected events.
	// Returns:
	//      TRUE if events were deleted successfully; otherwise FALSE.
	// See Also:
	//-----------------------------------------------------------------------
	virtual BOOL DoDeleteSelectedEvents(CXTPCalendarViewEvent* pViewEvent);

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

	// -----------------------------
	// Summary:
	//     This member function is making attempt to select event by passed event ID
	// Parameters:
	//      dEventID :  DWORD of event id
	//      bSelect : BOOL - EnsureVisible && Select if bSelect = TRUE, EnsureVisible only if bSelect = FALSE
	// Returns:
	//     TRUE is Success
	// -----------------------------
	BOOL SelectEventById(DWORD dEventID, BOOL bSelect = TRUE);

	// -----------------------------
	// Summary:
	//     This member function is making attempt to select view event by passed Schedule ID and event ID
	// Parameters:
	//      nScheduleID: Schedule ID - used in case with multiple links
	//      dEventID :  DWORD of event id
	//      bSelect : BOOL - EnsureVisible && Select if bSelect = TRUE, EnsureVisible only if bSelect = FALSE
	// Returns:
	//     TRUE is Success
	// -----------------------------
	BOOL SelectViewEventByIdAndGroup(int nScheduleID, DWORD dEventID, BOOL bSelect = TRUE);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//## Generated message map functions
	//{{AFX_MSG(CXTPCalendarControl)
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseLeave();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT_PTR uTimerID);
	afx_msg void OnDestroy();
	afx_msg UINT OnGetDlgCode();

	afx_msg void OnSysColorChange();
	afx_msg void OnTimeChange();
	afx_msg LRESULT OnTimeZoneChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnSwitchView(WPARAM nDate, LPARAM); // for compatibility with previous versions
	afx_msg LRESULT OnUserAction(WPARAM dwParam1, LPARAM dwParam2);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);

	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE
	CXTPNotifySink* m_pSink;

	virtual void OnEvent_FromDataProvider(XTP_NOTIFY_CODE Event, WPARAM wParam , LPARAM lParam);
	virtual void OnEvent_Reminders(XTP_NOTIFY_CODE Event, WPARAM wParam , LPARAM lParam);

	virtual void OnFinalRelease();
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	virtual void PreSubclassWindow();

	virtual void _InitCalendarView();
//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is called by the calendar control when calendar
	//      option was changed.
	// Remarks:
	//      It is called from the set option methods like SetWorkWeekMask(),
	//      SetFirstDayOfWeek(), SetWorkDayStartTime(), SetWorkDayEndTime(),
	//      DayView_SetAutoResetBusyFlag(), ... etc.
	//      See CXTPCalendarOptions for details.
	//      Default implementation sends XTP_NC_CALENDAROPTIONSWASCHANGED
	//      notification.
	// Parameters:
	//      nOptionRelation - Changed option relation (or kind).
	//                        Value -1 specify global or common option.
	//                        Value from XTPCalendarViewType enum specify
	//                        which kind of view changed option is related.
	// Returns:
	//      TRUE if events were deleted successfully; otherwise FALSE.
	// See Also:
	//      CXTPCalendarOptions,  CXTPCalendarData::GetCalendarOptions(),
	//      XTP_NC_CALENDAROPTIONSWASCHANGED,
	//-----------------------------------------------------------------------
	virtual void OnOptionsChanged(int nOptionRelation);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual void AdviseToDataProvider();
	virtual void _AdviseToReminders_StartMonitoring();
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      This member function is used to show a ToolTip rectangle.
	// Parameters:
	//      strText     - A text to be shown inside a tooltip rectangle.
	//      rcToolTip   - A rect for a tooltip window.
	//      pFont       - Font for to draw tooltip text.
	//      bAdvanced   - TRUE for advanced processing (Office2007 theme "add new appt")
	//-----------------------------------------------------------------------
	virtual void ShowToolTip(const CString& strText, const CRect rcToolTip,
		CFont* pFont, BOOL bAdvanced = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to hide the ToolTip rectangle.
	// See Also:
	//     IsToolTipVisible
	//-----------------------------------------------------------------------
	virtual void HideToolTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is the ToolTip rectangle visible on the screen.
	// Returns:
	//     TRUE if ToolTip rectangle visible, FALSE otherwise.
	// See Also:
	//     HideToolTip
	//-----------------------------------------------------------------------
	virtual BOOL IsToolTipVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns TRUE if the ToolTips enabled,
	//     FALSE otherwise.
	// Returns:
	//     TRUE if the ToolTips enabled, FALSE otherwise.
	// See Also:
	//     EnableToolTips
	//-----------------------------------------------------------------------
	BOOL IsToolTipsEnabled() const;

	// ---------------------------------------------------------------
	// Summary:
	//     This member function is used to enable or disable ToolTips
	//     for the control.
	// See Also:
	//     IsToolTipsEnabled
	// Parameters:
	//     bEnable : A BOOL. Specifies whether ToolTips should be
	//               enabled (TRUE) or disabled (FALSE).
	// ---------------------------------------------------------------
	void EnableToolTips(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns TRUE if the Reminders functionality
	//     is enabled for this Calendar control and FALSE otherwise.
	// Returns:
	//     TRUE if reminders are enabled, FALSE otherwise.
	// See Also:
	//     EnableReminders()
	//-----------------------------------------------------------------------
	BOOL IsRemindersEnabled() const;

	// ---------------------------------------------------------------
	// Summary:
	//     This member function is used to enable or disable Reminders
	//     functionality for the control.
	// Parameters:
	//     bEnable : A BOOL. Specifies whether Reminders functionality
	//               should be enabled (TRUE) or disabled (FALSE).
	// See Also:
	//     IsRemindersEnabled
	// ---------------------------------------------------------------
	void EnableReminders(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns set of flags which define calendar items
	//     to send XTP_NC_CALENDAR_GETITEMTEXT notification.
	//     If Calendar theme is set - return value is got from the theme object,
	//     otherwise it is got from the paint manager object.
	// Returns:
	//     Set of flags from enums XTPCalendarGetItemText, XTPCalendarGetItemTextEx.
	// See Also:
	//     XTPCalendarGetItemText, XTPCalendarGetItemTextEx,
	//     CXTPCalendarControlPaintManager::GetAskItemTextFlags,
	//     CXTPCalendarControlPaintManager::SetAskItemTextFlags,
	//     CXTPCalendarTheme::GetAskItemTextFlags,
	//     CXTPCalendarTheme::SetAskItemTextFlags, XTP_NC_CALENDAR_GETITEMTEXT
	//-----------------------------------------------------------------------
	virtual DWORD GetAskItemTextFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns TRUE if Markup functionality
	//     is enabled for this Calendar control and FALSE otherwise.
	//     In enabled state is automatically process static Markup text for
	//     events, and enables processing of dynamic Markup text customization
	//     using XTP_NC_CALENDAR_GETITEMTEXT notification.
	// Returns:
	//     TRUE if Markup feature is enabled, FALSE otherwise.
	// See Also:
	//     CXTPCalendarViewEvent::GetMarkupText()
	//-----------------------------------------------------------------------
	BOOL IsMarkupEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to enable or disable Markup
	//     functionality for the control. Enabling this functionality allows
	//     a control to process customized special property xtpMarkupTemplate
	//     which could be set for each event. If this property exists and
	//     contains text with Markup information, it will be drawn inside the
	//     Event rectangle (instead of Subject and Body).
	// Parameters:
	//     bEnableMarkup :  Specifies whether Markup functionality
	//                      should be enabled (TRUE) or disabled (FALSE).
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns markup context
	// Returns:
	// A pointer to the markup context object.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	// This member function Refresh Caption Bar after some layout modifications
	//-----------------------------------------------------------------------
	void RefreshCaptionBar();

private:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_CODEJOCK_PRIVATE

public:
	//=======================================================================
	// Summary:
	//     Class CUpdateContext updates context and redraws control parts
	//     depending on input parameters.
	// See Also: XTPCalendarUpdateOptions,
	//          AdjustLayout, AdjustScrollBar, RedrawControl
	//=======================================================================
	class _XTP_EXT_CLASS CUpdateContext
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//     Constructs a CUpdateContext object.
		// Parameters:
		//     pControl - Pointer to the CXTPCalendarControl to adjust.
		//     nOptions - A bitwise combination of the adjusting
		//                 parameters.
		// Remarks:
		//     When constructing a CUpdateContext object, specify
		//     if the control's layout needs adjustment and/or
		//     redrawing.
		// See Also: XTPCalendarUpdateOptions
		//-------------------------------------------------------------------
		CUpdateContext(CXTPCalendarControl* pControl, int nOptions = xtpCalendarUpdateLayout | xtpCalendarUpdateRedraw) :
			m_pControl(pControl)
		{
			// WARNING. (no error)
			// There is no need to use xtpCalendarUpdateRedraw and xtpCalendarUpdateRedrawIfNeed together
			ASSERT(!((nOptions & xtpCalendarUpdateRedraw) && (nOptions & xtpCalendarUpdateRedrawIfNeed)));

			m_nOptions = nOptions;

			if (!m_pControl)
			{
				ASSERT(FALSE);
				return;
			}

			if (m_nOptions & (xtpCalendarUpdateLayout | xtpCalendarUpdateScrollBar))
				m_pControl->BeginAdjust(nOptions & xtpCalendarUpdateScrollBar);

			if (m_nOptions & xtpCalendarUpdateRedraw)
				m_pControl->BeginUpdate();

			if (m_nOptions & xtpCalendarUpdateRedrawIfNeed)
				m_pControl->LockUpdate();
		}

		//-------------------------------------------------------------------
		// Summary:
		//     Destroys a CUpdateContext object and calls all the required
		//     Calendar Control adjusting and redrawing functions.
		// See Also: XTPCalendarUpdateOptions
		//-------------------------------------------------------------------
		~CUpdateContext()
		{
			if (!m_pControl)
			{
				return;
			}

			if (m_nOptions & (xtpCalendarUpdateLayout | xtpCalendarUpdateScrollBar))
				m_pControl->EndAdjust(m_nOptions & xtpCalendarUpdateScrollBar);

			if (m_nOptions & xtpCalendarUpdateRedraw)
				m_pControl->EndUpdate();

			if (m_nOptions & xtpCalendarUpdateRedrawIfNeed)
				m_pControl->UnlockUpdate();
		}

	private:
		CXTPCalendarControl* m_pControl;
		int m_nOptions;
	};

	//=======================================================================
	// Summary:
	//      Class CViewChangedContext used to avoid multiple sending of
	//      XTP_NC_CALENDARVIEWWASCHANGED notification for one action.
	//=======================================================================
	class CViewChangedContext
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//     Constructs a CViewChangedContext object.
		// Parameters:
		//      pView    - Pointer to parent CXTPCalendarView object.
		//      pControl - Pointer to parent CXTPCalendarControl object.
		//      eType    - Value from enum XTPCalendarViewChangedUpdateOptions.
		// See Also: XTP_NC_CALENDARVIEWWASCHANGED
		//-------------------------------------------------------------------
		CViewChangedContext(CXTPCalendarView* pView, int eType = xtpCalendarViewChangedSend);
		CViewChangedContext(CXTPCalendarControl* pControl, int eType = xtpCalendarViewChangedSend); //<COMBINE CXTPCalendarControl::CViewChangedContext::CViewChangedContext@CXTPCalendarView*@int>

		//-------------------------------------------------------------------
		// Summary:
		//      Destroys a CViewChangedContext object and send notification
		//      if this is last locker object.
		//-------------------------------------------------------------------
		virtual ~CViewChangedContext();

	private:
		void Init(CXTPCalendarControl* pControl, int eType);

		CXTPCalendarControl* m_pControl;
		int m_eType;
	};

	//{{AFX_CODEJOCK_PRIVATE
	friend class CUpdateContext;
	friend class CViewChangedContext;
	//}}AFX_CODEJOCK_PRIVATE

protected:

	//=======================================================================
	// Summary:
	//      Class CViewChanged_ContextData is used as locker context for
	//      CViewChangedContext.
	//=======================================================================
	class CViewChanged_ContextData
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//      Default object constructor.
		//-------------------------------------------------------------------
		CViewChanged_ContextData();

		int m_nLockCount;     // View change notification lock counter.
		BOOL m_bRequest;      // View change send notification request.
	};

	CViewChanged_ContextData    m_cntViewChanged;   // Locker context for CViewChangedContext.

	int m_nRowsPerWheel;                            // Amount of rows to scroll when using the mouse wheel.

	CString m_strUndoUIText;                        // Text label for the Undo UI command.
	CString m_strRedoUIText;                        // Text label for the Redo UI command.
	BOOL m_bUpdateWhenEventChangedNotify;           // Whether to update control when notification comes about an event change.
	CXTPCalendarRemindersManager*   m_pRemindersManager; // Stores reminders manager object.
	CXTPMarkupContext* m_pMarkupContext;            //The markup context.

private:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL m_bChanged;

	int m_nLockUpdateCount;

	int m_nLockAdjustCount;
	BOOL m_bAdjustScrollBar;

	CXTPCalendarView*       m_pActiveView;

	CXTPCalendarDayView*    m_pDayView;
	CXTPCalendarMonthView*  m_pMonthView;
	CXTPCalendarWeekView*   m_pWeekView;
	CXTPCalendarTimeLineView*   m_pTimeLineView;

	CXTPCalendarResourcesNf*    m_pResourcesNf; // Resources array

	CXTPCalendarTheme*              m_pTheme;
	CXTPCalendarPaintManager*       m_pPaintManager;
	CXTPNotifyConnection*           m_pConnection;

	CXTPCalendarOptions* m_pOptions;    // This member stores user's calendar view options.

	CBitmap m_bmpCache; // Current view cached picture

	CXTPCalendarTip* m_pWndTip;

	UINT m_uNextTimerID;

	BOOL m_bEnableToolTips;
	BOOL m_bEnableSendNotifications;
	UINT m_uPopulateRequest_TimerID;
	UINT m_uRedrawRequest_TimerID;

	XTP_CONNECTION_ID m_cnidOnReminders;
	COleDateTimeSpan m_spRemindersUpdatePeriod;

	LCID m_lcidActiveLocale;
	int m_nFirstWeekOfYearDays;         // This member shows the first Week Of Year Days.

	COleDateTime m_dtTodayOverride;

	//}}AFX_CODEJOCK_PRIVATE

public:

	//-------------------------------------------------------------------
	// Summary:
	//     This member function is used to get state of Read-only mode
	// Returns:
	//     TRUE if current mode is Read-only.
	//-------------------------------------------------------------------
	BOOL IsReadonlyMode() const;

	XTPCalendarMouseMode m_mouseMode;          // Current mouse operation mode.
	BOOL m_bDeleteOnFinalRelease;   // If TRUE - Delete self OnFinalRelease() call.
	BOOL m_bDisableRedraw;  // If TRUE - control window is not redraw, it is draw last window content (draw cached bitmap). FALSE by default.
	BOOL m_bTimelineMode; // flag to separate timeline (if TRUE) and other calendar modes
	XTPCalendarViewType m_eViewType; // current view type
	int m_nTimelineScale; // current timeline scale type
	int m_nCaptionHeight; // height param - can be used for custom layout adjustment
	BOOL m_bPreventAutoAllDayMode; // Flag to prevent dragging event into AllDay zone
	BOOL m_bMultiColorScheduleMode; // Flag to show Schedules titles in different colors
	BOOL m_bMultiColumnWeekMode; // Flag to select Outlook2007 and Outlook2003 behavior
	BOOL m_bHideCaptionBar; // Flag to Hide Caption Bar when TimeLine mode used
	BOOL m_bReadOnlyMode; // View- and Print-only mode to use Calendar
	BOOL m_bFullDateMode; // Useful for Multi-schedules case flag to use DayHeader special format
	BOOL m_bTooltipUnderMouse; // 2-way tooltip positioning - TRUE - under mouse cursor, FALSE - on the top of mouse cursor

	BOOL m_bShowLinks; //flag to show or hide 'virtual' view events
	BOOL m_bHideLinkContainer; //flag to hide or show 'virtual' view events container column
	BOOL m_bPromptToDeleteRecurrentEvent; //flag to use Ocurrence or Master case selector (default = TRUE)
	BOOL m_bDeleteOcurrenceEventIfNoPrompt; //True to delete Ocurrence event, False - delete Master (default = TRUE)

	CString m_OwnerUser;  // string as current user name place holder
	CString m_sCustomFormat4Tooltip; // Free-styled tooltip tag-based way to fill tooltip string used tags &lt;BODY&gt;,&lt;LOC&gt;,&lt;SUBJ&gt;,&lt;TIME&gt; in any combination with fillers
	CString m_sCustomTitle; //used for unique title - e.g. for PrintJob name
	BOOL m_bForcePagination; //flag to Force Pagination during PrintPreview
	BOOL m_bShowTooltipForNotFocused; //flag to Show Tooltip For Not Focused Control
	BOOL m_bShowTooltipForAllDayEvents; //flag to Show Tooltip For All Day Events
	BOOL m_bSwitchToDayViewIfPickedSingleDay; //flag to switch to DatView on DatePicker one day selection (if TRUE) or keep previous view (FALSE)
	BOOL m_bMultipleSchedulesMode; //flag to use MutilpleSchedulesModel (one event - many view events case)
	BOOL m_bMultipleSchedulesNoSelfLinks; //flag to use MutilpleSchedulesModel (one event - many view events case - allow to eliminate selflinks)

	// custom time intervals for reminder
	CMap<int,int,CString,CString&> m_mapCustomReminderValues;
	BOOL m_bIgnoreDefaultReminderValues;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create a data provider from
	//     a specified Calendar connection string.
	// Parameters:
	//     lpszConnectionString - A text Calendar connection string.
	// Remarks:
	//     This is a helper method which creates calendar's data provider of
	//     the specified type with settings taken from the connection string.
	// Returns:
	//     Pointer to the created Calendar Data Provider object.
	//-----------------------------------------------------------------------
	static CXTPCalendarData* AFX_CDECL CreateDataProvider(LPCTSTR lpszConnectionString);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create a data provider of the
	//     specified type.
	// Parameters:
	//     eDataProvider - A member of XTPCalendarDataProvider enumeration,
	//                     which specifies a type of the Data Provider to create.
	// Remarks:
	//     This is a helper method which creates calendar's data provider of
	//     the specified type.
	// Returns:
	//     Pointer to the created Calendar Data Provider object.
	//-----------------------------------------------------------------------
	static CXTPCalendarData* AFX_CDECL CreateDataProvider(XTPCalendarDataProvider eDataProvider);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve a type of the data provider from
	//     its connection string.
	// Parameters:
	//     lpszConnectionString - A text Calendar connection string.
	//     eDPDefault           - Default data provider type to be returned if
	//                            it can't be determined from the string.
	// Remarks:
	//     This is a helper method which parses a connection string and
	//     identifies a type of the data provider which is specified there.
	// Returns:
	//     One of the values from XTPCalendarDataProvider enumeration.
	// See Also:
	//     DataSourceFromConStr(), CreateDataProvider()
	//-----------------------------------------------------------------------
	static XTPCalendarDataProvider AFX_CDECL DataProviderTypeFromConStr(
		LPCTSTR lpszConnectionString,
		XTPCalendarDataProvider eDPDefault = xtpCalendarDataProviderUnknown);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve a data source connection string from
	//     a regular Calendar connection string.
	// Parameters:
	//     lpszConnectionString - A text Calendar connection string.
	// Remarks:
	//     This is a helper method which parses a connection string and
	//     identifies a data source connection string there.
	// Returns:
	//     Identified data source connection string.
	// See Also:
	//     DataProviderTypeFromConStr(), CreateDataProvider()
	//-----------------------------------------------------------------------
	static CString AFX_CDECL DataSourceFromConStr(LPCTSTR lpszConnectionString);
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPCalendarControl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE CXTPCalendarPaintManager* CXTPCalendarControl::GetPaintManager() const {
	ASSERT(this);
	return this ? m_pPaintManager : NULL;
}
AFX_INLINE CXTPCalendarTheme* CXTPCalendarControl::GetTheme() const {
	ASSERT(this);
	return this ? m_pTheme : NULL;
}
AFX_INLINE CXTPNotifyConnection* CXTPCalendarControl::GetConnection() const {
	ASSERT(this);
	return this ? m_pConnection : NULL;
}
AFX_INLINE CXTPCalendarRemindersManager* CXTPCalendarControl::GetRemindersManager() const {
	ASSERT(this);
	return this ? m_pRemindersManager : NULL;
}
AFX_INLINE BOOL CXTPCalendarControl::IsToolTipsEnabled() const {
	return m_bEnableToolTips;
}
AFX_INLINE void CXTPCalendarControl::EnableToolTips(BOOL bEnable) {
	if (!bEnable && IsToolTipVisible())
	{
		HideToolTip();
	}
	m_bEnableToolTips = bEnable;
}
AFX_INLINE void CXTPCalendarControl::EnableSendNotifications(BOOL bEnable) {
	m_bEnableSendNotifications = bEnable;
}
AFX_INLINE BOOL CXTPCalendarControl::IsMarkupEnabled() const {
	return m_pMarkupContext != NULL;
}
AFX_INLINE CXTPMarkupContext* CXTPCalendarControl::GetMarkupContext() const {
	return m_pMarkupContext;
}
AFX_INLINE CXTPCalendarView* CXTPCalendarControl::GetActiveView() const {
	return m_pActiveView;
}
AFX_INLINE CXTPCalendarDayView* CXTPCalendarControl::GetDayView() const {
	return m_pDayView;
}
AFX_INLINE CXTPCalendarWeekView* CXTPCalendarControl::GetWeekView() const {
	return m_pWeekView;
}
AFX_INLINE CXTPCalendarMonthView* CXTPCalendarControl::GetMonthView() const {
	return m_pMonthView;
}
AFX_INLINE CXTPCalendarTimeLineView* CXTPCalendarControl::GetTimeLineView() const {
	return m_pTimeLineView;
}
AFX_INLINE CXTPCalendarOptions* CXTPCalendarControl::GetCalendarOptions() const {
	return m_pOptions;
}

#endif // !defined(_XTPCALENDARCONTROL_H__)
