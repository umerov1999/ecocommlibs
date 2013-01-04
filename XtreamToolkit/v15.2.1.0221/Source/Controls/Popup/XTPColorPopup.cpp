// XTPColorPopup.cpp : implementation file
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include "../Util/XTPControlTheme.h"

#include "../Defines.h"
#include "XTPColorSelectorCtrl.h"
#include "XTPColorPopup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPColorPopup

CXTPColorPopup::CXTPColorPopup(BOOL bAutoDelete/*= FALSE*/, LPARAM callerParam /*= 0*/)
: m_callerParam(callerParam)
{
	m_bAutoDelete = bAutoDelete;
	m_bDisplayShadow = TRUE;
	m_bEndSelection = FALSE;

	m_pShadowManager = new CXTPShadowManager();
}

CXTPColorPopup::~CXTPColorPopup()
{
	CMDTARGET_RELEASE(m_pShadowManager);
}

BEGIN_MESSAGE_MAP(CXTPColorPopup, CXTPColorSelectorCtrl)
	//{{AFX_MSG_MAP(CXTPColorPopup)
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPColorPopup message handlers

BOOL CXTPColorPopup::Create(CRect& rect, CWnd* pParentWnd, DWORD dwPopup, COLORREF clrColor,
	COLORREF clrDefault/*= CLR_DEFAULT*/)
{
	if (m_pTheme->GetThemeStyle() != xtpControlThemeDefault)
	{
		SetBorders(4, 4, 6, 6);
	}
	else
	{
		SetBorders(4, 4, 10, 10);
	}

	m_rcExclude = rect;

	if (!CXTPColorSelectorCtrl::Create(rect, pParentWnd, dwPopup | WS_POPUP, clrColor, clrDefault))
		return FALSE;

	SetFont(pParentWnd->GetFont());


	return TRUE;
}

void CXTPColorPopup::PostNcDestroy()
{
	CXTPMouseMonitor::SetupHook(NULL);

	if (m_bAutoDelete)
	{
		delete this;
	}
}

void CXTPColorPopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	// If we lose activation (other than to the color dialog) post
	// a close message...

	if (nState == WA_INACTIVE && !IsColorDlgVisible())
	{
		PostMessage(WM_CLOSE);
	}
}

void CXTPColorPopup::OnDestroy()
{
	CWnd::OnDestroy();

	// Inform the parent that we are closing up.
	m_pParentWnd->SendMessage(CPN_XTP_CLOSEUP,
		(WPARAM)m_clrColor, m_callerParam);

}


int CXTPColorPopup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPColorSelectorCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bOfficeTheme = GetTheme()->m_bFlatTheme;

	if (bOfficeTheme)
	{
		ModifyStyle(WS_DLGFRAME | WS_THICKFRAME | WS_BORDER, 0, SWP_FRAMECHANGED);
		ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME, 0, SWP_FRAMECHANGED);
	}
	else
	{
		ModifyStyle(0, WS_DLGFRAME, SWP_FRAMECHANGED);
	}

	CRect rcWork = XTPMultiMonitor()->GetWorkArea(m_rcWnd);

	CSize size = m_rcWnd.Size();

	// move right
	if (m_rcWnd.left < rcWork.left)
	{
		m_rcWnd.left = rcWork.left;
		m_rcWnd.right = m_rcWnd.left + size.cx;
	}

	// move left
	else if (m_rcWnd.right > rcWork.right)
	{
		m_rcWnd.right = rcWork.right;
		m_rcWnd.left = m_rcWnd.right - size.cx;
	}

	// move up
	if (m_rcWnd.bottom > rcWork.bottom)
	{
		if (m_rcExclude.IsRectEmpty())
			m_rcWnd.bottom = rcWork.top;
		else
			m_rcWnd.bottom = m_rcExclude.top;

		m_rcWnd.top = m_rcWnd.bottom - size.cy;
	}

	HWND hWnd = m_hWnd;
	SetFocus();

	if (!IsWindow(hWnd))
		return -1;

	SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, 0);
	ModifyStyle(WS_CHILD, WS_POPUP);
	SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, (LONG_PTR)m_pParentWnd->m_hWnd);

	CWnd* pTopParent = m_pParentWnd->GetParentOwner();

	SetWindowPos(pTopParent->GetExStyle() & WS_EX_TOPMOST ? &CWnd::wndTopMost : &CWnd::wndTop, m_rcWnd.left, m_rcWnd.top, m_rcWnd.Width(), m_rcWnd.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

	if (bOfficeTheme && m_bDisplayShadow)
	{
		m_pShadowManager->SetShadowOptions(xtpShadowOfficeAlpha);
		m_pShadowManager->SetShadow(this);
	}

	CXTPMouseMonitor::SetupHook(this);
	m_bEndSelection = FALSE;

	m_pParentWnd->SendMessage(CPN_XTP_DROPDOWN,
		(WPARAM)m_clrColor, m_callerParam);

	return 0;
}

// Adds a window to send color picker notifications to.
void CXTPColorPopup::AddListener(HWND hwndListener)
{
	m_listeners.Add(hwndListener);
}

void CXTPColorPopup::EndSelection(int nCurSel)
{
	m_bEndSelection = TRUE;

	CXTPMouseMonitor::SetupHook(NULL);

	OnEndSelection(nCurSel, m_callerParam);

	DestroyWindow();
}

void CXTPColorPopup::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	if (!m_pColorDlg && !m_bEndSelection)
	{
		EndSelection(-1);
	}
}

void CXTPColorPopup::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	if (!rc.PtInRect(point) && !IsColorDlgVisible())
	{
		EndSelection(-1);
		return;
	}
	CXTPColorSelectorCtrl::OnLButtonDown(nFlags, point);
}

void CXTPColorPopup::OnNcPaint()
{
	BOOL bOfficeTheme = GetTheme()->m_bFlatTheme;

	if (bOfficeTheme)
	{
		CWindowDC dc(this);
		const COLORREF clrFrame = GetTheme()->m_crPopupBorder;
		CRect rc;
		GetWindowRect(&rc);
		int cx = rc.Width(); // for right border is exclusive
		int cy = rc.Height() ; // see above
		dc.Draw3dRect(0, 0, cx, cy, clrFrame, clrFrame);

	}
	else
	{
		Default();
	}
}

void CXTPColorPopup::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	BOOL bOfficeTheme = GetTheme()->m_bFlatTheme;

	if (bOfficeTheme)
	{
		// adjust non-client area for border space
		lpncsp->rgrc[0].left += 1;
		lpncsp->rgrc[0].top += 1;
		lpncsp->rgrc[0].right += -1;
		lpncsp->rgrc[0].bottom += -1;
	}
	else
	{
		CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}
