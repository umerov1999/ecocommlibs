// XTPCalendarOptions.cpp: implementation of the CXTPCalendarOptions class.
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarNotifications.h"
#include "XTPCalendarOptions.h"
#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarTimeZoneHelper.h"

#include "XTPCalendarData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// void CXTPCalendarOptions
IMPLEMENT_DYNAMIC(CXTPCalendarOptions, CCmdTarget)

CXTPCalendarOptions::CXTPCalendarOptions()
{
	// default settings
	nWorkWeekMask = xtpCalendarDayMo_Fr;
	nFirstDayOfTheWeek = xtpCalendarDayMonday;
	dtWorkDayStartTime.SetTime(8, 0, 0);
	dtWorkDayEndTime.SetTime(17, 0, 0);

	dtScaleMinTime.SetTime(7, 0, 0);
	dtScaleMaxTime.SetTime(18, 0, 0);

	bEnableInPlaceEditEventSubject_ByF2 = TRUE;
	bEnableInPlaceEditEventSubject_ByMouseClick = TRUE;
	bEnableInPlaceEditEventSubject_ByTab = TRUE;
	bEnableInPlaceEditEventSubject_AfterEventResize = TRUE;

	bEnableInPlaceCreateEvent = TRUE;
	bUseOutlookFontGlyphs = FALSE;

	nDayView_ScaleInterval = 30;
	bDayView_AutoResetBusyFlag = TRUE;

	bDayView_Scale2Visible = FALSE;
	//strDayView_ScaleLabel;
	//strDayView_Scale2Label;
	::ZeroMemory(&tziDayView_Scale2TimeZone, sizeof(tziDayView_Scale2TimeZone));
	VERIFY( ::GetTimeZoneInformation(&tziDayView_Scale2TimeZone) != TIME_ZONE_ID_INVALID);

	nDayView_CurrentTimeMarkVisible = xtpCalendarCurrentTimeMarkVisibleForToday;
	bDayView_TimeScaleShowMinutes = FALSE;

	bMonthView_CompressWeekendDays = TRUE;
	bMonthView_ShowEndDate = FALSE;
	bMonthView_ShowTimeAsClocks = FALSE;
	bMonthView_HideTimes = FALSE;

	bWeekView_ShowEndDate = FALSE;
	bWeekView_ShowTimeAsClocks = FALSE;

	bTimeLineCompact = TRUE;

	bShowAllDayExpandButton = TRUE;

	//dwAdditionalOptions = 0;
	//set of default settings - app can overwrite during initialization
	dwAdditionalOptions =
		xtpCalendarOptMonthViewShowStartTimeAlways |
		xtpCalendarOptMonthViewShowEndTimeAlways |
		xtpCalendarOptWeekViewShowStartTimeAlways |
		xtpCalendarOptWeekViewShowEndTimeAlways |
		//        xtpCalendarOptDayViewNoWordBreak |
		xtpCalendarOptWorkWeekViewShowStartTimeAlways |
		xtpCalendarOptWorkWeekViewShowEndTimeAlways |
		xtpCalendarOptDayViewShowStartTimeAlways |
		xtpCalendarOptDayViewShowEndTimeAlways;


	bEnableAddNewTooltip = TRUE;
	strTooltipAddNewText = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_CLICKTOADD);

	bEnablePrevNextEventButtons = TRUE;

	m_pDataProvider = NULL;

	bHatchAllDayViewEventsBkgnd = TRUE;

	bShowCategoryIcons = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarOptions::~CXTPCalendarOptions()
{
	//CMDTARGET_RELEASE(m_pDataProvider);
}

