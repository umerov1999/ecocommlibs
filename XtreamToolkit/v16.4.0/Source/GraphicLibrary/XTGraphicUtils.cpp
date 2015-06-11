// XTGraphicUtils.cpp interface for the XTPGraphicUtils class.
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

#include "stdafx.h"
#include "GdiPlus/GdiPlus.h"
#include "XTGraphicUtils.h"

#include "Common/XTPResourceManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

BOOL GetEncoderClsid(const WCHAR *pFormat, CLSID *pClsid)
{
	UINT num, size;
	GdipGetImageEncodersSize(&num, &size);

	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	GdipGetImageEncoders(num, size, pImageCodecInfo);

	BOOL found = FALSE;

	for (UINT ix = 0; !found && ix < num; ++ix)
	{
		if (_wcsicmp(pImageCodecInfo[ix].MimeType, pFormat) == 0)
		{
			*pClsid = pImageCodecInfo[ix].Clsid;
			found = TRUE;
			break;
		}
	}

	free(pImageCodecInfo);
	return found;
}

LPCWSTR GetMimeType(LPCTSTR lpszFileName)
{
	int len = (int)_tcslen(lpszFileName);

	if (len > 3 && _tcsicmp(lpszFileName + len - 4, _T(".png")) == 0)
		return L"image/png";

	if (len > 3 && _tcsicmp(lpszFileName + len - 4, _T(".jpg")) == 0)
		return L"image/jpeg";

	if (len > 3 && _tcsicmp(lpszFileName + len - 4, _T(".gif")) == 0)
		return L"image/gif";

	return L"image/bmp";
}

void CXTPGraphicUtils::SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpszFileName) // static
{
	GpBitmap* pGpBitmap = NULL;
	GdipCreateBitmapFromHBITMAP(hBitmap, 0, &pGpBitmap);

	if (pGpBitmap)
	{
		CLSID encoder;
		if (GetEncoderClsid(GetMimeType(lpszFileName), &encoder))
		{
			GdipSaveImageToFile(pGpBitmap, XTP_CT2CW(lpszFileName), &encoder, NULL);
		}

		GdipDisposeImage(pGpBitmap);
	}

	return;
}

// this function used a bit long way of creating Gdiplus::Bitmap from Gdiplus::GpBitmap, but found no
// other way to create Gdiplus::Bitmap from Gdiplus::GpBitmap

Gdiplus::Status CXTPGraphicUtils::GdipCreateBitmapFromHICON_Fixed(HICON hicon, Gdiplus::Bitmap** bitmap)
{
	Gdiplus::Status retStatus = Gdiplus::GenericError;

	Gdiplus::GpBitmap *nativeBmp = NULL;
	GdipCreateBitmapFromHICON_Fixed_Native(hicon, &nativeBmp);

	if (nativeBmp)
	{
		HICON hIconFixed = NULL;
		DllExports::GdipCreateHICONFromBitmap(nativeBmp, &hIconFixed);

		if (hIconFixed)
		{
			if (bitmap)
			{
				*bitmap = Gdiplus::Bitmap::FromHICON(hIconFixed);

				retStatus = (*bitmap)->GetLastStatus();
			}

			::DestroyIcon(hIconFixed);
		}

		// GpBitmap is not required to be freed
	}

	return retStatus;
}

