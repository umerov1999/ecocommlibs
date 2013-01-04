// XTRegistryManager.cpp : implementation file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include "Common/XTPVC80Helpers.h"
#include "XTPRegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHKey - helper automatically closes open HKEY

class CXTPRegistryManager::CHKey
{
public:
	CHKey(HKEY hKey = NULL) : m_hKey(hKey)
	{
	}

	~CHKey()
	{
		if (m_hKey != NULL)
		{
			::RegCloseKey(m_hKey);
		}
	}

	HKEY& operator=(HKEY hKey)
	{
		if (m_hKey != NULL)
		{
			::RegCloseKey(m_hKey);
		}

		m_hKey = hKey;
		return m_hKey;
	}

	BOOL operator==(HKEY hKey)
	{
		return m_hKey == hKey;
	}

	operator HKEY() const
	{
		return m_hKey;
	}

	operator PHKEY()
	{
		return &m_hKey;
	}

private:
	HKEY m_hKey;
};

/////////////////////////////////////////////////////////////////////////////
// CXTPRegistryManager

CString CXTPRegistryManager::m_strINIFileName = _T("");

CXTPRegistryManager::CXTPRegistryManager(HKEY hKeyBase/*= HKEY_CURRENT_USER*/)
{
	ASSERT(hKeyBase == HKEY_CURRENT_USER || hKeyBase == HKEY_LOCAL_MACHINE);
	m_hKeyBase = hKeyBase;

	m_lResult = ERROR_SUCCESS;
}

CXTPRegistryManager::~CXTPRegistryManager()
{

}

void CXTPRegistryManager::SetRegistryKey(LPCTSTR lpszRegistryKey, LPCTSTR lpszProfileName)
{
	m_strRegistryKey = lpszRegistryKey;
	m_strProfileName = lpszProfileName;
}

BOOL CXTPRegistryManager::GetProfileInfo()
{
	if (m_strRegistryKey.IsEmpty() || m_strProfileName.IsEmpty())
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			m_strRegistryKey = pWinApp->m_pszRegistryKey;
			m_strProfileName = pWinApp->m_pszProfileName;
		}

		// If this fails, you need to call SetRegistryKey(...); in your
		// applications CWinApp::InitInstance() override, or you have called
		// this method before you call SetRegistryKey..  Calling SetRegistryKey
		// will initialize m_pszProfileName and m_pszRegistryKey for CWinApp.

		// NOTE: If you want to write to an INI file instead of the system
		// registry, call CXTPRegistryManager::SetINIFileName(...) prior to
		// making this call.

		if (m_strRegistryKey.IsEmpty() || m_strProfileName.IsEmpty())
		{
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

HKEY CXTPRegistryManager::GetAppRegistryKey(REGSAM samDesired)
{
	if (!GetProfileInfo())
		return 0;

	HKEY hAppKey = NULL;
	CHKey hSoftKey;
	CHKey hCompanyKey;

	m_lResult = ::RegOpenKeyEx(m_hKeyBase, _T("Software"), 0,
		samDesired, hSoftKey);

	if (m_lResult != ERROR_SUCCESS)
		return NULL;

	DWORD dw = 0;
	m_lResult = ::RegCreateKeyEx(hSoftKey, m_strRegistryKey, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, samDesired, NULL, hCompanyKey, &dw);

	if (m_lResult != ERROR_SUCCESS)
		return NULL;

	m_lResult = ::RegCreateKeyEx(hCompanyKey, m_strProfileName, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, samDesired, NULL, &hAppKey, &dw);

	if (m_lResult != ERROR_SUCCESS)
		return NULL;

	return hAppKey;
}

HKEY CXTPRegistryManager::GetFullPathKey(LPCTSTR lpszSection) const
{
	if (_tcsncmp(lpszSection, _T("HKCU"), 4) == 0)
		return HKEY_CURRENT_USER;

	if (_tcsncmp(lpszSection, _T("HKLM"), 4) == 0)
		return HKEY_LOCAL_MACHINE;

	return 0;
}

HKEY CXTPRegistryManager::RecurseOpenKey(LPCTSTR lpszSection, REGSAM samDesired)
{
	HKEY hKeyBase = GetFullPathKey(lpszSection);
	if (hKeyBase == 0)
		return 0;

	CString strRegistryKey = lpszSection + 5;

	CHKey hParentKey = NULL;
	DWORD dw = 0;

	int nIndex = strRegistryKey.Find('\\');
	while (nIndex != -1)
	{
		CString strKey = strRegistryKey.Left(nIndex);
		strRegistryKey = strRegistryKey.Mid(nIndex + 1);
		HKEY hKey = 0;

		m_lResult = ::RegCreateKeyEx(hParentKey == NULL ? hKeyBase : hParentKey, strKey, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, samDesired, NULL, &hKey, &dw);

		if (m_lResult != ERROR_SUCCESS)
			return NULL;

		hParentKey = hKey;
		nIndex = strRegistryKey.Find('\\');
	}


	HKEY hAppKey = NULL;
	m_lResult = ::RegCreateKeyEx(hParentKey, strRegistryKey, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, samDesired, NULL, &hAppKey, &dw);

	if (m_lResult != ERROR_SUCCESS)
		return NULL;

	return hAppKey;
}


HKEY CXTPRegistryManager::GetSectionKey(LPCTSTR lpszSection, REGSAM samDesired)
{
	if (GetFullPathKey(lpszSection))
	{
		return RecurseOpenKey(lpszSection, samDesired);
	}

	if (!GetProfileInfo())
		return 0;

	ASSERT(lpszSection != NULL);
	if (!lpszSection)
		return 0;

	HKEY hSectionKey = NULL;
	CHKey hAppKey(GetAppRegistryKey(samDesired));

	if (hAppKey == NULL)
		return NULL;

	DWORD dw = 0;
	m_lResult = ::RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, samDesired, NULL, &hSectionKey, &dw);

	if (m_lResult != ERROR_SUCCESS)
		return NULL;

	return hSectionKey;
}

