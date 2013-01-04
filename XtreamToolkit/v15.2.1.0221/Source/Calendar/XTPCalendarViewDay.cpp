// XTPCalendarViewDay.cpp: implementation of the CXTPCalendarViewDay class.
//
// This file is a part of the XTREME CALENDAR MFC class library.
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

#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPColorManager.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarUtils.h"
#include "XTPCalendarOptions.h"
#include "XTPCalendarNotifications.h"
#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarEvent.h"
#include "XTPCalendarEvents.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewDay.h"
#include "XTPCalendarViewEvent.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarResource.h"
#include "XTPCalendarControl.h"

#include "XTPCalendarPaintManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarViewGroup, CXTPCalendarWMHandler)
IMPLEMENT_DYNAMIC(CXTPCalendarViewDay, CXTPCalendarWMHandler)
//////////////////////////////////////////////////////////////////////////

CXTPCalendarViewGroup::CXTPCalendarViewGroup(CXTPCalendarViewDay* pViewDay)
{
	m_pViewDay = pViewDay;

	m_pResources = new CXTPCalendarResources();

	m_Layout.m_rcGroup.SetRectEmpty();
	m_Layout.m_rcGroupHeader.SetRectEmpty();

}

CXTPCalendarViewGroup::~CXTPCalendarViewGroup()
{
	CMDTARGET_RELEASE(m_pResources);
}

void CXTPCalendarViewGroup::AddViewEvent(CXTPCalendarEvent* pEvent)
{
	if (pEvent)
	{
		for (int i = 0; i < m_arEvents.GetCount(); i++)
		{
			if (m_arEvents.GetAt(i))
			{
				if (m_arEvents.GetAt(i)->GetEvent() == pEvent)
					return;
			}
		}
		m_arEvents.Add(CreateViewEvent(pEvent));
	}
}

void CXTPCalendarViewGroup::AddResource(CXTPCalendarResource* pResource, BOOL bWithAddRef)
{
	ASSERT(pResource);

	if (m_pResources && pResource)
	{
		m_pResources->Add(pResource, bWithAddRef);
	}
}

void CXTPCalendarViewGroup::SetResources(CXTPCalendarResources* pResources)
{
	if (!m_pResources)
	{
		return;
	}
	m_pResources->RemoveAll();

	ASSERT(pResources);
	if (pResources)
	{
		m_pResources->Append(pResources);
	}
}

CXTPCalendarData* CXTPCalendarViewGroup::GetDataProvider() const
{
	if (m_pResources && m_pResources->GetCount() && m_pResources->GetAt(0))
	{
		return m_pResources->GetAt(0)->GetDataProvider();
	}
	return NULL;
}

UINT CXTPCalendarViewGroup::GetScheduleID() const
{
	UINT uScheduleID = XTP_CALENDAR_UNKNOWN_SCHEDULE_ID;
	if (XTP_SAFE_GET1(GetResources(), GetCount(), 0) &&
		XTP_SAFE_GET3(GetResources(), GetAt(0), GetSchedules(), GetSize(), 0) )
	{
		uScheduleID = XTP_SAFE_GET3(GetResources(), GetAt(0), GetSchedules(),
									GetAt(0), 0);
	}

	return uScheduleID;
}

int CXTPCalendarViewGroup::GetGroupIndex() const
{
	if (!GetViewDay_())
	{
		return -1;
	}
	for (int i = 0; i < GetViewDay_()->GetViewGroupsCount(); i++)
	{
		if (GetViewDay_()->GetViewGroup_(i) == this)
			return i;
	}

	ASSERT(FALSE);
	return -1;
}

