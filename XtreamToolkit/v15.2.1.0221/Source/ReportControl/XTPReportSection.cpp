// XTPReportSection.cpp : implementation of the CXTPReportSection class.
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

#include "StdAfx.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPReportDefines.h"
#include "XTPReportRow.h"
#include "XTPReportGroupRow.h"
#include "XTPReportRecord.h"
#include "XTPReportRecords.h"
#include "XTPReportRecordItem.h"
//#include "ItemTypes/XTPReportRecordItemText.h"
#include "XTPReportControl.h"
#include "XTPReportColumns.h"
#include "XTPReportColumn.h"
#include "XTPReportRows.h"
#include "XTPReportSelectedRows.h"
#include "XTPReportSection.h"
#include "XTPReportSections.h"
#include "XTPReportPaintManager.h"
#include "XTPReportBorder.h"
#include "XTPReportThickness.h"

#define XTP_REPORT_DEBUG()          (0)
#define XTP_REPORT_PERFORMANCE()    (0)



CXTPReportSection::CXTPReportSection(CXTPReportControl *pControl, XTPReportRowType rowType)
	: m_bAllowAccess    (TRUE)
	, m_bAllowEdit      (FALSE)
	, m_bAllowGroup     (FALSE)
	, m_bAllowSort      (FALSE)
	, m_bSelectionEnable(TRUE)
	, m_bVisible        (FALSE)

	, m_rcSection       (0,0,0,0)
	, m_heightMode      (xtpReportSectionHeightModeAuto)
	, m_dividerPosition (xtpReportSectionDividerPositionBottom)
	, m_dividerStyle    (xtpReportFixedRowsDividerNone)
	, m_nHeight         (0)

	// Markup
	, m_pMarkupContext(NULL)
	, m_pBorder       (NULL)

	, m_pControl(pControl)
	, m_pRecords(NULL)
	, m_pRows   (NULL)
	, m_pTree   (NULL)

	, m_nScrollIndexV (0)
	, m_nScrollOffsetV(0)

	, m_nPopulatedRecordsCount(0)
	, m_rowType (rowType)
{
	m_pBorder  = new CXTPReportBorder();

	m_pRecords = new CXTPHeapObjectT<CXTPReportRecords, CXTPReportAllocatorDefault>;
	m_pRows    = new CXTPHeapObjectT<CXTPReportRows,    CXTPReportAllocatorDefault>;
	m_pTree    = new CXTPHeapObjectT<CXTPReportRows,    CXTPReportAllocatorDefault>;

	m_pRecords->m_pControl = pControl;

	m_clrDivider.SetStandardValue(COLOR_BTNFACE);
}


CXTPReportSection::~CXTPReportSection()
{
	m_pRecords->SetMarkupContext(NULL);

	SAFE_DELETE(m_pBorder);

	CMDTARGET_RELEASE(m_pRecords);
	CMDTARGET_RELEASE(m_pRows);
	CMDTARGET_RELEASE(m_pTree);
}

void CXTPReportSection::SetMarkupContext(CXTPMarkupContext *pMarkupContext)
{
	m_pMarkupContext = pMarkupContext;
	GetRecords()->SetMarkupContext(pMarkupContext);
}


BOOL CXTPReportSection::IsVirtualMode() const
{
	return m_pRecords ? m_pRecords->IsVirtualMode() : FALSE;
}


CXTPReportRecord* CXTPReportSection::AddRecord(CXTPReportRecord *pRecord)
{
	ASSERT(NULL != pRecord);

	if (NULL != pRecord)
	{
		m_pRecords->Add(pRecord);
	}

	return pRecord;
}

void CXTPReportSection::ResetContent()
{
	if (NULL != m_pRecords)
	{
		m_pRecords->RemoveAll();
	}

	if (NULL != m_pRows)
	{
		m_pRows->Clear();
	}

	if (NULL != m_pTree)
	{
		m_pTree->Clear();
	}
}


