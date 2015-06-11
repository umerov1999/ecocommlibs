// XTPSkinManagerApiHook.cpp: implementation of the CXTPSkinManagerApiHook class.
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

#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

#include <Common/XTPVC80Helpers.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSynchro.h>

#include "XTPSkinManager.h"
#include "XTPSkinManagerSchema.h"
#include "XTPSkinObject.h"
#include "XTPSkinObjectFrame.h"
#include "XTPSkinObjectMenu.h"
#include "XTPSkinDrawTools.h"
#include "XTPSkinManagerApiHook.h"
#include "XTPSkinManagerModuleList.h"
#include "XTPSkinManagerResource.h"

#ifdef _UNICODE
	#define lpszSystemParametersInfo "SystemParametersInfoW"
#else
	#define lpszSystemParametersInfo "SystemParametersInfoA"
#endif


int XTPCompareStringNoCase(const char* dst, const char* src)
{
	int f, l;
	do
	{
		f = (unsigned char)(*(dst++));
		if ((f >= 'A') && (f <= 'Z'))
			f -= ('A' - 'a');

		l = (unsigned char)(*(src++));
		if ((l >= 'A') && (l <= 'Z'))
			l -= ('A' - 'a');
	}
	while ( f && (f == l) );

	return (f - l);
}

int XTPCompareStringNoCase(const TCHAR* dst, const TCHAR* src, int len)
{
	int f, l;
	do
	{
		f = (TCHAR)(*(dst++));
		if ((f >= 'A') && (f <= 'Z'))
			f -= ('A' - 'a');

		l = (TCHAR)(*(src++));
		if ((l >= 'A') && (l <= 'Z'))
			l -= ('A' - 'a');

		len--;
	}
	while ( f && (f == l) && len != 0);

	return (f - l);
}

CXTPSkinManagerApiHook::CXTPSkinManagerApiHook()
{
	m_bInitialized = FALSE;
	ZeroMemory(&m_arrFunctions, sizeof(m_arrFunctions));
	m_nLockCondition = 0;

	m_bInAppCompatMode = (XTPSystemVersion()->IsWin7OrGreater()
		&& NULL != GetModuleHandle(_T("APPHELP.DLL")));

	ExcludeModule(_T("SHLWAPI.DLL"), TRUE);
	ExcludeModule(_T("COMCTL32.DLL"), TRUE);
	ExcludeModule(_T("KERNEL32.DLL"), FALSE);
	ExcludeModule(_T("USER32.DLL"), FALSE);
	ExcludeModule(_T("GDI32.DLL"), FALSE);
	ExcludeModule(_T("WININET.DLL"), FALSE);
	ExcludeModule(_T("MSCTF.DLL"), FALSE);
}

CXTPSkinManagerApiHook::~CXTPSkinManagerApiHook()
{
	FinalizeHookManagement();
	UnhookAllFunctions(TRUE);
}

void CXTPSkinManagerApiHook::ConnectWrapper(CXTPWinThemeWrapper* pThemeWrapper)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	pThemeWrapper->CreateSharedData();
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperOpenThemeData, &CXTPSkinManagerApiHook::OnHookOpenThemeData);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperDrawThemeBackground, &CXTPSkinManagerApiHook::OnHookDrawThemeBackground);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperCloseThemeData, &CXTPSkinManagerApiHook::OnHookCloseThemeData);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperGetThemeColor, &CXTPSkinManagerApiHook::OnHookGetThemeColor);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperGetThemeInt, &CXTPSkinManagerApiHook::OnHookGetThemeInt);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperIsAppThemed, &CXTPSkinManagerApiHook::OnHookIsAppThemed);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperIsThemeActive, &CXTPSkinManagerApiHook::OnHookIsThemeActive);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperGetCurrentThemeName, &CXTPSkinManagerApiHook::OnHookGetCurrentThemeName);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperGetThemeSysBool, &CXTPSkinManagerApiHook::OnHookGetThemeSysBool);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperGetThemeSysColor, &CXTPSkinManagerApiHook::OnHookGetThemeSysColor);
	pThemeWrapper->SetThemeWrapperPtr(xtpWrapperGetThemePartSize, &CXTPSkinManagerApiHook::OnHookGetThemePartSize);

}


CXTPSkinManagerApiHook* CXTPSkinManagerApiHook::GetInstance()
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);
	return CXTPSkinManager::s_pInstance->m_pApiHook;
}

void CXTPSkinManagerApiHook::ExcludeModule(LPCTSTR lpszModule, BOOL bWin9x)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	EXCLUDEDMODULE em;
	em.bWin9x = bWin9x;
	em.strModule = lpszModule;
	em.hModule = 0;
	XTPAccessExclusive(this)->m_arrExcludedModules.Add(em);
}

BOOL CXTPSkinManagerApiHook::IsModuleExcluded(HMODULE hModule) const
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	XTP_GUARD_SHARED_CONST(CXTPSkinManagerApiHook, this)
	{
		BOOL bWin9x = XTPSkinManager()->IsWin9x();

		for (int i = 0; i < (int)m_arrExcludedModules.GetSize(); i++)
		{
			const EXCLUDEDMODULE& em = m_arrExcludedModules[i];

			if (em.bWin9x && !bWin9x)
				continue;

			if (em.hModule == hModule)
				return TRUE;

			if (em.strModule[em.strModule.GetLength() - 1] == '*')
			{
				TCHAR fn[MAX_PATH];
				GetModuleFileName(hModule, fn, MAX_PATH);

				TCHAR* lpszName = _tcsrchr(fn, '\\');
				if (lpszName == NULL)
					lpszName = fn;
				else
					lpszName++;

				if (XTPCompareStringNoCase(lpszName, em.strModule, em.strModule.GetLength() - 1) == 0)
					return TRUE;

			}

		}
	}

	return FALSE;
}


void CXTPSkinManagerApiHook::UnhookAllFunctions(BOOL bFinal)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	XTP_GUARD_EXCLUSIVE(CXTPSkinManagerApiHook, this)
	{
		for (int i = 0; i < XTP_SKIN_APIHOOKCOUNT; i++)
		{
			CXTPSkinManagerApiFunction*& pHook = m_arrFunctions[i];
			if (NULL != pHook)
			{
				XTP_GUARD_EXCLUSIVE(CXTPSkinManagerApiFunction, pHook)
				{
					if (pHook->IsHooked())
					{
						pHook->UnhookImport();
					}
				}
				
				if (bFinal)
				{
					delete pHook;
					pHook = NULL;
				}
			}
		}
	}
}

CXTPSkinManagerApiFunction* CXTPSkinManagerApiHook::HookImport(XTPSkinFrameworkApiFunctionIndex nIndex, LPCSTR pszCalleeModName, LPCSTR pszFuncName, PROC  pfnHook)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	CXTPSkinManagerApiFunction* pResult = NULL;

	try
	{
		if (XTPCompareStringNoCase(pszCalleeModName, "UXTHEME.DLL") == 0)
		{
			return AddHook(nIndex, pszCalleeModName, pszFuncName, NULL, pfnHook);
		}

		PROC pfnOrig = GetProcAddressWindows(::GetModuleHandleA(pszCalleeModName), pszFuncName);

		if (NULL == pfnOrig)
		{
			HMODULE hmod = ::LoadLibraryA(pszCalleeModName);
			if (hmod)
			{
				pfnOrig = GetProcAddressWindows(::GetModuleHandleA(pszCalleeModName), pszFuncName);
			}
		}

		if (pfnOrig)
		{
			pResult = AddHook(nIndex, pszCalleeModName, pszFuncName, pfnOrig, pfnHook);
		}
	}
	catch(...)
	{

	}

	return pResult;
}

CXTPSkinManagerApiFunction* CXTPSkinManagerApiHook::AddHook(XTPSkinFrameworkApiFunctionIndex nIndex, LPCSTR pszCalleeModName, LPCSTR pszFuncName, PROC pfnOrig, PROC  pfnHook)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	ASSERT(nIndex < XTP_SKIN_APIHOOKCOUNT);
	if (nIndex >= XTP_SKIN_APIHOOKCOUNT)
		return NULL;

	XTP_GUARD_EXCLUSIVE(CXTPSkinManagerApiHook, this)
	{
		CXTPSkinManagerApiFunction* pHook = m_arrFunctions[nIndex];
		if (NULL == pHook)
		{
			// Starting from Win7 some functions can be already hooked by
			// apphelp.dll but not everywhere, in this case we need to find
			// initial function address manually and use it as an alternative
			// original function address.
			PROC pfnAltOrig = NULL;
			if (m_bInAppCompatMode && IsHookedByAppHelpDll(pfnOrig))
			{
				HMODULE hColleeModule = GetModuleHandleA(pszCalleeModName);
				if (NULL != hColleeModule)
				{
					pfnAltOrig = GetProcAddressManual(hColleeModule, pszFuncName);
				}
			}

			pHook = new CXTPSkinManagerApiFunction(this, pszCalleeModName, pszFuncName, pfnOrig, pfnHook, pfnAltOrig);
			m_arrFunctions[nIndex] = pHook;
		}
		
		pHook->HookImport();

		return pHook;
	}
}

FARPROC WINAPI CXTPSkinManagerApiHook::GetProcAddressWindows(HMODULE hModule, LPCSTR pszProcName)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	PROC pfnOrig = GetOriginalProc(xtpSkinApiGetProcAddress);

	if (pfnOrig)
	{
		typedef FARPROC(WINAPI* LPFNGETPROCADDRESS)(HMODULE hModule, LPCSTR pszProcName);
		return ((LPFNGETPROCADDRESS)pfnOrig)(hModule, pszProcName);
	}

	return ::GetProcAddress(hModule, pszProcName);
}

