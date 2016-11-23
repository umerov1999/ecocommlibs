/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Mattias Mårtensson

#ifndef _FILE_STDIOFILE_H_
#define _FILE_STDIOFILE_H_

// Common includes
#include <os/typedefs.h>

#include "anotofile.h"
#include "inputstream.h"
#include "outputstream.h"

#ifdef __cplusplus
extern "C"
{
#endif


/** \ingroup File AStdioFile */
/** \defgroup AStdioFileAPI Anoto Standard File API

 File is an abstraction for file like data sources. AStdioFile implements
 this for Anoto Standard Input and Output (aStdio). Use stdFileCreate to 
 get an AnotoFile that operates on the local file system through aStdio.
@{*/

///////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////
EXTAPI_COMMON_H struct AnotoFile* stdFileCreate(const char* izFileName);
EXTAPI_COMMON_H s32 stdFileCreateVerbose(const char *izFileName, DOC_OPEN_MODE itMode, struct AnotoFile** opptFile);


EXTAPI_COMMON_H struct InputStream* stdFileOpenInputStream(const char* izFileName);
EXTAPI_COMMON_H struct OutputStream* stdFileOpenOutputStream(const char* izFileName);

/*@}*/
/*@}*/

#ifdef __cplusplus
}
#endif


#endif



