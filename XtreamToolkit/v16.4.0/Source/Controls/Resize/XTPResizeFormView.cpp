// XTPResizeFormView.cpp: implementation of the CXTPResizeFormView class.
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


#include "XTPResizeRect.h"
#include "XTPResizePoint.h"
#include "XTPResize.h"
#include "XTPResizeFormView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CXTPResizeFormView

IMPLEMENT_DYNCREATE(CXTPResizeFormView, CFormView)

BEGIN_MESSAGE_MAP(CXTPResizeFormView, CFormView)
	//{{AFX_MSG_MAP(CXTPResizeFormView)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPResizeFormView construction/destruction

CXTPResizeFormView::CXTPResizeFormView(const UINT nID, const UINT nFlags) :
	CFormView(nID),
	CXTPResize(this, nFlags | xtpResizeNoSizeIcon)
{

}

void CXTPResizeFormView::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	// Notice that WM_GETMINMAXINFO must be handled in the parent frame
	CXTPResize::GetMinMaxInfo(pMMI);
}



void CXTPResizeFormView::AdjustResizeRect(CSize& szWindow)
{
	CSize szMin = m_totalLog;
	szWindow.cx = max(szWindow.cx, szMin.cx);
	szWindow.cy = max(szWindow.cy, szMin.cy);
}


void CXTPResizeFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CXTPResize::Size();
}

void CXTPResizeFormView::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty())
	{
		SavePlacement(m_strSection);
	}

	CFormView::OnDestroy();
}

LRESULT CXTPResizeFormView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet = CFormView::WindowProc(message, wParam, lParam);

	if (message == WM_INITDIALOG)
	{
		OnInitDialog();
	}

	return lRet;
}

BOOL CXTPResizeFormView::OnInitDialog()
{
	CXTPResize::Init();

	if (!UpdateData(FALSE))
	{
		TRACE0("UpdateData failed during CXTPResizeFormView::OnInitDialog().\n");
	}

	return TRUE;
}
