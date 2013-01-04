// XTPMarkupInline.cpp: implementation of the CXTPMarkupInline class.
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

#include "StdAfx.h"

#include <Common/XTPSystemHelpers.h>
#include <Common/XTPResourceManager.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupInputElement.h>
#include <Markup/XTPMarkupUIElement.h>
#include <Markup/XTPMarkupFrameworkElement.h>
#include <Markup/XTPMarkupControl.h>
#include <Markup/XTPMarkupResourceDictionary.h>
#include <Markup/XTPMarkupDrawingContext.h>
#include <Markup/XTPMarkupBuilder.h>
#include <Markup/XTPMarkupTextBlock.h>
#include <Markup/XTPMarkupContext.h>
#include <Markup/XTPMarkupButton.h>
#include <Markup/Transform/XTPMarkupRenderTransform.h>
#include <Markup/Transform/XTPMarkupRotateTransform.h>
#include <Markup/Text/XTPMarkupInline.h>
#include <Markup/Text/XTPMarkupSpan.h>
#include <Markup/Text/XTPMarkupRun.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupFrameworkContentElement

CXTPMarkupDependencyProperty* CXTPMarkupFrameworkContentElement::m_pTagProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"FrameworkContentElement", CXTPMarkupFrameworkContentElement, CXTPMarkupInputElement);

void CXTPMarkupFrameworkContentElement::RegisterMarkupClass()
{
	CXTPMarkupFrameworkElement::RegisterType();

	m_pTagProperty = CXTPMarkupFrameworkElement::m_pTagProperty->AddOwner(MARKUP_TYPE(CXTPMarkupFrameworkContentElement));
}

CXTPMarkupFrameworkContentElement::CXTPMarkupFrameworkContentElement()
{
}

void CXTPMarkupFrameworkContentElement::OnPropertyChanged(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pOldValue, CXTPMarkupObject* pNewValue)
{
	CXTPMarkupInputElement::OnPropertyChanged(pProperty, pOldValue, pNewValue);

	FireTriggers(pProperty, pNewValue);

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagAffectsMeasure)
	{
		CXTPMarkupFrameworkElement* pParent = GetParent();
		if (pParent) pParent->InvalidateMeasure();
	}

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagAffectsArrange)
	{
		CXTPMarkupFrameworkElement* pParent = GetParent();
		if (pParent) pParent->InvalidateArrange();
	}

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagAffectsRender)
	{
		CXTPMarkupFrameworkElement* pParent = GetParent();
		if (pParent) pParent->InvalidateVisual();
	}
}

CXTPMarkupFrameworkElement* CXTPMarkupFrameworkContentElement::GetParent() const
{
	CXTPMarkupObject* pParent = m_pLogicalParent;
	while (pParent)
	{
		if (pParent->IsKindOf(MARKUP_TYPE(CXTPMarkupFrameworkElement)))
			return (CXTPMarkupFrameworkElement*)pParent;

		pParent = pParent->GetLogicalParent();
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
//
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pBackgroundProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pForegroundProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pFontSizeProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pFontWeightProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pFontFamilyProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pFontStyleProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pTextDecorationsProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pFontQualityProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTextElement::m_pFontCharsetProperty = NULL;

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupTextElement

IMPLEMENT_MARKUPCLASS(L"TextElement", CXTPMarkupTextElement, CXTPMarkupFrameworkContentElement);

void CXTPMarkupTextElement::RegisterMarkupClass()
{
	m_pBackgroundProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"Background", MARKUP_TYPE(CXTPMarkupBrush), MARKUP_TYPE(CXTPMarkupTextElement),
			new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pForegroundProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"Foreground", MARKUP_TYPE(CXTPMarkupBrush), MARKUP_TYPE(CXTPMarkupTextElement),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pFontSizeProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"FontSize", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupTextElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertLength, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pFontWeightProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"FontWeight", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupTextElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertFontWeight, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pFontFamilyProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"FontFamily", MARKUP_TYPE(CXTPMarkupString), MARKUP_TYPE(CXTPMarkupTextElement),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pFontStyleProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"FontStyle", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupTextElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertFontStyle, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pTextDecorationsProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"TextDecorations", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupTextElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertTextDecorations, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pFontQualityProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"FontQuality", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupTextElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertFontQuality, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pFontCharsetProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"FontCharset", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupTextElement),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagInherited | CXTPMarkupPropertyMetadata::flagAffectsRender));

}

