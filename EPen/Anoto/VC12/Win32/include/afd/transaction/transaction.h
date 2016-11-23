/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef TRANSACTION_H
#define TRANSACTION_H

// Common includes
#include <os/typedefs.h>

#include <common/anotocommon.h>
#include <common/penid.h>

#include <areaengine/areacollection.h>
#include <areaengine/strokecollection.h>
#include <afd/core/affcontext.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup TransactionListAPI
	\defgroup TransactionAPI Transaction API
	See \ref Transaction
@{*/

	/** 
		\struct Transaction
		\brief A Transaction object

	  A Transaction object specifies a specific existing or imaginary transaction in a AFD file.\n
	  A Transaction should be thought of as a piece of generated data that has a pen id, timestamp
	  and page. The primary type of transaction is strokes but any type of data is accepted. There
	  can be many transactions on a page.
	  It has the properties Page, Pen, Time stamp low, Time stamp high and type\n
	  
	  A transaction can be used both to specify a specific transaction or to be used as a filter to filter 
	  a\n \ref TransactionList of specific information to be extracted from all transactions in a file. When
	  used as a filter it is possible to enter wildcards for most parameters.\n
	  
	  Note: To create a transaction for a specific \ref PageInstance in an AFD file use piCreateTransaction().

	  Note: If a transaction is created with the same time stamp, pen id, file type etc, as a previous transaction and
	  written to the afd file the transaction already written will be lost.

	@{*/
	
	struct Transaction;

	/*@}*/	
/*@}*/

////////////////////////////////////////////////////////////////////////////////////
// Transaction and Transaction Filter Functions
////////////////////////////////////////////////////////////////////////////////////


/**\ingroup TransactionAPI
   \defgroup TransactionFilter Transaction Filters
@{*/
/** \ref Transaction filter of all pens */
#define TRANS_PEN_ID_WILD	PEN_ID_UNDEFINED
/** \ref Transaction filter of all pages */
#define TRANS_PAGE_WILD		PA_WILDCARD
/** \ref Transaction  filter of all types */
#define TRANS_TYPE_WILD		"*"
/** \ref Transaction  filter of all strokes */
#define TRANS_STROKE_TYPE	"*STF"
/** \ref Transaction that filters all strokes stored in the .stf format */	
#define TRANS_STROKES_AS_STF "stf"
/** \ref Transaction that filters all strokes stored in the areas (.xtf) format */
#define TRANS_STROKES_AS_AREAS "xtf"
/** \ref Transaction  filter of all data from beginning of time */
#define TRANS_TIME_LOW		(u32)0
/** \ref Transaction  filter of all data until end of time */
#define TRANS_TIME_HIGH		(u32)(-1)
/** \ref Transaction  maximum size of the file suffix incl null termination */
#define MAX_SUFFIX_SIZE		8
/*@}*/

#define TRANS_PEN_DATA_FOLDER		"data"
#define TRANS_PATH_SEPARATOR_SLASH	"/"
#define TRANS_PATH_SEPARATOR_DOT	"."
#define TRANS_PEN_DATA_FILENAME		"0x%08x-0x%08x"

// Constructors / Destructor

EXTAPI_DOCUMENT_H struct Transaction *tCreate(  u64 iullPenid, u64 iullPage, u32 iulTimestampLow, u32 iulTimeStampHigh, const char *filetype );
EXTAPI_DOCUMENT_H void tDestroy( struct Transaction **ipT );

EXTAPI_DOCUMENT_H struct Transaction *tCreateCopy( struct Transaction *iptT );

// Useful create functions to make filters easy for 

EXTAPI_DOCUMENT_H struct Transaction *tCreateFilterAll( void );
EXTAPI_DOCUMENT_H struct Transaction *tCreateFilterAllOnPage( u64 iullPage );
EXTAPI_DOCUMENT_H struct Transaction *tCreateFilterAllOnPageAndPen( u64 iullPage, u64 iullPenid );
EXTAPI_DOCUMENT_H struct Transaction *tCreateFilterAllStrokes( void );
EXTAPI_DOCUMENT_H struct Transaction *tCreateFilterAllStrokesOnPage( u64 iullPage );
EXTAPI_DOCUMENT_H struct Transaction *tCreateFilterAllStrokesOnPageAndPen( u64 iullPage, u64 iullPenid );

// Get set functions

EXTAPI_DOCUMENT_H u64 tGetPage( struct Transaction *iptT );
EXTAPI_DOCUMENT_H u64 tGetPenId( struct Transaction *iptT );
EXTAPI_DOCUMENT_H void tGetTimestamps( struct Transaction *iptT, u32 * opulTimestampLow, u32 * opulTimestampHigh );
EXTAPI_DOCUMENT_H void tGetType( struct Transaction *iptT, char *opDest );

EXTAPI_DOCUMENT_H void tSetPage( struct Transaction *iptT, u64 iullPage );
EXTAPI_DOCUMENT_H void tSetPenId( struct Transaction *iptT, u64 iullPenId );
EXTAPI_DOCUMENT_H void tSetTimestamps( struct Transaction *iptT, u32  iulTimestampLow, u32 iulTimestampHigh );
EXTAPI_DOCUMENT_H void tSetType( struct Transaction *iptT, char *ipType );

EXTAPI_DOCUMENT_H boolean tGetPathTo( struct Transaction *iptT, char *opDest );

// gets a path to a specified transaction (compare to tGetPathTo() )
EXTAPI_DOCUMENT_H boolean tGetPathToTransaction( u64 iullPenid, u64 iullPage, u32 iulTimestampLow, u32 iulTimeStampHigh,char *ipType, char *opDest );


// destroys transaction on disc
EXTAPI_DOCUMENT_H boolean tDelete(  struct Transaction *ipT, struct AFFileContext * iptFC);

// saves some strokes at a specific page
EXTAPI_DOCUMENT_H boolean tSaveStrokes( struct Transaction* iptT, struct AFFileContext * iptFC, struct StrokeCollection *iptSC );
EXTAPI_DOCUMENT_H boolean tSaveStrokesVerbose(struct Transaction* iptT, 
												struct AFFileContext * iptFC, 
												struct StrokeCollection *iptSC,
												boolean ieSaveVerbose);

EXTAPI_DOCUMENT_H struct StrokeCollection *tLoadStrokes( struct Transaction* iptT, struct AFFileContext * iptFC );

boolean tIsSubset( struct Transaction *iptSubset, struct Transaction *iptSet );
boolean tIsOfType( struct Transaction *iptTrans, const char *type );
s64 tMakeHash( struct Transaction *iptTrans );


#ifdef __cplusplus
}
#endif

#endif
