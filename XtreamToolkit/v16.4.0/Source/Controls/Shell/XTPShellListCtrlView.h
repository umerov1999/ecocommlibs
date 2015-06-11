// XTPShellListCtrlView.h : header file
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
#if !defined(__XTPSHELLLISTCTRLVIEW_H__)
#define __XTPSHELLLISTCTRLVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLATE_SHELLLIST_BASE(CXTPShellListViewBase, CListView, CXTPShellListBase)
DECLATE_SHELLLIST_BASE(CXTPShellListCtrlBase, CListCtrl, CXTPShellListBase)

//===========================================================================
// Summary:
//     CXTPShellListView is a multiple inheritance class derived from CXTPListView
//     and CXTPShellPidl. It is used to create a CXTPShellListView class object.
//===========================================================================
class _XTP_EXT_CLASS CXTPShellListView : public CXTPShellListViewBase
{
	DECLARE_DYNCREATE(CXTPShellListView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShellListView object
	//-----------------------------------------------------------------------
	CXTPShellListView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShellListView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPShellListView();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Occurs when the user double clicks on the list control.
	// Parameters:
	//     pNMHDR  - Address of an NMHDR structure that contains the notification
	//               code and additional information. For some notification messages,
	//               this parameter points to a larger structure that has the NMHDR
	//               structure as its first member.
	//     pResult - Pointer to an LRESULT variable in which to store the result
	//               code if the message is handled.
	//-----------------------------------------------------------------------
	void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPShellListView)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPShellListView)
	afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

};

//===========================================================================
// Summary:
//     CXTPShellListCtrl is a multiple inheritance class derived from CXTPListCtrl
//     and CXTPShellPidl. It is used to create a CXTPShellListCtrl window.
//===========================================================================
class _XTP_EXT_CLASS CXTPShellListCtrl : public CXTPShellListCtrlBase
{

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShellListCtrl object
	//-----------------------------------------------------------------------
	CXTPShellListCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShellListCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPShellListCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to associate the tree control with the list.
	// Parameters:
	//     pWnd - Points to the tree control that is associated with the list.
	//-----------------------------------------------------------------------
	virtual void AssociateTree(CWnd* pWnd);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPShellListCtrl)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:
	CWnd*               m_pTreeCtrl;    // Window that receives update notification, usually a CXTPShellTreeCtrl.
	bool                m_bInitControl; // true if the control has not been initialized.

};

//{{AFX_CODEJOCK_PRIVATE
#define CXTPShellList CXTPShellListCtrl
//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPShellListCtrl::AssociateTree(CWnd* pWnd) {
	ASSERT_VALID(pWnd); m_pTreeCtrl = pWnd;
}

//===========================================================================
// Summary:
//     CXTPShellListCtrlEx is a CXTPShellListCtrl derived class. It is used to create
//     a stand-alone shell list control that is not dependent on a CXTPShellTreeCtrl
//     for initialization. It is used to create a CXTPShellListCtrlEx window for
//     displaying the contents of file folders.
//===========================================================================
class _XTP_EXT_CLASS CXTPShellListCtrlEx : public CXTPShellListCtrl
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShellListCtrlEx object
	//-----------------------------------------------------------------------
	CXTPShellListCtrlEx();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShellListCtrlEx object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPShellListCtrlEx();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to browse to a particular file folder.
	// Parameters:
	//     lpszPath - A NULL terminated string that represents the folder to select.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool BrowseToFolder(LPCTSTR lpszPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will associate a CWnd object with the shell list control. This
	//     window is usually a CEdit control, but can be any CWnd object. This window will have
	//     its window text updated whenever the selected folder is changed.
	// Parameters:
	//     pSyncWnd - Points to a valid CWnd object.
	//-----------------------------------------------------------------------
	void SetSyncWnd(CWnd* pSyncWnd);


protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPShellListCtrlEx)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPShellListCtrlEx)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:

	CWnd*   m_pSyncWnd;    // Points to the window to synchronize with the shell list control.
	CString m_strItemPath; // A NULL terminated string that represents the currently selected folder.
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPShellListCtrlEx::SetSyncWnd(CWnd* pSyncWnd) {
	m_pSyncWnd = pSyncWnd;
}

#endif // !defined(__XTPSHELLLISTCTRLVIEW_H__)
