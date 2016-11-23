/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef affcontext_h
#define affcontext_h

#include <common/anotocommon.h>
#include <common/io/anotofile.h>

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * \ingroup AFDModuleAPI
 * \defgroup AFFileContextAPI AFFileContext API
 */
/** @{ */
/** \struct AFFileContext
	\brief An abstract context for a handling files

	The AFFileContext should hide in what context a file is contained. 
@{*/
struct AFFileContext;
/**@}*/

/**@}*/


////////////////////////////////////////////////
// Creation/Destruction of the default context.                                                
////////////////////////////////////////////////

// create a context
EXTAPI_DOCUMENT_H struct AFFileContext* affcCreateContext(const char *iPath, DOC_MOUNT_TYPE itMountType);

// open close API
EXTAPI_DOCUMENT_H s32 affcOpenContext(const char *iPath, DOC_MOUNT_TYPE itMountType, DOC_OPEN_MODE itMode, struct AFFileContext ** opptAFFC);

// Verbose open function.
EXTAPI_DOCUMENT_H s32 affcOpenContextVerbose(const char* izPath,
									  DOC_MOUNT_TYPE itMountType,
									  DOC_OPEN_MODE itMode,
									  struct AFFileContext **opptContext);

// closes a context
EXTAPI_DOCUMENT_H void affcCloseContext(struct AFFileContext** ippAFFC);


//////////////////////////////////////
// Context accessors
//////////////////////////////////////
EXTAPI_DOCUMENT_H DOC_OPEN_MODE affcGetOpenMode(struct AFFileContext* iptAFFC);

EXTAPI_DOCUMENT_H struct AnotoFile* affcCreateFile(struct AFFileContext *iptAFFC, const char* izPath);

EXTAPI_DOCUMENT_H struct InputStream* affcOpenInputStream(struct AFFileContext *iptAFFC, const char* izPath);
EXTAPI_DOCUMENT_H struct OutputStream* affcOpenOutputStream(struct AFFileContext *iptAFFC, const char* izPath);


#ifdef __cplusplus
}
#endif

#endif //affcontext_h
