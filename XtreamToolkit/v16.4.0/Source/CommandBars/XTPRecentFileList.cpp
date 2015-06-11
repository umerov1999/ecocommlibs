// XTPRecentFileList.cpp : implementation file
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include "XTPRecentFileList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPRecentFileListItem

IMPLEMENT_DYNAMIC(CXTPRecentFileListItem, CXTPCmdTarget);

CXTPRecentFileListItem::CXTPRecentFileListItem(CXTPRecentFileList* pList)
{
	m_bPinned = FALSE;
	m_nIconId = -1;
	m_pList = pList;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
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

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
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

#ifdef _XTP_ACTIVEX



BEGIN_DISPATCH_MAP(CXTPRecentFileListItem, CCmdTarget)
//{{AFX_DISPATCH_MAP(CXTPRecentFileListItem)
	DISP_PROPERTY_ID(CXTPRecentFileListItem, "Caption", 1, m_strCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPRecentFileListItem, "Tag", 2, m_strTag, VT_BSTR)
	DISP_PROPERTY_ID(CXTPRecentFileListItem, "IconId", 3, m_nIconId, VT_I4)
	DISP_PROPERTY_ID(CXTPRecentFileListItem, "Pinned", 4, m_bPinned, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPRecentFileListItem, "PathName", DISPID_VALUE, OleGetPathName, SetNotSupported, VT_BSTR)
//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {9113D51B-CBD0-4AF0-9D33-EB56AB0648D8}
static const IID IID_IRecentFileListItem =
{ 0x9113d51b, 0xcbd0, 0x4af0, { 0x9d, 0x33, 0xeb, 0x56, 0xab, 0x6, 0x48, 0xd8 } };

BEGIN_INTERFACE_MAP(CXTPRecentFileListItem, CCmdTarget)
	INTERFACE_PART(CXTPRecentFileListItem, IID_IRecentFileListItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRecentFileListItem, IID_IRecentFileListItem)

BSTR CXTPRecentFileListItem::OleGetPathName()
{
	return GetPathName().AllocSysString();
}


/////////////////////////////////////////////////////////////////////////////
// CXTPRecentFileList


BEGIN_DISPATCH_MAP(CXTPRecentFileList, CCmdTarget)
//{{AFX_DISPATCH_MAP(CXTPRecentFileList)
	DISP_FUNCTION_ID(CXTPRecentFileList, "Add", 1, OleAdd, VT_DISPATCH, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CXTPRecentFileList, "Count", 3, OleGetItemCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPRecentFileList, "MaxSize", 2, OleGetMaxSize, OleSetMaxSize, VT_I4)
	DISP_FUNCTION_ID(CXTPRecentFileList, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPRecentFileList, "Item", DISPID_VALUE, OleItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPRecentFileList, "Remove", 4, OleRemove, VT_EMPTY, VTS_I4)
	DISP_PROPERTY_ID(CXTPRecentFileList, "Pinable", 5, m_bPinable, VT_BOOL)
//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICXTPRecentFileList to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {C6A3D51B-CBD0-4AF0-9D33-EB56AB0648D8}
static const IID IID_IRecentFileList =
{ 0xc6a3d51b, 0xcbd0, 0x4af0, { 0x9d, 0x33, 0xeb, 0x56, 0xab, 0x6, 0x48, 0xd8 } };

BEGIN_INTERFACE_MAP(CXTPRecentFileList, CCmdTarget)
	INTERFACE_PART(CXTPRecentFileList, IID_IRecentFileList, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_ENUM_VARIANT(CXTPRecentFileList)


/////////////////////////////////////////////////////////////////////////////
// CXTPRecentFileList message handlers

IMPLEMENT_OLETYPELIB_EX(CXTPRecentFileList, IID_IRecentFileList)

LPDISPATCH CXTPRecentFileList::OleAdd(LPCTSTR lpszFileName)
{
	return XTPGetDispatch(AddItem(lpszFileName));
}

long CXTPRecentFileList::OleGetItemCount()
{
	for (int i = 0; i < m_nSize; i++)
		if (m_pItems[i] == NULL)
			return i;

	return m_nSize;
}

long CXTPRecentFileList::OleGetMaxSize()
{
	return m_nSize;
}

void CXTPRecentFileList::OleSetMaxSize(int nMaxSize)
{
	if (nMaxSize <= 0)
		AfxThrowOleException(E_INVALIDARG);

	if (nMaxSize <= m_nSize)
	{
		for (int i = nMaxSize; i < m_nSize; i++)
		{
			CMDTARGET_RELEASE(m_pItems[i]);
		}

		m_nSize = nMaxSize;
	}
	else
	{
		int i;

		for (i = 0; i < m_nSize; i++)
		{
			CMDTARGET_RELEASE(m_pItems[i]);
		}

		delete[] m_pItems;
		delete[] m_arrNames;

		m_nSize = nMaxSize;

		m_arrNames = new CString[m_nSize];

		m_pItems = new CXTPRecentFileListItem*[m_nSize];
		for (i = 0; i < m_nSize; i++)
			m_pItems[i] = NULL;
	}

}

LPDISPATCH CXTPRecentFileList::OleItem(int nIndex)
{
	return OleGetItem(nIndex - 1);
}

LPDISPATCH CXTPRecentFileList::OleGetItem(int nIndex)
{
	if (nIndex >= m_nSize || nIndex < 0)
		AfxThrowOleException(E_INVALIDARG);

	return XTPGetDispatch(m_pItems[nIndex]);
}

void CXTPRecentFileList::OleRemove(int nIndex)
{
	nIndex--;

	if (nIndex >= m_nSize || nIndex < 0)
		AfxThrowOleException(E_INVALIDARG);

	Remove(nIndex);

}


#endif
