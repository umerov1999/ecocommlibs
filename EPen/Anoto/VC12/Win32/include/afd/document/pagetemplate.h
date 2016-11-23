/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef PAGE_TEMPLATE_H
#define PAGE_TEMPLATE_H


// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <common/collection/stringlist.h>
#include <afd/transaction/transactionlist.h>
#include <areaengine/areacollection.h>
#include <afd/core/afdcore.h> 
#include <afd/properties/afdproperties.h> 
#include <afd/gfx/graphicscollection.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup DocAPI */
/** \defgroup PTAPI Page Template API
 
 A page template contains the static layout for a page in a document. All page instances, virtually created when adding 
 pattern (page address ranges), will contain one of these page templates. \n

 Note: Unless you have special needs, create page templates using \ref docAddPageTemplate(). This will
       create a working page template using good default values. To access already created page templates
	   in a document use \ref docGetReferenceToPageTemplate()

 A page template has a number of attributes. Most of these are set when the page template is created and for most cases
 they should not be set manually after creation. Attributes are read using functions starting with ptGet.

 Graphics on a page is represented by a \ref GraphicsCollection. Use \ref ptLoadGraphicsCollection() and 
 \ref ptSaveGraphicsCollection() to access the graphics on a page.

 User areas and properties are accessed in the same way as a graphics collection.

 See \ref PT
@{*/
	/** \struct PageTemplate
		\brief Representing a page in a document instance.
	<pre>
	______________________________
	|D                           |
  	|  ______  ______  ______    |     
	| |PT 0  ||PT 1  ||PT 2  |   |  
	| |      ||      ||      |   | 	
	| |      ||      ||      |   |  
	| |______||______||______|   |  
	|____________________________|

		D = Document   
		PT = Page Template      
		
	</pre>

  	See \ref PT
	@{*/
	
	struct PageTemplate;

	/*@}*/

//////////////////////////////
//	Construction
//////////////////////////////

// Creates a the page template and populates it
EXTAPI_DOCUMENT_H struct PageTemplate * ptCreate(s32 ilPageNbr, s32 ilWidth, s32 ilHeight,
                                                 boolean ieHasPattern, boolean ieHidden,
                                                 char *ipBasePath,  char *ipAreafile,
                                                 char *ipGfxfile, boolean ieLocked,
                                                 struct AFDCoreDoc *iptParentAFD);

// Destroys a page template
EXTAPI_DOCUMENT_H void ptDestroy( struct PageTemplate ** ipptPT );

// Copy a page template
EXTAPI_DOCUMENT_H struct PageTemplate *ptCreateCopy( struct PageTemplate * iptPT  );


//////////////////////////////
// Serialization
//////////////////////////////

// Writes/serializes a graphics collection to a page template (written to file)  
EXTAPI_DOCUMENT_H boolean ptSaveGraphicsCollection(struct PageTemplate *iptPT, struct GraphicsCollection *iptGC);

// Writes/serializes an area collection to a page template (written to file)
EXTAPI_DOCUMENT_H boolean ptSaveAreaCollection(struct PageTemplate *iptPT, struct AreaCollection *itpAC);

// Reads and creates the graphics collection from a page template.
EXTAPI_DOCUMENT_H struct GraphicsCollection *ptLoadGraphicsCollection(struct PageTemplate *iptPT);

// Reads and creates the area collection from a page template.
EXTAPI_DOCUMENT_H struct AreaCollection *ptLoadAreaCollection(struct PageTemplate *iptPT);


////////////////////////////////////
// Properties
////////////////////////////////////

// Convenience function for loading a property. Slow for multiple reads.
EXTAPI_DOCUMENT_H s32 ptGetProperty(struct PageTemplate *iptPT, s64 illAreaId, const char* izPropertyName, char* ozValueBuffer, s32 ilBufferSize);

// Convenience function for saving a property
EXTAPI_DOCUMENT_H s32 ptSetProperty(struct PageTemplate *iptPT, s64 illAreaId, const char* izPropertyName, const char* izValue);

// Writes/serializes a property collection to a page template (written to file)
EXTAPI_DOCUMENT_H boolean ptSavePropertyCollection(struct PageTemplate *iptPT, struct PropertyCollection *iptPC, 
												 const char *izPropertyName);

// Reads and creates a property collection (given by file suffix) from a page template.
EXTAPI_DOCUMENT_H struct PropertyCollection *ptLoadPropertyCollection(struct PageTemplate *iptPT, const char *izPropertyName);

// Gets all property names for the given area
EXTAPI_DOCUMENT_H struct StringList *ptGetPropertyNames(struct PageTemplate *iptPT);

//////////////////////////////
// get  
//////////////////////////////

EXTAPI_DOCUMENT_H s32 ptGetWidth(struct PageTemplate *iptPT);
EXTAPI_DOCUMENT_H s32 ptGetHeight(struct PageTemplate *iptPT);
EXTAPI_DOCUMENT_H s32 ptGetPageNbr(struct PageTemplate *iptPT);
EXTAPI_DOCUMENT_H boolean ptIsLocked(struct PageTemplate *iptPT);
EXTAPI_DOCUMENT_H boolean ptHasPattern(struct PageTemplate *iptPI);
EXTAPI_DOCUMENT_H boolean ptGetAreaFileName(struct PageTemplate *iptPT, char *opFileNameBuf, s32 ilSizeBuf);
EXTAPI_DOCUMENT_H boolean ptGetGfxFileName(struct PageTemplate *iptPT, char *opFileNameBuf, s32 ilSizeBuf);
EXTAPI_DOCUMENT_H boolean ptGetBasePath(struct PageTemplate *iptPT, char *opFileNameBuf, s32 ilSizeBuf);
EXTAPI_DOCUMENT_H boolean ptGetAbsoluteAreaFileName(struct PageTemplate *iptPT, char *oipFileNameBuf, s32 ilSizeBuf);
EXTAPI_DOCUMENT_H boolean ptGetAbsoluteGfxFileName(struct PageTemplate *iptPT, char *oipFileNameBuf, s32 ilSizeBuf);
EXTAPI_DOCUMENT_H boolean ptIsHidden(struct PageTemplate *iptPT);

//////////////////////////////
// set 
//////////////////////////////

EXTAPI_DOCUMENT_H boolean ptSetWidth(struct PageTemplate *iptPT, s32 ilWidth);
EXTAPI_DOCUMENT_H boolean ptSetHeight(struct PageTemplate *iptPT, s32 ilHeight);
EXTAPI_DOCUMENT_H boolean ptSetHasPattern(struct PageTemplate *iptPT, boolean ieHasPattern);
EXTAPI_DOCUMENT_H boolean ptSetBasePath(struct PageTemplate *iptPT, const char *ipFileName);
EXTAPI_DOCUMENT_H boolean ptSetAreaFileName(struct PageTemplate *iptPT, const char *ipFileName);
EXTAPI_DOCUMENT_H boolean ptSetGfxFileName(struct PageTemplate *iptPT, const char *ipFileName);
EXTAPI_DOCUMENT_H boolean ptSetHidden(struct PageTemplate *iptPT, boolean ieHidden);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif //PAGE_TEMPLATE_H


