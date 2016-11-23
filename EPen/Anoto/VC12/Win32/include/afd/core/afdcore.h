/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef AFD_H
#define AFD_H

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <common/io/anotofile.h>

#include <areaengine/areacollection.h>
#include <areaengine/strokecollection.h>

#include <afd/info/documentinfo.h>
#include <afd/transaction/transaction.h>

#include <afd/core/affcontext.h>

#ifdef __cplusplus
extern "C"
{
#endif

// The current version of the AFD format
#define AFD_CURRENT_VERSION "2.5"
// the maximum length of the version string is 16 characters
#define AFD_VERSION_MAX_LENGTH 16

/** \ingroup ErrorCodeRanges
    \defgroup AfdCoreErrorCodes Anoto Document Error codes
@{ */
/** The indata is either empty or contains only strokes that already exist in the afd. */
#define AFD_CORE_NO_UNIQUE_STROKES
/*@}*/

/** \ingroup DocAPI */
/** \defgroup AFDModuleAPI AFD API
 Low level API intended to be used when doing unusual things with an AFD. This API gives more flexibility but also requires some knowledge 
 of the internal structure of an AFD for getting it right.
 
 This API should only be used if you are planning to to something which is not possible with higher level APIs. Before using functions in this
 API look for similar functions in \ref AnotoDocument, \ref PageTemplate and \ref PageInstance.
@{*/

	/** \struct AFDCoreDoc
		\brief
		Object representation of a Anoto Functionality Document file. Layered between the \ref AnotoDocument and
		the \ref AFFileContext.
	@{*/

		struct AFDCoreDoc;

	/*@}*/

// Forward declaration needed to fix circular include
struct Transaction;


//////////////////////////////
//	Construction
//////////////////////////////

// creates a new core document need to call afdSerialize document afterwards
EXTAPI_DOCUMENT_H struct AFDCoreDoc *afdCreateNewAFD( const char *iPath, DOC_MOUNT_TYPE itDocType );

// open an existing core document
EXTAPI_DOCUMENT_H struct AFDCoreDoc *afdOpen( const char *iPath, DOC_MOUNT_TYPE itMountType );

// open an existing core document.
EXTAPI_DOCUMENT_H s32 afdOpenVerbose(const char* izPath, 
									  DOC_MOUNT_TYPE itMountType,
									  DOC_OPEN_MODE itMode,
									  struct AFDCoreDoc** opptDoc);

// open the core document
EXTAPI_DOCUMENT_H void afdClose( struct AFDCoreDoc**ipptPD );


//////////////////////////////
//	Serialization
//////////////////////////////

// Store (saves) strokes to the afd
EXTAPI_DOCUMENT_H boolean afdSaveStrokes(struct AFDCoreDoc *iptAFD, struct StrokeCollection *iptAC,
										  struct Transaction *iptTransaction );

EXTAPI_DOCUMENT_H boolean afdSaveStrokesVerbose(struct AFDCoreDoc *iptAFD, 
												struct StrokeCollection *iptAC, 
												struct Transaction *iptTransaction,
												boolean ieSaveVerbose);

// Store transactions to the afd
EXTAPI_DOCUMENT_H boolean afdStoreTransaction(struct AFDCoreDoc *iptAFD,
											  char *ipBuffer, s32 ilBufferLength, char *ipType, u64 iullPA,
											  u64 iullPenId, u32 iulTimeStampLow, u32 iulTimeStampHigh );


/////////////////////////////////////////////////////
// Document info convenience. Use for routing!
/////////////////////////////////////////////////////

// Load info data from the afd
EXTAPI_DOCUMENT_H struct DocumentInfo *afdLoadInfo(struct AFDCoreDoc *iptAFD );

// Save info data to the afd
EXTAPI_DOCUMENT_H s32 afdSaveInfo( struct AFDCoreDoc *iptAFD, struct DocumentInfo *iptDI );

//////////////////////////////
//	Resources
//////////////////////////////

// Returns an AnotoFile that can be used to add/remove/read (layout) resources.
EXTAPI_DOCUMENT_H struct AnotoFile *afdCreateResourceFile(struct AFDCoreDoc *iptAFD, const char *izResourcePath);

// Returns an AnotoFile that can be used to add/remove/read user data.
EXTAPI_DOCUMENT_H struct AnotoFile *afdCreateUserDataFile(struct AFDCoreDoc *iptAFD, const char *izUserDataPath);

//////////////////////////////////
//	Low-level file handling API
//////////////////////////////////

// Use with care. Accessing files in AFD could cause file corruption.
EXTAPI_DOCUMENT_H struct AnotoFile *afdCreateFile(struct AFDCoreDoc *iptAFD, const char* izFileName);

////////////////////////////////////////////
//	Low level API for getting the context
////////////////////////////////////////////

// Gets the lowest afd API level, which operates directly towards the zip interface (use on own risk)
EXTAPI_DOCUMENT_H struct AFFileContext *afdGetFileContext(struct AFDCoreDoc *iptAFD);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif

