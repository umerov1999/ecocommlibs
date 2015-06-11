// XTPCalendarData.cpp: implementation of the CXTPCalendarData class.
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


#include "Common/XTPNotifyConnection.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarUtils.h"
#include "XTPCalendarNotifications.h"
#include "XTPCalendarOptions.h"
#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarEvent.h"
#include "XTPCalendarEvents.h"
#include "XTPCalendarRecurrencePattern.h"

#include "XTPCalendarCustomProperties.h"

#include "XTPCalendarEventLabel.h"
#include "XTPCalendarResource.h"

#include "XTPCalendarData.h"
#include "XTPCalendarMemoryDataProvider.h"

#include "XTPCalendarADO.h"
#include "XTPCalendarDatabaseDataProvider.h"

#include "XTPCalendarMAPIWrapper.h"
#include "XTPCalendarMAPIDataProvider.h"
#include "XTPCalendarCustomDataProvider.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define DBG_CACHE_ASSERT ASSERT

#ifndef DBG_CACHE_ASSERT
#define DBG_CACHE_ASSERT
#endif

//===========================================================================
// Summary:
//     Define data provider cache hash table size. Should be a prime number!
//     like: 503, 1021, 1511, 2003, 3001, 4001, 5003, 6007, 8009, 12007,
//     16001, 32003, 48017, 64007
// See Also: CMap overview
//===========================================================================
#define XTP_DP_CACHE_HASH_TABLE_SIZE 1021

