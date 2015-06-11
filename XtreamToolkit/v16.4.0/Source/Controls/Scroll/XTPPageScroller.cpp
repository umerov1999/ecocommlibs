// XTPPageScroller.cpp : implementation of the CXTPPageScroller class.
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

#include "stdafx.h"
#include "XTPPageScroller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPageScroller
/////////////////////////////////////////////////////////////////////////////
CXTPPageScroller::CXTPPageScroller()
{

}

CXTPPageScroller::~CXTPPageScroller()
{

}

IMPLEMENT_DYNAMIC(CXTPPageScroller, CWnd)

BEGIN_MESSAGE_MAP(CXTPPageScroller, CWnd)
	//{{AFX_MSG_MAP(CXTPPageScroller)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPPageScroller::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	// initialize common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_PAGESCROLLER_CLASS;
	VERIFY(::InitCommonControlsEx(&icex));

	return CWnd::Create(WC_PAGESCROLLER, NULL, dwStyle | WS_CLIPCHILDREN, rect, pParentWnd, nID);
}
