// XTPSynchro.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "XTPSynchro.h"

//
// CXTPSimpleEvent
//

CXTPSimpleEvent::CXTPSimpleEvent(BOOL bInitialState)
: m_bState(!!bInitialState) {}

void CXTPSimpleEvent::Reset()
{
	InterlockedExchange((PLONG)&m_bState, 0);
}

void CXTPSimpleEvent::Set()
{
	InterlockedExchange((PLONG)&m_bState, 1);
}

BOOL CXTPSimpleEvent::IsSignaled() const
{
	return !!m_bState;
}

void CXTPSimpleEvent::Wait()
{
	if (!m_bState)
	{
		DWORD dwTimeout = 1;
		while (TRUE)
		{
			Sleep(dwTimeout);
			if (m_bState)
			{
				break;
			}
			else if (dwTimeout < XTP_SYNC_MAX_EXPONENTIAL_TIMEOUT)
			{
				dwTimeout <<= 1;
			}
		}
	}
}


//
// CXTPSimpleCriticalSection
//

CXTPSimpleCriticalSection::~CXTPSimpleCriticalSection()
{
	XTP_ASYNC_ASSERT(0 == m_nLock);
	XTP_ASYNC_ASSERT(0 == m_nRecursionCounter);
	XTP_ASYNC_ASSERT(0 == m_dwThreadId);
}

void CXTPSimpleCriticalSection::Enter()
{
	DWORD dwThreadId = GetCurrentThreadId();
	if (dwThreadId != m_dwThreadId)
	{
		DWORD dwTimeout = 1;
		while (true)
		{
			while (0 != m_nLock)
			{
				Sleep(dwTimeout);
				if (dwTimeout < XTP_SYNC_MAX_EXPONENTIAL_TIMEOUT)
				{
					dwTimeout <<= 1;
				}
			}

			if (1 == InterlockedIncrement((PLONG)&m_nLock))
			{
				XTP_ASYNC_ASSERT(0 == m_nRecursionCounter);
				InterlockedIncrement((PLONG)&m_nRecursionCounter);
				m_dwThreadId = dwThreadId;
				break;
			}
			else
			{
				InterlockedDecrement((PLONG)&m_nLock);
			}
		}
	}
	else
	{
		InterlockedIncrement((PLONG)&m_nRecursionCounter);
	}

	XTP_ASYNC_ASSERT(1 <= m_nRecursionCounter);
	XTP_ASYNC_ASSERT(dwThreadId == m_dwThreadId);
}

void CXTPSimpleCriticalSection::Leave()
{
	XTP_ASYNC_ASSERT(GetCurrentThreadId() == m_dwThreadId);

	if (0 == InterlockedDecrement((PLONG)&m_nRecursionCounter))
	{
		m_dwThreadId = 0;
		InterlockedDecrement((PLONG)&m_nLock);
	}
}

//
// CXTPRWCriticalSection
//

const DWORD CXTPRWCriticalSection::m_dwLocalSharedLockCounterTlsIdx = TlsAlloc();

CXTPRWCriticalSection::CXTPRWCriticalSection()
	: m_nSharedLockCounter(0)
	, m_nExclusiveLockCounter(0)
	, m_sharedLockEvent(TRUE)
	, m_dwExclusiveLockTid(0) {}

CXTPRWCriticalSection::~CXTPRWCriticalSection()
{
	XTP_ASYNC_ASSERT("Shared critical section is locked" && 0 == m_nSharedLockCounter);
	XTP_ASYNC_ASSERT("Exclusive critical section is locked" && 0 == m_nExclusiveLockCounter);
}

BOOL CXTPRWCriticalSection::IsLockedSharedly() const volatile
{
	return 0 < m_nSharedLockCounter;
}

BOOL CXTPRWCriticalSection::IsLockedExclusively() const volatile
{
	return 0 < m_nExclusiveLockCounter;
}

void CXTPRWCriticalSection::LockShared()
{
	if (GetCurrentThreadId() != m_dwExclusiveLockTid)
	{
		LockShared(FALSE);
		XTP_ASYNC_ASSERT(IsLockedSharedly());
		XTP_ASYNC_ASSERT(!IsLockedExclusively());
	}
}

void CXTPRWCriticalSection::UnlockShared()
{
	if (GetCurrentThreadId() != m_dwExclusiveLockTid)
	{
		XTP_ASYNC_ASSERT(IsLockedSharedly());
		XTP_ASYNC_ASSERT(!IsLockedExclusively());
		UnlockShared(FALSE);
	}
}

void CXTPRWCriticalSection::LockExclusive()
{
	if (GetCurrentThreadId() != m_dwExclusiveLockTid)
	{
		PushLocalSharedLockState();
	}

	while (true)
	{
		m_sharedLockEvent.Wait();
		m_csGeneral.Enter();
		if (IsLockedSharedly())
		{
			m_csGeneral.Leave();
			continue;
		}

		if (1 == InterlockedIncrement((PLONG)&m_nExclusiveLockCounter))
		{
			m_dwExclusiveLockTid = GetCurrentThreadId();
		}
		else
		{
			XTP_ASYNC_ASSERT(GetCurrentThreadId() == m_dwExclusiveLockTid);
		}
		break;
	}

	XTP_ASYNC_ASSERT(IsLockedExclusively());
	XTP_ASYNC_ASSERT(!IsLockedSharedly());
}