BOOL CXTPRegistryManager::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	if (!lpszSection)
		return 0;

	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection));
		if (hSecKey == NULL)
			return FALSE;

		m_lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL,
			REG_DWORD, (LPBYTE)&nValue, sizeof(nValue));

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		TCHAR szT[16];
		wsprintf(szT, _T("%d"), nValue);
		return ::WritePrivateProfileString(lpszSection, lpszEntry, szT,
			m_strINIFileName);
	}
}

BOOL CXTPRegistryManager::WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection));
		if (hSecKey == NULL)
			return FALSE;

		m_lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			pData, nBytes);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	// convert to string and write out
	LPTSTR lpsz = new TCHAR[nBytes * 2 + 1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i * 2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i * 2 + 1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i * 2] = 0;

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

BOOL CXTPRegistryManager::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
	ASSERT(lpszSection != NULL);
	if (!lpszSection)
		return 0;

	if (m_strINIFileName.IsEmpty())
	{
		if (lpszEntry == NULL) //delete whole section
		{
			CHKey hAppKey(GetAppRegistryKey(FALSE));
			if (hAppKey == NULL)
				return FALSE;

			m_lResult = ::RegDeleteKey(hAppKey, lpszSection);

			if (m_lResult != ERROR_SUCCESS)
				return FALSE;
		}
		else if (lpszValue == NULL)
		{
			CHKey hSecKey(GetSectionKey(lpszSection));
			if (hSecKey == NULL)
				return FALSE;

			// necessary to cast away const below
			m_lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszEntry);

			if (m_lResult != ERROR_SUCCESS)
				return FALSE;
		}
		else
		{
			CHKey hSecKey(GetSectionKey(lpszSection));
			if (hSecKey == NULL)
				return FALSE;

			m_lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,
				(LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));

			if (m_lResult != ERROR_SUCCESS)
				return FALSE;
		}

		return TRUE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);
		ASSERT(m_strINIFileName.GetLength() < 4095); // can't read in bigger
		return ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue,
			m_strINIFileName);
	}
}

UINT CXTPRegistryManager::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	if (!lpszSection || !lpszEntry)
		return 0;

	if (m_strINIFileName.IsEmpty()) // use registry
	{
		CHKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
		if (hSecKey == NULL)
			return nDefault;

		DWORD dwValue;
		DWORD dwType;
		DWORD dwCount = sizeof(DWORD);

		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)&dwValue, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return nDefault;

		ASSERT(dwType == REG_DWORD);
		ASSERT(dwCount == sizeof(dwValue));

		return (UINT)dwValue;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);
		return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault,
			m_strINIFileName);
	}
}

