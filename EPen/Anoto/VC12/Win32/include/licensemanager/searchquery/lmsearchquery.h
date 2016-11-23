/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef _lm_searchquery_h_
#define _lm_searchquery_h_

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <licensemanager/license/lmspecification.h>
#include <licensemanager/searchquery/lmsearchcondition.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup DatabaseAPI */
/** \defgroup SearchQueryAPI Search Query API
 
 The Search Query API is an API for the management of search queries.\n
 A search query (SQ) is a collection of search conditions to be used for a license search in the pattern 
 license database (sub or super).
 
 @{*/
	/** \struct SearchQuery
		\brief 	A collection of search conditions 

		A collection of search conditions to be used in the search for a pattern license.
	@{*/
		struct SearchQuery;
	/*@}*/


	/** \struct QueryIterator
		\brief Iterator over items in a search query

		Iterator enables to iterate over all the search conditions in the \ref SearchQuery.
	@{*/
		struct QueryIterator;
	/*@}*/

// Create an empty search query
EXTAPI_LICENSE_H struct SearchQuery * sqCreate( void );

// Create a search query from a license specification
EXTAPI_LICENSE_H struct SearchQuery * sqCreateFromLicSpec(struct LicenseSpec *itpLS);

// Destroy the search query
EXTAPI_LICENSE_H void sqDestroy( struct SearchQuery ** ipptSQ );

// Adds a search condition to the search query
EXTAPI_LICENSE_H boolean sqAddElement( struct SearchQuery * ipSQ, struct SearchCondition ** ipptSC, s64 illSCID ); 

// Remove search condition from the search query
EXTAPI_LICENSE_H struct SearchCondition* sqRemoveElement( struct SearchQuery * iptSQ, s64 illQueryID ); 

// Get reference to a search condition in the search query
EXTAPI_LICENSE_H struct SearchCondition* sqGetReferenceToElement( struct SearchQuery * iptSQ, s64 illSCID); 

// Get number of search condition in the search query
EXTAPI_LICENSE_H s32 sqGetNumberOfElements( struct SearchQuery * iptSQ );

///// Iterator
EXTAPI_LICENSE_H struct QueryIterator *sqQueryIteratorCreate( struct SearchQuery *ipSQ );
EXTAPI_LICENSE_H void sqQueryIteratorDestroy( struct QueryIterator **iptQI );
EXTAPI_LICENSE_H struct SearchCondition *sqQueryIteratorGetNext(  struct QueryIterator *iptQI, s64 *ollSCID );
EXTAPI_LICENSE_H void sqQueryIteratorStart(  struct QueryIterator *iptQI );
EXTAPI_LICENSE_H boolean sqQueryIteratorEnd(  struct QueryIterator *iptQIiptQI );

/*@}*/  

#ifdef __cplusplus
}
#endif


#endif  // _lm_searchquery_h_






