// XTPPropExchange.cpp: implementation of the CXTPPropExchange class.
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
#include "XTPPropExchange.h"
#include "XTPVC80Helpers.h"

#include "XTPResourceManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable : 4310)

#ifdef _UNICODE
#define CT2BSTR(x) (BSTR)(LPCWSTR)x
#else
#define CT2BSTR(x) (BSTR)XTP_CT2CW(x)
#endif


LCID CXTPPropExchange::m_lcidDateTime = XTP_LOCALE_ISO8601;

/////////////////////////////////////////////////////////////////////////////
BOOL ParseDateTimeISO8601(COleDateTime& rDT, LPCTSTR pcszDateTime)
{
	SYSTEMTIME st;
	ZeroMemory(&st, sizeof(SYSTEMTIME));

	int nResult = SCANF_S(pcszDateTime, _T("%hu-%hu-%huT%hu:%hu:%hu"),
		&st.wYear, &st.wDay, &st.wMonth, &st.wHour, &st.wMinute, &st.wSecond);

	if (nResult == 3 || nResult == 5 || nResult == 6)
	{
		COleDateTime dtTemp(st);
		if (dtTemp.GetStatus() != COleDateTime::valid)
			return FALSE;

		rDT = dtTemp;
		return TRUE;
	}

	ZeroMemory(&st, sizeof(SYSTEMTIME));

	nResult = SCANF_S(pcszDateTime, _T("%hu:%hu:%hu"),
		&st.wHour, &st.wMinute, &st.wSecond);

	if (nResult == 2 || nResult == 3)
	{
		double dblTime = (((long)st.wHour * 3600L) +  // hrs in seconds
			((long)st.wMinute * 60L) +  // mins in seconds
			((long)st.wSecond)) / 86400.;

		rDT = dblTime;
		return TRUE;
	}

	return FALSE;
}

CString FormatDateTimeISO8601(const COleDateTime& dt)
{
	ASSERT(dt.m_status == COleDateTime::valid);

	CString strValue;

	if ((DWORD)dt == 0)
	{
		strValue = dt.Format(_T("%H:%M:%S"));
	}
	else if ((double)(DWORD)(dt) == (double)dt)
	{
		strValue = dt.Format(_T("%Y-%d-%m"));
	}
	else
	{
		strValue = dt.Format(_T("%Y-%d-%mT%H:%M:%S"));
	}

	return strValue;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CXTPPropExchange, CCmdTarget)

