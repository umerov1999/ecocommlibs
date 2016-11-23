/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef docutil_h
#define docutil_h

#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup DocAPI

 \defgroup DocUtils Document Utility API

The Document Utility API includes various utility functions on different structures that
exist in the Document Module.
@{*/

 /** \defgroup DocUtilsDefines Document Utility defines
@{*/
/** Default value */
#define DOCUTIL_DEFAULT -1
/** Default margin for cut pattern (6.6mm). Defined in AU.*/
#define DOCUTIL_DEFAULT_MARGIN (22*8)
/*@}*/
/*@}*/
/*@}*/

EXTAPI_DOCUMENT_H boolean docUtilCreateTransposedShape(struct Shape *iptShape,
                                                       struct GraphicsCollection *iptGfxCo,
                                                       struct AreaCollection *iptAC,
                                                       s32 ilMargin,
                                                       struct Shape **opptShape);

#ifdef __cplusplus
}
#endif

#endif //docutil_h
