/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef VERTEX_H
#define VERTEX_H

//#include <os/amemory.h>
#include <common/anotocommon.h>
#include <common/io/inputstream.h>
#include <common/io/outputstream.h>
#include <xml/xmlwriter.h>

typedef u8 vertexarray;

/*
	Possible contents of a vertex type
	----------------------------------
	
	XY = X & Y coordinate u16 x 2
	F = Force u8
	D = Time diff u8
	T = Time absolute u16
	A = Angles tilt, rot, skew u16 x 3
	O1 = Optional parameters 1 u32 x1
	O2 = Optional parameters 2 u32 x2
	O3 = Optional parameters 3 u32 x3
*/

// Vertex array types

/** \ingroup ShapeAPI	
    \defgroup VertexType Vertex type integer
	Definitions used to identify vertex properties. Vertices are used e.g. in a 
	\ref Stroke, see shpCreateStroke(). A vertex can contain X and Y coordinates, 
	force value, and a time delta. 
@{*/

/** Empty */
#define VT_EMPTY 0	// 0 bytes
/**  X & Y */
#define VT_XY 1		// 4 bytes
/**  X, Y & Force */
#define VT_XYF 2	// 6 bytes
/**  X, Y, Force & coordinate number delta */
#define VT_XYFD 3	// 6 bytes
/*@}*/

#define VT_XYT 4	// 6 bytes
#define VT_XYFT 5	// 8 bytes
#define VT_XYFA 6	// 12 bytes
#define VT_XYFDA 7	// 12 bytes
#define VT_XYFTA 8	// 14 bytes
#define VT_XYFDO1 9	// 10 bytes
#define VT_XYFDO2 10	// 14 bytes
#define VT_XYFDO3 11	// 18 bytes
#define VT_XYFTAO1 12	// 18 bytes
#define VT_XYFTAO2 13	// 22 bytes
#define VT_XYFTAO3 14	// 26 bytes

//////////////////////////////
// Ask property
//////////////////////////////

boolean vtHasX( vertexarray *a );
boolean vtHasY( vertexarray *a );
boolean vtHasF( vertexarray *a );
boolean vtHasD( vertexarray *a );
boolean vtHasT( vertexarray *a );
boolean vtHasTilt( vertexarray *a );
boolean vtHasSkew( vertexarray *a );
boolean vtHasRot( vertexarray *a );
boolean vtHasOpt1( vertexarray *a );
boolean vtHasOpt2( vertexarray *a );
boolean vtHasOpt3( vertexarray *a );

//////////////////////////////
// Get functions
//////////////////////////////

u16 vtGetX( vertexarray *a, s32 i );
s32 vtGetXL( vertexarray *a, s32 i );	// to avoid casts will max return 65535
u16 vtGetY( vertexarray *a, s32 i );
s32 vtGetYL( vertexarray *a, s32 i );	// to avoid casts will max return 65535
s8 vtGetF( vertexarray *a, s32 i );
u8 vtGetD( vertexarray *a, s32 i );
u16 vtGetT( vertexarray *a, s32 i );
u16 vtGetTilt( vertexarray *a, s32 i );
u16 vtGetSkew( vertexarray *a, s32 i );
u16 vtGetRot( vertexarray *a, s32 i );
u32 vtGetOpt1( vertexarray *a, s32 i );
u32 vtGetOpt2( vertexarray *a, s32 i );
u32 vtGetOpt3( vertexarray *a, s32 i );

//////////////////////////////
// Set functions
//////////////////////////////

void vtSetX( vertexarray *a, s32 i, u16 v );
void vtSetY( vertexarray *a, s32 i, u16 v );
void vtSetF( vertexarray *a, s32 i, s8 v );
void vtSetD( vertexarray *a, s32 i, u8 v );
void vtSetT( vertexarray *a, s32 i, u16 v );
void vtSetTilt( vertexarray *a, s32 i, u16 v );
void vtSetSkew( vertexarray *a, s32 i, u16 v);
void vtSetRot( vertexarray *a, s32 i, u16 v );
void vtSetOpt1( vertexarray *a, s32 i, u32 v );
void vtSetOpt2( vertexarray *a, s32 i, u32 v );
void vtSetOpt3( vertexarray *a, s32 i, u32 v );

//////////////////////////////
// Copy functions
//////////////////////////////

void vtCopyX( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyY( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyF( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyD( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyT( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyTilt( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopySkew( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyRot( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyOpt1( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyOpt2( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtCopyOpt3( vertexarray *d, s32 iid, vertexarray *s, s32 iis );

//////////////////////////////
// Misc manipulation routines
//////////////////////////////

u8 vtGetType( vertexarray *a );
void vtCopyVertex( vertexarray *d, s32 iid, vertexarray *s, s32 iis );
void vtSwapVertex( vertexarray *a, s32 iia, vertexarray * b, s32 iib );

//////////////////////////////
// Printout Functions
//////////////////////////////

void vtGetPrintableType( vertexarray *a, char *o, char *indent );
void vtGetPrintableVertex( vertexarray *a, s32 i, char *o);

//////////////////////////////
// Sizeof functions
//////////////////////////////
s32 vtGetSizeElem(vertexarray *a);
s32 vtGetVertexArraySize(vertexarray *a, s32 numelements);
s32 vtGetVertexArraySizeType(u8 type, s32 numelements);

//////////////////////////////
// Create functions
//////////////////////////////
void vtInitializeMemAsVertexArray(vertexarray *a, u8 type);
vertexarray *vtCreateVertexArray( u8 type, s32 numelements );

//////////////////////////////
// Serialize functions
//////////////////////////////

s32 vtSerializeArray(vertexarray *p, s32 ilSizeofArray, struct OutputStream *iptStream, char *indent);
s32 vtSerializeArrayXml(vertexarray *iptVertexArray, s32 ilSizeofArray, struct XmlWriter *iptXmlWriter);
s32 vtDeserializeArray(vertexarray *p, s32 ilSizeofArray, struct InputStream *iptStream);

#endif
