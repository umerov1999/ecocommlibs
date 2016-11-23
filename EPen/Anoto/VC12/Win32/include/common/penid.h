/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/
/**
 * penid.h - conversion between number and string format for pen id.
 * The string format is called pen serial. Ex: TMJ-3C3-3YD-GE
 * The number format is called pen id. Ex: 0x69772B73A83F
 */

#ifndef penid_included_h
#define penid_included_h

#include <common/anotocommon.h>
#include <os/typedefs.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define PEN_SERIAL_SIZE		15	//!< Size of pen serial string including three "-" (3 + 1 + 3 + 1 + 3 + 1 + 2) and null termination

#define INVALID_PEN_ID_ERROR	-1	//!< Error code returned if an invalid pen id or pen serial was received

#define PEN_ID_UNDEFINED		ULL(0x0000000000000000)	//!< Value used to indicate an undefined pen id (note: this is a penid value, not an error code)
#define PEN_ID_INVALID			ULL(0xFFFFFFFFFFFFFFFE)	//!< Value used to indicate an invalid pen id (note: this is a penid value, not an error code)
#define PEN_ID_WILDCARD			PEN_ID_UNDEFINED

EXTAPI_COMMON_H s32 penSerialToId(const char *izSerial, u64 *opullId);
EXTAPI_COMMON_H s32 penIdToSerial(u64 iullId, char *ozSerial, s32 ilSize);

#ifdef __cplusplus
}
#endif

#endif // penid_included_h