BOOL CXTPRegistryManager::GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, BYTE** ppData, UINT* pBytes)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	ASSERT(ppData != NULL);
	ASSERT(pBytes != NULL);
	if (!ppData || !pBytes)
		return FALSE;

	*ppData = NULL;
	*pBytes = 0;

	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		*pBytes = dwCount;
		*ppData = new BYTE[*pBytes];
		ASSERT(dwType == REG_BINARY);

		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			*ppData, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
		{
			SAFE_DELETE_AR(*ppData);
			return FALSE;
		}

		ASSERT(dwType == REG_BINARY);
		return TRUE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;
		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();
		*pBytes = nLen/2;
		*ppData = new BYTE[*pBytes];
		int i;
		for (i = 0; i < nLen; i += 2)
		{
			(*ppData)[i/2] = (BYTE)
				(((str[i + 1] - 'A') << 4) + (str[i] - 'A'));
		}
		return TRUE;
	}
}

CString CXTPRegistryManager::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
		if (hSecKey == NULL)
			return lpszDefault;

		CString strValue;
		DWORD dwType, dwCount;

		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return lpszDefault;

		ASSERT(dwType == REG_SZ);
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
		strValue.ReleaseBuffer();

		if (m_lResult != ERROR_SUCCESS)
			return lpszDefault;

		ASSERT(dwType == REG_SZ);
		return strValue;
	}
	else
	{
		TCHAR chNil = '\0';

		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		if (lpszDefault == NULL)
		{
			lpszDefault = &chNil;    // don't pass in NULL
		}

		TCHAR szT[4096];
		DWORD dw = ::GetPrivateProfileString(lpszSection, lpszEntry,
			lpszDefault, szT, _countof(szT), m_strINIFileName);
		ASSERT(dw < 4095);
		return szT;
	}
}

int CXTPRegistryManager::EnumValues(LPCTSTR lpszSection, CMap<CString, LPCTSTR, DWORD, DWORD&>* mapItems,
									CStringArray * arrayNames)
{
	ASSERT(lpszSection != NULL);

	CHKey hKey(GetSectionKey(lpszSection, KEY_READ));
	if (hKey == NULL)
		return 0;

	int index = 0;

	TCHAR szValue[512];
	DWORD dwLen = 512;
	DWORD dwType;

	while (::RegEnumValue(hKey, index++, szValue, &dwLen,
		NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
	{
		if (mapItems) mapItems->SetAt(szValue, dwType);
		if (arrayNames) arrayNames->Add(szValue);
		dwLen = 512;
	}

	return --index;
}

int CXTPRegistryManager::EnumKeys(LPCTSTR lpszSection, CStringArray & arrayKeys)
{
	ASSERT(lpszSection != NULL);

	CHKey hKey(GetSectionKey(lpszSection, KEY_READ));
	if (hKey == NULL)
		return 0;

	int index = 0;

	TCHAR szValue[512];
	DWORD dwLen = 512;

	while (::RegEnumKeyEx(hKey, index++, szValue, &dwLen,
		NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		arrayKeys.Add(szValue);
		dwLen = 512;
	}

	return --index;
}

LONG CXTPRegistryManager::RecurseDeleteKey(HKEY hKey, LPCTSTR lpszKey)
{
	CHKey hSubKey;
	LONG lRes = ::RegOpenKeyEx(hKey, lpszKey, 0, KEY_READ | KEY_WRITE, hSubKey);
	if (lRes != ERROR_SUCCESS)
		return lRes;

	FILETIME time;
	DWORD dwSize = 256;
	TCHAR szBuffer[256];

	while (::RegEnumKeyEx(hSubKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time) == ERROR_SUCCESS)
	{
		lRes = RecurseDeleteKey(hSubKey, szBuffer);
		if (lRes != ERROR_SUCCESS)
			return lRes;

		dwSize = 256;
	}

	m_lResult = ::RegDeleteKey(hKey, lpszKey);
	return m_lResult;
}

bool CXTPRegistryManager::DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey)
{
	CHKey hSectionKey(GetSectionKey(lpszSection));
	if (hSectionKey == NULL)
		return false;

	return (RecurseDeleteKey(hSectionKey, lpszKey) == ERROR_SUCCESS);
}

bool CXTPRegistryManager::DeleteValue(LPCTSTR lpszSection, LPCTSTR lpszValue)
{
	CHKey hSecKey(GetSectionKey(lpszSection, KEY_ALL_ACCESS));
	if (hSecKey == NULL)
		return false;

	m_lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszValue);
	return (m_lResult == ERROR_SUCCESS);
}

BOOL CXTPRegistryManager::WriteProfilePoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, CPoint* pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection));
		if (hSecKey == NULL)
			return FALSE;

		m_lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)pValue, sizeof(CPoint));

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	CString str;
	str.Format(_T("%i,%i"), pValue->x, pValue->y);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, str);

	return bResult;
}

