// XTPReportIconNavigator.cpp : implementation of the CXTPReportIconNavigator class.
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

//////////////////////////////////////////////////////////////////////////
// CXTPReportIconNavigator

CXTPReportIconNavigator::CXTPReportIconNavigator(CXTPReportControl *pReportControl)
	: CXTPReportNavigator(pReportControl)
{
}

CXTPReportIconNavigator::~CXTPReportIconNavigator()
{
}

void CXTPReportIconNavigator::MoveDown(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportRow* pNextRow = NULL;
	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	int iPrevRowIndex = -1;
	int iFocusRow = pFocusedRow->GetIndex();
	int nRpL = m_pReportControl->GetRowsPerLine();
	int iRowOffset = iFocusRow % nRpL;
// body rows
	pNextRow = m_pReportControl->GetRows()->GetNext(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());

	if (pNextRow)
	{
		while(
		pNextRow
		&& pNextRow->GetIndex() != iPrevRowIndex
		&& iRowOffset != pNextRow->GetIndex() % nRpL)
		{
			iPrevRowIndex = pNextRow->GetIndex();
			pNextRow = m_pReportControl->GetRows()->GetNext(pNextRow, m_pReportControl->IsSkipGroupsFocusEnabled());
		}

		if (pNextRow && pNextRow->GetIndex() != iPrevRowIndex)
		{
// from the last body row jump to the first header row
			if (iFocusRow >= pNextRow->GetIndex())
			{
				if (m_pReportControl->IsFooterRowsVisible() && m_pReportControl->IsFooterRowsAllowAccess())
					MoveFirstVisibleRow(xtpRowTypeFooter);
			}
			else
			{
				m_pReportControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
			}
		}
	}
}


void CXTPReportIconNavigator::MoveUp(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportRow* pPrevRow = NULL;
	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	int iPrevRowIndex = -1;
	int iFocusRow = pFocusedRow->GetIndex();
	int nRpL = m_pReportControl->GetRowsPerLine();
	int iRowOffset = iFocusRow % nRpL;
// body rows
	pPrevRow = m_pReportControl->GetRows()->GetPrev(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());
	if (pPrevRow)
	{
		while(
		pPrevRow
		&& pPrevRow->GetIndex() != iPrevRowIndex
		&& iRowOffset != pPrevRow->GetIndex() % nRpL)
		{
			iPrevRowIndex = pPrevRow->GetIndex();
			pPrevRow = m_pReportControl->GetRows()->GetPrev(pPrevRow, m_pReportControl->IsSkipGroupsFocusEnabled());
		}
		if (pPrevRow && pPrevRow->GetIndex() != iPrevRowIndex)
		{
// from the first body row jump to the last header row
			if (iFocusRow <= pPrevRow->GetIndex())
			{
				if (m_pReportControl->IsHeaderRowsVisible() && m_pReportControl->IsHeaderRowsAllowAccess())
					MoveLastVisibleRow(xtpRowTypeHeader);
			}
			else
			{
				m_pReportControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
			}
		}
	}
}


void CXTPReportIconNavigator::MoveDownStep(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportRow* pNextRow = NULL;
	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (m_pReportControl->IsIconView())
	{
		int iPrevRowIndex = -1;
		int iFocusRow = pFocusedRow->GetIndex();
// body rows
		pNextRow = m_pReportControl->GetRows()->GetNext(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());
		if (pNextRow)
		{
			if (pNextRow && pNextRow->GetIndex() != iPrevRowIndex)
			{
// from the last body row jump to the first header row
				if (iFocusRow >= pNextRow->GetIndex())
				{
					if (m_pReportControl->IsFooterRowsVisible() && m_pReportControl->IsFooterRowsAllowAccess())
						MoveFirstVisibleRow(xtpRowTypeFooter);
				}
				else
				{
					m_pReportControl->SetFocusedRow(pNextRow, bShiftKey, bControlKey);
				}
			}
		}
	}
}

