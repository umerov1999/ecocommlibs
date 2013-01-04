// MarkupListCtrl.h : header file
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
#if !defined(__MARKUPLISTBOX_H__)
#define __MARKUPLISTBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _XTP_INCLUDE_MARKUP

class CXTPMarkupUIElement;

// ----------------------------------------------------------------------
// Summary:
// ----------------------------------------------------------------------
struct _XTP_EXT_CLASS XTP_MARKUP_LISTITEM
{
public:
	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	XTP_MARKUP_LISTITEM(CXTPMarkupContext* pContext);

	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	virtual ~XTP_MARKUP_LISTITEM();

public:
	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	void Resolve(LPCTSTR lpszMarkup);

	CString strMarkup; //
	CXTPMarkupUIElement* pItem; //
	CXTPMarkupContext* pContext; //
};

// ----------------------------------------------------------------------
// Summary:
// ----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPMarkupListBox : public CXTPListBox, public CXTPMarkupContext
{
public:
	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	CXTPMarkupListBox();

	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	virtual ~CXTPMarkupListBox();

	//{{AFX_VIRTUAL(CXTPMarkupListBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_VIRTUAL

public:
	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	void Init();

	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	void ItemChanged(XTP_MARKUP_LISTITEM* pItem);

	// ----------------------------------------------------------------------
	// Summary:
	// ----------------------------------------------------------------------
	void AddMarkupString(LPCTSTR lpszMarkup);

protected:
	//{{AFX_MSG(CXTPMarkupListBox)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__MARKUPLISTBOX_H__)
