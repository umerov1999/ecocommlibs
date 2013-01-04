// XTPReportBehavior.h: Declaration of the CXTPReportBehavior class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#if !defined(__XTPREPORTBEHAVIOR_H__)
#define __XTPREPORTBEHAVIOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum XTPReportMouseButton
{
	xtpReportMouseButtonNone,
	xtpReportMouseButtonLeft,
	xtpReportMouseButtonMiddle,
	xtpReportMouseButtonRight
};


enum XTPReportMouseEvent
{
	xtpReportMouseEventButtonDown,
	xtpReportMouseEventButtonUp
};

enum XTPReportBehavior
{
	xtpReportBehaviorOutlook2003     = 0x00001100, // 11.0
	xtpReportBehaviorOutlook2007     = 0x00001200, // 12.0
	xtpReportBehaviorOutlook2010     = 0x00001400, // 14.0

	xtpReportBehaviorExcel2003       = 0x10001100, // 11.0
	xtpReportBehaviorExcel2007       = 0x10001200, // 12.0
	xtpReportBehaviorExcel2010       = 0x10001400, // 14.0

	//xtpReportBehaviorCodejock1300    = 0x20001300, // 13.0.0
	//xtpReportBehaviorCodejock1321    = 0x20001321, // 13.2.1
	//xtpReportBehaviorCodejock1330    = 0x20001330, // 13.3.0
	//xtpReportBehaviorCodejock1331    = 0x20001331, // 13.3.1
	xtpReportBehaviorCodejock1340    = 0x20001340, // 13.4.0
	xtpReportBehaviorCodejock1341    = 0x20001341, // 13.4.1
	xtpReportBehaviorCodejock1342    = 0x20001342, // 13.4.2
	xtpReportBehaviorCodejock1501    = 0x20001501, // 15.0.1
	xtpReportBehaviorCodejock1502    = 0x20001502, // 15.0.2
	xtpReportBehaviorCodejockDefault = 0x20000000
};


/////////////////////////////////////////////////////////////////////////////
// Key events
/////////////////////////////////////////////////////////////////////////////

class _XTP_EXT_CLASS CXTPReportBehaviorRowKeyDown
{

};

class _XTP_EXT_CLASS CXTPReportBehaviorRowKeyUp
{

};


/////////////////////////////////////////////////////////////////////////////
// Mouse events
/////////////////////////////////////////////////////////////////////////////

class _XTP_EXT_CLASS CXTPReportBehaviorRowModifier : public CXTPCmdTarget
{
public:

	CXTPReportBehaviorRowModifier();
	void Reset();

	BOOL bFocusRow;               // TRUE if the row gets the focus
	BOOL bFocusRowTemporarily;    // TRUE if the row gets the focus temporarily

	BOOL bSelectRow;              // TRUE if the row gets selected
	BOOL bSelectRowTemporarily;   // TRUE if the row gets selected temporarily
	BOOL bMultipleSelection;      // TRUE to enable multiple selection

	BOOL bKeepSelection;          // TRUE if multiple rows stay selected
	BOOL bKeepFocus;              //

	BOOL bFocusCancelsSelect;     // If TRUE

protected:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_OLETYPELIB_EX(CXTPReportBehaviorRowModifier);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	XTP_DECLARE_PROPERTY(FocusRow,             BOOL);
	XTP_DECLARE_PROPERTY(FocusRowTemporarily,  BOOL);
	XTP_DECLARE_PROPERTY(SelectRow,            BOOL);
	XTP_DECLARE_PROPERTY(SelectRowTemporarily, BOOL);
	XTP_DECLARE_PROPERTY(MultipleSelection,    BOOL);
	XTP_DECLARE_PROPERTY(KeepSelection,        BOOL);
	XTP_DECLARE_PROPERTY(KeepFocus,            BOOL);
//}}AFX_CODEJOCK_PRIVATE
#endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPReportBehaviorRowMouseButton : public CXTPCmdTarget
{
public:

	CXTPReportBehaviorRowMouseButton(
		XTPReportMouseButton button,
		XTPReportMouseEvent  event
		);
	virtual ~CXTPReportBehaviorRowMouseButton();

	void Reset();

public:

	CXTPReportBehaviorRowModifier *None;
	CXTPReportBehaviorRowModifier *Control;
	CXTPReportBehaviorRowModifier *Shift;

	BOOL bFocusColumn;            // TRUE if the column gets the focus
	BOOL bEnsureVisible;          // TRUE if the row gets visible

	BOOL bClick;                  // TRUE
	BOOL bContextMenu;            // TRUE if OnContextMenu / NM_RCLICK gets called.
	BOOL bCheckSelectedRows;      // TRUE

	BOOL bDragBegin;              // TRUE to begin drag
	BOOL bDragEnd;                // TRUE to end drag
	BOOL bDragSelectGroup;        // TRUE to select the entire group on drag

public:

	const XTPReportMouseButton m_button;
	const XTPReportMouseEvent  m_event;

protected:

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPReportBehaviorRowMouseButton);

