// XTPSkinObject.cpp: implementation of the CXTPSkinObject class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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


#include "Common/Uxtheme.h"
#include "Common/Tmschema.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"

#include "XTPSkinManager.h"
#include "XTPSkinManagerSchema.h"
#include "XTPSkinObject.h"
#include "XTPSkinManagerApiHook.h"

#include "XTPSkinObjectFrame.h"
#include "XTPSkinObjectButton.h"
#include "XTPSkinObjectEdit.h"
#include "XTPSkinObjectComboBox.h"
#include "XTPSkinObjectListView.h"
#include "XTPSkinObjectListBox.h"
#include "XTPSkinObjectTreeView.h"
#include "XTPSkinObjectScrollBar.h"
#include "XTPSkinObjectTab.h"
#include "XTPSkinObjectToolBar.h"
#include "XTPSkinObjectProgress.h"
#include "XTPSkinObjectSpin.h"
#include "XTPSkinObjectHeader.h"
#include "XTPSkinObjectTrackBar.h"
#include "XTPSkinObjectStatusBar.h"
#include "XTPSkinObjectMDI.h"
#include "XTPSkinObjectMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CXTPSkinObjectPaintDC::CXTPSkinObjectPaintDC(CWnd* pWnd)
{
	ASSERT_VALID(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;

	m_ps.hdc = 0;
	m_hWnd = pWnd->m_hWnd;

	if (curMsg.wParam)
	{
		Attach((HDC)curMsg.wParam);
	}
	else
	{
		if (!Attach(::BeginPaint(m_hWnd, &m_ps)))
			AfxThrowResourceException();

		ASSERT(m_ps.hdc != NULL);
	}
}

CXTPSkinObjectPaintDC::~CXTPSkinObjectPaintDC()
{
	ASSERT(m_hDC != NULL);
	ASSERT(::IsWindow(m_hWnd));

	if (m_ps.hdc != NULL)
	{
		::EndPaint(m_hWnd, &m_ps);
	}
	Detach();
}
//////////////////////////////////////////////////////////////////////////
//

UINT CXTPSkinObject::m_nMsgUpdateSkinState = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_UPDATESTATE"));
UINT CXTPSkinObject::m_nMsgQuerySkinState = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_QUERYSTATE"));


CXTPSkinObject::CXTPSkinObject()
{
	m_hWnd = 0;
	m_pManager = NULL;
	m_pOldWndProc = NULL;

	m_bActiveX = FALSE;
	m_bWindowsForms = FALSE;

	m_bCustomDraw = FALSE;
	m_bUnicode = FALSE;

	m_bSystemWindowModule = FALSE;
	m_bWindowProcAttached = FALSE;
	m_bSetWindowProc = FALSE;

	m_pSchema = NULL;

	::InitializeCriticalSection(&m_csDescriptors);
}

CXTPSkinObject::~CXTPSkinObject()
{
	m_hWnd = 0;

	CMDTARGET_RELEASE(m_pSchema);

	::DeleteCriticalSection(&m_csDescriptors);
}

#if (_MSC_VER <= 1200) && !defined(_WIN64)
#define GetWindowLongPtrW GetWindowLongW
#define GetWindowLongPtrA GetWindowLongA
#define SetWindowLongPtrW SetWindowLongW
#define SetWindowLongPtrA SetWindowLongA
#endif

void CXTPSkinObject::UnattachHook(BOOL bAuto)
{

	if (m_bWindowProcAttached)
	{
		if (bAuto || !::SendMessage(m_hWnd, CXTPHookManager::m_nMsgSinkRemoved, (WPARAM)(LONG_PTR)CXTPSkinManager::HookWndProc, (LPARAM)(LONG_PTR)m_pOldWndProc))
		{
			if (m_bUnicode)
				SetWindowLongPtrW(m_hWnd, GWLP_WNDPROC, (DWORD_PTR)m_pOldWndProc);
			else
				SetWindowLongPtrA(m_hWnd, GWLP_WNDPROC, (DWORD_PTR)m_pOldWndProc);
		}

		m_bWindowProcAttached = FALSE;
	}

	OnHookDetached(bAuto);
}


void CXTPSkinObject::OnFinalRelease()
{
	delete this;
}

WNDPROC* CXTPSkinObject::GetSuperWndProcAddr()
{
	return &m_pOldWndProc;
}

LRESULT CXTPSkinObject::DoDefWindowProc (UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	OnBeginHook(nMsg, xtpSkinDefaultHookMessage, 0, 0);
	LRESULT lResult = ::DefWindowProc(m_hWnd, nMsg, wParam, lParam);
	OnEndHook();

	return lResult;
}

LRESULT CXTPSkinObject::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_arrDescriptors.IsEmpty())
		return ::CallWindowProc(m_pOldWndProc, m_hWnd, nMsg, wParam, lParam);

	DEFWINDOW_DESCRIPTIOR& des = m_arrDescriptors.GetHead();

	if (des.defProc == xtpSkinDefaultHookMessage)
		return ::CallWindowProc(m_pOldWndProc, m_hWnd, nMsg, wParam, lParam);

	if (des.defProc == xtpSkinDefaultCallWindowProc)
	{
		ASSERT(des.lpPrev);
		ASSERT(des.defProcAddr);

		typedef LRESULT (WINAPI* LPFNCALLWINDOWPROC)(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNCALLWINDOWPROC)des.defProcAddr)((WNDPROC)des.lpPrev, m_hWnd, nMsg, wParam, lParam);
	}

	if (des.defProc == xtpSkinDefaultDefWindowProc || des.defProc == xtpSkinDefaultDefMDIChildProc || des.defProc == xtpSkinDefaultDefDlgProc)
	{
		ASSERT(des.defProcAddr);

		typedef LRESULT (WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)des.defProcAddr)(m_hWnd, nMsg, wParam, lParam);
	}

	if (des.defProc == xtpSkinDefaultDefFrameProc)
	{
		ASSERT(des.defProcAddr);

		typedef LRESULT (WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, HWND hWndMDIClient, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)des.defProcAddr)(m_hWnd, (HWND)des.lpPrev, nMsg, wParam, lParam);
	}

	return 0;
}