long VAR2long(const COleVariant& oleVar)
{
	try
	{
		_variant_t vtValue((const VARIANT*)&oleVar);

		return (long)vtValue;
	}
	//catch(_com_error e)
	catch(...)
	{
		ASSERT(FALSE);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarData::CXTPDPCache::CXTPDPCache()
{
	m_pCacheDP = NULL;
	m_eCacheMode = xtpCalendarDPCacheModeOff;

	m_mapIsDayInCache.InitHashTable(XTP_DP_CACHE_HASH_TABLE_SIZE, FALSE);
}

CXTPCalendarData::CXTPDPCache::~CXTPDPCache()
{
	SafeClose();
}

void CXTPCalendarData::CXTPDPCache::SafeClose()
{
	if (m_pCacheDP)
	{
		if (m_pCacheDP->IsOpen())
		{
			m_pCacheDP->Close();
		}
	}
	CMDTARGET_RELEASE(m_pCacheDP);

	m_eCacheMode = xtpCalendarDPCacheModeOff;
}

void CXTPCalendarData::CXTPDPCache::SafeRemoveAll()
{
	if (m_pCacheDP)
	{
		m_pCacheDP->RemoveAllEvents();
	}
	m_mapIsDayInCache.RemoveAll();
}

BOOL CXTPCalendarData::CXTPDPCache::IsDayInCache(COleDateTime dtDay)
{
	BOOL bIs = FALSE;
	return m_mapIsDayInCache.Lookup((DWORD)(DATE)dtDay, bIs) && bIs;
}

void CXTPCalendarData::CXTPDPCache::SetDayInCache(COleDateTime dtDay, BOOL bSet)
{
	if (bSet)
	{
		m_mapIsDayInCache.SetAt((DWORD)(DATE)dtDay, TRUE);
	}
	else
	{
		m_mapIsDayInCache.RemoveKey((DWORD)(DATE)dtDay);
	}
}

BOOL CXTPCalendarData::CXTPDPCache::IsEventInCache(CXTPCalendarEvent* pEvent)
{
	if (!pEvent || !m_pCacheDP)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nRState = pEvent->GetRecurrenceState();

	if (nRState == xtpCalendarRecurrenceNotRecurring ||
		nRState == xtpCalendarRecurrenceMaster)
	{
		DWORD dwEventID = pEvent->GetEventID();
		CXTPCalendarEventPtr ptrEvInCache = m_pCacheDP->GetEvent(dwEventID);

		return ptrEvInCache != NULL;
	}
	else
	{
		ASSERT(nRState == xtpCalendarRecurrenceOccurrence || nRState == xtpCalendarRecurrenceException);

		DWORD dwRPatternID = pEvent->GetRecurrencePatternID();
		CXTPCalendarRecurrencePatternPtr ptrRPattern = m_pCacheDP->GetRecurrencePattern(dwRPatternID);
		return ptrRPattern != NULL;
	}
}

void CXTPCalendarData::CXTPDPCache::_AddToCacheIfNeed(CXTPCalendarRecurrencePattern* pPattern)
{
	ASSERT(pPattern);
	if (!pPattern || !m_pCacheDP)
	{
		return;
	}

	DWORD dwRPatternID = pPattern->GetPatternID();
	CXTPCalendarRecurrencePatternPtr ptrRPattern = m_pCacheDP->GetRecurrencePattern(dwRPatternID);

	if (ptrRPattern)
	{
		return;
	}

	#ifdef _DEBUG
	{
		DWORD dwMasterEventID = pPattern->GetMasterEventID();
		CXTPCalendarEventPtr ptrMasterEvent = m_pCacheDP->GetEvent(dwMasterEventID);
		ASSERT(ptrMasterEvent == NULL);
	}
	#endif

	CXTPCalendarEventPtr ptrMasterEvent = pPattern->GetMasterEvent(); // this call also add master event to cache
	ASSERT(ptrMasterEvent);

	if (ptrMasterEvent)
	{
		// Must be added to cache in call: pPattern->GetMasterEvent();
		//
		DWORD dwMasterEventID = ptrMasterEvent->GetEventID();
		CXTPCalendarEventPtr ptrEvInCache = m_pCacheDP->GetEvent(dwMasterEventID);
		DBG_CACHE_ASSERT(ptrEvInCache);

		if (!ptrEvInCache)
		{
			// this strange, but add if it is still not here
			VERIFY(m_pCacheDP->AddEvent(ptrMasterEvent));
		}
	}
}

void CXTPCalendarData::CXTPDPCache::AddToCacheIfNeed(CXTPCalendarEvent* pEvent)
{
	ASSERT(pEvent);
	if (!pEvent || !m_pCacheDP)
	{
		return;
	}

	int nRState = pEvent->GetRecurrenceState();

	if (nRState == xtpCalendarRecurrenceNotRecurring ||
		nRState == xtpCalendarRecurrenceMaster)
	{
		DWORD dwEventID = pEvent->GetEventID();
		CXTPCalendarEventPtr ptrEvInCache = m_pCacheDP->GetEvent(dwEventID);
		if (!ptrEvInCache)
		{
			VERIFY(m_pCacheDP->AddEvent(pEvent));
		}
	}
	else
	{
		// From previous version.
		// Non-Optimal. Should not be called.
		ASSERT(FALSE);
	}
}

void CXTPCalendarData::CXTPDPCache::AddToCache(CXTPCalendarEvents* pEvents, COleDateTime dtDay)
{
	if (!pEvents)
	{
		return;
	}

	int nCount = pEvents->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvent = pEvents->GetAt(i ,FALSE);
		AddToCacheIfNeed(pEvent);
	}

	SetDayInCache(dtDay, TRUE);
}

void CXTPCalendarData::SetCacheMode(int eCacheMode, CXTPCalendarData* pCacheDataProvider)
{
	if (eCacheMode == xtpCalendarDPCacheModeOff)
	{
		m_cache.SafeClose();
	}
	else
	{
		if (!m_cache.m_pCacheDP && !pCacheDataProvider)
		{
			m_cache.m_pCacheDP = new CXTPCalendarMemoryDataProvider();
		}
		else if (!m_cache.m_pCacheDP && pCacheDataProvider)
		{
			m_cache.m_pCacheDP = pCacheDataProvider;
		}
		else if (m_cache.m_pCacheDP && pCacheDataProvider)
		{
			m_cache.SafeClose();
			m_cache.m_pCacheDP = pCacheDataProvider;
		}
		else
		{
			ASSERT(m_cache.m_pCacheDP && !pCacheDataProvider);
		}

		if (!m_cache.m_pCacheDP)
		{
			return;
		}
		if (!m_cache.m_pCacheDP->IsOpen())
		{
			VERIFY( m_cache.m_pCacheDP->Open() );
		}
		m_cache.SafeRemoveAll();

		m_cache.m_eCacheMode = eCacheMode;
	}
}

void CXTPCalendarData::ClearCache()
{
	m_cache.SafeRemoveAll();
}

void CXTPCalendarData::RemoveAllEvents()
{
	if (!IsOpen())
	{
		ASSERT(FALSE);
		return;
	}

	m_cache.SafeRemoveAll();

	DoRemoveAllEvents();
}

//////////////////////////////////////////////////////////////////////////
DWORD CXTPCalendarData::ms_dwNextFreeTempID = DWORD(-1);
//===========================================================================
IMPLEMENT_DYNAMIC(CXTPCalendarData, CCmdTarget)

CXTPCalendarData::CXTPCalendarData()
{
	m_pCalendarOptions = NULL;
	m_bOwnershipMode = FALSE;

	m_bOpened = FALSE;
	m_pConnection = new CXTPNotifyConnection();
	m_bDisableNotificationsSending = FALSE;

	m_pLabelList = new CXTPCalendarEventLabels();
	m_pLabelList->InitDefaultValues();

	m_typeProvider = xtpCalendarDataProviderUnknown;

	m_pCustomProperties = new CXTPCalendarCustomProperties();
	m_pSchedules = new CXTPCalendarSchedules();

	m_pEventCategories = new CXTPCalendarEventCategories();
	m_pEventCategories->InitDefaultValues();


#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarData::~CXTPCalendarData()
{
	if (m_pCalendarOptions)
	{
		m_pCalendarOptions->SetDataProvider(NULL);
	}
	ASSERT(!m_pCalendarOptions);
	//CMDTARGET_RELEASE(m_pCalendarOptions);

	CMDTARGET_RELEASE(m_pLabelList);
	CMDTARGET_RELEASE(m_pConnection);
	CMDTARGET_RELEASE(m_pCustomProperties);
	CMDTARGET_RELEASE(m_pSchedules);
	CMDTARGET_RELEASE(m_pEventCategories);
}

void CXTPCalendarData::SetLabelList(CXTPCalendarEventLabels* pLabelList)
{
	CMDTARGET_RELEASE(m_pLabelList);

	m_pLabelList = pLabelList;
	m_pLabelList->SetDefaultValues(FALSE); // custom values, don't update them on theme change (RefreshMetrics of Paint Manager)

	CMDTARGET_ADDREF(m_pLabelList);
}

void CXTPCalendarData::UpdateEventLabels(COLORREF *clrEventColors, UINT uCount)
{
	m_pLabelList->UpdateDefaultValues(clrEventColors, uCount);
}

CXTPCalendarEventPtr CXTPCalendarData::CreateNewEvent(DWORD dwEventID)
{
	CXTPCalendarEventPtr ptrEvent = new CXTPCalendarEvent(this);

	if (ptrEvent)
	{
		DWORD dwTempID = dwEventID != XTP_CALENDAR_UNKNOWN_EVENT_ID ? dwEventID: GetNextFreeTempID();
		ptrEvent->SetEventID(dwTempID);
	}

	return ptrEvent;
}

CXTPCalendarRecurrencePatternPtr CXTPCalendarData::CreateNewRecurrencePattern(DWORD dwPatternID)
{
	CXTPCalendarRecurrencePatternPtr ptrPattern = new CXTPCalendarRecurrencePattern(this);
	if (ptrPattern)
	{
		DWORD dwTempID = dwPatternID != XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID? dwPatternID : GetNextFreeTempID();
		ptrPattern->SetPatternID(dwTempID);
	}
	return ptrPattern;
}

CString CXTPCalendarData::GetDataSource()
{
	return DataSourceFromConStr(m_strConnectionString);
}

CXTPCalendarEventsPtr CXTPCalendarData::RetrieveDayEvents(COleDateTime dtDay)
{
	if (!IsOpen())
	{
		DBG_DATA_ASSERT(FALSE);
		return NULL;
	}
	dtDay = CXTPCalendarUtils::ResetTime(dtDay);

	CXTPCalendarEventsPtr ptrResult = NULL;

	if ((m_cache.m_eCacheMode & xtpCalendarDPCacheModeOnMask) &&
		m_cache.IsDayInCache(dtDay))
	{
		ASSERT(m_cache.m_pCacheDP);
		ptrResult = m_cache.m_pCacheDP->RetrieveDayEvents(dtDay);
	}
	else
	{
		ptrResult = DoRetrieveDayEvents(dtDay);
		if (!ptrResult)
		{
			return NULL;
		}

		_PostProcessRecurrenceIfNeed(ptrResult);

		if (m_cache.m_eCacheMode & xtpCalendarDPCacheModeOnMask)
		{
			ASSERT(m_cache.m_pCacheDP);
			m_cache.AddToCache(ptrResult, dtDay);
		}

		_PostProcessOccurrencesFromMaster(dtDay, ptrResult);

		_FilterDayEventsInstancesByEndTime(dtDay, ptrResult);
	}

	return ptrResult;
}

BOOL CXTPCalendarData::AddEvents(CXTPCalendarEvents* pEvents)
{
	BOOL bResult = TRUE;
	int nEventsCount = pEvents->GetCount();
	for (int i = 0; i < nEventsCount; i++)
	{
		CXTPCalendarEvent* pEvent = pEvents->GetAt(i, FALSE);
		if (!AddEvent(pEvent))
		{
			bResult = FALSE;
		}
	}
	return bResult;
}

BOOL CXTPCalendarData::AddEvent(CXTPCalendarEvent* pEvent)
{
	if (!IsOpen())
	{
		return FALSE;
	}

	if (!pEvent)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pEvent->ClearIntermediateData();

	COleDateTime dtNow = CXTPCalendarUtils::GetCurrentTime();
	pEvent->SetLastModificationTime(dtNow);

	DWORD dwNewEventID = pEvent->GetEventID();

	if (!DoCreate_Event(pEvent, dwNewEventID))
	{
		DBG_DATA_ASSERT(FALSE);
		return FALSE;
	}
	ASSERT(dwNewEventID != XTP_CALENDAR_UNKNOWN_EVENT_ID);
	pEvent->SetEventID(dwNewEventID);

	int nRState = (long)pEvent->GetRecurrenceState();

	BOOL bSendPatternAddedEvent = FALSE;
	DWORD dwPatternID = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;

	//- Process Recurrence Pattern for Master event -------------------------
	if (nRState == xtpCalendarRecurrenceMaster)
	{
		CXTPCalendarRecurrencePattern* pPatternRef = pEvent->GetRPatternRef();
		if (!pPatternRef)
		{
			ASSERT(FALSE);
			return FALSE;
		}

		pPatternRef->SetMasterEventID(dwNewEventID);

		//pPatternRef->ResetReminderForOccFromThePast(pEvent);

		VERIFY( pPatternRef->GetOccReminders()->Save(pPatternRef->GetCustomProperties(), pPatternRef) );

		BOOL bRes = DoCreate_RPattern(pPatternRef, dwPatternID);
		DBG_DATA_ASSERT(bRes);

		pPatternRef->GetOccReminders()->ClearProperties(pPatternRef->GetCustomProperties());

		if (!bRes)
		{
			return FALSE;
		}
		ASSERT(dwPatternID != XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID);

		DWORD dwPatternID_prev = pEvent->GetRecurrencePatternID();
		pEvent->SetRecurrencePatternID(dwPatternID);

		if (dwPatternID_prev != dwPatternID)
		{
			bRes = DoUpdate_Event(pEvent);
			DBG_DATA_ASSERT(bRes);
			if (!bRes)
			{
				return FALSE;
			}
		}

		bSendPatternAddedEvent = TRUE;
	}

	//---------------------------------------------------------------------------
	if (m_cache.m_eCacheMode == xtpCalendarDPCacheModeOnRepeat)
	{
		ASSERT(m_cache.m_pCacheDP);
		if (m_cache.m_pCacheDP) VERIFY( m_cache.m_pCacheDP->AddEvent(pEvent) );
	}
	else if (m_cache.m_eCacheMode == xtpCalendarDPCacheModeOnClear)
	{
		ASSERT(m_cache.m_pCacheDP);
		//m_cache.m_pCacheDP->RemoveAllEvents();
		m_cache.SafeRemoveAll();
	}

	//---------------------------------------------------------------------------
	SendNotification(XTP_NC_CALENDAREVENTWASADDED, (WPARAM)pEvent->GetEventID(), (LPARAM)pEvent);
	if (bSendPatternAddedEvent)
	{
		SendNotification(XTP_NC_CALENDARPATTERNWASADDED, dwPatternID, 0);
	}

	return TRUE;
}

BOOL CXTPCalendarData::ChangeEvent(CXTPCalendarEvent* pEvent)
{
	if (!IsOpen())
	{
		return FALSE;
	}

	if (!pEvent)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pEvent->ClearIntermediateData();

	COleDateTime dtNow = CXTPCalendarUtils::GetCurrentTime();
	pEvent->SetLastModificationTime(dtNow);

	DWORD dwEventID = pEvent->GetEventID();
	ASSERT(dwEventID != XTP_CALENDAR_UNKNOWN_EVENT_ID);

	//- new recurrence state ---------------------------
	CXTPCalendarRecurrencePatternPtr ptrPattern_new;

	int nRState_new = pEvent->GetRecurrenceState();

	// - Add/Update/Remove Recurrence Exception --------------------
	if (nRState_new == xtpCalendarRecurrenceException ||
		nRState_new == xtpCalendarRecurrenceOccurrence)
	{
		return _ChangeRExceptionOccurrence_nf(pEvent, FALSE);
	}
	//===========================================================================

	//CXTPCalendarEventPtr ptrEventPrev = _GetEvent_UseCache(dwEventID);
	CXTPCalendarEventPtr ptrEventPrev = GetEvent(dwEventID);
	if (!ptrEventPrev)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//************************************************************
	CXTPCalendarRecurrencePatternPtr ptrPattern_prev;

	BOOL bSendPatternAddedEvent = FALSE;
	BOOL bSendPatternChangedEvent = FALSE;
	BOOL bSendPatternDeletedEvent = FALSE;
	DWORD dwPatternID = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;

	//- previous recurrence state ---------------------
	int nRState_prev = ptrEventPrev->GetRecurrenceState();

	//=== Update Event Recurrence Properties. IF NEED.
	//
	// 1.Event become recurrence
	if (nRState_prev == xtpCalendarRecurrenceNotRecurring &&
		nRState_new == xtpCalendarRecurrenceMaster)
	{
		ptrPattern_new.SetPtr(pEvent->GetRPatternRef(), TRUE);

//      ptrPattern_new->ResetReminderForOccFromThePast(pEvent);

		if (!_AddRPatternWithExceptions(ptrPattern_new))
		{
			ASSERT(FALSE);
			return FALSE;
		}

		dwPatternID = ptrPattern_new->GetPatternID();
		ASSERT(dwPatternID != XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID);

		pEvent->SetRecurrencePatternID(dwPatternID);

		bSendPatternAddedEvent = TRUE;
	}
	// 2.Event become Not Recurrence
	else if (nRState_prev == xtpCalendarRecurrenceMaster &&
		nRState_new == xtpCalendarRecurrenceNotRecurring)
	{
		ptrPattern_prev.SetPtr(ptrEventPrev->GetRPatternRef(), TRUE);

		dwPatternID = ptrPattern_prev->GetPatternID();
		bSendPatternDeletedEvent = TRUE;

		if (!_RemoveRPatternWithExceptions(ptrPattern_prev))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	// 3. Update Recurrence Event
	else if (nRState_prev == xtpCalendarRecurrenceMaster &&
		nRState_new == xtpCalendarRecurrenceMaster)
	{
		ptrPattern_new.SetPtr(pEvent->GetRPatternRef(), TRUE);

		//ptrPattern_new->ResetReminderForOccFromThePast(pEvent);

		if (!_ChangeRPatternWithExceptions(ptrPattern_new))
		{
			DBG_DATA_ASSERT(FALSE);
			return FALSE;
		}
		dwPatternID = ptrPattern_new->GetPatternID();
		bSendPatternChangedEvent = TRUE;
	}

	// Update Event properties
	BOOL bRes = DoUpdate_Event(pEvent);
	DBG_DATA_ASSERT(bRes);
	if (!bRes)
	{
		return FALSE;
	}

	//---------------------------------------------------------------------------
	if (m_cache.m_eCacheMode == xtpCalendarDPCacheModeOnRepeat)
	{
		ASSERT(m_cache.m_pCacheDP);
		if (m_cache.IsEventInCache(pEvent))
		{
			if (m_cache.m_pCacheDP) VERIFY( m_cache.m_pCacheDP->ChangeEvent(pEvent) );
		}
	}
	else if (m_cache.m_eCacheMode == xtpCalendarDPCacheModeOnClear)
	{
		ASSERT(m_cache.m_pCacheDP);
		//m_cache.m_pCacheDP->RemoveAllEvents();
		m_cache.SafeRemoveAll();
	}
	//---------------------------------------------------------------------------

	SendNotification(XTP_NC_CALENDAREVENTWASCHANGED, (WPARAM)pEvent->GetEventID(), (LPARAM)pEvent);

	if (bSendPatternAddedEvent)
	{
		SendNotification(XTP_NC_CALENDARPATTERNWASADDED, dwPatternID, 0);
	}
	else if (bSendPatternChangedEvent)
	{
		SendNotification(XTP_NC_CALENDARPATTERNWASCHANGED, dwPatternID, 0);
	}
	else if (bSendPatternDeletedEvent)
	{
		SendNotification(XTP_NC_CALENDARPATTERNWASDELETED, dwPatternID, 0);
	}

	return TRUE;
}

BOOL CXTPCalendarData::DeleteEvent(CXTPCalendarEvent* pEvent)
{
	if (!IsOpen())
	{
		return FALSE;
	}
	if (!pEvent)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	DWORD dwEventID = pEvent->GetEventID();
	ASSERT(dwEventID != XTP_CALENDAR_UNKNOWN_EVENT_ID);

	BOOL bRes = TRUE;
	BOOL bSendPatternDeletedEvent = FALSE;
	DWORD dwPatternID = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;

	//- recurrence state ---------------------
	int nRState = pEvent->GetRecurrenceState();

	//1. remove occurrence (set deleted property to exception)
	if (nRState == xtpCalendarRecurrenceOccurrence || nRState == xtpCalendarRecurrenceException)
	{
		CXTPCalendarEventPtr ptrException = pEvent->CloneEvent();
		if (!ptrException)
		{
			return FALSE;
		}

		if (nRState == xtpCalendarRecurrenceOccurrence)
		{
			// make exception from Occurrence
			ptrException->MakeAsRException();
		}
		ptrException->SetRExceptionDeleted(TRUE);

		return _ChangeRExceptionOccurrence_nf(ptrException, TRUE);
	}
	else
	{
		if (!DoDelete_Event(pEvent))
		{
			bRes = FALSE;
		}

		//-----------------------------------------------------------------
		if (nRState == xtpCalendarRecurrenceMaster)
		{
			dwPatternID = pEvent->GetRecurrencePatternID();

			CXTPCalendarRecurrencePattern* pPattern = pEvent->GetRPatternRef();
			if (!_RemoveRPatternWithExceptions(pPattern))
			{
				bRes = FALSE;
			}
			bSendPatternDeletedEvent = bRes;
		}
	}

	//---------------------------------------------------------------------------
	if (m_cache.m_eCacheMode == xtpCalendarDPCacheModeOnRepeat)
	{
		ASSERT(m_cache.m_pCacheDP);
		if (m_cache.IsEventInCache(pEvent))
		{
			if (m_cache.m_pCacheDP) VERIFY( m_cache.m_pCacheDP->DeleteEvent(pEvent) );
		}
	}
	else if (m_cache.m_eCacheMode == xtpCalendarDPCacheModeOnClear)
	{
		ASSERT(m_cache.m_pCacheDP);
		//m_cache.m_pCacheDP->RemoveAllEvents();
		m_cache.SafeRemoveAll();
	}
	//---------------------------------------------------------------------------

	if (bRes)
	{
		SendNotification(XTP_NC_CALENDAREVENTWASDELETED, (WPARAM)pEvent->GetEventID(), (LPARAM)pEvent);
	}
	if (bSendPatternDeletedEvent)
	{
		SendNotification(XTP_NC_CALENDARPATTERNWASDELETED, dwPatternID, 0);
	}

	return bRes;
}

CXTPCalendarEventPtr CXTPCalendarData::GetEvent(DWORD dwEventID)
{
	if (!IsOpen())
	{
		return NULL;
	}

	CXTPCalendarEventPtr ptrEvent;
	if ((m_cache.m_eCacheMode & xtpCalendarDPCacheModeOnMask))
	{
		ASSERT(m_cache.m_pCacheDP);
		if (m_cache.m_pCacheDP) ptrEvent = m_cache.m_pCacheDP->GetEvent(dwEventID);
	}

	if (!ptrEvent)
	{
		ptrEvent = _GetEvent_raw(dwEventID);
		if (!ptrEvent)
			return NULL;

		if (ptrEvent->GetRecurrenceState() != xtpCalendarRecurrenceNotRecurring &&
			ptrEvent->GetRecurrenceState() != xtpCalendarRecurrenceMaster)
		{
			return NULL;
		}

		if (m_cache.m_eCacheMode & xtpCalendarDPCacheModeOnMask)
		{
			ASSERT(m_cache.m_pCacheDP);
			m_cache.AddToCacheIfNeed(ptrEvent);
		}
	}

	return ptrEvent;
}

BOOL CXTPCalendarData::_PostProcessRecurrenceIfNeed(CXTPCalendarEvent* pEvent)
{
	COleVariant varRState;
	COleVariant varRPatternID;

	BOOL bRState_prop = pEvent->GetCustomProperties()->GetProperty(cszProcess_RecurrenceState, varRState);
	BOOL bRPatternID_prop = pEvent->GetCustomProperties()->GetProperty(cszProcess_RecurrencePatternID, varRPatternID);

	pEvent->GetCustomProperties()->RemoveProperty(cszProcess_RecurrenceState);
	pEvent->GetCustomProperties()->RemoveProperty(cszProcess_RecurrencePatternID);

	if (!bRState_prop)
	{
		return FALSE;
	}

	int nRState = VAR2long(varRState);
	if (xtpCalendarRecurrenceMaster == nRState)
	{
		if (!bRPatternID_prop)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		DWORD dwRPatternID = (DWORD)VAR2long(varRPatternID);

		CXTPCalendarRecurrencePatternPtr ptrPattern = _GetRecurrencePattern_raw(dwRPatternID);
		if (!ptrPattern)
		{
			ASSERT(FALSE);
			return FALSE;
		}

		VERIFY( pEvent->MakeEventAsRecurrence() );
		pEvent->SetRecurrencePatternID(dwRPatternID);
		VERIFY( pEvent->UpdateRecurrence(ptrPattern) );

		return TRUE;
	}
	else if (xtpCalendarRecurrenceException == nRState)
	{
		DWORD dwRPatternID = bRPatternID_prop ? (DWORD)VAR2long(varRPatternID) : 0;
		pEvent->SetRecurrencePatternID(dwRPatternID);

		if (pEvent->GetRecurrenceState() != xtpCalendarRecurrenceException)
		{
			pEvent->MakeAsRException(dwRPatternID);
		}
	}
	return FALSE;
}

void CXTPCalendarData::_PostProcessRecurrenceIfNeed(CXTPCalendarEvents* pEvents)
{
	int nCount = pEvents ? pEvents->GetCount() : 0;
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvent = pEvents->GetAt(i, FALSE);
		ASSERT(pEvent);
		if (pEvent)
		{
			_PostProcessRecurrenceIfNeed(pEvent);
		}
	}
}

CXTPCalendarEventPtr CXTPCalendarData::_GetEvent_raw(DWORD dwEventID)
{
	if (!IsOpen())
	{
		return NULL;
	}

	CXTPCalendarEventPtr ptrEvent = DoRead_Event(dwEventID);
	if (!ptrEvent)
	{
		return NULL;
	}

	_PostProcessRecurrenceIfNeed(ptrEvent);

	return ptrEvent;
}

CXTPCalendarRecurrencePatternPtr CXTPCalendarData::_GetRecurrencePattern_raw(DWORD dwPatternID)
{
	CXTPCalendarRecurrencePatternPtr ptrPattern = DoRead_RPattern(dwPatternID);

	if (ptrPattern)
	{
		VERIFY( ptrPattern->GetOccReminders()->Load(ptrPattern->GetCustomProperties()) );
		ptrPattern->GetOccReminders()->ClearProperties(ptrPattern->GetCustomProperties());
	}
	return ptrPattern;
}

CXTPCalendarRecurrencePatternPtr CXTPCalendarData::GetRecurrencePattern(DWORD dwPatternID)
{
	if (!IsOpen())
	{
		return NULL;
	}

	CXTPCalendarRecurrencePatternPtr ptrPattern;
	if ((m_cache.m_eCacheMode & xtpCalendarDPCacheModeOnMask))
	{
		ASSERT(m_cache.m_pCacheDP);
		if (m_cache.m_pCacheDP) ptrPattern = m_cache.m_pCacheDP->GetRecurrencePattern(dwPatternID);
	}

	if (!ptrPattern)
	{
		ptrPattern = _GetRecurrencePattern_raw(dwPatternID);

		if (ptrPattern && (m_cache.m_eCacheMode & xtpCalendarDPCacheModeOnMask))
		{
			m_cache._AddToCacheIfNeed(ptrPattern);
		}
	}

	return ptrPattern;
}

BOOL CXTPCalendarData::_AddRPatternWithExceptions(CXTPCalendarRecurrencePattern* pPattern)
{
	if (!IsOpen())
	{
		return FALSE;
	}
	if (!pPattern)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	VERIFY( pPattern->GetOccReminders()->Save(pPattern->GetCustomProperties(), pPattern) );

	DWORD dwNewPatternID = XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID;
	BOOL bRes = DoCreate_RPattern(pPattern, dwNewPatternID);
	DBG_DATA_ASSERT(bRes);

	pPattern->GetOccReminders()->ClearProperties(pPattern->GetCustomProperties());

	if (!bRes)
	{
		return FALSE;
	}
	ASSERT(dwNewPatternID != XTP_CALENDAR_UNKNOWN_RECURRENCE_PATTERN_ID);
	pPattern->SetPatternID(dwNewPatternID);

	BOOL bResult = TRUE;
	//-- Process exceptions ---------------------------------------------------
	CXTPCalendarEventsPtr prtExceptions = pPattern->GetExceptions();
	pPattern->RemoveAllExceptions();

	int nCount = prtExceptions ? prtExceptions->GetCount() : 0;
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pExc = prtExceptions->GetAt(i);
		ASSERT(pExc);
		if (!pExc)
			continue;

		DWORD dwNewEventID = XTP_CALENDAR_UNKNOWN_EVENT_ID;
		if (!DoCreate_Event(pExc, dwNewEventID))
		{
			bResult = FALSE;
			continue;
		}
		ASSERT(dwNewEventID != XTP_CALENDAR_UNKNOWN_EVENT_ID);
		pExc->SetEventID(dwNewEventID);

		pPattern->SetException(pExc);
	}
	//-------------------------------------------
	return bResult;
}

BOOL CXTPCalendarData::_RemoveRPatternWithExceptions(CXTPCalendarRecurrencePattern* pPattern)
{
	if (!IsOpen())
	{
		return FALSE;
	}
	if (!pPattern)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bResult = TRUE;
	//-- Process exceptions ---------------------------------------------------
	CXTPCalendarEventsPtr prtExceptions = pPattern->GetExceptions();

	int nCount = prtExceptions ? prtExceptions->GetCount() : 0;
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent *pExc = prtExceptions->GetAt(i);
		ASSERT(pExc);
		if (!DoDelete_Event(pExc))
		{
			bResult = FALSE;
		}
	}
	//-------------------------------------------
	if (!DoDelete_RPattern(pPattern))
	{
		return FALSE;
	}

	//-------------------------------------------
	return bResult;
}

