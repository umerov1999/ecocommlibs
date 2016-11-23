/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef _OS_AMUTEX_H_
#define _OS_AMUTEX_H_

#include <common/extapicfg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup AFPPortabilityAPI*/
/*@{*/

struct aMutexImpl;

/** Opaque, internal data type representing a mutex. Actual implementation is platform specific. */
typedef struct aMutexImpl aMutex;

/**
 * Create an aMutex object.
 *
 * @retval "Pointer to an initialised aMutex object" Success
 * @retval NULL Failure
 */
EXTAPI_PORTABILITY_H aMutex *aMutexCreate(void);

/**
 * Lock on a mutex.
 *
 * @param[in] iptMutex Pointer to the aMutex to lock.
 */
EXTAPI_PORTABILITY_H void    aMutexLock(aMutex *iptMutex);

/**
 * Release a lock on a mutex.
 *
 * @param[in] iptMutex Pointer to the aMutex to unlock.
 */
EXTAPI_PORTABILITY_H void    aMutexUnlock(aMutex *iptMutex);

/**
 * Destroy a mutex.
 *
 * @param[in] iptMutex Pointer to the aMutex to deallocate.
 */
EXTAPI_PORTABILITY_H void    aMutexDestroy(aMutex *iptMutex);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif // _OS_AMUTEX_H_
