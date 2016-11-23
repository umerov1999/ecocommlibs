/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef _penstroke_h_
#define _penstroke_h_

#include "area.h"

// included to indicate dependency for the wrapper generation
#include <common/anotocolor.h>
#include <geometry/shape.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup StrokeCollectionAPI */
/** \defgroup PenStroke PenStroke

PenStrokes are parts of a \ref StrokeCollection and their data is owned by the \ref StrokeCollection.
A \ref PenStroke is created when a \ref Stroke is added to a StrokeCollection. It is destroyed
when the \ref Stroke is removed or when the \ref StrokeCollection is destroyed. For this reason
there is no public constructor or public destructor in this class.
 
@{*/
///////////////////////////////////////////////
// Forward declarations.
///////////////////////////////////////////////

struct PenStroke;



// Stroke data
EXTAPI_COMMON_H s32 pstSetColor(struct PenStroke* iptStroke, struct AnotoColor** iopptColor);
EXTAPI_COMMON_H s32 pstSetTipType(struct PenStroke* iptStroke, u8 iubTipType);
EXTAPI_COMMON_H s32 pstSetTime(struct PenStroke* iptStroke, s64 illTime);


// Stroke data
EXTAPI_COMMON_H s32 pstGetColor(struct PenStroke* iptStroke, struct AnotoColor **opptColor);
EXTAPI_COMMON_H s32 pstGetTipType(struct PenStroke* iptStroke, u8 *opubTipType);
EXTAPI_COMMON_H s32 pstGetTime(struct PenStroke* iptStroke, s64 *opllTime);


// Contained Stroke. Used by the geometry package.
EXTAPI_COMMON_H s32 pstGetStroke(struct PenStroke* iptStroke, struct Stroke** opptStroke);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif  // _area_h_

