// XTPReportRow.cpp : implementation of the CXTPReportRow class.
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

#include "Common/Resource.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPReportDefines.h"
#include "XTPReportRecordItemRange.h"
#include "XTPReportRecord.h"
#include "XTPReportControl.h"
#include "XTPReportPaintManager.h"
#include "XTPReportRow.h"
#include "XTPReportRows.h"
#include "XTPReportSelectedRows.h"
#include "XTPReportColumns.h"
#include "XTPReportColumn.h"
#include "XTPReportTip.h"
#include "XTPReportHeader.h"
#include "XTPReportSection.h"
#include "XTPReportBorder.h"
#include "XTPReportRecordItem.h"
#include "ItemTypes/XTPReportRecordItemPreview.h"
#include "ItemTypes/XTPReportRecordItemIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPReportRow, CCmdTarget)
/////////////////////////////////////////////////////////////////////////////
// CXTPReportRow

CXTPReportRow::CXTPReportRow()
	: m_pParentRows(NULL)
	, m_pParentRow (NULL)
	, m_pRecord    (NULL)
	, m_pChilds    (NULL)
	, m_pControl   (NULL)
	, m_pSection   (NULL)

	, m_nIndex     (-1)
	, m_nChildIndex(-1)

	, m_nGroupLevel (0)
	, m_nRowLevel   (0)

	, m_bVisible   (FALSE)
	, m_bExpanded  (TRUE)
	, m_bLockExpand(FALSE)
{


	m_rcRow.SetRectEmpty();
	m_rcCollapse.SetRectEmpty();
	m_bHasSelectedChilds = FALSE;

	m_nPreviewHeight = 0;
	m_nFreeHeight = 0;

	EnableAutomation();
}

void CXTPReportRow::InitRow(
	CXTPReportControl *pControl,
	CXTPReportSection *pSection,
	CXTPReportRecord  *pRecord)
{
	ASSERT(pRecord || IsGroupRow());

	m_pControl = pControl;
	m_pSection = pSection;

	if (m_pControl->m_bFreeHeightMode)
		m_nFreeHeight = m_pControl->m_nDefaultRowFreeHeight;

	if (pRecord)
	{
		m_pRecord = pRecord;
		m_bExpanded = pRecord->m_bExpanded;
		m_pRecord->InternalAddRef();
	}
}

void CXTPReportRow::InitRow(CXTPReportRow *pRow)
{
	ASSERT(pRow->m_pRecord);
	ASSERT(pRow->m_pParentRow == NULL);

	m_pControl = pRow->m_pControl;
	m_pRecord  = pRow->m_pRecord;
	m_pSection = pRow->m_pSection;

	if (NULL != m_pRecord)
	{
		m_pRecord->InternalAddRef();
	}

	m_rcRow          = pRow->m_rcRow;
	m_rcCollapse     = pRow->m_rcCollapse;
	m_bVisible       = pRow->m_bVisible;
	m_bExpanded      = pRow->m_bExpanded;
	m_nIndex         = pRow->m_nIndex;
	m_nPreviewHeight = pRow->m_nPreviewHeight;
	m_nRowLevel      = pRow->m_nRowLevel;
	m_nGroupLevel    = pRow->m_nGroupLevel;

	if (m_pControl->m_bFreeHeightMode)
		m_nFreeHeight = m_pControl->m_nDefaultRowFreeHeight;
}


CXTPReportRows* CXTPReportRow::GetChilds(BOOL bCreate)
{
	if (!m_pChilds && bCreate)
		m_pChilds = new CXTPReportRows();

	return m_pChilds;
}



CXTPReportRow::~CXTPReportRow()
{
	if (m_pChilds)
		m_pChilds->InternalRelease();

	if (m_pRecord)
		m_pRecord->InternalRelease();
}

int CXTPReportRow::GetHeight(CDC *pDC, int nWidth)
{
	ASSERT(m_pControl);
	if (!m_pControl)
		return 0;

	int nHeight = m_pControl->GetPaintManager()->GetRowHeight(pDC, this, nWidth);

	if (!IsGroupRow() && !IsItemsVisible())
	{
		nHeight = 0;
	}


	m_nPreviewHeight = 0;

	if (IsPreviewVisible())
	{
		CXTPReportRecordItemPreview *pItem = GetRecord()->GetItemPreview();

		m_nPreviewHeight = pItem->GetPreviewHeight(pDC, this, nWidth);
		m_nPreviewHeight = m_pControl->GetPaintManager()->GetPreviewItemHeight(pDC, this, nWidth, m_nPreviewHeight);
	}

	return nHeight + m_nPreviewHeight;
}


BOOL CXTPReportRow::IsFocused() const
{
	return m_pSection->GetRows()->GetFocusedRowIndex() == m_nIndex;
}

BOOL CXTPReportRow::IsSelected() const
{
	return m_pControl->GetSelectedRows()->Contains(this);
}

void CXTPReportRow::SetSelected(BOOL bSelected)
{
	if (bSelected == IsSelected())
		return;

	if (bSelected)
	{
		if (!m_pControl->IsMultipleSelection())
			m_pControl->GetSelectedRows()->Clear();

		m_pControl->GetSelectedRows()->Add(this);
	}
	else
	{
		m_pControl->GetSelectedRows()->Remove(this);
	}

	m_pControl->RedrawControl();
}

int CXTPReportRow::GetLastChildRow(CXTPReportRows* pChilds) const
{
	CXTPReportRow* pRow = pChilds->GetAt(pChilds->GetCount() - 1);

	if (!pRow)
		return -1;

	if (pRow->HasChildren() && pRow->IsExpanded())
		return GetLastChildRow(pRow->GetChilds());

	return pRow->GetIndex();
}

void CXTPReportRow::SelectChilds()
{
	BOOL bM = m_pControl->IsMultipleSelection();
	BOOL bC = HasChildren();
	BOOL bE = IsExpanded();
	if (!(bM && bC && bE && m_nIndex != -1))
		return;

	m_pControl->BeginUpdate();

	int nIndexBegin = m_nIndex + 1;
	int nIndexEnd = GetLastChildRow(GetChilds());

	m_pControl->GetSelectedRows()->AddBlock(nIndexBegin, nIndexEnd);

	m_pControl->EndUpdate();
}

BOOL CXTPReportRow::IsItemsVisible() const
{
	return TRUE;
}


