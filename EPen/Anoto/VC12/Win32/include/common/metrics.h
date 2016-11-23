/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Stefan Lynggaard

#ifndef METRICS_H
#define METRICS_H

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** \ingroup Anotocommon
	\defgroup UnitConversionAPI Unit conversion API
	This api provides functions for converting between different units
@{*/

// Conversion functions to anoto units (au)
// mm = millimeters, in = inches, pt = postscript points, ad = anoto dots

EXTAPI_COMMON_H s32 metMM2AU( float iMM );
EXTAPI_COMMON_H s32 metIN2AU( float iAU );
EXTAPI_COMMON_H s32 metPT2AU( float iPT );
EXTAPI_COMMON_H s32 metAD2AU( float iAD );

// Conversion functions from anoto units (au)

EXTAPI_COMMON_H float metAU2MM( s32 iAU );
EXTAPI_COMMON_H float metAU2IN( s32 iAU );
EXTAPI_COMMON_H float metAU2PT( s32 iAU );
EXTAPI_COMMON_H float metAU2AD( s32 iAU );

EXTAPI_COMMON_H boolean metWideToUtf8(wchar_t *ipWideStr, s32 ilNbrOfSigns, char *oUTF8Buf, s32 ilUTF8BufSize);
	
//////////////////////////////////////////////////////////////////////

// Scale defines for graphics elements

/** Ordinary millimeters */
#define ANOTO_MM					"mm"
/** Ordinary centimeters */
#define ANOTO_CM					"cm"
/** Ordinary inches */
#define ANOTO_INCH					"in"
/** Postscript points - 1/72 inch or 25.4/72 mm */
#define ANOTO_POSTSCRIPT_POINTS		"pt"
/** Anoto dot (ad) - 0.3 mm (in normal cases)*/
#define ANOTO_DOTS					"ad"
/** Anoto unit (au) -  0.3/8 mm or 1/8 ad */
#define ANOTO_UNIT					"au"

//////////////////////////////////////////////////////////////////////

// Misc useful paper sizes (source: http://en.wikipedia.org/wiki/Paper_size )

// All sizes are in Anoto units 1au = 0.3/8 mm. All sizes rounded down

// ISO/DIN paper sizes 

// European

/** Defines an A7 width in anoto units */
#define ANOTO_A7_WIDTH  1872
/** Defines an A7 height in anoto units */
#define ANOTO_A7_HEIGHT 2800
/** Defines an A6 width in anoto units */
#define ANOTO_A6_WIDTH  2800
/** Defines an A6 height in anoto units */
#define ANOTO_A6_HEIGHT 3946
/** Defines an A5 width in anoto units */
#define ANOTO_A5_WIDTH  3946
/** Defines an A5 height in anoto units */
#define ANOTO_A5_HEIGHT 5600
/** Defines an A4 width in anoto units */
#define ANOTO_A4_WIDTH  5600
/** Defines an A4 height in anoto units */
#define ANOTO_A4_HEIGHT 7920
/** Defines an A3 width in anoto units */
#define ANOTO_A3_WIDTH  7920
/** Defines an A3 height in anoto units */
#define ANOTO_A3_HEIGHT 11200
/** Defines an A2 width in anoto units */
#define ANOTO_A2_WIDTH  11200
/** Defines an A2 height in anoto units */
#define ANOTO_A2_HEIGHT 15840
/** Defines an A1 width in anoto units */
#define ANOTO_A1_WIDTH  15840
/** Defines an A1 height in anoto units */
#define ANOTO_A1_HEIGHT 22426
/** Defines an A0 width in anoto units */
#define ANOTO_A0_WIDTH  22426
/** Defines an A0 height in anoto units */
#define ANOTO_A0_HEIGHT 31706

/** Defines an A7 (width and height) in anoto units */
#define ANOTO_A7 ANOTO_A7_WIDTH, ANOTO_A7_HEIGHT
/** Defines an A6 (width and height) in anoto units */
#define ANOTO_A6 ANOTO_A6_WIDTH, ANOTO_A6_HEIGHT
/** Defines an A5 (width and height) in anoto units */
#define ANOTO_A5 ANOTO_A5_WIDTH, ANOTO_A5_HEIGHT
/** Defines an A4 (width and height) in anoto units */
#define ANOTO_A4 ANOTO_A4_WIDTH, ANOTO_A4_HEIGHT
/** Defines an A3 (width and height) in anoto units */
#define ANOTO_A3 ANOTO_A3_WIDTH, ANOTO_A3_HEIGHT
/** Defines an A2 (width and height) in anoto units */
#define ANOTO_A2 ANOTO_A2_WIDTH, ANOTO_A2_HEIGHT
/** Defines an A1 (width and height) in anoto units */
#define ANOTO_A1 ANOTO_A1_WIDTH, ANOTO_A1_HEIGHT
/** Defines an A0 (width and height) in anoto units */
#define ANOTO_A0 ANOTO_A0_WIDTH, ANOTO_A0_HEIGHT

// American (note: Americans define papers landscape)

/** Defines a letter width in anoto units */
#define ANOTO_LETTER_WIDTH 5757 	
/** Defines a letter height in anoto units */
#define ANOTO_LETTER_HEIGHT 7450
/** Defines a ledger width in anoto units */
#define ANOTO_LEDGER_WIDTH  11514
/** Defines a ledger height in anoto units */
#define ANOTO_LEDGER_HEIGHT 7450
/** Defines a legal width in anoto units */
#define ANOTO_LEGAL_WIDTH  5757
/** Defines a legal height in anoto units */
#define ANOTO_LEGAL_HEIGHT 9483
/** Defines an indexcard width in anoto units */
#define ANOTO_INDEXCARD_WIDTH 3386
/** Defines an indexcard height in anoto units */
#define ANOTO_INDEXCARD_HEIGHT 2032

/** Defines a letter (width and height) in anoto units */
#define ANOTO_LETTER ANOTO_LETTER_WIDTH, ANOTO_LETTER_HEIGHT
/** Defines a ledger (width and height) in anoto units */
#define ANOTO_LEDGER ANOTO_LEDGER_WIDTH, ANOTO_LEDGER_HEIGHT
/** Defines an indexcard (width and height) in anoto units */
#define ANOTO_INDEXCARD ANOTO_INDEXCARD_WIDTH, ANOTO_INDEXCARD_HEIGHT

/** Defines an ANSI A width in anoto units*/
#define ANOTO_ANSI_A_WIDTH ANOTO_LETTER_WIDTH
/** Defines an ANSI A height in anoto units*/
#define ANOTO_ANSI_A_HEIGHT ANOTO_LETTER_HEIGHT
/** Defines an ANSI B width in anoto units*/
#define ANOTO_ANSI_B_WIDTH ANOTO_LEDGER_WIDTH
/** Defines an ANSI B width in anoto units*/
#define ANOTO_ANSI_B_HEIGHT ANOTO_LEDGER_HEIGHT

/** Defines an ANSI A (width and height) in anoto units */
#define ANOTO_ANSI_A ANOTO_LETTER_WIDTH, ANOTO_ANSI_A_HEIGHT
/** Defines an ANSI B (width and height) in anoto units */
#define ANOTO_ANSI_B ANOTO_LEDGER_WIDTH, ANOTO_ANSI_B_HEIGHT

// Japanese

// A series is identical to European therefore omitted

/** Defines a B7 width in anoto units - using JIS standard */
#define ANOTO_JIS_B7_WIDTH  2426
/** Defines a B7 height in anoto units - using JIS standard */
#define ANOTO_JIS_B7_HEIGHT 3413
/** Defines a B6 width in anoto units - using JIS standard */
#define ANOTO_JIS_B6_WIDTH  3413
/** Defines a B6 height in anoto units - using JIS standard */
#define ANOTO_JIS_B6_HEIGHT 4853
/** Defines a B5 width in anoto units - using JIS standard */
#define ANOTO_JIS_B5_WIDTH  4853
/** Defines a B5 height in anoto units - using JIS standard */
#define ANOTO_JIS_B5_HEIGHT 6853
/** Defines a B4 width in anoto units - using JIS standard */
#define ANOTO_JIS_B4_WIDTH  6853
/** Defines a B4 height in anoto units - using JIS standard */
#define ANOTO_JIS_B4_HEIGHT 9706
/** Defines a B3 width in anoto units - using JIS standard */
#define ANOTO_JIS_B3_WIDTH  9706
/** Defines a B3 height in anoto units - using JIS standard */
#define ANOTO_JIS_B3_HEIGHT 13733 
/** Defines a B2 width in anoto units - using JIS standard */
#define ANOTO_JIS_B2_WIDTH  13733 
/** Defines a B2 height in anoto units - using JIS standard */
#define ANOTO_JIS_B2_HEIGHT 19413
/** Defines a B1 width in anoto units - using JIS standard */
#define ANOTO_JIS_B1_WIDTH  19413
/** Defines a B1 height in anoto units - using JIS standard */
#define ANOTO_JIS_B1_HEIGHT 27466
/** Defines a B0 width in anoto units - using JIS standard */
#define ANOTO_JIS_B0_WIDTH  27466
/** Defines a B0 height in anoto units - using JIS standard */
#define ANOTO_JIS_B0_HEIGHT 38826 

/** Defines a B7 (width and height) in anoto units - using JIS standard */
#define ANOTO_JIS_B7 ANOTO_JIS_B7_WIDTH, ANOTO_JIS_B7_HEIGHT
/** Defines a B6 (width and height) in anoto units - using JIS standard */
#define ANOTO_JIS_B6 ANOTO_JIS_B6_WIDTH, ANOTO_JIS_B6_HEIGHT
/** Defines a B5 (width and height) in anoto units - using JIS standard */
#define ANOTO_JIS_B5 ANOTO_JIS_B5_WIDTH, ANOTO_JIS_B5_HEIGHT
/** Defines a B4 (width and height) in anoto units - using JIS standard */
#define ANOTO_JIS_B4 ANOTO_JIS_B4_WIDTH, ANOTO_JIS_B4_HEIGHT
/** Defines a B3 (width and height) in anoto units - using JIS standard */
#define ANOTO_JIS_B3 ANOTO_JIS_B3_WIDTH, ANOTO_JIS_B3_HEIGHT
/** Defines a B2 (width and height) in anoto units - using JIS standard */
#define ANOTO_JIS_B2 ANOTO_JIS_B2_WIDTH, ANOTO_JIS_B2_HEIGHT
/** Defines a B1 (width and height) in anoto units - using JIS standard */
#define ANOTO_JIS_B1 ANOTO_JIS_B1_WIDTH, ANOTO_JIS_B1_HEIGHT
/** Defines a B0 (width and height) in anoto units - using JIS standard */
#define ANOTO_JIS_B0 ANOTO_JIS_B0_WIDTH, ANOTO_JIS_B0_HEIGHT

/*@}*/

#ifdef __cplusplus
}
#endif

#endif
