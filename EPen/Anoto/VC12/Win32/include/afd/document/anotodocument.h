/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef ANOTO_DOCUMENT_H
#define ANOTO_DOCUMENT_H

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <common/io/anotofile.h>

#include <afd/core/afdcore.h>
#include <afd/info/documentinfo.h>

#include <licensemanager/license/lmlicensereader.h>
#include <licensemanager/license/lmkeypair.h>

#include <areaengine/strokecollection.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Current document/pattern format versions */	
#define DOC_DOCUMENT_CURRENT_VERSION "1.0"
#define DOC_PATTERN_CURRENT_VERSION	 "1.0"

/** \defgroup DocAPI Document Module

 An AnotoDocument is the digital representation of a physical Anoto document with Anoto pattern.\n
 It contains pages normally with:\n\n
 1. A background layout.\n
 2. Additional graphics such as user areas , logos, and images.\n
 3. Anoto pattern (both pattern just for positioning and also to trigger events such as pen-events ("pidgets"))\n\n

 See \ref Doc
@{*/
	/** \struct AnotoDocument
		\brief An Anoto document containing pages with layouts and pattern.

	  An AnotoDocument is the digital representation of a physical Anoto document with Anoto pattern.


		See \ref Doc
	@{*/

	struct AnotoDocument;

	/*@}*/

#if defined(_WIN32)
  #define DOCUMENT_MODULE_VERSION "2.7.1.0_2"
#elif defined(__APPLE__)
  #define DOCUMENT_MODULE_VERSION "2.5.0.0_2"
#else
  #define DOCUMENT_MODULE_VERSION "2.5.0.0_1"	// Other plattform...ARM, Linux
#endif

/** \ingroup ErrorCodeRanges
    \defgroup DocErrorCodes Anoto Document Error codes
@{ */
/** The document's major version is not supported. Document cannot be opened. Version format is major.minor, for example 2.1 */
#define DOC_UNSUPPORTED_MAJOR_VERSION  -1100
/** The document's minor version is not fully supported. Document can only be opened in DOC_OPEN_READ mode. Version format is major.minor, for example 2.1 */
#define DOC_UNSUPPORTED_MINOR_VERSION  -1101
/** The document's version is already current version. For example when trying to convert a document of current version to current version */
#define DOC_ALREADY_CURRENT_VERSION  -1102
/** The specified licensefile could not be read. Either it does not exist or it contains errors. */
#define DOC_INVALID_LICENSE_FILE -1103
/** Trying to add pages that have already been added to this document. Check the contents of the document and see which pages it contains. */
#define DOC_OVERLAPPING_PAGES -1104
/** Trying to add a license to a document that does not contain any pages with pattern. Illegal as the number of copies will be infinite. */
#define DOC_NO_PAGES_WITH_PATTERN -1105
/*@}*/

///////////////////////////
// Open/Close
///////////////////////////

//create a new doc (afd, afs, aft)
EXTAPI_DOCUMENT_H struct AnotoDocument* docCreateNewDoc( const char *izPath, DOC_MOUNT_TYPE itDocType );

//open an existing doc
EXTAPI_DOCUMENT_H struct AnotoDocument* docOpen( const char *izPath, DOC_MOUNT_TYPE itDocType );

//open an existing doc with more options.
EXTAPI_DOCUMENT_H s32 docOpenVerbose(const char* izPath, 
									 DOC_MOUNT_TYPE itDocType, 
									 DOC_OPEN_MODE itDocMode, 
									 struct AnotoDocument **opptDoc);

// Deprecated. Use docSaveAndClose() instead.
EXTAPI_DOCUMENT_H s32 docSave( struct AnotoDocument *iptAD );

//save a doc and close it.
EXTAPI_DOCUMENT_H s32 docSaveAndClose( struct AnotoDocument **ipptAD );

//close doc and destroy doc object
EXTAPI_DOCUMENT_H boolean docClose( struct AnotoDocument **ipptPD );

///////////////////////////
// Pages
///////////////////////////

// Adds/removes pages. Will not execute if document is locked

// Gets a page template given a specific page number.
EXTAPI_DOCUMENT_H struct PageTemplate *docGetReferenceToPageTemplate( struct AnotoDocument *iptAD, s32 ilPageNbr);

// Adds a page template to the doc
EXTAPI_DOCUMENT_H s32 docAddPageTemplate(struct AnotoDocument * iptAD, s32 ilPageno, s32 ilWidth,
                                         s32 ilHeight, boolean ieHasPattern, boolean ieHidden);

// Adds a page template to the doc - verbose (detailed) setting
EXTAPI_DOCUMENT_H s32 docAddPageTemplateVerbose(struct AnotoDocument * iptAD, s32 ilPageno,
                                                char *ipBasePath, char *ipAreafile,
                                                char *ipGfxfile, s32 ilWidth, s32 ilHeight,
                                                boolean ieHasPattern, boolean ieHidden);

