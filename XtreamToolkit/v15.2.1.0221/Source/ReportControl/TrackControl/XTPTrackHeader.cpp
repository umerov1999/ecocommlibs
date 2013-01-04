// XTPTrackHeader.cpp : implementation of the CXTPTrackHeader class.
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
#include "Common/XTPColorManager.h"

#include "../XTPReportDefines.h"
#include "../XTPReportColumn.h"
#include "../XTPReportHeader.h"
#include "../XTPReportControl.h"
#include "../XTPReportPaintManager.h"

#include "XTPTrackHeader.h"
#include "XTPTrackControl.h"
#include "XTPTrackPaintManager.h"
#include "XTPTrackUndoManager.h"
#include "XTPTrackBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTrackHeader


CXTPTrackHeader::CXTPTrackHeader(CXTPReportControl* pControl, CXTPReportColumns* pColumns)
	: CXTPReportHeader(pControl, pColumns)
{

}


CXTPTrackHeader::~CXTPTrackHeader()
{

}


void CXTPTrackHeader::Draw(CDC* pDC, CRect rcHeader, int nLeftOffset)
{

	CXTPReportHeader::Draw(pDC, rcHeader, nLeftOffset);

	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	pTrackControl->GetTrackPaintManager()->DrawTrackHeader(pDC);
}


void CXTPTrackHeader::OnMoveScrollBar(CPoint pt, BOOL bResize)
{
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	int fromPosition = pControl->TrackToPosition(m_ptStartDrag.x);
	int toPosition = pControl->TrackToPosition(pt.x);

	int delta = toPosition - fromPosition;
	if (pControl->IsLayoutRTL())
		delta = -delta;


	int nWorkAreaMin = m_nOldWorkAreaMin + (bResize != 2 ? delta : 0);
	int nWorkAreaMax = m_nOldWorkAreaMax + (bResize != 1 ? delta : 0);

	if (nWorkAreaMin < pControl->GetTimeLineMin())
	{
		if (bResize != 1)
			nWorkAreaMax = pControl->GetTimeLineMin() + nWorkAreaMax - nWorkAreaMin;
		nWorkAreaMin = pControl->GetTimeLineMin();
	}

	if (nWorkAreaMax > pControl->GetTimeLineMax())
	{
		if (bResize != 2)
			nWorkAreaMin = pControl->GetTimeLineMax() - (nWorkAreaMax - nWorkAreaMin);
		nWorkAreaMax = pControl->GetTimeLineMax();
	}

	if (nWorkAreaMax < nWorkAreaMin)
		if (bResize == 1) nWorkAreaMin = nWorkAreaMax; else nWorkAreaMax = nWorkAreaMin;


	pControl->SetWorkArea(nWorkAreaMin, nWorkAreaMax);
	pControl->RedrawControl();
}

void CXTPTrackHeader::StartDragScrollBar(BOOL bResize)
{
	// set capture to the window which received this message
	m_pControl->SetCapture();
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;
	m_nOldWorkAreaMin = pControl->GetWorkAreaMin();
	m_nOldWorkAreaMax = pControl->GetWorkAreaMax();


	::SetCursor(((CXTPTrackControl*)pControl)->m_hResizeCursor);


	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pControl)
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
			OnMoveScrollBar(pt, bResize);
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
}



void CXTPTrackHeader::OnMoveSlider(CPoint pt, BOOL bResize)
{
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea = pTrackControl->GetElementRect(xtpReportElementRectHeaderArea);

	CXTPReportColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	int nLeftOffset = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset, rcTimeLineArea.top + 10);


	int fromPosition = MulDiv(m_ptStartDrag.x, (pControl->GetTimeLineMax() - pControl->GetTimeLineMin()), rcSliderArea.Width());
	int toPosition = MulDiv(pt.x, (pControl->GetTimeLineMax() - pControl->GetTimeLineMin()), rcSliderArea.Width());

	int delta = toPosition - fromPosition;
	if (pControl->IsLayoutRTL()) delta = -delta;


	int nViewPortMin = m_nOldViewPortMin + (bResize != 2 ? delta : 0);
	int nViewPortMax = m_nOldViewPortMax + (bResize != 1 ? delta : 0);

	if (nViewPortMin < pControl->GetTimeLineMin())
	{
		if (bResize != 1)
			nViewPortMax = pControl->GetTimeLineMin() + nViewPortMax - nViewPortMin;
		nViewPortMin = pControl->GetTimeLineMin();
	}

	if (nViewPortMax > pControl->GetTimeLineMax())
	{
		if (bResize != 2)
			nViewPortMin = pControl->GetTimeLineMax() - (nViewPortMax - nViewPortMin);
		nViewPortMax = pControl->GetTimeLineMax();
	}

	if (nViewPortMax <= nViewPortMin)
	{
		if (bResize == 1) nViewPortMin = nViewPortMax - 1; else nViewPortMax = nViewPortMin + 1;
	}

	pControl->SetViewPort(nViewPortMin, nViewPortMax);
}

