// XTPCalendarEvent.cpp: implementation of the CXTPCalendarEvent class.
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarUtils.h"
#include "XTPCalendarPtrCollectionT.h"
#include "XTPCalendarEventLabel.h"
#include "XTPCalendarCustomProperties.h"
#include "XTPCalendarTimeZoneHelper.h"
#include "XTPCalendarNotifications.h"

#include "XTPCalendarRecurrencePattern.h"
#include "XTPCalendarEvent.h"
#include "XTPCalendarEvents.h"

#include "XTPCalendarData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static LPCTSTR cszEventProp_EventID = _T("EventID");
static LPCTSTR cszEventProp_ScheduleID= _T("ScheduleID");
static LPCTSTR cszEventProp_StartTime = _T("StartTime");
static LPCTSTR cszEventProp_EndTime = _T("EndTime");

static LPCTSTR cszEventProp_Subject = _T("Subject");
static LPCTSTR cszEventProp_Location = _T("Location");
static LPCTSTR cszEventProp_Body = _T("Body");

static LPCTSTR cszEventProp_MultipleSchedules= _T("MultipleSchedules");
// OLD. used to read old data files
static LPCTSTR cszEventPropOld_Description = _T("Description");

static LPCTSTR cszEventProp_AllDayEvent = _T("AllDayEvent");
static LPCTSTR cszEventProp_LabelID = _T("LabelID");
static LPCTSTR cszEventProp_BusyStatus = _T("BusyStatus");
static LPCTSTR cszEventProp_Importance = _T("Importance");

static LPCTSTR cszEventProp_ReminderMinutesBeforeStart = _T("ReminderMinutesBeforeStart");
static LPCTSTR cszEventProp_ReminderSoundFile = _T("ReminderSoundFile");

static LPCTSTR cszEventProp_CreationTime = _T("CreationTime");
static LPCTSTR cszEventProp_LastModificationTime = _T("LastModificationTime");

static LPCTSTR cszEventProp_RecurrenceState = _T("RecurrenceState");

static LPCTSTR cszEventProp_IsMeeting = _T("IsMeeting");
static LPCTSTR cszEventProp_IsPrivate = _T("IsPrivate");
static LPCTSTR cszEventProp_IsReminder = _T("IsReminder");

//## Recurrence Event properties
static LPCTSTR cszEventProp_RecurrencePatternID = _T("RecurrencePatternID");

//## Recurrence Event Exception properties
static LPCTSTR cszEventProp_RExceptionDeleted = _T("RExceptionDeleted");
static LPCTSTR cszEventProp_RException_StartTimeOrig= _T("RException_StartTimeOrig");
static LPCTSTR cszEventProp_RException_EndTimeOrig = _T("RException_EndTimeOrig");

/////////////////////////////////////////////////////////////////////////////
#define XTP_EVENT_PROP_SET_DATA_VER_1 1
#define XTP_EVENT_PROP_SET_DATA_VER_2 2
#define XTP_EVENT_PROP_SET_DATA_VER_3 3
#define XTP_EVENT_PROP_SET_DATA_VER 4

