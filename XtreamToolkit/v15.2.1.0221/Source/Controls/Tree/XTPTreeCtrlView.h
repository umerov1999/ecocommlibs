// XTPTreeCtrlView.h interface for the CXTPTreeView class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPTREEVIEW_H__)
#define __XTPTREEVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLATE_TREE_BASE(CXTPTreeViewBase, CTreeView, CXTPTreeBase)
DECLATE_TREE_BASE(CXTPTreeCtrlBase, CTreeCtrl, CXTPTreeBase)

//===========================================================================
// Summary:
//     CXTPTreeView is a CTreeView derived class. It extends the CTreeView
//     class to add additional functionality, including tree traversal, searching,
//     color, and settings.
//===========================================================================
class _XTP_EXT_CLASS CXTPTreeView : public CXTPTreeViewBase
{
	DECLARE_DYNCREATE(CXTPTreeView)

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Protected constructor used by dynamic creation. Constructs a CXTPTreeView
	//     object
	//-----------------------------------------------------------------------
	CXTPTreeView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTreeView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPTreeView();

	DECLARE_MESSAGE_MAP()
};

//===========================================================================
// Summary:
//     CXTPTreeCtrl is a CTreeCtrl derived class. It extends the CTreeCtrl
//     class to add additional functionality, including tree traversal, searching,
//     color, and settings.
//===========================================================================
class _XTP_EXT_CLASS CXTPTreeCtrl : public CXTPTreeCtrlBase
{
	DECLARE_DYNAMIC(CXTPTreeCtrl)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTreeCtrl object
	//-----------------------------------------------------------------------
	CXTPTreeCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTreeCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPTreeCtrl();

protected:

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(__XTPTREEVIEW_H__)
