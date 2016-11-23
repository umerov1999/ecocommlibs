/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef afdproperties_collection_h
#define afdproperties_collection_h

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>

#include <common/io/inputstream.h>
#include <common/io/outputstream.h>


#ifdef __cplusplus
extern "C"
{
#endif

// Current version of document format
#define PC_CURRENT_VERSION "1.0"


////////////////////////////////////////////////////////////////////////////////////
// Property collection
////////////////////////////////////////////////////////////////////////////////////

/** \ingroup DocAPI */
/** \defgroup PCAPI Properties Collection API
 
 A Property collection is a map. The key of the map is the property id and the value is a string.

 A property collection could be used to specify properties for areas. In this case the id on the area is equal
 to the property id. 
 
 It is considered good practice to have one property collection per different property. So if
 each \ref Area in an \ref AreaCollection should have a "readable name" and an "HWR category" (HWR = handwriting recognition)
 there should be two properties files. One that contains the "readable names" and one that contains the "HWR category".\n

 Note: Loading and saving properties for pages should normally be done using the \ref PageTemplate and \ref PageInstance APIs.
  
 Properties collection abstract \ref PC
@{*/
	/** \struct PropertyCollection
		\brief A Collection of properties

		Property collection is a map that maps id to string.
	@{*/
		struct PropertyCollection;
	/*@}*/

	/** \struct PropertyIterator
		\brief Iterator over Properties in an PC

		A property iterator enables iteration over properties in a Property collection
	@{*/
		struct PropertyIterator;
	/*@}*/

	// Initiates the PropertyCollection
	EXTAPI_DOCUMENT_H struct PropertyCollection * pcCreate( void );

	// Destroys the Property collection and all properties in it
	EXTAPI_DOCUMENT_H void pcDestroy( struct PropertyCollection ** ipptPC );

	// merges all properties from source to target. Destroys Source PC
	EXTAPI_DOCUMENT_H void pcMerge( struct PropertyCollection *iptPCTarget,  struct PropertyCollection ** ipptPCSource );

	// Adds the Property to the PC. The ID is used to identify the property
	// This function makes a copy of the string.
	EXTAPI_DOCUMENT_H boolean pcAddProperty( struct PropertyCollection * iptPC, char * ipProperty, s64 illPropertyID ); 

	// Removes the Property from PC and returns the pointer to the property again
	// Caller is responsible to free the char *
	EXTAPI_DOCUMENT_H char* pcRemoveProperty( struct PropertyCollection * iptPC, s64 illPropertyID ); 

	// Serialization functions
	EXTAPI_DOCUMENT_H struct PropertyCollection *pcLoad( struct InputStream *iptStream );
	EXTAPI_DOCUMENT_H s32 pcSave( struct PropertyCollection * iptPC, struct OutputStream *iptStream );

	EXTAPI_DOCUMENT_H char* pcGetReferencetoProperty( struct PropertyCollection * iptPC, s64 illPropertyID ); 

	EXTAPI_DOCUMENT_H size_t pcGetMemSize( struct PropertyCollection * iptPC );
	EXTAPI_DOCUMENT_H s32 pcGetNumberOfProperties( struct PropertyCollection * iptPC );

	// Iterator

	EXTAPI_DOCUMENT_H struct PropertyIterator *pcPropertyIteratorCreate( struct PropertyCollection * iptPC );
	EXTAPI_DOCUMENT_H void pcPropertyIteratorDestroy( struct PropertyIterator **iptPCI );

	EXTAPI_DOCUMENT_H char* pcPropertyIteratorGetNextProperty(  struct PropertyIterator *iptPCI, s64 *opllPropertyId );

	EXTAPI_DOCUMENT_H void pcPropertyIteratorStart(  struct PropertyIterator *iptPCI );
	EXTAPI_DOCUMENT_H boolean pcPropertyIteratorEnd(  struct PropertyIterator *iptPCI );

	

/*@}*/
/*@}*/


#ifdef __cplusplus
}
#endif

#endif //afdproperties_collection_h