void CXTPReportSection::Populate(BOOL bUpdate)
{
	if (bUpdate)
	{
		m_pControl->EditItem(NULL);
		m_pControl->BeginUpdate();
	}

	// Save current focused row
	CXTPReportRecord *pFocusedRecord = m_pRows->GetFocusedRow()
		? m_pRows->GetFocusedRow()->GetRecord()
		: NULL;

	m_pRows->Clear();
	m_pTree->Clear();

	if (IsVirtualMode())
	{
		CXTPReportRow *pRow = m_pControl->CreateRow(); // new CXTPReportRow_Batch();
		pRow->InitRow(m_pControl, this, GetRecords()->m_pVirtualRecord);
		pRow->SetVisible(TRUE);

		m_pRows->SetVirtualMode(pRow, GetRecords()->GetCount());
		m_nPopulatedRecordsCount = GetRecords()->GetCount();
	}
	else
	{
		BuildTree(m_pTree, NULL, m_pRecords);
		m_nPopulatedRecordsCount = m_pTree->GetCount();

		SortRows(m_pTree);
		int nGroupRowsCount = 0;

		CXTPReportColumns *pColumns = m_pControl->m_pColumns;

		if (IsAllowGroup() && pColumns->GetGroupsOrder()->GetCount() > 0)
		{
			CXTPReportRows *pGroupTree = new CXTPHeapObjectT<CXTPReportRows, CXTPReportAllocatorDefault>;

			int nReserve = m_pRecords->GetCount() / (pColumns->GetGroupsOrder()->GetCount() + 1);
			nReserve = max(nReserve, 300);
			pGroupTree->ReserveSize(nReserve);

			CXTPReportGroupRow* pLastGroup = NULL;

			for (int nPlainRow = 0; nPlainRow < m_pTree->GetCount(); nPlainRow++)
			{
				CXTPReportRow      *pRow = m_pTree->GetAt(nPlainRow);
				CXTPReportGroupRow *pGroupToAdd = NULL;

				for (int nColumn = 0; nColumn < pColumns->GetGroupsOrder()->GetCount(); nColumn++)
				{
					CXTPReportColumn* pColumn = pColumns->GetGroupsOrder()->GetAt(nColumn);
					CXTPReportRecordItem* pItem = pRow->GetRecord()->GetItem(pColumn);
					CString strGroup = pItem ? pItem->GetGroupCaption(pColumn) : _T("");

					if (pLastGroup && GetRecords()->Compare(pLastGroup->GetCaption(), strGroup) == 0)
					{
						pGroupToAdd = pLastGroup;

						if (pGroupToAdd->HasChildren())
							pLastGroup = (CXTPReportGroupRow*)pGroupToAdd->GetChilds()->GetAt(pGroupToAdd->GetChilds()->GetCount() - 1);
					}
					else
					{
						CXTPReportGroupRow* pGroup = m_pControl->CreateGroupRow(); // new CXTPReportGroupRow_Batch();
						nGroupRowsCount++;

						pGroup->InitRow(m_pControl, this, NULL);
						pGroup->SetCaption(strGroup);

						if (pGroupToAdd)
							pGroupToAdd->AddChild(pGroup);
						else
							pGroupTree->Add(pGroup);

						pGroupToAdd = pGroup;
						pLastGroup = NULL;
					}

				}
				if (pGroupToAdd)
					pGroupToAdd->AddChild(pRow);
				pRow->InternalAddRef();
				pLastGroup = (CXTPReportGroupRow*)pGroupTree->GetAt(pGroupTree->GetCount() - 1);
			}

			m_pTree->InternalRelease();
			m_pTree = pGroupTree;
		}

		GetRows()->ReserveSize(m_pRecords->GetCount() + nGroupRowsCount + 10);

		for (int nGroupRow = 0; nGroupRow < m_pTree->GetCount(); nGroupRow++)
		{
			CXTPReportRow *pRow = m_pTree->GetAt(nGroupRow);
			InsertRow(GetRows()->GetCount(), pRow);
			pRow->m_nChildIndex = nGroupRow;
			ASSERT(pRow->m_pParentRows == m_pTree);
		}

		for (int nRow=0; nRow<m_pRows->GetCount(); nRow++)
		{
			CXTPReportRow *pRow = m_pRows->GetAt(nRow);

			// Update indexes
			if (NULL != pRow)
			{
				pRow->SetIndex(nRow);
				pRow->m_pSection = this;
				pRow->SetVisible(TRUE);
			}

			// Restore focus
			if (pFocusedRecord && pRow->GetRecord() == pFocusedRecord)
			{
				m_pRows->m_nFocusedRow = pRow->GetIndex();
				if (IsSelectionEnabled() && m_pControl->IsInitialSelectionEnabled())
				{
					//m_pSelectedRows->Select(pRow);
					CXTPReportRow* pFirst = pRow;
					if (pFirst)
					{
						if (!m_pControl->IsSkipGroupsFocusEnabled() || !pFirst->IsGroupRow())
						{
							m_pControl->m_pSelectedRows->Select(pFirst);
						}
						else
						{
							while (pFirst->IsGroupRow())
							{
								if (pFirst->HasChildren())
								{
									pFirst = pFirst->GetChilds()->GetAt(0);
								}
							}
							m_pControl->m_pSelectedRows->Select(pFirst);
						}
					}
				}
			}
		}


	} // VirtualMode()

	if (bUpdate)
	{
		m_pControl->AdjustIndentation();
		m_pControl->AdjustLayout();
		m_pControl->AdjustScrollBars();
		m_pControl->UpdateSubList();
		m_pControl->EndUpdate();
	}
}


int CXTPReportSection::InsertRow(int nIndex, CXTPReportRow *pRow)
{
	m_pRows->InsertAt(nIndex, pRow);
	pRow->InternalAddRef();
	pRow->m_pSection = this;
	pRow->SetVisible(TRUE);


	int nRowsInserted = 1;

	if (pRow->IsExpanded() && pRow->HasChildren())
	{
		nRowsInserted += _DoExpand(nIndex, pRow);
	}

	return nRowsInserted;
}


int CXTPReportSection::_DoExpand(int nIndex, CXTPReportRow *pRow)
{
	if (!pRow->HasChildren())
		return 0;

	int nStartIndex = nIndex;
	for (int i = 0; i < pRow->GetChilds()->GetCount(); i++)
	{
		CXTPReportRow* pRowChild = pRow->GetChilds()->GetAt(i);
		if (pRowChild)
		{
			pRowChild->m_nRowLevel   = pRow->m_nRowLevel + 1;
			pRowChild->m_nGroupLevel = pRow->m_nGroupLevel + (pRow->IsGroupRow() ? 1 : 0);

			nIndex += InsertRow(nIndex + 1, pRowChild);
		}
	}
	return nIndex - nStartIndex;
}