CXTPMarkupTextElement::CXTPMarkupTextElement()
{

}

CXTPMarkupTextElement::~CXTPMarkupTextElement()
{

}

void CXTPMarkupTextElement::SetBackground(CXTPMarkupBrush* pBrush)
{
	SetValue(m_pBackgroundProperty, pBrush);
}

void CXTPMarkupTextElement::SetBackground(COLORREF clr)
{
	SetValue(m_pBackgroundProperty, new CXTPMarkupSolidColorBrush(clr));
}

void CXTPMarkupTextElement::SetBackground(CXTPMarkupObject* pObject, CXTPMarkupBrush* pBrush)
{
	pObject->SetValue(m_pBackgroundProperty, pBrush);
}

CXTPMarkupBrush* CXTPMarkupTextElement::GetBackground() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pBackgroundProperty));
}

void CXTPMarkupTextElement::SetForeground(CXTPMarkupBrush* pBrush)
{
	SetValue(m_pForegroundProperty, pBrush);
}

void CXTPMarkupTextElement::SetForeground(COLORREF clr)
{
	SetValue(m_pForegroundProperty, new CXTPMarkupSolidColorBrush(clr));
}

void CXTPMarkupTextElement::SetForeground(CXTPMarkupObject* pObject, CXTPMarkupBrush* pBrush)
{
	pObject->SetValue(m_pForegroundProperty, pBrush);
}

CXTPMarkupBrush* CXTPMarkupTextElement::GetForeground() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pForegroundProperty));
}


void CXTPMarkupTextElement::SetFontSize(int nFontSize)
{
	SetValue(m_pFontSizeProperty, new CXTPMarkupInt(nFontSize));
}

int CXTPMarkupTextElement::GetFontSize() const
{
	CXTPMarkupInt* pFontSize = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pFontSizeProperty));
	if (!pFontSize)
		return -12;

	return *pFontSize;
}

void CXTPMarkupTextElement::SetFontWeight(int nFontWeight)
{
	SetValue(m_pFontWeightProperty, CXTPMarkupEnum::CreateValue(nFontWeight));
}

int CXTPMarkupTextElement::GetFontWeight() const
{
	CXTPMarkupEnum* pFontWeigh = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pFontWeightProperty));
	if (!pFontWeigh)
		return FW_NORMAL;

	return *pFontWeigh;
}

void CXTPMarkupTextElement::SetFontStyle(int nFontStyle)
{
	SetValue(m_pFontStyleProperty, CXTPMarkupEnum::CreateValue(nFontStyle));
}

int CXTPMarkupTextElement::GetFontStyle() const
{
	CXTPMarkupEnum* pFontStyle = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pFontStyleProperty));
	if (!pFontStyle)
		return 0;

	return *pFontStyle;
}

void CXTPMarkupTextElement::SetTextDecorations(int nTextDecorations)
{
	SetValue(m_pTextDecorationsProperty, CXTPMarkupEnum::CreateValue(nTextDecorations));
}

int CXTPMarkupTextElement::GetTextDecorations() const
{
	CXTPMarkupEnum* pTextDecorations = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pTextDecorationsProperty));
	if (!pTextDecorations)
		return 0;

	return *pTextDecorations;
}


void CXTPMarkupTextElement::SetFontFamily(LPCWSTR lpszFontFamily)
{
	SetValue(m_pFontFamilyProperty, CXTPMarkupString::CreateValue(lpszFontFamily));

}
LPCWSTR CXTPMarkupTextElement::GetFontFamily() const
{
	CXTPMarkupString* pFontFamily = MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pFontFamilyProperty));
	if (!pFontFamily)
		return NULL;

	return *pFontFamily;
}

CXTPMarkupInputElement* CXTPMarkupFrameworkContentElement::InputHitTest(CPoint /*point*/) const
{
	return (CXTPMarkupInputElement*)this;
}

