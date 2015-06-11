// XTPFlowGraphImage.cpp : implementation of the CXTPFlowGraphImage class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#include "GraphicLibrary/GdiPlus/GdiPlus.h"
#include "GraphicLibrary/XTGraphicUtils.h"

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "Common/XTPResourceManager.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPImageManager.h"

#include "XTPFlowGraphImage.h"

#include "XTPFlowGraphControl.h"

#include "Markup/XTPMarkupContext.h"

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
	: m_pImage    (NULL)
	, m_pIcon     (NULL)
	, m_nCommandId((UINT)-1),
	m_pParent(NULL)
{
	m_szImage = CSize(0, 0);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
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

	if (CXTPGraphicUtils::GdipCreateBitmapFromHICON_Fixed(hIcon, &m_pImage) != Gdiplus::Ok) // Now 'image' has the icon, with transparency
	{
		ASSERT(FALSE);
	}

	ImageChanged();
}

void CXTPFlowGraphImage::ImageChanged()
{
	CMDTARGET_RELEASE(m_pIcon);
	m_szImage = CSize(0, 0);

	if (NULL == m_pImage || Gdiplus::Ok != m_pImage->GetLastStatus())
	{
		SAFE_DELETE(m_pImage);
		return;
	}

	m_szImage = CSize(m_pImage->GetWidth(), m_pImage->GetHeight());

	Gdiplus::PixelFormat eSrcPixelFormat = m_pImage->GetPixelFormat();
	BOOL bAlpha = FALSE;
	if( Gdiplus::IsAlphaPixelFormat(eSrcPixelFormat))
	{
		bAlpha = TRUE;
	}

	if (m_pParent)
	{
		HBITMAP hBitmap;
		m_pImage->GetHBITMAP(RGB(255,255,255), &hBitmap);

		CBitmap bitmap;
		bitmap.Attach(hBitmap);

		m_pParent->GetImageManager()->SetIcons(bitmap, &m_nCommandId, 1, m_szImage, xtpImageNormal, bAlpha);

		m_pIcon = m_pParent->GetImageManager()->GetImage(m_nCommandId, m_szImage.cx);

		m_pIcon->InternalAddRef();
	}
	else
	{
		HBITMAP hBitmap;
		m_pImage->GetHBITMAP(RGB(255,255,255), &hBitmap);

		CXTPImageManagerIconHandle ih(hBitmap);

		CXTPImageManagerIcon *pIcon = new CXTPImageManagerIcon(1, m_szImage.cx, m_szImage.cy);
		pIcon->SetIcon(ih);

		m_pIcon = pIcon;

		// we don't have to do InternalAddRef();
		// because image manager doesn't present
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

CXTPFlowGraphImages::CXTPFlowGraphImages(CXTPFlowGraphControl *pControl)
{
	m_pControl = pControl;

	ASSERT(pControl->GetMarkupContext());

	if (pControl->GetMarkupContext())
	{
		// each control must have its own image manager (not library-shared)
		pControl->GetMarkupContext()->SetImageManager(new CXTPImageManager());

		m_pImageManager = pControl->GetMarkupContext()->GetImageManager();
	}

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

CXTPFlowGraphImages::~CXTPFlowGraphImages()
{
	RemoveAll();
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

	if (m_pImageManager)
		m_pImageManager->RemoveAll();
}


CXTPFlowGraphImage* CXTPFlowGraphImages::AddImage(CXTPFlowGraphImage* pImage)
{
	m_arrImages.Add(pImage);
	pImage->m_pParent = this;
	pImage->m_nCommandId = (UINT)(m_arrImages.GetSize() - 1);

	return pImage;
}

void CXTPFlowGraphImages::Remove(CXTPFlowGraphImage* pImage)
{
	for (int i = 0; i < m_arrImages.GetSize(); i++)
	{
		if (m_arrImages[i] == pImage)
		{
			if (m_pImageManager)
				m_pImageManager->RemoveIcon(pImage->m_nCommandId);

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

	if (m_pImageManager)
		m_pImageManager->RemoveIcon(pImage->m_nCommandId);
}


#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CXTPFlowGraphImage, CCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphImages, "LoadFromFile", 1, LoadFromFile, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// {66353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphImage =
{ 0x66353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphImage, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphImage, IID_IFlowGraphImage, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphImage, IID_IFlowGraphImage)



//////////////////////////////////////////////////////////////////////////

BEGIN_DISPATCH_MAP(CXTPFlowGraphImages, CCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphImages, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphImages, "Image", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPFlowGraphImages, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphImages, "AddImage", 2, OleAddImage, VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPFlowGraphImages, "RemoveAll", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphImages, "RemoveAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// {65353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphImages =
{ 0x65353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphImages, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphImages, IID_IFlowGraphImages, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphImages, IID_IFlowGraphImages)
IMPLEMENT_ENUM_VARIANT(CXTPFlowGraphImages)


LPDISPATCH CXTPFlowGraphImages::OleGetItem(int nIndex)
{
	return XTPGetDispatch(GetAt(nIndex));
}

int CXTPFlowGraphImages::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPFlowGraphImages::OleAddImage()
{
	return XTPGetDispatch(AddImage(new CXTPFlowGraphImage()));
}


#endif