BOOL CXTPReportRow::IsPreviewVisible() const
{
	return !IsGroupRow()
		&& m_pRecord
		&& m_pControl->IsPreviewMode()
		&& !m_pControl->IsIconView()
		&& m_pRecord->GetItemPreview();
}


void CXTPReportRow::DrawItemGrid(CDC *pDC, CXTPReportColumn *pColumn, CRect rcItem)
{
	CXTPReportPaintManager *pPaintManager = m_pControl->GetPaintManager();

	const BOOL bGridVisible = pPaintManager->IsGridVisible(FALSE);

	rcItem.DeflateRect(0,0,0,1);

	// Draw horizontal grid
	if (bGridVisible && !IsPreviewVisible())
	{
		pPaintManager->DrawGrid(pDC, xtpReportOrientationHorizontal, rcItem);
	}

	// Draw vertical grid
	switch(GetType())
	{
		case xtpRowTypeBody:
		{
			pPaintManager->DrawGrid(pDC, xtpReportOrientationVertical, rcItem);
		}
		break;

		case xtpRowTypeHeader:
		{
			if (pColumn->GetDrawHeaderRowsVGrid())
			{
				pPaintManager->DrawGrid(pDC, xtpReportOrientationVertical, rcItem);
			}
		}
		break;

		case xtpRowTypeFooter:
		{
			if (pColumn->GetDrawFooterRowsVGrid())
			{
				pPaintManager->DrawGrid(pDC, xtpReportOrientationVertical, rcItem);
			}
		}
		break;
	} // switch
}


BOOL AdjustClipRect(CDC *pDC, CRect &rcClip)
{
	if (NULL != pDC && pDC->IsKindOf(RUNTIME_CLASS(CPreviewDC)))
	{
		CPreviewDC *pPreviewDC = DYNAMIC_DOWNCAST(CPreviewDC, pDC);

		pPreviewDC->PrinterDPtoScreenDP(&rcClip.TopLeft());
		pPreviewDC->PrinterDPtoScreenDP(&rcClip.BottomRight());

		// Offset the result by the viewport
		// origin of the print preview window
		CPoint ptOrg;

		// Note: pDC->GetViewportOrg() uses the m_hAttributDC
		::GetViewportOrgEx(pDC->m_hDC, &ptOrg);
		rcClip += ptOrg;
	}

	return TRUE;
}


