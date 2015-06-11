// XTPSkinObject.cpp: implementation of the CXTPSkinObject class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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


#include <Common/Uxtheme.h>
#include <Common/Tmschema.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/XTPSynchro.h>

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
#include "XTPSkinObjectReBar.h"
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

const UINT CXTPSkinObject::m_nMsgUpdateSkinState = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_UPDATESTATE"));
const UINT CXTPSkinObject::m_nMsgQuerySkinState = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_QUERYSTATE"));
const UINT CXTPSkinObject::m_nMsgSkinChanged = RegisterWindowMessage(_T("WM_SKINFRAMEWORK_SKINCHANGED"));


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

	m_bDrawingEnabled = -1;

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

CString CXTPSkinObject::GetWndClassTreePath(HWND hWnd, BOOL bPathOnly /*= FALSE*/)
{
	ASSERT(NULL != hWnd);
	ASSERT(::IsWindow(hWnd));

	LPCTSTR lpszSeparator = _T("/");

	CString strPath;

	if (!bPathOnly)
	{
		TCHAR szClassName[100];
		if (0 < ::GetClassName(hWnd, szClassName, _countof(szClassName)))
		{
			HWND hwndParent = ::GetParent(hWnd);
			if (NULL != hwndParent)
			{
				strPath = GetWndClassTreePath(hwndParent) + lpszSeparator + szClassName;
			}
			else
			{
				strPath = CString(lpszSeparator) + szClassName;
			}
		}
	}
	else
	{
		HWND hwndParent = ::GetParent(hWnd);
		if (NULL != hwndParent)
		{
			strPath = GetWndClassTreePath(hwndParent, FALSE);
		}
		else
		{
			strPath = lpszSeparator;
		}
	}

	return strPath;
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

#ifdef _XTP_ACTIVEX
	if (m_bWindowsForms) m_bSystemWindowModule = TRUE;
#endif

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
	m_bDrawingEnabled = -1;
}

BOOL CXTPSkinObject::IsDefWindowProcAvail(int nMessage) const
{
	if (nMessage == WM_TIMER)
		return FALSE;

#ifdef _XTP_ACTIVEX
	if (m_bWindowsForms)
	{
		if (nMessage < WM_CTLCOLORMSGBOX || nMessage > WM_CTLCOLORSTATIC)
			return FALSE;
		return !IsWindowEnabled();
	}
#endif

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

	if (nMessage == m_nMsgSkinChanged)
	{
		OnSkinChanged((BOOL)wParam, (LPARAM)lParam);
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

BOOL CXTPSkinObject::IsSkinEnabled() const
{
	return !m_bCustomDraw && m_hWnd && m_pManager->IsEnabled();
}

BOOL CXTPSkinObject::IsDrawingEnabled(BOOL bForceUpdateStatus /*= FALSE*/) const
{
	BOOL bEnabled = m_bDrawingEnabled;
	if (-1 == bEnabled || bForceUpdateStatus)
	{
		bEnabled = const_cast<CXTPSkinObject*>(this)->CheckDrawingEnabled();
		const_cast<CXTPSkinObject*>(this)->m_bDrawingEnabled = bEnabled;
	}

	return bEnabled;
}

BOOL CXTPSkinObject::CheckDrawingEnabled()
{
	return TRUE;
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
	AddClass(_T("ReBarWindow32"),      RUNTIME_CLASS_INFO(CXTPSkinObjectReBar));

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

#ifdef _XTP_ACTIVEX
	AddClass(_T("msvb_lib_header"), RUNTIME_CLASS_INFOSYSACTIVEX(CXTPSkinObjectHeader));

	AddClass(_T("ThunderRT6CheckBox"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectButton));
	AddClass(_T("ThunderCheckBox"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectButton));
	AddClass(_T("ThunderRT6OptionButton"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectButton));
	AddClass(_T("ThunderRT6CommandButton"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectButton));
	AddClass(_T("ThunderOptionButton"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectButton));
	AddClass(_T("ThunderCommandButton"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectButton));

	AddClass(_T("ThunderRT6Frame"), RUNTIME_CLASS_INFO(CXTPSkinObjectButton));
	AddClass(_T("ThunderFrame"), RUNTIME_CLASS_INFO(CXTPSkinObjectButton));

	AddClass(_T("ThunderRT6FormDC"), RUNTIME_CLASS_INFOSYSACTIVEX2(CXTPSkinObjectApplicationFrame));
	AddClass(_T("ThunderFormDC"), RUNTIME_CLASS_INFOSYSACTIVEX2(CXTPSkinObjectApplicationFrame));
	AddClass(_T("ThunderRT6MDIForm"), RUNTIME_CLASS_INFOSYSACTIVEX2(CXTPSkinObjectApplicationFrame));
	AddClass(_T("ThunderMDIForm"), RUNTIME_CLASS_INFOSYSACTIVEX2(CXTPSkinObjectApplicationFrame));


	AddClass(_T("ThunderRT6ComboBox"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectComboBox));
	AddClass(_T("ThunderComboBox"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectComboBox));

	AddClass(_T("ThunderRT6ListBox"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectListBox));
	AddClass(_T("ThunderListBox"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectListBox));

	AddClass(_T("ThunderRT6HScrollBar"), RUNTIME_CLASS_INFO(CXTPSkinObjectScrollBar));
	AddClass(_T("ThunderHScrollBar"), RUNTIME_CLASS_INFO(CXTPSkinObjectScrollBar));
	AddClass(_T("ThunderRT6VScrollBar"), RUNTIME_CLASS_INFO(CXTPSkinObjectScrollBar));
	AddClass(_T("ThunderVScrollBar"), RUNTIME_CLASS_INFO(CXTPSkinObjectScrollBar));

	AddClass(_T("ThunderRT6TextBox"), RUNTIME_CLASS_INFO(CXTPSkinObjectEdit));
	AddClass(_T("ThunderTextBox"), RUNTIME_CLASS_INFO(CXTPSkinObjectEdit));
	AddClass(_T("MSMaskWndClass"), RUNTIME_CLASS_INFO(CXTPSkinObjectEdit));

	AddClass(_T("SliderWndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectTrackBar));
	AddClass(_T("Slider20WndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectTrackBar));

	AddClass(_T("ProgressBarWndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectProgress));
	AddClass(_T("ProgressBar20WndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectProgress));

	AddClass(_T("TabStripWndClass"), RUNTIME_CLASS_INFOSYSACTIVEX(CXTPSkinObjectTab));
	AddClass(_T("TabStrip20WndClass"), RUNTIME_CLASS_INFOSYSACTIVEX(CXTPSkinObjectTab));

	AddClass(_T("ListViewWndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectListView));
	AddClass(_T("ListView20WndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectListView));

	AddClass(_T("TreeViewWndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectTreeView));
	AddClass(_T("TreeView20WndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectTreeView));

	AddClass(_T("StatusBarWndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectStatusBar));
	AddClass(_T("StatusBar20WndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectStatusBar));

	AddClass(_T("msvb_lib_toolbar"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectToolBar));

	AddClass(_T("DTPicker20WndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectDateTime));
	AddClass(_T("MonthView20WndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectMonthCal));

	AddClass(_T("UpDownWndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectSpin));
	AddClass(_T("UpDown20WndClass"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectSpin));
	AddClass(_T("msvb_lib_updown"), RUNTIME_CLASS_INFOACTIVEX(CXTPSkinObjectSpin));

#endif

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
