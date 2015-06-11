/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.03.0110 */
/* at Sat Sep 29 16:06:21 2012
 */
/* Compiler settings for XTPChartDefines.odl:
	Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
	error checks: none
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

//{{AFX_CODEJOCK_PRIVATE
#ifndef __XTPChartDefines_h__
#define __XTPChartDefines_h__
//}}AFX_CODEJOCK_PRIVATE

#ifdef __cplusplus
extern "C"{
#endif

/* Forward Declarations */

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * );


//{{AFX_CODEJOCK_PRIVATE
#ifndef __XtremeChartControl_LIBRARY_DEFINED__
#define __XtremeChartControl_LIBRARY_DEFINED__
//}}AFX_CODEJOCK_PRIVATE

/****************************************
 * Generated header for library: XtremeChartControl
 * at Sat Sep 29 16:06:21 2012
 * using MIDL 3.03.0110
 ****************************************/
/* [control][uuid] */


typedef /* [helpcontext][helpstring][uuid] */
enum XTPChartScaleType
	{   xtpChartScaleQualitative    = 0,
	xtpChartScaleNumerical  = xtpChartScaleQualitative + 1,
	xtpChartScaleDateTime   = xtpChartScaleNumerical + 1,
	xtpChartScaleTime   = xtpChartScaleDateTime + 1
	}   XTPChartScaleType;

typedef XTPChartScaleType ChartScaleType;

typedef /* [helpcontext][helpstring][uuid] */
enum XTPChartStringAlignment
	{   xtpChartAlignNear   = 0,
	xtpChartAlignCenter = xtpChartAlignNear + 1,
	xtpChartAlignFar    = xtpChartAlignCenter + 1
	}   XTPChartStringAlignment;

typedef XTPChartStringAlignment ChartStringAlignment;

typedef /* [helpcontext][helpstring][uuid] */
enum XTPChartDashStyle
	{   xtpChartDashStyleEmpty  = 0,
	xtpChartDashStyleSolid  = xtpChartDashStyleEmpty + 1,
	xtpChartDashStyleDash   = xtpChartDashStyleSolid + 1,
	xtpChartDashStyleDot    = xtpChartDashStyleDash + 1,
	xtpChartDashStyleDashDot    = xtpChartDashStyleDot + 1,
	xtpChartDashStyleDashDotDot = xtpChartDashStyleDashDot + 1,
	xtpChartDashStyleCustom = xtpChartDashStyleDashDotDot + 1
	}   XTPChartDashStyle;

typedef XTPChartDashStyle ChartDashStyle;

typedef /* [helpcontext][helpstring][uuid] */
enum XTPChartFillMode
	{   xtpChartFillEmpty   = 0,
	xtpChartFillSolid   = xtpChartFillEmpty + 1,
	xtpChartFillGradient    = xtpChartFillSolid + 1,
	xtpChartFillHatch   = xtpChartFillGradient + 1
	}   XTPChartFillMode;

typedef XTPChartFillMode ChartFillMode;

typedef /* [helpcontext][helpstring][uuid] */
enum XTPChartGradientAngle
	{   xtpChartGradientAngle0  = 0,
	xtpChartGradientAngle45 = 45,
	xtpChartGradientAngle90 = 90,
	xtpChartGradientAngle135    = 135,
	xtpChartGradientAngle180    = 180,
	xtpChartGradientAngle225    = 225,
	xtpChartGradientAngle270    = 270,
	xtpChartGradientAngle315    = 315
	}   XTPChartGradientAngle;

typedef XTPChartGradientAngle ChartGradientAngle;

typedef /* [helpcontext][helpstring][uuid] */
enum XTPChartLinearGradientMode
	{   xtpChartLinearGradientModeHorizontal    = 0,
	xtpChartLinearGradientModeVertical  = xtpChartLinearGradientModeHorizontal + 1,
	xtpChartLinearGradientModeForwardDiagonal   = xtpChartLinearGradientModeVertical + 1,
	xtpChartLinearGradientModeBackwardDiagonal  = xtpChartLinearGradientModeForwardDiagonal + 1,
	xtpChartLinearGradientModeCenterHorizontal  = xtpChartLinearGradientModeBackwardDiagonal + 1,
	xtpChartLinearGradientModeCenterVertical    = xtpChartLinearGradientModeCenterHorizontal + 1
	}   XTPChartLinearGradientMode;

