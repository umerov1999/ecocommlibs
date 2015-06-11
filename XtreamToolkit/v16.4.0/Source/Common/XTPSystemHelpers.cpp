// XTPSystemHelpers.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#include "XTPMacros.h"
#include "XTPSystemHelpers.h"

#ifdef _XTP_INCLUDE_GRAPHICLIBRARY
#include "GraphicLibrary/zlib/zlib.h"
#endif

#pragma warning (disable : 4996) // warning C4996: 'GetVersionExA': was declared deprecated
#pragma warning (disable : 4201)
	#include <mmsystem.h>
#pragma warning (default : 4201)

#pragma message("   Automatically linking with WinMM library")
#pragma message("      (Windows Multimedia System)")
#pragma comment(lib, "winmm.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TRACE_ACCESSIBLE(x)

#ifndef VER_EQUAL
#define VER_EQUAL                       1
#endif

#ifndef VER_GREATER_EQUAL
#define VER_GREATER_EQUAL               3
#endif

#ifndef VER_MINORVERSION
#define VER_MINORVERSION                0x0000001
#endif

#ifndef VER_MAJORVERSION
#define VER_MAJORVERSION                0x0000002
#endif

#ifndef VER_PLATFORMID
#define VER_PLATFORMID                  0x0000008
#endif

#ifndef VER_SERVICEPACKMINOR
#define VER_SERVICEPACKMINOR            0x0000010
#endif

#ifndef VER_SERVICEPACKMAJOR
#define VER_SERVICEPACKMAJOR            0x0000020
#endif

#define VER_SET_CONDITION_MASK(_m_,_t_,_c_)  \
	((_m_) = m_pfnVerSetConditionMask((_m_), (_t_), (_c_)))

//===========================================================================
// CXTPSystemVersion class
//===========================================================================

CXTPSystemVersion::CXTPSystemVersion()
	: m_pfnVerifyVersionInfo(NULL)
	, m_pfnVerSetConditionMask(NULL)
{
	// load Kernel32.dll and check for VerifyVersionInfo API existence.
	m_modKernel32.Init(TEXT("Kernel32.dll"));
	if (m_modKernel32)
	{
#ifdef UNICODE
		m_modKernel32.GetProcAddress((FARPROC*)&m_pfnVerifyVersionInfo, "VerifyVersionInfoW");
		m_modKernel32.GetProcAddress((FARPROC*)&m_pfnVerSetConditionMask, "VerSetConditionMask");
#else
		m_modKernel32.GetProcAddress((FARPROC*)&m_pfnVerifyVersionInfo, "VerifyVersionInfoA");
		m_modKernel32.GetProcAddress((FARPROC*)&m_pfnVerSetConditionMask, "VerSetConditionMask");
#endif
	}
}

LRESULT CXTPSystemVersion::VerifyVersion(BYTE dwConditionMask, DWORD dwMajorVersion, DWORD dwMinorVersion, DWORD dwPlatformId) const
{
	if (m_pfnVerifyVersionInfo && m_pfnVerSetConditionMask)
	{
		// Initialize the OSVERSIONINFOEX structure.
		OSVERSIONINFOEX osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		osvi.dwMajorVersion = dwMajorVersion;
		osvi.dwMinorVersion = dwMinorVersion;
		osvi.dwPlatformId = dwPlatformId;

		// Initialize the condition mask.
		DWORDLONG dwlConditionMask = 0;
		VER_SET_CONDITION_MASK(dwlConditionMask, VER_MAJORVERSION, dwConditionMask);
		VER_SET_CONDITION_MASK(dwlConditionMask, VER_MINORVERSION, dwConditionMask);
		VER_SET_CONDITION_MASK(dwlConditionMask, VER_PLATFORMID, dwConditionMask);

		// Perform the test.
		if (m_pfnVerifyVersionInfo(&osvi,
			VER_MAJORVERSION | VER_MINORVERSION | VER_PLATFORMID, dwlConditionMask) != 0)
		{
			return S_OK;
		}

		return S_FALSE;
	}

	return E_NOTIMPL;
}

LRESULT CXTPSystemVersion::GetVersion(BYTE dwConditionMask, DWORD dwMajorVersion, DWORD dwMinorVersion, DWORD dwPlatformId) const
{
	// Initialize the OSVERSIONINFOEX structure.
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	// get the Windows OS version information.
	::GetVersionEx(&osvi);

	if (dwConditionMask == VER_EQUAL)
	{
		if (osvi.dwPlatformId == dwPlatformId)
		{
			if (osvi.dwMajorVersion == dwMajorVersion &&
				osvi.dwMinorVersion == dwMinorVersion)
			{
				return S_OK;
			}
		}
	}

	if (dwConditionMask == VER_GREATER_EQUAL)
	{
		if (osvi.dwPlatformId >= dwPlatformId)
		{
			if (osvi.dwMajorVersion > dwMajorVersion ||
				osvi.dwMajorVersion == dwMajorVersion &&
				osvi.dwMinorVersion >= dwMinorVersion)
			{
				return S_OK;
			}
		}
	}

	return S_FALSE;
}

