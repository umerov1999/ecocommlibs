/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Mattias Mårtensson

#ifndef _FILE_ANOTOCOLOR_H_
#define _FILE_ANOTOCOLOR_H_

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** \ingroup Anotocommon */
/** \defgroup COLAPI AnotoColor API

Represents a color in the AFP system.\n

@{*/

	/** 
	\struct AnotoColor
	\brief Holds color information.

	@{*/
struct AnotoColor;
	/*@}*/

EXTAPI_COMMON_H struct AnotoColor* colCreateRGB(u8 iubR, u8 iubG, u8 iubB);
EXTAPI_COMMON_H struct AnotoColor* colCreateX11Color(u16 id);
EXTAPI_COMMON_H struct AnotoColor* colCreateCopy(struct AnotoColor* iptColor);
EXTAPI_COMMON_H s32 colDestroy(struct AnotoColor** opptColor);

EXTAPI_COMMON_H u8 colGetR(struct AnotoColor* iptColor);
EXTAPI_COMMON_H u8 colGetG(struct AnotoColor* iptColor);
EXTAPI_COMMON_H u8 colGetB(struct AnotoColor* iptColor);

/* @} */

#ifdef __cplusplus
}
#endif


#endif



