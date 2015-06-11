// XTPSyntaxEditSectionManager.cpp: implementation of the CXTPSyntaxEditSectionManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XTPSyntaxEditSectionManager.h"

//-----------------------------------------------------------------------
// struct: XTP_EDIT_SCHEMAFILEINFO
//-----------------------------------------------------------------------
XTP_EDIT_SCHEMAFILEINFO::XTP_EDIT_SCHEMAFILEINFO() : uValue((UINT)-1)
{

}

XTP_EDIT_SCHEMAFILEINFO::XTP_EDIT_SCHEMAFILEINFO(const XTP_EDIT_SCHEMAFILEINFO& info)
	: csName(info.csName)
	, csValue(info.csValue)
	, csDesc(info.csDesc)
	, uValue(info.uValue)
{

}

XTP_EDIT_SCHEMAFILEINFO::XTP_EDIT_SCHEMAFILEINFO(const CString& srcName, const CString& srcValue)
	: csName(srcName)
	, csValue(srcValue)
	, uValue((UINT)-1)
{
	if (csValue.Find(_T(';')) != -1)
	{
		CXTPSyntaxEditSectionManager().SplitString(
			csValue, csDesc, srcValue, _T(';'));
	}
}

const XTP_EDIT_SCHEMAFILEINFO& XTP_EDIT_SCHEMAFILEINFO::operator=(const XTP_EDIT_SCHEMAFILEINFO& info)
{
	csName  = info.csName;
	csValue = info.csValue;
	csDesc  = info.csDesc;
	uValue  = info.uValue;
	return *this;
}

BOOL XTP_EDIT_SCHEMAFILEINFO::operator==(const XTP_EDIT_SCHEMAFILEINFO& info) const
{
	if (&info == this)
		return TRUE;

	if (csName  != info.csName)
		return FALSE;
	if (csValue != info.csValue)
		return FALSE;
	if (csDesc  != info.csDesc)
		return FALSE;
	if (uValue  != info.uValue)
		return FALSE;

	return FALSE;
}

//-----------------------------------------------------------------------
// class: CXTPSyntaxEditSchemaFileInfoList
//-----------------------------------------------------------------------

POSITION CXTPSyntaxEditSchemaFileInfoList::LookupName(const CString& csName, XTP_EDIT_SCHEMAFILEINFO& info) const
{
	for (POSITION pos = GetHeadPosition(); pos;)
	{
		POSITION posCurr = pos;
		info = GetNext(pos);
		if (csName.CompareNoCase(info.csName) == 0)
		{
			return posCurr;
		}
	}
	return NULL;
}

POSITION CXTPSyntaxEditSchemaFileInfoList::LookupValue(const CString& csValue, XTP_EDIT_SCHEMAFILEINFO& info) const
{
	for (POSITION pos = GetHeadPosition(); pos;)
	{
		POSITION posCurr = pos;
		info = GetNext(pos);
		if (csValue.CompareNoCase(info.csValue) == 0)
		{
			return posCurr;
		}
	}
	return NULL;
}

POSITION CXTPSyntaxEditSchemaFileInfoList::LookupDesc(const CString& csDesc, XTP_EDIT_SCHEMAFILEINFO& info) const
{
	for (POSITION pos = GetHeadPosition(); pos;)
	{
		POSITION posCurr = pos;
		info = GetNext(pos);
		if (csDesc.CompareNoCase(info.csDesc) == 0)
		{
			return posCurr;
		}
	}
	return NULL;
}