CXTPPropExchange::CXTPPropExchange()
{
	m_bLoading = FALSE;
	m_dwData = 0;
	m_pOwner = NULL;
	m_nSchema = _XTP_SCHEMA_CURRENT;
	m_bChildSection = FALSE;
	m_bInitialized = FALSE;
	m_bEmptySection = FALSE;
	m_bUseDefaultOnLoad = TRUE;
	m_bUseDefaultOnSave = TRUE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPPropExchange::~CXTPPropExchange()
{

}

void CXTPPropExchange::InitSection(CXTPPropExchange* pRootSection)
{
	ASSERT(m_bLoading == pRootSection->m_bLoading);

	m_bChildSection = TRUE;
	m_nSchema = pRootSection->m_nSchema;
	m_dwData = pRootSection->m_dwData;
	m_bEmptySection = pRootSection->m_bEmptySection;
	m_bUseDefaultOnLoad = pRootSection->m_bUseDefaultOnLoad;
	m_bUseDefaultOnSave = pRootSection->m_bUseDefaultOnSave;
}



void CXTPPropExchange::EmptySection()
{
	m_bEmptySection = TRUE;
}


DWORD CXTPPropExchange::GetSizeOfVarType(VARTYPE vt)
{
	switch (vt)
	{
	case VT_BOOL: return sizeof(BOOL);
	case VT_UI1: return sizeof(BYTE);
	case VT_I2: return 2;
	case VT_I4: return 4;
	case VT_R4:
		ASSERT(sizeof(float) == 4);
		return 4;
	case VT_R8:
		ASSERT(sizeof(double) == 8);
		return 8;
	case VT_EX_RECT: return sizeof(RECT);
	case VT_EX_SIZE: return sizeof(SIZE);
	case VT_CY: return sizeof(CURRENCY);
	case VT_BSTR:return sizeof(BSTR);
	case VT_VARIANT: return sizeof(VARIANT);
	case VT_DATE: return sizeof(COleDateTime);
	}

	return 0;
}

void CXTPPropExchange::Write (LPCTSTR pszPropName, const void* lpBuf, UINT nCount)
{
	ASSERT(IsStoring());
	ASSERT(lpBuf != NULL);
	ExchangeBlobProp(pszPropName, (BYTE*&)lpBuf, (DWORD&)nCount);
}

UINT CXTPPropExchange::Read (LPCTSTR pszPropName, void* lpBuf, UINT nCount)
{
	ASSERT(IsLoading());
	ASSERT(lpBuf != NULL);
	if (!ExchangeBlobProp(pszPropName, (BYTE*&)lpBuf, (DWORD&)nCount))
		return 0;

	return nCount;
}

AFX_INLINE int REPLACEW_S(BSTR& strReplace, LPCWSTR lpszOld, LPCWSTR lpszNew)
{
	int nOldLength = (strReplace == NULL) ? 0 : (int)wcslen(strReplace);
	int nSourceLen = (lpszOld == NULL) ? 0 : (int)wcslen(lpszOld);

	if (nSourceLen == 0 || nOldLength == 0)
		return 0;

	int nReplacementLen = (lpszNew == NULL) ? 0 : (int)wcslen(lpszNew);

	// loop once to figure out the size of the result string
	int nCount = 0;
	LPWSTR lpszStart = strReplace;
	LPWSTR lpszTarget;

	while ((lpszTarget = wcsstr(lpszStart, lpszOld)) != NULL)
	{
		nCount++;
		lpszStart = lpszTarget + nSourceLen;
	}


	// if any changes were made, make them
	if (nCount > 0)
	{
		int nNewLength = nOldLength + (nReplacementLen-nSourceLen)*nCount;
		ASSERT(nNewLength <= nOldLength); // not implemented

		lpszStart = strReplace;

		// loop again to actually do the work
		while ((lpszTarget = wcsstr(lpszStart, lpszOld)) != NULL)
		{
			int nBalance = nOldLength - int(lpszTarget - strReplace + nSourceLen);
			MEMMOVE_S(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
				nBalance * sizeof(WORD));
			MEMCPY_S(lpszTarget, lpszNew, nReplacementLen*sizeof(WORD));
			lpszStart = lpszTarget + nReplacementLen;
			lpszStart[nBalance] = '\0';
			nOldLength += (nReplacementLen - nSourceLen);
		}

		ASSERT(strReplace[nNewLength] == '\0');
	}

	return nCount;
}

void CXTPPropExchange::PreformatString(BSTR bstrValue)
{
	ASSERT(bstrValue && m_bLoading);

	REPLACEW_S(bstrValue, L"\\\\", L"\001\001");
	REPLACEW_S(bstrValue, L"\\n", L"\n");
	REPLACEW_S(bstrValue, L"\\r", L"\r");
	REPLACEW_S(bstrValue, L"\\t", L"\t");
	REPLACEW_S(bstrValue, L"\001\001", L"\\");
}

void CXTPPropExchange::PreformatString(CString& strValue, BOOL bLoading)
{
	if (bLoading)
	{
		if (strValue.Find(_T('\\')) == -1)
			return;

		strValue.Replace(_T("\\\\"), _T("\001\001"));
		strValue.Replace(_T("\\n"), _T("\n"));
		strValue.Replace(_T("\\r"), _T("\r"));
		strValue.Replace(_T("\\t"), _T("\t"));
		strValue.Replace(_T("\001\001"), _T("\\"));
	}
	else
	{
		strValue.Replace(_T("\\"), _T("\\\\"));
		strValue.Replace(_T("\n"), _T("\\n"));
		strValue.Replace(_T("\r"), _T("\\r"));
		strValue.Replace(_T("\t"), _T("\\t"));
	}
}

void CXTPPropExchange::PreformatString(CString& strValue)
{
	PreformatString(strValue, m_bLoading);
}

BOOL CXTPPropExchange::ExchangePropDateHelper(CString& strValue, COleDateTime& dt)
{
	if (m_lcidDateTime == XTP_LOCALE_ISO8601)
	{
		if (m_bLoading)
		{
			return ParseDateTimeISO8601(dt, strValue);
		}
		else
		{
			strValue = FormatDateTimeISO8601(dt);
		}

		return TRUE;

	}

	if (m_bLoading)
	{
		if (!dt.ParseDateTime(strValue, 0, m_lcidDateTime))
			return FALSE;

	}
	else
	{
		TRY
		{
			strValue = dt.Format(0, m_lcidDateTime);
		}
		CATCH_ALL(e)
		{
			VERIFY(strValue.LoadString(AFX_IDS_INVALID_DATETIME));
		}
		END_CATCH_ALL

	}

	return TRUE;
}


BOOL CXTPPropExchange::ExchangePropStringHelper(CString& strValue, VARTYPE vtProp,
	void* pvProp, const void* pvDefault)
{
	if (m_bLoading)
	{
		switch (vtProp)
		{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_BOOL:
			{
				int dwValue = _ttoi(strValue);
				return CopyPropValue(vtProp, pvProp, &dwValue);
			}
		case VT_R4:
			{
				TCHAR* pChar = NULL;
				float fValue = (float)_tcstod(strValue, &pChar);
				return CopyPropValue(vtProp, pvProp, &fValue);
			}
		case VT_R8:
			{
				TCHAR* pChar = NULL;
				double fValue = _tcstod(strValue, &pChar);
				return CopyPropValue(vtProp, pvProp, &fValue);
			}
		case VT_DATE:
			{
				COleDateTime dt;

				if (!ExchangePropDateHelper(strValue, dt))
					return FALSE;

				*(COleDateTime*)pvProp = dt;
				return TRUE;
			}

		case VT_BSTR:
			ASSERT(FALSE);
			return FALSE;

		case VT_LPSTR:
		case VT_EX_SIZE:
		case VT_EX_RECT:
			{
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
					PreformatString(strValue);
					*(CString*)pvProp = strValue;
				}
				return TRUE;
			}
		case VT_VARIANT:
			{
				COleVariant varValue(strValue);
				if (m_lcidDateTime == XTP_LOCALE_ISO8601)
				{
					COleDateTime dt;
					if (ParseDateTimeISO8601(dt, strValue))
					{
						varValue = dt;
					}
				}
				return CopyPropValue(vtProp, pvProp, &varValue);
			}
		}

	}
	else
	{
		switch (vtProp)
		{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_BOOL:
			{
				int nValue = 0;
				memcpy(&nValue, pvProp, GetSizeOfVarType(vtProp));

				strValue.Format(_T("%i"), nValue);

			}
			return TRUE;

		case VT_BSTR:
			ASSERT(FALSE);
			return FALSE;

		case VT_LPSTR:
			{
				strValue = *(CString*)pvProp;
				PreformatString(strValue);
			}
			return TRUE;

		case VT_DATE:
			{
				COleDateTime& dt = *(COleDateTime*)pvProp;
				ExchangePropDateHelper(strValue, dt);
			}
			return TRUE;

		case VT_R4:
			{
				double d = (double) *(float*)pvProp;
				strValue.Format(_T("%g"), d);
				return TRUE;
			}

		case VT_R8:

			strValue.Format(_T("%g"), *(double*)pvProp);
			return TRUE;

		case VT_EX_SIZE:
			{
				strValue.Format(_T("%i, %i"), ((LPSIZE)pvProp)->cx, ((LPSIZE)pvProp)->cy);
			}
			return TRUE;

		case VT_EX_RECT:
			{
				strValue.Format(_T("%i, %i, %i, %i"), ((LPRECT)pvProp)->left, ((LPRECT)pvProp)->top, ((LPRECT)pvProp)->right, ((LPRECT)pvProp)->bottom);
			}
			return TRUE;

		case VT_VARIANT:

			LCID lcidID = m_lcidDateTime;
			COleVariant& varProp = *((COleVariant*)pvProp);

			if (lcidID == XTP_LOCALE_ISO8601 && varProp.vt == VT_DATE)
			{
				strValue = FormatDateTimeISO8601(varProp.date);
				return TRUE;
			}
			if (lcidID == XTP_LOCALE_ISO8601)
			{
				lcidID = LOCALE_NEUTRAL;
			}
			COleVariant var;
			HRESULT hr = VariantChangeTypeEx(&var, &varProp, lcidID, 0, VT_BSTR);

			if (FAILED(hr) || var.vt != VT_BSTR)
				return FALSE;

			strValue = var.bstrVal;

			return TRUE;
		}
	}
	return FALSE;
}

