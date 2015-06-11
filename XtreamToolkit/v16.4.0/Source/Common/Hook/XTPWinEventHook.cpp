#include "StdAfx.h"
#include <Common/Hook/XTPWinEventHook.h>

CXTPWinEventHook::CXTPWinEventHook()
	: m_pSetWinEventHook(NULL)
	, m_pUnhookWinEvent (NULL)
	, m_hWinEventHook   (NULL)
	, m_nCount          (0)
{
	HMODULE hLibrary = GetModuleHandle(_T("USER32"));

	if (NULL != hLibrary)
	{
		m_pSetWinEventHook = (LPFNSETWINEVENTHOOK)::GetProcAddress(hLibrary, "SetWinEventHook");
		m_pUnhookWinEvent  = (LPFNUNHOOKWINEVENT) ::GetProcAddress(hLibrary, "UnhookWinEvent");
	}
}

CXTPWinEventHook::~CXTPWinEventHook()
{

}

void CXTPWinEventHook::SetEventHook(UINT eventMin, UINT eventMax, WINEVENTPROC pfnEventProc)
{
	if (m_nCount > 0)
	{
		m_nCount++;
		return;
	}

	if (NULL != m_pSetWinEventHook)
	{
		m_hWinEventHook = m_pSetWinEventHook(eventMin, eventMax, NULL,
			pfnEventProc, ::GetCurrentProcessId(), 0, 0);
	}

	m_nCount++;
}

void CXTPWinEventHook::RemoveEventHook()
{
	InterlockedDecrement(&m_nCount);

	if (0 == m_nCount)
	{
		if (NULL != m_pUnhookWinEvent)
		{
			m_pUnhookWinEvent(m_hWinEventHook);
			m_hWinEventHook = NULL;
		}
	}
}
