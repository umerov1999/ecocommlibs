// XTPHookManager.cpp : implementation of the CXTPHookManager class.
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

#include "Common/XTPHookManager.h"

#include "XTPCommandBarsDefines.h"
#include "XTPKeyboardManager.h"
#include "XTPMouseManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPKeyboardManager

CThreadLocal<CXTPKeyboardManager> CXTPKeyboardManager::_xtpKeyboardThreadState;


CXTPKeyboardManager::CXTPKeyboardManager()
{
	m_hHookKeyboard = 0;
	m_hHookCallWndProc = 0;
	m_hHookGetMessage = 0;

#ifdef _AFXDLL
	m_pModuleState = 0;
#endif
}

void CXTPKeyboardManager::SetupKeyboardHook(BOOL bSetup)
{
	if (bSetup && m_hHookKeyboard == 0)
	{
		m_hHookKeyboard = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, AfxGetInstanceHandle(), GetCurrentThreadId());
	}
	if (!bSetup && m_hHookKeyboard)
	{
		UnhookWindowsHookEx(m_hHookKeyboard);
		m_hHookKeyboard = 0;
	}
}


void CXTPKeyboardManager::SetupCallWndProcHook(BOOL bSetup)
{
	if (bSetup && m_hHookCallWndProc == 0)
	{
		m_hHookCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, AfxGetInstanceHandle(), GetCurrentThreadId());
	}
	if (!bSetup && m_hHookCallWndProc)
	{
		UnhookWindowsHookEx(m_hHookCallWndProc);
		m_hHookCallWndProc = 0;
	}
}

CXTPKeyboardManager::~CXTPKeyboardManager()
{
	SetupKeyboardHook(FALSE);
	SetupCallWndProcHook(FALSE);
}

void CXTPKeyboardManager::HookKeyboard(CXTPHookManagerHookAble* pHook)
{
	SetupKeyboardHook(TRUE);
	SetupCallWndProcHook(TRUE);

#ifdef _AFXDLL
	m_pModuleState = AfxGetModuleState();
#endif

	if (m_lstKeyboardHooks.Find(pHook) == NULL)
	{
		m_lstKeyboardHooks.AddTail(pHook);
	}

}
void CXTPKeyboardManager::UnhookKeyboard(CXTPHookManagerHookAble* pHook)
{
	POSITION pos = m_lstKeyboardHooks.Find(pHook);

	if (pos != NULL)
	{
		m_lstKeyboardHooks.RemoveAt(pos);
	}

	if (m_lstKeyboardHooks.IsEmpty())
	{
		SetupCallWndProcHook(FALSE);
		SetupKeyboardHook(FALSE);
	}

}
BOOL CXTPKeyboardManager::ProcessKeyboardHooks(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	POSITION pos = m_lstKeyboardHooks.GetTailPosition();
	while (pos)
	{
		CXTPHookManagerHookAble* pHook = m_lstKeyboardHooks.GetPrev(pos);
		LRESULT lResult = 0;
		BOOL bResult = pHook->OnHookMessage(0, nMessage, wParam, lParam, lResult);
		if (bResult)
		{
			return bResult != FALSE_EXIT;
		}
	}
	return TRUE;
}


LRESULT CALLBACK CXTPKeyboardManager::CallWndProc(int code, WPARAM wParam, LPARAM lParam)
{
	CXTPKeyboardManager* pKeyboardManager = XTPKeyboardManager();

	CWPSTRUCT* pMSG = (CWPSTRUCT*)lParam;

	if (pMSG && pMSG->message == WM_ACTIVATEAPP && !pMSG->wParam)
	{
#ifdef _AFXDLL
		AFX_MANAGE_STATE(pKeyboardManager->m_pModuleState);
#endif

		XTPMouseManager()->SendTrackLost();
	}

	return CallNextHookEx(pKeyboardManager->m_hHookCallWndProc, code, wParam, lParam);
}


LRESULT CALLBACK CXTPKeyboardManager::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	CXTPKeyboardManager* pKeyboardManager = XTPKeyboardManager();

	if (code != HC_ACTION)
		return CallNextHookEx(pKeyboardManager->m_hHookKeyboard, code, wParam, lParam);

	if (pKeyboardManager->IsKeyboardHooked() && (!(HIWORD(lParam) & KF_UP)))
	{
		SAFE_MANAGE_STATE(pKeyboardManager->m_pModuleState);

		if (!(HIWORD(lParam) & KF_ALTDOWN))
		{
			if (pKeyboardManager->ProcessKeyboardHooks(WM_KEYDOWN, wParam, lParam))
			{
				return TRUE;
			}
		}
		else
		{
			if (wParam == VK_MENU && !XTPMouseManager()->IsMouseLocked())
			{
				if (!(HIWORD(lParam) & KF_REPEAT))
				{
					XTPMouseManager()->SendTrackLost();
					return TRUE;
				}
			}
			else
			{
				if (pKeyboardManager->ProcessKeyboardHooks(WM_KEYDOWN, wParam, lParam))
				{
					return TRUE;
				}
			}
		}
	}


	return CallNextHookEx(pKeyboardManager->m_hHookKeyboard, code, wParam, lParam);

}