void CXTPPropExchange::BinToString(BYTE* pData, DWORD nBytes, CString& str)
{
	if (nBytes > 0)
	{
		LPTSTR lpsz = str.GetBuffer(nBytes * 2);

		for (UINT i = 0; i < nBytes; i++)
		{
			*lpsz++ = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
			*lpsz++ = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
		}

		str.ReleaseBuffer(nBytes * 2);
	}
	else
	{
		str.Empty();
	}
}

BOOL CXTPPropExchange::StringToBin(const CString& str, BYTE*& pData, DWORD& nBytes)
{
	if (str.GetLength() > 0)
	{
		int nLen = str.GetLength();
		if ((nLen == 0) || (nLen % 2 != 0))
			return FALSE;

		DWORD nCount = nLen/2;
		if (pData == NULL)
		{
			pData = (BYTE*)malloc(nCount);
			nBytes = nCount;
		}
		if (pData == NULL)
			return FALSE;

		if (nBytes < nCount)
			return FALSE;

		for (int i = 0; i < nLen; i += 2)
		{
			(pData)[i/2] = (BYTE)
				(((str[i + 1] - 'A') << 4) + (str[i] - 'A'));
		}
	}
	else
	{
		nBytes = 0;
	}

	return TRUE;
}


BOOL CXTPPropExchange::ExchangeBlobProp(LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes)
{
	if (IsStoring())
	{
		CString str;
		BinToString(pData, nBytes, str);

		PX_String(this, pszPropName, str, _T(""));
	}
	else
	{
		CString str;
		if (!PX_String(this, pszPropName, str))
			return FALSE;

		return StringToBin(str, pData, nBytes);
	}
	return TRUE;

}


BOOL CXTPPropExchange::IsSamePropValue(VARTYPE vtProp, const void* pv1, const void* pv2)
{
	if (pv1 == pv2)
		return TRUE;

	if ((pv1 == NULL) || (pv2 == NULL))
		return FALSE;

	BOOL bSame = FALSE;

	switch (vtProp)
	{
	case VT_BSTR:
		bSame = wcscmp(*(BSTR*)pv1, (BSTR)pv2) == 0;
		break;

	case VT_LPSTR:
		bSame = ((CString*)pv1)->Compare((LPCTSTR)pv2) == 0;
		break;

	case VT_BOOL:
	case VT_UI1:
	case VT_I2:
	case VT_I4:
	case VT_CY:
	case VT_R4:
	case VT_R8:
	case VT_EX_RECT:
	case VT_EX_SIZE:
		bSame = memcmp(pv1, pv2, GetSizeOfVarType(vtProp)) == 0;
		break;

	case VT_VARIANT:
		bSame = *((COleVariant*)pv1) == *((COleVariant*)pv2);
		break;

	case VT_DATE:
		bSame = *((COleDateTime*)pv1) == *((COleDateTime*)pv2);
		break;
	}

	return bSame;
}

BOOL CXTPPropExchange::CopyPropValue(VARTYPE vtProp, void* pvDest, const void* pvSrc)
{
	ASSERT(AfxIsValidAddress(pvDest, 1));

	if (pvSrc != NULL)
	{
		ASSERT(AfxIsValidAddress(pvSrc, 1, FALSE));

		switch (vtProp)
		{
		case VT_UI1:
			*(BYTE*)pvDest = *(BYTE*)pvSrc;
			break;

		case VT_I2:
			*(short*)pvDest = *(short*)pvSrc;
			break;

		case VT_I4:
			*(long*)pvDest = *(long*)pvSrc;
			break;

		case VT_BOOL:
			*(BOOL*)pvDest = *(BOOL*)pvSrc;
			break;

		case VT_BSTR:
			*(BSTR*)pvDest = SysAllocString((BSTR)pvSrc);
			break;

		case VT_LPSTR:
			*(CString*)pvDest = (LPCTSTR)pvSrc;
			break;

		case VT_CY:
			*(CY*)pvDest = *(CY*)pvSrc;
			break;

		case VT_R4:
			*(float*)pvDest = *(float*)pvSrc;
			break;

		case VT_R8:
			*(double*)pvDest = *(double*)pvSrc;
			break;

		case VT_EX_RECT:
			*(RECT*)pvDest = *(RECT*)pvSrc;
			break;

		case VT_EX_SIZE:
			*(SIZE*)pvDest = *(SIZE*)pvSrc;
			break;

		case VT_DATE:
			*(COleDateTime*)pvDest = *(COleDateTime*)pvSrc;
			break;

		case VT_VARIANT:
			TRY
			{
				*(COleVariant*)pvDest = *(COleVariant*)pvSrc;
			}
			CATCH_ALL(e)
			{
				return FALSE;
			}
			END_CATCH_ALL

			return TRUE;

		default:
			return FALSE;
		}
	}
	return pvSrc != NULL;
}

