// XTPCalendarRemindersDialog.h: interface for the CXTPCalendarDayViewTimeScale class.
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
#if !defined(_XTP_CALENDAR_REMINDERS_DIALOG_H__)
#define _XTP_CALENDAR_REMINDERS_DIALOG_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPCalendarRemindersManager;
class CXTPCalendarControl;
class CXTPNotifySink;
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// Summary:
//      This class used to display calendar reminders. This is a default
//      implementation which can be used as an example or as a base class
//      for custom dialogs.
//      It designed to be localized - XTPResourceManager is used to load
//      resources.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarRemindersDialog : public CDialog
{
// Construction
public:
	//{{AFX_CODEJOCK_PRIVATE
	enum { IDD = XTP_IDD_CALENDAR_REMINDERS};
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Dialog class constructor.
	// Parameters:
	//     pParent       - [in] Pointer to parent window. Can be NULL.
	//     nIDTemplate   - [in] Contains the ID number of a dialog-box template resource.
	//-----------------------------------------------------------------------
	CXTPCalendarRemindersDialog(CWnd* pParent = NULL,
		UINT nIDTemplate = CXTPCalendarRemindersDialog::IDD);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarRemindersDialog();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this method to create a dialog window..
	// Parameters:
	//     pParent  - [in] Pointer to parent window. Can be NULL.
	// Returns:
	//     TRUE if dialog window created successfully, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParent = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this method to attach dialog to calendar control (and reminders manager).
	// Parameters:
	//     pCalendar    - [in] Pointer to control.
	//-----------------------------------------------------------------------
	virtual void SetRemindersManager(CXTPCalendarControl* pCalendar);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this method to update dialog reminders list from the reminders
	//      manager.
	//-----------------------------------------------------------------------
	virtual void UpdateFromManager();

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this method to update dialog reminders list from the calendar
	//      control (and reminders manager).
	//-----------------------------------------------------------------------
	virtual void Detach();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to determine is dialog window displayed in
	//     the Microsoft Windows taskbar.
	// Returns:
	//     TRUE if dialog displayed in the taskbar, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsShowInTaskBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to set is dialog window displayed in
	//     the Microsoft Windows taskbar.
	// Parameters:
	//     bShow    - [in] Set TRUE to show dialog in the taskbar, FALSE otherwise
	//-----------------------------------------------------------------------
	virtual void SetShowInTaskBar(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to determine is dialog window pop up automatically
	//     when new reminders arrived.
	// Returns:
	//     TRUE if dialog pop up automatically, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsAutoPopup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to set is dialog window pop up automatically
	//     when new reminders arrived.
	// Parameters:
	//     bAutoPopup - [in] Set TRUE to pop up  dialog automatically, FALSE otherwise
	//-----------------------------------------------------------------------
	void SetAutoPopup(BOOL bAutoPopup = TRUE);

protected:
	CXTPCalendarRemindersManager*   m_pRemindersMan;        // Store pointer to reminders manager.
	CXTPCalendarReminders           m_arActiveReminders;    // Store active reminders list.

	CXTPCalendarControl* m_pCalendar;   // Store pointer to calendar control.

	int m_nNextRefreshIndex;    // Store next reminder item index for automatic refresh by timer or -1 to refresh all.

	CXTPNotifySink* m_pSink;

	virtual void OnReminders(XTP_NOTIFY_CODE Event, WPARAM wParam , LPARAM lParam);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void InitSnoozeComboBox();

	virtual void UpdateControlsBySelection();
	virtual void SelectItem(int nSelItemIndex);
	virtual void UpdateTitle();
	virtual BOOL _RemoveReminderInfo(CXTPCalendarReminder* pReminder);

	virtual void OnOK();
	afx_msg void OnDismiss();
	afx_msg void OnDismissAll();
	afx_msg void OnOpenItem();
	afx_msg void OnSnooze();
	afx_msg void OnRemindersList_ItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRemindersList_DblClick(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CListCtrl   m_ctrlRemindersList;    // Reminders list control.
	CStatic     m_ctrlDescription;      // Description control.

	CComboBox   m_ctrlSnoozeTimeCB; // Snooze combobox control.
	CButton     m_ctrlSnoozeBtn;    // Snooze button.

	CButton m_ctrlDismissBtn;       // Dismiss button.
	CButton m_ctrlDismissAllBtn;    // Dismiss All button.
	CButton m_ctrlOpenItemBtn;      // Open Item button.

	BOOL m_bShowInTaskBar;  // Store ShowInTaskBar property value.
	BOOL m_bAutoPopup;      // Store AutoPopup property value.
};

/////////////////////////////////////////////////////////////////////////////
AFX_INLINE void CXTPCalendarRemindersDialog::Detach() {
	SetRemindersManager(NULL);
}

AFX_INLINE BOOL CXTPCalendarRemindersDialog::IsAutoPopup() const {
	return m_bAutoPopup;
}

AFX_INLINE void CXTPCalendarRemindersDialog::SetAutoPopup(BOOL bAutoPopup) {
	m_bAutoPopup = bAutoPopup;
}

#endif // !defined(_XTP_CALENDAR_REMINDERS_DIALOG_H__)