FARPROC WINAPI CXTPSkinManagerApiHook::GetProcAddressManual(HMODULE hModule, LPCSTR pszProcName)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	LPVOID pFunction = NULL;
	PBYTE pModuleStart = reinterpret_cast<PBYTE>(hModule);

	PIMAGE_DOS_HEADER pDosHdr = reinterpret_cast<PIMAGE_DOS_HEADER>(pModuleStart);
	if (pDosHdr->e_magic == IMAGE_DOS_SIGNATURE)
	{
		PIMAGE_NT_HEADERS pNtHdr = reinterpret_cast<PIMAGE_NT_HEADERS>(pModuleStart + pDosHdr->e_lfanew);
		if (pNtHdr->Signature == IMAGE_NT_SIGNATURE)
		{
			if (pNtHdr->OptionalHeader.DataDirectory[0].VirtualAddress != 0)
			{
				PIMAGE_EXPORT_DIRECTORY pExportDir = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(
					pModuleStart + pNtHdr->OptionalHeader.DataDirectory[0].VirtualAddress);
				for (DWORD i = 0; i < pExportDir->NumberOfNames - 1; ++i)
				{
					DWORD_PTR address = reinterpret_cast<DWORD_PTR>(pModuleStart + (pExportDir->AddressOfNames + (i * sizeof(DWORD))));
					LPCSTR name = reinterpret_cast<LPCSTR>(pModuleStart + *reinterpret_cast<PDWORD>(address));
					if (0 == strcmp(name, pszProcName))
					{
						address = pExportDir->AddressOfNameOrdinals + (i * sizeof(WORD));
						DWORD ordinal = *reinterpret_cast<PDWORD>(pModuleStart + address) & 0xffff;
						address = pExportDir->AddressOfFunctions + (ordinal * sizeof(DWORD));
						pFunction = pModuleStart + *reinterpret_cast<PDWORD>(pModuleStart + address);
						if ((pModuleStart + pNtHdr->OptionalHeader.DataDirectory[0].VirtualAddress) < pFunction)
						{
							if (pFunction < (pModuleStart 
								+ pNtHdr->OptionalHeader.DataDirectory[0].VirtualAddress 
								+ pNtHdr->OptionalHeader.DataDirectory[0].Size))
							{
								// Routine is forwarded
								LPCSTR functionName = reinterpret_cast<LPCSTR>(pFunction);
								LPCSTR forwardedName = strstr(functionName, ".") + 1;
								DWORD_PTR diffSize = forwardedName - functionName;
								char* libName = new char[diffSize];
								lstrcpynA(libName, functionName, static_cast<int>(diffSize));
								HMODULE library = GetModuleHandleA(libName);
								if(NULL != library)
								{
									pFunction = GetProcAddressManual(library, forwardedName);
								}
								delete [] libName;
							}
						}
					}
				}
			}
		}
	}

	return reinterpret_cast<FARPROC>(pFunction);
}

LRESULT WINAPI CXTPSkinManagerApiHook::CallWindowProcOrig(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	XTP_SKINFRAMEWORK_ASSERT_WINDOW_THREAD(hWnd);

#ifdef _UNICODE
	PROC pfnOrig = GetOriginalProc(xtpSkinApiCallWindowProcW);
#else
	PROC pfnOrig = GetOriginalProc(xtpSkinApiCallWindowProcA);
#endif

	if (pfnOrig)
	{
		typedef LRESULT(WINAPI* LPFNCALLWINDOWPROC)(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNCALLWINDOWPROC)pfnOrig)(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
	}

	return ::CallWindowProc(lpPrevWndFunc, hWnd, Msg, wParam, lParam);
}

CXTPSkinManagerApiFunction* CXTPSkinManagerApiHook::GetHookedFunction(XTPSkinFrameworkApiFunctionIndex nIndex)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	ASSERT(nIndex < XTP_SKIN_APIHOOKCOUNT);
	if (nIndex < 0 || nIndex >= XTP_SKIN_APIHOOKCOUNT)
		return NULL;

	return XTPAccessShared(this, m_nLockCondition)->m_arrFunctions[nIndex];
}

PROC AFX_CDECL CXTPSkinManagerApiHook::GetOriginalProc(XTPSkinFrameworkApiFunctionIndex nIndex)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	CXTPSkinManagerApiFunction* pFunction = XTPAccessShared(GetInstance(), 
		GetInstance()->m_nLockCondition)->GetHookedFunction(nIndex);
	if (!pFunction)
		return NULL;

	return pFunction->m_pfnOrig;
}


CXTPSkinManagerApiFunction* CXTPSkinManagerApiHook::GetHookedFunction(LPCSTR pszCalleeModName, LPCSTR pszFuncName)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	XTP_GUARD_SHARED(CXTPSkinManagerApiHook, this)
	{
		for (int i = 0; i < XTP_SKIN_APIHOOKCOUNT; i++)
		{
			CXTPSkinManagerApiFunction* pHook = m_arrFunctions[i];
			if (!pHook)
			{
				continue;
			}

			XTP_GUARD_SHARED(CXTPSkinManagerApiFunction, pHook)
			{
				if (XTPCompareStringNoCase(pHook->m_szCalleeModName, pszCalleeModName) == 0 &&
					XTPCompareStringNoCase(pHook->m_szFuncName, pszFuncName) == 0)
				{
					return pHook;
				}
			}
		}
	}

	return NULL;
}

//
// The PUSH opcode on x86 platforms
//
const BYTE cPushOpCode = 0x68;

PVOID CXTPSkinManagerApiFunction::sm_pvMaxAppAddr = NULL;
PVOID CXTPSkinManagerApiFunction::sm_pfnAfxWndProc = NULL;

//////////////////////////////////////////////////////////////////////////
// CXTPSkinManagerApiFunction

CXTPSkinManagerApiFunction::CXTPSkinManagerApiFunction( CXTPSkinManagerApiHook* pApiHook,
	LPCSTR pszCalleeModName, LPCSTR pszFuncName, PROC pfnOrig, PROC pfnHook, PROC pfnAltOrig /*= NULL*/)
:
	m_pApiHook(pApiHook),
	m_bHooked(FALSE),
	m_pfnOrig(pfnOrig),
	m_pfnHook(pfnHook),
	m_pfnAltOrig(pfnAltOrig)
{
#if (_MSC_VER > 1310) // VS2005
	strcpy_s(m_szCalleeModName, _countof(m_szCalleeModName), pszCalleeModName);
	strcpy_s(m_szFuncName, _countof(m_szFuncName), pszFuncName);
#else
	strcpy(m_szCalleeModName, pszCalleeModName);
	strcpy(m_szFuncName, pszFuncName);
#endif

	if (sm_pfnAfxWndProc == NULL)
	{
		sm_pfnAfxWndProc = (FARPROC)AfxGetAfxWndProc();

	}

	if (sm_pvMaxAppAddr == NULL)
	{
		//
		// Functions with address above lpMaximumApplicationAddress require
		// special processing (Windows 9x only)
		//
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		sm_pvMaxAppAddr = si.lpMaximumApplicationAddress;
	}

	if (m_pfnOrig > sm_pvMaxAppAddr)
	{
		//
		// The address is in a shared DLL; the address needs fixing up
		//
		PBYTE pb = (PBYTE) m_pfnOrig;
		if (pb[0] == cPushOpCode)
		{
			//
			// Skip over the PUSH op code and grab the real address
			//
			PVOID pv = * (PVOID*) &pb[1];
			m_pfnOrig = (PROC) pv;
		}
	}

}


CXTPSkinManagerApiFunction::~CXTPSkinManagerApiFunction()
{
	UnhookImport();
}

BOOL CXTPSkinManagerApiFunction::HookImport()
{
	XTP_GUARD_SHARED(CXTPSkinManagerApiFunction, this)
	{
		m_bHooked = ReplaceInAllModules(m_szCalleeModName, m_pfnOrig, m_pfnHook, m_pfnAltOrig);
		return m_bHooked;
	}
}

BOOL CXTPSkinManagerApiFunction::UnhookImport()
{
	XTP_GUARD_SHARED(CXTPSkinManagerApiFunction, this)
	{
		if (m_bHooked)
		{
			ReplaceInAllModules(m_szCalleeModName, m_pfnHook, m_pfnOrig, m_pfnAltOrig);
			m_bHooked = FALSE;
		}
		return !m_bHooked;
	}
}

BOOL CXTPSkinManagerApiFunction::ReplaceInAllModules(
	LPCSTR pszCalleeModName,
	PROC pfnCurrent,
	PROC pfnNew,
	PROC pfnAltCurrent /*= NULL*/)
{
	BOOL bResult = FALSE;

	if ((NULL != pfnCurrent) && (NULL != pfnNew))
	{
		HINSTANCE hInstance = XTPGetInstanceHandle();

		CXTPSkinManagerModuleList moduleList(::GetCurrentProcessId());

		HMODULE hModule = moduleList.GetFirstModule();

		while (hModule)
		{
#ifdef _DEBUG
			TCHAR fn[MAX_PATH];
			XTP_ENTER_GUARD_CONDITION(m_pApiHook->m_nLockCondition)
			{
				GetModuleFileName(hModule, fn, MAX_PATH);
			}
#endif
			if ((hModule != hInstance) && !XTPAccessShared(m_pApiHook)->IsModuleExcluded(hModule))
			{
				if (ReplaceInOneModule(pszCalleeModName, pfnCurrent, pfnNew, hModule, pfnAltCurrent))
				{
					bResult = TRUE;
				}
			}

			hModule = moduleList.GetNextModule();
		}


		if (ReplaceInOneModule(pszCalleeModName, pfnCurrent, pfnNew, hInstance, pfnAltCurrent))
		{
			bResult = TRUE;
		}
	}
	return bResult;
}

