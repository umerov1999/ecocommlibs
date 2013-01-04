// XTPMarkupFrameworkElement.cpp: implementation of the CXTPMarkupFrameworkElement class.
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

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupBuilder.h"
#include "XTPMarkupResourceDictionary.h"
#include "XTPMarkupThickness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupFrameworkElement

CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pMarginProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pHorizontalAlignmentProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pVerticalAlignmentProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pWidthProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pHeightProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pMinWidthProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pMinHeightProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pMaxWidthProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pMaxHeightProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupFrameworkElement::m_pTagProperty = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_MARKUPCLASS(L"FrameworkElement", CXTPMarkupFrameworkElement, CXTPMarkupUIElement)

void CXTPMarkupFrameworkElement::RegisterMarkupClass()
{
	CXTPMarkupStyle::RegisterType();

	m_pMarginProperty = CXTPMarkupDependencyProperty::Register(L"Margin", MARKUP_TYPE(CXTPMarkupThickness), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(CXTPMarkupThickness::CreateValue(), CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pHorizontalAlignmentProperty = CXTPMarkupDependencyProperty::Register(L"HorizontalAlignment", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertHorizontalAlignment, CXTPMarkupPropertyMetadata::flagAffectsArrange));

	m_pVerticalAlignmentProperty = CXTPMarkupDependencyProperty::Register(L"VerticalAlignment", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertVerticalAlignment, CXTPMarkupPropertyMetadata::flagAffectsArrange));

	m_pWidthProperty = CXTPMarkupDependencyProperty::Register(L"Width", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertLength, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pHeightProperty = CXTPMarkupDependencyProperty::Register(L"Height", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertLength, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pMinWidthProperty = CXTPMarkupDependencyProperty::Register(L"MinWidth", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertLength, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pMinHeightProperty = CXTPMarkupDependencyProperty::Register(L"MinHeight", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertLength, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pMaxWidthProperty = CXTPMarkupDependencyProperty::Register(L"MaxWidth", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertLength, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pMaxHeightProperty = CXTPMarkupDependencyProperty::Register(L"MaxHeight", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupFrameworkElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertLength, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pTagProperty = CXTPMarkupDependencyProperty::Register(L"Tag", MARKUP_TYPE(CXTPMarkupObject), MARKUP_TYPE(CXTPMarkupFrameworkElement));
}

CXTPMarkupFrameworkElement::CXTPMarkupFrameworkElement()
{
	m_bNeedsClipBounds = FALSE;

	m_bUnclippedDesiredSize = FALSE;
	m_szUnclippedDesiredSize = CSize(0, 0);
}

CXTPMarkupFrameworkElement::~CXTPMarkupFrameworkElement()
{
}

CXTPMarkupFrameworkElement::MINMAX::MINMAX()
{
	nMaxHeight = nMaxWidth = INT_MAX;
	nMinWidth = nMinHeight = 0;
}

AFX_INLINE int InlineMin(int a, int b) {
	return a < b ? (a) : (b);
}

AFX_INLINE int InlineMax(int a, int b) {
	return a > b ? (a) : (b);
}

void CXTPMarkupFrameworkElement::MINMAX::Update(const CXTPMarkupFrameworkElement* pElement)
{
	CXTPMarkupInt* pHeight = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(CXTPMarkupFrameworkElement::m_pHeightProperty));
	CXTPMarkupInt* pMinHeight = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(CXTPMarkupFrameworkElement::m_pMinHeightProperty));
	CXTPMarkupInt* pMaxHeight = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(CXTPMarkupFrameworkElement::m_pMaxHeightProperty));
	CXTPMarkupInt* pWidth = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(CXTPMarkupFrameworkElement::m_pWidthProperty));
	CXTPMarkupInt* pMinWidth = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(CXTPMarkupFrameworkElement::m_pMinWidthProperty));
	CXTPMarkupInt* pMaxWidth = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(CXTPMarkupFrameworkElement::m_pMaxWidthProperty));

	if (pHeight && (int)*pHeight== INT_MAX) pHeight = NULL; // for Auto case
	if (pWidth && (int)*pWidth== INT_MAX) pWidth = NULL;


	nMaxHeight = InlineMax(InlineMin(pHeight ? (int)*pHeight : INT_MAX, pMaxHeight ? (int)*pMaxHeight : INT_MAX), pMinHeight ? (int)*pMinHeight : 0);
	nMinHeight = InlineMax(InlineMin(nMaxHeight, pHeight ? (int)*pHeight : 0), pMinHeight ? (int)*pMinHeight : 0);

	nMaxWidth = InlineMax(InlineMin(pWidth ? (int)*pWidth : INT_MAX, pMaxWidth ? (int)*pMaxWidth : INT_MAX), pMinWidth ? (int)*pMinWidth : 0);
	nMinWidth = InlineMax(InlineMin(nMaxWidth, pWidth ? (int)*pWidth : 0), pMinWidth ? (int)*pMinWidth : 0);
}



inline int SafeSum(int a, int b)
{
	if (a > INT_MAX / 2 || b > INT_MAX / 2)
		return INT_MAX;
	return a + b;
}

CSize CXTPMarkupFrameworkElement::MeasureCore(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	CXTPMarkupThickness* pMargin = GetMargin();

	long num = pMargin->GetLeft() + pMargin->GetRight();
	long num2 = pMargin->GetTop() + pMargin->GetBottom();

	CSize transformSpaceBounds = CSize(max(SafeSum(szAvailableSize.cx, -num), 0), max(SafeSum(szAvailableSize.cy, -num2), 0));

	m_mmBounds.Update(this);

	transformSpaceBounds.cx = max(m_mmBounds.nMinWidth, min(transformSpaceBounds.cx, m_mmBounds.nMaxWidth));
	transformSpaceBounds.cy = max(m_mmBounds.nMinHeight, min(transformSpaceBounds.cy, m_mmBounds.nMaxHeight));

	CSize size2 = MeasureOverride(pDC, transformSpaceBounds);
	size2 = CSize(max(size2.cx, m_mmBounds.nMinWidth), max(size2.cy, m_mmBounds.nMinHeight));
	CSize size = size2;

	BOOL fClip = FALSE;

	if (size2.cx > m_mmBounds.nMaxWidth)
	{
		size2.cx = m_mmBounds.nMaxWidth;
		fClip = TRUE;
	}

	if (size2.cy > m_mmBounds.nMaxHeight)
	{
		size2.cy = m_mmBounds.nMaxHeight;
		fClip = TRUE;
	}


	long width = size2.cx + num;
	long height = size2.cy + num2;

	if (width > szAvailableSize.cx)
	{
		width = szAvailableSize.cx;
		fClip = TRUE;
	}

	if (height > szAvailableSize.cy)
	{
		height = szAvailableSize.cy;
		fClip = TRUE;
	}

	m_bNeedsClipBounds = FALSE;
	m_bUnclippedDesiredSize = FALSE;

	if ((fClip || (width < 0)) || (height < 0))
	{
		m_bNeedsClipBounds = TRUE;

		m_bUnclippedDesiredSize = TRUE;
		m_szUnclippedDesiredSize = size;
	}

	return CSize(max(0, width), max(0, height));
}

BOOL CXTPMarkupFrameworkElement::GetClipToBounds() const
{
	CXTPMarkupBool* pClipToBounds = MARKUP_STATICCAST(CXTPMarkupBool, GetValue(m_pClipToBoundsProperty));
	return pClipToBounds && (BOOL)*pClipToBounds;
}

