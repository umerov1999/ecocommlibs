// XTPMarkupUIElement.cpp: implementation of the CXTPMarkupUIElement class.
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
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupContext.h"
#include "XTPMarkupBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupVisual, CXTPMarkupInputElement);

void CXTPMarkupVisual::RegisterMarkupClass()
{

}

CXTPMarkupVisual::CXTPMarkupVisual()
{
	m_ptVisualOffset = CPoint(0, 0);
	m_szRenderSize = 0;

	m_rcBoundRect.SetRectEmpty();
}

BOOL CXTPMarkupVisual::GetLayoutClip(CRect& /*rc*/) const
{
	return FALSE;
}

void CXTPMarkupVisual::RenderClipped(CXTPMarkupDrawingContext* drawingContext)
{
	int nVisualCount = GetVisualChildrenCount();
	for (int i = 0; i < nVisualCount; i++)
	{
		GetVisualChild(i)->RenderClipped(drawingContext);
	}
}

void CXTPMarkupVisual::Render(CXTPMarkupDrawingContext* drawingContext)
{
	CRect rcClipBox = drawingContext->GetClipBox();
	rcClipBox.OffsetRect(-m_ptVisualOffset);

	RECT rcIntersect;
	if (!::IntersectRect(&rcIntersect, GetBoundRect(), &rcClipBox))
	{
		RenderClipped(drawingContext);
		return;
	}

	CRect rcLayoutClip(0, 0, 0, 0);
	BOOL bClipLayout = FALSE;

	if (GetLayoutClip(rcLayoutClip))
	{
		if (rcLayoutClip.IsRectEmpty())
		{
			RenderClipped(drawingContext);
			return;
		}

		rcClipBox.IntersectRect(rcClipBox, rcLayoutClip);

		if (!::IntersectRect(&rcIntersect, GetBoundRect(), &rcClipBox))
		{
			RenderClipped(drawingContext);
			return;
		}

		bClipLayout = TRUE;
	}

	drawingContext->OffsetViewport(m_ptVisualOffset);

	HRGN hrgnClip = 0;

	if (bClipLayout)
	{
		hrgnClip = drawingContext->SaveClipRegion();

		drawingContext->IntersectClipRect(rcLayoutClip);
	}

	OnRender(drawingContext);

	int nVisualCount = GetVisualChildrenCount();
	for (int i = 0; i < nVisualCount; i++)
	{
		GetVisualChild(i)->Render(drawingContext);
	}

	if (IsKeyboardFocused())
	{
		OnRenderFocusVisual(drawingContext);

	}

	if (hrgnClip != NULL)
	{
		drawingContext->RestoreClipRegion(hrgnClip);
	}

	drawingContext->OffsetViewport(-m_ptVisualOffset);
}

int CXTPMarkupVisual::GetVisualChildrenCount() const
{
	return 0;
}

CXTPMarkupVisual* CXTPMarkupVisual::GetVisualChild(int /*nIndex*/) const
{
	ASSERT(FALSE);
	return NULL;
}

int CXTPMarkupVisual::GetLogicalChildrenCount() const
{
	return GetVisualChildrenCount();
}

CXTPMarkupObject* CXTPMarkupVisual::GetLogicalChild(int nIndex) const
{
	return GetVisualChild(nIndex);
}

CXTPMarkupInputElement* CXTPMarkupVisual::InputHitTest(CPoint point) const
{
	CPoint ptVisualPoint = point - m_ptVisualOffset;

	if (!m_rcBoundRect.PtInRect(ptVisualPoint))
		return NULL;

	CRect rcClip;
	if (GetLayoutClip(rcClip))
	{
		if (!rcClip.PtInRect(ptVisualPoint))
			return NULL;
	}

	int nVisualCount = GetVisualChildrenCount();
	for (int i = nVisualCount - 1; i >= 0; i--)
	{
		CXTPMarkupInputElement* pObject = GetVisualChild(i)->InputHitTest(ptVisualPoint);
		if (pObject)
			return pObject;
	}

	CRect rcRender(0, 0, m_szRenderSize.cx, m_szRenderSize.cy);
	if (!rcRender.PtInRect(ptVisualPoint))
		return NULL;

	return InputHitTestOverride(ptVisualPoint);
}

void CXTPMarkupVisual::UpdateBoundRect()
{
	m_rcBoundRect.SetRect(0, 0, m_szRenderSize.cx, m_szRenderSize.cy);

	int nVisualCount = GetVisualChildrenCount();
	for (int i = 0; i < nVisualCount; i++)
	{
		CXTPMarkupVisual* pVisual = GetVisualChild(i);
		pVisual->UpdateBoundRect();

		CRect rcBoundRect = pVisual->m_rcBoundRect;
		rcBoundRect.OffsetRect(pVisual->m_ptVisualOffset);

		m_rcBoundRect.UnionRect(m_rcBoundRect, rcBoundRect);
	}
}

CXTPMarkupInputElement* CXTPMarkupVisual::InputHitTestOverride(CPoint /*point*/) const
{
	return (CXTPMarkupInputElement*)this;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPMarkupDependencyProperty* CXTPMarkupUIElement::m_pClipToBoundsProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupUIElement::m_pVisibilityProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupUIElement::m_pFocusableProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupUIElement::m_pIsEnabledProperty = NULL;


IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupUIElement, CXTPMarkupVisual);