CString CXTPCalendarViewGroup::GetCaption() const
{
	if (!m_pResources)
	{
		return _T("");
	}

	//BOOL bShowAll = TRUE;
	CString strCaption;

	int nResCount = m_pResources->GetCount();
	for (int i = 0; i < nResCount; i++)
	{
		if (m_pResources->GetAt(i) /* && m_pResources->GetAt(i)->GetSchedules()*/)
		{
			if (!strCaption.IsEmpty())
			{
				strCaption += _T(", ");
			}
			strCaption += m_pResources->GetAt(i)->GetName();
		}
	}

	WPARAM wParam = (WPARAM)this;
	LPARAM lParam = (LPARAM)(CString*)&strCaption;
	XTP_SAFE_CALL3(GetViewDay_(), GetView_(), GetCalendarControl(),
		SendNotification(XTP_NC_CALENDAR_GETGROPVIEWCAPTION, wParam, lParam));

	return strCaption;
}

CRect CXTPCalendarViewGroup::GetTooltipRect(const CPoint& ptHit, const XTP_CALENDAR_HITTESTINFO& hitInfo) const
{
	UNREFERENCED_PARAMETER(ptHit);
	UNREFERENCED_PARAMETER(hitInfo);

	CRect rcTip;
	rcTip.SetRectEmpty();
	return rcTip;
}

void CXTPCalendarViewGroup::AdjustLayout(CDC* pDC, const CRect& rcGroup)
{
	m_Layout.m_rcGroup = rcGroup;

	int nCount = GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = GetViewEvent_(i);
		ASSERT(pViewEvent);
		if (pViewEvent)
		{
			pViewEvent->AdjustLayout(pDC, rcGroup, i);
		}
	}
}

void CXTPCalendarViewGroup::AdjustLayout2(CDC* pDC, const CRect& rcGroup)
{
	CXTPCalendarTheme* pTheme = XTP_SAFE_GET2(GetViewDay_(), GetView_(), GetTheme(), NULL);
	ASSERT(pTheme);

	//XTP_SAFE_CALL4(pTheme, Get_mw_ViewPart(), GetDayPart(), GetGroupPart(), AdjustLayout(this, pDC, rcGroup));

	m_Layout.m_rcGroup = rcGroup;

	int nCount = GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pVEvent = GetViewEvent_(i);
		ASSERT(pVEvent);
		if (pVEvent)
		{
			pVEvent->AdjustLayout2(pDC, rcGroup, i);
		}
	}
}

void CXTPCalendarViewGroup::Draw(CDC* pDC)
{
	int nCount = GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = GetViewEvent_(i);
		ASSERT(pViewEvent);
		if (pViewEvent && pViewEvent->IsVisible())
		{
			pViewEvent->Draw(pDC);
		}
	}
}

CXTPCalendarWMHandler* CXTPCalendarViewGroup::GetChildHandlerAt(int nIndex) const
{
	ASSERT_KINDOF(CXTPCalendarWMHandler, GetViewEvent_(nIndex));
	return GetViewEvent_(nIndex);
}


UINT CXTPCalendarViewGroup::SetTimer(UINT uTimeOut_ms)
{
	return XTP_SAFE_GET1(GetViewDay_(), SetTimer(uTimeOut_ms), 0);
}


void CXTPCalendarViewGroup::KillTimer(UINT uTimerID)
{
	XTP_SAFE_CALL1(GetViewDay_(), KillTimer(uTimerID) );
}

