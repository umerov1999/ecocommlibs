// XTPResizePropertySheet.cpp: implementation of the CXTPResizePropertySheet class.
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

#include "Common/XTPDrawHelpers.h"


#include "XTPResizeRect.h"
#include "XTPResizePoint.h"
#include "XTPResize.h"
#include "XTPResizePropertySheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

// Wizard mode ID's not in AFX headers
#define ID_WIZLINE      (ID_WIZFINISH + 1)

/////////////////////////////////////////////////////////////////////////////
// CXTPResizePropertySheet

IMPLEMENT_DYNCREATE(CXTPResizePropertySheet, CPropertySheet)

BEGIN_MESSAGE_MAP(CXTPResizePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CXTPResizePropertySheet)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT_EX(PSN_SETACTIVE, OnPageChanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPResizePropertySheet construction/destruction

CXTPResizePropertySheet::CXTPResizePropertySheet() // add default constructor, PCVA - Yair 3/6/2003
: CPropertySheet()
, CXTPResize(this, 0)
{

}

CXTPResizePropertySheet::CXTPResizePropertySheet(const UINT nCaption, CWnd* pParent, const UINT nSelectPage, const UINT nFlags)
: CPropertySheet(nCaption, pParent, nSelectPage)
, CXTPResize(this, nFlags)
{

}

CXTPResizePropertySheet::CXTPResizePropertySheet(LPCTSTR pszCaption, CWnd* pParent, const UINT nSelectPage, const UINT nFlags)
: CPropertySheet(pszCaption, pParent, nSelectPage)
, CXTPResize(this, nFlags)
{

}

BOOL CXTPResizePropertySheet::OnInitDialog()
{

	CRect rcClientBegin, rcClientEnd;
	GetWindowRect(rcClientBegin);
	SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)(LPRECT)rcClientBegin);

	// Modify the window style to include WS_THICKFRAME for resizing.
	::SetWindowLong(m_hWnd,
		GWL_STYLE, GetStyle() | WS_THICKFRAME);

	GetWindowRect(rcClientEnd);
	SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)(LPRECT)rcClientEnd);

	CPropertySheet::OnInitDialog();

	// subclass our "flicker-free" tab control.
	m_tabCtrl.SubclassWindow(GetTabControl()->m_hWnd);

	// the size icon is too close to the buttons, so inflate the sheet
	CRect rc;
	GetWindowRect(rc);

	if (rcClientBegin.Width() - rcClientEnd.Width() > 3)
	{
		rc.InflateRect((rcClientBegin.Width() - rcClientEnd.Width()) / 2,
			(rcClientBegin.Height() - rcClientEnd.Height()) / 2);
		MoveWindow(rc);
	}
	// Do this last so that any prop pages are moved accordingly
	else if (!HasFlag(xtpResizeNoSizeIcon) && !IsWizard())
	{
		rc.InflateRect(1, 1, 2, 2);
		MoveWindow(rc);
	}

	// add sizing entries to the system menu
	CMenu* pSysMenu = (CMenu*)GetSystemMenu(FALSE);
	if (pSysMenu)
	{
		CString szMaximize, szMinimize, szSize, szRestore;
		// try to get the strings from the topmost window
		CWnd* pwndTop;
		for (pwndTop = this; pwndTop->GetParent(); pwndTop = pwndTop->GetParent());

		CMenu* pTopSysMenu = (CMenu*)pwndTop->GetSystemMenu(FALSE);
		if (pTopSysMenu)
		{
			pTopSysMenu->GetMenuString(SC_MAXIMIZE, szMaximize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_MINIMIZE, szMinimize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_SIZE, szSize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_RESTORE, szRestore, MF_BYCOMMAND);
		}
		// if we din't get the strings then set them to the English defaults
		if (szMaximize.IsEmpty()) szMaximize = _T("Ma&ximize");
		if (szMinimize.IsEmpty()) szMinimize = _T("Mi&nimize");
		if (szSize.IsEmpty()) szSize = _T("&Size");
		if (szRestore.IsEmpty()) szRestore = _T("&Restore");

		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR, 0, (LPCTSTR) 0);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_MAXIMIZE, szMaximize);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_MINIMIZE, szMinimize);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_SIZE, szSize);
		pSysMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, SC_RESTORE, szRestore);
	}

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwStyle & WS_THICKFRAME) == 0)
	{
		SetFlag(xtpResizeNoSizeIcon);
	}

	CXTPResize::Init();

	// Check which buttons are available in sheet or wizard
	if (IsWizard())
	{
		SetResize(ID_WIZBACK, XTP_ATTR_REPOS(1));
		SetResize(ID_WIZNEXT, XTP_ATTR_REPOS(1));
		SetResize(ID_WIZFINISH, XTP_ATTR_REPOS(1));
		SetResize(ID_WIZLINE, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMRIGHT);
	}
	else
	{
		SetResize(IDOK, XTP_ATTR_REPOS(1));
		SetResize(ID_APPLY_NOW, XTP_ATTR_REPOS(1));
		SetResize(AFX_IDC_TAB_CONTROL, XTP_ATTR_RESIZE(1));
	}
	SetResize(IDCANCEL, XTP_ATTR_REPOS(1));
	SetResize(IDHELP, XTP_ATTR_REPOS(1));

	// set page sizings
	CRect rcPage;
	GetActivePage()->GetWindowRect(rcPage);
	ScreenToClient(rcPage);
	int i;
	for (i = 0; i <GetPageCount(); i++)
	{
		SetResize(GetPage(i), XTP_ATTR_RESIZE(1), rcPage);
	}


	return TRUE;
}

void CXTPResizePropertySheet::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTPResize::GetMinMaxInfo(pMMI);
}

void CXTPResizePropertySheet::OnSize(UINT nType, int cx, int cy)
{
	CPropertySheet::OnSize(nType, cx, cy);
	CXTPResize::Size();
}

BOOL CXTPResizePropertySheet::OnPageChanging(NMHDR* /*pNotifyStruct*/, LRESULT* /*pResult*/)
{
	// When in wizard mode, the property sheet seems to resize/move the page
	// back to it's original size/position on the sheet.  We need to let the
	// size manager know that this has occurred, so that it can move the pages
	// back to the correct position
	if (IsWizard())
	{
		CXTPResize::Reset();
	}

	// Update new wizard page, the active page changes after this notification
	CXTPResize::Size();

	// and continue routing...
	return FALSE;
}

void CXTPResizePropertySheet::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty())
	{
		SavePlacement(m_strSection);
	}

	CPropertySheet::OnDestroy();
}