BOOL CXTPSkinManagerApiFunction::ReplaceInOneModule(
	LPCSTR pszCalleeModName,
	PROC pfnCurrent,
	PROC pfnNew,
	HMODULE hmodCaller,
	PROC pfnAltCurrent /*= NULL*/)
{
	if (pfnCurrent == NULL || pfnNew == NULL)
		return FALSE;

	static CXTPSimpleCriticalSection cs;
	CXTPSimpleCriticalSection::CLock lock(cs);

	try
	{
		ULONG ulSize;
		// Get the address of the module's import section
		PIMAGE_IMPORT_DESCRIPTOR pImportDesc =
			(PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
			hmodCaller,
			TRUE,
			IMAGE_DIRECTORY_ENTRY_IMPORT,
			&ulSize
			);
		// Does this module has import section ?
		if (pImportDesc == NULL)
			return FALSE;

		while (pImportDesc != NULL)
		{

			// Loop through all descriptors and
			// find the import descriptor containing references to callee's functions
			while (pImportDesc->Name)
			{
				LPCSTR lpszName = ((LPCSTR)((PBYTE)hmodCaller + pImportDesc->Name));
				if (XTPCompareStringNoCase(lpszName, pszCalleeModName) == 0)
					break;   // Found
				pImportDesc++;
			} // while
			// Does this module import any functions from this callee ?
			if (pImportDesc->Name == 0)
				return FALSE;

			PIMAGE_THUNK_DATA pThunk =
				(PIMAGE_THUNK_DATA)((PBYTE)hmodCaller + (UINT_PTR)pImportDesc->FirstThunk);

			while (pThunk->u1.Function)
			{
				PROC* ppfn = (PROC*)&pThunk->u1.Function;

				BOOL bFound = (*ppfn == pfnCurrent);
				BOOL bReplaced = (*ppfn == pfnNew);

				if (!bReplaced && !bFound && (*ppfn > sm_pvMaxAppAddr))
				{
					PBYTE pbInFunc = (PBYTE)*ppfn;
					// Is this a wrapper (debug thunk) represented by PUSH instruction?
					if (pbInFunc[0] == cPushOpCode)
					{
						ppfn = (PROC*)&pbInFunc[1];
						bFound = (*ppfn == pfnCurrent);
						bReplaced = (*ppfn == pfnNew);
					}
				}

				if (!bReplaced && !bFound && NULL != pfnAltCurrent)
				{
					bFound = (*ppfn == pfnAltCurrent);
					if (!bFound && (*ppfn > sm_pvMaxAppAddr))
					{
						PBYTE pbInFunc = (PBYTE)*ppfn;
						// Is this a wrapper (debug thunk) represented by PUSH instruction?
						if (pbInFunc[0] == cPushOpCode)
						{
							ppfn = (PROC*)&pbInFunc[1];
							bFound = (*ppfn == pfnAltCurrent);
							bReplaced = (*ppfn == pfnNew);
						}
					}
				}

				if (bFound)
				{
					// some useful info can be found on
					// http://www.gunsmoker.ru/2012/09/task-15-answer.html

					MEMORY_BASIC_INFORMATION mbi;
					::VirtualQuery(ppfn, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
					// In order to provide writable access to this part of the
					// memory we need to change the memory protection
					if (!::VirtualProtect(mbi.BaseAddress, mbi.RegionSize,
						PAGE_EXECUTE_READWRITE, &mbi.Protect))
					{
						return FALSE;
					}

					// Hook the function.
					*ppfn = *pfnNew;

					// Restore the protection back
					DWORD dwOldProtect;
					::VirtualProtect(mbi.BaseAddress, mbi.RegionSize,
						mbi.Protect, &dwOldProtect);

					::FlushInstructionCache(GetCurrentProcess(), mbi.BaseAddress, mbi.RegionSize);

					return TRUE;
				}
				else if (bReplaced)
				{
					return TRUE;
				}

				pThunk++;
			}
			pImportDesc++;

		}
	}
	catch (...)
	{
		// do nothing
	}

	return FALSE;
}

void CXTPSkinManagerApiHook::InitializeHookManagement()
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	XTP_GUARD_EXCLUSIVE(CXTPSkinManagerApiHook, this)
	{
		if (m_bInitialized)
			return;

		for (int i = 0; i < (int)m_arrExcludedModules.GetSize(); i++)
		{
			CString& strModule = m_arrExcludedModules[i].strModule;

			if (strModule[strModule.GetLength() - 1] != '*')
			{
				m_arrExcludedModules[i].hModule = GetModuleHandle(strModule);
			}
		}


	#define ADDHOOK(Dll, ProcName) \
		HookImport(xtpSkinApi##ProcName, Dll, #ProcName, (PROC)&CXTPSkinManagerApiHook::OnHook##ProcName)

		ADDHOOK("KERNEL32.DLL", LoadLibraryA);
		ADDHOOK("KERNEL32.DLL", LoadLibraryW);
		ADDHOOK("KERNEL32.DLL", LoadLibraryExA);
		ADDHOOK("KERNEL32.DLL", LoadLibraryExW);
		ADDHOOK("KERNEL32.DLL", GetProcAddress);
		ADDHOOK("KERNEL32.DLL", GetModuleHandleA);
		ADDHOOK("KERNEL32.DLL", CreateThread);

		if (XTPSystemVersion()->IsWin7OrGreater())
		{
			HookImport(xtpSkinApiLoadLibraryEx2W, "API-MS-WIN-CORE-LIBRARYLOADER-L1-1-0.DLL",
				"LoadLibraryExW", (PROC)&CXTPSkinManagerApiHook::OnHookLoadLibraryEx2W);
			ADDHOOK("DWMAPI.DLL", DwmExtendFrameIntoClientArea);
		}

		ADDHOOK("USER32.DLL", RegisterClassA);
		ADDHOOK("USER32.DLL", RegisterClassW);

		XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
		{
			if (XTPSkinManager()->GetApplyOptions() & xtpSkinApplyColors)
			{
				ADDHOOK("USER32.DLL", GetSysColor);
				ADDHOOK("USER32.DLL", GetSysColorBrush);
				ADDHOOK("USER32.DLL", DrawEdge);
				ADDHOOK("USER32.DLL", FillRect);
				ADDHOOK("GDI32.DLL", DeleteObject);

				ADDHOOK("UXTHEME.DLL", OpenThemeData);
				ADDHOOK("UXTHEME.DLL", DrawThemeBackground);
				ADDHOOK("UXTHEME.DLL", CloseThemeData);
				ADDHOOK("UXTHEME.DLL", GetThemeColor);
				ADDHOOK("UXTHEME.DLL", GetThemeInt);
				ADDHOOK("UXTHEME.DLL", IsAppThemed);
				ADDHOOK("UXTHEME.DLL", IsThemeActive);
				ADDHOOK("UXTHEME.DLL", GetCurrentThemeName);
				ADDHOOK("UXTHEME.DLL", GetThemeSysBool);
				ADDHOOK("UXTHEME.DLL", GetThemeSysColor);
				ADDHOOK("UXTHEME.DLL", GetThemePartSize);

				ADDHOOK("USER32.DLL", DrawFrameControl);
			}

			if (XTPSkinManager()->GetApplyOptions() & xtpSkinApplyFrame)
			{
				ADDHOOK("USER32.DLL", SetScrollInfo);
				ADDHOOK("USER32.DLL", SetScrollPos);
				ADDHOOK("USER32.DLL", GetScrollInfo);
				ADDHOOK("USER32.DLL", EnableScrollBar);
				ADDHOOK("USER32.DLL", DrawMenuBar);
			}

			if (XTPSkinManager()->GetApplyOptions() & (xtpSkinApplyMetrics | xtpSkinApplyFrame))
			{
				HookImport(xtpSkinApiSystemParametersInfo, "USER32.DLL", lpszSystemParametersInfo, (PROC)&CXTPSkinManagerApiHook::OnHookSystemParametersInfo);
				ADDHOOK("USER32.DLL", AdjustWindowRectEx);
				ADDHOOK("USER32.DLL", AdjustWindowRect);
				ADDHOOK("USER32.DLL", GetSystemMetrics);
			}

			if (XTPSkinManager()->GetApplyOptions() & xtpSkinApplyMenus)
			{
				ADDHOOK("USER32.DLL", TrackPopupMenu);
				ADDHOOK("USER32.DLL", TrackPopupMenuEx);
				ADDHOOK("USER32.DLL", GetMenuItemRect);
			}
		}

		ADDHOOK("USER32.DLL", DefWindowProcA);
		ADDHOOK("USER32.DLL", DefWindowProcW);
		ADDHOOK("USER32.DLL", DefFrameProcA);
		ADDHOOK("USER32.DLL", DefFrameProcW);
		ADDHOOK("USER32.DLL", DefDlgProcA);
		ADDHOOK("USER32.DLL", DefDlgProcW);
		ADDHOOK("USER32.DLL", DefMDIChildProcA);
		ADDHOOK("USER32.DLL", DefMDIChildProcW);

		ADDHOOK("USER32.DLL", CallWindowProcA);
		ADDHOOK("USER32.DLL", CallWindowProcW);

		m_bInitialized = TRUE;
	}
}

void CXTPSkinManagerApiHook::FinalizeHookManagement()
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	XTP_GUARD_EXCLUSIVE(CXTPSkinManagerApiHook, this)
	{
		if (!m_bInitialized)
			return;

		UnhookAllFunctions(FALSE);

		m_bInitialized = FALSE;
	}
}

void CXTPSkinManagerApiHook::HackModuleOnLoad(HMODULE hmod, DWORD dwFlags)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	XTP_GUARD_SHARED(CXTPSkinManagerApiHook, this)
	{
		for (int i = 0; i < (int)m_arrExcludedModules.GetSize(); i++)
		{
			EXCLUDEDMODULE& em = m_arrExcludedModules[i];

			if (em.hModule == NULL && em.strModule[em.strModule.GetLength() - 1] != '*')
			{
				em.hModule = GetModuleHandle(em.strModule);
			}
		}

		if (IsModuleExcluded(hmod))
			return;

		if ((hmod != NULL) && ((dwFlags & LOAD_LIBRARY_AS_DATAFILE) == 0))
		{
			for (int i = 0; i < XTP_SKIN_APIHOOKCOUNT; i++)
			{
				CXTPSkinManagerApiFunction* pHook = m_arrFunctions[i];
				if (pHook)
				{
					XTP_GUARD_EXCLUSIVE(CXTPSkinManagerApiFunction, pHook)
					{
						if (pHook->ReplaceInOneModule(pHook->m_szCalleeModName, pHook->m_pfnOrig, pHook->m_pfnHook, hmod, pHook->m_pfnAltOrig))
						{
							pHook->m_bHooked = TRUE;
						}
					}
				}
			}
		}
	}
}