int CXTPReportSection::_DoCollapse(CXTPReportRow *pRow)
{
	int nIndex = pRow->GetIndex() + 1;
	int nCount = 0;

	while (nIndex < m_pRows->GetCount())
	{
		CXTPReportRow* pRowChild = m_pRows->GetAt(nIndex);
		if (pRowChild)
		{
			if (!pRowChild->HasParent(pRow))
				break;

			pRowChild->SetVisible(FALSE);
			pRowChild->m_nIndex = -1;
			m_pRows->RemoveAt(nIndex);
			nCount++;
		}
	}
	if (nCount > 0)
	{
		if (m_pRows->m_nFocusedRow >= nIndex)
			m_pRows->m_nFocusedRow = max(nIndex - 1, m_pRows->m_nFocusedRow - nCount);
	}

	return nCount;
}


int CXTPReportSection::GetPageRowCount(CDC *pDC) const
{
	int nCount = 0;

	int nHeight = m_rcSection.Height();

	for (int nRow=m_nScrollIndexV; nRow<m_pRows->GetCount(); nRow++)
	{
		CXTPReportRow *pRow = m_pRows->GetAt(nRow);

		int nRowHeight = pRow->GetHeight(pDC, m_rcSection.Width());

		if (nHeight < nRowHeight)
		{
			break;
		}
		else
		{
			nHeight -= nRowHeight;
			nCount++;
		}
	}

	return nCount;
}

CXTPReportBorder* CXTPReportSection::GetBorder() const
{
	return m_pBorder;
}

int CXTPReportSection::GetTotalHeight(CDC *pDC) const
{
	CSize szBorder = GetBorder()->GetSize();

	return szBorder.cy + GetRowsHeight(pDC);
}

int CXTPReportSection::GetRowsHeight(CDC *pDC) const
{
#if XTP_REPORT_PERFORMANCE()
	DWORD dwStart = ::GetTickCount();
#endif

	int nRowsHeight = 0;

	for (int i=0; i<m_pRows->GetCount(); ++i)
	{
		CXTPReportRow *pRow = m_pRows->GetAt(i);

		nRowsHeight += pRow->GetHeight(pDC, m_rcSection.Width());
	}

#if XTP_REPORT_PERFORMANCE()
	DWORD dwEnd = ::GetTickCount();
	TRACE(_T("GetRowsHeight: %d\n"), dwEnd-dwStart);
#endif

	return nRowsHeight;
}


BOOL CXTPReportSection::GetRowAt(CDC *pDC, int nWidth, int nOffset, int &nIndex, int &nTop)
{
#if XTP_REPORT_PERFORMANCE()
	DWORD dwStart = ::GetTickCount();
#endif

	int nHeight = 0;
	nTop = 0;

	for (nIndex=0; nIndex<m_pRows->GetCount(); ++nIndex)
	{
		CXTPReportRow *pRow = m_pRows->GetAt(nIndex);

		if (pRow->GetRect().Width() == nWidth && FALSE)
		{
			nHeight = pRow->GetRect().Height();
		}
		else
		{
			nHeight = pRow->GetHeight(pDC, nWidth);
		}


		if (nOffset>= nTop && nOffset<= nTop+nHeight)
		{
#if XTP_REPORT_PERFORMANCE()
			DWORD dwEnd = ::GetTickCount();
			TRACE(_T("GetRowAt: %d\n"), dwEnd-dwStart);
#endif
			return TRUE;
		}

		nTop += nHeight;
	}

	return FALSE;
}


