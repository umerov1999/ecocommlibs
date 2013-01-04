// XTPMarkupStatic.h : header file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__MARKUPSTATIC_H__)
#define __MARKUPSTATIC_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _XTP_INCLUDE_MARKUP

class CXTPMarkupUIElement;
class CXTPMarkupContext;

// ----------------------------------------------------------------------
// Summary:
// ----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPMarkupStatic : public CStatic
{
public:
	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	CXTPMarkupStatic();

	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	virtual ~CXTPMarkupStatic();

public:
	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	void SetMarkupText(LPCTSTR lpszMarkup);

	// ----------------------------------------------------------------------
	// Summary: Special version used by the Ribbon Cusomize dialog and will
	//          handle % to load a string from a resouce
	// ----------------------------------------------------------------------
	void SetMarkupTextEx(LPCTSTR lpszMarkup);

protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPMarkupStatic)
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPMarkupStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	CXTPMarkupUIElement* m_pUIElement; //
	CXTPMarkupContext* m_pMarkupContext;
};

#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MARKUPSTATIC_H__)
