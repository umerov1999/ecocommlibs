/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef AFDINFO_H
#define AFDINFO_H

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <common/io/inputstream.h>
#include <common/io/outputstream.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup DocAPI 
    \defgroup DIAPI Document Info API
 The Info API provides some special easy accessible info about the AFD file, see \ref DI.

 Note: Use \ref AnotoDocument API to get the document info struct for an AFD file. 

@{*/

	/** \struct DocumentInfo
		\brief AFD Document Info

		Contains global info about the document such as version number and author, see \ref DI.
	@{*/
		struct DocumentInfo;
	/*@}*/

#define DOC_INFO_VERSION "1.0"

#define DOC_INFO_ERROR -1

/** The maximum length of strings used as input do this module (excluding NUL termination). 
	The size is chosen to be big enough to contain a path string. */
#define DOC_INFO_MAX_STR_LEN 255

///////////////////
// Construcor / destructor
////////////////////

// Create
EXTAPI_DOCUMENT_H struct DocumentInfo *diCreate( void );

// Destroy
EXTAPI_DOCUMENT_H void diDestroy( struct DocumentInfo **ipptDI );


////////////////////
// Get
////////////////////

// if returns true the diSerialize needs to be called before closing file
EXTAPI_DOCUMENT_H boolean diIsDirty( struct DocumentInfo *iptDI );

//Gets the File type (afd,aft,afs etc)
EXTAPI_DOCUMENT_H boolean diGetFileType( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Gets AFD format version - globally
EXTAPI_DOCUMENT_H boolean diGetAFDVersion( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Gets Info file version
EXTAPI_DOCUMENT_H boolean diGetInfoVersion( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Get the Globally Unique Id for the document
EXTAPI_DOCUMENT_H boolean diGetGUID( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Get the URI where the document shall be routed
EXTAPI_DOCUMENT_H boolean diGetTargetURI( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Get owner
EXTAPI_DOCUMENT_H boolean diGetOwner( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Get title
EXTAPI_DOCUMENT_H boolean diGetTitle( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Get author
EXTAPI_DOCUMENT_H boolean diGetAuthor( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Gets background DPIs if any
EXTAPI_DOCUMENT_H boolean diGetBackgrounds( struct DocumentInfo *iptDI, char *ozResult, s32 ilBuffersize );

// Gets the lowest page address in the document
EXTAPI_DOCUMENT_H u64 diGetPageStart( struct DocumentInfo *iptDI );

// Gets the highest page address in the document
EXTAPI_DOCUMENT_H u64 diGetPageStop( struct DocumentInfo *iptDI );

// Gets the current copy to be printed in the document
EXTAPI_DOCUMENT_H s32 diGetCurrentCopy( struct DocumentInfo *iptDI );

// Gets the total number of copies 
EXTAPI_DOCUMENT_H s32 diGetTotalCopies( struct DocumentInfo *iptDI );

// Get a custom parameter
EXTAPI_DOCUMENT_H s32 diGetCustomParameter(struct DocumentInfo *iptDI, const char* izKey, const char** oppValue);

/////////////////////
// Set
/////////////////////

// makes this a dirty copy and needs to be serialized (automatically done with all set functions)
EXTAPI_DOCUMENT_H void diSetDirty( struct DocumentInfo *iptDI );

// Set the file type (afs, aft, afd)
EXTAPI_DOCUMENT_H boolean diSetFileType( struct DocumentInfo *iptDI, char *izValue );

// Set info file version
EXTAPI_DOCUMENT_H boolean diSetInfoVersion( struct DocumentInfo *iptDI, char *izValue );

// Set document Globally Unique ID
EXTAPI_DOCUMENT_H boolean diSetGUID( struct DocumentInfo *iptDI, char *izValue );

// Set the target URI - mainly for routing 
EXTAPI_DOCUMENT_H boolean diSetTargetURI( struct DocumentInfo *iptDI, char *izValue );

// Set owner 
EXTAPI_DOCUMENT_H boolean diSetOwner( struct DocumentInfo *iptDI, char *izValue );

// Set title
EXTAPI_DOCUMENT_H boolean diSetTitle( struct DocumentInfo *iptDI, char *izValue );

// Set author
EXTAPI_DOCUMENT_H boolean diSetAuthor( struct DocumentInfo *iptDI, char *izValue );

// Set background images DPI
EXTAPI_DOCUMENT_H boolean diSetBackgrounds( struct DocumentInfo *iptDI, char *izValue );

// Set lowest page address 
EXTAPI_DOCUMENT_H boolean diSetPageStart( struct DocumentInfo *iptDI, u64 iullPageStart );

// Set highest page address
EXTAPI_DOCUMENT_H boolean diSetPageStop( struct DocumentInfo *iptDI, u64 iullPageStop );

// Set current copy (will be printed next)
EXTAPI_DOCUMENT_H boolean diSetCurrentCopy( struct DocumentInfo *iptDI, s32 ilCurrentCopy );


// Set total number of copies
EXTAPI_DOCUMENT_H boolean diSetTotalCopies( struct DocumentInfo *iptDI, s32 ilTotalCopies );

// Set a custom parameter
EXTAPI_DOCUMENT_H s32 diSetCustomParameter(struct DocumentInfo *iptDI, const char* izKey, const char* izValue);

// Remove a custom parameter
EXTAPI_DOCUMENT_H s32 diRemoveCustomParameter(struct DocumentInfo *iptDI, const char* izKey);

/////////////////
// Serialize / Deserialize
/////////////////

// Load
EXTAPI_DOCUMENT_H struct DocumentInfo *diLoad(struct InputStream *iptStream);

// Save
EXTAPI_DOCUMENT_H s32 diSave( struct DocumentInfo *iptDI, struct OutputStream *iptStream);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif
