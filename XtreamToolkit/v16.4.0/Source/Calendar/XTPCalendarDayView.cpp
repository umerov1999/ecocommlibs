// XTPCalendarDayView.cpp: implementation of the CXTPCalendarDayView class.
//
// This file is a part of the XTREME CALENDAR MFC class library.
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPColorManager.h"
#include "Resource.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarOptions.h"
#include "XTPCalendarUtils.h"
#include "XTPCalendarNotifications.h"
#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarTimeZoneHelper.h"

#include "XTPCalendarEvent.h"
#include "XTPCalendarResource.h"
#include "XTPCalendarData.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewDay.h"
#include "XTPCalendarViewEvent.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarDayViewEvent.h"
#include "XTPCalendarDayViewDay.h"
#include "XTPCalendarDayView.h"

#include "XTPCalendarDayViewTimeScale.h"

#include "XTPCalendarPaintManager.h"
#include "XTPCalendarTheme.h"
#include "XTPCalendarThemePrevNextEventButton.h"
#include "XTPCalendarThemePrevNextEventButtons.h"
#include "XTPCalendarThemeOffice2007.h"

#include "XTPCalendarControl.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define XTP_SCROLL_TIMER_RESOLUTION_MS  30
#define XTP_REDRAW_TIMER_RESOLUTION_MS  2000

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTPCalendarDayView, CXTPCalendarView)

CXTPCalendarDayView::CXTPCalendarDayView(CXTPCalendarControl* pCalendarControl) :
		TBase(pCalendarControl, xtpCalendarDayView),
		m_ptLastMousePos(0, 0)
{
	m_pTimeScaleHeader = new CXTPCalendarDayViewTimeScale(this);
	m_pTimeScaleHeader2 = new CXTPCalendarDayViewTimeScale(this, FALSE, FALSE);

	m_LayoutX.m_nRowCount = 0;
	m_LayoutX.m_nVisibleRowCount = 0;
	m_LayoutX.m_nTopRow = 0;

	m_LayoutX.m_nTopAllDayRow = 0;
	m_LayoutX.m_nVisibleAllDayCount = 0;

	m_LayoutX.m_rcAllDayEvents.SetRectEmpty();
	m_LayoutX.m_rcDayHeader.SetRectEmpty();

	m_LayoutX.m_nAllDayEventsCountMax = 0;
	m_LayoutX.m_nAllDayEventHeight = 0;

	m_nAllDayEventsCountMin_WhenDrag = 0;

	m_spDraggingStartOffset_Time = 0.;

	m_bShowAllDayEventsOnly = FALSE;
	m_bHideAllDayEventsArea = FALSE;

	m_nScrollOffsetX = 0;
	m_nMinColumnWidth = -1;
	// 0 - means disabled; -1 use auto for multiresources; -X use for multiresources only; +X used always (in single and multi resources mode).

	// add today to the day view by default
	COleDateTime dtNow(CXTPCalendarUtils::GetCurrentTime());
	m_arDays.Add(new CXTPCalendarDayViewDay(this, dtNow));

	// go to the beginning of the work day
	int nHour = 0, nMin = 0, nSec = 0;
	XTP_SAFE_CALL1(m_pControl, GetWorkDayStartTime(nHour, nMin, nSec));
	int nPos = GetCellNumber(nHour, nMin, nSec, FALSE);
	_ScrollV(nPos, nPos);

	m_dwScrollingEventTimerID = 0;
	m_dwScrollingEventTimerHorID = 0;

	m_bMouseOutOfDragArea = FALSE;
	m_dtDraggingStartPoint = COleDateTime(0, 0, 0, 0, 0, 0);

	m_dtTimeScaleMin.SetTime(0, 0, 0);
	m_dtTimeScaleMax.SetTime(23, 59, 59);

	//m_bEcoMode = FALSE;
	m_bEcoAllDayMode = FALSE;
	m_bPreventDragAllDayEventInDayView = FALSE;
	m_bDraftMode = FALSE;
	m_bPrintRepeatHeaderFooterMode = FALSE;
	//m_bPrintRepeatHeaderFooterMode = TRUE;
	m_bDF_mode = FALSE;
	m_bPrintFullScale = TRUE;
	m_nDayHeaderPeriod = 1;
	m_nDayViewPrintInterval = 60;
	m_nCurPage = 0;
	m_bUseNewHitTestCode = TRUE;
	m_bShowLinks = FALSE;
	m_bHideLinkContainer = FALSE;

	m_dwRedrawNowLineTimerID = 0;
	m_dtLastRedrawTime = CXTPCalendarUtils::GetCurrentTime();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarDayView::~CXTPCalendarDayView()
{
	CMDTARGET_RELEASE(m_pTimeScaleHeader);
	CMDTARGET_RELEASE(m_pTimeScaleHeader2);

	ClearDays();
}

void CXTPCalendarDayView::SetIsHideAllDayEventsArea(BOOL bHide)
{
	CXTPCalendarControl::CUpdateContext updateContext2(GetCalendarControl(), xtpCalendarUpdateRedraw | xtpCalendarUpdateLayout);
	m_bHideAllDayEventsArea = bHide;
}

XTPCalendarViewType CXTPCalendarDayView::GetViewType() const
{
//  int nWWMask = m_pControl ? m_pControl->GetWorkWeekMask() : xtpCalendarDayMo_Fr;
	int nWWMask = xtpCalendarDayMo_Fr; //Default 62 as 2 + 4 + 8 + 16 + 32
	if (m_pControl)
		nWWMask = m_pControl->GetWorkWeekMask();

	int nDaysCount = GetViewDayCount();
	int nWWCount = CXTPCalendarUtils::GetDayOfWeekCount(nWWMask);
	if (nDaysCount != nWWCount && nDaysCount != 7)
		return xtpCalendarDayView;

	int nFirstDayOfWeek = xtpCalendarDayMonday; //Default 2
	if (m_pControl)
		nFirstDayOfWeek = m_pControl->GetFirstDayOfWeek();

	int nNextWWeekIdx = nFirstDayOfWeek;

//Dennis Volodomanov Issue #: 18604
//the first call returns a 7 (for example, for a Saturday)
//and then this 7 is passed into the function that expects a bitmask.

	BOOL bWDOrderFull = TRUE;
	int nViewType = xtpCalendarWorkWeekView;

	for (int i = 0; i < nDaysCount; i++)
	{
		COleDateTime dtDay = GetViewDayDate(i);
		int nDayOfWeek = dtDay.GetDayOfWeek();
		int nDayMask = CXTPCalendarUtils::GetDayOfWeekMask(nDayOfWeek);

		if ((nWWMask & nDayMask) == 0)
			nViewType = xtpCalendarDayView;

		if (nNextWWeekIdx != nDayOfWeek)
			bWDOrderFull = FALSE;

		nNextWWeekIdx = (nNextWWeekIdx % 7) + 1;
	}

	if (nDaysCount == 7 && bWDOrderFull)
		nViewType = xtpCalendarFullWeekView;

	return (XTPCalendarViewType)nViewType;
}

void CXTPCalendarDayView::ClearDays()
{
	m_arDays.RemoveAll();
}

COleDateTime CXTPCalendarDayView::GetCellTime(int nCell) const
{
	double dblTime = (long)nCell * (double)GetCellDuration();
	COleDateTime dtCellTime(m_dtTimeScaleMin);
	dtCellTime += (DATE)dblTime;
	return dtCellTime;
}

COleDateTimeSpan CXTPCalendarDayView::GetCellDuration() const
{
	return m_pTimeScaleHeader ? m_pTimeScaleHeader->GetScaleInterval() : (double) 0;
}

int CXTPCalendarDayView::GetCellNumber(COleDateTime dtTime, BOOL bForEndTime) const
{
	double dblTime = (double) GetDateTimeTime(dtTime) - m_dtTimeScaleMin;


	//dblTime += (double) COleDateTimeSpan(0,0,0,1);

	double dblEpsilon = XTP_HALF_SECOND * (bForEndTime ? -1 : 1);
	//double dblEpsilon = .5 * (bForEndTime ? -1 : 1);
	double dblCellDur = (double) GetCellDuration();
	if (dblCellDur <= XTP_HALF_SECOND)
		dblCellDur = XTP_HALF_SECOND * 2 * 60 * 30; //30min if too small

	double dCell = dblTime / dblCellDur + dblEpsilon;
	int nCell = int (dCell);
	return nCell;
}

int CXTPCalendarDayView::GetCellNumber(int nHour, int nMin, int nSec, BOOL bForEndTime) const
{
	COleDateTime dtTime;
	dtTime.SetTime(nHour, nMin, nSec);
	return GetCellNumber(dtTime, bForEndTime);
}

void CXTPCalendarDayView::ScrollV(int nIndex, int nPos_raw)
{
	if (m_bScrollV_Disabled)
		return;

	_ScrollV(nIndex, nPos_raw);
}

void CXTPCalendarDayView::_ScrollV(int nIndex, int /*nPos_raw*/)
{
	if (m_bShowAllDayEventsOnly)
	{
		if (nIndex + m_LayoutX.m_nVisibleAllDayCount >= m_LayoutX.m_nAllDayEventsCountMax)
			nIndex = m_LayoutX.m_nAllDayEventsCountMax - m_LayoutX.m_nVisibleAllDayCount;

		if (nIndex < 0)
			nIndex = 0;

		m_LayoutX.m_nTopAllDayRow = nIndex;
	}
	else
	{
		if (nIndex + m_LayoutX.m_nVisibleRowCount >= m_LayoutX.m_nRowCount)
			nIndex = m_LayoutX.m_nRowCount - m_LayoutX.m_nVisibleRowCount;

		if (nIndex == m_LayoutX.m_nTopRow)
			return;
		m_LayoutX.m_nTopRow = nIndex;
	}
}

void CXTPCalendarDayView::ScrollH(int nPos, int nPos_raw)
{
	SCROLLINFO si;
	if (!GetScrollBarInfoH(&si))
		m_nScrollOffsetX = 0;

	int nXmax = si.nMax - si.nPage + 1;

	m_nScrollOffsetX = min(max(nPos, si.nMin), nXmax);

	if (!m_bScrollH_Disabled)
	{
		if (RecalcMinColumnWidth() > 0)
		{
			RedrawControl();
		}
		else
		{
			if (nPos_raw < 0)
				ScrollDaysToPrev();
			else if (nPos_raw > nXmax)
				ScrollDaysToNext();
		}
	}
}

BOOL CXTPCalendarDayView::GetScrollBarInfoV(SCROLLINFO* pSI) const
{
	ASSERT(pSI);
	if (!pSI || m_bScrollV_Disabled)
		return FALSE;

	if (m_bShowAllDayEventsOnly)
	{
		pSI->nPos = m_LayoutX.m_nTopAllDayRow;
		pSI->nMax = m_LayoutX.m_nAllDayEventsCountMax - 1;
		pSI->nPage = m_LayoutX.m_nVisibleAllDayCount;
		return m_LayoutX.m_nVisibleAllDayCount < m_LayoutX.m_nAllDayEventsCountMax;
	}
	else
	{
		pSI->nPos = m_LayoutX.m_nTopRow;
		pSI->nMax = m_LayoutX.m_nRowCount - 1;
		pSI->nPage = m_LayoutX.m_nVisibleRowCount;
		return m_LayoutX.m_nVisibleRowCount != m_LayoutX.m_nRowCount;
	}
}

BOOL CXTPCalendarDayView::GetScrollBarInfoH(SCROLLINFO* pSI, int* pnScrollStep) const
{
	UNREFERENCED_PARAMETER(pnScrollStep);

	ASSERT(pSI);
	if (!pSI)
		return FALSE;

	int nMinColumnWidth = RecalcMinColumnWidth();
	if (nMinColumnWidth <= 0)
		return FALSE;

	CRect rcClient = _GetScrollRectClient();
	int nColumnsCount = GetTotalGroupsCount();
	int nFullWidth = nColumnsCount * nMinColumnWidth;
	if (nFullWidth <= rcClient.Width())
		return FALSE;

	pSI->nPos = min(m_nScrollOffsetX, nFullWidth);
	pSI->nMin = 0;
	pSI->nMax = nFullWidth;
	pSI->nPage = rcClient.Width();

	return TRUE;
}

int CXTPCalendarDayView::GetTotalGroupsCount() const
{
	int nGroupsCount = 0;

	int nDays = GetViewDayCount();
	for (int i = 0; i < nDays; i++)
	{
		CXTPCalendarDayViewDay* pDay = GetViewDay(i);
		ASSERT(pDay);
		if (pDay)
			nGroupsCount += pDay->GetViewGroupsCount();
	}
	return nGroupsCount;
}

int CXTPCalendarDayView::RecalcMinColumnWidth() const
{
	int nMinColumnWidth = m_nMinColumnWidth;
	if (nMinColumnWidth < 0)
	{
		// automatically use horizontal scrolling for multi-resources mode
		if (GetResources() && GetResources()->GetCount() <= 1)
			return -1;

		if (nMinColumnWidth == -1)
			nMinColumnWidth = ::GetSystemMetrics(SM_CXFULLSCREEN) / 8;
//          nMinColumnWidth = ::GetSystemMetrics(SM_CXFULLSCREEN) / 6;
//in average screen case this setting allows to see 6 columns
//(2 days with 3 schedules in day) without scrolling
		else
			nMinColumnWidth = abs(nMinColumnWidth);
	}
	return nMinColumnWidth;
}

int CXTPCalendarDayView::GetAllDayEventsMaxCount() const
{
	int nAllDayMax = 0;
	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarDayViewDay* pViewDay = GetViewDay(i);
		ASSERT(pViewDay);
		if (!pViewDay)
			continue;
		int nGroupsCount = pViewDay->GetViewGroupsCount();
		for (int g = 0; g < nGroupsCount; g++)
		{
			int nAllDay = 0;

			CXTPCalendarDayViewGroup* pViewGroup = pViewDay->GetViewGroup(g);
			ASSERT(pViewGroup);
			if (!pViewGroup)
			{
				continue;
			}
			int nECount = pViewGroup->GetViewEventsCount();
			for (int j = 0; j < nECount; j++)
			{
				CXTPCalendarDayViewEvent* pViewEvent = pViewGroup->GetViewEvent(j);
				ASSERT(pViewEvent);
				if (pViewEvent && pViewEvent->IsMultidayEvent())
				{
					nAllDay++;
				}
			}

			nAllDayMax = max(nAllDayMax, nAllDay);
		}
	}
	return nAllDayMax;
}

void CXTPCalendarDayView::AdjustAllDayEvents()
{
	if (GetResources() && GetResources()->GetCount() > 1)
		return;

	int nDaysCount = GetViewDayCount();
	for (int nDayIdx = 0; nDayIdx < nDaysCount; nDayIdx++)
	{
		CXTPCalendarDayViewDay* pViewDay = GetViewDay(nDayIdx);
		if (!pViewDay)
		{
			ASSERT(FALSE);
			continue;
		}
		// go through all days to look for multiday events
		int nGroupsCount = pViewDay->GetViewGroupsCount();
		for (int g = 0; g < nGroupsCount; g++)
		{
			CXTPCalendarDayViewGroup* pViewGroup = pViewDay->GetViewGroup(g);
			ASSERT(pViewGroup);
			if (!pViewGroup)
			{
				continue;
			}
			int nEventsCount = pViewGroup->GetViewEventsCount();
			for (int nEvent = 0; nEvent < nEventsCount; nEvent++)
			{
				CXTPCalendarViewEvent* pViewEvent = pViewGroup->GetViewEvent(nEvent);
				if (!pViewEvent)
				{
					ASSERT(FALSE);
					continue;
				}

				int nMDEFlags = pViewEvent->GetMultiDayEventFlags();

				if ((nMDEFlags & xtpCalendarMultiDayMSmask) || !(nMDEFlags & xtpCalendarMultiDayFMLmask))
					continue; // already marked or not multi day

				pViewEvent->SetMultiDayEvent_MasterSlave(TRUE, NULL);
				int nMasterPlace = pViewEvent->GetEventPlaceNumber();

				if ((nMDEFlags & xtpCalendarMultiDayLast) == 0)
				{
					// find other visible instances and mark them as Slave
					for (int i = nDayIdx + 1; i < nDaysCount; i++)
					{
						CXTPCalendarViewDay* pViewDay2 = GetViewDay(i);
						if (XTP_SAFE_GET1(pViewDay2, GetViewGroupsCount(), 0) != 1)
						{
							ASSERT(FALSE);
							break;
						}
						CXTPCalendarViewEvent* pViewEvent2 = XTP_SAFE_GET2(pViewDay2,
							GetViewGroup_(0), GetViewEventByEvent_(pViewEvent->GetEvent()), NULL);

						if (!pViewEvent2)
							break;

						pViewEvent->AddMultiDayEvent_Slave(pViewEvent2, nMasterPlace);
					}
				}
			}
		}
	}
}

