// XTGraphicUtils.h interface for the XTPGraphicUtils class.
//
// This file is a part of the XTREME TOOLKITPRO MFC class library.
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
#if !defined(__XTPGRAPHICUTILS_H__)
#define __XTPGRAPHICUTILS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPGraphicUtils
{
public:

	static void SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);

	static Gdiplus::Status GdipCreateBitmapFromHICON_Fixed(HICON hicon, Gdiplus::Bitmap** bitmap);

	//////////////////////////////////////////////////////////////////////////
	// The following function (GdipCreateBitmapFromHICON_Fixed_Native)
	// is a fixed version of Microsoft one
	// The problem of the original code not saving of transparency
	// of HICON (alpha channel). The bug dates to 2001 (!) when GDI+
	// was originally introduced. It is widely mentioned on Microsoft websites
	// with some workarounds (not working for our case).
	// The bug is not fixed and doesn't seem to be planned to be fixed.
	// The code is based on wine (www.winehq.org) code
	//////////////////////////////////////////////////////////////////////////
	static Gdiplus::Status GdipCreateBitmapFromHICON_Fixed_Native(HICON hicon, Gdiplus::GpBitmap** bitmap);
};

#endif // #if !defined(__XTPGRAPHICUTILS_H__)
