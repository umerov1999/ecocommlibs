/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef graphicscollection_h_
#define graphicscollection_h_

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <common/io/inputstream.h>
#include <common/io/outputstream.h>

#include <afd/gfx/gfxelement.h>

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////////
// Gfx Collection
////////////////////////////////////////////////////////////////////////////////////

// Current version of document format
#define GFX_CURRENT_VERSION "1.1"

/** \ingroup DocAPI */
/** \defgroup GFXAPI Graphics Collection API
 
 A graphics collection is a map. The key of the map is an \ref Id and the value is a \ref GfxElement. It is used to specify the graphics
 on a page. See \ref GC

 Elements are created using the \ref GfxElement API. 

 To add pattern to a page you must create of the pattern Gfx elements (in most cases the GfxDrawingArea) and add it to the 
 graphics collection of the appropriate page. 
 
 Please note that some elements, such as for example \ref GfxImage, require binary data. This data must 
 be added separately using \ref docAddLayoutResource().

 Elements are added with a z-order calculated from the elements id. Elements with higher id it put above elments with lower.
 The exception is the pattern elements. Here there is a reversed z-order i.e. no pattern is written over (seen from the perspective that 
 the "rendering" is done starting with the pattern element with the lowest id and higher is then added).  

@{*/
	/** \struct GraphicsCollection
		\brief A Collection of graphics elements

		A graphics collection is a number of graphical elements with properties, size and locations on a page
	@{*/

	struct GraphicsCollection;

	/*@}*/

	/** \struct GfxElementIterator
		\brief A GfxElement iterator

		A GfxElementIterator enables iteration over the shapes in a graphics collection
	@{*/

	struct GfxElementIterator;

	/*@}*/

	// Initiates the GfxEC instance
	EXTAPI_DOCUMENT_H struct GraphicsCollection * gfxcCreate( void );

	EXTAPI_DOCUMENT_H struct GraphicsCollection *gfxcCreateCopy( struct GraphicsCollection * iptGfxEC  );

	// Destroys the Gfx element collection and all elements in it
	EXTAPI_DOCUMENT_H void gfxcDestroy( struct GraphicsCollection ** ipptGfxEC );

	// merges all Gfx element from source to target. Destroys Source ac
	EXTAPI_DOCUMENT_H void gfxcMerge( struct GraphicsCollection *iptGfxECTarget,  struct GraphicsCollection ** ipptGfxECSource );

	// Adds the Gfx element to the GfxEC. The ID is used to identify the Gfx element
	// Object is now owned by GfxEC and will be destroyed when the GfxEC is destroyed
	EXTAPI_DOCUMENT_H boolean gfxcAddElement( struct GraphicsCollection * iptGfxEC, struct GfxElement ** ipptGfxElement , s64 ilGfxElemId); 

	// Removes the Gfx element from GfxEC and returns the pointer to the Gfx element again
	// Caller is responsible to free the Gfx element *
	EXTAPI_DOCUMENT_H struct GfxElement * gfxcRemoveElement( struct GraphicsCollection * iptGfxEC, s64 ilGfxElementID ); 

	// Serialization functions
	EXTAPI_DOCUMENT_H struct GraphicsCollection *gfxcLoad( char *ipResourceBasePath , struct InputStream* iptStream );
	EXTAPI_DOCUMENT_H s32 gfxcSave( struct GraphicsCollection * iptGfxEC, char *ipResourceBasePath, struct OutputStream *iptStream );

	EXTAPI_DOCUMENT_H s32 gfxcGetNumberOfElements( struct GraphicsCollection * iptGfxEC );

	// Iterator
	EXTAPI_DOCUMENT_H struct GfxElementIterator *gfxcElementIteratorCreate( struct GraphicsCollection * iptGfxEC );
	EXTAPI_DOCUMENT_H void gfxcElementIteratorDestroy( struct GfxElementIterator **ipptGfxElemIter );

	EXTAPI_DOCUMENT_H struct GfxElement *gfxcIteratorGetNextElement(  struct GfxElementIterator *iptGfxElemIter, s64 *oplGfxElementId );

	EXTAPI_DOCUMENT_H void gfxcIteratorStart(  struct GfxElementIterator *iptGfxElemIter );
	EXTAPI_DOCUMENT_H boolean gfxcIteratorEnd(  struct GfxElementIterator *iptGfxElemIter );

/*@}*/
/*@}*/

#ifdef __cplusplus
}
#endif


#endif //graphicscollection_h_