int CXTPCalendarDayView::CalculateHeaderFormatAndHeight(CDC* pDC, int nCellWidth)
{
	ASSERT(pDC);

	const int cnMinHeight = 19;

	CXTPCalendarViewPart* pPart = XTP_SAFE_GET1(GetPaintManager(), GetDayViewHeaderPart(), NULL);
	ASSERT(pPart);
	if (!pPart || !pDC)
	{
		return cnMinHeight;
	}

	CalculateHeaderFormat(pDC, nCellWidth, &pPart->GetTextFont());

	CString strHearderTest = _T("QW(");

	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarDayViewDay* pDay = GetViewDay(i);
		ASSERT(pDay);
		if (pDay)
		{
			strHearderTest += pDay->GetCaption();
		}
	}

	CXTPFontDC fnt(pDC, &pPart->GetTextFont());

	CSize sizeH = pDC->GetTextExtent(strHearderTest);
	int nHeight = max(cnMinHeight, sizeH.cy + 6);

	return nHeight;
}

CXTPCalendarData* CXTPCalendarDayView::_GetDataProviderByConnStr(LPCTSTR pcszConnStr, BOOL bCompareNoCase)
{
	UNREFERENCED_PARAMETER(pcszConnStr);
	UNREFERENCED_PARAMETER(bCompareNoCase);

	return NULL;
}

BOOL CXTPCalendarDayView::IsGroupHeaderVisible() const
{
	BOOL bHideHeader = XTP_SAFE_GET1(GetResources(), GetCount(), 0) == 1 &&
			XTP_SAFE_GET2(GetResources(), GetAt(0), IsSchedulesSetEmpty(), FALSE);
	return !bHideHeader;
}

void CXTPCalendarDayView::AdjustLayout(CDC* pDC, const CRect& rcView, BOOL bCallPostAdjustLayout)
{
	ASSERT(pDC);
	if (!pDC)
	{
		return;
	}

	XTP_SAFE_CALL1(m_pTimeScaleHeader, SetCaption(GetScaleText()) );
	XTP_SAFE_CALL1(m_pTimeScaleHeader, SetVisible(!m_bShowAllDayEventsOnly) );

	XTP_SAFE_CALL1(m_pTimeScaleHeader2, SetCaption(GetScale2Text()) );
	XTP_SAFE_CALL1(m_pTimeScaleHeader2, SetVisible(IsScale2Visible()) );



	TBase::AdjustLayout(pDC, rcView, FALSE);

	// COleDateTimeSpan spDaySpan(1, 0, 0, 0);
	COleDateTimeSpan spDaySpan(m_dtTimeScaleMax - m_dtTimeScaleMin);

	double dRows = spDaySpan / max((double) m_pTimeScaleHeader->GetScaleInterval(), XTP_HALF_SECOND * 2);
	//m_LayoutX.m_nRowCount = (int)(dRows + XTP_HALF_SECOND);
	m_LayoutX.m_nRowCount = (int)(dRows + .5);
	// to prevent loosing 1 :  (int)(47.99999999999) = 47; // need 48 !
	CRect rcView2 = rcView;
	// alternative time scale
	if (IsScale2Visible())
	{
		int nTimeScale2Width = XTP_SAFE_GET1(m_pTimeScaleHeader2, CalcWidth(pDC), 0);

		m_pTimeScaleHeader2->m_Layout.m_rcHeader.SetRect(
			rcView2.left, rcView2.top,
			rcView2.left + nTimeScale2Width, rcView2.bottom);
		rcView2.left += nTimeScale2Width;

		AdjustScale2TimeZone();
	}

	// regular time scale
	if (m_pTimeScaleHeader)
	{
		int nTimeScaleWidth = XTP_SAFE_GET1(m_pTimeScaleHeader, CalcWidth(pDC), 0);
		m_pTimeScaleHeader->m_Layout.m_rcHeader.SetRect(
			rcView2.left, rcView2.top,
			rcView2.left + nTimeScaleWidth, rcView2.bottom);
		rcView2.left += nTimeScaleWidth;
	}

	CXTPCalendarViewPart* pPart = XTP_SAFE_GET1(GetPaintManager(), GetDayViewHeaderPart(), NULL);
	int nDaysCount = GetViewDayCount();
	if (nDaysCount <= 0 || !pPart)
		return;

	int nColumnsCount = GetTotalGroupsCount();

	// Adjust Header
	int nCellWidth = rcView2.Width() / max(nDaysCount, 1);
	int nDayHeaderHeight = CalculateHeaderFormatAndHeight(pDC, nCellWidth);

	int nMinRowHeight = nDayHeaderHeight;
	m_LayoutX.m_nAllDayEventsCountMax = max(1, GetAllDayEventsMaxCount());

	if (m_eDraggingMode == xtpCalendaDragModeResizeBegin
		|| m_eDraggingMode == xtpCalendaDragModeResizeEnd
		|| _IsDragModeCopyMove(m_eDraggingMode))
	{
		m_LayoutX.m_nAllDayEventsCountMax = max(m_LayoutX.m_nAllDayEventsCountMax, m_nAllDayEventsCountMin_WhenDrag);
		m_nAllDayEventsCountMin_WhenDrag = m_LayoutX.m_nAllDayEventsCountMax;
	}

	BOOL bGroupHeaderVisible = IsGroupHeaderVisible();
	int nDGHeadersHeight = nDayHeaderHeight * (bGroupHeaderVisible ? 2 : 1);

	// day view part
	int nRowCount = m_LayoutX.m_nRowCount;
	int nTotalRows = nRowCount + m_LayoutX.m_nAllDayEventsCountMax + (bGroupHeaderVisible ? 2 : 1);

	m_Layout.m_nRowHeight = max(nMinRowHeight, rcView2.Height() / max(nTotalRows, 1));

	double dVisibleRowCountMax = (double)(rcView2.Height()) / (double)(max(m_Layout.m_nRowHeight, 1));
	double dVisibleRowCount = dVisibleRowCountMax - (0.2 + m_LayoutX.m_nAllDayEventsCountMax) -
								(bGroupHeaderVisible ? 2 : 1);

	m_LayoutX.m_nVisibleRowCount = max((int)dVisibleRowCount, max(0, (int)dVisibleRowCountMax/2));
	m_LayoutX.m_nVisibleRowCount = max(0, min(nRowCount, m_LayoutX.m_nVisibleRowCount));

	if (!pDC->IsPrinting() && m_LayoutX.m_nTopRow + m_LayoutX.m_nVisibleRowCount >= m_LayoutX.m_nRowCount)
		m_LayoutX.m_nTopRow = m_LayoutX.m_nRowCount - m_LayoutX.m_nVisibleRowCount;

	int nAllDayEventsHeight = max(0, rcView2.Height() - (m_LayoutX.m_nVisibleRowCount * m_Layout.m_nRowHeight + nDGHeadersHeight));

	m_LayoutX.m_rcDayHeader.SetRect(
		rcView2.left,
		rcView2.top,
		rcView2.right,
		rcView2.top + nDayHeaderHeight);

	m_LayoutX.m_rcAllDayEvents.SetRect(
		rcView2.left,
		rcView2.top + nDGHeadersHeight,
		rcView2.right,
		rcView2.top + nAllDayEventsHeight + nDGHeadersHeight);

	m_LayoutX.m_nVisibleAllDayCount = max(1, nAllDayEventsHeight / m_Layout.m_nRowHeight);

	int nCols = 0;
	for (int i = 0; i < nDaysCount; i++)
	{
		CXTPCalendarDayViewDay* pDay = GetViewDay(i);
		if (!pDay)
		{
			ASSERT(FALSE);
			continue;
		}

		//int nWidth = (i == nDaysCount - 1) ? rcView2.Width() : rcView2.Width() / (nColumnsCount - nCols);
		int nGroupsCount = pDay->GetViewGroupsCount();
		int nWidth = (rcView2.Width() / (nColumnsCount - nCols)) * nGroupsCount;
		//Last Column
		if (i == nDaysCount - 1)
			nWidth = rcView2.Width();

		CRect rcDay(rcView2.left, rcView2.top, rcView2.left + nWidth, rcView2.bottom);
		pDay->AdjustLayout(pDC, rcDay);

		rcView2.left += nWidth;
		nCols += pDay->GetViewGroupsCount();
	}

	//----------------------------------------------
	if (nDaysCount && XTP_SAFE_GET1(GetViewDay(0), GetViewGroupsCount(), 0)
		&& GetViewDay(0)->GetViewGroup(0))
	{
		CRect rcAllDayEvents = GetViewDay(0)->GetViewGroup(0)->GetAllDayEventsRect();
		ASSERT(rcAllDayEvents.Height() == m_LayoutX.m_rcAllDayEvents.Height());

		m_LayoutX.m_rcAllDayEvents.top = rcAllDayEvents.top;
		m_LayoutX.m_rcAllDayEvents.bottom = rcAllDayEvents.bottom;
	}


	AdjustAllDayEvents();

	//----------------------------------------------------------
	XTP_SAFE_CALL1(m_pTimeScaleHeader, AdjustLayout(pDC) );
	XTP_SAFE_CALL1(m_pTimeScaleHeader2, AdjustLayout(pDC) );

	//---------------------------------------------------------------------------
	int nDVCount = GetViewDayCount();
	if (nDVCount)
	{
		COleDateTime dtFirstDay = GetViewDayDate(0);
		COleDateTime dtLastDay = GetViewDayDate(nDVCount-1);
		BOOL bSelChanged = FALSE;

		if (m_selectedBlock.dtEnd.GetStatus() != COleDateTime::valid ||
			m_selectedBlock.dtBegin.GetStatus() != COleDateTime::valid)
		{
			int nSelRow = GetTopRow() + min(2, GetVisibleRowCount());

			m_selectedBlock.dtBegin = CXTPCalendarUtils::UpdateTime(dtFirstDay, GetCellTime(nSelRow));
			m_selectedBlock.dtEnd = m_selectedBlock.dtBegin + GetCellDuration();
			m_selectedBlock.bAllDayEvent = FALSE;
			m_selectedBlock.nGroupIndex = 0;

			bSelChanged = TRUE;
		}

		COleDateTime dtSelBeginDay = GetDateTimeDay(m_selectedBlock.dtBegin);
		COleDateTime dtSelEndDay = GetDateTimeDay(m_selectedBlock.dtEnd - COleDateTimeSpan(0, 0, 0, 1));

		// compare with the first day
		if (dtSelBeginDay < dtFirstDay)
		{
			m_selectedBlock.dtBegin = CXTPCalendarUtils::UpdateDate(m_selectedBlock.dtBegin, dtFirstDay);
			bSelChanged = TRUE;
		}
		if (dtSelEndDay < dtFirstDay)
		{
			m_selectedBlock.dtEnd = CXTPCalendarUtils::UpdateDate(m_selectedBlock.dtEnd, dtFirstDay);
			bSelChanged = TRUE;
		}
		// compare with the last day
		if (dtSelEndDay > dtLastDay)
		{
			m_selectedBlock.dtEnd = CXTPCalendarUtils::UpdateDate(m_selectedBlock.dtEnd, dtLastDay);
			bSelChanged = TRUE;
		}
		if (dtSelBeginDay > dtLastDay)
		{
			m_selectedBlock.dtBegin = CXTPCalendarUtils::UpdateDate(m_selectedBlock.dtBegin, dtLastDay);
			bSelChanged = TRUE;
		}

		if (bSelChanged)
			CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
	}
	//---------------------------------------------------------------------------
	if (bCallPostAdjustLayout)
		OnPostAdjustLayout();
}


void CXTPCalendarDayView::AdjustLayout2(CDC* pDC, const CRect& rcView, BOOL bCallPostAdjustLayout)
{
	if (!pDC || !GetTheme())
	{
		ASSERT(FALSE);
		return;
	}
	TBase::AdjustLayout2(pDC, rcView, bCallPostAdjustLayout);

	//*** Pre-Theme Adjust Layout step
	XTP_SAFE_CALL1(m_pTimeScaleHeader, SetCaption(GetScaleText()) );
	XTP_SAFE_CALL1(m_pTimeScaleHeader, SetVisible(!m_bShowAllDayEventsOnly));
	XTP_SAFE_CALL1(m_pTimeScaleHeader2, SetCaption(GetScale2Text()) );
	XTP_SAFE_CALL1(m_pTimeScaleHeader2, SetVisible(IsScale2Visible()) );

	if (IsScale2Visible())
	{
		AdjustScale2TimeZone();
	}

	COleDateTimeSpan spDaySpan(m_dtTimeScaleMax - m_dtTimeScaleMin);

	double dRows = spDaySpan / max((double)m_pTimeScaleHeader->GetScaleInterval(), XTP_HALF_SECOND * 2);
	//m_LayoutX.m_nRowCount = (int)(dRows + XTP_HALF_SECOND);
	m_LayoutX.m_nRowCount = (int)(dRows + .5);
	// to prevent loosing 1 :  (int)(47.99999999999) = 47; // need 48 !

	m_LayoutX.m_nAllDayEventsCountMax = max(1, GetAllDayEventsMaxCount());

	if (m_eDraggingMode == xtpCalendaDragModeResizeBegin
		|| m_eDraggingMode == xtpCalendaDragModeResizeEnd
		|| _IsDragModeCopyMove(m_eDraggingMode))
	{
		m_LayoutX.m_nAllDayEventsCountMax = max(m_LayoutX.m_nAllDayEventsCountMax, m_nAllDayEventsCountMin_WhenDrag);
		m_nAllDayEventsCountMin_WhenDrag = m_LayoutX.m_nAllDayEventsCountMax;
	}

	GetTheme()->GetDayViewPart()->AdjustLayout(pDC, rcView);

	//*** Post-Theme Adjust Layout step
	//---------------------------------------------------------------------------
	int nDVCount = GetViewDayCount();
	if (nDVCount)
	{
		COleDateTime dtFirstDay = GetViewDayDate(0);
		COleDateTime dtLastDay = GetViewDayDate(nDVCount-1);
		BOOL bSelChanged = FALSE;

		if (m_selectedBlock.dtEnd.GetStatus() != COleDateTime::valid ||
			m_selectedBlock.dtBegin.GetStatus() != COleDateTime::valid)
		{
			int nSelRow = GetTopRow() + min(2, GetVisibleRowCount());

			m_selectedBlock.dtBegin = CXTPCalendarUtils::UpdateTime(dtFirstDay, GetCellTime(nSelRow));
			m_selectedBlock.dtEnd = m_selectedBlock.dtBegin + GetCellDuration();
			m_selectedBlock.bAllDayEvent = FALSE;
			m_selectedBlock.nGroupIndex = 0;

			bSelChanged = TRUE;
		}

		COleDateTime dtSelBeginDay = GetDateTimeDay(m_selectedBlock.dtBegin);
		COleDateTime dtSelEndDay = GetDateTimeDay(m_selectedBlock.dtEnd-COleDateTimeSpan(0, 0, 0, 1));

		// compare with the first day
		if (dtSelBeginDay < dtFirstDay)
		{
			m_selectedBlock.dtBegin = CXTPCalendarUtils::UpdateDate(m_selectedBlock.dtBegin, dtFirstDay);
			bSelChanged = TRUE;
		}
		if (dtSelEndDay < dtFirstDay)
		{
			m_selectedBlock.dtEnd = CXTPCalendarUtils::UpdateDate(m_selectedBlock.dtEnd, dtFirstDay);
			bSelChanged = TRUE;
		}
		// compare with the last day
		if (dtSelEndDay > dtLastDay)
		{
			m_selectedBlock.dtEnd = CXTPCalendarUtils::UpdateDate(m_selectedBlock.dtEnd, dtLastDay);
			bSelChanged = TRUE;
		}
		if (dtSelBeginDay > dtLastDay)
		{
			m_selectedBlock.dtBegin = CXTPCalendarUtils::UpdateDate(m_selectedBlock.dtBegin, dtLastDay);
			bSelChanged = TRUE;
		}

		if (bSelChanged)
		{
			CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
		}
	}
	//---------------------------------------------------------------------------
	if (bCallPostAdjustLayout)
	{
		OnPostAdjustLayout();

		GetTheme()->GetDayViewPart()->OnPostAdjustLayout();
	}
}

