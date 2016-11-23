/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/
#ifndef _OS_AMEMORY_H_
#define _OS_AMEMORY_H_

#include <os/typedefs.h>
#include <common/extapicfg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup AFPPortabilityAPI*/
/*@{*/

/**
 * Allocate memory, with ANSI C "malloc" semantics.
 *
 * @param[in] iulSize Number of bytes to allocate.
 * @retval "Pointer to the allocated memory" Success
 * @retval NULL Failure
 */
EXTAPI_PORTABILITY_H void *aMalloc(size_t iulSize);

/**
 * Resize dynamically allocated memory, with ANSI C "realloc" semantics.
 *
 * @param[in] ipMem Pointer to dynamically allocated memory region to resize.
 * @param[in] iulNewSize New size of memory region.
 * @retval "Pointer to the resized memory" Success
 * @retval NULL Failure
 */
EXTAPI_PORTABILITY_H void *aRealloc(void *ipMem, size_t iulNewSize);

/**
 * Allocate zeroed memory, with ANSI C "calloc" semantics.
 *
 * @param[in] iulMembers Number of objects to allocate memory for.
 * @param[in] iulSize The size of each object.
 * @retval "Pointer to the allocated memory" Success
 * @retval NULL Failure
 */
EXTAPI_PORTABILITY_H void *aCalloc(u32 iulMembers, size_t iulSize);

/**
 * Free dynamically allocted memory, with ANSI C "free" semantics.
 *
 * @param[in] ipMem Pointer to dynamically allocated memory.
 * @retval None
 */
EXTAPI_PORTABILITY_H void  aFree(void *ipMem);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _OS_AMEMORY_H_ */