bool CXTPSystemVersion::VerifyWinVer(BYTE dwConditionMask, DWORD dwMajorVersion, DWORD dwMinorVersion, DWORD dwPlatformId) const
{
	LRESULT lResult = VerifyVersion(dwConditionMask, dwMajorVersion, dwMinorVersion, dwPlatformId);

	if (lResult == E_NOTIMPL)
	{
		lResult = GetVersion(dwConditionMask, dwMajorVersion, dwMinorVersion, dwPlatformId);
	}

	if (lResult == S_OK)
	{
		return true;
	}

	return false;
}

_XTP_EXT_CLASS CXTPSystemVersion* AFXAPI XTPSystemVersion()
{
	static CXTPSystemVersion instance;
	return &instance;
}

bool CXTPSystemVersion::IsWin31() const
{
	return VerifyWinVer(VER_EQUAL, 3, 10, VER_PLATFORM_WIN32s);
}

bool CXTPSystemVersion::IsWin95() const
{
	return VerifyWinVer(VER_EQUAL, 4, 0, VER_PLATFORM_WIN32_WINDOWS);
}

bool CXTPSystemVersion::IsWin98() const
{
	return VerifyWinVer(VER_EQUAL, 4, 1, VER_PLATFORM_WIN32_WINDOWS);
}

bool CXTPSystemVersion::IsWinME() const
{
	return VerifyWinVer(VER_EQUAL, 4, 90, VER_PLATFORM_WIN32_WINDOWS);
}

bool CXTPSystemVersion::IsWin9x() const
{
	return IsWin95() || IsWin98() || IsWinME();
}

bool CXTPSystemVersion::IsWinNT4() const
{
	return VerifyWinVer(VER_EQUAL, 4, 0, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWin2K() const
{
	return VerifyWinVer(VER_EQUAL, 5, 0, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWinXP() const
{
	return VerifyWinVer(VER_EQUAL, 5, 1, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWin95OrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 4, 0, VER_PLATFORM_WIN32_WINDOWS);
}

bool CXTPSystemVersion::IsWin98OrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 4, 1, VER_PLATFORM_WIN32_WINDOWS);
}

bool CXTPSystemVersion::IsWinMEOrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 4, 90, VER_PLATFORM_WIN32_WINDOWS);
}

bool CXTPSystemVersion::IsWinNT4OrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 4, 0, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWin2KOrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 5, 0, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWinXPOrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 5, 1, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWinVistaOrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 6, 0, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWin7OrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 6, 1, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWin8OrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 6, 2, VER_PLATFORM_WIN32_NT);
}

bool CXTPSystemVersion::IsWin81OrGreater() const
{
	return VerifyWinVer(VER_GREATER_EQUAL, 6, 3, VER_PLATFORM_WIN32_NT);
}

DWORD CXTPSystemVersion::GetComCtlVersion() const
{
	static DWORD dwVersion = 0;
	if (dwVersion != 0)
		return dwVersion;

	CXTPModuleHandle modComCtl32(_T("COMCTL32.DLL"));

	dwVersion = modComCtl32.GetVersion();

	if (dwVersion == NULL)
		dwVersion = MAKELONG(0, 4);   // Old ComCtl32 had version 4.0

	return dwVersion;
}

BOOL CXTPSystemVersion::IsLayoutRTLSupported() const
{
	return IsWin2KOrGreater() || GetSystemMetrics(SM_MIDEASTENABLED);
}

BOOL CXTPSystemVersion::IsClearTypeTextQualitySupported() const
{
	return IsWinXPOrGreater();
}

int CXTPSystemVersion::GetMaxCharSize() const
{
#ifdef _UNICODE
	return 1;
#else
	static int nMaxCharSize = -1;
	if (nMaxCharSize == -1)
	{
		CPINFO info;
		GetCPInfo(GetOEMCP(), &info);
		nMaxCharSize = info.MaxCharSize;
	}
	return nMaxCharSize;
#endif
}


//=============================================================================
// Multi-Monitor API
//=============================================================================


_XTP_EXT_CLASS CXTPMultiMonitor* AFX_CDECL XTPMultiMonitor()
{
	static CXTPMultiMonitor instance;
	return &instance;
}