IMPLEMENT_DYNAMIC(CXTPCalendarEvent, CCmdTarget)

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
CXTPCalendarEvent::CXTPCalendarEvent(CXTPCalendarData* pDataProvider)
{
	ASSERT(pDataProvider);
	m_pDataProvider = pDataProvider;

	if (m_pDataProvider)
	{
		m_pDataProvider->InternalAddRef();
	}

	m_dwEventID = XTP_CALENDAR_UNKNOWN_EVENT_ID;
	m_nLabelID = XTP_CALENDAR_DEFAULT_LABEL_ID;
	m_uScheduleID = XTP_CALENDAR_UNKNOWN_SCHEDULE_ID;

	m_nBusyStatus = xtpCalendarBusyStatusFree;
	m_nImportance = xtpCalendarImportanceNormal;

	m_dwRecurrencePatternID = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;
	m_nRecurrenceState = xtpCalendarRecurrenceNotRecurring;
	m_bRExceptionDeleted = FALSE;

//<<EventVisible>>
	m_bEventVisible = TRUE;
//<<EventVisible>>
	m_bAllDayEvent = FALSE;
	m_dtStartTime = (DATE)0; //.SetStatus(COleDateTime::null);
	m_dtEndTime = (DATE)0; //.SetStatus(COleDateTime::null);

	m_dtCreationTime = m_dtLastModificationTime = CXTPCalendarUtils::GetCurrentTime();

	m_bReminder = FALSE;
	m_nReminderMinutesBeforeStart = XTP_CALENDAR_DEFAULT_REMINDER_MINUTES;

	m_dtRException_StartTimeOrig = (DATE)0; // .SetStatus(COleDateTime::null);
	m_dtRException_EndTimeOrig = (DATE)0; // .SetStatus(COleDateTime::null);

	m_bMeeting = FALSE;
	m_bPrivate = FALSE;

	m_dwRecurrencePatternID_ToUse = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;

	m_pCustomProperties = new CXTPCalendarCustomProperties;
	m_pCustomIconsIDs = new CXTPCalendarCustomIconIDs;
	m_pEventCategoryIDs = new CXTPCalendarEventCategoryIDs;

	m_sMultipleSchedules.Empty();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

CXTPCalendarEvent::~CXTPCalendarEvent()
{
	CMDTARGET_RELEASE(m_pDataProvider);
	CMDTARGET_RELEASE(m_pCustomProperties);
	CMDTARGET_RELEASE(m_pCustomIconsIDs);
	CMDTARGET_RELEASE(m_pEventCategoryIDs);
}

int CXTPCalendarEvent::GetCategoriesCount() const
{
	return XTP_SAFE_GET1(GetCategories(), GetSize(), 0);
}

CXTPCalendarEventLabel* CXTPCalendarEvent::GetLabel() const
{
	CXTPCalendarEventLabel* pResultLab = NULL;

	if (m_pDataProvider && m_pDataProvider->GetLabelList())
	{
		pResultLab = m_pDataProvider->GetLabelList()->Find(m_nLabelID);
	}

	return pResultLab;
}

void CXTPCalendarEvent::SetStartTime(COleDateTime dtStart)
{
	if (m_nRecurrenceState == xtpCalendarRecurrenceOccurrence ||
		m_nRecurrenceState == xtpCalendarRecurrenceMaster)
	{
		ASSERT(FALSE);
		return;
	}

	m_dtStartTime = dtStart;

	if (m_dtStartTime.GetStatus() == COleDateTime::valid &&
		m_dtEndTime.GetStatus() == COleDateTime::valid &&
		(m_dtStartTime > m_dtEndTime ||
		CXTPCalendarUtils::IsEqual(m_dtEndTime, m_dtStartTime)))
	{
		m_dtEndTime = m_dtStartTime;
	}
}

void CXTPCalendarEvent::SetEndTime(COleDateTime dtEnd)
{
	if (m_nRecurrenceState == xtpCalendarRecurrenceOccurrence ||
		m_nRecurrenceState == xtpCalendarRecurrenceMaster)
	{
		ASSERT(FALSE);
		return;
	}

	m_dtEndTime = dtEnd;

	if (m_dtStartTime.GetStatus() == COleDateTime::valid &&
		m_dtEndTime.GetStatus() == COleDateTime::valid &&
		(m_dtEndTime < m_dtStartTime ||
		CXTPCalendarUtils::IsEqual(m_dtEndTime, m_dtStartTime)))
	{
		m_dtStartTime = m_dtEndTime;
	}
}

void CXTPCalendarEvent::SetDuration(COleDateTimeSpan spDuration, BOOL bKeepStart)
{
	if (bKeepStart)
	{
		SetEndTime(m_dtStartTime + spDuration);
	}
	else
	{
		SetStartTime(m_dtEndTime - spDuration);
	}
}

void CXTPCalendarEvent::MoveEvent(COleDateTime dtNewStartTime)
{
	COleDateTimeSpan spDuration = m_dtEndTime - m_dtStartTime;
	SetStartTime(dtNewStartTime);
	SetEndTime(dtNewStartTime + spDuration);

	//CXTPCalendarRecurrencePatternPtr CalendarRecurrencePatternPtr = GetRecurrencePattern();
	//if (CalendarRecurrencePatternPtr)
	//{
	//  CalendarRecurrencePatternPtr->SetStartTime(dtNewStartTime);
	//  CalendarRecurrencePatternPtr->SetDurationMinutes((int) spDuration.GetTotalMinutes());
	//}
}

int CXTPCalendarEvent::GetDurationMinutes() const
{
	return CXTPCalendarUtils::GetDurationMinutes(m_dtStartTime, m_dtEndTime, m_bAllDayEvent);
}

COleDateTimeSpan CXTPCalendarEvent::GetDuration() const
{
	COleDateTimeSpan spDuration;

	if (m_bAllDayEvent)
	{
		spDuration = CXTPCalendarUtils::ResetTime(m_dtEndTime) - CXTPCalendarUtils::ResetTime(m_dtStartTime);
	}
	else
	{
		spDuration = m_dtEndTime - m_dtStartTime;
	}
	return spDuration;
}


CXTPCalendarRecurrencePatternPtr CXTPCalendarEvent::GetRecurrencePattern() const
{
	if (m_ptrRPattern)
	{
		ASSERT(m_nRecurrenceState == xtpCalendarRecurrenceMaster);

		CXTPCalendarRecurrencePatternPtr ptrPattern = m_ptrRPattern->ClonePattern();
		return ptrPattern;
	}

	if (!m_pDataProvider)
	{
		ASSERT(FALSE);
		return NULL;
	}
	CXTPCalendarRecurrencePatternPtr ptrPattern = m_pDataProvider->GetRecurrencePattern(m_dwRecurrencePatternID);
	return ptrPattern;
}

BOOL CXTPCalendarEvent::SetRecurrenceState_Master(CXTPCalendarRecurrencePattern* pPattern)
{
	if (!pPattern || m_nRecurrenceState != xtpCalendarRecurrenceNotRecurring)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT(!m_ptrRPattern);

	m_nRecurrenceState = xtpCalendarRecurrenceMaster;

	m_ptrRPattern = pPattern;
	m_ptrRPattern->InternalAddRef();
	m_dwRecurrencePatternID = m_ptrRPattern->GetPatternID();

	m_ptrRPattern->SetMasterEventID(m_dwEventID);

	BOOL bRes = CalcStartEndDatesFromPattern();
	ASSERT(bRes);

	return bRes;
}

BOOL CXTPCalendarEvent::SetRecurrenceState_ExceptionOccurrence(XTPCalendarEventRecurrenceState nState, DWORD dwNewPatternID)
{
	ASSERT(nState == xtpCalendarRecurrenceOccurrence || nState == xtpCalendarRecurrenceException);

	if (m_ptrRPattern || m_nRecurrenceState == xtpCalendarRecurrenceMaster)
	{
		ASSERT(FALSE); //set new recur. state to master event. strange! why ???
		return FALSE;
	}

	// update original Start/End times for exception event
	if (nState == xtpCalendarRecurrenceException)
	{
		// You should change state to exception only for occurrence or new event (event copy).
		// !!! Otherwise this may not work correctly !!!
		//ASSERT(m_nRecurrenceState == xtpCalendarRecurrenceOccurrence);
		ASSERT(m_nRecurrenceState == xtpCalendarRecurrenceNotRecurring ||
			m_nRecurrenceState == xtpCalendarRecurrenceOccurrence ||
			m_nRecurrenceState == xtpCalendarRecurrenceException);

		COleDateTime dtTmpOrig = GetStartTime();
		SetRException_StartTimeOrig(dtTmpOrig);

		dtTmpOrig = GetEndTime();
		SetRException_EndTimeOrig(dtTmpOrig);
	}

	m_nRecurrenceState = nState;

	if (dwNewPatternID != XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID)
	{
		m_dwRecurrencePatternID = dwNewPatternID;
	}
	//ASSERT(m_dwRecurrencePatternID != XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID);

	return TRUE;
}

BOOL CXTPCalendarEvent::SetRecurrenceState_NotRecurring()
{
	if (m_ptrRPattern)
	{
		m_ptrRPattern->SetMasterEventID(XTP_CALENDAR_UNKNOWN_EVENT_ID);
	}
	m_dwRecurrencePatternID_ToUse = m_dwRecurrencePatternID;

	m_nRecurrenceState = xtpCalendarRecurrenceNotRecurring;
	m_ptrRPattern = NULL;
	m_dwRecurrencePatternID = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;

	return TRUE;
}

BOOL CXTPCalendarEvent::CalcStartEndDatesFromPattern()
{
	if (m_nRecurrenceState != xtpCalendarRecurrenceMaster || !m_ptrRPattern)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	COleDateTime dtS = m_ptrRPattern->GetPatternStartDate();
	COleDateTime dtE(9999, 1, 1, 0, 0, 0);

	if (m_ptrRPattern->GetUseEndMethod() == xtpCalendarPatternEndDate ||
		m_ptrRPattern->GetUseEndMethod() == xtpCalendarPatternEndAfterOccurrences &&
		m_ptrRPattern->GetEndAfterOccurrences() < 4000)
	{
		CXTPCalendarDatesArray arDates;
		m_ptrRPattern->GetOccurrencesDates(arDates, dtS, dtE);

		int nCount = (int) arDates.GetSize();
		if (nCount)
		{
			dtS = arDates[0];
			dtE = arDates[nCount - 1];

			m_dtStartTime = m_ptrRPattern->GetStartOccurrenceTime(dtS);
			m_dtEndTime = m_ptrRPattern->GetEndOccurrenceTime(dtS);

			ASSERT(GetEventPeriodDays() > 0);
			int nDays = max(1, GetEventPeriodDays());

			dtE += COleDateTimeSpan(nDays - 1, 0, 0, 0);
		}
		else
		{
			dtE = dtS;
		}
	}

	m_dtStartTime = dtS;
	m_dtEndTime = dtE;

	UpdateStartEndDatesFromPatternExceptions();

	return TRUE;
}

BOOL CXTPCalendarEvent::UpdateStartEndDatesFromPatternExceptions()
{
	COleDateTime dtS = GetStartTime();
	COleDateTime dtE = GetEndTime();

	BOOL bRes = FALSE;
	CXTPCalendarEventsPtr ptrExceptions = m_ptrRPattern->GetExceptions();
	int nCount = ptrExceptions ? ptrExceptions->GetCount() : 0;
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pExcept = ptrExceptions->GetAt(i);
		if (pExcept->GetStartTime() < dtS)
		{
			dtS = pExcept->GetStartTime();
			bRes = TRUE;
		}
		if (pExcept->GetEndTime() > dtE)
		{
			dtE = pExcept->GetEndTime();
			bRes = TRUE;
		}
	}

	if (bRes)
	{
		m_dtStartTime = dtS;
		m_dtEndTime = dtE;
	}

	return bRes;
}

