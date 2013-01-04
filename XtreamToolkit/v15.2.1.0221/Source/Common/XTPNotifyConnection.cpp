// XTPNotifyConnection.cpp: implementation of the CXTPNotifyConnection and CXTPNotifySink classes.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "XTPNotifyConnection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4571) // warning C4571: catch(...) blocks compiled with /EHs do not catch or re-throw Structured Exceptions

/////////////////////////////////////////////////////////////////////////////
static LPCTSTR XTP_NOTIFICATION_SINK_MT_MSGWND_NAME  = _T("XTPNotificationSinkMT_MsgWnd");


/////////////////////////////////////////////////////////////////////////////
//class CEmptySyncObject : public CSyncObject

CXTPNotifyConnection::CEmptySyncObject::CEmptySyncObject() :
		CSyncObject(_T("XTPEmptySyncObject"))
{
}

////////////////////////////////////////////////////////////////////////////

CXTPNotifyConnection::CXTPNotifyConnection()
{
	m_nSendQueueCacheSize = 0;
}

CXTPNotifyConnection::~CXTPNotifyConnection()
{
	RemoveAll();
}

void CXTPNotifyConnection::RemoveAll()
{
	CSingleLock singleLock(GetDataLock(), TRUE);

	int nCount = (int)m_arrConnections.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CONNECTION_DESCRIPTOR* pCurElem = m_arrConnections[i];
		ASSERT(pCurElem);
		if (pCurElem)
		{
			delete pCurElem;
		}
		m_arrConnections[i] = NULL;
	}

	m_arrConnections.RemoveAll();
}

XTP_CONNECTION_ID CXTPNotifyConnection::Advise(XTP_NOTIFY_CODE Event, CXTPNotifySinkBase* pSink)
{
	ASSERT(pSink);

	CSingleLock singleLock(GetDataLock(), TRUE);

	CONNECTION_DESCRIPTOR* pNewElem = new CONNECTION_DESCRIPTOR;

	if (!pNewElem)
	{
		return 0;
	}

	pNewElem->dwConnectionID = (XTP_CONNECTION_ID)pNewElem;
	pNewElem->dwNotifyCode = Event;
	pNewElem->pSink = pSink;

	m_arrConnections.Add(pNewElem);

	return pNewElem->dwConnectionID;
}

