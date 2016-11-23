/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Mattias Mårtensson

#ifndef _FILE_ANOTOFILE_H_
#define _FILE_ANOTOFILE_H_

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** \ingroup Anotocommon */
/** \defgroup File File API
	File is an abstraction for file like data sources. 
@{*/

	/** \struct FileIterator
	\brief A File iterator

	A FileIterator enables iteration over the files in a directory.

	Currently there are some limitations on what can be done with a
	FileIterator. See fileCreateFileIterator() for details.

	@{*/
struct FileIterator;
	/*@}*/

	/** \struct AnotoFile
	\brief An Anoto file

	An Anoto file is a member of an \ref AnotoDocument.
	@{*/
struct AnotoFile;
	/*@}*/

/*@}*/

#define ANOTO_FILE_SEPARATOR		'/'
#define ANOTO_FILE_SEPARATOR_STRING "/"

///////////////////////////////////////////
// Error codes.
///////////////////////////////////////////

/**
 * \ingroup ErrorCodeRanges
 * \defgroup FileErrorCodes File Error codes
 * @{*/

/** Trying to output data to a file that is opened in read-only mode.*/
#define ANOTO_FILE_IS_READ_ONLY -1300
/*@}*/

///////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////

// Call this to destroy any AnotoFile!
EXTAPI_COMMON_H void fileDestroy(struct AnotoFile** iopptFile);

////////////////////////////////////////////
// Accessors
////////////////////////////////////////////
EXTAPI_COMMON_H const char* fileGetPath(struct AnotoFile *iptFile);

EXTAPI_COMMON_H const char* fileGetFileName(struct AnotoFile *iptFile);

EXTAPI_COMMON_H boolean fileIsDirectory(struct AnotoFile* iptFile);

EXTAPI_COMMON_H boolean fileExists(struct AnotoFile* iptFile);


////////////////////////////////////////////
// File operations (both files and dirs)
////////////////////////////////////////////
EXTAPI_COMMON_H s32 fileRemove(struct AnotoFile* iptFile);

EXTAPI_COMMON_H s32 fileRemoveRecursive(struct AnotoFile *iptFile);

EXTAPI_COMMON_H s32 fileCopy(struct AnotoFile* iptDestFile, struct AnotoFile* iptSrcFile);

EXTAPI_COMMON_H s32 fileMove(struct AnotoFile* iptDestFile, struct AnotoFile* iptSrcFile);

/////////////////////////////////////////////
// File only (not directories)
/////////////////////////////////////////////
EXTAPI_COMMON_H struct InputStream* fileOpenInputStream(struct AnotoFile* iptFile);

EXTAPI_COMMON_H struct OutputStream* fileOpenOutputStream(struct AnotoFile* iptFile);

EXTAPI_COMMON_H struct OutputStream* fileOpenOutputStreamAppend(struct AnotoFile* iptFile);


/////////////////////////////////////////////
// Directory only (not files)
/////////////////////////////////////////////
EXTAPI_COMMON_H struct FileIterator* fileCreateFileIterator(struct AnotoFile* iptFile);

EXTAPI_COMMON_H s32 fileMakeDirectories(struct AnotoFile* iptFile);

EXTAPI_COMMON_H struct AnotoFile *fileCreateLocalFile(struct AnotoFile *iptFile, const char* izRelPath);

EXTAPI_COMMON_H s32 fileRemoveAllFilesInDir(struct AnotoFile *iptFile);


//////////////////////////////////////////////
// FileIterator
//////////////////////////////////////////////


/////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////

// Call this function to destroy a FileIterator
EXTAPI_COMMON_H void fileIteratorDestroy(struct FileIterator** ipptIter);


////////////////////////////////////
// Access functions
////////////////////////////////////

// Returns next file in set and steps iterator one step ahead.
EXTAPI_COMMON_H struct AnotoFile* fileIteratorGetNext(struct FileIterator* iptIter);

EXTAPI_COMMON_H boolean fileIteratorEnd(struct FileIterator* iptIter);

#ifdef __cplusplus
}
#endif


#endif



