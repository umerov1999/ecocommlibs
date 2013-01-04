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
//#include "ItemTypes/XTPReportRecordItemText.h"
#include "XTPReportControl.h"
#include "XTPReportColumns.h"
#include "XTPReportColumn.h"
#include "XTPReportRows.h"
#include "XTPReportSelectedRows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPReportSelectedRows

CXTPReportSelectedRows::CXTPReportSelectedRows(CXTPReportControl* pControl)
	: m_pControl(pControl)
{


	m_nRowBlockBegin = -1;
	m_nPosSelected = 0;
	m_bChanged = FALSE;
	m_nRowType = xtpRowTypeBody;
	m_bNotifyOnClear = TRUE;
}

BOOL CXTPReportSelectedRows::Clear()
{
	return Clear(TRUE);
}

BOOL CXTPReportSelectedRows::Clear(BOOL bNotifyOnClear)
{
	BOOL bClear = FALSE;

	if (m_arrSelectedBlocks.GetSize() > 0)
	{
		if (bNotifyOnClear)
		{
			const BOOL bCancel = _NotifySelChanging(xtpReportSelectionClear);

			if (!bCancel)
			{
				bClear = TRUE;
				m_bChanged = TRUE;
				m_arrSelectedBlocks.RemoveAll();
				m_nRowBlockBegin = -1;
				m_nRowType = xtpRowTypeBody;
				m_pControl->RedrawControl();
			}
		}

	}

	return bClear;
}

void CXTPReportSelectedRows::_InsertBlock(int nIndexInsert, int nIndexBegin, int nIndexEnd)
{
	SwapIfNeed(nIndexBegin, nIndexEnd);

	SELECTED_BLOCK block;
	block.nIndexBegin = nIndexBegin;
	block.nIndexEnd = nIndexEnd;
	m_arrSelectedBlocks.InsertAt(nIndexInsert, block);
}

void CXTPReportSelectedRows::_OnCollapsed(int nIndex, int nCount)
{
	ASSERT(nCount > 0);

	for (int i = (int) m_arrSelectedBlocks.GetSize() - 1; i >= 0 ; i--)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd   = m_arrSelectedBlocks[i].nIndexEnd;

		if (nIndexBegin <= nIndex && nIndexEnd > nIndex + 1)
		{
			nIndexEnd = max(nIndexBegin + 1, nIndexEnd - nCount);

		}
		else if (nIndexBegin > nIndex + nCount)
		{
			nIndexBegin -= nCount;
			nIndexEnd -= nCount;
		}
		else if (nIndexEnd < nIndex)
			break;
		else if (nIndexBegin > nIndex)
		{
			nIndexBegin = nIndex;
			nIndexEnd = max(nIndexBegin + 1, nIndexEnd - nCount);
		}
	}
}

void CXTPReportSelectedRows::_OnExpanded(int nIndex, int nCount)
{
	ASSERT(nCount > 0);

	for (int i = (int) m_arrSelectedBlocks.GetSize() - 1; i >= 0 ; i--)
	{
		int& nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int& nIndexEnd = m_arrSelectedBlocks[i].nIndexEnd;

		if (nIndexBegin <= nIndex && nIndexEnd > nIndex + 1)
		{
			_InsertBlock(i + 1, nIndex + 1 + nCount, nIndexEnd + nCount);
			m_arrSelectedBlocks[i].nIndexEnd = nIndex + 1;

		}
		else if (nIndexBegin > nIndex)
		{
			nIndexBegin += nCount;
			nIndexEnd += nCount;
		}
		else if (nIndexEnd < nIndex)
			break;
	}
}

