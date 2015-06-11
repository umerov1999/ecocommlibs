// XTPTrackControl.cpp : implementation of the CXTPTrackControl class.
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
#include <math.h>

#include "Common/Resource.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include <ReportControl/XTPReportDefines.h>
#include <ReportControl/XTPReportControl.h>
#include <ReportControl/XTPReportHeader.h>
#include <ReportControl/XTPReportColumn.h>
#include <ReportControl/XTPReportColumns.h>
#include <ReportControl/XTPReportRow.h>
#include <ReportControl/XTPReportRecord.h>
#include <ReportControl/XTPReportRecordItem.h>
#include <ReportControl/XTPReportRows.h>
#include <ReportControl/XTPReportSections.h>
#include <ReportControl/XTPReportSection.h>
#include <ReportControl/XTPReportPaintManager.h>
#include <ReportControl/XTPReportHitTestInfo.h>

#include <ReportControl/TrackControl/XTPTrackPaintManager.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeDefault.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeExplorer.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeOffice2003.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeOffice2003Luna.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeOffice2013.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeOfficeXP.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeResource.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeVisualStudio2012.h>

#include "XTPTrackBlock.h"
#include "XTPTrackControl.h"
#include "XTPTrackControlItem.h"
#include "XTPTrackHeader.h"
#include "XTPTrackUndoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CXTPTrackControl,CXTPReportControl)
//{{AFX_MSG_MAP(CXTPTrackControl)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_GETDLGCODE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CXTPTrackControl::CXTPTrackControl()
{
	RegisterWindowClass();

	m_nTimeLinePosition = 0;

	m_nWorkAreaMin = 0;
	m_nWorkAreaMax = 100;

	m_nTimeLineMin = 0;
	m_nTimeLineMax = 100;

	m_nViewPortMin = 0;
	m_nViewPortMax = 100;

	m_bScaleOnResize = TRUE;

	m_nSnapMargin = 5;
	m_bSnapToBlocks = FALSE;
	m_bSnapToMarkers = TRUE;

	m_bFlexibleDrag = FALSE;
	m_nTrackColumnIndex = -1;

	m_bAllowBlockRemove = TRUE;
	m_bAllowBlockScale = TRUE;
	m_bAllowBlockMove = TRUE;

	m_rcSelectedArea.SetRectEmpty();

	m_hMoveCursor = AfxGetApp()->LoadCursor(XTP_IDC_HAND);
	m_hResizeCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);

	m_strTimeFormat = _T("%d");

	m_pMarkers = new CXTPTrackMarkers(this);

	m_nOldTrackWidth = 0;

	m_bShowWorkArea = TRUE;
	m_bShowTimeLinePosition = TRUE;

	SetPaintManager(new CXTPTrackPaintManager());
	SetReportHeader(new CXTPTrackHeader(this, m_pColumns));

	m_pUndoManager = new CXTPTrackUndoManager();

	m_pSelectedBlocks = new CXTPTrackSelectedBlocks();
}

CXTPTrackControl::~CXTPTrackControl()
{
	m_pUndoManager->InternalRelease();

	m_pMarkers->InternalRelease();

	m_pSelectedBlocks->InternalRelease();
}

void CXTPTrackControl::SetPaintManager(CXTPTrackPaintManager *pPaintManager)
{
	CXTPReportControl::SetPaintManager(pPaintManager);
}

void CXTPTrackControl::SetTheme(XTPReportPaintTheme paintTheme, BOOL bEnableMetrics/*=FALSE*/)
{
	m_themeCurrent = paintTheme;
	m_bThemeMetrics = bEnableMetrics;

	switch(m_themeCurrent)
	{
	case xtpReportThemeOfficeXP:
		SetPaintManager(new CXTPTrackThemeOfficeXP());
		break;
	case xtpReportThemeOffice2003:
		SetPaintManager(new CXTPTrackThemeOffice2003());
		break;
	case xtpReportThemeOffice2003Luna:
		SetPaintManager(new CXTPTrackThemeOffice2003Luna());
		break;
	case xtpReportThemeOffice2013:
		SetPaintManager(new CXTPTrackThemeOffice2013());
		break;
	case xtpReportThemeVisualStudio2012Light:
		SetPaintManager(new CXTPTrackThemeVisualStudio2012Light());
		break;
	case xtpReportThemeVisualStudio2012Dark:
		SetPaintManager(new CXTPTrackThemeVisualStudio2012Dark());
		break;
	case xtpReportThemeVisualStudio2012Blue:
		SetPaintManager(new CXTPTrackThemeVisualStudio2012Blue());
		break;
	case xtpReportThemeExplorer:
		SetPaintManager(new CXTPTrackThemeExplorer());
		break;
	case xtpReportThemeResource:
		SetPaintManager(new CXTPTrackThemeResource());
		break;
	default:
		SetPaintManager(new CXTPTrackThemeDefault());
		break;
	}
}

