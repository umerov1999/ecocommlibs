// XTPMarkupBorder.cpp: implementation of the CXTPMarkupBorder class.
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
#include "XTPMarkupDecorator.h"
#include "XTPMarkupBorder.h"
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupThickness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CXTPMarkupDependencyProperty* CXTPMarkupBorder::m_pBackgroundProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupBorder::m_pPaddingProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupBorder::m_pBorderThicknessProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupBorder::m_pBorderBrushProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupBorder::m_pCornerRadiusProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Border", CXTPMarkupBorder, CXTPMarkupFrameworkElement);

void CXTPMarkupBorder::RegisterMarkupClass()
{
	m_pBackgroundProperty = CXTPMarkupDependencyProperty::Register(L"Background", MARKUP_TYPE(CXTPMarkupBrush), MARKUP_TYPE(CXTPMarkupBorder),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pPaddingProperty = CXTPMarkupDependencyProperty::Register(L"Padding", MARKUP_TYPE(CXTPMarkupThickness), MARKUP_TYPE(CXTPMarkupBorder),
		new CXTPMarkupPropertyMetadata(CXTPMarkupThickness::CreateValue(), CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pBorderThicknessProperty = CXTPMarkupDependencyProperty::Register(L"BorderThickness", MARKUP_TYPE(CXTPMarkupThickness), MARKUP_TYPE(CXTPMarkupBorder),
		new CXTPMarkupPropertyMetadata(CXTPMarkupThickness::CreateValue(), CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pBorderBrushProperty = CXTPMarkupDependencyProperty::Register(L"BorderBrush", MARKUP_TYPE(CXTPMarkupBrush), MARKUP_TYPE(CXTPMarkupBorder),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pCornerRadiusProperty = CXTPMarkupDependencyProperty::Register(L"CornerRadius", MARKUP_TYPE(CXTPMarkupThickness), MARKUP_TYPE(CXTPMarkupBorder),
		new CXTPMarkupPropertyMetadata(CXTPMarkupThickness::CreateValue(), CXTPMarkupPropertyMetadata::flagAffectsRender));
}

CXTPMarkupBorder::CXTPMarkupBorder()
{
}

CXTPMarkupBorder::~CXTPMarkupBorder()
{
}

CXTPMarkupBrush* CXTPMarkupBorder::GetBackground() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pBackgroundProperty));
}

CXTPMarkupBrush* CXTPMarkupBorder::GetBorderBrush() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pBorderBrushProperty));
}

void CXTPMarkupBorder::SetBackground(CXTPMarkupBrush* brush)
{
	SetValue(m_pBackgroundProperty, brush);
}

void CXTPMarkupBorder::SetBorderBrush(CXTPMarkupBrush* brush)
{
	SetValue(m_pBorderBrushProperty, brush);
}


CSize CXTPMarkupBorder::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	CSize size2 = GetBorderThickness()->GetSize();
	CSize size3 = GetPadding()->GetSize();

	if (m_pChild != NULL)
	{
		CSize size4(size2.cx + size3.cx, size2.cy + size3.cy);

		CSize availableSize(max(0, szAvailableSize.cx - size4.cx), max(0, szAvailableSize.cy - size4.cy));
		m_pChild->Measure(pDC, availableSize);

		CSize desiredSize = m_pChild->GetDesiredSize();

		return CSize(desiredSize.cx + size4.cx, desiredSize.cy + size4.cy);
	}

	return CSize(size2.cx + size3.cx, size2.cy + size3.cy);
}

CSize CXTPMarkupBorder::ArrangeOverride(CSize szFinalSize)
{
	if (m_pChild != NULL)
	{
		CRect rt(0, 0, szFinalSize.cx, szFinalSize.cy);
		CRect rect2 = CXTPMarkupThickness::HelperDeflateRect(rt, GetBorderThickness());

		CRect finalRect = CXTPMarkupThickness::HelperDeflateRect(rect2, GetPadding());
		m_pChild->Arrange(finalRect);
	}
	return szFinalSize;
}


