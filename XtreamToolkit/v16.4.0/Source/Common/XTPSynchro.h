// XTPSynchro.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNCHRO_H__)
#define __XTPSYNCHRO_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{AFX_CODEJOCK_PRIVATE
#ifdef _DEBUG
#	define XTP_ASYNC_ASSERT(expr) if (!(expr)) DebugBreak();
#else
#	define XTP_ASYNC_ASSERT(expr)
#endif

#define XTP_SYNC_MAX_EXPONENTIAL_TIMEOUT 0x400

namespace XTPDetails {
	struct XTPSynchroPrivateUseTag {};
}

//}}AFX_CODEJOCK_PRIVATE

//---------------------------------------------------------------------------
// Summary:
//     Implements nonamed simple and lightweight synchronization event primitive.
//---------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPSimpleEvent
{
	volatile long m_bState; // Current event state (1 - signaled, 0 - non-signaled)

public:
	//---------------------------------------------------------------------------
	// Summary:
	//     Constructs an even object.
	// Parameters:
	//     bInitialState - TRUE to construct an event in signaled state. FALSE
	//                     to construct an event in non-signaled state.
	//---------------------------------------------------------------------------
	explicit CXTPSimpleEvent(BOOL bInitialState);

	//---------------------------------------------------------------------------
	// Summary:
	//     Resets an event to non-signaled state.
	//---------------------------------------------------------------------------
	void Reset();

	//---------------------------------------------------------------------------
	// Summary:
	//     Sets an event to signaled state.
	//---------------------------------------------------------------------------
	void Set();

	//---------------------------------------------------------------------------
	// Summary:
	//     Determines if an event in signaled state.
	// Rertuns:
	//     TRUE if an event in signaled state, FALSE otherwise.
	//---------------------------------------------------------------------------
	BOOL IsSignaled() const;

	//---------------------------------------------------------------------------
	// Summary:
	//     Waits infinitly for an event to enter signaled states.
	//---------------------------------------------------------------------------
	void Wait();

	//{{AFX_CODEJOCK_PRIVATE
private:
	// noncopyable
	CXTPSimpleEvent(const CXTPSimpleEvent&);
	CXTPSimpleEvent& operator = (const CXTPSimpleEvent&);
	//}}AFX_CODEJOCK_PRIVATE
};



