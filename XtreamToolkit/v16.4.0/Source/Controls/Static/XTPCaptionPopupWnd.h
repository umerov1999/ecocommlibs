// XTPCaptionPopupWnd.h : interface for the CXTPCaptionPopupWnd class.
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
#if !defined(__XTPCAPTIONPOPUPWND_H__)
#define __XTPCAPTIONPOPUPWND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// --------------------------------------------------------------------
// Summary:
//     CXTPCaptionPopupWnd is a CWnd derived class. It is used by
//     the CXTPCaption class to display a popup child window similar to
//     Outlook(tm).
// --------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPCaptionPopupWnd : public CWnd
{
	DECLARE_DYNAMIC(CXTPCaptionPopupWnd)

public:

	// -------------------------------------------------
	// Summary:
	//     Constructs a CXTPCaptionPopupWnd object
	// -------------------------------------------------
	CXTPCaptionPopupWnd();

	// --------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCaptionPopupWnd object, handles cleanup and
	//     deallocation
	// --------------------------------------------------------------------
	virtual ~CXTPCaptionPopupWnd();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to create and display the popup
	//     window.
	// Parameters:
	//     rect       - A CRect reference that represents the size of the
	//                  popup window.
	//     pParentWnd - A CWnd pointer that represents the popup window.
	//     pChildWnd  - A CWnd pointer that represents the child to be
	//                  displayed when the window is activated.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(const CRect& rect, CWnd* pParentWnd, CWnd* pChildWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to recalculate the layout of the
	//     caption and child window.
	//-----------------------------------------------------------------------
	virtual void RecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to reset the parent of the child
	//     window back to its default.
	// Returns:
	//     Returns TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL ResetParent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to send a cancel message that will
	//     cause the popup to close.
	// Returns:
	//     Returns TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SendCancelMessage();

	void SetTheme(XTPControlTheme nTheme);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to register the window class for
	//     the popup window.
	// Returns:
	//     Returns TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPCaptionPopupWnd)
	afx_msg void OnDestroy();
	afx_msg void OnCaptButton();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	CWnd*            m_pParentWnd;    // A CWnd pointer that represents the parent window.
	CWnd*            m_pChildWnd;     // A CWnd pointer that represents the child displayed in the popup.
	CWnd*            m_pChildParent;  // A CWnd pointer that represents the parent of the child window.
	CXTPCaption       m_Caption;       // Caption that is displayed when the window is active.
	CXTPCaptionButton m_CaptionButton; // Close button associated with the caption.
	CXTPIconHandle    m_hIconOn;       // Handle to a normal button icon.
	CXTPIconHandle    m_hIconOff;      // Handle to a pressed button icon.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPCaptionPopupWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // #if !defined(__XTPCAPTIONPOPUPWND_H__)