CRect CXTPMarkupFrameworkContentElement::GetBoundRect() const
{
	return GetParent()->GetBoundRect();
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupInline

CXTPMarkupDependencyProperty* CXTPMarkupInline::m_pBaselineAlignmentProperty = NULL;

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupInline, CXTPMarkupTextElement);

void CXTPMarkupInline::RegisterMarkupClass()
{
	m_pBaselineAlignmentProperty = CXTPMarkupDependencyProperty::Register(L"BaselineAlignment", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupInline),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertBaselineAlignment, CXTPMarkupPropertyMetadata::flagAffectsArrange));
}


CXTPMarkupInline::CXTPMarkupInline()
{
	m_nIndex = -1;
}

CXTPMarkupInline::~CXTPMarkupInline()
{

}

POSITION CXTPMarkupInline::GetContentStartPosition() const
{
	return MARKUP_POSITION_EOF;
};

void CXTPMarkupInline::GetContentNextPosition(POSITION& pos) const
{
	pos = NULL;
}

BOOL CXTPMarkupInline::IsWordBreakPosition(POSITION /*pos*/) const
{
	return FALSE;
}

BOOL CXTPMarkupInline::IsLineBreakPosition(POSITION /*pos*/) const
{
	return FALSE;
}

BOOL CXTPMarkupInline::IsCaretReturnPosition(POSITION /*pos*/) const
{
	return FALSE;
}

BOOL CXTPMarkupInline::IsWhiteSpacePosition(POSITION /*pos*/) const
{
	return FALSE;
}

void CXTPMarkupInline::PrepareMeasure(CXTPMarkupDrawingContext *pDC, CXTPMarkupRenderTransform *pRenderTransform)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pRenderTransform);
}


CSize CXTPMarkupInline::Measure(CXTPMarkupDrawingContext* /*pDC*/, POSITION /*posStart*/, POSITION /*posEnd*/)
{
	return CSize(0, 0);
}

int CXTPMarkupInline::GetBaseline() const
{
	return 0;
}

void CXTPMarkupInline::Arrange(CRect /*rcFinalRect*/, POSITION /*posStart*/, POSITION /*posEnd*/)
{

}

void CXTPMarkupInline::Render(CXTPMarkupDrawingContext* /*pDC*/, CRect /*rc*/, POSITION /*posStart*/, POSITION /*posEnd*/)
{

}

CXTPMarkupInline* CXTPMarkupInline::GetFirstInline() const
{
	return (CXTPMarkupInline*)this;
}

CXTPMarkupInline* CXTPMarkupInline::GetNextInline() const
{
	CXTPMarkupInlineCollection* pOwner = MARKUP_DYNAMICCAST(CXTPMarkupInlineCollection, m_pLogicalParent);

	if (!pOwner)
		return NULL;

	if (pOwner->GetCount() > m_nIndex + 1)
		return pOwner->GetInline(m_nIndex + 1)->GetFirstInline();

	if (MARKUP_DYNAMICCAST(CXTPMarkupSpan, pOwner->GetLogicalParent()))
		return ((CXTPMarkupSpan*)(pOwner->GetLogicalParent()))->GetNextInline();

	return NULL;
}

CXTPMarkupTextBlock* CXTPMarkupInline::GetTextBlock() const
{
	return MARKUP_STATICCAST(CXTPMarkupTextBlock, GetParent());
}



//////////////////////////////////////////////////////////////////////////
// CXTPMarkupLineBreak

IMPLEMENT_MARKUPCLASS(L"LineBreak", CXTPMarkupLineBreak, CXTPMarkupRun);

void CXTPMarkupLineBreak::RegisterMarkupClass()
{

}

CXTPMarkupLineBreak::CXTPMarkupLineBreak()
{
	SetText(L"\n");
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupInlineCollection

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupInlineCollection, CXTPMarkupCollection);

void CXTPMarkupInlineCollection::RegisterMarkupClass()
{

}
CXTPMarkupInlineCollection::CXTPMarkupInlineCollection()
{
	m_pElementType = MARKUP_TYPE(CXTPMarkupInline);
}

CXTPMarkupInlineCollection::~CXTPMarkupInlineCollection()
{
}

