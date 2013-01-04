// XTPPopupItem.cpp: implementation of the CXTPPopupItem class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPRichRender.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"

#include "../Defines.h"
#include "XTPPopupItem.h"
#include "XTPPopupControl.h"
#include "XTPPopupPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  IMAGE_PLACEHOLDER 0xDEADBEAF

/////////////////////////////////////////////////////////////////////////////
// CXTPPopupItem

IMPLEMENT_DYNCREATE(CXTPPopupItem, CCmdTarget)


//////////////////////////////////////////////////////////////////////////
// CXTPPopupItem

CXTPPopupItem::CXTPPopupItem(LPRECT rcItem, LPCTSTR lpszCaption, COLORREF clrBackground, COLORREF clrBorder)
: m_nID(0)
, m_nIndex(0)
, m_nIconIndex((int)-1)
, m_bButton(FALSE)
, m_bHyperLink(TRUE)
, m_bBold(FALSE)
, m_bUnderline(TRUE)
, m_nAlign(DT_LEFT)
, m_ptOffset(0,0)
, m_clrText((COLORREF)-1)
, m_clrHotText((COLORREF)-1)
, m_clrBack(clrBackground)
, m_clrBorder(clrBorder)
, m_rcItem(rcItem)
, m_strCaption(lpszCaption)
, m_pControl(NULL)
, m_pRichRender(NULL)
, m_pUIElement(NULL)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPPopupItem::~CXTPPopupItem()
{
	SAFE_DELETE(m_pRichRender);

	XTPMarkupReleaseElement(m_pUIElement);
}

void CXTPPopupItem::SetRTFText(LPCTSTR str)
{
	SAFE_DELETE(m_pRichRender);
	m_pRichRender = new CXTPRichRender;

	m_strCaption = str;
	m_bHyperLink = FALSE;
	m_pRichRender->SetText(str);
}

void CXTPPopupItem::SetRTFText(UINT nIDResource)
{
	HMODULE hModule = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), _T("RTF"));
	if (!hModule)
		return;
	HRSRC hResource = ::FindResource(hModule, MAKEINTRESOURCE(nIDResource), _T("RTF"));
	if (!hResource)
		return;

	HGLOBAL hMem= ::LoadResource(hModule, hResource);
	if (!hMem)
		return;

	LPSTR lpBuffer = (LPSTR)LockResource(hMem);

	CString str(lpBuffer);

	SetRTFText(str);
}

void CXTPPopupItem::SetCaption(LPCTSTR str)
{
	m_strCaption = str;

	XTPMarkupReleaseElement(m_pUIElement);

	if (m_pControl->GetMarkupContext())
	{
		m_pUIElement = XTPMarkupParseText(m_pControl->GetMarkupContext(), str);
	}

	RedrawControl();
}

void CXTPPopupItem::SetMarkupText(LPCTSTR str)
{
	m_pControl->EnableMarkup(TRUE);

	m_bHyperLink = FALSE;

	SetCaption(str);
}

void CXTPPopupItem::RedrawControl()
{
	ASSERT(m_pControl);
	if (m_pControl)
		m_pControl->RedrawControl();
}


BOOL CXTPPopupItem::IsSelected() const
{
	ASSERT(m_pControl);
	return m_pControl && m_pControl->m_pSelected == this;
}

BOOL CXTPPopupItem::IsPressed() const
{
	ASSERT(m_pControl);
	return m_pControl && m_pControl->m_pPressed == this;
}

void CXTPPopupItem::OnItemInserted()
{
}

void CXTPPopupItem::Draw(CDC* pDC)
{
	if (m_pUIElement)
	{
		XTPMarkupSetDefaultFont(m_pControl->GetMarkupContext(), (HFONT)GetTextFont()->GetSafeHandle(), m_clrText == (COLORREF)-1 ? m_pControl->GetPaintManager()->m_clrText : m_clrText);

		XTPMarkupRenderElement(m_pUIElement, pDC->GetSafeHdc(), m_rcItem);
	}
	else
	{
		ASSERT(m_pControl);
		if (m_pControl)
			m_pControl->GetPaintManager()->DrawItem(pDC, this);
	}
}

CFont* CXTPPopupItem::GetTextFont()
{
	ASSERT(m_pControl);
	return (m_pControl && m_fntText.m_hObject == NULL) ?
		&m_pControl->GetPaintManager()->m_fntText : &m_fntText;
}

void CXTPPopupItem::SetTextFont(CFont* pFntText)
{
	ASSERT(pFntText);
	if (!pFntText)
		return;

	LOGFONT lf;
	pFntText->GetLogFont(&lf);

	SetTextFont(&lf);
}

