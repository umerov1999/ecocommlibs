/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef _INTERS_H_
#define _INTERS_H_

/** \ingroup ShapeAPI
	\defgroup IntersectReturnCodes Intersect package return codes 
	Definitions of how two objects can intersect. Used as return codes for 
	\ref shpIntersect() and also related to \ref acIntersect().

	@{*/
	/*! The objects are disjunctive \image html shpexternal.gif */
	#define INT_EXTERNAL		0 
	/*! First object is completely inside second object \image html shpinternal1.gif*/
	#define INT_INTERNAL_1		1 
	/*! Second object is completely inside first object \image html shpinternal2.gif*/
	#define INT_INTERNAL_2		2 
	/*! The objects clip in one or more Points \image html shpclip.gif*/
	#define INT_CLIP			3
	/*@}*/
/*@}*/

#define INT_UNKNOWN			4
#define INT_POS_CLIP		5


#include <os/typedefs.h>
#include <geometry/vertex.h>

struct coordinate
{
	s32 x,y;
};

#define SQR(x) ((x)*(x))


// misc routines

s32 intEllipseValid( vertexarray *ia );

// Pos32 vs routines

s32 intPointRect(  vertexarray *ia, vertexarray *ib );
s32 intPointPoly( vertexarray *ia, s32 iNumSamples, vertexarray *ib );
s32 intPointRect(  vertexarray *ia, vertexarray *ib );
s32 intPointEllipse(  vertexarray *ia, vertexarray *ib );

// Segment vs routines

s32 intSegSeg( vertexarray *ia, vertexarray *ib );
s32 intSegRect( vertexarray *ia, vertexarray *ib );
s32 intSegStroke( vertexarray *ia,s32 iNumSamples, vertexarray *ib );
s32 intSegPoly( vertexarray *ia,s32 iNumSamples, vertexarray *ib );
s32 intSegEllipse( vertexarray *ia, vertexarray *ib);

// Rect vs routines

s32 intRectRect( vertexarray *ia, vertexarray *ib );
s32 intRectEllipse( vertexarray *ia, vertexarray *ib );
s32 intRectPoly( vertexarray *ia,s32 iNumSamples, vertexarray *ib);

// Stroke vs routines

s32 intStrokeStroke( s32 iNumSamples1, vertexarray *ia,s32 iNumSamples2, vertexarray *ib);
s32 intStrokeRect(  s32 iNumSamples1, vertexarray *ia,  vertexarray *ib);
s32 intStrokeEllipse( s32 iNumSamples1, vertexarray *ia,  vertexarray *ib);
s32 intStrokePoly( s32 iNumSamples1, vertexarray *ia,s32 iNumSamples2, vertexarray *ib);

// Ellipse vs routines

s32 intEllipsePoly(vertexarray *ia,s32 iNumSamples, vertexarray *ib);
s32 intEllipseEllipse( vertexarray *ia, vertexarray *ib );

// Poly vs routines

s32 intPolyPoly( s32 iNumSamples1, vertexarray *ia,s32 iNumSamples2, vertexarray *ib);

u32 sqrt64(u64 a);

#endif