BOOL CXTPTrackControl::RegisterWindowClass(HINSTANCE hInstance)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance, XTPTRACKCTRL_CLASSNAME, CS_DBLCLKS);
}

LRESULT CXTPTrackControl::SendMessageToParent(UINT nMessage, CXTPTrackBlock* pBlock, CXTPTrackMarker* pMarker) const
{
	if (!IsWindow(m_hWnd))
		return 0;

	XTP_NM_TRACKCONTROL nmgv;
	nmgv.pBlock = pBlock;
	nmgv.pMarker = pMarker;

	return SendNotifyMessage(nMessage, (NMHDR*)&nmgv);
}

void CXTPTrackControl::SetViewPort(int nViewPortMin, int nViewPortMax)
{
	if (nViewPortMin < m_nTimeLineMin)
	{
		nViewPortMax = m_nTimeLineMin + nViewPortMax - nViewPortMin;
		nViewPortMin = m_nTimeLineMin;
	}
	if (nViewPortMax > m_nTimeLineMax)
	{
		nViewPortMin = m_nTimeLineMax - (nViewPortMax - nViewPortMin);
		nViewPortMax = m_nTimeLineMax;
	}

	m_nOldTrackWidth = 0;
	m_nViewPortMin = nViewPortMin;
	m_nViewPortMax = nViewPortMax;

	RedrawControl();

	SendMessageToParent(XTP_NM_TRACK_SLIDERCHANGED);
}

int CXTPTrackControl::GetTrackColumnIndex() const
{
	if (m_nTrackColumnIndex != -1)
		return m_nTrackColumnIndex;

	return m_pColumns->GetCount() - 1;

}

int CXTPTrackControl::GetTrackOffset() const
{
	CXTPReportColumn* pColumn = m_pColumns->Find(GetTrackColumnIndex());
	ASSERT(pColumn && pColumn->IsVisible());

	return pColumn->GetRect().left;
}

void CXTPTrackControl::SetTimeLinePosition(int nTimeLinePos)
{
	if (m_nTimeLinePosition != nTimeLinePos)
	{
		m_nTimeLinePosition = nTimeLinePos; //Main Marker
		RedrawControl();
	}
}

void CXTPTrackControl::OnDraw(CDC* pDC)
{
	AdjustTrackColumnWidth();

	CXTPReportControl::OnDraw(pDC);
}

void CXTPTrackControl::AdjustTrackColumnWidth()
{
	CXTPReportColumn* pColumn = GetTrackColumn();

	if (pColumn && pColumn->IsVisible())
	{
		int nTrackWidth = max(1, pColumn->GetWidth() - 14);

		if (m_bScaleOnResize)
		{
			m_nOldTrackWidth = 0;
		}
		else if (!m_bScaleOnResize && nTrackWidth > 1)
		{
			if (m_nOldTrackWidth != 0 )
			{
				double nViewPortMax = m_nViewPortMin + nTrackWidth * m_nOldTrackViewPortRange / m_nOldTrackWidth;
				if (nViewPortMax > m_nTimeLineMax)
					nViewPortMax = m_nTimeLineMax;

				if (fabs(m_nViewPortMax - nViewPortMax) > 1e-6)
				{
					m_nViewPortMax = nViewPortMax;

					SendMessageToParent(XTP_NM_TRACK_SLIDERCHANGED);
				}
			}

			if (m_nOldTrackWidth == 0)
			{
				m_nOldTrackWidth = nTrackWidth;
				m_nOldTrackViewPortRange = m_nViewPortMax - m_nViewPortMin;
			}
		}
	}
}

