// XTPReportColumns.cpp : implementation of the CXTPReportColumns class.
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPReportDefines.h"
#include "XTPReportControl.h"
#include "XTPReportHeader.h"
#include "XTPReportColumn.h"
#include "XTPReportColumns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPReportColumns

CXTPReportColumns::CXTPReportColumns(CXTPReportControl* pControl)
	: m_pControl(pControl)
{
	m_pGroupsOrder = new CXTPReportColumnOrder(this);
	m_pSortOrder = new CXTPReportColumnOrder(this);

	m_pTreeColumn = NULL;

}

CXTPReportColumns::~CXTPReportColumns()
{
	Clear();

	if (m_pGroupsOrder)
		m_pGroupsOrder->InternalRelease();
	if (m_pSortOrder)
		m_pSortOrder->InternalRelease();

}

int CXTPReportColumns::GetAtOffset(int nOffset, BOOL bFrozen) const
{
	int nIndex = -1;
	int nLeft  =  0;

	for (int nColumn=0; nColumn<GetCount(); nColumn++)
	{
		const CXTPReportColumn *pColumn = GetAt(nColumn);

		if (pColumn->IsVisible() && (!pColumn->IsFrozen() || bFrozen))
		{
			if (nOffset >= nLeft &&
				 nOffset <= nLeft+pColumn->GetWidth())
			{
				nIndex = nColumn;
			}

			nLeft += pColumn->GetWidth();
		}
	}

	return nIndex;
}

int CXTPReportColumns::GetOffset(int nIndex) const
{
	int nOffset = 0;

	for (int nColumn=0; nColumn<GetCount(); nColumn++)
	{
		const CXTPReportColumn *pColumn = GetAt(nColumn);

		if (pColumn->IsVisible() && !pColumn->IsFrozen() && (nIndex > 0))
		{
			nIndex  -= 1;
			nOffset += pColumn->GetWidth();
		}
	}

	return nOffset;
}


void CXTPReportColumns::Clear()
{
	// array cleanup
	for (int nColumn = GetCount() - 1; nColumn >= 0; nColumn--)
	{
		CXTPReportColumn* pColumn = m_arrColumns.GetAt(nColumn);
		if (pColumn)
			pColumn->InternalRelease();
	}
	m_arrColumns.RemoveAll();

	m_pSortOrder->Clear();
	m_pGroupsOrder->Clear();

	// clear variables which could be references to those values
	if (m_pControl && (m_pControl->GetColumns() == this))
		m_pControl->SetFocusedColumn(NULL);
}

int CXTPReportColumnOrder::GetCount() const
{
	return (int) m_arrColumns.GetSize();
}


void CXTPReportColumns::Add(CXTPReportColumn* pColumn)
{
	pColumn->m_pColumns = this;
	m_arrColumns.Add(pColumn);

	GetReportHeader()->OnColumnsChanged(xtpReportColumnOrderChanged | xtpReportColumnAdded, pColumn);
}

CXTPReportHeader* CXTPReportColumns::GetReportHeader() const
{
	return m_pControl->GetReportHeader();
}


void CXTPReportColumns::Remove(CXTPReportColumn* pColumn)
{
	m_pGroupsOrder->Remove(pColumn);
	m_pSortOrder->Remove(pColumn);

	int nIndex = IndexOf(pColumn);
	if (nIndex != -1)
	{
		m_arrColumns.RemoveAt(nIndex);
		pColumn->InternalRelease();

		GetReportHeader()->OnColumnsChanged(xtpReportColumnOrderChanged | xtpReportColumnRemoved, pColumn);
	}
}

int CXTPReportColumns::IndexOf(const CXTPReportColumn* pColumn) const
{
	// array cleanup
	for (int nColumn = GetCount() - 1; nColumn >= 0; nColumn--)
	{
		if (m_arrColumns.GetAt(nColumn) == pColumn)
			return nColumn;
	}
	return -1;
}

void CXTPReportColumns::ResetSortOrder()
{
	m_pSortOrder->Clear();
}

void CXTPReportColumns::SetSortColumn(CXTPReportColumn* pColumn, BOOL bIncreasing)
{
	ResetSortOrder();
	m_pSortOrder->Add(pColumn, bIncreasing);
}

int CXTPReportColumns::ChangeColumnOrder(int nNewOrder, int nItemIndex)
{
	if (nNewOrder < 0 || nItemIndex < 0)
		return -1;

	CXTPReportColumn* pColumn = GetAt(nItemIndex);

	if (pColumn)
	{
		if (nNewOrder == nItemIndex)
			return nNewOrder;

		if (nNewOrder > nItemIndex)
			nNewOrder--;

		m_arrColumns.RemoveAt(nItemIndex);
		m_arrColumns.InsertAt(nNewOrder, pColumn);
	}

	return nNewOrder;
}