void CXTPReportRow::Draw(CDC *pDC, CRect rcRow, CRect rcClip, int nLeftOffset,
	CXTPReportRecordMergeItems &mergeItems, int nColumnFrom, int nColumnTo)
{
	// Check preconditions
	ASSERT(NULL != m_pControl);

	if (NULL == m_pControl)
	{
		return;
	}

	const BOOL bIsPrinting = pDC->IsPrinting();
	const int nFreezeCount = m_pControl->GetFreezeColumnsCount();
	const int nFreezeIndex = m_pControl->GetFreezeColumnsIndex();
	const int nHeaderIndent = m_pControl->GetHeaderIndent();

	// Paint Manager
	CXTPReportPaintManager *pPaintManager = m_pControl->GetPaintManager();
	const BOOL bGridVisible = pPaintManager->IsGridVisible(xtpReportOrientationHorizontal);

	int nIconColumnIndex = -1;
	CXTPReportColumn* pRecNumCol = m_pControl->GetColumns()->Find(m_pControl->m_iColumnForNum);
	if (pRecNumCol)
		nIconColumnIndex = pRecNumCol->GetIndex();

	m_rcRow        = rcRow;
	m_rcRow.left  -= nLeftOffset;
	m_rcRow.right -= nLeftOffset;

	if (nFreezeCount == 0)
		rcRow = m_rcRow;

	XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
	drawArgs.pDC      = pDC;
	drawArgs.pControl = m_pControl;
	drawArgs.pRow     = this;

	CRect rcIndent(nFreezeCount ? rcRow : m_rcRow);
	rcIndent.right = rcIndent.left + nHeaderIndent;

	CRect rcItem(m_rcRow);
	rcItem.bottom -= m_nPreviewHeight; // Deduct preview item

	int nFreezeRight = 0;

	if (nFreezeCount > 0)
	{
		CXTPReportColumn *pColumn = m_pControl->GetColumns()->GetAt(nFreezeIndex);
		nFreezeRight = pColumn->GetRect().right;
	}

	if (NULL != m_pRecord) // if drawing record, not group
	{
		if (bIsPrinting)
		{
			rcItem.right = rcRow.left;
		}

		///////////////////////////////////////////////////////////////////////
		// Paint record items
		///////////////////////////////////////////////////////////////////////
		CRect rcClipRow;   // Clipping rectangle for row
		CRect rcClipItem;  // Clipping rectangle for item

		BOOL bFirstColumn = TRUE;

		int nColumnPos = -1;

		for (int nColumn=nColumnFrom; nColumn<nColumnTo; nColumn++)
		{
			CXTPReportColumn *pColumn = m_pControl->GetColumns()->GetAt(nColumn);

			if (NULL != pColumn && pColumn->IsVisible() && IsItemsVisible())
			{
				++nColumnPos;

				/////////////////////////////////////////////////////////////////
				// Determine item position
				/////////////////////////////////////////////////////////////////

				rcItem.bottom  = m_rcRow.bottom;
				rcItem.bottom -= m_nPreviewHeight;

				if (bIsPrinting) // Printing
				{
					if (pPaintManager->IsColumnWidthWYSIWYG())
					{
						rcItem.left  = rcItem.right;
						rcItem.right = rcItem.left + pColumn->GetWidth();
					}
					else
					{
						rcItem.left  = rcItem.right;
						rcItem.right = rcItem.left + pColumn->GetPrintWidth(rcRow.Width());
					}
				}
				else // Drawing
				{
					rcItem.left  = pColumn->GetRect().left;
					rcItem.right = pColumn->GetRect().right;
				}

				// Offset first column by header indentation
				if (bFirstColumn)
				{
					bFirstColumn = FALSE;
					rcItem.left += nHeaderIndent;
				}

				/////////////////////////////////////////////////////////////////
				//
				/////////////////////////////////////////////////////////////////
				BOOL bDrawFreeze = nColumn<=nFreezeIndex;

				if (bDrawFreeze)
				{
					// Frozen column
					rcClipRow.SetRect(rcRow.left, rcRow.top, nFreezeRight, rcRow.bottom);
				}
				else
				{
					// Non-frozen column
					rcClipRow.SetRect(nFreezeRight, rcRow.top, rcRow.right, rcRow.bottom);
				}

				// Determine clipping rectangle
				rcClipItem.IntersectRect(rcItem, rcClip);        // Clip section
				rcClipItem.IntersectRect(rcClipItem, rcClipRow); // Clip row

				// In print preview mode the clipping rectangle needs to be
				// adjusted before creating the clipping region
				//AdjustClipRect(pDC, rcClipItem);


				if (!bDrawFreeze && !bIsPrinting && rcItem.right < nFreezeRight)
				{
					// Column is entirely under frozen column, do not draw
					continue;
				}

				if (!CRect().IntersectRect(rcClip, rcItem) && !bIsPrinting)
					continue;

				// Create clipping region
				CRgn rgnClipItem;
				if (!bIsPrinting)
				{
					rgnClipItem.CreateRectRgnIndirect(rcClipItem);
					pDC->SelectClipRgn(&rgnClipItem, RGN_COPY);
				}

				CXTPReportRecordItem *pItem = m_pRecord->GetItem(pColumn);

#if 0
				CXTPReportBorder *pBorder = pPaintManager->GetItemBorder(pItem);

				if (NULL != pBorder)
				{
					pBorder->Draw(pDC, rcItem);
					rcItem = CXTPReportBorder::DeflateRect(rcItem, pBorder);
				}
#endif
				// Draw row background
				pPaintManager->FillRow(pDC, this, rcItem);

				// Draw shade background if sorted by this row
				if (pColumn->IsSorted())
				{
					pPaintManager->FillItemShade(pDC, rcItem);
				}

				if (NULL != pItem)
				{
					// draw item
					drawArgs.pColumn    = pColumn;
					drawArgs.rcItem     = rcItem;
					drawArgs.nTextAlign = pColumn->GetAlignment();
					drawArgs.pItem      = pItem;

					if (!pItem->IsMerged())
					{
						int nItemTextWidth = pItem->Draw(&drawArgs);
						pColumn->m_nMaxItemWidth = max(pColumn->m_nMaxItemWidth, nItemTextWidth);
					}
					else
					{
						CXTPReportRecordItem *pMergeItem = pItem->GetMergeItem();
						CXTPReportRecordMergeItemId itemId(nColumnPos, nColumn, GetIndex());
						mergeItems[pMergeItem].AddItem(itemId, rcItem);
					}
				}
				else if (m_pControl->IsIconView() && !m_pControl->IsVirtualMode())
				{
					// IconView - non Virtual Mode!
					pItem = new CXTPReportRecordItemIcon();
					drawArgs.pColumn = pColumn;
					drawArgs.rcItem = rcItem;
					CXTPReportColumn* pDataColumn = m_pControl->GetColumns()->GetAt(m_pControl->m_iIconPropNum);
					CXTPReportRecordItem* pDataItem = GetRecord()->GetItem(m_pControl->m_iIconPropNum);
					if (pDataColumn != NULL && pDataItem != NULL)
					{
						pItem->SetCaption(pDataItem->GetCaption(pDataColumn));
						pItem->SetIconIndex(m_pControl->m_iIconNum);
					}
					drawArgs.pItem = pItem;
					pItem->Draw(&drawArgs);
					delete pItem;
					pItem = NULL;
				}
				else if (nIconColumnIndex > -1
					&& !m_pControl->IsIconView()
					&& !m_pControl->IsVirtualMode()) //no Item! (ghost)
				{
					CRect rcIcon = m_pControl->GetColumns()->GetAt(nIconColumnIndex)->GetRect();
					rcIcon.top = m_rcRow.top;
					rcIcon.bottom = m_rcRow.bottom - m_nPreviewHeight;
					rcIcon.left = 0;
					pPaintManager->DrawRowNumber(pDC, rcIcon, this);
				}
#if 1
				if (pItem && !pItem->IsMerged())
				{
					CRect rcGridItem(rcItem);
					rcGridItem.left--;
					DrawItemGrid(pDC, pColumn, rcGridItem);
				}
#endif
				// Release clipping
				if (!bIsPrinting)
				{
					pDC->SelectClipRgn(NULL, RGN_COPY);
					rgnClipItem.DeleteObject();
				}

				if (nColumn == nFreezeIndex)
				{
					pPaintManager->DrawFreezeColsDivider(pDC, rcItem, GetControl(), this);
				}
			}
		} // for (nColumn)

		if (IsPreviewVisible())
		{
			CRect rcPreviewItem(m_rcRow);
			rcPreviewItem.DeflateRect(nHeaderIndent, rcPreviewItem.Height() - m_nPreviewHeight, 0, 0);

			drawArgs.rcItem     = rcPreviewItem;
			drawArgs.nTextAlign = DT_LEFT;
			drawArgs.pColumn    = NULL;
			drawArgs.pItem      = m_pRecord->GetItemPreview();

			// Determine clipping rectangle
			rcClipItem.IntersectRect(rcPreviewItem, rcClip); // Clip section
			rcClipItem.IntersectRect(rcClipItem, rcClipRow); // Clip row

			CRgn rgnClipItem;
			if (!bIsPrinting)
			{
				rgnClipItem.CreateRectRgnIndirect(rcClipItem);
				pDC->SelectClipRgn(&rgnClipItem, RGN_COPY);
			}

			drawArgs.pItem->Draw(&drawArgs);

			// Release clipping
			if (!bIsPrinting)
			{
				pDC->SelectClipRgn(NULL, RGN_COPY);
				rgnClipItem.DeleteObject();
			}
		}

		if (nHeaderIndent > 0) // draw indent column
		{
			rcIndent.IntersectRect(rcIndent, rcClip);
			pPaintManager->FillIndent(pDC, rcIndent);
		}
	} // (NULL != m_pRecord)

	CRgn rgnClip;
	if (!bIsPrinting)
	{
		rgnClip.CreateRectRgnIndirect(&rcClip);
		pDC->SelectClipRgn(&rgnClip, RGN_COPY);
	}

	CRect rcFocus(rcIndent.right, m_rcRow.top, m_rcRow.right, m_rcRow.bottom - (bGridVisible ? 1 : 0));

	if (!m_pControl->IsIconView() && IsFocused() && m_pControl->HasFocus() && m_pControl->IsRowFocusVisible())
	{
		pPaintManager->DrawFocusedRow(pDC, rcFocus);
	}

	if (nHeaderIndent > 0 && bGridVisible)
	{
		if (m_nIndex < m_pControl->GetRows()->GetCount() - 1)
		{
			CXTPReportRow *pNextRow = m_pControl->GetRows()->GetAt(m_nIndex+1);
			ASSERT(pNextRow);
			rcFocus.left = rcIndent.left + min(nHeaderIndent, pPaintManager->m_nTreeIndent * pNextRow->GetTreeDepth());
		}
		else
		{
			rcFocus.left = m_rcRow.left;
		}
		pPaintManager->DrawGrid(pDC, xtpReportOrientationHorizontal, rcFocus);
	}

	if (IsPreviewVisible())
	{
		pPaintManager->DrawGrid(pDC, xtpReportOrientationHorizontal, rcFocus);
	}

	if (!bIsPrinting)
	{
		pDC->SelectClipRgn(NULL, RGN_COPY);
		rgnClip.DeleteObject();
	}
}

