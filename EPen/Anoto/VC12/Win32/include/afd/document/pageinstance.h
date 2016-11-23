/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef PAGE_INSTANCE_H
#define PAGE_INSTANCE_H


// Common includes
#include <os/typedefs.h>
#include <afd/transaction/transactionlist.h>
#include <areaengine/areacollection.h>
#include <afd/core/afdcore.h> 
#include <afd/properties/afdproperties.h> 
#include <afd/gfx/graphicscollection.h>
#include <common/collection/stringlist.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup DocAPI */
/** \defgroup PIAPI Page Instance API
 
 A page instance represents a physical page in a copy of a document. A page instance is uniquely linked to a page address
 but is normally referred to using copy and page. Page in this case identifies which \ref PageTemplate this page instance
 is linked to.

 Note: Page instances in an AFD are normally created using docCreatePageInstance().

 A page instance has a \ref GraphicsCollection, an \ref AreaCollection and any number of \ref PropertyCollection (s). 
 These should be accessed using piLoad/piSave functions to make sure the files end up in the correct place in the AFD.

 In addition \ref Transaction (s) can be linked to a page instance. Entering transactions using this API makes sure
 it is written correctly to the AFD.

  See \ref PI
@{*/
	/** \struct PageInstance
		\brief Representing a page in a document instance.
\htmlonly
<pre>
	______________________________________________________________________________  
	|Copy 0                      |Copy 1                        |Copy 2
  	|  ______  ______  ______    |   _______  ______  ______    |    _______       
	| |PI 0  ||PI 1  ||PI 2  |   |  |PI 0   ||PI 1  ||PI 2  |   |   |PI 0   |		
	| |      ||      ||      |   |  |       ||      ||      |   |   |       |		
	| |      ||      ||      |   |  |       ||      ||      |   |   |       |		
	| |______||______||______|   |  |_______||______||______|   |   |_______| ...	
	|____________________________|______________________________|_________________	

		PI = Page Instance      
		
	</pre>
	\endhtmlonly

	See \ref PI

	@{*/
	
	struct PageInstance;

	/*@}*/

//////////////////////////////
// Construction
//////////////////////////////

// Creates a the page instance and populates it
EXTAPI_DOCUMENT_H struct PageInstance * piCreate(s32 ilPageNbr, s32 ilPageCopy, u64 iullPageAddress, char *ipAreafile, 
							   char *ipGfxfile, struct AFDCoreDoc *iptParentAFD);

// Destroys a page instance
EXTAPI_DOCUMENT_H void piDestroy( struct PageInstance ** ipptPI );


//////////////////////////////
// Serialization
//////////////////////////////

// Save/serializes a graphics collection to a page instance (written to file)  
EXTAPI_DOCUMENT_H boolean piSaveGraphicsCollection(struct PageInstance *iptPI, struct GraphicsCollection *tpGC);

// Save/serializes an area collection to a page instance (written to file)
EXTAPI_DOCUMENT_H boolean piSaveAreaCollection(struct PageInstance *iptPI, struct AreaCollection *tpAC);

// Reads and creates the graphics collection from a page instance.
EXTAPI_DOCUMENT_H struct GraphicsCollection *piLoadGraphicsCollection(struct PageInstance *iptPI);

// Reads and creates the area collection from a page instance.
EXTAPI_DOCUMENT_H struct AreaCollection *piLoadAreaCollection(struct PageInstance *iptPI);


//////////////////////////////
// Properties
//////////////////////////////

// Convenience function for loading a property. Slow for multiple reads.
EXTAPI_DOCUMENT_H s32 piGetProperty(struct PageInstance *iptPI, s64 illAreaId, const char* izPropertyName, char* ozValueBuffer, s32 ilBufferSize);

// Convenience function for saving a property
EXTAPI_DOCUMENT_H s32 piSetProperty(struct PageInstance *iptPI, s64 illAreaId, const char* izPropertyName, const char* izValue);

// Save/serializes a property collection to a page instance (written to file)
EXTAPI_DOCUMENT_H boolean piSavePropertyCollection(struct PageInstance *iptPI, struct PropertyCollection *iptPC, const char *ipFileSuffix);

// Reads and creates a property collection (given by file suffix) from a page instance.
EXTAPI_DOCUMENT_H struct PropertyCollection *piLoadPropertyCollection(struct PageInstance *iptPI, const char *ipFileSuffix);

// Gets all property names for the given area
EXTAPI_DOCUMENT_H struct StringList *piGetPropertyNames(struct PageInstance *iptPI);
//////////////////////////////
// Transaction and strokes 
//////////////////////////////

// Get reference to the transation list
EXTAPI_DOCUMENT_H struct TransactionList *piCreateTransactionList( struct PageInstance *iptPI, u64 iullPenid );

// Create transaction
EXTAPI_DOCUMENT_H struct Transaction *piCreateTransaction( struct PageInstance *iptPI, u64 iullPenid, u32 iulTimestampLow, u32 iulTimestampHigh, const char *izFiletype);

// Store Strokes
EXTAPI_DOCUMENT_H boolean piSaveStrokes( struct PageInstance *iptPI, struct StrokeCollection *iptAC, u64 iullPenid );
EXTAPI_DOCUMENT_H boolean piSaveStrokesVerbose( struct PageInstance *iptPI, 
												struct StrokeCollection *iptSC, 
												u64 iullPenid, 
												boolean ieSaveVerbose);

// Load Strokes
EXTAPI_DOCUMENT_H struct StrokeCollection *piLoadStrokes( struct PageInstance *iptPI, u64 iullPenid );


//////////////////////////////
// get  
//////////////////////////////

EXTAPI_DOCUMENT_H s32 piGetPageNbr(struct PageInstance *iptPI);
EXTAPI_DOCUMENT_H s32 piGetCopyNbr(struct PageInstance *iptPI);
EXTAPI_DOCUMENT_H u64 piGetPageAddress(struct PageInstance *iptPI);
EXTAPI_DOCUMENT_H boolean piGetAreaFileName(struct PageInstance *iptPI, char *opFileNameBuf, s32 ilSizeBuf);
EXTAPI_DOCUMENT_H boolean piGetGfxFileName(struct PageInstance *iptPI, char *opFileNameBuf, s32 ilSizeBuf);


//////////////////////////////
// set 
//////////////////////////////

EXTAPI_DOCUMENT_H boolean piSetAreaFileName(struct PageInstance *iptPI, const char *ipFileName);
EXTAPI_DOCUMENT_H boolean piSetGfxFileName(struct PageInstance *iptPI, const char *ipFileName);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif // PAGE_INSTANCE_H


