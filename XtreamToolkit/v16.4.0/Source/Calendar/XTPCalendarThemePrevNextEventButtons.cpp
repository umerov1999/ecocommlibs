// XTPCalendarThemePrevNextEventButtons.cpp : implementation file
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

#include "Resource.h"

#include "Common/XTPColorManager.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPVC80Helpers.h"

#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarUtils.h"
#include "XTPCalendarOptions.h"
#include "XTPCalendarNotifications.h"

#include "XTPCalendarResource.h"

#include "XTPCalendarEvent.h"
#include "XTPCalendarEvents.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewDay.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarControl.h"

#include "XTPCalendarTheme.h"
#include "XTPCalendarThemePrevNextEventButton.h"
#include "XTPCalendarThemePrevNextEventButtons.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// DBG
//#define DBG_TRACE_PREVNEXTBTN  TRACE
//#define DBG_TRACE_PREVNEXTBTN_ARR  TRACE
//#define DBG_ASSERT_PREVNEXTBTN  ASSERT

#ifndef DBG_TRACE_PREVNEXTBTN
#define DBG_TRACE_PREVNEXTBTN
#endif
#ifndef DBG_TRACE_PREVNEXTBTN_ARR
#define DBG_TRACE_PREVNEXTBTN_ARR
#endif
#ifndef DBG_ASSERT_PREVNEXTBTN
#define DBG_ASSERT_PREVNEXTBTN
#endif
// DBG

CTOPrevNextEventButtons::CTOPrevNextEventButtons()
{
	m_bHide.SetStandardValue(FALSE);

	m_nRefreshDaysPerIdleStep.SetStandardValue(2);
	m_nIdleStepTime_ms.SetStandardValue(120);
	m_bUseActiveViewResources.SetStandardValue(FALSE);

	m_nRefreshTimerID = 0;
	m_ndtRefreshDatePrev = 0;
	m_ndtRefreshDateNext = 0;

	m_bScanPrev = FALSE;
	m_bScanNext = FALSE;

	m_pSink = new CXTPNotifySink();
}

CTOPrevNextEventButtons::~CTOPrevNextEventButtons()
{
	ASSERT(m_nRefreshTimerID == 0);

	m_pSink->Delete();
}

void CTOPrevNextEventButtons::SetCacheSize(int nDays)
{
	m_mapDaysState.SetDataSize(nDays);
}

void CTOPrevNextEventButtons::RefreshMetrics(BOOL bRefreshChildren)
{
	m_pSink->UnadviseAll();

	CXTPNotifyConnection* pConnection = XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetConnection(), NULL);
	if (pConnection)
	{
		m_pSink->Advise(pConnection, XTP_NC_CALENDAREVENTWASADDED,
			CreateNotfySinkClassDelegate(this, &CTOPrevNextEventButtons::OnEvent_Calendar));
		m_pSink->Advise(pConnection, XTP_NC_CALENDAREVENTWASDELETED,
			CreateNotfySinkClassDelegate(this, &CTOPrevNextEventButtons::OnEvent_Calendar));
		m_pSink->Advise(pConnection, XTP_NC_CALENDAREVENTWASCHANGED,
			CreateNotfySinkClassDelegate(this, &CTOPrevNextEventButtons::OnEvent_Calendar));

		m_pSink->Advise(pConnection, XTP_NC_CALENDARVIEWWASCHANGED,
			CreateNotfySinkClassDelegate(this, &CTOPrevNextEventButtons::OnEvent_Calendar));
	}

	TBase::RefreshMetrics(bRefreshChildren);
}

void CTOPrevNextEventButtons::AdjustLayout(CXTPCalendarView* /*pObject*/, CDC* pDC, const CRect& rcRect)
{
	if (!GetPrevEventButtonPartX() || !GetNextEventButtonPartX())
	{
		ASSERT(FALSE);
		return;
	}

	m_rcRect = rcRect;

	BOOL bIsPrevNextButtonsVisible = IsPrevNextButtonsVisible();
	if (pDC && pDC->IsPrinting())
		bIsPrevNextButtonsVisible = FALSE;

	GetPrevEventButtonPartX()->m_bVisible = bIsPrevNextButtonsVisible;
	GetNextEventButtonPartX()->m_bVisible = bIsPrevNextButtonsVisible;

	if (!bIsPrevNextButtonsVisible)
	{
		_StopRequests();
	}
	else
	{
		GetPrevEventButtonPartX()->m_bDisabled = !GetPrevEventDay();
		GetNextEventButtonPartX()->m_bDisabled = !GetNextEventDay();
	}

	if (GetPrevEventButtonPartX())
		GetPrevEventButtonPartX()->AdjustLayout(NULL, pDC, rcRect);

	if (GetNextEventButtonPartX())
		GetNextEventButtonPartX()->AdjustLayout(NULL, pDC, rcRect);
}