void CXTPReportSection::Draw(CDC *pDC)
{
#if XTP_REPORT_PERFORMANCE()
	DWORD dwStart = ::GetTickCount();
#endif

	CRect rcSection(m_rcSection);

	// Draw border
	if (NULL != m_pBorder)
	{
		m_pBorder->SetBorderColor(m_pControl->GetPaintManager()->GetGridColor());
		m_pBorder->Draw(pDC, rcSection);

		// Deduct margin, border and padding from section rectangle
		rcSection = CXTPReportBorder::DeflateRect(rcSection, m_pBorder);
	}

	int nIndex  = m_nScrollIndexV;
	int nOffset = 0;

	if (m_pControl->m_scrollModeV    == xtpReportScrollModeSmooth &&
		 m_pControl->m_pSectionScroll == this)
	{
		int nTop;

		if (GetRowAt(pDC, rcSection.Width(), m_nScrollOffsetV, nIndex, nTop))
		{
			m_nScrollIndexV = nIndex;
			nOffset = m_nScrollOffsetV - nTop;
		}
	}

	m_pControl->DrawRows(pDC, rcSection, rcSection.top-nOffset, m_pRows, nIndex, 0, m_pControl->GetColumns()->GetCount());

	if (xtpReportSectionDividerStyleNone != m_dividerStyle)
	{
		m_pControl->GetPaintManager()->DrawSectionDivider(pDC, m_rcDivider,
			m_dividerPosition, m_dividerStyle, m_clrDivider);
	}

#if XTP_REPORT_PERFORMANCE()
	DWORD dwEnd = ::GetTickCount();
	TRACE(_T("CXTPReportSection::Draw: %d\n"), dwEnd-dwStart);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// Internal methods
/////////////////////////////////////////////////////////////////////////////

void CXTPReportSection::BuildTree(
	CXTPReportRows    *pTree,
	CXTPReportRow     *pParentRow,
	CXTPReportRecords *pRecords)
{
	ASSERT(pTree->GetCount() == 0);

	pTree->ReserveSize(pRecords->GetCount());

	for (int i = 0; i < pRecords->GetCount(); i++)
	{
		CXTPReportRecord *pRecord = pRecords->GetAt(i);

		if (pRecord == NULL)
			continue;

		// add record if all conditions are met
		if (pRecord->IsLocked() ||
			(pRecord->IsVisible() && !m_pControl->ApplyFilter(pRecord, m_pControl->GetFilterText(), m_pControl->IsPreviewMode())))
		{
			CXTPReportRow *pRow = m_pControl->CreateRow(); // new CXTPReportRow_Batch();
			pRow->InitRow(m_pControl, this, pRecord);

			pRow->m_pParentRow = pParentRow;
			pTree->Add(pRow);

			if (pRecord->HasChildren())
			{
				BuildTree(pRow->GetChilds(), pRow, pRecord->GetChilds());

				if (pRow->GetChilds() && m_pControl->IsSortRecordChilds())
				{
					SortRows(pRow->GetChilds());

					pRow->GetChilds()->RefreshChildIndices(FALSE);
				}
			}
		}
	}
}


void CXTPReportSection::SortTree(CXTPReportRows *pTree)
{
	ASSERT(NULL != pTree);

	if (NULL == pTree)
	{
		return;
	}

	SortRows(pTree);

	for (int nRow=0; nRow<pTree->GetCount(); nRow++)
	{
		CXTPReportRow *pRow = pTree->GetAt(nRow);
		ASSERT(NULL != pRow);

		if (NULL != pRow)
		{
			CXTPReportRecord *pRecord = pRow->GetRecord();

			BOOL bRecordHasChildren  = NULL != pRecord ? pRecord->HasChildren() : FALSE;
			BOOL bRecordSortChildren = m_pControl->m_bSortRecordChilds;

			if (pRow->HasChildren() && (bRecordHasChildren && bRecordSortChildren || !bRecordHasChildren))
			{
				SortTree(pRow->GetChilds());
			}
		}

	}

	pTree->RefreshChildIndices(FALSE);
}

void CXTPReportSection::RefreshIndexes(BOOL bAdjustLayout, BOOL bReverseOrder)
{
	int nRowCount = m_pRows->GetCount();
	int nStartIdx = 0, nEndIdx = nRowCount, nStep = 1;
	if (bReverseOrder)
	{
		nStartIdx = nRowCount - 1;
		nEndIdx = nStep = -1;
	}
	for (int nIndex = nStartIdx; nIndex != nEndIdx; nIndex += nStep)
	{
		CXTPReportRow *pRow = m_pRows->GetAt(nIndex);
		if (NULL != pRow)
		{
			BOOL bSelected = GetSelectedRows()->Contains(pRow);
			if (bSelected)
			{
				GetSelectedRows()->Remove(pRow);
			}

			pRow->m_nIndex = nIndex;
			ASSERT(pRow->IsVisible());

			if (bSelected)
			{
				GetSelectedRows()->Add(pRow);
			}
		}
	}

	if (bAdjustLayout)
	{
		m_pControl->AdjustScrollBars();
	}
}

void CXTPReportSection::ReSortRows()
{
	if (IsVirtualMode())
	{
		Populate(TRUE);
		return;
	}

	// Are there any selected rows?
	BOOL bRowsSelected = m_pControl->m_bInitialSelectionEnable;
	if (GetSelectedRows()!= NULL)
	{
		if (GetSelectedRows()->GetCount() > 0)
			bRowsSelected = TRUE;
	}


	CXTPReportRecord *pFocusedRecord = NULL;
	if (NULL != GetFocusedRow())
	{
		pFocusedRecord = GetFocusedRow()->GetRecord();
	}

	SortTree(m_pTree);
	m_pRows->Clear();

	for (int nGroupRow=0; nGroupRow<m_pTree->GetCount(); nGroupRow++)
	{
		CXTPReportRow *pRow = m_pTree->GetAt(nGroupRow);
		InsertRow(m_pRows->GetCount(), pRow);
		pRow->m_nChildIndex = nGroupRow;
		ASSERT(pRow->m_pParentRows == m_pTree);
	}

	m_pRows->m_nFocusedRow = -1;

	// Update indexes on virtual rows
	int nRowCount = m_pRows->GetCount();

	for (int nRow=0; nRow<nRowCount; nRow++)
	{
		CXTPReportRow *pRow = m_pRows->GetAt(nRow);
		if (NULL != pRow)
		{
			pRow->SetIndex(nRow);
			pRow->SetVisible(TRUE);
			CXTPReportRecord *pRecord = pRow->GetRecord();
			if (NULL != pRecord)
			{
				if (pFocusedRecord && pRecord == pFocusedRecord)
				{
					m_pRows->m_nFocusedRow = pRow->GetIndex();
					if (IsSelectionEnabled() && bRowsSelected)
					{
						GetSelectedRows()->Select(pRow);
					}
				}
				BOOL b1 = pRow->IsSelected();
				BOOL b2 = pRecord->IsSelected();
				if (b1 != b2)
					pRow->SetSelected(b2);
			}
		}
	}
}

void CXTPReportSection::SortRows(CXTPReportRows *pRows)
{
	CXTPReportColumns *pColumns = m_pControl->m_pColumns;
	XTPReportRowsCompareFunc pRowsCompareFunc = m_pControl->m_pRowsCompareFunc;

	if (0 == pRows->GetCount())
		return;

	if (0 == pColumns->GetSortOrder()->GetCount() &&
		 0 == pColumns->GetGroupsOrder()->GetCount())
	{
		return;
	}

	CXTPReportRow    *pRow;
	CXTPReportRecord *pRecord;

	if (NULL != pRowsCompareFunc)
	{
		pRows->SortEx(pRowsCompareFunc);
	}
	else
	{
		pRows->Sort();
	}

	m_UaSorted.RemoveAll();
	for (int nRow=0; nRow<pRows->GetCount(); nRow++)
	{
		pRow = pRows->GetAt(nRow);
		if (pRow)
		{
			pRecord = pRow->GetRecord();
			if (NULL != pRecord)
			{
				m_UaSorted.Add(pRecord->GetIndex());
			}
		}
	}
}


BOOL CXTPReportSection::EnsureVisible(CDC *pDC, CXTPReportRow *pRow)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRow);

	if (NULL == pDC || NULL == pRow)
	{
		return FALSE;
	}

	// Row must be visible
	ASSERT(pRow->IsVisible());

	if (!pRow->IsVisible())
	{
		return FALSE;
	}

	// Row must be in this section
	ASSERT(this == pRow->GetSection());

	BOOL bResult = FALSE;

	if (m_pControl->IsIconView())
	{
		bResult = EnsureVisibleIcon(pDC, pRow);
	}
	else
	{
		switch(m_pControl->GetScrollMode(xtpReportOrientationVertical))
		{
		case xtpReportScrollModeSmooth:
			bResult = EnsureVisibleSmooth(pDC, pRow);
			break;

		case xtpReportScrollModeBlockCount:
		case xtpReportScrollModeBlockSize:
			bResult = EnsureVisibleBlock(pDC, pRow);
			break;
		}
	}

	return bResult;
}