BOOL CXTPSkinManagerApiHook::EnterHookedCall()
{
	BOOL bInstantiated = (NULL != CXTPSkinManager::s_pInstance);
	if (!bInstantiated)
	{
		SetLastError(ERROR_PROC_NOT_FOUND);
	}
	return bInstantiated;
}

CXTPSkinManagerApiFunction* CXTPSkinManagerApiHook::EnterHookedCall(
	LPCSTR pszCalleeModName,
	LPCSTR pszFuncName)
{
	CXTPSkinManagerApiFunction* pFunction = NULL;
	if (EnterHookedCall())
	{
		pFunction = GetInstance()->GetHookedFunction(pszCalleeModName, pszFuncName);
		if (!(NULL != pFunction && NULL != pFunction->m_pfnOrig))
		{
			SetLastError(ERROR_PROC_NOT_FOUND);
			pFunction = NULL;
		}
	}

	return pFunction;
}

CXTPSkinManagerApiFunction* CXTPSkinManagerApiHook::EnterHookedCall(
	XTPSkinFrameworkApiFunctionIndex nIndex)
{
	CXTPSkinManagerApiFunction* pFunction = NULL;
	if (EnterHookedCall())
	{
		pFunction = GetInstance()->GetHookedFunction(nIndex);
		if (!(NULL != pFunction && NULL != pFunction->m_pfnOrig))
		{
			SetLastError(ERROR_PROC_NOT_FOUND);
			pFunction = NULL;
		}
	}

	return pFunction;
}

DWORD WINAPI CXTPSkinManagerApiHook::OnHookGetSysColor(int nIndex)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiGetSysColor);
	if (NULL == pFunction)
	{
		return 0;
	}

	XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
	{
		CXTPSkinManagerMetrics* pMetrics = XTPSkinManager()->GetMetrics();

		if (pMetrics && XTPSkinManager()->IsEnabled() && nIndex < XTP_SKINMETRICS_COLORTABLESIZE && pMetrics->m_clrTheme[nIndex] != -1)
			return pMetrics->m_clrTheme[nIndex];

		typedef int(WINAPI* LPFNGETSYSCOLOR)(int);
		DWORD nResult = ((LPFNGETSYSCOLOR)pFunction->m_pfnOrig)(nIndex);

		return nResult;
	}
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookDeleteObject(HGDIOBJ hObject)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDeleteObject);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
	{
		CXTPSkinManagerMetrics* pMetrics = XTPSkinManager()->GetMetrics();
		if (pMetrics && pMetrics->IsMetricObject(hObject))
			return FALSE;

		typedef BOOL(WINAPI* LPFNDELETEOBJECT)(HGDIOBJ);
		return ((LPFNDELETEOBJECT)pFunction->m_pfnOrig)(hObject);
	}
}

HBRUSH WINAPI CXTPSkinManagerApiHook::OnHookGetSysColorBrush(  int nIndex)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiGetSysColorBrush);
	if (NULL == pFunction)
	{
		return NULL;
	}

	XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
	{
		CXTPSkinManagerMetrics* pMetrics = XTPSkinManager()->GetMetrics();

		if (pMetrics && XTPSkinManager()->IsEnabled() && nIndex > 0 && nIndex < XTP_SKINMETRICS_COLORTABLESIZE && pMetrics->m_brTheme[nIndex])
		{
			return pMetrics->m_brTheme[nIndex];
		}

		typedef HBRUSH(WINAPI* LPFNGETSYSCOLORBRUSH)(int);
		HBRUSH nResult = ((LPFNGETSYSCOLORBRUSH)pFunction->m_pfnOrig)(nIndex);

		return nResult;
	}
}

int WINAPI CXTPSkinManagerApiHook::OnHookSetScrollPos( HWND hwnd,  int nBar,   int nPos,  BOOL redraw)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiSetScrollPos);
	if (NULL == pFunction)
	{
		return 0;
	}

	XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
	{
		typedef int(WINAPI* LPFNSETSCROLLPOS)(HWND hWnd, int nBar, int nPos, BOOL bRedraw);

		CXTPSkinObjectFrame* pFrame = (CXTPSkinObjectFrame*)XTPSkinManager()->Lookup(hwnd);

		if (!pFrame)
			return ((LPFNSETSCROLLPOS)pFunction->m_pfnOrig)(hwnd, nBar, nPos, redraw);

		XTP_GUARD_SHARED(CXTPSkinObjectFrame, pFrame)
		{
			int nResult = ((LPFNSETSCROLLPOS)pFunction->m_pfnOrig)(hwnd, nBar, nPos, FALSE);

			if (redraw)
			{
				if (nBar == SB_CTL) InvalidateRect(hwnd, NULL, FALSE);
				else pFrame->RedrawScrollBar(nBar);
			}
			return nResult;
		}
	}
}


int WINAPI CXTPSkinManagerApiHook::OnHookSetScrollInfo( HWND hwnd,  int nBar,  LPCSCROLLINFO lpsi,  BOOL redraw)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiSetScrollInfo);
	if (NULL == pFunction)
	{
		return 0;
	}

	XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
	{
		typedef int(WINAPI* LPFNSETSCROLLINFO)(HWND hwnd, int fnBar, LPCSCROLLINFO lpsi, BOOL fRedraw);

		CXTPSkinObjectFrame* pFrame = (CXTPSkinObjectFrame*)XTPSkinManager()->Lookup(hwnd);

		if (!pFrame || (nBar == SB_CTL) || !redraw)
			return ((LPFNSETSCROLLINFO)pFunction->m_pfnOrig)(hwnd, nBar, lpsi, redraw);

		XTP_GUARD_SHARED(CXTPSkinObjectFrame, pFrame)
		{
			int nResult = ((LPFNSETSCROLLINFO)pFunction->m_pfnOrig)(hwnd, nBar, lpsi, FALSE);
			pFrame->RedrawScrollBar(nBar);
			return nResult;
		}
	}
}



BOOL WINAPI CXTPSkinManagerApiHook::OnHookEnableScrollBar(HWND hWnd, UINT wSBflags, UINT wArrows)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiEnableScrollBar);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
	{
		typedef int(WINAPI* LPFNENABLESCROLLBAR)(HWND hWnd, UINT wSBflags, UINT wArrows);

		CXTPSkinObjectFrame* pFrame = (CXTPSkinObjectFrame*)XTPSkinManager()->Lookup(hWnd);

		if (!pFrame)
			return ((LPFNENABLESCROLLBAR)pFunction->m_pfnOrig)(hWnd, wSBflags, wArrows);

		XTP_GUARD_SHARED(CXTPSkinObjectFrame, pFrame)
		{
			int nResult = ((LPFNENABLESCROLLBAR)pFunction->m_pfnOrig)(hWnd, wSBflags, wArrows);

			if (wSBflags == SB_BOTH || wSBflags == SB_HORZ)
				pFrame->RedrawScrollBar(SB_HORZ);

			if (wSBflags == SB_BOTH || wSBflags == SB_VERT)
				pFrame->RedrawScrollBar(SB_VERT);

			return nResult;
		}
	}
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookGetScrollInfo(HWND hWnd, int nBar, LPSCROLLINFO lpsi)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiGetScrollInfo);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	typedef BOOL(WINAPI* LPFNGETSCROLLINFO)(HWND hwnd, int nBar, LPCSCROLLINFO lpsi);
	BOOL nResult = ((LPFNGETSCROLLINFO)pFunction->m_pfnOrig)(hWnd, nBar, lpsi);

	if (lpsi && (lpsi->fMask & SIF_TRACKPOS))
	{
		XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
		{
			CXTPSkinObject* pSink = XTPSkinManager()->Lookup(hWnd);

			if (pSink)
			{
				XTP_GUARD_SHARED(CXTPSkinObject, pSink)
				{
					XTP_SKINSCROLLBAR_TRACKINFO *pSBTrack = ((CXTPSkinObjectFrame*)pSink)->GetScrollBarTrackInfo();

					if (pSBTrack && (pSBTrack->nBar == nBar))
					{
						// posNew is in the context of psbiSB's window and bar code
						lpsi->nTrackPos = pSBTrack->posNew;
					}
				}
			}
		}
	}

	return nResult;
}