void CTOPrevNextEventButtons::Draw(CXTPCalendarView* /*pObject*/, CDC* pDC)
{
	if (GetPrevEventButtonPartX())
		GetPrevEventButtonPartX()->Draw(NULL, pDC);

	if (GetNextEventButtonPartX())
		GetNextEventButtonPartX()->Draw(NULL, pDC);
}

int CTOPrevNextEventButtons::HitTest(const CPoint* pPoint) const
{
	int nHit = 0;
	if (GetPrevEventButtonPartX())
		nHit = GetPrevEventButtonPartX()->HitTest(pPoint);

	if (!nHit && GetNextEventButtonPartX())
		nHit = GetNextEventButtonPartX()->HitTest(pPoint);

	return nHit;
}

void CTOPrevNextEventButtons::OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	if (GetPrevEventButtonPartX())
		GetPrevEventButtonPartX()->OnMouseMove(pObject, nFlags, point);

	if (GetNextEventButtonPartX())
		GetNextEventButtonPartX()->OnMouseMove(pObject, nFlags, point);
}

BOOL CTOPrevNextEventButtons::OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	BOOL b1 = FALSE, b2 = FALSE;

	if (GetPrevEventButtonPartX())
		b1 = GetPrevEventButtonPartX()->OnLButtonDown(pObject, nFlags, point, this);

	if (GetNextEventButtonPartX())
		b2 = GetNextEventButtonPartX()->OnLButtonDown(pObject, nFlags, point, this);

	return b1 || b2;
}

void CTOPrevNextEventButtons::OnStatusChanged()
{
	if (!GetTheme())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarControl::CUpdateContext updateContext(GetTheme()->GetCalendarControl(), xtpCalendarUpdateRedraw);

	AdjustLayout(NULL, NULL, m_rcRect);

	DBG_TRACE_PREVNEXTBTN(_T("PrevNextEventController::OnStatusChanged() \n"));
}


BOOL CTOPrevNextEventButtons::IsPrevNextButtonsVisible()
{
	HWND hWnd = XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetSafeHwnd(), NULL);
	BOOL bEnabled = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), bEnablePrevNextEventButtons, TRUE);

	if (!hWnd || m_bHide || !bEnabled)
		return FALSE;

	return !_IsSomeEventVisible();
}

BOOL CTOPrevNextEventButtons::_IsSomeEventVisible()
{
	CXTPCalendarView* pView = XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetActiveView(), NULL);
	ASSERT(pView);
	if (!pView)
		return FALSE;

	int nDays = pView->GetViewDayCount();
	for (int nDay = 0; nDay < nDays; nDay++)
	{
		CXTPCalendarViewDay* pDay = pView->GetViewDay_(nDay);
		ASSERT(pDay);
		if (!pDay)
			continue;
		int nGroupsCount = pDay->GetViewGroupsCount();
		for (int i = 0; i < nGroupsCount; i++)
		{
			if (pDay->GetViewGroup_(i) && pDay->GetViewGroup_(i)->GetViewEventsCount())
				return TRUE;
		}
	}

	return FALSE;
}

COleDateTime CTOPrevNextEventButtons::GetVisibleDay(BOOL bFirst)
{
	CXTPCalendarView* pView = XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetActiveView(), NULL);
	ASSERT(pView);
	if (!pView)
		return (DATE)0;

	if (bFirst)
	{
		return pView->GetViewDayDate(0);
	}

	int nDays = pView->GetViewDayCount();
	return pView->GetViewDayDate(max(0, nDays - 1));
}