int CXTPReportSection::GetRowOffset(CDC *pDC, int nWidth, int nIndex)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);

	if (NULL == pDC)
	{
		return -1;
	}

	int nOffset = 0;

	for (int nRow=0; nRow<nIndex; nRow++)
	{
		CXTPReportRow *pRow = GetRows()->GetAt(nRow);

		nOffset += pRow->GetHeight(pDC, nWidth);
	}

	return nOffset;
}

BOOL CXTPReportSection::EnsureVisibleSmooth(CDC *pDC, CXTPReportRow *pRow)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRow);

	if (NULL == pDC || NULL == pRow)
	{
		return FALSE;
	}

	int nOffset = GetRowOffset(pDC, GetRect().Width(), pRow->GetIndex());

	// Scroll up
	if (nOffset < m_nScrollOffsetV)
	{
		m_pControl->SetScrollOffsetV(nOffset);
	}

	// Scroll down
	int nRowHeight = pRow->GetHeight(pDC, GetRect().Width());
	nOffset += nRowHeight;

	if (nOffset > m_nScrollOffsetV + GetRect().Height())
	{
		m_pControl->SetScrollOffsetV(nOffset - GetRect().Height());
	}

	return TRUE;
}

int CXTPReportSection::EnsureVisibleCalc(CDC *pDC, CXTPReportRow *pRow)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRow);

	if (NULL == pDC || NULL == pRow)
	{
		return -1;
	}

	int nIndexRow = pRow->GetIndex();
	int nIndexTop = m_nScrollIndexV;
	int nCount    = GetRows()->GetCount();
	int nReturn   = -1;

	// Scroll up
	if (nIndexRow <= nIndexTop)
	{
		nReturn = nIndexRow;
	}

	// Scroll down
	if (nIndexRow > nIndexTop)
	{
		LONG top = m_rcSection.top;

		int nIndex = nIndexTop;

		// Figure out if row is fully visible
		while ((top < m_rcSection.bottom) && (nIndex <= nIndexRow) && (nIndex < nCount))
		{
			CXTPReportRow *pRow = GetRows()->GetAt(nIndex);
			ASSERT(NULL != pRow);

			int nRowHeight = pRow->GetHeight(pDC, m_rcSection.Width());

			nIndex++; // Move to next row

			top += nRowHeight;
		}

		// Row not fully visible
		if (top >= m_rcSection.bottom)
		{
			int nHeight = m_rcSection.Height();

			for (nIndex=nIndexRow; nIndex>=0; nIndex--)
			{
				CXTPReportRow *pRow = m_pRows->GetAt(nIndex);
				ASSERT(NULL != pRow);

				int nRowHeight = pRow->GetHeight(pDC, m_rcSection.Width());

				if (nHeight - nRowHeight < 0)
				{
					break;
				}

				nHeight -= nRowHeight;
			}

			nReturn = nIndex + 1;
		}
	}

	return nReturn;

}

BOOL CXTPReportSection::EnsureVisibleBlock(CDC *pDC, CXTPReportRow *pRow)
{
	int nIndex = EnsureVisibleCalc(pDC, pRow);

	if (-1 == nIndex)
	{
		return FALSE;
	}

	SetScrollIndexV(nIndex);

	return TRUE;
}