void CXTPSkinObject::SetWindowProc()
{
	if (m_bWindowProcAttached)
		return;

	if (m_bUnicode)
	{
		m_pOldWndProc = (WNDPROC)GetWindowLongPtrW(m_hWnd, GWLP_WNDPROC);
	}
	else
	{
		m_pOldWndProc = (WNDPROC)GetWindowLongPtrA(m_hWnd, GWLP_WNDPROC);
	}


	if (m_bUnicode)
	{
		SetWindowLongPtrW(m_hWnd, GWLP_WNDPROC, (DWORD_PTR)CXTPSkinManager::HookWndProc);
	}
	else
	{
		SetWindowLongPtrA(m_hWnd, GWLP_WNDPROC, (DWORD_PTR)CXTPSkinManager::HookWndProc);
	}
	m_bWindowProcAttached = TRUE;
}

void CXTPSkinObject::AttachHook(HWND hWnd, LPCREATESTRUCT lpcs, BOOL bAuto)
{
	m_hWnd = hWnd;
	m_bUnicode = IsWindowUnicode(hWnd);

	if (m_bUnicode)
	{
		m_pOldWndProc = (WNDPROC)GetWindowLongPtrW(hWnd, GWLP_WNDPROC);
	}
	else
	{
		m_pOldWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
	}

	if (CXTPSkinManagerApiHook::IsSystemWindowModule(m_pOldWndProc))
	{
		m_bSystemWindowModule = TRUE;
	}

	if (m_pManager->IsWin9x() && ((DWORD_PTR)m_pOldWndProc > (DWORD_PTR)0x70000000) && (m_pOldWndProc != CXTPSkinManagerApiFunction::sm_pfnAfxWndProc))
	{
		m_bSystemWindowModule = TRUE;
	}


	if (m_bSystemWindowModule || m_bSetWindowProc)
	{
		SetWindowProc();
	}

	OnHookAttached(lpcs, bAuto);
}

