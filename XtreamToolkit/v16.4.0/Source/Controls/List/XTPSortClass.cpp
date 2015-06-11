// XTPSortClass.cpp : implementation of the CXTPSortClass class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "XTPSortClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------------------
// CompareProc callback used for sorting.
//---------------------------------------------------------------------------
int CALLBACK CXTPSortClass::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	ASSERT(lParam1 && lParam2 && lParamSort);
	if (!lParam1 || !lParam2 || !lParamSort)
		return FALSE;

	// set item data pointers.
	ITEMDATA* pData1 = (ITEMDATA*)lParam1;
	ITEMDATA* pData2 = (ITEMDATA*)lParam2;

	// Restore data type and sort order from lParamSort
	// if lParamSort positive - ascending sort order, negative - descending
	short sOrder = (lParamSort < 0)? (short)-1 : (short)1;

	// get rid of sort order flag
	XTPSortType eType = (XTPSortType)(lParamSort * sOrder);

	switch (eType)
	{
	case xtpSortInt:
		return SortInt(
			pData1->strItem, pData2->strItem, sOrder);

	case xtpSortString:
		return SortString(
			pData1->strItem, pData2->strItem, sOrder);

	case xtpSortDateTime:
		return SortDateTime(
			pData1->strItem, pData2->strItem, sOrder);

	case xtpSortDecimal:
		return SortDecimal(
			pData1->strItem, pData2->strItem, sOrder);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// CXTPSortClass
//
CXTPSortClass::CXTPSortClass(CListCtrl* pListCtrl, const int nCol, PFNLVCOMPARE pfnCompare /*=NULL*/)
: m_pListCtrl(pListCtrl)
, m_pfnCompare(pfnCompare)
{
	ASSERT_VALID(m_pListCtrl);

	if (::IsWindow(m_pListCtrl->GetSafeHwnd()))
	{
		// Replace item data with pointer with new ITEMDATA struct.
		for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)
		{
			ITEMDATA* pData = new ITEMDATA;
			pData->dwData  = m_pListCtrl->GetItemData(i); // save current item data.
			pData->strItem = m_pListCtrl->GetItemText(i, nCol);
			m_pListCtrl->SetItemData(i, (DWORD_PTR)pData);
		}

		if (m_pfnCompare == NULL)
			m_pfnCompare = Compare;
	}
}

CXTPSortClass::~CXTPSortClass()
{
	ASSERT_VALID(m_pListCtrl);

	if (::IsWindow(m_pListCtrl->GetSafeHwnd()))
	{
		// Restore item data from ITEMDATA struct.
		for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)
		{
			ITEMDATA* pData = (ITEMDATA*)m_pListCtrl->GetItemData(i);
			if (pData)
			{
				m_pListCtrl->SetItemData(i, pData->dwData);
				SAFE_DELETE(pData);
			}
		}
	}
}

void CXTPSortClass::Sort(bool bAsc, int iType)
{
	ASSERT_VALID(m_pListCtrl);

	if (::IsWindow(m_pListCtrl->GetSafeHwnd()))
	{
		LPARAM lParamSort = iType;

		// m_dwData positive - ascending, negative - descending
		if (bAsc == FALSE)
			lParamSort *= -1;

		m_pListCtrl->SortItems(m_pfnCompare, (DWORD_PTR)lParamSort);
	}
}

int AFX_CDECL CXTPSortClass::SortInt(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder)
{
	CString cs1 = lpszItem1, cs2 = lpszItem2;

	if (RemoveNonNumeric(cs1) == 0)
		return 0;

	if (RemoveNonNumeric(cs2) == 0)
		return 0;

	if (cs1 == cs2)
		return 0; // two items are equal, no change.

	return (_ttol(cs1) - _ttol(cs2)) * sOrder;
}

int AFX_CDECL CXTPSortClass::SortString(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder)
{
	return (_tcsicmp(lpszItem1, lpszItem2) * sOrder);
}

int AFX_CDECL CXTPSortClass::SortDateTime(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder)
{
	COleDateTime t1;
	if (!t1.ParseDateTime(lpszItem1))
		return 0;

	COleDateTime t2;
	if (!t2.ParseDateTime(lpszItem2))
		return 0;

	if (t1 == t2)
		return 0; // two items are equal, no change.

	return (((t1 < t2)? -1: 1) * sOrder);
}

int AFX_CDECL CXTPSortClass::SortDecimal(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder)
{
	double d1 = 0, d2 = 0;

#ifdef _UNICODE
	char szText1[256];
	::WideCharToMultiByte(CP_ACP, 0, lpszItem1, -1, szText1,
		255, NULL, NULL);

	char szText2[256];
	::WideCharToMultiByte(CP_ACP, 0, lpszItem2, -1, szText2,
		255, NULL, NULL);

	d1 = atof(szText1);
	d2 = atof(szText2);
#else
	d1 = atof(lpszItem1);
	d2 = atof(lpszItem2);
#endif

	if (d1 == d2)
		return 0; // two items are equal, no change.

	return (((d1 < d2)? -1: 1) * sOrder);
}

int AFX_CDECL CXTPSortClass::RemoveNonNumeric(CString &strSource)
{
	TCHAR ch;
	CString temp;
	int nLen = strSource.GetLength();
	int i = 0;

	while (i < nLen)
	{
		ch = strSource.GetAt(i);
		if (_istdigit(ch))
			temp += ch;
		++i;
	}
	strSource = temp;
	return strSource.GetLength();
}
