// XTPReportBehaviorRowMouseButton.cpp : implementation of the CXTPReportBehaviorRowMouseButton class.
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

#include "StdAfx.h"

#include <Common/XTPCustomHeap.h>

#include <ReportControl/XTPReportDefines.h>
#include <ReportControl/Behavior/XTPReportBehavior.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CXTPReportBehaviorRowMouseButton::CXTPReportBehaviorRowMouseButton(
	XTPReportMouseButton button,
	XTPReportMouseEvent  event
	)
	: m_button(button)
	, m_event (event)

	, bFocusColumn           (FALSE)
	, bEnsureVisible         (FALSE)
	, bClick                 (FALSE)
	, bContextMenu           (FALSE)
	, bCheckSelectedRows     (FALSE)
	, bDragBegin             (FALSE)
	, bDragEnd               (FALSE)
	, bDragSelectGroup       (FALSE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	None    = new CXTPReportBehaviorRowModifier();
	Control = new CXTPReportBehaviorRowModifier();
	Shift   = new CXTPReportBehaviorRowModifier();
}

CXTPReportBehaviorRowMouseButton::~CXTPReportBehaviorRowMouseButton()
{
	SAFE_DELETE(None);
	SAFE_DELETE(Control);
	SAFE_DELETE(Shift);
}

void CXTPReportBehaviorRowMouseButton::Reset()
{
	None   ->Reset();
	Control->Reset();
	Shift  ->Reset();

	bFocusColumn            = FALSE;
	bEnsureVisible          = FALSE;
	bClick                  = FALSE;
	bContextMenu            = FALSE;
	bCheckSelectedRows      = FALSE;
	bDragBegin              = FALSE;
	bDragEnd                = FALSE;
	bDragSelectGroup        = FALSE;
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPReportBehaviorRowMouseButton, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowMouseButton, FocusColumn,       1, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowMouseButton, EnsureVisible,     2, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowMouseButton, Click,             3, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowMouseButton, ContextMenu,       4, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowMouseButton, CheckSelectedRows, 5, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowMouseButton, DragBegin,         6, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowMouseButton, DragEnd,           7, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowMouseButton, DragSelectGroup,   8, VT_BOOL)

	DISP_FUNCTION_ID(CXTPReportBehaviorRowMouseButton, "None",     9, OleGetNone,    VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportBehaviorRowMouseButton, "Control", 10, OleGetControl, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportBehaviorRowMouseButton, "Shift",   11, OleGetShift,   VT_DISPATCH, VTS_NONE)

END_DISPATCH_MAP()

// {2BE742A7-3EBE-44e6-8EA0-6A32924E7D97}
static const GUID IID_IReportBehaviorRowMouseButton =
{ 0x2be742a7, 0x3ebe, 0x44e6, { 0x8e, 0xa0, 0x6a, 0x32, 0x92, 0x4e, 0x7d, 0x97 } };

BEGIN_INTERFACE_MAP(CXTPReportBehaviorRowMouseButton, CXTPCmdTarget)
	INTERFACE_PART(CXTPReportBehaviorRowMouseButton, IID_IReportBehaviorRowMouseButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportBehaviorRowMouseButton, IID_IReportBehaviorRowMouseButton)

XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowMouseButton, FocusColumn,          bFocusColumn,             BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowMouseButton, EnsureVisible,        bEnsureVisible,           BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowMouseButton, Click,                bClick,                   BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowMouseButton, ContextMenu,          bContextMenu,             BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowMouseButton, CheckSelectedRows,    bCheckSelectedRows,       BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowMouseButton, DragBegin,            bDragBegin,               BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowMouseButton, DragEnd,              bDragEnd,                 BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowMouseButton, DragSelectGroup,      bDragSelectGroup,         BOOL)

LPDISPATCH CXTPReportBehaviorRowMouseButton::OleGetNone()
{
	return XTPGetDispatch(None);
}

LPDISPATCH CXTPReportBehaviorRowMouseButton::OleGetControl()
{
	return XTPGetDispatch(Control);
}

LPDISPATCH CXTPReportBehaviorRowMouseButton::OleGetShift()
{
	return XTPGetDispatch(Shift);
}

#endif // _XTP_ACTIVEX