BOOL CXTPReportSection::EnsureVisibleIcon(CDC *pDC, CXTPReportRow *pRow)
{
	int nIndex = EnsureVisibleCalc(pDC, pRow);

	if (-1 == nIndex)
	{
		return FALSE;
	}

	int nIndexRow = pRow->GetIndex();

	if (nIndex != nIndexRow)
	{
		nIndex += m_pControl->GetRowsPerLine();
	}

	m_pControl->SetTopRow(nIndex);

	return TRUE;
}

int CXTPReportSection::GetScrollOffsetV() const
{
	return m_nScrollOffsetV;
}

void CXTPReportSection::SetScrollOffsetV(int nScrollOffsetV)
{
	m_nScrollOffsetV = nScrollOffsetV;
	m_nScrollIndexV  = -1; // Recalculate
}

XTPReportRowType CXTPReportSection::GetRowType() const
{
	return m_rowType;
}

CXTPReportSelectedRows* CXTPReportSection::GetSelectedRows() const
{
	return m_pControl->GetSelectedRows();
}

CXTPReportRow* CXTPReportSection::GetFocusedRow() const
{
	return GetRows()->GetFocusedRow();
}

int CXTPReportSection::GetIndex() const
{
	int nIndex = -1;

	const CXTPReportSections *pSections = m_pControl->GetSections();

	for (int nSection=0; nSection<pSections->GetSize(); nSection++)
	{
		if (this == pSections->GetAt(nSection))
		{
			nIndex = nSection;
		}
	}

	return -1;
}

