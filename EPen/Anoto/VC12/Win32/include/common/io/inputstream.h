/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Mattias Mårtensson

#ifndef _FILE_INPUTSTREAM_H_
#define _FILE_INPUTSTREAM_H_

// Common includes
#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** \ingroup File File API */
/** \defgroup InputStreamAPI InputStream API
 *  The InputStream API provides functionality for an InputStream.
 @{*/
 
/** \struct InputStream
    \brief An InputStream is an abstraction for any data source that can be read one
     byte at a time.
 
@{ */
struct InputStream;
/*@}*/

/**
 * Prototype for close callback function.
 *
 * @param[in]	ipptStream	The InputStream to close
 */
typedef void InputStreamCloseFunc(struct InputStream** ipptStream);
 
// Construction 
EXTAPI_COMMON_H struct InputStream* istrCreate(AnotoReadFunc* ipReadFunc, void* ipContext, InputStreamCloseFunc* ipClose);

EXTAPI_COMMON_H void istrClose(struct InputStream** ipptStream);

// Member access
EXTAPI_COMMON_H AnotoReadFunc* istrGetReadFunc(struct InputStream *iptStream);
EXTAPI_COMMON_H void* istrGetContext(struct InputStream *iptStream);

// Data access
EXTAPI_COMMON_H s32 istrRead(struct InputStream* iptStream, char* ozBuffer, s32 ilBufferLength);

/*@}*/
/*@}*/

#ifdef __cplusplus
}
#endif


#endif