// Deletes a page template from the doc
EXTAPI_DOCUMENT_H boolean docDeletePageTemplate( struct AnotoDocument *iptAD, s32 ilPageno,
											  boolean ieUsingVerbosePageSetting);

// Creates a page instance object for the doc.
EXTAPI_DOCUMENT_H struct PageInstance *docCreatePageInstance( struct AnotoDocument *iptAD, s32 ilPage, s32 ilCopy );

// Creates a page instance object for the doc using a specific page address
EXTAPI_DOCUMENT_H struct PageInstance *docCreatePageInstanceFromPA( struct AnotoDocument * iptAD, u64 iullPA );

// Set number of reserved pattern pages per copy
EXTAPI_DOCUMENT_H boolean docSetReservedPatternPages( struct AnotoDocument *iptAD, s32 lPatternPages);

// Get number of reserved pattern pages per copy
EXTAPI_DOCUMENT_H s32 docGetReservedPatternPages( struct AnotoDocument *iptAD);

// Get the page size bounding box
EXTAPI_DOCUMENT_H s32 docGetPageSizeBoundingBox( struct AnotoDocument *iptAD, s32 *oplWidth, s32 *oplHeight );

// Get total number of pages (pattern pages + non pattern pages) per copy
EXTAPI_DOCUMENT_H s32 docGetNumberOfPageTemplates( struct AnotoDocument *iptAD);

// Get number of pages with pattern.
EXTAPI_DOCUMENT_H s32 docGetNumberOfPatternPageTemplates( struct AnotoDocument *iptAD );

// Get number of copies
EXTAPI_DOCUMENT_H s32 docGetNumberOfCopies( struct AnotoDocument *iptAD );

// Get total number of page instances = pages * copies.
EXTAPI_DOCUMENT_H s32 docGetTotalNumberOfPageInstances( struct AnotoDocument *iptAD );


///////////////////////////
// Lock
///////////////////////////

//check if locked
EXTAPI_DOCUMENT_H boolean docIsLocked( struct AnotoDocument *iptAD );

//set locked
EXTAPI_DOCUMENT_H void docLock( struct AnotoDocument *iptAD );

//set unlocked
EXTAPI_DOCUMENT_H void docUnLock( struct AnotoDocument *iptAD );


//////////////////////////////
// Layout resource functions
//////////////////////////////

// Get an AnotoFile representing the layout resource from the layout resources directory in the Anoto document.
EXTAPI_DOCUMENT_H struct AnotoFile* docCreateLayoutResourceFile( struct AnotoDocument *iptAD, const char* izResourcePath);

// Add any kind of layout resource to the Anoto document in the layout resources directory
EXTAPI_DOCUMENT_H s32 docAddLayoutResource(struct AnotoDocument *iptAD, const char *izResourcePath, struct AnotoFile* iptSource);

// Remove layout resource from the Anoto document in the layout resources directory
EXTAPI_DOCUMENT_H s32 docDeleteLayoutResource(struct AnotoDocument *iptAD, const char *izResourcePath);


//////////////////////////////
// User data functions
//////////////////////////////

// Get an AnotoFile representing the user data from the user data directory in the Anoto document.
EXTAPI_DOCUMENT_H struct AnotoFile* docCreateUserDataFile(struct AnotoDocument *iptAD, const char* izUserDataPath);

//Add any kind of user data to the Anoto document in the user data directory
EXTAPI_DOCUMENT_H s32 docAddUserData(struct AnotoDocument *iptAD, const char *izUserDataPath, struct AnotoFile* iptSource);

//Remove user data from the Anoto document in the user data directory
EXTAPI_DOCUMENT_H s32 docDeleteUserData(struct AnotoDocument *iptAD, const char *izUserDataPath);


//////////////////////////////
// Strokes 
//////////////////////////////

// Store strokes to the document
EXTAPI_DOCUMENT_H boolean docSaveStrokes( struct AnotoDocument *iptAD, u64 iullPA,
										struct StrokeCollection *iptAC, u64 iullPenid );


// Store strokes to the document
EXTAPI_DOCUMENT_H boolean docSaveStrokesVerbose( struct AnotoDocument *iptAD, 
												u64 iullPA,
										        struct StrokeCollection *iptAC, 
												u64 iullPenid,
												boolean ieSaveVerbose);

// Load strokes from a document
EXTAPI_DOCUMENT_H struct StrokeCollection * docLoadStrokes( struct AnotoDocument *iptAD, u64 iullPA, u64 iullPenid );

// Set the time difference in milliseconds between time stamps and UTC time for the strokes.
EXTAPI_DOCUMENT_H boolean docSetTimeDiff( struct AnotoDocument *iptAD, u64 iullPenid, u64 iullTimeDiff );

EXTAPI_DOCUMENT_H u64 docGetTimeDiff( struct AnotoDocument *iptAD, u64 iullPenid );


//////////////////////////////
// License
//////////////////////////////

// Opens the license directory. Use this \ref AnotoFile object to create an iterator over all license files.
EXTAPI_DOCUMENT_H struct AnotoFile *docCreateLicenseDir(struct AnotoDocument *iptDoc);

