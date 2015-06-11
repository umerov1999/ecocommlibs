// XTPMarkupRender.cpp: implementation.
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

#ifdef _XTP_INCLUDE_MARKUP

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupFrameworkElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupBuilder.h"
#include "Markup/XTPMarkupParser.h"

#include "XTPMarkupRender.h"


CXTPMarkupContext* AFX_CDECL XTPMarkupCreateContext(HWND hWnd /*= NULL*/)
{
	CXTPMarkupContext* pContext = new CXTPMarkupContext();
	pContext->m_hContextWnd = hWnd;
	return pContext;
}

void AFX_CDECL XTPMarkupAssignHandle(CXTPMarkupContext* pContext, HWND hWnd)
{
	if (pContext) pContext->m_hContextWnd = hWnd;
}

void AFX_CDECL XTPMarkupReleaseContext(CXTPMarkupContext*& pContext)
{
	if (pContext)
	{
		pContext->Release();
		pContext = NULL;
	}
}

void AFX_CDECL XTPMarkupReleaseElement(CXTPMarkupUIElement*& pElement)
{
	if (pElement)
	{
		pElement->Release();
		pElement = NULL;
	}
}

CXTPMarkupUIElement* AFX_CDECL XTPMarkupParseText(CXTPMarkupContext* pContext, LPCSTR lpszMarkup)
{
	if (NULL == pContext || NULL == lpszMarkup)
		return NULL;

	CXTPMarkupParser sc;
	sc.SetBuffer(lpszMarkup, lpszMarkup + strlen(lpszMarkup));

	CXTPMarkupBuilder builder(pContext);
	return builder.Parse(&sc);
}

CXTPMarkupUIElement* AFX_CDECL XTPMarkupParseText(CXTPMarkupContext* pContext, LPCWSTR lpszMarkup)
{
	if (NULL == pContext || NULL == lpszMarkup)
		return NULL;

	CXTPMarkupParser sc;
	sc.SetBuffer(lpszMarkup, lpszMarkup + wcslen(lpszMarkup));

	CXTPMarkupBuilder builder(pContext);
	return builder.Parse(&sc);
}

CXTPMarkupContext* AFX_CDECL XTPMarkupElementContext(CXTPMarkupUIElement* pElement) {
	return pElement ? pElement->GetMarkupContext() : NULL;
}

CSize AFX_CDECL XTPMarkupMeasureElement(CXTPMarkupUIElement* pElement, int cxAvail /*= INT_MAX*/, int cyAvail /*= INT_MAX*/)
{
	if (!pElement)
		return CSize(0, 0);

	CXTPMarkupDrawingContext dc(pElement->GetMarkupContext());
	pElement->Measure(&dc, CSize(cxAvail, cyAvail));

	return pElement->GetDesiredSize();
}

void AFX_CDECL XTPMarkupRenderElement(CXTPMarkupUIElement* pElement, HDC hDC, LPCRECT lpRect)
{
	if (!pElement)
		return;

	if (!hDC || !lpRect)
		return;

	CRect rc(lpRect);

	CXTPMarkupDrawingContext dc(pElement->GetMarkupContext(), hDC);
	pElement->Measure(&dc, rc.Size());
	pElement->Arrange(rc);
	pElement->Render(&dc);
}

void AFX_CDECL XTPMarkupSetDefaultFont(CXTPMarkupContext* pContext, HFONT hFont, COLORREF clrText)
{
	if (!pContext)
		return;

	if (hFont)
	{
		pContext->SetDefaultFont(hFont);
	}

	if (clrText != (COLORREF)-1)
	{
		pContext->SetDefaultTextColor(clrText);
	}
}

BOOL AFX_CDECL XTPMarkupRelayMessage(CXTPMarkupUIElement* pElement, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (pElement && pElement->GetMarkupContext())
		return pElement->GetMarkupContext()->OnWndMsg(pElement, message, wParam, lParam, pResult);
	return FALSE;
}

BOOL AFX_CDECL XTPMarkupRelayMessage(CXTPMarkupContext* pContext, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (!pContext)
		return FALSE;
	return pContext->OnWndMsg(NULL, message, wParam, lParam, pResult);
}

void AFX_CDECL XTPMarkupDrawText(CXTPMarkupContext* pContext, HDC hDC, LPCTSTR lpszText, LPRECT lpRect, UINT nFormat)
{
	CXTPMarkupUIElement* pElement = pContext ? XTPMarkupParseText(pContext, lpszText) : NULL;

	if (pElement)
	{
		XTPMarkupSetDefaultFont(pContext, (HFONT)::GetCurrentObject(hDC, OBJ_FONT), ::GetTextColor(hDC));

		CSize sz = XTPMarkupMeasureElement(pElement, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);

		int nLeft = lpRect->left;
		int nTop = lpRect->top;

		if (nFormat & DT_CENTER)
			nLeft = (lpRect->right + lpRect->left - sz.cx) / 2;
		else if (nFormat & DT_RIGHT)
			nLeft = lpRect->right - sz.cx;

		if (nFormat & DT_VCENTER)
			nTop = (lpRect->top + lpRect->bottom - sz.cy) / 2;
		else if (nFormat & DT_BOTTOM)
			nTop = lpRect->bottom - sz.cy;

		XTPMarkupRenderElement(pElement, hDC, CRect(nLeft, nTop, nLeft + sz.cx, nTop + sz.cy));


		XTPMarkupReleaseElement(pElement);
	}
	else
	{
		::DrawText(hDC, lpszText, (int)_tcslen(lpszText), lpRect, nFormat);
	}
}


#endif