int CXTPTrackControl::DrawRows(CDC* pDC, CRect& rcClient, int y, CXTPReportRows *pRows, int nTopRow, int nColumnFrom, int nColumnTo, int *pnHeight)
{
	int nIndex = CXTPReportControl::DrawRows(pDC, rcClient, y, pRows, nTopRow, nColumnFrom, nColumnTo, pnHeight);

	CXTPReportColumn *pColumn = GetTrackColumn();
	if (!pColumn || !pColumn->IsVisible())
	{
		return nIndex;
	}

	CRect rcTrack = pColumn->GetRect();

	if (m_bShowTimeLinePosition)
	{
		int nPos = PositionToTrack(GetTimeLinePosition());

		if (nPos >= rcTrack.left && nPos <= rcTrack.right)
		{
			pDC->FillSolidRect(CRect(nPos, m_pSectionBody->GetRect().top, nPos + 1, m_pSectionBody->GetRect().bottom + 10), RGB(205, 0, 0));
		}
	}

	if (!m_rcSelectedArea.IsRectEmpty())
	{
		CRect rcSelectedArea = m_rcSelectedArea;
		rcSelectedArea.top = max(rcSelectedArea.top, m_pSectionBody->GetRect().top);

		COLORREF clrSelectedArea = GetTrackPaintManager()->m_clrSelectedArea;


		LPDWORD lpBits;
		HBITMAP hBitmap = CXTPImageManager::Create32BPPDIBSection(pDC->GetSafeHdc(), 1, 1, (LPBYTE*)&lpBits);
		if (hBitmap)
		{
			CXTPCompatibleDC dc(pDC, hBitmap);
			lpBits[0] = RGB(GetBValue(clrSelectedArea), GetGValue(clrSelectedArea), GetRValue(clrSelectedArea));

			XTPImageManager()->AlphaBlend2(pDC->GetSafeHdc(), rcSelectedArea, dc, CRect(0, 0, 1, 1), 100);
			//XTPImageManager()->DoAlphaBlend(pDC->GetSafeHdc(), rcSelectedArea, dc, CRect(0, 0, 1, 1));


			DeleteObject(hBitmap);
		}

		pDC->Draw3dRect(rcSelectedArea, clrSelectedArea, clrSelectedArea);
	}

	return nIndex;
}


void CXTPTrackControl::AdjustLayout()
{
/*
	------------------------------
	| Group by                   |
	------------------------------
	| Time line                  |
	------------------------------
	| Header                     |
	------------------------------
	| Sections                   |
	------------------------------
	| Footer                     |
	------------------------------
*/

	if (NULL == GetSafeHwnd())
		return;

	if (m_bAdjustLayoutRunning) //guard to prevent the recursion similar to OnSize function
		return;

	m_bAdjustLayoutRunning = TRUE;

	CXTPClientRect rcClient(this);
	CWindowDC dc(this);

	int nHeaderWidth = m_rcHeaderArea.Width();

	CXTPReportHeader *pHeader = GetReportHeader();
	CXTPReportColumn *pTrack  = GetTrackColumn();

	int nHeightGroupBy  = 0;
	int nHeightHeader   = 0;
	int nHeightFooter   = 0;
	int nHeightTimeLine = 0;

	if (NULL != pHeader && m_bGroupByEnabled)
	{
		nHeightGroupBy = pHeader->GetGroupByHeight();
	}

	if (pTrack && pTrack->IsVisible())
	{
		nHeightTimeLine = 30;
	}

	if (m_bHeaderVisible)
	{
		nHeightHeader = GetPaintManager()->GetHeaderHeight(this, &dc);
	}

	if (m_bFooterVisible)
	{
		nHeightFooter = GetPaintManager()->GetFooterHeight(this, &dc);
	}

	// Group by rect
	m_rcGroupByArea.SetRect(0, 0, rcClient.Width(), nHeightGroupBy);

	// Time line
	m_rcTimelineArea.SetRect(0, m_rcGroupByArea.bottom, rcClient.Width(), m_rcGroupByArea.bottom + nHeightTimeLine);

	// Header
	m_rcHeaderArea.SetRect(0, m_rcTimelineArea.bottom, rcClient.Width(), m_rcTimelineArea.bottom + nHeightHeader);

	// Sections
	int nHeightSections = rcClient.Height() - nHeightGroupBy - nHeightTimeLine - nHeightHeader - nHeightFooter;
	CRect rcSections(0, m_rcHeaderArea.bottom, rcClient.Width(), m_rcHeaderArea.bottom+nHeightSections);

	m_pSections->AdjustLayout(&dc, rcSections);

	// Footer
	m_rcFooterArea.SetRect(0, rcClient.Height() - nHeightFooter, rcClient.Width(), rcClient.Height());

	if (nHeaderWidth != m_rcHeaderArea.Width() && pHeader)
	{
		pHeader->AdjustColumnsWidth(m_rcHeaderArea.Width());
	}

	m_bAdjustLayoutRunning = FALSE;
}

