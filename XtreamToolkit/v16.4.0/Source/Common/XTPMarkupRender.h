// XTPMarkupRender.h: API interface for the markup.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPMARKUPRENDER_H__)
#define __XTPMARKUPRENDER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupContext;
class CXTPMarkupUIElement;

const UINT XTP_WM_MARKUPREDRAW = (WM_USER + 9820);

//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupCreateContext is part of Markup API that creates new CXTPMarkupContext object
// Parameters:
//     hWnd - Window handle to link context with
// Remarks:
//     XTPMarkupCreateContext creates new CXTPMarkupContext that holds common properties of Markup Elements like default font, default text color, current selected element, etc.
// Example:
// <code>
//     CXTPMarkupContext* pContext = XTPMarkupCreateContext();
//     CXTPMarkupUIElement* pElement = XTPMarkupParseText(pContext, "<Ellipse Fill='Red' Width='100' Height='100'/>");
//     CSize sz = XTPMarkupMeasureElement(pElement);
//     XTPMarkupRenderElement(pElement, pDC->GetSafeHdc(), CRect(CPoint(0), sz));
//     XTPMarkupReleaseElement(pElement);
//     XTPMarkupReleaseContext(pContext);
// </code>
// Returns:
//     Returns new CXTPMarkupContext object pointer
// See Also: XTPMarkupReleaseContext, XTPMarkupParseText, XTPMarkupReleaseElement
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CXTPMarkupContext* AFX_CDECL XTPMarkupCreateContext(HWND hWnd = NULL);
_XTP_EXT_CLASS void AFX_CDECL XTPMarkupAssignHandle(CXTPMarkupContext* pContext, HWND hWnd);


//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupReleaseContext is part of Markup API that destroys CXTPMarkupContext object
// Parameters:
//     pContext - Pointer to CXTPMarkupContext that will be released
// Example:
//     See Example for XTPMarkupCreateContext function
// See Also: XTPMarkupCreateContext
//-----------------------------------------------------------------------
_XTP_EXT_CLASS void AFX_CDECL XTPMarkupReleaseContext(CXTPMarkupContext*& pContext);

//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupParseText is part of Markup API that creates new Markup elements using Markup string
// Parameters:
//     pContext   - Pointer to CXTPMarkupContext. All new elements will be part of this context.
//     lpszMarkup - String to parse
// Example:
//     See Example for XTPMarkupCreateContext function
// See Also: XTPMarkupCreateContext, XTPMarkupReleaseElement, XTPMarkupMeasureElement, XTPMarkupRenderElement
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CXTPMarkupUIElement* AFX_CDECL XTPMarkupParseText(CXTPMarkupContext* pContext, LPCSTR lpszMarkup);
_XTP_EXT_CLASS CXTPMarkupUIElement* AFX_CDECL XTPMarkupParseText(CXTPMarkupContext* pContext, LPCWSTR lpszMarkup);

//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupReleaseElement is part of Markup API that destroys CXTPMarkupUIElement object
// Parameters:
//     pElement   - Pointer to CXTPMarkupUIElement that will be released
// Example:
//     See Example for XTPMarkupCreateContext function
// See Also: XTPMarkupCreateContext, XTPMarkupParseText, XTPMarkupMeasureElement, XTPMarkupRenderElement
//-----------------------------------------------------------------------
_XTP_EXT_CLASS void AFX_CDECL XTPMarkupReleaseElement(CXTPMarkupUIElement*& pElement);

//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupMeasureElement is part of Markup API that calculates dimensions of Markup element
// Parameters:
//     pElement   - Pointer to CXTPMarkupUIElement that size need to calculate
//     cxAvail    - Available width
//     cyAvail    - Available height
// Example:
//     See Example for XTPMarkupCreateContext function
// Returns:
//     Returns the dimensions of the CXTPMarkupUIElement
// See Also: XTPMarkupCreateContext, XTPMarkupParseText, XTPMarkupRenderElement
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CSize AFX_CDECL XTPMarkupMeasureElement(CXTPMarkupUIElement* pElement, int cxAvail = INT_MAX, int cyAvail = INT_MAX);

//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupRenderElement is part of Markup API that draws Markup element to device context
// Parameters:
//     pElement   - CXTPMarkupUIElement object to draw
//     hDC        - Handle to the device context
//     rc         - Pointer to a RECT structure that contains the rectangle in which the element is to be formatted
// Example:
//     See Example for XTPMarkupCreateContext function
// See Also: XTPMarkupCreateContext, XTPMarkupParseText, XTPMarkupMeasureElement
//-----------------------------------------------------------------------
_XTP_EXT_CLASS void AFX_CDECL XTPMarkupRenderElement(CXTPMarkupUIElement* pElement, HDC hDC, LPCRECT rc);