CXTPCalendarEventPtr CXTPCalendarEvent::CloneEvent()
{
	if (!m_pDataProvider)
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPCalendarEventPtr ptrNewEvent = m_pDataProvider->CreateNewEvent(GetEventID());

	if (!ptrNewEvent)
	{
		ASSERT(FALSE);
		return NULL;
	}

	if (CloneEventTo(ptrNewEvent))
		return ptrNewEvent;

	ASSERT(FALSE);
	return NULL;
}

BOOL CXTPCalendarEvent::CloneEventTo(CXTPCalendarEvent* pEventDest)
{
	if (!m_pDataProvider || !pEventDest)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pEventDest->Clear();

	BOOL bRes = pEventDest->Update(this);
	if (!bRes)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	XTPCalendarEventRecurrenceState nRState = GetRecurrenceState();

	if (nRState == xtpCalendarRecurrenceMaster)
	{
		CXTPCalendarRecurrencePatternPtr ptrRPattern = GetRecurrencePattern();
		bRes = pEventDest->SetRecurrenceState_Master(ptrRPattern);
	}
	else if (nRState != xtpCalendarRecurrenceNotRecurring)
	{
		ASSERT(nRState == xtpCalendarRecurrenceException || nRState == xtpCalendarRecurrenceOccurrence);
		DWORD dwPatternID = GetRecurrencePatternID();
		bRes = pEventDest->SetRecurrenceState_ExceptionOccurrence(nRState, dwPatternID);
	}

	if (nRState == xtpCalendarRecurrenceException)
	{
		COleDateTime dtTmpOrig = GetRException_StartTimeOrig();
		pEventDest->SetRException_StartTimeOrig(dtTmpOrig);

		dtTmpOrig = GetRException_EndTimeOrig();
		pEventDest->SetRException_EndTimeOrig(dtTmpOrig);
	}
	pEventDest->m_dwRecurrencePatternID_ToUse = m_dwRecurrencePatternID_ToUse;

	ASSERT(bRes);
	return bRes;
}