BOOL CXTPCalendarData::_ChangeRPatternWithExceptions(CXTPCalendarRecurrencePattern* pPattern)
{
	if (!IsOpen())
	{
		return FALSE;
	}

	if (!pPattern)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	DWORD dwPatternID = pPattern->GetPatternID();
	//CXTPCalendarRecurrencePatternPtr ptrPatternPrev = _GetRecurrencePattern_UseCache(dwPatternID);
	CXTPCalendarRecurrencePatternPtr ptrPatternPrev = GetRecurrencePattern(dwPatternID);

	if (!ptrPatternPrev)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//-------------------------------------
	VERIFY( pPattern->GetOccReminders()->Save(pPattern->GetCustomProperties(), pPattern) );

	BOOL bRes = DoUpdate_RPattern(pPattern);

	pPattern->GetOccReminders()->ClearProperties(pPattern->GetCustomProperties());

	if (!bRes)
	{
		return FALSE;
	}
	//-------------------------------------

	BOOL bResult = TRUE;
	//-- Process exceptions ---------------------------------------------------
	CXTPCalendarEventsPtr prtExceptions_prev = ptrPatternPrev->GetExceptions();
	CXTPCalendarEventsPtr prtExceptions_new = pPattern->GetExceptions();

	//-- find deleted exceptions and remove them from DB
	int nCount = (prtExceptions_prev && prtExceptions_new) ? prtExceptions_prev->GetCount() : 0;
	int i;
	for (i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pExc = prtExceptions_prev->GetAt(i);
		DWORD dwExcID = pExc ? pExc->GetEventID() : XTP_CALENDAR_UNKNOWN_EVENT_ID;
		int nFIndex = prtExceptions_new->Find(dwExcID);
		if (nFIndex < 0)
		{
			if (!DoDelete_Event(pExc))
			{
				bResult = FALSE;
			}
		}
	}

	//-- update existing or add new exceptions (int DB)
	nCount = (prtExceptions_prev && prtExceptions_new) ? prtExceptions_new->GetCount() : 0;
	for (i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pExc = prtExceptions_new->GetAt(i);
		if (!pExc)
		{
			ASSERT(FALSE);
			continue;
		}

		DWORD dwExcID = pExc->GetEventID();

		int nFIndex = prtExceptions_prev->Find(dwExcID);

		if (nFIndex < 0)
		{
			// Add new exception
			DWORD dwNewEventID = XTP_CALENDAR_UNKNOWN_EVENT_ID;
			if (!DoCreate_Event(pExc, dwNewEventID))
			{
				bResult = FALSE;
				continue;
			}
			ASSERT(dwNewEventID != XTP_CALENDAR_UNKNOWN_EVENT_ID);
			pExc->SetEventID(dwNewEventID);

			VERIFY( pPattern->RemoveException(pExc) );
			if (!pPattern->SetException(pExc))
			{
				bResult = FALSE;
			}
		}
		else
		{
			// Update existing exception
			if (!DoUpdate_Event(pExc))
			{
				bResult = FALSE;
			}
		}
	}

	return bResult;
}