void CXTPReportIconNavigator::MoveUpStep(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	CXTPReportRow* pPrevRow = NULL;
	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	if (m_pReportControl->IsIconView())
	{
		int iPrevRowIndex = -1;
		int iFocusRow = pFocusedRow->GetIndex();
// body rows
		pPrevRow = m_pReportControl->GetRows()->GetPrev(pFocusedRow, m_pReportControl->IsSkipGroupsFocusEnabled());
		if (pPrevRow)
		{
			if (pPrevRow && pPrevRow->GetIndex() != iPrevRowIndex)
			{
// from the first body row jump to the last header row
				if (iFocusRow <= pPrevRow->GetIndex())
				{
					if (m_pReportControl->IsHeaderRowsVisible() && m_pReportControl->IsHeaderRowsAllowAccess())
						MoveLastVisibleRow(xtpRowTypeHeader);
				}
				else
				{
					m_pReportControl->SetFocusedRow(pPrevRow, bShiftKey, bControlKey);
				}
			}
		}
	}
}

void CXTPReportIconNavigator::MovePageDown(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = m_pReportControl->GetFocusedRow() ?
		m_pReportControl->GetFocusedRow()->GetIndex() : 0;
	int iNextRowIndex = nCurrentRowIndex;
	while(iNextRowIndex % m_pReportControl->GetRowsPerLine())
	{
	  iNextRowIndex--;
	}
	iNextRowIndex = min(
		m_pReportControl->GetRows()->GetCount() - 1,
		nCurrentRowIndex + m_pReportControl->GetReportAreaRows(iNextRowIndex, TRUE));

	int nRpL = m_pReportControl->GetRowsPerLine();
// Now go backwards until we get the same equivalency class.
	while(
	nCurrentRowIndex < iNextRowIndex
	&& (nCurrentRowIndex % nRpL) !=
	(iNextRowIndex % nRpL))
	{
		iNextRowIndex--;
	}
	if (nCurrentRowIndex == iNextRowIndex)
	{
		MoveLastRow(bShiftKey, bControlKey);
	}
	else
	{
		m_pReportControl->SetFocusedRow(
		m_pReportControl->GetRows()->GetAt(iNextRowIndex),
		bShiftKey,
		bControlKey);
	}
}


void CXTPReportIconNavigator::MovePageUp(BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	int nCurrentRowIndex = m_pReportControl->GetFocusedRow() ?
		m_pReportControl->GetFocusedRow()->GetIndex() : 0;
	int iNextRowIndex = nCurrentRowIndex;
	while(iNextRowIndex % m_pReportControl->GetRowsPerLine())
	{
	  iNextRowIndex--;
	}
	iNextRowIndex = max(0, nCurrentRowIndex - m_pReportControl->GetReportAreaRows(iNextRowIndex, FALSE));

	int nRpL = m_pReportControl->GetRowsPerLine();
// Now go forward until we get the same equivalency class.
	while(
	nCurrentRowIndex > iNextRowIndex
	&& (nCurrentRowIndex % nRpL) != (iNextRowIndex % nRpL))
	{
		iNextRowIndex++;
	}

	if (nCurrentRowIndex == iNextRowIndex)
	{
		MoveFirstRow(bShiftKey, bControlKey);
	}
	else
	{
		m_pReportControl->SetFocusedRow(
		m_pReportControl->GetRows()->GetAt(iNextRowIndex),
		bShiftKey,
		bControlKey);
	}
}


void CXTPReportIconNavigator::BeginEdit()
{
	if (!m_pReportControl)
		return;

	CXTPReportRow* pFocusedRow = m_pReportControl->GetFocusedRow();
	if (!pFocusedRow)
		return;

	CXTPReportColumn* pIconColumn = m_pReportControl->GetColumns()->Find(m_pReportControl->m_iIconViewColumn);

	if (pIconColumn &&
		pFocusedRow && pFocusedRow->GetRecord())
	{
		XTP_REPORTRECORDITEM_ARGS itemArgs(m_pReportControl, pFocusedRow, pIconColumn);

		if (itemArgs.pItem && itemArgs.pItem->IsAllowEdit(&itemArgs))
			m_pReportControl->EditItem(&itemArgs);
	}
}


void CXTPReportIconNavigator::MoveLeftRight(BOOL bBack, BOOL bShiftKey, BOOL bControlKey)
{
	if (!m_pReportControl)
		return;

	if (m_pReportControl->IsMultiSelectionMode())
		bControlKey = TRUE;

	if (bBack)
		MoveUpStep(bShiftKey, bControlKey);
	else
		MoveDownStep(bShiftKey, bControlKey);
}
