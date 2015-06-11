// XTPReportNavigator.cpp : implementation of the CXTPReportNavigator class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "StdAfx.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPReportDefines.h"
#include "XTPReportNavigator.h"
#include "XTPReportControl.h"
#include "XTPReportRow.h"
#include "XTPReportRows.h"
#include "XTPReportSection.h"
#include "XTPReportRecord.h"
#include "XTPReportRecordItem.h"
#include "XTPReportColumn.h"
#include "XTPReportColumns.h"
#include "XTPReportInplaceControls.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CXTPReportNavigator

CXTPReportNavigator::CXTPReportNavigator(CXTPReportControl *pReportControl)
	: m_pReportControl(pReportControl)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPReportNavigator::~CXTPReportNavigator()
{
}

void CXTPReportNavigator::MoveDown(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportRow* pNextRow = NULL;
	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (m_pReportControl->GetHeaderRows()->HasFocus())
	{
		pNextRow = m_pReportControl->GetHeaderRows()->GetNext(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());
		// from the last header row jump to the first visible body row
		if (pFocusedRow == pNextRow)
		{
			MoveFirstVisibleRow(xtpRowTypeBody);
		}
		else
		{
			m_pReportControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
		}
	}
	else if (m_pReportControl->GetFooterRows()->HasFocus())
	{
		pNextRow = m_pReportControl->GetFooterRows()->GetNext(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());
		if (pNextRow != pFocusedRow)
			m_pReportControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
	}
	else
	{
		// body rows
		pNextRow = m_pReportControl->GetRows()->GetNext(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());
		if (pNextRow)
		{
			// from the last body row jump to the first header row
			if (m_pReportControl->GetRows()->m_nFocusedRow == pNextRow->GetIndex())
			{
				if (m_pReportControl->IsFooterRowsVisible() && m_pReportControl->IsFooterRowsAllowAccess())
				{
					MoveFirstVisibleRow(xtpRowTypeFooter);
				}
			}
			else
			{
				m_pReportControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
			}
		}
	}
	m_pReportControl->UnselectGroupRows();
}

void CXTPReportNavigator::MoveUp(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportRow* pPrevRow = NULL;
	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (m_pReportControl->GetHeaderRows()->HasFocus())
	{
		pPrevRow = m_pReportControl->GetHeaderRows()->GetPrev(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());
		if (pPrevRow != pFocusedRow)
			m_pReportControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
	}
	else if (m_pReportControl->GetFooterRows()->HasFocus())
	{
		pPrevRow = m_pReportControl->GetFooterRows()->GetPrev(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());

		// from the first footer row jump to the last visible body row
		if (pFocusedRow == pPrevRow)
			MoveLastVisibleRow(xtpRowTypeBody);
		else
			m_pReportControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
	}
	else
	{
		// body rows
		pPrevRow = m_pReportControl->GetRows()->GetPrev(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());

		if (pPrevRow)
		{
			if (m_pReportControl->GetRows()->m_nFocusedRow == pPrevRow->GetIndex())
			{
				if (m_pReportControl->IsHeaderRowsVisible() &&
					 m_pReportControl->IsHeaderRowsAllowAccess())
				{
					CXTPReportRow* pParRow = pFocusedRow->GetParentRow();
					while(pParRow)
					{
						m_pReportControl->EnsureVisible(pParRow);
						pParRow = pParRow->GetParentRow();
					}
					MoveLastVisibleRow(xtpRowTypeHeader);
				}
			}
			else
			{
				//if (!pPrevRow->IsSelected())// && !bControlKey && !bShiftKey)
				//  m_pReportControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
				m_pReportControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
			}
		}
	}
	m_pReportControl->UnselectGroupRows();
}

void CXTPReportNavigator::MoveDownStep(BOOL bShiftKey, BOOL bControlKey)
{
	UNREFERENCED_PARAMETER(bShiftKey);
	UNREFERENCED_PARAMETER(bControlKey);
}