void CXTPCalendarViewGroup::OnPrePopulate(CXTPCalendarEvents* pEvents)
{
	WPARAM wParam = (WPARAM)(CXTPCalendarViewGroup*)this;
	LPARAM lParam = (LPARAM)pEvents;

	XTP_SAFE_CALL3(GetViewDay_(), GetView_(), GetCalendarControl(), SendNotification(
		XTP_NC_CALENDAR_PREPOPULATE, wParam, lParam));
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPCalendarViewDay::CXTPCalendarViewDay(const CXTPCalendarView* pView)
{
	m_pView = (CXTPCalendarView*)pView;

	m_dtDate.SetStatus(COleDateTime::null);
	m_pResources = NULL;

	m_Layout.m_rcExpandSign.SetRectEmpty();
	m_Layout.m_nHotState = 0;

	m_Layout.m_rcDay.SetRectEmpty();
	m_Layout.m_rcDayHeader.SetRectEmpty();


}

CXTPCalendarViewDay::~CXTPCalendarViewDay()
{
}

int CXTPCalendarViewDay::GetViewEventsCount() const
{
	int nGroups = GetViewGroupsCount();
	ASSERT(nGroups <= 1);
	if (nGroups == 1 && GetViewGroup_(0))
	{
		return GetViewGroup_(0)->GetViewEventsCount();
	}
	return 0;
}

CXTPCalendarViewEvent* CXTPCalendarViewDay::GetViewEvent_(int nIndex) const
{
	int nGroups = GetViewGroupsCount();
	ASSERT(nGroups <= 1);
	if (nGroups == 1 && GetViewGroup_(0))
	{
		return GetViewGroup_(0)->GetViewEvent_(nIndex);
	}
	return NULL;
}

CXTPCalendarControl* CXTPCalendarViewDay::GetCalendarControl() const
{
	ASSERT(this);
	if (!this)
	{
		return NULL;
	}
	return XTP_SAFE_GET1(GetView_(), GetCalendarControl(), NULL);
}

CXTPCalendarResources* CXTPCalendarViewDay::GetResources() const
{
	ASSERT(this);
	if (m_pResources)
	{
		return m_pResources;
	}

	return  GetView_() ? GetView_()->GetResources() : NULL;
}

void CXTPCalendarViewDay::SetResources(CXTPCalendarResources* pResources)
{
	BOOL bChanged = TRUE;

	if (!pResources)
	{
		bChanged = m_pResources != NULL;
		CMDTARGET_RELEASE(m_pResources);
	}
	else
	{
		if (!m_pResources)
			m_pResources = new CXTPCalendarResources();

		if (!m_pResources)
			return;

		// copy data to allow using self as input parameter
		CXTPCalendarResources arRes;
		arRes.Append(pResources);

		m_pResources->RemoveAll();
		m_pResources->Append(&arRes);
	}

	if (bChanged && GetView_())
	{
		GetView_()->SendNotification(XTP_NC_CALENDAR_RESOURCES_WHERE_CHANGED, 2, (LPARAM)this);
	}
}

void CXTPCalendarViewDay::AdjustLayout(CDC* pDC, const CRect& rcDay)
{
	UNREFERENCED_PARAMETER(pDC);

	m_Layout.m_rcDay.CopyRect(&rcDay);
	m_Layout.m_rcDayHeader.SetRectEmpty();
	m_Layout.m_rcExpandSign.SetRect(rcDay.right, rcDay.bottom, rcDay.right, rcDay.bottom);
	m_Layout.m_nHotState = 0;
}

BOOL CXTPCalendarViewDay::UserAction_OnExpandDay(XTPCalendarExpandDayButton eButton)
{
	XTP_CALENDAR_USERACTION_PARAMS uaParams;
	::ZeroMemory(&uaParams, sizeof(uaParams));

	uaParams.m_eAction = xtpCalendarUserAction_OnExpandDay;
	uaParams.m_ExpandDayButton.nButton = eButton;
	uaParams.m_ExpandDayButton.pViewDay = this;

	BOOL bCancel = FALSE;

	if (GetCalendarControl())
		GetCalendarControl()->SendNotification(XTP_NC_CALENDAR_USERACTION, (WPARAM)&uaParams, (LPARAM)&bCancel);

	return bCancel;
}

CXTPCalendarViewEvent* CXTPCalendarViewGroup::GetViewEventByEvent_(CXTPCalendarEvent* pEvent) const
{
	if (!pEvent)
	{
		ASSERT(FALSE);
		return NULL;
	}

	int nCount = GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pView = GetViewEvent_(i);
		if (!pView)
		{
			ASSERT(FALSE);
			continue;
		}

		BOOL bIDsEqual = pEvent->IsEqualIDs(pView->GetEvent());
		BOOL bTimesEqual = pEvent->IsEqualStartEnd(pView->GetEvent());
		if (bIDsEqual && bTimesEqual)
		{
			return pView;
		}
	}

	return NULL;
}