BOOL CXTPMarkupFrameworkElement::GetLayoutClip(CRect& rc) const
{
	if (CXTPMarkupUIElement::GetLayoutClip(rc))
		return TRUE;

	BOOL bClipToBounds = GetClipToBounds();

	if (!bClipToBounds && !m_bNeedsClipBounds)
		return FALSE;

	CSize szRenderSize = m_szRenderSize;

	int num = m_mmBounds.nMaxWidth == INT_MAX ? szRenderSize.cx : m_mmBounds.nMaxWidth;
	int num2 = m_mmBounds.nMaxHeight == INT_MAX ? szRenderSize.cy : m_mmBounds.nMaxHeight;
	BOOL bFlag = bClipToBounds ||num < szRenderSize.cx || num2 < szRenderSize.cy;
	szRenderSize = CSize(min(szRenderSize.cx, m_mmBounds.nMaxWidth), min(szRenderSize.cy, m_mmBounds.nMaxHeight));

	CXTPMarkupThickness* pMargin = GetMargin();

	long num3 = pMargin->GetLeft() + pMargin->GetRight();
	long num4 = pMargin->GetTop() + pMargin->GetBottom();

	CSize szClientSize  = CSize(max(m_rcFinalRect.Width() - num3, 0), max(m_rcFinalRect.Height() - num4, 0));
	BOOL bFlag2 = bClipToBounds || szClientSize.cx < szRenderSize.cx || szClientSize.cy < szRenderSize.cy;

	if (bFlag && !bFlag2)
	{
		rc = CRect(0, 0, num, num2);
		return TRUE;
	}

	if (!bFlag && !bFlag2)
		return FALSE;

	CPoint offset = ComputeAlignmentOffset(szClientSize, szRenderSize);

	rc = CRect(-offset, szClientSize);
	if (bFlag)
	{
		rc.IntersectRect(rc, CRect(0, 0, num, num2));
	}
	return TRUE;
}

CSize CXTPMarkupFrameworkElement::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize /*szAvailableSize*/)
{
	return CSize(0, 0);
}

XTPMarkupHorizontalAlignment CXTPMarkupFrameworkElement::GetHorizontalAlignment() const
{
	CXTPMarkupEnum* pAlign = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pHorizontalAlignmentProperty));
	return pAlign ? (XTPMarkupHorizontalAlignment)(int)(*pAlign) : xtpMarkupHorizontalAlignmentStretch;
}

XTPMarkupVerticalAlignment CXTPMarkupFrameworkElement::GetVerticalAlignment() const
{
	CXTPMarkupEnum* pAlign = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pVerticalAlignmentProperty));
	return pAlign ? (XTPMarkupVerticalAlignment)(int)(*pAlign) : xtpMarkupVerticalAlignmentStretch;
}


void CXTPMarkupFrameworkElement::ArrangeCore(CRect rcFinalRect)
{
	CSize size = rcFinalRect.Size();

	CXTPMarkupThickness* pMargin = GetMargin();

	long num = pMargin->GetLeft() + pMargin->GetRight();
	long num2 = pMargin->GetTop() + pMargin->GetBottom();

	size.cx = max(0, (size.cx - num));
	size.cy = max(0, (size.cy - num2));

	CSize untransformedDS(0, 0);
	if (!m_bUnclippedDesiredSize)
	{
		untransformedDS = CSize(m_szDesiredSize.cx - num, m_szDesiredSize.cy - num2);
	}
	else
	{
		untransformedDS = m_szUnclippedDesiredSize;
	}

	if (size.cx < untransformedDS.cx)
	{
		m_bNeedsClipBounds = TRUE;
		size.cx = untransformedDS.cx;
	}

	if (size.cy < untransformedDS.cy)
	{
		m_bNeedsClipBounds = TRUE;
		size.cy = untransformedDS.cy;
	}

	if (GetHorizontalAlignment() != xtpMarkupHorizontalAlignmentStretch)
	{
		size.cx = untransformedDS.cx;
	}

	if (GetVerticalAlignment() != xtpMarkupVerticalAlignmentStretch)
	{
		size.cy = untransformedDS.cy;
	}

	long num3 = max(untransformedDS.cx, m_mmBounds.nMaxWidth);
	if (num3 < size.cx)
	{
		m_bNeedsClipBounds = TRUE;
		size.cx = num3;
	}

	long num4 = max(untransformedDS.cy, m_mmBounds.nMaxHeight);
	if (num4 < size.cy)
	{
		m_bNeedsClipBounds = TRUE;
		size.cy = num4;
	}

	CSize size7 = ArrangeOverride(size);
	m_szRenderSize = size7;

	CSize inkSize = CSize(min(size7.cx, m_mmBounds.nMaxWidth), min(size7.cy, m_mmBounds.nMaxHeight));

	if (inkSize.cx < size7.cx || inkSize.cy < size7.cy)
	{
		m_bNeedsClipBounds = TRUE;
	}

	CSize clientSize = CSize(max(0, rcFinalRect.Width() - num), max(0, rcFinalRect.Height() - num2));

	if (clientSize.cx < inkSize.cx || clientSize.cy < inkSize.cy)
	{
		m_bNeedsClipBounds = TRUE;
	}

	CPoint offset = ComputeAlignmentOffset(clientSize, inkSize);
	offset.x += rcFinalRect.left + pMargin->GetLeft();
	offset.y += rcFinalRect.top + pMargin->GetTop();

	m_ptVisualOffset = offset;
}