void CXTPCalendarDayView::Draw2(CDC* pDC)
{
	if (!GetTheme())
	{
		ASSERT(FALSE);
		return;
	}

	GetTheme()->GetDayViewPart()->Draw(pDC);

	m_dtLastRedrawTime = CXTPCalendarUtils::GetCurrentTime();
}

void CXTPCalendarDayView::OnDraw(CDC* pDC)
{
	SCROLLINFO si;
	if (!GetScrollBarInfoH(&si) || pDC->IsPrinting())
	{
		TBase::OnDraw(pDC);
		return;
	}

	if (m_pTimeScaleHeader)
		m_pTimeScaleHeader->MoveTo(m_nScrollOffsetX);

	if (m_pTimeScaleHeader2 && IsScale2Visible())
		m_pTimeScaleHeader2->MoveTo(m_nScrollOffsetX);

	TBase::OnDraw(pDC);

}

void CXTPCalendarDayView::Draw(CDC* pDC)
{
	pDC->SetBkMode(TRANSPARENT);

	// days
	int i = 0;
	const int nCount = GetViewDayCount();
	for (i = nCount - 1; i >= 0; i--)
	{
		XTP_SAFE_CALL1(GetViewDay(i), Draw(pDC));
	}

	for (i = nCount - 1; i >= 0; i--)
	{
		XTP_SAFE_CALL1(GetViewDay(i), DrawTopmostLayer(pDC));
	}

	// alternative time scale
	if (IsScale2Visible() && m_pTimeScaleHeader2)
		m_pTimeScaleHeader2->Draw(pDC);

	// regular time scale
	XTP_SAFE_CALL1(m_pTimeScaleHeader, Draw(pDC));

	m_dtLastRedrawTime = CXTPCalendarUtils::GetCurrentTime();
}

BOOL CXTPCalendarDayView::HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pHitTest->pt = pt;

	if (m_pTimeScaleHeader2 && m_pTimeScaleHeader2->HitTestDateTime(pHitTest))
		return TRUE;

	if (m_pTimeScaleHeader && m_pTimeScaleHeader->HitTestDateTime(pHitTest))
		return TRUE;

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());


	BOOL bShowExpandButton = XTP_SAFE_GET1(GetCalendarOptions(), bShowAllDayExpandButton, TRUE);

	BOOL bNeedScroll = FALSE;
	int iVisibleEventsMaxCount = m_LayoutX.m_nAllDayEventsCountMax;
	int nVisAllDayEvents = m_LayoutX.m_nVisibleAllDayCount;
	bNeedScroll = (iVisibleEventsMaxCount > nVisAllDayEvents);


	if ((pThemeX != NULL) && bShowExpandButton)
	{
		// check if allday expand button is hit
		if (bNeedScroll && m_LayoutX.m_rcAllDayEvents.PtInRect(pt))
		{
			int nDaysCount = GetViewDayCount();
			for (int nDayIdx = 0; nDayIdx < nDaysCount; nDayIdx++)
			{
				CXTPCalendarDayViewDay* pViewDay = GetViewDay(nDayIdx);
				if (!pViewDay)
				{
					ASSERT(FALSE);
					continue;
				}

				int nGroupsCount = pViewDay->GetViewGroupsCount();
				for (int g = 0; g < nGroupsCount; g++)
				{
					CXTPCalendarDayViewGroup* pViewGroup = pViewDay->GetViewGroup(g);
					ASSERT(pViewGroup);
					if (!pViewGroup)
					{
						continue;
					}

					if(pThemeX->ExpandDayButton_HitTest(pt, pViewGroup->GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollDown), FALSE))
					{
						XTP_CALENDAR_HITTESTINFO emptyData;
						*pHitTest = emptyData;
						pHitTest->uHitCode = xtpCalendarHitTestAllDayEventsScrollDown;
						return TRUE;
					}

					if(pThemeX->ExpandDayButton_HitTest(pt, pViewGroup->GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollUp), TRUE))
					{
						XTP_CALENDAR_HITTESTINFO emptyData;
						*pHitTest = emptyData;
						pHitTest->uHitCode = xtpCalendarHitTestAllDayEventsScrollUp;
						return TRUE;
					}

					int iAllDayCount = pViewGroup->GetEventsCountInAllDayArea();
					if (iAllDayCount > nVisAllDayEvents)
					{
						CRect rcIconsMax1 = pViewGroup->GetAllDayEventsRect();
						rcIconsMax1.top = rcIconsMax1.bottom - 13;
						rcIconsMax1.bottom = rcIconsMax1.top + 9;
						rcIconsMax1.left = pViewGroup->GetAllDayEventsRect().right - 17;
						rcIconsMax1.right = rcIconsMax1.left + 14;

						if (pThemeX->ExpandDayButton_HitTest(pt, rcIconsMax1, FALSE))
						{
							XTP_CALENDAR_HITTESTINFO emptyData;
							*pHitTest = emptyData;
							//pHitTest->uHitCode = xtpCalendarHitTestDayExpandButton | xtpCalendarHitTestDayViewAllDayEvent;
							pHitTest->uHitCode = xtpCalendarHitTestAllDayExpandButton;
							return TRUE;
						}
					}
				}
			}
		}
	}

	if(!pThemeX && m_LayoutX.m_rcAllDayEvents.PtInRect(pt))
	{
		int nDaysCount = GetViewDayCount();
		for (int nDayIdx = 0; nDayIdx < nDaysCount; nDayIdx++)
		{
			CXTPCalendarDayViewDay* pViewDay = GetViewDay(nDayIdx);
			if (!pViewDay)
			{
				ASSERT(FALSE);
				continue;
			}

			int nGroupsCount = pViewDay->GetViewGroupsCount();
			for (int g = 0; g < nGroupsCount; g++)
			{
				CXTPCalendarDayViewGroup* pViewGroup = pViewDay->GetViewGroup(g);
				ASSERT(pViewGroup);
				if (!pViewGroup)
				{
					continue;
				}

				if(!pViewGroup->NeedScrollAllDayEvents())
					continue;

				if(pViewGroup->HitTestAllDayEventsScrollIcons(pt, pHitTest) == TRUE)
					return TRUE;
			}
		}
	}

	int nHit = XTP_SAFE_GET2(pThemeX, GetPrevNextEventButtonsPartX(), HitTest(&pt), 0);

	if (nHit)
	{
		XTP_CALENDAR_HITTESTINFO emptyData;
		*pHitTest = emptyData;
		pHitTest->uHitCode = nHit;
		return TRUE;
	}

//<< fix for Multiday Events >>
	XTPCalendarViewType cvt = GetCalendarControl()->GetActiveView()->GetViewType();
	if (m_bUseNewHitTestCode
		&& (cvt == xtpCalendarWorkWeekView
			|| (cvt == xtpCalendarWeekView && GetCalendarControl()->m_bMultiColumnWeekMode)
			|| (cvt == xtpCalendarFullWeekView && GetCalendarControl()->m_bMultiColumnWeekMode)))
	{
		for (int i = 0; i < GetViewDayCount(); i++)
		{
			CXTPCalendarDayViewDay* pViewDay = GetViewDay(i);
			if (pViewDay)
			{
				for (int j = 0; j < pViewDay->GetViewGroupsCount(); j++)
				{
					CXTPCalendarDayViewGroup* pViewGroup = pViewDay->GetViewGroup(j);
					if (pViewGroup)
					{
						for (int nEvent = 0; nEvent < pViewGroup->GetViewEventsCount(); nEvent++)
						{
							CXTPCalendarDayViewEvent* pViewEvent = pViewGroup->GetViewEvent(nEvent);
							if (pViewEvent)
							{
								if (pViewEvent->IsMultidayEvent())
								{
									CRect rcEvent(pViewEvent->GetEventRect());
									if (rcEvent.PtInRect(pt))
									{
										if ((pViewEvent->GetMultiDayEventFlags() & xtpCalendarMultiDaySlave) > 0)
											return FALSE;

										pt.x = rcEvent.left + 10;
										pHitTest->pt = pt;
										return TBase::HitTestEx(pt, pHitTest);
									}
								}
							}
						}
					}
				}
			}
		}
	}
//<< fix for Multiday Events >>
	return TBase::HitTestEx(pt, pHitTest);
}

BOOL CXTPCalendarDayView::IsUseCellAlignedDraggingInTimeArea() const
{
	if (GetTheme() && GetTheme()->GetDayViewPart())
		return GetTheme()->GetDayViewPart()->IsUseCellAlignedDraggingInTimeArea();

	return CXTPCalendarView::IsUseCellAlignedDraggingInTimeArea();
}

void CXTPCalendarDayView::OnStartDragging(CPoint point, XTP_CALENDAR_HITTESTINFO* pHitTest)
{
	if (!pHitTest || !m_pControl)
	{
		ASSERT(FALSE);
		return;
	}

	m_dtDraggingStartPoint = pHitTest->dt;
	m_spDraggingStartOffset_Time = 0.;

	m_nAllDayEventsCountMin_WhenDrag = max(1, GetAllDayEventsMaxCount());

	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);

	TBase::OnStartDragging(point, pHitTest);
//<<>

	if (pHitTest->pViewEvent != NULL
		&& pHitTest->pViewEvent->GetEvent()->IsAllDayEvent())
	{
		if (m_bPreventDragAllDayEventInDayView)
		{
			m_eDraggingMode = xtpCalendaDragModeUnknown;
			return;
		}
		else
		{
//TRACE(_T("OnStartDragging\n"));
			m_bUseNewHitTestCode = FALSE;
		}
	}
//<<>
	if (double(m_spDraggingStartOffset) != 0.0 && (DATE)m_dtTimeScaleMax <= 1
		&& IsUseCellAlignedDraggingInTimeArea())
	{
		// Align to cell begin time
		m_spDraggingStartOffset = pHitTest->dt - pHitTest->pViewEvent->GetEvent()->GetStartTime();
		int nTmpCell = GetCellNumber((DATE)(double)m_spDraggingStartOffset, FALSE);
		m_spDraggingStartOffset = (double)GetCellTime(nTmpCell);
	}

	m_selectedBlock.dtEnd = m_selectedBlock.dtBegin;
	m_selectedBlock.bAllDayEvent = FALSE;
	m_selectedBlock.nGroupIndex = pHitTest->nGroup;

	CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
}

BOOL CXTPCalendarDayView::OnDragging(CPoint point, XTP_CALENDAR_HITTESTINFO* pHitTest)
{
	if (!pHitTest)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (!m_ptrDraggingEventNew
		|| m_eDraggingMode == xtpCalendaDragModeEditSubject
		|| !GetCalendarControl())
	{
		return FALSE;
	}
	// Restriction for Issue #: 18965
	if (GetCalendarControl()->m_bPreventAutoAllDayMode)
	{
		if (pHitTest->uHitCode ==
			(xtpCalendarHitTestDayViewAllDayEvent | xtpCalendarHitTestDayArea))
			return FALSE;
	}

	XTP_CALENDAR_HITTESTINFO hitInfo2 = *pHitTest;
	XTP_CALENDAR_HITTESTINFO* pHitTest2 = &hitInfo2;

	BOOL bChanged = FALSE;

	XTP_CALENDAR_HITTESTINFO hitTestInfo;

	if (!HitTestEx(point, &hitTestInfo))
		return FALSE;

	//========================================================================
	BOOL bScrolled = FALSE;
	XTPCalendarDayViewScrollDirection scrollDir = GetNeededScrollDirection(point);

	if (scrollDir != xtpCalendarDayViewScrollNotNeeded)
	{
		if (scrollDir == xtpCalendarDayViewScrollUp)
		{
			bScrolled = VertEventScroll(TRUE);//scroll up
			SetMouseOutOfDragArea(FALSE);

			if (bScrolled)
			{
				if (HitTestEx(point, &hitTestInfo))
					pHitTest2->dt = hitTestInfo.dt;
			}
		}
		else if (scrollDir == xtpCalendarDayViewScrollDown)
		{
			bScrolled = VertEventScroll(FALSE);//scroll down
			SetMouseOutOfDragArea(FALSE);

			if (bScrolled)
			{
				if (HitTestEx(point, &hitTestInfo))
					pHitTest2->dt = hitTestInfo.dt;
			}
		}
//--------------------------------------------------------
		else if (scrollDir == xtpCalendarDayViewScrollLeft)
			bScrolled = HorzEventScroll(TRUE);//scroll left
		else if (scrollDir == xtpCalendarDayViewScrollRight)
			bScrolled = HorzEventScroll(FALSE);//scroll right
	}

	if (IsOutOfClientRect(point, &hitTestInfo))
	{
		SetMouseOutOfDragArea(TRUE);
		StopVertEventScroll();
		bScrolled = TRUE;
		return TRUE;
	}

	if (!bScrolled)
	{
		SetMouseOutOfDragArea(FALSE);
		StopVertEventScroll();
	}

	// check drag on TimeScale:
	if (_IsDragModeCopyMove(m_eDraggingMode)
		&& (m_pTimeScaleHeader2 && m_pTimeScaleHeader2->HitTestDateTime(&hitTestInfo) ||
		(m_pTimeScaleHeader && m_pTimeScaleHeader->HitTestDateTime(&hitTestInfo))))
	{
		COleDateTime dtNewTime = pHitTest->dt;
		dtNewTime = COleDateTime(m_dtDraggingStartPoint.GetYear(),
			m_dtDraggingStartPoint.GetMonth(), m_dtDraggingStartPoint.GetDay(),
			dtNewTime.GetHour(), dtNewTime.GetMinute(), dtNewTime.GetSecond());

		m_ptrDraggingEventNew->MoveEvent(dtNewTime);

		return TRUE;
	}

	if (!m_ptrDraggingEventOrig)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bAllDayEventArea = 0 != (hitTestInfo.uHitCode & xtpCalendarHitTestDayViewAllDayEvent);

	BOOL bAllDayEventNew_prev = m_ptrDraggingEventNew->IsAllDayEvent();
	BOOL bAllMultiDayEvent = bAllDayEventNew_prev || m_ptrDraggingEventNew->GetDurationMinutes() > 24 * 60;

	BOOL bAllMultiDayEventOrig = m_ptrDraggingEventOrig->IsAllDayEvent();
	bAllMultiDayEventOrig |= m_ptrDraggingEventOrig->GetDurationMinutes() > 24 * 60;

	bChanged = (bAllDayEventArea != bAllMultiDayEvent);// && !bAllDayEventNew_prev;

	if (bChanged)
	{
//TRACE(_T("bAllDayEventArea != bAllMultiDayEvent\n"));
		if (!bAllDayEventArea && bAllMultiDayEventOrig && _IsDragModeCopyMove(m_eDraggingMode))
		{
			//m_ptrDraggingEventNew->SetDuration(GetCellDuration());
			m_ptrDraggingEventNew->SetDuration(GetCellDuration(), FALSE);
		}
		else if (bAllDayEventArea && !bAllMultiDayEventOrig && _IsDragModeCopyMove(m_eDraggingMode))
		{
			m_ptrDraggingEventNew->SetStartTime(GetDateTimeDay(m_ptrDraggingEventOrig->GetStartTime()));
			m_ptrDraggingEventNew->SetEndTime(m_ptrDraggingEventNew->GetStartTime() + COleDateTimeSpan(1, 0, 0, 0));
		}
		else
		{
			m_ptrDraggingEventNew->SetStartTime(m_ptrDraggingEventOrig->GetStartTime());
			m_ptrDraggingEventNew->SetEndTime(m_ptrDraggingEventOrig->GetEndTime());
		}

		if (_IsDragModeCopyMove(m_eDraggingMode))
		{
			if (bAllDayEventArea == bAllMultiDayEventOrig)
			{
				m_spDraggingStartOffset = m_spDraggingStartOffset_Time;
				m_spDraggingStartOffset_Time = 0.;
			}
			else
			{
				m_spDraggingStartOffset_Time = m_spDraggingStartOffset;
				m_spDraggingStartOffset = 0.;
			}

			if (bAllDayEventArea)
			{
				if (GetCalendarControl()->DayView_IsAutoResetBusyFlag())
				{
					if (!bAllDayEventNew_prev &&
						xtpCalendarBusyStatusBusy == m_ptrDraggingEventNew->GetBusyStatus()
						&&
						!(bAllMultiDayEventOrig &&
						xtpCalendarBusyStatusBusy == m_ptrDraggingEventOrig->GetBusyStatus()))
					{
						m_ptrDraggingEventNew->SetBusyStatus(xtpCalendarBusyStatusFree);
					}
				}
			}
			else
			{
				if (GetCalendarControl()->DayView_IsAutoResetBusyFlag())
				{
					if (bAllMultiDayEvent &&
						xtpCalendarBusyStatusFree == m_ptrDraggingEventNew->GetBusyStatus()
						&&
						!(!bAllMultiDayEventOrig &&
						xtpCalendarBusyStatusFree == m_ptrDraggingEventOrig->GetBusyStatus()))
					{
						m_ptrDraggingEventNew->SetBusyStatus(xtpCalendarBusyStatusBusy);
					}
				}
			}
		}
	}

	//========================================================================
	if (m_eDraggingMode == xtpCalendaDragModeResizeEnd)
	{
		if (pHitTest2->bTimePartValid)
		{
			pHitTest2->dt += GetCellDuration();
		}
	}

	BOOL bChangedBase = TBase::OnDragging(point, pHitTest2);

	//-------------------------------------------------------------------------
	// multi-resources movement

	if (_IsDragModeCopyMove(m_eDraggingMode))
	{
		UINT uScheduleID = XTP_SAFE_GET2(pHitTest2, pViewGroup, GetScheduleID(), 0);
		if (uScheduleID != m_ptrDraggingEventNew->GetScheduleID())
		{
			if (uScheduleID != XTP_CALENDAR_UNKNOWN_SCHEDULE_ID)
			{
				m_ptrDraggingEventNew->SetScheduleID(uScheduleID);
				bChanged = TRUE;
			}
			else if (m_ptrDraggingEventNew->GetScheduleID() != m_ptrDraggingEventOrig->GetScheduleID())
			{
				m_ptrDraggingEventNew->SetScheduleID(m_ptrDraggingEventOrig->GetScheduleID());
				bChanged = TRUE;
			}
		}
	}
	//---
	CXTPCalendarData* pHitData = XTP_SAFE_GET2(pHitTest2, pViewGroup, GetDataProvider(), NULL);
	if (pHitData && pHitData != m_ptrDraggingEventNew->GetDataProvider())
	{
		CXTPCalendarEventPtr ptrHitEventNew = pHitData->CreateNewEvent();
		if (ptrHitEventNew)
		{
			if (m_ptrDraggingEventNew->CloneEventTo(ptrHitEventNew))
			{
				m_ptrDraggingEventNew = ptrHitEventNew;
				bChanged = TRUE;
			}
		}
	}

	BOOL bIsResizing = m_eDraggingMode == xtpCalendaDragModeResizeBegin ||
						m_eDraggingMode == xtpCalendaDragModeResizeEnd;

	BOOL bIsZeroLen = CXTPCalendarUtils::IsEqual(m_ptrDraggingEventNew->GetStartTime(),
						m_ptrDraggingEventNew->GetEndTime());

	if (bAllDayEventArea && (bIsResizing && !bIsZeroLen || !bIsResizing) &&
		(m_ptrDraggingEventOrig->IsAllDayEvent() || !bAllMultiDayEventOrig
		|| m_ptrDraggingEventNew->IsAllDayEvent()))
	{
		m_ptrDraggingEventNew->SetAllDayEvent(TRUE);
	}
	else
	{
		m_ptrDraggingEventNew->SetAllDayEvent(FALSE);
	}

	return bChanged || bChangedBase;
}