void CXTPMarkupInlineCollection::OnItemAdded(CXTPMarkupObject* pItem, int nIndex)
{
	((CXTPMarkupInline*)pItem)->m_nIndex = nIndex;
}

void CXTPMarkupInlineCollection::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	CXTPMarkupInputElement* pParent = MARKUP_DYNAMICCAST(CXTPMarkupInputElement, m_pLogicalParent);
	ASSERT(pParent);
	if (!pParent)
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' object cannot be added to '%ls'. Object cannot be converted to type 'CXTPMarkupInline'"),
			(LPCTSTR)pContent->GetType()->m_lpszClassName, (LPCTSTR)GetType()->m_lpszClassName));
	}

	if (IsStringObject(pContent))
	{
		CXTPMarkupRun* pRun = MARKUP_CREATE(CXTPMarkupRun, pParent->GetMarkupContext());
		pRun->SetText((CXTPMarkupString*)pContent);
		Add(pRun);
	}
	else if (pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupInline)))
	{
		Add((CXTPMarkupInline*)pContent);
	}
	else if  (pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupUIElement)))
	{
		CXTPMarkupInlineUIContainer* pInlineUIContainer = MARKUP_CREATE(CXTPMarkupInlineUIContainer, pParent->GetMarkupContext());
		pInlineUIContainer->SetChild((CXTPMarkupUIElement*)pContent);
		Add(pInlineUIContainer);
	}
	else
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' object cannot be added to '%ls'. Object cannot be converted to type 'CXTPMarkupInline'"),
			(LPCTSTR)pContent->GetType()->m_lpszClassName, (LPCTSTR)GetType()->m_lpszClassName));
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupSpan

IMPLEMENT_MARKUPCLASS(L"Span", CXTPMarkupSpan, CXTPMarkupInline);

void CXTPMarkupSpan::RegisterMarkupClass()
{

}

CXTPMarkupSpan::CXTPMarkupSpan()
{
	m_pInlines = new CXTPMarkupInlineCollection();
	m_pInlines->SetLogicalParent(this);
}

CXTPMarkupSpan::~CXTPMarkupSpan()
{
	if (m_pInlines)
	{
		m_pInlines->SetLogicalParent(NULL);
		m_pInlines->Release();
	}
}

void CXTPMarkupSpan::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	m_pInlines->SetContentObject(pBuilder, pContent);
}
BOOL CXTPMarkupSpan::HasContentObject() const
{
	return m_pInlines->HasContentObject();
}

BOOL CXTPMarkupSpan::AllowWhiteSpaceContent() const
{
	return TRUE;
}


