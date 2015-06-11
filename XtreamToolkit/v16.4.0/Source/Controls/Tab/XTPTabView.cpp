// XTPTabView.cpp : implementation of the CXTPTabView class.
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

#include "../Util/XTPControlTheme.h"

#include "../Defines.h"
#include "../Util/XTPGlobal.h"
#include "XTPTabBase.h"
#include "XTPTabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTabView

IMPLEMENT_DYNCREATE(CXTPTabView, CCtrlView)

BEGIN_MESSAGE_MAP(CXTPTabView, CCtrlView)
	//{{AFX_MSG_MAP(CXTPTabView)
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
	ON_TABCTRLEX_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPTabView construction/destruction

CXTPTabView::CXTPTabView()
{
	m_bBoldFont = FALSE;
	ImplAttach(&GetTabCtrl());
}

CXTPTabView::~CXTPTabView()
{
}

BOOL CXTPTabView::Init()
{
	if (!CXTPTabViewBase::Init())
		return FALSE;

	// Make sure the common controls library is initialized.
	::InitCommonControls();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTabView tooltip related functions

void CXTPTabView::UpdateDocTitle()
{
	TCHAR szText[256];

	TC_ITEM tci;
	tci.mask = TCIF_TEXT;
	tci.cchTextMax = _countof(szText);
	tci.pszText = szText;

	GetTabCtrl().GetItem(GetTabCtrl().GetCurSel(), &tci);
	GetDocument()->SetTitle(tci.pszText);
}

int CXTPTabView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// If current active view is a child view, then do nothing.
	CFrameWnd* pParentFrame = GetParentFrame();
	CView* pView = pParentFrame ? pParentFrame->GetActiveView() : NULL;

	if (IsChildView(pView))
	{
		return MA_NOACTIVATE;
	}

	return CCtrlView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CXTPTabView::OnInitialUpdate()
{
	CCtrlView::OnInitialUpdate();

	// Get a pointer to the active view.
	CView* pActiveView = DYNAMIC_DOWNCAST(CView, GetActiveView());
	if (pActiveView != NULL)
	{
		// Get a pointer to the parent frame.
		CFrameWnd* pParentFrame = GetParentFrame();
		if (pParentFrame != NULL)
		{
			// If the active view for the frame does not match the active
			// view for the tab control, set the frames active view.
			if (pParentFrame->GetActiveView() != pActiveView)
			{
				pParentFrame->SetActiveView(pActiveView);
			}
		}
	}
}
