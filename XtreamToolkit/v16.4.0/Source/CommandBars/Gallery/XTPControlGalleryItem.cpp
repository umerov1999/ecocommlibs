// XTPControlGallery.cpp : implementation file.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#include <math.h>

#include <Common/XTPImageManager.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPMarkupRender.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBarsOptions.h>

#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>

#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPPaintManager.h>

#include "XTPControlGalleryItem.h"
#include "XTPControlGalleryItems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryItem

IMPLEMENT_DYNAMIC(CXTPControlGalleryItem, CXTPCmdTarget);

CXTPControlGalleryItem::CXTPControlGalleryItem()
{
	m_pItems = NULL;

	m_nId = 0;
	m_nIndex = -1;
	m_nImage = -1;
	m_dwData = 0;
	m_bLabel = FALSE;
	m_bEnabled = TRUE;

	m_szItem = CSize(0, 0);

	m_nImageWidth = ICON_SMALL;

	m_pMarkupUIElement = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPControlGalleryItem::~CXTPControlGalleryItem()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPControlGalleryItem::SetEnabled(BOOL bEnabled)
{
	if (m_bEnabled == bEnabled)
		return;

	m_bEnabled = bEnabled;

	if (m_pItems && m_pItems->GetAction())
		m_pItems->GetAction()->RedrawControls();
}

void CXTPControlGalleryItem::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;

	XTPMarkupReleaseElement(m_pMarkupUIElement);

	if (m_pItems && m_pItems->GetMarkupContext())
	{
		m_pMarkupUIElement = XTPMarkupParseText(m_pItems->GetMarkupContext(), lpszCaption);
	}
}

BOOL CXTPControlGalleryItem::GetEnabled() const
{
	return m_bEnabled;
}


int CXTPControlGalleryItem::GetIndex() const
{
	return m_nIndex;
}

int CXTPControlGalleryItem::GetDefaultHeight()
{
	return (CXTPControlGallery::m_bAutoScaleDpi ? XTPDpiHelper()->ScaleY(19) : 19);
}

CSize CXTPControlGalleryItem::GetSize()
{
	if (m_bLabel)
	{
		return CSize(0, m_szItem.cy == 0 ? CXTPControlGalleryItem::GetDefaultHeight() : m_szItem.cy);
	}

	return CSize(m_szItem.cx == 0 ? m_pItems->m_szItem.cx : m_szItem.cx,
		m_szItem.cy == 0 ? m_pItems->m_szItem.cy : m_szItem.cy);
}

void CXTPControlGalleryItem::SetSize(CSize szItem)
{
	if (CXTPControlGallery::m_bAutoScaleDpi)
	{
		szItem = CSize(XTPDpiHelper()->ScaleX(szItem.cx), XTPDpiHelper()->ScaleY(szItem.cy));
	}

	m_szItem = szItem;
}