CXTPReportColumn* CXTPTrackControl::GetTrackColumn() const
{
	CXTPReportColumn* pColumn = m_pColumns->Find(GetTrackColumnIndex());
	return pColumn;
}

int CXTPTrackControl::PositionToTrack(int nPosition)
{
	CXTPReportColumn* pColumn = GetTrackColumn();


	CRect rcTrack = pColumn->GetRect();
	rcTrack.DeflateRect(7, 0);

	return int(rcTrack.left + (nPosition - m_nViewPortMin) * rcTrack.Width() / (m_nViewPortMax - m_nViewPortMin));
}

int CXTPTrackControl::TrackToPosition(int nTrack) const
{
	CXTPReportColumn* pColumn = GetTrackColumn();

	CRect rcTrack = pColumn->GetRect();
	rcTrack.DeflateRect(7, 0);

	return int(m_nViewPortMin + (nTrack - rcTrack.left) * (m_nViewPortMax - m_nViewPortMin) / rcTrack.Width());
}

void CXTPTrackControl::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportControl::DoPropExchange(pPX);


	PX_Int(pPX, _T("TimeScaleMin"), m_nTimeLineMin);
	PX_Int(pPX, _T("TimeScaleMax"), m_nTimeLineMax);

	//PX_Int(pPX, _T("ViewPortMin"), m_nViewPortMin);
	//PX_Int(pPX, _T("ViewPortMax"), m_nViewPortMax);

	PX_Int(pPX, _T("WorkAreaMin"), m_nWorkAreaMin);
	PX_Int(pPX, _T("WorkAreaMax"), m_nWorkAreaMax);

	PX_Int(pPX, _T("TimeLinePos"), m_nTimeLinePosition);
	PX_Int(pPX, _T("TrackColumnIndex"), m_nTrackColumnIndex, -1);


	CXTPPropExchangeSection secMarkers(pPX->GetSection(_T("Markers")));
	m_pMarkers->DoPropExchange(&secMarkers);

}



void CXTPTrackControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPReportColumn* pColumn = GetTrackColumn();
	if (!pColumn || !pColumn->IsVisible())
	{
		CXTPReportControl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	if ((GetKeyState(VK_CONTROL) < 0 || GetKeyState(VK_SHIFT) < 0)
		&& (nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_UP || nChar == VK_DOWN))
	{

		CreateDragBlocks();
		if (m_arrDragBlocks.GetSize() == 0)
			return;

		BOOL bResize = m_bAllowBlockScale && GetKeyState(VK_SHIFT) < 0 ? 2 : 0;

		if (!bResize && !m_bAllowBlockMove)
			return;

		XTPDrawHelpers()->KeyToLayout(this, nChar);

		CRect rcBlock = m_arrDragBlocks[m_arrDragBlocks.GetSize() - 1].pBlock->GetRect();
		CPoint pt = rcBlock.CenterPoint();
		ClientToScreen(&pt);

		m_ptStartDrag = pt;

		if (nChar == VK_LEFT) pt.x -= 20;
		if (nChar == VK_RIGHT) pt.x += 20;
		if (nChar == VK_UP) pt.y -= 20;
		if (nChar == VK_DOWN) pt.y += 20;


		OnMoveBlock(pt, bResize);
		RedrawControl();
		UpdateWindow();

		rcBlock = m_arrDragBlocks[m_arrDragBlocks.GetSize() - 1].pBlock->GetRect();
		pt = rcBlock.CenterPoint();
		if (bResize) pt.x = rcBlock.right - 2;
		ClientToScreen(&pt);

		SetCursorPos(pt.x, pt.y);

		ReleaseDragBlocks();
		return;
	}

	if (nChar == VK_RIGHT && !m_bFocusSubItems)
	{
		int nDelta = TrackToPosition(10) - TrackToPosition(0);
		if (IsLayoutRTL()) nDelta = -nDelta;

		SetViewPort(int(m_nViewPortMin + nDelta), int(m_nViewPortMax + nDelta));
		return;
	}
	if (nChar == VK_LEFT && !m_bFocusSubItems)
	{
		int nDelta = TrackToPosition(10) - TrackToPosition(0);
		if (IsLayoutRTL()) nDelta = -nDelta;

		SetViewPort(int(m_nViewPortMin - nDelta), int(m_nViewPortMax - nDelta));
		return;
	}
	if (nChar == VK_TAB)
	{
		CXTPTrackBlock* pOldSelectedBlock = m_pSelectedBlocks->GetAt(m_pSelectedBlocks->GetCount() - 1);

		CXTPTrackBlock* pNewSelectedBlock = NULL;

		if ((GetKeyState(VK_SHIFT) < 0)) // Back
		{
			int nOldPosition = pOldSelectedBlock ? pOldSelectedBlock->GetPosition() : INT_MAX;

			for (int i = pOldSelectedBlock == 0 ? GetRows()->m_nFocusedRow : GetRows()->GetCount() - 1; i >= 0; i--)
			{
				CXTPReportRow* pRow = GetRows()->GetAt(i);
				if (!pRow || !pRow->GetRecord())
					continue;

				CXTPTrackControlItem* pTrack = DYNAMIC_DOWNCAST(CXTPTrackControlItem, pRow->GetRecord()->GetItem(GetTrackColumnIndex()));
				if (!pTrack || pTrack->GetBlockCount() == 0)
					continue;

				if (pOldSelectedBlock == NULL || pOldSelectedBlock->GetItem() == pTrack)
				{
					for (int j = 0; j < pTrack->GetBlockCount(); j++)
					{
						CXTPTrackBlock* pBlock = pTrack->GetBlock(j);
						if (pBlock->IsLocked())
							continue;
						if (pBlock->GetPosition() >= nOldPosition)
							continue;

						if (pNewSelectedBlock == NULL || pBlock->GetPosition() > pNewSelectedBlock->GetPosition())
							pNewSelectedBlock = pBlock;
					}

					if (pNewSelectedBlock)
						break;

					nOldPosition = INT_MAX;
					pOldSelectedBlock = 0;
				}
			}

		}
		else
		{
			int nOldPosition = pOldSelectedBlock ? pOldSelectedBlock->GetPosition() : -1;

			for (int i = pOldSelectedBlock == 0 ? GetRows()->m_nFocusedRow : 0; i < GetRows()->GetCount(); i++)
			{
				CXTPReportRow* pRow = GetRows()->GetAt(i);
				if (!pRow || !pRow->GetRecord())
					continue;

				CXTPTrackControlItem* pTrack = DYNAMIC_DOWNCAST(CXTPTrackControlItem, pRow->GetRecord()->GetItem(GetTrackColumnIndex()));
				if (!pTrack || pTrack->GetBlockCount() == 0)
					continue;

				if (pOldSelectedBlock == NULL || pOldSelectedBlock->GetItem() == pTrack)
				{
					for (int j = 0; j < pTrack->GetBlockCount(); j++)
					{
						CXTPTrackBlock* pBlock = pTrack->GetBlock(j);
						if (pBlock->IsLocked())
							continue;
						if (pBlock->GetPosition() <= nOldPosition)
							continue;

						if (pNewSelectedBlock == NULL || pBlock->GetPosition() < pNewSelectedBlock->GetPosition())
							pNewSelectedBlock = pBlock;
					}

					if (pNewSelectedBlock)
						break;

					nOldPosition = -1;
					pOldSelectedBlock = 0;
				}
			}
		}


		if (pNewSelectedBlock != NULL)
		{
			m_pSelectedBlocks->RemoveAll();

			m_pSelectedBlocks->Add(pNewSelectedBlock);

			SendMessageToParent(XTP_NM_TRACK_SELECTEDBLOCKSCHANGED);

			EnsureVisible(pNewSelectedBlock);
		}

		RedrawControl();

		return;
	}

	CXTPReportControl::OnKeyDown(nChar, nRepCnt, nFlags);
}


UINT CXTPTrackControl::OnGetDlgCode()
{
	return DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS;
}

void CXTPTrackControl::EnsureVisible(CXTPTrackBlock* pBlock)
{
	CXTPReportRecord* pRecord = pBlock->GetItem()->GetRecord();

	CXTPReportRow* pRow = GetRows()->Find(pRecord);

	if (!pRow)
		return;

	CXTPReportControl::EnsureVisible(pRow);
	CXTPReportControl::EnsureVisible(GetTrackColumn());


	CXTPReportColumn* pColumn = GetTrackColumn();
	if (!pColumn || !pColumn->IsVisible())
	{
		return;
	}

	if (m_nViewPortMin >= pBlock->GetPosition() + pBlock->GetLength())
	{
		SetViewPort(pBlock->GetPosition(), pBlock->GetPosition() + int(m_nViewPortMax - m_nViewPortMin));
	}
	else if (m_nViewPortMax <= pBlock->GetPosition())
	{
		int nViewPortMin = pBlock->GetPosition() + pBlock->GetLength() - int(m_nViewPortMax - m_nViewPortMin);
		if (nViewPortMin < m_nTimeLineMin)
			nViewPortMin = m_nTimeLineMin;

		SetViewPort(nViewPortMin, nViewPortMin + int(m_nViewPortMax - m_nViewPortMin));
	}
}