BOOL CXTPCalendarData::_ChangeRExceptionOccurrence_nf(CXTPCalendarEvent* pExcOcc, BOOL bSend_EventDeleted)
{
	if (!pExcOcc)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pExcOcc->ClearIntermediateData();

	DWORD dwEventID = pExcOcc->GetEventID();
	ASSERT(dwEventID != XTP_CALENDAR_UNKNOWN_EVENT_ID);

	//- new recurrence state ---------------------------
	CXTPCalendarRecurrencePatternPtr ptrPattern;

	int nRState_new = pExcOcc->GetRecurrenceState();

	// - Add/Update/Remove Recurrence Exception --------------------
	if (nRState_new != xtpCalendarRecurrenceException &&
		nRState_new != xtpCalendarRecurrenceOccurrence)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	DWORD dwPatternID_new = pExcOcc->GetRecurrencePatternID();
	//ptrPattern = _GetRecurrencePattern_UseCache(dwPatternID_new);
	ptrPattern = GetRecurrencePattern(dwPatternID_new);
	if (!ptrPattern)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	DWORD dwMasterEventID = ptrPattern->GetMasterEventID();
	//CXTPCalendarEventPtr ptrMasterEvent = _GetEvent_UseCache(dwMasterEventID);
	CXTPCalendarEventPtr ptrMasterEvent = GetEvent(dwMasterEventID);
	if (!ptrMasterEvent)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	ptrPattern.SetPtr(ptrMasterEvent->GetRPatternRef(), TRUE);

	if (!ptrPattern)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bSendPatternChangedEvent = FALSE;
	BOOL bRes = FALSE;
	if (nRState_new == xtpCalendarRecurrenceException)
	{
		bRes = ptrPattern->SetException(pExcOcc, ptrMasterEvent);
	}
	else if (nRState_new == xtpCalendarRecurrenceOccurrence)
	{
		bRes = ptrPattern->RemoveException(pExcOcc, ptrMasterEvent);
	}
	else
	{
		ASSERT(FALSE);
	}

	if (bRes)
	{
		bRes = _ChangeRPatternWithExceptions(ptrPattern);
		bSendPatternChangedEvent = bRes;
	}
	if (bRes)
	{
		bRes = DoUpdate_Event(ptrMasterEvent);
	}

	//---------------------------------------------------------------------------
	if (m_cache.m_eCacheMode == xtpCalendarDPCacheModeOnRepeat)
	{
		ASSERT(m_cache.m_pCacheDP);
		if (m_cache.IsEventInCache(ptrMasterEvent))
		{
			if (m_cache.m_pCacheDP) VERIFY( m_cache.m_pCacheDP->ChangeEvent(ptrMasterEvent) );
		}
	}
	else if (m_cache.m_eCacheMode == xtpCalendarDPCacheModeOnClear)
	{
		ASSERT(m_cache.m_pCacheDP);
		//m_cache.m_pCacheDP->RemoveAllEvents();
		m_cache.SafeRemoveAll();
	}
	//---------------------------------------------------------------------------

	if (bRes)
	{
		XTP_NOTIFY_CODE nfCode = bSend_EventDeleted ?
			XTP_NC_CALENDAREVENTWASDELETED : XTP_NC_CALENDAREVENTWASCHANGED;
		SendNotification(nfCode, (WPARAM)pExcOcc->GetEventID(), (LPARAM)pExcOcc);

	}
	if (bSendPatternChangedEvent)
	{
		SendNotification(XTP_NC_CALENDARPATTERNWASCHANGED, ptrPattern->GetPatternID(), 0);
	}

	return bRes;
}

void CXTPCalendarData::_PostProcessOccurrencesFromMaster(COleDateTime dtDay, CXTPCalendarEvents* pEvents)
{
	_PostProcessOccurrencesFromMaster2(dtDay, dtDay, pEvents);
}

void CXTPCalendarData::_PostProcessOccurrencesFromMaster2(COleDateTime dtDayFrom, COleDateTime dtDayTo, CXTPCalendarEvents* pEvents)
{
	if (!pEvents)
	{
		return;
	}
	int nCount = pEvents->GetCount();
	for (int i = nCount - 1; i >= 0; i--)
	{
		CXTPCalendarEventPtr ptrEvent = pEvents->GetAt(i, TRUE);
		if (!ptrEvent)
		{
			ASSERT(FALSE);
			continue;
		}

		int nRState = ptrEvent->GetRecurrenceState();
		if (xtpCalendarRecurrenceMaster == nRState)
		{
			pEvents->RemoveAt(i);

			CXTPCalendarRecurrencePattern* pPattern = ptrEvent->GetRPatternRef();
			if (!pPattern)
			{
				ASSERT(FALSE);
				continue;
			}

			VERIFY(pPattern->GetOccurrences(pEvents, dtDayFrom, dtDayTo, ptrEvent));
		}
	}
}

void CXTPCalendarData::_FilterDayEventsInstancesByEndTime(COleDateTime dtDay,
													CXTPCalendarEvents* pEvents)
{
	ASSERT(pEvents);
	if (!pEvents)
	{
		return;
	}
	dtDay = CXTPCalendarUtils::ResetTime(dtDay);

	// filter events;
	int nCount = pEvents->GetCount();
	for (int i = nCount-1; i >= 0; i--)
	{
		CXTPCalendarEvent* pEvent = pEvents->GetAt(i);
		if (!pEvent)
		{
			ASSERT(FALSE);
			continue;
		}
		if (pEvent->GetRecurrenceState() == xtpCalendarRecurrenceMaster)
		{
			continue;
		}

		if (CXTPCalendarUtils::ResetTime(pEvent->GetStartTime()) < dtDay &&
			CXTPCalendarUtils::IsEqual(pEvent->GetEndTime(), dtDay) )
		{
			pEvents->RemoveAt(i);
		}
	}
}


