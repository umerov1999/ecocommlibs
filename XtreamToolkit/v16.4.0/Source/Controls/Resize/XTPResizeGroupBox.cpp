// XTPResizeGroupBox.cpp : implementation file
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

#include "XTPResizeGroupBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPResizeGroupBox

CXTPResizeGroupBox::CXTPResizeGroupBox()
{
}

CXTPResizeGroupBox::~CXTPResizeGroupBox()
{
}

IMPLEMENT_DYNAMIC(CXTPResizeGroupBox, CButton)

BEGIN_MESSAGE_MAP(CXTPResizeGroupBox, CButton)
	//{{AFX_MSG_MAP(CXTPResizeGroupBox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPResizeGroupBox message handlers

void CXTPResizeGroupBox::Exclude(CDC* pDC, CRect& rcClient)
{
	// get a pointer to the parent.
	CWnd* pWndParent = GetParent();
	if (!pWndParent)
		return;

	// get a pointer to the parents first child.
	CWnd* pWnd = pWndParent->GetWindow(GW_CHILD);
	if (pWnd == NULL)
		return;

	// iterate thru all children and exclude those children that
	// are located inside the group box.
	CWnd* pChildWnd = pWnd->GetWindow(GW_HWNDFIRST);
	while (pChildWnd != NULL)
	{
		// make sure we do not exclude ourself
		if (pChildWnd != this && pChildWnd->IsWindowVisible())
		{
			CRect rc;
			pChildWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);

			// if the parent's child is located in our group box, exclude
			// it from painting.
			if (rcClient.PtInRect(rc.TopLeft()) ||
				rcClient.PtInRect(rc.BottomRight()))
			{
				pDC->ExcludeClipRect(&rc);
			}
		}

		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

BOOL CXTPResizeGroupBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPResizeGroupBox::OnPaint()
{
	CPaintDC dc(this);

	// get the client area size.
	CRect rcClient;
	GetClientRect(&rcClient);

	// exclude controls that we "group"
	Exclude(&dc, rcClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc);

	HBRUSH hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORBTN, (WPARAM)memDC.GetSafeHdc(), (LRESULT)m_hWnd);
	if (hBrush)
	{
		::FillRect(memDC, rcClient, hBrush);
	}
	else
	{
		memDC.FillSolidRect(rcClient, GetSysColor(COLOR_3DFACE));
	}

	// Let the window do its default painting...
	CButton::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
}