//////////////////////////////////////////////////////////////////////////
//

int CXTPTrackControl::SnapPosition(int nPosition)
{
	CXTPTrackControl* pControl = this;
	int nMargin = pControl->GetSnapMargin();
	if (nMargin == 0)
		return nPosition;

	int nScreenPosition = pControl->PositionToTrack(nPosition);

	if (pControl->m_bSnapToMarkers)
	{

		if (abs(nScreenPosition - pControl->PositionToTrack(pControl->GetTimeLinePosition())) <= nMargin)
		{
			return pControl->GetTimeLinePosition();
		}

		for (int i = 0; i < pControl->GetMarkers()->GetCount(); i++)
		{
			if (abs(nScreenPosition - pControl->PositionToTrack(pControl->GetMarkers()->GetAt(i)->GetPosition())) <= nMargin)
			{
				return pControl->GetMarkers()->GetAt(i)->GetPosition();
			}
		}
	}

	if (pControl->m_bSnapToBlocks)
	{
		CXTPReportScreenRows *pScreenRows = pControl->GetRows()->GetScreenRows();

		for (int i = 0; i < pScreenRows->GetSize(); i++)
		{
			CXTPReportRow* pRow = pScreenRows->GetAt(i);
			if (!pRow || !pRow->GetRecord())
				continue;

			CXTPTrackControlItem* pTrack = DYNAMIC_DOWNCAST(CXTPTrackControlItem, pRow->GetRecord()->GetItem(GetTrackColumnIndex()));
			if (!pTrack)
				continue;

			for (int j = 0; j < pTrack->GetBlockCount(); j++)
			{
				CXTPTrackBlock* pBlock = pTrack->GetBlock(j);

				if (pBlock->m_bDragging)
					continue;

				if (abs(nScreenPosition - pControl->PositionToTrack(pBlock->GetPosition())) <= nMargin)
				{
					return pBlock->GetPosition();
				}

				if (abs(nScreenPosition - pControl->PositionToTrack(pBlock->GetPosition() + pBlock->GetLength())) <= nMargin)
				{
					return pBlock->GetPosition() + pBlock->GetLength();
				}
			}
		}

	}

	return nPosition;
}

