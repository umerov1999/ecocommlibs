// XTPMarkupButton.cpp: implementation of the CXTPMarkupButton class.
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
#include "XTPMarkupControl.h"
#include "XTPMarkupBuilder.h"
#include "XTPMarkupTextBlock.h"
#include "XTPMarkupContext.h"
#include "XTPMarkupDecorator.h"
#include "XTPMarkupBorder.h"
#include "XTPMarkupPanel.h"
#include "Text/XTPMarkupInline.h"
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupThickness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CXTPMarkupDependencyProperty* CXTPMarkupControl::m_pBackgroundProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupControl::m_pForegroundProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupControl::m_pPaddingProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Control", CXTPMarkupControl, CXTPMarkupFrameworkElement)

void CXTPMarkupControl::RegisterMarkupClass()
{
	CXTPMarkupBorder::RegisterType();
	CXTPMarkupPanel::RegisterType();
	CXTPMarkupTextElement::RegisterType();

	m_pPaddingProperty = CXTPMarkupBorder::m_pPaddingProperty->AddOwner(MARKUP_TYPE(CXTPMarkupControl));
	m_pBackgroundProperty = CXTPMarkupPanel::m_pBackgroundProperty->AddOwner(MARKUP_TYPE(CXTPMarkupControl));
	m_pForegroundProperty = CXTPMarkupTextElement::m_pForegroundProperty->AddOwner(MARKUP_TYPE(CXTPMarkupControl));
}

CSize CXTPMarkupControl::ArrangeOverride(CSize szFinalSize)
{
	if (GetVisualChildrenCount() > 0)
	{
		CXTPMarkupUIElement* pVisualChild = (CXTPMarkupUIElement*)GetVisualChild(0);

		pVisualChild->Arrange(CRect(0, 0, szFinalSize.cx, szFinalSize.cy));
	}
	return szFinalSize;
}

CSize CXTPMarkupControl::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	if (GetVisualChildrenCount() > 0)
	{
		CXTPMarkupUIElement* pVisualChild = (CXTPMarkupUIElement*)GetVisualChild(0);

		pVisualChild->Measure(pDC, szAvailableSize);
		return pVisualChild->GetDesiredSize();
	}

	return CSize(0, 0);
}


CXTPMarkupBrush* CXTPMarkupControl::GetBackground() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pBackgroundProperty));
}

void CXTPMarkupControl::SetBackground(CXTPMarkupBrush* brush)
{
	SetValue(m_pBackgroundProperty, brush);
}

CXTPMarkupBrush* CXTPMarkupControl::GetForeground() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pForegroundProperty));
}

void CXTPMarkupControl::SetForeground(CXTPMarkupBrush* brush)
{
	SetValue(m_pForegroundProperty, brush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupContentControl

CXTPMarkupDependencyProperty* CXTPMarkupContentControl::m_pContentProperty = NULL;

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupContentControl, CXTPMarkupControl)

void CXTPMarkupContentControl::RegisterMarkupClass()
{
	m_pContentProperty = CXTPMarkupDependencyProperty::Register(L"Content", MARKUP_TYPE(CXTPMarkupUIElement), MARKUP_TYPE(CXTPMarkupContentControl),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertContent, CXTPMarkupPropertyMetadata::flagAffectsMeasure));
}

CXTPMarkupContentControl::CXTPMarkupContentControl()
{

}

CXTPMarkupContentControl::~CXTPMarkupContentControl()
{
}

CXTPMarkupUIElement* CXTPMarkupContentControl::GetContent() const
{
	return MARKUP_STATICCAST(CXTPMarkupUIElement, GetValue(m_pContentProperty));
}

void CXTPMarkupContentControl::SetContent(CXTPMarkupUIElement* pContent)
{
	SetValue(m_pContentProperty, pContent);
}

void CXTPMarkupContentControl::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	if (GetContent() != NULL)
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' already has a child and cannot add '%ls'")
			_T(". '%ls' can accept only one child."),
			(LPCTSTR)GetType()->m_lpszClassName, (LPCTSTR)pContent->GetType()->m_lpszClassName, (LPCTSTR)GetType()->m_lpszClassName));
	}

	if (IsStringObject(pContent))
	{
		CXTPMarkupTextBlock* pTextBlock = MARKUP_CREATE(CXTPMarkupTextBlock, GetMarkupContext());
		pTextBlock->SetContentObject(pBuilder, (CXTPMarkupString*)pContent);

		SetContent(pTextBlock);
	}
	else if (pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupUIElement)))
	{
		SetContent((CXTPMarkupUIElement*)pContent);
	}
	else
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' object cannot be added to '%ls'. Object cannot be converted to type 'CXTPMarkupUIElement'"),
			(LPCTSTR)pContent->GetType()->m_lpszClassName, (LPCTSTR)GetType()->m_lpszClassName));
	}
}

BOOL CXTPMarkupContentControl::HasContentObject() const
{
	return GetContent() != NULL;
}

void CXTPMarkupControl::SetPadding(int nLeft, int nTop, int nRight, int nBottom)
{
	SetValue(m_pPaddingProperty, new CXTPMarkupThickness(nLeft, nTop, nRight, nBottom));
}

void CXTPMarkupControl::SetPadding(int padding)
{
	SetValue(m_pPaddingProperty, new CXTPMarkupThickness(padding));
}

CXTPMarkupThickness* CXTPMarkupControl::GetPadding() const
{
	return  MARKUP_STATICCAST(CXTPMarkupThickness, GetValue(m_pPaddingProperty));
}


