// XTPMarkupImage.cpp: implementation of the CXTPMarkupImage class.
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

#include "Common/XTPImageManager.h"

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupShape.h"
#include "XTPMarkupImage.h"
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupContext.h"
#include "XTPMarkupBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CXTPMarkupDependencyProperty* CXTPMarkupImage::m_pSourceProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupImage::m_pStretchProperty = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_MARKUPCLASS(L"Image", CXTPMarkupImage, CXTPMarkupFrameworkElement)

void CXTPMarkupImage::RegisterMarkupClass()
{
	m_pSourceProperty = CXTPMarkupDependencyProperty::Register(L"Source", MARKUP_TYPE(CXTPMarkupString), MARKUP_TYPE(CXTPMarkupImage),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pStretchProperty = CXTPMarkupDependencyProperty::Register(L"Stretch", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupImage),
		new CXTPMarkupPropertyMetadata(CXTPMarkupEnum::CreateValue(xtpMarkupStretchNone), &CXTPMarkupBuilder::ConvertStretch, CXTPMarkupPropertyMetadata::flagAffectsMeasure));
}

CXTPMarkupImage::CXTPMarkupImage()
{
	m_pImageManager = NULL;
	m_pImage = NULL;

}

CXTPMarkupImage::~CXTPMarkupImage()
{
	CMDTARGET_RELEASE(m_pImage);
	CMDTARGET_RELEASE(m_pImageManager);
}

CString CXTPMarkupImage::GetSource()
{
	CXTPMarkupString* pSource = MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pSourceProperty));
	if (!pSource)
		return _T("");

	return CString(*pSource);
}

void CXTPMarkupImage::SetSource(LPCTSTR lpszSource)
{
	SetValue(m_pSourceProperty, new CXTPMarkupString(lpszSource));
}


void CXTPMarkupImage::LoadImage(int cx)
{
	CXTPMarkupString* pSource = MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pSourceProperty));
	if (!pSource)
		return;

	CMDTARGET_RELEASE(m_pImageManager);
	CMDTARGET_RELEASE(m_pImage);

	LPCWSTR lpszSource = (*pSource);
	if (wcsncmp(lpszSource, L"file://", 7) == 0)
	{
		CString strFileName(lpszSource + 7);

		m_pImageManager = new CXTPImageManager();
		m_pImageManager->SetMaskColor(0x00FF00);

		if (strFileName.Right(3).CompareNoCase(_T("ico")) == 0)
		{
			m_pImageManager->SetIconFromIcoFile(strFileName, 0, 0,xtpImageNormal);
		}
		else
		{
			BOOL bAlphaBitmap = FALSE;
			HBITMAP hBitmap = CXTPImageManagerIcon::LoadBitmapFromFile(strFileName, &bAlphaBitmap);

			if (hBitmap)
			{
				CBitmap bmp;
				bmp.Attach(hBitmap);
				m_pImageManager->SetIcons(bmp, 0, 1, 0, xtpImageNormal, bAlphaBitmap);
			}
		}
		m_pImage = m_pImageManager->GetImage(0, cx);
		CMDTARGET_ADDREF(m_pImage);
	}
	else if (wcsncmp(lpszSource, L"res://", 6) == 0)
	{
		CString strResourceName(lpszSource + 6);

		LPCTSTR lpszResource = (LPCTSTR)strResourceName;
		if (lpszResource[0] == _T('#'))
		{
			lpszResource = MAKEINTRESOURCE(_ttoi(lpszResource + 1));
		}

		m_pImageManager = new CXTPImageManager();
		m_pImageManager->SetMaskColor(0x00FF00);

		BOOL bAlphaBitmap = FALSE;
		HBITMAP hBitmap = CXTPImageManagerIcon::LoadBitmapFromResource(AfxGetResourceHandle(), lpszResource, &bAlphaBitmap);

		if (!hBitmap)
		{
			hBitmap = CXTPImageManagerIcon::LoadBitmapFromResource(lpszResource, &bAlphaBitmap);
		}

		if (hBitmap)
		{
			CBitmap bmp;
			bmp.Attach(hBitmap),
			m_pImageManager->SetIcons(bmp, 0, 1, 0, xtpImageNormal, bAlphaBitmap);
		}
		else
		{
			m_pImageManager->SetIcon(strResourceName, 0);
		}

		m_pImage = m_pImageManager->GetImage(0, cx);
		CMDTARGET_ADDREF(m_pImage);
	}
	else
	{
		int nIndex = _wtoi(lpszSource);
		//if (nIndex > 0)
		{
			m_pImageManager = m_pMarkupContext->GetImageManager();
			CMDTARGET_ADDREF(m_pImageManager);

			m_pImage = m_pImageManager->GetImage(nIndex, cx);
			CMDTARGET_ADDREF(m_pImage);
		}
	}
}

