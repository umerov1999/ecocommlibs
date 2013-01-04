// XTPMiniToolBar.cpp : implementation of the CXTPMiniToolBar class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#include "Common/XTPColorManager.h"

#include "XTPCommandBarsDefines.h"
#include "XTPCommandBar.h"
#include "XTPPopupBar.h"
#include "XTPPaintManager.h"
#include "XTPMouseManager.h"
#include "XTPMiniToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifndef LWA_ALPHA
#define LWA_ALPHA               0x00000002
#endif
#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMiniToolBar

IMPLEMENT_XTP_COMMANDBAR(CXTPMiniToolBar, CXTPPopupToolBar);

CXTPMiniToolBar::CXTPMiniToolBar()
{
	m_pfnSetLayeredWindowAttributes = NULL;
	m_pfnUpdateLayeredWindow = NULL;

	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (hLib)
	{
		m_pfnSetLayeredWindowAttributes = (PFNSETLAYEREDWINDOWATTRIBUTES) ::GetProcAddress(hLib, "SetLayeredWindowAttributes");
		m_pfnUpdateLayeredWindow = (LPFNUPDATELAYEREDWINDOW) GetProcAddress(hLib, "UpdateLayeredWindow");
	}

	m_bActivated = FALSE;
	m_bTracking = FALSE;
	m_nOpacity = 0;
	m_pContextMenu = FALSE;
	m_bTrackOnHover = FALSE;

	m_rcBorders = CRect(2, 2, 2, 2);
	m_szButtonSpacing = CSize(0, 3);
}

CXTPMiniToolBar::~CXTPMiniToolBar()
{

}