CRuntimeClass* CXTPPropExchange::FindRuntimeClass(LPCSTR szClassName)
{

	CRuntimeClass* pClass;
	// search app specific classes
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();

	for (pClass = pModuleState->m_classList; pClass != NULL;
	pClass = pClass->m_pNextClass)
	{
		if (lstrcmpA(szClassName, pClass->m_lpszClassName) == 0)
		{
			return pClass;
		}
	}

#ifdef _AFXDLL
	// search classes in shared DLLs
	for (CDynLinkLibrary* pDLL = pModuleState->m_libraryList; pDLL != NULL;
	pDLL = pDLL->m_pNextDLL)
	{
		for (pClass = pDLL->m_classList; pClass != NULL;
		pClass = pClass->m_pNextClass)
		{
			if (lstrcmpA(szClassName, pClass->m_lpszClassName) == 0)
			{
				return pClass;
			}
		}
	}
#endif
	ASSERT(FALSE);
	return 0;
}

BOOL CXTPPropExchange::ExchangeRuntimeClass(LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass)
{
	CString strClassName;

	if (!IsLoading())
	{
		strClassName = CString(pClass->m_lpszClassName);
		PX_String(this, pszPropName, strClassName, CString(pDefaultClass->m_lpszClassName));
	}
	else
	{
		if (!PX_String(this, pszPropName, strClassName))
		{
			pClass = pDefaultClass;
			return TRUE;
		}

		pClass = FindRuntimeClass(XTP_CT2CA(strClassName));
		if (!pClass)
			return FALSE;
	}
	return TRUE;
}

BOOL CXTPPropExchange::ExchangeObjectInstance(CObject*& pObject, CRuntimeClass* pObjectRuntime)
{
	if (!IsLoading())
	{
		CRuntimeClass* pClass = pObject->GetRuntimeClass();
		PX_RuntimeClass(this, _T("Class"), pClass, pObjectRuntime);
	}
	else
	{
		CRuntimeClass* pClass = NULL;
		PX_RuntimeClass(this, _T("Class"), pClass, pObjectRuntime);
		if (!pClass)
			return FALSE;

		pObject = (CObject*)AfxDynamicDownCast(pObjectRuntime, pClass->CreateObject());
		if (!pObject)
			return FALSE;
	}
	return TRUE;
}
void CXTPPropExchange::WriteCount (DWORD dwCount)
{
	PX_DWord(this, _T("Count"), dwCount, 0);
}

DWORD CXTPPropExchange::ReadCount()
{
	DWORD nCount = 0;
	PX_DWord(this, _T("Count"), nCount, 0);
	return nCount;
}

CArchive* CXTPPropExchange::GetArchive(LPCTSTR pszPropName)
{
	if (IsStoring())
	{

		CMemFile* pMemFile = new CMemFile();
		CArchive* pArchive = new CArchive(pMemFile, CArchive::store);

		return pArchive;
	}

	DWORD nBytes = 0;
	BYTE* pData = NULL;
	PX_Blob(this, pszPropName, pData, nBytes);

	if (nBytes == 0)
		return NULL;

	CMemFile* pMemFile = new CMemFile(pData, nBytes);
	CArchive* pArchive = new CArchive(pMemFile, CArchive::load);

	return pArchive;
}

void CXTPPropExchange::ReleaseArchive(LPCTSTR pszPropName, CArchive* pArchive)
{
	ASSERT(pArchive);
	if (!pArchive)
		return;

	CMemFile* pFile = DYNAMIC_DOWNCAST(CMemFile, pArchive->GetFile());
	ASSERT(pFile);
	if (!pFile)
		return;

	BYTE* pData = NULL;
	if (IsStoring())
	{
		pArchive->Flush();

		DWORD nBytes = (DWORD)pFile->GetPosition();
		pData = pFile->Detach();
		PX_Blob(this, pszPropName, pData, nBytes);

	}
	else
	{
		pData = pFile->Detach();
	}

	pArchive->Abort();
	pFile->Close();
	free(pData);

	delete pFile;
	delete pArchive;
}

void CXTPPropExchange::ExchangeSchema()
{
	PX_Long(this, _T("Schema"), (long&)m_nSchema, _XTP_SCHEMA_CURRENT);
}

void CXTPPropExchange::ExchangeSchemaSafe()
{
	PX_Long(this, _T("Schema"), (long&)m_nSchema, _XTP_SCHEMA_1122);
}

void CXTPPropExchange::ExchangeLocale()
{
	PX_DWord(this, _T("Locale"), (DWORD&)CXTPPropExchange::m_lcidDateTime, XTP_LOCALE_ISO8601);
}



CXTPPropExchangeEnumerator* CXTPPropExchange::GetEnumerator(LPCTSTR lpszSection)
{
	return new CXTPPropExchangeEnumerator(this, lpszSection);
}




/////////////////////////////////////////////////////////////////////////////
// PX_ functions