CXTPMarkupInline* CXTPMarkupSpan::GetFirstInline() const
{
	return m_pInlines->GetCount() > 0 ? m_pInlines->GetInline(0)->GetFirstInline() : (CXTPMarkupInline*)this;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupBold

IMPLEMENT_MARKUPCLASS(L"Bold", CXTPMarkupBold, CXTPMarkupSpan);

void CXTPMarkupBold::RegisterMarkupClass()
{

}

CXTPMarkupBold::CXTPMarkupBold()
{
	SetFontWeight(FW_BOLD);

}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupHyperlink

CXTPMarkupRoutedEvent* CXTPMarkupHyperlink::m_pClickEvent = NULL;

IMPLEMENT_MARKUPCLASS(L"Hyperlink", CXTPMarkupHyperlink, CXTPMarkupSpan);

void CXTPMarkupHyperlink::RegisterMarkupClass()
{
	CXTPMarkupStyle::RegisterType();
	CXTPMarkupButtonBase::RegisterType();
	CXTPMarkupSolidColorBrush::RegisterType();
	CXTPMarkupHyperlink::RegisterType();

	CXTPMarkupType* pType = MARKUP_TYPE(CXTPMarkupHyperlink);

	CXTPMarkupStyle* pStyle = new CXTPMarkupStyle();
	pStyle->GetSetters()->Add(new CXTPMarkupSetter(CXTPMarkupHyperlink::m_pTextDecorationsProperty, CXTPMarkupEnum::CreateValue(1)));
	pStyle->GetSetters()->Add(new CXTPMarkupSetter(CXTPMarkupHyperlink::m_pForegroundProperty, new CXTPMarkupSolidColorBrush(RGB(0, 0, 255))));
	pStyle->GetSetters()->Add(new CXTPMarkupSetter(CXTPMarkupHyperlink::m_pCursorProperty, new CXTPMarkupInt(32649)));

	CXTPMarkupTriggerCollection* pTriggers = new CXTPMarkupTriggerCollection();
	CXTPMarkupTrigger* pTrigger = new CXTPMarkupTrigger(CXTPMarkupHyperlink::m_pIsMouseOverProperty, CXTPMarkupBool::CreateTrueValue());
	CXTPMarkupSetterColection* pSetters = new CXTPMarkupSetterColection();
	pSetters->Add(new CXTPMarkupSetter(CXTPMarkupHyperlink::m_pForegroundProperty, new CXTPMarkupSolidColorBrush(RGB(255, 0, 0))));
	pTrigger->SetSetters(pSetters);
	pTriggers->Add(pTrigger);
	pStyle->SetTriggers(pTriggers);

	pType->SetTypeStyle(pStyle);

	m_pClickEvent = (CXTPMarkupRoutedEvent*)CXTPMarkupButtonBase::m_pClickEvent->AddOwner(MARKUP_TYPE(CXTPMarkupHyperlink));
}

CXTPMarkupHyperlink::CXTPMarkupHyperlink()
{
	m_bPressed = FALSE;
}

void CXTPMarkupHyperlink::OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* e)
{
	if (m_bPressed)
	{
		m_bPressed = FALSE;
		OnClick();
		e->SetHandled();
	}
}

void CXTPMarkupHyperlink::OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* e)
{
	m_bPressed = TRUE;
	e->SetHandled();
}

void CXTPMarkupHyperlink::OnClick()
{
	CXTPMarkupRoutedEventArgs* e = new CXTPMarkupRoutedEventArgs(m_pClickEvent, this);
	RaiseEvent(e);

	e->Release();
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupItalic

IMPLEMENT_MARKUPCLASS(L"Italic", CXTPMarkupItalic, CXTPMarkupSpan);

void CXTPMarkupItalic::RegisterMarkupClass()
{

}

CXTPMarkupItalic::CXTPMarkupItalic()
{
	SetFontStyle(1);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupUnderline

IMPLEMENT_MARKUPCLASS(L"Underline", CXTPMarkupUnderline, CXTPMarkupSpan);

void CXTPMarkupUnderline::RegisterMarkupClass()
{

}

CXTPMarkupUnderline::CXTPMarkupUnderline()
{
	SetTextDecorations(1);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupInlineUIContainer

IMPLEMENT_MARKUPCLASS(L"InlineUIContainer", CXTPMarkupInlineUIContainer, CXTPMarkupInline);

void CXTPMarkupInlineUIContainer::RegisterMarkupClass()
{

}

CXTPMarkupInlineUIContainer::CXTPMarkupInlineUIContainer()
{
	m_pChild = NULL;
}

CXTPMarkupInlineUIContainer::CXTPMarkupInlineUIContainer(CXTPMarkupUIElement* pElement)
{
	m_pChild = NULL;
	SetChild(pElement);
}

CXTPMarkupInlineUIContainer::~CXTPMarkupInlineUIContainer()
{
	MARKUP_RELEASE(m_pChild);
}

CXTPMarkupUIElement* CXTPMarkupInlineUIContainer::GetChild() const
{
	return m_pChild;
}

void CXTPMarkupInlineUIContainer::SetChild(CXTPMarkupUIElement* pChild)
{
	if (m_pChild)
	{
		m_pChild->SetLogicalParent(NULL);
		MARKUP_RELEASE(m_pChild);
	}

	m_pChild = pChild;

	if (m_pChild)
	{
		m_pChild->SetLogicalParent(this);
	}
}


void CXTPMarkupInlineUIContainer::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	if (!pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupUIElement)))
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' object cannot be added to '%ls'. Object cannot be converted to type 'CXTPMarkupUIElement'"),
			(LPCTSTR)pContent->GetType()->m_lpszClassName, (LPCTSTR)GetType()->m_lpszClassName));
	}

	SetChild((CXTPMarkupUIElement*)pContent);
}

