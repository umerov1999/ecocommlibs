// XTPRecentFileList.cpp : implementation file
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include "XTPRecentFileList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPRecentFileListItem

CXTPRecentFileListItem::CXTPRecentFileListItem(CXTPRecentFileList* pList)
{
	m_bPinned = FALSE;
	m_nIconId = -1;
	m_pList = pList;

}


//////////////////////////////////////////////////////////////////////////
// CXTPRecentFileList

CXTPRecentFileList::CXTPRecentFileList(UINT nStart, LPCTSTR lpszSection, LPCTSTR lpszEntryFormat, int nSize, int nMaxDispLen)
	: CRecentFileList(nStart, lpszSection, lpszEntryFormat, nSize, nMaxDispLen)
{
	m_pItems = new CXTPRecentFileListItem*[nSize];
	for (int i = 0; i < m_nSize; i++)
		m_pItems[i] = NULL;

	m_strOriginal = _T("PinableRecentFileList");

	m_bPinable = TRUE;

}

CXTPRecentFileList::~CXTPRecentFileList()
{
	for (int i = 0; i < m_nSize; i++)
	{
		CMDTARGET_RELEASE(m_pItems[i]);
	}
	delete[] m_pItems;
}

CXTPRecentFileListItem* CXTPRecentFileList::GetItem(int nIndex) const
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < m_nSize);

	return m_pItems[nIndex];
}

int CXTPRecentFileList::GetCount() const
{
	for (int i = 0; i < m_nSize; i++)
		if (m_pItems[i] == NULL)
			return i;

	return m_nSize;
}


BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2);

void CXTPRecentFileList::OnNewItem(CXTPRecentFileListItem* /*pItem*/)
{

}

void CXTPRecentFileList::Add(LPCTSTR lpszPathName)
{
	AddItem(lpszPathName);
}

CXTPRecentFileListItem* CXTPRecentFileList::AddItem(LPCTSTR lpszPathName)
{
	ASSERT(m_arrNames != NULL);
	ASSERT(lpszPathName != NULL);
	ASSERT(AfxIsValidString(lpszPathName));

	// fully qualify the path name
	TCHAR szTemp[_MAX_PATH];
	AfxFullPath(szTemp, lpszPathName);

	int iMRU = 0;
	CXTPRecentFileListItem* pItem = NULL;

	// update the MRU list, if an existing MRU string matches file name
	for (; iMRU < m_nSize - 1; iMRU++)
	{
		if (AfxComparePath(m_arrNames[iMRU], szTemp))
		{
			pItem = m_pItems[iMRU];
			break;      // iMRU will point to matching entry
		}
	}

	if (iMRU == m_nSize - 1) // Not found
	{
		for (; iMRU >= 0; iMRU--)
		{
			if (!m_pItems[iMRU] || !m_pItems[iMRU]->IsPinned())
				break;
		}
	}

	if (iMRU < 0)
		return NULL;

	if (pItem == NULL && m_pItems[iMRU] != NULL)
	{
		CMDTARGET_RELEASE(m_pItems[iMRU]);
	}

	// move MRU strings before this one down
	for (; iMRU > 0; iMRU--)
	{
		ASSERT(iMRU > 0);
		ASSERT(iMRU < m_nSize);
		m_arrNames[iMRU] = m_arrNames[iMRU - 1];
		m_pItems[iMRU] = m_pItems[iMRU - 1];
	}
	// place this one at the beginning
	m_arrNames[0] = szTemp;

	if (pItem)
	{
		m_pItems[0] = pItem;
	}
	else
	{
		pItem = new CXTPRecentFileListItem(this);
		pItem->m_strPathName = m_arrNames[0];

		m_pItems[0] = pItem;

		OnNewItem(pItem);
	}
	return pItem;
}