BOOL AFX_CDECL PX_Byte(CXTPPropExchange* pPX, LPCTSTR pszPropName, BYTE& nValue, BYTE nDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&nValue, short);
	if (!pPX)
		return FALSE;

	return  pPX->ExchangeProp(pszPropName, VT_UI1, &nValue, &nDefault);
}

BOOL AFX_CDECL PX_Short(CXTPPropExchange* pPX, LPCTSTR pszPropName, short& sValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&sValue, short);
	if (!pPX)
		return FALSE;

	return  pPX->ExchangeProp(pszPropName, VT_I2, &sValue);
}

BOOL AFX_CDECL PX_Short(CXTPPropExchange* pPX, LPCTSTR pszPropName, short& sValue,
	short sDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&sValue, short);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I2, &sValue, &sDefault);
}

BOOL AFX_CDECL PX_UShort(CXTPPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&usValue, USHORT);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I2, &usValue);
}

BOOL AFX_CDECL PX_UShort(CXTPPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue,
	USHORT usDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&usValue, USHORT);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I2, &usValue, &usDefault);
}

BOOL AFX_CDECL PX_Long(CXTPPropExchange* pPX, LPCTSTR pszPropName, long& lValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&lValue, long);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I4, &lValue);
}

BOOL AFX_CDECL PX_Long(CXTPPropExchange* pPX, LPCTSTR pszPropName, long& lValue,
	long lDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&lValue, long);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I4, &lValue, &lDefault);
}

BOOL AFX_CDECL PX_DWord(CXTPPropExchange* pPX, LPCTSTR pszPropName, DWORD& dwValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&dwValue, DWORD);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I4, &dwValue);
}

BOOL AFX_CDECL PX_DWord(CXTPPropExchange* pPX, LPCTSTR pszPropName, DWORD& dwValue,
	DWORD dwDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&dwValue, DWORD);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I4, &dwValue, &dwDefault);
}

BOOL AFX_CDECL PX_UI8(CXTPPropExchange* pPX, LPCTSTR pszPropName,
	ULONGLONG& ullValue, ULONGLONG ullDefault)
{
	POINT ptData64 = {(LONG)(DWORD)(ullValue >> 32), (LONG)(DWORD)ullValue};
	POINT ptDefault = {(LONG)(DWORD)(ullDefault >> 32), (LONG)(DWORD)ullDefault};

	BOOL bResult = PX_Point(pPX, pszPropName, ptData64, ptDefault);

	if (bResult && pPX->IsLoading())
	{
		ullValue = (DWORD)ptData64.x;
		ullValue = ullValue << 32;
		ullValue += (DWORD)ptData64.y;
	}

	return bResult;
}

BOOL AFX_CDECL PX_Int(CXTPPropExchange* pPX, LPCTSTR pszPropName, int& nValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&nValue, int);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I4, &nValue);
}

BOOL AFX_CDECL PX_Int(CXTPPropExchange* pPX, LPCTSTR pszPropName, int& nValue,
	int nDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&nValue, int);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I4, &nValue, &nDefault);
}

BOOL AFX_CDECL PX_ULong(CXTPPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&ulValue, ULONG);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I4, &ulValue);
}

BOOL AFX_CDECL PX_ULong(CXTPPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue,
	ULONG ulDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&ulValue, ULONG);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_I4, &ulValue, &ulDefault);
}

BOOL AFX_CDECL PX_Bool(CXTPPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&bValue, BOOL);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_BOOL, &bValue);
}

BOOL AFX_CDECL PX_Bool(CXTPPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue,
	BOOL bDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&bValue, BOOL);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_BOOL, &bValue, &bDefault);
}

BOOL AFX_CDECL PX_String(CXTPPropExchange* pPX, LPCTSTR pszPropName, CString& strValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&strValue, CString);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_LPSTR, &strValue);
}

BOOL AFX_CDECL PX_String(CXTPPropExchange* pPX, LPCTSTR pszPropName, CString& strValue,
	LPCTSTR lpszDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&strValue, CString);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_LPSTR, &strValue, lpszDefault);
}

BOOL AFX_CDECL PX_Bstr(CXTPPropExchange* pPX, LPCTSTR pszPropName, BSTR& lpszValue,
	BSTR lpszDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&lpszValue, BSTR);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_BSTR, &lpszValue, lpszDefault);
}

BOOL AFX_CDECL PX_Float(CXTPPropExchange* pPX, LPCTSTR pszPropName, float& floatValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&floatValue, float);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_R4, &floatValue);
}

BOOL AFX_CDECL PX_Float(CXTPPropExchange* pPX, LPCTSTR pszPropName, float& floatValue,
	float floatDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&floatValue, float);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_R4, &floatValue, &floatDefault);
}

BOOL AFX_CDECL PX_Double(CXTPPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&doubleValue, double);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_R8, &doubleValue);
}

BOOL AFX_CDECL PX_DateTime(CXTPPropExchange* pPX, LPCTSTR pszPropName, COleDateTime& dtValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&dtValue, COleDateTime);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_DATE, &dtValue);
}

BOOL AFX_CDECL PX_Double(CXTPPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue,
	double doubleDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT_POINTER(&doubleValue, double);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_R8, &doubleValue, &doubleDefault);
}

BOOL AFX_CDECL PX_Size(CXTPPropExchange* pPX, LPCTSTR pszPropName, SIZE& szValue, SIZE szDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_EX_SIZE, &szValue, &szDefault);
}

BOOL AFX_CDECL PX_Point(CXTPPropExchange* pPX, LPCTSTR pszPropName, POINT& ptValue, POINT ptDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(sizeof(POINT) == sizeof(SIZE));
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_EX_SIZE, &ptValue, &ptDefault);
}