CXTPMultiMonitor::CXTPMultiMonitor()
{
#ifdef UNICODE
	BOOL bIsPlatformNT = XTPSystemVersion()->IsWinNT4OrGreater();
#endif

	m_modUser32.Init(TEXT("User32.dll"));
	if (!m_modUser32)
	{
		TRACE(_T("WARNING: Could not locate User32.dll.\n"));
	}

	if (!m_modUser32 ||
		!m_modUser32.GetProcAddress((FARPROC*)&m_pfnGetSystemMetrics, "GetSystemMetrics") ||
		!m_modUser32.GetProcAddress((FARPROC*)&m_pfnMonitorFromWindow, "MonitorFromWindow") ||
		!m_modUser32.GetProcAddress((FARPROC*)&m_pfnMonitorFromRect, "MonitorFromRect") ||
		!m_modUser32.GetProcAddress((FARPROC*)&m_pfnMonitorFromPoint, "MonitorFromPoint") ||
#ifndef UNICODE
		!m_modUser32.GetProcAddress((FARPROC*)&m_pfnGetMonitorInfo, "GetMonitorInfoA"))
#else
		!m_modUser32.GetProcAddress((FARPROC*)&m_pfnGetMonitorInfo, bIsPlatformNT ? "GetMonitorInfoW" : "GetMonitorInfoA"))
#endif
	{
		m_pfnGetSystemMetrics   = NULL;
		m_pfnMonitorFromWindow  = NULL;
		m_pfnMonitorFromRect    = NULL;
		m_pfnMonitorFromPoint   = NULL;
		m_pfnGetMonitorInfo     = NULL;
	}
}

BOOL CXTPMultiMonitor::GetMonitorInfo(XTP_HMONITOR hMonitor, XTP_MONITORINFO* lpMonitorInfo)
{
	if (hMonitor && m_pfnGetMonitorInfo)
	{
		lpMonitorInfo->cbSize = sizeof(XTP_MONITORINFO);
		ASSERT(sizeof(XTP_MONITORINFO) == 40);

		if (m_pfnGetMonitorInfo(hMonitor, lpMonitorInfo))
			return TRUE;
	}
	return FALSE;
}

CRect CXTPMultiMonitor::GetWorkArea(XTP_HMONITOR hMonitor)
{
	XTP_MONITORINFO info;

	if (GetMonitorInfo(hMonitor, &info))
	{
		return info.rcWork;
	}

	RECT rcWork;
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcWork, 0);
	return rcWork;
}