typedef XTPChartLinearGradientMode ChartLinearGradientMode;

typedef /* [helpcontext][helpstring][uuid] */
enum XTPChartHatchStyle
	{   xtpChartHatchStyleHorizontal    = 0,
	xtpChartHatchStyleVertical  = xtpChartHatchStyleHorizontal + 1,
	xtpChartHatchStyleForwardDiagonal   = xtpChartHatchStyleVertical + 1,
	xtpChartHatchStyleBackwardDiagonal  = xtpChartHatchStyleForwardDiagonal + 1,
	xtpChartHatchStyleCross = xtpChartHatchStyleBackwardDiagonal + 1,
	xtpChartHatchStyleDiagonalCross = xtpChartHatchStyleCross + 1,
	xtpChartHatchStyle05Percent = xtpChartHatchStyleDiagonalCross + 1,
	xtpChartHatchStyle10Percent = xtpChartHatchStyle05Percent + 1,
	xtpChartHatchStyle20Percent = xtpChartHatchStyle10Percent + 1,
	xtpChartHatchStyle25Percent = xtpChartHatchStyle20Percent + 1,
	xtpChartHatchStyle30Percent = xtpChartHatchStyle25Percent + 1,
	xtpChartHatchStyle40Percent = xtpChartHatchStyle30Percent + 1,
	xtpChartHatchStyle50Percent = xtpChartHatchStyle40Percent + 1,
	xtpChartHatchStyle60Percent = xtpChartHatchStyle50Percent + 1,
	xtpChartHatchStyle70Percent = xtpChartHatchStyle60Percent + 1,
	xtpChartHatchStyle75Percent = xtpChartHatchStyle70Percent + 1,
	xtpChartHatchStyle80Percent = xtpChartHatchStyle75Percent + 1,
	xtpChartHatchStyle90Percent = xtpChartHatchStyle80Percent + 1,
	xtpChartHatchStyleLightDownwardDiagonal = xtpChartHatchStyle90Percent + 1,
	xtpChartHatchStyleLightUpwardDiagonal   = xtpChartHatchStyleLightDownwardDiagonal + 1,
	xtpChartHatchStyleDarkDownwardDiagonal  = xtpChartHatchStyleLightUpwardDiagonal + 1,
	xtpChartHatchStyleDarkUpwardDiagonal    = xtpChartHatchStyleDarkDownwardDiagonal + 1,
	xtpChartHatchStyleWideDownwardDiagonal  = xtpChartHatchStyleDarkUpwardDiagonal + 1,
	xtpChartHatchStyleWideUpwardDiagonal    = xtpChartHatchStyleWideDownwardDiagonal + 1,
	xtpChartHatchStyleLightVertical = xtpChartHatchStyleWideUpwardDiagonal + 1,
	xtpChartHatchStyleLightHorizontal   = xtpChartHatchStyleLightVertical + 1,
	xtpChartHatchStyleNarrowVertical    = xtpChartHatchStyleLightHorizontal + 1,
	xtpChartHatchStyleNarrowHorizontal  = xtpChartHatchStyleNarrowVertical + 1,
	xtpChartHatchStyleDarkVertical  = xtpChartHatchStyleNarrowHorizontal + 1,
	xtpChartHatchStyleDarkHorizontal    = xtpChartHatchStyleDarkVertical + 1,
	xtpChartHatchStyleDashedDownwardDiagonal    = xtpChartHatchStyleDarkHorizontal + 1,
	xtpChartHatchStyleDashedUpwardDiagonal  = xtpChartHatchStyleDashedDownwardDiagonal + 1,
	xtpChartHatchStyleDashedHorizontal  = xtpChartHatchStyleDashedUpwardDiagonal + 1,
	xtpChartHatchStyleDashedVertical    = xtpChartHatchStyleDashedHorizontal + 1,
	xtpChartHatchStyleSmallConfetti = xtpChartHatchStyleDashedVertical + 1,
	xtpChartHatchStyleLargeConfetti = xtpChartHatchStyleSmallConfetti + 1,
	xtpChartHatchStyleZigZag    = xtpChartHatchStyleLargeConfetti + 1,
	xtpChartHatchStyleWave  = xtpChartHatchStyleZigZag + 1,
	xtpChartHatchStyleDiagonalBrick = xtpChartHatchStyleWave + 1,
	xtpChartHatchStyleHorizontalBrick   = xtpChartHatchStyleDiagonalBrick + 1,
	xtpChartHatchStyleWeave = xtpChartHatchStyleHorizontalBrick + 1,
	xtpChartHatchStylePlaid = xtpChartHatchStyleWeave + 1,
	xtpChartHatchStyleDivot = xtpChartHatchStylePlaid + 1,
	xtpChartHatchStyleDottedGrid    = xtpChartHatchStyleDivot + 1,
	xtpChartHatchStyleDottedDiamond = xtpChartHatchStyleDottedGrid + 1,
	xtpChartHatchStyleShingle   = xtpChartHatchStyleDottedDiamond + 1,
	xtpChartHatchStyleTrellis   = xtpChartHatchStyleShingle + 1,
	xtpChartHatchStyleSphere    = xtpChartHatchStyleTrellis + 1,
	xtpChartHatchStyleSmallGrid = xtpChartHatchStyleSphere + 1,
	xtpChartHatchStyleSmallCheckerBoard = xtpChartHatchStyleSmallGrid + 1,
	xtpChartHatchStyleLargeCheckerBoard = xtpChartHatchStyleSmallCheckerBoard + 1,
	xtpChartHatchStyleOutlinedDiamond   = xtpChartHatchStyleLargeCheckerBoard + 1,
	xtpChartHatchStyleSolidDiamond  = xtpChartHatchStyleOutlinedDiamond + 1
	}   XTPChartHatchStyle;