BOOL AFX_CDECL PX_Rect(CXTPPropExchange* pPX, LPCTSTR pszPropName, RECT& rcValue, RECT rcDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_EX_RECT, &rcValue, &rcDefault);
}

BOOL AFX_CDECL PX_Variant(CXTPPropExchange* pPX, LPCTSTR pszPropName, COleVariant& variant, COleVariant variantDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	if (!pPX)
		return FALSE;

	return pPX->ExchangeProp(pszPropName, VT_VARIANT, &variant, &variantDefault);
}


BOOL AFX_CDECL PX_RuntimeClass(CXTPPropExchange* pPX, LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass)
{
	if (!pPX)
		return FALSE;

	return pPX->ExchangeRuntimeClass(pszPropName, pClass, pDefaultClass);
}

BOOL AFX_CDECL PX_Blob(CXTPPropExchange* pPX, LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes)
{
	if (!pPX)
		return FALSE;

	return pPX->ExchangeBlobProp(pszPropName, pData, nBytes);
}


BOOL AFX_CDECL PX_Serialize(CXTPPropExchange* pPX, LPCTSTR pszPropName, CObject* pObject)
{
	if (!pPX)
		return FALSE;

	CArchive* pArchive = pPX->GetArchive(pszPropName);

	if (!pArchive)
		return FALSE;

	BOOL bResult = FALSE;
	try
	{
		pObject->Serialize(*pArchive);
		bResult = TRUE;
	}
	catch (COleException* pEx)
	{
		pEx->Delete ();

	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
	}

	pPX->ReleaseArchive(pszPropName, pArchive);

	return bResult;
}


#ifdef _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////////
// CPropExchangeCtrl command target
enum PropExchangeType
{
	propExchangeUndefined,
	propExchangeXML,
	propExchangeRegistry,
	propExchangeArchive,
};

class CPropExchangeCtrl : public CCmdTarget
{
	DECLARE_DYNCREATE(CPropExchangeCtrl)

	CPropExchangeCtrl();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropExchangeCtrl)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPropExchangeCtrl();

	// Generated message map functions
	//{{AFX_MSG(CPropExchangeCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
#ifdef _XTP_PROPEXCHANGE_OLECREATE
	DECLARE_OLECREATE_EX(CPropExchangeCtrl)
#endif

	BOOL OleCreateAsXML(BOOL bLoading, LPCTSTR lpszSection);
	BOOL OleCreateAsRegistry(BOOL bLoading, LPCTSTR lpszSection);
	BOOL OleCreateAsBinaryFile(BOOL bLoading, LPCTSTR lpszPath);

	void OleSaveToFile(LPCTSTR lpszFilePath);
	BOOL OleLoadFromFile(LPCTSTR lpszFilePath);
	void OleSetValue(const VARIANT* varValue);
	const VARIANT OleGetValue();
	LPDISPATCH OleGetSection(LPCTSTR lpszSection);
	void OleExchangeString(LPCTSTR lpszString, BSTR* bstrValue, LPCTSTR lpszDefaultValue);
	void OleExchangeInt(LPCTSTR lpszPropName, long* pbValue, long nDefaultValue);
	LPDISPATCH OleGetSafeHandle();

	BSTR OleGetEncoding();
	void OleSetEncoding(LPCTSTR lpszEncoding);
	BOOL OleGetLoading();
	BOOL OleGetValid();
	void OleEmptySection();

	void CleanUp();

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPropExchangeCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CPropExchangeCtrl)

	CXTPPropExchange* m_pPropExchange;
	PropExchangeType m_propExchangeType;
	HKEY m_hParentKey;
	CFile m_file;
	CArchive* m_pArchive;
};



/////////////////////////////////////////////////////////////////////////////
// CPropExchangeCtrl

IMPLEMENT_DYNCREATE(CPropExchangeCtrl, CCmdTarget)

CPropExchangeCtrl::CPropExchangeCtrl()
{
	EnableAutomation();
	EnableTypeLib();

	// To keep the application running as long as an OLE automation
	//  object is active, the constructor calls AfxOleLockApp.
	m_pPropExchange = NULL;
	m_propExchangeType = propExchangeUndefined;
	m_hParentKey = NULL;
	m_pArchive = NULL;

	AfxOleLockApp();
}

void CPropExchangeCtrl::CleanUp()
{
	CMDTARGET_RELEASE(m_pPropExchange);

	if (m_hParentKey)
	{
		RegCloseKey(m_hParentKey);
		m_hParentKey = NULL;
	}
	m_propExchangeType = propExchangeUndefined;

	if (m_pArchive)
	{
		m_pArchive->Close();
		delete m_pArchive;
		m_pArchive = NULL;
	}

#if _MSC_VER < 1300
	if ((HFILE)m_file != (HFILE)CFile::hFileNull)
#else
	if ((HANDLE)m_file != (HANDLE)CFile::hFileNull)
#endif
	{
		m_file.Close();
	}
}

CPropExchangeCtrl::~CPropExchangeCtrl()
{
	CleanUp();

	// To terminate the application when all objects created with
	//  with OLE automation, the destructor calls AfxOleUnlockApp.
	AfxOleUnlockApp();
}