int CXTPReportColumns::GetVisibleColumnsCount() const
{
	int nCount = 0;

	for (int nColumn=0; nColumn<GetCount(); nColumn++)
	{
		const CXTPReportColumn *pColumn = GetAt(nColumn);

		if (NULL != pColumn && pColumn->IsVisible())
		{
			nCount++;
		}
	}

	return nCount;
}

void CXTPReportColumns::GetVisibleColumns(CXTPReportColumns& arrColumns) const
{
	int nCount = GetCount();
	for (int nColumn = 0; nColumn < nCount; nColumn++)
	{
		CXTPReportColumn* pColumn = GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			arrColumns.m_arrColumns.Add(pColumn);
			pColumn->InternalAddRef();
		}
	}
}

CXTPReportColumn* CXTPReportColumns::Find(int nItemIndex) const
{
	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		CXTPReportColumn* pColumn = GetAt(nColumn);
		if (pColumn->GetItemIndex() == nItemIndex)
			return pColumn;
	}
	return NULL;
}

CXTPReportColumn* CXTPReportColumns::Find(const CString& strInternalName) const
{
	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		CXTPReportColumn* pColumn = GetAt(nColumn);
		if (pColumn->GetInternalName() == strInternalName)
			return pColumn;
	}
	return NULL;
}

void CXTPReportColumns::InsertSortColumn(CXTPReportColumn* pColumn)
{
	if (m_pSortOrder->IndexOf(pColumn) == -1)
		m_pSortOrder->Add(pColumn);
}

CXTPReportColumn* CXTPReportColumns::GetVisibleAt(int nIndex) const
{
	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		CXTPReportColumn* pColumn = GetAt(nColumn);
		if (!pColumn->IsVisible())
			continue;

		if (nIndex == 0)
			return pColumn;

		nIndex--;
	}
	return NULL;
}

CXTPReportColumn* CXTPReportColumns::GetFirstVisibleColumn() const
{
	for (int nColumn = 0; nColumn < GetCount(); nColumn++)
	{
		CXTPReportColumn* pColumn = GetAt(nColumn);
		if (pColumn->IsVisible())
			return pColumn;
	}
	return NULL;
}

CXTPReportColumn* CXTPReportColumns::GetLastVisibleColumn() const
{
	for (int nColumn = GetCount() - 1; nColumn >= 0; nColumn--)
	{
		CXTPReportColumn* pColumn = GetAt(nColumn);
		if (pColumn->IsVisible())
			return pColumn;
	}
	return NULL;
}