void CXTPReportNavigator::MoveUpStep(BOOL bShiftKey, BOOL bControlKey)
{
	UNREFERENCED_PARAMETER(bShiftKey);
	UNREFERENCED_PARAMETER(bControlKey);
}

void CXTPReportNavigator::MovePageDown(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = CheckDeadEnd(TRUE);
	if (nCurrentRowIndex == -1)
		return;

	CXTPReportRow* pCurRow = m_pReportControl->GetRows()->GetAt(nCurrentRowIndex);
	if (pCurRow && pCurRow->IsGroupRow() && m_pReportControl->IsSkipGroupsFocusEnabled())
		pCurRow = m_pReportControl->GetRows()->GetPrev(pCurRow, m_pReportControl->IsSkipGroupsFocusEnabled());
	if (pCurRow)
		m_pReportControl->SetFocusedRow(pCurRow, bShiftKey, bControlKey);

	m_pReportControl->UnselectGroupRows();
}

void CXTPReportNavigator::MovePageUp(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = CheckDeadEnd(FALSE);
	if (nCurrentRowIndex == -1)
		return;

	CXTPReportRow* pCurRow = m_pReportControl->GetRows()->GetAt(nCurrentRowIndex);
	if (pCurRow && pCurRow->IsGroupRow() && m_pReportControl->IsSkipGroupsFocusEnabled())
		pCurRow = m_pReportControl->GetRows()->GetNext(pCurRow, m_pReportControl->IsSkipGroupsFocusEnabled());
	if (pCurRow)
		m_pReportControl->SetFocusedRow(pCurRow, bShiftKey, bControlKey);

	m_pReportControl->UnselectGroupRows();
}

void CXTPReportNavigator::MoveFirstRow(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportRow* pFirst = m_pReportControl->GetRows()->GetAt(0);
	if (m_pReportControl->IsVirtualMode())
		m_pReportControl->SetFocusedRow(pFirst, bShiftKey, bControlKey);
	else if (pFirst != m_pReportControl->GetFocusedRow())
		m_pReportControl->SetFocusedRow(pFirst, bShiftKey, bControlKey);

	m_pReportControl->UnselectGroupRows();
}

void CXTPReportNavigator::MoveLastRow(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportRow* pLast = m_pReportControl->GetRows()->GetAt(m_pReportControl->GetRows()->GetCount() - 1);
	if (m_pReportControl->IsVirtualMode())
		m_pReportControl->SetFocusedRow(pLast, bShiftKey, bControlKey);
	else if (pLast != m_pReportControl->GetFocusedRow())
		m_pReportControl->SetFocusedRow(pLast, bShiftKey, bControlKey);

	m_pReportControl->UnselectGroupRows();
}

void CXTPReportNavigator::MoveToRow(int nRowIndex, BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = max(0, nRowIndex);
	nCurrentRowIndex = min(nCurrentRowIndex, m_pReportControl->GetRows()->GetCount() - 1);
	if (nCurrentRowIndex < 0)
		return;

	m_pReportControl->SetFocusedRow(
		m_pReportControl->GetRows()->GetAt(nCurrentRowIndex),
		bShiftKey,
		bControlKey);
}