BOOL CXTPSkinManagerApiHook::IsSystemWindowModule(WNDPROC lpWndProc)
{
	if ((HIWORD((ULONG)(ULONG_PTR)lpWndProc)) == 0xFFFF)
		return TRUE;

	static XTP_MODULEINFO miUser32 = {(LPVOID)-1, 0, 0, 0};

	if (miUser32.lpBaseOfDll == (LPVOID)-1)
	{
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		miUser32.hModule = hUser32;
		if (hUser32)
		{
			CXTPSkinManagerModuleList moduleList(::GetCurrentProcessId());
			moduleList.GetModuleInformation(hUser32, &miUser32);
		}
	}

	if ((LPBYTE)lpWndProc > (LPBYTE)miUser32.lpBaseOfDll
		&& (LPBYTE)lpWndProc < (LPBYTE)miUser32.lpBaseOfDll + miUser32.SizeOfImage)
		return TRUE;

	static XTP_MODULEINFO miComCtrl32  = {(LPVOID)-1, 0, (LPVOID)-1, 0};

	HMODULE hComCtl32 = GetModuleHandle(_T("COMCTL32.DLL"));

	if (hComCtl32 && (miComCtrl32.lpBaseOfDll == (LPVOID)-1 || miComCtrl32.hModule != hComCtl32 ))
	{
		miComCtrl32.hModule = hComCtl32;
		CXTPSkinManagerModuleList moduleList(::GetCurrentProcessId());
		moduleList.GetModuleInformation(hComCtl32, &miComCtrl32);
	}

	if ((LPBYTE)lpWndProc > (LPBYTE)miComCtrl32.lpBaseOfDll
		&& (LPBYTE)lpWndProc < (LPBYTE)miComCtrl32.lpBaseOfDll + miComCtrl32.SizeOfImage)
		return TRUE;

	if (XTPSystemVersion()->IsWinXPOrGreater())
	{
		static XTP_MODULEINFO miNtDll = {(LPVOID)-1, 0, 0, 0};

		if (miNtDll.lpBaseOfDll == (LPVOID)-1)
		{
			HMODULE hNtDLL = GetModuleHandle(_T("NTDLL.DLL"));
			miNtDll.hModule = hNtDLL;
			if (hNtDLL)
			{
				CXTPSkinManagerModuleList moduleList(::GetCurrentProcessId());
				moduleList.GetModuleInformation(hNtDLL, &miNtDll);
			}
		}

		if ((LPBYTE)lpWndProc > (LPBYTE)miNtDll.lpBaseOfDll
			&& (LPBYTE)lpWndProc < (LPBYTE)miNtDll.lpBaseOfDll + miNtDll.SizeOfImage)
			return TRUE;

		if(IsHookedByAppHelpDll(reinterpret_cast<PROC>(lpWndProc)))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL AFX_CDECL CXTPSkinManagerApiHook::IsHookedByAppHelpDll(PROC lpProc)
{
	BOOL bHooked = FALSE;

	if(XTPSystemVersion()->IsWin7OrGreater())
	{
		static XTP_MODULEINFO miAppHelp = {(LPVOID)-1, 0, 0, 0};
		if (miAppHelp.lpBaseOfDll == (LPVOID)-1)
		{
			HMODULE hAppHelp = GetModuleHandle(_T("APPHELP.DLL"));
			miAppHelp.hModule = hAppHelp;
			if (NULL != hAppHelp)
			{
				CXTPSkinManagerModuleList moduleList(::GetCurrentProcessId());
				moduleList.GetModuleInformation(hAppHelp, &miAppHelp);
			}
		}

		bHooked = ((LPBYTE)lpProc > (LPBYTE)miAppHelp.lpBaseOfDll
			&& (LPBYTE)lpProc < (LPBYTE)miAppHelp.lpBaseOfDll + miAppHelp.SizeOfImage);
	}

	return bHooked;
}

BOOL CXTPSkinManagerApiHook::CallHookDefWindowProc(HWND hWnd, PROC pfnOrig, XTPSkinDefaultProc defProc, LPVOID lpPrev, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult)
{
	ASSERT(NULL != CXTPSkinManager::s_pInstance);

	XTP_SKINFRAMEWORK_ASSERT_WINDOW_THREAD(hWnd);

	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (!pSkinManager->IsEnabled())
			return FALSE;

		CXTPSkinObject* pSkinObject = pSkinManager->Lookup(hWnd);

		if (!pSkinObject)
			return FALSE;

		BOOL bWindowProcAttached = FALSE;

		XTP_GUARD_SHARED(CXTPSkinObject, pSkinObject)
		{
			if (pSkinObject->m_bCustomDraw)
				return FALSE;

			if (nMessage == pSkinObject->GetHeadMessage())
				return FALSE;

			bWindowProcAttached = pSkinObject->m_bWindowProcAttached;
		}

		if (nMessage == WM_NCDESTROY && !bWindowProcAttached)
		{
			XTPAccessExclusive(XTPSkinManager())->Remove(hWnd, TRUE);
			return FALSE;
		}

		BOOL bResult = FALSE;

		XTP_GUARD_SHARED(CXTPSkinObject, pSkinObject)
		{
			if (defProc == xtpSkinDefaultCallWindowProc && pSkinObject->IsDefWindowProcAvail(nMessage))
			{
				WNDPROC lpWndProc = (WNDPROC)lpPrev;

				if (IsSystemWindowModule(lpWndProc))
				{
					lpWndProc = 0;
				}

				if (lpWndProc != 0 && (!pSkinManager->IsWin9x() ||
					((DWORD_PTR)lpWndProc < (DWORD_PTR)0x70000000)))
				{
					return FALSE;
				}
			}

			MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
			MSG  oldMsg = curMsg;
			curMsg.hwnd = hWnd;
			curMsg.message = nMessage;
			curMsg.wParam = wParam;
			curMsg.lParam = lParam;

			pSkinObject->OnBeginHook(nMessage, defProc, pfnOrig, lpPrev);

			bResult = pSkinObject->OnHookDefWindowProc(nMessage, wParam, lParam, lResult);

			curMsg = oldMsg;
		}

		pSkinObject->OnEndHook();

		if (bResult)
			return TRUE;
	}

	return FALSE;
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookCallWindowProcW(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiCallWindowProcW);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (lpPrevWndFunc && GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultCallWindowProc, lpPrevWndFunc, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNCALLWINDOWPROC)(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		LRESULT bResult = ((LPFNCALLWINDOWPROC)pFunction->m_pfnOrig)(lpPrevWndFunc, hWnd, Msg, wParam, lParam);

		return bResult;
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookCallWindowProcA(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiCallWindowProcA);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (lpPrevWndFunc && GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultCallWindowProc, lpPrevWndFunc, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNCALLWINDOWPROC)(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		LRESULT bResult = ((LPFNCALLWINDOWPROC)pFunction->m_pfnOrig)(lpPrevWndFunc, hWnd, Msg, wParam, lParam);

		return bResult;
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookDefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDefWindowProcA);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultDefWindowProc, NULL, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)pFunction->m_pfnOrig)(hWnd, Msg, wParam, lParam);
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookDefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDefWindowProcW);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultDefWindowProc, NULL, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)pFunction->m_pfnOrig)(hWnd, Msg, wParam, lParam);
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookDefFrameProcA(HWND hWnd, HWND hWndMDIClient, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDefFrameProcA);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultDefFrameProc, (LPVOID)hWndMDIClient, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, HWND hWndMDIClient, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)pFunction->m_pfnOrig)(hWnd, hWndMDIClient, Msg, wParam, lParam);
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookDefFrameProcW(HWND hWnd, HWND hWndMDIClient, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDefFrameProcW);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultDefFrameProc, (LPVOID)hWndMDIClient, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, HWND hWndMDIClient, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)pFunction->m_pfnOrig)(hWnd, hWndMDIClient, Msg, wParam, lParam);
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookDefDlgProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDefDlgProcA);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultDefDlgProc, NULL, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)pFunction->m_pfnOrig)(hWnd, Msg, wParam, lParam);
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookDefDlgProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDefDlgProcW);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultDefDlgProc, NULL, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)pFunction->m_pfnOrig)(hWnd, Msg, wParam, lParam);
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookDefMDIChildProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDefMDIChildProcA);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultDefMDIChildProc, NULL, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)pFunction->m_pfnOrig)(hWnd, Msg, wParam, lParam);
	}
}

LRESULT WINAPI CXTPSkinManagerApiHook::OnHookDefMDIChildProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDefMDIChildProcW);
	if (NULL == pFunction)
	{
		return 0;
	}

	LRESULT lResult = 0;
	if (GetInstance()->CallHookDefWindowProc(hWnd, pFunction->m_pfnOrig, xtpSkinDefaultDefMDIChildProc, NULL, Msg, wParam, lParam, lResult))
	{
		return lResult;
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		typedef LRESULT(WINAPI* LPFNDEFWINDOWPROC)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		return ((LPFNDEFWINDOWPROC)pFunction->m_pfnOrig)(hWnd, Msg, wParam, lParam);
	}
}





struct XTP_SKINFRAMEWORK_THREADPROCPARAMETER
{
	LPTHREAD_START_ROUTINE lpStartAddress;
	LPVOID lpParameter;
};

DWORD WINAPI CXTPSkinManagerApiHook::ThreadProcHook(LPVOID lpThreadData)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	XTP_SKINFRAMEWORK_THREADPROCPARAMETER* parameter = (XTP_SKINFRAMEWORK_THREADPROCPARAMETER*)lpThreadData;
	LPTHREAD_START_ROUTINE lpStartAddress = parameter->lpStartAddress;
	LPVOID lpParam = parameter->lpParameter;
	delete lpThreadData;

	if (NULL != CXTPSkinManager::s_pInstance)
	{
		XTPAccessShared(XTPSkinManager())->EnableCurrentThread();
	}

	return (lpStartAddress)(lpParam);
}

HRESULT WINAPI CXTPSkinManagerApiHook::OnHookDwmExtendFrameIntoClientArea(
	HWND hWnd,
	LPCVOID pMarInset)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDwmExtendFrameIntoClientArea);
	if (NULL == pFunction)
	{
		return NULL;
	}

	HRESULT hr = E_NOTIMPL;
	CXTPSkinObject* pSkinObject = XTPAccessShared(XTPSkinManager())->Lookup(hWnd);
	if (NULL == pSkinObject)
	{
		typedef HRESULT(WINAPI* LPFNDWMEXTENDFRAMEINTOCLIENTAREA)(HWND, LPCVOID);
		hr = ((LPFNDWMEXTENDFRAMEINTOCLIENTAREA)pFunction->m_pfnOrig)(hWnd, pMarInset);
	}

	return hr;
}