void CXTPRecentFileList::Remove(int nIndex)
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < m_nSize);

	if (!m_pItems[nIndex])
		return;

	if (m_pItems[nIndex]->IsPinned())
		return;

	CMDTARGET_RELEASE(m_pItems[nIndex]);

	int iMRU(0);
	for (iMRU = nIndex; iMRU < m_nSize - 1; iMRU++)
	{
		m_arrNames[iMRU] = m_arrNames[iMRU + 1];
		m_pItems[iMRU] = m_pItems[iMRU + 1];
	}

	ASSERT(iMRU == m_nSize - 1);
	m_arrNames[iMRU].Empty();
	m_pItems[iMRU] = NULL;
}

void CXTPRecentFileList::WriteList()
{
	ASSERT(m_arrNames != NULL);
	ASSERT(!m_strSectionName.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());
	LPTSTR pszEntry = new TCHAR[max(20, m_strEntryFormat.GetLength() + 7)];

	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(m_strSectionName, NULL, NULL);

	for (int iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		if (m_pItems[iMRU] && !m_arrNames[iMRU].IsEmpty())
		{
			wsprintf(pszEntry, m_strEntryFormat, iMRU + 1);
			pApp->WriteProfileString(m_strSectionName, pszEntry, m_arrNames[iMRU]);

			if (m_pItems[iMRU]->IsPinned())
			{
				wsprintf(pszEntry, _T("Pinned%d"), iMRU + 1);
				pApp->WriteProfileInt(m_strSectionName, pszEntry, 1);
			}

			if (!m_pItems[iMRU]->GetCaption().IsEmpty())
			{
				wsprintf(pszEntry, _T("Caption%d"), iMRU + 1);
				pApp->WriteProfileString(m_strSectionName, pszEntry, m_pItems[iMRU]->GetCaption());
			}

			if (!m_pItems[iMRU]->GetTag().IsEmpty())
			{
				wsprintf(pszEntry, _T("Tag%d"), iMRU + 1);
				pApp->WriteProfileString(m_strSectionName, pszEntry, m_pItems[iMRU]->GetTag());
			}

			if (m_pItems[iMRU]->GetIconId() != -1)
			{
				wsprintf(pszEntry, _T("IconId%d"), iMRU + 1);
				pApp->WriteProfileInt(m_strSectionName, pszEntry, m_pItems[iMRU]->GetIconId());
			}
		}
	}
	delete[] pszEntry;
}

void CXTPRecentFileList::ReadList()
{
	ASSERT(m_arrNames != NULL);
	ASSERT(!m_strSectionName.IsEmpty());
	ASSERT(!m_strEntryFormat.IsEmpty());

	LPTSTR pszEntry = new TCHAR[max(20, m_strEntryFormat.GetLength() + 7)];
	CWinApp* pApp = AfxGetApp();

	for (int iMRU = 0; iMRU < m_nSize; iMRU++)
	{
		CMDTARGET_RELEASE(m_pItems[iMRU]);

		wsprintf(pszEntry, m_strEntryFormat, iMRU + 1);
		m_arrNames[iMRU] = pApp->GetProfileString(m_strSectionName, pszEntry, _T(""));

		if (!m_arrNames[iMRU].IsEmpty())
		{
			m_pItems[iMRU] = new CXTPRecentFileListItem(this);

			wsprintf(pszEntry, _T("Pinned%d"), iMRU + 1);
			m_pItems[iMRU]->m_bPinned = pApp->GetProfileInt(m_strSectionName, pszEntry, FALSE);

			wsprintf(pszEntry, _T("IconId%d"), iMRU + 1);
			m_pItems[iMRU]->m_nIconId = pApp->GetProfileInt(m_strSectionName, pszEntry, -1);

			wsprintf(pszEntry, _T("Caption%d"), iMRU + 1);
			m_pItems[iMRU]->m_strCaption = pApp->GetProfileString(m_strSectionName, pszEntry, _T(""));

			wsprintf(pszEntry, _T("Tag%d"), iMRU + 1);
			m_pItems[iMRU]->m_strTag = pApp->GetProfileString(m_strSectionName, pszEntry, _T(""));

			m_pItems[iMRU]->m_strPathName = m_arrNames[iMRU];
		}
	}
	delete[] pszEntry;
}

