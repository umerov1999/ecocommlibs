/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

// original author Tommy Sighagen

#ifndef GFX_ELEMENT_H
#define GFX_ELEMENT_H

// Common includes
#include <os/typedefs.h>
#include <common/anotocommon.h>

#ifdef __cplusplus
extern "C"
{
#endif


////////////////////////////////////////////////////////////////////////////////////
// gfx.h
////////////////////////////////////////////////////////////////////////////////////

/** \ingroup GFXAPI	
    \defgroup GFXEAPI Graphics Element API
 
 A graphics element represent visible (human readable) graphics and is typically added to
 a \ref GraphicsCollection of a \ref PageTemplate or \ref PageInstance.

 An important graphics element is the \ref GfxDrawingArea. It is used to put pattern on a page.

 Some elements, such as for example \ref GfxImage, require binary resources. In this case the
 binary data must be added using \ref docAddLayoutResource() in addition to adding the element.

 2D Graphics see \ref GfxE
	
@{*/

/** \defgroup GfxCodes Gfx Return codes and defines
@{*/

/** Operation failed */
#define GFX_ERROR -1 
/** Use width and height specified in the graphics element */
#define GFX_NO_AREA -1 
/** Current Page */
#define GFX_CURRENT_PAGE -1 
/** Current Copy */
#define GFX_CURRENT_COPY -1 
/** Default value*/
#define GFX_DEFAULT -1 
/** Default margin for cut pattern (7mm). Defined in AU. If other scale used this value must be transformed. I.e. if mm use metAU2MM(GFX_DEFAULT_MARGIN)*/
#define GFX_DEFAULT_MARGIN (22*8) // defined in AU !
/** Default dotcode height (1cm) */
#define GFX_DEFAULT_DC_HEIGHT (33)
/** dotcode can be any width */
#define GFX_DC_NO_WIDTH (-1)
/** Do not fill the graphics element */
#define GFX_BOX_NO_FILL 50
/** Fill the graphics element */
#define GFX_BOX_FILL 51
/** Rotate graphics element horizontally*/
#define GFX_HORIZONTAL 52
/** Rotate graphics element vertically*/
#define GFX_VERTICAL 53
/*@}*/

/** \defgroup GfxElementType Gfx Element type integer
@{*/
/** Gfx No type */
#define GFX_GRAPHICS			0
/** \ref GfxText */
#define GFX_TEXT			    1
/** \ref GfxImage */
#define GFX_IMAGE				2
/** \ref GfxBox */
#define GFX_BOX					3
/** \ref GfxPidgetBox */
#define GFX_PIDGETBOX			4
/** \ref GfxRGBColor */
#define GFX_RGBCOLOR			5
/** \ref GfxCMYKColor */
#define GFX_CMYKCOLOR			6
/** \ref GfxScale */
#define GFX_SCALE				7
/** \ref GfxDrawingArea */
#define GFX_DRAWINGAREA			8
/** \ref GfxPattern */
#define GFX_PATTERN				9
/** \ref GfxPidget */
#define GFX_PIDGET				10
/** \ref GfxClearPattern */
#define GFX_CLEARPATTERN		11
/** \ref GfxDotcode */
#define GFX_DOTCODE				12
/** \ref GfxActionArea */
#define GFX_ACTIONAREA			13
/*@}*/

	


/** \struct GfxElement
	\brief A abstract 2D Graphics element

	A graphics element is a 2d graphics element. The subclasses can be cast to this type for certain operations
	A collection of graphics elements build up a \ref GraphicsCollection
@{*/
	struct GfxElement;
/*@}*/

/** \struct GfxText
	\brief Text graphics

	A GfxText draws a text on the canvas\n
	A GfxText element contains the following properties\n
	AreaId, X, Y, Size, Font, Text
@{*/
	struct GfxText;
/*@}*/

/** \struct GfxImage
	\brief Bitmap or vector image

	A GfxImage element adds an image to the canvas
	A GfxImage element contains the following properties\n
	AreaId, X, Y, Width, Height, image source page and source file
@{*/
	struct GfxImage;
/*@}*/

/** \struct GfxBox
	\brief A box object

	A GfxBox element draws a box on the canvas
	A GfxBox element contains the following properties\n
	AreaId, X, Y, Width, Height, Fill
@{*/
	struct GfxBox;
/*@}*/

	/** \struct GfxPidgetBox
	\brief A pidget box object

	A GfxPidgetBox element draws a pidgetbox (round corners) on the canvas
	A GfxPidgetBox element contains the following properties\n
	AreaId, X, Y, Width, Height
@{*/
	struct GfxPidgetBox;
/*@}*/

/** \struct GfxRGBColor
	\brief Changes color

	A GfxRGBColor element changes the color of items after it and it contains the following properties\n
	Red, Green, Blue
@{*/
	struct GfxRGBColor;
/*@}*/

/** \struct GfxCMYKColor
	\brief Changes color

	A GfxCMYKColor element changes the color of items after it and it contains the following properties\n
	C,M,Y,K
@{*/
	struct GfxCMYKColor;
/*@}*/


/** \struct GfxScale
	\brief Changes scale

	A GfxScale element changes the scale of all items up to the next GfxScale it contains the property\n
	scale
@{*/
	struct GfxScale;
/*@}*/

/** \struct GfxDrawingArea
	\brief Pattern drawing area

	A GfxDrawingArea element contains the following properties\n
	AreaId, X, Y, Width, Height
	
	Drawing area is added according to the \ref CCP

@{*/
	struct GfxDrawingArea;
/*@}*/

/** \struct GfxPattern
	\brief Adds cut pattern into a page

	A GfxPattern object cuts pattern into a page from a source area on a page and copy into the current page
	It shall be the size of the referenced object and the data inside it. Note that it needs to include appropriate extra
	region offset to work in all cases because the pen tip bias. 
	It contains the following properties:\n
	AreaId (what area to copy to), X, Y, Source page, Source Copy, Source Area Margin
@{*/
	struct GfxPattern;
/*@}*/

/** \struct GfxPidget
	\brief Adds an area from AFD to page

	A GfxPidget object cuts pattern into a page from a source area on a page and copy from another AFD into the current page
	It shall be the size of the referenced object and the data inside it. Note that it needs to include appropriate extra
	region offset to work in all cases because the pen tip bias
	It contains the following properties:\n
	AreaId (what area to copy to), X, Y, Source page, Source Copy, Source Area, Margin, Source AFD
@{*/
	struct GfxPidget;
/*@}*/

/** \struct GfxClearPattern
	\brief Cuts holes in pattern

	A GfxClearPattern element contains the following properties\n
	AreaId, X, Y, Width, Height
	
@{*/
	struct GfxClearPattern;
/*@}*/



/** \struct GfxDotcode
	\brief Creates a Dotcode

	A GfxDotcode element contains the following properties\n
	AreaId, X, Y, Width, Height, MessageType, Encoding, Transpose and Data

	Note that if the dotcode is rotated 90 degrees width and height are swapped

	See \ref DC
	
@{*/
	struct GfxDotcode;
/*@}*/

/** \struct GfxActionArea
	\brief Pattern area for activating functionality in the pen or system.

	A GfxActionArea object will cut in special pattern with the ability to activate functionality 
	in the pen or system such as pairing, color etc. The action area concept is quite general, it only
	connects a pattern space to a specific type and value. This pattern space, when written on by the pen,
	will set or trigger a specific state or pen action. Exactly which is pen specific except for 
	type 0 (Pairing) and type 1 (Color). The support for action areas also differs between pens,
	some don't support it at all.
@{*/
	struct GfxActionArea;
/*@}*/



//////////////////////////////
// Construction
//////////////////////////////

EXTAPI_DOCUMENT_H struct GfxText *gfxeCreateText( float ifX, float ifY, float ifAngle, const char *izFont,  const char *izFontEncoding,char *izText, float ifTextSize, s64 illAreaId);
EXTAPI_DOCUMENT_H struct GfxImage *gfxeCreateImage( float ifX, float ifY, float ifWidth, float ifHeight, s32 ilSourcePage, const char *izFilename, s64 illAreaId);
EXTAPI_DOCUMENT_H struct GfxBox *gfxeCreateBox( float ifX, float ifY, float ifWidth, float ifHeight, float ifThickness, s32 ilFill , s64 illAreaId);
EXTAPI_DOCUMENT_H struct GfxPidgetBox *gfxeCreatePidgetBox( float ifX, float ifY, float ifWidth, float ifHeight , s64 illAreaId);
EXTAPI_DOCUMENT_H struct GfxRGBColor *gfxeCreateRGBColor(float ifRed, float ifGreen, float ifBlue);
EXTAPI_DOCUMENT_H struct GfxCMYKColor *gfxeCreateCMYKColor(float ifC, float ifM, float ifY, float ifK );
EXTAPI_DOCUMENT_H struct GfxScale * gfxeCreateScale(const char* izScale);
EXTAPI_DOCUMENT_H struct GfxDrawingArea *gfxeCreateDrawingArea( float ifX, float ifY, float ifWidth, float ifHeight , s64 illAreaId);
EXTAPI_DOCUMENT_H struct GfxPattern *gfxeCreatePattern( float ifDestX, float ifDestY, s64 illAreaId,  s32 ilSourcePage, s32 ilSourceCopy, s64 illSourceAreaId , float ifMargin );
EXTAPI_DOCUMENT_H struct GfxPidget *gfxeCreatePidget( float ifDestX, float ifDestY, s64 illAreaId, s32 ilSourcePage, s32 ilSourceCopy, s64 illSourceAreaId, char *ipAFDfilename , float ifMargin );
EXTAPI_DOCUMENT_H struct GfxClearPattern *gfxeCreateClearPattern( float ifX, float ifY, float ifWidth, float ifHeight , s64 illAreaId);
EXTAPI_DOCUMENT_H struct GfxDotcode *gfxeCreateDotcode( float ifX, float ifY, float ifWidth, float ifHeight ,  s64 illAreaId, u8 iubMessageType, s32 ilTranspose, char *izEncoding, u8 *iupbData,  s32 ilDataLen );
EXTAPI_DOCUMENT_H struct GfxActionArea *gfxeCreateActionArea(float ifX, float ifY, u8 iubSize, u8 iubType, u16 iusValue, float ifMargin);


EXTAPI_DOCUMENT_H struct GfxElement *gfxeCreateCopy( struct GfxElement * iptGfxElem );

EXTAPI_DOCUMENT_H void gfxeDestroy( struct GfxElement ** iopptGfxElem );
////////////////////////////////

EXTAPI_DOCUMENT_H size_t gfxeGetMemSize( struct GfxElement *i );

//////////////////////////////
// set / get 
//////////////////////////////

EXTAPI_DOCUMENT_H s32 gfxeGetType( struct GfxElement *iptGfxElem );

/* Get superclass for derived objects: */
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForText( struct GfxText *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForImage( struct GfxImage *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForBox( struct GfxBox *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForPidgetBox( struct GfxPidgetBox *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForRGBColor( struct GfxRGBColor *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForCMYKColor( struct GfxCMYKColor *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForScale( struct GfxScale *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForDrawingArea( struct GfxDrawingArea *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForPattern( struct GfxPattern *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForPidget( struct GfxPidget *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForClearPattern( struct GfxClearPattern *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForDotCode( struct GfxDotcode *iptElement );
EXTAPI_DOCUMENT_H struct GfxElement *gfxeGetSuperForActionArea( struct GfxActionArea *iptElement );

// ---  get functions ---

EXTAPI_DOCUMENT_H s64 gfxeGetAreaId(struct GfxElement *iptGfxElem);

EXTAPI_DOCUMENT_H float gfxeGetX(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetY(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetWidth(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetHeight(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetAngle(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetMargin(struct GfxElement *iptGfxElem);

// image centric

EXTAPI_DOCUMENT_H s32 gfxeGetImagePage(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H boolean gfxeGetImageFilename(struct GfxElement *iptGfxElem, char *ozFilenameBuf, s32 ilSizeBuf);

// text centric

EXTAPI_DOCUMENT_H boolean gfxeGetFont(struct GfxElement *iptGfxElem, char *ozFontBuf, s32 lSizeBuf);
EXTAPI_DOCUMENT_H boolean gfxeGetFontEncoding(struct GfxElement *iptGfxElem, char *ozFontBuf, s32 lSizeBuf);
EXTAPI_DOCUMENT_H boolean gfxeGetText(struct GfxElement *iptGfxElem, char *ozTextBuf, s32 lSizeBuf);
EXTAPI_DOCUMENT_H float gfxeGetFontSize(struct GfxElement *iptGfxElem);

// color centric

EXTAPI_DOCUMENT_H float gfxeGetRed(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetGreen(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetBlue(struct GfxElement *iptGfxElem);

EXTAPI_DOCUMENT_H float gfxeGetCyan(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetMagenta(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetYellow(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H float gfxeGetK(struct GfxElement *iptGfxElem);

// scale centric

EXTAPI_DOCUMENT_H boolean gfxeGetScale(struct GfxElement *iptGfxElem, char *ozScale, s32 lSizeBuf);
EXTAPI_DOCUMENT_H s32 gfxeGetScaleToAu(struct GfxElement *iptGfxElem);

// pattern centric

EXTAPI_DOCUMENT_H s32 gfxeGetPage(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H s32 gfxeGetCopy(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H s64 gfxeGetSourceAreaId(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H boolean gfxeGetSourceAFD(struct GfxElement *iptGfxElem, char *ozFilenameBuf, s32 lSizeBuf);


// graphics centric

EXTAPI_DOCUMENT_H float gfxeGetThickness(struct GfxElement *iptGfxElem );
EXTAPI_DOCUMENT_H s32 gfxeGetFill(struct GfxElement *iptGfxElem );


// dotcode centric

EXTAPI_DOCUMENT_H s32 gfxeGetMessageType(struct GfxElement *iptGfxElem );
EXTAPI_DOCUMENT_H s32 gfxeGetTranspose(struct GfxElement *iptGfxElem );
EXTAPI_DOCUMENT_H boolean gfxeGetEncoding(struct GfxElement *iptGfxElem, char *ozEncoding, s32 lSizeBuf);
EXTAPI_DOCUMENT_H s32 gfxeGetDataSize(struct GfxElement *iptGfxElem);
EXTAPI_DOCUMENT_H boolean gfxeGetData(struct GfxElement *iptGfxElem, char *opData);

// action area centric

EXTAPI_DOCUMENT_H s32 gfxeGetSize(struct GfxElement *iptGfxElem );
EXTAPI_DOCUMENT_H s32 gfxeGetActionAreaType(struct GfxElement *iptGfxElem );
EXTAPI_DOCUMENT_H s32 gfxeGetActionAreaValue(struct GfxElement *iptGfxElem );


// --- set functions ---

EXTAPI_DOCUMENT_H boolean gfxeSetAreaId(struct GfxElement *iptGfxElem, s64 illAreaId);

EXTAPI_DOCUMENT_H boolean gfxeSetX(struct GfxElement *iptGfxElem, float ifX);
EXTAPI_DOCUMENT_H boolean gfxeSetY(struct GfxElement *iptGfxElem, float ifY);
EXTAPI_DOCUMENT_H boolean gfxeSetAngle(struct GfxElement *iptGfxElem, float ifAngle);
EXTAPI_DOCUMENT_H boolean gfxeSetWidth(struct GfxElement *iptGfxElem, float ifWidth);
EXTAPI_DOCUMENT_H boolean gfxeSetHeight(struct GfxElement *iptGfxElem, float ifHeight);

// image centric

EXTAPI_DOCUMENT_H boolean gfxeSetImagePage(struct GfxElement *iptGfxElem, s32 ilSourcePage);
EXTAPI_DOCUMENT_H boolean gfxeSetImageFilename(struct GfxElement *iptGfxElem, const char *izFilename);

// text centric

EXTAPI_DOCUMENT_H boolean gfxeSetFont(struct GfxElement *iptGfxElem, const char *izFont);
EXTAPI_DOCUMENT_H boolean gfxeSetFontEncoding(struct GfxElement *iptGfxElem, const char *izFontEncoding);
EXTAPI_DOCUMENT_H boolean gfxeSetText(struct GfxElement *iptGfxElem, const char *izText);
EXTAPI_DOCUMENT_H boolean gfxeSetFontSize(struct GfxElement *iptGfxElem, float ifFontSize);

// color centric

EXTAPI_DOCUMENT_H boolean gfxeSetRed(struct GfxElement *iptGfxElem, float ifRed);
EXTAPI_DOCUMENT_H boolean gfxeSetGreen(struct GfxElement *iptGfxElem, float ifGreen);
EXTAPI_DOCUMENT_H boolean gfxeSetBlue(struct GfxElement *iptGfxElem, float ifBlue);

EXTAPI_DOCUMENT_H boolean gfxeSetCyan(struct GfxElement *iptGfxElem, float ifC);
EXTAPI_DOCUMENT_H boolean gfxeSetMagenta(struct GfxElement *iptGfxElem, float ifM);
EXTAPI_DOCUMENT_H boolean gfxeSetYellow(struct GfxElement *iptGfxElem, float ifY);
EXTAPI_DOCUMENT_H boolean gfxeSetK(struct GfxElement *iptGfxElem, float ifK);

// scale centric

EXTAPI_DOCUMENT_H boolean gfxeSetScale(struct GfxElement *iptScaleElem, const char *izScale);

// pattern centric

EXTAPI_DOCUMENT_H boolean gfxeSetPage(struct GfxElement *iptGfxElem, s32 ilPage);
EXTAPI_DOCUMENT_H boolean gfxeSetCopy(struct GfxElement *iptGfxElem, s32 ilCopy);
EXTAPI_DOCUMENT_H boolean gfxeSetSourceAreaId(struct GfxElement *iptGfxElem, s64 illSourceAreaId);
EXTAPI_DOCUMENT_H boolean gfxeSetMargin(struct GfxElement *iptGfxElem, float ifMargin);

EXTAPI_DOCUMENT_H boolean gfxeSetSourceAFD(struct GfxElement *iptGfxElem,  const char *izFilename );

// graphics centric

EXTAPI_DOCUMENT_H boolean gfxeSetThickness(struct GfxElement *iptGfxElem, float ifThickness );
EXTAPI_DOCUMENT_H boolean gfxeSetFill(struct GfxElement *iptGfxElem, s32 ilFill );

// dotcode centric

EXTAPI_DOCUMENT_H boolean gfxeSetMessageType(struct GfxElement *iptGfxElem, u8 iubMessageType );
EXTAPI_DOCUMENT_H boolean gfxeSetTranspose(struct GfxElement *iptGfxElem, s32 ilOrientation );
EXTAPI_DOCUMENT_H boolean gfxeSetEncoding(struct GfxElement *iptGfxElem, const char *izEncoding );
EXTAPI_DOCUMENT_H boolean gfxeSetData(struct GfxElement *iptGfxElem, const u8 *ipubData, s32 ilDataLen );


// action area centric

EXTAPI_DOCUMENT_H boolean gfxeSetSize(struct GfxElement *iptGfxElem, u8 iubSize );
EXTAPI_DOCUMENT_H boolean gfxeSetActionAreaType(struct GfxElement *iptGfxElem, u8 iubActionAreaType );
EXTAPI_DOCUMENT_H boolean gfxeSetActionAreaValue(struct GfxElement *iptGfxElem, u16 iusActionAreaValue );


/*@}*/
/*@}*/

#ifdef __cplusplus
}
#endif

#endif //GFX_ELEMENT_H