void CXTPTrackControl::OnMoveBlock(CPoint pt, BOOL bResize)
{
	CXTPTrackControl* pControl = this;

	pControl->BeginUpdate();

	CArray<CXTPTrackBlock*, CXTPTrackBlock*> arrBlocks;
	CArray<CXTPTrackControlItem*, CXTPTrackControlItem*> arrItems;

	int i;

	for (i = 0; i < m_arrDragBlocks.GetSize(); i++)
	{
		CXTPTrackBlock* pBlock = m_arrDragBlocks[i].pBlock;

		if (bResize && !pBlock->IsResizable())
			continue;

		CXTPTrackControlItem* pOldTrack = pBlock->GetItem();

		CRect rc = m_arrDragBlocks[i].rcOrigin;
		rc.OffsetRect(pt - m_ptStartDrag);

		CPoint ptHit = CPoint(0, (rc.top + rc.bottom) / 2);

		CXTPReportRow* pRow = CXTPReportControl::HitTest(ptHit);
		CXTPTrackControlItem* pNewTrack = 0;

		if (!bResize && pControl->m_bAllowBlockRemove)
		{
			if (pRow && pRow->GetRecord())
			{
				pNewTrack = DYNAMIC_DOWNCAST(CXTPTrackControlItem, pRow->GetRecord()->GetItem(GetTrackColumnIndex()));
			}
		}
		else
		{
			pNewTrack = pBlock->GetItem();
		}

		if (pNewTrack && !pNewTrack->IsLocked())
		{
			if (pBlock->GetItem() != pNewTrack)
			{
				pBlock->InternalAddRef();
				pBlock->Remove();
				pNewTrack->Add(pBlock);
			}

			int nOffet = -(pControl->TrackToPosition(m_ptStartDrag.x) - pControl->TrackToPosition(pt.x));
			if (IsLayoutRTL()) nOffet = -nOffet;

			int nPosition = pBlock->m_nMRUPosition;
			int nLength = pBlock->m_nLength;


			if (bResize == 1)
			{
				int nLeft = m_arrDragBlocks[i].nOldPos;
				int nRight =m_arrDragBlocks[i].nOldLength + m_arrDragBlocks[i].nOldPos;

				nLeft = max(pControl->GetTimeLineMin(), SnapPosition(nLeft + nOffet));

				if (nRight - nLeft < pBlock->GetMinLength())
					nLeft = nRight - pBlock->GetMinLength();
				else if (nRight - nLeft > pBlock->GetMaxLength())
					nLeft = nRight - pBlock->GetMaxLength();

				nPosition = nLeft;
				nLength = nRight - nLeft;

			}
			else if (bResize == 2)
			{
				int nLeft = m_arrDragBlocks[i].nOldPos;
				int nRight =m_arrDragBlocks[i].nOldLength + m_arrDragBlocks[i].nOldPos;

				nRight = min(pControl->GetTimeLineMax(), SnapPosition(nRight + nOffet));

				if (nRight - nLeft < pBlock->GetMinLength())
					nRight = nLeft + pBlock->GetMinLength();
				else if (nRight - nLeft > pBlock->GetMaxLength())
					nRight = nLeft + pBlock->GetMaxLength();

				nLength = nRight - nLeft;
			}
			else
			{
				int nRight = min(pControl->GetTimeLineMax(), SnapPosition(m_arrDragBlocks[i].nOldPos + m_arrDragBlocks[i].nOldLength + nOffet));
				int nLeft = max(pControl->GetTimeLineMin(), SnapPosition(nRight - m_arrDragBlocks[i].nOldLength));

				nPosition = nLeft;
			}

			pBlock->SetPosition(nPosition);
			pBlock->SetLength(nLength);

			pBlock->m_nLastDragTime = (int)time(0);

			arrBlocks.Add(pBlock);

			if (pOldTrack)
			{
				arrItems.Add(pOldTrack);
			}
			if (pNewTrack != pOldTrack)
			{
				arrItems.Add(pNewTrack);
			}

		}
	}

	for (i = (int)arrBlocks.GetSize() - 1; i >= 0; i--)
	{
		CXTPTrackBlock* pBlock = arrBlocks[i];

		if (!pControl->m_bFlexibleDrag)
		{
			pBlock->GetItem()->AdjustBlockPosition(pBlock, bResize);
		}

		pControl->SendMessageToParent(XTP_NM_TRACK_BLOCKCHANGED, pBlock);
	}

	for (i = 0; i < arrItems.GetSize(); i++)
	{
		CXTPTrackControlItem* pItem = arrItems[i];
		pItem->RecalcLayout();
	}

	EndUpdate();
}

void CXTPTrackControl::CreateDragBlocks()
{
	m_arrDragBlocks.RemoveAll();
	int i;

	for (i = 0; i < m_pSelectedBlocks->GetCount(); i++)
	{
		DRAGBLOCK block;
		block.pBlock = (CXTPTrackBlock*)m_pSelectedBlocks->GetAt(i);
		if (block.pBlock->IsLocked())
			continue;

		block.nOldPos = block.pBlock->GetPosition();
		block.nOldLength = block.pBlock->GetLength();
		block.rcOrigin = block.pBlock->GetRect();

		block.pBlock->m_bDragging = TRUE;

		m_arrDragBlocks.Add(block);
	}
}

void CXTPTrackControl::ReleaseDragBlocks()
{
	for (int i = 0; i < m_arrDragBlocks.GetSize(); i++)
	{
		m_arrDragBlocks[i].pBlock->m_bDragging = FALSE;
	}
}

void CXTPTrackControl::StartDragBlocks(BOOL bResize)
{
	m_pUndoManager->StartGroup();

	// set capture to the window which received this message
	SetCapture();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;

	CreateDragBlocks();

	::SetCursor(bResize ? m_hResizeCursor : m_hMoveCursor);


	// get messages until capture lost or cancelled/accepted
	while (::GetCapture() == m_hWnd)
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;
			OnMoveBlock(pt, bResize);
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);

	}

	ReleaseCapture();

	ReleaseDragBlocks();

	m_pUndoManager->EndGroup();
}