void  CXTPNotifyConnection::Unadvise(XTP_CONNECTION_ID ConnectionID)
{
	CSingleLock singleLock(GetDataLock(), TRUE);

	try
	{
		int nCount = (int)m_arrConnections.GetSize();
		int nFIndex = FindConnection(ConnectionID);

		if (nFIndex >= 0 && nFIndex < nCount)
		{
			CONNECTION_DESCRIPTOR* pElem = m_arrConnections[nFIndex];

			ASSERT(pElem);
			if (pElem)
			{
				delete pElem;
			}
			m_arrConnections.RemoveAt(nFIndex);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	catch(...)
	{
		ASSERT(FALSE);
		TRACE(_T("EXCEPTION! CXTPNotifyConnection::Unadvise(ConnectionID = %d)\n"), ConnectionID);
	}
}

BOOL CXTPNotifyConnection::SendEvent(XTP_NOTIFY_CODE Event,
								WPARAM wParam , LPARAM lParam, DWORD dwFlags)
{
	CSingleLock singleLock(GetDataLock(), TRUE);
	InternalAddRef();

	int nCount = (int)m_arrConnections.GetSize();

	if (m_arrSendQueueCache.GetSize() < nCount + m_nSendQueueCacheSize)
	{
		m_arrSendQueueCache.SetSize(nCount + m_nSendQueueCacheSize);
	}
	//******************************************************************************
	int nFirstLocalClientIndex = m_nSendQueueCacheSize;

	int i;
	for (i = 0; i < nCount; i++)
	{
		CONNECTION_DESCRIPTOR* pElem = m_arrConnections[i];
		ASSERT(pElem);
		if (pElem && pElem->dwNotifyCode == Event)
		{
			m_arrSendQueueCache.SetAt(m_nSendQueueCacheSize, *pElem);
			m_nSendQueueCacheSize++;
		}
	}
	int nLastLocalClientIndex = m_nSendQueueCacheSize-1;

	singleLock.Unlock();
	//************************************************************************
	for (i = nFirstLocalClientIndex; i <= nLastLocalClientIndex; i++)
	{
		singleLock.Lock();

		if (i >= m_arrSendQueueCache.GetSize())
		{
			ASSERT(FALSE);
			return FALSE;
		}

		CONNECTION_DESCRIPTOR& rElem = m_arrSendQueueCache.ElementAt(i);
		int nFIndex = FindConnection(rElem.dwConnectionID);
		if (nFIndex < 0)
		{
			// Unadvise was called inside OnEvent(...) Call
			continue;
		}
		try
		{
			CXTPNotifySinkBase* ptrSink = rElem.pSink;

			singleLock.Unlock();

			if (ptrSink)
			{
				ptrSink->OnEvent(Event, wParam, lParam, dwFlags);
			}
			else
			{
				ASSERT(FALSE);
				TRACE(_T("CXTPNotifyConnection::SendEvent(Event = %d, wParam = %d, lParam = %d, dwFlags = %x) pSink = %x\n"),
						Event, wParam, lParam, dwFlags, rElem.pSink);
			}
		}
		catch(...)
		{
			ASSERT(FALSE);
			TRACE(_T("EXCEPTION! CXTPNotifyConnection::SendEvent(Event = %d, wParam = %d, lParam = %d, dwFlags = %x)\n"),
					Event, wParam, lParam, dwFlags);
		}
	}

	//==============================================================================
	singleLock.Lock();

	m_nSendQueueCacheSize = nFirstLocalClientIndex;

	singleLock.Unlock();

	InternalRelease();
	return (nFirstLocalClientIndex <= nLastLocalClientIndex);
}

int CXTPNotifyConnection::FindConnection(XTP_CONNECTION_ID ConnectionID)
{
	CSingleLock singleLock(GetDataLock(), TRUE);

	int nCount = (int)m_arrConnections.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CONNECTION_DESCRIPTOR* pCurElem = m_arrConnections[i];
		ASSERT(pCurElem);
		if (pCurElem)
		{
			if (pCurElem->dwConnectionID == ConnectionID)
			{
				return i;
			}
		}
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////
//class CXTPNotifyConnectionMT : public CXTPNotifyConnection

CXTPNotifyConnectionMT::CXTPNotifyConnectionMT()
{
}

CXTPNotifyConnectionMT::~CXTPNotifyConnectionMT()
{
}


////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CXTPNotifySinkImplMTMsgWnd, CWnd)
	ON_REGISTERED_MESSAGE(xtp_wm_NotificationSinkMTOnEvent, HandleInterThreadEvent)
END_MESSAGE_MAP()

CXTPNotifySinkImplMTMsgWnd::CXTPNotifySinkImplMTMsgWnd()
{
}

CXTPNotifySinkImplMTMsgWnd::~CXTPNotifySinkImplMTMsgWnd()
{
}

LRESULT CXTPNotifySinkImplMTMsgWnd::HandleInterThreadEvent(WPARAM wParam, LPARAM lParam)
{
	return OnInterThreadEvent(wParam, lParam);
}

BOOL CXTPNotifySinkImplMTMsgWnd::CreateWnd()
{
	LPCTSTR pcszSimpleWndClass = AfxRegisterWndClass(0);
	CRect rcEmpty(0, 0, 0, 0);

	BOOL bCreated = CreateEx(0, pcszSimpleWndClass,
		XTP_NOTIFICATION_SINK_MT_MSGWND_NAME,
		WS_POPUP, rcEmpty, NULL, 0);
	ASSERT(bCreated);
	return bCreated;
}

//////////////////////////////////////////////////////////////////////////
// CXTPNotifySinkBase

CXTPNotifySinkBase::CXTPNotifySinkBase()
{
}

CXTPNotifySinkBase::~CXTPNotifySinkBase()
{
	// Should call UnadviseAll before delete
	ASSERT(m_mapAdviseData.IsEmpty());
}

XTP_CONNECTION_ID CXTPNotifySinkBase::Advise(CXTPNotifyConnection* pConnection,
								XTP_NOTIFY_CODE dwNotifyCode)
{
	ASSERT(pConnection);
	if (!pConnection)
	{
		return 0;
	}

	XTP_CONNECTION_ID dwConnectionID = pConnection->Advise(dwNotifyCode, this);
	ASSERT(dwConnectionID);

	if (dwConnectionID)
	{
		ADVISE_DESCRIPTOR advDataNew;
		ASSERT(!m_mapAdviseData.Lookup(dwConnectionID, advDataNew));

		advDataNew.dwConnectionID = dwConnectionID;
		advDataNew.pConnection = pConnection;
		advDataNew.dwNotifyCode = dwNotifyCode;
		pConnection->InternalAddRef();

		m_mapAdviseData.SetAt(dwConnectionID, advDataNew);

	}
	return dwConnectionID;
}

void CXTPNotifySinkBase::UnadviseAll()
{
	XTP_CONNECTION_ID ConnectionID = 0;
	ADVISE_DESCRIPTOR advData;
	POSITION pos = m_mapAdviseData.GetStartPosition();
	while (pos)
	{
		m_mapAdviseData.GetNextAssoc(pos, ConnectionID, advData);
		if (advData.pConnection)
		{
			OnUnadvise(advData.dwNotifyCode);

			advData.pConnection->Unadvise(advData.dwConnectionID);
			advData.pConnection->InternalRelease();
		}
	}
	m_mapAdviseData.RemoveAll();
}

void CXTPNotifySinkBase::Unadvise(XTP_CONNECTION_ID ConnectionID)
{
	ADVISE_DESCRIPTOR advData;
	if (m_mapAdviseData.Lookup(ConnectionID, advData))
	{
		if (advData.pConnection)
		{
			OnUnadvise(advData.dwNotifyCode);

			advData.pConnection->Unadvise(advData.dwConnectionID);
			advData.pConnection->InternalRelease();
		}
		m_mapAdviseData.RemoveKey(ConnectionID);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CXTPNotifySinkBase::OnUnadvise(XTP_NOTIFY_CODE /*dwNotifyCode*/)
{

}


//////////////////////////////////////////////////////////////////////////
// CXTPNotifySink


CXTPNotifySink::CXTPNotifySink()
{
	m_mapHandlers.InitHashTable(10, FALSE);
}

CXTPNotifySink::~CXTPNotifySink()
{
	ASSERT(m_mapHandlers.IsEmpty());
}

void CXTPNotifySink::OnUnadvise(XTP_NOTIFY_CODE dwNotifyCode)
{
	CXTPNotifySinkDelegate* pOldDelegate = NULL;

	if (m_mapHandlers.Lookup(dwNotifyCode, pOldDelegate))
	{
		delete pOldDelegate;
		m_mapHandlers.RemoveKey(dwNotifyCode);
	}
	else
	{
		ASSERT(FALSE);
	}
}

XTP_CONNECTION_ID CXTPNotifySink::Advise(CXTPNotifyConnection* pConnection, XTP_NOTIFY_CODE dwNotifyCode, CXTPNotifySinkDelegate* pDelegate)
{
	CXTPNotifySinkDelegate* pOldDelegate = NULL;

	if (m_mapHandlers.Lookup(dwNotifyCode, pOldDelegate))
	{
		ASSERT(FALSE);
		delete pOldDelegate;
	}

	m_mapHandlers[dwNotifyCode] = pDelegate;

	return CXTPNotifySinkBase::Advise(pConnection, dwNotifyCode);
}

void CXTPNotifySink::OnEvent(XTP_NOTIFY_CODE dwNotifyCode, WPARAM wParam, LPARAM lParam, DWORD /*dwFlags*/)
{
	CXTPNotifySinkDelegate* pDelegate = NULL;
	if (m_mapHandlers.Lookup(dwNotifyCode, pDelegate) && (pDelegate != NULL))
	{
		pDelegate->OnEvent(dwNotifyCode, wParam, lParam);
	}
	else
	{
		//WARNING. no handler found. ???
		ASSERT(FALSE);
	}
}

void CXTPNotifySink::Delete()
{
	UnadviseAll();
	delete this;
}

//////////////////////////////////////////////////////////////////////////
// CXTPNotifySinkMT



CXTPNotifySinkMT::CXTPNotifySinkMT(BOOL bInitInternal /*= TRUE*/)
{
	m_dwTraceFlag0 = 0;
	m_dwNexDataID = 0;
	m_bWndCreated = FALSE;
	m_dwOwnerThreadID = ::GetCurrentThreadId();

	if (bInitInternal)
	{
		m_bWndCreated = CreateWnd();
	}

	m_PostedEvents.InitHashTable(199, FALSE);
}

CXTPNotifySinkMT::~CXTPNotifySinkMT()
{
	UnadviseAll();

	DestroyWindow();
	m_bWndCreated = FALSE;
}

void CXTPNotifySinkMT::OnEvent(XTP_NOTIFY_CODE dwNotifyCode, WPARAM wParam, LPARAM lParam, DWORD dwFlags)
{
	ASSERT(m_bWndCreated);

	if (dwFlags & xtpNotifyDirectCallForOneThread)
	{
		if (GetCurrentThreadId() == m_dwOwnerThreadID)
		{
			CXTPNotifySink::OnEvent(dwNotifyCode, wParam, lParam, dwFlags);
			return;
		}
	}

	//====================================================================
	XTP_INTER_THREAD_EVENT_DATA ithData = {dwNotifyCode, wParam, lParam, dwFlags};

	if (dwFlags & xtpNotifyPostMessage)
	{
		CSingleLock singleLock(&m_DataCS, TRUE);

#ifdef _DEBUG
		XTP_INTER_THREAD_EVENT_DATA ithDataTmp;
		ASSERT(m_PostedEvents.Lookup(m_dwNexDataID, ithDataTmp) == FALSE);
#endif

		DWORD dwDataID = m_dwNexDataID;
		m_dwNexDataID++;


		BOOL bTrace1 = FALSE;
		BOOL bPosted = FALSE;
		do
		{
			bPosted = PostMessage(xtp_wm_NotificationSinkMTOnEvent, NULL, dwDataID);
			if (!bPosted && (dwFlags & xtpNotifyGuarantyPost))
			{
				if (!bTrace1) {
					TRACE(_T("WARNING!  CXTPNotifySinkBaseImplMT: PostMessage return FALSE. WAIT and retry. (ThreadID = %x, dataID = %d) \n"),
						GetCurrentThreadId(),   dwDataID);
					bTrace1 = TRUE;
				}

				singleLock.Unlock();
				Sleep(100);
				singleLock.Lock();
			}
		}
		while (!bPosted && (dwFlags & xtpNotifyGuarantyPost));

		if (bPosted)
		{
			m_PostedEvents[dwDataID] = ithData;

			if (bTrace1) {
				TRACE(_T("WARNING.*  CXTPNotifySinkBaseImplMT: Event is posted! (ThreadID = %x, dataID = %d) \n"),
					GetCurrentThreadId(),   dwDataID);
			}

			m_dwTraceFlag0 = 0;
		}
		else if (!(dwFlags & xtpNotifyGuarantyPost))
		{
			if (m_dwTraceFlag0 < 3)
			{
				TRACE(_T("WARNING!  CXTPNotifySinkBaseImplMT: PostMessage return FALSE - Event is skipped! (ThreadID = %x, dataID = %d) \n"),
					GetCurrentThreadId(),   dwDataID);
				TRACE(_T("          Use xtpNotifyGuarantyPost flag for important events. \n"));
			}
			m_dwTraceFlag0++;
		}
	}
	else
	{
		SendMessage(xtp_wm_NotificationSinkMTOnEvent, (WPARAM)&ithData, 0);
	}
};

LRESULT CXTPNotifySinkMT::OnInterThreadEvent(WPARAM pEventData, LPARAM dwPostDataID)
{
	XTP_INTER_THREAD_EVENT_DATA* pIthData = (XTP_INTER_THREAD_EVENT_DATA*)pEventData;

	if (pIthData)
	{
		CXTPNotifySink::OnEvent(pIthData->dwNotifyCode, pIthData->wParam,
			pIthData->lParam, pIthData->dwFlags);
	}
	else
	{
		XTP_INTER_THREAD_EVENT_DATA ithData;

		CSingleLock singleLock(&m_DataCS, TRUE);

		if (m_PostedEvents.Lookup((DWORD)dwPostDataID, ithData))
		{
			m_PostedEvents.RemoveKey((DWORD)dwPostDataID);

			singleLock.Unlock();

			CXTPNotifySink::OnEvent(ithData.dwNotifyCode, ithData.wParam,
				ithData.lParam, ithData.dwFlags);
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	//====================================================================
	return 0;
};