	afx_msg LPDISPATCH OleGetNone();
	afx_msg LPDISPATCH OleGetControl();
	afx_msg LPDISPATCH OleGetShift();

	XTP_DECLARE_PROPERTY(FocusColumn, BOOL);
	XTP_DECLARE_PROPERTY(EnsureVisible, BOOL);

	XTP_DECLARE_PROPERTY(Click, BOOL);
	XTP_DECLARE_PROPERTY(ContextMenu, BOOL);
	XTP_DECLARE_PROPERTY(CheckSelectedRows, BOOL);

	XTP_DECLARE_PROPERTY(DragBegin, BOOL);
	XTP_DECLARE_PROPERTY(DragEnd, BOOL);
	XTP_DECLARE_PROPERTY(DragSelectGroup, BOOL);
//}}AFX_CODEJOCK_PRIVATE
#endif // _XTP_ACTIVEX
};

class _XTP_EXT_CLASS CXTPReportBehaviorRowMouse : public CXTPCmdTarget
{
public:

	CXTPReportBehaviorRowMouse(XTPReportMouseEvent event);
	virtual ~CXTPReportBehaviorRowMouse();

	CXTPReportBehaviorRowMouseButton *Left;
	CXTPReportBehaviorRowMouseButton *Right;

protected:

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPReportBehaviorRowMouse);

	afx_msg LPDISPATCH OleGetLeft();
	afx_msg LPDISPATCH OleGetRight();
//}}AFX_CODEJOCK_PRIVATE
#endif // _XTP_ACTIVEX
};


class _XTP_EXT_CLASS CXTPReportBehaviorRow : public CXTPCmdTarget
{
public:
	CXTPReportBehaviorRow();
	virtual ~CXTPReportBehaviorRow();

public:
	CXTPReportBehaviorRowMouse *MouseDown;
	CXTPReportBehaviorRowMouse *MouseUp;

#ifdef _XTP_ACTIVEX
protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPReportBehaviorRow);

	afx_msg LPDISPATCH OleGetMouseDown();
	afx_msg LPDISPATCH OleGetMouseUp();
//}}AFX_CODEJOCK_PRIVATE
#endif // _XTP_ACTIVEX
};

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

class _XTP_EXT_CLASS CXTPReportNotificationsPopulate : public CXTPCmdTarget
{
public:
	CXTPReportNotificationsPopulate();
	virtual ~CXTPReportNotificationsPopulate();

public:

	BOOL bFocusChanging;
	BOOL bSelectionChanging;
	BOOL bSelectionChanged;
};


class _XTP_EXT_CLASS CXTPReportNotificationsSelectedRows : public CXTPCmdTarget
{
public:
	CXTPReportNotificationsSelectedRows();
	virtual ~CXTPReportNotificationsSelectedRows();

public:

	BOOL bClear;           // If TRUE
	BOOL bAdd;             // If TRUE
	BOOL bRemove;          // If TRUE

public:

};

class _XTP_EXT_CLASS CXTPReportNotifications : public CXTPCmdTarget
{

public:
	CXTPReportNotifications();
	virtual ~CXTPReportNotifications();

public:

	CXTPReportNotificationsPopulate     *Populate;
	CXTPReportNotificationsSelectedRows *SelectedRows;
};


class _XTP_EXT_CLASS CXTPReportBehavior : public CXTPCmdTarget
{
public:

	CXTPReportBehavior(XTPReportBehavior behavior);
	virtual ~CXTPReportBehavior();

	void SetScheme(XTPReportBehavior behavior);

public:

	CXTPReportBehaviorRow *Row;
	CXTPReportBehaviorRow *GroupRow;

	CXTPReportNotifications *Notifications;

private:

	void SetSchemeOutlook();
	void SetSchemeExcel();
	void SetSchemeCodejock134x();
	void SetSchemeCodejock1501();
	void SetSchemeCodejock1502();
	void SetSchemeCodejockDefault();

	XTPReportBehavior m_scheme;      // The last set scheme

#ifdef _XTP_ACTIVEX
protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPReportBehavior);

	XTP_DECLARE_PROPERTY(Scheme, XTPReportBehavior);

	afx_msg LPDISPATCH OleGetRow();
	afx_msg LPDISPATCH OleGetGroupRow();
//}}AFX_CODEJOCK_PRIVATE
#endif // _XTP_ACTIVEX
};

#endif //#if !defined(__XTPREPORTBEHAVIOR_H__)
