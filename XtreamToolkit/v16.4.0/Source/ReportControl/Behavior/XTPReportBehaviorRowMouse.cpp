// XTPReportBehaviorRowMouse.cpp : implementation of the CXTPReportBehaviorRowMouse class.
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


CXTPReportBehaviorRowMouse::CXTPReportBehaviorRowMouse(
	XTPReportMouseEvent event)

	: Left (NULL)
	, Right(NULL)
{
	Left  = new CXTPReportBehaviorRowMouseButton(xtpReportMouseButtonLeft,  event);
	Right = new CXTPReportBehaviorRowMouseButton(xtpReportMouseButtonRight, event);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPReportBehaviorRowMouse::~CXTPReportBehaviorRowMouse()
{
	SAFE_DELETE(Left);
	SAFE_DELETE(Right);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPReportBehaviorRowMouse, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPReportBehaviorRowMouse, "LeftButton",  1, OleGetLeft,  VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportBehaviorRowMouse, "RightButton", 2, OleGetRight, VT_DISPATCH, VTS_NONE)
END_DISPATCH_MAP()

// {40F1E807-CBD4-4fe7-8E43-E00882A117E8}
static const GUID IID_IReportBehaviorRowMouse =
{ 0x40f1e807, 0xcbd4, 0x4fe7, { 0x8e, 0x43, 0xe0, 0x08, 0x82, 0xa1, 0x17, 0xe8 } };

BEGIN_INTERFACE_MAP(CXTPReportBehaviorRowMouse, CXTPCmdTarget)
	INTERFACE_PART(CXTPReportBehaviorRowMouse, IID_IReportBehaviorRowMouse, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportBehaviorRowMouse, IID_IReportBehaviorRowMouse)


LPDISPATCH CXTPReportBehaviorRowMouse::OleGetLeft()
{
	return XTPGetDispatch(Left);
}

LPDISPATCH CXTPReportBehaviorRowMouse::OleGetRight()
{
	return XTPGetDispatch(Right);
}

#endif // _XTP_ACTIVEX