XTPCalendarDayViewScrollDirection CXTPCalendarDayView::GetNeededScrollDirection(CPoint pnt) const
{
	static const int nVScrollZone = 20;
	static const int nHScrollZone = 80;

	CRect rcCtrl;

	if (!m_pControl)
		return xtpCalendarDayViewScrollNotNeeded;

	m_pControl->GetWindowRect(rcCtrl);
	int nHeight = rcCtrl.Height();

	CRect rcAllDayEvents = GetAllDayEventsRectangle();
	CRect rcVisible = _GetScrollRectClient();

	int nRight = rcVisible.right;
	int nLeft = rcVisible.left;
	int nTop = rcAllDayEvents.bottom;
	int nCurPosH = 0;
	int nLimitH = rcVisible.Width();
	int nCurPos = 0;
	int nLimit = rcVisible.Height();

	SCROLLINFO si;
	if (GetScrollBarInfoV(&si))
	{
		nCurPos = si.nPos;
		nLimit = m_pControl->GetScrollLimit(SB_VERT);
	}
	SCROLLINFO siH;
	if (GetScrollBarInfoH(&siH))
	{
		nCurPosH = siH.nPos;
		nLimitH = m_pControl->GetScrollLimit(SB_HORZ);
	}
	BOOL bWidtdhOK = (nLeft <= pnt.x) && (pnt.x <= nRight);

	if (!bWidtdhOK)
		return xtpCalendarDayViewScrollNotNeeded;

	BOOL b2Up = (m_ptLBtnDownMousePos.y - pnt.y) > 2;
	BOOL b2Down = (pnt.y - m_ptLBtnDownMousePos.y) > 2;
	if (b2Up
		&& pnt.y >= nTop
		&& pnt.y <= (nTop + nVScrollZone)
		&& nCurPos > 0)
		return xtpCalendarDayViewScrollUp;
	if (b2Down
		&& (pnt.y + 3) < nHeight
		&& (pnt.y + 3) >= (nHeight - nVScrollZone)
		&& nCurPos < nLimit)
		return xtpCalendarDayViewScrollDown;

	int iCnt = 1;
	if (GetResources())
		iCnt = GetResources()->GetCount();

	if (iCnt == 1)
		return xtpCalendarDayViewScrollNotNeeded;

	BOOL b2Left = (m_ptLBtnDownMousePos.x - pnt.x) > 2;
	BOOL b2Right = (pnt.x - m_ptLBtnDownMousePos.x) > 2;

	if (b2Left
		&& pnt.x > rcVisible.left
		&& pnt.x <= (rcVisible.left + nHScrollZone)
		&& nCurPosH > 0)
		return xtpCalendarDayViewScrollLeft;
	if (b2Right
		&& (pnt.x + 3) < rcVisible.right
		&& (pnt.x + 3) >= (rcVisible.right - nHScrollZone)
		&& nCurPosH < nLimitH)
		return xtpCalendarDayViewScrollRight;

	return xtpCalendarDayViewScrollNotNeeded;
}

BOOL CXTPCalendarDayView::IsOutOfClientRect(CPoint pnt, XTP_CALENDAR_HITTESTINFO* pHitInfo)
{
	if (!m_pControl)
		return FALSE;

	CRect rcCtrl;
	m_pControl->GetWindowRect(rcCtrl);

	if (m_pTimeScaleHeader2 && m_pTimeScaleHeader2->HitTestDateTime(pHitInfo))
		return FALSE;

	if (m_pTimeScaleHeader && m_pTimeScaleHeader->HitTestDateTime(pHitInfo))
		return FALSE;

	//bottom
	if (pnt.y + 3 >= rcCtrl.Height())
		return TRUE;
	//top
	CRect rcAllDayEvents = GetAllDayEventsRectangle();
	if (pnt.y <= rcAllDayEvents.top)
		return TRUE;

	//left
	if (pnt.x < rcAllDayEvents.left)
		return TRUE;

	//right
	if (pnt.x > rcAllDayEvents.right)
		return TRUE;

	if (pHitInfo->uHitCode & xtpCalendarHitTestDayViewTimeScale)
		return TRUE;

	return FALSE;
}

void CXTPCalendarDayView::StopVertEventScroll()
{
	if (m_dwScrollingEventTimerID)
	{
		KillTimer(m_dwScrollingEventTimerID);
		m_dwScrollingEventTimerID = 0;
	}
}

void CXTPCalendarDayView::StopHorzEventScroll()
{
	if (m_dwScrollingEventTimerHorID)
	{
		KillTimer(m_dwScrollingEventTimerHorID);
		m_dwScrollingEventTimerHorID = 0;
	}
}

void CXTPCalendarDayView::SetMouseOutOfDragArea(BOOL bOutOfArea)
{
	BOOL bChanged = m_bMouseOutOfDragArea != bOutOfArea;
	m_bMouseOutOfDragArea = bOutOfArea;

	BOOL bNeedUpd = FALSE;

	if (m_bMouseOutOfDragArea)
	{
		if (m_ptrDraggingEventNew)
			m_ptrDraggingEventNew->MoveEvent(m_dtDraggingStartPoint);

		if (m_pControl && m_pControl->m_mouseMode != xtpCalendarMouseEventDraggingOut)
		{
			m_pControl->m_mouseMode = xtpCalendarMouseEventDraggingOut;
			bNeedUpd = TRUE;
		}
	}

	if (bChanged && !m_bMouseOutOfDragArea)
	{
		XTPCalendarMouseMode mouseMode = xtpCalendarMouseNothing;
		switch (m_eDraggingMode)
		{
			case xtpCalendaDragModeUnknown:
			case xtpCalendaDragModeEditSubject:
				mouseMode = xtpCalendarMouseNothing;
				break;
			case xtpCalendaDragModeCopy:
				mouseMode = xtpCalendarMouseEventDragCopy;
				break;
			case xtpCalendaDragModeMove:
				mouseMode = xtpCalendarMouseEventDragMove;
				break;
			case xtpCalendaDragModeResizeBegin:
			case xtpCalendaDragModeResizeEnd:
				mouseMode = xtpCalendarMouseEventResizingV;
				break;
		}

		XTP_SAFE_SET1(m_pControl, m_mouseMode, mouseMode);
		bNeedUpd = TRUE;
	}

	if (bNeedUpd && m_pControl)
		m_pControl->UpdateMouseCursor();
}

BOOL CXTPCalendarDayView::VertEventScroll(BOOL bUp)
{
	if (!m_pControl)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	SCROLLINFO si;
	if (!GetScrollBarInfoV(&si))
		return FALSE;

	int nCurPos = si.nPos;
	int nCurPos_raw = si.nPos;
	int nLimit = m_pControl->GetScrollLimit(SB_VERT);

	if (bUp && nCurPos > 0)
	{
		//scroll up
		nCurPos = max(nCurPos - 1, 0);
		nCurPos_raw--;
		m_bScrollingEventUp = TRUE;
	}
	else if (!bUp && nCurPos < nLimit)
	{
		//scroll down
		nCurPos = min(nCurPos + 1, nLimit);
		nCurPos_raw++;
		m_bScrollingEventUp = FALSE;
	}
	else
	{
		//limit is reached
		StopVertEventScroll();
		return FALSE;//not handled
	}

	ScrollV(nCurPos, nCurPos_raw);
	m_pControl->SetScrollPos(SB_VERT, nCurPos, FALSE);
	m_pControl->AdjustScrollBar();

	if (!m_dwScrollingEventTimerID)
		m_dwScrollingEventTimerID = SetTimer(XTP_SCROLL_TIMER_RESOLUTION_MS);

	return TRUE;
}

BOOL CXTPCalendarDayView::HorzEventScroll(BOOL bLeft)
{
	if (!m_pControl)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	SCROLLINFO si;
	if (!GetScrollBarInfoH(&si))
		return FALSE;

	BOOL bFast = TRUE;
	int nCurPos = si.nPos;
	int nCurPos_raw = si.nPos;
	int nPage = si.nPage;
	int nLimit = m_pControl->GetScrollLimit(SB_HORZ);

	if (bLeft && nCurPos > 0)
	{
		//scroll left
		if (bFast)
		{
			nCurPos -= nPage;
			nCurPos = max(nCurPos, 0);
			nCurPos_raw -= nPage;
		}
		else
		{
			nCurPos = max(nCurPos - 1, 0);
			nCurPos_raw--;
		}
		m_bScrollingEventLeft = TRUE;
	}
	else if (!bLeft && nCurPos < nLimit)
	{
		//scroll right
		if (bFast)
		{
			nCurPos += nPage;
			nCurPos = min(nCurPos, nLimit);
			nCurPos_raw += nPage;
		}
		else
		{
			nCurPos = min(nCurPos + 1, nLimit);
			nCurPos_raw++;
		}
		m_bScrollingEventLeft = FALSE;
	}
	else
	{
		//limit is reached
		return FALSE;//not handled
	}
	ScrollH(nCurPos, nCurPos_raw);
	m_pControl->SetScrollPos(SB_HORZ, nCurPos, FALSE);
	m_pControl->AdjustScrollBar();

	return TRUE;
}

void CXTPCalendarDayView::OnActivateView(BOOL bActivate, CXTPCalendarView* pActivateView,
										CXTPCalendarView* pInactiveView)
{
	TBase::OnActivateView(bActivate, pActivateView, pInactiveView);

	if (!bActivate)
	{
		if (m_dwRedrawNowLineTimerID)
		{
			KillTimer(m_dwRedrawNowLineTimerID);
			m_dwRedrawNowLineTimerID = 0;
		}
		StopVertEventScroll();
	}

	if (bActivate && m_dwRedrawNowLineTimerID == 0)
		m_dwRedrawNowLineTimerID = SetTimer(XTP_REDRAW_TIMER_RESOLUTION_MS);
}

BOOL CXTPCalendarDayView::OnTimer(UINT_PTR uTimerID)
{
	if (uTimerID == m_dwScrollingEventTimerID)
	{
		OnMouseMove(MK_LBUTTON, m_ptLastMousePos);
		return TRUE;
	}
	else if (uTimerID == m_dwRedrawNowLineTimerID)
	{
		COleDateTime dtNow = CXTPCalendarUtils::GetCurrentTime();

		if (dtNow.GetMinute() != m_dtLastRedrawTime.GetMinute())
			CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);

		return TRUE;
	}

	return TBase::OnTimer(uTimerID);
}


BOOL CXTPCalendarDayView::OnEndDragging(CPoint point, XTP_CALENDAR_HITTESTINFO* pHitInfo)
{
//TRACE(_T("OnEndDragging\n"));
	m_bUseNewHitTestCode = TRUE;

	if (!m_pControl || !pHitInfo)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CRect rcCtrl;
	m_pControl->GetWindowRect(rcCtrl);
	BOOL bReturnBack = point.y + 3 >= rcCtrl.Height();

	if (m_dwScrollingEventTimerID)
	{
		if (bReturnBack)
		{
			pHitInfo->dt = m_dtDraggingStartPoint;
			OnDragging(point, pHitInfo);
		}
		else
		{
			OnDragging(point, pHitInfo);
		}
	}

	StopVertEventScroll();

	m_dtDraggingStartPoint = COleDateTime(0, 0, 0, 0, 0, 0);

	if (_IsDragModeCopyMove(m_eDraggingMode) && m_ptrDraggingEventNew && m_ptrDraggingEventOrig)
	{
		if (!m_ptrDraggingEventNew->IsEqualStartEnd(m_ptrDraggingEventOrig))
		{
			COleDateTime dtS = m_ptrDraggingEventNew->GetStartTime();
			COleDateTime dtE = m_ptrDraggingEventNew->GetEndTime();
			COleDateTime dtS0 = GetDateTimeTime(dtS);
			COleDateTime dtE0 = GetDateTimeTime(dtE);

			m_selectedBlock.dtBegin = CXTPCalendarUtils::UpdateTime(dtS, GetCellTime(GetCellNumber(dtS0, FALSE)));
			m_selectedBlock.dtEnd = GetCellTime(GetCellNumber(dtE0, dtS0 == dtE0 ? FALSE : TRUE)) + GetCellDuration();
			m_selectedBlock.dtEnd = CXTPCalendarUtils::UpdateTime(dtE, m_selectedBlock.dtEnd);

			if (m_ptrDraggingEventNew->IsAllDayEvent() ||
					m_ptrDraggingEventNew->GetDurationMinutes() > 24 * 60)
			{
				m_selectedBlock.bAllDayEvent = TRUE;
			}
			m_selectedBlock.nGroupIndex = pHitInfo->nGroup;
			CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
		}
	}

	m_nAllDayEventsCountMin_WhenDrag = 0;

	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateScrollBar);

	return TBase::OnEndDragging(point, pHitInfo);
}

BOOL CXTPCalendarDayView::OnLButtonUp(UINT nFlags, CPoint point)
{
	return TBase::OnLButtonUp(nFlags, point);
}

