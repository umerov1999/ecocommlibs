// XTPShortcutBarPopup.cpp : implementation of the CXTPShortcutBarPopup class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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
#include "Resource.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"

#include "XTPShortcutBarPaintManager.h"
#include "XTPShortcutBarPopup.h"
#include "XTPShortcutBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  FRAME_GAP 5

int CXTPShortcutBarPopup::m_nPopupWidth = 250;


CXTPShortcutBarPopup::CXTPShortcutBarPopup(CXTPShortcutBar* pShortcutBar, HWND hWndChild)
{
	m_pShortcutBar = pShortcutBar;
	m_hWndChild = hWndChild;
};

CXTPShortcutBarPopup::~CXTPShortcutBarPopup()
{

}

BEGIN_MESSAGE_MAP(CXTPShortcutBarPopup, CWnd)
	ON_WM_PAINT()

	ON_WM_MOUSEACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_NCCREATE()
	ON_MESSAGE(WM_FLOATSTATUS, OnFloatStatus)
	ON_WM_NCHITTEST_EX()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CXTPShortcutBarPopup::Close()
{
	if (m_hWnd)
	{
		ShowWindow(SW_HIDE);
	}

	if (m_hWndChild)
	{
		::SetParent(m_hWndChild, m_pShortcutBar->GetSafeHwnd());
		::ShowWindow(m_hWndChild, SW_HIDE);
		m_hWndChild = NULL;
	}
}

void CXTPShortcutBarPopup::DoModal()
{
	int nClassStyle = 0;
	if (XTPSystemVersion()->IsWinXPOrGreater())
		nClassStyle |= 0x00020000;

	CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(nClassStyle, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), 0,
		MFS_SYNCACTIVE | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), m_pShortcutBar, 0);


	CXTPWindowRect rcShortcutBar(m_pShortcutBar);

	CRect rc(rcShortcutBar.right, rcShortcutBar.top, rcShortcutBar.right + m_nPopupWidth + FRAME_GAP + FRAME_GAP, rcShortcutBar.bottom);

	SetWindowPos(0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

	::SetParent(m_hWndChild, m_hWnd);
	::SetWindowPos(m_hWndChild, 0, FRAME_GAP, FRAME_GAP, rc.Width() - FRAME_GAP - FRAME_GAP, rc.Height() - FRAME_GAP - FRAME_GAP, SWP_NOZORDER | SWP_SHOWWINDOW);

	::SetFocus(m_hWndChild);

	PumpMessage();


	if (m_hWndChild)
	{
		::SetParent(m_hWndChild, m_pShortcutBar->GetSafeHwnd());
		::ShowWindow(m_hWndChild, SW_HIDE);
		m_hWndChild = NULL;
	}

	m_nPopupWidth = CXTPWindowRect(this).Width() - FRAME_GAP - FRAME_GAP;

	DestroyWindow();

}

void CXTPShortcutBarPopup::PumpMessage()
{
	CWinThread* pThread = XTPGetThread();

	while (IsTrackingMode())
	{
		MSG msg;
		if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{

			if (!::GetMessage(&msg, NULL, 0, 0))
				break;


			if (msg.message != WM_KICKIDLE && !pThread->PreTranslateMessage(&msg))
			{
				// process this message
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		else
		{
			WaitMessage();
		}
	}
}

BOOL CXTPShortcutBarPopup::IsTrackingMode() const
{
	if (!m_hWndChild)
		return FALSE;

	HWND hWnd = ::GetFocus();

	while (hWnd)
	{
		if (hWnd == m_hWndChild)
			return TRUE;

		if (::GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP)
			hWnd = ::GetWindow(hWnd, GW_OWNER);
		else
			hWnd = ::GetParent(hWnd);
	}

	return FALSE;

}


void CXTPShortcutBarPopup::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);

	CXTPShortcutBarPaintManager* pPaintManager = m_pShortcutBar->GetPaintManager();

	dc.FillSolidRect(rc, pPaintManager->m_clrPopupBackground);
	dc.Draw3dRect(rc, pPaintManager->m_clrPopupFrame, pPaintManager->m_clrPopupFrame);
	dc.Draw3dRect(FRAME_GAP - 1, FRAME_GAP - 1, rc.Width() - FRAME_GAP - FRAME_GAP + 2, rc.Height() - FRAME_GAP - FRAME_GAP + 2, pPaintManager->m_clrPopupFrame, pPaintManager->m_clrPopupFrame);
}


int CXTPShortcutBarPopup::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	return MA_NOACTIVATE;
}

BOOL CXTPShortcutBarPopup::OnNcActivate(BOOL bActive)
{
	return bActive ? FALSE : CWnd::OnNcActivate(bActive);
}

BOOL CXTPShortcutBarPopup::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CWnd::OnNcCreate(lpCreateStruct))
		return FALSE;

	if (GetStyle() & MFS_SYNCACTIVE)
	{
		// syncronize activation state with top level parent
		CWnd* pParentWnd = GetTopLevelParent();
		ASSERT(pParentWnd != NULL);
		if (!pParentWnd)
			return FALSE;

		CWnd* pActiveWnd = GetForegroundWindow();
		BOOL bActive = (pParentWnd == pActiveWnd) ||
			(pParentWnd->GetLastActivePopup() == pActiveWnd &&
			pActiveWnd->SendMessage(WM_FLOATSTATUS, FS_SYNCACTIVE) != 0);

		// the WM_FLOATSTAzTUS does the actual work
		SendMessage(WM_FLOATSTATUS, bActive ? FS_ACTIVATE : FS_DEACTIVATE);
	}


	return TRUE;
}


LRESULT CXTPShortcutBarPopup::OnFloatStatus(WPARAM wParam, LPARAM)
{
	// FS_SYNCACTIVE is used to detect MFS_SYNCACTIVE windows
	LRESULT lResult = ((GetStyle() & MFS_SYNCACTIVE) && (wParam & FS_SYNCACTIVE));
	return lResult;
}


LRESULT CXTPShortcutBarPopup::OnNcHitTest(CPoint point)
{
	CRect rectWindow;
	GetWindowRect(&rectWindow);

	CRect rcBorder(rectWindow.right - FRAME_GAP, rectWindow.top, rectWindow.right, rectWindow.bottom);
	if (rcBorder.PtInRect(point))
		return HTRIGHT;



	return HTCLIENT;
}


void CXTPShortcutBarPopup::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	::SetWindowPos(m_hWndChild, 0, FRAME_GAP, FRAME_GAP, cx - FRAME_GAP - FRAME_GAP, cy - FRAME_GAP - FRAME_GAP, SWP_NOZORDER);

	Invalidate(FALSE);
}