void CXTPRWCriticalSection::UnlockExclusive()
{
	XTP_ASYNC_ASSERT(IsLockedExclusively());
	XTP_ASYNC_ASSERT(!IsLockedSharedly());

	XTP_ASYNC_ASSERT(GetCurrentThreadId() == m_dwExclusiveLockTid);
	if (GetCurrentThreadId() == m_dwExclusiveLockTid)
	{
		if (1 == m_nExclusiveLockCounter)
		{
			PopSharedLockState();
		}

		long nLockCounter = InterlockedDecrement((PLONG)&m_nExclusiveLockCounter);
		XTP_ASYNC_ASSERT("Extra release" && 0 <= nLockCounter);

		if (0 == nLockCounter)
		{
			m_dwExclusiveLockTid = 0;
		}

		m_csGeneral.Leave();
	}
}

void CXTPRWCriticalSection::PushLocalSharedLockState()
{
	XTP_ASYNC_ASSERT(GetCurrentThreadId() != m_dwExclusiveLockTid);

	long nLocalSharedLockCounter = GetLocalSharedLockCounter();
	XTP_ASYNC_ASSERT(nLocalSharedLockCounter <= m_nSharedLockCounter);

	for (long i = 0; i < nLocalSharedLockCounter; ++i)
	{
		UnlockShared(TRUE);
	}
}

void CXTPRWCriticalSection::PopSharedLockState()
{
	XTP_ASYNC_ASSERT(GetCurrentThreadId() == m_dwExclusiveLockTid);
	XTP_ASYNC_ASSERT(1 == m_nExclusiveLockCounter);

	long nLocalSharedLockCounter = GetLocalSharedLockCounter();
	for (long i = 0; i < nLocalSharedLockCounter; ++i)
	{
		LockShared(TRUE);
	}
}

void CXTPRWCriticalSection::LockShared(BOOL bPopMode)
{
	CXTPSimpleCriticalSection::CLock lock(m_csGeneral);

	if (bPopMode)
	{
		XTP_ASYNC_ASSERT(GetCurrentThreadId() == m_dwExclusiveLockTid);
		XTP_ASYNC_ASSERT(1 == m_nExclusiveLockCounter);
	}

	if (!bPopMode)
	{
		IncrementLocalSharedLockCounter();
	}

	if (1 == InterlockedIncrement((PLONG)&m_nSharedLockCounter))
	{
		m_sharedLockEvent.Reset();
	}
}

void CXTPRWCriticalSection::UnlockShared(BOOL bPushMode)
{
	if (bPushMode)
	{
		XTP_ASYNC_ASSERT(GetCurrentThreadId() != m_dwExclusiveLockTid);
	}

	if (!bPushMode)
	{
		DecrementLocalSharedLockCounter();
	}

	long nLockCounter = InterlockedDecrement((PLONG)&m_nSharedLockCounter);
	if (0 == nLockCounter)
	{
		m_sharedLockEvent.Set();
	}

	XTP_ASYNC_ASSERT("Extra release" && 0 <= nLockCounter);
}

long CXTPRWCriticalSection::IncrementLocalSharedLockCounter()
{
	long nCounter;
	LocalSharedLockCounterMap* pMap = reinterpret_cast<LocalSharedLockCounterMap*>(
		TlsGetValue(m_dwLocalSharedLockCounterTlsIdx));
	if (NULL == pMap)
	{
		pMap = new LocalSharedLockCounterMap();
		(*pMap)[this] = 1;
		TlsSetValue(m_dwLocalSharedLockCounterTlsIdx, pMap);
		nCounter = 1;
	}
	else
	{
		nCounter = ++(*pMap)[this];
	}

	return nCounter;
}

long CXTPRWCriticalSection::DecrementLocalSharedLockCounter()
{
	long nCounter = 0;
	LocalSharedLockCounterMap* pMap = reinterpret_cast<LocalSharedLockCounterMap*>(
		TlsGetValue(m_dwLocalSharedLockCounterTlsIdx));
	ASSERT(NULL != pMap);
	if (NULL != pMap)
	{
		nCounter = --(*pMap)[this];
		XTP_ASYNC_ASSERT("Extra decrementing" && 0 <= nCounter);
		if (0 == nCounter)
		{
			if (pMap->RemoveKey(this))
			{
				if (pMap->IsEmpty())
				{
					delete pMap;
					TlsSetValue(m_dwLocalSharedLockCounterTlsIdx, NULL);
				}
			}
		}
	}

	return nCounter;
}

long CXTPRWCriticalSection::GetLocalSharedLockCounter() const
{
	long nCounter = 0;
	LocalSharedLockCounterMap* pMap = reinterpret_cast<LocalSharedLockCounterMap*>(
		TlsGetValue(m_dwLocalSharedLockCounterTlsIdx));
	if (NULL != pMap)
	{
		pMap->Lookup(this, nCounter);
	}

	return nCounter;
}