BOOL CXTPMarkupInlineUIContainer::HasContentObject() const
{
	return m_pChild != NULL;
}

POSITION CXTPMarkupInlineUIContainer::GetContentStartPosition() const
{
	return m_pChild ? MARKUP_POSITION_START : MARKUP_POSITION_EOF;
};

void CXTPMarkupInlineUIContainer::GetContentNextPosition(POSITION& pos) const
{
	pos = (pos == MARKUP_POSITION_START ? MARKUP_POSITION_END : MARKUP_POSITION_EOF);
}

BOOL CXTPMarkupInlineUIContainer::IsWordBreakPosition(POSITION pos) const
{
	return pos == MARKUP_POSITION_START || pos == MARKUP_POSITION_END;
}

CSize CXTPMarkupInlineUIContainer::Measure(CXTPMarkupDrawingContext* pDC, POSITION posStart, POSITION /*posEnd*/)
{
	if (!m_pChild || posStart != MARKUP_POSITION_START)
		return CSize(0, 0);

	m_pChild->Measure(pDC, CSize(32000, 32000));

	return m_pChild->GetDesiredSize();
}

void CXTPMarkupInlineUIContainer::Arrange(CRect rcFinalRect, POSITION posStart, POSITION /*posEnd*/)
{
	if (!m_pChild || posStart != MARKUP_POSITION_START)
		return;

	m_pChild->Arrange(rcFinalRect);
}

void CXTPMarkupInlineUIContainer::Render(CXTPMarkupDrawingContext* pDC, CRect /*rc*/, POSITION posStart, POSITION /*posEnd*/)
{
	if (!m_pChild  || posStart != MARKUP_POSITION_START)
		return;

	m_pChild->Render(pDC);
}

CXTPMarkupInputElement* CXTPMarkupInlineUIContainer::InputHitTest(CPoint point) const
{
	CXTPMarkupInputElement* pObject = m_pChild ? m_pChild->InputHitTest(point) : (CXTPMarkupInputElement*)this;

	if (pObject)
		return pObject;

	return (CXTPMarkupInputElement*)this;
}

#ifdef _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////
// TextElement


BEGIN_DISPATCH_MAP(CXTPMarkupTextElement, CXTPMarkupFrameworkContentElement)
	DISP_PROPERTY_EX_ID(CXTPMarkupTextElement, "Tag", 400, OleGetTag, OleSetTag, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPMarkupTextElement, "TextDecorations", 503, OleGetTextDecorations, OleSetTextDecorations, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupTextElement, "FontSize", 504, OleGetFontSize, OleSetFontSize, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupTextElement, "FontWeight", 505, OleGetFontWeight, OleSetFontWeight, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupTextElement, "FontFamily", 506, OleGetFontFamily, OleSetFontFamily, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPMarkupTextElement, "FontStyle", 507, OleGetFontStyle, OleSetFontStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupTextElement, "Background", 510, OleGetBackground, OleSetBackground, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupTextElement, "Foreground", 511, OleGetForeground, OleSetForeground, VT_DISPATCH)
END_DISPATCH_MAP()

// {10289842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupTextElement =
{ 0x10289842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupTextElement, CXTPMarkupFrameworkContentElement)
	INTERFACE_PART(CXTPMarkupTextElement, IID_IMarkupTextElement, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupTextElement, IID_IMarkupTextElement)

long CXTPMarkupTextElement::OleGetTextDecorations()
{
	return GetTextDecorations();
}

void CXTPMarkupTextElement::OleSetTextDecorations(long nValue)
{
	SetTextDecorations(nValue);
}

long CXTPMarkupTextElement::OleGetFontSize()
{
	return GetFontSize();
}

void CXTPMarkupTextElement::OleSetFontSize(long nValue)
{
	SetFontSize(nValue);
}

long CXTPMarkupTextElement::OleGetFontWeight()
{
	return GetFontWeight();
}

void CXTPMarkupTextElement::OleSetFontWeight(long nValue)
{
	SetFontWeight(nValue);
}

BSTR CXTPMarkupTextElement::OleGetFontFamily()
{
	return SysAllocString(GetFontFamily());
}

