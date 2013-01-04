// XTPMarkupRun.cpp: implementation of the CXTPMarkupRun class.
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
// CXTPMarkupRun

CXTPMarkupDependencyProperty* CXTPMarkupRun::m_pTextProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Run", CXTPMarkupRun, CXTPMarkupInline);

void CXTPMarkupRun::RegisterMarkupClass()
{
	m_pTextProperty = CXTPMarkupDependencyProperty::Register(L"Text", MARKUP_TYPE(CXTPMarkupString), MARKUP_TYPE(CXTPMarkupRun),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));
}


CXTPMarkupRun::CXTPMarkupRun()
{
	m_nBaseline = 0;
	m_pFont = NULL;
}

CXTPMarkupRun::~CXTPMarkupRun()
{
	MARKUP_RELEASE(m_pFont);
}

void CXTPMarkupRun::OnFinalRelease()
{
	MARKUP_RELEASE(m_pFont);

	CXTPMarkupInline::OnFinalRelease();
}

void CXTPMarkupRun::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	if (IsStringObject(pContent))
	{
		SetValue(m_pTextProperty, pContent);
	}
	else
	{
		CXTPMarkupInline::SetContentObject(pBuilder, pContent);
	}
}

BOOL CXTPMarkupRun::HasContentObject() const
{
	return GetValue(m_pTextProperty) != NULL;
}

int CXTPMarkupRun::GetBaseline() const
{
	return m_nBaseline;
}

CString CXTPMarkupRun::GetText() const
{
	CXTPMarkupString* pString = MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pTextProperty));
	if (!pString)
		return _T("");

	return CString((LPCWSTR)*pString);
}

LPCWSTR CXTPMarkupRun::GetTextW() const
{
	CXTPMarkupString* pString = MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pTextProperty));
	if (!pString)
		return L"";

	return (LPCWSTR)*pString;
}


void CXTPMarkupRun::SetText(LPCWSTR lpszText)
{
	SetValue(m_pTextProperty, CXTPMarkupString::CreateValue(lpszText));
}

void CXTPMarkupRun::SetText(CXTPMarkupString* pText)
{
	SetValue(m_pTextProperty, pText);
}

POSITION CXTPMarkupRun::GetContentStartPosition() const
{
	CXTPMarkupString* pString = MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pTextProperty));
	if (!pString)
		return MARKUP_POSITION_EOF;

	LPCWSTR lpszText = (LPCWSTR)(*pString);

	if ((!lpszText) || (*lpszText == '\0'))
		return MARKUP_POSITION_EOF;

	return (POSITION)lpszText;
}

void CXTPMarkupRun::GetContentNextPosition(POSITION& pos) const
{
	if (!pos || pos == MARKUP_POSITION_EOF)
		return;

	LPCWSTR lpszText = (LPCWSTR)pos;
	if (*lpszText == '\0')
	{
		pos = MARKUP_POSITION_EOF;
	}
	else
	{
		lpszText++;
		pos = (POSITION)lpszText;
	}
}

BOOL CXTPMarkupRun::IsWordBreakPosition(POSITION pos) const
{
	if (!pos)
		return FALSE;
	LPCWSTR lpszText = (LPCWSTR)pos;

	return *lpszText == _T(' ') || *lpszText == _T('\t') || *lpszText == _T('\n');
}

BOOL CXTPMarkupRun::IsWhiteSpacePosition(POSITION pos) const
{
	if (!pos)
		return FALSE;

	LPCWSTR lpszText = (LPCWSTR)pos;

	return *lpszText == _T(' ') || *lpszText == _T('\t');
}

BOOL CXTPMarkupRun::IsLineBreakPosition(POSITION pos) const
{
	if (!pos)
		return FALSE;
	LPCWSTR lpszText = (LPCWSTR)pos;

	return *lpszText == _T('\n');
}

BOOL CXTPMarkupRun::IsCaretReturnPosition(POSITION pos) const
{
	if (!pos)
		return FALSE;
	LPCWSTR lpszText = (LPCWSTR)pos;

	return *lpszText == _T('\r');
}

