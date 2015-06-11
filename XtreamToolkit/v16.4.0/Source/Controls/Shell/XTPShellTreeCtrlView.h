// XTPShellTreeCtrl.h : header file
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
#if !defined(__XTPSHELLTREECTRL_H__)
#define __XTPSHELLTREECTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLATE_SHELLTREE_BASE(CXTPShellTreeViewBase, CTreeView, CXTPShellTreeBase)
DECLATE_SHELLTREE_BASE(CXTPShellTreeCtrlBase, CTreeCtrl, CXTPShellTreeBase)

//===========================================================================
// Summary:
//     CXTPShellTreeView is a multiple inheritance class derived from CXTPTreeView
//     and CXTPShellPidl. It is used to create a CXTPShellTreeView class object.
//===========================================================================
class _XTP_EXT_CLASS CXTPShellTreeView : public CXTPShellTreeViewBase
{
	DECLARE_DYNCREATE(CXTPShellTreeView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShellTreeView object
	//-----------------------------------------------------------------------
	CXTPShellTreeView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShellTreeView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPShellTreeView();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Called after a user makes a new tree selection.
	// Remarks:
	//     This member function is called whenever a user makes a selection
	//     within the tree control to synchronize any shell list or shell
	//     combo boxes associated with the tree. You can override this member
	//     to provide additional functionality.
	// Parameters:
	//     hItem         - Handle to the newly selected tree item.
	//     strFolderPath - NULL terminated string representing the fully
	//                     qualified path to the selected tree item.
	//-----------------------------------------------------------------------
	virtual void SelectionChanged(HTREEITEM hItem, CString strFolderPath);

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPShellTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
//}}AFX_CODEJOCK_PRIVATE

protected:
};

//===========================================================================
// Summary:
//     CXTPShellTreeCtrl is a multiple inheritance class derived from CXTPTreeCtrl
//     and CXTPShellPidl. CXTPShellTreeCtrl is used to create a tree control
//     that displays an explorer style tree, and can be associated with a combo box
//     and list control.
//===========================================================================
class _XTP_EXT_CLASS CXTPShellTreeCtrl : public CXTPShellTreeCtrlBase
{
	DECLARE_DYNAMIC(CXTPShellTreeCtrl)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShellTreeCtrl object
	//-----------------------------------------------------------------------
	CXTPShellTreeCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShellTreeCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPShellTreeCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CInitialize class to
	//     perform initialization when the window is created or sub-classed.
	// Returns:
	//     TRUE if the window was successfully initialized, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual bool Init();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable auto-initialization
	//     of the shell tree control.
	// Parameters:
	//     bEnable - TRUE if the tree initializes upon creation.
	//-----------------------------------------------------------------------
	void EnableAutoInit(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to associate the list control with the tree.
	// Parameters:
	//     pWnd - Points to the list control that is associated with the tree.
	//-----------------------------------------------------------------------
	virtual void AssociateList(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to associate a CComboBox object with
	//     the control. Whenever the path changes, the combo is updated.
	// Parameters:
	//     pWnd - Points to the combo box that is associated with the tree.
	//-----------------------------------------------------------------------
	virtual void AssociateCombo(CWnd* pWnd);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Called after a user makes a new tree selection.
	// Remarks:
	//     This member function is called whenever a user makes a selection
	//     within the tree control to synchronize any shell list or shell
	//     combo boxes associated with the tree. You can override this member
	//     to provide additional functionality.
	// Parameters:
	//     hItem         - Handle to the newly selected tree item.
	//     strFolderPath - NULL terminated string representing the fully
	//                     qualified path to the selected tree item.
	//-----------------------------------------------------------------------
	virtual void SelectionChanged(HTREEITEM hItem, CString strFolderPath);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPShellTreeCtrl)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPShellTreeCtrl)
	afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL  m_bAutoInit; // TRUE if the tree control is to initialize when created.
	CWnd* m_pListCtrl; // Window that receives the update notification, usually a CXTPShellListCtrl.
};

//{{AFX_CODEJOCK_PRIVATE
#define CXTPShellTree    CXTPShellTreeCtrl
//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPShellTreeCtrl::EnableAutoInit(BOOL bEnable) {
	m_bAutoInit = bEnable;
}

#endif // !defined(__XTPSHELLTREECTRL_H__)
