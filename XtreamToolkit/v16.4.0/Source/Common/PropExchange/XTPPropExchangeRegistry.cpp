#include "StdAfx.h"
#include <Common/XTPPropExchange.h>
#include <Common/XTPVC80Helpers.h>


//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeRegistry

IMPLEMENT_DYNAMIC(CXTPPropExchangeRegistry, CXTPPropExchange)

CXTPPropExchangeRegistry::CXTPPropExchangeRegistry(BOOL bLoading, HKEY hParentKey, LPCTSTR lpszSection)
{
	ASSERT(lpszSection != NULL);
	if (hParentKey == 0)
	{
		m_hParentKey = AfxGetApp()->GetAppRegistryKey();
		m_bCloseParentKey = TRUE;
	}
	else
	{
		m_hParentKey = hParentKey;
		m_bCloseParentKey = FALSE;
	}

	m_bLoading = bLoading;
	m_strSectionName = lpszSection;
	m_hSectionKey = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPPropExchangeRegistry::~CXTPPropExchangeRegistry()
{
	if (m_hSectionKey)
	{
		RegCloseKey(m_hSectionKey);
	}
	if (m_bCloseParentKey && m_hParentKey)
	{
		RegCloseKey(m_hParentKey);
	}
}

CXTPPropExchange* CXTPPropExchangeRegistry::GetSection(LPCTSTR lpszSection)
{
	OnBeforeExchange();

	CXTPPropExchangeRegistry* pSection = new CXTPPropExchangeRegistry(IsLoading(), m_hSectionKey, lpszSection);
	pSection->InitSection(this);
	return pSection;
}

BOOL CXTPPropExchangeRegistry::IsPropertyExists(LPCTSTR pszPropName)
{
	if (!OnBeforeExchange())
		return FALSE;

	return RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, NULL, NULL, NULL) == ERROR_SUCCESS;
}

BOOL CXTPPropExchangeRegistry::IsSectionExists(LPCTSTR lpszSection)
{
	if (!OnBeforeExchange())
		return FALSE;

	HKEY hSectionKey = 0;

	if (RegOpenKeyEx(m_hSectionKey, lpszSection, 0, m_bLoading ? KEY_READ : KEY_WRITE, &hSectionKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hSectionKey);
		return TRUE;
	}

	return FALSE;
}


BOOL CXTPPropExchangeRegistry::OnBeforeExchange()
{
	if (m_hParentKey == NULL)
		return FALSE;

	if (!m_bInitialized)
	{
		ASSERT(m_hSectionKey == 0);

		VERIFY(RegCreateKeyEx(m_hParentKey, m_strSectionName, 0, REG_NONE, REG_OPTION_NON_VOLATILE, m_bLoading ? KEY_READ : KEY_WRITE, NULL, &m_hSectionKey, NULL) == ERROR_SUCCESS);

		m_bInitialized = TRUE;
	}
	return m_hSectionKey != NULL;
}

void CXTPPropExchangeRegistry::EmptySection()
{
	CXTPPropExchange::EmptySection();

	if (m_hSectionKey)
	{
		RegCloseKey(m_hSectionKey);
		m_hSectionKey = 0;
	}

	AfxDelRegTreeHelper(m_hParentKey, m_strSectionName);
	m_bInitialized = FALSE;
}