typedef XTPChartHatchStyle ChartHatchStyle;

typedef /* [helpcontext][helpstring][uuid] */
enum XTPChartNearTextPosition
	{   xtpChartTextNearLeft    = 0,
	xtpChartTextNearRight   = xtpChartTextNearLeft + 1,
	xtpChartTextNearTop = xtpChartTextNearRight + 1,
	xtpChartTextNearBottom  = xtpChartTextNearTop + 1
	}   XTPChartNearTextPosition;

typedef XTPChartNearTextPosition ChartNearTextPosition;

typedef /* [helpstring][uuid] */
enum XTPChartErrorBarType
	{   xtpChartErrorBarTypeNone    = 0,
	xtpChartErrorBarTypeFixedValue  = 1,
	xtpChartErrorBarTypePercentage  = 2,
	xtpChartErrorBarTypeStandardDeviation   = 3,
	xtpChartErrorBarTypeStandardError   = 4,
	xtpChartErrorBarTypeCustom  = 5
	}   XTPChartErrorBarType;

typedef XTPChartErrorBarType ChartErrorBarType;

typedef /* [helpstring][uuid] */
enum XTPChartErrorBarDirection
	{   xtpChartErrorBarDirectionNone   = 0,
	xtpChartErrorBarDirectionPlus   = 1,
	xtpChartErrorBarDirectionMinus  = 2,
	xtpChartErrorBarDirectionBoth   = 3
	}   XTPChartErrorBarDirection;

typedef XTPChartErrorBarDirection ChartErrorBarDirection;

typedef /* [helpstring][uuid] */
enum XTPChartErrorBarEndStyle
	{   xtpChartErrorBarEndStyleNoCap   = 0,
	xtpChartErrorBarEndStyleCap = 1
	}   XTPChartErrorBarEndStyle;

typedef XTPChartErrorBarEndStyle ChartErrorBarEndStyle;

typedef /* [helpstring][uuid] */
enum XTPChartErrorBarPosition
	{   xtpChartErrorBarPositionNone    = 0,
	xtpChartErrorBarPositionTop = 1,
	xtpChartErrorBarPositionCenter  = 2
	}   XTPChartErrorBarPosition;

typedef XTPChartErrorBarPosition ChartErrorBarPosition;


EXTERN_C const IID LIBID_XtremeChartControl;
//{{AFX_CODEJOCK_PRIVATE
#endif /* __XtremeChartControl_LIBRARY_DEFINED__ */
//}}AFX_CODEJOCK_PRIVATE

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

//{{AFX_CODEJOCK_PRIVATE
#endif
//}}AFX_CODEJOCK_PRIVATE