void CXTPTrackHeader::StartDragSlider(BOOL bResize)
{
	// set capture to the window which received this message
	m_pControl->SetCapture();
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;
	m_nOldViewPortMin= pControl->GetViewPortMin();
	m_nOldViewPortMax = pControl->GetViewPortMax();


	::SetCursor(((CXTPTrackControl*)pControl)->m_hResizeCursor);


	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pControl)
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
			OnMoveSlider(pt, bResize);
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
}


void CXTPTrackHeader::OnMoveMarker(CPoint pt, int nMarkerIndex)
{
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	int fromPosition = pControl->TrackToPosition(m_ptStartDrag.x);
	int toPosition = pControl->TrackToPosition(pt.x);

	int delta = toPosition - fromPosition;
	if (pControl->IsLayoutRTL())
		delta = -delta;

	if (nMarkerIndex == -1)
	{
		int nPosition = max(pControl->GetTimeLineMin(), min(pControl->GetTimeLineMax(), m_nOldMarkerPos + delta));
		pControl->SetTimeLinePosition(nPosition);

		pControl->SendMessageToParent(XTP_NM_TRACK_TIMELINECHANGED);
	}
	else
	{
		CXTPTrackMarker* pMarker = pControl->GetMarkers()->GetAt(nMarkerIndex);

		int nPosition = max(pControl->GetTimeLineMin(), min(pControl->GetTimeLineMax(), m_nOldMarkerPos + delta));
		pMarker->SetPosition(nPosition);

		pControl->SendMessageToParent(XTP_NM_TRACK_MARKERCHANGED, 0, pMarker);
	}

	pControl->RedrawControl();
}

void CXTPTrackHeader::StartDragMarker(int nMarkerIndex)
{
	// set capture to the window which received this message
	m_pControl->SetCapture();
	CXTPTrackControl* pControl = (CXTPTrackControl*)GetControl();

	pControl->GetUndoManager()->StartGroup();

	CPoint pt(0, 0);
	GetCursorPos(&pt);

	m_ptStartDrag = pt;
	m_nOldMarkerPos = nMarkerIndex == -1 ? pControl->GetTimeLinePosition() : pControl->GetMarkers()->GetAt(nMarkerIndex)->GetPosition();


	::SetCursor(((CXTPTrackControl*)pControl)->m_hMoveCursor);


	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pControl)
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
			OnMoveMarker(pt, nMarkerIndex);
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

	pControl->GetUndoManager()->EndGroup();

	ReleaseCapture();
}