BOOL CTOPrevNextEventButtons::GetPrevEventDay(COleDateTime* pdtDay)
{
	if (pdtDay)
		*pdtDay = COleDateTime((DATE)0);

	long nPrevDay = 0;
	//int nMinDay = max(m_ndtMinScanDay, m_mapDaysState.GetMinDay());
	//int nMinDay = m_mapDaysState.GetMinDay();

	long nDay = (long)(double)GetVisibleDay(TRUE) - 1;
	m_mapDaysState.AdjustMiddleDay(nDay);

	for (; nDay >= m_mapDaysState.GetMinDay(); nDay--)
	{
		int nState = m_mapDaysState.GetDayState(nDay);
		if (nState >= 0)
		{
			if (nState > 0)
				nPrevDay = nDay;
			break;
		}
	}

	if (nPrevDay > 0)
	{
		if (pdtDay)
			*pdtDay = COleDateTime((DATE)nPrevDay);
		return TRUE;
	}

	if (nDay >= m_mapDaysState.GetMinDay())
	{
		m_ndtRefreshDatePrev = (long)(double)GetVisibleDay(TRUE) - 1;
		m_bScanPrev = TRUE;
		_RequestToFindEvents();
	}

	return FALSE;

}

BOOL CTOPrevNextEventButtons::GetNextEventDay(COleDateTime* pdtDay)
{
	if (pdtDay)
		*pdtDay = COleDateTime((DATE)0);

	long nNextDay = 0;
	//int nMaxDay = min(m_ndtMaxScanDay, m_mapDaysState.GetMaxDay());
	//int nMaxDay = m_mapDaysState.GetMaxDay();

	long nDay = (long)(double)GetVisibleDay(FALSE) + 1;

	m_mapDaysState.AdjustMiddleDay(nDay);

	for (; nDay <= m_mapDaysState.GetMaxDay(); nDay++)
	{
		int nState = m_mapDaysState.GetDayState(nDay);
		if (nState >= 0)
		{
			if (nState > 0)
				nNextDay = nDay;
			break;
		}
	}

	if (nNextDay > 0)
	{
		if (pdtDay)
			*pdtDay = COleDateTime((DATE)nNextDay);
		return TRUE;
	}

	if (nDay <= m_mapDaysState.GetMaxDay())
	{
		m_ndtRefreshDateNext = (long)(double)GetVisibleDay(FALSE) + 1;
		m_bScanNext = TRUE;
		_RequestToFindEvents();
	}

	return FALSE;
}