void CXTPTrackControl::OnMoveSelection(CPoint pt)
{
	CXTPTrackControl* pControl = this;

	CRect rc(m_ptStartDrag, pt);
	rc.NormalizeRect();

	pControl->ScreenToClient(&rc);

	int sz = pControl->m_pSelectedBlocks->GetCount();
	pControl->m_pSelectedBlocks->RemoveAll();

	CXTPReportScreenRows *pScreenRows = pControl->GetRows()->GetScreenRows();

	for (int i = 0; i < pScreenRows->GetSize(); i++)
	{
		CXTPReportRow* pRow = pScreenRows->GetAt(i);
		if (!pRow->GetRecord())
			continue;

		if (pRow->GetRect().top >= rc.bottom || pRow->GetRect().bottom <= rc.top)
			continue;

		CXTPTrackControlItem* pTrack = DYNAMIC_DOWNCAST(CXTPTrackControlItem, pRow->GetRecord()->GetItem(GetTrackColumnIndex()));
		if (!pTrack)
			continue;

		for (int j = 0; j < pTrack->GetBlockCount(); j++)
		{
			CXTPTrackBlock* pBlock = pTrack->GetBlock(j);
			if (pBlock->IsLocked())
				continue;

			if (CRect().IntersectRect(pBlock->GetRect(), rc))
			{
				pControl->m_pSelectedBlocks->Add(pBlock);
			}
		}
	}

	if (sz != pControl->m_pSelectedBlocks->GetCount())
	{
		pControl->SendMessageToParent(XTP_NM_TRACK_SELECTEDBLOCKSCHANGED);
	}

	pControl->m_rcSelectedArea = rc;
	pControl->RedrawControl();

}

void CXTPTrackControl::StartDragSelection()
{
	// set capture to the window which received this message
	SetCapture();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;


	// get messages until capture lost or canceled/accepted
	while (::GetCapture() == m_hWnd)
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;
			OnMoveSelection(pt);
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		}
		else
			DispatchMessage(&msg);

	}

	ReleaseCapture();

	m_rcSelectedArea.SetRectEmpty();
	RedrawControl();

}

void CXTPTrackControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pSectionBody->GetRect().PtInRect(point))
	{
		if (CXTPReportControl::HitTest(point) == NULL)
		{
			if (m_pSelectedBlocks->GetCount())
			{
				m_pSelectedBlocks->RemoveAll();
				SendMessageToParent(XTP_NM_TRACK_SELECTEDBLOCKSCHANGED);
			}
			StartDragSelection();
			return;
		}
	}
	CXTPReportControl::OnLButtonDown(nFlags, point);
}

void CXTPTrackControl::Populate()
{
	CXTPReportControl::Populate();

	if (m_pUndoManager) m_pUndoManager->Clear();
}

BOOL CXTPTrackControl::HitTest(CPoint pt, CXTPReportHitTestInfo *pInfo) const
{
	if (NULL == pInfo)
	{
		return FALSE;
	}

	pInfo->m_pColumn        = NULL;
	pInfo->m_pRow           = NULL;
	pInfo->m_pItem          = NULL;
	pInfo->m_pBlock         = NULL;
	pInfo->m_htCode         = xtpReportHitTestUnknown;
	pInfo->m_iTrackPosition = 0;

	if (m_pSections->GetRect().PtInRect(pt))
	{
		pInfo->m_pRow = CXTPReportControl::HitTest(pt);

		if (pInfo->m_pRow)
		{
			pInfo->m_pItem = pInfo->m_pRow->HitTest(pt, NULL, &pInfo->m_pColumn);
			if (pInfo->m_pItem && pInfo->m_pItem->IsKindOf(RUNTIME_CLASS(CXTPTrackControlItem)))
			{
				CXTPTrackControlItem *pItem = DYNAMIC_DOWNCAST(CXTPTrackControlItem, pInfo->m_pItem);
				if (pItem)
				{
					CXTPTrackBlock *pBlock = pItem->HitTest(pt);

					pInfo->m_iTrackPosition = TrackToPosition(pt.x);

					if (NULL != pBlock)
					{
						pInfo->m_htCode = xtpReportHitTestBlock;
						pInfo->m_pBlock = pBlock;
					}
				}
			}
		}
	}

	if (xtpReportHitTestUnknown == pInfo->m_htCode)
	{
		return CXTPReportControl::HitTest(pt, pInfo);
	}
	else
	{
		return TRUE;
	}
}