BOOL CXTPRegistryManager::GetProfilePoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, CPoint* ptResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)ptResult, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);
		return TRUE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		SCANF_S(str, _T("%ld,%ld"), &ptResult->x, &ptResult->y);
		return TRUE;
	}
}

BOOL CXTPRegistryManager::WriteProfileRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect* pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection));
		if (hSecKey == NULL)
			return FALSE;

		m_lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)pValue, sizeof(CRect));

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	CString str;
	str.Format(_T("%i,%i,%i,%i"), pValue->left, pValue->top, pValue->right, pValue->bottom);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, str);

	return bResult;
}

BOOL CXTPRegistryManager::GetProfileRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect* rcResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)rcResult, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}
	else
	{

		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		// convert to string and write out
		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		SCANF_S(str, _T("%ld,%ld,%ld,%ld"), &rcResult->left, &rcResult->top, &rcResult->right, &rcResult->bottom);
		return TRUE;
	}
}

BOOL CXTPRegistryManager::WriteProfileSize(LPCTSTR lpszSection, LPCTSTR lpszEntry, CSize* pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection));
		if (hSecKey == NULL)
			return FALSE;

		m_lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)pValue, sizeof(CSize));

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	CString str;
	str.Format(_T("%i,%i"), pValue->cx, pValue->cy);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, str);
	return bResult;
}

BOOL CXTPRegistryManager::GetProfileSize(LPCTSTR lpszSection, LPCTSTR lpszEntry, CSize* szResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)szResult, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);
		return TRUE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		// convert to string and write out
		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		SCANF_S(str, _T("%i,%i"), &szResult->cx, &szResult->cy);
		return TRUE;
	}
}

BOOL CXTPRegistryManager::WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double* pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection));
		if (hSecKey == NULL)
			return FALSE;

		m_lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)pValue, sizeof(*pValue));

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	LPBYTE pData = (LPBYTE) pValue;
	UINT nBytes = sizeof(double);
	LPTSTR lpsz = new TCHAR[nBytes * 2 + 1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i * 2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i * 2 + 1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i * 2] = 0;

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;

}

BOOL CXTPRegistryManager::GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double* dResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{

		CHKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)dResult, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);
		return TRUE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();

		LPBYTE pData = (LPBYTE) dResult;

		int i;
		for (i = 0; i < nLen; i += 2)
		{
			(pData)[i/2] = (BYTE)
				(((str[i + 1] - 'A') << 4) + (str[i] - 'A'));
		}

		return TRUE;
	}
}

BOOL CXTPRegistryManager::WriteProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD* pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection));
		if (hSecKey == NULL)
			return FALSE;

		m_lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_DWORD,
			(LPBYTE)pValue, sizeof(*pValue));

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}


	BOOL bResult = WriteProfileInt(lpszSection, lpszEntry, int(*pValue));
	return bResult;
}

BOOL CXTPRegistryManager::GetProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD* dwResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		CHKey hSecKey(GetSectionKey(lpszSection, KEY_READ));
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_DWORD);
		m_lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)dwResult, &dwCount);

		if (m_lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_DWORD);
		return TRUE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		*dwResult = (DWORD)GetProfileInt(lpszSection, lpszEntry, 0);
		return TRUE;
	}
}

BOOL CXTPRegistryManager::WriteProfileColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, COLORREF* pValue)
{
	return WriteProfileDword(lpszSection, lpszEntry, pValue);
}

BOOL CXTPRegistryManager::GetProfileColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, COLORREF* rgbResult)
{
	return GetProfileDword(lpszSection, lpszEntry, rgbResult);
}

CString CXTPRegistryManager::GetErrorMessage() const
{
	LPVOID lpMsgBuf = 0;

	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetErrorCode(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
		);

	CString csMessage((LPCTSTR)lpMsgBuf);

	// Free the buffer.
	::LocalFree( lpMsgBuf );

	return csMessage;
}