void CXTPCalendarData::SendNotification(XTP_NOTIFY_CODE EventCode, WPARAM wParam , LPARAM lParam)
{
	if (m_pConnection && !m_bDisableNotificationsSending)
	{
		m_pConnection->SendEvent(EventCode, wParam, lParam);
	}
}

CXTPCalendarEventsPtr CXTPCalendarData::GetUpcomingEvents(COleDateTime dtFrom, COleDateTimeSpan spPeriod)
{
	CXTPCalendarEventsPtr ptrEvents = DoGetUpcomingEvents(dtFrom, spPeriod);

	_PostProcessRecurrenceIfNeed(ptrEvents);
	return ptrEvents;
}

CXTPCalendarEventsPtr CXTPCalendarData::GetAllEvents_raw()
{
	CXTPCalendarEventsPtr ptrEvents = DoGetAllEvents_raw();

	_PostProcessRecurrenceIfNeed(ptrEvents);
	return ptrEvents;
}

void CXTPCalendarData::SetOptionsToUpdate(CXTPCalendarOptions* pOptions)
{
	CMDTARGET_ADDREF(pOptions);
	CMDTARGET_RELEASE(m_pCalendarOptions);
	m_pCalendarOptions = pOptions;
}

CXTPCalendarEventsPtr CXTPCalendarData::DoGetAllEvents_raw()
{
	return NULL;
}