//---------------------------------------------------------------------------
// Summary:
//     Implements simple and lightweight synchronization critical section primitive.
//     Multi-core and multi-CPU safe.
//---------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPSimpleCriticalSection
{
	volatile long m_nLock;             // Not 0 if the critical section is locked
	volatile long m_nRecursionCounter; // Recursive lock counter
	volatile DWORD m_dwThreadId;       // The owner thread's ID
public:

	//---------------------------------------------------------------------------
	// Summary:
	//     Construct critical section.
	//---------------------------------------------------------------------------
	CXTPSimpleCriticalSection()
		: m_nLock(0)
		, m_nRecursionCounter(0)
		, m_dwThreadId(0)
	{
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Handles critical section destruction.
	//---------------------------------------------------------------------------
	~CXTPSimpleCriticalSection();

	//---------------------------------------------------------------------------
	// Summary:
	//     Enters critical section. If another thread has already entered the
	//     same critical section, the calling thread will be suspended until
	//     the critical section is fully released by another thread.
	//     Enterring a critical section more then one time on the same thread 
	//     only increases recusion counter.
	//---------------------------------------------------------------------------
	void Enter();

	//---------------------------------------------------------------------------
	// Summary:
	//     Releases the critical section. In case of re-entry on the same thread
	//     the internal recusion counter is decreased without releasing the 
	//     critical section.
	//---------------------------------------------------------------------------
	void Leave();

	//---------------------------------------------------------------------------
	// Summary:
	//     A helper class for locking a scope or providing exception safe locking.
	//---------------------------------------------------------------------------
	class CLock
	{
		CXTPSimpleCriticalSection& m_cs;
	public:
		//---------------------------------------------------------------------------
		// Summary:
		//     Locks a critical section passed.
		// Parameters:
		//     cs - A reference of critical section to lock.
		//---------------------------------------------------------------------------
		explicit CLock(CXTPSimpleCriticalSection& cs)
			: m_cs(cs)
		{
			m_cs.Enter();
		}

		//---------------------------------------------------------------------------
		// Summary:
		//     Unlocks the previously locked critical.
		//---------------------------------------------------------------------------
		~CLock()
		{
			m_cs.Leave();
		}
	};

	//{{AFX_CODEJOCK_PRIVATE
private:
	// noncopyable
	CXTPSimpleCriticalSection(const CXTPSimpleCriticalSection&);
	CXTPSimpleCriticalSection& operator = (const CXTPSimpleCriticalSection&);
	//}}AFX_CODEJOCK_PRIVATE
};

//---------------------------------------------------------------------------
// Summary:
//     Implements recursive read-write lock synchronization pattern.
//---------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPRWCriticalSection
{
public:
	//---------------------------------------------------------------------------
	// Summary:
	//     Constructs read-write lock critical section.
	//---------------------------------------------------------------------------
	CXTPRWCriticalSection();

	//---------------------------------------------------------------------------
	// Summary:
	//     Handles read-write lock critical section. destruction.
	//---------------------------------------------------------------------------
	~CXTPRWCriticalSection();

public:
	class CSharedLock;
	class CExclusiveLock;

	//---------------------------------------------------------------------------
	// Summary:
	//     Determines if critical section is currently locked in shared mode.
	// Returns:
	//     TRUE if critical section is locked in shared mode, otherwise FALSE.
	//     FALSE is also returned if critical section has been previously locked
	//     exclusively in the same thread, regardless whether shared mode lock has
	//     been obtained later or not.
	//---------------------------------------------------------------------------
	BOOL IsLockedSharedly() const volatile;

	//---------------------------------------------------------------------------
	// Summary:
	//     Determines if critical section is currently locked exclusively.
	// Returns:
	//     TRUE if critical section is locked in exclusive mode, otherwise FALSE.
	//---------------------------------------------------------------------------
	BOOL IsLockedExclusively() const volatile;

	//---------------------------------------------------------------------------
	// Summary:
	//     Enters shared lock mode. If the critical section is currently locked 
	//     exclusively in another thread, the call will be blocked until exclusive
	//     lock is released. If the critical section is currently locked
	//     exclusively in the current thread the function returns immediately without
	//     acquiring shared lock.
	//---------------------------------------------------------------------------
	void LockShared();

	//---------------------------------------------------------------------------
	// Summary:
	//     Leaves shared lock mode.
	//---------------------------------------------------------------------------
	void UnlockShared();

	//---------------------------------------------------------------------------
	// Summary:
	//     Enters exclusive lock mode. If the critical section is currently locked 
	//     sharedly in another thread, the call will be blocked until all shared
	//     lock are released. If the critical section is currently locked
	//     sharedly in the current thread this state is cached and then exclusive
	//     lock gets acquired. Once exclusive lock is released, the cached shared lock
	//     state for the current thread is restored.
	//---------------------------------------------------------------------------
	void LockExclusive();

	//---------------------------------------------------------------------------
	// Summary:
	//     Leaves exclusive lock mode.
	//---------------------------------------------------------------------------
	void UnlockExclusive();

	//{{AFX_CODEJOCK_PRIVATE
private:
	void PushLocalSharedLockState();
	void PopSharedLockState();
	void LockShared(BOOL bPopMode);
	void UnlockShared(BOOL bPushMode);
	long IncrementLocalSharedLockCounter();
	long DecrementLocalSharedLockCounter();
	long GetLocalSharedLockCounter() const;

private:
	static const DWORD m_dwLocalSharedLockCounterTlsIdx; // TLS index for thread lock state data

	volatile long m_nSharedLockCounter; // Number of shared locks obtained.
	volatile long m_nExclusiveLockCounter; // Number of exclusive locks obtained.
	volatile DWORD m_dwExclusiveLockTid; // Thread ID from where exclusive lock has been obtained.

	CXTPSimpleCriticalSection m_csGeneral; // Internal data access guard.
	CXTPSimpleEvent m_sharedLockEvent; // Shared lock event

	typedef CMap<const CXTPRWCriticalSection*, const CXTPRWCriticalSection*, long, long> LocalSharedLockCounterMap;
	//}}AFX_CODEJOCK_PRIVATE
};

//---------------------------------------------------------------------------
// Summary:
//     A convenience class for obtaining shared lock for a scope.
//---------------------------------------------------------------------------
class CXTPRWCriticalSection::CSharedLock
{
public:
	//---------------------------------------------------------------------------
	// Summary:
	//     Constructs shared lock instance, obtains shared lock on critical section provided.
	// Parameters:
	//     cs - Critical section for which to obtain shared lock.
	//---------------------------------------------------------------------------
	explicit CSharedLock(CXTPRWCriticalSection& cs) : m_pCS(&cs)
	{
		m_pCS->LockShared();
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Destructs shared lock instance, releases shared lock on critical section 
	//     provided during construction.
	//---------------------------------------------------------------------------
	~CSharedLock()
	{
		if (NULL != m_pCS)
		{
			m_pCS->UnlockShared();
		}
	}

	//{{AFX_CODEJOCK_PRIVATE
public:
	CSharedLock(
		CXTPRWCriticalSection* pCS,
		XTPDetails::XTPSynchroPrivateUseTag) 
		: m_pCS(pCS)
	{
		if (NULL != m_pCS)
		{
			m_pCS->LockShared();
		}
	}

private:
	CSharedLock(const CSharedLock&); // deleted

private:
	CXTPRWCriticalSection* m_pCS;
	//}}AFX_CODEJOCK_PRIVATE
};

//---------------------------------------------------------------------------
// Summary:
//     A convenience class for obtaining exclusive lock for a scope.
//---------------------------------------------------------------------------
class CXTPRWCriticalSection::CExclusiveLock
{
public:
	//---------------------------------------------------------------------------
	// Summary:
	//     Constructs exclusive lock instance, obtains exclusive lock on critical 
	//     section provided.
	// Parameters:
	//     cs - Critical section for which to obtain exclusive lock.
	//---------------------------------------------------------------------------
	explicit CExclusiveLock(CXTPRWCriticalSection& cs) : m_pCS(&cs)
	{
		m_pCS->LockExclusive();
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Destructs exclusive lock instance, releases exclusive lock on critical 
	//     section provided during construction.
	//---------------------------------------------------------------------------
	~CExclusiveLock()
	{
		if (NULL != m_pCS)
		{
			m_pCS->UnlockExclusive();
		}
	}

	//{{AFX_CODEJOCK_PRIVATE
public:
	CExclusiveLock(
		CXTPRWCriticalSection* pCS,
		XTPDetails::XTPSynchroPrivateUseTag) 
		: m_pCS(pCS)
	{
		if (NULL != m_pCS)
		{
			m_pCS->LockExclusive();
		}
	}

private:
	CExclusiveLock(const CExclusiveLock&); // deleted

private:
	CXTPRWCriticalSection* m_pCS;
	//}}AFX_CODEJOCK_PRIVATE
};


//---------------------------------------------------------------------------
// Summary:
//     Obtains shared lock for the current scope.
// Parameters:
//     cs - Critical section for which to obtain shared lock.
//---------------------------------------------------------------------------
#define XTP_RWCS_LOCK_SHARED_SCOPE(cs) \
	CXTPRWCriticalSection::CSharedLock $__xtpRWCSSharedLock(cs)

//---------------------------------------------------------------------------
// Summary:
//     Obtains exclusive lock for the current scope.
// Parameters:
//     cs - Critical section for which to obtain exclusive lock.
//---------------------------------------------------------------------------
#define XTP_RWCS_LOCK_EXCLUSIVE_SCOPE(cs) \
	CXTPRWCriticalSection::CExclusiveLock $__xtpRWCSExclusiveLock(cs)


//---------------------------------------------------------------------------
// Summary:
//     A convenience class for guarding an adapted object reference by
//     a RW-lock provided.
// Parameters:
//     Adaptee - Type of an adapted object reference.
//     RWLock - RW-lock type. Can be either CXTPRWCriticalSection::CSharedLock
//              or CXTPRWCriticalSection::CExclusiveLock.
//---------------------------------------------------------------------------
template<class Adaptee, class RWLock>
class CXTPAsyncGuard : public CXTPBlockStatementBase
{
public:
	//---------------------------------------------------------------------------
	// Summary:
	//     Constructs the guard object.
	// Parameters:
	//     adaptee - A reference to an object to be guarded.
	//     cs - Critical section reference for while a lock is to be obtained.
	//---------------------------------------------------------------------------
	CXTPAsyncGuard(
		Adaptee& adaptee,
		CXTPRWCriticalSection& cs)
		: m_adaptee(adaptee)
		, m_locker(cs)
		, m_pConditionCounter(NULL) {}

	//---------------------------------------------------------------------------
	// Summary:
	//     Constructs the guard object.
	// Parameters:
	//     adaptee - A reference to an object to be guarded.
	//     cs - Critical section reference for while a lock is to be obtained.
	//     nConditionCounter - If zero, the guard will lock the critical section and
	//                         increment the counter until the critical section is unlocked.
	//                         Otherwise critical section does not get locked.
	//---------------------------------------------------------------------------
	CXTPAsyncGuard(
		Adaptee& adaptee,
		CXTPRWCriticalSection& cs,
		long& nConditionCounter)
		: m_adaptee(adaptee)
		, m_locker(
			1 == InterlockedIncrement(&nConditionCounter) ? &cs : NULL,
			XTPDetails::XTPSynchroPrivateUseTag())
		, m_pConditionCounter(&nConditionCounter)
	{
		ASSERT(0 < nConditionCounter);
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Copies the guard object.
	// Parameters:
	//     rhs - A reference to a source guard object.
	//---------------------------------------------------------------------------
	CXTPAsyncGuard(const CXTPAsyncGuard& rhs)
		: m_adaptee(rhs.m_adaptee)
		, m_locker(rhs.m_locker.m_pCS)
		, m_pConditionCounter(rhs.m_pConditionCounter)
	{
		if (NULL != m_pConditionCounter)
		{
			InterlockedIncrement(m_pConditionCounter);
		}
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Handles guard destruction. Unlocks owned critical section. If there was an
	//     option condition counter provided, unlocks critical section only if decrmented
	//     counter value is zero.
	//---------------------------------------------------------------------------
	~CXTPAsyncGuard()
	{
		if (NULL != m_pConditionCounter)
		{
			long nConditionCounter = InterlockedDecrement(m_pConditionCounter);
			ASSERT(0 <= nConditionCounter);
		}
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Accesses an adapted object's pointer.
	// Returns:
	//     An adapted object's pointer.
	//---------------------------------------------------------------------------
	Adaptee* operator -> ()
	{
		return &m_adaptee;
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Accesses an adapted object's pointer.
	// Returns:
	//     An adapted object's pointer.
	//---------------------------------------------------------------------------
	const Adaptee* operator -> () const
	{
		return &m_adaptee;
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Accesses an adapted object's reference.
	// Returns:
	//     An adapted object's reference.
	//---------------------------------------------------------------------------
	Adaptee& operator * ()
	{
		return m_adaptee;
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Accesses an adapted object's reference.
	// Returns:
	//     An adapted object's reference.
	//---------------------------------------------------------------------------
	const Adaptee& operator * () const
	{
		return m_adaptee;
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Casts object to an adapted object's pointer.
	// Returns:
	//     An adapted object's pointer.
	//---------------------------------------------------------------------------
	operator Adaptee* ()
	{
		return &m_adaptee;
	}

	//---------------------------------------------------------------------------
	// Summary:
	//     Casts object to an adapted object's pointer.
	// Returns:
	//     An adapted object's pointer.
	//---------------------------------------------------------------------------
	operator const Adaptee* () const
	{
		return &m_adaptee;
	}

	//{{AFX_CODEJOCK_PRIVATE
private:
	RWLock m_locker;
	Adaptee& m_adaptee;
	long* m_pConditionCounter;
	//}}AFX_CODEJOCK_PRIVATE
};

//{{AFX_CODEJOCK_PRIVATE
namespace XTPDetails {
	template<class T>
	struct XTPAsyncGuardShortcut
	{
		typedef CXTPAsyncGuard<T, CXTPRWCriticalSection::CSharedLock> XTPAsyncSharedGuard;
		typedef CXTPAsyncGuard<T, CXTPRWCriticalSection::CExclusiveLock> XTPAsyncExclusiveGuard;
		typedef CXTPAsyncGuard<const T, CXTPRWCriticalSection::CSharedLock> XTPAsyncConstSharedGuard;
		typedef CXTPAsyncGuard<const T, CXTPRWCriticalSection::CExclusiveLock> XTPAsyncConstExclusiveGuard;
	};

	class CXTPConditionBlock : public CXTPBlockStatementBase
	{
		long& m_nConditionCounter;

	public:
		CXTPConditionBlock(long& nConditionCounter)
			: m_nConditionCounter(nConditionCounter)
		{
			ASSERT(0 <= m_nConditionCounter);
			InterlockedIncrement(&m_nConditionCounter);
		}

		~CXTPConditionBlock()
		{
			long nConditionCounter = InterlockedDecrement(&m_nConditionCounter);
			ASSERT(0 <= nConditionCounter);
		}
	};
}
//}}AFX_CODEJOCK_PRIVATE

//---------------------------------------------------------------------------
// Summary:
//     A base class for a synchronized object.
//---------------------------------------------------------------------------
class CXTPSynchronized
{
public:
	//---------------------------------------------------------------------------
	// Summary:
	//     Accesses a synchronized object's critical section.
	// Returns:
	//     Synchronized object's critical section.
	//---------------------------------------------------------------------------
	CXTPRWCriticalSection& GetAccessCriticalSection() const
	{
		return m_cs;
	}

	//{{AFX_CODEJOCK_PRIVATE
protected:
	CXTPSynchronized() {}

private:
	mutable CXTPRWCriticalSection m_cs;
	//}}AFX_CODEJOCK_PRIVATE
};

//---------------------------------------------------------------------------
// Summary:
//     A convenience function for building a shared-lock guard
//     for an object specified.
// Parameters:
//     pObject - A pointer to a synchronized object to be guarded.
// Returns:
//     A shared-lock guard for an object provided.
//---------------------------------------------------------------------------
template<class T>
AFX_INLINE CXTPAsyncGuard<T, CXTPRWCriticalSection::CSharedLock>
XTPAccessShared(T* pObject)
{
	ASSERT(NULL != pObject);
	return CXTPAsyncGuard<T, CXTPRWCriticalSection::CSharedLock>(*pObject,
		pObject->CXTPSynchronized::GetAccessCriticalSection());
}

//---------------------------------------------------------------------------
// Summary:
//     A convenience function for building a shared-lock guard
//     for an object specified.
// Parameters:
//     pObject - A pointer to a synchronized object to be guarded.
//     nConditionCounter - If zero, the guard will lock the critical section and
//                         increment the counter until the critical section is unlocked.
//                         Otherwise critical section does not get locked.
// Returns:
//     A shared-lock guard for an object provided.
//---------------------------------------------------------------------------
template<class T>
AFX_INLINE CXTPAsyncGuard<T, CXTPRWCriticalSection::CSharedLock>
XTPAccessShared(T* pObject, long& nConditionCounter)
{
	ASSERT(NULL != pObject);
	return CXTPAsyncGuard<T, CXTPRWCriticalSection::CSharedLock>(*pObject,
		pObject->CXTPSynchronized::GetAccessCriticalSection(),
		nConditionCounter);
}

//---------------------------------------------------------------------------
// Summary:
//     A convenience function for building an exclusive-lock guard
//     for an object specified.
// Parameters:
//     pObject - A pointer to a synchronized object to be guarded.
// Returns:
//     An exclusive-lock guard for an object provided.
//---------------------------------------------------------------------------
template<class T>
AFX_INLINE CXTPAsyncGuard<T, CXTPRWCriticalSection::CExclusiveLock>
XTPAccessExclusive(T* pObject)
{
	ASSERT(NULL != pObject);
	return CXTPAsyncGuard<T, CXTPRWCriticalSection::CExclusiveLock>(*pObject,
		pObject->CXTPSynchronized::GetAccessCriticalSection());
}

//---------------------------------------------------------------------------
// Summary:
//     A convenience function for building an exclusive-lock guard
//     for an object specified.
// Parameters:
//     pObject - A pointer to a synchronized object to be guarded.
//     nConditionCounter - If zero, the guard will lock the critical section and
//                         increment the counter until the critical section is unlocked.
//                         Otherwise critical section does not get locked.
// Returns:
//     An exclusive-lock guard for an object provided.
//---------------------------------------------------------------------------
template<class T>
AFX_INLINE CXTPAsyncGuard<T, CXTPRWCriticalSection::CExclusiveLock>
XTPAccessExclusive(T* pObject, long& nConditionCounter)
{
	ASSERT(NULL != pObject);
	return CXTPAsyncGuard<T, CXTPRWCriticalSection::CExclusiveLock>(*pObject,
		pObject->CXTPSynchronized::GetAccessCriticalSection(),
		nConditionCounter);
}

//---------------------------------------------------------------------------
// Summary:
//     Creates a shared-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     Guard - Guard variable name
//---------------------------------------------------------------------------
#define XTP_GUARD_SHARED_(Type, Object, Guard) \
	XTP_BLOCKSTATEMENT(::XTPDetails::XTPAsyncGuardShortcut<Type>::XTPAsyncSharedGuard, Guard, XTPAccessShared(Object))

//---------------------------------------------------------------------------
// Summary:
//     Creates a shared-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     Guard - Guard variable name
//     ConditionCounter - If zero, the guard will lock the critical section and
//                        increment the counter until the critical section is unlocked.
//                        Otherwise critical section does not get locked.
//---------------------------------------------------------------------------
#define XTP_GUARD_SHARED_COND_(Type, Object, Guard, ConditionCounter) \
	XTP_BLOCKSTATEMENT(::XTPDetails::XTPAsyncGuardShortcut<Type>::XTPAsyncSharedGuard, Guard, XTPAccessShared(Object, ConditionCounter))

//---------------------------------------------------------------------------
// Summary:
//     Creates a shared-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//---------------------------------------------------------------------------
#define XTP_GUARD_SHARED(Type, Object) \
	XTP_GUARD_SHARED_(Type, Object, $__xtpGuard)

//---------------------------------------------------------------------------
// Summary:
//     Creates a shared-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     ConditionCounter - If zero, the guard will lock the critical section and
//                        increment the counter until the critical section is unlocked.
//                        Otherwise critical section does not get locked.
//---------------------------------------------------------------------------
#define XTP_GUARD_SHARED_COND(Type, Object, ConditionCounter) \
	XTP_GUARD_SHARED_COND_(Type, Object, $__xtpGuard, ConditionCounter)

//---------------------------------------------------------------------------
// Summary:
//     Creates a shared-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     Guard - Guard variable name
//---------------------------------------------------------------------------
#define XTP_GUARD_SHARED_CONST_(Type, Object, Guard) \
	XTP_BLOCKSTATEMENT(::XTPDetails::XTPAsyncGuardShortcut<Type>::XTPAsyncConstSharedGuard, Guard, XTPAccessShared(Object))

//---------------------------------------------------------------------------
// Summary:
//     Creates a shared-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     Guard - Guard variable name
//     ConditionCounter - If zero, the guard will lock the critical section and
//                        increment the counter until the critical section is unlocked.
//                        Otherwise critical section does not get locked.
//---------------------------------------------------------------------------
#define XTP_GUARD_SHARED_COND_CONST_(Type, Object, Guard, ConditionCounter) \
	XTP_BLOCKSTATEMENT(::XTPDetails::XTPAsyncGuardShortcut<Type>::XTPAsyncConstSharedGuard, Guard, XTPAccessShared(Object, ConditionCounter))

//---------------------------------------------------------------------------
// Summary:
//     Creates a shared-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//---------------------------------------------------------------------------
#define XTP_GUARD_SHARED_CONST(Type, Object) \
	XTP_GUARD_SHARED_CONST_(Type, Object, $__xtpGuard)

//---------------------------------------------------------------------------
// Summary:
//     Creates a shared-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     ConditionCounter - If zero, the guard will lock the critical section and
//                        increment the counter until the critical section is unlocked.
//                        Otherwise critical section does not get locked.
//---------------------------------------------------------------------------
#define XTP_GUARD_SHARED_COND_CONST(Type, Object, ConditionCounter) \
	XTP_GUARD_SHARED_COND_CONST_(Type, Object, $__xtpGuard, ConditionCounter)

//---------------------------------------------------------------------------
// Summary:
//     Creates an exclusive-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     Guard - Guard variable name
//---------------------------------------------------------------------------
#define XTP_GUARD_EXCLUSIVE_(Type, Object, Guard) \
	XTP_BLOCKSTATEMENT(::XTPDetails::XTPAsyncGuardShortcut<Type>::XTPAsyncExclusiveGuard, Guard, XTPAccessExclusive(Object))

//---------------------------------------------------------------------------
// Summary:
//     Creates an exclusive-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     Guard - Guard variable name
//     ConditionCounter - If zero, the guard will lock the critical section and
//                        increment the counter until the critical section is unlocked.
//                        Otherwise critical section does not get locked.
//---------------------------------------------------------------------------
#define XTP_GUARD_EXCLUSIVE_COND_(Type, Object, Guard, ConditionCounter) \
	XTP_BLOCKSTATEMENT(::XTPDetails::XTPAsyncGuardShortcut<Type>::XTPAsyncExclusiveGuard, Guard, XTPAccessExclusive(Object, ConditionCounter))

//---------------------------------------------------------------------------
// Summary:
//     Creates an exclusive-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//---------------------------------------------------------------------------
#define XTP_GUARD_EXCLUSIVE(Type, Object) \
	XTP_GUARD_EXCLUSIVE_(Type, Object, $__xtpGuard)

//---------------------------------------------------------------------------
// Summary:
//     Creates an exclusive-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     ConditionCounter - If zero, the guard will lock the critical section and
//                        increment the counter until the critical section is unlocked.
//                        Otherwise critical section does not get locked.
//---------------------------------------------------------------------------
#define XTP_GUARD_EXCLUSIVE_COND(Type, Object, ConditionCounter) \
	XTP_GUARD_EXCLUSIVE_COND_(Type, Object, $__xtpGuard, ConditionCounter)

//---------------------------------------------------------------------------
// Summary:
//     Creates an exclusive-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     Guard - Guard variable name
//---------------------------------------------------------------------------
#define XTP_GUARD_EXCLUSIVE_CONST_(Type, Object, Guard) \
	XTP_BLOCKSTATEMENT(::XTPDetails::XTPAsyncGuardShortcut<Type>::XTPAsyncConstExclusiveGuard, Guard, XTPAccessExclusive(Object))

//---------------------------------------------------------------------------
// Summary:
//     Creates an exclusive-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     Guard - Guard variable name
//     ConditionCounter - If zero, the guard will lock the critical section and
//                        increment the counter until the critical section is unlocked.
//                        Otherwise critical section does not get locked.
//---------------------------------------------------------------------------
#define XTP_GUARD_EXCLUSIVE_CONST_COND_(Type, Object, Guard, ConditionCounter) \
	XTP_BLOCKSTATEMENT(::XTPDetails::XTPAsyncGuardShortcut<Type>::XTPAsyncConstExclusiveGuard, Guard, XTPAccessExclusive(Object, ConditionCounter))

//---------------------------------------------------------------------------
// Summary:
//     Creates an exclusive-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//---------------------------------------------------------------------------
#define XTP_GUARD_EXCLUSIVE_CONST(Type, Object) \
	XTP_GUARD_EXCLUSIVE_CONST_(Type, Object, $__xtpGuard)

//---------------------------------------------------------------------------
// Summary:
//     Creates an exclusive-lock guarded scope for an object specified.
// Parameters:
//     Type - Object type
//     Object - Synchronized object pointer
//     ConditionCounter - If zero, the guard will lock the critical section and
//                        increment the counter until the critical section is unlocked.
//                        Otherwise critical section does not get locked.
//---------------------------------------------------------------------------
#define XTP_GUARD_EXCLUSIVE_COND_CONST(Type, Object, ConditionCounter) \
	XTP_GUARD_EXCLUSIVE_COND_CONST_(Type, Object, $__xtpGuard, ConditionCounter)

#define XTP_ENTER_GUARD_CONDITION(ConditionCounter) \
	XTP_BLOCKSTATEMENT(::XTPDetails::CXTPConditionBlock, $__xtpConditionBlock, ConditionCounter)

#endif // !defined(__XTPSYNCHRO_H__)