void CXTPReportSection::AddRecordEx(
	CXTPReportRecord *pRecord,
	CXTPReportRecord *pParentRecord,
	int nRowChildIndex,
	int nRecordChildIndex)
{
	ASSERT(pRecord);
	if (!pRecord)
		return;

	// add record
	BOOL bAddRecord = TRUE;
	CXTPReportRecords* pParentRecords = pParentRecord ? pParentRecord->HasChildren() ? pParentRecord->GetChilds() : NULL : GetRecords();
	if (pParentRecords)
	{
		for (int nChild = 0; nChild < pParentRecords->GetCount(); nChild++)
		{
			if (pRecord == pParentRecords->GetAt(nChild))
			{
				bAddRecord = FALSE;
				break;
			}
		}
	}
	if (bAddRecord)
	{
		CXTPReportRecords* pRecords = pParentRecord ? pParentRecord->GetChilds() : GetRecords();

		if (nRecordChildIndex >= 0 && nRecordChildIndex < pRecords->GetCount())
			pRecords->InsertAt(nRecordChildIndex, pRecord);
		else
			pRecords->Add(pRecord);
	}

	if (!pRecord->IsVisible())
		return;

	// find parent record rows
	CXTPReportRows* pParentRows = m_pTree;
	CXTPReportRow* pParentRow = NULL;
	if (pParentRecord)
	{
		pParentRow = m_pTree->FindInTree(pParentRecord);
		if (pParentRow)
			pParentRows = pParentRow->GetChilds();
	}
	BOOL bRoot = pParentRows == m_pTree;

	// create new row
	CXTPReportRow* pNewRow = m_pControl->CreateRow();
	pNewRow->InitRow(m_pControl, this, pRecord);
	pNewRow->m_pParentRow = NULL;

	// add row to pParentRows
	BOOL bInsertAfter = FALSE;
	CXTPReportRow* pPlainTreeRow = NULL;
	CXTPReportRow* pInsertRowPos = NULL;
	int nNextSiblingIndex = m_pRows->GetCount();
	if (bRoot)
	{
		pInsertRowPos = pParentRows->FindInsertionPos(pNewRow, bInsertAfter);
		CXTPReportRow* pRow = pInsertRowPos;
		while (pRow)
		{
			if (pRow->GetNextSiblingRow())
			{
				nNextSiblingIndex = pRow->GetNextSiblingRow()->GetIndex();
				break;
			}
			else
				pRow = pRow->GetParentRow();
		}

		CXTPReportColumns *pColumns = m_pControl->GetColumns();

		if (pColumns->GetGroupsOrder()->GetCount() > 0)
		{
			CXTPReportGroupRow* pGroupToAdd = pInsertRowPos ? (CXTPReportGroupRow*)pInsertRowPos->GetParentRow() : NULL;
			if (!pInsertRowPos || (pInsertRowPos && pInsertRowPos->IsGroupRow()))
			{
				for (int nColumn = pInsertRowPos ? pInsertRowPos->GetGroupLevel() : 0; nColumn < pColumns->GetGroupsOrder()->GetCount(); nColumn++)
				{
					CXTPReportColumn* pColumn = pColumns->GetGroupsOrder()->GetAt(nColumn);
					CXTPReportRecordItem* pItem = pRecord->GetItem(pColumn);
					CString strGroup = pItem ? pItem->GetGroupCaption(pColumn) : _T("");

					CXTPReportGroupRow* pGroup = m_pControl->CreateGroupRow();
					pGroup->InitRow(m_pControl, this, NULL);
					pGroup->SetCaption(strGroup);
					pGroup->m_nGroupLevel = pGroupToAdd ? pGroupToAdd->m_nRowLevel + 1 : 0;
					pGroup->m_nRowLevel = nColumn;
					if (pGroupToAdd)
					{
						if (!pPlainTreeRow)
						{
							pPlainTreeRow = pGroup;
							pGroupToAdd->GetChilds()->InsertAt(bInsertAfter ? pInsertRowPos ? pInsertRowPos->m_nChildIndex + 1 : 0 : pInsertRowPos ? pInsertRowPos->m_nChildIndex : 0, pGroup);
						}
						else
							pGroupToAdd->AddChild(pGroup);
						pGroup->m_pParentRows = pGroupToAdd->GetChilds();
						pGroup->m_pParentRow = pGroupToAdd;
						pGroup->SetVisible(pGroupToAdd->IsExpanded());
					}
					else
					{
						pPlainTreeRow = pGroup;
						pParentRows->InsertAt(pInsertRowPos ? pInsertRowPos->m_nChildIndex : pParentRows->GetCount(), pGroup);
						pGroup->m_pParentRows = pParentRows;
						pGroup->m_pParentRow = NULL;
						pGroup->SetVisible(TRUE);
					}
					pGroupToAdd = pGroup;
				}
				// insert row
				if (pGroupToAdd)
				{
					pGroupToAdd->AddChild(pNewRow);
					pNewRow->m_pParentRows = pGroupToAdd->GetChilds();
	//              pNewRow->m_pParentRow = pGroupToAdd;
					pNewRow->SetVisible(pGroupToAdd->IsExpanded());
				}
			}
			else
			{
				// insert row
				if (pGroupToAdd)
				{
					pGroupToAdd->GetChilds()->InsertAt(bInsertAfter ? pInsertRowPos ? pInsertRowPos->m_nChildIndex + 1 : 0 : pInsertRowPos ? pInsertRowPos->m_nChildIndex : 0, pNewRow);
					pNewRow->m_pParentRows = pGroupToAdd->GetChilds();
					pNewRow->m_pParentRow = pGroupToAdd;
					pNewRow->SetVisible(pGroupToAdd->IsExpanded());
				}
			}
		}
		else
		{
			pParentRows->InsertAt(pInsertRowPos ? pInsertRowPos->m_nChildIndex : pParentRows->GetCount(), pNewRow);
			pNewRow->m_pParentRows = pParentRows;
			pNewRow->m_pParentRow = NULL;
			pNewRow->SetVisible(TRUE);
		}
	}
	else
	{
		if (nRowChildIndex >= 0 && nRowChildIndex < pParentRows->GetCount())
			pParentRows->InsertAt(nRowChildIndex, pNewRow);
		else
			pParentRows->Add(pNewRow);
		pNewRow->m_pParentRows = pParentRows;
		pNewRow->m_pParentRow = pParentRow;
		pNewRow->SetVisible(TRUE);
		pNewRow->m_nRowLevel = pParentRow->m_nRowLevel + 1;
		pNewRow->m_nGroupLevel = pParentRow->m_nGroupLevel;
		pNewRow->m_nChildIndex = nRowChildIndex >= 0 ? nRowChildIndex : pParentRows->GetCount() - 1;

		CXTPReportRow* pRow = pNewRow;
		while (pRow)
		{
			if (pRow->GetNextSiblingRow())
			{
				nNextSiblingIndex = pRow->GetNextSiblingRow()->GetIndex();
				break;
			}
			else
				pRow = pRow->GetParentRow();
		}
	}
	// refresh child indices
	if (pInsertRowPos && pInsertRowPos->GetParentRows())
		pInsertRowPos->GetParentRows()->RefreshChildIndices();
	else
		pParentRows->RefreshChildIndices();

	// add row to m_pRows
	if (bRoot)
	{
		if (pInsertRowPos)
		{
			CXTPReportRow* pRow = pInsertRowPos->GetParentRow();
			BOOL bIsExpanded = TRUE;
			while (pRow && bIsExpanded)
			{
				bIsExpanded = bIsExpanded && pRow->IsExpanded();
				pRow = pRow->GetParentRow();
			}

			if (bIsExpanded)
			{
				if (bInsertAfter)
					InsertRow(nNextSiblingIndex, pPlainTreeRow ? pPlainTreeRow : pNewRow);
				else
					InsertRow(pInsertRowPos->GetIndex(), pPlainTreeRow ? pPlainTreeRow : pNewRow);
			}
		}
		else
		{
			InsertRow(m_pRows->GetCount(), pPlainTreeRow ? pPlainTreeRow : pNewRow);
		}
	}
	else
	{
		CXTPReportRow* pRow = pNewRow->GetParentRow();
		BOOL bIsExpanded = TRUE;
		while (pRow && bIsExpanded)
		{
			bIsExpanded = bIsExpanded && pRow->IsExpanded();
			pRow = pRow->GetParentRow();
		}

		if (bIsExpanded)
			InsertRow(nNextSiblingIndex, pNewRow);
	}

	RefreshIndexes(FALSE, TRUE);

	// add children
	if (pRecord && pRecord->HasChildren())
	{
		for (int nChild = 0; nChild < pRecord->GetChilds()->GetCount(); nChild++)
		{
			AddRecordEx(pRecord->GetChilds()->GetAt(nChild), pRecord);
		}
	}
}