CXTPSkinManagerClass* CXTPSkinObject::GetSkinClass() const
{
	return GetSkinManager()->GetSkinClass(this, m_strClassName);
}

CXTPSkinManagerMetrics* CXTPSkinObject::GetMetrics() const
{
	CXTPSkinManagerSchema* pSchema = GetSchema();
	return pSchema ? pSchema->GetMetrics() : NULL;
}

CXTPSkinManagerSchema* CXTPSkinObject::GetSchema() const
{
	return m_pSchema ? m_pSchema : GetSkinManager()->GetSchema();
}

COLORREF CXTPSkinObject::GetColor(int nIndex) const
{
	CXTPSkinManagerMetrics* pMetrics = GetMetrics();
	return pMetrics ? pMetrics->GetColor(nIndex) : GetXtremeColor(nIndex);
}


void CXTPSkinObject::OnSkinChanged(BOOL /*bPrevState*/, BOOL /*bNewState*/)
{
	RefreshMetrics();
}

void CXTPSkinObject::OnHookAttached(LPCREATESTRUCT /*lpcs*/, BOOL bAuto)
{
	m_bCustomDraw = CWnd::FromHandlePermanent(m_hWnd) ? SendMessage(m_nMsgQuerySkinState) != 0 : FALSE;

	if (!bAuto) RefreshMetrics();
}

void CXTPSkinObject::OnHookDetached(BOOL /*bAuto*/)
{

}

void CXTPSkinObject::RefreshMetrics()
{

}

BOOL CXTPSkinObject::IsDefWindowProcAvail(int nMessage) const
{
	if (nMessage == WM_TIMER)
		return FALSE;


	if (m_bSystemWindowModule)
		return FALSE;

	if (m_bActiveX && !m_bSetWindowProc)
		return FALSE;

	return TRUE;
}

BOOL CXTPSkinObject::PreHookMessage(UINT nMessage)
{
	if (nMessage == WM_SETCURSOR || nMessage == WM_NOTIFY|| nMessage == WM_COMMAND || nMessage == WM_ACTIVATE)
		return TRUE;

	if (nMessage == m_nMsgUpdateSkinState)
	{
		m_bCustomDraw = SendMessage(m_nMsgQuerySkinState) != 0;
	}

	return FALSE;
}