CXTPCalendarEventsPtr CXTPCalendarData::DoGetUpcomingEvents(
							COleDateTime dtFrom, COleDateTimeSpan spPeriod)
{
	UNREFERENCED_PARAMETER(dtFrom); UNREFERENCED_PARAMETER(spPeriod);
	//return DoRetrieveAllEvents_raw();
	return NULL;
}

CXTPCalendarEventsPtr CXTPCalendarData::DoRetrieveDayEvents(COleDateTime dtDay)
{
	UNREFERENCED_PARAMETER(dtDay);
	return NULL;
}

void CXTPCalendarData::DoRemoveAllEvents()
{
}

CXTPCalendarEventPtr CXTPCalendarData::DoRead_Event(DWORD dwEventID)
{
	UNREFERENCED_PARAMETER(dwEventID);
	return NULL;
}

CXTPCalendarRecurrencePatternPtr CXTPCalendarData::DoRead_RPattern(DWORD dwPatternID)
{
	UNREFERENCED_PARAMETER(dwPatternID);
	return NULL;
}

BOOL CXTPCalendarData::DoCreate_Event(CXTPCalendarEvent* pEvent, DWORD& rdwNewEventID)
{
	UNREFERENCED_PARAMETER(pEvent);
	UNREFERENCED_PARAMETER(rdwNewEventID);
	return TRUE;
}

BOOL CXTPCalendarData::DoUpdate_Event(CXTPCalendarEvent* pEvent)
{
	UNREFERENCED_PARAMETER(pEvent);
	return TRUE;
}

BOOL CXTPCalendarData::DoDelete_Event(CXTPCalendarEvent* pEvent)
{
	UNREFERENCED_PARAMETER(pEvent);
	return TRUE;
}

BOOL CXTPCalendarData::DoCreate_RPattern(CXTPCalendarRecurrencePattern* pPattern, DWORD& rdwNewPatternID)
{
	UNREFERENCED_PARAMETER(pPattern);
	UNREFERENCED_PARAMETER(rdwNewPatternID);
	return TRUE;
}

BOOL CXTPCalendarData::DoUpdate_RPattern(CXTPCalendarRecurrencePattern* pPattern)
{
	UNREFERENCED_PARAMETER(pPattern);
	return TRUE;
}

BOOL CXTPCalendarData::DoDelete_RPattern(CXTPCalendarRecurrencePattern* pPattern)
{
	UNREFERENCED_PARAMETER(pPattern);
	return TRUE;
}


CString CXTPCalendarData::DataSourceFromConStr(LPCTSTR lpszConnectionString)
{
	ASSERT(lpszConnectionString);

	if (!lpszConnectionString || 0 == _tcslen(lpszConnectionString))
		return _T("");

	CString strConnStr_lower = lpszConnectionString;
	strConnStr_lower.MakeLower();

	int nIndex = strConnStr_lower.Find(_T("data source="));
	if (nIndex != -1)
	{
		CString strDataSource = strConnStr_lower.Mid(nIndex + 12);
		if (strDataSource.IsEmpty())
			return _T("");

		if ((strDataSource[0] == _T('\'') || strDataSource[0] == _T('"')) && strDataSource.GetLength() > 2)
		{
			TCHAR chQuot = strDataSource[0];

			strDataSource.Delete(0);

			int nQuot2Idx = strDataSource.Find(chQuot, 0);

			if (nQuot2Idx >= 0)
			{
				strDataSource.Delete(nQuot2Idx, strDataSource.GetLength() - nQuot2Idx);
			}
		}

		return strDataSource;
	}

	return strConnStr_lower;
}

XTPCalendarDataProvider CXTPCalendarData::DataProviderTypeFromConStr(
																		LPCTSTR lpszConnectionString,
																		XTPCalendarDataProvider eDPDefault)
{
	ASSERT(lpszConnectionString);

	if (!lpszConnectionString)
	{
		return eDPDefault;
	}

	CString strConnStr_lower(lpszConnectionString);
	strConnStr_lower.MakeLower();

	BOOL bProviderCustom = strConnStr_lower.Find(_T("provider=custom")) != -1;
	if (bProviderCustom)
	{
		return xtpCalendarDataProviderCustom;
	}

	BOOL bProviderMAPI   = strConnStr_lower.Find(_T("provider=mapi"))   != -1;
	if (bProviderMAPI)
	{
		return xtpCalendarDataProviderMAPI;
	}

	BOOL bProviderXML   = strConnStr_lower.Find(_T("provider=xml")) != -1;
	if (bProviderXML)
	{
		return xtpCalendarDataProviderMemory;
	}

	BOOL bProvider = strConnStr_lower.Find(_T("provider=")) != -1;
	BOOL bDSN      = strConnStr_lower.Find(_T("dsn="))      != -1;
	if (bProvider || bDSN)
	{
		return xtpCalendarDataProviderDatabase;
	}

	return eDPDefault;
}

CXTPCalendarData* CXTPCalendarData::CreateDataProvider(LPCTSTR lpszConnectionString)
{
	ASSERT(lpszConnectionString);

	if (!lpszConnectionString)
	{
		return NULL;
	}

	XTPCalendarDataProvider eDPType = DataProviderTypeFromConStr(lpszConnectionString, xtpCalendarDataProviderMemory);

	CXTPCalendarData* pDataProvider = CreateDataProvider(eDPType);

	if (pDataProvider)
	{
		pDataProvider->SetConnectionString(lpszConnectionString);
	}
	return pDataProvider;
}


CXTPCalendarData* CXTPCalendarData::CreateDataProvider(XTPCalendarDataProvider eDataProvider)
{
	CXTPCalendarData* pDataProvider = NULL;
	switch (eDataProvider)
	{
	case xtpCalendarDataProviderMemory:
		pDataProvider = new CXTPCalendarMemoryDataProvider();
		break;
	case xtpCalendarDataProviderDatabase:
		pDataProvider = new CXTPCalendarDatabaseDataProvider();
		break;
	case xtpCalendarDataProviderMAPI:
		pDataProvider = new CXTPCalendarMAPIDataProvider();
		break;
	case xtpCalendarDataProviderCustom:
		pDataProvider = new CXTPCalendarCustomDataProvider();
		break;
	default:
		ASSERT(FALSE);
	}

	return pDataProvider;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarData, CCmdTarget)
	DISP_FUNCTION_ID(CXTPCalendarData, "CreateEvent", 1, OleCreateEvent, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarData, "CreateEventEx", 51, OleCreateEventEx, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarData, "CreateRecurrencePattern", 52, OleCreateRecurrencePattern, VT_DISPATCH, VTS_I4)

	DISP_FUNCTION_ID(CXTPCalendarData, "ChangeEvent", 2, OleChangeEvent, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarData, "AddEvent", 3, OleAddEvent, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarData, "DeleteEvent", 4, OleDeleteEvent, VT_EMPTY, VTS_DISPATCH)

	DISP_FUNCTION_ID(CXTPCalendarData, "Open", 5, OleOpen, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarData, "Create", 6, OleCreate, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarData, "Close", 7, OleClose, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarData, "Save", 8, OleSave, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarData, "RemoveAllEvents", 9, OleRemoveAllEvents, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarData, "RetrieveDayEvents", 10, OleRetrieveDayEvents, VT_DISPATCH, VTS_DATE)

	DISP_FUNCTION_ID(CXTPCalendarData, "GetEvent", 11, OleGetEvent, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarData, "GetRecurrencePattern", 12, OleGetRecurrencePattern, VT_DISPATCH, VTS_I4)

	DISP_FUNCTION_ID(CXTPCalendarData, "LabelList", 13, OleGetLabelList, VT_DISPATCH, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPCalendarData, "CacheMode", 15, OleGetCacheMode, OleSetCacheMode, VT_I4)
	DISP_FUNCTION_ID(CXTPCalendarData, "ClearCache", 14, OleClearCache, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarData, "GetAllEventsRaw", 16, OleGetAllEventsRaw, VT_DISPATCH, VTS_NONE)

	DISP_PROPERTY_ID(CXTPCalendarData, "ConnectionString", 17, m_strConnectionString, VT_BSTR)
	DISP_FUNCTION_ID(CXTPCalendarData, "DataSource", 18, OleGetDataSource, VT_BSTR, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarData, "Schedules", 19, OleGetSchedules, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarData, "IsOpen", 20, IsOpen, VT_BOOL, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarData, "OpenEx", 21, OleOpenEx, VT_BOOL, VTS_DISPATCH)

	DISP_FUNCTION_ID(CXTPCalendarData, "EventCategories", 22, OleGetEventCategories, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPCalendarData, "OwnershipMode", 23, OleGetOwnershipMode, OleSetOwnershipMode, VT_BOOL)
