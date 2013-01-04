// XTPResourceManager.cpp: implementation of the CXTPResourceManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#include "XTPVC80Helpers.h"

#include "XTPResourceManager.h"
#include "XTPMacros.h"
#include "XTPPropExchange.h"
#include "XTPSystemHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAKELANGINFO(l, s, w, e, c, f) {MAKELANGID(l, s), w, e, c, _T(#l), _T(#s), f}
#define ENGLISH_CHARSET ANSI_CHARSET

const WORD cwXTPResManDefLangID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

static XTP_RESOURCEMANAGER_LANGINFO langinfo[] =
{
	MAKELANGINFO(LANG_RUSSIAN, SUBLANG_DEFAULT, 1251, _T("windows-1251"), _T("Russian"), RUSSIAN_CHARSET),
	MAKELANGINFO(LANG_JAPANESE, SUBLANG_DEFAULT, 932, _T("shift-jis"), _T("Japanese"), SHIFTJIS_CHARSET),
	MAKELANGINFO(LANG_ARABIC, SUBLANG_ARABIC_SAUDI_ARABIA, 1256, _T("windows-1256"), _T("Arabic (Saudi Arabia)"), ARABIC_CHARSET),
	MAKELANGINFO(LANG_CZECH, SUBLANG_DEFAULT, 1250, _T("windows-1250"), _T("Czech"), EASTEUROPE_CHARSET),
	MAKELANGINFO(LANG_DANISH, SUBLANG_DEFAULT, 1252, _T("windows-1252"), _T("Danish"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_GERMAN, SUBLANG_GERMAN, 1252, _T("windows-1252"), _T("German (Germany)"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_GREEK, SUBLANG_DEFAULT, 1253, _T("windows-1253"), _T("Greek"), GREEK_CHARSET),
	MAKELANGINFO(LANG_ENGLISH, SUBLANG_ENGLISH_US, CP_ACP, _T("windows-1252"), _T("English (United States)"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_SPANISH, SUBLANG_SPANISH_MODERN, 1252, _T("windows-1252"), _T("Spanish (Spain - Modern Sort)"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_ESTONIAN, SUBLANG_DEFAULT, 1257, _T("windows-1257"), _T("Estonian"), BALTIC_CHARSET),
	MAKELANGINFO(LANG_FINNISH, SUBLANG_DEFAULT, 1252, _T("windows-1252"), _T("Finnish"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_FRENCH, SUBLANG_FRENCH, 1252, _T("windows-1252"), _T("French (France)"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_HEBREW, SUBLANG_DEFAULT, 1255, _T("windows-1255"), _T("Hebrew"), HEBREW_CHARSET),
	MAKELANGINFO(LANG_CROATIAN, SUBLANG_DEFAULT, 1250, _T("windows-1250"), _T("Croatian"), EASTEUROPE_CHARSET),
	MAKELANGINFO(LANG_HUNGARIAN, SUBLANG_DEFAULT, 1250, _T("windows-1250"), _T("Hungarian"), EASTEUROPE_CHARSET),
	MAKELANGINFO(LANG_ITALIAN, SUBLANG_ITALIAN, 1252, _T("windows-1252"), _T("Italian (Italy)"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_KOREAN, SUBLANG_DEFAULT, 949, _T("ks_c_5601"), _T("Korean"), HANGEUL_CHARSET),
	MAKELANGINFO(LANG_LITHUANIAN, SUBLANG_LITHUANIAN, 1257, _T("windows-1257"), _T("Lithuanian"), BALTIC_CHARSET),
	MAKELANGINFO(LANG_LATVIAN, SUBLANG_DEFAULT, 1257, _T("windows-1257"), _T("Latvian"), BALTIC_CHARSET),
	MAKELANGINFO(LANG_DUTCH, SUBLANG_DUTCH, 1252, _T("windows-1252"), _T("Dutch"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL, 1252, _T("windows-1252"), _T("Norwegian (Bokmal)"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_POLISH, SUBLANG_DEFAULT, 1250, _T("windows-1250"), _T("Polish"), EASTEUROPE_CHARSET),
	MAKELANGINFO(LANG_PORTUGUESE, SUBLANG_PORTUGUESE, 1252, _T("windows-1252"), _T("Portuguese (Portugal)"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN, 1252, _T("windows-1252"), _T("Portuguese (Brazil)"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_ROMANIAN, SUBLANG_DEFAULT, 1250, _T("windows-1250"), _T("Romanian"), EASTEUROPE_CHARSET),
	MAKELANGINFO(LANG_SERBIAN, SUBLANG_SERBIAN_LATIN, 1250, _T("windows-1250"), _T("Serbian"), EASTEUROPE_CHARSET),
	MAKELANGINFO(LANG_SLOVAK, SUBLANG_DEFAULT, 1250, _T("windows-1250"), _T("Slovak"), EASTEUROPE_CHARSET),
	MAKELANGINFO(LANG_SLOVENIAN, SUBLANG_DEFAULT, 1250, _T("windows-1250"), _T("Slovenian"), EASTEUROPE_CHARSET),
	MAKELANGINFO(LANG_SWEDISH, SUBLANG_DEFAULT, 1252, _T("windows-1252"), _T("Swedish"), ENGLISH_CHARSET),
	MAKELANGINFO(LANG_THAI, SUBLANG_DEFAULT, 874, _T("windows-874"), _T("Thai"), THAI_CHARSET),
	MAKELANGINFO(LANG_TURKISH, SUBLANG_DEFAULT, 1254, _T("windows-1254"), _T("Turkish"), TURKISH_CHARSET),
	MAKELANGINFO(LANG_UKRAINIAN, SUBLANG_DEFAULT, 1251, _T("windows-1251"), _T("Ukrainian "), RUSSIAN_CHARSET),
	MAKELANGINFO(LANG_BULGARIAN, SUBLANG_DEFAULT, 1251, _T("windows-1251"), _T("Bulgarian"), RUSSIAN_CHARSET),
	MAKELANGINFO(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED, 936, _T("gb2312"), _T("Chinese (PRC)"), GB2312_CHARSET),
	MAKELANGINFO(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL, 950, _T("big5"), _T("Chinese (Taiwan)"), CHINESEBIG5_CHARSET)
};

//////////////////////////////////////////////////////////////////////////
// CW2CT

CXTPResourceManager::CXTPW2A::CXTPW2A(LPCWSTR lpwstr)
{
	UINT nCodePage = XTPResourceManager()->GetLanguageInfo()->nCodePage;
	int nDestLen = WideCharToMultiByte(nCodePage, 0, lpwstr, -1, NULL, 0, NULL, NULL);

	m_lpsrt = new CHAR[nDestLen + 1];
	WideCharToMultiByte(nCodePage, 0, lpwstr, -1, m_lpsrt, nDestLen, NULL, NULL);
	m_lpsrt[nDestLen] = 0;
}

CXTPResourceManager::CXTPW2A::~CXTPW2A()
{
	delete[] m_lpsrt;
}

CXTPResourceManager::CXTPW2A::operator LPCSTR() const
{
	return m_lpsrt;
}

CXTPResourceManager::CXTPA2W::CXTPA2W(LPCSTR lptstr)
{
	UINT nCodePage = XTPResourceManager()->GetLanguageInfo()->nCodePage;
	int nDestLen = MultiByteToWideChar(nCodePage, 0, lptstr, -1, NULL, 0);

	m_lpwsrt = new WCHAR[nDestLen + 1];
	MultiByteToWideChar(nCodePage, 0, lptstr, -1, m_lpwsrt, nDestLen);
	m_lpwsrt[nDestLen] = 0;
}

CXTPResourceManager::CXTPA2W::~CXTPA2W()
{
	delete[] m_lpwsrt;
}

CXTPResourceManager::CXTPA2W::operator LPCWSTR() const
{
	return m_lpwsrt;
}


//////////////////////////////////////////////////////////////////////////
// CXTPResourceManager

CProcessLocal<CXTPResourceManager> CXTPResourceManager::m_managerInstance;


CXTPResourceManager* AFX_CDECL XTPResourceManager()
{
	return CXTPResourceManager::m_managerInstance.GetData();
}


BOOL CXTPResourceManager::SetResourceManager(CXTPResourceManager* pResourceManager)
{
	if (pResourceManager == NULL)
		return FALSE;

	if (!pResourceManager->m_bValid)
	{
		delete pResourceManager;
		return FALSE;

	}

	pResourceManager->UpdateResourcesLangID();


	if (m_managerInstance.m_pObject != NULL)
		delete m_managerInstance.m_pObject;

	m_managerInstance.m_pObject = pResourceManager;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// CXTPResourceManager


HINSTANCE CXTPResourceManager::m_hModXTResource = NULL;

CXTPResourceManager::CManageState::CManageState()
{
	m_bSet = false;
	Redo();
}

CXTPResourceManager::CManageState::~CManageState()
{
	Undo();
	m_hModOldResource = NULL;
}

void CXTPResourceManager::CManageState::Undo()
{
	if (!m_bSet)
		return;

	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	pModuleState->m_hCurrentResourceHandle = m_hModOldResource;

	m_bSet = false;
}

void CXTPResourceManager::CManageState::Redo()
{
	if (XTPResourceManager()->GetResourceHandle() == NULL)
	{
		m_bSet = false;
		return;
	}

	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	m_hModOldResource = pModuleState->m_hCurrentResourceHandle;

	pModuleState->m_hCurrentResourceHandle = XTPResourceManager()->GetResourceHandle();

	m_bSet = true;
}


CXTPResourceManager::CXTPResourceManager()
{
	m_bValid = TRUE;
	m_hResourceFile = 0;

	m_pLanguageInfo = GetLanguageInfo(cwXTPResManDefLangID);

	LOGFONT lfIcon;
	VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lfIcon), &lfIcon, 0));
	m_nSystemCharset = lfIcon.lfCharSet;

	m_bResourceCharset = FALSE;
}

CXTPResourceManager::~CXTPResourceManager()
{
	Close();
}

void CXTPResourceManager::Close()
{
	if (m_hResourceFile != 0)
	{
		FreeLibrary(m_hResourceFile);
		m_hResourceFile = 0;
	}
}

HMODULE CXTPResourceManager::GetResourceHandle() const
{
	return m_hModXTResource;
}

HRSRC CXTPResourceManager::FindResource(HMODULE& hModule, LPCTSTR lpName, LPCTSTR lpType)
{
	// Try to locate the resource in the library resources.
	hModule = GetResourceHandle();
	HRSRC hResource = ::FindResource(hModule, lpName, lpType);

	if (hResource == NULL)
	{
		// Try to locate the resource by walking the resource chain until
		// a handle to the module containing the resource if found.
		hModule = AfxFindResourceHandle(lpName, lpType);
		hResource = ::FindResource(hModule, lpName, lpType);
	}

	return hResource;
}

void CXTPResourceManager::SetResourceHandle(HMODULE hModRes)
{
	m_hModXTResource = hModRes;
	UpdateResourcesLangID();
}

void CXTPResourceManager::SetResourceFile(const CString& strResourceFile)
{
	CString strExtension = strResourceFile.GetLength() > 3 ? strResourceFile.Right(3) : _T("");
	strExtension.MakeLower();

	if (strExtension == _T("xml"))
	{
#ifndef _XTP_EXCLUDE_XML
		SetResourceManager(new CXTPResourceManagerXML(strResourceFile));
#endif
	}
	else
	{
		CXTPResourceManager* pResourceManager = new CXTPResourceManager;

		pResourceManager->m_hResourceFile = strResourceFile.IsEmpty() ? 0 : LoadLibrary(strResourceFile);

		if (pResourceManager->m_hResourceFile)
		{
			SetResourceHandle(pResourceManager->m_hResourceFile);
		}
		else
		{
			SetResourceHandle(AfxGetInstanceHandle());
		}

		SetResourceManager(pResourceManager);
	}
}

void CXTPResourceManager::UpdateResourcesLangID()
{
	HMODULE hResModule = GetResourceHandle();
	WORD wLangID = GetResourceLanguage(hResModule);
	ASSERT(wLangID);

	SetResourceLanguage(wLangID);
}

void CXTPResourceManager::SetResourceLanguage(WORD wLangID)
{
	m_pLanguageInfo = GetLanguageInfo(wLangID);

	if (!m_pLanguageInfo)
		m_pLanguageInfo = GetLanguageInfo(cwXTPResManDefLangID);
}

WORD CXTPResourceManager::GetResourceLanguage(HMODULE hResModule)
{
	WORD wLangID = 0;
	::EnumResourceTypes(hResModule, &CXTPResourceManager::EnumResTypeProc, (LONG_PTR)&wLangID);
	return wLangID;
}

BOOL CXTPResourceManager::EnumResTypeProc(HMODULE hModule, LPTSTR lpszType, LONG_PTR lParam)
{
	WORD* pwLangID = (WORD*)lParam;
	ASSERT(pwLangID);
	if (!pwLangID)
		return FALSE;

	::EnumResourceNames(hModule, lpszType, &CXTPResourceManager::EnumResNameProc, lParam);

	if (*pwLangID == LANG_NEUTRAL)
		return TRUE; // continue if neutral

	return *pwLangID == cwXTPResManDefLangID; // continue if default
}

BOOL CXTPResourceManager::EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	WORD* pwLangID = (WORD*)lParam;
	ASSERT(pwLangID);
	if (!pwLangID)
		return FALSE;

	::EnumResourceLanguages(hModule, lpszType, lpszName,
							&CXTPResourceManager::EnumResLangProc, lParam);

	if (*pwLangID == LANG_NEUTRAL)
		return TRUE; // continue if neutral

	return *pwLangID == cwXTPResManDefLangID; // continue if default
}

BOOL CXTPResourceManager::EnumResLangProc(HMODULE hModule, LPCTSTR lpszType,
		LPCTSTR lpszName, WORD wIDLanguage, LONG_PTR lParam)
{
	UNREFERENCED_PARAMETER(hModule); UNREFERENCED_PARAMETER(lpszType); UNREFERENCED_PARAMETER(lpszName);

	WORD* pwLangID = (WORD*)lParam;
	ASSERT(pwLangID);

	if (!pwLangID)
		return FALSE; // stop emuneration

	if (wIDLanguage == LANG_NEUTRAL)
		return TRUE; // continue if neutral

	*pwLangID = wIDLanguage;

	return *pwLangID == cwXTPResManDefLangID; // continue if default
}

BOOL CXTPResourceManager::LoadFailed()
{
	return FALSE;
}

BYTE CXTPResourceManager::GetFontCharset() const
{
	return m_pLanguageInfo && m_bResourceCharset ? m_pLanguageInfo->nFontCharSet : m_nSystemCharset;
}

void CXTPResourceManager::AssertValid(BOOL bValid)
{
	if (!bValid)
	{
		XTP_ERROR_MSG(
			"Error Locating Toolkit Resources.\n\n"
			"Make sure you have included the resources to your project's .rc2 file.\n\n"
			"Add #include <XTToolkitPro.rc> to your project .rc2 file");
	}
}

BOOL CXTPResourceManager::LoadString(CString* pString, UINT nIDResource)
{
	HMODULE hResourceInstanse = GetResourceHandle();

	if (hResourceInstanse && ::FindResource(hResourceInstanse, MAKEINTRESOURCE((nIDResource >> 4)+1), RT_STRING))
	{
		if (LoadLocaleString(hResourceInstanse, nIDResource, *pString))
			return TRUE;
	}

	if (pString->LoadString(nIDResource))
		return TRUE;

	return LoadFailed();
}

BOOL CXTPResourceManager::LoadMenu(CMenu* lpMenu, UINT nIDResource)
{
	HMODULE hResourceInstanse = GetResourceHandle();

	if (hResourceInstanse && ::FindResource(hResourceInstanse, MAKEINTRESOURCE(nIDResource), RT_MENU))
	{
		if (lpMenu->Attach(::LoadMenu(hResourceInstanse, MAKEINTRESOURCE(nIDResource))))
			return TRUE;
	}

	if (lpMenu->LoadMenu(nIDResource))
		return TRUE;

	return LoadFailed();
}

BOOL CXTPResourceManager::LoadToolBar(CToolBar* pToolBar, UINT nIDResource)
{
	CManageState state;

	if (pToolBar->LoadToolBar(nIDResource))
		return TRUE;

	return LoadFailed();
}

BOOL CXTPResourceManager::LoadBitmap(CBitmap* pBitmap, UINT nIDResource)
{
	HMODULE hResourceInstanse = GetResourceHandle();

	if (hResourceInstanse && ::FindResource(hResourceInstanse, MAKEINTRESOURCE(nIDResource), RT_BITMAP))
	{
		if (pBitmap->Attach(::LoadBitmap(hResourceInstanse, MAKEINTRESOURCE(nIDResource))))
			return TRUE;

	}

	if (pBitmap->LoadBitmap(nIDResource))
		return TRUE;

	return LoadFailed();
}

HCURSOR CXTPResourceManager::LoadCursor(UINT nIDResource)
{
	HMODULE hResourceInstanse = GetResourceHandle();

	if (hResourceInstanse && ::FindResource(hResourceInstanse, MAKEINTRESOURCE(nIDResource), RT_GROUP_CURSOR))
	{
		HCURSOR hCursor = ::LoadCursor(hResourceInstanse, MAKEINTRESOURCE(nIDResource));
		if (hCursor)
			return hCursor;
	}

	HCURSOR hCursor = AfxGetApp()->LoadCursor(nIDResource);
	if (hCursor)
		return hCursor;

	LoadFailed();
	return NULL;
}

HGLOBAL CXTPResourceManager::LoadDialogTemplate2(LPCTSTR pszTemplate)
{
	HINSTANCE hResourceInstanse = GetResourceHandle();
	HRSRC hResource = hResourceInstanse ? FindResource(hResourceInstanse, pszTemplate, RT_DIALOG) : NULL;

	if (hResource == NULL)
	{
		hResourceInstanse = AfxFindResourceHandle(pszTemplate, RT_DIALOG);
		hResource = hResourceInstanse ? FindResource(hResourceInstanse, pszTemplate, RT_DIALOG) : NULL;
	}

	if (hResource)
	{
		HGLOBAL hTemplate = LoadResource(hResourceInstanse, hResource);
		return hTemplate;
	}

	LoadFailed();
	return NULL;
}

LPCDLGTEMPLATE CXTPResourceManager::LoadDialogTemplate(UINT nIDResource)
{
	if (!nIDResource)
		return 0;

	LPCTSTR pszTemplate = MAKEINTRESOURCE(nIDResource);

	HGLOBAL hTemplate = LoadDialogTemplate2(pszTemplate);
	return (LPCDLGTEMPLATE)LockResource(hTemplate);
}

BOOL CXTPResourceManager::LoadHTML(CString* pText, UINT nIDResource)
{
	if (pText == NULL)
		return FALSE;

	HMODULE hResourceInstanse = GetResourceHandle();
	HRSRC hResource = ::FindResource(hResourceInstanse, MAKEINTRESOURCE(nIDResource), RT_HTML);

	if (hResource == NULL)
	{
		hResourceInstanse = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_HTML);
		if (hResourceInstanse == NULL)
			return FALSE;

		hResource = ::FindResource(hResourceInstanse, MAKEINTRESOURCE(nIDResource), RT_HTML);
	}

	if (!hResource)
		return FALSE;

	HGLOBAL hMem = ::LoadResource(hResourceInstanse, hResource);
	if (!hMem)
		return FALSE;

	DWORD dwSize = ::SizeofResource(hResourceInstanse, hResource);
	char *pSrc = (char*)::LockResource(hMem);
	if (!pSrc)
		return FALSE;

	const DWORD dwDstSize = (dwSize + 1) * sizeof(TCHAR);
	TCHAR *pDst = pText->GetBuffer(dwDstSize);
	if (pDst == NULL)
		return FALSE;

	::ZeroMemory((BYTE*)pDst, dwDstSize);

#ifdef _UNICODE
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pSrc, dwSize,
		pDst, dwDstSize);
#else
	::CopyMemory(pDst, pSrc, dwSize);
	int nLen = dwSize;
#endif

	pText->ReleaseBuffer();

	return nLen > 0;
}

HICON CXTPResourceManager::CreateIconFromResource(HMODULE hModule, LPCTSTR lpszID, CSize iconSize)
{
	// Find the icon directory whose identifier is lpszID.
	HRSRC hResource = ::FindResource(hModule,
		lpszID, RT_GROUP_ICON);

	if (hResource == NULL)
		return NULL;

	// Load and lock the icon directory.
	HGLOBAL hMem = ::LoadResource(hModule, hResource);
	if (!hMem)
		return NULL;
	LPVOID lpResource = ::LockResource(hMem);

	// Get the identifier of the icon that is most appropriate
	// for the video display.
	int nID = ::LookupIconIdFromDirectoryEx((PBYTE) lpResource, TRUE,
		iconSize.cx, iconSize.cy, LR_DEFAULTCOLOR);

	// Find the bits for the nID icon.
	hResource = ::FindResource(hModule,
		MAKEINTRESOURCE(nID), MAKEINTRESOURCE(RT_ICON));

	if (hResource == NULL)
		return NULL;

	// Load and lock the icon.
	hMem = ::LoadResource(hModule, hResource);
	if (!hMem)
		return NULL;
	lpResource = ::LockResource(hMem);

	// Create a handle to the icon.
	HICON hIcon = ::CreateIconFromResourceEx((PBYTE)lpResource,
		::SizeofResource(hModule, hResource), TRUE, 0x00030000,
		iconSize.cx, iconSize.cy, LR_DEFAULTCOLOR);

	return hIcon;
}

HICON CXTPResourceManager::LoadIcon(LPCTSTR lpszID, CSize iconSize)
{
	HICON hIcon = NULL;

	HMODULE hResource = GetResourceHandle();
	if (hResource && ::FindResource(hResource, lpszID, RT_GROUP_ICON))
	{
		hIcon = (HICON)::LoadImage(hResource, lpszID, IMAGE_ICON, iconSize.cx, iconSize.cy, 0);

		if (hIcon != NULL)
			return hIcon;
	}

	hResource = AfxFindResourceHandle(lpszID, RT_GROUP_ICON);
	if (hResource)
	{
		hIcon = (HICON)::LoadImage(hResource, lpszID, IMAGE_ICON, iconSize.cx, iconSize.cy, 0);

		if (hIcon != NULL)
			return hIcon;
	}

	return NULL;
}

HICON CXTPResourceManager::LoadIcon(int nID, CSize iconSize)
{
	return LoadIcon(MAKEINTRESOURCE(nID), iconSize);
}

int CXTPResourceManager::ShowMessageBox(UINT nIDPrompt, UINT nType)
{
	CString strPrompt;
	VERIFY(LoadString(&strPrompt, nIDPrompt));

	return ShowMessageBox(strPrompt, nType);
}

int CXTPResourceManager::ShowMessageBox(LPCTSTR lpszText, UINT nType)
{
	if (m_pLanguageInfo && (m_pLanguageInfo->nCodePage == 1256 || m_pLanguageInfo->nCodePage == 1255)) // Arabic or Hebrew ?
		nType |= MB_RTLREADING;

	return AfxMessageBox(lpszText, nType);
}

//////////////////////////////////////////////////////////////////////////
// Locale methods

XTP_RESOURCEMANAGER_LANGINFO* CXTPResourceManager::GetLanguageInfo(UINT nLangId)
{
	for (int i = 0; i < _countof(langinfo); i++)
	{
		if (nLangId == langinfo[i].wLanguage)
			return &langinfo[i];
	}
	return 0;
}

int CXTPResourceManager::GetMenuLocaleString(CMenu* pMenu, UINT nIDItem, CString& rString, UINT nFlags) const
{
	if (!pMenu || !pMenu->m_hMenu)
		return 0;

	ASSERT(m_pLanguageInfo);
	if (!m_pLanguageInfo)
	{
		return pMenu->GetMenuString(nIDItem, rString, nFlags);
	}

	ASSERT(::IsMenu(pMenu->m_hMenu));

	// offer no buffer first
	int nStringLen = ::GetMenuString(pMenu->m_hMenu, nIDItem, NULL, 0, nFlags);

	// use exact buffer length
	if (nStringLen > 0)
	{
#ifndef _UNICODE
		if (XTPSystemVersion()->IsWin2KOrGreater())
		{
			LPWSTR lpwstr = new WCHAR[nStringLen + 1];
			::GetMenuStringW(pMenu->m_hMenu, nIDItem, lpwstr, nStringLen + 1, nFlags);

			int nDestLen = WideCharToMultiByte(m_pLanguageInfo->nCodePage, 0, lpwstr, nStringLen, NULL, 0, NULL, NULL);
			LPTSTR pstrString = rString.GetBufferSetLength(nDestLen);

			WideCharToMultiByte(m_pLanguageInfo->nCodePage, 0, lpwstr, nStringLen, pstrString, nDestLen, NULL, NULL);
			pstrString[nDestLen] = 0;
			delete[] lpwstr;
			rString.ReleaseBuffer();
		}
		else
#endif
		{
			LPTSTR pstrString = rString.GetBufferSetLength(nStringLen);
			::GetMenuString(pMenu->m_hMenu, nIDItem, pstrString, nStringLen + 1, nFlags);
			rString.ReleaseBuffer(nStringLen);
		}
	}
	else
		rString.Empty();

	return nStringLen;

}

int CXTPResourceManager::LoadLocaleString(HINSTANCE hInstance, UINT nID, CString& rString) const
{
	rString.Empty();

	HRSRC hResource = ::FindResource(hInstance, MAKEINTRESOURCE(((nID >> 4) + 1)), RT_STRING);
	if (!hResource)
		return 0;

	HGLOBAL hGlobal = ::LoadResource(hInstance, hResource);
	if (!hGlobal)
		return 0;

	LPWORD pImage = (LPWORD)::LockResource(hGlobal);
	if (!pImage)
		return 0;

	ULONG nResourceSize = ::SizeofResource(hInstance, hResource);
	WORD* pImageEnd = (LPWORD)(LPBYTE(pImage) + nResourceSize);
	UINT iIndex = nID & 0x000F;

	while ((iIndex > 0) && (pImage < pImageEnd))
	{
		pImage = (LPWORD)(LPBYTE(pImage) + (sizeof(WORD) + (*pImage * sizeof(WCHAR))));
		iIndex--;
	}

	if (pImage >= pImageEnd)
		return 0;

	int nLength = *pImage;

	if (nLength == 0)
		return 0;


#ifdef _UNICODE
	LPTSTR lpsz = rString.GetBufferSetLength(nLength);
	MEMCPY_S(lpsz, (LPWSTR)(pImage + 1), nLength * sizeof(WCHAR));
	rString.ReleaseBuffer(nLength);
	return nLength;

#else
	UINT nCodePage = m_pLanguageInfo ? m_pLanguageInfo->nCodePage : CP_ACP;
	int nDestLen = WideCharToMultiByte(nCodePage, 0, (LPWSTR)(pImage + 1), nLength, NULL, 0, NULL, NULL);

	LPTSTR lpsz = rString.GetBufferSetLength(nDestLen);
	WideCharToMultiByte(nCodePage, 0, (LPWSTR)(pImage + 1), nLength, lpsz, nDestLen, NULL, NULL);
	rString.ReleaseBuffer(nDestLen);
	return nDestLen;
#endif


}



#ifndef  _XTP_EXCLUDE_XML


//////////////////////////////////////////////////////////////////////////
// CXTPResourceManagerXML

CXTPResourceManagerXML::CXTPResourceManagerXML(LPCTSTR strFileName)
{
	m_pResourceRoot = new CXTPPropExchangeXMLNode(TRUE, 0, _T("resource"));

	m_bValid = FALSE;

	if (!m_pResourceRoot->LoadFromFile(strFileName))
		return;

	if (!m_pResourceRoot->OnBeforeExchange())
		return;

	m_bValid = TRUE;
}

CXTPResourceManagerXML::CXTPResourceManagerXML(CXTPPropExchangeXMLNode* pResourceRoot)
{
	m_bValid = FALSE;

	m_pResourceRoot = pResourceRoot;

	if (m_pResourceRoot == NULL)
		return;

	if (!m_pResourceRoot->OnBeforeExchange())
		return;

	m_bValid = TRUE;
}


CXTPResourceManagerXML::~CXTPResourceManagerXML()
{
	Close();
}

void CXTPResourceManagerXML::Close()
{
	CXTPResourceManager::Close();

	CMDTARGET_RELEASE(m_pResourceRoot);

	POSITION pos = m_mapDialogs.GetStartPosition();
	while (pos)
	{
		DLGTEMPLATE* pDlgTemplate;
		UINT nIDResource;
		m_mapDialogs.GetNextAssoc(pos, nIDResource, pDlgTemplate);
		free (pDlgTemplate);
	}
	m_mapDialogs.RemoveAll();

	m_bValid = FALSE;
}

void CXTPResourceManagerXML::UpdateResourcesLangID()
{
	long wResourcesLangID = cwXTPResManDefLangID;
	PX_Long(m_pResourceRoot, _T("LANGID"), wResourcesLangID);

	SetResourceLanguage((WORD)wResourcesLangID);
}

BOOL CXTPResourceManagerXML::LoadString(CString* pString, UINT nIDResource)
{
	if (!m_bValid || m_pResourceRoot == NULL)
		return CXTPResourceManager::LoadString(pString, nIDResource);

	CString strPattern;
	strPattern.Format(_T("string[@id = \"%i\"]"), nIDResource);

	if (!m_pResourceRoot->IsSectionExists(strPattern))
		return CXTPResourceManager::LoadString(pString, nIDResource);


	CXTPPropExchangeSection secString(m_pResourceRoot->GetSection(strPattern));
	PX_String(&secString, _T("value"), *pString);

	return TRUE;
}

struct CXTPResourceManagerXML::MENUITEMTEMPLATEINFO : public MENUITEMTEMPLATE
{
	BSTR lpszCaption;

public:
	MENUITEMTEMPLATEINFO()
	{
		mtOption = 0;
		lpszCaption = NULL;
	}
	~MENUITEMTEMPLATEINFO()
	{
		if (lpszCaption)
		{
			SysFreeString(lpszCaption);
		}
	}
	UINT GetLength() const
	{
		return sizeof(mtOption) + sizeof(WCHAR) * ((int)wcslen(lpszCaption) + 1) +
			(mtOption & MF_POPUP ? 0 : sizeof(mtID));
	}
};

struct CXTPResourceManagerXML::MENUTEMPLATEINFO
{
	CArray<MENUITEMTEMPLATEINFO*, MENUITEMTEMPLATEINFO*> aItems;

public:
	~MENUTEMPLATEINFO()
	{
		for (int i = 0; i < (int)aItems.GetSize(); i++)
		{
			delete aItems[i];
		}
	}
	UINT GetLength() const
	{
		int nLength = sizeof(MENUITEMTEMPLATEHEADER);

		for (int i = 0; i < aItems.GetSize(); i++)
		{
			nLength += aItems[i]->GetLength();
		}

		return nLength;
	}
};

/////////////////////////////////////////////////////////////////////////////
// Extended dialog templates (new in Win95)

#pragma pack(push, 1)

struct CXTPResourceManagerXML::DLGTEMPLATEEX
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
};

struct CXTPResourceManagerXML::DLGITEMTEMPLATEEX
{
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	short x;
	short y;
	short cx;
	short cy;
	DWORD id;
};

#pragma pack(pop)


struct CXTPResourceManagerXML::DLGITEMTEMPLATEINFO : public DLGITEMTEMPLATEEX
{
	BSTR lpszCaption;
	CString strClassName;
	int nCaptionID;

public:
	DLGITEMTEMPLATEINFO()
	{
		lpszCaption = NULL;
		nCaptionID = 0;
	}
	~DLGITEMTEMPLATEINFO()
	{
		if (lpszCaption)
		{
			SysFreeString(lpszCaption);
		}
	}
	ULONG AlignDWord(ULONG uLong) const
	{
		return ((uLong + 3) & ~3);
	}

	UINT GetLength() const
	{
		return AlignDWord(sizeof(DLGITEMTEMPLATEEX)
			+ ((nCaptionID != 0 ? 1 : (int)wcslen(lpszCaption)) * sizeof(WORD)) +  sizeof(WORD)
			+ strClassName.GetLength() * sizeof(WORD) +  sizeof(WORD)
			+ 0
			+ sizeof(WORD)
		);
	}
};

struct CXTPResourceManagerXML::DLGTEMPLATEINFO : public DLGTEMPLATEEX
{
	BSTR lpszCaption;
	CString strFaceName;
	int nPointSize;

	CArray<DLGITEMTEMPLATEINFO*, DLGITEMTEMPLATEINFO*> aItems;

public:
	DLGTEMPLATEINFO()
	{
		lpszCaption = NULL;
	}
	~DLGTEMPLATEINFO()
	{
		if (lpszCaption)
		{
			SysFreeString(lpszCaption);
		}
		for (int i = 0; i < (int)aItems.GetSize(); i++)
		{
			delete aItems[i];
		}
	}

	ULONG AlignDWord(ULONG uLong) const
	{
		return ((uLong + 3) & ~3);
	}

	UINT GetHeaderLength() const
	{
		return AlignDWord(sizeof(DLGTEMPLATEEX)
			+ sizeof(WORD)
			+ sizeof(WORD)
			+ ((int)wcslen(lpszCaption) * sizeof(WORD) +  sizeof(WORD))
			+ (style & DS_SETFONT ? strFaceName.GetLength() * sizeof(WORD) + sizeof(WORD)
			+ sizeof(WORD) + sizeof(WORD) + sizeof(BYTE) + sizeof(BYTE)
			: 0)
		);
	}

	UINT GetLength() const
	{
		int nLength = GetHeaderLength();

		for (int i = 0; i < aItems.GetSize(); i++)
		{
			nLength += aItems[i]->GetLength();
		}

		return nLength;
	}
};

void CXTPResourceManagerXML::AddMenuItems(CXTPPropExchange* pPX, MENUTEMPLATEINFO* pItems)
{
	CXTPPropExchangeEnumeratorPtr enumerator(pPX->GetEnumerator(_T("menuitem")));
	POSITION pos = enumerator->GetPosition();

	while (pos)
	{
		CXTPPropExchangeSection sec(enumerator->GetNext(pos));
		int id = 0;

		MENUITEMTEMPLATEINFO* pItemInfo = new MENUITEMTEMPLATEINFO;
		PX_Int(&sec, _T("id"), id, 0);
		PX_Bstr(&sec, _T("caption"), pItemInfo->lpszCaption, L"");

		pItemInfo->mtID = (WORD)id;

		pItems->aItems.Add(pItemInfo);

		CXTPPropExchangeEnumeratorPtr enumeratorChilds(sec->GetEnumerator(_T("menuitem")));

		if (enumeratorChilds->GetPosition())
		{
			pItemInfo->mtOption |= MF_POPUP;
			AddMenuItems(&sec, pItems);
		}
		else if (id == 0)
		{
			pItemInfo->mtOption |= MF_SEPARATOR;
		}

		if (!pos)
		{
			pItemInfo->mtOption |= MF_END;
		}
	}
}

BOOL CXTPResourceManagerXML::CreateMenu(CMenu& menu, CXTPPropExchange* pPX)
{
	MENUTEMPLATEINFO menuTemplate;

	AddMenuItems(pPX, &menuTemplate);

	if (menuTemplate.aItems.GetSize() == 0)
		return FALSE;

	ASSERT(menuTemplate.aItems[menuTemplate.aItems.GetSize() - 1]->mtOption & MF_END);

	int nLength = menuTemplate.GetLength() + sizeof(MENUITEMTEMPLATE);
	LPVOID lpDlgTemplate = malloc(nLength);
	ZeroMemory(lpDlgTemplate, nLength);

	MENUITEMTEMPLATE* mitem = (MENUITEMTEMPLATE*) ((BYTE*)lpDlgTemplate + sizeof(MENUITEMTEMPLATEHEADER));

	for (int i = 0; i < menuTemplate.aItems.GetSize(); i++)
	{
		MENUITEMTEMPLATEINFO& itemInfo = *menuTemplate.aItems[i];
		mitem->mtOption = itemInfo.mtOption;

		if (itemInfo.mtOption & MF_POPUP)
		{
			MEMCPY_S((BYTE*)mitem + sizeof(mitem->mtOption), itemInfo.lpszCaption, (wcslen(itemInfo.lpszCaption) + 1) * sizeof(WORD));
		}
		else
		{
			mitem->mtID = itemInfo.mtID;
			MEMCPY_S(mitem->mtString, itemInfo.lpszCaption, (wcslen(itemInfo.lpszCaption) + 1) * sizeof(WORD));
		}

		mitem = (MENUITEMTEMPLATE*) ((BYTE*)mitem + itemInfo.GetLength());
	}
	mitem->mtOption = MF_END;

	BOOL bResult = menu.LoadMenuIndirect(lpDlgTemplate);

	free(lpDlgTemplate);

	return bResult;
}

BOOL CXTPResourceManagerXML::LoadMenu(CMenu* lpMenu, UINT nIDResource)
{
	if (!m_bValid || m_pResourceRoot == NULL)
		return CXTPResourceManager::LoadMenu(lpMenu, nIDResource);

	CString strPattern;
	strPattern.Format(_T("menu[@id = \"%i\"]"), nIDResource);

	if (!m_pResourceRoot->IsSectionExists(strPattern))
		return CXTPResourceManager::LoadMenu(lpMenu, nIDResource);

	CXTPPropExchangeSection secMenu(m_pResourceRoot->GetSection(strPattern));

	if (CreateMenu(*lpMenu, &secMenu))
		return TRUE;

	return CXTPResourceManager::LoadMenu(lpMenu, nIDResource);
}

//////////////////////////////////////////////////////////////////////////
// Utils


LPDLGTEMPLATE CXTPResourceManagerXML::CreateDialogTemplate(DLGTEMPLATEINFO& dlgTemplate)
{
	int nDlgLength = dlgTemplate.GetLength();

	DLGTEMPLATEEX* lpDlgTemplate = (DLGTEMPLATEEX*)malloc(nDlgLength);
	if (lpDlgTemplate == NULL)
		return NULL;

	ZeroMemory(lpDlgTemplate, nDlgLength);
	*lpDlgTemplate = dlgTemplate;

	BYTE* pMain = (BYTE*) lpDlgTemplate + sizeof(DLGTEMPLATEEX) + sizeof(WORD) * 2;

	// Caption
	ASSERT(dlgTemplate.lpszCaption != 0);
	if (!dlgTemplate.lpszCaption)
		return NULL;

	size_t nLength = (wcslen(dlgTemplate.lpszCaption) + 1) * sizeof(WORD);
	MEMCPY_S(pMain, dlgTemplate.lpszCaption, nLength);
	pMain += nLength;

	if (dlgTemplate.style & DS_SETFONT)
	{
		*((WORD *)pMain) = (WORD)dlgTemplate.nPointSize;
		pMain += sizeof(WORD);

		pMain += sizeof(WORD) + sizeof(BYTE) + sizeof(BYTE);

		MBSTOWCS_S((LPWSTR)pMain, dlgTemplate.strFaceName, dlgTemplate.strFaceName.GetLength() + 1);
	}

	pMain = (BYTE*) lpDlgTemplate + dlgTemplate.GetHeaderLength();
	ASSERT(dlgTemplate.cDlgItems == dlgTemplate.aItems.GetSize());


	CArray<DLGITEMTEMPLATEINFO*, DLGITEMTEMPLATEINFO*>& aItems = dlgTemplate.aItems;

	for (int i = 0; i < aItems.GetSize(); i++)
	{
		DLGITEMTEMPLATEINFO& itemInfo = *aItems[i];

		// Copy constant part of the template
		*(DLGITEMTEMPLATEEX*)pMain = itemInfo;
		BYTE* pControlMain = pMain + sizeof(DLGITEMTEMPLATEEX);

		// Class
		MBSTOWCS_S((LPWSTR)pControlMain, itemInfo.strClassName, itemInfo.strClassName.GetLength() + 1);
		pControlMain += (itemInfo.strClassName.GetLength() + 1) * sizeof(WORD);


		// Copy Caption
		if (itemInfo.nCaptionID != 0)
		{
			*((PWORD)pControlMain) = 0xFFFF;
			*((PWORD)pControlMain + 1) = (WORD)itemInfo.nCaptionID;

			pControlMain += sizeof(WORD) * 2;
		}
		else
		{
			ASSERT(itemInfo.lpszCaption != 0);
			if (itemInfo.lpszCaption)
			{
				nLength = (wcslen(itemInfo.lpszCaption) + 1) * sizeof(WORD);
				MEMCPY_S(pControlMain, itemInfo.lpszCaption, nLength);
				pControlMain += nLength;
			}
			else
			{
				pControlMain += sizeof(WORD);
			}
		}


		// Init Data length
		*((WORD *)pControlMain) = 0;
		pControlMain += sizeof(WORD);

		pMain += itemInfo.GetLength();
	}

	return (LPDLGTEMPLATE)lpDlgTemplate;
}


LPDLGTEMPLATE CXTPResourceManagerXML::CreateDialogTemplate(CXTPPropExchange* pPX)
{
	DLGTEMPLATEINFO dlgTemplate;

	CString strPos;
	CRect rc;
	DWORD dwStyle = 0, dwExtendedStyle = 0, id;

	PX_DWord(pPX, _T("style"), dwStyle, 0);
	PX_DWord(pPX, _T("styleex"), dwExtendedStyle, 0);
	PX_Rect(pPX, _T("position"), rc, CRect(0, 0, 0, 0));
	PX_String(pPX, _T("fontface"), dlgTemplate.strFaceName, _T("MS Sans Serif"));
	PX_Int(pPX, _T("fontsize"), dlgTemplate.nPointSize, 8);
	PX_Bstr(pPX, _T("caption"), dlgTemplate.lpszCaption, L"");
	PX_DWord(pPX, _T("id"), id, 0);

	dlgTemplate.x = (short)rc.left;
	dlgTemplate.y = (short)rc.top;
	dlgTemplate.cx = (short)rc.right;
	dlgTemplate.cy = (short)rc.bottom;
	dlgTemplate.style = dwStyle;
	dlgTemplate.exStyle = dwExtendedStyle;
	dlgTemplate.helpID = id;

	dlgTemplate.cDlgItems = 0;
	dlgTemplate.style |= DS_SETFONT;
	dlgTemplate.dlgVer = 1;
	dlgTemplate.signature = 0xFFFF;


	CArray<DLGITEMTEMPLATEINFO*, DLGITEMTEMPLATEINFO*>& aItems = dlgTemplate.aItems;

	CXTPPropExchangeEnumeratorPtr enumerator(pPX->GetEnumerator(_T("control")));
	POSITION pos = enumerator->GetPosition();

	while (pos)
	{
		CXTPPropExchangeSection sec(enumerator->GetNext(pos));

		DLGITEMTEMPLATEINFO* pItemInfo = new DLGITEMTEMPLATEINFO;

		PX_DWord(&sec, _T("style"), dwStyle, WS_CHILD | WS_VISIBLE | WS_GROUP);
		PX_DWord(&sec, _T("styleex"), dwExtendedStyle, 0);
		PX_Rect(&sec, _T("position"), rc, CRect(0, 0, 0, 0));
		PX_Bstr(&sec, _T("caption"), pItemInfo->lpszCaption, L"");
		PX_String(&sec, _T("class"), pItemInfo->strClassName, _T("STATIC"));
		PX_DWord(&sec, _T("id"), id, (DWORD)IDC_STATIC);
		PX_Int(&sec, _T("resource"), pItemInfo->nCaptionID, 0);

		pItemInfo->x = (short)rc.left;
		pItemInfo->y = (short)rc.top;
		pItemInfo->cx = (short)rc.right;
		pItemInfo->cy = (short)rc.bottom;
		pItemInfo->style = dwStyle;
		pItemInfo->exStyle = dwExtendedStyle;
		pItemInfo->helpID = 0;
		pItemInfo->id = id;

		aItems.Add(pItemInfo);
		dlgTemplate.cDlgItems++;
	}


	return CreateDialogTemplate(dlgTemplate);
}

LPCDLGTEMPLATE CXTPResourceManagerXML::LoadDialogTemplate(UINT nIDResource)
{
	if (!m_bValid || m_pResourceRoot == NULL)
		return CXTPResourceManager::LoadDialogTemplate(nIDResource);

	CString strPattern;
	strPattern.Format(_T("dialog[@id = \"%i\"]"), nIDResource);

	DLGTEMPLATE* pTemplate = 0;
	if (m_mapDialogs.Lookup(nIDResource, (DLGTEMPLATE*&)pTemplate))
		return pTemplate;

	if (!m_pResourceRoot->IsSectionExists(strPattern))
		return CXTPResourceManager::LoadDialogTemplate(nIDResource);

	CXTPPropExchangeSection secDialog(m_pResourceRoot->GetSection(strPattern));

	pTemplate = CreateDialogTemplate(&secDialog);

	if (pTemplate)
	{
		m_mapDialogs.SetAt(nIDResource, pTemplate);
		return pTemplate;
	}

	return CXTPResourceManager::LoadDialogTemplate(nIDResource);
}


#endif