void CXTPMarkupTextElement::OleSetFontFamily(LPCTSTR lpszValue)
{
	SetFontFamily(XTP_CT2CW(lpszValue));
}

long CXTPMarkupTextElement::OleGetFontStyle()
{
	return GetFontStyle();
}

void CXTPMarkupTextElement::OleSetFontStyle(long nValue)
{
	SetFontStyle(nValue);
}

LPDISPATCH CXTPMarkupTextElement::OleGetBackground()
{
	return XTPGetDispatch(GetBackground());
}

void CXTPMarkupTextElement::OleSetBackground(LPDISPATCH lpBackground)
{
	SetBackground(MARKUP_STATICCAST(CXTPMarkupBrush, FromDispatch(lpBackground)));
}

LPDISPATCH CXTPMarkupTextElement::OleGetForeground()
{
	return XTPGetDispatch(GetForeground());

}

void CXTPMarkupTextElement::OleSetForeground(LPDISPATCH lpBackground)
{
	SetForeground(MARKUP_STATICCAST(CXTPMarkupBrush, FromDispatch(lpBackground)));
}


VARIANT CXTPMarkupTextElement::OleGetTag()
{
	VARIANT va;
	VariantInit(&va);
	va.vt = VT_NULL;

	CXTPMarkupObject* pTag = GetTag();
	if (!pTag)
		return va;

	if (IsStringObject(pTag))
	{
		va.vt = VT_BSTR;
		va.bstrVal = SysAllocString(*(CXTPMarkupString*)pTag);
	}
	else
	{
		va.vt = VT_DISPATCH;
		va.pdispVal = XTPGetDispatch(pTag);
	}
	return va;
}

void CXTPMarkupTextElement::OleSetTag(const VARIANT& va)
{
	if (va.vt == VT_BSTR)
	{
		SetTag(new CXTPMarkupString(va.bstrVal));
	}
}

/////////////////////////////////////////////////////////////////////////
// Inline


BEGIN_DISPATCH_MAP(CXTPMarkupInline, CXTPMarkupTextElement)
	DISP_PROPERTY_EX_ID(CXTPMarkupInline, "BaselineAlignment", 600, OleGetBaselineAlignment, OleSetBaselineAlignment, VT_I4)
END_DISPATCH_MAP()

// {10279842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupInline =
{ 0x10279842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupInline, CXTPMarkupTextElement)
	INTERFACE_PART(CXTPMarkupInline, IID_IMarkupInline, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupInline, IID_IMarkupInline)

int CXTPMarkupInline::OleGetBaselineAlignment()
{
	return GetBaselineAlignment();
}