void CXTPPopupItem::SetTextFont(PLOGFONT pLogfText)
{
	ASSERT(pLogfText);

	m_fntText.DeleteObject();
	m_fntText.CreateFontIndirect(pLogfText);

	RedrawControl();
}

void CXTPPopupItem::CalculateHeight()
{
	if (m_pRichRender)
	{
		return;
	}

	if (m_pUIElement)
	{
		XTPMarkupSetDefaultFont(m_pControl->GetMarkupContext(), (HFONT)GetTextFont()->GetSafeHandle(), COLORREF_NULL);

		CSize sz = XTPMarkupMeasureElement(m_pUIElement, m_rcItem.Width());
		m_rcItem.right = m_rcItem.left + sz.cx;
		m_rcItem.bottom = m_rcItem.top + sz.cy;
		return;
	}

	CXTPImageManagerIcon* pIcon = GetImage();

	if (pIcon)
	{
		m_rcItem.bottom = m_rcItem.top + pIcon->GetHeight();
		return;
	}

	CWindowDC dc(NULL);
	CRect rcText(0, 0, m_rcItem.Width(), 0);

	CFont fntUnderline;
	CFont* pFont = GetTextFont();

	if (IsHyperLink() && IsUnderline() || IsBold())
	{
		LOGFONT lpLogFont;
		pFont->GetLogFont(&lpLogFont);
		lpLogFont.lfUnderline = (IsHyperLink() && IsUnderline());
		lpLogFont.lfWeight = IsBold() ? FW_BOLD : FW_NORMAL;

		VERIFY(fntUnderline.CreateFontIndirect(&lpLogFont));
		pFont = &fntUnderline;
	}

	CXTPFontDC font(&dc, pFont);

	dc.DrawText(m_strCaption, rcText, m_nAlign | DT_CALCRECT);

	m_rcItem.bottom = m_rcItem.top + rcText.Height();
}

void CXTPPopupItem::FitToContent()
{
	if (m_pRichRender)
	{
		CWindowDC dc(NULL);
		CSize sz = m_pRichRender->GetTextExtent(&dc, m_rcItem.Width());
		m_rcItem.right = m_rcItem.left + sz.cx;
		m_rcItem.bottom = m_rcItem.top + sz.cy;
		return;
	}

	if (m_pUIElement)
	{
		XTPMarkupSetDefaultFont(m_pControl->GetMarkupContext(), (HFONT)GetTextFont()->GetSafeHandle(), COLORREF_NULL);

		CSize sz = XTPMarkupMeasureElement(m_pUIElement, m_rcItem.Width());
		m_rcItem.right = m_rcItem.left + sz.cx;
		m_rcItem.bottom = m_rcItem.top + sz.cy;
		return;
	}

	CalculateWidth();
	CalculateHeight();
}

void CXTPPopupItem::CalculateWidth()
{
	if (m_pRichRender)
	{
		return;
	}

	if (m_pUIElement)
	{
		XTPMarkupSetDefaultFont(m_pControl->GetMarkupContext(), (HFONT)GetTextFont()->GetSafeHandle(), COLORREF_NULL);

		CSize sz = XTPMarkupMeasureElement(m_pUIElement, m_rcItem.Width());
		m_rcItem.right = m_rcItem.left + sz.cx;
		m_rcItem.bottom = m_rcItem.top + sz.cy;
		return;
	}

	CXTPImageManagerIcon* pIcon = GetImage();

	if (pIcon)
	{
		m_rcItem.right = m_rcItem.left + pIcon->GetWidth();
		return;
	}

	if (m_nAlign & DT_WORDBREAK) return;

	CWindowDC dc(NULL);
	CRect rcText(0, 0, 0, m_rcItem.Height());

	CFont fntUnderline;
	CFont* pFont = GetTextFont();

	if (IsHyperLink() && IsUnderline() || IsBold())
	{
		LOGFONT lpLogFont;
		pFont->GetLogFont(&lpLogFont);
		lpLogFont.lfUnderline = (IsHyperLink() && IsUnderline());
		lpLogFont.lfWeight = IsBold() ? FW_BOLD : FW_NORMAL;

		VERIFY(fntUnderline.CreateFontIndirect(&lpLogFont));
		pFont = &fntUnderline;
	}

	CXTPFontDC font(&dc, pFont);

	dc.DrawText(m_strCaption, rcText, m_nAlign | DT_CALCRECT);

	if (m_nAlign & DT_RIGHT)
	{
		m_rcItem.left = m_rcItem.right - rcText.Width();
	}
	else if (m_nAlign & DT_CENTER)
	{
		LONG lCentrl = m_rcItem.left + m_rcItem.Width()/2;
		m_rcItem.left = lCentrl - rcText.Width()/2;
		m_rcItem.right = m_rcItem.left + rcText.Width();
	}
	else
	{
		m_rcItem.right = m_rcItem.left + rcText.Width();
	}
}