int CXTPReportRow::GetTreeDepth() const
{
	return m_nRowLevel;
}

INT_PTR CXTPReportRow::OnToolHitTest(CPoint point, TOOLINFO* pTI)
{
	CRect rcItem;
	CXTPReportRecordItem* pItem = HitTest(point, &rcItem);

	if (!pItem)
		return -1;


	INT_PTR nHit = pItem->OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	nHit = (INT_PTR) pItem;
	CString strTip = pItem->GetTooltip();

	m_pControl->OnGetToolTipInfo(this, pItem, strTip);

	if (strTip.IsEmpty() || strTip == _T(" "))
		return -1;

	if (m_pControl->GetPaintManager()->m_bCleanTooltip)
	{
		//Clean markup
		CString s(strTip), u, v;
		int j = s.Find(_T("<"));
		int k = s.Find(_T(">"));
		while (j > -1 && k > -1)
		{
			u = s.Left(j);
			v = s.Mid(k + 1);
			s = u + v;
			strTip = s;
			j = s.Find(_T("<"));
			k = s.Find(_T(">"));
		}
	}

	CXTPToolTipContext::FillInToolInfo(pTI, m_pControl->m_hWnd, rcItem, nHit, strTip);

	return nHit;
}

BOOL CXTPReportRow::OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS *pClickArgs)
{
	if (m_pControl->m_bFreeHeightMode)
	{
		XTPReportMouseMode mouseMode = m_pControl->GetMouseMode();
		if (mouseMode == xtpReportMouseOverRowDivide)
		{
			m_pControl->SetCapture();
			m_pControl->SetMouseMode(xtpReportMouseNothing);
			CPoint point = pClickArgs->ptClient;
			m_pControl->ClientToScreen(&point);
			CRect rcControl = m_pControl->GetReportRectangle();
			CRect rcRow(GetRect());
			rcRow.right = rcControl.right;
			m_pControl->ClientToScreen(&rcRow);
			m_pControl->ClientToScreen(&rcControl);

			CXTPSplitterTracker tracker(TRUE, m_pControl->m_bDesktopTrackerMode);

			CRect rcBound(rcRow.left, rcRow.top + m_pControl->m_nDefaultRowFreeHeight, rcRow.right, point.y);
			CRect rcTracker(rcRow.left, point.y - 2, rcRow.right, point.y - 1);
			CRect rcAvail(rcRow.left, rcRow.top + m_pControl->m_nDefaultRowFreeHeight, rcRow.right, rcControl.bottom);

			if (tracker.Track(m_pControl, rcAvail, rcTracker, point, FALSE))
			{
				m_nFreeHeight = rcTracker.bottom - rcRow.top;

				if (GetRecord())
					GetRecord()->m_nFreeHeight = m_nFreeHeight;

				m_pControl->AdjustScrollBars();
				m_pControl->RedrawControl();
			}
			return TRUE;
		}
	}

	if (pClickArgs->pItem && pClickArgs->pItem->OnLButtonDown(pClickArgs))
	{
		return TRUE;
	}

	return (BOOL) m_pControl->SendMessageToParent(this, pClickArgs->pItem, pClickArgs->pColumn, XTP_NM_REPORT_LBUTTONDOWN, &pClickArgs->ptClient);
}

BOOL CXTPReportRow::OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS *pClickArgs)
{
	if (m_pControl->m_bFreeHeightMode)
	{
		m_pControl->SetMouseMode(xtpReportMouseNothing);
		ReleaseCapture();
	}

	if (pClickArgs->pItem && pClickArgs->pItem->OnLButtonUp(pClickArgs))
	{
		return TRUE;
	}

	return (BOOL) m_pControl->SendMessageToParent(this, pClickArgs->pItem, pClickArgs->pColumn, XTP_NM_REPORT_LBUTTONUP, &pClickArgs->ptClient);
}



BOOL CXTPReportRow::IsLockExpand() const
{
	BOOL bLockExpand = FALSE;

	// Locking on control level
	if (m_pControl)
	{
		bLockExpand = bLockExpand || m_pControl->IsLockExpand();
	}

	// Locking on rows level (not supported)
	if (m_pParentRows)
	{
		//bLockExpand = bLockExpand || m_pParentRows;
	}

	// Locking on row level
	bLockExpand = bLockExpand || m_bLockExpand;

	// Locking on record level (not supported)
	if (m_pRecord)
	{
		//bLockExpand = bLockExpand || m_pRecord;
	}

	return bLockExpand;
}

void CXTPReportRow::OnClick(CPoint ptClicked)
{
	XTP_REPORTRECORDITEM_CLICKARGS clickArgs;
	clickArgs.pControl = m_pControl;
	clickArgs.pRow     = this;
	clickArgs.pColumn  = NULL;
	clickArgs.ptClient = ptClicked;

	// find clicked item
	clickArgs.pItem = HitTest(ptClicked, &clickArgs.rcItem, &clickArgs.pColumn);

	if (m_pControl->IsVirtualMode())
	{
		if (m_rcCollapse.PtInRect(ptClicked)
			&& clickArgs.pColumn && clickArgs.pColumn->IsTreeColumn()
			&& m_rcCollapse.PtInRect(m_pControl->m_mouseDownState.ptMouse))
		{
			m_pControl->SendMessageToParent(this, NULL, clickArgs.pColumn, XTP_NM_REPORT_ROWEXPANDED, NULL);
			return;
		}
	}
	if (HasChildren() && m_rcCollapse.PtInRect(ptClicked)
		&& clickArgs.pColumn && clickArgs.pColumn->IsTreeColumn()
		&& m_rcCollapse.PtInRect(m_pControl->m_mouseDownState.ptMouse)
		&& !IsLockExpand())
	{
		SetExpanded(!IsExpanded());
		return;
	}

	// notify item if found
	if (!clickArgs.pItem)
		return;

	clickArgs.pItem->OnClick(&clickArgs);
}