void CXTPTrackHeader::OnLButtonDown(CPoint point)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea = pTrackControl->GetElementRect(xtpReportElementRectHeaderArea);

	CXTPReportColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	if (!pTrackColumn || !pTrackColumn->IsVisible())
	{
		CXTPReportHeader::OnLButtonDown(point);
		return;
	}

	int nLeftOffset = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset, rcTimeLineArea.top + 10);
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.bottom);
	CRect rcColumnArea(nLeftOffset, rcHeaderArea.top, nRightOffset, rcHeaderArea.bottom - 3);


	int nMarker = pTrackControl->GetMarkers()->HitTest(point);
	if (nMarker != -1)
	{
		StartDragMarker(nMarker);
		return;
	}


	if (pTrackControl->m_bShowWorkArea)
	{
		int nWorkAreaMin = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMin());
		int nWorkAreaMax = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMax());


		CRect rcWorkAreaScrollBar(max(nLeftOffset, nWorkAreaMin), rcColumnArea.top, min(nRightOffset, nWorkAreaMax), rcColumnArea.bottom);

		rcWorkAreaScrollBar.InflateRect(7, 0);

		if (rcWorkAreaScrollBar.PtInRect(point))
		{
			int nResize = 0;
			if (point.x < rcWorkAreaScrollBar.left + 7)
				nResize = 1;
			else if (point.x > rcWorkAreaScrollBar.right - 7)
				nResize = 2;

			StartDragScrollBar(nResize);

			return;
		}
	}


	rcSliderArea.DeflateRect(7, 0);

	int nZoomAreaMin = rcSliderArea.left +
		MulDiv(pTrackControl->GetViewPortMin() - pTrackControl->GetTimeLineMin(), rcSliderArea.Width(), pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin());
	int nZoomAreaMax = rcSliderArea.left +
		MulDiv(pTrackControl->GetViewPortMax() - pTrackControl->GetTimeLineMin(), rcSliderArea.Width(), pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin());

	CRect rcZoomSlider(nZoomAreaMin, rcSliderArea.top, nZoomAreaMax, rcSliderArea.bottom);
	rcZoomSlider.InflateRect(7, 0);

	if (rcZoomSlider.PtInRect(point))
	{
		int nResize = 0;
		if (point.x < rcZoomSlider.left + 7)
			nResize = 1;
		else if (point.x > rcZoomSlider.right - 7)
			nResize = 2;

		StartDragSlider(nResize);
		return;
	}


	CRect rcRullerArea(rcTimeArea.left, rcTimeArea.top, rcTimeArea.right, rcColumnArea.top);
	if (rcRullerArea.PtInRect(point))
	{
		pTrackControl->SetTimeLinePosition(pTrackControl->TrackToPosition(point.x));

		StartDragMarker(-1);
		return;
	}

	if (rcColumnArea.PtInRect(point))
		return;

	CXTPReportHeader::OnLButtonDown(point);
}



void CXTPTrackHeader::OnMouseMove(UINT nFlags, CPoint point)
{
	if (::GetCapture() != NULL)
	{
		CXTPReportHeader::OnMouseMove(nFlags, point);
		return;
	}
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CXTPReportColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	if (!pTrackColumn || !pTrackColumn->IsVisible())
	{
		CXTPReportHeader::OnMouseMove(nFlags, point);
		return;
	}
	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea = pTrackControl->GetElementRect(xtpReportElementRectHeaderArea);

	int nLeftOffset = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset, rcTimeLineArea.top + 10);
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.bottom);
	CRect rcColumnArea(nLeftOffset, rcHeaderArea.top, nRightOffset, rcHeaderArea.bottom - 3);



	int nMarker = pTrackControl->GetMarkers()->HitTest(point);
	if (nMarker != -1)
	{
		::SetCursor(((CXTPTrackControl*)GetControl())->m_hMoveCursor);
		return;
	}

	if (pTrackControl->m_bShowWorkArea)
	{
		int nWorkAreaMin = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMin());
		int nWorkAreaMax = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMax());


		CRect rcWorkAreaScrollBar(max(nLeftOffset, nWorkAreaMin), rcColumnArea.top, min(nRightOffset, nWorkAreaMax), rcColumnArea.bottom);

		rcWorkAreaScrollBar.InflateRect(7, 0);

		if (rcWorkAreaScrollBar.PtInRect(point))
		{
			::SetCursor(((CXTPTrackControl*)GetControl())->m_hResizeCursor);
			return;
		}
	}


	int nZoomAreaMin = rcSliderArea.left +
		MulDiv(pTrackControl->GetViewPortMin() - pTrackControl->GetTimeLineMin(), rcSliderArea.Width(), pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin());
	int nZoomAreaMax = rcSliderArea.left +
		MulDiv(pTrackControl->GetViewPortMax() - pTrackControl->GetTimeLineMin(), rcSliderArea.Width(), pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin());

	CRect rcZoomSlider(nZoomAreaMin, rcSliderArea.top, nZoomAreaMax, rcSliderArea.bottom);
	rcZoomSlider.InflateRect(7, 0);

	if (rcZoomSlider.PtInRect(point))
	{
		::SetCursor(((CXTPTrackControl*)GetControl())->m_hResizeCursor);
		return;

	}


	CXTPReportHeader::OnMouseMove(nFlags, point);
}