Gdiplus::Status CXTPGraphicUtils::GdipCreateBitmapFromHICON_Fixed_Native(HICON hicon, Gdiplus::GpBitmap** bitmap)
{
	Gdiplus::GpStatus returnStatus = Gdiplus::Ok;

	Gdiplus::GpStatus stat;

	BITMAP bm;
	int width = 0, height = 0;
	Gdiplus::Rect rect;
	Gdiplus::BitmapData lockeddata;
	BOOL has_alpha;
	int x, y;

	BITMAPINFOHEADER bih;
	DWORD *src = NULL;
	BYTE *dst_row = NULL;
	DWORD *dst = NULL;

	ICONINFO iinfo;
	ZeroMemory(&iinfo, sizeof(ICONINFO));

	BYTE *pbBitmapBits = NULL;

	HDC hScreenDC = NULL;

	try
	{
		if(!bitmap || !GetIconInfo(hicon, &iinfo))
			throw Gdiplus::InvalidParameter;

		// get the size of the icon
		if (!GetObject(iinfo.hbmColor ? iinfo.hbmColor : iinfo.hbmMask, sizeof(bm), &bm))
			throw Gdiplus::GenericError;

		width = bm.bmWidth;

		if (iinfo.hbmColor)
			height = abs(bm.bmHeight);
		else // combined bitmap + mask
			height = abs(bm.bmHeight) / 2;

		pbBitmapBits = (BYTE *)HeapAlloc(GetProcessHeap(), 0, 4*width*height);

		if (!pbBitmapBits)
			throw Gdiplus::OutOfMemory;

		stat = DllExports::GdipCreateBitmapFromScan0(width, height, 0, PixelFormat32bppARGB, NULL, bitmap);

		rect.X = 0;
		rect.Y = 0;
		rect.Width = width;
		rect.Height = height;

		stat = DllExports::GdipBitmapLockBits(*bitmap, &rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &lockeddata);

		bih.biSize = sizeof(bih);
		bih.biWidth = width;
		bih.biHeight = -height;
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = 0;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		bih.biClrUsed = 0;
		bih.biClrImportant = 0;

		hScreenDC = GetDC(NULL);
		if (iinfo.hbmColor)
		{
			GetDIBits(hScreenDC, iinfo.hbmColor, 0, height, pbBitmapBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

			if (bm.bmBitsPixel == 32)
			{
				has_alpha = FALSE;

				// If any pixel has a non-zero alpha, ignore hbmMask
				src = (DWORD*)pbBitmapBits;
				for (x=0; x<width && !has_alpha; x++)
					for (y=0; y<height && !has_alpha; y++)
						if ((*src++ & 0xff000000) != 0)
							has_alpha = TRUE;
			}
			else
				has_alpha = FALSE;
		}
		else
		{
			GetDIBits(hScreenDC, iinfo.hbmMask, 0, height, pbBitmapBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

			has_alpha = FALSE;
		}

		// copy the image data to the Bitmap
		src = (DWORD*)pbBitmapBits;
		dst_row = (BYTE *)lockeddata.Scan0;
		for (y=0; y<height; y++)
		{
			memcpy(dst_row, src, width*4);
			src += width;
			dst_row += lockeddata.Stride;
		}

		if (!has_alpha)
		{
			if (iinfo.hbmMask)
			{
				// read alpha data from the mask
				if (iinfo.hbmColor)
					GetDIBits(hScreenDC, iinfo.hbmMask, 0, height, pbBitmapBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
				else
					GetDIBits(hScreenDC, iinfo.hbmMask, height, height, pbBitmapBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

				src = (DWORD*)pbBitmapBits;
				dst_row = (BYTE *)lockeddata.Scan0;
				for (y=0; y<height; y++)
				{
					dst = (DWORD*)dst_row;
					for (x=0; x<height; x++)
					{
						DWORD src_value = *src++;
						if (src_value)
							*dst++ = 0;
						else
							*dst++ |= 0xff000000;
					}
					dst_row += lockeddata.Stride;
				}
			}
			else
			{
				// set constant alpha of 255
				dst_row = pbBitmapBits;
				for (y=0; y<height; y++)
				{
					dst = (DWORD*)dst_row;
					for (x=0; x<height; x++)
						*dst++ |= 0xff000000;
					dst_row += lockeddata.Stride;
				}
			}
		}
	}
	catch (Gdiplus::Status err_status)
	{
		returnStatus = err_status;
	}

	if (hScreenDC)
		ReleaseDC(0, hScreenDC);

	if (iinfo.hbmColor)
		DeleteObject(iinfo.hbmColor);

	if (iinfo.hbmColor)
		DeleteObject(iinfo.hbmMask);

	if (bitmap && *bitmap)
		stat = DllExports::GdipBitmapUnlockBits(*bitmap,&lockeddata);

	if (pbBitmapBits)
		HeapFree(GetProcessHeap(), 0, pbBitmapBits);

	return returnStatus;
}