END_DISPATCH_MAP()

// {6FED5027-B4A5-492f-ADA0-56E1B0772A53}
static const GUID IID_ICalendarData =
{ 0x6fed5027, 0xb4a5, 0x492f, { 0xad, 0xa0, 0x56, 0xe1, 0xb0, 0x77, 0x2a, 0x53 } };


BEGIN_INTERFACE_MAP(CXTPCalendarData, CCmdTarget)
	INTERFACE_PART(CXTPCalendarData, IID_ICalendarData, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarData, IID_ICalendarData)

LPDISPATCH CXTPCalendarData::OleCreateEvent()
{
	CXTPCalendarEventPtr ptrEvent = CreateNewEvent();
	return ptrEvent ? ptrEvent->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarData::OleCreateEventEx(long EventID)
{
	CXTPCalendarEventPtr ptrEvent = CreateNewEvent((DWORD)EventID);
	return ptrEvent ? ptrEvent->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarData::OleCreateRecurrencePattern(long PatternID)
{
	CXTPCalendarRecurrencePatternPtr ptrPattern = CreateNewRecurrencePattern((DWORD)PatternID);
	return ptrPattern ? ptrPattern->GetIDispatch(TRUE) : NULL;
}

void CXTPCalendarData::OleRemoveAllEvents()
{
	RemoveAllEvents();
}

LPDISPATCH CXTPCalendarData::OleRetrieveDayEvents(DATE dt)
{
	CXTPCalendarEventsPtr ptr = RetrieveDayEvents(dt);
	if (ptr)
	{
		return ptr->GetIDispatch(TRUE);
	}

	CXTPCalendarEvents* ptrEvents = new CXTPCalendarEvents();
	return ptrEvents ? ptrEvents->GetIDispatch(FALSE) : NULL;
}

LPDISPATCH CXTPCalendarData::OleGetAllEventsRaw()
{
	CXTPCalendarEventsPtr ptr = GetAllEvents_raw();
	if (ptr)
	{
		return ptr->GetIDispatch(TRUE);
	}

	CXTPCalendarEvents* ptrEvents = new CXTPCalendarEvents();
	return ptrEvents ? ptrEvents->GetIDispatch(FALSE) : NULL;
}

void CXTPCalendarData::OleAddEvent(LPDISPATCH Event)
{
	CXTPCalendarEvent* pEvent = DYNAMIC_DOWNCAST(CXTPCalendarEvent, CXTPCalendarEvent::FromIDispatch(Event));
	if (!pEvent)
		AfxThrowOleException(E_INVALIDARG);

	AddEvent(pEvent);
}

void CXTPCalendarData::OleChangeEvent(LPDISPATCH Event)
{
	CXTPCalendarEvent* pEvent = DYNAMIC_DOWNCAST(CXTPCalendarEvent, CXTPCalendarEvent::FromIDispatch(Event));
	if (!pEvent)
		AfxThrowOleException(E_INVALIDARG);

	ChangeEvent(pEvent);
}

BOOL CXTPCalendarData::OleOpen()
{
	return Open();
}

BOOL CXTPCalendarData::OleCreate()
{
	return Create();
}

void CXTPCalendarData::OleClose()
{
	Close();
}

void CXTPCalendarData::OleSave()
{
	Save();
}

BSTR CXTPCalendarData::OleGetDataSource()
{
	return GetDataSource().AllocSysString();
}

void CXTPCalendarData::OleDeleteEvent(LPDISPATCH Event)
{
	CXTPCalendarEvent* pEvent = DYNAMIC_DOWNCAST(CXTPCalendarEvent, CXTPCalendarEvent::FromIDispatch(Event));
	if (!pEvent)
		AfxThrowOleException(E_INVALIDARG);

	DeleteEvent(pEvent);
}

LPDISPATCH CXTPCalendarData::OleGetEvent(long EventID)
{
	CXTPCalendarEventPtr ptrEvent = GetEvent((DWORD)EventID);
	return ptrEvent ? ptrEvent->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarData::OleGetRecurrencePattern(long PatternID)
{
	CXTPCalendarRecurrencePatternPtr ptrRecurrencePattern = GetRecurrencePattern((DWORD)PatternID);
	return ptrRecurrencePattern ? ptrRecurrencePattern->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarData::OleGetLabelList()
{
	if (GetLabelList())
	{
		return GetLabelList()->GetIDispatch(TRUE);
	}

	return NULL;
}

BOOL CXTPCalendarData::OleGetOwnershipMode()
{
	return m_bOwnershipMode;
}

void CXTPCalendarData::OleSetOwnershipMode(BOOL bMode)
{
	m_bOwnershipMode = bMode;
}

long CXTPCalendarData::OleGetCacheMode()
{
	return (long)GetCacheMode();
}

void CXTPCalendarData::OleSetCacheMode(long eCacheMode)
{
	SetCacheMode(eCacheMode);
}

void CXTPCalendarData::OleClearCache()
{
	ClearCache();
}

LPDISPATCH CXTPCalendarData::OleGetSchedules()
{
	return m_pSchedules ? m_pSchedules->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarData::OleGetEventCategories()
{
	return m_pEventCategories ? m_pEventCategories->GetIDispatch(TRUE) : NULL;
}

BOOL CXTPCalendarData::OleOpenEx(LPDISPATCH pdispADOConnection)
{
	if (!pdispADOConnection)
		AfxThrowOleException(E_INVALIDARG);
	return OpenEx(pdispADOConnection);
}

BOOL CXTPCalendarData::OpenEx(LPDISPATCH pdispADOConnection)
{
	AfxThrowOleException(E_NOTIMPL);
	return FALSE;
}

#endif
