// XTPShellTreeCtrl.cpp : implementation file
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

#include "Common/XTPSystemHelpers.h"

#include "../Defines.h"
#include "XTPShellPidl.h"
#include "XTPShellSettings.h"
#include "../Tree/XTPTreeBase.h"
#include "XTPShellTreeBase.h"
#include "XTPShellTreeCtrlView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPShellTreeView

CXTPShellTreeView::CXTPShellTreeView()
{
	m_pTreeCtrl = (CTreeCtrl_Friendly*)&GetTreeCtrl();
}

CXTPShellTreeView::~CXTPShellTreeView()
{

}

IMPLEMENT_DYNCREATE(CXTPShellTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CXTPShellTreeView, CTreeView)
	ON_SHELLTREE_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPShellTreeView message handlers

void CXTPShellTreeView::SelectionChanged(HTREEITEM hItem, CString strFolderPath)
{
	if (hItem != NULL && !m_bTunneling)
	{
		CDocument* pDoc = GetDocument();
		if (pDoc != NULL)
		{
			pDoc->UpdateAllViews(this, SHN_XTP_TREESELCHANGE,
				(CObject*)GetTreeCtrl().GetItemData(hItem));
		}
	}

	CXTPShellTreeBase::SelectionChanged(hItem, strFolderPath);

}

void CXTPShellTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// mimic native Windows Explorer styles.
	DWORD dwStyle = TVS_SHOWSELALWAYS | TVS_HASBUTTONS;

	if (XTPSystemVersion()->IsWinXPOrGreater())
	{
		dwStyle |= (TVS_TRACKSELECT);
	}
	else
	{
		dwStyle |= (TVS_HASLINES | TVS_LINESATROOT);
	}

	if (InitializeTree(dwStyle))
	{
		PopulateTreeView();
	}

	// make the parent of the selected item visible if found.
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM hItemParent = GetTreeCtrl().GetParentItem(hItem);

	GetTreeCtrl().EnsureVisible(hItemParent ? hItemParent : hItem);
}

void CXTPShellTreeView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	// Update selection of tree view to the
	case SHN_XTP_SELECTCHILD:
		{
			XTP_LVITEMDATA*  lplvid = (XTP_LVITEMDATA*)pHint;
			ASSERT(lplvid != NULL);

			if (!FindTreeItem(GetTreeCtrl().GetSelectedItem(), lplvid, FALSE))
			{
				// The folder was not found so we send back a message
				// to the listview to execute the itemid
				CDocument* pDoc = GetDocument();
				pDoc->UpdateAllViews(this, SHN_XTP_NOFOLDER, (CObject*)lplvid);
			}
		}
		break;

	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPShellTreeCtrl

IMPLEMENT_DYNAMIC(CXTPShellTreeCtrl, CTreeCtrl)

CXTPShellTreeCtrl::CXTPShellTreeCtrl()
{
	m_pListCtrl = NULL;
	m_bAutoInit = TRUE;
	m_pTreeCtrl = (CTreeCtrl_Friendly*)this;
}

CXTPShellTreeCtrl::~CXTPShellTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CXTPShellTreeCtrl, CTreeCtrl)
	ON_SHELLTREE_REFLECT()
	ON_MESSAGE(WM_XTP_SHELL_NOTIFY, OnUpdateShell)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPShellTreeCtrl message handlers

void CXTPShellTreeCtrl::SelectionChanged(HTREEITEM hItem, CString strFolderPath)
{
	if (hItem != NULL && !m_bTunneling)
	{
		CWnd* pWndNotify = (CWnd*)m_pListCtrl;
		if (!::IsWindow(pWndNotify->GetSafeHwnd()))
		{
			pWndNotify = GetOwner();
		}

		// notify list control of selection change.
		if (::IsWindow(pWndNotify->GetSafeHwnd()))
		{
			pWndNotify->SendMessage(WM_XTP_SHELL_NOTIFY,
				(WPARAM)SHN_XTP_TREESELCHANGE,
				(LPARAM)(CObject*)GetItemData(hItem));
		}
	}
	CXTPShellTreeBase::SelectionChanged(hItem, strFolderPath);
}

LRESULT CXTPShellTreeCtrl::OnUpdateShell(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	// Update selection of tree view to the
	case SHN_XTP_SELECTCHILD:
		{
			XTP_LVITEMDATA*  lplvid = (XTP_LVITEMDATA*)lParam;
			ASSERT(lplvid != NULL);

			if (!FindTreeItem(GetSelectedItem(), lplvid, FALSE) && m_pListCtrl != NULL)
			{
				// The folder was not found so we send back a message
				// to the listview to execute the itemid
				m_pListCtrl->SendMessage(WM_XTP_SHELL_NOTIFY,
					(WPARAM)SHN_XTP_NOFOLDER,
					(LPARAM)(CObject*)lplvid);
			}
		}
		break;

	default:
		break;
	}

	return 0;
}

bool CXTPShellTreeCtrl::Init()
{
	if (!CXTPShellTreeCtrlBase::Init())
		return false;

	if (m_bAutoInit)
	{
		// mimic native Windows Explorer styles.
		DWORD dwStyle = TVS_SHOWSELALWAYS | TVS_HASBUTTONS;

		if (XTPSystemVersion()->IsWinXPOrGreater())
		{
			dwStyle |= (TVS_TRACKSELECT);
		}
		else
		{
			dwStyle |= (TVS_HASLINES | TVS_LINESATROOT);
		}

		if (InitializeTree(dwStyle))
		{
			PopulateTreeView();
		}

		// make the parent of the selected item visible if found.
		HTREEITEM hItem = GetSelectedItem();
		HTREEITEM hItemParent = GetParentItem(hItem);

		EnsureVisible(hItemParent ? hItemParent : hItem);
	}

	return true;
}

void CXTPShellTreeCtrl::AssociateList(CWnd* pWnd)
{
	ASSERT_VALID(pWnd); // must be a valid window.

	if (::IsWindow(pWnd->GetSafeHwnd()))
	{
		m_pListCtrl = pWnd;

		// make sure list gets updated.
		HTREEITEM hItem = GetSelectedItem();
		if (hItem)
		{
			XTP_TVITEMDATA* pItemData = (XTP_TVITEMDATA*)GetItemData(hItem);
			if (pItemData)
			{
				m_pListCtrl->SendMessage(WM_XTP_SHELL_NOTIFY,
					(WPARAM)SHN_XTP_TREESELCHANGE, (LPARAM)pItemData);
			}
		}
	}
}

void CXTPShellTreeCtrl::AssociateCombo(CWnd* pWnd)
{
	CXTPShellTreeBase::AssociateCombo(pWnd);

	if (::IsWindow(pWnd->GetSafeHwnd()))
	{
		CString strSelectedPath;
		GetSelectedFolderPath(strSelectedPath);

		// make sure list gets updated.
		CXTPShellTreeBase::SelectionChanged(
			GetSelectedItem(), strSelectedPath);
	}
}
