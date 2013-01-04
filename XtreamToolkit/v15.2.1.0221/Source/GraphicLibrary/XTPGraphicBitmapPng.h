// XTPGraphicBitmapPng.h interface for the XTPGraphicBitmapPng class.
//
// This file is a part of the XTREME TOOLKITPRO MFC class library.
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
#if !defined(__XTPGRAPHICBITMAPPNG_H__)
#define __XTPGRAPHICBITMAPPNG_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     CXTPGraphicBitmapPng is CBitmap derived class used to load PNG files from file and resources.
//===========================================================================
class _XTP_EXT_CLASS CXTPGraphicBitmapPng : public CBitmap
{
	struct CCallback;
public:

	//-------------------------------------------------------------------------
	// Summary: Default constructor
	//-------------------------------------------------------------------------
	CXTPGraphicBitmapPng();

public:

	//-----------------------------------------------------------------------
	// Summary: Call this method to load PNG file
	// Input:   lpszFileName - A string value that contains the name of a file to be loaded.
	// Returns: TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromFile(LPCTSTR lpszFileName);

	//-----------------------------------------------------------------------
	// Summary: Call this member function to load the PNG file from specified resource.
	// Input:   hModule - Module instance to load
	//          hRes - Resource handle
	// Returns: TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromResource(HMODULE hModule, HRSRC hRes);

	//-----------------------------------------------------------------------
	// Summary: Call this method to load PNG file
	// Input:   pFile - pointer to CFile to be loaded.
	// Returns: TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromFile(CFile* pFile);

	//-----------------------------------------------------------------------
	// Summary: Determines if loaded PNG has Alpha layer
	// Returns: TRUE if PNG file has alpha layer.
	//-----------------------------------------------------------------------
	BOOL IsAlpha() const;

private:
	HBITMAP ConvertToBitmap(BYTE* pbImage, CSize szImage,  int cImgChannels) const;

protected:
	BOOL m_bAlpha;          // TRUE if PNG file has alpha layer.
};

AFX_INLINE BOOL CXTPGraphicBitmapPng::IsAlpha() const {
	return m_bAlpha;
}

_XTP_EXT_CLASS int AFX_CDECL ZLibCompress(BYTE *dest, ULONG* destLen, const BYTE *source, ULONG sourceLen);
_XTP_EXT_CLASS ULONG AFX_CDECL ZLibCompressBound(ULONG sourceLen);
_XTP_EXT_CLASS int AFX_CDECL ZLibUncompress (BYTE *dest, ULONG* destLen, const BYTE* source, ULONG sourceLen);

#endif // #if !defined(__XTPGRAPHICBITMAPPNG_H__)
