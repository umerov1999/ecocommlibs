// XTPBrowseDialog.cpp : implementation of the CXTPBrowseDialog class.
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

#include "stdafx.h"
#include "Common/XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "Common/XTPResourceManager.h"
#include "Common/XTPSystemHelpers.h"

#include "../Resource.h"
#include "XTPBrowseDialog.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPBrowseDialog::CXTPBrowseDialog(CWnd* pParent/*= NULL*/)
	: CCommonDialog(pParent)
{
	// Initialize member data to 0.
	::ZeroMemory(&m_bi, sizeof(BROWSEINFO));
	::ZeroMemory(&m_szSelPath, sizeof(m_szSelPath));

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strTitle, XTP_IDS_SELDIR));

	// Setup some defaults for the BROWSEINFO base members.
	SetOwner(pParent ? pParent->m_hWnd : NULL);
	SetPidlRoot(NULL);
	SetDisplayName(m_szSelPath);
	SetTitle((TCHAR*)(LPCTSTR)m_strTitle);
	SetOptions(BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE/*BIF_STATUSTEXT*/);
	SetCallback(BrowseCtrlCallback);
	SetData((LPARAM)this);
}

CXTPBrowseDialog::~CXTPBrowseDialog()
{

}

int CALLBACK CXTPBrowseDialog::BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	szDir[0] = 0;

	switch (uMsg)
	{
	// Set the selected directory at startup.
	case BFFM_INITIALIZED:
		{
			// use previously selected path if availiable.
			CXTPBrowseDialog* pBrowseDlg = (CXTPBrowseDialog*)lpData;
			if (pBrowseDlg && _tcslen(pBrowseDlg->GetSelPath()) != 0)
			{
				STRCPY_S(szDir, MAX_PATH, pBrowseDlg->GetSelPath());
			}

			// if no path was availiable, try the current directory.
			if (_tcslen(szDir) == 0)
			{
				::GetCurrentDirectory(_countof(szDir), szDir);
			}

			// set the directory path only if szDir is defined.
			if (_tcslen(szDir) != 0)
			{
				// wParam is TRUE since you are passing a path,
				// it would be FALSE if you were passing a pidl.
				::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
			}
		}
		break;

	// Set the status window to the currently selected path.
	case BFFM_SELCHANGED:
		{
			CXTPBrowseDialog* pBrowseDlg = (CXTPBrowseDialog*)lpData;
			if (pBrowseDlg && (pBrowseDlg->m_bi.ulFlags & BIF_STATUSTEXT) != 0)
			{
				if (::SHGetPathFromIDList((LPITEMIDLIST) lParam , szDir))
				{
					::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)szDir);
				}
			}
		}
		break;

	case BFFM_VALIDATEFAILED:
		break;

	default:
		break;
	}
	return 0;
}

INT_PTR CXTPBrowseDialog::DoModal()
{
	INT_PTR uReturn = IDCANCEL;
	LPMALLOC pMalloc = NULL;

#ifdef _XTP_ACTIVEX
	HWND hWndParent = 0;
	BOOL bEnableParent = FALSE;
	CWinApp* pApp = AfxGetApp();

	if (!XTPSystemVersion()->IsWinVistaOrGreater()) // Was not modal in old OS
	{
		if (pApp != NULL)
			pApp->EnableModeless(FALSE);

		HWND hWndTop = 0;
#if (_MSC_VER <= 1100)
		CWnd* pParentWnd = CWnd::GetSafeOwner(CWnd::FromHandle(m_bi.hwndOwner), &hWndTop);
		hWndParent = pParentWnd->GetSafeHwnd();
#else
		hWndParent = CWnd::GetSafeOwner_(m_bi.hwndOwner, &hWndTop);
#endif

		if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
		{
			::EnableWindow(hWndParent, FALSE);
			bEnableParent = TRUE;
		}
	}

#endif

	// Retrieve a pointer to the shell's IMalloc interface.
	if (::SHGetMalloc(&pMalloc) == NOERROR)
	{
		LPITEMIDLIST pItemIDList = SHBrowseForFolder(&m_bi);

		// Display the browse dialog box that enables the user to
		// select a shell folder.
		if (pItemIDList != NULL)
		{
			TCHAR szSelPath[MAX_PATH];

			// Converts an item identifier list to a file system path,
			// this will be the path that the user has just selected.
			if (::SHGetPathFromIDList(pItemIDList, szSelPath))
			{
				SetSelPath(szSelPath);
				uReturn = IDOK;
			}
			else if ((m_bi.ulFlags & BIF_BROWSEFORCOMPUTER) && m_szSelPath[0] != 0)
			{
				// a machine is selected. Although a valid pidl
				// is returned, SHGetPathFromIDList only return
				// paths to valid file system objects, of which
				// a networked machine is not. However, the
				// BROWSEINFO displayname member does contain
				// the selected item, which we return

				int nLength = (int)_tcslen(m_szSelPath);
				for (int i = nLength - 1; i >= 0; i--)
					m_szSelPath[i + 2] = m_szSelPath[i];

				m_szSelPath[0] = m_szSelPath[1] = '\\';
				m_szSelPath[nLength + 2] = 0;

				uReturn = IDOK;
			}
			pMalloc->Free(pItemIDList);
		}
		pMalloc->Release();
	}

#ifdef _XTP_ACTIVEX
	if (!XTPSystemVersion()->IsWinVistaOrGreater()) // Was not modal in old OS
	{

		if (bEnableParent)
			::EnableWindow(hWndParent, TRUE);
		if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
			::SetActiveWindow(hWndParent);

		if (pApp != NULL)
			pApp->EnableModeless(TRUE);
	}
#endif


	return uReturn;
}

void CXTPBrowseDialog::SetSelPath(LPCTSTR szSelPath)
{
	STRCPY_S(m_szSelPath, MAX_PATH, szSelPath);
}