BOOL CXTPCalendarEvent::Update(CXTPCalendarEvent* pNewData)
{
	if (pNewData == this)
	{
		return TRUE;
	}

	UINT uScheduleID = pNewData->GetScheduleID();
	SetScheduleID(uScheduleID);

	SetScheduleSet(pNewData->GetScheduleSet());

	CString strTmp = pNewData->GetSubject();
	SetSubject(strTmp);

	strTmp = pNewData->GetLocation();
	SetLocation(strTmp);

	int nTmp = pNewData->GetLabelID();
	SetLabelID(nTmp);

	BOOL bTmp = pNewData->IsAllDayEvent();
	SetAllDayEvent(bTmp);
//<<EventVisible>>
	bTmp = pNewData->IsEventVisible();
	SetEventVisible(bTmp);
//<<EventVisible>>
	COleDateTime dtTmp = pNewData->GetStartTime();
	m_dtStartTime = dtTmp;

	dtTmp = pNewData->GetEndTime();
	m_dtEndTime = dtTmp;

	bTmp = pNewData->IsReminder();
	SetReminder(bTmp);

	nTmp = pNewData->GetReminderMinutesBeforeStart();
	SetReminderMinutesBeforeStart(nTmp);

	strTmp = pNewData->GetReminderSoundFile();
	SetReminderSoundFile(strTmp);

	strTmp = pNewData->GetBody();
	SetBody(strTmp);

	nTmp = pNewData->GetBusyStatus();
	SetBusyStatus(nTmp);

	nTmp = pNewData->GetImportance();
	SetImportance(nTmp);

	dtTmp = pNewData->GetCreationTime();
	SetCreationTime(dtTmp);

	dtTmp = pNewData->GetLastModificationTime();
	SetLastModificationTime(dtTmp);

	nTmp = pNewData->IsMeeting();
	SetMeeting(nTmp);

	nTmp = pNewData->IsPrivate();
	SetPrivate(nTmp);

	bTmp = pNewData->IsRExceptionDeleted();
	SetRExceptionDeleted(bTmp);

	dtTmp = pNewData->GetRException_StartTimeOrig();
	SetRException_StartTimeOrig(dtTmp);

	dtTmp = pNewData->GetRException_EndTimeOrig();
	SetRException_EndTimeOrig(dtTmp);

	// Custom properties
	ASSERT(pNewData->GetCustomProperties());
	if (pNewData->GetCustomProperties())
	{
		pNewData->GetCustomProperties()->CopyTo(m_pCustomProperties);
	}

	// Custom Icons
	if (GetCustomIcons() && pNewData->GetCustomIcons())
	{
		GetCustomIcons()->Copy(*pNewData->GetCustomIcons());
	}
	else
	{
		XTP_SAFE_CALL1(GetCustomIcons(), RemoveAll());
	}

	// Event categories
	if (GetCategories() && pNewData->GetCategories())
	{
		GetCategories()->Copy(*pNewData->GetCategories());
	}
	else
	{
		XTP_SAFE_CALL1(GetCategories(), RemoveAll());
	}

	return TRUE;
}

CXTPCalendarEventPtr CXTPCalendarEvent::CloneForOccurrence(COleDateTime dtStart,
										COleDateTime dtEnd, DWORD dwNewEventID)
{
	int eRecurrenceState = GetRecurrenceState();
	if (eRecurrenceState != xtpCalendarRecurrenceMaster || !m_pDataProvider)
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPCalendarEventPtr ptrNewEvent = m_pDataProvider->CreateNewEvent(dwNewEventID);
	if (!ptrNewEvent)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bRes = ptrNewEvent->Update(this);
	if (!bRes)
	{
		ASSERT(FALSE);
		return NULL;
	}

	ptrNewEvent->SetStartTime(dtStart);
	ptrNewEvent->SetEndTime(dtEnd);

	ptrNewEvent->SetRecurrenceState_ExceptionOccurrence(xtpCalendarRecurrenceOccurrence, m_dwRecurrencePatternID);

	ptrNewEvent->SetRExceptionDeleted(FALSE);

	return ptrNewEvent;
}