void CXTPReportColumns::DoPropExchange(CXTPPropExchange* pPX)
{
	int nItemIndex;
	CString strInternalName;

	if (pPX->IsStoring())
	{
		int nCount = GetCount();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Column")));
		POSITION pos = pEnumerator->GetPosition(nCount, FALSE);

		for (int nColumn = 0; nColumn < nCount; nColumn++)
		{
			CXTPReportColumn* pColumn = GetAt(nColumn);
			CXTPPropExchangeSection secColumn(pEnumerator->GetNext(pos));

			nItemIndex = pColumn->GetItemIndex();
			strInternalName = pColumn->GetInternalName();

			PX_Int(&secColumn, _T("ItemIndex"), nItemIndex);
			PX_String(&secColumn, _T("InternalName"), strInternalName);

			pColumn->DoPropExchange(&secColumn);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Column")));
		POSITION pos = pEnumerator->GetPosition(0, FALSE);

		CXTPReportColumn tmpColumn(0, _T(""), 0);
		int i = 0;
		while (pos)
		{
			CXTPPropExchangeSection secColumn(pEnumerator->GetNext(pos));

			CXTPReportColumn* pColumn = NULL;
			PX_Int(&secColumn, _T("ItemIndex"), nItemIndex, -1);

			if (pPX->GetSchema() > _XTP_SCHEMA_1100)
			{
				PX_String(&secColumn, _T("InternalName"), strInternalName);

				if (!strInternalName.IsEmpty())
				{
					pColumn = Find(strInternalName);

					if (!pColumn && pPX->GetSchema() < _XTP_SCHEMA_1500) // before 15.0 release internal name was equal to caption
					{
						for (int nColumn = 0; nColumn < GetCount(); nColumn++)
						{
							CXTPReportColumn* p = GetAt(nColumn);
							if (p->GetCaption() == strInternalName)
							{
								pColumn = p;
								break;
							}
						}
					}

					// column data is exists but column is not in the collection.
					if (!pColumn)
					{
						// just read data to skeep (to be safe for array serialization)
						tmpColumn.DoPropExchange(&secColumn);
						continue;
					}
				}
			}

			if (!pColumn)
				pColumn = Find(nItemIndex);

			if (!pColumn)
				AfxThrowArchiveException(CArchiveException::badIndex);

			pColumn->DoPropExchange(&secColumn);
			ChangeColumnOrder(i, IndexOf(pColumn));
			i++;
		}
	}

	CXTPPropExchangeSection secGroupsOrder(pPX->GetSection(_T("GroupsOrder")));
	m_pGroupsOrder->DoPropExchange(&secGroupsOrder);

	CXTPPropExchangeSection secSortOrder(pPX->GetSection(_T("SortOrder")));
	m_pSortOrder->DoPropExchange(&secSortOrder);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPReportColumnOrder

CXTPReportColumnOrder::CXTPReportColumnOrder(CXTPReportColumns* pColumns) :
	m_pColumns(pColumns)
{
}


CXTPReportColumn* CXTPReportColumnOrder::GetAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
		return m_arrColumns.GetAt(nIndex);
	else
		return NULL;
}

int CXTPReportColumnOrder::InsertAt(int nIndex, CXTPReportColumn* pColumn)
{
	if (nIndex < 0)
		return -1;

	if (nIndex >= GetCount())
		nIndex = GetCount();

	int nPrevIndex = IndexOf(pColumn);

	if (nPrevIndex != -1)
	{
		if (nPrevIndex == nIndex)
			return nIndex;

		if (nIndex > nPrevIndex)
			nIndex--;

		if (nIndex == nPrevIndex)
			return nIndex;

		// change order
		m_arrColumns.RemoveAt(nPrevIndex);

	}
	m_arrColumns.InsertAt(nIndex, pColumn);
	return nIndex;
}

int CXTPReportColumnOrder::Add(CXTPReportColumn* pColumn, BOOL bSortIncreasing)
{
	pColumn->m_bSortIncreasing = bSortIncreasing;
	return (int) m_arrColumns.Add(pColumn);
}

int CXTPReportColumnOrder::Add(CXTPReportColumn* pColumn)
{
	return (int) m_arrColumns.Add(pColumn);
}

void CXTPReportColumnOrder::Clear()
{
	m_arrColumns.RemoveAll();
}

int CXTPReportColumnOrder::IndexOf(const CXTPReportColumn* pColumn)
{
	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (GetAt(i) == pColumn)
			return i;
	}
	return -1;
}

void CXTPReportColumnOrder::RemoveAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
		m_arrColumns.RemoveAt(nIndex);
}

void CXTPReportColumnOrder::Remove(CXTPReportColumn* pColumn)
{
	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (GetAt(i) == pColumn)
		{
			m_arrColumns.RemoveAt(i);
			break;
		}
	}
}

void CXTPReportColumnOrder::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsStoring())
	{
		int nCount = GetCount();
		PX_Int(pPX, _T("Count"), nCount, 0);

		for (int i = 0; i < nCount; i++)
		{
			CXTPReportColumn* pColumn = GetAt(i);
			if (pColumn)
			{
				int nItemIndex = pColumn->GetItemIndex();
				CString strInternalName = pColumn->GetInternalName();
				CString strParamName;

				strParamName.Format(_T("Column%i"), i);
				PX_Int(pPX, strParamName, nItemIndex, 0);

				strParamName.Format(_T("InternalName%i"), i);
				PX_String(pPX, strParamName, strInternalName);
			}
		}
	}
	else
	{
		Clear();

		int nCount = 0;
		PX_Int(pPX, _T("Count"), nCount, 0);

		for (int i = 0; i < nCount; i++)
		{
			int nItemIndex = 0;
			CString strParamName;
			strParamName.Format(_T("Column%i"), i);
			PX_Int(pPX, strParamName, nItemIndex, 0);

			CXTPReportColumn* pColumn = NULL;
			if (pPX->GetSchema() > _XTP_SCHEMA_1100)
			{
				strParamName.Format(_T("InternalName%i"), i);
				CString strInternalName;
				PX_String(pPX, strParamName, strInternalName);

				if (!strInternalName.IsEmpty())
					pColumn = m_pColumns->Find(strInternalName);
			}
			if (!pColumn)
				pColumn = m_pColumns->Find(nItemIndex);

			if (pColumn)
				Add(pColumn);
		}
	}
}