void CXTPMarkupInline::OleSetBaselineAlignment(int nValue)
{
	SetBaselineAlignment((XTPMarkupBaselineAlignment)nValue);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupInlineCollection

BEGIN_DISPATCH_MAP(CXTPMarkupInlineCollection, CXTPMarkupObject)
	DISP_FUNCTION_ID(CXTPMarkupInlineCollection, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupInlineCollection, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPMarkupInlineCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupInlineCollection, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupInlineCollection, "Insert", 3, OleInsert, VT_EMPTY, VTS_I4 VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupInlineCollection, "Clear", 4, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupInlineCollection, "RemoveAt", 5, Remove, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// {10299842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupInlineCollection =
{ 0x10299842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupInlineCollection, CXTPMarkupObject)
	INTERFACE_PART(CXTPMarkupInlineCollection, IID_IMarkupInlineCollection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupInlineCollection, IID_IMarkupInlineCollection)

IMPLEMENT_ENUM_VARIANT(CXTPMarkupInlineCollection)

long CXTPMarkupInlineCollection::OleGetItemCount()
{
	return (long)GetCount();
}

LPDISPATCH CXTPMarkupInlineCollection::OleGetItem(long nIndex)
{
	return XTPGetDispatch(GetInline(nIndex));
}

void CXTPMarkupInlineCollection::OleAdd(LPDISPATCH lpElementDisp)
{
	if (!lpElementDisp)
		AfxThrowOleException(E_INVALIDARG);

	Add(MARKUP_STATICCAST(CXTPMarkupInline, FromDispatch(lpElementDisp)));
}

void CXTPMarkupInlineCollection::OleInsert(long Index, LPDISPATCH lpElementDisp)
{
	if (!lpElementDisp)
		AfxThrowOleException(E_INVALIDARG);

	Insert(Index, MARKUP_STATICCAST(CXTPMarkupInline, FromDispatch(lpElementDisp)));
}


/////////////////////////////////////////////////////////////////////////
// Span


BEGIN_DISPATCH_MAP(CXTPMarkupSpan, CXTPMarkupInline)
	DISP_PROPERTY_EX_ID(CXTPMarkupSpan, "Inlines", 700, OleGetInlines, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

// {10319842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupSpan =
{ 0x10319842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupSpan, CXTPMarkupInline)
	INTERFACE_PART(CXTPMarkupSpan, IID_IMarkupSpan, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupSpan, IID_IMarkupSpan)

LPDISPATCH CXTPMarkupSpan::OleGetInlines()
{
	return XTPGetDispatch(GetInlines());
}


/////////////////////////////////////////////////////////////////////////
// CXTPMarkupBold

BEGIN_DISPATCH_MAP(CXTPMarkupBold, CXTPMarkupSpan)
END_DISPATCH_MAP()

// {10329842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupBold =
{ 0x10329842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupBold, CXTPMarkupSpan)
	INTERFACE_PART(CXTPMarkupBold, IID_IMarkupBold, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupBold, IID_IMarkupBold)


/////////////////////////////////////////////////////////////////////////
// CXTPMarkupHyperlink

BEGIN_DISPATCH_MAP(CXTPMarkupHyperlink, CXTPMarkupSpan)
	DISP_PROPERTY_EX_ID(CXTPMarkupHyperlink, "ClickEvent", 800, OleGetClickEvent, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

// {10339842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupHyperlink =
{ 0x10339842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupHyperlink, CXTPMarkupSpan)
	INTERFACE_PART(CXTPMarkupHyperlink, IID_IMarkupHyperlink, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupHyperlink, IID_IMarkupHyperlink)

LPDISPATCH CXTPMarkupHyperlink::OleGetClickEvent()
{
	return XTPGetDispatch(m_pClickEvent);
}


/////////////////////////////////////////////////////////////////////////
// CXTPMarkupItalic

BEGIN_DISPATCH_MAP(CXTPMarkupItalic, CXTPMarkupSpan)
END_DISPATCH_MAP()

// {10349842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupItalic =
{ 0x10349842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupItalic, CXTPMarkupSpan)
	INTERFACE_PART(CXTPMarkupItalic, IID_IMarkupItalic, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupItalic, IID_IMarkupItalic)


/////////////////////////////////////////////////////////////////////////
// CXTPMarkupUnderline

BEGIN_DISPATCH_MAP(CXTPMarkupUnderline, CXTPMarkupSpan)
END_DISPATCH_MAP()

// {10359842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupUnderline =
{ 0x10359842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupUnderline, CXTPMarkupSpan)
	INTERFACE_PART(CXTPMarkupUnderline, IID_IMarkupUnderline, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupUnderline, IID_IMarkupUnderline)


/////////////////////////////////////////////////////////////////////////
// CXTPMarkupInlineUIContainer

BEGIN_DISPATCH_MAP(CXTPMarkupInlineUIContainer, CXTPMarkupInline)
	DISP_PROPERTY_EX_ID(CXTPMarkupInlineUIContainer, "Child", 700, OleGetChild, OleSetChild, VT_DISPATCH)
END_DISPATCH_MAP()

// {10369842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupInlineUIContainer =
{ 0x10369842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupInlineUIContainer, CXTPMarkupInline)
	INTERFACE_PART(CXTPMarkupInlineUIContainer, IID_IMarkupInlineUIContainer, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupInlineUIContainer, IID_IMarkupInlineUIContainer)

LPDISPATCH CXTPMarkupInlineUIContainer::OleGetChild()
{
	return XTPGetDispatch(GetChild());
}

void CXTPMarkupInlineUIContainer    ::OleSetChild(LPDISPATCH lpDisp)
{
	SetChild(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpDisp)));
}



#endif