void CXTPMarkupUIElement::RegisterMarkupClass()
{
	m_pClipToBoundsProperty = CXTPMarkupDependencyProperty::Register(L"ClipToBounds", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupUIElement));

	m_pVisibilityProperty = CXTPMarkupDependencyProperty::Register(L"Visibility", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupUIElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertVisibility, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pFocusableProperty = CXTPMarkupDependencyProperty::Register(L"Focusable", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupUIElement),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateFalseValue()));

	m_pIsEnabledProperty = CXTPMarkupDependencyProperty::Register(L"IsEnabled", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupUIElement),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateTrueValue()));
}

CXTPMarkupUIElement::CXTPMarkupUIElement()
{
	m_szDesiredSize = 0;
	m_rcFinalRect.SetRectEmpty();

	m_bNeverMeasured = TRUE;
	m_bMeasureDirty = FALSE;

	m_bNeverArranged = TRUE;
	m_bArrangeDirty = FALSE;

	m_szPreviousAvailableSize = 0;
	m_bArrangeInProgress = FALSE;
	m_bMeasureInProgress = FALSE;
}

CXTPMarkupUIElement::~CXTPMarkupUIElement()
{

}

CXTPMarkupVisual* CXTPMarkupVisual::GetVisualParent() const
{
	CXTPMarkupObject* pUIElement = m_pLogicalParent;

	while (pUIElement && !pUIElement->IsKindOf(MARKUP_TYPE(CXTPMarkupVisual)))
	{
		pUIElement = pUIElement->GetLogicalParent();
	}

	return (CXTPMarkupVisual*)pUIElement;
}

void CXTPMarkupUIElement::InvalidateArrange()
{
	if (!m_bArrangeInProgress && !m_bNeverArranged)
	{
		m_bArrangeDirty = TRUE;

		Arrange(m_rcFinalRect);
		InvalidateVisual();
	}
}

void CXTPMarkupUIElement::InvalidateMeasure()
{
	CXTPMarkupDrawingContext dc(GetMarkupContext());
	InvalidateMeasureOverride(&dc);
}

void CXTPMarkupUIElement::InvalidateMeasureOverride(CXTPMarkupDrawingContext* pDC)
{
	if (!m_bMeasureInProgress && !m_bNeverMeasured)
	{
		m_bMeasureDirty = TRUE;
		m_bArrangeDirty = TRUE;

		CSize sz = GetDesiredSize();

		Measure(pDC, m_szPreviousAvailableSize);

		if (sz != GetDesiredSize())
		{
			m_bMeasureDirty = TRUE;
			m_bArrangeDirty = TRUE;

			CXTPMarkupUIElement* pParent = MARKUP_DYNAMICCAST(CXTPMarkupUIElement, GetVisualParent());
			if (pParent)
			{
				pParent->InvalidateMeasureOverride(pDC);
			}
			else if (m_pMarkupContext)
			{
				m_pMarkupContext->OnInvalidateArrange(this);
			}
		}
		else
		{
			Arrange(m_rcFinalRect);
			InvalidateVisual();
		}
	}
}

void CXTPMarkupUIElement::InvalidateVisual()
{
	if (m_pMarkupContext) m_pMarkupContext->OnInvalidateVisual(this);
}

void CXTPMarkupUIElement::Arrange(CRect rcFinalRect)
{
	if (GetVisibility() == xtpMarkupVisibilityCollapsed)
	{
		m_rcFinalRect = rcFinalRect;
		m_bArrangeDirty = FALSE;
		m_bNeverArranged = FALSE;
		return;
	}

	if (m_bNeverMeasured)
	{
		CXTPMarkupDrawingContext dc(GetMarkupContext());
		Measure(&dc, rcFinalRect.Size());
	}

	if ((m_bArrangeDirty || m_bNeverArranged) || (rcFinalRect != m_rcFinalRect))
	{
		m_bNeverArranged = FALSE;
		m_bArrangeInProgress = TRUE;
		ArrangeCore(rcFinalRect);
		m_bArrangeInProgress = FALSE;

		UpdateBoundRect();
	}

	m_rcFinalRect = rcFinalRect;
	m_bArrangeDirty = FALSE;
}

void CXTPMarkupUIElement::ArrangeCore(CRect rcFinalRect)
{
	m_szRenderSize = rcFinalRect.Size();
}

void CXTPMarkupUIElement::Measure(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	if (GetVisibility() == xtpMarkupVisibilityCollapsed)
	{
		m_bMeasureDirty = TRUE;
		m_bNeverMeasured = FALSE;
		m_szPreviousAvailableSize = szAvailableSize;
		m_szDesiredSize = CSize(0, 0);
		return;
	}

	if ((!m_bMeasureDirty && !m_bNeverMeasured) && (szAvailableSize == m_szPreviousAvailableSize) && !pDC->IsPrinting())
	{
		return;
	}

	m_bNeverMeasured = FALSE;

	m_bArrangeDirty = TRUE;

	m_bMeasureInProgress = TRUE;
	CSize size = MeasureCore(pDC, szAvailableSize);
	m_bMeasureInProgress = FALSE;

	m_szDesiredSize = size;
	m_szPreviousAvailableSize = szAvailableSize;
	m_bMeasureDirty = pDC->IsPrinting() ? TRUE : FALSE;
}

CSize CXTPMarkupUIElement::MeasureCore(CXTPMarkupDrawingContext* /*pDC*/, CSize /*szAvailableSize*/)
{
	return CSize(0, 0);
}

BOOL CXTPMarkupUIElement::GetLayoutClip(CRect& rc) const
{
	if (GetVisibility() != xtpMarkupVisibilityVisible)
	{
		rc.SetRectEmpty();
		return TRUE;
	}
	return FALSE;
}