void CXTPReportSection::UpdateRecord(CXTPReportRecord *pRecord, BOOL bUpdateChildren)
{
	// Get parent record
	CXTPReportRecord *pParentRecord = NULL;
	if (pRecord->GetRecords())
	{
		pParentRecord = pRecord->GetRecords()->GetOwnerRecord();
	}

	// Update record
	if (NULL == pParentRecord || bUpdateChildren)
	{
		// Internal AddRef
		pRecord->TreeAddRef();

		// Get record row child index
		CXTPReportRow *pRow = m_pTree->FindInTree(pRecord);

		if (NULL != pRow)
		{
			int nRowChildIndex = pRow->m_nChildIndex;
			int nRecordChildIndex = pRecord->GetIndex();

			// Remove record
			RemoveRecordEx(pRecord, FALSE);

			// Add record
			pRecord->InternalAddRef(); //AddRecordEx expects that pRecord has +1 ref count.
			AddRecordEx(pRecord, pParentRecord, nRowChildIndex, nRecordChildIndex);

			// Internal release
			pRecord->TreeRelease();
		}
	}

	m_pControl->RedrawControl();
}

BOOL CXTPReportSection::RemoveRecordEx(CXTPReportRecord* pRecord, BOOL bAdjustLayout, BOOL bRemoveFromParent)
{
	CXTPReportRecords *pRecords = GetRecords();

	ASSERT(pRecord && pRecords);

	if (!pRecord || !pRecords)
		return FALSE;

	if (pRecord->HasChildren())
	{
		for (int i=pRecord->GetChilds()->GetCount() - 1; i>=0 ; i--)
		{
			RemoveRecordEx(pRecord->GetChilds()->GetAt(i), bAdjustLayout, FALSE);
		}
	}

	BOOL bResult = FALSE;

	//--------------------------------------------------------
	CXTPReportRow *pRow0 = m_pTree->FindInTree(pRecord);

	if (NULL != pRow0)
	{
		CXTPReportRow* pRow = pRow0;
		pRow0 = NULL;

		do
		{
			CXTPReportRow* pRow_parent = pRow->GetParentRow();

			ASSERT(pRow->GetParentRows());

			// 1. Remove from selected rows
			if (GetSelectedRows()->Contains(pRow))
			{
				GetSelectedRows()->Remove(pRow);
			}

			// 2. remove from Rows Tree
			if (pRow->GetParentRows())
			{
				VERIFY(pRow->GetParentRows()->RemoveRow(pRow) >= 0);
			}

			// 3. remove from Display Rows array
			m_pRows->RemoveRow(pRow);

			pRow = pRow_parent;

		} while (pRow && pRow->IsGroupRow() && pRow->GetChilds()->GetCount() == 0);

		// refresh child indices
		if (pRow && pRow->HasChildren())
			pRow->GetChilds()->RefreshChildIndices();
		else
			m_pTree->RefreshChildIndices();

		bResult = TRUE;
	}

	//-------------------------------------------------------
	//pRecord->Delete(); // the code below is more safe when record already removed!

	ASSERT(pRecord->GetRecords());

	if (bRemoveFromParent && pRecord->GetRecords())
	{
		BOOL bRecordRem = pRecord->GetRecords()->RemoveRecord(pRecord) >= 0;
		pRecord = NULL;

		bResult |= bRecordRem;
	}

	if (m_pRows->m_nFocusedRow >= 0 && m_pRows->m_nFocusedRow >= m_pRows->GetCount())
		m_pRows->m_nFocusedRow = m_pRows->GetCount() - 1;

	//-------------------------------------------------------
	if (bResult)
	{
		RefreshIndexes(bAdjustLayout, FALSE);
	}

	return bResult;
}

BOOL CXTPReportSection::RemoveRowEx(CXTPReportRow *pRow, BOOL bAdjustLayout)
{
	ASSERT(NULL != pRow);
	if (NULL == pRow)
	{
		return FALSE;
	}

	if (!pRow->IsGroupRow())
	{
		ASSERT(pRow->GetRecord());
		return RemoveRecordEx(pRow->GetRecord(), bAdjustLayout);
	}
	else
	{
		int nCount = pRow->GetChilds()->GetCount();

		for (int i = nCount - 1; i >= 0 ; i--)
		{
			CXTPReportRow* pRowI = pRow->GetChilds()->GetAt(i);
			RemoveRowEx(pRowI, bAdjustLayout);
		}
		return TRUE;
	}
}


/////////////////////////////////////////////////////////////////////////////
// Scroll index
/////////////////////////////////////////////////////////////////////////////

int CXTPReportSection::GetScrollIndexV()
{
	if (-1 == m_nScrollIndexV)
	{
		switch(GetScrollModeV())
		{
		case xtpReportScrollModeSmooth:
		{
			int nIndex = 0;
			int nTop   = 0;

			CRect rcSection(m_rcSection);

			// Draw border
			if (NULL != m_pBorder)
			{
				// Deduct margin, border and padding from section rectangle
				rcSection = CXTPReportBorder::DeflateRect(rcSection, m_pBorder);
			}

			CClientDC dc(m_pControl);
			if (GetRowAt(&dc, rcSection.Width(), m_nScrollOffsetV, nIndex, nTop))
			{
				m_nScrollIndexV = nIndex;
			}
		}
		break;

		default:
			m_nScrollIndexV = 0;
			break;
		}
	}

	return m_nScrollIndexV;
}

void CXTPReportSection::SetScrollIndexV(int nScrollIndexV)
{
	m_nScrollIndexV = nScrollIndexV;

	if (xtpReportScrollModeSmooth == GetScrollModeV())
	{
		CClientDC dc(m_pControl);
		m_nScrollOffsetV = GetRowOffset(&dc, GetRect().Width(), nScrollIndexV);
	}

	// Update scrollbar position
	if (this == m_pControl->m_pSectionScroll)
	{
		m_pControl->UpdateScrollBarV();
	}
}