BOOL CXTPCalendarDayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSelectionChangedContext selChanged(this);

	TBase::OnLButtonDown(nFlags, point);

	if (m_eDraggingMode != xtpCalendaDragModeUnknown)
		return FALSE;

	XTP_CALENDAR_HITTESTINFO hitTestInfo;

	m_bUseNewHitTestCode = FALSE;

	if (HitTestEx(point, &hitTestInfo))
	{
		m_bUseNewHitTestCode = TRUE;


		BOOL bExpandButton = (hitTestInfo.uHitCode & xtpCalendarHitTestDayExpandButton) != 0;

		if (bExpandButton)
		{
			return TRUE;
		}

		if (hitTestInfo.dt.GetStatus() != COleDateTime::valid)
			return FALSE;

		ASSERT(hitTestInfo.dt.GetStatus() == COleDateTime::valid);

		BOOL bEvent = (hitTestInfo.uHitCode & xtpCalendarHitTestEvent_Mask) != 0;

		if (bEvent)
		{
			CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);

			m_selectedBlock.dtEnd = m_selectedBlock.dtBegin = hitTestInfo.dt;
			m_dtSelectionStart = hitTestInfo.dt;
			m_selectedBlock.bAllDayEvent = FALSE;
			m_selectedBlock.nGroupIndex = hitTestInfo.nGroup;

			CSelectionChangedContext selChanged1(this, xtpCalendarSelectionDays);
		}
		else
		{
			BOOL bFixBegin = (nFlags & MK_SHIFT);
			BOOL bAllDay = 0 != (hitTestInfo.uHitCode & xtpCalendarHitTestDayViewAllDayEvent);
			COleDateTime dtNewEnd = m_selectedBlock.dtEnd;

			if (hitTestInfo.uHitCode & xtpCalendarHitTestDayViewTimeScale)
			{
				dtNewEnd = CXTPCalendarUtils::UpdateTime(dtNewEnd, hitTestInfo.dt + GetCellDuration());
				hitTestInfo.nGroup = max(0, m_selectedBlock.nGroupIndex);
			}
			else if (hitTestInfo.uHitCode & xtpCalendarHitTestDayViewCell)
			{
				dtNewEnd = hitTestInfo.dt + GetCellDuration();
			}
			else if (bAllDay)
			{
				dtNewEnd = hitTestInfo.dt + COleDateTimeSpan(1, 0, 0, 0);
			}
			else
			{
				return TRUE;
			}

			ProcessCellSelection(dtNewEnd, bFixBegin, bAllDay, hitTestInfo.nGroup);
		}
		return TRUE;
	}
	return FALSE;
}

void CXTPCalendarDayView::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedrawIfNeed);
	CSelectionChangedContext selChanged(this);

	m_ptLastMousePos = point;

	TBase::OnMouseMove(nFlags, point);

	if ((m_eDraggingMode != xtpCalendaDragModeUnknown)
		|| ((nFlags & MK_LBUTTON) == 0)
		|| !m_bStartedClickInside)
		return;
	// process selection mode

	BOOL bScrolled = FALSE;
	XTPCalendarDayViewScrollDirection direction = GetNeededScrollDirection(point);
	if (direction == xtpCalendarDayViewScrollLeft)
	{
		bScrolled = HorzEventScroll(TRUE);//scroll left
		return;
	}
	else if (direction == xtpCalendarDayViewScrollRight)
	{
		bScrolled = HorzEventScroll(FALSE);
		return;
	}
//----------------------------------------------------------------------------------
	XTP_CALENDAR_HITTESTINFO hitTestInfo;
	bScrolled = FALSE;
	if (HitTestEx(point, &hitTestInfo))
	{
		COleDateTime dtEnd = m_selectedBlock.dtEnd;
		BOOL bAllDayEvent = m_selectedBlock.bAllDayEvent;

		if (direction != xtpCalendarDayViewScrollNotNeeded &&
			hitTestInfo.dt.GetStatus() == COleDateTime::valid)
		{
			if (direction == xtpCalendarDayViewScrollUp)
			{
				bScrolled = VertEventScroll(TRUE);//scroll up
				hitTestInfo.dt -= GetCellDuration();
			}
			else if (direction == xtpCalendarDayViewScrollDown)
			{
				bScrolled = VertEventScroll(FALSE);
				hitTestInfo.dt += GetCellDuration();
			}
		}
		if (!bScrolled)
			StopVertEventScroll();

		// selection processing

		if (hitTestInfo.uHitCode & xtpCalendarHitTestDayViewTimeScale)
		{
			m_selectedBlock.bAllDayEvent = FALSE;
			if (bAllDayEvent)
				m_selectedBlock.dtBegin = m_dtSelectionStart;

			m_selectedBlock.dtEnd = CXTPCalendarUtils::UpdateTime(m_selectedBlock.dtEnd, hitTestInfo.dt);
			if (m_selectedBlock.dtEnd >= m_selectedBlock.dtBegin)
				m_selectedBlock.dtEnd += GetCellDuration();
		}
		else if (hitTestInfo.uHitCode & xtpCalendarHitTestDayViewCell)
		{
			m_selectedBlock.bAllDayEvent = FALSE;
			if (bAllDayEvent)
				m_selectedBlock.dtBegin = m_dtSelectionStart;

			m_selectedBlock.dtEnd = hitTestInfo.dt;
			if (m_selectedBlock.dtEnd >= m_selectedBlock.dtBegin)
				m_selectedBlock.dtEnd += GetCellDuration();
		}
		else if (hitTestInfo.uHitCode & xtpCalendarHitTestDayViewAllDayEvent)
		{
			if (!bAllDayEvent)
			{
				m_dtSelectionStart = m_selectedBlock.dtBegin;
				m_selectedBlock.dtBegin = GetDateTimeDay(m_selectedBlock.dtBegin);
			}
			m_selectedBlock.dtEnd = hitTestInfo.dt;
			if (m_selectedBlock.dtEnd >= m_selectedBlock.dtBegin)
				m_selectedBlock.dtEnd += COleDateTimeSpan(1, 0, 0, 0);

			m_selectedBlock.bAllDayEvent = TRUE;
		}

		if (m_selectedBlock.dtEnd != dtEnd || m_selectedBlock.bAllDayEvent != bAllDayEvent)
		{
			CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateRedraw);
			CSelectionChangedContext selChanged1(this, xtpCalendarSelectionDays);
		}
	}
}

BOOL CXTPCalendarDayView::GetSelection(COleDateTime* pBegin, COleDateTime* pEnd,
								BOOL* pbAllDayEvent, int* pnGroupIndex,
								COleDateTimeSpan* pspSelectionResolution) const
{
	if (!m_selectedBlock.IsValid())
		return FALSE;

	COleDateTime dtBegin = m_selectedBlock.dtBegin;
	COleDateTime dtEnd = m_selectedBlock.dtEnd;

	BOOL bAllDayEvent = m_selectedBlock.bAllDayEvent;

	if (pbAllDayEvent)
		*pbAllDayEvent = bAllDayEvent;

	if (bAllDayEvent)
	{
		if (dtBegin > dtEnd)
		{
			COleDateTime dt = dtEnd;
			dtEnd = dtBegin + COleDateTimeSpan(1, 0, 0, 0);
			dtBegin = dt;
		}

		dtBegin = CXTPCalendarUtils::ResetTime(dtBegin);
		if (!CXTPCalendarUtils::IsZeroTime(dtEnd))
			dtEnd += COleDateTimeSpan(1, 0, 0, 0);

		dtEnd = CXTPCalendarUtils::ResetTime(dtEnd);

	}
	else
	{
		if (dtBegin >= dtEnd)
		{
			COleDateTime dt = dtEnd;
			dtEnd = dtBegin + GetCellDuration();
			dtBegin = dt;
		}

		bAllDayEvent = CXTPCalendarUtils::IsZeroTime(dtBegin) && CXTPCalendarUtils::IsZeroTime(dtEnd);
	}

	if (pBegin)
		*pBegin = dtBegin;

	if (pEnd)
		*pEnd = dtEnd;

	if (pnGroupIndex)
		*pnGroupIndex = m_selectedBlock.nGroupIndex;

	if (pspSelectionResolution)
	{
		if (bAllDayEvent)
			*pspSelectionResolution = COleDateTimeSpan(1, 0, 0, 0);
		else
			*pspSelectionResolution = GetCellDuration();
	}

	return TRUE;
}

void CXTPCalendarDayView::ShowDay(const COleDateTime& date, BOOL bSelect)
{
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl);
	ClearDays();
	AddDay(date);

	if (bSelect)
	{
		CXTPCalendarViewDay* pDay = m_arDays.GetCount() > 0 ? m_arDays.GetAt(0) : NULL;
		if (pDay)
			SelectDay(pDay);
	}
}

void CXTPCalendarDayView::AddDay(const COleDateTime& date)
{
	CXTPCalendarControl::CViewChangedContext viewChanged(this);

	_AddDay(date);
}

void CXTPCalendarDayView::_AddDay(const COleDateTime& date)
{
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl);
	CXTPCalendarDayViewDay* pDay = new CXTPCalendarDayViewDay(this, date);
	if (pDay)
	{
		m_arDays.Add(pDay);
		m_pControl->m_bShowLinks = m_bShowLinks;
		m_pControl->m_bHideLinkContainer = m_bHideLinkContainer;
		pDay->Populate(date);
	}
}

void CXTPCalendarDayView::ShowDays(const COleDateTime& dtBegin, const COleDateTime& dtEnd)
{
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl);

	CXTPCalendarControl::CViewChangedContext viewChanged(this);

	ClearDays();

	COleDateTimeSpan spDay(1, 0, 0, 0);
	COleDateTimeSpan spDaysRange = GetDateTimeDay(dtEnd) - GetDateTimeDay(dtBegin);

	int nDays = CXTPCalendarUtils::GetTotalDays(spDaysRange) + 1;
	COleDateTime dtDay = GetDateTimeDay(dtBegin);

	nDays = max(1, nDays); // to be sure that at leas one day will be added
	for (int i = 0; i < nDays; i++)
	{
		_AddDay(dtDay);
		dtDay += spDay;
	}
}

void CXTPCalendarDayView::ShowWorkingDays(const COleDateTime& dtDay)
{
	if (!m_pControl)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarControl::CUpdateContext updateContext(m_pControl);
	CXTPCalendarControl::CViewChangedContext viewChanged(this);

	COleDateTime dtWeekDay(dtDay);

	int nFirstWeekDay = m_pControl->GetFirstDayOfWeek();
	// adjust beginning of the week iteration period to the FirstDayOfWeek
	while (dtWeekDay.GetDayOfWeek() != nFirstWeekDay)
	{
		dtWeekDay -= COleDateTimeSpan(1);
	}

	// add all working days during the week to the collection
	ClearDays();
	for (int nWeekDay = 0; nWeekDay < 7; nWeekDay++)
	{
		int nWWMask = m_pControl->GetWorkWeekMask();
		int nDayOfWeek = dtWeekDay.GetDayOfWeek();
		int nDayMask = CXTPCalendarUtils::GetDayOfWeekMask(nDayOfWeek);
		if (nWWMask & nDayMask)
		{
			_AddDay(dtWeekDay);
		}
		dtWeekDay += COleDateTimeSpan(1);
	}

	if (GetViewDayCount() == 0)
	{
		_AddDay(GetDateTimeDay(COleDateTime::GetCurrentTime()));
	}
}

COleDateTimeSpan CXTPCalendarDayView::GetScaleInterval() const
{
	int nSI_min = XTP_SAFE_GET1(GetCalendarOptions(), nDayView_ScaleInterval, 30);
	return COleDateTimeSpan(0, 0, nSI_min, 0);
}

void CXTPCalendarDayView::SetScaleInterval(const COleDateTimeSpan spScaleInterval)
{
	BOOL bValidSVal = CXTPCalendarUtils::GetTotalMinutes(spScaleInterval) >= 1;
	ASSERT(bValidSVal);

	if (m_pTimeScaleHeader && bValidSVal)
	{
		int nOldScaleInterval = XTP_SAFE_GET1(GetCalendarOptions(), nDayView_ScaleInterval, 30);
		int nScaleInterval = (int)CXTPCalendarUtils::GetTotalMinutes(spScaleInterval);

		if (nScaleInterval != nOldScaleInterval)
		{
			XTP_SAFE_SET1(GetCalendarOptions(), nDayView_ScaleInterval, nScaleInterval);

			CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateAll);

			SendNotification(XTP_NC_CALENDAROPTIONSWASCHANGED, xtpCalendarDayView, 0);
			SendNotification(XTP_NC_CALENDARVIEWTIMESCALEWASCHANGED, 0, 0);
		}
	}
}

COleDateTime CXTPCalendarDayView::GetScaleMinTime() const
{
	return m_dtTimeScaleMin;
}

void CXTPCalendarDayView::SetScaleMinTime(const COleDateTime dtNewScaleMinTime)
{
	if (dtNewScaleMinTime.GetStatus() == COleDateTime::valid)
	{
		m_dtTimeScaleMin = CXTPCalendarUtils::ResetDate(dtNewScaleMinTime);

		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);
	}
}

COleDateTime CXTPCalendarDayView::GetScaleMaxTime() const
{
	return m_dtTimeScaleMax;
}

void CXTPCalendarDayView::SetScaleMaxTime(const COleDateTime dtNewScaleMaxTime)
{
	if (dtNewScaleMaxTime.GetStatus() == COleDateTime::valid)
	{
		m_dtTimeScaleMax = CXTPCalendarUtils::ResetDate(dtNewScaleMaxTime);

		if ((double)dtNewScaleMaxTime > 1 && (double)dtNewScaleMaxTime < 2) // Special Case for 6:00 - 5:59 of second day.
			m_dtTimeScaleMax += COleDateTimeSpan(1, 0, 0, 0);

		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);
	}
	//else
	//{
	//  CXTPCalendarUtils::SetTime_235959(m_dtTimeScaleMax);
	//
	//  CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);
	//}
}

//void CXTPCalendarDayView::SetScaleMaxTime(const COleDateTime dtNewScaleMaxTime)
//{
//  if (dtNewScaleMaxTime.GetStatus() == COleDateTime::valid)
//  {
//      m_dtTimeScaleMax = dtNewScaleMaxTime;
//      if (m_dtTimeScaleMax.m_dt != 1.0)
//          m_dtTimeScaleMax = CXTPCalendarUtils::ResetDate(m_dtTimeScaleMax);
//      CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);
//  }
//}

void CXTPCalendarDayView::SetScaleText(LPCTSTR strText)
{
	XTP_SAFE_CALL1(m_pTimeScaleHeader, SetCaption(strText));

	XTP_SAFE_SET1(GetCalendarOptions(), strDayView_ScaleLabel, strText);

	SendNotification(XTP_NC_CALENDAROPTIONSWASCHANGED, xtpCalendarDayView, 0);

	SendNotification(XTP_NC_CALENDARVIEWTIMESCALEWASCHANGED, 0, 0);
}
CString CXTPCalendarDayView::GetScaleText() const
{
	return XTP_SAFE_GET2(m_pControl, GetCalendarOptions(), strDayView_ScaleLabel, _T(""));
}

void CXTPCalendarDayView::SetScale2Text(LPCTSTR strText)
{
	XTP_SAFE_CALL1(m_pTimeScaleHeader2, SetCaption(strText));

	XTP_SAFE_SET1(GetCalendarOptions(), strDayView_Scale2Label, strText);

	SendNotification(XTP_NC_CALENDAROPTIONSWASCHANGED, xtpCalendarDayView, 0);
	SendNotification(XTP_NC_CALENDARVIEWTIMESCALEWASCHANGED, 0, 0);
}

CString CXTPCalendarDayView::GetScale2Text() const
{
	return XTP_SAFE_GET2(m_pControl, GetCalendarOptions(), strDayView_Scale2Label, _T(""));
}

void CXTPCalendarDayView::SetScale2TimeZone(const TIME_ZONE_INFORMATION* pTzInfo)
{
	if (!pTzInfo || !m_pControl || !m_pControl->GetCalendarOptions())
	{
		ASSERT(FALSE);
		return;
	}

	m_pControl->GetCalendarOptions()->tziDayView_Scale2TimeZone = *pTzInfo;

	AdjustScale2TimeZone();

	SendNotification(XTP_NC_CALENDAROPTIONSWASCHANGED, xtpCalendarDayView, 0);
	SendNotification(XTP_NC_CALENDARVIEWTIMESCALEWASCHANGED, 0, 0);
}

