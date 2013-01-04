// XTPMarkupPanel.cpp: implementation of the CXTPMarkupPanel class.
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
#include "XTPMarkupPanel.h"
#include "XTPMarkupDrawingContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupUIElementCollection

CXTPMarkupUIElementCollection::CXTPMarkupUIElementCollection()
{
	m_pElementType = MARKUP_TYPE(CXTPMarkupUIElement);
}

CXTPMarkupUIElementCollection::~CXTPMarkupUIElementCollection()
{
}

void CXTPMarkupUIElementCollection::OnChanged()
{
	CXTPMarkupUIElement* pPanel = MARKUP_DYNAMICCAST(CXTPMarkupUIElement, GetLogicalParent());
	if (pPanel)
	{
		pPanel->InvalidateMeasure();
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CXTPMarkupDependencyProperty* CXTPMarkupPanel::m_pBackgroundProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Panel", CXTPMarkupPanel, CXTPMarkupFrameworkElement)

void CXTPMarkupPanel::RegisterMarkupClass()
{
	m_pBackgroundProperty = CXTPMarkupDependencyProperty::Register(L"Background", MARKUP_TYPE(CXTPMarkupBrush), MARKUP_TYPE(CXTPMarkupPanel),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));
}

CXTPMarkupPanel::CXTPMarkupPanel()
{
	m_pChildren = new CXTPMarkupUIElementCollection();
	m_pChildren->SetLogicalParent(this);
}

CXTPMarkupPanel::~CXTPMarkupPanel()
{
	if (m_pChildren)
	{
		m_pChildren->SetLogicalParent(NULL);
		m_pChildren->Release();
	}
}

void CXTPMarkupPanel::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	m_pChildren->SetContentObject(pBuilder, pContent);
}

BOOL CXTPMarkupPanel::HasContentObject() const
{
	return m_pChildren->HasContentObject();
}

CXTPMarkupBrush* CXTPMarkupPanel::GetBackground() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pBackgroundProperty));
}

void CXTPMarkupPanel::SetBackground(CXTPMarkupBrush* brush)
{
	SetValue(m_pBackgroundProperty, brush);
}

void CXTPMarkupPanel::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CSize szRender = GetRenderSize();
	if (szRender.cx <= 0 || szRender.cy <= 0)
		return;

	CXTPMarkupBrush* pBackground = GetBackground();

	if (pBackground)
	{
		pDC->FillRectangle(CRect(0, 0, szRender.cx, szRender.cy), pBackground);
	}
}

CXTPMarkupInputElement* CXTPMarkupPanel::InputHitTestOverride(CPoint /*point*/) const
{
	return GetBackground() != NULL ? (CXTPMarkupInputElement*)this : NULL;
}