BOOL CXTPSkinObject::OnHookDefWindowProc(UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult)
{
	if (PreHookMessage(nMessage))
		return FALSE;

	if (nMessage == m_nMsgQuerySkinState)
	{
		lResult = 0;
		return TRUE;
	}

	if (OnWndMsg(nMessage, wParam, lParam, &lResult))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPSkinObject::OnHookMessage(UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult)
{
	if (PreHookMessage(nMessage))
		return FALSE;

	if (IsDefWindowProcAvail(nMessage))
		return FALSE;

	if (OnWndMsg(nMessage, wParam, lParam, &lResult))
	{
		return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// CXTPSkinManager

void CXTPSkinManager::ApplyWindow(HWND hWnd)
{
	if (Lookup(hWnd))
		return;

	TCHAR lpszClassName[50];
	GetClassName(hWnd, lpszClassName, 50);

	CREATESTRUCT cs;
	ZeroMemory(&cs, sizeof(cs));

	cs.dwExStyle = (DWORD)GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	cs.style = (LONG)GetWindowLongPtr(hWnd, GWL_STYLE);
	cs.hwndParent = ::GetParent(hWnd);
	cs.lpszClass = lpszClassName;

	CXTPSkinObject* pSkinObject = SetHook(hWnd, lpszClassName, &cs, FALSE);

	if (pSkinObject)
	{
		pSkinObject->OnSkinChanged(FALSE, m_bEnabled);
	}

	if (pSkinObject && pSkinObject->m_strClassName == _T("COMBOBOX"))
	{
		HWND hWndList = CXTPSkinObjectComboBox::GetComboListBox(hWnd);
		if (hWndList)
		{
			ApplyWindow(hWndList);
		}
	}

	hWnd = ::GetWindow(hWnd, GW_CHILD);
	while (hWnd)
	{
		ApplyWindow(hWnd);

		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
}


BOOL CALLBACK CXTPSkinManager::EnumWindowsProcNetBroadcast(HWND hWnd, LPARAM /*lParam*/)
{
	DWORD dwProcessId = 0;
	if (GetWindowThreadProcessId(hWnd, &dwProcessId) && dwProcessId == GetCurrentProcessId())
	{
		::PostMessage(hWnd, WM_SYSCOLORCHANGE, 0, 0);
	}

	return TRUE;
}

void CXTPSkinManager::OnSkinChanged(BOOL bPrevState, BOOL bNewState)
{
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd) pWnd->SendMessage(WM_SYSCOLORCHANGE);

	HWND hWnd;
	POSITION pos = m_mapObjects.GetStartPosition();
	CXTPSkinObject* pSink;

	EnumWindows(EnumWindowsProcNetBroadcast, 0);

	while (pos)
	{
		m_mapObjects.GetNextAssoc(pos, hWnd, pSink);

		if (::IsWindow(hWnd))
		{
			pSink->OnSkinChanged(bPrevState, bNewState);
		}
		else
		{
			Remove(hWnd);
		}
	}
}

CXTPSkinObject* CXTPSkinManager::Lookup(HWND hWnd)
{
	CXTPLockGuard lock(m_csObjects);

	CXTPSkinObject* pSink;

	if (m_mapObjects.Lookup(hWnd, pSink))
		return pSink;

	return NULL;
}

#ifndef OBJID_WINDOW
#define OBJID_WINDOW 0x00000000
#endif

#ifndef EVENT_OBJECT_REORDER
#define EVENT_OBJECT_REORDER 0x8004
#endif

void CALLBACK CXTPSkinManager::WinEventProc(HWINEVENTHOOK /*hWinEventHook*/,
	DWORD event, HWND hWnd, LONG idObject, LONG /*idChild*/, DWORD /*dwEventThread*/, DWORD /*dwmsEventTime*/)
{
	if (event == EVENT_OBJECT_REORDER && idObject == OBJID_WINDOW && (XTPSkinManager()->GetApplyOptions() & xtpSkinApplyFrame)
		&& XTPSkinManager()->IsEnabled())
	{
		CXTPSkinObjectFrame* pFrame = (CXTPSkinObjectFrame*)XTPSkinManager()->Lookup(hWnd);

		if (pFrame && ::IsWindow(hWnd))
		{
			DWORD dwStyle = pFrame->GetStyle();
			DWORD dwStyleRemove = (WS_CAPTION | WS_VSCROLL | WS_HSCROLL);

			if (dwStyle & dwStyleRemove)
			{
				pFrame->SetTimer(XTP_TID_REFRESHFRAME, 1000, &CXTPSkinObjectFrame::OnTimerInternal);
			}
		}
	}

	return;
}

BOOL CXTPSkinObject::IsSkinEnabled() const
{
	return !m_bCustomDraw && m_hWnd && m_pManager->IsEnabled();
}

#define PSH_WIZARD97IE4            0x00002000
#define PSH_WIZARD97IE5            0x01000000

#ifndef DWLP_USER
#define DWLP_USER 8
#endif

AFX_INLINE BOOL IsTabPage(LPCREATESTRUCT lpcs)
{
	if (!lpcs || !lpcs->hwndParent)
		return FALSE;

	if (!((lpcs->style & DS_CONTROL) && (lpcs->style & WS_CHILD)))
		return FALSE;

	CXTPSkinObjectFrame* pParent = (CXTPSkinObjectFrame*)XTPSkinManager()->Lookup(lpcs->hwndParent);
	if (pParent && pParent->m_dwDialogTexture == ETDT_ENABLETAB)
		return TRUE;

	HWND hwndTabControl = (HWND)::SendMessage(lpcs->hwndParent, PSM_GETTABCONTROL, 0, 0);
	if (!hwndTabControl || !::IsWindow(hwndTabControl))
		return FALSE;

	HWND* ppd = (HWND*)GetWindowLongPtr(lpcs->hwndParent, DWLP_USER);
	if (!ppd || *ppd != lpcs->hwndParent)
		return FALSE;

	PROPSHEETHEADER* psh = (PROPSHEETHEADER*)(ppd + 1);
	if (!psh)
		return FALSE;

	if ((psh->dwFlags & (PSH_WIZARD | PSH_WIZARD97IE4 | PSH_WIZARD97IE5)) != 0)
		return FALSE;

	return TRUE;
}

CXTPSkinObject* CXTPSkinManager::SetHook(HWND hWnd, LPCTSTR lpszClassName, LPCREATESTRUCT lpcs, BOOL bAuto)
{
	if (_tcscmp(lpszClassName, _T("tooltips_class32")) == 0)
		return NULL;

	if (_tcsicmp(lpszClassName, _T("IME")) == 0)
		return NULL;

	if(_tcsicmp(lpszClassName,_T("CicMarshalWndClass")) == 0)
		return NULL;

	if(_tcsicmp(lpszClassName,_T("MSCTFIME UI")) == 0)
		return NULL;

	if (_tcsicmp(lpszClassName, _T("WorkerW")) == 0)
		return NULL;

	if (_tcsicmp(lpszClassName, _T("SHELLDLL_DefView")) == 0)
		return NULL;

	CXTPSkinObject* pSink = NULL;

	if (pSink == NULL && _tcsstr(lpszClassName, _T("WindowsForms")) != NULL)
	{

		CString strClass(lpszClassName);
		int nClassNameStart = strClass.Find(_T('.'));
		int nClassNameEnd = strClass.Find(_T('.'), nClassNameStart + 1);

		if (nClassNameStart != -1 && nClassNameEnd != -1 && nClassNameEnd != nClassNameStart)
		{
			strClass = strClass.Mid(nClassNameStart + 1, nClassNameEnd - nClassNameStart - 1);

			CXTPSkinObjectClassInfo* pInfo = m_pClassMap->Lookup(strClass);

			if (pInfo)
			{
				pSink = pInfo->CreateObject(lpszClassName, lpcs);
			}
		}

		if (pSink == NULL)
		{
			pSink = new CXTPSkinObjectApplicationFrame();
		}

		pSink->m_bWindowsForms = TRUE;
	}

	if (pSink == NULL)
	{
		CXTPSkinObjectClassInfo* pInfo = m_pClassMap->Lookup(lpszClassName);

		if (pInfo)
		{
			pSink = pInfo->CreateObject(lpszClassName, lpcs);
		}
		else
		{
			if (_tcsstr(lpszClassName, _T("Internet Explorer")) != NULL)
				return NULL;

			if (_tcsstr(lpszClassName, _T("OleMainThread")) != NULL)
				return NULL;

			pSink = new CXTPSkinObjectApplicationFrame();
		}
	}

	if (pSink && _tcscmp(lpszClassName, _T("#32770")) == 0)
	{
		((CXTPSkinObjectFrame*)pSink)->m_dwDialogTexture = IsTabPage(lpcs) ? ETDT_ENABLETAB : ETDT_ENABLE;
	}

	if (pSink)
	{

		pSink->m_pManager = this;
		{
			CXTPLockGuard lock(m_csObjects);
			m_mapObjects[hWnd] = pSink;
		}

		pSink->AttachHook(hWnd, lpcs, bAuto);
	}

	return pSink;
}

void CXTPSkinManager::Remove(HWND hWnd)
{
	Remove(hWnd, FALSE);
}

void CXTPSkinManager::Remove(HWND hWnd, BOOL bAuto)
{
	CXTPSkinObject* pSink = Lookup(hWnd);
	if (pSink)
	{
		{
			CXTPLockGuard lock(m_csObjects);
			m_mapObjects.RemoveKey(hWnd);
		}
		pSink->UnattachHook(bAuto);

#ifdef _AFXDLL
		pSink->m_pModuleState = AfxGetModuleState();
#endif
		pSink->InternalRelease();
	}
}

void CXTPSkinManager::RemoveAll(BOOL bUnattach)
{

	HWND hWnd;
	POSITION pos = m_mapObjects.GetStartPosition();
	CXTPSkinObject* pSink;

	while (pos)
	{
		m_mapObjects.GetNextAssoc(pos, hWnd, pSink);
		if (bUnattach) pSink->UnattachHook(FALSE);

#ifdef _AFXDLL
		pSink->m_pModuleState = AfxGetModuleState();
#endif
		pSink->InternalRelease();
	}


	CXTPLockGuard lock(m_csObjects);

	m_mapObjects.RemoveAll();
}

void CXTPSkinObject::OnBeginHook(UINT nMessage, XTPSkinDefaultProc defProc, PROC defProcAddr, LPVOID defProcPrevWndFunc)
{
	InternalAddRef();
	CXTPLockGuard lock(m_csDescriptors);

	DEFWINDOW_DESCRIPTIOR des;

	des.defProc = defProc;
	des.nMessage = nMessage;
	des.defProcAddr = defProcAddr;
	des.lpPrev = defProcPrevWndFunc;

	m_arrDescriptors.AddHead(des);
}

void CXTPSkinObject::OnEndHook()
{
	{
		CXTPLockGuard lock(m_csDescriptors);
		m_arrDescriptors.RemoveHead();
	}
	InternalRelease();
}

UINT CXTPSkinObject::GetHeadMessage()
{
	CXTPLockGuard lock(m_csDescriptors);

	if (!m_arrDescriptors.IsEmpty())
	{
		return m_arrDescriptors.GetHead().nMessage;
	}
	return WM_NULL;
}

LRESULT CALLBACK CXTPSkinManager::DoCallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return CXTPSkinManagerApiHook::CallWindowProcOrig(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK CXTPSkinManager::HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Get hook object for this window. Get from hook map
	CXTPSkinObject* pSink = XTPSkinManager()->Lookup(hWnd);
	if (!pSink)
		return 0;

	LRESULT lResult = 0;
	WNDPROC wndProc = pSink->m_pOldWndProc;

	if (!pSink->GetSkinManager()->m_bEnabled)
	{
		if (message == WM_NCDESTROY)
		{
			pSink->GetSkinManager()->Remove(hWnd, TRUE);
		}

		return ::CallWindowProc(wndProc, hWnd, message, wParam, lParam);
	}


	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	MSG  oldMsg = curMsg;
	curMsg.hwnd = hWnd;
	curMsg.message = message;
	curMsg.wParam  = wParam;
	curMsg.lParam  = lParam;

	BOOL bCallWindowProc = TRUE;

	// Window is being destroyed: unhook all hooks (for this window)
	// and pass message to orginal window proc
	if (message == WM_NCDESTROY)
	{
		pSink->GetSkinManager()->Remove(hWnd, TRUE);
	}
	// pass to message hook
	else if (!pSink->m_bCustomDraw || message == CXTPSkinObject::m_nMsgUpdateSkinState)
	{
		pSink->OnBeginHook(message, xtpSkinDefaultHookMessage, 0, 0);

		if (pSink->OnHookMessage(message, wParam, lParam, lResult))
		{
			bCallWindowProc = FALSE;
		}

		pSink->OnEndHook();
	}

	if (bCallWindowProc)
	{
		lResult = DoCallWindowProc(wndProc, hWnd, message, wParam, lParam);
	}

	curMsg = oldMsg;
	return lResult;
}

//////////////////////////////////////////////////////////////////////////
// CXTPSkinObjectClassInfo

CXTPSkinObjectClassInfo::CXTPSkinObjectClassInfo(CRuntimeClass* pClass)
{
	m_pMap = NULL;
	m_pClass = pClass;
}

CXTPSkinObject* CXTPSkinObjectClassInfo::CreateObject(LPCTSTR /*lpszClassName*/, LPCREATESTRUCT /*lpcs*/)
{
	if (!m_pClass)
		return NULL;

	return (CXTPSkinObject*)m_pClass->CreateObject();
}

void CXTPSkinObjectClassInfo::AddClass(LPCTSTR lpszClassName)
{
	m_pMap->AddClass(lpszClassName, this);
}

void CXTPSkinObjectClassInfo::Remove()
{
	m_pMap->RemoveClass(this);
}

//////////////////////////////////////////////////////////////////////////
// CXTPSkinObjectClassMap

CXTPSkinObjectClassMap::CXTPSkinObjectClassMap()
{

}

CXTPSkinObjectClassMap::~CXTPSkinObjectClassMap()
{
	RemoveAll();
}

#define RUNTIME_CLASS_INFO(x) new CXTPSkinObjectClassInfo(RUNTIME_CLASS(x))
#define RUNTIME_CLASS_INFOACTIVEX(x) new CXTPSkinObjectClassInfoActiveX(RUNTIME_CLASS(x), FALSE, FALSE)
#define RUNTIME_CLASS_INFOSYSACTIVEX(x) new CXTPSkinObjectClassInfoActiveX(RUNTIME_CLASS(x), TRUE, FALSE)
#define RUNTIME_CLASS_INFOSYSACTIVEX2(x) new CXTPSkinObjectClassInfoActiveX(RUNTIME_CLASS(x), FALSE, TRUE)

void CXTPSkinObjectClassMap::AddStandardClasses()
{
	AddClass(_T("Button"),             RUNTIME_CLASS_INFO(CXTPSkinObjectButton));
	AddClass(_T("ComboBox"),           RUNTIME_CLASS_INFO(CXTPSkinObjectComboBox));
	AddClass(_T("Edit"),               RUNTIME_CLASS_INFO(CXTPSkinObjectEdit));
	AddClass(_T("ListBox"),            RUNTIME_CLASS_INFO(CXTPSkinObjectListBox));
	AddClass(_T("SysDateTimePick32"),  RUNTIME_CLASS_INFO(CXTPSkinObjectDateTime));
	AddClass(_T("SysMonthCal32"),      RUNTIME_CLASS_INFO(CXTPSkinObjectMonthCal));
	AddClass(_T("ScrollBar"),          RUNTIME_CLASS_INFO(CXTPSkinObjectScrollBar));
	AddClass(_T("SysListView32"),      RUNTIME_CLASS_INFO(CXTPSkinObjectListView));
	AddClass(_T("SysHeader32"),        RUNTIME_CLASS_INFO(CXTPSkinObjectHeader));
	AddClass(_T("msctls_statusbar32"), RUNTIME_CLASS_INFO(CXTPSkinObjectStatusBar));
	AddClass(_T("SysTreeView32"),      RUNTIME_CLASS_INFO(CXTPSkinObjectTreeView));
	AddClass(_T("msctls_progress32"),  RUNTIME_CLASS_INFO(CXTPSkinObjectProgress));
	AddClass(_T("msctls_trackbar32"),  RUNTIME_CLASS_INFO(CXTPSkinObjectTrackBar));
	AddClass(_T("SysTabControl32"),    RUNTIME_CLASS_INFO(CXTPSkinObjectTab));
	AddClass(_T("MDIClient"),          RUNTIME_CLASS_INFO(CXTPSkinObjectMDIClient));
	AddClass(_T("msctls_updown32"),    RUNTIME_CLASS_INFO(CXTPSkinObjectSpin));
	AddClass(_T("ToolbarWindow32"),    RUNTIME_CLASS_INFO(CXTPSkinObjectToolBar));

	AddSynonymClass(_T("SysIPAddress32"), _T("Edit"));
	AddSynonymClass(_T("msctls_hotkey32"), _T("Edit"));
	AddSynonymClass(_T("ComboLBox"), _T("ListBox"));
	AddSynonymClass(_T("ATL:ScrollBar"), _T("ScrollBar"));

	AddSynonymClass(_T("TButton"), _T("Button"));
	AddSynonymClass(_T("TEdit"), _T("Edit"));
	AddSynonymClass(_T("TMemo"), _T("Edit"));
	AddSynonymClass(_T("TNewProgressBar"), _T("msctls_progress32"));
	AddSynonymClass(_T("TRichEditViewer"), _T("Edit"));
	AddSynonymClass(_T("TRadioButton"), _T("Button"));
	AddSynonymClass(_T("TCheckBox"), _T("Button"));
	AddSynonymClass(_T("TComboBox"), _T("ComboBox"));
	AddSynonymClass(_T("TGroupBox"), _T("Button"));
	AddSynonymClass(_T("TScrollBar"), _T("ScrollBar"));
	AddSynonymClass(_T("TProgressBar"), _T("msctls_progress32"));
	AddSynonymClass(_T("TTrackBar"), _T("msctls_trackbar32"));
	AddSynonymClass(_T("TTabControl"), _T("SysTabControl32"));
	AddSynonymClass(_T("TTreeView"), _T("SysTreeView32"));
	AddSynonymClass(_T("TListView"), _T("SysListView32"));
	AddSynonymClass(_T("TListBox"), _T("ListBox"));

	AddClass(_T("#32768"), new CXTPSkinObjectClassInfo(NULL));
	AddClass(_T("XTPSkinManagerMenu"), new CXTPSkinObjectClassInfo(NULL));


	AddClass(_T("SysPager"), new CXTPSkinObjectClassInfo(NULL));
}

void CXTPSkinObjectClassMap::AddSynonymClass(LPCTSTR lpszClassNameSynonym, LPCTSTR lpszClassName)
{
	if (Lookup(lpszClassNameSynonym))
		return;

	if (lpszClassName == NULL || *lpszClassName == 0)
	{
		AddClass(lpszClassNameSynonym, new CXTPSkinObjectClassInfo(NULL));
		return;
	}

	CXTPSkinObjectClassInfo* pInfo = Lookup(lpszClassName);

	ASSERT(pInfo);
	if (!pInfo)
		return;

	AddClass(lpszClassNameSynonym, pInfo);
}

CXTPSkinObjectClassInfo* CXTPSkinObjectClassMap::AddClass(LPCTSTR lpszClassName, CXTPSkinObjectClassInfo* pInfo)
{
	CString strClassName(lpszClassName);
	strClassName.MakeUpper();

	ASSERT(Lookup(strClassName) == NULL);

	m_mapInfo.SetAt(strClassName, pInfo);

	if (pInfo->m_pMap == 0)
	{
		pInfo->m_pMap = this;
	}
	else
	{
		ASSERT(pInfo->m_pMap == this);
		pInfo->InternalAddRef();
	}

	return pInfo;
}

void CXTPSkinObjectClassMap::RemoveClass(LPCTSTR lpszClassName)
{
	CString strClassName(lpszClassName);
	strClassName.MakeUpper();

	CXTPSkinObjectClassInfo* pInfo = Lookup(strClassName);
	if (!pInfo)
		return;

	m_mapInfo.RemoveKey(strClassName);
	pInfo->InternalRelease();
}

void CXTPSkinObjectClassMap::RemoveClass(CXTPSkinObjectClassInfo* pInfo)
{
	POSITION pos = m_mapInfo.GetStartPosition();
	CString strClassName;
	void* p;

	while (pos)
	{
		m_mapInfo.GetNextAssoc(pos, strClassName, p);

		if (p == pInfo)
		{
			m_mapInfo.RemoveKey(strClassName);
			pInfo->InternalRelease();
		}
	}
}

void CXTPSkinObjectClassMap::RemoveAll()
{
	POSITION pos = m_mapInfo.GetStartPosition();
	CString strClassName;
	CXTPSkinObjectClassInfo* pInfo;

	while (pos)
	{
		m_mapInfo.GetNextAssoc(pos, strClassName, (void*&)pInfo);
		pInfo->InternalRelease();
	}

	m_mapInfo.RemoveAll();
}

CXTPSkinObjectClassInfo* CXTPSkinObjectClassMap::Lookup(LPCTSTR lpszClassName)
{
	CString strClassName(lpszClassName);
	strClassName.MakeUpper();

	void* pInfo;

	if (!m_mapInfo.Lookup(strClassName, pInfo))
		return NULL;

	return (CXTPSkinObjectClassInfo*)pInfo;
}