// Adds a license to the document
EXTAPI_DOCUMENT_H s32 docAddLicense2(struct AnotoDocument * iptAD, const char* ipLicFilename,
										boolean ieVerifyAndSetGUID, struct KeyPair *itpKP);

// Deprecated. Use docAddLicense2() instead!
// Adds a license to the document
EXTAPI_DOCUMENT_H boolean docAddLicense(struct AnotoDocument * iptAD, const char* ipLicFilename,
										boolean ieVerifyAndSetGUID, struct KeyPair *itpKP);

// Removes a license from the doc
EXTAPI_DOCUMENT_H boolean docRemoveLicense(struct AnotoDocument * iptAD, const char* ipUniqueId, boolean ieKeepPARanges);

// Removes all licenses from the doc.
EXTAPI_DOCUMENT_H boolean docRemoveAllLicenses( struct AnotoDocument * iptAD, boolean ieKeepPARanges);

// Loads a license from the doc
EXTAPI_DOCUMENT_H struct License *docLoadLicense(struct AnotoDocument * iptAD, const char* ipUniqueId);

///////////////////////////
// Pattern
///////////////////////////

// gets the page address bounding box
EXTAPI_DOCUMENT_H s32 docGetPABoundingBox( struct AnotoDocument * iptAD, u64 *opullPALow, u64 *opullPAHigh );

// adds a PA range to the document  - normally a license should be used instead (without a valid license the created
// doc (afd) will not be printable).
EXTAPI_DOCUMENT_H s32 docAddPARange2( struct AnotoDocument * iptAD, u64 iullStartPA, u64 iullStopPA);

// Deprecated! Use docAddPARange2 instead!
// adds a PA range to the document  - normally a license should be used instead (without a valid license the created
// doc (afd) will not be printable).
EXTAPI_DOCUMENT_H boolean docAddPARange( struct AnotoDocument * iptAD, u64 iullStartPA, u64 iullStopPA);

// delete a PA range - will not work if document is locked
EXTAPI_DOCUMENT_H boolean docDeletePARange( struct AnotoDocument * iptAD, u64 iullStartPA );

// sets a pattern file name - will be done automatically by the license
EXTAPI_DOCUMENT_H boolean docSetPatternFilename( struct AnotoDocument *iptAD, char *ipBuffer);

// gets the docs pattern file name
EXTAPI_DOCUMENT_H boolean docGetPatternFilename( struct AnotoDocument *iptAD, char *opBuffer, s32 buffersize );


//////////////////////////////
// Other
//////////////////////////////

// Get a reference to the document core doc object
EXTAPI_DOCUMENT_H struct AFDCoreDoc* docGetReferenceToAFD(struct AnotoDocument* ipPD);

// Get a reference to the documents info object
EXTAPI_DOCUMENT_H struct DocumentInfo *docGetReferenceToInfo( struct AnotoDocument * iptAD );

// get page address from a given page and copy
EXTAPI_DOCUMENT_H u64 docGetPAFromPageAndCopy( struct AnotoDocument * iptAD, s32 ilPage, s32 ilCopy );

// get page and copy from a given page address
EXTAPI_DOCUMENT_H boolean docGetPageNumberFromPA( struct AnotoDocument * iptAD, u64 ipullPA, s32 *olCopy, s32 *olPage );


// merges the strokes from two docs
EXTAPI_DOCUMENT_H boolean docMerge(struct AnotoDocument *iptDocSource1,
										 struct AnotoDocument *iptDocSource2, struct AnotoDocument *iptDocDest,
										 u64 iullPageAddress, u64 iullPenId);

// merges the strokes from one document to another document, and writes it all on ZIP format to an output stream. 
EXTAPI_DOCUMENT_H s32 docMergeToStream(struct AnotoDocument *iptDocSource1,
                                       struct AnotoDocument *iptDocSource2, 
                                       struct OutputStream  *iptOutStream,
                                       u64 iullPageAddress, 
                                       u64 iullPenId);

// splits the document with strokes using pen id and page addresse
EXTAPI_DOCUMENT_H boolean docSplit( struct AnotoDocument *iptAD, const char *ipBaseOutputFileName, u64 iullPageAddress, u64 iullPenId, s32 *ilNbrOfCreatedDocs);

// Calculates a GUID (Globally Unique Id) for the doc
EXTAPI_DOCUMENT_H u64 docCalculateGUID( struct AnotoDocument *iptAD );

EXTAPI_DOCUMENT_H boolean docGetModuleVersion(struct AnotoDocument *iptAD, char *opVersionBuf,  s32 ilBufSize);

EXTAPI_DOCUMENT_H DOC_MOUNT_TYPE docGetMountType(struct AnotoDocument *iptAD);

EXTAPI_DOCUMENT_H s32 docConvertToCurrentVersion(const char* izSource, DOC_MOUNT_TYPE itSourceMount, const char* izTarget);


/*@}*/

#ifdef __cplusplus   // ANOTO_DOCUMENT_H
}
#endif

#endif