void CXTPCalendarDayView::AdjustScale2TimeZone()
{
	TIME_ZONE_INFORMATION tziCurrent;
	::ZeroMemory(&tziCurrent, sizeof(tziCurrent));

	if (::GetTimeZoneInformation(&tziCurrent) == TIME_ZONE_ID_INVALID)
	{
		ASSERT(FALSE);
		return;
	}
	TIME_ZONE_INFORMATION tziScale2 = GetScale2TimeZone();

	SYSTEMTIME systm00, systmScale1, systmScale2;
	ZeroMemory(&systm00, sizeof(systm00));
	ZeroMemory(&systmScale1, sizeof(systmScale1));
	ZeroMemory(&systmScale2, sizeof(systmScale2));

	::GetLocalTime(&systm00);
	systm00.wHour = systm00.wMinute = systm00.wSecond = 0;

	if (!CXTPCalendarUtils::SystemTimeToTzSpecificLocalTime(&tziCurrent,
												&systm00, &systmScale1))
	{
		ASSERT(FALSE);
		return;
	}
	if (!CXTPCalendarUtils::SystemTimeToTzSpecificLocalTime(&tziScale2,
												&systm00, &systmScale2))
	{
		ASSERT(FALSE);
		return;
	}

	COleDateTime dtScale1(systmScale1), dtScale2(systmScale2);
	COleDateTimeSpan spScale2Offset = dtScale2 - dtScale1;
	ASSERT((CXTPCalendarUtils::GetTotalSeconds(spScale2Offset) % 60) == 0);

	int nTimeShift2_min = CXTPCalendarUtils::GetTotalMinutes(spScale2Offset);

	XTP_SAFE_CALL1(m_pTimeScaleHeader2, SetTimeshift(nTimeShift2_min));
}

const TIME_ZONE_INFORMATION& CXTPCalendarDayView::GetScale2TimeZone() const
{
	static TIME_ZONE_INFORMATION s_tzEmty = {0};

	return XTP_SAFE_GET2(m_pControl, GetCalendarOptions(), tziDayView_Scale2TimeZone, s_tzEmty);
}


void CXTPCalendarDayView::ShowScale2(BOOL bShow)
{
	if (!m_pTimeScaleHeader2 || !m_pControl)
	{
		ASSERT(FALSE);
		return;
	}

	m_pTimeScaleHeader2->SetVisible(bShow);

	m_pControl->GetCalendarOptions()->bDayView_Scale2Visible = bShow;
	m_pControl->AdjustLayout();

	SendNotification(XTP_NC_CALENDAROPTIONSWASCHANGED, xtpCalendarDayView, 0);
	SendNotification(XTP_NC_CALENDARVIEWTIMESCALEWASCHANGED, 0, 0);
}
BOOL CXTPCalendarDayView::IsScale2Visible() const
{
	return XTP_SAFE_GET2(m_pControl, GetCalendarOptions(), bDayView_Scale2Visible, FALSE);
}

COleDateTime CXTPCalendarDayView::GetViewDayDate(int nIndex) const
{
	CXTPCalendarDayViewDay* pViewDay = GetViewDay(nIndex);
	ASSERT(pViewDay);
	return pViewDay ? pViewDay->m_dtDate :
				GetDateTimeDay(CXTPCalendarUtils::GetCurrentTime());
}

void CXTPCalendarDayView::_ScrollDays(int nScrollDaysCount, BOOL bPrev)
{
	ASSERT(nScrollDaysCount >= 1 && nScrollDaysCount <= 14);
	nScrollDaysCount = min(14, max(1, nScrollDaysCount));

	int nCount = GetViewDayCount();
	if (!nCount)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarControl::CUpdateContext updateContext(m_pControl);
	CXTPCalendarControl::CViewChangedContext viewChanged(this);

	COleDateTimeSpan spDay(bPrev ? -1 : 1, 0, 0, 0);
	COleDateTime dtNewDay = GetViewDayDate(bPrev ? 0 : nCount-1);

	for (int i = 0; i < nScrollDaysCount; i++)
	{
		dtNewDay += spDay;

		CXTPCalendarDayViewDay* pDay = new CXTPCalendarDayViewDay(this, dtNewDay);
		if (pDay)
		{
			// when a day scrolls, the tooltip should be hidden
			// moreover without this check there was a crash when Calendar.Unicode.ocx is built on VC6
			// and vb calendar sample tried scrolling days (with keyboard) and tooltip was active
			// possible reason of the crash was calling HideToolTip from the smart pointer destructor (invoked by RemoveAt)
			// when a pointer on the call stack became invalid

			if (m_pControl)
				m_pControl->HideToolTip();

			m_arDays.RemoveAt(bPrev ? nCount-1 : 0);
			m_arDays.InsertAt(bPrev ? 0 : nCount-1, pDay);

			pDay->Populate(dtNewDay);
		}
	}
}

void CXTPCalendarDayView::ScrollDaysToPrev(int nScrollDaysCount)
{
	_ScrollDays(nScrollDaysCount, TRUE);
}

void CXTPCalendarDayView::ScrollDaysToNext(int nScrollDaysCount)
{
	_ScrollDays(nScrollDaysCount, FALSE);
}

BOOL CXTPCalendarDayView::IsExpandDown() const
{
	int nDays = GetViewDayCount();
	for (int nDay = 0; nDay < nDays; nDay++)
	{
		CXTPCalendarDayViewDay* pDay = GetViewDay(nDay);
		ASSERT(pDay);
		if (!pDay)
			continue;
		int nGroupsCount = pDay->GetViewGroupsCount();
		for (int i = 0; i < nGroupsCount; i++)
		{
			if (pDay->GetViewGroup(i) && pDay->GetViewGroup(i)->IsExpandDown())
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CXTPCalendarDayView::IsExpandUp() const
{
	int nDays = GetViewDayCount();
	for (int nDay = 0; nDay < nDays; nDay++)
	{
		CXTPCalendarDayViewDay* pDay = GetViewDay(nDay);
		ASSERT(pDay);
		if (!pDay)
			continue;
		int nGroupsCount = pDay->GetViewGroupsCount();
		for (int i = 0; i < nGroupsCount; i++)
		{
			if (pDay->GetViewGroup(i) && pDay->GetViewGroup(i)->IsExpandUp())
				return TRUE;
		}
	}
	return FALSE;
}

CXTPCalendarDayViewDay* CXTPCalendarDayView::GetDay(COleDateTime dtDay)
{
	dtDay = GetDateTimeDay(dtDay);

	int nDays = GetViewDayCount();
	for (int i = 0; i < nDays; i++)
	{
		COleDateTime dtDay_I = GetViewDayDate(i);

		if (CXTPCalendarUtils::IsEqual(dtDay, dtDay_I))
		{
			return GetViewDay(i);
		}
	}
	return NULL;
}

void CXTPCalendarDayView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CSelectionChangedContext selChanged(this);

	TBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_LEFT
		|| nChar == VK_RIGHT
		|| nChar == VK_UP
		|| nChar == VK_DOWN
		|| nChar == VK_PRIOR
		|| nChar == VK_NEXT
		|| nChar == VK_HOME
		|| nChar == VK_END)
	{
		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);

		UnselectAllEvents();

		BOOL bVKShift = (GetKeyState(VK_SHIFT) < 0);
		BOOL bVKControl = (GetKeyState(VK_CONTROL) < 0);
		COleDateTimeSpan spCell = GetCellDuration();
		COleDateTimeSpan spDay(1, 0, 0, 0);
		COleDateTime dtNewSelEnd = m_selectedBlock.dtEnd;

		BOOL bEndIsEnd = m_selectedBlock.dtEnd > m_selectedBlock.dtBegin || !bVKShift;
		int nSelCell = GetCellNumber(GetDateTimeTime(m_selectedBlock.dtEnd -
			COleDateTimeSpan(0, 0, 0, bEndIsEnd ? 1 : 0)), bEndIsEnd);

		CXTPDrawHelpers::KeyToLayout(m_pControl, nChar);

		int nDVCount = GetViewDayCount();
		COleDateTime dtFirstDay = GetViewDayDate(0);
		COleDateTime dtLastDay = GetViewDayDate(nDVCount-1);
		int iStP = RecalcMinColumnWidth();
		int iGrCnt = GetTotalGroupsCount();
		int iResCnt = 0;
		if (GetResources())
		{
			iResCnt = GetResources()->GetCount();
			if (iResCnt > 0)
				iResCnt = iGrCnt / iResCnt;
//TRACE(_T("iGrCnt=%d iResCnt=%d\n"), iGrCnt, iResCnt);
		}

		SCROLLINFO si;
		int nScrollStep = 1;
		if (GetScrollBarInfoH(&si, &nScrollStep))
		{
			int nCurPos = si.nPos;
			int nLimit = m_pControl->GetScrollLimit(SB_HORZ);

			if (bVKControl && iStP > 0 && iResCnt > 1) //control horizontal scrolling for multi-schedules case
			{
				if (nChar == VK_LEFT)
				{
					if (bVKShift)
						nCurPos = max(nCurPos - iResCnt * iStP, 0);
					else
						nCurPos = max(nCurPos - iStP, 0);
				}
				else if (nChar == VK_RIGHT)
				{
					if (bVKShift)
						nCurPos = min(nCurPos + iResCnt * iStP, nLimit);
					else
						nCurPos = min(nCurPos + iStP, nLimit);
				}
				else if (nChar == VK_PRIOR)
				{
					if (bVKShift)
						nCurPos = max(nCurPos - 2 * (int)si.nPage, 0);
					else
						nCurPos = max(nCurPos - (int)si.nPage, 0);
				}
				else if (nChar == VK_NEXT)
				{
					if (bVKShift)
						nCurPos = min(nCurPos + 2 * (int)si.nPage, nLimit);
					else
						nCurPos = min(nCurPos + (int)si.nPage, nLimit);
				}
				else if (nChar == VK_HOME)
				{
					nCurPos = 0;
				}
				else if (nChar == VK_END)
				{
					nCurPos = nLimit;
				}

				int nCurPos_raw = nCurPos;
				ScrollH(nCurPos, nCurPos_raw);

				m_pControl->SetScrollPos(SB_HORZ, nCurPos, TRUE);
				m_pControl->AdjustScrollBar();

				return;
			}

			COleDateTime dtNSel = GetDateTimeDay(dtNewSelEnd);
			if (nChar == VK_LEFT)
			{
				BOOL bCanHScrollGroupsLeft = GetIsCanHScrollGroupsLeft(dtNewSelEnd, bVKShift);
				if (!m_bScrollH_Disabled || dtNSel > dtFirstDay || bCanHScrollGroupsLeft)
				{
					if (!bVKShift && m_selectedBlock.nGroupIndex > 0)
					{
						m_selectedBlock.nGroupIndex--;
					}
					else if (!bVKShift && m_selectedBlock.nGroupIndex == 0)
					{
						dtNewSelEnd -= spDay;

						CXTPCalendarDayViewDay* pDay = GetDay(dtNewSelEnd);
						if (!bVKShift && pDay)
						{
							if (m_selectedBlock.bAllDayEvent && iResCnt > 1)
								m_selectedBlock.nGroupIndex = pDay->GetViewGroupsCount() - 1;
						}
					}

					if (m_selectedBlock.bAllDayEvent)
					{
						nCurPos = max(nCurPos - iStP, 0);
						ScrollH(nCurPos, nCurPos);
						m_pControl->SetScrollPos(SB_HORZ, nCurPos, TRUE);
						m_pControl->AdjustScrollBar();
					}
				}
			}
			else if (nChar == VK_RIGHT)
			{
				BOOL bCanHScrollGroupsRight = GetIsCanHScrollGroupsRight(dtNewSelEnd, bVKShift);

				if (!m_bScrollH_Disabled || dtNSel < dtLastDay || bCanHScrollGroupsRight)
				{
					CXTPCalendarDayViewDay* pDay = GetDay(dtNewSelEnd);
					if (!bVKShift && pDay)
					{
						if (m_selectedBlock.nGroupIndex < pDay->GetViewGroupsCount() - 1)
						{
							m_selectedBlock.nGroupIndex++;
						}
						else
						{
							dtNewSelEnd += spDay;
							m_selectedBlock.nGroupIndex = 0;
						}
					}
					else //outside the range
					{
						dtNewSelEnd += spDay;
						m_selectedBlock.nGroupIndex = 0;
					}
//TRACE(_T("nGroupIndex=%d %s nCurPos=%d\n"), m_selectedBlock.nGroupIndex, dtNewSelEnd.Format(_T("%d %m %Y")), nCurPos);
					if (m_selectedBlock.bAllDayEvent)
					{
						nCurPos = min(nCurPos + iStP, nLimit);
						ScrollH(nCurPos, nCurPos);
						m_pControl->SetScrollPos(SB_HORZ, nCurPos, TRUE);
						m_pControl->AdjustScrollBar();
					}
				}
			}
			else if (nChar == VK_UP)
			{
				BOOL bRange = !m_bScrollV_Disabled || nSelCell > m_LayoutX.m_nTopRow;

				if (nSelCell > 0 && bRange)
				{
					dtNewSelEnd = CXTPCalendarUtils::UpdateTime(dtNewSelEnd, GetCellTime(nSelCell));
				}
				if (m_selectedBlock.bAllDayEvent)
				{
					m_selectedBlock.bAllDayEvent = FALSE;
					dtNewSelEnd -= spDay;
				}
			}
			else if (nChar == VK_DOWN)
			{
				BOOL bRange = !m_bScrollV_Disabled || nSelCell < m_LayoutX.m_nTopRow + m_LayoutX.m_nVisibleRowCount - 1;
				if (nSelCell + 1 < GetRowCount() && bRange)
				{
					dtNewSelEnd = CXTPCalendarUtils::UpdateTime(dtNewSelEnd, GetCellTime(nSelCell+1)) + spCell;
					if (nSelCell == GetRowCount()-2)
					{
						dtNewSelEnd -= COleDateTimeSpan(0, 0, 0, 1);
					}
				}
			}
			else if (nChar == VK_PRIOR)
			{
				if (!m_bScrollV_Disabled)
				{
					nSelCell = max(1, nSelCell - m_LayoutX.m_nVisibleRowCount + 1);
					m_LayoutX.m_nTopRow = max(0, m_LayoutX.m_nTopRow - m_LayoutX.m_nVisibleRowCount);
					dtNewSelEnd = CXTPCalendarUtils::UpdateTime(dtNewSelEnd, GetCellTime(nSelCell));

					CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateScrollBar);
				}
			}
			else if (nChar == VK_NEXT)
			{
				if (!m_bScrollV_Disabled)
				{
					nSelCell = min(max(0, GetRowCount()-1), nSelCell + m_LayoutX.m_nVisibleRowCount);
					m_LayoutX.m_nTopRow += m_LayoutX.m_nVisibleRowCount;
					dtNewSelEnd = CXTPCalendarUtils::UpdateTime(dtNewSelEnd, GetCellTime(nSelCell)) + spCell;
					if (nSelCell == GetRowCount()-1)
					{
						dtNewSelEnd -= COleDateTimeSpan(0, 0, 0, 1);
					}

					CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateScrollBar);
				}
			}
			ProcessCellSelection(dtNewSelEnd, bVKShift, m_selectedBlock.bAllDayEvent, m_selectedBlock.nGroupIndex);
		}
		else
		{
			if (nChar == VK_LEFT)
			{
				BOOL bCanHScrollGroupsLeft = GetIsCanHScrollGroupsLeft(dtNewSelEnd, bVKShift);

				if (!m_bScrollH_Disabled || GetDateTimeDay(dtNewSelEnd) > dtFirstDay || bCanHScrollGroupsLeft)
				{
					if (bCanHScrollGroupsLeft)
					{
						m_selectedBlock.nGroupIndex--;
					}
					else
					{
						dtNewSelEnd -= spDay;

						CXTPCalendarDayViewDay* pDay = GetDay(dtNewSelEnd);
						if (!bVKShift && pDay)
						{
							m_selectedBlock.nGroupIndex = pDay->GetViewGroupsCount() - 1;
						}
					}
				}
			}
			else if (nChar == VK_RIGHT)
			{
				BOOL bCanHScrollGroupsRight = GetIsCanHScrollGroupsRight(dtNewSelEnd, bVKShift);

				if (!m_bScrollH_Disabled || GetDateTimeDay(dtNewSelEnd) < dtLastDay || bCanHScrollGroupsRight)
				{
					if (bCanHScrollGroupsRight)
					{
						m_selectedBlock.nGroupIndex++;
					}
					else
					{
						dtNewSelEnd += spDay;

						m_selectedBlock.nGroupIndex = 0;
					}
				}
			}
			else if (nChar == VK_UP)
			{
				BOOL bRange = !m_bScrollV_Disabled || nSelCell > m_LayoutX.m_nTopRow;

				if (nSelCell > 0 && bRange)
				{
					dtNewSelEnd = CXTPCalendarUtils::UpdateTime(dtNewSelEnd, GetCellTime(nSelCell));
				}
				if (m_selectedBlock.bAllDayEvent)
				{
					m_selectedBlock.bAllDayEvent = FALSE;
					dtNewSelEnd -= spDay;
				}
			}
			else if (nChar == VK_DOWN)
			{
				BOOL bRange = !m_bScrollV_Disabled || nSelCell < m_LayoutX.m_nTopRow + m_LayoutX.m_nVisibleRowCount - 1;
				if (nSelCell + 1 < GetRowCount() && bRange)
				{
					dtNewSelEnd = CXTPCalendarUtils::UpdateTime(dtNewSelEnd, GetCellTime(nSelCell+1)) + spCell;
					if (nSelCell == GetRowCount()-2)
					{
						dtNewSelEnd -= COleDateTimeSpan(0, 0, 0, 1);
					}
				}
			}
			else if (nChar == VK_PRIOR)
			{
				if (!m_bScrollV_Disabled)
				{
					nSelCell = max(1, nSelCell - m_LayoutX.m_nVisibleRowCount + 1);
					m_LayoutX.m_nTopRow = max(0, m_LayoutX.m_nTopRow - m_LayoutX.m_nVisibleRowCount);
					dtNewSelEnd = CXTPCalendarUtils::UpdateTime(dtNewSelEnd, GetCellTime(nSelCell));

					CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateScrollBar);
				}
			}
			else if (nChar == VK_NEXT)
			{
				if (!m_bScrollV_Disabled)
				{
					nSelCell = min(max(0, GetRowCount()-1), nSelCell + m_LayoutX.m_nVisibleRowCount);
					m_LayoutX.m_nTopRow += m_LayoutX.m_nVisibleRowCount;
					dtNewSelEnd = CXTPCalendarUtils::UpdateTime(dtNewSelEnd, GetCellTime(nSelCell)) + spCell;
					if (nSelCell == GetRowCount()-1)
					{
						dtNewSelEnd -= COleDateTimeSpan(0, 0, 0, 1);
					}

					CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateScrollBar);
				}
			}

			ProcessCellSelection(dtNewSelEnd, bVKShift, m_selectedBlock.bAllDayEvent, m_selectedBlock.nGroupIndex);
		}
	}
}