CXTPCalendarViewEvent* CXTPCalendarViewGroup::GetViewEventByPlace_(int nEventPlace) const
{
	int nCount = GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pView = GetViewEvent_(i);
		if (!pView)
		{
			ASSERT(FALSE);
			continue;
		}
		int nPlace = pView->GetEventPlaceNumber();
		if (nPlace == nEventPlace)
		{
			return pView;
		}
	}

	return NULL;
}

void CXTPCalendarViewGroup::ExchangeEventPlace(CXTPCalendarViewEvent* pViewEvent, int nNewPlace)
{
	if (!pViewEvent)
	{
		ASSERT(FALSE);
		return;
	}

	int nCurentPlace = pViewEvent->GetEventPlaceNumber();

	if (nCurentPlace == nNewPlace)
	{
		return;
	}

	CXTPCalendarViewEvent* pViewEventNew = GetViewEventByPlace_(nNewPlace);
	if (pViewEventNew)
	{
		pViewEventNew->ChangeEventPlace(nCurentPlace);
	}

	pViewEvent->ChangeEventPlace(nNewPlace);
}


void CXTPCalendarViewGroup::Populate(COleDateTime dtDayDate)
{
	dtDayDate = CXTPCalendarUtils::ResetTime(dtDayDate);

	m_arEvents.RemoveAll();

	CXTPCalendarEventsPtr ptrEventsAr = new CXTPCalendarEvents();
	CXTPCalendarView* pView = XTP_SAFE_GET1(GetViewDay_(), GetView_(), NULL);
	if (!GetResources() || !pView || !ptrEventsAr)
		return;

	CXTPCalendarOptions* pOptions = GetCalendarControl()->GetCalendarOptions();
	BOOL bNeedNextDay = (DATE)pOptions->dtScaleMaxTime > 1;

	int nResourcesCount = GetResources()->GetCount();
	for (int nRc = 0; nRc < nResourcesCount; nRc++)
	{
		CXTPCalendarResource* pRC = GetResources()->GetAt(nRc);
		if (!pRC)
			continue;

		// to support dragging common mechanism "pView->RetrieveDayEvents" is used.
		CXTPCalendarEventsPtr ptrEvents = pView->RetrieveDayEvents(pRC, dtDayDate);
		if (ptrEvents)
		{
			// append events
			ptrEventsAr->Append(ptrEvents);
		}

		if (bNeedNextDay)
		{
			CXTPCalendarEventsPtr ptrEvents = pView->RetrieveDayEvents(pRC, dtDayDate + COleDateTimeSpan(1, 0, 0, 0));
			if (ptrEvents)
			{
				// append events
				ptrEventsAr->Append(ptrEvents);
			}
		}
	}

	// sort events by start time and some other criteria
	ptrEventsAr->Sort(CXTPCalendarEvents::CompareEvents_ForView);

	//---------------------
	CXTPCalendarEvent* pDragNew = XTP_SAFE_GET2(GetViewDay_(), GetView_(), GetDraggingEventNew(), NULL);
	CXTPCalendarEvent* pDragOrig = XTP_SAFE_GET2(GetViewDay_(), GetView_(), GetDraggingEventOrig(), NULL);

	// remove duplicated events
	int nCount = ptrEventsAr->GetCount();
	for (int nEvent = nCount - 2; nEvent >= 0; nEvent--)
	{
		CXTPCalendarEvent* pEvent0 = ptrEventsAr->GetAt(nEvent);
		CXTPCalendarEvent* pEvent1 = ptrEventsAr->GetAt(nEvent + 1);
		if (!pEvent0 || !pEvent1)
			continue;
		if (pEvent0 == pDragNew || pEvent1 == pDragNew)
			continue;

		if (pEvent0->GetDataProvider() == pEvent1->GetDataProvider() &&
			pEvent0->IsEqualIDs(pEvent1))
		{
			ASSERT(pEvent0->GetScheduleID() == pEvent1->GetScheduleID());
			ptrEventsAr->RemoveAt(nEvent + 1);
		}
	}

	// dragging support
	if (pDragNew && !pDragOrig)
	{
		int nFIdx = ptrEventsAr->Find(pDragNew);
		if (nFIdx > 0)
		{
			ptrEventsAr->InsertAt(0, pDragNew, TRUE);
			ptrEventsAr->RemoveAt(nFIdx+1);
		}
	}

	// to allow some customization before show events
	OnPrePopulate(ptrEventsAr);

	// Create events views:
	nCount = ptrEventsAr->GetCount();
	for (int nEventId = 0; nEventId < nCount; nEventId++)
	{
		CXTPCalendarEvent* pEvent = ptrEventsAr->GetAt(nEventId);
		if (!pEvent)
		{
			ASSERT(FALSE);
			continue;
		}

		if (bNeedNextDay)
		{
			if ((DATE)pEvent->GetEndTime() <= dtDayDate + pOptions->dtScaleMinTime)
				continue;

			if ((DATE)pEvent->GetStartTime() >= dtDayDate + pOptions->dtScaleMaxTime)
				continue;

			if (pEvent->IsAllDayEvent() && (DATE)pEvent->GetStartTime() > dtDayDate)
				continue;
		}

		// the same checking is in the data provider.
		// check again for dragging event.
		if (!(pEvent->GetStartTime() < dtDayDate
			&& CXTPCalendarUtils::IsEqual(pEvent->GetEndTime(), dtDayDate)))
		{
			AddViewEvent(pEvent);
		}
	}
}