void CXTPControlGalleryItem::Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	if (IsLabel())
	{
		pGallery->GetGalleryPaintManager()->
			DrawLabel(pDC, this, rcItem);
		return;
	}

	CXTPPaintManager* pPaintManager = pGallery->GetPaintManager();

	if (m_pMarkupUIElement)
	{
		BOOL bListStyle = this->GetSize().cx == 0;

		COLORREF clrTextColor;

		if (bListStyle)
		{
			pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypePopup, xtpBarPopup);
			clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, FALSE, bEnabled, bChecked, FALSE, xtpBarTypePopup, xtpBarPopup);
		}
		else
		{
			pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);
			clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);
		}

		XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement),
			(HFONT)pPaintManager->GetRegularFont()->GetSafeHandle(), clrTextColor);

		CRect rcDest(rcItem);
		rcDest.DeflateRect(3, 3);

		XTPMarkupRenderElement(m_pMarkupUIElement, pDC->GetSafeHdc(), rcDest);

		return;
	}

	CXTPImageManagerIcon* pImage = GetImage();

	BOOL bDrawImgeAndText = (pImage != NULL && !m_strCaption.IsEmpty()) &&
		(pGallery->GetStyle() == xtpButtonIconAndCaption || pGallery->GetStyle() == xtpButtonCaptionAndDescription || pGallery->GetStyle() == xtpButtonIconAndCaptionBelow);

	if (bDrawImgeAndText && pGallery->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);
		CSize szImage(pImage->GetWidth(), pImage->GetHeight());
		CPoint pt((rcItem.left + rcItem.right - szImage.cx) / 2, rcItem.top + 2);

		pImage->Draw(pDC, pt, pImage->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled), szImage);

		COLORREF clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, FALSE, bEnabled, FALSE, FALSE, xtpBarTypePopup, xtpBarPopup);

		pDC->SetTextColor(clrTextColor);

		CRect rcText(rcItem);
		rcText.top += 2 + szImage.cy + 2;

		CXTPFontDC font(pDC, pPaintManager->GetRegularFont());
		pDC->DrawText(m_strCaption, rcText, DT_WORDBREAK | DT_CENTER | DT_NOPREFIX);

	}
	else if (bDrawImgeAndText)
	{
		pPaintManager->DrawRectangle(pDC, rcItem, bSelected, FALSE, bEnabled, FALSE, FALSE, xtpBarTypePopup, xtpBarPopup);

		CSize szImage(pImage->GetWidth(), pImage->GetHeight());
		int nImageBoxWidth  = max(rcItem.Height() - 4, szImage.cx);

		if (bChecked)
		{
			CRect rcCheckBox(rcItem.left + 2, rcItem.top + 2, rcItem.left + 2 + nImageBoxWidth, rcItem.bottom - 2);
			pPaintManager->DrawRectangle(pDC, rcCheckBox, FALSE, FALSE, bEnabled, TRUE, FALSE, xtpBarTypePopup, xtpBarPopup);
		}

		CPoint pt(rcItem.left + 2 + (nImageBoxWidth - szImage.cx) / 2, (rcItem.top + rcItem.bottom - szImage.cy) / 2);

		pImage->Draw(pDC, pt, pImage->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled), szImage);

		COLORREF clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, FALSE, bEnabled, FALSE, FALSE, xtpBarTypePopup, xtpBarPopup);

		pDC->SetTextColor(clrTextColor);

		CRect rcText(rcItem);
		rcText.left += 2 + nImageBoxWidth + szImage.cx / 3;

		if (pGallery->GetStyle() == xtpButtonCaptionAndDescription)
		{
			CXTPFontDC font(pDC, pPaintManager->GetRegularBoldFont());
			pDC->DrawText(m_strCaption, rcText, DT_END_ELLIPSIS | DT_SINGLELINE | DT_TOP | DT_NOPREFIX);

			rcText.top += pDC->GetTextExtent(_T("XXX"), 3).cy;
			font.SetFont(pPaintManager->GetRegularFont());
			pDC->DrawText(m_strDescriptionText, rcText, DT_WORDBREAK | DT_TOP | DT_NOPREFIX);
		}
		else
		{
			CXTPFontDC font(pDC, pPaintManager->GetRegularFont());
			pDC->DrawText(m_strCaption, rcText, DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
		}
	}
	else if (pImage)
	{
		pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);
		CSize szImage(pImage->GetWidth(), pImage->GetHeight());
		CPoint pt((rcItem.left + rcItem.right - szImage.cx) / 2, (rcItem.top + rcItem.bottom - szImage.cy) / 2);

		pImage->Draw(pDC, pt, pImage->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled), szImage);
	}
	else
	{
		pPaintManager->DrawRectangle(pDC, rcItem, bSelected, FALSE, bEnabled, bChecked, FALSE, xtpBarTypePopup, xtpBarPopup);
		COLORREF clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, FALSE, bEnabled, bChecked, FALSE, xtpBarTypePopup, xtpBarPopup);

		pDC->SetTextColor(clrTextColor);
		CXTPFontDC font(pDC, pPaintManager->GetRegularFont());

		CRect rcText(rcItem);
		rcText.left += 3;

		pDC->DrawText(m_strCaption, rcText, DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}
}

CXTPImageManagerIcon* CXTPControlGalleryItem::GetImage()
{
	if (!m_pItems)
		return NULL;

	return m_pItems->GetImageManager()->GetImage(m_nImage >= 0 ? m_nImage : m_nId, m_nImageWidth);
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlGalleryItem, CCmdTarget)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "Id", 1, m_nId, VT_I4)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "ImageIndex", 2, m_nImage, VT_I4)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "Caption", 3, m_strCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "ToolTip", 4, m_strToolTip, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "Index", 5, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "Tag", 6, m_oleTag, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "Label", 7, OleGetLabel, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "ItemWidth", 8, OleGetItemWidth, OleSetItemWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "ItemHeight", 9, OleGetItemHeight, OleSetItemHeight, VT_I4)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "DescriptionText", 10, m_strDescriptionText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "Enabled", 11, GetEnabled, SetEnabled, VT_BOOL)
END_DISPATCH_MAP()

// {ADB28A35-789A-40d9-B564-7577DA549319}
static const GUID IID_ICommandBarGalleryItem =
{ 0xadb28a35, 0x789a, 0x40d9, { 0xb5, 0x64, 0x75, 0x77, 0xda, 0x54, 0x93, 0x19 } };

BEGIN_INTERFACE_MAP(CXTPControlGalleryItem, CCmdTarget)
	INTERFACE_PART(CXTPControlGalleryItem, IID_ICommandBarGalleryItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlGalleryItem, IID_ICommandBarGalleryItem)

BOOL CXTPControlGalleryItem::OleGetLabel()
{
	return IsLabel();
}

int CXTPControlGalleryItem::OleGetItemWidth()
{
	return m_szItem.cx == 0 ? m_pItems->m_szItem.cx : m_szItem.cx;
}

void CXTPControlGalleryItem::OleSetItemWidth(int cx)
{
	m_szItem.cx = cx;
}

int CXTPControlGalleryItem::OleGetItemHeight()
{
	return m_szItem.cy == 0 ? m_pItems->m_szItem.cy : m_szItem.cy;
}

void CXTPControlGalleryItem::OleSetItemHeight(int cy)
{
	m_szItem.cy = cy;
}

#endif
