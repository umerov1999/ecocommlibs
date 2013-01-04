// XTPReportRows.cpp : implementation of the CXTPReportRows class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"


#include "XTPReportDefines.h"
#include "XTPReportRow.h"
#include "XTPReportGroupRow.h"
#include "XTPReportRecord.h"
#include "XTPReportRecords.h"
#include "XTPReportRecordItem.h"
#include "XTPReportControl.h"
#include "XTPReportColumns.h"
#include "XTPReportColumn.h"
#include "XTPReportRows.h"
#include "ItemTypes/XTPReportRecordItemVariant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPReportScreenRows

CXTPReportScreenRows::CXTPReportScreenRows()
{
}

CXTPReportScreenRows::~CXTPReportScreenRows()
{
	Clear();
}

void CXTPReportScreenRows::Clear()
{
	// array cleanup
	for (int nRow = 0; nRow < GetSize(); nRow++)
	{
		CXTPReportRow* pRow = GetAt(nRow);
		pRow->InternalRelease();
	}

	RemoveAll();
}

CXTPReportRow* CXTPReportScreenRows::HitTest(CPoint pt) const
{
	for (int nRow=0; nRow<GetSize(); nRow++)
	{
		CXTPReportRow *pRow = GetAt(nRow);

		if (pRow->GetRect().PtInRect(pt))
		{
			return pRow;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportRows

CXTPReportRows::CXTPReportRows()
	: m_nFocusedRow(-1)
{
	m_pVirtualRow = 0;
	m_nVirtualRowsCount = 0;
	m_pScreenRows = NULL;
}

CXTPReportRows::~CXTPReportRows()
{
	Clear();
	SAFE_DELETE(m_pScreenRows);
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportRows member functions

CXTPReportScreenRows *CXTPReportRows::GetScreenRows()
{
	if (NULL == m_pScreenRows)
	{
		// Create on demand
		m_pScreenRows = new CXTPReportScreenRows();
	}
	return m_pScreenRows;
}

BOOL CXTPReportRows::Contains(const CXTPReportRow *pRow) const
{
	for (int nRow=0; nRow<m_arrRows.GetSize(); nRow++)
	{
		if (pRow == m_arrRows.GetAt(nRow))
		{
			return TRUE;
		}
	}

	return FALSE;
}

int CXTPReportRows::GetFocusedRowIndex() const
{
	return m_nFocusedRow;
}

BOOL CXTPReportRows::HasFocus() const
{
	return (-1 != m_nFocusedRow);
}

void CXTPReportRows::SetFocusedRowIndex(int nRow)
{
	m_nFocusedRow = nRow;
}


CXTPReportRow *CXTPReportRows::GetFocusedRow()
{
	CXTPReportRow *pFocusedRow = NULL;

	if (-1 != m_nFocusedRow)
	{
		pFocusedRow = GetAt(m_nFocusedRow);
	}

	return pFocusedRow;
}

void CXTPReportRows::Clear(BOOL bResetRow)
{
	// array cleanup
	for (int nRow = (int) m_arrRows.GetSize() - 1; nRow >= 0; nRow--)
	{
		CXTPReportRow* pRow = m_arrRows.GetAt(nRow);
		if (bResetRow)
		{
			pRow->SetVisible(FALSE);
			pRow->m_nIndex = -1;
		}
		pRow->InternalRelease();
	}

	m_arrRows.RemoveAll();

	if (m_pVirtualRow)
	{
		m_pVirtualRow->InternalRelease();
		m_pVirtualRow = 0;
	}

	GetScreenRows()->Clear();

	m_nFocusedRow = -1;
}

CXTPReportRow* CXTPReportRows::GetNext(CXTPReportRow* pRow, BOOL bSkipGroupFocus)
{
	if (pRow)
	{
		int index = pRow->GetIndex();

		while (index < GetCount() - 1)
		{
			index++;
			pRow = GetAt(index);

			if (!bSkipGroupFocus || !pRow->IsGroupRow() || !pRow->IsExpanded())
				return pRow;
		}
	}
	else if (GetCount() > 0)
	{
		return GetAt(0);
	}
	return pRow;
}

CXTPReportRow* CXTPReportRows::GetPrev(CXTPReportRow* pRow, BOOL bSkipGroupFocus)
{
	if (pRow)
	{
		int index = pRow->GetIndex();

		while (index > 0)
		{
			index--;
			pRow = GetAt(index);

			if (!bSkipGroupFocus || !pRow->IsGroupRow() || !pRow->IsExpanded())
				return pRow;
		}
	}
	else if (GetCount() > 0)
	{
		return GetAt(0);
	}
	return pRow;
}

void CXTPReportRows::InsertAt(int nIndex, CXTPReportRow* pRow)
{
	m_arrRows.InsertAt(nIndex, pRow);
}

int CXTPReportRows::Add(CXTPReportRow* pRow)
{
	int nIndex = (int)m_arrRows.Add(pRow);

	pRow->m_nChildIndex = nIndex;
	pRow->m_pParentRows = this;

	return nIndex;
}

void CXTPReportRows::RemoveAt(int nIndex)
{
	m_arrRows.GetAt(nIndex)->InternalRelease();
	m_arrRows.RemoveAt(nIndex);
}

int CXTPReportRows::RemoveRow(CXTPReportRow* pRow)
{
	for (int i = 0; i < (int) m_arrRows.GetSize(); i++)
	{
		if (m_arrRows.GetAt(i) == pRow)
		{
			RemoveAt(i);
			return i;
		}
	}
	return -1;
}

void CXTPReportRows::SetVirtualMode(CXTPReportRow* pRow, int nCount)
{
	ASSERT(m_pVirtualRow == NULL);

	m_pVirtualRow = pRow;
	m_nVirtualRowsCount = nCount;
}

int _cdecl CXTPReportRows::CompareRows(const CXTPReportRow** ppRow1, const CXTPReportRow** ppRow2)
{
	CXTPReportColumns* pColumns = (**ppRow1).GetControl()->GetColumns();

	CXTPReportRecord* pRecord1 = (**ppRow1).GetRecord();
	CXTPReportRecord* pRecord2 = (**ppRow2).GetRecord();

	ASSERT(pColumns);
	ASSERT(pRecord1 && pRecord2 || !pRecord1 && !pRecord2);

	if (pRecord1 == pRecord2 && pRecord1)
		return 0;

	if (!pRecord1 && !pRecord2)
	{
		// compare groupRows by its first not group child rows
		if ((*ppRow1)->HasChildren() && (*ppRow2)->HasChildren())
		{
			CXTPReportRow* pRow1a = (CXTPReportRow*)(*ppRow1);
			CXTPReportRow* pRow2a = (CXTPReportRow*)(*ppRow2);

			if (pRow1a->GetChilds() && pRow1a->GetChilds()->GetCount() &&
				pRow2a->GetChilds() && pRow2a->GetChilds()->GetCount())
			{
				pRow1a = pRow1a->GetChilds()->GetAt(0);
				pRow2a = pRow2a->GetChilds()->GetAt(0);

				return CompareRows((const CXTPReportRow**)&pRow1a, (const CXTPReportRow**)&pRow2a);
			}
		}
	}

	if (!pRecord1 || !pRecord2)
	{
		ASSERT(FALSE);
		return 0;
	}

	for (int nGroupOrder = 0; nGroupOrder < pColumns->GetGroupsOrder()->GetCount(); nGroupOrder++)
	{
		CXTPReportColumn* pColumn = pColumns->GetGroupsOrder()->GetAt(nGroupOrder);
		if (!pColumn->m_bAutoSortWhenGrouped)
			continue;

		BOOL bIncreasing = pColumn->m_bSortIncreasing;

		CXTPReportRecordItem* pItem1 = pRecord1->GetItem(pColumn);
		CXTPReportRecordItem* pItem2 = pRecord2->GetItem(pColumn);

		if (!pItem1 || !pItem2)
			continue;

		int nCompareResult = pItem1->CompareGroupCaption(pColumn, pItem2);

		if (nCompareResult != 0)
			return nCompareResult * (bIncreasing ? 1 : -1);
	}


	for (int nSortOrder = 0; nSortOrder < pColumns->GetSortOrder()->GetCount(); nSortOrder++)
	{
		CXTPReportColumn* pColumn = pColumns->GetSortOrder()->GetAt(nSortOrder);
		BOOL bIncreasing = pColumn->IsSortedIncreasing();

		CXTPReportRecordItem* pItem1 = pRecord1->GetItem(pColumn);
		CXTPReportRecordItem* pItem2 = pRecord2->GetItem(pColumn);

		if (!pItem1 || !pItem2)
			continue;

		int nCompareResult = pItem1->Compare(pColumn, pItem2);

		if (nCompareResult != 0)
			return nCompareResult * (bIncreasing ? 1 : -1);
	}

	return pRecord1->GetIndex() > pRecord2->GetIndex() ? 1 : -1;
}

int _cdecl CXTPReportRows::CompareRows2(const CXTPReportRow** ppRow1, const CXTPReportRow** ppRow2)
{
	CXTPReportColumns* pColumns = (**ppRow1).GetControl()->GetColumns();

	CXTPReportRecord* pRecord1 = (**ppRow1).GetRecord();
	CXTPReportRecord* pRecord2 = (**ppRow2).GetRecord();

	ASSERT(pColumns);
	ASSERT(pRecord1 && pRecord2 || !pRecord1 && !pRecord2);

	if (pRecord1 == pRecord2 && pRecord1)
		return 0;

	if (!pRecord1 && !pRecord2)
	{
		// compare groupRows by its first not group child rows
		if ((*ppRow1)->HasChildren() && (*ppRow2)->HasChildren())
		{
			CXTPReportRow* pRow1a = (CXTPReportRow*)(*ppRow1);
			CXTPReportRow* pRow2a = (CXTPReportRow*)(*ppRow2);

			if (pRow1a->GetChilds() && pRow1a->GetChilds()->GetCount() &&
				pRow2a->GetChilds() && pRow2a->GetChilds()->GetCount())
			{
				pRow1a = pRow1a->GetChilds()->GetAt(0);
				pRow2a = pRow2a->GetChilds()->GetAt(0);

				return CompareRows((const CXTPReportRow**)&pRow1a, (const CXTPReportRow**)&pRow2a);
			}
		}
	}

	if (!pRecord1 || !pRecord2)
	{
		ASSERT(FALSE);
		return 0;
	}

	for (int nGroupOrder = 0; nGroupOrder < pColumns->GetGroupsOrder()->GetCount(); nGroupOrder++)
	{
		CXTPReportColumn* pColumn = pColumns->GetGroupsOrder()->GetAt(nGroupOrder);
		if (!pColumn->m_bAutoSortWhenGrouped)
			continue;

		BOOL bIncreasing = pColumn->m_bSortIncreasing;

		CXTPReportRecordItem* pItem1 = pRecord1->GetItem(pColumn);
		CXTPReportRecordItem* pItem2 = pRecord2->GetItem(pColumn);

		if (!pItem1 || !pItem2)
			continue;

		int nCompareResult = pItem1->CompareGroupCaption(pColumn, pItem2);

		if (nCompareResult != 0)
		{
			if (nGroupOrder > 0)
				nCompareResult = bIncreasing ? -1 : 1;
			return nCompareResult * (bIncreasing ? 1 : -1);
		}
	}

	for (int nSortOrder = 0; nSortOrder < pColumns->GetSortOrder()->GetCount(); nSortOrder++)
	{
		CXTPReportColumn* pColumn = pColumns->GetSortOrder()->GetAt(nSortOrder);
		BOOL bIncreasing = pColumn->IsSortedIncreasing();

		CXTPReportRecordItem* pItem1 = pRecord1->GetItem(pColumn);
		CXTPReportRecordItem* pItem2 = pRecord2->GetItem(pColumn);

		if (!pItem1 || !pItem2)
			continue;

		int nCompareResult = pItem1->Compare(pColumn, pItem2);

		if (nCompareResult != 0)
			return nCompareResult * (bIncreasing ? 1 : -1);
	}

	if (pRecord1->GetIndex() == pRecord2->GetIndex())
	{
		ASSERT(FALSE);
		return 0;
	}
	return pRecord1->GetIndex() < pRecord2->GetIndex() ? -1 : 1;
}

void CXTPReportRows::SortEx(XTPReportRowsCompareFunc pCompareFunc)
{
	typedef int (_cdecl *GENERICCOMPAREFUNC)(const void *, const void*);

	qsort(m_arrRows.GetData(), (size_t) m_arrRows.GetSize(), sizeof(CXTPReportRow*), (GENERICCOMPAREFUNC)pCompareFunc);
}

void CXTPReportRows::Sort()
{
	SortEx(CompareRows);
}

int CXTPReportRows::GetCount() const
{
	if (m_pVirtualRow)
		return m_nVirtualRowsCount;

	return (int) m_arrRows.GetSize();
}

CXTPReportRow* CXTPReportRows::GetAt(int nIndex) const
{
	if (m_pVirtualRow)
	{
		m_pVirtualRow->m_nIndex = nIndex;
		return m_pVirtualRow;
	}

	return (nIndex >= 0) && (nIndex < GetCount()) ? m_arrRows.GetAt(nIndex) : NULL;
}

CXTPReportRow* CXTPReportRows::Find(CXTPReportRecord *pRecord, BOOL bRecursive)
{
	int nSize = (int)m_arrRows.GetSize();

	for (int nRow=0; nRow<nSize; nRow++)
	{
		CXTPReportRow *pRow = m_arrRows.GetAt(nRow);

		if (pRow->GetRecord() == pRecord)
		{
			return pRow;
		}

		if (pRow->HasChildren() && bRecursive)
		{
			pRow = pRow->GetChilds()->Find(pRecord, TRUE);

			if (NULL != pRow)
			{
				return pRow;
			}
		}
	}

	return NULL;
}

CXTPReportRow* CXTPReportRows::Find(CXTPReportRecord *pRecord)
{
	return Find(pRecord, FALSE);
}

CXTPReportRow* CXTPReportRows::FindInTree(CXTPReportRecord *pRecord)
{
	return Find(pRecord, TRUE);
}

CXTPReportRow* CXTPReportRows::FindInsertionPos(CXTPReportRow* pRow, BOOL& bInsertAfter)
{
	CXTPReportRow* pInsertionRowPos = NULL;
	bInsertAfter = FALSE;

	for (int i = 0; i < (int) m_arrRows.GetSize(); i++)
	{
		CXTPReportRow* pCurRow = m_arrRows.GetAt(i);
		if (pCurRow->IsGroupRow())
		{
			// compare group caption
			CXTPReportRecord* pRecord = pRow->GetRecord();
			CXTPReportColumns* pColumns = pRow->GetControl()->GetColumns();
			if (!(pRecord && pColumns))
				continue;
			CXTPReportColumn* pColumn = pColumns->GetGroupsOrder()->GetAt(pCurRow->GetGroupLevel());
			CXTPReportRecordItem* pItem = pRecord->GetItem(pColumn);
			COleVariant varGroup(pItem ? (LPCTSTR)pItem->GetGroupCaption(pColumn) : _T(""));
			COleVariant varGroupRowCaption((LPCTSTR)((CXTPReportGroupRow*)pCurRow)->GetCaption());
			ULONG dwFlags = pRecord->GetRecords()->IsCaseSensitive() ? 0 : NORM_IGNORECASE;
			LCID lcidnSortLocale = CXTPReportRecordItemVariant::m_nSortLocale;
			if (lcidnSortLocale == LOCALE_USER_DEFAULT)
			{
				lcidnSortLocale = CXTPReportControlLocale::GetActiveLCID();
			}
			int nCompareResult = VarCmp(varGroup, varGroupRowCaption, lcidnSortLocale, dwFlags) - VARCMP_EQ;
			if (pColumn->m_bSortIncreasing && nCompareResult > 0 || !pColumn->m_bSortIncreasing && nCompareResult < 0)
				continue;
			// find in children
			if (!pCurRow->GetChilds())
				continue;
			if (nCompareResult == 0)
				pInsertionRowPos = pCurRow->GetChilds()->FindInsertionPos(pRow, bInsertAfter);
			if (!pInsertionRowPos)
			{
				if (nCompareResult != 0)
					pInsertionRowPos = pCurRow;
				else
				{
					pInsertionRowPos = pCurRow->GetChilds()->GetAt(pCurRow->GetChilds()->GetCount() - 1);
					bInsertAfter = TRUE;
				}
			}
			break;
		}
		if (CompareRows2((const CXTPReportRow**)&pRow, (const CXTPReportRow**)&pCurRow) <= 0)
		{
			pInsertionRowPos = pCurRow;
			break;
		}
	}

	return pInsertionRowPos;
}

void CXTPReportRows::RefreshChildIndices(BOOL bRunInChildren)
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPReportRow* pRow = GetAt(nIndex);
		pRow->m_nChildIndex = nIndex;
		ASSERT(pRow->m_pParentRows == this);
		if (bRunInChildren && pRow->HasChildren() && pRow->GetChilds())
			pRow->GetChilds()->RefreshChildIndices(bRunInChildren);
	}
}

void CXTPReportRows::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy)
{
	int nSize = GetCount();
	if (nNewSize < nSize)
	{
		for (int i = (int)nNewSize; i < nSize; i++)
		{
			CXTPReportRow* pRow = GetAt(i);
			if (pRow)
				pRow->InternalRelease();
		}
	}

	m_arrRows.SetSize(nNewSize, nGrowBy);
}

void CXTPReportRows::ReserveSize(INT_PTR nNewSize, INT_PTR nGrowBy)
{
	m_arrRows.ReserveSize(nNewSize, nGrowBy);
}

void CXTPReportRows::SetAt(INT_PTR nIndex, CXTPReportRow* pRow)
{
	ASSERT(pRow);
	ASSERT(nIndex >= 0 && nIndex < GetCount());

	if (!pRow || nIndex < 0 || nIndex >= GetCount())
		return;

	CXTPReportRow* pRow_prev = GetAt((int)nIndex);
	if (pRow_prev)
		pRow_prev->InternalRelease();

	pRow->m_nChildIndex = (int)nIndex;
	pRow->m_pParentRows = this;

	m_arrRows.SetAt(nIndex, pRow);
}

CXTPReportRecordItem* CXTPReportRows::FindRecordItemByRows(int nStartIndex, int nEndIndex,
												int nStartColumn, int nEndColumn,
												int nRecordIndex, int nItem,
												LPCTSTR pcszText, int nFlags)
{
	CString sFind(pcszText);
	BOOL bExactStart = (nFlags & xtpReportTextSearchExactStart) > 0;
	if (bExactStart)
		nFlags -= xtpReportTextSearchExactStart;
	// NO xtpReportTextSearchBackward case
	if ((nFlags & xtpReportTextSearchBackward) > 0)
		nFlags -= xtpReportTextSearchBackward;
	// validate parameters
	int N = GetCount();
	if (N < 1)
		return NULL;
	nStartIndex = max(nStartIndex, 0);
	nStartIndex = min(nStartIndex, N - 1);

	nEndIndex = max(nStartIndex, nEndIndex);
	nEndIndex = min(nEndIndex, N - 1);

	nStartColumn = max(nStartColumn, 0);
	nEndColumn = max(nEndColumn, 0);

	nEndColumn = max(nStartColumn, nEndColumn);

	nRecordIndex = max(nRecordIndex, 0);
	nRecordIndex = min(nRecordIndex, N - 1);

	nItem = max(nItem, nStartColumn);
	nItem = min(nEndColumn, nItem);

	for (int i = nStartIndex; i <= nEndIndex; i++)
	{
		if (i < nRecordIndex) continue;

		CXTPReportRow* pTryRow = GetAt(i);
		CXTPReportRecord* pTryRecord = pTryRow->GetRecord();
		if (pTryRecord)
		{
			for (int j = nStartColumn; j <= nEndColumn; j++)
			{
				if (i == nRecordIndex && j < nItem) continue;

				CXTPReportRecordItem* pTryItem = pTryRecord->GetItem(j);
				if (pTryItem)
				{
					CString strCaption = pTryItem->GetCaption(NULL);
					if (strCaption.IsEmpty()) continue;
					int k = strCaption.Find(sFind);
					//if (k > -1)
					if ((bExactStart && k == 0) || (!bExactStart && k > -1))
					{
						if (nFlags == (xtpReportTextSearchExactPhrase |
							xtpReportTextSearchMatchCase))
						{
							if (strCaption == sFind)
								return pTryItem;
						}
						else if (nFlags == xtpReportTextSearchExactPhrase)
						{
							CString sCAP(strCaption);
							sCAP.MakeUpper();
							CString sFIND(sFind);
							sFIND.MakeUpper();
							if (sCAP == sFIND)
								return pTryItem;
						}
						else if (nFlags == xtpReportTextSearchMatchCase)
						{
							if (strCaption.Mid(k) == sFind)
								return pTryItem;
						}
						else if (nFlags == 0)
						{
							return pTryItem;
						}
					}
					else if (!bExactStart)
					{
						CString sCAP(strCaption);
						sCAP.MakeUpper();
						CString sFIND(sFind);
						sFIND.MakeUpper();

						k = sCAP.Find(sFIND);
						if (k > -1)
						{
							if (nFlags == xtpReportTextSearchExactPhrase)
							{
								if (sCAP == sFIND)
									return pTryItem;
							}
							else if (nFlags == 0)
							{
								return pTryItem;
							}
						}
					}
				}
			}
		}
	}

	return NULL;
}