void CXTPMiniToolBar::UpdateOpacity()
{
	CXTPWindowRect rc(this);
	CPoint pt;
	GetCursorPos(&pt);

	int x = pt.x < rc.left ? rc.left - pt.x : pt.x > rc.right ? pt.x - rc.right : 0;
	int y = pt.y < rc.top ? rc.top - pt.y : pt.y > rc.bottom ? pt.y - rc.bottom : 0;

	int nLen = max(x, y);
	int nOpacity = 0;

	if (m_nPopuped != -1 || m_pContextMenu || IsTrackingMode() || nLen == 0)
	{
		nLen = 0;
		nOpacity = 255;
	}
	else if (!m_bActivated)
	{
		if (nLen < 17) nOpacity = 17 + (16 - nLen) * 14;
	}
	else
	{
		if (nLen < 90) nOpacity = MulDiv(255, 90 - nLen, 90);
	}

	if ((m_nOpacity != nOpacity))
	{
		m_nOpacity = nOpacity;

		if (m_pfnSetLayeredWindowAttributes)
		{
			m_pfnSetLayeredWindowAttributes(m_hWnd, 0x00, (BYTE)nOpacity, LWA_ALPHA);
		}
		else
		{
			SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | (nOpacity > 20 ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
		}

		CXTPShadowManager* pShadowManager = GetPaintManager()->GetShadowManager();

		POSITION pos = pShadowManager->GetHeadPosition(this);
		while (pos)
		{
			CWnd* pShadow = pShadowManager->GetNext(pos);

			if (m_pfnUpdateLayeredWindow)
			{
				BLENDFUNCTION bf;
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = (BYTE)nOpacity;
				bf.AlphaFormat = 0x01;
				m_pfnUpdateLayeredWindow(pShadow->GetSafeHwnd(), (HDC)0, 0, 0, 0, 0, 0, &bf, 0x02);
			}
			else
			{
				pShadow->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | (nOpacity > 20 ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
			}
		}
	}

	if (m_nOpacity == 255)
	{
		m_bActivated = TRUE;
	}

	if (nLen > (m_bActivated ? 180 : 110))
	{
		m_bTracking = FALSE;
	}
}

void CXTPMiniToolBar::Animate()
{
	GetPaintManager()->SetCommandBarRegion(this);

	if (m_pfnSetLayeredWindowAttributes != 0)
	{
		ModifyStyleEx(0, WS_EX_LAYERED);
	}
}

BOOL CXTPMiniToolBar::TrackMiniBar(UINT nFlags, int x, int y)
{
	CXTPMouseManager* pMouseManager = XTPMouseManager();

	m_bActivated = FALSE;
	m_bTracking = TRUE;

	m_popupFlags = xtpPopupUp;
	m_nOpacity = 0;

	CWnd* pWnd = GetSite();

	if (pWnd && pWnd->GetExStyle() & (WS_EX_LAYOUTRTL | WS_EX_RIGHT))
		nFlags |= TPM_RIGHTALIGN;

	m_bExecOnRButton = nFlags & TPM_RIGHTBUTTON;
	m_popupFlags = (nFlags & TPM_RIGHTALIGN) ? xtpPopupLeft | xtpPopupUp : xtpPopupUp;

	pMouseManager->SendTrackLost();
	ReleaseCapture();

	if (!Popup(x, y, CRect(0, 0, 0, 0)))
	{
		return FALSE;
	}

	InternalAddRef();

	SetTrackingMode(FALSE);

	UpdateOpacity();

	PumpMessage();

	DestroyWindow();

	InternalRelease();

	return TRUE;
}

BOOL CXTPMiniToolBar::TrackPopupMenu(CXTPPopupBar* pPopup, UINT nFlags, int x, int y)
{
	ASSERT(pPopup);
	if (!pPopup)
	{
		return FALSE;
	}

	m_bActivated = TRUE;
	m_bTracking = TRUE;
	m_nOpacity = 0;
	m_popupFlags = xtpPopupUp;

	CWnd* pWnd = GetSite();

	if (pWnd && pWnd->GetExStyle() & (WS_EX_LAYOUTRTL | WS_EX_RIGHT))
		nFlags |= TPM_RIGHTALIGN;

	UINT nReturn = TRUE;
	pPopup->m_pReturnCmd = NULL;

	if (nFlags & TPM_RETURNCMD)
	{
		pPopup->m_pReturnCmd = &nReturn;
		nReturn = 0;
	}

	m_bExecOnRButton = nFlags & TPM_RIGHTBUTTON;

	pPopup->m_bIgnoreUpdateHandler = nFlags & TPM_NONOTIFY;
	pPopup->m_bExecOnRButton = nFlags & TPM_RIGHTBUTTON;

	if (nFlags & TPM_RIGHTALIGN)
	{
		SetPopupFlags(xtpPopupLeft | xtpPopupUp);
		pPopup->SetPopupFlags(xtpPopupLeft | xtpPopupDown);
	}
	else
	{
		SetPopupFlags(xtpPopupUp);
		pPopup->SetPopupFlags(xtpPopupDown);
	}

	CXTPMouseManager* pMouseManager = XTPMouseManager();

	pMouseManager->SendTrackLost();
	ReleaseCapture();

	pMouseManager->IgnoreLButtonUp();

	if (!Create())
		return FALSE;

	if (!pPopup->Create())
		return FALSE;

	int nPopupWidth = pPopup->GetWidth();
	int nMiniBarWidth = GetWidth();

	CSize szMiniBar = CalcDynamicLayout(0, 0);
	CSize szPopupBar = pPopup->CalcDynamicLayout(0, 0);

	CPoint ptPopup(x, y);
	CRect rc = pPopup->CalculatePopupRect(ptPopup, szPopupBar);

	int nWidth = max(szMiniBar.cx, szPopupBar.cx);
	int nTop = rc.top - 15;

	pPopup->SetWidth(nWidth);
	SetWidth(nWidth);

	CRect rcWorkArea = XTPMultiMonitor()->GetWorkArea(ptPopup);

	if (nTop < rcWorkArea.top)
	{
		nTop = rc.bottom + 15;
		m_popupFlags &= ~xtpPopupUp;
	}

	if (!Popup(x, nTop, CRect(0, 0, 0, 0)))
	{
		return FALSE;
	}

	m_pContextMenu = pPopup;

	UpdateOpacity();
	UpdateWindow();


	if (!pPopup->Popup(x, y, CRect(0, 0, 0, 0)))
	{
		m_pContextMenu = NULL;
		return FALSE;
	}

	InternalAddRef();

	PumpMessage();

	SetWidth(nMiniBarWidth);
	pPopup->SetWidth(nPopupWidth);

	m_pContextMenu = NULL;

	DestroyWindow();
	InternalRelease();


	return nReturn;
}

BOOL CXTPMiniToolBar::OnHookKeyDown(UINT nChar, LPARAM lParam)
{
	if (m_pContextMenu)
		return FALSE;

	return CXTPPopupToolBar::OnHookKeyDown(nChar, lParam);
}

BOOL CXTPMiniToolBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	return CXTPCommandBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard);
}

BOOL CXTPMiniToolBar::CursorInWindow() const
{
	CPoint pt;
	GetCursorPos(&pt);

	if (CXTPWindowRect(this).PtInRect(pt))
		return TRUE;

	CXTPCommandBar* pCommandBar = XTPMouseManager()->HitTest(pt);
	if (!pCommandBar)
		return FALSE;

	return pCommandBar->GetRootParent() == (CXTPCommandBar*)this;
}