POSITION CXTPSyntaxEditSchemaFileInfoList::LookupValue(const UINT& uValue, XTP_EDIT_SCHEMAFILEINFO& info) const
{
	for (POSITION pos = GetHeadPosition(); pos;)
	{
		POSITION posCurr = pos;
		info = GetNext(pos);
		if (uValue == info.uValue)
		{
			return posCurr;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------
// singleton: CXTPSyntaxEditSectionManager
//-----------------------------------------------------------------------

CXTPSyntaxEditSectionManager::CXTPSyntaxEditSectionManager()
{
}

CXTPSyntaxEditSectionManager::~CXTPSyntaxEditSectionManager()
{
}

BOOL CXTPSyntaxEditSectionManager::SplitString(CString& csLeft, CString& csRight, const CString& csBuffer, TCHAR chSep) const
{
	if (!AfxExtractSubString(csLeft, csBuffer, 0, chSep))
		return FALSE;

	// trim whitespace from string.
	csLeft.TrimLeft();
	csLeft.TrimRight();

	if (!AfxExtractSubString(csRight, csBuffer, 1, chSep))
		return FALSE;

	// trim whitespace from string.
	csRight.TrimLeft();
	csRight.TrimRight();

	return TRUE;
}

BOOL CXTPSyntaxEditSectionManager::ParseSectionKey(XTP_EDIT_SCHEMAFILEINFO& info, const CString& csBuffer) const
{
	// extract the name and value pair from the string.
	if (!SplitString(info.csName, info.csValue, csBuffer, _T('=')))
		return FALSE;

	// check to see if there is a description associated with the info,
	// if not return success.
	if (info.csValue.Find(_T(';')) == -1)
		return TRUE;

	// extract the value and description pair from the string.
	CString csValue = info.csValue;
	if (!SplitString(info.csValue, info.csDesc, csValue, _T(';')))
		return FALSE;

	return TRUE;
}

int CXTPSyntaxEditSectionManager::GetSectionNames(CStringArray& arSections, LPCTSTR lpszFilePath) const
{
	TCHAR* pszBuffer = NULL;
	int nMaxSize = 1024;
	for (;;)
	{
		pszBuffer = new TCHAR[nMaxSize];
		long lSize = ::GetPrivateProfileSectionNames(pszBuffer, nMaxSize, lpszFilePath);
		if ((lSize > 0) && (lSize == (nMaxSize - 2)))
		{
			nMaxSize *= 2;
			SAFE_DELETE_AR(pszBuffer);
		}
		else
		{
			nMaxSize = lSize;
			break;
		}
	}

	CString csBuffer;
	for (int i = 0; i < nMaxSize; i++)
	{
		if (pszBuffer[i] != '\0')
		{
			csBuffer += pszBuffer[i];
		}
		else
		{
			if (!csBuffer.IsEmpty())
			{
				arSections.Add(csBuffer);
				csBuffer.Empty();
			}
		}
	}

	SAFE_DELETE_AR(pszBuffer);

	return (int)arSections.GetSize();
}

int CXTPSyntaxEditSectionManager::GetSectionKeyList(CXTPSyntaxEditSchemaFileInfoList& infoList, LPCTSTR lpszFilePath, LPCTSTR lpszSectionName) const
{
	TCHAR* pszBuffer = NULL;
	int nMaxSize = 1024;

	for (;;)
	{
		pszBuffer = new TCHAR[nMaxSize];
		long lSize = ::GetPrivateProfileSection(lpszSectionName, pszBuffer, nMaxSize, lpszFilePath);
		if ((lSize > 0) && (lSize == (nMaxSize - 2)))
		{
			nMaxSize *= 2;
			SAFE_DELETE_AR(pszBuffer);
		}
		else
		{
			nMaxSize = lSize;
			break;
		}
	}

	CString csBuffer;

	for (int i = 0; i < nMaxSize; i++)
	{
		if (pszBuffer[i] != '\0')
		{
			csBuffer += pszBuffer[i];
		}
		else
		{
			if (!csBuffer.IsEmpty())
			{
				XTP_EDIT_SCHEMAFILEINFO info;
				ParseSectionKey(info, csBuffer);
				infoList.AddHead(info);
				csBuffer.Empty();
			}
		}
	}

	SAFE_DELETE_AR(pszBuffer);

	return (int)infoList.GetCount();
}

int CXTPSyntaxEditSectionManager::GetSectionNames(CStringArray& arSections, CStringArray& arPassedString) const
{
	int N = (int)arPassedString.GetSize();
	int n(0);
	CString s;
	for (int i = 0; i < N; i++)
	{
		s = arPassedString.GetAt(i);
		s.TrimLeft(); s.TrimRight();
		if (s.IsEmpty())
			continue;
		n = s.GetLength();
		if (n  == 0)
			continue;
		if (s[0] == '[' && s[n - 1] == ']')
		{
			s.Replace(_T("["), _T(""));
			s.Replace(_T("]"), _T(""));
			arSections.Add(s);
		}
	}
	return (int)arSections.GetSize();
}

int CXTPSyntaxEditSectionManager::GetSectionKeyList(CXTPSyntaxEditSchemaFileInfoList& infoList, CStringArray& arPassedString, CString sSectionName) const
{
	int N = (int)arPassedString.GetSize();
	int n(0);
	CString s;
	for (int i = 0; i < N; i++)
	{
		s = arPassedString.GetAt(i);
		s.TrimLeft(); s.TrimRight();
		if (s.IsEmpty())
			continue;
		n = s.GetLength();
		if (n  == 0)
			continue;

		XTP_EDIT_SCHEMAFILEINFO info;
		ParseSectionKey(info, s);
		infoList.AddHead(info);
	}
	return (int)infoList.GetCount();
}