CRect CXTPMultiMonitor::GetScreenArea(XTP_HMONITOR hMonitor)
{
	XTP_MONITORINFO info;

	if (GetMonitorInfo(hMonitor, &info))
	{
		return info.rcMonitor;
	}

	return CRect(0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
}


CRect CXTPMultiMonitor::GetScreenArea(HWND hWnd)
{
	return GetScreenArea(m_pfnMonitorFromWindow ? m_pfnMonitorFromWindow(hWnd, 0) : 0);
}

CRect CXTPMultiMonitor::GetScreenArea(const CWnd* pWnd)
{
	return GetScreenArea(pWnd->GetSafeHwnd());
}

CRect CXTPMultiMonitor::GetScreenArea(const POINT& ptScreenCoords)
{
	return GetScreenArea(m_pfnMonitorFromPoint ? m_pfnMonitorFromPoint(ptScreenCoords, 0) : 0);
}

CRect CXTPMultiMonitor::GetScreenArea(LPCRECT lprcScreenCoords)
{
	return GetScreenArea(m_pfnMonitorFromRect ? m_pfnMonitorFromRect(lprcScreenCoords, 0) : 0);
}

CRect CXTPMultiMonitor::GetWorkArea(HWND hWnd)
{
	return GetWorkArea(m_pfnMonitorFromWindow ? m_pfnMonitorFromWindow(hWnd, 0) : 0);
}

CRect CXTPMultiMonitor::GetWorkArea(const POINT& ptScreenCoords)
{
	return GetWorkArea(m_pfnMonitorFromPoint ? m_pfnMonitorFromPoint(ptScreenCoords, 0) : 0);
}

CRect CXTPMultiMonitor::GetWorkArea(LPCRECT lprcScreenCoords)
{
	return GetWorkArea(m_pfnMonitorFromRect ? m_pfnMonitorFromRect(lprcScreenCoords, 0) : 0);
}

CRect CXTPMultiMonitor::GetWorkArea(const CWnd* pWnd)
{
	return GetWorkArea(pWnd->GetSafeHwnd());
}

CRect CXTPMultiMonitor::GetWorkArea()
{
	POINT point;
	::GetCursorPos(&point);

	return GetWorkArea(point);
}


//=============================================================================
// CXTPModuleHandle :
//=============================================================================

CXTPModuleHandle::CXTPModuleHandle()
	: m_hModule(0)
	, m_eModuleState(xtpModNone)
{
	::ZeroMemory(&m_dvInfo, sizeof(m_dvInfo));
}

CXTPModuleHandle::CXTPModuleHandle(LPCTSTR lpszModuleName)
	: m_hModule(0)
	, m_eModuleState(xtpModNone)
{
	::ZeroMemory(&m_dvInfo, sizeof(m_dvInfo));

	Init(lpszModuleName);
}

CXTPModuleHandle::~CXTPModuleHandle()
{
	VERIFY(FreeLibrary());
}

BOOL CXTPModuleHandle::Init(LPCTSTR lpszModuleName)
{
	// first, try to obtain the module that was mapped into
	// the address space of the calling process, if not found,
	// attempt to load the library.

	return (GetModuleHandle(lpszModuleName) || LoadLibrary(lpszModuleName));
}

BOOL CXTPModuleHandle::GetModuleHandle(LPCTSTR lpszModuleName)
{
	FreeLibrary();

	if ((m_hModule = ::GetModuleHandle(lpszModuleName)) != NULL)
	{
		m_eModuleState = xtpModMapped;
		m_strModuleName = lpszModuleName;
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPModuleHandle::LoadLibrary(LPCTSTR lpszModuleName)
{
	FreeLibrary();

	if ((m_hModule = ::LoadLibrary(lpszModuleName)) != NULL)
	{
		m_eModuleState = xtpModLoaded;
		m_strModuleName = lpszModuleName;
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPModuleHandle::FreeLibrary()
{
	BOOL bRet = TRUE;

	if (m_hModule && m_eModuleState == xtpModLoaded)
		bRet = ::FreeLibrary(m_hModule);

	m_hModule = NULL;
	m_eModuleState = xtpModNone;
	m_strModuleName.Empty();

	return bRet;
}

BOOL CXTPModuleHandle::GetVersionInfo()
{
	if (!m_hModule)
		return FALSE;

	m_dvInfo.cbSize = sizeof(m_dvInfo);

	typedef HRESULT (WINAPI* PFNDLLVERSIONINFO)(XTP_DLLVERSIONINFO*);
	PFNDLLVERSIONINFO pfn = (PFNDLLVERSIONINFO)::GetProcAddress(m_hModule, "DllGetVersion");

	if (pfn != NULL)
	{
		return (pfn(&m_dvInfo) != NOERROR);
	}

	return FALSE;
}

DWORD CXTPModuleHandle::GetVersion()
{
	if (m_dvInfo.cbSize == 0)
		GetVersionInfo();

	DWORD dwModVer = MAKELONG(m_dvInfo.dwMinorVersion, m_dvInfo.dwMajorVersion);

	ASSERT(HIWORD(dwModVer) <= 0xFFFF);
	ASSERT(LOWORD(dwModVer) <= 0xFFFF);

	return dwModVer;
}

BOOL CXTPModuleHandle::GetProcAddress(FARPROC* ppFnPtr, LPCSTR lpProcName, DWORD dwMinVer /*= NULL*/)
{
	if (!m_hModule)
		return FALSE;

	if (dwMinVer != NULL)
	{
		DWORD dwModVer = GetVersion();

		if ((HIWORD(dwMinVer) >  HIWORD(dwModVer)) ||
			(HIWORD(dwMinVer) == HIWORD(dwModVer)) &&
			(LOWORD(dwMinVer) >  LOWORD(dwModVer)))
		{
			*ppFnPtr = NULL;
			return FALSE;
		}
	}

	*ppFnPtr = ::GetProcAddress(m_hModule, lpProcName);
	return (*ppFnPtr != NULL);
}


//=============================================================================
// CXTPCriticalSection
//=============================================================================

CXTPCriticalSection::CXTPCriticalSection()
{
	::InitializeCriticalSection(&m_csMutex);
}

CXTPCriticalSection::~CXTPCriticalSection()
{
	::DeleteCriticalSection(&m_csMutex);
}

void CXTPCriticalSection::EnterCritical()
{
	::EnterCriticalSection(&m_csMutex);
}

void CXTPCriticalSection::LeaveCritical()
{
	::LeaveCriticalSection(&m_csMutex);
}

//=============================================================================
// CXTPLockGuard
//=============================================================================

CXTPLockGuard::CXTPLockGuard(CRITICAL_SECTION& key)
: m_key(key)
{
	LockThread();
}

CXTPLockGuard::~CXTPLockGuard()
{
	UnLockThread();
}

void CXTPLockGuard::LockThread()
{
	::EnterCriticalSection(&m_key);
}

void CXTPLockGuard::UnLockThread()
{
	::LeaveCriticalSection(&m_key);
}


//////////////////////////////////////////////////////////////////////////
// CXTPAccessible


CXTPAccessible::CXTPAccessible()
{
	m_pNotifyWinEvent = NULL;
	m_pLresultFromObject = NULL;
	m_pAccessibleObjectFromWindow = NULL;

	m_hOleAcc = NULL;
	m_hUser32 = NULL;
}

CXTPAccessible::~CXTPAccessible()
{
	if (m_hOleAcc && m_hOleAcc != INVALID_HANDLE_VALUE)
	{
		FreeLibrary(m_hOleAcc);
	}
}

void CXTPAccessible::AccessibleNotifyWinEvent(DWORD event, HWND hwnd, LONG idObject, LONG idChild)
{
	if (m_hUser32 == INVALID_HANDLE_VALUE)
		return;

	if (m_hUser32 == NULL)
	{
		m_hUser32 = GetModuleHandle(_T("user32.dll"));
	}

	if (m_pNotifyWinEvent == NULL && m_hUser32)
	{
		m_pNotifyWinEvent = (LPFNNOTIFYWINEVENT)GetProcAddress(m_hUser32, "NotifyWinEvent");

		if (!m_pNotifyWinEvent)
		{
			m_hUser32 = (HMODULE)INVALID_HANDLE_VALUE;
		}
	}

	if (m_pNotifyWinEvent != NULL)
	{
		m_pNotifyWinEvent(event, hwnd, idObject, idChild);
	}
}

HRESULT CXTPAccessible::AccessibleObjectFromWindow(HWND hwnd, DWORD dwId, REFIID riid, void** ppvObject)
{
	if (m_hOleAcc == INVALID_HANDLE_VALUE)
		return E_FAIL;

	if (m_hOleAcc == NULL)
	{
		m_hOleAcc = LoadLibrary(_T("oleacc.dll"));
	}

	if (m_hOleAcc != NULL && m_pAccessibleObjectFromWindow == NULL)
	{
		m_pAccessibleObjectFromWindow = (LPFNACCESSIBLEOBJECTFROMWINDOW)GetProcAddress(m_hOleAcc, "AccessibleObjectFromWindow");

		if (!m_pAccessibleObjectFromWindow)
		{
			FreeLibrary(m_hOleAcc);

			m_hOleAcc = (HMODULE)INVALID_HANDLE_VALUE;
			m_pLresultFromObject = NULL;
			m_pAccessibleObjectFromWindow = NULL;
		}
	}

	if (m_pAccessibleObjectFromWindow)
	{
		return m_pAccessibleObjectFromWindow(hwnd, dwId, riid, ppvObject);
	}
	return E_FAIL;
}

LRESULT CXTPAccessible::LresultFromObject(REFIID riid, WPARAM wParam, LPUNKNOWN punk)
{
	if (m_hOleAcc == INVALID_HANDLE_VALUE)
		return E_FAIL;

	if (m_hOleAcc == NULL)
	{
		m_hOleAcc = LoadLibrary(_T("oleacc.dll"));
	}

	if (m_hOleAcc != NULL && m_pLresultFromObject == NULL)
	{
		m_pLresultFromObject = (LPFNLRESULTFROMOBJECT)GetProcAddress(m_hOleAcc, "LresultFromObject");

		if (!m_pLresultFromObject)
		{
			FreeLibrary(m_hOleAcc);

			m_hOleAcc = (HMODULE)INVALID_HANDLE_VALUE;
			m_pLresultFromObject = NULL;
			m_pAccessibleObjectFromWindow = NULL;
		}
	}

	if (m_pLresultFromObject)
	{
		return m_pLresultFromObject(riid, wParam, punk);
	}

	return E_FAIL;
}

long CXTPAccessible::GetChildIndex(VARIANT* varChild)
{
	if (!varChild) return -1;
	if (varChild->vt == VT_EMPTY)
		return CHILDID_SELF;

	if (varChild->vt == VT_I4) return V_I4(varChild);
	if (varChild->vt == VT_I2) return V_I2(varChild);

	if (varChild->vt == (VT_I4 | VT_BYREF) && V_I4REF(varChild)) return *V_I4REF(varChild);
	if (varChild->vt == (VT_I2 | VT_BYREF) && V_I2REF(varChild)) return *V_I2REF(varChild);

	if (varChild->vt == (VT_VARIANT | VT_BYREF)) return GetChildIndex(varChild->pvarVal);
	return -1;
}

STDMETHODIMP_(ULONG) CXTPAccessible::XAccessible::AddRef()
{
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible)
	return (ULONG)pThis->GetAccessible()->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CXTPAccessible::XAccessible::Release()
{
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible)
	return (ULONG)pThis->GetAccessible()->ExternalRelease();
}

STDMETHODIMP CXTPAccessible::XAccessible::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible)
	return (HRESULT)pThis->GetAccessible()->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CXTPAccessible::XAccessible::GetTypeInfoCount(
	unsigned int*)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPAccessible::XAccessible::GetTypeInfo(
	unsigned int, LCID, ITypeInfo**)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPAccessible::XAccessible::GetIDsOfNames(
	REFIID, LPOLESTR*, unsigned int, LCID, DISPID*)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accParent(IDispatch* FAR* ppdispParent)
{
	TRACE_ACCESSIBLE(_T("get_accParent\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleParent(ppdispParent);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accChildCount(long FAR* pChildCount)
{
	TRACE_ACCESSIBLE(_T("get_accChildCount\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleChildCount(pChildCount);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accChild(VARIANT varChild, IDispatch* FAR* ppdispChild)
{
	TRACE_ACCESSIBLE(_T("get_accChild\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleChild(varChild, ppdispChild);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accName(VARIANT varChild, BSTR* pszName)
{
	TRACE_ACCESSIBLE(_T("get_accName\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleName(varChild, pszName);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accValue(VARIANT varChild, BSTR* pszValue)
{
	TRACE_ACCESSIBLE(_T("get_accValue\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleValue(varChild, pszValue);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accDescription(VARIANT varChild, BSTR FAR* pszDescription)
{
	TRACE_ACCESSIBLE(_T("get_accDescription\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleDescription(varChild, pszDescription);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accRole(VARIANT varChild, VARIANT* pvarRole)
{
	TRACE_ACCESSIBLE(_T("get_accRole\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleRole(varChild, pvarRole);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accState(VARIANT varChild, VARIANT* pvarState)
{
	TRACE_ACCESSIBLE(_T("get_accState\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleState(varChild, pvarState);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accHelp(VARIANT varChild, BSTR* pszHelp)
{
	TRACE_ACCESSIBLE(_T("get_accHelp\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleHelp(varChild, pszHelp);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accHelpTopic(BSTR* pszHelpFile, VARIANT varChild, long* pidTopic)
{
	TRACE_ACCESSIBLE(_T("get_accHelpTopic\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleHelpTopic(pszHelpFile, varChild, pidTopic);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accKeyboardShortcut(VARIANT varChild, BSTR* pszKeyboardShortcut)
{
	TRACE_ACCESSIBLE(_T("get_accKeyboardShortcut\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleKeyboardShortcut(varChild, pszKeyboardShortcut);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accFocus(VARIANT FAR* pvarFocusChild)
{
	TRACE_ACCESSIBLE(_T("get_accFocus\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleFocus(pvarFocusChild);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accSelection(VARIANT FAR* pvarSelectedChildren)
{
	TRACE_ACCESSIBLE(_T("get_accSelection\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleSelection(pvarSelectedChildren);
}

STDMETHODIMP CXTPAccessible::XAccessible::get_accDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
{
	TRACE_ACCESSIBLE(_T("get_accDefaultAction\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->GetAccessibleDefaultAction(varChild, pszDefaultAction);
}

STDMETHODIMP CXTPAccessible::XAccessible::accSelect(long flagsSelect, VARIANT varChild)
{
	TRACE_ACCESSIBLE(_T("accSelect\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->AccessibleSelect(flagsSelect, varChild);
}

STDMETHODIMP CXTPAccessible::XAccessible::accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	TRACE_ACCESSIBLE(_T("accLocation\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->AccessibleLocation(pxLeft, pyTop, pcxWidth, pcyHeight, varChild);
}

STDMETHODIMP CXTPAccessible::XAccessible::accNavigate(long navDir, VARIANT varStart, VARIANT* pvarEndUpAt)
{
	TRACE_ACCESSIBLE(_T("accNavigate\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->AccessibleNavigate(navDir, varStart, pvarEndUpAt);
}

STDMETHODIMP CXTPAccessible::XAccessible::accHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	TRACE_ACCESSIBLE(_T("accHitTest\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->AccessibleHitTest(xLeft, yTop, pvarID);
}

STDMETHODIMP CXTPAccessible::XAccessible::accDoDefaultAction(VARIANT varChild)
{
	TRACE_ACCESSIBLE(_T("accDoDefaultAction\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->AccessibleDoDefaultAction(varChild);
}

STDMETHODIMP CXTPAccessible::XAccessible::put_accName(VARIANT varChild, BSTR szName)
{
	TRACE_ACCESSIBLE(_T("put_accName\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->PutAccessibleName(varChild, szName);
}

STDMETHODIMP CXTPAccessible::XAccessible::put_accValue(VARIANT varChild, BSTR pszValue)
{
	TRACE_ACCESSIBLE(_T("put_accValue\n"));
	METHOD_PROLOGUE_EX_(CXTPAccessible, ExternalAccessible);

	return pThis->PutAccessibleValue(varChild, pszValue);
}

STDMETHODIMP CXTPAccessible::XAccessible::Invoke(
	DISPID /*dispid*/, REFIID, LCID, unsigned short /*wFlags*/,
	DISPPARAMS* /*pDispParams*/, VARIANT* /*pvarResult*/,
	EXCEPINFO*, unsigned int*)
{
	return E_NOTIMPL;
}


HRESULT CXTPAccessible::GetAccessibleParent(IDispatch**  /*ppdispParent*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleChildCount(long* /*pcountChildren*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleChild(VARIANT /*varChild*/, IDispatch** /*ppdispChild*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleName(VARIANT /*varChild*/, BSTR* /*pszName*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleValue(VARIANT /*varChild*/, BSTR* /*pszValue*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleDescription(VARIANT /*varChild*/, BSTR* /*pszDescription*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleRole(VARIANT /*varChild*/, VARIANT* /*pvarRole*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleState(VARIANT /*varChild*/, VARIANT* /*pvarState*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleHelp(VARIANT /*varChild*/, BSTR* /*pszHelp*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleHelpTopic(BSTR* /*pszHelpFile*/, VARIANT /*varChild*/, long* /*pidTopic*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleKeyboardShortcut(VARIANT /*varChild*/, BSTR* /*pszKeyboardShortcut*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::GetAccessibleFocus(VARIANT* /*pvarChild*/)
{
	return S_FALSE;
}
HRESULT CXTPAccessible::GetAccessibleSelection(VARIANT* /*pvarChildren*/)
{
	return S_FALSE;
}
HRESULT CXTPAccessible::GetAccessibleDefaultAction(VARIANT /*varChild*/, BSTR* /*pszDefaultAction*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::AccessibleSelect(long /*flagsSelect*/, VARIANT /*varChild*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::AccessibleLocation(long* /*pxLeft*/, long* /*pyTop*/, long* /*pcxWidth*/, long* /*pcyHeight*/, VARIANT /*varChild*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::AccessibleNavigate(long /*navDir*/, VARIANT /*varStart*/, VARIANT* /*pvarEndUpAt*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::AccessibleHitTest(long /*xLeft*/, long /*yTop*/, VARIANT* /*pvarChild*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::AccessibleDoDefaultAction(VARIANT /*varChild*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::PutAccessibleName(VARIANT /*varChild*/, BSTR /*szName*/)
{
	return E_NOTIMPL;
}
HRESULT CXTPAccessible::PutAccessibleValue(VARIANT /*varChild*/, BSTR /*szValue*/)
{
	return E_NOTIMPL;
}
CCmdTarget* CXTPAccessible::GetAccessible()
{
	ASSERT(FALSE);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPSoundManager

CXTPSoundManager::CXTPSoundManager()
{
	m_hThread = 0;
	m_dwThreadId = 0;
	m_bSystemSounds = TRUE;
	m_bSoundAvailable = TRUE;
	m_hThreadStartEvent = NULL;
}

CXTPSoundManager* AFX_CDECL XTPSoundManager()
{
	static CXTPSoundManager s_managerInstance;
	return &s_managerInstance;
}

CXTPSoundManager::~CXTPSoundManager()
{
	StopThread();
}

DWORD WINAPI CXTPSoundManager::SoundThreadProc(LPVOID lp)
{
	CXTPSoundManager* pSoundManager = (CXTPSoundManager*)lp;

	// Create thread message queue.
	MSG msg;
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	SetEvent(pSoundManager->m_hThreadStartEvent);

	while (TRUE)
	{
		BOOL bQuit = FALSE;
		XTPSoundManagerState soundState = pSoundManager->FetchSoundState(bQuit);
		if (!bQuit)
		{
			switch (soundState)
			{
			case xtpSoundMenuCommand:
				::PlaySound (_T("MenuCommand"), NULL, (SND_ASYNC | SND_NODEFAULT | SND_ALIAS));
				break;

			case xtpSoundMenuPopup:
				::PlaySound (_T("MenuPopup"), NULL, (SND_ASYNC | SND_NODEFAULT | SND_ALIAS));
				break;
			}
		}
		else
		{
			break;
		}
	}

	::PlaySound(NULL, NULL, SND_PURGE);

	return 0;
}

XTPSoundManagerState CXTPSoundManager::FetchSoundState(BOOL& bQuit)
{
	XTPSoundManagerState soundState = xtpSoundNone;
	bQuit = FALSE;

	MSG msg;
	if(GetMessage(&msg, NULL, 0, 0))
	{
		if (WM_USER == msg.message)
		{
			soundState = static_cast<XTPSoundManagerState>(msg.wParam);
		}
	}
	else
	{
		bQuit = TRUE;
	}

	return soundState;
}

void CXTPSoundManager::StartThread()
{
	ASSERT (m_hThread == NULL);

	m_hThread = ::CreateThread(NULL, 0, SoundThreadProc, this, CREATE_SUSPENDED, &m_dwThreadId);
	if (m_hThread != NULL)
	{
		m_hThreadStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		::SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);
		::ResumeThread(m_hThread);

		// Make sure thread is properly started.
		WaitForSingleObject(m_hThreadStartEvent, INFINITE);
		CloseHandle(m_hThreadStartEvent);
		m_hThreadStartEvent = m_hThreadStartEvent;
	}
}

void CXTPSoundManager::StopThread()
{
	if (NULL != m_hThread)
	{
		PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);
		::WaitForSingleObject(m_hThread, INFINITE);

		::CloseHandle(m_hThread);
		m_hThread = 0;
		m_dwThreadId = 0;
	}
}

void CXTPSoundManager::PlaySystemSound(XTPSoundManagerState state)
{
	if (m_bSystemSounds && m_bSoundAvailable)
	{
		if (NULL == m_hThread)
		{
			StartThread();
		}

		if (m_hThread != NULL)
		{
			PostThreadMessage(m_dwThreadId, WM_USER, static_cast<WPARAM>(state), 0);
		}
	}
}
void CXTPSoundManager::EnableSystemSounds(BOOL bEnable)
{
	m_bSystemSounds = bEnable;
}

//////////////////////////////////////////////////////////////////////////
// XTPGetThread

CWinThread* AFX_CDECL XTPGetThread()
{
	CWinThread* pThread = AfxGetThread();
	if (pThread != NULL)
		return pThread;

	return AfxGetApp();
}

//////////////////////////////////////////////////////////////////////////
// CXTPZipMemFile

#define ZIP_HEADER ('Z' + ('I' << 8) + ('P' << 16) + ('!' << 24))


CXTPZipMemFile::CXTPZipMemFile()
{
}

CXTPZipMemFile::CXTPZipMemFile(BYTE* lpBuffer, UINT nBufferSize)
{
	AttachCompressedBuffer(lpBuffer, nBufferSize);
}

CXTPZipMemFile::~CXTPZipMemFile()
{
}

void CXTPZipMemFile::AttachCompressedBuffer(BYTE* lpBuffer, UINT nBufferSize, BOOL bAutoDelete)
{
#ifdef ZLIB_H
	ZIPBUFFERINFO* pbi = (ZIPBUFFERINFO*)lpBuffer;

	if (nBufferSize < sizeof(ZIPBUFFERINFO) ||
		pbi->bh.dwType != ZIP_HEADER)
	{
		Attach(lpBuffer, nBufferSize);
		m_bAutoDelete = bAutoDelete;
		return;
	}

	DWORD dwDestCount = pbi->bh.dwSize;

	BYTE* lpBufferDest = (BYTE*)malloc(dwDestCount);
	uncompress(lpBufferDest, &dwDestCount, (LPBYTE)&pbi->bBuffer, nBufferSize - sizeof(ZIPBUFFERHEADER));

	ASSERT(dwDestCount == pbi->bh.dwSize);

	Attach(lpBufferDest, dwDestCount);
	m_bAutoDelete = TRUE;

	if (bAutoDelete)
	{
		free(lpBuffer);
	}
#else
	Attach(lpBuffer, nBufferSize);
	m_bAutoDelete = bAutoDelete;
#endif
}

BOOL CXTPZipMemFile::OpenCompressedFile(LPCTSTR lpszFileName)
{
	CFile file;
	if (!file.Open(lpszFileName, CFile::modeRead))
		return FALSE;

	UINT nSize = (UINT)file.GetLength();
	LPBYTE lpBuffer = (LPBYTE)malloc(nSize);
	file.Read(lpBuffer, nSize);

	file.Close();

	AttachCompressedBuffer(lpBuffer, nSize, TRUE);

	return TRUE;
}

void CXTPZipMemFile::DetachCompressedBuffer(BYTE*& lpBuffer, DWORD& dwCount)
{
	dwCount = (DWORD)GetPosition();
	lpBuffer = Detach();

#ifdef ZLIB_H
	if (dwCount < 20)
		return;

	ZIPBUFFERINFO* pbi = (ZIPBUFFERINFO*)malloc(dwCount + sizeof(ZIPBUFFERHEADER));
	if (!pbi)
		return;

	DWORD dwDestCount = dwCount;

	if (compress((LPBYTE)&pbi->bBuffer, &dwDestCount, lpBuffer, dwCount) != Z_OK)
	{
		free(pbi);
		return;
	}

	pbi->bh.dwType = ZIP_HEADER;
	pbi->bh.dwSize = dwCount;

	free(lpBuffer);
	dwCount = dwDestCount + sizeof(ZIPBUFFERHEADER);
	lpBuffer = (LPBYTE)pbi;
#endif
}