HANDLE STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookCreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes,
	UINT_PTR dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter,
	DWORD dwCreationFlags, LPDWORD lpThreadId)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiCreateThread);
	if (NULL == pFunction)
	{
		return NULL;
	}

	typedef HANDLE(WINAPI* LPFNCREATETHREAD)(LPSECURITY_ATTRIBUTES lpThreadAttributes,
		UINT_PTR dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter,
		DWORD dwCreationFlags, LPDWORD lpThreadId);

	XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
	{
		if (XTPSkinManager()->IsEnabled() && XTPSkinManager()->GetAutoApplyNewThreads())
		{
			XTP_SKINFRAMEWORK_THREADPROCPARAMETER* pParam = new XTP_SKINFRAMEWORK_THREADPROCPARAMETER;
			pParam->lpParameter = lpParameter;
			pParam->lpStartAddress = lpStartAddress;

			return ((LPFNCREATETHREAD)pFunction->m_pfnOrig)(lpThreadAttributes,
				dwStackSize, ThreadProcHook, pParam,
				dwCreationFlags, lpThreadId);
		}
	}

	return ((LPFNCREATETHREAD)pFunction->m_pfnOrig)(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
}


ATOM WINAPI CXTPSkinManagerApiHook::OnHookRegisterClassA ( const WNDCLASSA *lpWndClass)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiRegisterClassA);
	if (NULL == pFunction)
	{
		return 0;
	}

	typedef ATOM(WINAPI* LPFNREGISTERCLASSA)(const WNDCLASSA* lpWndClass);

	XTP_GUARD_SHARED_(CXTPSkinManagerApiHook, GetInstance(), pInstance)
	{
		CXTPSkinManagerApiFunction* pDefWindowFunction = NULL;
		if (lpWndClass && lpWndClass->lpfnWndProc == (WNDPROC)&CXTPSkinManagerApiHook::OnHookDefWindowProcA)
		{
			pDefWindowFunction = pInstance->GetHookedFunction(xtpSkinApiDefWindowProcA);
		}
		if (lpWndClass && lpWndClass->lpfnWndProc == (WNDPROC)&CXTPSkinManagerApiHook::OnHookDefFrameProcA)
		{
			pDefWindowFunction = pInstance->GetHookedFunction(xtpSkinApiDefFrameProcA);
		}
		if (lpWndClass && lpWndClass->lpfnWndProc == (WNDPROC)&CXTPSkinManagerApiHook::OnHookDefMDIChildProcA)
		{
			pDefWindowFunction = pInstance->GetHookedFunction(xtpSkinApiDefMDIChildProcA);
		}
		if (lpWndClass && lpWndClass->lpfnWndProc == (WNDPROC)&CXTPSkinManagerApiHook::OnHookDefDlgProcA)
		{
			pDefWindowFunction = pInstance->GetHookedFunction(xtpSkinApiDefDlgProcA);
		}

		if (pDefWindowFunction)
		{
			WNDCLASSA wc;
			wc = *lpWndClass;
			wc.lpfnWndProc = (WNDPROC)XTPAccessShared(pDefWindowFunction)->m_pfnOrig;

			XTP_SKINFRAMEWORK_ORIGINAL_STATE()
			{
				return ((LPFNREGISTERCLASSA)pFunction->m_pfnOrig)(&wc);
			}
		}
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		return ((LPFNREGISTERCLASSA)pFunction->m_pfnOrig)(lpWndClass);
	}
}

ATOM WINAPI CXTPSkinManagerApiHook::OnHookRegisterClassW ( const WNDCLASSW *lpWndClass)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiRegisterClassW);
	if (NULL == pFunction)
	{
		return 0;
	}

	typedef ATOM(WINAPI* LPFNREGISTERCLASSW)(const WNDCLASSW* lpWndClass);

	XTP_GUARD_SHARED_(CXTPSkinManagerApiHook, GetInstance(), pInstance)
	{
		CXTPSkinManagerApiFunction* pDefWindowFunction = NULL;
		if (lpWndClass && lpWndClass->lpfnWndProc == (WNDPROC)&CXTPSkinManagerApiHook::OnHookDefWindowProcW)
		{
			pDefWindowFunction = pInstance->GetHookedFunction(xtpSkinApiDefWindowProcW);
		}
		if (lpWndClass && lpWndClass->lpfnWndProc == (WNDPROC)&CXTPSkinManagerApiHook::OnHookDefFrameProcW)
		{
			pDefWindowFunction = pInstance->GetHookedFunction(xtpSkinApiDefFrameProcW);
		}
		if (lpWndClass && lpWndClass->lpfnWndProc == (WNDPROC)&CXTPSkinManagerApiHook::OnHookDefMDIChildProcW)
		{
			pDefWindowFunction = pInstance->GetHookedFunction(xtpSkinApiDefMDIChildProcW);
		}
		if (lpWndClass && lpWndClass->lpfnWndProc == (WNDPROC)&CXTPSkinManagerApiHook::OnHookDefDlgProcW)
		{
			pDefWindowFunction = pInstance->GetHookedFunction(xtpSkinApiDefDlgProcW);
		}

		if (pDefWindowFunction)
		{
			WNDCLASSW wc;
			wc = *lpWndClass;
			wc.lpfnWndProc = (WNDPROC)XTPAccessShared(pDefWindowFunction)->m_pfnOrig;

			XTP_SKINFRAMEWORK_ORIGINAL_STATE()
			{
				return ((LPFNREGISTERCLASSW)pFunction->m_pfnOrig)(&wc);
			}
		}
	}

	XTP_SKINFRAMEWORK_ORIGINAL_STATE()
	{
		return ((LPFNREGISTERCLASSW)pFunction->m_pfnOrig)(lpWndClass);
	}

}

int WINAPI CXTPSkinManagerApiHook::OnHookGetSystemMetrics(int nIndex)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiGetSystemMetrics);
	if (NULL == pFunction)
	{
		return 0;
	}

	typedef int (WINAPI* LPFNGETSYSTEMMETRICS)(int nIndex);
	int nResult = ((LPFNGETSYSTEMMETRICS)pFunction->m_pfnOrig)(nIndex);

	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (pSkinManager->IsEnabled())
		{
			XTP_GUARD_SHARED_(CXTPSkinManagerMetrics, pSkinManager->GetMetrics(), pMetrics)
			{
				if (pSkinManager->IsEnabled() || pMetrics == NULL || pMetrics->m_bRefreshMetrics)
					return nResult;

				if (!pSkinManager->HasApplyOptions(xtpSkinApplyMetrics | xtpSkinApplyColors | xtpSkinApplyFrame))
					return nResult;

				switch (nIndex)
				{
				case SM_CYCAPTION: return pMetrics->m_cyCaption;
				case SM_CYSMCAPTION: return pMetrics->m_cySmallCaption;

				case SM_CXBORDER: return pMetrics->m_cxBorder;
				case SM_CYBORDER: return pMetrics->m_cyBorder;

				case SM_CXHSCROLL: return pMetrics->m_cxHScroll;
				case SM_CYHSCROLL: return pMetrics->m_cyHScroll;
				case SM_CXVSCROLL: return pMetrics->m_cxVScroll;
				case SM_CYVSCROLL: return pMetrics->m_cyVScroll;

				case SM_CXEDGE: return pMetrics->m_cxEdge;
				case SM_CYEDGE: return pMetrics->m_cyEdge;
				}
			}
		}
	}

	return nResult;
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookSystemParametersInfo (UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiSystemParametersInfo);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	typedef BOOL(WINAPI* LPFNSYSTEMPARAMETERSINFO)(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni);
	BOOL bResult = ((LPFNSYSTEMPARAMETERSINFO)pFunction->m_pfnOrig)(uiAction, uiParam, pvParam, fWinIni);

	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (!pSkinManager->IsEnabled() || pSkinManager->GetMetrics()->m_bRefreshMetrics)
			return bResult;

		if (!pSkinManager->HasApplyOptions(xtpSkinApplyMetrics | xtpSkinApplyColors | xtpSkinApplyFrame))
			return bResult;

		if (uiAction == SPI_GETICONTITLELOGFONT && uiParam == sizeof(LOGFONT))
		{
			LOGFONT* pLogFont = (LOGFONT*)pvParam;

			XTP_GUARD_SHARED_(CXTPSkinManagerSchema, XTPSkinManager()->GetSchema(), pSchema)
			{
				UINT nSysMetrics = pSchema->GetClassCode(_T("SYSMETRICS"));

				pSchema->GetFontProperty(nSysMetrics, 0, 0, TMT_ICONTITLEFONT, *pLogFont);
			}
		}

		if (uiAction == SPI_GETNONCLIENTMETRICS && uiParam == sizeof(NONCLIENTMETRICS))
		{
			NONCLIENTMETRICS* pncm = (NONCLIENTMETRICS*)pvParam;

			XTP_GUARD_SHARED_(CXTPSkinManagerSchema, XTPSkinManager()->GetSchema(), pSchema)
			{
				UINT nSysMetrics = pSchema->GetClassCode(_T("SYSMETRICS"));

				pSchema->GetFontProperty(nSysMetrics, 0, 0, TMT_MENUFONT, pncm->lfMenuFont);
				pSchema->GetFontProperty(nSysMetrics, 0, 0, TMT_STATUSFONT, pncm->lfStatusFont);
				pSchema->GetFontProperty(nSysMetrics, 0, 0, TMT_CAPTIONFONT, pncm->lfCaptionFont);
			}
		}
	}

	return bResult;
}


BOOL CXTPSkinManagerApiHook::AdjustWindowRectExOrig(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle)
{
	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiAdjustWindowRectEx);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	typedef BOOL(WINAPI* LPFNADJUSTWINDOWRECTEX)(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);
	return ((LPFNADJUSTWINDOWRECTEX)pFunction->m_pfnOrig)(lpRect, dwStyle, bMenu, dwExStyle);
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookAdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiAdjustWindowRectEx);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	BOOL bResult = FALSE;
	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (pSkinManager->IsEnabled()
			&& pSkinManager->HasApplyOptions(xtpSkinApplyFrame)
			&& (0 == (dwStyle & WS_MAXIMIZE))
			&& NULL != lpRect
			&& !bMenu)
		{
			CRect rcBorders = XTPAccessShared(pSkinManager->GetSchema())->CalcFrameBorders(dwStyle, dwExStyle);
			lpRect->top -= rcBorders.top;
			lpRect->left -= rcBorders.left;
			lpRect->right += rcBorders.right;
			lpRect->bottom += rcBorders.bottom;

			bResult = TRUE;
		}
		else
		{
			typedef BOOL(WINAPI* LPFNADJUSTWINDOWRECTEX)(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);
			bResult = ((LPFNADJUSTWINDOWRECTEX)pFunction->m_pfnOrig)(lpRect, dwStyle, bMenu, dwExStyle);
		}
	}

	return bResult;
}


