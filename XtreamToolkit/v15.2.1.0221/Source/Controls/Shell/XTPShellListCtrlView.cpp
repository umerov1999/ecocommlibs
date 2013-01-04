// XTPShellListCtrl.cpp : implementation file
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

#include "Common/XTPVC80Helpers.h"
#include "../Util/XTPControlTheme.h"

#include "../Defines.h"
#include "../Header/XTPHeaderCtrl.h"
#include "../List/XTPListBase.h"
#include "XTPShellPidl.h"
#include "XTPShellSettings.h"
#include "XTPShellListBase.h"
#include "XTPShellListCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPShellListView

CXTPShellListView::CXTPShellListView()
{
	ImplAttach(&GetListCtrl());
}

CXTPShellListView::~CXTPShellListView()
{
}

IMPLEMENT_DYNCREATE(CXTPShellListView, CListView)

BEGIN_MESSAGE_MAP(CXTPShellListView, CListView)
	ON_SHELLLIST_REFLECT()
	ON_MESSAGE(WM_XTP_SHELL_NOTIFY, OnUpdateShell)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPShellListView message handlers

void CXTPShellListView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
	{
		int iIndex = GetListCtrl().GetNextItem(-1, LVNI_FOCUSED);

		if (iIndex >= 0)
		{
			CDocument* pDoc = GetDocument();
			pDoc->UpdateAllViews(this, SHN_XTP_SELECTCHILD,
				(CObject*)GetListCtrl().GetItemData(iIndex));
		}
	}

	CXTPShellListViewBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPShellListView::OnDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	int iIndex = GetDoubleClickedItem();
	if (iIndex >= 0)
	{
		CDocument* pDoc = GetDocument();
		pDoc->UpdateAllViews(this, SHN_XTP_SELECTCHILD,
			(CObject*)GetListCtrl().GetItemData(iIndex));
	}
	*pResult = 0;
}

BOOL CXTPShellListView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CXTPShellListViewBase::PreCreateWindow(cs))
		return FALSE;

	cs.style |= LVS_REPORT | LVS_SHAREIMAGELISTS;

	return TRUE;
}

LRESULT CXTPShellListView::OnUpdateShell(WPARAM wParam, LPARAM lParam)
{
	UpdateList((int)wParam, (XTP_TVITEMDATA*)lParam);
	return 0;
}

void CXTPShellListView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	UpdateList((int)lHint, (XTP_TVITEMDATA*)pHint);
}


/////////////////////////////////////////////////////////////////////////////
// CXTPShellListCtrl

CXTPShellListCtrl::CXTPShellListCtrl()
{
	ImplAttach(this);
	m_bInitControl = true;
	m_pTreeCtrl = NULL;

	m_nNameColumnWidth = 200;
}

CXTPShellListCtrl::~CXTPShellListCtrl()
{
}

BEGIN_MESSAGE_MAP(CXTPShellListCtrl, CListCtrl)
	ON_SHELLLIST_REFLECT()
	ON_MESSAGE(WM_XTP_SHELL_NOTIFY, OnUpdateShell)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPShellListCtrl message handlers


void CXTPShellListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
	{
		int iIndex = GetNextItem(-1, LVNI_FOCUSED);

		if (iIndex >= 0 && m_pTreeCtrl != NULL)
		{
			m_pTreeCtrl->SendMessage(WM_XTP_SHELL_NOTIFY,
				(WPARAM)SHN_XTP_SELECTCHILD, (LPARAM)(XTP_LVITEMDATA*)GetItemData(iIndex));
		}
	}

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPShellListCtrl::OnDblclk(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	int iIndex = GetDoubleClickedItem();
	if (iIndex >= 0 && m_pTreeCtrl != NULL)
	{
		m_pTreeCtrl->SendMessage(WM_XTP_SHELL_NOTIFY,
			(WPARAM)SHN_XTP_SELECTCHILD, (LPARAM)(XTP_LVITEMDATA*)GetItemData(iIndex));
	}
	*pResult = 0;
}


LRESULT CXTPShellListCtrl::OnUpdateShell(WPARAM wParam, LPARAM lParam)
{
	UpdateList((int)wParam, (XTP_TVITEMDATA*)lParam);
	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CXTPShellListCtrlEx

CXTPShellListCtrlEx::CXTPShellListCtrlEx()
: m_pSyncWnd(0)
{
}

CXTPShellListCtrlEx::~CXTPShellListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CXTPShellListCtrlEx, CXTPShellListCtrl)
	//{{AFX_MSG_MAP(CXTPShellListCtrlEx)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPShellListCtrlEx message handlers

bool CXTPShellListCtrlEx::BrowseToFolder(LPCTSTR lpszPath)
{
	XTP_TVITEMDATA lpTVID;

	LPITEMIDLIST  pidl;
	LPSHELLFOLDER pDesktopFolder;
	OLECHAR       szOleChar[MAX_PATH];
	ULONG         chEaten;
	ULONG         dwAttributes;
	HRESULT       hr;

	// Get a pointer to the Desktop's IShellFolder interface.
	if (SUCCEEDED(::SHGetDesktopFolder(&pDesktopFolder)))
	{
		// IShellFolder::ParseDisplayName requires the file name be in
		// Unicode.

#if !defined(_UNICODE)
		::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszPath, -1,
			szOleChar, MAX_PATH);
#else
		STRCPY_S(szOleChar, MAX_PATH, lpszPath);
#endif

		// Convert the path to an ITEMIDLIST.
		hr = pDesktopFolder->ParseDisplayName(NULL, NULL, szOleChar,
			&chEaten, &pidl, &dwAttributes);

		if (SUCCEEDED(hr))
		{
			IShellFolder *psfMyFolder;

			lpTVID.lpi = lpTVID.lpifq = pidl;

			pDesktopFolder->BindToObject(lpTVID.lpifq, NULL,
				IID_IShellFolder, (LPVOID*)&psfMyFolder);

			lpTVID.lpsfParent = psfMyFolder;
			PopulateListView(&lpTVID, lpTVID.lpsfParent);

			m_strItemPath = lpszPath;
			pDesktopFolder->Release();

			return true;
		}

		pDesktopFolder->Release();
	}

	return false;
}

void CXTPShellListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CXTPShellListCtrl::OnLButtonDblClk(nFlags, point);

	int iItem = GetDoubleClickedItem();
	if (iItem >= 0)
	{
		XTP_LVITEMDATA* lplvid = (XTP_LVITEMDATA*)GetItemData(iItem);
		if (lplvid)
		{
			ULONG ulAttrs = GetAttributes(lplvid->lpsfParent,
				lplvid->lpi, SHCIDS_ALLFIELDS);

			if ((ulAttrs & SFGAO_FOLDER) == SFGAO_FOLDER)
			{
				if (m_strItemPath.GetAt(m_strItemPath.GetLength() -1) != _T('\\'))
				{
					m_strItemPath += _T('\\');
				}

				m_strItemPath += GetItemText(iItem, 0);

				BrowseToFolder(m_strItemPath);

				if (m_pSyncWnd != 0)
				{
					m_pSyncWnd->SetWindowText(m_strItemPath);
				}
			}
			else
			{
				ShellOpenItem(lplvid);
			}
		}
	}
}