void CXTPReportNavigator::BeginEdit()
{
	if (!m_pReportControl)
		return;

	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (!m_pReportControl->IsVirtualMode())
	{
		m_pReportControl->AdjustScrollBars();
		m_pReportControl->RedrawControl();
		m_pReportControl->UpdateWindow();
	}
	if (m_pReportControl->m_pFocusedColumn &&
		pFocusedRow && pFocusedRow->GetRecord())
	{
		XTP_REPORTRECORDITEM_ARGS itemArgs(m_pReportControl, pFocusedRow, m_pReportControl->m_pFocusedColumn);

		if (itemArgs.pItem && itemArgs.pItem->IsAllowEdit(&itemArgs))
		{
			if (!m_pReportControl->IsVirtualMode())
				m_pReportControl->EnsureVisible(pFocusedRow);

			m_pReportControl->EditItem(&itemArgs);

			if (m_pReportControl->GetInplaceEdit()->GetSafeHwnd() &&
				m_pReportControl->GetInplaceEdit()->GetItem() == itemArgs.pItem)
			{
				CXTPReportRecordItemEditOptions* pEditOptions = itemArgs.pItem->GetEditOptions(itemArgs.pColumn);
				if (pEditOptions && pEditOptions->m_bSelectTextOnEdit)
				{
					m_pReportControl->GetInplaceEdit()->SetSel(0, -1);
				}
				else
				{
					CString str;
					m_pReportControl->GetInplaceEdit()->GetWindowText(str);
					m_pReportControl->GetInplaceEdit()->SetSel(str.GetLength(), str.GetLength());
				}
			}
		}
	}
}

void CXTPReportNavigator::MoveLeftRight(BOOL bBack, BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportControl::CUpdateContext updateContext(m_pReportControl);

	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	CXTPReportColumn* pFocusedColumn = m_pReportControl->GetNextFocusableColumn(pFocusedRow,
		m_pReportControl->m_pFocusedColumn ? m_pReportControl->m_pFocusedColumn->GetIndex() : -1,
		bBack ? -1 : 1);

	if (pFocusedColumn)
	{
		m_pReportControl->SetFocusedColumn(pFocusedColumn);
	}
	else
	{
		CXTPReportRows* pRows;
		int nFocusedRow = m_pReportControl->GetFocusedRow() ? m_pReportControl->GetFocusedRow()->GetIndex() : -1;
		switch (pFocusedRow->GetType())
		{
			case xtpRowTypeHeader : pRows = m_pReportControl->GetHeaderRows(); break;
			case xtpRowTypeFooter : pRows = m_pReportControl->GetFooterRows(); break;
			default : pRows = m_pReportControl->GetRows(); break;
		}
		CXTPReportRow* pRow = bBack ? pRows->GetPrev(pFocusedRow, FALSE) : pRows->GetNext(pFocusedRow, FALSE);
		if (pRow && pRow->GetIndex() != nFocusedRow)
		{
			m_pReportControl->SetFocusedRow(pRow, bShiftKey, bControlKey);
			m_pReportControl->SetFocusedColumn(
				m_pReportControl->GetNextFocusableColumn(
					m_pReportControl->GetFocusedRow(),
					bBack ? m_pReportControl->m_pColumns->GetCount() : -1,
					bBack ? -1 : +1));
			if (pRow->IsGroupRow() && m_pReportControl->IsSkipGroupsFocusEnabled())
				pRow->SetSelected(FALSE);
		}
	}
	m_pReportControl->UnselectGroupRows();
}

void CXTPReportNavigator::MoveLeft(BOOL bShiftKey, BOOL bControlKey)
{
	MoveLeftRight(TRUE, bShiftKey, bControlKey);
}

void CXTPReportNavigator::MoveRight(BOOL bShiftKey, BOOL bControlKey)
{
	MoveLeftRight(FALSE, bShiftKey, bControlKey);
}

void CXTPReportNavigator::MoveFirstColumn()
{
	if (!m_pReportControl)
		return;

	CXTPReportControl::CUpdateContext updateContext(m_pReportControl);
	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	CXTPReportColumn* pFocusedColumn = m_pReportControl->GetNextFocusableColumn(pFocusedRow, -1, +1);
	if (pFocusedColumn)
		m_pReportControl->SetFocusedColumn(pFocusedColumn);
}

