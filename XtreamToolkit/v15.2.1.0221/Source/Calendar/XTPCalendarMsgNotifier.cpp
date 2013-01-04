// XTPCalendarMsgNotifier.cpp: implementation of the CXTPCalendarMsgNotifier class.
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
#include "XTPCalendarMsgNotifier.h"
#include "XTPCalendarUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTP_TOPLEVELNOTIFIER_CHECKDATE_TIMEOUT 10 * 1000

//===========================================================================
// Summary:
//     Define name for XTP_WM_TIME_ZONE_CHANGED windows message.
//===========================================================================
static LPCTSTR XTP_WM_TIME_ZONE_CHANGED_MSG = _T("XTP_WM_TIMEZONECHANGED");

UINT XTP_WM_TIME_ZONE_CHANGED = RegisterWindowMessage(XTP_WM_TIME_ZONE_CHANGED_MSG);


static LPCTSTR XTP_TOP_LEVEL_WND_NOTIFIER_NAME = _T("XTPCalendarMsgNotifier");

CXTPCalendarMsgNotifier* XTPCalendarMsgNotifier()
{
	static CXTPCalendarMsgNotifier sMsgNotifier;
	return &sMsgNotifier;
}
/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarMsgNotifier

CXTPCalendarMsgNotifier::CXTPCalendarMsgNotifier()
{
	::ZeroMemory(&m_timeLastDate, sizeof(m_timeLastDate));
	::ZeroMemory(&m_tziLast, sizeof(m_tziLast));
}

CXTPCalendarMsgNotifier::~CXTPCalendarMsgNotifier()
{
	DestroyWindow();
}

BOOL CXTPCalendarMsgNotifier::CreateWnd()
{
	LPCTSTR pcszSimpleWndClass = AfxRegisterWndClass(0);
	CRect rcEmpty(0, 0, 0, 0);

	BOOL bCreated = CreateEx(0, pcszSimpleWndClass,
		XTP_TOP_LEVEL_WND_NOTIFIER_NAME, WS_POPUP, rcEmpty, NULL, 0);

	CXTPCalendarUtils::GetCurrentTime().GetAsSystemTime(m_timeLastDate);

	VERIFY (::GetTimeZoneInformation(&m_tziLast) != TIME_ZONE_ID_INVALID);

	SetTimer(1, XTP_TOPLEVELNOTIFIER_CHECKDATE_TIMEOUT, NULL);
	return bCreated;
}

void CXTPCalendarMsgNotifier::Advise(CWnd* pWnd, UINT uWndMsg)
{
	ASSERT(pWnd);

	if (m_hWnd == NULL)
	{
		VERIFY(CreateWnd());
	}

	// WARNING! Already Advised.
	ASSERT(-1 == m_arClients.FindExact(pWnd, uWndMsg));

	m_arClients.Remove(pWnd, uWndMsg);

	CLIENT_INFO clInf = {pWnd, msgAll};
	m_arClients.Add(clInf);
}

void CXTPCalendarMsgNotifier::Unadvise(CWnd* pWnd, UINT uWndMsg)
{
	// WARNING! Already Unadvised.
	ASSERT(m_arClients.FindExact(pWnd, uWndMsg) >= 0);

	m_arClients.Remove(pWnd, uWndMsg);

	if (m_arClients.GetSize() == 0 && m_hWnd)
	{
		VERIFY(DestroyWindow());
	}
}

void CXTPCalendarMsgNotifier::SendToClients(UINT uWndMsg, WPARAM wParam, LPARAM lParam)
{
	CClientsArray arClients;

	// prepare clients to local cache
	int nCount = (int)m_arClients.GetSize(), i;
	for (i = 0; i < nCount; i++)
	{
		CLIENT_INFO& rData = m_arClients.ElementAt(i);

		if (rData.m_uWndMsg == uWndMsg || rData.m_uWndMsg == msgAll)
		{
			arClients.Add(rData);
		}
	}

	// Send to clients from local cache
	nCount = (int)arClients.GetSize();
	for (i = 0; i < nCount; i++)
	{
		const CLIENT_INFO& rData = arClients.ElementAt(i);

		if (-1 == m_arClients.FindClient(rData.m_pWnd, rData.m_uWndMsg))
		{
			continue;
		}

		try
		{
			if (::IsWindow(rData.m_pWnd->GetSafeHwnd()))
			{
				::SendMessage(rData.m_pWnd->GetSafeHwnd(), uWndMsg, wParam, lParam);
			}
		}
		catch(...)
		{
			ASSERT(FALSE);
			m_arClients.Remove(rData.m_pWnd, msgAll);
		}
	}

	if (m_arClients.GetSize() == 0 && m_hWnd)
	{
		VERIFY(DestroyWindow());
	}
}

