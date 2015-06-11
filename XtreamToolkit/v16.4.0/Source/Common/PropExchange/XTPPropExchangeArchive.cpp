#include "StdAfx.h"
#include <Common/XTPPropExchange.h>
#include <Common/XTPVC80Helpers.h>


//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeArchive

IMPLEMENT_DYNAMIC(CXTPPropExchangeArchive, CXTPPropExchange)

CXTPPropExchangeArchive::CXTPPropExchangeArchive(CArchive& ar) :
	m_ar((CSelfArchive&)ar)
{
	ASSERT_POINTER(&ar, CArchive);
	m_bLoading = m_ar.IsLoading();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPPropExchange* CXTPPropExchangeArchive::GetSection(LPCTSTR /*pszSection*/)
{
	CXTPPropExchangeArchive* pSection = new CXTPPropExchangeArchive(m_ar);
	pSection->InitSection(this);
	return pSection;
}


BOOL CXTPPropExchangeArchive::ExchangeRuntimeClass(LPCTSTR /*pszPropName*/, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass)
{
	if (!OnBeforeExchange())
		return FALSE;

	if (!IsLoading())
	{
		m_ar.WriteClass(pClass);
	}
	else
	{
		pClass = m_ar.ReadClass(pDefaultClass, &m_nSchema);

		if (!pClass)
			return FALSE;
	}
	return TRUE;
}

BOOL CXTPPropExchangeArchive::ExchangeObjectInstance(CObject*& pObject, CRuntimeClass* pObjectRuntime)
{
	if (!CXTPPropExchange::ExchangeObjectInstance(pObject, pObjectRuntime))
		return FALSE;

	if (IsLoading())
	{
		m_ar.CheckCount();
		m_ar.m_pLoadArray->InsertAt(m_ar.m_nMapCount++, pObject);
	}
	else
	{
		m_ar.MapObject(NULL);
		m_ar.CheckCount();
		m_ar.m_pStoreMap->SetAt(pObject, (LPVOID)(DWORD_PTR)m_ar.m_nMapCount++);

	}

	return TRUE;
}

BOOL CXTPPropExchangeArchive::ExchangeProp(LPCTSTR /*pszPropName*/, VARTYPE vtProp,
		void* pvProp, const void* pvDefault)
{
	if (!OnBeforeExchange())
		return FALSE;

	UNUSED(pvDefault);       // unused in release builds

	ASSERT(AfxIsValidAddress(pvProp, 1, FALSE));
	ASSERT((pvDefault == NULL) || AfxIsValidAddress(pvDefault, 1, FALSE));

	if (m_bLoading)
	{
		switch (vtProp)
		{
		case VT_UI1:
			m_ar >> *(BYTE*)pvProp;
			break;
		case VT_I2:
			m_ar >> *(WORD*)pvProp;
			break;
		case VT_I4:
			m_ar >> *(long*)pvProp;
			break;
		case VT_BOOL:
			m_ar >> *(BOOL*)pvProp;
			break;
		case VT_BSTR:
			ASSERT(FALSE);
			break;
		case VT_LPSTR:
			m_ar >> *(CString*)pvProp;
			break;
		case VT_CY:
			m_ar >> ((CY*)pvProp)->Lo;
			m_ar >> ((CY*)pvProp)->Hi;
			break;
		case VT_R4:
			m_ar >> *(float*)pvProp;
			break;
		case VT_R8:
			m_ar >> *(double*)pvProp;
			break;
		case VT_EX_RECT:
			m_ar >> *(RECT*)pvProp;
			break;
		case VT_EX_SIZE:
			m_ar >> *(SIZE*)pvProp;
			break;
		case VT_VARIANT:
			m_ar >> *(COleVariant*)pvProp;
			break;
		case VT_DATE:
			m_ar >> *(COleDateTime*)pvProp;
			break;
		}
	}
	else
	{
		switch (vtProp)
		{
		case VT_UI1:
			m_ar << *(BYTE*)pvProp;
			break;
		case VT_I2:
			m_ar << *(WORD*)pvProp;
			break;
		case VT_I4:
			m_ar << *(long*)pvProp;
			break;
		case VT_BOOL:
			m_ar << *(BOOL*)pvProp;
			break;
		case VT_BSTR:
			ASSERT(FALSE);
			break;
		case VT_LPSTR:
			m_ar << *(CString*)pvProp;
			break;
		case VT_CY:
			m_ar << ((CY*)pvProp)->Lo;
			m_ar << ((CY*)pvProp)->Hi;
			break;
		case VT_R4:
			m_ar << *(float*)pvProp;
			break;
		case VT_R8:
			m_ar << *(double*)pvProp;
			break;
		case VT_EX_RECT:
			m_ar << *(RECT*)pvProp;
			break;
		case VT_EX_SIZE:
			m_ar << *(SIZE*)pvProp;
			break;
		case VT_VARIANT:
			m_ar << *(COleVariant*)pvProp;
			break;
		case VT_DATE:
			m_ar << *(COleDateTime*)pvProp;
			break;
		}
	}

	return TRUE;
}

void CXTPPropExchangeArchive::WriteCount (DWORD dwCount)
{
	m_ar.WriteCount(dwCount);
}
DWORD CXTPPropExchangeArchive::ReadCount()
{
	return (DWORD)m_ar.ReadCount();
}
void CXTPPropExchangeArchive::Write (LPCTSTR /*pszPropName*/, const void* lpBuf, UINT nCount)
{
	ASSERT(lpBuf);
	m_ar.Write(lpBuf, nCount);
}

UINT CXTPPropExchangeArchive::Read (LPCTSTR /*pszPropName*/, void* lpBuf, UINT nCount)
{
	ASSERT(lpBuf);
	return m_ar.Read(lpBuf, nCount);
}

BOOL CXTPPropExchangeArchive::ExchangeBlobProp(LPCTSTR /*pszPropName*/, BYTE*& pData, DWORD& nBytes)
{
	if (!OnBeforeExchange())
		return FALSE;

	if (IsStoring())
	{
		WriteCount(nBytes);
		m_ar.Write(pData, nBytes);
	}
	else
	{
		DWORD nCount = ReadCount();

		if (pData == NULL)
		{
			pData = (BYTE*)malloc(nCount);
			nBytes = nCount;
		}

		if (nBytes < nCount)
			return FALSE;

		return m_ar.Read(pData, nCount) == nCount;
	}

	return TRUE;
}

CArchive* CXTPPropExchangeArchive::GetArchive(LPCTSTR /*pszPropName*/)
{
	return &m_ar;
}

void CXTPPropExchangeArchive::ReleaseArchive(LPCTSTR /*pszPropName*/, CArchive* /*pArchive*/)
{

}

void CXTPPropExchangeArchive::ExchangeSchemaSafe()
{
	DWORD dwSafe = 0xFF0ADBF;

	if (IsStoring())
	{
		m_ar << dwSafe;
		m_ar << m_nSchema;
	}
	else
	{
		if (m_ar.m_lpBufCur + sizeof(DWORD) > m_ar.m_lpBufMax)
			m_ar.FillBuffer(sizeof(DWORD));

		DWORD dw = *(DWORD*)m_ar.m_lpBufCur;

		if (dw == dwSafe)
		{
			m_ar >> dwSafe;
			m_ar >> m_nSchema;
		}
		else
		{
			m_nSchema = _XTP_SCHEMA_100;
		}
	}
}