void CXTPReportNavigator::MoveLastColumn()
{
	if (!m_pReportControl)
		return;

	CXTPReportControl::CUpdateContext updateContext(m_pReportControl);
	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	CXTPReportColumn* pFocusedColumn = m_pReportControl->GetNextFocusableColumn(pFocusedRow, m_pReportControl->GetColumns()->GetCount(), -1);
	if (pFocusedColumn)
	{
		m_pReportControl->SetFocusedColumn(pFocusedColumn);
	}
}

void CXTPReportNavigator::MoveToColumn(int nColumnIndex, BOOL bClearIfNonFocusable)
{
	if (!m_pReportControl)
		return;

	nColumnIndex = max(0, nColumnIndex);
	nColumnIndex = min(nColumnIndex, m_pReportControl->GetColumns()->GetCount()-1);
	if (nColumnIndex < 0)
		return;

	CXTPReportControl::CUpdateContext updateContext(m_pReportControl);

	CXTPReportColumn* pColumn = m_pReportControl->GetColumns()->GetAt(nColumnIndex);
	if (!pColumn)
		return;

	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (pFocusedRow->GetRecord())
	{
		CXTPReportRecordItem* pItem = pFocusedRow->GetRecord()->GetItem(pColumn);

		if (!pItem || !pItem->IsFocusable())
		{
			if (bClearIfNonFocusable)
				pColumn = NULL;
			else
				return;
		}

		m_pReportControl->SetFocusedColumn(pColumn);
	}
}

void CXTPReportNavigator::SetCurrentFocusInHeadersRows(BOOL bCurrentFocusInHeadersRows)
{
	if (!m_pReportControl->m_pSectionHeader->IsVisible() ||
		 !m_pReportControl->m_pSectionHeader->IsAllowAccess())
	{
		bCurrentFocusInHeadersRows = FALSE;
	}

	if (bCurrentFocusInHeadersRows)
	{
		MoveFirstVisibleRow(xtpRowTypeHeader);
	}
	else if (!m_pReportControl->GetFooterRows()->HasFocus() &&
				 m_pReportControl->m_pSectionHeader->IsAllowAccess())
	{
		MoveFirstVisibleRow(xtpRowTypeBody);    // neither header nor footer is active
	}
}

void CXTPReportNavigator::SetCurrentFocusInFootersRows(BOOL bCurrentFocusInFootersRows)
{
	if (!m_pReportControl->m_pSectionFooter->IsVisible() ||
		 !m_pReportControl->m_pSectionFooter->IsAllowAccess())
	{
		bCurrentFocusInFootersRows = FALSE;
	}

	if (bCurrentFocusInFootersRows)
	{
		MoveFirstVisibleRow(xtpRowTypeFooter);
	}
	else if (!m_pReportControl->GetHeaderRows()->HasFocus() &&
				 m_pReportControl->m_pSectionFooter->IsAllowAccess())
	{
		MoveFirstVisibleRow(xtpRowTypeBody);    // neither header nor footer is active
	}
}

BOOL CXTPReportNavigator::GetCurrentFocusInHeadersRows()
{
	return m_pReportControl->GetHeaderRows()->HasFocus();
}

BOOL CXTPReportNavigator::GetCurrentFocusInFootersRows()
{
	return m_pReportControl->GetFooterRows()->HasFocus();
}

