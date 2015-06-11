// XTPCalendarEventPropertiesDlg.h: interface for the CXTPCalendarEventPropertiesDlg.
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
#if !defined(_XTP_CALENDAR_EVENT_PROPERTIS_DLG_H__)
#define _XTP_CALENDAR_EVENT_PROPERTIS_DLG_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPCalendarViewEvent;
class CXTPCalendarControl;
class CXTPCalendarEventCategories;
class CXTPCalendarEventCategoryIDs;

//===========================================================================
// Summary:
//      This class implements event category selector combobox.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarEventCategoryComboBox : public CComboBox
{
	//DECLARE_DYNAMIC(CXTPCalendarEventCategoryComboBox)
public:
	// -------------------------------------------------------------------
	// Summary:
	//     Standard object constructor.
	// -------------------------------------------------------------------
	CXTPCalendarEventCategoryComboBox();

	// -------------------------------------------------------------------
	// Summary:
	//     Standard object destructor.
	// -------------------------------------------------------------------
	virtual ~CXTPCalendarEventCategoryComboBox();

	//--------------------------------------------------------------------
	// Summary:
	//     Use this member to add default set of colors.
	// Parameters:
	//     pCategories  - Pointer to categories array.
	//     pChecked     - Pointer to array with checked items IDs.
	// Returns:
	//     true if successful, false otherwise.
	//--------------------------------------------------------------------
	virtual BOOL Init(CXTPCalendarEventCategories* pCategories, CXTPCalendarEventCategoryIDs* pChecked);

	//--------------------------------------------------------------------
	// Summary:
	//     Use this member to get selected categories.
	// Parameters:
	//     pChecked  - [in/out] A pointer to CXTPCalendarEventCategoryIDs
	//                 collection to receive selected category's IDs.
	//     pbChanged - [in/out] This member is set to TRUE if category's
	//                 IDs set changed.
	//--------------------------------------------------------------------
	virtual void GetChecked(CXTPCalendarEventCategoryIDs* pChecked, BOOL* pbChanged);

protected:

	//--------------------------------------------------------------------
	// Summary:
	//     This function implements custom draw combobox items.
	// Parameters:
	//     lpDIS - A pointer to LPDRAWITEMSTRUCT structure.
	//--------------------------------------------------------------------
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

	// -----------------------------------------------------------
	// Summary:
	//     This function implements measure items for custom draw.
	// Parameters:
	//     lpMIS :  A pointer to LPMEASUREITEMSTRUCT structure.
	//
	// -----------------------------------------------------------
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

protected:
	// -------------------------------------------------------------------
	// Summary:
	//      Use this member function to get list box selected item (works
	//      for dropped state only).
	// Returns:
	//      Currently selected item index or LB_ERR.
	// -------------------------------------------------------------------
	virtual int GetLBCurSel() const;

	// -------------------------------------------------------------------
	// Summary:
	//      Use this member function to redraw list box (works for dropped
	//      state only).
	// Parameters:
	//      bUpdateNow - Set as TRUE to call UpdateWindow, FALSE to call Invalidate.
	// -------------------------------------------------------------------
	virtual void RedrawLB(BOOL bUpdateNow = FALSE);

	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OnSelChange();
	afx_msg void OnDropDown();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void _OnSelChange(int nItemIndex);
	//}}AFX_CODEJOCK_PRIVATE

protected:

	CXTPCalendarEventCategories*    m_pCategories;  // Pointer to categories array.
	CXTPCalendarEventCategoryIDs    m_arChecked;    // Array which store checked item IDs.

private:
	UINT m_nLastKey;

	DECLARE_MESSAGE_MAP()
};