BOOL CXTPCalendarViewGroup::HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	return HitTestEx(pt, pHitTest);
}

BOOL CXTPCalendarViewGroup::HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (!m_Layout.m_rcGroup.PtInRect(pt))
	{
		return FALSE;
	}

	if (m_Layout.m_rcGroupHeader.PtInRect(pt))
	{
		FillHitTestEx(pHitTest);
		pHitTest->uHitCode = xtpCalendarHitTestGroupHeader;
		return TRUE;
	}

	int nCount = GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = GetViewEvent_(i);
		ASSERT(pViewEvent);
		if (pViewEvent && pViewEvent->HitTestEx(pt, pHitTest))
		{
			ASSERT(!pHitTest->pViewEvent || pHitTest->pViewEvent == pViewEvent);
			pHitTest->nEvent = i;
			pHitTest->pViewEvent = pViewEvent;
			return TRUE;
		}
	}
	FillHitTestEx(pHitTest);
	pHitTest->uHitCode = xtpCalendarHitTestGroupArea;
	return TRUE;
}

BOOL CXTPCalendarViewDay::NoAllEventsAreVisible()
{
	if (CXTPCalendarViewDay::_NoAllEventsAreVisible())
	{
		if (GetView_() && !GetView_()->GetTheme())
		{
			CSize szBm = XTP_SAFE_GET2(GetView_(), GetPaintManager(), GetExpandSignSize(), CSize(0) );
			m_Layout.m_rcExpandSign.SetRect(m_Layout.m_rcDay.right - szBm.cx, m_Layout.m_rcDay.bottom - szBm.cy,
				m_Layout.m_rcDay.right, m_Layout.m_rcDay.bottom);
		}
		return TRUE;
	}
	else
	{
		m_Layout.m_rcExpandSign.SetRect(m_Layout.m_rcDay.right, m_Layout.m_rcDay.bottom,
			m_Layout.m_rcDay.right, m_Layout.m_rcDay.bottom);
		return FALSE;
	}
}