void CXTPReportNavigator::MoveFirstVisibleRow(XTPReportRowType TargetType)
{
	CXTPReportRows *pRows = NULL;

	switch (TargetType)
	{
		case xtpRowTypeBody:
		{
			pRows = m_pReportControl->GetRows();
			CXTPReportRow* pTopRow = pRows->GetAt(m_pReportControl->GetTopRowIndex());
			CXTPReportRow* pNextRow = NULL;
			if (pTopRow && pTopRow->IsGroupRow() && m_pReportControl->IsSkipGroupsFocusEnabled())
				pNextRow = pRows->GetNext(pTopRow, m_pReportControl->IsSkipGroupsFocusEnabled());
			if (pNextRow && pNextRow != pTopRow && pTopRow->IsExpanded())
				m_pReportControl->SetFocusedRow(pNextRow);
			else
				m_pReportControl->SetFocusedRow(pTopRow);
		}
		break;

		case xtpRowTypeHeader:
		{
			pRows = m_pReportControl->GetHeaderRows();
			if (pRows && pRows->GetCount() > 0)
			{
				CXTPReportRow* pFirst = pRows->GetAt(0);
				if (pFirst != m_pReportControl->GetFocusedRow())
					m_pReportControl->SetFocusedRow(FALSE, pFirst, FALSE, FALSE);
			}
		}
		break;

		case xtpRowTypeFooter:
		{
			pRows = m_pReportControl->GetFooterRows();
			if (pRows && pRows->GetCount() > 0)
			{
				CXTPReportRow* pFirst = pRows->GetAt(0);
				if (pFirst != m_pReportControl->GetFocusedRow())
				{
					m_pReportControl->SetFocusedRow(FALSE, pFirst, FALSE, FALSE);
				}
			}
		}
		break;
	}
	m_pReportControl->UnselectGroupRows();
}

void CXTPReportNavigator::MoveLastVisibleRow(XTPReportRowType TargetType)
{
	CXTPReportRows *pRows = NULL;

	switch (TargetType)
	{
		case xtpRowTypeBody:
		{
			pRows = m_pReportControl->GetRows();
			int nRows = m_pReportControl->GetReportAreaRows(m_pReportControl->GetTopRowIndex(), TRUE);

			if (nRows > -1 && pRows->GetCount()>0)
			{
				int nIdx = min(m_pReportControl->GetTopRowIndex() + nRows, pRows->GetCount() - 1);

				m_pReportControl->SetFocusedRow(pRows->GetAt(nIdx));
			}
		}
		break;

		case xtpRowTypeHeader:
		{
			pRows = m_pReportControl->GetHeaderRows();
			if (pRows && pRows->GetCount() > 0)
			{
				CXTPReportRow* pLast = pRows->GetAt(pRows->GetCount() - 1);
				if (pLast != m_pReportControl->GetFocusedRow())
					m_pReportControl->SetFocusedRow(pLast);
			}
		}
		break;

		case xtpRowTypeFooter:
		{
			pRows = m_pReportControl->GetFooterRows();
			if (pRows && pRows->GetCount() > 0)
			{
				CXTPReportRow* pLast = pRows->GetAt(pRows->GetCount() - 1);
				if (pLast != m_pReportControl->GetFocusedRow())
					m_pReportControl->SetFocusedRow(pLast);
			}
		}
		break;
	}
	m_pReportControl->UnselectGroupRows();
}

int CXTPReportNavigator::CheckDeadEnd(BOOL bMoveDown)
{
	int nCurrentRowIndex = m_pReportControl->GetRows()->m_nFocusedRow != -1 ? m_pReportControl->GetRows()->m_nFocusedRow : 0;
	int nDelta = m_pReportControl->GetReportAreaRows(nCurrentRowIndex, bMoveDown);
	if (nDelta == 0)
	{
		m_pReportControl->UnselectGroupRows();
		return -1;
	}
	if (bMoveDown)
		return min(m_pReportControl->GetRows()->GetCount() - 1, nCurrentRowIndex + nDelta);
	else
		return max(0, nCurrentRowIndex - nDelta);
}



//////////////////////////////////////////////////////////////////////////

#ifdef _XTP_ACTIVEX

#define IMPLEMENT_OLE_MOVE(Direction) \
void CXTPReportNavigator::OleMove##Direction(const VARIANT& oleSelectBlock, const VARIANT& oleIgnoreSelection) \
{ \
	BOOL bShiftKey = VariantToBool(&oleSelectBlock); \
	BOOL bControlKey = VariantToBool(&oleIgnoreSelection); \
	Move##Direction(bShiftKey, bControlKey);    \
}