void CXTPReportRow::OnDblClick(CPoint ptClicked)
{
	XTP_REPORTRECORDITEM_CLICKARGS clickArgs;
	clickArgs.pControl = m_pControl;
	clickArgs.pRow = this;
	clickArgs.ptClient = ptClicked;
	clickArgs.pColumn = NULL;

	// find clicked item
	clickArgs.pItem = HitTest(ptClicked, &clickArgs.rcItem, &clickArgs.pColumn);

	// notify item if found
	if (clickArgs.pItem != NULL)
	{
		clickArgs.pItem->OnDblClick(&clickArgs);
	}
	else
	{
		// just notify parent
		m_pControl->SendMessageToParent(this, NULL, clickArgs.pColumn, NM_DBLCLK, &ptClicked, -1);
	}
}

CRect CXTPReportRow::GetItemRect(CXTPReportRecordItem* pItem)
{
	if (!IsItemsVisible() || !pItem || !m_pRecord)
		return CRect(0, 0, 0, 0);

	if (pItem->IsMerged())
	{
	}

	if (IsPreviewVisible() && pItem == GetRecord()->GetItemPreview())
	{
		return CRect(m_rcRow.left + m_pControl->GetHeaderIndent(),
			m_rcRow.bottom - m_nPreviewHeight, m_rcRow.right, m_rcRow.bottom);
	}

	CXTPReportColumns* pColumns = m_pControl->GetColumns();
	int nColumnCount = pColumns->GetCount();

	CRect rcItem(0, m_rcRow.top, 0, m_rcRow.bottom - m_nPreviewHeight);

	int nFreezeColCount = m_pControl->GetFreezeColumnsCount();
	if (nFreezeColCount > m_pControl->GetColumns()->GetCount() - 1)
	{
		m_pControl->SetFreezeColumnsCount(0);
		nFreezeColCount = 0;
	}

	int nLeft = nFreezeColCount ? pColumns->GetAt(nFreezeColCount - 1)->GetRect().right : 0;

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			if (m_pRecord->GetItem(pColumn) != pItem)
				continue;

			rcItem.left = pColumn->GetRect().left;
			rcItem.right = pColumn->GetRect().right;
			if (pColumn->GetIndex() >= nFreezeColCount)
			{
				rcItem.left = max(rcItem.left, nLeft);
				rcItem.right = max(rcItem.right, nLeft);
			}
			if (rcItem.Width() <= 0)
				return CRect(0, 0, 0, 0);
			ShiftTreeIndent(rcItem, pColumn);
			return rcItem;
		}
	}
	return CRect(0, 0, 0, 0);
}

void CXTPReportRow::ShiftTreeIndent(CRect& rcItem, CXTPReportColumn* pColumn) const
{
	if (pColumn->IsTreeColumn())
	{
		int nTreeDepth = GetTreeDepth() - m_nGroupLevel;
		if (nTreeDepth > 0)
			nTreeDepth++;
		rcItem.left += m_pControl->GetIndent(nTreeDepth);

		CRect rcBitmap(rcItem);
		int nIndent = m_pControl->GetPaintManager()->DrawCollapsedBitmap(NULL, this, rcBitmap).cx;
		rcItem.left += nIndent + 2;
	}
}

CXTPReportRecordItem* CXTPReportRow::HitTest(CPoint ptPoint, CRect* pRectItem, CXTPReportColumn** ppColumn) const
{
	if (!m_pRecord)
		return NULL;

	// find record item
	int x = m_rcRow.left + m_pControl->GetHeaderIndent();

	CXTPReportColumns* pColumns = m_pControl->GetColumns();
	int nColumnCount = pColumns->GetCount();

	// if hittest for Preview item
	if (IsPreviewVisible())
	{
		CXTPReportRecordItemPreview* pPreviewItem = GetRecord()->GetItemPreview();
		if (pPreviewItem)
		{
			CRect rcItem(x, m_rcRow.bottom - m_nPreviewHeight, m_rcRow.right, m_rcRow.bottom);
			if (rcItem.PtInRect(ptPoint))
			{
				if (pRectItem)
				{
					*pRectItem = rcItem;
				}
				return pPreviewItem;
			}
		}
	}

	CRect rcItem(0, m_rcRow.top, 0, m_rcRow.bottom - m_nPreviewHeight);

	if (!IsItemsVisible())
		return NULL;

	int nFreezeColCount = m_pControl->GetFreezeColumnsCount();
	if (nFreezeColCount > m_pControl->GetColumns()->GetCount() - 1)
	{
		m_pControl->SetFreezeColumnsCount(0);
		nFreezeColCount = 0;
	}

	int nLeft = nFreezeColCount ? pColumns->GetAt(nFreezeColCount - 1)->GetRect().right : 0;

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			rcItem.left = pColumn->GetRect().left;
			rcItem.right = pColumn->GetRect().right;
			if (pColumn->GetIndex() >= nFreezeColCount)
			{
				rcItem.left = max(rcItem.left, nLeft);
				rcItem.right = max(rcItem.right, nLeft);
			}
			if (!rcItem.Width())
				continue;

			if (rcItem.PtInRect(ptPoint) && ppColumn)
			{
				*ppColumn = pColumn;
			}
			// make tooltip shift if tree view (see also Draw function)
			ShiftTreeIndent(rcItem, pColumn);

			// check point
			if (rcItem.PtInRect(ptPoint))
			{
				if (pRectItem)
				{
					*pRectItem = rcItem;
				}

				CXTPReportRecordItem *pItem = m_pRecord->GetItem(pColumn);

				if (pItem && pItem->IsMerged())
				{
					pItem = pItem->GetMergeItem();
				}

				return pItem;
			}
		}
	}

	return NULL;
}

