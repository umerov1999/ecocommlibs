// XTPReportBehaviorRowModifier.cpp : implementation of the CXTPReportBehaviorRowModifier class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "StdAfx.h"

#include <Common/XTPCustomHeap.h>

#include <ReportControl/XTPReportDefines.h>
#include <ReportControl/Behavior/XTPReportBehavior.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CXTPReportBehaviorRowModifier::CXTPReportBehaviorRowModifier()
	: bFocusRow            (FALSE)
	, bFocusRowTemporarily (FALSE)
	, bSelectRow           (FALSE)
	, bSelectRowTemporarily(FALSE)
	, bMultipleSelection   (FALSE)
	, bKeepSelection       (FALSE)
	, bKeepFocus           (FALSE)
	, bFocusCancelsSelect  (FALSE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}


void CXTPReportBehaviorRowModifier::Reset()
{
	bFocusRow             = FALSE;
	bFocusRowTemporarily  = FALSE;
	bSelectRow            = FALSE;
	bSelectRowTemporarily = FALSE;
	bMultipleSelection    = FALSE;
	bKeepSelection        = FALSE;
	bKeepFocus            = FALSE;
}

#ifdef _XTP_ACTIVEX

// {633AD83A-0DEE-437e-99BC-AE745A3AD92E}
static const GUID IID_IReportBehaviorRowModifier =
{ 0x633ad83a, 0xdee, 0x437e, { 0x99, 0xbc, 0xae, 0x74, 0x5a, 0x3a, 0xd9, 0x2e } };

BEGIN_INTERFACE_MAP(CXTPReportBehaviorRowModifier, CXTPCmdTarget)
	INTERFACE_PART(CXTPReportBehaviorRowModifier, IID_IReportBehaviorRowModifier, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportBehaviorRowModifier, IID_IReportBehaviorRowModifier)

BEGIN_DISPATCH_MAP(CXTPReportBehaviorRowModifier, CXTPCmdTarget)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowModifier, FocusRow,             1, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowModifier, FocusRowTemporarily,  2, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowModifier, SelectRow,            3, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowModifier, SelectRowTemporarily, 4, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowModifier, MultipleSelection,    5, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowModifier, KeepSelection,        6, VT_BOOL)
	XTP_DISP_PROPERTY_EX_ID(CXTPReportBehaviorRowModifier, KeepFocus,            7, VT_BOOL)
END_DISPATCH_MAP()

XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowModifier, FocusRow,             bFocusRow,                BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowModifier, FocusRowTemporarily,  bFocusRowTemporarily,     BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowModifier, SelectRow,            bSelectRow,               BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowModifier, SelectRowTemporarily, bSelectRowTemporarily,    BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowModifier, MultipleSelection,    bMultipleSelection,       BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowModifier, KeepSelection,        bKeepSelection,           BOOL)
XTP_IMPLEMENT_PROPERTY(CXTPReportBehaviorRowModifier, KeepFocus,            bKeepFocus,               BOOL)

#endif