BOOL CXTPCalendarEvent::MakeEventAsRecurrence()
{
	if (m_nRecurrenceState != xtpCalendarRecurrenceNotRecurring || !m_pDataProvider)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT(m_ptrRPattern == NULL);

	CXTPCalendarRecurrencePatternPtr ptrPattern;
	ptrPattern = m_pDataProvider->CreateNewRecurrencePattern(m_dwRecurrencePatternID_ToUse);

	if (!SetRecurrenceState_Master(ptrPattern))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

BOOL CXTPCalendarEvent::UpdateRecurrence(CXTPCalendarRecurrencePattern* pRPattern)
{
	if (m_nRecurrenceState != xtpCalendarRecurrenceMaster ||
			!m_pDataProvider || !m_ptrRPattern || !pRPattern)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//ASSERT(m_dwRecurrencePatternID == m_ptrRPattern->GetPatternID());
	//ASSERT(m_dwEventID == m_ptrRPattern->GetMasterEventID());

	//DWORD dwPatternID2 = pRPattern->GetPatternID();
	//DWORD dwMasterID2 = pRPattern->GetMasterEventID();

	//if (m_dwRecurrencePatternID != dwPatternID2 || m_dwEventID != dwMasterID2)
	//{
	//  ASSERT(FALSE);
	//  return FALSE;
	//}
	//------------------------------------------------
	BOOL bRes = m_ptrRPattern->Update(pRPattern);
	if (!bRes)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_dwRecurrencePatternID = m_ptrRPattern->GetPatternID();
	m_ptrRPattern->SetMasterEventID(m_dwEventID);

	bRes = CalcStartEndDatesFromPattern();
	ASSERT(bRes);

	return bRes;
}

BOOL CXTPCalendarEvent::RemoveRecurrence()
{
	COleDateTime dtStart, dtEnd;
	BOOL bIsMaster = GetRecurrenceState() == xtpCalendarRecurrenceMaster;

	if (bIsMaster && m_ptrRPattern)
	{
		dtStart = CXTPCalendarUtils::UpdateTime(m_ptrRPattern->GetPatternStartDate(),
					m_ptrRPattern->GetStartTime());

		dtEnd = dtStart + m_ptrRPattern->GetDuration();
	}
	else
	{
		bIsMaster = FALSE;
	}

	if (!SetRecurrenceState_NotRecurring())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (bIsMaster)
	{
		SetStartTime(dtStart);
		SetEndTime(dtEnd);
	}

	return TRUE;
}

CXTPCalendarRecurrencePattern* CXTPCalendarEvent::GetRPatternRef() const
{
	ASSERT(GetRecurrenceState() == xtpCalendarRecurrenceMaster);

	return m_ptrRPattern;
}

int CXTPCalendarEvent::GetEventPeriodDays() const
{
	return CXTPCalendarUtils::GetEventPeriodDays(GetStartTime(), GetEndTime());
}

BOOL CXTPCalendarEvent::IsEqualStartEnd(const CXTPCalendarEvent* pEvent) const
{
	COleDateTime dtStart2 = pEvent->GetStartTime();
	COleDateTime dtEnd2 = pEvent->GetEndTime();

	BOOL bRes = CXTPCalendarUtils::IsEqual(m_dtStartTime, dtStart2, TRUE) &&
				CXTPCalendarUtils::IsEqual(m_dtEndTime, dtEnd2, TRUE);
	return bRes;
}

BOOL CXTPCalendarEvent::IsEqualIDs(const CXTPCalendarEvent* pEvent) const
{
	if (!pEvent)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nRState2 = pEvent->GetRecurrenceState();

	BOOL bRes = FALSE;
	if (m_nRecurrenceState == xtpCalendarRecurrenceNotRecurring &&
			nRState2 == xtpCalendarRecurrenceNotRecurring)
	{
		DWORD dwEventID2 = pEvent->GetEventID();
		bRes = (m_dwEventID == dwEventID2);
	}
	else
	{
		DWORD dwPatternID2 = pEvent->GetRecurrencePatternID();

		// for xtpCalendarRecurrenceMaster & xtpCalendarRecurrenceOccurrence
		COleDateTime dtStart = m_dtStartTime;
		COleDateTime dtEnd = m_dtEndTime;

		COleDateTime dtStart2 = pEvent->GetStartTime();
		COleDateTime dtEnd2 = pEvent->GetEndTime();

		if (m_nRecurrenceState == xtpCalendarRecurrenceException)
		{
			dtStart = GetRException_StartTimeOrig();
			dtEnd = GetRException_EndTimeOrig();
		}

		if (pEvent->GetRecurrenceState() == xtpCalendarRecurrenceException)
		{
			dtStart2 = pEvent->GetRException_StartTimeOrig();
			dtEnd2 = pEvent->GetRException_EndTimeOrig();
		}

		bRes = m_dwRecurrencePatternID == dwPatternID2 &&
			CXTPCalendarUtils::IsEqual(dtStart, dtStart2, TRUE) &&
			CXTPCalendarUtils::IsEqual(dtEnd, dtEnd2, TRUE);
	}
	return bRes;
}

void CXTPCalendarEvent::Clear()
{
	CXTPCalendarEventPtr ptrEmptyEvent = m_pDataProvider->CreateNewEvent(GetEventID());
	if (!ptrEmptyEvent)
	{
		ASSERT(FALSE);
		return;
	}

	SetRecurrenceState_NotRecurring();
	Update(ptrEmptyEvent);
}

void CXTPCalendarEvent::DoPropExchange(CXTPPropExchange* pPX)
{
	long nVersion = XTP_EVENT_PROP_SET_DATA_VER;
	PX_Long(pPX, _T("Version"), nVersion, XTP_EVENT_PROP_SET_DATA_VER);

	// TODO:
	PX_DWord(pPX, cszEventProp_ScheduleID, (DWORD&)m_uScheduleID, 0);

	PX_DWord(pPX, cszEventProp_EventID, m_dwEventID, XTP_CALENDAR_UNKNOWN_EVENT_ID);
	PX_DateTime(pPX, cszEventProp_StartTime, m_dtStartTime);
	PX_DateTime(pPX, cszEventProp_EndTime, m_dtEndTime);

	PX_String(pPX, cszEventProp_Subject, m_strSubject, _T(""));
	PX_String(pPX, cszEventProp_Location, m_strLocation, _T(""));

	if (nVersion > XTP_EVENT_PROP_SET_DATA_VER_3)
	{
		if (pPX->IsStoring())
		{
			if (m_sMultipleSchedules.IsEmpty())
				m_sMultipleSchedules.Format(_T("|%d|"), m_uScheduleID);
		}

		PX_String(pPX, cszEventProp_MultipleSchedules, m_sMultipleSchedules, _T(""));

		if (!pPX->IsStoring())
		{
			if (m_sMultipleSchedules.IsEmpty())
				m_sMultipleSchedules.Format(_T("|%d|"), m_uScheduleID);
		}
	}

	if (nVersion > XTP_EVENT_PROP_SET_DATA_VER_1)
	{
		PX_String(pPX, cszEventProp_Body, m_strBody, _T(""));
	}
	else
	{
		PX_String(pPX, cszEventPropOld_Description, m_strBody, _T(""));
	}

	PX_Bool(pPX, cszEventProp_AllDayEvent, m_bAllDayEvent, FALSE);
	PX_Int(pPX, cszEventProp_LabelID, m_nLabelID, XTP_CALENDAR_DEFAULT_LABEL_ID);

	PX_Int(pPX, cszEventProp_BusyStatus, m_nBusyStatus, xtpCalendarBusyStatusFree);
	PX_Int(pPX, cszEventProp_Importance, m_nImportance, xtpCalendarImportanceNormal);

	if (nVersion > XTP_EVENT_PROP_SET_DATA_VER_1)
	{
		PX_Bool(pPX, cszEventProp_IsReminder, m_bReminder, FALSE);
	}

	PX_Int(pPX, cszEventProp_ReminderMinutesBeforeStart, m_nReminderMinutesBeforeStart, 15);
	PX_String(pPX, cszEventProp_ReminderSoundFile, m_strReminderSoundFile, _T(""));

	PX_DateTime(pPX, cszEventProp_CreationTime, m_dtCreationTime);
	PX_DateTime(pPX, cszEventProp_LastModificationTime, m_dtLastModificationTime);

	PX_Bool(pPX, cszEventProp_IsMeeting, m_bMeeting, FALSE);
	PX_Bool(pPX, cszEventProp_IsPrivate, m_bPrivate, FALSE);

	if (pPX->IsStoring())
	{
		PX_Enum(pPX, cszEventProp_RecurrenceState, m_nRecurrenceState, xtpCalendarRecurrenceNotRecurring);
		PX_DWord(pPX, cszEventProp_RecurrencePatternID, m_dwRecurrencePatternID, XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID);

		if (m_nRecurrenceState == xtpCalendarRecurrenceException)
		{
			PX_Bool(pPX, cszEventProp_RExceptionDeleted, m_bRExceptionDeleted, FALSE);
			PX_DateTime(pPX, cszEventProp_RException_StartTimeOrig, m_dtRException_StartTimeOrig);
			PX_DateTime(pPX, cszEventProp_RException_EndTimeOrig, m_dtRException_EndTimeOrig);
		}
	}
	else
	{
		//===========================================================================
		XTPCalendarEventRecurrenceState nRecurrenceState = m_nRecurrenceState = xtpCalendarRecurrenceNotRecurring;
		PX_Enum(pPX, cszEventProp_RecurrenceState, nRecurrenceState, xtpCalendarRecurrenceNotRecurring);

		DWORD dwRecurrencePatternID = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;
		PX_DWord(pPX, cszEventProp_RecurrencePatternID, dwRecurrencePatternID, XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID);

		if (nRecurrenceState == xtpCalendarRecurrenceMaster)
		{
			if (!MakeEventAsRecurrence())
			{
				ASSERT(FALSE);
				return;
			}
		}
		else if (nRecurrenceState == xtpCalendarRecurrenceException || nRecurrenceState == xtpCalendarRecurrenceOccurrence)
		{
			SetRecurrenceState_ExceptionOccurrence(nRecurrenceState, dwRecurrencePatternID);

			if (nRecurrenceState == xtpCalendarRecurrenceException)
			{
				PX_Bool(pPX, cszEventProp_RExceptionDeleted, m_bRExceptionDeleted, FALSE);
				PX_DateTime(pPX, cszEventProp_RException_StartTimeOrig, m_dtRException_StartTimeOrig);
				PX_DateTime(pPX, cszEventProp_RException_EndTimeOrig, m_dtRException_EndTimeOrig);
			}
		}
	}

	//------------------------------------------------------------------------
	if (m_pCustomProperties)
		m_pCustomProperties->DoPropExchange(pPX);

	if (m_pCustomIconsIDs)
		m_pCustomIconsIDs->DoPropExchange(pPX);

	if (m_pEventCategoryIDs && nVersion > XTP_EVENT_PROP_SET_DATA_VER_2)
		m_pEventCategoryIDs->DoPropExchange(pPX);
}

void CXTPCalendarEvent::ClearIntermediateData()
{
	m_dwRecurrencePatternID_ToUse = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;

	if (!IsReminder() && m_pCustomProperties)
	{
		// clear this property if reminder disabled
		m_pCustomProperties->RemoveProperty(cszEventCustProp_NextReminderTime_Snoozed);
	}

	if (!IsReminder() && m_ptrRPattern)
	{
		ASSERT(m_nRecurrenceState == xtpCalendarRecurrenceMaster);

		// clear reminders info for occurrences
		// if reminder disabled for whole series.
		m_ptrRPattern->GetOccReminders()->RemoveAll();
	}
}

void CXTPCalendarEvent::SetRecurrencePatternID(DWORD dwRecurrencePatternID)
{
	m_dwRecurrencePatternID = dwRecurrencePatternID;

	if (m_ptrRPattern)
	{
		ASSERT(m_nRecurrenceState == xtpCalendarRecurrenceMaster);
		m_ptrRPattern->SetPatternID(dwRecurrencePatternID);
	}
}

void CXTPCalendarEvent::SetEventID(DWORD dwEventID)
{
	m_dwEventID = dwEventID;

	if (m_nRecurrenceState == xtpCalendarRecurrenceMaster)
	{
		ASSERT(m_ptrRPattern);
		if (m_ptrRPattern)
		{
			m_ptrRPattern->SetMasterEventID(m_dwEventID);
		}
	}
}

////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarEvent, CCmdTarget)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "Subject", DISPID_VALUE, m_strSubject, VT_BSTR)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "Location", 1, m_strLocation, VT_BSTR)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "Body", 2, m_strBody, VT_BSTR)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "MeetingFlag", 3, m_bMeeting, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "PrivateFlag", 4, m_bPrivate, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "Label", 5, m_nLabelID, VT_I4)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "BusyStatus", 6, m_nBusyStatus, VT_I4)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "Importance", 7, m_nImportance, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "StartTime", 8, OleGetStartTime, OleSetStartTime, VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "EndTime", 9, OleGetEndTime, OleSetEndTime, VT_DATE)

	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "StartTimeV", 36, OleGetStartTimeV, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "EndTimeV", 37, OleGetEndTimeV, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_ID(CXTPCalendarEvent, "AllDayEvent", 14, m_bAllDayEvent, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "RecurrenceState", 10, OleGetRecurrence, SetNotSupported, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "Id", 11, OleGetEventID, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "CreationTime", 12, OleGetCreationTime, OleSetCreationTime, VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "LastModificationTime", 13, OleGetLastModificationTime, OleSetLastModificationTime, VT_DATE)

	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "RecurrencePattern", 15, OleGetRecurrencePattern, SetNotSupported, VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "CreateRecurrence", 16, OleCreateRecurrence, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "CreateRecurrenceEx", 116, OleCreateRecurrenceEx, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "UpdateRecurrence", 17, OleUpdateRecurrence, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "RemoveRecurrence", 18, OleRemoveRecurrence, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarEvent, "MakeAsRException", 19, OleMakeAsRException, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "MakeAsRExceptionEx", 119, OleMakeAsRExceptionEx, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "SetRExceptionDeleted", 20, OleSetRExceptionDeleted, VT_EMPTY, VTS_BOOL)

	DISP_FUNCTION_ID(CXTPCalendarEvent, "CloneEvent", 21, OleCloneEvent, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "Update", 22, OleUpdate, VT_EMPTY, VTS_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPCalendarEvent, "CustomProperties", 23, OleGetCustomProperties, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_ID(CXTPCalendarEvent, "Location", 1, m_strLocation, VT_BSTR)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "Body", 2, m_strBody, VT_BSTR)

	DISP_PROPERTY_ID(CXTPCalendarEvent, "Reminder", 24, m_bReminder, VT_BOOL)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "ReminderMinutesBeforeStart", 25, m_nReminderMinutesBeforeStart, VT_I4)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "ReminderSoundFile", 26, m_strReminderSoundFile, VT_BSTR)

	DISP_PROPERTY_ID(CXTPCalendarEvent, "RExceptionStartTimeOrig", 27, m_dtRException_StartTimeOrig, VT_DATE)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "RExceptionEndTimeOrig", 28, m_dtRException_EndTimeOrig, VT_DATE)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "RExceptionDeleted", 29, m_bRExceptionDeleted, VT_BOOL)

	DISP_PROPERTY_ID(CXTPCalendarEvent, "ScheduleID", 30, m_uScheduleID, VT_I4)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "MultipleSchedules", 35, m_sMultipleSchedules, VT_BSTR)

	DISP_FUNCTION_ID(CXTPCalendarEvent, "CustomIcons", 31, OleGetCustomIcons, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "DataProvider", 32, OleGetDataProvider, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarEvent, "Categories", 33, OleGetCategories, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_ID(CXTPCalendarEvent, "EventVisible", 34, m_bEventVisible, VT_BOOL)
END_DISPATCH_MAP()

// {5FED5027-B4A5-492f-ADA0-56E1B0772A53}
static const GUID IID_ICalendarEvent =
{ 0x5fed5027, 0xb4a5, 0x492f, { 0xad, 0xa0, 0x56, 0xe1, 0xb0, 0x77, 0x2a, 0x53 } };


BEGIN_INTERFACE_MAP(CXTPCalendarEvent, CCmdTarget)
	INTERFACE_PART(CXTPCalendarEvent, IID_ICalendarEvent, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarEvent, IID_ICalendarEvent)

DATE CXTPCalendarEvent::OleGetStartTime()
{
	return m_dtStartTime;
}

void CXTPCalendarEvent::OleSetStartTime(DATE dt)
{
	SetStartTime(dt);
}

DATE CXTPCalendarEvent::OleGetEndTime()
{
	return m_dtEndTime;
}

DATE CXTPCalendarEvent::OleGetCreationTime()
{
	return GetCreationTime();
}

void CXTPCalendarEvent::OleSetCreationTime(DATE dt)
{
	SetCreationTime(dt);
}

DATE CXTPCalendarEvent::OleGetLastModificationTime()
{
	return GetLastModificationTime();
}

void CXTPCalendarEvent::OleSetLastModificationTime(DATE dt)
{
	SetLastModificationTime(dt);
}

void CXTPCalendarEvent::OleSetEndTime(DATE dt)
{
	SetEndTime(dt);
}


LPDISPATCH CXTPCalendarEvent::OleGetStartTimeV()
{
	SYSTEMTIME st;
	m_dtStartTime.GetAsSystemTime(st);

	CXTPCalendarSystemTime* lpDisp = new CXTPCalendarSystemTime(&st);
	return lpDisp->GetIDispatch(FALSE);
}


LPDISPATCH CXTPCalendarEvent::OleGetEndTimeV()
{
	SYSTEMTIME st;
	m_dtEndTime.GetAsSystemTime(st);

	CXTPCalendarSystemTime* lpDisp = new CXTPCalendarSystemTime(&st);
	return lpDisp->GetIDispatch(FALSE);
}

long CXTPCalendarEvent::OleGetEventID()
{
	return GetEventID();
}

LPDISPATCH CXTPCalendarEvent::OleGetCustomProperties()
{
	CXTPCalendarCustomProperties* pProps = GetCustomProperties();
	return pProps ? pProps->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarEvent::OleGetCustomIcons()
{
	CXTPCalendarCustomIconIDs* pIcons = GetCustomIcons();
	return pIcons ? pIcons->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarEvent::OleGetCategories()
{
	CXTPCalendarEventCategoryIDs* pCategories = GetCategories();
	return pCategories ? pCategories->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarEvent::OleGetDataProvider()
{
	CXTPCalendarData* pDP = GetDataProvider();
	return pDP ? pDP->GetIDispatch(TRUE) : NULL;
}

int CXTPCalendarEvent::OleGetRecurrence()
{
	return GetRecurrenceState();
}

LPDISPATCH CXTPCalendarEvent::OleGetRecurrencePattern()
{
	CXTPCalendarRecurrencePatternPtr pPattern = GetRecurrencePattern();
	return pPattern ? pPattern->GetIDispatch(TRUE) : 0;
}

LPDISPATCH CXTPCalendarEvent::OleCreateRecurrence()
{
	MakeEventAsRecurrence();
	return m_ptrRPattern ? m_ptrRPattern->GetIDispatch(TRUE) : FALSE;
}

LPDISPATCH CXTPCalendarEvent::OleCreateRecurrenceEx(long nPatternID)
{
	m_dwRecurrencePatternID_ToUse = (DWORD)nPatternID;
	MakeEventAsRecurrence();
	return m_ptrRPattern ? m_ptrRPattern->GetIDispatch(TRUE) : FALSE;
}

void CXTPCalendarEvent::OleUpdateRecurrence(LPDISPATCH Pattern)
{
	CXTPCalendarRecurrencePattern* pPattern = DYNAMIC_DOWNCAST(CXTPCalendarRecurrencePattern, CCmdTarget::FromIDispatch(Pattern));
	if (!pPattern)
		AfxThrowOleException(E_INVALIDARG);

	UpdateRecurrence(pPattern);
}

void CXTPCalendarEvent::OleRemoveRecurrence()
{
	RemoveRecurrence();
}

void CXTPCalendarEvent::OleMakeAsRException()
{
	MakeAsRException();
}

void CXTPCalendarEvent::OleMakeAsRExceptionEx(long nPatternID)
{
	MakeAsRException((DWORD)nPatternID);
}

void CXTPCalendarEvent::OleSetRExceptionDeleted(BOOL bDeleted)
{
	SetRExceptionDeleted(bDeleted);
}

LPDISPATCH CXTPCalendarEvent::OleCloneEvent()
{
	CXTPCalendarEventPtr ptrNewEvent = CloneEvent();
	return ptrNewEvent ? ptrNewEvent->GetIDispatch(TRUE) : 0;
}

void CXTPCalendarEvent::OleUpdate(LPDISPATCH SourceEvent)
{
	CXTPCalendarEvent* pSourceEvent = DYNAMIC_DOWNCAST(CXTPCalendarEvent, CCmdTarget::FromIDispatch(SourceEvent));
	if (!pSourceEvent)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	Update(pSourceEvent);
}

#endif