BOOL CTOPrevNextEventButtons::OnTimer(UINT_PTR uTimerID)
{
	if (m_nRefreshTimerID != (UINT)uTimerID)
		return FALSE;

	BOOL bIsEventsPrev = FALSE;
	BOOL bIsEventsNext = FALSE;
	const DWORD cdwTimeRun_ms = 10;

	DWORD dwTime0 = ::GetTickCount();
	int nRefreshedDays = 0;
	int i;

	//int nMinDay = m_mapDaysState.GetMinDay();
	//int nMaxDay = m_mapDaysState.GetMaxDay();

	if (m_bScanPrev && m_ndtRefreshDatePrev >= m_mapDaysState.GetMinDay())
	{
		DBG_TRACE_PREVNEXTBTN(_T("%d - PrevNextEventController::RefreshDays-- (from %d - \n"), m_nRefreshTimerID, m_ndtRefreshDatePrev);

		for (i = 0; m_ndtRefreshDatePrev >= m_mapDaysState.GetMinDay(); i++)
		{
			//DBG_TRACE_PREVNEXTBTN(_T("%d - PrevNextEventController::_RefreshDay(-- %d) \n"), m_nRefreshTimerID, m_ndtRefreshDatePrev);

			BOOL bRefreshed;
			bIsEventsPrev = _RefreshDay(m_ndtRefreshDatePrev, bRefreshed, FALSE);
			m_ndtRefreshDatePrev--;

			if (bRefreshed)
				nRefreshedDays++;

			if (bIsEventsPrev)
			{
				m_bScanPrev = FALSE;
				break;
			}

			DWORD dwTime1 = dwTime0;

			if (bRefreshed || (i % 500 == 0))
				dwTime1 = ::GetTickCount();

			if (nRefreshedDays >= (int)m_nRefreshDaysPerIdleStep &&
				abs((int)(dwTime1 - dwTime0)) >= cdwTimeRun_ms)
			{
				break;
			}
		}
		DBG_TRACE_PREVNEXTBTN(_T("  --- to %d) [nRefreshedDays = %d] \n"), m_ndtRefreshDatePrev, nRefreshedDays);
	}
	//else

	if (m_ndtRefreshDatePrev < m_mapDaysState.GetMinDay())
	{
		m_bScanPrev = FALSE;
		DBG_TRACE_PREVNEXTBTN(_T("%d  ## MIN reached \n"), m_nRefreshTimerID);
	}

	if (m_bScanNext && m_ndtRefreshDateNext <= m_mapDaysState.GetMaxDay())
	{
		DBG_TRACE_PREVNEXTBTN(_T("%d - PrevNextEventController::RefreshDays++ (from %d - \n"), m_nRefreshTimerID, m_ndtRefreshDateNext);

		dwTime0 = ::GetTickCount();
		nRefreshedDays = 0;

		for (i = 0; m_ndtRefreshDateNext <= m_mapDaysState.GetMaxDay(); i++)
		{
			//DBG_TRACE_PREVNEXTBTN(_T("%d - PrevNextEventController::_RefreshDay(%d ++) \n"), m_nRefreshTimerID, m_ndtRefreshDateNext);

			BOOL bRefreshed;
			bIsEventsNext = _RefreshDay(m_ndtRefreshDateNext, bRefreshed, FALSE);
			m_ndtRefreshDateNext++;

			if (bRefreshed)
				nRefreshedDays++;

			if (bIsEventsNext)
			{
				m_bScanNext = FALSE;
				break;
			}

			DWORD dwTime1 = dwTime0;

			if (bRefreshed || (i % 500 == 0))
				dwTime1 = ::GetTickCount();

			if (nRefreshedDays >= (int)m_nRefreshDaysPerIdleStep &&
				abs((int)(dwTime1 - dwTime0)) >= cdwTimeRun_ms)
			{
				break;
			}
		}

		DBG_TRACE_PREVNEXTBTN(_T("  --- to %d)  [nRefreshedDays = %d] \n"), m_ndtRefreshDateNext, nRefreshedDays);
	}
	//else
	if (m_ndtRefreshDateNext > m_mapDaysState.GetMaxDay())
	{
		m_bScanNext = FALSE;
		DBG_TRACE_PREVNEXTBTN(_T("%d  ## MAX reached \n"), m_nRefreshTimerID);
	}


	BOOL bStatusChanged = bIsEventsPrev || bIsEventsNext;

	if (!m_bScanPrev && !m_bScanNext)
	{
		DBG_TRACE_PREVNEXTBTN(_T("%d - PrevNextEventController::_StopRequests(Prev=%d, Next=%d) \n"),
			m_nRefreshTimerID, m_bScanPrev, m_bScanNext);

		_StopRequests();
		bStatusChanged = TRUE;
	}

	if (bStatusChanged)
	{
		OnStatusChanged();
	}

	return TRUE;
}

void CTOPrevNextEventButtons::OnEvent_Calendar(XTP_NOTIFY_CODE Event, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if (Event == XTP_NC_CALENDAREVENTWASADDED ||
		Event == XTP_NC_CALENDAREVENTWASDELETED ||
		Event == XTP_NC_CALENDAREVENTWASCHANGED)
	{
		CXTPCalendarEvent* pEvent = (CXTPCalendarEvent*)lParam;

		CXTPCalendarResources* parRC = _GetActiveResources();
		if (parRC && parRC->Find(pEvent))
		{
			long nStartDay = (long)(double)pEvent->GetStartTime();
			long nEndDay = (long)(double)pEvent->GetEndTime();

			m_mapDaysState.SetDaysStateSafe(nStartDay, nEndDay, 0);
		}
	}
	else if (Event == XTP_NC_CALENDARVIEWWASCHANGED)
	{
		if ((BOOL)m_bUseActiveViewResources)
			m_mapDaysState.SetDataSize(m_mapDaysState.GetDataSize());
			//m_mapDaysState.SetDaysStateSafe(-1, -1, 0);
	}
	else {
		ASSERT(FALSE);
	}
}

CXTPCalendarResources* CTOPrevNextEventButtons::_GetActiveResources()
{
	if ((BOOL)m_bUseActiveViewResources)
		return XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetActiveView(), GetResources(), NULL);
	else
		return XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetResources(), NULL);
}