void CXTPCalendarOptions::DoPropExchange(CXTPPropExchange* pPX)
{
	long nSchema = 7;
	PX_Long(pPX, _T("Version"), nSchema, 7);

	PX_Int(pPX, _T("WorkWeekMask"), nWorkWeekMask, xtpCalendarDayMo_Fr);
	PX_Int(pPX, _T("FirstDayOfTheWeek"), nFirstDayOfTheWeek, xtpCalendarDayMonday);

	PX_DateTime(pPX, _T("WorkDayStartTime"), dtWorkDayStartTime);
	PX_DateTime(pPX, _T("WorkDayEndTime"), dtWorkDayEndTime);

	if (nSchema >= 7)
	{
		PX_DateTime(pPX, _T("ScaleMinTime"), dtScaleMinTime);

		if (!pPX->IsLoading() && dtScaleMaxTime.GetHour() == 0)
			dtScaleMaxTime.SetTime(23, 59, 0);

		PX_DateTime(pPX, _T("dtScaleMaxTime"), dtScaleMaxTime);

		if (pPX->IsLoading() && dtScaleMaxTime.GetHour() == 0)
			dtScaleMaxTime.SetTime(23, 59, 0);
	}

//----------------------------------------
	PX_Bool(pPX, _T("EnableInPlaceEditEventSubject_ByF2"), bEnableInPlaceEditEventSubject_ByF2, TRUE);
	PX_Bool(pPX, _T("EnableInPlaceEditEventSubject_ByMouseClick"), bEnableInPlaceEditEventSubject_ByMouseClick, TRUE);
	PX_Bool(pPX, _T("EnableInPlaceEditEventSubject_ByTab"), bEnableInPlaceEditEventSubject_ByTab, TRUE);
	PX_Bool(pPX, _T("EnableInPlaceEditEventSubject_AfterEventResize"), bEnableInPlaceEditEventSubject_AfterEventResize, TRUE);

	PX_Bool(pPX, _T("EnableInPlaceCreateEvent"), bEnableInPlaceCreateEvent, TRUE);

	if (nSchema >= 2)
	{
		PX_Bool(pPX, _T("UseOutlookFontGlyphs"), bUseOutlookFontGlyphs, FALSE);
	}
	else if (pPX->IsLoading())
	{
		bUseOutlookFontGlyphs = FALSE;
	}

	PX_Bool(pPX, _T("DayView_AutoResetBusyFlag"), bDayView_AutoResetBusyFlag, TRUE);
	PX_Int(pPX, _T("DayView_ScaleInterval"), nDayView_ScaleInterval, 30);
	PX_String(pPX, _T("DayView_ScaleLabel"), strDayView_ScaleLabel, _T(""));
	PX_String(pPX, _T("DayView_Scale2Label"), strDayView_Scale2Label, _T(""));
	PX_Bool(pPX, _T("DayView_Scale2Visible"), bDayView_Scale2Visible, FALSE);

	if (nSchema >= 3)
	{
		PX_Int(pPX, _T("DayView_CurrentTimeMarkVisible"), nDayView_CurrentTimeMarkVisible, xtpCalendarCurrentTimeMarkVisibleForToday);
	}
	else if (pPX->IsLoading())
	{
		nDayView_CurrentTimeMarkVisible = xtpCalendarCurrentTimeMarkVisibleForToday;
	}

	PX_Bool(pPX, _T("DayView_TimeScaleShowMinutes"), bDayView_TimeScaleShowMinutes, FALSE);

	BYTE* pTZIdata = (BYTE*)&tziDayView_Scale2TimeZone;
	DWORD dwBytes = sizeof(tziDayView_Scale2TimeZone);
	PX_Blob(pPX, _T("DayView_Scale2TimeZone"), pTZIdata, dwBytes);

	PX_Bool(pPX, _T("MonthView_CompressWeekendDays"), bMonthView_CompressWeekendDays, TRUE);
	PX_Bool(pPX, _T("MonthView_ShowEndDate"), bMonthView_ShowEndDate, FALSE);
	PX_Bool(pPX, _T("MonthView_ShowTimeAsClocks"), bMonthView_ShowTimeAsClocks, FALSE);

	PX_Bool(pPX, _T("WeekView_ShowEndDate"), bWeekView_ShowEndDate, FALSE);
	PX_Bool(pPX, _T("WeekView_ShowTimeAsClocks"), bWeekView_ShowTimeAsClocks, FALSE);

	if (nSchema >= 4)
	{
		PX_ULong(pPX, _T("AdditionalOptions"), (ULONG&)dwAdditionalOptions, 0);
	}
	else if (pPX->IsLoading())
	{
		dwAdditionalOptions = 0;
	}

	if (nSchema >= 5)
	{
		PX_Bool(pPX, _T("EnableAddNewTooltip"), bEnableAddNewTooltip, TRUE);

		CString strTooltipAddNewTextDef = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_CLICKTOADD);
		PX_String(pPX, _T("TooltipAddNewText"), strTooltipAddNewText, strTooltipAddNewTextDef);
	}

	if (nSchema >= 6)
	{
		PX_Bool(pPX, _T("EnablePrevNextEventButtons"), bEnablePrevNextEventButtons, TRUE);
	}

	if (nSchema >= 7)
	{
		PX_Bool(pPX, _T("CompactTimelineEvents"), bTimeLineCompact, TRUE);
	}

	if (pPX->GetSchema() >= 37)
	{
		PX_Bool(pPX, _T("ShowAllDayExpandButton"), bShowAllDayExpandButton, TRUE);
	}
}

