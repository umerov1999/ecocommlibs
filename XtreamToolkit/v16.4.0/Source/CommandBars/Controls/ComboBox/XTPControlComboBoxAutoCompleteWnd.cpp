// XTPControlComboBoxAutoCompleteWnd.cpp : implementation of the CXTPControlComboBoxAutoCompleteWnd class.
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
// CXTPControlComboBoxAutoCompleteWnd


HHOOK CXTPControlComboBoxAutoCompleteWnd::m_hHookMessage = 0;
CXTPControlComboBoxAutoCompleteWnd* CXTPControlComboBoxAutoCompleteWnd::m_pWndMonitor = 0;

CXTPControlComboBoxAutoCompleteWnd::CXTPControlComboBoxAutoCompleteWnd()
{
	m_hWndAutoComplete = 0;
	m_hWndEdit = 0;
}

CXTPControlComboBoxAutoCompleteWnd::~CXTPControlComboBoxAutoCompleteWnd()
{
	SetupMessageHook(FALSE);
	SetAutoCompeteHandle(NULL);
}

BOOL CXTPControlComboBoxAutoCompleteWnd::IsDialogCode(UINT nChar, LPARAM lParam)
{
	if (!m_hWndAutoComplete)
		return FALSE;

	if (! ((nChar == VK_UP || nChar == VK_DOWN || nChar == VK_NEXT || nChar == VK_PRIOR) && (GetKeyState(VK_MENU) >= 0)))
		return FALSE;

	HWND hWnd = ::GetWindow(m_hWndAutoComplete, GW_CHILD);
	hWnd = ::GetWindow(hWnd, GW_HWNDLAST);

	MSG msg;
	msg.message = WM_KEYDOWN;
	msg.hwnd = hWnd;
	msg.wParam = (WPARAM)nChar;
	msg.lParam = lParam;

	if (::IsDialogMessage(m_hWndAutoComplete, &msg))
		return TRUE;

	return FALSE;
}


HRESULT CXTPControlComboBoxAutoCompleteWnd::ShellAutoComplete(HWND hEdit, DWORD dwFlags)
{
	m_hWndEdit = hEdit;
	HRESULT hResult = E_FAIL;

	HMODULE hLib = LoadLibraryA("shlwapi.dll");
	if (hLib)
	{
		typedef HRESULT (STDAPICALLTYPE *SHAUTOCOMPLETE)(HWND hwndEdit, DWORD dwFlags);

		SHAUTOCOMPLETE _SHAutoComplete = NULL;
		_SHAutoComplete = (SHAUTOCOMPLETE)GetProcAddress(hLib, "SHAutoComplete");

		if (_SHAutoComplete)
		{
			hResult = _SHAutoComplete(hEdit, dwFlags);
		}

		FreeLibrary(hLib);
	}

	return hResult;
}

void CXTPControlComboBoxAutoCompleteWnd::CloseWindow()
{
	if (m_hWndAutoComplete)
	{
		ShowWindow(m_hWndAutoComplete, SW_HIDE);
	}
}

void CXTPControlComboBoxAutoCompleteWnd::SetAutoCompeteHandle(HWND hWnd)
{
	if (m_hWndAutoComplete == hWnd)
		return;

	if (hWnd)
	{
		XTPMouseManager()->AddTrustedWindow(hWnd);
	}
	else
	{
		XTPMouseManager()->RemoveTrustedWindow(m_hWndAutoComplete);
	}

	m_hWndAutoComplete = hWnd;
}

void CXTPControlComboBoxAutoCompleteWnd::SetupMessageHook(BOOL bSetup)
{
	if (bSetup && m_hHookMessage == 0)
	{
		ASSERT(m_pWndMonitor == NULL);
		m_hHookMessage = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, XTPGetInstanceHandle(), GetCurrentThreadId());
		XTPKeyboardManager()->HookKeyboard(this);
		m_pWndMonitor = this;
	}

	if (!bSetup && m_hHookMessage && (m_pWndMonitor == this))
	{
		UnhookWindowsHookEx(m_hHookMessage);
		XTPKeyboardManager()->UnhookKeyboard(m_pWndMonitor);
		m_hHookMessage = 0;
		m_pWndMonitor = NULL;
	}

	if (!bSetup)
	{
		SetAutoCompeteHandle(NULL);
	}
}

int CXTPControlComboBoxAutoCompleteWnd::OnHookMessage(HWND /*hWnd*/, UINT nMessage, WPARAM& /*wParam*/, LPARAM& /*lParam*/, LRESULT& /*lResult*/)
{
	if (nMessage == WM_MOUSEWHEEL && m_hWndAutoComplete)
		return FALSE_EXIT;

	return FALSE;
}

LRESULT CALLBACK CXTPControlComboBoxAutoCompleteWnd::CallWndProc(int code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* pMSG = (CWPSTRUCT*)lParam;

	if (pMSG && m_pWndMonitor && pMSG->message == WM_WINDOWPOSCHANGING)
	{
		const HWND& hWndAutoComplete = m_pWndMonitor->m_hWndAutoComplete;
		WINDOWPOS* pWindowPos = (WINDOWPOS*)pMSG->lParam;

		if ((hWndAutoComplete != 0 && hWndAutoComplete == pWindowPos->hwnd) && (pWindowPos->flags & SWP_HIDEWINDOW))
		{
			m_pWndMonitor->SetAutoCompeteHandle(NULL);
		}
		else if ((hWndAutoComplete == 0) && (pWindowPos->flags & SWP_SHOWWINDOW))
		{
			HWND hWnd = pWindowPos->hwnd;
			TCHAR szClassName[255];
			::GetClassName(hWnd, szClassName, 255);

			if (_tcscmp(szClassName, _T("Auto-Suggest Dropdown")) == 0)
			{
				m_pWndMonitor->SetAutoCompeteHandle(hWnd);
				PostMessage(m_pWndMonitor->m_hWndEdit, WM_XTP_SHELLAUTOCOMPLETESTART, 0, 0);
			}
		}
	}

	return CallNextHookEx(m_hHookMessage, code, wParam, lParam);
}
