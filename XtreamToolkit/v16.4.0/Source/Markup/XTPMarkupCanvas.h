// XTPMarkupCanvas.h: interface for the CXTPMarkupCanvas class.
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
#if !defined(__XTPMARKUPCANVAS_H__)
#define __XTPMARKUPCANVAS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class _XTP_EXT_CLASS CXTPMarkupCanvas : public CXTPMarkupPanel
{
	DECLARE_MARKUPCLASS(CXTPMarkupCanvas)
protected:
	CXTPMarkupCanvas();
	virtual ~CXTPMarkupCanvas();

public:
	static void AFX_CDECL SetLeft(CXTPMarkupUIElement* pElement, int nLeft);
	static void AFX_CDECL SetTop(CXTPMarkupUIElement* pElement, int nTop);
	static void AFX_CDECL SetRight(CXTPMarkupUIElement* pElement, int nRight);
	static void AFX_CDECL SetBottom(CXTPMarkupUIElement* pElement, int nBottom);

	static int AFX_CDECL GetLeft(CXTPMarkupUIElement* pElement);
	static int AFX_CDECL GetTop(CXTPMarkupUIElement* pElement);
	static int AFX_CDECL GetRight(CXTPMarkupUIElement* pElement);
	static int AFX_CDECL GetBottom(CXTPMarkupUIElement* pElement);

protected:
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize availableSize);
	virtual CSize ArrangeOverride(CSize arrangeSize);


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupTextBlock);

	afx_msg void OleSetLeft(LPDISPATCH lpElementDisp, int nValue);
	afx_msg void OleSetRight(LPDISPATCH lpElementDisp, int nValue);
	afx_msg void OleSetTop(LPDISPATCH lpElementDisp, int nValue);
	afx_msg void OleSetBottom(LPDISPATCH lpElementDisp, int nValue);
	afx_msg int OleGetLeft(LPDISPATCH lpElementDisp);
	afx_msg int OleGetTop(LPDISPATCH lpElementDisp);
	afx_msg int OleGetRight(LPDISPATCH lpElementDisp);
	afx_msg int OleGetBottom(LPDISPATCH lpElementDisp);
//}}AFX_CODEJOCK_PRIVATE
#endif


public:
	static CXTPMarkupDependencyProperty* m_pTopProperty;
	static CXTPMarkupDependencyProperty* m_pLeftProperty;
	static CXTPMarkupDependencyProperty* m_pRightProperty;
	static CXTPMarkupDependencyProperty* m_pBottomProperty;

};

#endif // !defined(__XTPMARKUPCANVAS_H__)