BOOL CXTPCalendarViewDay::_NoAllEventsAreVisible() const
{
	int nGroups = GetViewGroupsCount();
	for (int g = 0; g < nGroups; g++)
	{
		CXTPCalendarViewGroup* pGroup = GetViewGroup_(g);
		ASSERT(pGroup);
		if (!pGroup)
		{
			continue;
		}

		int nCount = pGroup->GetViewEventsCount();
		for (int i = 0; i < nCount; i++)
		{
			CXTPCalendarViewEvent* pView = pGroup->GetViewEvent_(i);
			ASSERT(pView);
			if (pView && !pView->IsVisible())
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

UINT CXTPCalendarViewDay::SetTimer(UINT uTimeOut_ms)
{
	return XTP_SAFE_GET1(GetView_(), SetTimer(uTimeOut_ms), 0);
}


void CXTPCalendarViewDay::KillTimer(UINT uTimerID)
{
	XTP_SAFE_CALL1(GetView_(), KillTimer(uTimerID));
}

BOOL CXTPCalendarViewDay::IsSelected() const
{
	return XTP_SAFE_GET1(GetView_(), SelectionContains(GetDayDate()), FALSE);
}

void CXTPCalendarViewDay::SetSelected()
{
	XTP_SAFE_CALL1(GetView_(), SetSelection(GetDayDate(), GetDayDate()));
}

int CXTPCalendarViewDay::GetDayIndex() const
{
	if (!GetView_())
	{
		return -1;
	}
	for (int i = 0; i < GetView_()->GetViewDayCount(); i++)
	{
		if (GetView_()->GetViewDay_(i) == this)
			return i;
	}

	ASSERT(FALSE);
	return -1;
}

void CXTPCalendarViewDay::OnPrePopulateDay()
{
	WPARAM wParam = (WPARAM)(CXTPCalendarViewDay*)this;

	XTP_SAFE_CALL2(GetView_(), GetCalendarControl(), SendNotification(
		XTP_NC_CALENDAR_PREPOPULATE_DAY, wParam, 0));
}

COleDateTime CXTPCalendarViewDay::SetDayDate(COleDateTime dtDayDate)
{
	COleDateTime dtOld(m_dtDate);
	m_dtDate = CXTPCalendarUtils::ResetTime(dtDayDate);
	return dtOld;
}

void CXTPCalendarViewDay::Populate(COleDateTime dtDayDate)
{
	m_dtDate = CXTPCalendarUtils::ResetTime(dtDayDate);
	m_arViewGroups.RemoveAll();

	if (!GetView_())
	{
		return;
	}

	OnPrePopulateDay();

	CXTPCalendarResources* pResources = GetResources();
	if (!pResources)
		return;

	CXTPCalendarViewGroup* pGroup = CreateGroup();
	if (!pGroup)
	{
		return;
	}
	m_arViewGroups.Add(pGroup);

	pGroup->SetResources(pResources);
	pGroup->Populate(m_dtDate);
}


BOOL CXTPCalendarViewDay::HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	return HitTestEx(pt, pHitTest);
}

BOOL CXTPCalendarViewDay::HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (!m_Layout.m_rcDay.PtInRect(pt))
	{
		return FALSE;
	}

	if (m_Layout.m_rcDayHeader.PtInRect(pt))
	{
		FillHitTestEx(pHitTest);
		pHitTest->uHitCode = xtpCalendarHitTestDayHeader;
		return TRUE;
	}

	if (m_Layout.m_rcExpandSign.PtInRect(pt) &&
		!XTP_SAFE_GET1(GetView_(), GetTheme(), NULL))
	{
		FillHitTestEx(pHitTest);
		pHitTest->uHitCode = xtpCalendarHitTestDayExpandButton;
		return TRUE;
	}

	int nCount = GetViewGroupsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewGroup* pViewGroup = GetViewGroup_(i);
		ASSERT(pViewGroup);
		if (pViewGroup && pViewGroup->HitTestEx(pt, pHitTest))
		{
			ASSERT(!pHitTest->pViewGroup || pHitTest->pViewGroup == pViewGroup);
			pHitTest->nGroup = i;
			pHitTest->pViewGroup = pViewGroup;
			return TRUE;
		}
	}

	FillHitTestEx(pHitTest);
	pHitTest->uHitCode = xtpCalendarHitTestDayArea;
	return TRUE;
}