/////////////////////////////////////////////////////////////////////////////
//class CClientsArray : public CArray<CLIENT_INFO, CLIENT_INFO&>

CXTPCalendarMsgNotifier::CClientsArray::CClientsArray()
{
}

CXTPCalendarMsgNotifier::CClientsArray::~CClientsArray()
{
}

int CXTPCalendarMsgNotifier::CClientsArray::FindExact(CWnd* pWnd, UINT uWndMsg) const
{
	int nCount = (int)GetSize();
	for (int i = 0; i < nCount; i++)
	{
		const CLIENT_INFO& rData = m_pData[i];

		if (rData.m_pWnd == pWnd && rData.m_uWndMsg == uWndMsg)
		{
			return i;
		}
	}
	return -1;
}

int CXTPCalendarMsgNotifier::CClientsArray::FindClient(CWnd* pWnd, UINT uWndMsg) const
{
	int nCount = (int)GetSize();
	for (int i = 0; i < nCount; i++)
	{
		const CLIENT_INFO& rData = m_pData[i];

		if (rData.m_pWnd == pWnd)
		{
			if (rData.m_uWndMsg == uWndMsg || uWndMsg == CXTPCalendarMsgNotifier::msgAll)
			{
				return i;
			}
		}
	}
	return -1;
}

void CXTPCalendarMsgNotifier::CClientsArray::Remove(CWnd* pWnd, UINT uWndMsg)
{
	int nCount = (int)GetSize();
	for (int i = nCount-1; i >= 0; i--)
	{
		const CLIENT_INFO& rData = ElementAt(i);

		if (rData.m_pWnd == pWnd)
		{
			if (rData.m_uWndMsg == uWndMsg || uWndMsg == CXTPCalendarMsgNotifier::msgAll)
			{
				RemoveAt(i);
			}
		}
	}
}


BEGIN_MESSAGE_MAP(CXTPCalendarMsgNotifier, CWnd)
	ON_WM_TIMECHANGE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarMsgNotifier message handlers

void CXTPCalendarMsgNotifier::OnTimeChange()
{
	_ProcessTimeZone();

	SendToClients(WM_TIMECHANGE, 0, 0);
}

void CXTPCalendarMsgNotifier::OnSysColorChange()
{
	SendToClients(WM_SYSCOLORCHANGE, 0, 0);
}

void CXTPCalendarMsgNotifier::OnTimer(UINT_PTR uTimerID)
{
	if (uTimerID == 1)
	{
		SYSTEMTIME timeCurrent;
		::ZeroMemory(&timeCurrent, sizeof(timeCurrent));
		CXTPCalendarUtils::GetCurrentTime().GetAsSystemTime(timeCurrent);

		//Is date changed ?
		if (timeCurrent.wDay    != m_timeLastDate.wDay ||
			timeCurrent.wMonth  != m_timeLastDate.wMonth ||
			timeCurrent.wYear   != m_timeLastDate.wYear)
		{
			m_timeLastDate = timeCurrent;
			//OnTimeChange();
			SendToClients(WM_TIMECHANGE, 0, 0);
		}
	}
}

void CXTPCalendarMsgNotifier::_ProcessTimeZone()
{
	TIME_ZONE_INFORMATION tziCurrent;
	::ZeroMemory(&tziCurrent, sizeof(tziCurrent));

	if (::GetTimeZoneInformation(&tziCurrent) == TIME_ZONE_ID_INVALID)
	{
		ASSERT(FALSE);
		return;
	}

	if (!IsTimeZoneEqual(tziCurrent, m_tziLast))
	{
		m_tziLast = tziCurrent;

		SendToClients(XTP_WM_TIME_ZONE_CHANGED, 0, 0);

		//TRACE(_T("XTP_WM_TIME_ZONE_CHANGED is sent \n"));
	}
}

BOOL CXTPCalendarMsgNotifier::IsTimeZoneEqual(const TIME_ZONE_INFORMATION& rTzi1, const TIME_ZONE_INFORMATION& rTzi2) const
{
	return  rTzi1.Bias == rTzi2.Bias &&

			memcmp(&rTzi1.StandardDate, &rTzi2.StandardDate, sizeof(SYSTEMTIME)) == 0 &&

			rTzi1.StandardBias == rTzi2.StandardBias &&

			memcmp(&rTzi1.DaylightDate, &rTzi2.DaylightDate, sizeof(SYSTEMTIME)) == 0 &&

			rTzi1.DaylightBias == rTzi2.DaylightBias;

	//WCHAR StandardName[ 32 ];
	//WCHAR DaylightName[ 32 ];
}