CXTPCalendarTimeZonePtr CXTPCalendarOptions::GetCurrentTimeZoneInfo()
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

void CXTPCalendarOptions::OnOptionsChanged()
{
	XTP_SAFE_CALL2(m_pDataProvider, GetConnection(), SendEvent(XTP_NC_CALENDAROPTIONSWASCHANGED, (WPARAM)-1, 0));
}

///////////////////////////////////////////////////////////////////////////
// Data part
//

void CXTPCalendarOptions::SetDataProvider(CXTPCalendarData* pDataProvider)
{
	// free old data provider
	if (m_pDataProvider)
	{
		m_pDataProvider->SetOptionsToUpdate(NULL);
	}

	// set a new one
	if (pDataProvider)
	{
		m_pDataProvider = pDataProvider;

		if (m_pDataProvider)
		{
			m_pDataProvider->SetOptionsToUpdate(this);
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
#ifdef _XTP_ACTIVEX

// {20DC2896-972D-41c0-93DB-0ECFEB85D536}
static const GUID IID_ICalendarFlagsSet=
{ 0x20dc2896, 0x972d, 0x41c0, { 0x93, 0xdb, 0xe, 0xcf, 0xeb, 0x85, 0xd5, 0x36 } };

BEGIN_INTERFACE_MAP(CXTPCalendarFlagsSet_imp, CCmdTarget)
INTERFACE_PART(CXTPCalendarFlagsSet_imp, IID_ICalendarFlagsSet, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarFlagsSet_imp, IID_ICalendarFlagsSet)

BEGIN_DISPATCH_MAP(CXTPCalendarFlagsSet_imp, CCmdTarget)
DISP_PROPERTY_EX_ID(CXTPCalendarFlagsSet_imp, "Flags", DISPID_VALUE, OleGetFlags, OleSetFlags, VT_I4)

DISP_FUNCTION_ID(CXTPCalendarFlagsSet_imp, "IsFlagSet", 10, OleIsFlagSet, VT_BOOL, VTS_I4)
DISP_FUNCTION_ID(CXTPCalendarFlagsSet_imp, "SetFlag", 11, OleSetFlag, VT_EMPTY, VTS_I4)
DISP_FUNCTION_ID(CXTPCalendarFlagsSet_imp, "ResetFlag", 12, OleResetFlag, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

CXTPCalendarFlagsSet_imp::CXTPCalendarFlagsSet_imp()
{
	m_pdwFlags = NULL;
	m_pDataRef = NULL;

	EnableAutomation();
	EnableTypeLib();
}

CXTPCalendarFlagsSet_imp::~CXTPCalendarFlagsSet_imp()
{
	CMDTARGET_RELEASE(m_pDataRef);
}

void CXTPCalendarFlagsSet_imp::SetDataRef(DWORD* pdwFlags, CCmdTarget* pDataRef)
{
	m_pdwFlags = pdwFlags;
	m_pDataRef = pDataRef;

	CMDTARGET_ADDREF(m_pDataRef);
}

long CXTPCalendarFlagsSet_imp::OleGetFlags()
{
	if (m_pdwFlags)
		return (long)*m_pdwFlags;
	return 0;
}

void CXTPCalendarFlagsSet_imp::OleSetFlags(long nFlags)
{
	if (m_pdwFlags)
		*m_pdwFlags = (DWORD)nFlags;
}

BOOL CXTPCalendarFlagsSet_imp::OleIsFlagSet(long nFlag)
{
	if (m_pdwFlags)
		return ((*m_pdwFlags) & (DWORD)nFlag) != 0;
	return FALSE;
}

void CXTPCalendarFlagsSet_imp::OleSetFlag(long nFlag)
{
	if (m_pdwFlags)
		*m_pdwFlags |= (DWORD)nFlag;

}

void CXTPCalendarFlagsSet_imp::OleResetFlag(long nFlag)
{
	if (m_pdwFlags)
		*m_pdwFlags &= (DWORD)(~nFlag);
}
#endif


/////////////////////////////////////////////////////////////////////////////

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarOptions, CCmdTarget)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "WorkWeekMask", 1, nWorkWeekMask, OnOptionsChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "FirstDayOfTheWeek", 2, nFirstDayOfTheWeek, OnOptionsChanged, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarOptions, "WorkDayStartTime", 3, OleGetWorkDayStartTime, OleSetWorkDayStartTime, VT_DATE)
	DISP_PROPERTY_EX_ID(CXTPCalendarOptions, "WorkDayEndTime", 4, OleGetWorkDayEndTime, OleSetWorkDayEndTime, VT_DATE)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "DayViewAutoResetBusyFlag", 5, bDayView_AutoResetBusyFlag, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "DayViewScaleInterval", 6, nDayView_ScaleInterval, OnOptionsChanged, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPCalendarOptions, "DayViewScaleLabel", 7, OleGetDayViewScaleLabel, OleSetDayViewScaleLabel, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPCalendarOptions, "DayViewScale2Label", 8, OleGetDayViewScale2Label, OleSetDayViewScale2Label, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "DayViewScale2Visible", 9, bDayView_Scale2Visible, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "DayViewCurrentTimeMarkVisible", 21, nDayView_CurrentTimeMarkVisible, OnOptionsChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "DayViewTimeScaleShowMinutes", 22, bDayView_TimeScaleShowMinutes, OnOptionsChanged, VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "MonthViewCompressWeekendDays", 10, bMonthView_CompressWeekendDays, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "MonthViewShowEndDate", 11, bMonthView_ShowEndDate, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "MonthViewShowTimeAsClocks", 12, bMonthView_ShowTimeAsClocks, OnOptionsChanged, VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "WeekViewShowEndDate", 13, bWeekView_ShowEndDate, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "WeekViewShowTimeAsClocks", 14, bWeekView_ShowTimeAsClocks, OnOptionsChanged, VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "EnableInPlaceEditEventSubject_ByF2", 15, bEnableInPlaceEditEventSubject_ByF2, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "EnableInPlaceEditEventSubject_ByMouseClick", 16, bEnableInPlaceEditEventSubject_ByMouseClick, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "EnableInPlaceEditEventSubject_ByTab", 17, bEnableInPlaceEditEventSubject_ByTab, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "EnableInPlaceEditEventSubject_AfterEventResize", 18, bEnableInPlaceEditEventSubject_AfterEventResize, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "EnableInPlaceCreateEvent", 19, bEnableInPlaceCreateEvent, OnOptionsChanged, VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "UseOutlookFontGlyphs", 20, bUseOutlookFontGlyphs, OnOptionsChanged, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPCalendarOptions, "TooltipAddNewText", 23, OleGetTooltipAddNewText, OleSetTooltipAddNewText, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "EnableAddNewTooltip", 24, bEnableAddNewTooltip, OnOptionsChanged, VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "EnablePrevNextEventButtons", 25, bEnablePrevNextEventButtons, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "CompactTimelineEvents", 26, bTimeLineCompact, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "ShowAllDayExpandButton", 30, bShowAllDayExpandButton, OnOptionsChanged, VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "HatchAllDayViewEventsBackground", 130, bHatchAllDayViewEventsBkgnd, OnOptionsChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPCalendarOptions, "ShowCategoryIcons", 135, bShowCategoryIcons, OnOptionsChanged, VT_BOOL)

	DISP_FUNCTION_ID(CXTPCalendarOptions, "GetCurrentTimeZone", 100, OleGetCurrentTimeZone, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarOptions, "GetScale2TimeZone", 101, OleGetScale2TimeZone, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarOptions, "SetScale2TimeZone", 102, OleSetScale2TimeZone, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarOptions, "EnumAllTimeZones", 103, OleEnumAllTimeZones, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarOptions, "AdditionalOptionsFlags", 104, OleGetAdditionalOptionsFlags, VT_DISPATCH, VTS_NONE)
END_DISPATCH_MAP()

// {DFED5027-B4A5-492f-ADA0-56E1B0772A53}
static const GUID IID_ICalendarOptions =
{ 0xdfed5027, 0xb4a5, 0x492f, { 0xad, 0xa0, 0x56, 0xe1, 0xb0, 0x77, 0x2a, 0x53 } };


BEGIN_INTERFACE_MAP(CXTPCalendarOptions, CCmdTarget)
	INTERFACE_PART(CXTPCalendarOptions, IID_ICalendarOptions, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarOptions, IID_ICalendarOptions)

DATE CXTPCalendarOptions::OleGetWorkDayStartTime()
{
	return dtWorkDayStartTime;
}
DATE CXTPCalendarOptions::OleGetWorkDayEndTime()
{
	return dtWorkDayEndTime;
}
void CXTPCalendarOptions::OleSetWorkDayStartTime(DATE dt)
{
	dtWorkDayStartTime = dt;
	OnOptionsChanged();
}
void CXTPCalendarOptions::OleSetWorkDayEndTime(DATE dt)
{
	dtWorkDayEndTime = dt;
	OnOptionsChanged();
}

BSTR CXTPCalendarOptions::OleGetDayViewScaleLabel()
{
	return strDayView_ScaleLabel.AllocSysString();
}

void CXTPCalendarOptions::OleSetDayViewScaleLabel(LPCTSTR pcszLabel)
{
	strDayView_ScaleLabel = pcszLabel;
	OnOptionsChanged();
}

BSTR CXTPCalendarOptions::OleGetDayViewScale2Label()
{
	return strDayView_Scale2Label.AllocSysString();
}

void CXTPCalendarOptions::OleSetDayViewScale2Label(LPCTSTR pcszLabel)
{
	strDayView_Scale2Label = pcszLabel;
	OnOptionsChanged();
}

BSTR CXTPCalendarOptions::OleGetTooltipAddNewText()
{
	return strTooltipAddNewText.AllocSysString();
}

void CXTPCalendarOptions::OleSetTooltipAddNewText(LPCTSTR pcszLabel)
{
	strTooltipAddNewText = pcszLabel;
	OnOptionsChanged();
}

LPDISPATCH CXTPCalendarOptions::OleGetCurrentTimeZone()
{
	CXTPCalendarTimeZonePtr ptrXTP_TZI;
	ptrXTP_TZI = GetCurrentTimeZoneInfo();
	if (ptrXTP_TZI)
	{
		return ptrXTP_TZI->GetIDispatch(TRUE);
	}

	return NULL;
}

LPDISPATCH CXTPCalendarOptions::OleGetScale2TimeZone()
{
	CXTPCalendarTimeZonePtr ptrXTP_TZI;
	ptrXTP_TZI = CXTPCalendarTimeZone::GetTimeZoneInfo(&tziDayView_Scale2TimeZone);
	if (ptrXTP_TZI)
	{
		return ptrXTP_TZI->GetIDispatch(TRUE);
	}

	return NULL;
}

void CXTPCalendarOptions::OleSetScale2TimeZone(LPDISPATCH pDispTZInfo)
{
	CXTPCalendarTimeZone* pTZInfo;
	pTZInfo = DYNAMIC_DOWNCAST(CXTPCalendarTimeZone, CXTPCalendarTimeZone::FromIDispatch(pDispTZInfo));
	if (!pTZInfo)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	if (pTZInfo)
	{
		tziDayView_Scale2TimeZone = *((TIME_ZONE_INFORMATION*)pTZInfo);
		OnOptionsChanged();
	}
}

LPDISPATCH CXTPCalendarOptions::OleEnumAllTimeZones()
{
	CXTPCalendarTimeZones* pXTP_TZI_array;
	pXTP_TZI_array = new CXTPCalendarTimeZones();
	if (!pXTP_TZI_array)
	{
		AfxThrowOleException(E_OUTOFMEMORY);
	}
	VERIFY( pXTP_TZI_array->InitFromRegistry() );

	return pXTP_TZI_array->GetIDispatch(FALSE);
}

LPDISPATCH CXTPCalendarOptions::OleGetAdditionalOptionsFlags()
{
	CXTPCalendarFlagsSet_imp* pFlagsWrapper = new CXTPCalendarFlagsSet_imp();
	if (pFlagsWrapper)
		pFlagsWrapper->SetDataRef(&dwAdditionalOptions, this);
	else
		AfxThrowOleException(E_OUTOFMEMORY);

	return pFlagsWrapper->GetIDispatch(FALSE);
}

#endif