//===========================================================================
// Summary:
//      This class implements event label selector combobox.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarEventLabelComboBox : public CComboBox
{
public:

	//--------------------------------------------------------------------
	// Summary:
	//     Standard object constructor.
	//--------------------------------------------------------------------
	CXTPCalendarEventLabelComboBox();

	// -------------------------------------------------------------------
	// Summary:
	//     Standard object destructor.
	// -------------------------------------------------------------------
	virtual ~CXTPCalendarEventLabelComboBox();

	//--------------------------------------------------------------------
	// Summary:
	//     Use this member to add default set of colors.
	// Parameters:
	//     pLabels - Pointer to labels array.
	//--------------------------------------------------------------------
	virtual void Init(CXTPCalendarEventLabels*  pLabels);

	//--------------------------------------------------------------------
	// Summary:
	//     Use this member to get selected label ID.
	// Returns:
	//     A selected label ID.
	//--------------------------------------------------------------------
	virtual int GetLabelID() const;

	//--------------------------------------------------------------------
	// Summary:
	//     Use this member to set selected label ID.
	// Parameters:
	//     nID - A selected label ID.
	//--------------------------------------------------------------------
	virtual void SetLabelID(int nID);

protected:

	//--------------------------------------------------------------------
	// Summary:
	//     This function implements custom draw combobox items.
	// Parameters:
	//     lpDIS - A pointer to LPDRAWITEMSTRUCT structure.
	//--------------------------------------------------------------------
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

	// -----------------------------------------------------------
	// Summary:
	//     This function implements measure items for custom draw.
	// Parameters:
	//     lpMIS :  A pointer to LPMEASUREITEMSTRUCT structure.
	//
	// -----------------------------------------------------------
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

protected:
	CXTPCalendarEventLabels*    m_pLabels;  // Pointer to labels array.
};

//===========================================================================
// Summary:
//      This class used to edit calendar event properties. This is a default
//      implementation which can be used as an example or as a base class
//      for custom dialogs.
//      It designed to be localized - XTPResourceManager is used to load
//      resources.
// See Also:
//      CXTPCalendarEventRecurrenceDlg
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarEventPropertiesDlg : public CDialog
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarEventPropertiesDlg)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//{{AFX_CODEJOCK_PRIVATE
	enum { IDD = XTP_IDD_CALENDAR_EVENT_PROPERTIES };
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Dialog class constructor.
	// Parameters:
	//     pControl      - [in] Pointer to control.
	//     bAddEvent     - [in] If TRUE DataProvider->AddEvent() will be called
	//                          on OK button pressed, otherwise ChangeEvent
	//                          will be called.
	//     pParent       - [in] Pointer to parent window. Can be NULL.
	//
	//     pEditingEvent - [in, out] Pointer to editing event object. If NULL
	//                               a new event will be created.
	//     nIDTemplate   - [in] Contains the ID number of a dialog-box template resource.
	// Remarks:
	//     If pEditingEvent is NULL the new event will be created and StartTime,
	//     EndTime, AllDayEvent, ScheduleID properties will be initialized using
	//     current view selection.
	//     If OK button pressed event will be added or changed and control updated
	//     automatically.
	// See Also:
	//     ~CXTPCalendarEventRecurrenceDlg()
	//-----------------------------------------------------------------------
	//CXTPCalendarEventPropertiesDlg(CXTPCalendarControl* pControl, BOOL bAddEvent,
	//              CWnd* pParent = NULL, CXTPCalendarEvent* pEditingEvent = NULL,
	//              UINT nIDTemplate = CXTPCalendarEventPropertiesDlg::IDD);

	// ------------------------------------------------------------------
	// Summary:
	//     Constructor with extra COleDateTime parameter - initial Dialog
	//     setting - user can change it if needed<p />
	//
	// Parameters:
	//     pControl :       pointer to CXTPCalendarControl
	//     bAddEvent :      BOOL flag to add event (if TRUE) or modify existed (if FALSE)
	//     pParent :        parent CWnd*
	//     pEditingEvent :  pointer to paseed  event
	//     nIDTemplate :    dialog resource template id
	//     dtInitial :      initial datetime passed
	//
	// ------------------------------------------------------------------
	CXTPCalendarEventPropertiesDlg(CXTPCalendarControl* pControl,
									BOOL bAddEvent,
									CWnd* pParent = NULL,
									CXTPCalendarEvent* pEditingEvent = NULL,
									UINT nIDTemplate = CXTPCalendarEventPropertiesDlg::IDD,
									COleDateTime dtInitial = COleDateTime::GetCurrentTime());
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarEventPropertiesDlg(){};

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Update dialog controls values using editing event properties.
	//-----------------------------------------------------------------------
	virtual void UpdateControlsFromEvent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Update dialog controls state (enabled and visible) using editing
	//     event properties.
	//-----------------------------------------------------------------------
	virtual void UpdateControlsState();

	//-----------------------------------------------------------------------
	// Summary:
	//     Initialize Reminder combobox.
	// Remarks:
	//      Time duration formatted using active resource file.
	//-----------------------------------------------------------------------
	virtual void InitReminderComboBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Initialize Busy Status combobox.
	//-----------------------------------------------------------------------
	virtual void InitBusyStatusComboBox();

	// Generated message map functions
	//{{AFX_CODEJOCK_PRIVATE
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheckReminder();
	afx_msg void OnBnClickedButtonRecurrecnce();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedCheckAlldayEvent();
	afx_msg void OnScheduleIdChanged();

	virtual CString LoadString(UINT nIDResource) const;
	virtual void AddEndDate_ResetTime(COleDateTime dtStartDate, COleDateTime& rdtEndDate, int nDays);

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