void CXTPMarkupBorder::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CSize szRender = GetActualSize();
	if (szRender.cx <= 0 || szRender.cy <= 0)
		return;

	CXTPMarkupThickness* pBorderThickness = GetBorderThickness();

	CXTPMarkupBrush* pBorderBrush = GetBorderBrush();
	CXTPMarkupBrush* pBackground = GetBackground();

	if (!pBorderBrush && !pBackground)
		return;

	CXTPMarkupThickness* pCornerRadius = GetCornerRadius();

	double* pnCorners = NULL;
	if (pCornerRadius && !pCornerRadius->IsZero())
	{
		pnCorners = new double[8];
		pnCorners[0] = pnCorners[1] = pCornerRadius->GetLeft();
		pnCorners[2] = pnCorners[3] = pCornerRadius->GetTop();
		pnCorners[4] = pnCorners[5] = pCornerRadius->GetRight();
		pnCorners[6] = pnCorners[7] = pCornerRadius->GetBottom();
	}

	if (!pBorderThickness->IsZero() && pBorderBrush)
	{
		pDC->DrawRectangle(CRect(0, 0, szRender.cx, szRender.cy), pBorderBrush, pBorderThickness, pnCorners);
	}

	if (pBackground)
	{
		CPoint point(pBorderThickness->GetLeft(), pBorderThickness->GetTop());
		CPoint point2(szRender.cx - pBorderThickness->GetRight(), szRender.cy - pBorderThickness->GetBottom());
		if ((point2.x > point.x) && (point2.y > point.y))
		{
			if (pnCorners)
			{
				double dStroke = (double)pBorderThickness->GetLeft() / 2.0;
				for (int i = 0; i < 8; i++) pnCorners[i] = max(.0, pnCorners[i] - dStroke);
			}

			pDC->FillRectangle(CRect(point, point2), pBackground, pnCorners);
		}
	}

	if (pnCorners) delete[] pnCorners;
}

CXTPMarkupInputElement* CXTPMarkupBorder::InputHitTestOverride(CPoint /*point*/) const
{
	return GetBackground() != NULL ? (CXTPMarkupInputElement*)this : NULL;
}

void CXTPMarkupBorder::SetPadding(int nLeft, int nTop, int nRight, int nBottom)
{
	SetValue(m_pPaddingProperty, new CXTPMarkupThickness(nLeft, nTop, nRight, nBottom));
}

void CXTPMarkupBorder::SetPadding(int padding)
{
	SetValue(m_pPaddingProperty, new CXTPMarkupThickness(padding));
}

CXTPMarkupThickness* CXTPMarkupBorder::GetPadding() const
{
	return MARKUP_STATICCAST(CXTPMarkupThickness, GetValue(m_pPaddingProperty));
}

void CXTPMarkupBorder::SetBorderThickness(int nLeft, int nTop, int nRight, int nBottom)
{
	SetValue(m_pBorderThicknessProperty, new CXTPMarkupThickness(nLeft, nTop, nRight, nBottom));
}

void CXTPMarkupBorder::SetBorderThickness(int padding)
{
	SetValue(m_pBorderThicknessProperty, new CXTPMarkupThickness(padding));
}

CXTPMarkupThickness* CXTPMarkupBorder::GetBorderThickness() const
{
	return MARKUP_STATICCAST(CXTPMarkupThickness, GetValue(m_pBorderThicknessProperty));
}

CXTPMarkupThickness* CXTPMarkupBorder::GetCornerRadius() const
{
	return MARKUP_STATICCAST(CXTPMarkupThickness, GetValue(m_pCornerRadiusProperty));
}

void CXTPMarkupBorder::SetCornerRadius(int nCornerRadius)
{
	SetValue(m_pCornerRadiusProperty, new CXTPMarkupThickness(nCornerRadius));
}

CSize CXTPMarkupBorder::GetSize() const
{
	return GetMargin()->GetSize() + GetBorderThickness()->GetSize() + GetPadding()->GetSize();
}

