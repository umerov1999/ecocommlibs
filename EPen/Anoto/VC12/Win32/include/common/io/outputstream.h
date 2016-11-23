/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Mattias Mårtensson

#ifndef _FILE_OUTPUTSTREAM_H_
#define _FILE_OUTPUTSTREAM_H_

// Common includes
#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup File File API */
/** \defgroup OutputStreamAPI OutputStream API
 *  The OutputStream API provides functionality for an OutputStream.  
 @{*/

/** \struct OutputStream
    \brief OutputStream is an abstraction for any data source that can be written
		 one byte at a time.
 
@{ */
struct OutputStream;
/*@}*/

/**
* Prototype for close callback function.
*
* @param[in]	ipptStream	The OutputStream to close
*/
typedef void OutputStreamCloseFunc(struct OutputStream** ipptStream);

/**
 * Prototype for flush callback function.
 *
 * @param[in]	iptStream	The OutputStream to flush
 */
typedef s32 OutputStreamFlushFunc(struct OutputStream *iptStream);

EXTAPI_COMMON_H struct OutputStream* ostrCreate(AnotoWriteFunc* ipWriteFunc, void* ipContext, OutputStreamFlushFunc *ipFlushFunc, OutputStreamCloseFunc* ipClose);

EXTAPI_COMMON_H void ostrClose(struct OutputStream** ipptStream);

// Member access
EXTAPI_COMMON_H AnotoWriteFunc* ostrGetWriteFunc(struct OutputStream *iptStream);
EXTAPI_COMMON_H void* ostrGetContext(struct OutputStream *iptStream);

// Data access
EXTAPI_COMMON_H s32 ostrWrite(struct OutputStream* iptStream,
                              const char* izBuffer,
                              s32 ilBufferLength);

// Flush
EXTAPI_COMMON_H s32 ostrFlush(struct OutputStream *iptStream);

/*@}*/
/*@}*/

#ifdef __cplusplus
}
#endif


#endif



