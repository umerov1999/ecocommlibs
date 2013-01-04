// XTPListCtrlView.h interface for the CXTPListView and CXTPListCtrlView class.
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
#if !defined(__XTPLISTVIEW_H__)
#define __XTPLISTVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

DECLATE_LIST_BASE(CXTPListViewBase, CListView, CXTPListBase)
DECLATE_LIST_BASE(CXTPListCtrlBase, CListCtrl, CXTPListBase)

//===========================================================================
// Summary:
//     CXTPListView is a multiple inheritance class derived from CListView and
//     CXTPListBase. This class implements flat header and generic sorting
//     routines.
// See Also: CXTPListBase
//===========================================================================
class _XTP_EXT_CLASS CXTPListView : public CXTPListViewBase
{
	DECLARE_DYNCREATE(CXTPListView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPListView object
	//-----------------------------------------------------------------------
	CXTPListView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPListView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPListView();

protected:
	DECLARE_MESSAGE_MAP()
};

//---------------------------------------------------------------------------
// Summary:
//     CXTPListCtrl is a multiple inheritance class derived from CListCtrl and
//     CXTPListBase. This class implements flat header and generic sorting
//     routines. See CXTPListBase for additional functionality.
// See Also: CXTPListBase
//---------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPListCtrl : public CXTPListCtrlBase
{
	DECLARE_DYNAMIC(CXTPListCtrl)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPListCtrl object
	//-----------------------------------------------------------------------
	CXTPListCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPListCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};

#endif // #if !defined(__XTPLISTVIEW_H__)