IMPLEMENT_OLE_MOVE(Up)
IMPLEMENT_OLE_MOVE(Down)
IMPLEMENT_OLE_MOVE(PageUp)
IMPLEMENT_OLE_MOVE(PageDown)
IMPLEMENT_OLE_MOVE(FirstRow)
IMPLEMENT_OLE_MOVE(LastRow)
IMPLEMENT_OLE_MOVE(Left)
IMPLEMENT_OLE_MOVE(Right)

BEGIN_DISPATCH_MAP(CXTPReportNavigator, CCmdTarget)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveUp",      1, OleMoveUp, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveDown",    2, OleMoveDown, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MovePageUp",  3, OleMovePageUp, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MovePageDown",4, OleMovePageDown, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveFirstRow",5, OleMoveFirstRow, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveLastRow", 6, OleMoveLastRow, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveLeft",    7, OleMoveLeft, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveRight",   8, OleMoveRight, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "BeginEdit",   9, BeginEdit, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveFirstColumn", 10, MoveFirstColumn, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveLastColumn",  11, MoveLastColumn, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveToRow",   12, OleMoveToRow, VT_EMPTY, VTS_I4 VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveToColumn",13, OleMoveToColumn, VT_EMPTY, VTS_I4 VTS_VARIANT)

	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveFirstVisibleRow", 14, OleMoveFirstVisibleRow, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportNavigator, "MoveLastVisibleRow", 15, OleMoveLastVisibleRow, VT_EMPTY, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPReportColumn, "CurrentFocusInHeadersRows", 16, GetCurrentFocusInHeadersRows, SetCurrentFocusInHeadersRows, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPReportColumn, "CurrentFocusInFootersRows", 17, GetCurrentFocusInFootersRows, SetCurrentFocusInFootersRows, VT_BOOL)

END_DISPATCH_MAP()

// {18B11C4D-E540-4f68-8B20-DDC0E2FE8FA4}
static const GUID IID_IReportNavigator =
{ 0x18b11c4d, 0xe540, 0x4f68, { 0x8b, 0x20, 0xdd, 0xc0, 0xe2, 0xfe, 0x8f, 0xa4 } };

BEGIN_INTERFACE_MAP(CXTPReportNavigator, CCmdTarget)
	INTERFACE_PART(CXTPReportNavigator, IID_IReportNavigator, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportNavigator, IID_IReportNavigator)

void CXTPReportNavigator::OleMoveToRow(int nRowIndex, const VARIANT& oleSelectBlock,
	const VARIANT& oleIgnoreSelection)
{
	BOOL bShiftKey = VariantToBool(&oleSelectBlock);
	BOOL bControlKey = VariantToBool(&oleIgnoreSelection);

	MoveToRow(nRowIndex, bShiftKey, bControlKey);
}

void CXTPReportNavigator::OleMoveToColumn(int nColumnIndex,
	const VARIANT& oleClearIfNonFocusable)
{
	BOOL bClearIfNonFocusable = VariantToBool(&oleClearIfNonFocusable);

	MoveToColumn(nColumnIndex, bClearIfNonFocusable);
}

void CXTPReportNavigator::OleMoveFirstVisibleRow()
{
	if (m_pReportControl->GetHeaderRows()->HasFocus() && m_pReportControl->m_pSectionHeader->IsVisible())
	{
		MoveFirstVisibleRow(xtpRowTypeHeader);
	}
	else
	{
		MoveFirstVisibleRow(xtpRowTypeBody);
	}
}

void CXTPReportNavigator::OleMoveLastVisibleRow()
{
	if (m_pReportControl->GetFooterRows()->HasFocus() && m_pReportControl->m_pSectionFooter->IsVisible())
	{
		MoveLastVisibleRow(xtpRowTypeFooter);
	}
	else
	{
		MoveLastVisibleRow(xtpRowTypeBody);
	}
}

#endif
