// XTPMaskEdit.h interface for the CXTPMaskEdit class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPMASKEDIT_H__)
#define __XTPMASKEDIT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTPMaskEdit is a CXTPEdit derived class. It allows text masking to be
//     applied to the control to format it for special editing restrictions.
//===========================================================================
class _XTP_EXT_CLASS CXTPMaskEdit : public CXTPEdit
{
	DECLARE_DYNAMIC(CXTPMaskEdit)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMaskEdit object
	//-----------------------------------------------------------------------
	CXTPMaskEdit();

	// ----------------------------------------------------------------------
	// Summary:
	//     Initializes the CXTPMaskEdit control.
	// Remarks:
	//     Call this member function to initialize edit control. You can
	//     call this after you have created or subclassed your edit
	//     control.
	// Parameters:
	//     pParentWnd -  Pointer to the parent of the edit control.
	// Returns:
	//     True if the control was successfully initialized, otherwise false.
	// ----------------------------------------------------------------------
	virtual bool Initialize(CWnd* pParentWnd);
};

//===========================================================================
// Summary:
//     CXTPDateEdit is a CXTPMaskEdit derived class. It is specifically
//     geared toward editing date fields.
//===========================================================================
class _XTP_EXT_CLASS CXTPDateEdit : public CXTPMaskEdit
{
	DECLARE_DYNAMIC(CXTPDateEdit)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDateEdit object
	//-----------------------------------------------------------------------
	CXTPDateEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the time based on the text string
	//     passed in as 'strDate'.
	// Parameters:
	//     dt      - A reference to a COleDateTime object that represents the
	//               date to display.
	//     strDate - A NULL terminated string that represents the date to display.
	//-----------------------------------------------------------------------
	virtual void SetDateTime(COleDateTime& dt);
	virtual void SetDateTime(LPCTSTR strDate); //<combine CXTPDateEdit::SetDateTime@COleDateTime&>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve a COleDateTime object that
	//     represents the currently displayed date.
	// Returns:
	//     A COleDateTime object representing the currently displayed date.
	//-----------------------------------------------------------------------
	virtual COleDateTime GetDateTime();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve a CString object that represents
	//     the currently displayed date.
	// Returns:
	//     A CString object representing the currently displayed date.
	//-----------------------------------------------------------------------
	virtual CString GetWindowDateTime();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will read the date string passed in as 'lpszData'
	//     and is converted and returned as a COleDateTime object.
	// Parameters:
	//     lpszData - A NULL terminated string that represents the date to convert.
	// Returns:
	//     A COleDateTime object that represents the converted date string.
	//-----------------------------------------------------------------------
	virtual COleDateTime ReadOleDateTime(LPCTSTR lpszData);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used internally to process the character passed
	//     in by 'nChar' whose index is specified by 'nEndPos'.
	// Parameters:
	//     nChar   - Contains the character code value of the key.
	//     nEndPos - Index of character in display string.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual BOOL ProcessMask(TCHAR& nChar, int nEndPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will read the date passed in as 'dt', and format
	//     the 'strData' string as DD/MM/YY.
	// Parameters:
	//     strData - String reference that is filled with the date.
	//     dt      - COleDateTime object that represents the date to format.
	//-----------------------------------------------------------------------
	virtual void FormatOleDateTime(CString& strData, COleDateTime dt);
};

// ----------------------------------------------------------------------------
// Summary:
//     The DDX_XTPOleDateTime function manages the transfer of
//     integer data between a date edit control, in a dialog box, form
//     view, or control view object, and a COleDateTime data member of
//     the dialog box, form view, or control view object.
//
//     When DDX_XTPOleDateTime is called, <i>'value'</i> is set to
//     the current state of the date edit control.
// Parameters:
//     pDX -        A pointer to a CDataExchange object. The framework supplies
//                  this object to establish the context of the data exchange,
//                  including its direction.
//     nIDC -       The resource ID of the date edit control associated with the
//                  control property.
//     rControl -   A reference to a member variable of the dialog box, form
//                  view, or control view object with which data is exchanged.
//     rDateTime -  A reference to a member variable of the dialog box, form
//                  view, or control view object with which data is exchanged.
// See Also:
//     CXTPDateEdit
// ----------------------------------------------------------------------------
_XTP_EXT_CLASS void AFXAPI DDX_XTPOleDateTime(CDataExchange* pDX, int nIDC, CXTPDateEdit& rControl, COleDateTime& rDateTime);

//===========================================================================
// Summary:
//     CXTPTimeEdit is a CXTPDateEdit derived class. It is specifically geared
//     toward editing time fields.
//===========================================================================
class _XTP_EXT_CLASS CXTPTimeEdit : public CXTPDateEdit
{
	DECLARE_DYNAMIC(CXTPTimeEdit)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTimeEdit object
	//-----------------------------------------------------------------------
	CXTPTimeEdit();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will update the hours displayed.
	// Parameters:
	//     nHours - The new hour to be displayed.
	//-----------------------------------------------------------------------
	virtual void SetHours(int nHours);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will update the minutes displayed.
	// Parameters:
	//     nMins - The new minutes to be displayed.
	//-----------------------------------------------------------------------
	virtual void SetMins(int nMins);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will update the hours and minutes that are displayed
	//     for the time edit control.
	// Parameters:
	//     nHours - The new hour to be displayed.
	//     nMins  - The new minutes to be displayed.
	//-----------------------------------------------------------------------
	virtual void SetTime(int nHours, int nMins);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve the hours displayed for the time edit
	//     control.
	// Returns:
	//     An integer value that represents the hours displayed.
	//-----------------------------------------------------------------------
	int GetHours() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve the minutes displayed for the time
	//     edit control.
	// Returns:
	//     An integer value that represents the minutes displayed.
	//-----------------------------------------------------------------------
	int GetMins() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used internally to process the character
	//     passed in by 'nChar' whose index is specified by 'nEndPos'.
	// Parameters:
	//     nChar   - Contains the character code value of the key.
	//     nEndPos - Index of the character in the display string.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual BOOL ProcessMask(TCHAR& nChar, int nEndPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will read the time passed in as 'dt',
	//     and format the 'strData' string as HH:MM.
	// Parameters:
	//     strData - String reference that is filled with the time.
	//     dt      - COleDateTime object that represents the time to format.
	//-----------------------------------------------------------------------
	virtual void FormatOleDateTime(CString& strData, COleDateTime dt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets military time format
	// Parameters:
	//     bMilitary - true if military time is used.
	//-----------------------------------------------------------------------
	void SetMilitary(BOOL bMilitary = TRUE);

protected:
	int m_iHours;       // Represents the hours to display.
	int m_iMins;        // Represents the minutes to display.
	BOOL m_bMilitary;   // true if military time is used.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTPTimeEdit::GetHours() const {
	return m_iHours;
}
AFX_INLINE int CXTPTimeEdit::GetMins() const {
	return m_iMins;
}
AFX_INLINE void CXTPTimeEdit::SetMilitary(BOOL bMilitary /*= TRUE*/) {
	m_bMilitary = bMilitary;

}

#endif // #if !defined(__XTPMASKEDIT_H__)