BOOL CXTPCalendarDayView::GetIsCanHScrollGroupsLeft(COleDateTime dtNewSelEnd, BOOL bVKShift)
{
	BOOL bCanHScrollGroupsLeft = FALSE;

	CXTPCalendarDayViewDay* pDay = GetDay(dtNewSelEnd);
	if(!bVKShift && m_selectedBlock.nGroupIndex > 0 && pDay->GetViewGroupsCount() > 1)
		bCanHScrollGroupsLeft = TRUE;

	return bCanHScrollGroupsLeft;
}

BOOL CXTPCalendarDayView::GetIsCanHScrollGroupsRight(COleDateTime dtNewSelEnd, BOOL bVKShift)
{
	BOOL bCanHScrollGroupsRight = FALSE;

	CXTPCalendarDayViewDay* pDay = GetDay(dtNewSelEnd);
	if (!bVKShift && pDay && m_selectedBlock.nGroupIndex < pDay->GetViewGroupsCount() - 1 && pDay->GetViewGroupsCount() > 1)
		bCanHScrollGroupsRight = TRUE;

	return bCanHScrollGroupsRight;
}

COleDateTime CXTPCalendarDayView::GetDateTimeDay(const COleDateTime& dt) const
{
	if ((DATE)m_dtTimeScaleMax <= 1)
		return CXTPCalendarUtils::ResetTime(dt);

	return CXTPCalendarUtils::ResetTime(dt - m_dtTimeScaleMin);
}

COleDateTime CXTPCalendarDayView::GetDateTimeTime(const COleDateTime& dt) const
{
	if ((DATE)m_dtTimeScaleMax <= 1)
		return CXTPCalendarUtils::ResetDate(dt);


	return CXTPCalendarUtils::ResetDate((DATE)dt - m_dtTimeScaleMin) + m_dtTimeScaleMin;
}

void CXTPCalendarDayView::ProcessCellSelection(COleDateTime dtNewSelEnd,
				BOOL bFixSelBegin, BOOL bAllDayEventSel, int nGroupIndex)
{
	int nCount = GetViewDayCount();
	if (!nCount)
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);

	COleDateTimeSpan spDay(1, 0, 0, 0);
	COleDateTimeSpan spCell = GetCellDuration();
	COleDateTimeSpan spSelStep = bAllDayEventSel ? spDay : spCell;

	COleDateTime dtFirstDay = GetViewDayDate(0);

	COleDateTime dtSelBegin_prev = m_selectedBlock.dtBegin;
	COleDateTime dtSelEnd_prev = m_selectedBlock.dtEnd;

	if (CXTPCalendarUtils::IsEqual(m_selectedBlock.dtEnd, m_selectedBlock.dtBegin))
	{
		int nSelRow = GetTopRow() + min(2, GetVisibleRowCount());
		m_selectedBlock.dtBegin = CXTPCalendarUtils::UpdateTime(dtFirstDay, GetCellTime(nSelRow));
		m_dtSelectionStart = m_selectedBlock.dtBegin;

		m_selectedBlock.dtEnd = m_selectedBlock.dtBegin + spCell;
	}

	m_selectedBlock.dtEnd = dtNewSelEnd;

	if (!bFixSelBegin)
	{
		m_selectedBlock.dtBegin = m_selectedBlock.dtEnd - spSelStep;
		m_dtSelectionStart = m_selectedBlock.dtBegin;
	}
	else if (CXTPCalendarUtils::IsEqual(m_selectedBlock.dtEnd, m_selectedBlock.dtBegin)
			|| (!bAllDayEventSel
				&& m_selectedBlock.dtEnd < m_selectedBlock.dtBegin
				&& dtSelEnd_prev > dtSelBegin_prev))
	{
		m_selectedBlock.dtEnd -= spSelStep;
	}

	m_selectedBlock.bAllDayEvent = bAllDayEventSel;
	m_selectedBlock.nGroupIndex = nGroupIndex;

	//===========================================================================
	int nDVCount = GetViewDayCount();
	COleDateTime dtLastDay = GetViewDayDate(nDVCount-1);
	COleDateTime& rdtSel1 = m_selectedBlock.dtEnd < m_selectedBlock.dtBegin ?
							m_selectedBlock.dtEnd : m_selectedBlock.dtBegin;
	COleDateTime& rdtSel2 = m_selectedBlock.dtEnd > m_selectedBlock.dtBegin ?
							m_selectedBlock.dtEnd : m_selectedBlock.dtBegin;

	COleDateTime dtSelDay1 = bAllDayEventSel ? rdtSel1 : GetDateTimeDay(rdtSel1);
	COleDateTime dtSelDay2 = GetDateTimeDay(rdtSel2 - COleDateTimeSpan(0, 0, 0, 1));

	//---------------------------------------------------------------------------
	if (!m_bScrollH_Disabled)
	{
		if (dtSelDay1 < dtFirstDay)
			ScrollDaysToPrev();
		else if (dtSelDay2 > dtLastDay)
			ScrollDaysToNext();
	}

	//---------------------------------------------------------------------------
	// update variables after scrolling
	nDVCount = GetViewDayCount();
	dtFirstDay = GetViewDayDate(0);
	dtLastDay = GetViewDayDate(nDVCount-1);

	if (dtSelDay1 < dtFirstDay || (!bFixSelBegin && dtSelDay2 >= dtLastDay))
	{
		if (bFixSelBegin)
		{
			rdtSel1 = CXTPCalendarUtils::UpdateTime(dtFirstDay, rdtSel1);
			rdtSel2 = rdtSel1 + spSelStep;
		}
	}
	//---------------------------------------------------------------------------
	if (dtSelDay2 > dtLastDay || (!bFixSelBegin && dtSelDay1 <= dtFirstDay))
		rdtSel2 = rdtSel1 + spSelStep;

	//---------------------------------------------------------------------------
	BOOL bEndIsEnd = m_selectedBlock.dtEnd > m_selectedBlock.dtBegin;
	int nSelEndCell = GetCellNumber(GetDateTimeTime(m_selectedBlock.dtEnd -
		COleDateTimeSpan(0, 0, 0, bEndIsEnd ? 1 : 0)), bEndIsEnd);

	if (nSelEndCell < m_LayoutX.m_nTopRow && !bAllDayEventSel)
	{
		if (!m_bScrollV_Disabled)
		{
			m_LayoutX.m_nTopRow = min(max(0, nSelEndCell), max(0, GetRowCount()-1));

			CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateScrollBar);
		}
	}
	else if (nSelEndCell >= m_LayoutX.m_nTopRow + m_LayoutX.m_nVisibleRowCount && !bAllDayEventSel)
	{
		if (!m_bScrollV_Disabled)
		{
			m_LayoutX.m_nTopRow = nSelEndCell - m_LayoutX.m_nVisibleRowCount + 1;
			m_LayoutX.m_nTopRow = min(max(0, m_LayoutX.m_nTopRow), max(0, GetRowCount() - 1));

			CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateScrollBar);
		}
	}

	if (!bAllDayEventSel || m_ptLBtnUpMousePos.x == m_ptLBtnDownMousePos.x)
	{
		if (_EnsureVisibleSelectionH())
		{
			GetCalendarControl()->AdjustLayout();
			CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateScrollBar);
		}
	}

	CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
}