BOOL CXTPPopupItem::SetIcon(HICON hIcon, XTPPopupItemIcon itemIcon)
{
	if (!hIcon)
		return FALSE;

	return m_pControl->GetImageManager()->SetIcon(hIcon, GetIconIndex(), 0,
		itemIcon == xtpPopupItemIconNormal ? xtpImageNormal: itemIcon == xtpPopupItemIconSelected ? xtpImageHot : xtpImageChecked);
}

BOOL CXTPPopupItem::SetIcon(UINT nIDIcon, XTPPopupItemIcon itemIcon)
{

	return m_pControl->GetImageManager()->SetIcon(nIDIcon, GetIconIndex(), 0,
		itemIcon == xtpPopupItemIconNormal ? xtpImageNormal: itemIcon == xtpPopupItemIconSelected ? xtpImageHot : xtpImageChecked);
}

BOOL CXTPPopupItem::SetIcons(UINT nIDBitmap, COLORREF clrTransparent, int itemIcon)
{
	BOOL bAlphaBitmap = FALSE;

	HBITMAP hBitmap = CXTPImageManagerIcon::LoadBitmapFromResource(MAKEINTRESOURCE(nIDBitmap), &bAlphaBitmap);
	if (!hBitmap)
		return FALSE;

	BOOL res =  SetIcons(hBitmap, clrTransparent, itemIcon, bAlphaBitmap);

	DeleteObject(hBitmap);

	return res;
}


BOOL CXTPPopupItem::SetIcons(HBITMAP hBitmap, COLORREF clrTransparent, int itemIcon, BOOL bAlphaBitmap)
{
	BITMAP bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(BITMAP));
	::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

	int nCount = (itemIcon & xtpPopupItemIconNormal ? 1 : 0) +
		(itemIcon & xtpPopupItemIconSelected ? 1 : 0) +
		(itemIcon & xtpPopupItemIconPressed ? 1 : 0);

	if (nCount == 0)
		return FALSE;

	int nIndex = 0;

	CBitmap bmp;
	bmp.Attach(hBitmap);


	COLORREF clr = m_pControl->GetImageManager()->GetMaskColor();
	m_pControl->GetImageManager()->SetMaskColor(clrTransparent);

	if ((itemIcon & xtpPopupItemIconNormal) != 0)
	{
		UINT nIDs[] = {IMAGE_PLACEHOLDER, IMAGE_PLACEHOLDER, IMAGE_PLACEHOLDER};
		nIDs[nIndex++] = GetIconIndex();

		m_pControl->GetImageManager()->SetIcons(bmp, nIDs, nCount, CSize(bmpInfo.bmWidth / nCount, bmpInfo.bmHeight), xtpImageNormal, bAlphaBitmap);
	}

	if ((itemIcon & xtpPopupItemIconSelected) != 0)
	{
		UINT nIDs[] = {IMAGE_PLACEHOLDER, IMAGE_PLACEHOLDER, IMAGE_PLACEHOLDER};
		nIDs[nIndex++] = GetIconIndex();

		m_pControl->GetImageManager()->SetIcons(bmp, nIDs, nCount, CSize(bmpInfo.bmWidth / nCount, bmpInfo.bmHeight), xtpImageHot, bAlphaBitmap);
	}

	if ((itemIcon & xtpPopupItemIconPressed) != 0)
	{
		UINT nIDs[] = {IMAGE_PLACEHOLDER, IMAGE_PLACEHOLDER, IMAGE_PLACEHOLDER};
		nIDs[nIndex++] = GetIconIndex();

		m_pControl->GetImageManager()->SetIcons(bmp, nIDs, nCount, CSize(bmpInfo.bmWidth / nCount, bmpInfo.bmHeight), xtpImageChecked, bAlphaBitmap);
	}


	m_pControl->GetImageManager()->SetMaskColor(clr);

	bmp.Detach();


	return TRUE;
}

CXTPImageManagerIcon* CXTPPopupItem::GetImage() const
{
	return m_pControl->GetImageManager()->GetImage(GetIconIndex(), m_rcItem.Width());
}

void CXTPPopupItem::CenterIcon()
{
	m_ptOffset = CPoint(0, 0);

	CXTPImageManagerIcon* pIcon = GetImage();

	if (!pIcon)
		return;

	CSize sz = pIcon->GetExtent();
	CRect rc = GetRect();

	if (rc.Width() > sz.cx) m_ptOffset.x = rc.Width()/2 - sz.cx/2;
	if (rc.Height() > sz.cy) m_ptOffset.y = rc.Height()/2 - sz.cy/2;

}



