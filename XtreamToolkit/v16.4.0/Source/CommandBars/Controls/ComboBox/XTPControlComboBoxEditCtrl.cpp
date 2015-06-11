// XTPControlComboBoxEditCtrl.cpp : implementation of the CXTPControlComboBoxEditCtrl class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include "StdAfx.h"

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include "Common/XTPResourceManager.h"

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlComboBox.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPMouseManager.h>
#include <CommandBars/XTPKeyboardManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPControlComboBoxEditCtrl

BEGIN_MESSAGE_MAP(CXTPControlComboBoxEditCtrl, CXTPCommandBarEditCtrl)
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_MESSAGE(XTP_TTM_WINDOWFROMPOINT, OnWindowFromPoint)
	ON_MESSAGE_VOID(WM_XTP_SHELLAUTOCOMPLETESTART, OnShellAutoCompleteStart)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEditChanged)
	ON_MESSAGE(WM_ENABLE, OnEnableDisable)
END_MESSAGE_MAP()

// [IP] This trick (handling WM_ENABLE) allows to skip using COLOR_GRAYTEXT for disabled control background (default Win32)
// You can now handle the WM_CTLCOLOR/CTL_COLOREDIT message in the parent or the reflected (=WM_CTLCOLOR) message in the
// derived subclass as you normally would. And you can still use IsWindowEnabled should you care to.
LRESULT CXTPControlComboBoxEditCtrl::OnEnableDisable(WPARAM, LPARAM)
{
	Invalidate();

	return 0;
}

void CXTPControlComboBoxEditCtrl::OnEditChanged()
{
	m_pControl->m_bEditChanged = TRUE;

	if (::GetFocus() == m_hWnd && !m_bIgonoreEditChanged)
		m_pControl->OnEditChanged();

	SetMargins(0, 0);
	UpdateCharFormat();
}

void CXTPControlComboBoxEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN && ::GetFocus() == m_hWnd)
	{
		m_pControl->OnExecute();
		return;
	}

	CXTPCommandBarEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPControlComboBoxEditCtrl::UpdateCharFormat()
{
	CHARFORMAT2 cf = m_pControl->GetDefaultCharFormat();
	cf.cbSize = GetRichEditContext().m_bRichEdit2 ? sizeof(CHARFORMAT2) : sizeof(CHARFORMAT);

	::SendMessage(m_hWnd, EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	::SendMessage(m_hWnd, EM_SETBKGNDCOLOR, FALSE, cf.crBackColor);
}

void CXTPControlComboBoxEditCtrl::OnShellAutoCompleteStart()
{
	m_pControl->GetCommandBar()->OnTrackLost();
}

void CXTPControlComboBoxEditCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
}

void CXTPControlComboBoxEditCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		ClientToScreen(&point);
		m_pControl->GetParent()->ScreenToClient(&point);

		m_pControl->GetParent()->OnRButtonDown(nFlags, point);
		return;
	}

	m_pControl->GetCommandBar()->OnTrackLost();
	m_pControl->SetFocused(TRUE);

	if (!ShowContextMenu(m_pControl, point))
	{
		CXTPCommandBarEditCtrl::OnRButtonDown(nFlags, point);
	}
}

void CXTPControlComboBoxEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		if (m_pControl->IsCustomizeMovable())
			m_pControl->OnClick();
	}
	else
	{
		CXTPCommandBarEditCtrl::OnLButtonDown(nFlags, point);
	}
}

void CXTPControlComboBoxEditCtrl::OnDestroy()
{
	GetWindowTextEx(m_pControl->m_strEditText);
	CXTPCommandBarEditCtrl::OnDestroy();
}

BOOL CXTPControlComboBoxEditCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CXTPCommandBar* pCommandBar = m_pControl->GetParent();

	if (pCommandBar)
	{
		pCommandBar->FilterToolTipMessage(message, wParam, lParam);
	}

	return CXTPCommandBarEditCtrl::OnWndMsg(message, wParam, lParam, pResult);
}

LRESULT CXTPControlComboBoxEditCtrl::OnWindowFromPoint(WPARAM, LPARAM)
{
	return 1;
}

void CXTPControlComboBoxEditCtrl::OnSetFocus(CWnd* pOldWnd)
{
	if (!m_pControl->IsCustomizeMode())
	{
		CXTPCommandBarEditCtrl::OnSetFocus(pOldWnd);
		m_pControl->OnSetFocus(pOldWnd);

		SetSel(0, 0);
		::PostMessage(m_hWnd, EM_SETSEL, 0, -1);
	}
}

void CXTPControlComboBoxEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CXTPCommandBarEditCtrl::OnKillFocus(pNewWnd);
	m_pControl->OnKillFocus();
}

void CXTPControlComboBoxEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCommandBarEditCtrl::OnMouseMove(nFlags, point);

	MapWindowPoints(m_pControl->GetParent(), &point, 1);
	m_pControl->GetParent()->OnMouseMove(nFlags, point);
}