BOOL CTOPrevNextEventButtons::_RefreshDay(long ndtDay, BOOL& rbRefreshed, BOOL bRefreshPermanently)
{
	int nSate = bRefreshPermanently ? 0 : m_mapDaysState.GetDayState(ndtDay);
	rbRefreshed = FALSE;

	if (nSate == 0)
	{
		// TODO:
		//  + Change to use multi-Data Providers from Resources
		//  - also PrePopulate events have to be used too ???
		//

		CXTPCalendarResources* parRC = _GetActiveResources();

		//CXTPCalendarEventsPtr ptrEvents = pData->RetrieveDayEvents((DATE)ndtDay);
		CXTPCalendarEventsPtr ptrEvents = parRC->RetrieveDayEvents((DATE)ndtDay);
		BOOL bIsEvents = ptrEvents && ptrEvents->GetCount() > 0;
		nSate = bIsEvents ? 1 : -1;

		m_mapDaysState.SetDayState(ndtDay, nSate);

		//DBG_TRACE_PREVNEXTBTN(_T("   >>> _RefreshDay(%d) DATA readed  <<< \n"), ndtDay);

		rbRefreshed = TRUE;
	}

	return nSate > 0;
}


void CTOPrevNextEventButtons::_RequestToFindEvents()
{
	if (m_nRefreshTimerID)
		return;

	m_nRefreshTimerID = SetTimer((UINT)(int)m_nIdleStepTime_ms);
	ASSERT(m_nRefreshTimerID);

	DBG_TRACE_PREVNEXTBTN(_T("\n%d *** PrevNextEventController::_RequestToFindEvents() \n"), m_nRefreshTimerID);
}

void CTOPrevNextEventButtons::_StopRequests()
{
	if (m_nRefreshTimerID)
		KillTimer(m_nRefreshTimerID);

	m_nRefreshTimerID = 0;
}

void CTOPrevNextEventButtons::OnDetachCalendar()
{
	_StopRequests();
}

#define XTP_CALENDAR_OFFICE2007_PREVNEXTDAYS (366 * 2)

CTOPrevNextEventButtons::CDaysMap::CDaysMap()
{
	m_nDaysOffset = (long)(double)CXTPCalendarUtils::GetCurrentTime();
	m_nDaysOffset -= XTP_CALENDAR_OFFICE2007_PREVNEXTDAYS;
	m_nCacheSizeMax_days = (366 * 10) * 2; // +10/-10 years;

	SetSize(XTP_CALENDAR_OFFICE2007_PREVNEXTDAYS * 2, 1024);
}

long CTOPrevNextEventButtons::CDaysMap::GetMinDay()
{
	return m_nDaysOffset;
}

long CTOPrevNextEventButtons::CDaysMap::GetMaxDay()
{
	return m_nDaysOffset + (long)GetSize() - 1;
}

int CTOPrevNextEventButtons::CDaysMap::GetDayState(long nDay)
{
	// states: (0)-unknown, (-1)-has no-events, (1)-has events.

	_GrowArrayIfNeed(nDay);

	long nDay_idx = nDay - m_nDaysOffset;
	if (nDay_idx < 0 || nDay_idx >= GetSize())
	{
		DBG_ASSERT_PREVNEXTBTN(FALSE); // data ASSERT
		return 0;
	}

	int nState = (int)GetAt(nDay_idx);
	ASSERT(nState >= -1 && nState <= 1);

	return nState;
}

void CTOPrevNextEventButtons::CDaysMap::SetDayState(long nDay, int nState)
{
	// states: (0)-unknown, (-1)-has no-events, (1)-has events.

	_GrowArrayIfNeed(nDay);

	long nDay_idx = nDay - m_nDaysOffset;
	if (nDay_idx < 0 || nDay_idx >= GetSize())
	{
		//_ChangeOffset(long nNewOffset)

		DBG_ASSERT_PREVNEXTBTN(FALSE); // data ASSERT
		return;
	}

	ASSERT(nState >= -1 && nState <= 1);
	SetAt(nDay_idx, (char)nState);
}

void CTOPrevNextEventButtons::CDaysMap::SetDaysStateSafe(long nDayStart, long nDayEnd, int nState)
{
	int nDay_idx = nDayStart - m_nDaysOffset;

	if (nDay_idx >= GetSize())
		return;

	nDay_idx = max(0, nDay_idx);

	int nCount = nDayEnd > 0 ? (nDayEnd - nDayStart + 1) : (int)GetSize();
	nCount = min(nCount, (int)GetSize() - nDay_idx);

	for (int i = 0; i < nCount; i++)
		SetAt(nDay_idx + i, (char)nState);
}

