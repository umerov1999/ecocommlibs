// XTPFlowGraphImage.cpp : implementation of the CXTPFlowGraphImage class.
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

#include "stdafx.h"
#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPImageManager.h"

#include "XTPFlowGraphImage.h"

class CXTPFlowGraphResourceStream
{
public:
	CXTPFlowGraphResourceStream(HINSTANCE hInstance, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType);
	~CXTPFlowGraphResourceStream();

public:
	IStream* m_pStream;
};

CXTPFlowGraphResourceStream::CXTPFlowGraphResourceStream(HINSTANCE hInstance, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType)
{
	m_pStream = NULL;

	HRSRC hRsrc = ::FindResource(hInstance, lpszResourceName, lpszResourceType);
	if (hRsrc == NULL)
		return;

	HGLOBAL hGlobal = LoadResource(hInstance, hRsrc);
	if (hGlobal == NULL)
		return;

	LPCSTR pData = (LPCSTR)LockResource(hGlobal);
	if (pData == NULL)
		return;

	DWORD dwSize = (DWORD)SizeofResource(hInstance, hRsrc);
	if (dwSize == 0)
		return;

	// Create a stream object on a memory block.
	HGLOBAL hGlobalBuff = GlobalAlloc(GMEM_MOVEABLE, dwSize);

	CHAR* szBuffer = (CHAR*)::GlobalLock(hGlobalBuff);
	if (szBuffer)
	{
		MEMCPY_S(szBuffer, pData, dwSize);

		::GlobalUnlock(hGlobalBuff);

		if (FAILED(CreateStreamOnHGlobal(hGlobalBuff, TRUE, &m_pStream)))
		{
			m_pStream = NULL;
			GlobalFree(hGlobalBuff);
		}
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);
}

CXTPFlowGraphResourceStream::~CXTPFlowGraphResourceStream()
{
	if (m_pStream)
	{
		m_pStream->Release();
	}
};


//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphImage

CXTPFlowGraphImage::CXTPFlowGraphImage()
{
	m_pImage = NULL;
	m_pIcon = NULL;
	m_szImage = CSize(0, 0);

}

CXTPFlowGraphImage::~CXTPFlowGraphImage()
{
	SAFE_DELETE(m_pImage);
	CMDTARGET_RELEASE(m_pIcon);
}

void CXTPFlowGraphImage::OnRemoved()
{
	SAFE_DELETE(m_pImage);
	CMDTARGET_RELEASE(m_pIcon);
}

void CXTPFlowGraphImage::LoadFromResource(HINSTANCE hInstance, UINT nResourceName)
{
	SAFE_DELETE(m_pImage);
	m_pImage =  new Gdiplus::Bitmap(hInstance, MAKEINTRESOURCEW(nResourceName));
	ImageChanged();
}

void CXTPFlowGraphImage::LoadFromResource(HINSTANCE hInstance, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType)
{
	SAFE_DELETE(m_pImage);

	CXTPFlowGraphResourceStream st(hInstance, lpszResourceName, lpszResourceType);

	if (st.m_pStream != NULL)
	{
		m_pImage =  new Gdiplus::Bitmap(st.m_pStream, FALSE);
	}
	ImageChanged();
}

void CXTPFlowGraphImage::LoadFromFile(LPCTSTR lpszFileName)
{
	SAFE_DELETE(m_pImage);
	m_pImage =  new Gdiplus::Bitmap(XTP_CT2CW(lpszFileName));

	ImageChanged();
}

void CXTPFlowGraphImage::SetBitmap(HBITMAP hBitmap)
{
	SAFE_DELETE(m_pImage);
	m_pImage = new Gdiplus::Bitmap(hBitmap, 0);

	ImageChanged();
}

void CXTPFlowGraphImage::SetIcon(HICON hIcon)
{
	SAFE_DELETE(m_pImage);
	m_pImage =  new Gdiplus::Bitmap(hIcon);

	ImageChanged();
}


template< typename N >
inline N WINAPI XTPAlignUp( N n, ULONG nAlign ) throw()
{
	return( N( (n+(nAlign-1))&~(N( nAlign )-1) ) );
}