void CXTPReportSelectedRows::AddBlock(int ib, int ie)
{
	SwapIfNeed(ib, ie);

	int i = 0;
	int nCount = (int) m_arrSelectedBlocks.GetSize();

	if (nCount > 0)
	{
		int& nIndexEnd = m_arrSelectedBlocks[nCount - 1].nIndexEnd;

		if (nIndexEnd == ib)
		{
			nIndexEnd = ie + 1;
			return;
		}

		if (nIndexEnd < ib)
		{
			i = nCount;
		}
	}

	for (; i < nCount; i++)
	{
		int &nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int &nIndexEnd   = m_arrSelectedBlocks[i].nIndexEnd;

		if ((nIndexBegin <= ib) && (nIndexEnd > ie))
		{
			return;
		}

		if (nIndexEnd == ib)
		{
			nIndexEnd = ie + 1;
			if (i + 1 < nCount && m_arrSelectedBlocks[i + 1].nIndexBegin == nIndexEnd)
			{
				nIndexEnd = m_arrSelectedBlocks[i + 1].nIndexEnd;
				m_arrSelectedBlocks.RemoveAt(i + 1);
			}

			return;
		}
		if (nIndexBegin == ie + 1)
		{
			nIndexBegin = ib;
			return;
		}

		if (nIndexBegin > ie)
			break;
	}

	_InsertBlock(i, ib, ie + 1);
}


BOOL CXTPReportSelectedRows::Add(CXTPReportRow *pRow)
{
	BOOL bAdd = FALSE;

	if (NULL != pRow)
	{
		BOOL bCancel = _NotifySelChanging(xtpReportSelectionAdd, pRow);

		if (!bCancel)
		{
			int nIndex = pRow->GetIndex();

			if (nIndex != -1)
			{
				AddBlock(nIndex, nIndex);
				bAdd = TRUE;
			}
		}
	}

	return bAdd;
}


void CXTPReportSelectedRows::Remove(CXTPReportRow *pRow)
{
	if (NULL == pRow)
	{
		return;
	}

	if (_NotifySelChanging(xtpReportSelectionRemove, pRow))
	{
		return; // Handled / Cancel
	}

	int nIndex = pRow->GetIndex();

	for (int i = 0; i < (int) m_arrSelectedBlocks.GetSize(); i++)
	{
		int nIndexBegin = m_arrSelectedBlocks[i].nIndexBegin;
		int nIndexEnd = m_arrSelectedBlocks[i].nIndexEnd;

		if ((nIndexBegin <= nIndex) && (nIndexEnd > nIndex))
		{
			if (nIndexBegin != nIndex)
			{
				_InsertBlock(i, nIndexBegin, nIndex);
				i++;
			}

			if (nIndexEnd - 1 != nIndex)
			{
				_InsertBlock(i + 1, nIndex + 1, nIndexEnd);
			}

			m_arrSelectedBlocks.RemoveAt(i);
			m_pControl->RedrawControl();
			return;
		}
	}
}

BOOL CXTPReportSelectedRows::Select(CXTPReportRow *pRow)
{
	BOOL bSelect = FALSE;

	if (NULL != pRow)
	{
		// If new selected row belongs to the other type - clear previous selection.
		if (m_nRowType != pRow->GetType())
		{
			Clear(GetNotifyOnClear());
			m_bChanged = TRUE;
		}

		if (GetCount() == 1 && Contains(pRow))
		{
		}
		else
		{
			BOOL bClear = Clear(GetNotifyOnClear());
			BOOL bAdd   = Add(pRow);

			bSelect = bClear && bAdd;

			if (bClear || bAdd /* || Always */)
			{
				m_bChanged = TRUE;
			}
			m_nRowType = pRow->GetType();
		}
	}

	return bSelect;
}