BOOL CXTPCalendarDayView::_EnsureVisibleSelectionH()
{
	if (!m_selectedBlock.IsValid())
		return FALSE;

	CXTPCalendarViewDay* pVDay = _GetViewDay(m_selectedBlock.bAllDayEvent ? m_selectedBlock.dtEnd : GetDateTimeDay(m_selectedBlock.dtEnd));
	ASSERT(pVDay);

	if (!pVDay)
		return FALSE;

	CRect rcGroup = pVDay->GetDayRect();
	{
		CXTPCalendarViewGroup* pVGroup = pVDay->GetViewGroup_(m_selectedBlock.nGroupIndex);
		ASSERT(pVGroup);
		if (pVGroup)
			rcGroup = pVGroup->GetRect();
	}

	CRect rcVisible = _GetScrollRectClient();

	int nNewScrollPos = m_nScrollOffsetX;

	if (rcGroup.left < rcVisible.left)
	{
		nNewScrollPos -= rcVisible.left - rcGroup.left;
		if (nNewScrollPos <= 2) nNewScrollPos = 0;
	}
	else if (rcGroup.right > rcVisible.right)
	{
		nNewScrollPos += rcGroup.right - rcVisible.right + 1;
	}

	if (nNewScrollPos != m_nScrollOffsetX)
	{
		ScrollH(nNewScrollPos, nNewScrollPos);
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPCalendarDayView::EnsureVisibleH(CXTPCalendarViewGroup* pViewGroup)
{
	ASSERT(pViewGroup);
	if (!pViewGroup)
		return FALSE;

	SCROLLINFO si;
	if (!GetScrollBarInfoH(&si))
		return FALSE;

	CRect rcVisible = _GetScrollRectClient();
	CRect rcGroup = pViewGroup->GetRect();

	int nNewScrollPos = m_nScrollOffsetX;

	if (rcGroup.left < rcVisible.left)
	{
		nNewScrollPos -= rcVisible.left - rcGroup.left;
	}
	else if (rcGroup.right > rcVisible.right)
	{
		nNewScrollPos += rcGroup.right - rcVisible.right + 1;
	}

	if (nNewScrollPos != m_nScrollOffsetX)
	{
		ScrollH(nNewScrollPos, nNewScrollPos);

		GetCalendarControl()->AdjustLayout();
		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateScrollBar);

		return TRUE;
	}

	return FALSE;
}

BOOL CXTPCalendarDayView::_EnsureVisibleH(CXTPCalendarViewEvent* pViewEvent)
{
	ASSERT(pViewEvent);

	CXTPCalendarViewGroup* pVGroup = pViewEvent ? pViewEvent->GetViewGroup_() : NULL;
	ASSERT(pVGroup);

	if (!pVGroup)
		return FALSE;

	SCROLLINFO si;
	if (!GetScrollBarInfoH(&si))
		return FALSE;

	CRect rcVisible = _GetScrollRectClient();

	CRect rcObj = pViewEvent->GetEventRect();
	CRect rcGroup = pVGroup->GetRect();

	if (pViewEvent->IsMultidayEvent()
		&& (pViewEvent->GetMultiDayEventFlags() & xtpCalendarMultiDayMaster))
	{
		rcObj.left = max(rcObj.left, rcVisible.left);
		rcObj.right = min(rcObj.right, rcVisible.right);

		rcGroup.right = rcGroup.left + rcVisible.Width();
	}

	int nNewScrollPos = m_nScrollOffsetX;

	if (rcObj.left < rcVisible.left)
		nNewScrollPos -= rcVisible.left - rcGroup.left;
	else if (rcObj.right > rcVisible.right)
		nNewScrollPos += rcGroup.right - rcVisible.right + 1;

	if (nNewScrollPos != m_nScrollOffsetX)
	{
		ScrollH(nNewScrollPos, nNewScrollPos);
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPCalendarDayView::_EnsureVisibleV(CXTPCalendarViewEvent* pViewEvent)
{
	if (!pViewEvent || !pViewEvent->GetEvent() || !GetCalendarControl() ||
		!XTP_SAFE_GET2(pViewEvent, GetViewGroup_(), GetViewDay_(), NULL) )
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//---------------------------------------------------------------------------
	if (pViewEvent->IsMultidayEvent())
	{
		int nEventIndex = (pViewEvent->m_rcEvent.top - m_LayoutX.m_rcAllDayEvents.top) / m_LayoutX.m_nAllDayEventHeight + m_LayoutX.m_nTopAllDayRow;

		if (nEventIndex >= m_LayoutX.m_nTopAllDayRow + m_LayoutX.m_nVisibleAllDayCount)
		{
			m_LayoutX.m_nTopAllDayRow = nEventIndex - m_LayoutX.m_nVisibleAllDayCount + 1;
			return TRUE;
		}
		else if (nEventIndex < m_LayoutX.m_nTopAllDayRow)
		{
			m_LayoutX.m_nTopAllDayRow = nEventIndex;
			return TRUE;
		}

		return FALSE;
	}

	//---------------------------------------------------------------------------
	COleDateTime dtDay = XTP_SAFE_GET3(pViewEvent, GetViewGroup_(), GetViewDay_(), GetDayDate(), (DATE)0);
	dtDay = GetDateTimeDay(dtDay);

	COleDateTime dtStart = pViewEvent->GetEvent()->GetStartTime();
	COleDateTime dtEnd = pViewEvent->GetEvent()->GetEndTime();

	COleDateTime stSTime = GetDateTimeTime(dtStart);
	COleDateTime stETime = GetDateTimeTime(dtEnd);

	if (CXTPCalendarUtils::IsZeroTime(stETime))
		stETime = CXTPCalendarUtils::SetTime_235959(stETime);

	int nSCell = 0;
	int nECell = max(0, GetRowCount() - 1);

	if (GetDateTimeDay(dtStart) == dtDay)
		nSCell =  GetCellNumber(stSTime, FALSE);

	if (GetDateTimeDay(dtEnd) == dtDay)
		nECell = GetCellNumber(stETime, TRUE);

	if (nSCell < m_LayoutX.m_nTopRow)
	{
		m_LayoutX.m_nTopRow = min(max(0, nSCell-1), max(0, GetRowCount()-1));

		return TRUE;
	}
	else if (nECell >= m_LayoutX.m_nTopRow + m_LayoutX.m_nVisibleRowCount)
	{
		int nTopRowPrev = m_LayoutX.m_nTopRow;

		m_LayoutX.m_nTopRow = nECell - m_LayoutX.m_nVisibleRowCount + 1;
		m_LayoutX.m_nTopRow = min(max(0, m_LayoutX.m_nTopRow), max(0, GetRowCount()-1));
		m_LayoutX.m_nTopRow = min(max(0, nSCell-1), m_LayoutX.m_nTopRow);

		if (nTopRowPrev != m_LayoutX.m_nTopRow)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTPCalendarDayView::EnsureVisible(CXTPCalendarViewEvent* pViewEvent)
{
	ASSERT(pViewEvent);
	if (!pViewEvent)
		return FALSE;

	BOOL bChanged1 = _EnsureVisibleH(pViewEvent);
	BOOL bChanged2 = _EnsureVisibleV(pViewEvent);

	if (bChanged1 || bChanged2)
	{
		GetCalendarControl()->AdjustLayout();
		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateScrollBar);
	}

	ASSERT(pViewEvent->IsVisible());

	return TRUE;
}

BOOL CXTPCalendarDayView::EnsureVisibleH(CXTPCalendarViewEvent* pViewEvent)
{
	ASSERT(pViewEvent);
	if (!pViewEvent)
		return FALSE;

	BOOL bChanged1 = _EnsureVisibleH(pViewEvent);

	if (bChanged1)
	{
		GetCalendarControl()->AdjustLayout();
		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateScrollBar);
	}

	ASSERT(pViewEvent->IsVisible());

	return TRUE;
}


CXTPCalendarViewEvent* CXTPCalendarDayView::FindEventToEditByTAB(COleDateTime dtMinStart,
								BOOL bReverse, CXTPCalendarEvent* pAfterEvent)
{
	CXTPCalendarViewEvent* pViewEvent = TBase::FindEventToEditByTAB(dtMinStart, bReverse, pAfterEvent);
	if (pViewEvent)
	{
		if (EnsureVisible(pViewEvent))
			return pViewEvent;
	}
	return NULL;
}

COleDateTime CXTPCalendarDayView::GetNextTimeEditByTAB() const
{
	COleDateTime dtSel1 = min(m_selectedBlock.dtBegin, m_selectedBlock.dtEnd);
	COleDateTime dtSel2 = max(m_selectedBlock.dtBegin, m_selectedBlock.dtEnd);
	COleDateTime dtSel1Day = GetDateTimeDay(dtSel1);
	COleDateTime dtSel2Day = GetDateTimeDay(dtSel2);

	int nDVCount = GetViewDayCount();
	COleDateTime dtFirstDay = GetViewDayDate(0);
	COleDateTime dtLastDay = GetViewDayDate(nDVCount-1);

	if (dtSel1Day >= dtFirstDay && dtSel1Day <= dtLastDay)
		return dtSel1;
	else if (dtSel2Day >= dtFirstDay && dtSel2Day <= dtLastDay)
		return dtSel2;

	return TBase::GetNextTimeEditByTAB();
}

void CXTPCalendarDayView::UpdateNextTimeEditByTAB(COleDateTime dtNext, BOOL bReverse, BOOL bReset)
{
	m_selectedBlock.dtBegin = m_selectedBlock.dtEnd = GetNextTimeEditByTAB();
	m_dtSelectionStart = m_selectedBlock.dtBegin;

	COleDateTime dtNextDay = GetDateTimeDay(dtNext);
	COleDateTime dtCurrDay = GetDateTimeDay(GetNextTimeEditByTAB());

	if (dtNextDay != dtCurrDay || bReset)
	{
		m_selectedBlock.dtBegin = m_selectedBlock.dtEnd = dtNextDay;
		m_dtSelectionStart = m_selectedBlock.dtBegin;

		COleDateTime dtFirstDay = GetViewDayDate(bReverse ? GetViewDayCount()-1 : 0);
		if (dtNextDay == dtFirstDay || bReset)
		{
			if (bReverse)
			{
				m_selectedBlock.dtBegin = m_selectedBlock.dtEnd = dtNextDay + COleDateTimeSpan(1, 0, 0, 0);
				m_selectedBlock.dtBegin = m_selectedBlock.dtEnd - GetCellDuration();
			}
			else
			{
				m_selectedBlock.dtEnd = m_selectedBlock.dtBegin + GetCellDuration();
			}
			ProcessCellSelection(m_selectedBlock.dtEnd, FALSE, FALSE, 0);
		}
	}
}

COleDateTimeSpan CXTPCalendarDayView::GetEventDurationMin() const
{
	return GetCellDuration();
}

CXTPCalendarTimeZonePtr CXTPCalendarDayView::GetCurrentTimeZoneInfo() const
{
	TIME_ZONE_INFORMATION tziCurrent;
	::ZeroMemory(&tziCurrent, sizeof(tziCurrent));

	if (::GetTimeZoneInformation(&tziCurrent) == TIME_ZONE_ID_INVALID)
	{
		ASSERT(FALSE);
		return NULL;
	}

	return CXTPCalendarTimeZone::GetTimeZoneInfo(&tziCurrent);
}

CXTPCalendarTimeZonePtr CXTPCalendarDayView::GetScale2TimeZoneInfo() const
{
	return CXTPCalendarTimeZone::GetTimeZoneInfo(&GetScale2TimeZone());
}

void CXTPCalendarDayView::ScrollToWorkDayBegin()
{
	if (!m_pControl || !m_pControl->GetCalendarOptions())
	{
		ASSERT(FALSE);
		return;
	}

	COleDateTime dtStartDate = GetViewDayDate(0);
	COleDateTime dtSelBegin = CXTPCalendarUtils::UpdateTime(dtStartDate, m_pControl->GetCalendarOptions()->dtWorkDayStartTime);
	COleDateTime dtSelEnd = dtSelBegin + GetCellDuration();

	SetSelection(dtSelBegin, dtSelEnd, FALSE);

	int nWDStartCell = GetCellNumber(m_pControl->GetCalendarOptions()->dtWorkDayStartTime, FALSE);
	_ScrollV(nWDStartCell, nWDStartCell);
}

int CXTPCalendarDayView::_GetTimeScaleWith() const
{
	if (m_bShowAllDayEventsOnly)
		return 0;

	int nWidth = XTP_SAFE_GET1(GetTimeScale(), GetWidth(), 0);
	if (IsScale2Visible())
		nWidth += XTP_SAFE_GET1(GetTimeScale(2), GetWidth(), 0);

	return nWidth;
}

CRect CXTPCalendarDayView::_GetScrollRectClient() const
{
	CXTPClientRect rcClient(GetCalendarControl());
	rcClient.left += _GetTimeScaleWith();
	rcClient.left = min(rcClient.left, rcClient.right);

	return rcClient;
}

void CXTPCalendarDayView::ScrollToColumn(long lColumn)
{
	ScrollToWorkDayBegin();

	int iPos = lColumn;
	int iStP = RecalcMinColumnWidth();
	int iResCnt = 0;
	if (GetResources())
		iResCnt = GetResources()->GetCount();

	SCROLLINFO si;
	int nScrollStep = 1;
	if (GetScrollBarInfoH(&si, &nScrollStep))
	{
		if (iStP > 0 && iResCnt > 1) //control horizontal scrolling for multi-schedules case
		{
			int nLimit = m_pControl->GetScrollLimit(SB_HORZ);
			int nCurPos = min(iStP * (iPos - 1), nLimit);

			ScrollH(nCurPos, nCurPos);

			m_pControl->SetScrollPos(SB_HORZ, nCurPos, TRUE);
			m_pControl->AdjustScrollBar();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarDayView, CXTPCalendarView)
	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "TimeScale", 1, OleGetScaleInterval, OleSetScaleInterval, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "ScaleText", 2, OleGetScaleText, SetScaleText, VT_BSTR)

	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "Scale2Visible", 4, OleIsScale2Visible, OleShowScale2, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "Scale2Text", 5, OleGetScale2Text, SetScale2Text, VT_BSTR)

	DISP_FUNCTION_ID(CXTPCalendarDayView, "ShowDays", 8, OleShowDays, VT_EMPTY, VTS_DATE VTS_DATE)
	DISP_FUNCTION_ID(CXTPCalendarDayView, "AddDay", 9, OleAddDay, VT_EMPTY, VTS_DATE)

	DISP_FUNCTION_ID(CXTPCalendarDayView, "GetCellNumber", 10, OleGetCellNumber, VT_I4, VTS_DATE)
	DISP_FUNCTION_ID(CXTPCalendarDayView, "ScrollV", 11, OleScrollV, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarDayView, "ScrollToWorkDayBegin", 12, OleScrollToWorkDayBegin, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarDayView, "EnsureVisible", 13, OleEnsureVisible, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarDayView, "EnsureVisibleGroup", 15, OleEnsureVisibleGroup, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarDayView, "ScrollToColumn", 16, OleScrollToColumn, VT_EMPTY, VTS_I4)

	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "MinColumnWidth", 14, GetMinColumnWidth, SetMinColumnWidth, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "TimeScaleMinTime", 21, OleGetTimeScaleMinTime, OleSetTimeScaleMinTime, VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "TimeScaleMaxTime", 22, OleGetTimeScaleMaxTime, OleSetTimeScaleMaxTime, VT_DATE)

	//DISP_PROPERTY_ID(CXTPCalendarDayView, "EcoMode", 23, m_bEcoMode, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "DayHeaderPeriod", 24, OleGetDayHeaderPeriod, OleSetDayHeaderPeriod, VT_I4)
	DISP_PROPERTY_ID(CXTPCalendarDayView, "DraftMode", 25, m_bDraftMode, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarDayView, "RepeatHeaderMode", 26, m_bPrintRepeatHeaderFooterMode, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarDayView, "DF_Mode", 27, m_bDF_mode, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarDayView, "EcoAllDayMode", 28, m_bEcoAllDayMode, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "PreventAutoDropAllDayMode", 29, OleGetPreventAutoDropAllDayMode, OleSetPreventAutoDropAllDayMode, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPCalendarDayView, "DayViewPrintInterval", 30, OleGetDayViewPrintInterval, OleSetDayViewPrintInterval, VT_I4)
	DISP_PROPERTY_ID(CXTPCalendarDayView, "PrintFullScale", 31, m_bPrintFullScale, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarDayView, "PreventDragAllDayEventInDayView", 32, m_bPreventDragAllDayEventInDayView, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarDayView, "ShowLinks", 33, m_bShowLinks, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarDayView, "HideLinkContainer", 34, m_bHideLinkContainer, VT_BOOL)

	DISP_PROPERTY_ID(CXTPCalendarDayView, "ShowAllDayEventsOnly", 35, m_bShowAllDayEventsOnly, VT_BOOL)

//  DISP_FUNCTION_ID(CXTPCalendarDayView, "GetScale2TimeZone", 104, OleGetScale2TimeZone, VT_DISPATCH, VTS_NONE)
//  DISP_FUNCTION_ID(CXTPCalendarDayView, "SetScale2TimeZone", 105, OleSetScale2TimeZone, VT_EMPTY, VTS_DISPATCH)
//  DISP_FUNCTION_ID(CXTPCalendarDayView, "EnumAllTimeZones", 106, OleEnumAllTimeZones, VT_DISPATCH, VTS_NONE)
//  DISP_FUNCTION_ID(CXTPCalendarDayView, "GetCurrentTimeZone", 107, OleGetCurrentTimeZone, VT_DISPATCH, VTS_NONE)
END_DISPATCH_MAP()

// {2FED5027-B4A5-492f-ADA0-56E1B0772A53}
static const GUID IID_ICalendarDayView =
{ 0x2fed5027, 0xb4a5, 0x492f, { 0xad, 0xa0, 0x56, 0xe1, 0xb0, 0x77, 0x2a, 0x53 } };

BEGIN_INTERFACE_MAP(CXTPCalendarDayView, CXTPCalendarView)
	INTERFACE_PART(CXTPCalendarDayView, IID_ICalendarDayView, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarDayView, IID_ICalendarDayView)


long CXTPCalendarDayView::OleGetScaleInterval()
{
	COleDateTimeSpan dt = GetScaleInterval();

	return (long)CXTPCalendarUtils::GetTotalMinutes(dt);

}

void CXTPCalendarDayView::OleSetScaleInterval(long nScalInterval)
{
	if (nScalInterval < 1)
	{
		ASSERT(FALSE);
		AfxThrowOleException(E_INVALIDARG);
	}

	COleDateTimeSpan dt(0, 0, nScalInterval, 0);
	SetScaleInterval(dt);
}

DATE CXTPCalendarDayView::OleGetTimeScaleMinTime()
{
	return GetScaleMinTime();
}

void CXTPCalendarDayView::OleSetTimeScaleMinTime(DATE dtNewScaleMinTime)
{
	SetScaleMinTime(dtNewScaleMinTime);
	GetCalendarControl()->GetCalendarOptions()->dtScaleMinTime = m_dtTimeScaleMin;
}

DATE CXTPCalendarDayView::OleGetTimeScaleMaxTime()
{
	return GetScaleMaxTime();
}

void CXTPCalendarDayView::OleSetTimeScaleMaxTime(DATE dtNewScaleMaxTime)
{
	SetScaleMaxTime(dtNewScaleMaxTime);
	GetCalendarControl()->GetCalendarOptions()->dtScaleMaxTime = m_dtTimeScaleMax;
}

BSTR CXTPCalendarDayView::OleGetScaleText()
{
	return GetScaleText().AllocSysString();
}

BSTR CXTPCalendarDayView::OleGetScale2Text()
{
	return GetScale2Text().AllocSysString();
}

BOOL CXTPCalendarDayView::OleIsScale2Visible()
{
	return IsScale2Visible() ? TRUE : FALSE;
}

void CXTPCalendarDayView::OleShowScale2(BOOL bShow)
{
	ShowScale2(bShow != FALSE);
}

void CXTPCalendarDayView::OleShowDays(DATE dtBegin, DATE dtEnd)
{
	ShowDays(dtBegin, dtEnd);
}

void CXTPCalendarDayView::OleAddDay(DATE dtDay)
{
	AddDay(dtDay);
}

long CXTPCalendarDayView::OleGetCellNumber(DATE dtTime)
{
	COleDateTime dtTmp(dtTime);
	return (long)GetCellNumber(dtTmp, FALSE);
}

void CXTPCalendarDayView::OleScrollV(long lCell)
{
	_ScrollV(lCell, lCell);
}

void CXTPCalendarDayView::OleScrollToColumn(long lColumn)
{
	ScrollToColumn(lColumn);
}

void CXTPCalendarDayView::OleScrollToWorkDayBegin()
{
	ScrollToWorkDayBegin();
}

BOOL CXTPCalendarDayView::OleEnsureVisible(LPDISPATCH pdispViewEvent)
{
	CXTPCalendarViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarViewEvent, CXTPCalendarViewEvent::FromIDispatch(pdispViewEvent));
	if (!pViewEvent)
		AfxThrowOleException(E_INVALIDARG);

	return EnsureVisible(pViewEvent);
}

BOOL CXTPCalendarDayView::OleEnsureVisibleGroup(LPDISPATCH pdispViewGroup)
{
	CXTPCalendarViewGroup* pViewGroup = DYNAMIC_DOWNCAST(CXTPCalendarViewGroup, CXTPCalendarViewGroup::FromIDispatch(pdispViewGroup));
	if (!pViewGroup)
		AfxThrowOleException(E_INVALIDARG);

	return EnsureVisibleH(pViewGroup);
}

int CXTPCalendarDayView::OleGetDayHeaderPeriod()
{
	return m_nDayHeaderPeriod;
}

void CXTPCalendarDayView::OleSetDayHeaderPeriod(int nDayHeaderPeriod)
{
	m_nDayHeaderPeriod = max(1, nDayHeaderPeriod);
}

BOOL CXTPCalendarDayView::OleGetPreventAutoDropAllDayMode()
{
	return GetCalendarControl()->m_bPreventAutoAllDayMode;
}

void CXTPCalendarDayView::OleSetPreventAutoDropAllDayMode(BOOL bSet)
{
	GetCalendarControl()->m_bPreventAutoAllDayMode = bSet;
}

int CXTPCalendarDayView::OleGetDayViewPrintInterval()
{
	return m_nDayViewPrintInterval;
}

void CXTPCalendarDayView::OleSetDayViewPrintInterval(int nDayViewPrintInterval)
{
	if (nDayViewPrintInterval == 60
		|| nDayViewPrintInterval == 45
		|| nDayViewPrintInterval == 30
		|| nDayViewPrintInterval == 15)
		m_nDayViewPrintInterval = nDayViewPrintInterval;
}

#endif
