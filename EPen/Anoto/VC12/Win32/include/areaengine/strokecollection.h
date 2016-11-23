/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef areaengine_stroke_collection_h
#define areaengine_stroke_collection_h

#include "penstroke.h"
#include "areacollection.h"

#include <geometry/shape.h>
#include <common/io/inputstream.h>

// included to indicate dependancy for the wrapper generation
#include <common/anotocolor.h>

#ifdef __cplusplus
extern "C"
{
#endif



////////////////////////////////////////////////////////////////////////////////////
// Stroke Collection
////////////////////////////////////////////////////////////////////////////////////

/** \ingroup ACAPI
    \defgroup StrokeCollectionAPI StrokeCollection API

 A stroke collection is a collection of \ref Stroke (s). It can be used
 to specify strokes on a page.

@{*/
	/** \struct StrokeCollection
		\brief A Collection of strokes

		Stroke collection is a number of strokes. It is a subclass of \ref AreaCollection and it shares all properties of an area collection and two additional properties. The additional properties is a low and high time bounding box of the strokes in the collection. 
		The time of strokes are in the unit of milliseconds since 1970 (standard). The shape id (or time) is the start time of the stroke
	@{*/
		struct StrokeCollection;
	/*@}*/
			
	/** \struct StrokeIterator
		\brief Iterator over a elements in a \ref StrokeCollection

	@{*/
		struct StrokeIterator;
	/*@}*/


		struct StrokeStream;

	// Initiates the AC instance
	EXTAPI_COMMON_H struct StrokeCollection * stcCreate( void );

	EXTAPI_COMMON_H struct AreaCollection *stcGetSuper(struct StrokeCollection* ptSC);
	
	// Destroys the area collection and all areas in it
	EXTAPI_COMMON_H void stcDestroy( struct StrokeCollection ** ipptSC );

	EXTAPI_COMMON_H boolean stcAddStroke( struct StrokeCollection * iptSC, struct Stroke ** ipptStroke, s64 illAbsoluteStartTime ); 										 
	EXTAPI_COMMON_H boolean stcAddStrokeVerbose(struct StrokeCollection * iptSC,
												struct Stroke ** ipptStroke, 
												s64 illAbsoluteStartTime, 
												struct AnotoColor** iopptColor,
												u8 ibTipType, 
												struct PenStroke** opptPenStroke); 

	// Removes the Stroke from SC and returns the pointer to the Stroke again
	// Caller is responsible to free the Stroke *
	EXTAPI_COMMON_H struct Stroke * stcRemoveStroke( struct StrokeCollection * iptSC, s64 illShapeID ); 

	// Query
	EXTAPI_COMMON_H s64 stcGetLowTime( struct StrokeCollection * iptSC );
	EXTAPI_COMMON_H s64 stcGetHighTime( struct StrokeCollection * iptSC );

	// Manipulation / special
	EXTAPI_COMMON_H struct StrokeCollection *stcLoadStrokes( struct InputStream *iptStream );
	EXTAPI_COMMON_H s32 stcSaveStrokes( struct StrokeCollection * iptSC, struct OutputStream *iptStream);


	// Iterator
	EXTAPI_COMMON_H struct StrokeIterator *stcIteratorCreate( struct StrokeCollection * iptSC );
	EXTAPI_COMMON_H void stcIteratorDestroy( struct StrokeIterator **iptSI );

	EXTAPI_COMMON_H struct Stroke *stcIteratorGetNextStroke(  struct StrokeIterator *iptSI, s64 *ollTime );
	EXTAPI_COMMON_H struct PenStroke *stcIteratorGetNextPenStroke(  struct StrokeIterator *iptSI, s64 *ollTime );

	EXTAPI_COMMON_H void stcIteratorStart(  struct StrokeIterator *iptSI );
	EXTAPI_COMMON_H boolean stcIteratorEnd(  struct StrokeIterator *iptSI );


	// Stroke streams. 
	EXTAPI_COMMON_H struct StrokeStream *stcOpenReadStream( struct InputStream *iptStream );
	EXTAPI_COMMON_H struct StrokeStream *stcOpenWriteStream( struct OutputStream *iptStream );
	EXTAPI_COMMON_H struct StrokeStream *stcOpenWriteStreamAppend( AnotoWriteFunc ifpWriteFunc, void * ipUserData, AnotoSeekFunc ifpSeekFunc);

	EXTAPI_COMMON_H void stcDestroyStream(  struct StrokeStream **ipptSS  );	

	EXTAPI_COMMON_H struct Stroke *stcLoadStroke(  struct StrokeStream *iptSS, s64 *ollStartTime );
	EXTAPI_COMMON_H s32 stcSaveStroke(  struct StrokeStream *iptSS, struct Stroke *iptStroke, s64 illStartTime );

	EXTAPI_COMMON_H struct StrokeCollection* stcLoad(struct InputStream *iptIStream);
	EXTAPI_COMMON_H s32 stcSave(struct StrokeCollection* ptSC, struct OutputStream *iptOStream);

/*@}*/


#ifdef __cplusplus
}
#endif

#endif //areaengine_stroke_collection_h