int CXTPMiniToolBar::OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult)
{
	if (m_bTracking)
	{
		if (nMessage == WM_ENABLE || nMessage == WM_ACTIVATEAPP)
		{
			ShowWindow(SW_HIDE);
			m_bTracking = FALSE;
		}
	}

	return CXTPPopupToolBar::OnHookMessage(hWnd, nMessage, wParam, lParam, lResult);
}

void CXTPMiniToolBar::PumpMessage()
{
	HWND hwndSite = GetSite()->m_hWnd;
	CWinThread* pThread = XTPGetThread();

	XTPHookManager()->SetHook(hwndSite, this);

	HWND hWndTopLevelParent = hwndSite;
	while (GetWindowLong(hWndTopLevelParent, GWL_STYLE) & WS_CHILD)
		hWndTopLevelParent = ::GetParent(hWndTopLevelParent);

	if (hwndSite != hWndTopLevelParent)
		XTPHookManager()->SetHook(hWndTopLevelParent, this);

	while (m_bTracking)
	{

		MSG msg;
		if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!m_bTracking)
				break;

			if (!m_pContextMenu)
			{
				if (msg.message == WM_MOUSEMOVE)
				{
					UpdateOpacity();
				}

				if ((msg.message == WM_KEYDOWN || msg.message == WM_CHAR
					|| msg.message == WM_SYSKEYDOWN || msg.message == WM_MOUSEWHEEL) && !IsTrackingMode())
				{
					m_bTracking = FALSE;
				}

				if (msg.message == WM_CLOSE || msg.message == WM_SYSCOMMAND)
				{
					m_bTracking = FALSE;
				}

				if ((msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN || msg.message == WM_MBUTTONDOWN ||
					msg.message == WM_NCLBUTTONDOWN || msg.message == WM_NCRBUTTONDOWN) && !CursorInWindow())
				{
					m_bTracking = FALSE;
				}
			}
			else
			{
				if (msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN || msg.message == WM_MBUTTONDOWN ||
					msg.message == WM_NCLBUTTONDOWN || msg.message == WM_NCRBUTTONDOWN)
				{
					if (!CursorInWindow())
					{
						CPoint point;
						GetCursorPos(&point);

						if (XTPMouseManager()->HitTest(point) == 0)
							m_bTracking = FALSE;
					}
					else
					{
						m_pContextMenu->SetTrackingMode(FALSE);
						m_pContextMenu = NULL;
						SetTrackingMode(FALSE);
					}
				}
			}

			if (msg.message == WM_ACTIVATE || msg.message == WM_ACTIVATEAPP || msg.message == WM_CAPTURECHANGED)
			{
				m_bTracking = FALSE;
			}

			if (m_pContextMenu && !m_pContextMenu->IsTrackingMode())
			{
				ShowWindow(SW_HIDE);
				m_bTracking = FALSE;
			}

			if (!m_bTracking)
				break;

			if (!::GetMessage(&msg, NULL, 0, 0))
				break;

			if (m_pContextMenu && !m_pContextMenu->IsTrackingMode())
			{
				m_bTracking = FALSE;
			}

			if (!m_bTracking)
			{
				::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				break;
			}

			if (msg.message != WM_KICKIDLE && !pThread->PreTranslateMessage(&msg))
			{
				// process this message
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			pThread->OnIdle(-1);
			OnIdleUpdateCmdUI(TRUE, 0L);

		}
		else
		{
			WaitMessage();
		}
	}

	if (hwndSite != hWndTopLevelParent)
		XTPHookManager()->RemoveHook(hWndTopLevelParent, this);

	XTPHookManager()->RemoveHook(hwndSite, this);
}

CXTPMiniToolBar* CXTPMiniToolBar::CreateMiniToolBar(CXTPCommandBars* pCommandBars)
{
	CXTPMiniToolBar* pMiniToolBar = new CXTPMiniToolBar();
	pMiniToolBar->SetCommandBars(pCommandBars);
	return pMiniToolBar;
}

INT_PTR CXTPMiniToolBar::OnToolHitTest(CPoint /*point*/, TOOLINFO* /*pTI*/) const
{
	return -1; // No Tips.
}

BEGIN_MESSAGE_MAP(CXTPMiniToolBar, CXTPPopupToolBar)
	//{{AFX_MSG_MAP(CXTPMiniToolBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPMiniToolBar message handlers