void CXTPReportRow::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pControl->m_bFreeHeightMode)
	{
		XTPReportMouseMode mouseMode = m_pControl->GetMouseMode();
		if (mouseMode == xtpReportMouseNothing || mouseMode == xtpReportMouseOverRowDivide)
		{
			CRect rc = GetRect();
			if (rc.bottom - point.y > -2 && rc.bottom - point.y < 4)
			{
				if (mouseMode == xtpReportMouseNothing)
					SetCursor(XTPResourceManager()->LoadCursor(XTP_IDC_VSPLITBAR));

				m_pControl->SetMouseMode(xtpReportMouseOverRowDivide);
				return;
			}
		}
	}

	CRect rcItem;
	CXTPReportRecordItem* pItem = HitTest(point, &rcItem);
	if (pItem != NULL)
		pItem->OnMouseMove(nFlags, point, m_pControl);
}

void CXTPReportRow::FillMetrics(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem, XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	m_pControl->m_nLockUpdateCount++;
	XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
	drawArgs.pDC = NULL;
	drawArgs.pColumn = pColumn;
	drawArgs.pControl = m_pControl;
	drawArgs.pRow = this;
	drawArgs.rcItem.SetRectEmpty();
	drawArgs.pItem = pItem;
	drawArgs.nTextAlign = pColumn ? pColumn->GetAlignment() : DT_LEFT;
	GetItemMetrics(&drawArgs, pMetrics);
	m_pControl->m_nLockUpdateCount--;
}

void CXTPReportRow::ShowToolTip(CPoint ptTip, CXTPReportTip* pTipWnd)
{
	CRect rcItem(0, 0, 0, 0);
	CXTPReportColumn* pColumn = NULL;
	CXTPReportRecordItem* pItem = HitTest(ptTip, &rcItem, &pColumn);
	// show tooltip
	if (!(pItem &&
		(pItem->IsPreviewItem() || !pItem->IsPreviewItem() && pColumn)))
	{
		pTipWnd->m_pItem = NULL;
		pTipWnd->m_nRowIndex = -1;
		return;
	}

	if (!m_pControl->IsIconView() &&
		!CXTPDrawHelpers::IsTopParentActive(m_pControl->GetSafeHwnd())
		|| m_pControl->GetActiveItem())
		return;

	if ((pItem != pTipWnd->m_pItem)
		|| (m_nIndex != pTipWnd->m_nRowIndex))
	{
		pTipWnd->m_pItem = pItem;
		pTipWnd->m_nRowIndex = m_nIndex;

		CString strTip = pItem->GetTooltip();
		m_pControl->OnGetToolTipInfo(this, pItem, strTip);

		if (!strTip.IsEmpty()
			|| strTip == _T(" ")
			|| pColumn
			   &&
			  (pColumn->GetAlignment() & DT_WORDBREAK)
			  && !m_pControl->GetPaintManager()->m_bForceShowTooltip)
			return;

		if (pItem->GetMarkupUIElement() != NULL)
			return;

		XTP_REPORTRECORDITEM_METRICS* pMetrics = new XTP_REPORTRECORDITEM_METRICS();
		pMetrics->strText = pItem->GetCaption(pColumn);
		FillMetrics(pColumn, pItem, pMetrics);

		CString strText(pMetrics->strText);

		strText.TrimRight();

		if (strText.IsEmpty())
		{
			pMetrics->InternalRelease();
			return;
		}

		if (m_pControl->GetMarkupContext() && strText[0] == '<' && strText[strText.GetLength() - 1] == '>') // Markup check
		{
			CXTPMarkupUIElement* pElement = XTPMarkupParseText(m_pControl->GetMarkupContext(), strText);

			if (pElement)
			{
				XTPMarkupReleaseElement(pElement);

				pMetrics->InternalRelease();
				return;
			}
		}


		if (!pItem->GetFormula().IsEmpty())
			strText = pItem->GetFormula();

		XTP_REPORTRECORDITEM_ARGS itemArgs;
		itemArgs.pControl = m_pControl;
		itemArgs.pRow = this;

		pItem->GetCaptionRect(&itemArgs, rcItem);

		m_pControl->ClientToScreen(&rcItem);

		if (!pTipWnd->GetSafeHwnd())
		{
			pTipWnd->Create(m_pControl);
		}

		CWindowDC dc(m_pControl);
		CXTPFontDC font(&dc, pMetrics->pFont);
		CRect rcTooltip(rcItem);
		BOOL bActivate = FALSE;
		pTipWnd->SetTooltipText(strText);
		if (pItem->IsPreviewItem())
		{
			CRect rcCalc(rcTooltip.left, 0, rcTooltip.right, 0);
			dc.DrawText(strText, &rcCalc, DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX);
			bActivate = (rcCalc.Height() / dc.GetTextExtent(_T(" "), 1).cy) > m_pControl->GetPaintManager()->GetMaxPreviewLines();
			rcTooltip.bottom = rcTooltip.top + rcCalc.Height();
			rcTooltip.right++;
		}
		else if (m_pControl->IsIconView())
		{
			CRect rcCalc;
			rcCalc = m_pControl->GetPaintManager()->CalculateMaxTextRect(&dc,
				pMetrics->strText, &rcItem, TRUE, FALSE, DT_NOPREFIX | DT_WORDBREAK);
			bActivate = ((rcCalc.Width() >= rcItem.Width() - 3)
				|| (rcCalc.Height() >= rcItem.Height() - 3));

			rcTooltip.bottom = rcTooltip.top + rcCalc.Height() + 5;
			rcTooltip.right = rcTooltip.left + rcCalc.Width() + 5;
		}
		else
		{
			// Calculate tooltip fine rect
			if (pTipWnd->IsMultilineForce())
			{
				CRect rcCalc;
				rcCalc = m_pControl->GetPaintManager()->CalculateMaxTextRect(&dc,
				pMetrics->strText, &rcItem, TRUE, FALSE, DT_NOPREFIX | DT_WORDBREAK);
				bActivate = ((rcCalc.Width() >= rcItem.Width() - 3)
					|| (rcCalc.Height() >= rcItem.Height() - 3));

				// with small tuning
				rcTooltip.bottom = rcTooltip.top + rcCalc.Height();
				rcTooltip.right = rcTooltip.left + rcCalc.Width() + 15;
			}
			else
			{
				CSize sz = dc.GetTextExtent(strText);
				bActivate = sz.cx > (rcTooltip.Width() - 4);
			}
		}
		if (bActivate ||  m_pControl->GetPaintManager()->m_bForceShowTooltip)
		{
			rcTooltip.InflateRect(1, 1, 0, 0);

			if (!m_pControl->GetPaintManager()->IsGridVisible(FALSE))
				rcTooltip.top++;

			CRect rcHover(m_rcRow);
			m_pControl->ClientToScreen(&rcHover);
			pTipWnd->SetFont(pMetrics->pFont);
			//pTipWnd->SetTooltipText(strText);
			pTipWnd->SetHoverRect(m_pControl->IsIconView() ? rcHover : rcTooltip);
			pTipWnd->SetTooltipRect(rcTooltip);
			pTipWnd->Activate(TRUE, pTipWnd->IsMultilineForce() || pItem->IsPreviewItem() || m_pControl->IsIconView());

			TRACKMOUSEEVENT tme =
			{
				sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_pControl->GetSafeHwnd(), 0
			};
			_TrackMouseEvent (&tme);
		}

		pMetrics->InternalRelease();
	}
}