void CTOPrevNextEventButtons::CDaysMap::AdjustMiddleDay(long nDay)
{
	_GrowArrayIfNeed(nDay);

	int nSize = (int)GetSize();
	int nMiddleDay0 = (m_nDaysOffset + nSize/2);

	int nMiddleRange = max(XTP_CALENDAR_OFFICE2007_PREVNEXTDAYS / 2, nSize / 4);
	if (abs(nDay - nMiddleDay0) < nMiddleRange)
		return;

	int nShift = abs(nMiddleDay0 - nDay);
	int nCount2 = max(0, nSize - nShift);

	if (nCount2 == 0) // Shift too big
	{
		memset(GetData(), 0, nSize);
		m_nDaysOffset += nDay - nMiddleDay0;
	}
	else if (nDay < nMiddleDay0)
	{
		MEMMOVE_S(GetData() + nShift, GetData(), nCount2);
		memset(GetData(), 0, nShift);
		m_nDaysOffset -= nShift;
	}
	else
	{
		MEMMOVE_S(GetData(), GetData() + nShift, nCount2);
		memset(GetData() + nCount2, 0, nShift);
		m_nDaysOffset += nShift;
	}

	DBG_TRACE_PREVNEXTBTN_ARR(_T("CalendarThemeOffice2007::PrevNextEventController - *AdjustMiddleDay: [from-%d, to-%d]. \n"), nMiddleDay0, nDay);
}

void CTOPrevNextEventButtons::CDaysMap::_GrowArrayIfNeed(long nDay)
{
	const int cnGrowBy = 32 * 24; //24 month
	const int cnMaxArraySize = (int)m_nCacheSizeMax_days;
	long nNewOffset = 0;

	int nSize = (int)GetSize();

	long nDay_idx = nDay - m_nDaysOffset;

	int nNewSize = nDay_idx < 0 ? (nSize - nDay_idx + 1) : nDay_idx + 1;
	nNewSize = (nNewSize / cnGrowBy) * cnGrowBy + cnGrowBy;

	nNewSize = max(nNewSize, nSize + cnGrowBy);
	nNewSize = min(cnMaxArraySize, nNewSize);

	if ((nDay_idx <= 10 || nDay_idx >= nSize - 10) &&
		nNewSize < cnMaxArraySize)
	{
		ASSERT(nNewSize > nSize);

		nNewOffset = (m_nDaysOffset + nSize/2) - nNewSize/2;
		ASSERT(nNewOffset < m_nDaysOffset);

		SetSize(nNewSize);

		//int nShift = max(0, (nNewSize / 2 - nSize / 2));
		int nShift = m_nDaysOffset - nNewOffset;
		ASSERT(nShift > 0);
		nShift = max(0, nShift);
		if (nShift)
		{
			int nCount2 = max(0, min(nSize, nNewSize - nShift));
			MEMMOVE_S(GetData() + nShift, GetData(), nCount2);
			memset(GetData(), 0, min(nShift, nNewSize));
		}

		DBG_TRACE_PREVNEXTBTN_ARR(_T("CalendarThemeOffice2007::PrevNextEventController GrowArray: from-%d, to-%d. \n"), nSize, nNewSize);

		nSize = (int)GetSize();
		m_nDaysOffset = nNewOffset;
		nDay_idx = nDay - m_nDaysOffset;

		ASSERT(nDay_idx >= 0 && nDay_idx < nSize);
	}
}

int CTOPrevNextEventButtons::CDaysMap::GetDataSize()
{
	return (int)GetSize();
}

void CTOPrevNextEventButtons::CDaysMap::SetDataSize(int nDays)
{
	nDays = max(3, nDays);

	int nSize = (int)GetSize();
	int nMiddleDay = (m_nDaysOffset + nSize/2);

//  if (nDays > (int)m_nCacheSizeMax_days)
	m_nCacheSizeMax_days = nDays;

	SetSize(nDays);
	memset(GetData(), 0, nDays);

	nSize = (int)GetSize();
	m_nDaysOffset = nMiddleDay  - nSize/2;
}