public:
	DWORD GetLastEventID();
	BOOL m_bOccurrence;     // If TRUE the not recurring event or recurrence exception (ocurrence) is editing, otherwise master event is editing.
	BOOL m_bRecurrenceUpdate; // falg to force update event if recurrence dialog used

protected:
	void OnStartDateChanged(NMHDR *pNMHDR, LRESULT *pResult);
	void OnStartTimeChanged(NMHDR *pNMHDR, LRESULT *pResult);
	void OnEndDateChanged(NMHDR *pNMHDR, LRESULT *pResult);
	void OnEndTimeChanged(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	CXTPCalendarEventPtr m_ptrEditingEvent;     // Pointer to editing event object.
	CXTPCalendarControl* m_ptrControl;          // Pointer to Calendar control.
	CWnd* m_pOwnerWnd;                          // Pointer to Owner Wnd.
	BOOL m_bAddEvent;                           // Store edit mode: Add or Change.
	DWORD m_dNewEventID;                        // Successfully added Event ID
	BOOL m_bHScrollEnabled;                     // Tells whether the horizontal scrollbar is enabled or not.
	BOOL m_bVScrollEnabled;                     // Tells whether the vertical scrollbar is enabled or not.

protected:
	//------------------------------------------------------------------------
	CString         m_strSubject;   // Store subject editor value. UpdateData method used to Exchange value with dialog control.
	CString         m_strLocation;  // Store location editor value. UpdateData method used to Exchange value with dialog control
	CString         m_strBody;      // Store body editor value. UpdateData method used to Exchange value with dialog control.
	COleDateTime    m_dtStartDate;  // Store StartDate editor value. UpdateData method used to Exchange value with dialog control.
	COleDateTime    m_dtStartTime;  // Store StartTime editor value. UpdateData method used to Exchange value with dialog control.
	COleDateTime    m_dtEndDate;    // Store EndDate editor value. UpdateData method used to Exchange value with dialog control.
	COleDateTime    m_dtEndTime;    // Store EndTime editor value. UpdateData method used to Exchange value with dialog control.

	int m_nBusyStatus;      // Store BusyStatus checkbox value. UpdateData method used to Exchange value with dialog control.

	BOOL m_bAllDayEvent;    // Store AllDayEvent checkbox value. UpdateData method used to Exchange value with dialog control.
	BOOL m_bPrivate;        // Store Private checkbox value. UpdateData method used to Exchange value with dialog control.
	BOOL m_bMeeting;        // Store Meeting checkbox value. UpdateData method used to Exchange value with dialog control.
	BOOL m_bReminder;       // Store Reminder checkbox value. UpdateData method used to Exchange value with dialog control.

	int m_nAllDayEventSet;  // If value \< 0 - AllDayEvent control initialized from editing event, otherwise it is initialized from m_nAllDayEventSet value.


	BOOL m_bCategoriesEnabled;                          // Define are Categories or Labels used.
	CXTPCalendarEventCategoryComboBox m_ctrlCategories; // Categories combobox control instance.
	CXTPCalendarEventLabelComboBox    m_ctrlLabelCB;    // Label combobox control instance.

	CComboBox m_ctrlReminderCB;                         // Reminder combobox control instance.
	CComboBox m_ctrlBusyStatusCB;                       // BusyStatus combobox control instance.

	CComboBox m_ctrlScheduleIDCB;                       // ScheduleID combobox control instance.
	CStatic   m_ctrlScheduleLabel;                      // Label window for ScheduleID combobox control.
	CCheckListBox m_ctrlScheduleSet; //for MultipleSchedules support
};


#endif // !defined(_XTP_CALENDAR_EVENT_PROPERTIS_DLG_H__)