BOOL CXTPReportRow::HasParent(CXTPReportRow* pRow)
{
	if (m_pParentRow == NULL)
		return FALSE;
	if (pRow == m_pParentRow)
		return TRUE;
	return m_pParentRow->HasParent(pRow);
}

void CXTPReportRow::SetFullExpanded(BOOL bExpanded)
{
	if (bExpanded != m_bExpanded && HasChildren())
	{
		if (bExpanded)
		{
			m_pControl->_DoExpand(this);
		}
		else
		{
			m_pControl->_DoCollapse(this);
		}

		if (m_pRecord)
		{
			m_pRecord->m_bExpanded = bExpanded;
		}

		m_bExpanded = bExpanded;

		m_pControl->_RefreshIndexes();
		m_pControl->SendMessageToParent(this, NULL, NULL, XTP_NM_REPORT_ROWEXPANDED, NULL);

		int n = GetChilds()->GetCount();
		for (int i = n - 1; i >= 0; i--)
		{
			CXTPReportRow* pRow = GetChilds()->GetAt(i);
			if (pRow)
			{
				m_pControl->EndUpdate();
				m_pControl->BeginUpdate();

				pRow->SetExpanded(bExpanded, TRUE);
				m_pControl->_RefreshIndexes();
				m_pControl->SendMessageToParent(pRow, NULL, NULL, XTP_NM_REPORT_ROWEXPANDED, NULL);
			}
		}
	}
	else
	{
		m_bExpanded = bExpanded;
	}
}

void CXTPReportRow::SetExpanded(BOOL bExpanded, BOOL bRecursive)
{
	if (bExpanded != m_bExpanded && HasChildren())
	{
		if (bExpanded)
		{
			m_pControl->_DoExpand(this);
		}
		else
		{
			m_pControl->_DoCollapse(this);
		}

		if (m_pRecord)
		{
			//m_pRecord->m_bExpanded = bExpanded;
			m_pRecord->SetExpanded(bExpanded);
		}

		m_bExpanded = bExpanded;

		m_pControl->_RefreshIndexes();
		m_pControl->SendMessageToParent(this, NULL, NULL, XTP_NM_REPORT_ROWEXPANDED, NULL);

		if (bRecursive)
		{
			int n = GetChilds()->GetCount();
			for (int i = n - 1; i >= 0; i--)
			{
				CXTPReportRow* pRow = GetChilds()->GetAt(i);
				if (pRow)
				{
					m_pControl->EndUpdate();
					m_pControl->BeginUpdate();

					pRow->SetExpanded(bExpanded, bRecursive);
					m_pControl->_RefreshIndexes();
					m_pControl->SendMessageToParent(pRow, NULL, NULL, XTP_NM_REPORT_ROWEXPANDED, NULL);
				}
			}
		}
	}
	else
	{
		m_bExpanded = bExpanded;
	}
}

void CXTPReportRow::LockExpand(BOOL bLock)
{
	m_bLockExpand = bLock;
}

CXTPReportRow* CXTPReportRow::AddChild(CXTPReportRow* pRow)
{
	GetChilds()->Add(pRow);
	pRow->m_pParentRow = this;

	return pRow;
}

void CXTPReportRow::OnContextMenu(CPoint ptClick)
{
	CXTPReportColumn     *pColumn = NULL;
	CXTPReportRecordItem *pItem   = HitTest(ptClick, NULL, &pColumn);

	m_pControl->ClientToScreen(&ptClick);

	// send process notification to the user and wait for the reaction
	m_pControl->SendMessageToParent(this, pItem, pColumn, NM_RCLICK, &ptClick);
}

void CXTPReportRow::GetItemMetrics(
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pItemMetrics)
{
	ASSERT(m_pRecord);
	ASSERT(pDrawArgs->pRow  == this);
	ASSERT(pDrawArgs->pItem != NULL);

	if (!m_pRecord || !pDrawArgs->pItem)
		return;

	CXTPReportPaintManager *pPaintManager = pDrawArgs->pControl->GetPaintManager();

	pItemMetrics->pFont            = &pPaintManager->m_fontText;
	pItemMetrics->clrForeground    = pPaintManager->m_clrWindowText;
	pItemMetrics->clrBackground    = XTP_REPORT_COLOR_DEFAULT;
	pItemMetrics->nColumnAlignment = pDrawArgs->nTextAlign;
	pItemMetrics->nItemIcon        = XTP_REPORT_NOICON;

	m_pRecord->GetItemMetrics(pDrawArgs, pItemMetrics);
	pDrawArgs->pItem->GetItemMetrics(pDrawArgs, pItemMetrics);

	m_pControl->GetItemMetrics(pDrawArgs, pItemMetrics);
	pDrawArgs->nTextAlign = pItemMetrics->nColumnAlignment;

	if (IsSelected()
		&& (pDrawArgs->pDC
		&& !pDrawArgs->pDC->IsPrinting())
		&& !pDrawArgs->pControl->IsIconView())
	{
		if (pDrawArgs->pColumn
			&& IsFocused()
			&& m_pControl->m_pFocusedColumn == pDrawArgs->pColumn)
			return;

		if (GetControl()->HasFocus())
		{
			pItemMetrics->clrForeground = pPaintManager->m_clrHighlightText;
			pItemMetrics->clrBackground = pPaintManager->m_clrHighlight;
		}
		else if (!pPaintManager->m_bHideSelection)
		{
			pItemMetrics->clrForeground = pPaintManager->m_clrSelectedRowText;
			pItemMetrics->clrBackground = pPaintManager->m_clrSelectedRow;
		}
	}
}