void CXTPReportSelectedRows::SelectBlock(int nBlockBegin, int nEnd, BOOL bControlKey)
{
	CXTPReportRows* pRows;
	switch(m_nRowType)
	{
		case xtpRowTypeHeader : pRows = m_pControl->GetHeaderRows(); break;
		case xtpRowTypeFooter : pRows = m_pControl->GetFooterRows(); break;
		default : pRows = m_pControl->GetRows(); break;
	}
	int nRowsCount(0);
	if (pRows)
		nRowsCount = pRows->GetCount();
	BOOL bGo = (nBlockBegin >= 0 && nBlockBegin < nRowsCount && nEnd < nRowsCount);
	if (!bGo)
	{
		Clear(GetNotifyOnClear());
		return;
	}

	if (bControlKey == FALSE)
	{
		nBlockBegin = m_nRowBlockBegin != -1 ? m_nRowBlockBegin : nBlockBegin;

		int nBegin = nBlockBegin;

		if (nBegin == -1 || nEnd == -1)
			return;

		if (nBegin > nEnd)
		{
			nBegin = nEnd;
			nEnd = nBlockBegin;
		}

		if (m_arrSelectedBlocks.GetSize() == 1 && m_arrSelectedBlocks[0].nIndexBegin == nBegin &&
			m_arrSelectedBlocks[0].nIndexEnd == nEnd + 1)
		{
			return;
		}

		XTPReportRowType nRowType = m_nRowType;
		Clear(GetNotifyOnClear());
		m_nRowType = nRowType;
		if (m_nRowBlockBegin == -1) m_nRowBlockBegin = nBlockBegin;


		BOOL bSkipGroupFocus = m_pControl->IsSkipGroupsFocusEnabled();
		BOOL bHasGroups = m_pControl->GetColumns()->GetGroupsOrder()->GetCount() != 0;

		if (!bHasGroups || !bSkipGroupFocus)
		{
			_InsertBlock(0, nBegin, nEnd + 1);
		}
		else
		{
			for (int i = nBegin; i <= nEnd; i++)
			{
				CXTPReportRow* pRow = pRows->GetAt(i);
				if (!pRow)
					continue;

				if (!bSkipGroupFocus || !pRow->IsGroupRow() || !pRow->IsExpanded() || (i == nBegin) || (i == nEnd))
				{
					Add(pRow);
				}
			}
		}

		// notify owner the selection state has changed.
		_NotifyStateChanged(nBegin, nEnd);
	}
	else
	{
		int kSB = (int) m_arrSelectedBlocks.GetSize();
		if (kSB > 0)
		{
			int iMin = m_arrSelectedBlocks[0].nIndexBegin;
			int iMax = m_arrSelectedBlocks[kSB - 1].nIndexEnd;
			if (nEnd >= iMin && nEnd < iMax)
			{
				return;
			}
		}
		BOOL bSkipGroupFocus = FALSE;//m_pControl->IsSkipGroupsFocusEnabled();
		BOOL bHasGroups = m_pControl->GetColumns()->GetGroupsOrder()->GetCount() != 0;
		BOOL bWasShiftKey = m_pControl->m_bWasShiftKey;

		if (m_nRowBlockBegin != -1)
			nBlockBegin = m_nRowBlockBegin;
		int nBegin(nBlockBegin), iB, iE;
		if (nBegin == -1 || nEnd == -1)
		{
			return;
		}
		BOOL bSwap = SwapIfNeed(nBegin, nEnd);
		int nArSz = (int) m_arrSelectedBlocks.GetSize();

		CUIntArray ar;
		if (nArSz > 0)
		{
			for (int k = 0; k < nArSz; k++)
			{
				int iB = m_arrSelectedBlocks[nArSz - 1 - k].nIndexBegin;
				int iE = m_arrSelectedBlocks[nArSz - 1 - k].nIndexEnd;
				if (iE == iB + 1 && iB == 0)
					k++; //this fix a case of initial selection from any row to any row with SHIFT+CONTROL pressed
				if (k < nArSz)
				{
					ar.Add(m_arrSelectedBlocks[k].nIndexBegin);
					ar.Add(m_arrSelectedBlocks[k].nIndexEnd);
				}
			}
		}

		XTPReportRowType nRowType = m_nRowType;
		Clear(GetNotifyOnClear());
		m_nRowType = nRowType;

		if (m_nRowBlockBegin == -1)
			m_nRowBlockBegin = nBlockBegin;

		if (!bHasGroups || !bSkipGroupFocus) //<<>>
		{
			int k = 0;
			int N = (int) ar.GetSize();
			if (N > 0 && bControlKey && !bWasShiftKey)
			{
				for (k = 0; k < N / 2; k++)
				{
					iB = ar.GetAt(2 * k);
					iE = ar.GetAt(2 * k + 1);
					if (iE < nBegin || iB > nEnd)
					{
						_InsertBlock(k, iB, iE);
					}
					else if (iB >= nBegin && iE <= nEnd) //skip [iB:iE] segment - totally covered
					{
					}
					else if (iB <= nBegin && iE <= nEnd) //skip [iB:iE] segment amd modify new segment
					{
						nBegin = iB;
					}
					else if (iB <= nBegin && iE > nEnd) //skip [iB:iE] segment amd modify new segment
					{
						nBegin = iB;
						nEnd = iE;
					}
					else if (iB >= nBegin && iE > nEnd) //skip [iB:iE] segment amd modify new segment
					{
						nEnd = iE;
					}
					else
					{
					}
				}

				if (bSwap)
				{
					_InsertBlock(0, nBegin, nEnd);
				}
				else
				{
					_InsertBlock(k, nEnd + 1, nBegin);
				}
			}
			else
			{
				_InsertBlock(0, nBegin, nEnd + 1);
			}

			CUIntArray aDel;
			for (int l = 0; l < m_arrSelectedBlocks.GetSize(); l++)
			{
				if (m_arrSelectedBlocks[l].nIndexBegin == m_arrSelectedBlocks[l].nIndexEnd)
					aDel.Add(l);

			}
			for (int ll = 0; ll < aDel.GetSize(); ll++)
				m_arrSelectedBlocks.RemoveAt(aDel.GetAt(ll));
		}
		else
		{
			for (int i = nBegin; i <= nEnd; i++)
			{
				CXTPReportRow* pRow = pRows->GetAt(i);
				if (!pRow)
					continue;

				if (!pRow->IsGroupRow()
					|| !bSkipGroupFocus
					|| !pRow->IsExpanded()
					|| i == nBegin
					|| i == nEnd)
					Add(pRow);
			}
		}

		// notify owner the selection state has changed.
		_NotifyStateChanged(nBegin, nEnd);
	}

	if (m_pControl->m_bSelectionExcludeGroupRows)
		m_pControl->UnselectGroupRows();

	//to show only selected childs under group rows
	//flag - m_bSelectionExcludeGroupRows default = TRUE - this is 12.1 way to show selection
TRACE(_T("return from SelectBlock with Count=%d\n"), GetCount());
}

