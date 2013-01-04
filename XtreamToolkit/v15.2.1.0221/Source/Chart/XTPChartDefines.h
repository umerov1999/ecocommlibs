// XTPChartDefines.h
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTDEFINES_H__)
#define __XTPCHARTDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//===========================================================================
// Summary:
//     This enumeration defines the various text position used in the XTP chart.
// Remarks:
//===========================================================================
enum XTPChartNearTextPosition
{
	xtpChartTextNearLeft,    //The near left position.
	xtpChartTextNearRight,   //The near right position.
	xtpChartTextNearTop,     //The near top position.
	xtpChartTextNearBottom   //The near bottom position.
};


//===========================================================================
// Summary:
//     Enumeration defines the various scale types used in the chart.
// Remarks:
//===========================================================================
enum XTPChartScaleType
{
	xtpChartScaleQualitative,    //Qualitative type.
	xtpChartScaleNumerical,      //Numeric type.
	xtpChartScaleDateTime        //Date time type.
};

//===========================================================================
// Summary:
//     Enumeration defines the various alignments for the chart title.
// Remarks:
//===========================================================================
enum XTPChartStringAlignment
{
	xtpChartAlignNear,       //Left side of the chart.
	xtpChartAlignCenter,     //Center of the chart.
	xtpChartAlignFar,        //Right side of the chart.
};

//===========================================================================
// Summary:
//     Enumeration defines the various line styles for the charts.
// Remarks:
//===========================================================================
enum XTPChartDashStyle
{
	xtpChartDashStyleEmpty,          //Empty style, invisible.
	xtpChartDashStyleSolid,          //Solid line style.
	xtpChartDashStyleDash,           //Dash line style.
	xtpChartDashStyleDot,            //Dot line style.
	xtpChartDashStyleDashDot,        //Alternate dash and dot line style.
	xtpChartDashStyleDashDotDot,     //Dash dot dot line style.
	xtpChartDashStyleCustom
};


//===========================================================================
// Summary:
//     Enumeration defines the various background fill modes.
// Remarks:
//===========================================================================
enum XTPChartFillMode
{
	xtpChartFillEmpty,       //Empty style.
	xtpChartFillSolid,       //Solid style.
	xtpChartFillGradient,    //Gradient style.
	xtpChartFillHatch        //Hatch style.
};

//===========================================================================
// Summary:
//     Enumeration defines the different gradient angles used for background
//     filling.
// Remarks:
//===========================================================================
enum XTPChartGradientAngle
{
	xtpChartGradientAngle0   =   0,
	xtpChartGradientAngle45  =  45,
	xtpChartGradientAngle90  =  90,
	xtpChartGradientAngle135 = 135,
	xtpChartGradientAngle180 = 180,
	xtpChartGradientAngle225 = 225,
	xtpChartGradientAngle270 = 270,
	xtpChartGradientAngle315 = 315,
};

//===========================================================================
// Summary:
//     Enumeration defines the various hatch styles used for background
//     filling.
// Remarks:
//===========================================================================
enum XTPChartHatchStyle
{
	xtpChartHatchStyleHorizontal,                   // 0
	xtpChartHatchStyleVertical,                     // 1
	xtpChartHatchStyleForwardDiagonal,              // 2
	xtpChartHatchStyleBackwardDiagonal,             // 3
	xtpChartHatchStyleCross,                        // 4
	xtpChartHatchStyleDiagonalCross,                // 5
	xtpChartHatchStyle05Percent,                    // 6
	xtpChartHatchStyle10Percent,                    // 7
	xtpChartHatchStyle20Percent,                    // 8
	xtpChartHatchStyle25Percent,                    // 9
	xtpChartHatchStyle30Percent,                    // 10
	xtpChartHatchStyle40Percent,                    // 11
	xtpChartHatchStyle50Percent,                    // 12
	xtpChartHatchStyle60Percent,                    // 13
	xtpChartHatchStyle70Percent,                    // 14
	xtpChartHatchStyle75Percent,                    // 15
	xtpChartHatchStyle80Percent,                    // 16
	xtpChartHatchStyle90Percent,                    // 17
	xtpChartHatchStyleLightDownwardDiagonal,        // 18
	xtpChartHatchStyleLightUpwardDiagonal,          // 19
	xtpChartHatchStyleDarkDownwardDiagonal,         // 20
	xtpChartHatchStyleDarkUpwardDiagonal,           // 21
	xtpChartHatchStyleWideDownwardDiagonal,         // 22
	xtpChartHatchStyleWideUpwardDiagonal,           // 23
	xtpChartHatchStyleLightVertical,                // 24
	xtpChartHatchStyleLightHorizontal,              // 25
	xtpChartHatchStyleNarrowVertical,               // 26
	xtpChartHatchStyleNarrowHorizontal,             // 27
	xtpChartHatchStyleDarkVertical,                 // 28
	xtpChartHatchStyleDarkHorizontal,               // 29
	xtpChartHatchStyleDashedDownwardDiagonal,       // 30
	xtpChartHatchStyleDashedUpwardDiagonal,         // 31
	xtpChartHatchStyleDashedHorizontal,             // 32
	xtpChartHatchStyleDashedVertical,               // 33
	xtpChartHatchStyleSmallConfetti,                // 34
	xtpChartHatchStyleLargeConfetti,                // 35
	xtpChartHatchStyleZigZag,                       // 36
	xtpChartHatchStyleWave,                         // 37
	xtpChartHatchStyleDiagonalBrick,                // 38
	xtpChartHatchStyleHorizontalBrick,              // 39
	xtpChartHatchStyleWeave,                        // 40
	xtpChartHatchStylePlaid,                        // 41
	xtpChartHatchStyleDivot,                        // 42
	xtpChartHatchStyleDottedGrid,                   // 43
	xtpChartHatchStyleDottedDiamond,                // 44
	xtpChartHatchStyleShingle,                      // 45
	xtpChartHatchStyleTrellis,                      // 46
	xtpChartHatchStyleSphere,                       // 47
	xtpChartHatchStyleSmallGrid,                    // 48
	xtpChartHatchStyleSmallCheckerBoard,            // 49
	xtpChartHatchStyleLargeCheckerBoard,            // 50
	xtpChartHatchStyleOutlinedDiamond,              // 51
	xtpChartHatchStyleSolidDiamond                  // 52
};

//===========================================================================
// Summary:
//     Enumerates the linear gradient mode.
// Remarks:
//===========================================================================
enum XTPChartLinearGradientMode
{
	xtpChartLinearGradientModeHorizontal,        //The gradient mode horizontal
	xtpChartLinearGradientModeVertical,          //The gradient mode vertical
	xtpChartLinearGradientModeForwardDiagonal,   //The gradient mode forward dialoganl
	xtpChartLinearGradientModeBackwardDiagonal,  //The gradient mode backward dialoganl
	xtpChartLinearGradientModeCenterHorizontal,  //The gradient mode horizontal
	xtpChartLinearGradientModeCenterVertical,    //The gradient mode vertical
};

typedef CArray<float, float> CXTPChartLineDashArray;

#endif //#if !defined(__XTPCHARTDEFINES_H__)
