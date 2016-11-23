/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef _area_h_
#define _area_h_

#include <common/anotocommon.h>
#include <geometry/shape.h>

#ifdef __cplusplus
extern "C"
{
#endif

///////////////////////////////////////////////
// Forward declarations.
///////////////////////////////////////////////
struct Area;

/** \ingroup ACAPI */
/** \defgroup Area Area API

Areas are parts of an \ref AreaCollection and their data is owned by the \ref AreaCollection.
An area is created when a \ref Shape is added to an AreaCollection. It is destroyed
when the \ref Shape is removed or when the \ref AreaCollection is destroyed. For this reason
there is no public constructor or public destructor in this class.
 
@{*/

// Area management attributes
EXTAPI_COMMON_H s32 arSetId(struct Area* iptArea, s64 illId);
EXTAPI_COMMON_H s32 arSetZ(struct Area* iptArea, s64 illZ);
EXTAPI_COMMON_H s32 arSetTransparent(struct Area* iptArea, boolean iTransparent);

// Area management attributes
EXTAPI_COMMON_H s32 arGetId(struct Area* iptArea, s64* opId);
EXTAPI_COMMON_H s32 arGetZ(struct Area* iptArea, s64* opZ);
EXTAPI_COMMON_H s32 arGetTransparent(struct Area* iptArea, boolean* opTransparent);

EXTAPI_COMMON_H s32 arGetShape(struct Area* iptArea, struct Shape** opptShape);

// 
/*@}*/

#ifdef __cplusplus
}
#endif

#endif  // _area_h_