BOOL WINAPI CXTPSkinManagerApiHook::OnHookAdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiAdjustWindowRect);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	BOOL bResult = FALSE;
	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (pSkinManager->IsEnabled()
			&& pSkinManager->HasApplyOptions(xtpSkinApplyFrame)
			&& (0 == (dwStyle & WS_MAXIMIZE))
			&& NULL != lpRect
			&& !bMenu)
		{
			DWORD dwExStyle = 0;

			if ((dwStyle & WS_THICKFRAME))
				dwExStyle = WS_EX_WINDOWEDGE;

			if (((dwStyle & WS_CAPTION) == WS_DLGFRAME) || ((dwStyle & WS_CAPTION) == WS_CAPTION))
				dwExStyle = WS_EX_WINDOWEDGE;

			CRect rcBorders = XTPAccessShared(pSkinManager->GetSchema())->CalcFrameBorders(dwStyle, dwExStyle);
			lpRect->top -= rcBorders.top;
			lpRect->left -= rcBorders.left;
			lpRect->right += rcBorders.right;
			lpRect->bottom += rcBorders.bottom;

			bResult = TRUE;
		}
		else
		{
			typedef BOOL(WINAPI* LPFNADJUSTWINDOWRECT)(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);
			bResult = ((LPFNADJUSTWINDOWRECT)pFunction->m_pfnOrig)(lpRect, dwStyle, bMenu);
		}
	}

	return bResult;
}

int WINAPI CXTPSkinManagerApiHook::OnHookFillRect(HDC hDC, CONST RECT *lprc, HBRUSH hbr)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiFillRect);
	if (NULL == pFunction)
	{
		return 0;
	}

	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (pSkinManager->IsEnabled() && pSkinManager->GetMetrics() &&
			(DWORD_PTR)hbr > 0 && (DWORD_PTR)hbr < XTP_SKINMETRICS_COLORTABLESIZE)
		{
			hbr = pSkinManager->GetMetrics()->m_brTheme[(DWORD_PTR)hbr - 1];
		}
	}

	typedef int (WINAPI* LPFNFILLRECT)(HDC hDC, CONST RECT *lprc, HBRUSH hbr);
	return ((LPFNFILLRECT)pFunction->m_pfnOrig)(hDC, lprc, hbr);
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookDrawEdge(HDC hdc, LPRECT lprc, UINT edge, UINT flags)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDrawEdge);
	if (NULL == pFunction)
	{
		return 0;
	}

	typedef BOOL(WINAPI* LPFNDRAWEDGE)(HDC, LPRECT, UINT, UINT);
	if (!XTPSkinManager()->IsEnabled())
		return ((LPFNDRAWEDGE)pFunction->m_pfnOrig)(hdc, lprc, edge, flags);

	return XTRSkinFrameworkDrawEdge(hdc, lprc, edge, flags);
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookDrawFrameControl(HDC hdc, LPRECT lprc, UINT uType, UINT uState)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDrawFrameControl);
	if (NULL == pFunction)
	{
		return 0;
	}

	typedef BOOL(WINAPI* LPFNDRAWFRAMECONTROL)(HDC, LPRECT, UINT, UINT);
	if (!XTPSkinManager()->IsEnabled())
		return ((LPFNDRAWFRAMECONTROL)pFunction->m_pfnOrig)(hdc, lprc, uType, uState);

	if (XTRSkinFrameworkDrawFrameControl(hdc, lprc, uType, uState))
		return TRUE;

	return ((LPFNDRAWFRAMECONTROL)pFunction->m_pfnOrig)(hdc, lprc, uType, uState);
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookDrawMenuBar(HWND hWnd)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiDrawMenuBar);
	if (NULL == pFunction)
	{
		return 0;
	}

	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (pSkinManager->IsEnabled() && pSkinManager->HasApplyOptions(xtpSkinApplyFrame))
		{
			CXTPSkinObjectFrame* pFrame = (CXTPSkinObjectFrame*)pSkinManager->Lookup(hWnd);
			if (pFrame)
			{
				return XTPAccessShared(pFrame)->DrawMenuBar();
			}
		}
	}

	typedef BOOL(WINAPI* LPFNDRAWMENUBAR)(HWND hWnd);
	return ((LPFNDRAWMENUBAR)pFunction->m_pfnOrig)(hWnd);
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookGetMenuItemRect(HWND hWnd, HMENU hMenu, UINT uItem, LPRECT lprcItem)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiGetMenuItemRect);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (pSkinManager->IsEnabled() && pSkinManager->GetApplyOptions() & xtpSkinApplyMenus)
		{
			// Get frame object
			CXTPSkinObjectApplicationFrame *pFrame = DYNAMIC_DOWNCAST(
				CXTPSkinObjectApplicationFrame, pSkinManager->Lookup(hWnd));

			if (NULL != pFrame)
			{
				//CXTPSkinPopupMenu *pMenu = pFrame->LookUp(hMenu);
			}

			return FALSE;
		}
		else
		{
			typedef BOOL(WINAPI* LPFNGETMENUITEMRECT) (HWND hWnd, HMENU hMenu, UINT uItem, LPRECT lprcItem);
			return (LPFNGETMENUITEMRECT(pFunction->m_pfnOrig))(hWnd, hMenu, uItem, lprcItem);
		}
	}
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookTrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, CONST RECT *prcRect)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiTrackPopupMenu);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (pSkinManager->IsEnabled() && pSkinManager->GetApplyOptions() & xtpSkinApplyMenus)
		{
			return CXTPSkinObjectApplicationFrame::TrackPopupMenu(hMenu, uFlags, x, y, hWnd, prcRect);
		}
		else
		{
			typedef BOOL(WINAPI* LPFNTRACKPOPUPMENU)  (HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, CONST RECT *prcRect);
			return (LPFNTRACKPOPUPMENU(pFunction->m_pfnOrig))(hMenu, uFlags, x, y, nReserved, hWnd, prcRect);
		}
	}
}

BOOL WINAPI CXTPSkinManagerApiHook::OnHookTrackPopupMenuEx(HMENU hMenu, UINT uFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpParams)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiTrackPopupMenuEx);
	if (NULL == pFunction)
	{
		return FALSE;
	}

	XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
	{
		if (pSkinManager->IsEnabled() && pSkinManager->GetApplyOptions() & xtpSkinApplyMenus)
		{
			return CXTPSkinObjectApplicationFrame::TrackPopupMenu(hMenu, uFlags, x, y, hWnd, lptpParams ? &lptpParams->rcExclude : NULL);
		}
	}

	typedef BOOL(WINAPI* LPFNTRACKPOPUPMENUEX)(HMENU hMenu, UINT uFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpParams);
	return ((LPFNTRACKPOPUPMENUEX)pFunction->m_pfnOrig)(hMenu, uFlags, x, y, hWnd, lptpParams);
}


HMODULE WINAPI CXTPSkinManagerApiHook::OnHookLoadLibraryA(LPCSTR pszModuleName)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiLoadLibraryA);
	if (NULL == pFunction)
	{
		return NULL;
	}

	BOOL bAlreadyLoaded = (NULL != GetModuleHandleA(pszModuleName));

	typedef HMODULE(WINAPI* LPFNLOADLIBRARYA)(LPCSTR pszModuleName);
	HMODULE hModule = ((LPFNLOADLIBRARYA)pFunction->m_pfnOrig)(pszModuleName);

	if (!bAlreadyLoaded && NULL != hModule)
	{
		GetInstance()->HackModuleOnLoad(hModule, 0);
	}

	return hModule;
}

HMODULE WINAPI CXTPSkinManagerApiHook::OnHookLoadLibraryW(PCWSTR pszModuleName)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiLoadLibraryW);
	if (NULL == pFunction)
	{
		return NULL;
	}

	BOOL bAlreadyLoaded = (NULL != GetModuleHandleW(pszModuleName));

	typedef HMODULE(WINAPI* LPFNLOADLIBRARYW)(PCWSTR pszModuleName);
	HMODULE hModule = ((LPFNLOADLIBRARYW)pFunction->m_pfnOrig)(pszModuleName);

	if (!bAlreadyLoaded && NULL != hModule)
	{
		GetInstance()->HackModuleOnLoad(hModule, 0);
	}

	return hModule;
}

HMODULE WINAPI CXTPSkinManagerApiHook::OnHookLoadLibraryExA(LPCSTR  pszModuleName, HANDLE hFile, DWORD dwFlags)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiLoadLibraryExA);
	if (NULL == pFunction)
	{
		return NULL;
	}

	BOOL bAlreadyLoaded = (NULL != GetModuleHandleA(pszModuleName));

	typedef HMODULE(WINAPI* LPFNLOADLIBRARYEXA)(LPCSTR pszModuleName, HANDLE hFile, DWORD dwFlags);
	HMODULE hModule = ((LPFNLOADLIBRARYEXA)pFunction->m_pfnOrig)(pszModuleName, hFile, dwFlags);

	if (!bAlreadyLoaded && NULL != hModule)
	{
		GetInstance()->HackModuleOnLoad(hModule, dwFlags);
	}

	return hModule;
}