void CPropExchangeCtrl::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_DISPATCH_MAP(CPropExchangeCtrl, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CPropExchangeCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CPropExchangeCtrl, "CreateAsXML", 1, OleCreateAsXML, VT_BOOL, VTS_BOOL VTS_BSTR)
	DISP_FUNCTION_ID(CPropExchangeCtrl, "CreateAsRegistry", 2, OleCreateAsRegistry, VT_BOOL, VTS_BOOL VTS_BSTR)
	DISP_FUNCTION_ID(CPropExchangeCtrl, "CreateAsBinaryFile", 3, OleCreateAsBinaryFile, VT_BOOL, VTS_BOOL VTS_BSTR)
	DISP_FUNCTION_ID(CPropExchangeCtrl, "SaveToFile", 5, OleSaveToFile, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CPropExchangeCtrl, "LoadFromFile", 4, OleLoadFromFile, VT_BOOL, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CPropExchangeCtrl, "Value", DISPID_VALUE, OleGetValue, OleSetValue, VT_VARIANT)
	DISP_FUNCTION_ID(CPropExchangeCtrl, "GetSection", 7, OleGetSection, VT_DISPATCH, VTS_BSTR)

	DISP_FUNCTION_ID(CPropExchangeCtrl, "ExchangeString", 8, OleExchangeString, VT_EMPTY, VTS_BSTR VTS_PBSTR VTS_BSTR)
	DISP_FUNCTION_ID(CPropExchangeCtrl, "ExchangeInt", 9, OleExchangeInt, VT_EMPTY, VTS_BSTR VTS_PI4 VTS_I4)
	DISP_PROPERTY_EX_ID(CPropExchangeCtrl, "GetSafeHandle", 10, OleGetSafeHandle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CPropExchangeCtrl, "Encoding", 11, OleGetEncoding, OleSetEncoding, VT_BSTR)
	DISP_PROPERTY_EX_ID(CPropExchangeCtrl, "Loading", 12, OleGetLoading, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CPropExchangeCtrl, "Valid", 13, OleGetValid, SetNotSupported, VT_BOOL)

	DISP_FUNCTION_ID(CPropExchangeCtrl, "EmptySection", 14, OleEmptySection, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Note: we add support for IID_ICPropExchangeCtrl to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {A56F6C0F-BCC7-48A0-B69E-6DD726B9B309}
static const IID IID_IPropExchange =
{ 0xa56f6c0f, 0xbcc7, 0x48a0, { 0xb6, 0x9e, 0x6d, 0xd7, 0x26, 0xb9, 0xb3, 0x9 } };

BEGIN_INTERFACE_MAP(CPropExchangeCtrl, CCmdTarget)
	INTERFACE_PART(CPropExchangeCtrl, IID_IPropExchange, Dispatch)
END_INTERFACE_MAP()

#ifdef _XTP_PROPEXCHANGE_OLECREATE

// {13790E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CPropExchangeCtrl, "Codejock.PropExchange." _XTP_AXLIB_VERSION,
0x13790e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

#endif


IMPLEMENT_OLETYPELIB_EX(CPropExchangeCtrl, IID_IPropExchange)

/////////////////////////////////////////////////////////////////////////////
// CPropExchangeCtrl message handlers

BOOL CPropExchangeCtrl::OleCreateAsXML(BOOL bLoading, LPCTSTR lpszSection)
{
	CleanUp();

	m_pPropExchange = new CXTPPropExchangeXMLNode(bLoading, NULL, lpszSection);
	m_propExchangeType = propExchangeXML;

	return TRUE;
}

HKEY ParseRegistryRoot(CString strKey, CString& strSection)
{

	HKEY hKeyRoot = NULL;

	if (strKey.Left(17) == _T("HKEY_CURRENT_USER"))
	{
		strKey.Delete(0, 18);
		hKeyRoot = HKEY_CURRENT_USER;
	}

	if (strKey.Left(18) == _T("HKEY_LOCAL_MACHINE"))
	{
		strKey.Delete(0, 19);
		hKeyRoot = HKEY_LOCAL_MACHINE;
	}

	if (hKeyRoot == NULL)
		return NULL;

	int nIndex = strKey.Find(_T('\\'));
	HKEY hKey = NULL;

	while (nIndex > 0)
	{
		CString strSection = strKey.Left(nIndex);

		if (hKey == NULL)
		{
			if ((RegOpenKeyEx(hKeyRoot, strSection, 0, KEY_WRITE | KEY_READ,
				&hKey) != ERROR_SUCCESS) || (hKey == NULL))
			{
				return NULL;
			}
		}
		else
		{
			HKEY hKeySection = NULL;
			DWORD dw;
			if ((RegCreateKeyEx(hKey, strSection, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL,
				&hKeySection, &dw) != ERROR_SUCCESS) || (hKeySection == NULL))
			{
				RegCloseKey(hKey);
				return NULL;
			}

			RegCloseKey(hKey);
			hKey = hKeySection;
		}


		strKey.Delete(0, nIndex + 1);
		nIndex = strKey.Find(_T('\\'));
	}

	strSection = strKey;
	return hKey;
}

BOOL CPropExchangeCtrl::OleCreateAsRegistry(BOOL bLoading, LPCTSTR lpszSection)
{
	CleanUp();

	CString strSection;

	m_hParentKey = ParseRegistryRoot(lpszSection, strSection);
	if (!m_hParentKey)
		return FALSE;

	m_pPropExchange = new CXTPPropExchangeRegistry(bLoading, m_hParentKey, strSection);
	m_propExchangeType = propExchangeRegistry;

	return TRUE;
}

BOOL CPropExchangeCtrl::OleCreateAsBinaryFile(BOOL bLoading, LPCTSTR lpszPath)
{
	CleanUp();

	if (!m_file.Open(lpszPath, bLoading ? CFile::modeRead : CFile::modeWrite | CFile::modeCreate))
		return FALSE;

	m_pArchive = new CArchive(&m_file, bLoading ? CArchive::load : CArchive::store);

	m_pPropExchange = new CXTPPropExchangeArchive(*m_pArchive);
	m_propExchangeType = propExchangeArchive;

	return TRUE;
}


void CPropExchangeCtrl::OleSaveToFile(LPCTSTR lpszFilePath)
{
	if (m_propExchangeType == propExchangeXML && m_pPropExchange)
	{
		((CXTPPropExchangeXMLNode*)m_pPropExchange)->SaveToFile(lpszFilePath);
	}
}

BOOL CPropExchangeCtrl::OleLoadFromFile(LPCTSTR lpszFilePath)
{
	if (m_propExchangeType == propExchangeXML && m_pPropExchange)
	{
		return ((CXTPPropExchangeXMLNode*)m_pPropExchange)->LoadFromFile(lpszFilePath);
	}
	return FALSE;
}

const VARIANT CPropExchangeCtrl::OleGetValue()
{
	VARIANT var;
	var.vt = VT_EMPTY;

	if (m_propExchangeType == propExchangeXML && m_pPropExchange)
	{
		var.vt = VT_BSTR;
		CString str = ((CXTPPropExchangeXMLNode*)m_pPropExchange)->GetXML();
		var.bstrVal = str.AllocSysString();
	}

	return var;
}

void CPropExchangeCtrl::OleSetValue(const VARIANT* varValue)
{
	if (varValue->vt == (VT_VARIANT | VT_BYREF))
	{
		OleSetValue(varValue->pvarVal);
		return;
	}

	if (m_propExchangeType == propExchangeXML && m_pPropExchange && varValue->vt == VT_BSTR)
	{
		((CXTPPropExchangeXMLNode*)m_pPropExchange)->LoadFromString(varValue->bstrVal);
	}
	if (m_propExchangeType == propExchangeXML && m_pPropExchange && varValue->vt == (VT_BSTR | VT_BYREF))
	{
		((CXTPPropExchangeXMLNode*)m_pPropExchange)->LoadFromString(*varValue->pbstrVal);
	}
}

BSTR CPropExchangeCtrl::OleGetEncoding()
{
	if (m_propExchangeType == propExchangeXML && m_pPropExchange)
	{
		return ((CXTPPropExchangeXMLNode*)m_pPropExchange)->m_strEncoding.AllocSysString();
	}

	return NULL;
}

void CPropExchangeCtrl::OleSetEncoding(LPCTSTR lpszEncoding)
{
	if (m_propExchangeType == propExchangeXML && m_pPropExchange)
	{
		((CXTPPropExchangeXMLNode*)m_pPropExchange)->SetEncoding(lpszEncoding);
	}
}


LPDISPATCH CPropExchangeCtrl::OleGetSection(LPCTSTR lpszSection)
{
	CPropExchangeCtrl* pCtrl = new CPropExchangeCtrl();

	if (m_pPropExchange)
	{
		CXTPPropExchange* pPropExchange = m_pPropExchange->GetSection(lpszSection);

		pCtrl->m_pPropExchange = pPropExchange;
		pCtrl->m_propExchangeType = m_propExchangeType;
	}

	return pCtrl->GetIDispatch(FALSE);
}

void CPropExchangeCtrl::OleExchangeString(LPCTSTR lpszPropName, BSTR* bstrValue, LPCTSTR lpszDefaultValue)
{
	if (!bstrValue)
		AfxThrowOleException(E_INVALIDARG);

	if (m_pPropExchange)
	{
		if (m_pPropExchange->IsStoring())
		{
			CString strValue(*bstrValue);
			PX_String(m_pPropExchange, lpszPropName, strValue, lpszDefaultValue);
		}
		else
		{
			CString strValue;
			PX_String(m_pPropExchange, lpszPropName, strValue, lpszDefaultValue);

			if (*bstrValue)
			{
				SysFreeString(*bstrValue);
			}
			*bstrValue = strValue.AllocSysString();
		}
	}
}


void CPropExchangeCtrl::OleExchangeInt(LPCTSTR lpszPropName, long* pbValue, long nDefaultValue)
{
	if (!pbValue)
		AfxThrowOleException(E_INVALIDARG);

	if (m_pPropExchange)
	{
		PX_Long(m_pPropExchange, lpszPropName, *pbValue, nDefaultValue);
	}
}

LPDISPATCH CPropExchangeCtrl::OleGetSafeHandle()
{
	return m_pPropExchange ? m_pPropExchange->GetIDispatch(TRUE) : NULL;
}

BOOL CPropExchangeCtrl::OleGetLoading()
{
	return m_pPropExchange ? m_pPropExchange->IsLoading() : TRUE;
}

void CPropExchangeCtrl::OleEmptySection()
{
	if (m_pPropExchange) m_pPropExchange->EmptySection();
}

BOOL CPropExchangeCtrl::OleGetValid()
{
	return m_pPropExchange && m_propExchangeType != propExchangeUndefined &&
		m_pPropExchange->OnBeforeExchange();
}


CXTPPropExchange* PropExchangeFromControl(LPDISPATCH lpPropExchage)
{
	if (!lpPropExchage)
		return NULL;

	COleDispatchDriver disp(lpPropExchage, FALSE);
	LPDISPATCH lpDispatch = NULL;
	disp.GetProperty(10, VT_DISPATCH, &lpDispatch);

	if (!lpDispatch)
		return NULL;

	CXTPPropExchange* pPX = (CXTPPropExchange*)CXTPPropExchange::FromIDispatch(lpDispatch);

	if (!pPX)
	{
		lpDispatch->Release();
		return NULL;
	}

	return pPX;
}

LPDISPATCH COleControlGlobalSettings::OleCreatePropExchange()
{
	CPropExchangeCtrl* p = new CPropExchangeCtrl();
	return XTPGetDispatch(p, FALSE);

}

#endif
