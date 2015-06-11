#include "StdAfx.h"
#include <Common/XTPPropExchange.h>
#include <Common/XTPVC80Helpers.h>


//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeIniFile

IMPLEMENT_DYNAMIC(CXTPPropExchangeIniFile, CXTPPropExchange)

CXTPPropExchangeIniFile::CXTPPropExchangeIniFile(BOOL bLoading, LPCTSTR lpszParentSection, LPCTSTR lpszSection)
{
	ASSERT(lpszSection != NULL);

	m_bLoading = bLoading;
	m_bTempFile = TRUE;
	m_bEmptySection = TRUE;

	if (lpszParentSection == NULL)
	{
		m_strSectionName = lpszSection;
	}
	else
	{
		m_strSectionName = CString(lpszParentSection) + _T("\\") + CString(lpszSection);
	}

}

CXTPPropExchangeIniFile::~CXTPPropExchangeIniFile()
{
	if (IsRootSection() && !m_strFileName.IsEmpty() && m_bTempFile)
	{
		::DeleteFile((LPTSTR)(LPCTSTR)m_strFileName);
	}

}


void CXTPPropExchangeIniFile::EmptySection()
{
	if (m_bEmptySection || m_strSectionName.IsEmpty())
		return;

	CXTPPropExchange::EmptySection();

	size_t nLength = (size_t)m_strSectionName.GetLength();
	LPTSTR lpszBuffer = 0;
	int nSize = 8000;

	do
	{
		lpszBuffer = new TCHAR[nSize];
		int nResult = GetPrivateProfileSectionNames(lpszBuffer, nSize, m_strFileName);

		if (nResult == 0)
		{
			delete[] lpszBuffer;
			return;
		}

		if (nResult == nSize - 2)
		{
			nSize = nSize * 2;
			delete[] lpszBuffer;
			lpszBuffer = 0;
		}
	}
	while (lpszBuffer == 0);

	LPTSTR lpszSection = lpszBuffer;
	size_t nSectionLen = _tcslen(lpszSection);

	while (nSectionLen > 0)
	{
		if (nLength <= nSectionLen &&
			(memcmp((LPCTSTR)m_strSectionName, lpszSection, nLength * sizeof(TCHAR)) == 0) &&
			(nLength == nSectionLen || lpszSection[nLength] == _T('\\')))
		{
			WritePrivateProfileString(lpszSection, NULL, NULL, m_strFileName);
		}

		lpszSection = lpszSection + nSectionLen + 1;
		nSectionLen = _tcslen(lpszSection);
	}


	delete[] lpszBuffer;
}

CXTPPropExchange* CXTPPropExchangeIniFile::GetSection(LPCTSTR lpszSection)
{
	OnBeforeExchange();

	CXTPPropExchangeIniFile* pSection = new CXTPPropExchangeIniFile(IsLoading(), m_strSectionName, lpszSection);
	pSection->InitSection(this);
	pSection->m_strFileName = m_strFileName;
	pSection->m_bTempFile = m_bTempFile;
	return pSection;
}

BOOL CXTPPropExchangeIniFile::OnBeforeExchange()
{
	if (!m_bInitialized)
	{
		if (m_strFileName.IsEmpty())
		{
			TCHAR lpszTempPath[MAX_PATH];
			TCHAR lpszTempFileName[MAX_PATH];

			if (GetTempPath(MAX_PATH, lpszTempPath) == 0)
				return FALSE;

			if (GetTempFileName(lpszTempPath, _T("xtp"), 0, lpszTempFileName) == 0)
				return FALSE;

			m_strFileName = lpszTempFileName;
			m_bTempFile = TRUE;
		}

		m_bInitialized = TRUE;
	}
	return !m_strFileName.IsEmpty();
}


BOOL CXTPPropExchangeIniFile::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault /*= NULL*/)
{
	if (m_bLoading)
	{
		if (!m_bUseDefaultOnLoad)
			pvDefault = NULL;

		if (!OnBeforeExchange())
			return CopyPropValue(vtProp, pvProp, pvDefault);

		CString strValue;

		DWORD dwSize = GetPrivateProfileString(m_strSectionName, pszPropName, NULL, strValue.GetBuffer(256), 256, m_strFileName);

		if (dwSize == 0)
			return CopyPropValue(vtProp, pvProp, pvDefault);

		if (dwSize == 256 - 1)
		{
			dwSize = GetPrivateProfileString(m_strSectionName, pszPropName, NULL, strValue.GetBuffer(16000), 16000, m_strFileName);
			ASSERT(dwSize < 16000 - 1);
		}

		strValue.ReleaseBuffer(dwSize);

		return ExchangePropStringHelper(strValue, vtProp, pvProp, pvDefault);
	}
	else
	{
		if (!m_bUseDefaultOnSave)
			pvDefault = NULL;

		if (!OnBeforeExchange())
			return FALSE;

		if (IsSamePropValue(vtProp, pvProp, pvDefault))
		{
			if (!m_bEmptySection)
				WritePrivateProfileString (m_strSectionName, pszPropName, NULL, m_strFileName);

			return TRUE;
		}

		CString strValue;
		if (!ExchangePropStringHelper(strValue, vtProp, pvProp, pvDefault))
			return FALSE;

		if (!WritePrivateProfileString (m_strSectionName, pszPropName, strValue, m_strFileName))
			return FALSE;
	}

	return TRUE;
}


void CXTPPropExchangeIniFile::SaveToFile(LPCTSTR strFileName)
{
	ASSERT(IsRootSection() && IsStoring());

	if (m_strFileName.Compare(strFileName) != 0 && !m_strFileName.IsEmpty())
	{
		::DeleteFile(strFileName);
		::CopyFile(m_strFileName, strFileName, FALSE);
	}
}

BOOL CXTPPropExchangeIniFile::LoadFromFile(LPCTSTR strFileName)
{
	ASSERT(IsRootSection() && IsLoading());

	if (!FILEEXISTS_S(strFileName))
		return FALSE;

	if (!OnBeforeExchange())
		return FALSE;

	return ::CopyFile(strFileName, m_strFileName, FALSE);
}

void CXTPPropExchangeIniFile::AssignFile(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
	m_bTempFile = FALSE;
	m_bEmptySection = FALSE;
}