void CXTPMarkupRun::GetLogFont(LOGFONT *pLogFont, CXTPMarkupRenderTransform *pRenderTransform) const
{
	memcpy(pLogFont, GetMarkupContext()->GetDefaultLogFont(), sizeof(LOGFONT));

	CXTPMarkupInt* pFontSize = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pFontSizeProperty));
	if (pFontSize)
	{
		pLogFont->lfHeight = - *pFontSize;
	}

	CXTPMarkupEnum* pFontWeight = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pFontWeightProperty));
	if (pFontWeight)
	{
		pLogFont->lfWeight = *pFontWeight;
	}

	CXTPMarkupEnum* pFontStyle = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pFontStyleProperty));
	if (pFontStyle)
	{
		pLogFont->lfItalic = *pFontStyle ? (BYTE)1 : (BYTE)0;
	}

	CXTPMarkupEnum* pFontQuality = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pFontQualityProperty));
	if (pFontQuality && XTPSystemVersion()->IsClearTypeTextQualitySupported())
	{
		pLogFont->lfQuality = (BYTE)*pFontQuality;
	}

	CXTPMarkupInt* pFontCharset = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pFontCharsetProperty));
	if (pFontCharset)
	{
		pLogFont->lfCharSet = (BYTE)*pFontCharset;
	}

	if (NULL != pRenderTransform)
	{
		CXTPMarkupRotateTransform *pRotateTransform = pRenderTransform->GetRotateTransform();

		if (NULL != pRotateTransform)
		{
			pLogFont->lfEscapement  = 3600 - pRotateTransform->GetAngle() * 10;
			pLogFont->lfOrientation = 3600 - pRotateTransform->GetAngle() * 10;
		}
	}
	else
	{
		pLogFont->lfEscapement  = 0;
		pLogFont->lfOrientation = 0;
	}

	CXTPMarkupString* pFontFamily = MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pFontFamilyProperty));
	if (pFontFamily)
	{
		LPCWSTR lpszFaceName = *pFontFamily;
		LPTSTR lpszLogFontFaceName = pLogFont->lfFaceName;

		while ((*lpszLogFontFaceName++ = (TCHAR)*(lpszFaceName++)) != 0);
	}
}

void CXTPMarkupRun::PrepareMeasure(CXTPMarkupDrawingContext* pDC, CXTPMarkupRenderTransform *pRenderTransform)
{
	LOGFONT lf;
	GetLogFont(&lf, pRenderTransform);

	if (m_pFont == NULL || !CXTPMarkupContext::CompareFont(&m_pFont->m_lf, &lf))
	{
		CXTPMarkupFont* pFont = GetMarkupContext()->GetFont(&lf);
		MARKUP_RELEASE(m_pFont);

		m_pFont = pFont;
	}

	pDC->SetFont(m_pFont);

	TEXTMETRIC tm;

	HDC hDC = pDC->GetDC();
	GetTextMetrics(hDC, &tm);
	pDC->ReleaseDC(hDC);

	m_nBaseline = tm.tmDescent;
}

CSize CXTPMarkupRun::Measure(CXTPMarkupDrawingContext* pDC, POSITION posStart, POSITION posEnd)
{
	if (posStart == NULL || posEnd == NULL || posEnd == MARKUP_POSITION_EOF)
		return CSize(0, 0);

	pDC->SetFont(m_pFont);

	if (posEnd == posStart)
	{
		SIZE sz = pDC->GetTextExtent(L" ", 1);
		return CSize(0, sz.cy);
	}

	LPCWSTR lpszText = (LPCWSTR)posStart;
	int nCount = int(posEnd - posStart) / sizeof(WCHAR);

	SIZE sz = pDC->GetTextExtent(lpszText, nCount);

	return sz;
}

void CXTPMarkupRun::Render(CXTPMarkupDrawingContext* pDC, CRect rc, POSITION posStart, POSITION posEnd)
{
	if (posStart == NULL || posEnd == NULL || posEnd == posStart || posEnd == MARKUP_POSITION_EOF)
		return;

	LPCWSTR lpszText = (LPCWSTR)posStart;
	UINT nCount = UINT(posEnd - posStart) / sizeof(WCHAR);

	pDC->SetFont(m_pFont);
	pDC->DrawTextLine(lpszText, nCount, rc);
}


#ifdef _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////
// Run


BEGIN_DISPATCH_MAP(CXTPMarkupRun, CXTPMarkupInline)
	DISP_PROPERTY_EX_ID(CXTPMarkupRun, "Text", 700, OleGetText, OleSetText, VT_VARIANT)
END_DISPATCH_MAP()

// {10309842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupRun =
{ 0x10309842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupRun, CXTPMarkupInline)
	INTERFACE_PART(CXTPMarkupRun, IID_IMarkupRun, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupRun, IID_IMarkupRun)

VARIANT CXTPMarkupRun::OleGetText()
{
	VARIANT v;
	v.vt = VT_BSTR;
	v.bstrVal = ::SysAllocString(GetTextW());

	return v;
}


void CXTPMarkupRun::OleSetText(const VARIANT& lpCaption)
{
	VARIANT varCaption;
	VariantInit(&varCaption);
	VariantChangeType(&varCaption, (VARIANTARG*)&lpCaption, 0, VT_BSTR);

	if (varCaption.bstrVal != NULL)
	{
		SetText(varCaption.bstrVal);
	}
	else
	{
		SetText(L"");
	}

	VariantClear(&varCaption);
}

#endif
