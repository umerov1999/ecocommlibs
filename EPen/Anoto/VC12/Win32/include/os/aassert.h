/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef _OS_AASSERT_H_
#define _OS_AASSERT_H_

#include <os/typedefs.h>
#include <common/extapicfg.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \addtogroup AFPPortabilityAPI*/
/*@{*/

/**
 * Assert functionality for Anoto's portability API.
 * 
 * AASSERT(expr). Debug assertion. 
 * Displays a message and halts execution if expr is false in debug builds.
 * Does nothing in release builds.
 * 
 * AASSERT_PTR(ptr). Pointer validation.
 * Displays a message and halts execution if ptr is not valid for reading and 
 * writing in debug mode. Does nothing in release mode.
 * 
 */

/**
 * Internal function called when an assertion fails. This function calls the
 * installed assertion failure handler if one has been installed, and a default
 * assertion failure handler otherwise.
 *
 * @param[in] izExpr The asserted expression.
 * @param[in] izFile The name of the file containing izExpr.
 * @param[in] ilLine The line number of the failing assertion.
 */
EXTAPI_PORTABILITY_H void aAssertFailed(char *izExpr, const char *izFile, s32 ilLine);

/**
 * Validate a pointer value.
 * Whether a pointer can be successfully validated is platform
 * dependent. This assertion is only guaranteed to verify that a
 * pointer is not NULL, but may provide additional validation
 * on platforms capable of doing so.
 *
 * @param[in] ipPtr The pointer to examine.
 *
 * @retval TRUE Success
 * @retval FALSE Failure
 */
EXTAPI_PORTABILITY_H boolean aValidPtr(void *ipPtr);

/** AASSERT macro. */
#ifdef _DEBUG
#define AASSERT(expr) (void)((expr) || (aAssertFailed(#expr, __FILE__, __LINE__), 0))
#else
#define AASSERT(expr)
#endif
/** AASSERT_PTR macro. */
#define AASSERT_PTR(ptr) AASSERT(aValidPtr(ptr))

/**
 * Type of the assertion failure handler function.
 *
 * @param[in] izExpr The asserted expression.
 * @param[in] izFile The name of the file containing izExpr.
 * @param[in] ilLine The line number of the failing assertion.
 */
typedef void aAssertionFailedHandler(char *izExpr, const char *izFile, s32 ilLine);

/**
 * The default assertion failure handler. Actual behaviour is platform specific. For explicit control over assertion behaviour, implement and register a custom assertion failure handler (using \ref aSetAssertionFailedHandler()).

 @param[in] izExpr The asserted expression.
 @param[in] izFile The name of the file containing izExpr.
 @param[in] ilLine The line number of the failing assertion.
 */
EXTAPI_PORTABILITY_H void aDefaultAssertionFailedHandler(char *izExpr, const char *izFile, s32 ilLine);

/**
 * Install a custom assertion failure handler.
 *
 * @param[in] pfHandlerFunc Pointer to the assertion failure handler to install.
 */
EXTAPI_PORTABILITY_H void aSetAssertionFailedHandler(aAssertionFailedHandler *pfHandlerFunc);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif // _OS_AASSERT_H_
