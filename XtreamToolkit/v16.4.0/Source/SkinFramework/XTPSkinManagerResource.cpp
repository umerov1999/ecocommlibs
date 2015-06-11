// XTPSkinManagerResource.cpp: implementation of the CXTPSkinManagerResourceFile class.
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


#include <Common/XTPVC80Helpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPSynchro.h>

#include "XTPSkinManagerResource.h"
#include "XTPSkinImage.h"
#include "XTPSkinManager.h"
#include "XTPSkinManagerSchema.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CXTPSkinManagerResourceFile::CXTPSkinManagerResourceFile(CXTPSkinManager* pManager)
{
	m_lpTextFile = m_lpTextFileEnd = NULL;
	m_hModule = NULL;
	m_pIniFile = NULL;
	m_pManager = pManager ? pManager : XTPSkinManager();
	m_hInstance = NULL;
}

CXTPSkinManagerResourceFile::~CXTPSkinManagerResourceFile()
{
	Close();
}

void CXTPSkinManagerResourceFile::Close()
{
	SAFE_DELETE(m_pIniFile);

	if (m_hModule)
	{
		if (m_hModule != m_hInstance) FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

CXTPSkinManagerSchema* CXTPSkinManagerResourceFile::CreateSchema()
{
	return new CXTPSkinManagerSchema(this);
}

CString CXTPSkinManagerResourceFile::GetDefaultIniFileName()
{
	HRSRC hRsrc = FindResource(m_hModule, _T("THEMES_INI"), _T("TEXTFILE"));
	if (!hRsrc)
		return _T("");

	HGLOBAL hResData = LoadResource(m_hModule, hRsrc);
	if (!hResData)
		return _T("");

	LPWSTR lpwzThemes = (LPWSTR)LockResource(hResData);

	LPWSTR lpwzFile =  wcsstr(lpwzThemes, L"[File.");
	if (!lpwzFile)
		return _T("");

	LPWSTR pNextLine = wcschr(lpwzFile, L']');
	CString str = CString(lpwzFile + 6, (int)(pNextLine - lpwzFile) - 6);


	return str + _T("_ini");
}

BOOL CXTPSkinManagerResourceFile::Open(LPCTSTR lpszResourcePath, LPCTSTR lpszIniFileName)
{
	Close();

	m_strResourcePath = lpszResourcePath;
	m_strIniFileName = lpszIniFileName;

	if (m_hInstance == NULL)
	{
		if (m_strResourcePath.IsEmpty())
			return FALSE;

		CString strFileName = m_strResourcePath + _T('\\') + m_strIniFileName;

		if (!m_strIniFileName.IsEmpty() && FILEEXISTS_S(strFileName))
		{
			m_pIniFile = new CStdioFile;

			if (!m_pIniFile->Open(strFileName, CFile::modeRead))
			{
				SAFE_DELETE(m_pIniFile);
				return FALSE;
			}

			return TRUE;
		}

		m_hModule = LoadLibrary(lpszResourcePath);

		if (!m_hModule)
		{
			m_hModule = LoadLibraryEx(lpszResourcePath, NULL, LOAD_LIBRARY_AS_DATAFILE);
		}
	}
	else
	{
		m_hModule = m_strIniFileName.IsEmpty() ? NULL : m_hInstance;
	}

	if (!m_hModule)
	{
		return FALSE;
	}

	if (m_strIniFileName.IsEmpty())
	{
		m_strIniFileName = GetDefaultIniFileName();
	}

	CString strFilePath(m_strIniFileName);

	strFilePath.Replace(_T('.'), _T('_'));
	strFilePath.Replace(_T('\\'), _T('_'));

	HRSRC hRsrc = FindResource(m_hModule, strFilePath, _T("TEXTFILE"));
	if (!hRsrc)
		return FALSE;

	HGLOBAL hResData = LoadResource(m_hModule, hRsrc);
	if (!hResData)
		return FALSE;

	m_lpTextFile = (LPWSTR)LockResource(hResData);

	m_lpTextFileEnd = m_lpTextFile + SizeofResource(m_hModule, hRsrc)/2;

	if (m_lpTextFile && m_lpTextFile[0] == 0xFEFF)
		m_lpTextFile++;

	return TRUE;
}

BOOL CXTPSkinManagerResourceFile::ReadString(CString& str)
{
	if (m_pIniFile)
	{
		if (!m_pIniFile->ReadString(str))
		{
			m_pIniFile->Close();
			return FALSE;
		}
		return TRUE;
	}

	ASSERT(m_lpTextFile);
	if (!m_lpTextFile)
		return FALSE;

	LPWSTR pNextLine = m_lpTextFile;

	while ((pNextLine < m_lpTextFileEnd) && (*pNextLine != L'\0') && (*pNextLine != L'\r'))
		pNextLine++;

	if (pNextLine == NULL || pNextLine >= m_lpTextFileEnd || *pNextLine == L'\0')
		return FALSE;

	str = CString(m_lpTextFile, (int)(pNextLine - m_lpTextFile));

	m_lpTextFile = pNextLine + 1;
	if (m_lpTextFile < m_lpTextFileEnd && m_lpTextFile[0] == L'\n') m_lpTextFile = m_lpTextFile + 1;

	return TRUE;
}


CXTPSkinImage* CXTPSkinManagerResourceFile::LoadImage(CString strImageFile)
{
	CXTPSkinImage* pImage = NULL;

	if (m_pIniFile)
	{
		strImageFile = m_strResourcePath + _T('\\') + strImageFile;

		if (!FILEEXISTS_S(strImageFile))
			return NULL;

		pImage = new CXTPSkinImage();

		if (!pImage->LoadFile(strImageFile))
		{
			delete pImage;
			return NULL;
		}
	}
	else
	{
		strImageFile.Replace(_T('.'), _T('_'));
		strImageFile.Replace(_T('\\'), _T('_'));

		pImage = new CXTPSkinImage();

		if (!pImage->LoadFile(GetModuleHandle(), strImageFile))
		{
			delete pImage;
			return NULL;
		}
	}

	return pImage;
}
