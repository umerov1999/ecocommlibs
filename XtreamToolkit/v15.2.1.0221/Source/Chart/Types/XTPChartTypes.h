// XTPChartTypes.h
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
#if !defined(__XTPCHARTTYPES_H__)
#define __XTPCHARTTYPES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define float_EPSILON 1.192092896e-07F

class CXTPPropExchange;

#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

typedef CXTPPoint2f CXTPChartPointF;
typedef CXTPSizeF   CXTPChartSizeF;
typedef CXTPRectF   CXTPChartRectF;

typedef CArray<CXTPChartPointF, const CXTPChartPointF&> CXTPChartPoints;


typedef CString CXTPChartString;

//===========================================================================
// Summary:
//     This class abstracts a color entity.This object consists of an ARGB
//     value which represents a color.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartColor
{
public:
	typedef DWORD ARGB;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor, creates a CXTPChartColor object and initializes
	//     the values.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor()
	{
		Argb = CXTPChartColor::Empty;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates an opaque color object from the
	//     specified Red, Green, Blue values.
	// Parameters:
	//     r - Red value.
	//     g - Green value.
	//     b - Blue value.
	// Remarks:
	//     The alpha channel value is set to 255 hence the color is 100% opaque
	//     Color values are not premultiplied.
	//-----------------------------------------------------------------------
	CXTPChartColor(BYTE r, BYTE g, BYTE b)
	{
		Argb = MakeARGB(255, r, g, b);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a color object from the specified
	//     Red, Green, Blue values.
	// Parameters:
	//     r - Red value.
	//     g - Green value.
	//     b - Blue value.
	// Remarks:
	//     Color values are not premultiplied.
	//-----------------------------------------------------------------------
	CXTPChartColor(BYTE a, BYTE r, BYTE g, BYTE b)
	{
		Argb = MakeARGB(a, r, g, b);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a color object from the specified
	//     premultiplied ARGB values.
	// Parameters:
	//     argb - The ARGB value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor(ARGB argb)
	{
		Argb = argb;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the alpha channel value of the color.
	// Returns:
	//     A BYTE value specifying the alpha value, the range is from
	//     0 to 255.
	// Remarks:
	//-----------------------------------------------------------------------
	BYTE GetAlpha() const
	{
		return (BYTE) (Argb >> AlphaShift);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the alpha channel value of the color.
	// Returns:
	//     A BYTE value specifying the alpha value, the range is from
	//     0 to 255.
	// Remarks:
	//-----------------------------------------------------------------------
	BYTE GetA() const
	{
		return GetAlpha();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the RED channel value of the color.
	// Returns:
	//     A BYTE value specifying the RED value, the range is from
	//     0 to 255.
	// Remarks:
	//-----------------------------------------------------------------------
	BYTE GetRed() const
	{
		return (BYTE) (Argb >> RedShift);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the RED channel value of the color.
	// Returns:
	//     A BYTE value specifying the RED value, the range is from
	//     0 to 255.
	// Remarks:
	//-----------------------------------------------------------------------
	BYTE GetR() const
	{
		return GetRed();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the GREEN channel value of the color.
	// Returns:
	//     A BYTE value specifying the GREEN value, the range is from
	//     0 to 255.
	// Remarks:
	//-----------------------------------------------------------------------
	BYTE GetGreen() const
	{
		return (BYTE) (Argb >> GreenShift);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the GREEN channel value of the color.
	// Returns:
	//     A BYTE value specifying the GREEN value, the range is from
	//     0 to 255.
	// Remarks:
	//-----------------------------------------------------------------------
	BYTE GetG() const
	{
		return GetGreen();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the BLUE channel value of the color.
	// Returns:
	//     A BYTE value specifying the BLUE value, the range is from
	//     0 to 255.
	// Remarks:
	//-----------------------------------------------------------------------
	BYTE GetBlue() const
	{
		return (BYTE) (Argb >> BlueShift);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the BLUE channel value of the color.
	// Returns:
	//     A BYTE value specifying the BLUE value, the range is from
	//     0 to 255.
	// Remarks:
	//-----------------------------------------------------------------------
	BYTE GetB() const
	{
		return GetBlue();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the ARGB value of the color object.
	// Returns:
	//     A ARGB value specifying the color.
	// Remarks:
	//-----------------------------------------------------------------------
	ARGB GetValue() const
	{
		return Argb;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the ARGB value of the color object.
	// Parameters:
	//     argb - A ARGB value specifying the color.
	// Remarks:
	//-----------------------------------------------------------------------
	VOID SetValue(ARGB argb)
	{
		Argb = argb;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the opaque color value from a COLORREF value.
	// Parameters:
	//     rgb - A RGB value specifying the color.
	//     The alpha channel value is set to 255 hence the color is 100% opaque
	// Remarks:
	//-----------------------------------------------------------------------
	VOID SetFromCOLORREF(COLORREF rgb)
	{
		Argb = MakeARGB(255, GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the color value from a string value.
	// Parameters:
	//     lpsz - The string contains the color values in a specified format.
	// Remarks:
	//     The format should be "100,100,100,100" for ARGB values
	//     or "100,100,100" for opaque color values
	//-----------------------------------------------------------------------
	void SetFromString(LPCTSTR lpsz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to convert the object to a COLORREF value.
	// Returns:
	//     A ARGB value specifying the color.
	// Remarks:
	//-----------------------------------------------------------------------
	COLORREF ToCOLORREF() const
	{
		return RGB(GetRed(), GetGreen(), GetBlue());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the color object is empty.
	// Returns:
	//     A BOOL value of TRUE if the object is empty and FALSE if the object
	//     is not empty.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IsEmpty() const
	{
		return Argb == Empty;
	}

	CXTPChartColor GetDarkColor() const;

public:

	// Common color constants

	const static ARGB AliceBlue;
	const static ARGB AntiqueWhite;
	const static ARGB Aqua;
	const static ARGB Aquamarine;
	const static ARGB Azure;
	const static ARGB Beige;
	const static ARGB Bisque;
	const static ARGB Black;
	const static ARGB BlanchedAlmond;
	const static ARGB Blue;
	const static ARGB BlueViolet;
	const static ARGB Brown;
	const static ARGB BurlyWood;
	const static ARGB CadetBlue;
	const static ARGB Chartreuse;
	const static ARGB Chocolate;
	const static ARGB Coral;
	const static ARGB CornflowerBlue;
	const static ARGB Cornsilk;
	const static ARGB Crimson;
	const static ARGB Cyan;
	const static ARGB DarkBlue;
	const static ARGB DarkCyan;
	const static ARGB DarkGoldenrod;
	const static ARGB DarkGray;
	const static ARGB DarkGreen;
	const static ARGB DarkKhaki;
	const static ARGB DarkMagenta;
	const static ARGB DarkOliveGreen;
	const static ARGB DarkOrange;
	const static ARGB DarkOrchid;
	const static ARGB DarkRed;
	const static ARGB DarkSalmon;
	const static ARGB DarkSeaGreen;
	const static ARGB DarkSlateBlue;
	const static ARGB DarkSlateGray;
	const static ARGB DarkTurquoise;
	const static ARGB DarkViolet;
	const static ARGB DeepPink;
	const static ARGB DeepSkyBlue;
	const static ARGB DimGray;
	const static ARGB DodgerBlue;
	const static ARGB Firebrick;
	const static ARGB FloralWhite;
	const static ARGB ForestGreen;
	const static ARGB Fuchsia;
	const static ARGB Gainsboro;
	const static ARGB GhostWhite;
	const static ARGB Gold;
	const static ARGB Goldenrod;
	const static ARGB Gray;
	const static ARGB Green;
	const static ARGB GreenYellow;
	const static ARGB Honeydew;
	const static ARGB HotPink;
	const static ARGB IndianRed;
	const static ARGB Indigo;
	const static ARGB Ivory;
	const static ARGB Khaki;
	const static ARGB Lavender;
	const static ARGB LavenderBlush;
	const static ARGB LawnGreen;
	const static ARGB LemonChiffon;
	const static ARGB LightBlue;
	const static ARGB LightCoral;
	const static ARGB LightCyan;
	const static ARGB LightGoldenrodYellow;
	const static ARGB LightGray;
	const static ARGB LightGreen;
	const static ARGB LightPink;
	const static ARGB LightSalmon;
	const static ARGB LightSeaGreen;
	const static ARGB LightSkyBlue;
	const static ARGB LightSlateGray;
	const static ARGB LightSteelBlue;
	const static ARGB LightYellow;
	const static ARGB Lime;
	const static ARGB LimeGreen;
	const static ARGB Linen;
	const static ARGB Magenta;
	const static ARGB Maroon;
	const static ARGB MediumAquamarine;
	const static ARGB MediumBlue;
	const static ARGB MediumOrchid;
	const static ARGB MediumPurple;
	const static ARGB MediumSeaGreen;
	const static ARGB MediumSlateBlue;
	const static ARGB MediumSpringGreen;
	const static ARGB MediumTurquoise;
	const static ARGB MediumVioletRed;
	const static ARGB MidnightBlue;
	const static ARGB MintCream;
	const static ARGB MistyRose;
	const static ARGB Moccas;
	const static ARGB NavajoWhite;
	const static ARGB Navy;
	const static ARGB OldLace;
	const static ARGB Olive;
	const static ARGB OliveDrab;
	const static ARGB Orange;
	const static ARGB OrangeRed;
	const static ARGB Orchid;
	const static ARGB PaleGoldenrod;
	const static ARGB PaleGreen;
	const static ARGB PaleTurquoise;
	const static ARGB PaleVioletRed;
	const static ARGB PapayaWhip;
	const static ARGB PeachPuff;
	const static ARGB Peru;
	const static ARGB Pink;
	const static ARGB Plum;
	const static ARGB PowderBlue;
	const static ARGB Purple;
	const static ARGB Red;
	const static ARGB RosyBrown;
	const static ARGB RoyalBlue;
	const static ARGB SaddleBrown;
	const static ARGB Salmon;
	const static ARGB SandyBrown;
	const static ARGB SeaGreen;
	const static ARGB SeaShell;
	const static ARGB Sienna;
	const static ARGB Silver;
	const static ARGB SkyBlue;
	const static ARGB SlateBlue;
	const static ARGB SlateGray;
	const static ARGB Snow;
	const static ARGB SpringGreen;
	const static ARGB SteelBlue;
	const static ARGB Tan;
	const static ARGB Teal;
	const static ARGB Thistle;
	const static ARGB Tomato;
	const static ARGB Transparent;
	const static ARGB Turquoise;
	const static ARGB Violet;
	const static ARGB Wheat;
	const static ARGB White;
	const static ARGB WhiteSmoke;
	const static ARGB Yellow;
	const static ARGB YellowGreen;
	const static ARGB Empty;

	//===========================================================================
	// Summary:
	//     Enumeration for shift count of A, R, G, B components
	// Remarks:
	//===========================================================================
	enum
	{
		AlphaShift  = 24,
		RedShift    = 16,
		GreenShift  = 8,
		BlueShift   = 0
	};
	//===========================================================================
	// Summary:
	//     Enumeration for bit mask of A, R, G, B components
	// Remarks:
	//===========================================================================
	enum
	{
		AlphaMask   = 0xff000000,
		RedMask     = 0x00ff0000,
		GreenMask   = 0x0000ff00,
		BlueMask    = 0x000000ff
	};

	// Assemble A, R, G, B values into a 32-bit integer
	//===========================================================================
	// Summary:
	//     Call this static function to assemble A, R, G, B values into a 32-bit
	//     integer ARGB value.
	// Parameters:
	//     a - The Alpha value.
	//     r - The Red value
	//     g - The Green value
	//     b - The Blue value
	// Returns:
	//     A 32 bit ARGB value.
	// Remarks:
	//===========================================================================
	static ARGB MakeARGB(BYTE a,
						 BYTE r,
						 BYTE g,
						 BYTE b)
	{
		return (((ARGB) (b) <<  BlueShift) |
				((ARGB) (g) << GreenShift) |
				((ARGB) (r) <<   RedShift) |
				((ARGB) (a) << AlphaShift));
	}

protected:

	ARGB Argb;  //The 32 bit color value.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	OLE_COLOR ToOleColor();
	static CXTPChartColor AFX_CDECL FromOleColor(OLE_COLOR);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class abstracts a font entity.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFont : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor, creates a CXTPChartFont object and initializes
	//     the values.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a font object from the LOGFONT structure.
	// Parameters:
	//     pLogFont - A pointer to the LOGFONT structure.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFont(LOGFONT* pLogFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destructor, does the cleaning.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual ~CXTPChartFont();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the LOGFONT for this object.
	// Parameters:
	//     lf - A pointer to the LOGFONT structure.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetLogFont(LOGFONT* lf);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this static function to get Tahoma font with size 18.
	// Returns:
	//     A pointer to the CXTPChartFont object.
	// Remarks:
	//     This function create a new CXTPChartFont and set the underlying font to
	//     Tahoma with size 18.
	//-----------------------------------------------------------------------
	static CXTPChartFont* AFX_CDECL GetTahoma18();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this static function to get Tahoma font with size 12.
	// Returns:
	//     A pointer to the CXTPChartFont object.
	// Remarks:
	//     This function create a new CXTPChartFont and set the underlying font to
	//     Tahoma with size 12.
	//-----------------------------------------------------------------------
	static CXTPChartFont* AFX_CDECL GetTahoma12();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this static function to get Tahoma font with size 8.
	// Returns:
	//     A pointer to the CXTPChartFont object.
	// Remarks:
	//     This function create a new CXTPChartFont and set the underlying font to
	//     Tahoma with size 8.
	//-----------------------------------------------------------------------
	static CXTPChartFont* AFX_CDECL GetTahoma8();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the clean up.
	//-----------------------------------------------------------------------
	void Release();


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartFont);
	afx_msg void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg LPFONTDISP OleGetFont();
//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	LOGFONT m_lf;   //The LOGFONT variable.
};

BOOL AFX_CDECL PX_Color(CXTPPropExchange* pPX, LPCTSTR pszPropName, CXTPChartColor& clrValue);
BOOL AFX_CDECL PX_Font(CXTPPropExchange* pPX, LPCTSTR pszPropName, CXTPChartFont* pFont);

#ifdef _XTP_ACTIVEX
	extern double AFX_CDECL VariantToDoubleEx(const VARIANT* pVariant);
	extern BOOL AFX_CDECL IsStringVariant(const VARIANT* pVariant);
	extern double AFX_CDECL VariantToDouble(const VARIANT* pVariant);
#endif

#endif //#if !defined(__XTPCHARTTYPES_H__)