AFX_INLINE BOOL IsPositiveInfinity(int size) {
	return size > 32000;
}

CSize CXTPMarkupImage::GetOriginalSize() const
{
	CSize szRet(0,0);

	if (m_pImage)
		szRet = CSize(m_pImage->GetWidth(), m_pImage->GetHeight());

	return szRet;
}

CSize CXTPMarkupImage::MeasureArrangeHelper(CSize availableSize)
{
	if (!m_pImage)
		return CSize(0, 0);

	CSize szContent(m_pImage->GetWidth(), m_pImage->GetHeight());
	if (szContent.cx == 0 || szContent.cy == 0)
		return szContent;

	XTPMarkupStretch stretch = GetStretch();

	BOOL bWidth = !IsPositiveInfinity(availableSize.cx);
	BOOL bHeight = !IsPositiveInfinity(availableSize.cy);

	if ((stretch == xtpMarkupStretchNone) || (!bWidth && !bHeight))
	{
		return szContent;
	}

	double cx = (double)availableSize.cx / szContent.cx;
	double cy = (double)availableSize.cy / szContent.cy;

	if (!bWidth)
	{
		cx = cy;
	}
	else if (!bHeight)
	{
		cy = cx;
	}
	else
	{
		switch (stretch)
		{
		case xtpMarkupStretchUniform:
			cx = cy = min(cx, cy);
			break;

		case xtpMarkupStretchUniformToFill:
			cx = cy = max(cx, cy);
			break;
		}
	}

	return CSize(int(szContent.cx * cx), int(szContent.cy * cy));
}


CSize CXTPMarkupImage::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize constraint)
{
	LoadImage(constraint.cx);

	return MeasureArrangeHelper(constraint);
}

CSize CXTPMarkupImage::ArrangeOverride(CSize arrangeSize)
{
	return MeasureArrangeHelper(arrangeSize);
}

void CXTPMarkupImage::OnRender(CXTPMarkupDrawingContext* pDC)
{
	if (!m_pImage)
		return;

	CSize szRender = GetRenderSize();

	pDC->DrawImage(m_pImage, CRect(0, 0, szRender.cx, szRender.cy));

	// old code, not good because it didn't respect the applied transformation in GDI+
#if 0
	HDC hDC = pDC->GetDC();
	m_pImage->Draw(CDC::FromHandle(hDC), CPoint(0, 0), szRender);
	pDC->ReleaseDC(hDC);
#endif
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMarkupImage, CXTPMarkupFrameworkElement)
	DISP_PROPERTY_EX_ID(CXTPMarkupImage, "Source", 500, OleGetSource, OleSetSource, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPMarkupImage, "Stretch", 501, OleGetStretch, OleSetStretch, VT_I4)
END_DISPATCH_MAP()

// {10059842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupImage =
{ 0x10059842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupImage, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupImage, IID_IMarkupImage, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupImage, IID_IMarkupImage)

void CXTPMarkupImage::OleSetSource(LPCTSTR lpszSource)
{
	SetSource(lpszSource);
}

BSTR CXTPMarkupImage::OleGetSource()
{
	return GetSource().AllocSysString();
}

long CXTPMarkupImage::OleGetStretch()
{
	return GetStretch();
}

void CXTPMarkupImage::OleSetStretch(long stretch)
{
	SetStretch((XTPMarkupStretch)stretch);
}


#endif
