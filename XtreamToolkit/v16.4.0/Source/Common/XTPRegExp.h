// XTPRegExp.h : RegExp Helpers
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPREGEXP_H__)
#define __XTPREGEXP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#pragma pack(push, 8)

#if (_MSC_VER > 1100)
#pragma warning(push)
#endif

#pragma warning(disable : 4310)
#pragma warning(disable : 4244)
#pragma warning(disable : 4786)

#include <comdef.h>

namespace XTPREGEXP {

//
// Forward references and typedefs
//

struct __declspec(uuid("3f4daca0-160d-11d2-a8e9-00104b365c9f"))
/* dual interface */ IRegExp;
struct __declspec(uuid("3f4daca1-160d-11d2-a8e9-00104b365c9f"))
/* dual interface */ IMatch;
struct __declspec(uuid("3f4daca2-160d-11d2-a8e9-00104b365c9f"))
/* dual interface */ IMatchCollection;
struct /* coclass */ RegExp;

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(IRegExp, __uuidof(IRegExp));
_COM_SMARTPTR_TYPEDEF(IMatch, __uuidof(IMatch));
_COM_SMARTPTR_TYPEDEF(IMatchCollection, __uuidof(IMatchCollection));

//
// Type library items
//

struct __declspec(uuid("3f4daca0-160d-11d2-a8e9-00104b365c9f"))
IRegExp : public IDispatch
{
	//
	// Raw methods provided by interface
	//

	virtual HRESULT __stdcall get_Pattern (
		BSTR * pPattern ) = 0;
	virtual HRESULT __stdcall put_Pattern (
		BSTR pPattern ) = 0;
	virtual HRESULT __stdcall get_IgnoreCase (
		VARIANT_BOOL * pIgnoreCase ) = 0;
	virtual HRESULT __stdcall put_IgnoreCase (
		VARIANT_BOOL pIgnoreCase ) = 0;
	virtual HRESULT __stdcall get_Global (
		VARIANT_BOOL * pGlobal ) = 0;
	virtual HRESULT __stdcall put_Global (
		VARIANT_BOOL pGlobal ) = 0;
	virtual HRESULT __stdcall Execute (
		BSTR sourceString,
		IDispatch * * ppMatches ) = 0;
	virtual HRESULT __stdcall Test (
		BSTR sourceString,
		VARIANT_BOOL * pMatch ) = 0;
	virtual HRESULT __stdcall Replace (
		BSTR sourceString,
		BSTR replaceString,
		BSTR * pDestString ) = 0;
};

struct __declspec(uuid("3f4daca1-160d-11d2-a8e9-00104b365c9f"))
IMatch : public IDispatch
{
	//
	// Raw methods provided by interface
	//

	virtual HRESULT __stdcall get_Value (
		BSTR * pValue ) = 0;
	virtual HRESULT __stdcall get_FirstIndex (
		long * pFirstIndex ) = 0;
	virtual HRESULT __stdcall get_Length (
		long * pLength ) = 0;
};

struct __declspec(uuid("3f4daca2-160d-11d2-a8e9-00104b365c9f"))
IMatchCollection : public IDispatch
{
	//
	// Raw methods provided by interface
	//

	virtual HRESULT __stdcall get_Item (
		long index,
		IDispatch * * ppMatch ) = 0;
	virtual HRESULT __stdcall get_Count (
		long * pCount ) = 0;
	virtual HRESULT __stdcall get__NewEnum (
		IUnknown * * ppEnum ) = 0;
};

struct __declspec(uuid("3f4daca4-160d-11d2-a8e9-00104b365c9f")) RegExp;

}

class CXTPRegExp
{
public:
	CXTPRegExp()
	{

	}
public:
	void SetPattern(LPCTSTR lpszPattern)
	{
		if (!CreateInstance())
			return;
		m_regexpPtr->put_Pattern(_bstr_t(XTP_CT2CW(lpszPattern)));
	}

	void SetMatchCase(BOOL bMatchCase)
	{
		if (!CreateInstance())
			return;

		m_regexpPtr->put_IgnoreCase(bMatchCase ? VARIANT_FALSE : VARIANT_TRUE);
	}

public:
	BOOL Test(LPCTSTR lpszSourceString)
	{
		if (!CreateInstance())
			return FALSE;

		VARIANT_BOOL bTest = VARIANT_FALSE;
		if (FAILED(m_regexpPtr->Test(_bstr_t(XTP_CT2CW(lpszSourceString)), &bTest)))
			return FALSE;

		return bTest != VARIANT_FALSE;
	}

	BOOL Replace(LPCTSTR lpszSourceString, LPCTSTR lpszReplaceString, CString& strDestString)
	{
		if (!CreateInstance())
			return FALSE;

		BSTR bstrResult = 0;
		if (FAILED(m_regexpPtr->Replace(_bstr_t(XTP_CT2CW(lpszSourceString)), _bstr_t(XTP_CT2CW(lpszReplaceString)), &bstrResult)))
			return FALSE;

		strDestString = bstrResult;
		SysFreeString(bstrResult);

		return TRUE;
	}

	long Execute(LPCTSTR lpszSourceString)
	{
		m_matchesPtr = NULL;

		if (!CreateInstance())
			return 0;

		long nCount = 0;

		IDispatch* _result = 0;
		if (SUCCEEDED(m_regexpPtr->Execute(_bstr_t(XTP_CT2CW(lpszSourceString)), &_result)))
		{
			m_matchesPtr = _result;

			if (m_matchesPtr != NULL)
			{
				m_matchesPtr->get_Count(&nCount);
			}

		}
		if (_result) _result->Release();
		if (nCount == 0) m_matchesPtr = NULL;


		return nCount;
	}

	BOOL GetMatch(long nIndex, long* pnFirstIndex, long* pnLength)
	{
		if (m_matchesPtr == NULL)
			return FALSE;

		if (pnFirstIndex == 0 || pnLength == 0)
			return FALSE;

		long nCount = 0;
		m_matchesPtr->get_Count(&nCount);
		if (nIndex >= nCount)
			return FALSE;

		IDispatch* _result = 0;
		if (FAILED(m_matchesPtr->get_Item(nIndex, &_result)))
			return FALSE;

		XTPREGEXP::IMatchPtr match =_result;
		if (_result) _result->Release();

		if (match == NULL)
			return FALSE;

		match->get_FirstIndex(pnFirstIndex);
		match->get_Length(pnLength);
		return TRUE;
	}

protected:
	BOOL CreateInstance()
	{
		if (m_regexpPtr == NULL)
		{
			if (!SUCCEEDED(m_regexpPtr.CreateInstance(__uuidof(XTPREGEXP::RegExp))))
			{
				TRACE(_T("Warning: CreateInstance failed in CXTPRegExp --\n"));
				TRACE(_T("\tperhaps AfxOleInit() has not been called"));

				return FALSE;
			}
		}
		return TRUE;
	}


protected:
	XTPREGEXP::IRegExpPtr m_regexpPtr;
	XTPREGEXP::IMatchCollectionPtr m_matchesPtr;
};


#if (_MSC_VER > 1100)
#pragma warning(pop)
#endif

#pragma pack(pop)

#endif //#if !defined(__XTPREGEXP_H__)
