// XTPCalendarShadowDrawer.h: interface for the CXTPCalendarShadowDrawer class.
//
// This file is a part of the XTREME CALENDAR MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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
#if !defined(_XTPCALENDARSHADOWDRAWER_H__)
#define _XTPCALENDARSHADOWDRAWER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     This is a helper class which handles shadows drawing (for events, etc)
//===========================================================================
class CXTPCalendarShadowDrawer
{
public:
	//===========================================================================
	// Summary:
	//     Constructs the object and initializes.
	// Parameters:
	//     pDC          - A pointer to the device context.
	//     dMaxAlpha    - The maximum value for the alpha channel.
	//===========================================================================
	CXTPCalendarShadowDrawer(CDC* pDC, double dMaxAlpha);

	//===========================================================================
	// Summary:
	//     Destroy the CXTPCalendarShadowDrawer and does the cleanups.
	//===========================================================================
	virtual ~CXTPCalendarShadowDrawer();

	//===========================================================================
	// Summary:
	//     Call this function to get the alpha pixel.
	// Parameter:
	//     crPixel - The color of the pixel.
	//     i       - The alpha value.
	// Returns:
	//      The alpha pixel value.
	//===========================================================================
	static COLORREF AlphaPixel(const COLORREF crPixel, const UINT i);

	//===========================================================================
	// Summary:
	//     Call this function to check whether the input is within the limit of
	//     0 and 255.
	// Parameter:
	//      iValue - The input.
	// Returns:
	//      The returns 0 if the input is less than 0 and 255 if it is greater than
	//      255, else returns the value.
	//===========================================================================
	static int CheckValue(int iValue);

	enum XTPShadowPart
	{
		// Regular theme - Office 2003
		xtpShadowLR = 0,
		xtpShadowTB = 1,
		xtpShadowTR = 2,
		xtpShadowBR = 3,
		xtpShadowBL = 4,
		// Office 2007 Theme
		xtpShadow2007Right      = 5,
		xtpShadow2007Bottom     = 6,
		xtpShadow2007TopRight   = 7,
		xtpShadow2007BottomRight= 8,
		xtpShadow2007BottomLeft = 9

	};
	//-----------------------------------------------------------------------
	// Summary:
	//  Generic drawing of the shadow rectangle part
	// Parameters:
	//  rcShadow - rectangle to draw
	//  part - XTPShadowPart
	//-----------------------------------------------------------------------
	void DrawShadowRectPart(const CRect& rcShadow, const XTPShadowPart part);

	// Methods for Office 2003 theme

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadowLR(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadowTB(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadowTR(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadowBR(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadowBL(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	// methods for Office 2007 theme

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadow2007Right(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadow2007Bottom(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadow2007TR(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadow2007BL(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//  applied shadow to bitmap
	// Parameters:
	//  pBitmap - UINT*
	//  ulBitmapWidth - ULONG
	//  ulBitmapHeight - ULONG
	//-----------------------------------------------------------------------
	void ApplyShadow2007BR(UINT* pBitmap, const ULONG ulBitmapWidth, const ULONG ulBitmapHeight);

protected:
	CDC* m_pDC; // drawing device context
	double m_dMaxAlpha; // maximal shadow alpha grade

private:

};

#endif // !defined(_XTPCALENDARSHADOWDRAWER_H__)
