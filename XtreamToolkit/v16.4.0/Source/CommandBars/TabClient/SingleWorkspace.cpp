#include "StdAfx.h"

#include <Common/Resource.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/Hook/XTPWinEventHook.h>

#include <TabManager/Includes.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlExt.h>
#include <CommandBars/XTPMouseManager.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>

#include <CommandBars/XTPTabClientWnd.h>
#include <CommandBars/TabClient/Workspace.h>
#include <CommandBars/TabClient/SingleWorkspace.h>

//////////////////////////////////////////////////////////////////////////
// CSingleWorkspace

CXTPTabClientWnd::CSingleWorkspace::~CSingleWorkspace()
{
	DestroyWindow();
}

CWnd* CXTPTabClientWnd::CSingleWorkspace::GetWindow() const
{
	return (CWnd*)this;
}

INT_PTR CXTPTabClientWnd::CSingleWorkspace::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	return PerformToolHitTest(m_hWnd, point, pTI);
}

BOOL CXTPTabClientWnd::CSingleWorkspace::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_pTabClientWnd)
	{
		m_pTabClientWnd->m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CXTPTabClientWnd::CSingleWorkspace::PreTranslateMessage(MSG* pMsg)
{
#ifndef _XTP_ACTIVEX
	if (pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_RBUTTONDOWN ||
		pMsg->message == WM_MBUTTONDOWN ||
		pMsg->message == WM_LBUTTONDBLCLK ||
		pMsg->message == WM_RBUTTONDBLCLK)
	{

		if (GetParentFrame()->SendMessage(WM_XTP_PRETRANSLATEMOUSEMSG, (WPARAM)pMsg->message, pMsg->lParam))
			return TRUE;
	}

#endif

	return CWnd::PreTranslateMessage(pMsg);
}

void CXTPTabClientWnd::CSingleWorkspace::RedrawControl(LPCRECT lpRect, BOOL /*bAnimate*/)
{
	if (!m_pTabClientWnd->m_bLockReposition)
		InvalidateRect(lpRect, FALSE);
}

BEGIN_MESSAGE_MAP(CXTPTabClientWnd::CSingleWorkspace, CWnd)
	{ WM_LBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&CXTPTabClientWnd::CSingleWorkspace::OnLButtonDblClk },
	{ WM_PAINT, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&CXTPTabClientWnd::CSingleWorkspace::OnPaint },
	{ WM_PRINTCLIENT, 0, 0, 0, AfxSig_lwl, (AFX_PMSG)(AFX_PMSGW)(LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))&CXTPTabClientWnd::CSingleWorkspace::OnPrintClient },
	{ WM_ERASEBKGND, 0, 0, 0, AfxSig_bD, (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(CDC*))&CXTPTabClientWnd::CSingleWorkspace::OnEraseBkgnd },
	{ WM_LBUTTONDOWN, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&CXTPTabClientWnd::CSingleWorkspace::OnLButtonDown },
	{ WM_MOUSEMOVE, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&CXTPTabClientWnd::CSingleWorkspace::OnMouseMove },
	{ WM_MOUSELEAVE, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&CXTPTabClientWnd::CSingleWorkspace::OnMouseLeave },
	{ WM_RBUTTONDOWN, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&CXTPTabClientWnd::CSingleWorkspace::OnRButtonDown },
END_MESSAGE_MAP()

void CXTPTabClientWnd::CSingleWorkspace::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	UNREFERENCED_PARAMETER(point);

#ifdef _XTP_ACTIVEX
	CXTPTabManagerItem* pItem = HitTest(point);
	m_pTabClientWnd->FireRClick(pItem ? pItem->GetIDispatch(FALSE) : NULL);
#endif
}

void CXTPTabClientWnd::CSingleWorkspace::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint);

	if (m_pTabClientWnd)
	{
		CRect rectClient;
		m_pTabClientWnd->GetWindowRect(rectClient);
		ScreenToClient(&rectClient);
		dcPaint.ExcludeClipRect(rectClient);
		dc.ExcludeClipRect(rectClient);

	}
	GetPaintManager()->DrawTabControl(this, &dc, rc);
}

LRESULT CXTPTabClientWnd::CSingleWorkspace::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		if (m_pTabClientWnd)
		{
			CRect rectClient;
			m_pTabClientWnd->GetWindowRect(rectClient);
			ScreenToClient(&rectClient);
			pDC->ExcludeClipRect(rectClient);

		}
		CXTPClientRect rc(this);
		GetPaintManager()->DrawTabControl(this, pDC, rc);
	}
	return TRUE;
}

BOOL CXTPTabClientWnd::CSingleWorkspace::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPTabClientWnd::CSingleWorkspace::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	m_pTabClientWnd->m_bLockUpdate = TRUE;
#ifndef _XTP_ACTIVEX
	m_pTabClientWnd->m_bDelayLock = TRUE;
#endif

	PerformClick(m_hWnd, point);

#ifdef _XTP_ACTIVEX
	m_pTabClientWnd->m_bLockUpdate = FALSE;
#endif

}

void CXTPTabClientWnd::CSingleWorkspace::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}


void CXTPTabClientWnd::CSingleWorkspace::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	PerformMouseMove(m_hWnd, point);
}

void CXTPTabClientWnd::CSingleWorkspace::OnMouseLeave()
{
	PerformMouseMove(m_hWnd, CPoint(-1, -1));
}