#ifdef _XTP_ACTIVEX

// {569C30D0-E2D7-4ae7-ACCF-EC2209058C15}
static const GUID IID_IPopupItem =
{ 0x569c30d0, 0xe2d7, 0x4ae7, { 0xac, 0xcf, 0xec, 0x22, 0x9, 0x5, 0x8c, 0x15 } };


BEGIN_INTERFACE_MAP(CXTPPopupItem, CCmdTarget)
	INTERFACE_PART(CXTPPopupItem, IID_IPopupItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPopupItem, IID_IPopupItem)

BEGIN_DISPATCH_MAP(CXTPPopupItem, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPPopupItem, "Caption", 1, OleGetCaption, SetCaption , VT_BSTR)
	DISP_PROPERTY_ID(CXTPPopupItem, "TextAlignment", 2, m_nAlign, VT_I4)
	DISP_PROPERTY_ID(CXTPPopupItem, "HyperLink", 3, m_bHyperLink, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPopupItem, "Button", 4, m_bButton, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPopupItem, "Id", 5, m_nID, VT_I4)
	DISP_PROPERTY_ID(CXTPPopupItem, "TextColor", 6, m_clrText, VT_COLOR)
	DISP_PROPERTY_ID(CXTPPopupItem, "BackgroundColor", 7, m_clrBack, VT_COLOR)
	DISP_PROPERTY_ID(CXTPPopupItem, "BorderColor", 8, m_clrBorder, VT_COLOR)
	DISP_FUNCTION_ID(CXTPPopupItem, "SetIcon", 9, OleSetIcon, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPPopupItem, "SetIcons", 10, OleSetIcons, VT_EMPTY, VTS_I4 VTS_COLOR VTS_I4)
	DISP_FUNCTION_ID(CXTPPopupItem, "CenterIcon", 11, CenterIcon, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPopupItem, "CalculateHeight", 12, CalculateHeight, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPopupItem, "CalculateWidth", 13, CalculateWidth, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_ID(CXTPPopupItem, "Bold", 14, m_bBold, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPopupItem, "IconIndex", 15, m_nIconIndex, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPopupItem, "Font", DISPID_FONT, OleGetFont, OleSetFont, VT_FONT)

	DISP_PROPERTY_EX_ID(CXTPPopupItem, "Left", 16, OleGetLeft, OleSetLeft, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPopupItem, "Top", 17, OleGetTop, OleSetTop, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPopupItem, "Width", 18, OleGetWidth, OleSetWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPopupItem, "Height", 19, OleGetHeight, OleSetHeight, VT_I4)

END_DISPATCH_MAP()


void CXTPPopupItem::OleSetIcon(int hIcon, int itemIcon)
{
	SetIcon((HICON)hIcon, (XTPPopupItemIcon)itemIcon);
}

void CXTPPopupItem::OleSetIcons(int hBitmap, COLORREF clrTransparent, int itemIcon)
{
	SetIcons((HBITMAP)hBitmap, clrTransparent, itemIcon);
}

BSTR CXTPPopupItem::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}

LPFONTDISP CXTPPopupItem::OleGetFont()
{
	return AxCreateOleFont(GetTextFont(), this, (LPFNFONTCHANGED)&CXTPPopupItem::OleSetFont);
}

void CXTPPopupItem::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	AxCreateFontFromDispatch(&m_fntText, pFontDisp);
}

long CXTPPopupItem::OleGetLeft()
{
	return m_rcItem.left;
}

void CXTPPopupItem::OleSetLeft(long nLeft)
{
	m_rcItem.SetRect(nLeft, m_rcItem.top, nLeft + OleGetWidth(), m_rcItem.bottom);
}

long CXTPPopupItem::OleGetTop()
{
	return m_rcItem.top;
}

void CXTPPopupItem::OleSetTop(long nTop)
{
	m_rcItem.SetRect(m_rcItem.left, nTop, m_rcItem.right, nTop + OleGetHeight());
}

long CXTPPopupItem::OleGetWidth()
{
	return m_rcItem.Width();
}

void CXTPPopupItem::OleSetWidth(long nWidth)
{
	m_rcItem.SetRect(m_rcItem.left, m_rcItem.top, m_rcItem.left + nWidth, m_rcItem.bottom);
}

long CXTPPopupItem::OleGetHeight()
{
	return m_rcItem.Height();
}

void CXTPPopupItem::OleSetHeight(long nHeight)
{
	m_rcItem.SetRect(m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + nHeight);
}



#endif
