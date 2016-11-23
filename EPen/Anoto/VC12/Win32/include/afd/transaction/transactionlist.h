/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef TRANSACTIONLIST_H
#define TRANSACTIONLIST_H

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>

#include <areaengine/areacollection.h>
#include <afd/core/affcontext.h>

#include "transaction.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup PIAPI 
    \defgroup TransactionListAPI TransactionList API
	The TransctionList API gives access to strokes and other data added to a page by pen or similar device.

	See \ref TL.
@{*/

////////////////////////////////////////////////////////////////////////////////////
// AFD Strokes
////////////////////////////////////////////////////////////////////////////////////

	/** \struct TransactionList
		\brief A list of transactions

		A list of \ref Transaction objects for each \ref PageInstance in the AFD file a transaction list
		can be loaded. This list can be filtered and manipulated in different ways to be able to extract
		the specific data that is required for the operation, see \ref TL. \n
		
		The transactionlist has knowledge of the file context it belongs to\n
		
	@{*/

		struct TransactionList;

	/*@}*/

	/** \struct TransactionIterator
		\brief Transaction Iterator

		An iterator over a \ref TransactionList

	@{*/

		struct TransactionIterator;

	/*@}*/

////////////////////////////////////////////////////////////////////////////////////
// Transaction List
////////////////////////////////////////////////////////////////////////////////////

// Constructors / Destructors

EXTAPI_DOCUMENT_H struct TransactionList *tlCreate(  struct AFFileContext * iptFC  );
EXTAPI_DOCUMENT_H void tlDestroy( struct TransactionList ** ipptTL );

// Loads transaction filtered
EXTAPI_DOCUMENT_H struct TransactionList *tlLoadFiltered(  struct AFFileContext * iptFC , struct Transaction *iptT );

// Loads transaction list with all transactions in the file
EXTAPI_DOCUMENT_H struct TransactionList *tlLoad(  struct AFFileContext * iptFC );

// Loads a transaction list with all strokes
EXTAPI_DOCUMENT_H struct TransactionList * tlLoadStroke(   struct AFFileContext * iptFC   );

// Creates a copy with filter (use tCreateFilterAll() ) to make a straight copy
EXTAPI_DOCUMENT_H struct TransactionList * tlCreateCopyFromFilter( struct TransactionList *iptTL, struct Transaction *iptT );

// Manipulation (Does not affect data on disk)

EXTAPI_DOCUMENT_H boolean tlAddTransaction( struct TransactionList * iptTL,  struct Transaction **iptT );
EXTAPI_DOCUMENT_H struct Transaction *tlRemoveTransaction( struct TransactionList * iptTL,  struct Transaction *iptT );

// Query

// Returns number of transactions in the current transaction list
EXTAPI_DOCUMENT_H s32 tlGetNumberOfTransactions( struct TransactionList *iptTL );

EXTAPI_DOCUMENT_H struct AFFileContext *tlGetReferenceToAFFC( struct TransactionList *iptTL );

// Delete
// Also destroys the entries in the transactionlist but not the transactionlist itself

EXTAPI_DOCUMENT_H void tlDeleteTransactionsFiltered( struct TransactionList * iptTL,  struct Transaction *iptT );
EXTAPI_DOCUMENT_H void tlDeleteAllTransactions( struct TransactionList *iptTL );

EXTAPI_DOCUMENT_H boolean tlDeleteTransaction( struct TransactionList * iptTL,  struct Transaction *iptT );

////////////////////////////////////////////////////////////////////////////////////
// Transaction Iterator
////////////////////////////////////////////////////////////////////////////////////

// Create and destroy a transaction iterator
EXTAPI_DOCUMENT_H struct TransactionIterator *tlIteratorCreate( struct TransactionList *iptTL );
EXTAPI_DOCUMENT_H void tlIteratorDestroy( struct TransactionIterator **iptTI );

// gets next element in iterator
EXTAPI_DOCUMENT_H struct Transaction *tlIteratorGetNextTransaction( struct TransactionIterator *iptTI );

// returns iterator to start of list
EXTAPI_DOCUMENT_H void tlIteratorStart( struct TransactionIterator *iptTI );

// checks if end is reached in iterator
EXTAPI_DOCUMENT_H boolean tlIteratorEnd( struct TransactionIterator *iptTI );

////////////////////////////////////////////////////////////////////////////////////
// Misc writing
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
// Strokes
////////////////////////////////////////////////////////////////////////////////////

// read

// Gets a merged stroke collection from a transaction list 
EXTAPI_DOCUMENT_H struct StrokeCollection *tlCreateStrokeObjectFromTransactionListFiltered( struct TransactionList *iptTL, struct Transaction *iptT );

// get a merged area collection for  the current transaction list (this can superimpose strokes from many pages)
EXTAPI_DOCUMENT_H struct StrokeCollection *tlCreateStrokeObjectFromTransactionList( struct TransactionList *iptTL );

// write


/*@}*/

#ifdef __cplusplus
}
#endif

#endif
