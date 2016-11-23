/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef areaengine_shape_h
#define areaengine_shape_h

// External includes
#include <geometry/intersect.h>

// Common includes
#include <common/anotocolor.h>
#include <common/io/inputstream.h>
#include <common/io/outputstream.h>
#include <os/typedefs.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** \ingroup ACAPI	
    \defgroup ShapeAPI Shape API
 
	2D geometry package, containing functionality to create and manipulate 2D objects.

	The base class in this package is \ref Shape, which can be a 2D object of 
	many actual shapes. 
	Many of the subclasses (\ref Point, \ref Rectangle etc) can be cast to the 
	type \ref Shape. 
<br>
	A collection of objects of the type \ref Shape (or sublasses thereof) can 
	be contained in an \ref AreaCollection. When adding a Shape to an 
	AreaCollection, it is wrapped in an \ref Area object. 

@{*/

/** \defgroup ShapeType Shape type integer
	Definitions used to distinguish between the different \ref Shape types. Returned by \ref shpGetType().
@{*/
/** \ref Point */
#define SHP_POINT       0
/** \ref LineSegment */
#define SHP_LINESEGMENT 1
/** \ref Rectangle */
#define SHP_RECTANGLE   2
/** \ref Ellipse */
#define SHP_ELLIPSE     3
/** \ref Polygon */
#define SHP_POLYGON     4 
/** \ref Polyline */
#define SHP_POLYLINE    5
/** \ref Stroke */
#define SHP_STROKE		6
/** The number of different shapes. */
#define SHP_NBR_SHAPE_TYPES 7
/*@}*/

////////////////////////////////////////////////////////////////////////////////////
// Shape.h
////////////////////////////////////////////////////////////////////////////////////

// Define Shape types

/** \struct Shape
	\brief An abstract 2D shape. 

	The base class of this package.
@{*/
	struct Shape;
/*@}*/

/** \struct Point
	\brief A point

	A point is a coordinate in \ref ACS. It is also considered to be a \ref Shape
@{*/
	struct Point;
/*@}*/

/** \struct LineSegment
	\brief A line segment 

	\image html shpline.gif

	A line segment is two points in \ref ACS. It is also considered to be a \ref Shape
@{*/
	struct LineSegment;
/*@}*/

/** \struct Rectangle
	\brief A rectangle 

  	\image html shprectangle.gif

	A rectangle is an area in \ref ACS. It is also considered to be a \ref Shape
@{*/
	struct Rectangle;
/*@}*/

/** \struct Ellipse
	\brief An ellipse 

  	\image html shpellipse.gif

	An Ellipse is an area in \ref ACS. From a parameter perspective it is considered to be suspended inside a rectangle. It is also considered to be a \ref Shape
@{*/
	struct Ellipse;
/*@}*/

/** \struct Polygon
	\brief A polygon 

  	\image html shppolygon.gif

	A polygon is an area in \ref ACS it can be concave but should be non overlapping and it is built up from a sequence of 2D coordinates. It is also considered to be a \ref Shape
@{*/
	struct Polygon;
/*@}*/

/** \struct Polyline
	\brief A poly line 

 	\image html shppolyline.gif 

	A poly line is a polygon train that is unconnected and 1 dimensional in \ref ACS it is built up from a sequence of 2D coordinates. It is also considered to be a \ref Shape
@{*/
	struct Polyline;
/*@}*/

/** \struct Stroke
	\brief A pen stroke 

	\image html shpstroke.gif

	A Stroke is a sequence of pen movements and it is a named version of \ref Polyline. Those two shape types are very similar. It is also considered to be a \ref Shape
@{*/
	struct Stroke;
/*@}*/


// Extra prototype to avoid warnings
// Cannot include AreaCollection since it includes this file.
struct AreaCollection;

//////////////////////////////
// Construction
//////////////////////////////


EXTAPI_COMMON_H struct Point * shpCreatePoint( s32 ilX, s32 ilY );
EXTAPI_COMMON_H struct Point * shpCreatePointXYFD( s32 ilX, s32 ilY, u8 iubF, u8 iubD );
EXTAPI_COMMON_H struct LineSegment * shpCreateLineSegment( s32 ilX1, s32 ilY1, s32 ilX2, s32 ilY2 );
EXTAPI_COMMON_H struct Rectangle * shpCreateRectangle( s32 ilLeft, s32 ilTop, s32 ilRight, s32 ilBottom );
EXTAPI_COMMON_H struct Ellipse * shpCreateEllipse( s32 ilLeft, s32 ilTop, s32 ilRight, s32 ilBottom );
EXTAPI_COMMON_H struct Polygon * shpCreatePolygon( s32 ilNumvertices );
EXTAPI_COMMON_H struct Polyline * shpCreatePolyline( s32 ilNumvertices );
EXTAPI_COMMON_H struct Stroke * shpCreateStroke( s32 ilNumvertices, u8 iubVertexType );

EXTAPI_COMMON_H struct Shape *shpCreateCopy( struct Shape * iptShape );

EXTAPI_COMMON_H void shpDestroy( struct Shape ** iopptShape );

EXTAPI_COMMON_H size_t shpGetMemSize( struct Shape * iptShape );

//////////////////////////////
// Manipulation functions
//////////////////////////////

// note due to limited precision a lot of bad things can happen if the following functions are called repeatedly
// or in an unappropriate manner.
// if any value is moved outside allowed range functions return FALSE

// moves shape ildx, ildy anoto units
EXTAPI_COMMON_H boolean shpTranslate( struct Shape *iptShape, s32 ildx, s32 ildy );

// scales the shape ( 16 bits fixed pt fraction precision. i.e. 65536 = no scale )
EXTAPI_COMMON_H boolean shpScale( struct Shape *iptShape, u16 iusOrigoX, u16 iusOrigoY, s32 ilsx, s32 ilsy );

// Transforms according to matrix around origo. matr same prec as last func
// to do rotation ex:
// iaTrans[0][0] = cos(a);
// iaTrans[0][1] = sin(a);
// iaTrans[1][0] = -sin(a);
// iaTrans[1][1] = cos(a);

EXTAPI_COMMON_H boolean shpTransform( struct Shape *iptShape, u16 iusOrigoX, u16 iusOrigoY, s32 iaTrans[2][2] );

// fixes wrong vertex order on Rect,Ellipse. It needs to be top, left, bottom, right
EXTAPI_COMMON_H void shpFixSquare( struct Shape *iptShape );

// Add remove vertex functions. Pretty Bad performance and will reallocate memory
// Works on poly, polyline and stroke
EXTAPI_COMMON_H boolean shpRemoveVertex( struct Shape **iptShape, s32 ilIndex );
EXTAPI_COMMON_H boolean shpAddVertex( struct Shape **iptShape, s32 ilIndex, u16 iusX, u16 iusY );

//////////////////////////////
// I/O functions
//////////////////////////////

EXTAPI_COMMON_H struct Shape * shpLoad( s64 *opllID, struct InputStream* iptStream);
EXTAPI_COMMON_H s32 shpSave( struct Shape * iptShape, s64 illID, struct OutputStream *iptStream);


//////////////////////////////
// Query functions
//////////////////////////////

EXTAPI_COMMON_H s32 shpIntersect( struct Shape * iptShape1, struct Shape * iptShape2 );

// Bounding box functions
EXTAPI_COMMON_H struct Rectangle * shpCreateBoundingBox( struct Shape * iptShape );
EXTAPI_COMMON_H void shpIncreaseBB( struct Rectangle * ioptBoundingBox, s32 x, s32 y );
EXTAPI_COMMON_H void shpIncreaseBBwithBB( struct Rectangle * ioptBoundingBox, struct Rectangle *iptBB );

// Properties functions
EXTAPI_COMMON_H s32 shpGetType( struct Shape *iptShape );
EXTAPI_COMMON_H s32 shpGetNbrVertices( struct Shape *iptShape );

/* "Get" functions, to get a reference to superclass: */
EXTAPI_COMMON_H struct Shape *shpGetSuperForPoint( struct Point *iptShape );
EXTAPI_COMMON_H struct Shape *shpGetSuperForLine( struct LineSegment *iptShape );
EXTAPI_COMMON_H struct Shape *shpGetSuperForRectangle( struct Rectangle *iptShape );
EXTAPI_COMMON_H struct Shape *shpGetSuperForEllipse( struct Ellipse *iptShape );
EXTAPI_COMMON_H struct Shape *shpGetSuperForPolygon( struct Polygon *iptShape );
EXTAPI_COMMON_H struct Shape *shpGetSuperForPolyline( struct Polyline *iptShape );
EXTAPI_COMMON_H struct Shape *shpGetSuperForStroke( struct Stroke *iptShape );


// returns size of shape (physical size)
// pt = 0
// seg,polyline,stroke = length
// rect,ellipse,polygon = area

EXTAPI_COMMON_H s32 shpGetSize( struct Shape *iptShape );

// creates and returns a point that is center of mass
// in composite shapes it might in special cases not return a correct value (subtractive parts outside additive parts)
// Also on polygons self interesecting it fails
EXTAPI_COMMON_H struct Point *shpCreateCenterOfMass( struct Shape *iptShape );

// Access coordinates in shape

// Note that if elements outside allowed range is accesed silent discard or default value will be returned

// Get functions

EXTAPI_COMMON_H void shpGetXY( struct Shape *iptShape, s32 ilIndex, u16 *opusX, u16 *opusY );
EXTAPI_COMMON_H void shpGetXYF( struct Shape *iptShape, s32 ilIndex, u16 *opusX, u16 *opusY, s8 *opbF );
EXTAPI_COMMON_H void shpGetXYFD( struct Shape *iptShape, s32 ilIndex, u16 *opusX, u16 *opusY, s8 *opubF, u8 *opubD );


// Set functions

EXTAPI_COMMON_H void shpSetXY( struct Shape *iptShape, s32 ilIndex, u16 iusX, u16 iusY );
EXTAPI_COMMON_H void shpSetXYF( struct Shape *iptShape, s32 ilIndex, u16 iusX, u16 iusY, s8 ibF );
EXTAPI_COMMON_H void shpSetXYFD( struct Shape *iptShape, s32 ilIndex, u16 iusX, u16 iusY, s8 ibF, u8 iubD );

/*@}*/
/*@}*/

#ifdef __cplusplus
}
#endif

#endif //areaengine_shape_h