void CXTPFlowGraphImage::ImageChanged()
{
	CMDTARGET_RELEASE(m_pIcon);

	if (!m_pImage || m_pImage->GetLastStatus() != Gdiplus::Ok)
	{
		SAFE_DELETE(m_pImage);
	}

	m_szImage = CSize(0, 0);
	if (!m_pImage)
		return;

	m_szImage = CSize(m_pImage->GetWidth(), m_pImage->GetHeight());


	Gdiplus::PixelFormat eSrcPixelFormat = m_pImage->GetPixelFormat();
	UINT nBPP = 32;
	Gdiplus::PixelFormat eDestPixelFormat = PixelFormat32bppRGB;

	if( eSrcPixelFormat & PixelFormatGDI )
	{
		nBPP = Gdiplus::GetPixelFormatSize( eSrcPixelFormat );
		eDestPixelFormat = eSrcPixelFormat;
	}

	BOOL bAlpha = FALSE;
	if( Gdiplus::IsAlphaPixelFormat( eSrcPixelFormat ) )
	{
		nBPP = 32;
		bAlpha = TRUE;
		eDestPixelFormat = PixelFormat32bppARGB;
	}


	LPBYTE lpBits;
	HBITMAP hBitmap = CXTPImageManager::Create32BPPDIBSection(0, m_szImage.cx, m_szImage.cy, &lpBits);

	int nPitch = ( (((m_szImage.cx * nBPP)+31)/32)*4 );

	if( eDestPixelFormat == eSrcPixelFormat && nBPP == 32)
	{
		// The pixel formats are identical, so just memcpy the rows.
		Gdiplus::BitmapData data;
		Gdiplus::Rect rect( 0, 0, m_szImage.cx, m_szImage.cy );
		if(m_pImage->LockBits( &rect, Gdiplus::ImageLockModeRead, eSrcPixelFormat, &data )!=Gdiplus::Ok)
		{
			return;
		}

		size_t nBytesPerRow = XTPAlignUp( nBPP * m_szImage.cx, 8 )/8;
		BYTE* pbDestRow = static_cast< BYTE* >( lpBits );
		BYTE* pbSrcRow = static_cast< BYTE* >( data.Scan0 );
		for( int y = 0; y < m_szImage.cy; y++ )
		{
			MEMCPY_S(pbDestRow, pbSrcRow, nBytesPerRow);
			pbDestRow += nPitch;
			pbSrcRow += data.Stride;
		}

		m_pImage->UnlockBits( &data );
	}
	else
	{
		// Let GDI+ work its magic
		Gdiplus::Bitmap bmDest( m_szImage.cx, m_szImage.cy, nPitch, eDestPixelFormat, static_cast< BYTE* >( lpBits) );
		Gdiplus::Graphics gDest( &bmDest );

		gDest.DrawImage(m_pImage, 0, 0 );
	}

	CBitmap bmpIcons;
	bmpIcons.Attach(hBitmap);
	UINT nCommand[] = {0};

	m_pParent->GetImageManager()->SetIcons(bmpIcons, nCommand, 1, m_szImage, xtpImageNormal, bAlpha);

	m_pIcon = m_pParent->GetImageManager()->GetImage(nCommand[0], m_szImage.cx);

	if (m_pIcon)
	{
		m_pIcon->InternalAddRef();
	}

}


Gdiplus::Image* CXTPFlowGraphImage::GetImage() const
{
	return m_pImage;
}

CXTPImageManagerIcon* CXTPFlowGraphImage::GetIcon() const
{
	return m_pIcon;
}

//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphImages

CXTPFlowGraphImages::CXTPFlowGraphImages()
{
	m_pImageManager = new CXTPImageManager();


}

CXTPFlowGraphImages::~CXTPFlowGraphImages()
{
	RemoveAll();

	CMDTARGET_RELEASE(m_pImageManager);
}

void CXTPFlowGraphImages::RemoveAll()
{
	if (m_arrImages.GetSize() == 0)
		return;

	for (int i = 0; i < m_arrImages.GetSize(); i++)
	{
		m_arrImages[i]->OnRemoved();
		m_arrImages[i]->InternalRelease();
	}
	m_arrImages.RemoveAll();
}


CXTPFlowGraphImage* CXTPFlowGraphImages::AddImage(CXTPFlowGraphImage* pImage)
{
	m_arrImages.Add(pImage);
	pImage->m_pParent = this;

	return pImage;
}

void CXTPFlowGraphImages::Remove(CXTPFlowGraphImage* pImage)
{
	for (int i = 0; i < m_arrImages.GetSize(); i++)
	{
		if (m_arrImages[i] == pImage)
		{
			RemoveAt(i);
			return;
		}
	}
}

void CXTPFlowGraphImages::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrImages.GetSize())
		return;

	CXTPFlowGraphImage* pImage = m_arrImages[nIndex];

	m_arrImages.RemoveAt(nIndex);

	pImage->OnRemoved();

	pImage->InternalRelease();
}