BOOL CXTPPropExchangeRegistry::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault /*= NULL*/)
{
	if (m_bLoading)
	{
		if (!m_bUseDefaultOnLoad)
			pvDefault = NULL;

		if (!OnBeforeExchange())
			return CopyPropValue(vtProp, pvProp, pvDefault);

		ASSERT(m_hSectionKey);

		switch (vtProp)
		{
		case VT_R4:
		case VT_R8:
		case VT_DATE:
		case VT_BSTR:
			ASSERT(FALSE); // Not Implemented
			return FALSE;

		case VT_VARIANT:
			return FALSE; // Not Implemented

		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_BOOL:
			{

				DWORD dwValue;
				DWORD dwType;
				DWORD dwCount = sizeof(DWORD);
				LONG lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
					(LPBYTE)&dwValue, &dwCount);

				if (lResult == ERROR_SUCCESS)
				{
					ASSERT(dwType == REG_DWORD);
					ASSERT(dwCount == sizeof(DWORD));

					return CopyPropValue(vtProp, pvProp, &dwValue);

				}
				else
				{
					return CopyPropValue(vtProp, pvProp, pvDefault);
				}
			}
		case VT_LPSTR:
		case VT_EX_SIZE:
		case VT_EX_RECT:
			{
				CString strValue;
				DWORD dwType, dwCount;
				LONG lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
					NULL, &dwCount);
				if (lResult == ERROR_SUCCESS)
				{
					ASSERT(dwType == REG_SZ);
					lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
						(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
					strValue.ReleaseBuffer();
				}
				if (lResult == ERROR_SUCCESS)
				{
					ASSERT(dwType == REG_SZ);

					if (vtProp == VT_EX_SIZE)
					{
						SIZE sz = {0, 0};
						if (SCANF_S(strValue, _T("%i, %i"), &sz.cx, &sz.cy) != 2)
							return CopyPropValue(vtProp, pvProp, pvDefault);

						*((LPSIZE)pvProp) = sz;
					}
					else if (vtProp == VT_EX_RECT)
					{
						RECT rc = {0, 0, 0, 0};
						if (SCANF_S(strValue, _T("%i, %i, %i, %i"), &rc.left, &rc.top, &rc.right, &rc.bottom) != 4)
							return CopyPropValue(vtProp, pvProp, pvDefault);

						*((LPRECT)pvProp) = rc;
					}
					else
					{
						*(CString*)pvProp = strValue;
					}

				}
				else
				{
					return CopyPropValue(vtProp, pvProp, pvDefault);
				}
				return TRUE;
			}
		}
	}
	else
	{
		if (!m_bUseDefaultOnSave)
			pvDefault = NULL;

		if (!OnBeforeExchange())
			return FALSE;

		if (IsSamePropValue(vtProp, pvProp, pvDefault))
		{
			if (m_hSectionKey && !m_bEmptySection)
				RegDeleteValue(m_hSectionKey, pszPropName);
			return TRUE;
		}

		LONG lResult = -1;

		switch (vtProp)
		{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_BOOL:
			{
				DWORD dwValue = 0;
				CopyPropValue(vtProp, &dwValue, pvProp);
				lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
			}
			break;

		case VT_LPSTR:
			{
				CString& strValue = *(CString*)pvProp;
				lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_SZ,
					(LPBYTE)(LPCTSTR)strValue, (strValue.GetLength() + 1) * sizeof(TCHAR));
			}
			break;

		case VT_R4:
		case VT_R8:
			lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_BINARY, (LPBYTE)pvProp, GetSizeOfVarType(vtProp));
			break;

		case VT_DATE:
		case VT_VARIANT:
		case VT_BSTR:
			ASSERT(FALSE); // Not Implemented
			break;

		case VT_EX_SIZE:
			{
				CString strValue;
				strValue.Format(_T("%i, %i"), ((LPSIZE)pvProp)->cx, ((LPSIZE)pvProp)->cy);
				lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_SZ,
					(LPBYTE)(LPCTSTR)strValue, (strValue.GetLength() + 1) * sizeof(TCHAR));
			}
			break;

		case VT_EX_RECT:
				CString strValue;
				strValue.Format(_T("%i, %i, %i, %i"), ((LPRECT)pvProp)->left, ((LPRECT)pvProp)->top, ((LPRECT)pvProp)->right, ((LPRECT)pvProp)->bottom);
				lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_SZ,
					(LPBYTE)(LPCTSTR)strValue, (strValue.GetLength() + 1) * sizeof(TCHAR));
			break;

		}

		return lResult == ERROR_SUCCESS;
	}

	return FALSE;
}

BOOL CXTPPropExchangeRegistry::ExchangeBlobProp(LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes)
{
	if (!OnBeforeExchange())
		return FALSE;

	ASSERT(m_hSectionKey != 0);

	if (IsStoring())
	{
		LONG lResult = ::RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_BINARY,
			pData, nBytes);
		return lResult == ERROR_SUCCESS;

	}
	else
	{
		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
			NULL, &dwCount);

		if (lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);


		if (pData == NULL)
		{
			pData = (BYTE*)malloc(dwCount);
			nBytes = dwCount;
		}

		if (nBytes < dwCount)
			return FALSE;

		lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
				(LPBYTE)pData, &dwCount);

		if (lResult != ERROR_SUCCESS)
			return FALSE;
	}

	return TRUE;
}
