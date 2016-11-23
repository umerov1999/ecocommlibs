/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef areaengine_area_collection_h
#define areaengine_area_collection_h

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>
#include <common/io/anotofile.h>

// Module includes
#include <geometry/shape.h>
#include <areaengine/area.h>


#ifdef __cplusplus
extern "C"
{
#endif

// Current version of file format
#define	AC_CURRENT_VERSION "1.2"


/**
 * \ingroup ErrorCodeRanges
 * \defgroup AreaEngineErrorCodes	AreaEngine ErrorCodes
 *
 * @{*/

/** The \ref Area already exists in a \ref AreaCollection. An \ref Area can not be in two \ref AreaCollection (s)
 *  or appear twice in the same \ref AreaCollection. */
#define AC_AREA_ALREADY_IN_COLLECTION	-1050

/** The target id is already occupied by an \ref Area in the \ref AreaCollection. */
#define AC_TARGET_ID_TAKEN -1051

/** Another \ref Area was found at the same id as the source. Make sure the \ref Area is actually
 * in the \ref AreaCollection. */
#define AC_SOURCE_ID_TAKEN -1052

/* @} */

////////////////////////////////////////////////////////////////////////////////////
// Area Collection
////////////////////////////////////////////////////////////////////////////////////

/** \ingroup DocAPI */
/** \defgroup ACAPI Area Collection API
 
 An area collection is a collection of one or more objects of type \ref Shape 
 wrapped in \ref Area object(s) which keeps track of the shape's \ref Z order 
 and \ref ShapeId. It can be used to specify user areas on a page.

 \ref PenStroke is a special case of \ref Area. When working with strokes use
 \ref StrokeCollectionAPI instead.

 \ref AreaEngineErrorCodes
 
@{*/
	/** \struct AreaCollection
		\brief A Collection of areas

		Area collection is a number of 2D shapes grouped in an ordered list. Each shape has an ID and associated
	@{*/
		struct AreaCollection;
	/*@}*/

	/** \struct AreaIterator
		\brief Iterator over shapes in an AC

		An Area iterator enables iteration over areas in an area collection
	@{*/
		struct AreaIterator;
	/*@}*/

	// Initiates the AC instance
	EXTAPI_COMMON_H struct AreaCollection * acCreate( void );

	EXTAPI_COMMON_H struct AreaCollection *acCreateCopy( struct AreaCollection * iptAC );
	
	// Destroys the area collection and all areas in it
	EXTAPI_COMMON_H void acDestroy( struct AreaCollection ** ipptAC );

	// merges all shapes from source to target. Destroys Source ac
	EXTAPI_COMMON_H void acMerge( struct AreaCollection *iptACTarget,  struct AreaCollection ** ipptACSource );

	// Adds the Shape to the AC. The ID is used to identify the shape
	// Object is now owned by AC and will be destroyed when the AC is destroyed
	EXTAPI_COMMON_H boolean acAddShape( struct AreaCollection * iptAC, struct Shape ** ipptShape, s64 llId); 
	
	EXTAPI_COMMON_H s32 acAddShapeVerbose( struct AreaCollection * iptAC,
										  s64 illId, 
										  s64 illZ,
										  boolean isTransparent,
										  struct Shape ** ipptShape,
										  struct Area** opptArea);
	
	// Removes the Shape from AC and returns the pointer to the Shape again
	// Caller is responsible to free the Shape *
	EXTAPI_COMMON_H struct Shape * acRemoveShape( struct AreaCollection * iptAC, s64 illShapeID ); 

	// Query

	// gives a reference without removing it from collection.
	EXTAPI_COMMON_H struct Shape * acGetReferenceToShape( struct AreaCollection * iptAC, s64 illID ); 
	EXTAPI_COMMON_H struct Area* acGetReferenceToArea( struct AreaCollection * iptAC, s64 illAreaID );


	EXTAPI_COMMON_H struct Rectangle * acCreateBoundingBox( struct AreaCollection *iptAC );

	EXTAPI_COMMON_H size_t acGetMemSize( struct AreaCollection * iptAC );
	EXTAPI_COMMON_H s32 acGetNumberOfShapes( struct AreaCollection * iptAC );

	// geometric size
	EXTAPI_COMMON_H s32 acGetSize( struct AreaCollection * iptAC );

	// center of mass
	EXTAPI_COMMON_H struct Point *acCreateCenterOfMass( struct AreaCollection * iptAC );

	// Returns the number of shapes intersected
	EXTAPI_COMMON_H s32 acIntersect( struct AreaCollection * iptAC, struct Shape * iptShape, s64 * opllShapeIDs, s32 * opIntersectionTypes, s32 ilMaxIntersects );
	
	EXTAPI_COMMON_H s64 acGetMinId( struct AreaCollection * iptAC );
	EXTAPI_COMMON_H s64 acGetMaxId( struct AreaCollection * iptAC );

	EXTAPI_COMMON_H s64 acGetMinZ( struct AreaCollection * iptAC );
	EXTAPI_COMMON_H s64 acGetMaxZ( struct AreaCollection * iptAC );

	// Manipulation / special

	EXTAPI_COMMON_H struct Stroke *acCreateStrokeFromPointCollection( struct AreaCollection *iptAC );

	// see corresponding shape functions
	EXTAPI_COMMON_H boolean acTranslate( struct AreaCollection *iptAC, s32 ildx, s32 ildy );
	EXTAPI_COMMON_H boolean acScale( struct AreaCollection *iptAC, u16 iusOrigoX, u16 iusOrigoY, s32 ilsx, s32 ilsy );
	EXTAPI_COMMON_H boolean acTransform( struct AreaCollection *iptAC, u16 iusOrigoX, u16 iusOrigoY, s32 iaTrans[2][2] );

	// Serialization/Deserialize functions

	EXTAPI_COMMON_H struct AreaCollection *acLoad(struct InputStream *iptStream);
	EXTAPI_COMMON_H s32 acSave( struct AreaCollection * iptAC, struct OutputStream *iptStream);

	// Iterator

	EXTAPI_COMMON_H struct AreaIterator *acIteratorCreate( struct AreaCollection * iptAC );
	EXTAPI_COMMON_H void acIteratorDestroy( struct AreaIterator **iptSI );

	EXTAPI_COMMON_H struct Area *acIteratorGetNext(  struct AreaIterator *iptSI, s64 *ollShapeId );
	
	EXTAPI_COMMON_H void acIteratorStart(  struct AreaIterator *iptSI );
	EXTAPI_COMMON_H boolean acIteratorEnd(  struct AreaIterator *iptSI );

/*@}*/
/*@}*/


#ifdef __cplusplus
}
#endif

#endif //areaengine_area_collection_h