CPoint CXTPMarkupFrameworkElement::ComputeAlignmentOffset(CSize clientSize, CSize inkSize) const
{
	CPoint vector(0, 0);
	XTPMarkupHorizontalAlignment horizontalAlignment = GetHorizontalAlignment();
	XTPMarkupVerticalAlignment verticalAlignment = GetVerticalAlignment();

	if ((horizontalAlignment == xtpMarkupHorizontalAlignmentStretch) && (inkSize.cx > clientSize.cx))
	{
		horizontalAlignment = xtpMarkupHorizontalAlignmentLeft;
	}
	if ((verticalAlignment == xtpMarkupVerticalAlignmentStretch) && (inkSize.cy > clientSize.cy))
	{
		verticalAlignment = xtpMarkupVerticalAlignmentTop;
	}
	switch (horizontalAlignment)
	{
		case xtpMarkupHorizontalAlignmentCenter:
		case xtpMarkupHorizontalAlignmentStretch:
			vector.x = (clientSize.cx - inkSize.cx) / 2;
			break;

		default:
			if (horizontalAlignment == xtpMarkupHorizontalAlignmentRight)
			{
				vector.x = clientSize.cx - inkSize.cx;
			}
			else
			{
				vector.x = 0;
			}
			break;
	}
	switch (verticalAlignment)
	{
		case xtpMarkupVerticalAlignmentCenter:
		case xtpMarkupVerticalAlignmentStretch:
			vector.y = (clientSize.cy - inkSize.cy) / 2;
			return vector;

		case xtpMarkupVerticalAlignmentBottom:
			vector.y = clientSize.cy - inkSize.cy;
			return vector;
	}
	vector.y = 0;
	return vector;
}



CSize CXTPMarkupFrameworkElement::ArrangeOverride(CSize szFinalSize)
{
	return szFinalSize;
}

void CXTPMarkupFrameworkElement::OnPropertyChanged(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pOldValue, CXTPMarkupObject* pNewValue)
{
	CXTPMarkupUIElement::OnPropertyChanged(pProperty, pOldValue, pNewValue);

	FireTriggers(pProperty, pNewValue);

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagAffectsMeasure)
	{
		InvalidateMeasure();
	}

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagAffectsArrange)
	{
		InvalidateArrange();
	}

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagAffectsRender)
	{
		InvalidateVisual();
	}
}

void CXTPMarkupFrameworkElement::SetMargin(int nLeft, int nTop, int nRight, int nBottom)
{
	SetValue(m_pMarginProperty, new CXTPMarkupThickness(nLeft, nTop, nRight, nBottom));
}

void CXTPMarkupFrameworkElement::SetMargin(int nMargin)
{
	SetValue(m_pMarginProperty, new CXTPMarkupThickness(nMargin));
}

CXTPMarkupThickness* CXTPMarkupFrameworkElement::GetMargin() const
{
	return MARKUP_STATICCAST(CXTPMarkupThickness, GetValue(m_pMarginProperty));
}