HMODULE WINAPI CXTPSkinManagerApiHook::OnHookLoadLibraryExW(PCWSTR pszModuleName, HANDLE hFile, DWORD dwFlags)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiLoadLibraryExW);
	if (NULL == pFunction)
	{
		return NULL;
	}

	BOOL bAlreadyLoaded = (NULL != GetModuleHandleW(pszModuleName));

	typedef HMODULE(WINAPI* LPFNLOADLIBRARYEXW)(PCWSTR pszModuleName, HANDLE hFile, DWORD dwFlags);
	HMODULE hModule = ((LPFNLOADLIBRARYEXW)pFunction->m_pfnOrig)(pszModuleName, hFile, dwFlags);

	if (!bAlreadyLoaded && NULL != hModule)
	{
		GetInstance()->HackModuleOnLoad(hModule, dwFlags);
	}

	return hModule;
}

HMODULE WINAPI CXTPSkinManagerApiHook::OnHookLoadLibraryEx2W(PCWSTR pszModuleName, HANDLE hFile, DWORD dwFlags)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiLoadLibraryEx2W);
	if (NULL == pFunction)
	{
		return NULL;
	}

	BOOL bAlreadyLoaded = (NULL != GetModuleHandleW(pszModuleName));

	typedef HMODULE(WINAPI* LPFNLOADLIBRARYEXW)(PCWSTR pszModuleName, HANDLE hFile, DWORD dwFlags);
	HMODULE hModule = ((LPFNLOADLIBRARYEXW)pFunction->m_pfnOrig)(pszModuleName, hFile, dwFlags);

	if (!bAlreadyLoaded && NULL != hModule)
	{
		GetInstance()->HackModuleOnLoad(hModule, dwFlags);
	}

	return hModule;
}

HMODULE WINAPI CXTPSkinManagerApiHook::OnHookGetModuleHandleA(LPCSTR lpModuleName)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	CXTPSkinManagerApiFunction* pFunction = EnterHookedCall(xtpSkinApiGetModuleHandleA);
	if (NULL == pFunction)
	{
		return NULL;
	}

	if (lpModuleName && XTPCompareStringNoCase(lpModuleName, "UxTheme.dll") == 0)
	if (XTPSkinManager()->GetApplyOptions() & xtpSkinApplyColors)
		return XTP_UXTHEME_HANDLE;

	typedef HMODULE (WINAPI* LPFNGETMODULEHANDLEA)(LPCSTR lpModuleName);
	return ((LPFNGETMODULEHANDLEA)pFunction->m_pfnOrig)(lpModuleName);
}

FARPROC WINAPI CXTPSkinManagerApiHook::OnHookGetProcAddress(HMODULE hModule, PCSTR pszProcName)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	if (!EnterHookedCall())
	{
		return NULL;
	}

	if ((DWORD_PTR)pszProcName < 0xFFFF)
		return GetProcAddressWindows(hModule, pszProcName);

	CXTPSkinManagerApiFunction* pFuncHook = NULL;
	if (XTP_UXTHEME_HANDLE == hModule)
	{
		pFuncHook = EnterHookedCall("UXTHEME.DLL", pszProcName);
	}
	else
	{
		char szFullFileName[MAX_PATH];
		::GetModuleFileNameA(hModule, szFullFileName, MAX_PATH);
		// We must extract only the name and the extension

		char* szFileName = strrchr(szFullFileName, '\\');
		if (szFileName == NULL)
			szFileName = szFullFileName;
		else
			szFileName = szFileName + 1;

		if (XTPCompareStringNoCase(szFileName, "UxTheme.dll") == 0)
			return GetProcAddressWindows(hModule, pszProcName);

		pFuncHook = EnterHookedCall(szFileName, pszProcName);
	}


	if (pFuncHook)
		return pFuncHook->m_pfnHook;

	return GetProcAddressWindows(hModule, pszProcName);
}



HTHEME STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookOpenThemeData(HWND hWnd, LPCWSTR pszClassList)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	if (EnterHookedCall())
	{
		XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pSkinManager)
		{
			if (!pSkinManager->IsEnabled())
				return 0;

			CXTPSkinManagerClass* pClass = pSkinManager->GetSkinClass(pSkinManager->Lookup(hWnd), CString(pszClassList));
			return (HTHEME)pClass;
		}
	}

	return NULL;
}

HRESULT STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookDrawThemeBackground(HTHEME hTheme, HDC hDC, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	if (!hTheme)
		return E_FAIL;

	if (EnterHookedCall())
	{
		XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
		{
			CXTPSkinManagerClass* pClass = XTPSkinManager()->FromHandle(hTheme);
			if (!pClass)
				return E_FAIL;

			XTP_GUARD_SHARED(CXTPSkinManagerClass, pClass)
			{
				HRGN hrgnClip = 0;

				if (pClipRect && pRect && !EqualRect(pClipRect, pRect))
				{
					hrgnClip = CreateRectRgn(0, 0, 0, 0);
					if (hrgnClip != NULL)
					{
						if (GetClipRgn(hDC, hrgnClip) != 1)
						{
							DeleteObject(hrgnClip);
							hrgnClip = (HRGN)-1;
						}
						::IntersectClipRect(hDC, pClipRect->left, pClipRect->top, pClipRect->right, pClipRect->bottom);
					}
				}


				HRESULT hResult = S_OK;
				if (!pClass->DrawThemeBackground(CDC::FromHandle(hDC), iPartId, iStateId, pRect))
					hResult = E_FAIL;

				if (hrgnClip != NULL)
				{
					if (hrgnClip == (HRGN)-1)
					{
						ExtSelectClipRgn(hDC, NULL, RGN_COPY);
					}
					else
					{
						ExtSelectClipRgn(hDC, hrgnClip, RGN_COPY);
						DeleteObject(hrgnClip);
					}
				}

				return hResult;
			}
		}
	}

	return E_FAIL;
}


HRESULT STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookCloseThemeData(HTHEME)
{
	return EnterHookedCall() ? S_OK : E_FAIL;
}

HRESULT STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookGetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropID, COLORREF *pColor)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	if (!hTheme)
		return E_FAIL;

	if (EnterHookedCall())
	{
		XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
		{
			CXTPSkinManagerClass* pClass = XTPSkinManager()->FromHandle(hTheme);
			if (!pClass)
				return E_FAIL;

			COLORREF clr = XTPAccessShared(pClass)->GetThemeColor(iPartId, iStateId, iPropID);

			if (clr == (COLORREF)-1)
				return E_FAIL;

			*pColor = clr;
			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookGetThemeInt(HTHEME hTheme, int iPartId, int iStateId, int iPropID, int *piVal)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	if (!hTheme)
		return E_FAIL;

	if (EnterHookedCall())
	{
		XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
		{
			CXTPSkinManagerClass* pClass = XTPSkinManager()->FromHandle(hTheme);
			if (!pClass)
				return E_FAIL;

			CXTPSkinManagerSchemaProperty* pProperty = XTPAccessShared(pClass)->GetProperty(XTP_SKINPROPERTY_INT, iPartId, iStateId, iPropID);

			if (!pProperty)
				return E_FAIL;

			if (piVal) *piVal = pProperty->iVal;
			return S_OK;
		}
	}

	return E_FAIL;
}

BOOL STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookIsThemeActive()
{
	return EnterHookedCall() ? TRUE : FALSE;
}

BOOL STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookIsAppThemed()
{
	return EnterHookedCall() ? TRUE : FALSE;
}

HRESULT STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookGetThemePartSize(HTHEME hTheme, HDC, int iPartId, int iStateId, RECT *pRect, THEMESIZE eSize, SIZE* pSize)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();

	if (!hTheme)
		return E_FAIL;

	if (EnterHookedCall())
	{
		XTP_GUARD_SHARED(CXTPSkinManager, XTPSkinManager())
		{
			CXTPSkinManagerClass* pClass = XTPSkinManager()->FromHandle(hTheme);
			if (!pClass)
				return E_FAIL;

			if (!XTPAccessShared(pClass)->GetThemePartSize(iPartId, iStateId, pRect, eSize, pSize))
				return E_FAIL;

			return S_OK;
		}
	}

	return E_FAIL;
}


BOOL STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookGetThemeSysBool(HTHEME /*hTheme*/, int iBoolId)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();
	return EnterHookedCall() 
		? XTPAccessShared(XTPSkinManager())->GetThemeSysBool(iBoolId)
		: FALSE;
}

COLORREF STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookGetThemeSysColor(HTHEME /*hTheme*/, int iColorId)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();
	return EnterHookedCall()
		? XTPAccessShared(XTPSkinManager())->GetThemeSysColor(iColorId)
		: 0;
}


HRESULT STDAPICALLTYPE CXTPSkinManagerApiHook::OnHookGetCurrentThemeName(LPWSTR pszThemeFileName, int dwMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR /*pszSizeBuff*/, int /*cchMaxSizeChars*/)
{
	XTP_SKINFRAMEWORK_MANAGE_STATE();
	if (EnterHookedCall())
	{
		XTP_GUARD_SHARED_(CXTPSkinManager, XTPSkinManager(), pManager)
		{
			if (!pManager->GetResourceFile())
				return E_FAIL;

			if (pszThemeFileName)
			{
				CString strThemeFileName = XTPAccessShared(pManager->GetResourceFile())->GetResourcePath();
				int nLength = strThemeFileName.GetLength();

				if (nLength < dwMaxNameChars)
				{
					MBSTOWCS_S(pszThemeFileName, strThemeFileName, nLength + 1);
				}
			}

			if (pszColorBuff)
			{
				CString strColorBuff = XTPAccessShared(pManager->GetResourceFile())->GetIniFileName();
				strColorBuff.MakeUpper();

				if (strColorBuff.Find(_T("METALLIC")) > 0) strColorBuff = _T("metallic");
				if (strColorBuff.Find(_T("BLUE")) > 0) strColorBuff = _T("normalcolor");
				if (strColorBuff.Find(_T("HOMESTEAD")) > 0) strColorBuff = _T("homestead");


				int nLength = strColorBuff.GetLength();

				if (nLength < cchMaxColorChars)
				{
					MBSTOWCS_S(pszColorBuff, strColorBuff, nLength + 1);
				}
			}
		}
		return S_OK;
	}

	return E_FAIL;
}