BOOL CXTPReportRow::HasChildren() const
{
	return m_pChilds && m_pChilds->GetCount() > 0;
}

int CXTPReportRow::GetIndex() const
{
	return m_nIndex;
}

BOOL CXTPReportRow::IsGroupRow() const
{
	return FALSE;
}

BOOL CXTPReportRow::IsExpanded() const
{
	return m_bExpanded;
}

BOOL CXTPReportRow::IsVisible() const
{
	return m_bVisible;
}

void CXTPReportRow::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}

BOOL CXTPReportRow::IsLastTreeRow() const
{
	if (!m_pParentRow)
		return FALSE;

	CXTPReportRows* pRows = m_pParentRow->GetChilds();

	return pRows->GetCount() > 0 && pRows->GetAt(pRows->GetCount() - 1) == this;
}

void CXTPReportRow::EnsureVisible()
{
	m_pControl->EnsureVisible(this);
}

CXTPReportRow* CXTPReportRow::GetNextSiblingRow() const
{
	if (!m_pParentRows)
		return 0;

	if (m_nChildIndex == -1)
		return 0;

	ASSERT(m_pParentRows->GetAt(m_nChildIndex) == this);

	if (m_nChildIndex < m_pParentRows->GetCount() - 1)
		return m_pParentRows->GetAt(m_nChildIndex + 1);

	return 0;
}


BOOL CXTPReportRow::HasMergedItems() const
{
	BOOL bMerged = FALSE;

	CXTPReportRecord *pRecord = GetRecord();

	if (pRecord)
	{
		for (int iItem=0; iItem<pRecord->GetItemCount(); iItem++)
		{
			CXTPReportRecordItem *pItem = pRecord->GetItem(iItem);

			if (pItem->IsMerged())
			{
				bMerged = TRUE;
			}
		}
	}

	return bMerged;
}

XTPReportRowType CXTPReportRow::GetType() const
{
	XTPReportRowType rowType = xtpRowTypeInvalid;

	if (NULL != m_pSection)
	{
		rowType = m_pSection->GetRowType();
	}

	return rowType;
}


/////////////////////////////////////////////////////////////////////////////
// IAccessible
/////////////////////////////////////////////////////////////////////////////

CCmdTarget* CXTPReportRow::GetAccessible()
{
	return this;
}

HRESULT CXTPReportRow::GetAccessibleParent(IDispatch* FAR* ppdispParent)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	*ppdispParent = m_pControl->GetIDispatch(TRUE);
	return S_OK;
}

HRESULT CXTPReportRow::GetAccessibleChildCount(long FAR* pChildCount)
{
	if (pChildCount == 0)
	{
		return E_INVALIDARG;
	}
	*pChildCount = 0;

	if (m_pRecord)
	{
		*pChildCount = m_pRecord->GetItemCount();

	}

	return S_OK;
}

HRESULT CXTPReportRow::GetAccessibleChild(VARIANT varChild, IDispatch* FAR* ppdispChild)
{
	*ppdispChild = NULL;
	int nChild = GetChildIndex(&varChild);

	if (nChild <= 0)
	{
		return E_INVALIDARG;
	}

	*ppdispChild = NULL;
	return S_FALSE;
}

HRESULT CXTPReportRow::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	int nIndex = GetChildIndex(&varChild);

	if (nIndex == CHILDID_SELF)
	{
		CString strCaption = _T("Report Row");
		*pszName = strCaption.AllocSysString();
		return S_OK;
	}

	CXTPReportRecordItem* pItem = m_pRecord->GetItem(nIndex - 1);
	if (!pItem)
		return E_INVALIDARG;

	CString strCaption = pItem->GetCaption(m_pControl->GetColumns()->Find(pItem->GetIndex()));
	*pszName = strCaption.AllocSysString();
	return S_OK;

}

HRESULT CXTPReportRow::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	int nIndex = GetChildIndex(&varChild);

	if (nIndex == CHILDID_SELF)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_ROW;

	}
	else
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_CELL;
	}
	return S_OK;
}

HRESULT CXTPReportRow::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	pvarState->vt = VT_I4;
	pvarState->lVal = 0;
	int nChild = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF)
	{
		pvarState->lVal = 0;

		if (IsSelected())
			pvarState->lVal = STATE_SYSTEM_SELECTED;
	}
	else
	{
		CXTPReportRecordItem* pItem = m_pRecord->GetItem(nChild - 1);
		if (pItem)
		{
			CXTPReportColumn* pColumn = m_pControl->GetColumns()->Find(pItem->GetIndex());
			if (pColumn && !pColumn->IsVisible())
			{
				pvarState->lVal = STATE_SYSTEM_INVISIBLE;
			}
		}
	}

	return S_OK;
}


HRESULT CXTPReportRow::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	CRect rc(0, 0, 0, 0);

	int nChild = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF)
	{
		rc = GetRect();
		m_pControl->ClientToScreen(&rc);
	}
	else
	{
		CXTPReportRecordItem* pItem = m_pRecord->GetItem(nChild - 1);
		if (pItem)
		{
			rc = GetItemRect(pItem);
			m_pControl->ClientToScreen(&rc);
		}

	}

	*pxLeft = rc.left;
	*pyTop = rc.top;
	*pcxWidth = rc.Width();
	*pcyHeight = rc.Height();

	return S_OK;
}

HRESULT CXTPReportRow::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (pvarID == NULL)
		return E_INVALIDARG;

	pvarID->vt = VT_I4;
	pvarID->lVal = CHILDID_SELF;

	CPoint pt(xLeft, yTop);
	m_pControl->ScreenToClient(&pt);

	if (!GetRect().PtInRect(pt))
		return S_FALSE;


	CXTPReportRecordItem* pItem = HitTest(pt);
	if (pItem)
	{
		pvarID->lVal = pItem->GetIndex() + 1;
		return S_OK;
	}

	return S_OK;
}


BEGIN_INTERFACE_MAP(CXTPReportRow, CCmdTarget)
	INTERFACE_PART(CXTPReportRow, IID_IAccessible, ExternalAccessible)
END_INTERFACE_MAP()