BOOL CXTPReportSelectedRows::Contains(const CXTPReportRow *pRow)
{
	if (!pRow)
		return FALSE;

	if (m_nRowType != pRow->GetType())
		return FALSE;

	int nIndex = pRow->GetIndex();

	int nCount = (int) m_arrSelectedBlocks.GetSize();
	if (nCount == 0)
		return FALSE;

	const SELECTED_BLOCK &blockFirst = m_arrSelectedBlocks[0];
	const SELECTED_BLOCK &blockLast  = m_arrSelectedBlocks[nCount - 1];

	if (blockFirst.nIndexBegin > nIndex)
		return FALSE;

	if (blockLast.nIndexEnd <= nIndex)
		return FALSE;

	for (int i = 0; i<nCount; i++)
	{
		const SELECTED_BLOCK &block = m_arrSelectedBlocks[i];

		if ((block.nIndexBegin <= nIndex) && (block.nIndexEnd > nIndex))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CXTPReportSelectedRows::Invert(CXTPReportRow* pRow)
{
	if (pRow->GetType() != m_nRowType)
		return;

	if (Contains(pRow))
		Remove (pRow);
	else
		Add(pRow);

	m_nRowBlockBegin = -1;
	m_bChanged = TRUE;
}

int CXTPReportSelectedRows::GetCount()
{
	int nCount = 0;
	for (int i = 0; i < (int) m_arrSelectedBlocks.GetSize(); i++)
		nCount += m_arrSelectedBlocks[i].nIndexEnd - m_arrSelectedBlocks[i].nIndexBegin;

	return nCount;
}

POSITION CXTPReportSelectedRows::GetFirstSelectedRowPosition()
{
	if (m_arrSelectedBlocks.GetSize() == 0)
		return NULL;

	m_nPosSelected = 0;

//  int nLast = (int) m_arrSelectedBlocks.GetSize() - 1;
	POSITION pos = (POSITION)(DWORD_PTR) (m_arrSelectedBlocks[m_nPosSelected].nIndexBegin + 1);
//TRACE(_T("m_nPosSelected=%d Blocks=%d GetFirstSelectedRowPosition=%d\n"), m_nPosSelected, nLast, (int) pos);
	return pos;
}

CXTPReportRow* CXTPReportSelectedRows::GetNextSelectedRow(POSITION& pos)
{
	ASSERT(m_nPosSelected < m_arrSelectedBlocks.GetSize());

	int nIndex = (int)(DWORD_PTR) pos - 1;
	CXTPReportRow* pRow;
	switch (m_nRowType)
	{
		case xtpRowTypeHeader : pRow = m_pControl->GetHeaderRows()->GetAt(nIndex); break;
		case xtpRowTypeFooter : pRow = m_pControl->GetFooterRows()->GetAt(nIndex); break;
		default : pRow = m_pControl->GetRows()->GetAt(nIndex); break;
	}

	int nLast = (int) m_arrSelectedBlocks.GetSize() - 1;
	if (nIndex < m_arrSelectedBlocks[m_nPosSelected].nIndexEnd - 1)
		pos++;
	else
	{
		if (m_nPosSelected >= nLast)
		{
//TRACE(_T("m_nPosSelected=%d Blocks=%d GetNextSelectedRow=%d nIndex=%d\n"), m_nPosSelected, nLast, (int) pos, nIndex);
			pos = NULL;
		}
		else
			pos = (POSITION)(DWORD_PTR) (m_arrSelectedBlocks[++m_nPosSelected].nIndexBegin + 1);
	}

	return pRow;
}

CXTPReportRow* CXTPReportSelectedRows::GetAt(int nIndex)
{
	for (int i = 0; i < (int) m_arrSelectedBlocks.GetSize(); i++)
	{
		int nCount = m_arrSelectedBlocks[i].nIndexEnd - m_arrSelectedBlocks[i].nIndexBegin;
		if (nCount <= nIndex)
		{
			nIndex -= nCount;
			continue;
		}

		CXTPReportRow* pRow;
		switch (m_nRowType)
		{
			case xtpRowTypeHeader : pRow = m_pControl->GetHeaderRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin + nIndex); break;
			case xtpRowTypeFooter : pRow = m_pControl->GetFooterRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin + nIndex); break;
			default : pRow = m_pControl->GetRows()->GetAt(m_arrSelectedBlocks[i].nIndexBegin + nIndex); break;
		}
		return pRow;
	}

	return NULL;
}