//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupSetDefaultFont is part of Markup API. Call it to set default font and text color for Markup elements
// Parameters:
//     hFont   - Handle to default font
//     clrText - Default text color
// Example:
// <code>
//     CXTPMarkupContext* pContext = XTPMarkupCreateContext();
//     CXTPMarkupUIElement* pElement = XTPMarkupParseText(pContext, "<TextBlock Text='Hello, world'/>");
//     XTPMarkupSetDefaultFont(pContext, 0, RGB(255, 0, 0));
//     CSize sz = XTPMarkupMeasureElement(pElement);
//     XTPMarkupRenderElement(pElement, pDC->GetSafeHdc(), CRect(CPoint(0), sz));
//     XTPMarkupReleaseElement(pElement);
//     XTPMarkupReleaseContext(pContext);
// </code>
// See Also: XTPMarkupCreateContext, XTPMarkupParseText, XTPMarkupMeasureElement
//-----------------------------------------------------------------------
_XTP_EXT_CLASS void AFX_CDECL XTPMarkupSetDefaultFont(CXTPMarkupContext* pContext, HFONT hFont, COLORREF clrText);

//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupRelayMessage is part of Markup API. It used to notify Markup element about events of associated window.
// Parameters:
//     pElement  - Pointer to Markup element that need to notify
//     message   - Specifies the message to be sent.
//     wParam    - Specifies additional message-dependent information
//     lParam    - Specifies additional message-dependent information
//     pResult   - The return value of WindowProc. Depends on the message
// Returns:
//     Nonzero if message was handled; otherwise 0.
// See Also: XTPMarkupCreateContext
//-----------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL XTPMarkupRelayMessage(CXTPMarkupUIElement* pElement, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
_XTP_EXT_CLASS BOOL AFX_CDECL XTPMarkupRelayMessage(CXTPMarkupContext* pContext, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);


//-----------------------------------------------------------------------
// Summary:
//     XTPMarkupElementContext is part of Markup API that returns Markup contrext of Markup element
// Parameters:
//     pElement  - Pointer to Markup element that context need to know
// Returns:
//     Pointer of associated CXTPMarkupContext
// See Also: XTPMarkupCreateContext
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CXTPMarkupContext* AFX_CDECL XTPMarkupElementContext(CXTPMarkupUIElement* pElement);

_XTP_EXT_CLASS void AFX_CDECL XTPMarkupDrawText(CXTPMarkupContext* pContext, HDC hDC, LPCTSTR lpszText, LPRECT lpRect, UINT nFormat);


//{{AFX_CODEJOCK_PRIVATE
#ifndef _XTP_INCLUDE_MARKUP

AFX_INLINE CXTPMarkupContext* AFX_CDECL XTPMarkupCreateContext(HWND /*hWnd*/) {
	return NULL;
}
AFX_INLINE void AFX_CDECL XTPMarkupReleaseContext(CXTPMarkupContext*& /*pContext*/) {
}
AFX_INLINE void AFX_CDECL XTPMarkupReleaseElement(CXTPMarkupUIElement*& /*pElement*/) {
}
AFX_INLINE CXTPMarkupUIElement* AFX_CDECL XTPMarkupParseText(CXTPMarkupContext* /*pContext*/, LPCSTR /*lpszMarkup*/) {
	return NULL;
}
AFX_INLINE CXTPMarkupUIElement* AFX_CDECL XTPMarkupParseText(CXTPMarkupContext* /*pContext*/, LPCWSTR /*lpszMarkup*/) {
	return NULL;
}
AFX_INLINE CXTPMarkupContext* AFX_CDECL XTPMarkupElementContext(CXTPMarkupUIElement* /*pElement*/) {
	return NULL;
}
AFX_INLINE CSize AFX_CDECL XTPMarkupMeasureElement(CXTPMarkupUIElement* /*pElement*/, int /*cxAvail = INT_MAX*/, int /*cyAvail = INT_MAX*/) {
	return CSize(0, 0);
}
AFX_INLINE void AFX_CDECL XTPMarkupRenderElement(CXTPMarkupUIElement* /*pElement*/, HDC /*hDC*/, LPCRECT /*lpRect*/) {
}
AFX_INLINE void AFX_CDECL XTPMarkupSetDefaultFont(CXTPMarkupContext* /*pContext*/, HFONT /*hFont*/, COLORREF /*clrText*/) {
}
AFX_INLINE BOOL AFX_CDECL XTPMarkupRelayMessage(CXTPMarkupUIElement* /*pElement*/, UINT /*message*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT* /*pResult*/) {
	return FALSE;
}
AFX_INLINE BOOL AFX_CDECL XTPMarkupRelayMessage(CXTPMarkupContext* /*pContext*/, UINT /*message*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT* /*pResult*/) {
	return FALSE;
}
AFX_INLINE void AFX_CDECL XTPMarkupAssignHandle(CXTPMarkupContext* /*pContext*/, HWND /*hWnd*/) {

}
AFX_INLINE  void AFX_CDECL XTPMarkupDrawText(CXTPMarkupContext* /*pContext*/, HDC hDC, LPCTSTR lpszText, LPRECT lpRect, UINT nFormat) {
	::DrawText(hDC, lpszText, (int) _tcslen(lpszText), lpRect, nFormat);
}

#endif // _XTP_INCLUDE_MARKUP
//}}AFX_CODEJOCK_PRIVATE


#endif // !defined(__XTPMARKUPRENDER_H__)