void CXTPReportSelectedRows::_NotifyStateChanged(int nBegin, int nEnd)
{
	if (m_pControl)
	{
		XTP_NM_REPORTSTATECHANGED nm;
		::ZeroMemory(&nm, sizeof(nm));

		nm.hdr.hwndFrom = m_pControl->GetSafeHwnd();
		nm.hdr.idFrom = m_pControl->GetDlgCtrlID();
		nm.nBegin = nBegin;
		nm.nEnd = nEnd;

		m_pControl->SendNotifyMessage(XTP_NM_REPORT_STATECHANGED,(NMHDR*)&nm);
	}
}

BOOL CXTPReportSelectedRows::_NotifySelChanging(XTPReportSelectionChangeType nType, CXTPReportRow* pRow)
{
	ASSERT(NULL != m_pControl);

	if (NULL == m_pControl)
	{
		return FALSE; // Message not handled
	}

	XTP_NM_SELECTION_CHANGING nm;
	::ZeroMemory(&nm, sizeof(nm));

	nm.pRow  = pRow;
	nm.nType = nType;

	LRESULT result = m_pControl->SendNotifyMessage(XTP_NM_REPORT_SELCHANGING, reinterpret_cast<NMHDR*>(&nm));

	return 0 != result;
}

BOOL CXTPReportSelectedRows::SwapIfNeed(int& indexB, int& indexE)
{
	if (indexB > indexE)
	{
		int indexT = indexB;
		indexB = indexE;
		indexE = indexT;
		return TRUE;
	}
	return FALSE;
}


