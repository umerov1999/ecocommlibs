// XTPMarkupSpan.h: interface for the CXTPMarkupSpan class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPMARKUPSPAN_H__)
#define __XTPMARKUPSPAN_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupInlineCollection;
class CXTPMarkupDrawingContext;
class CXTPMarkupBrush;
class CXTPMarkupUIElement;
class CXTPMarkupBuilder;
class CXTPMarkupTextBlock;
class CXTPMarkupFont;
class CXTPMarkupFrameworkElement;
class CXTPMarkupStyle;


class _XTP_EXT_CLASS CXTPMarkupSpan : public CXTPMarkupInline
{
	DECLARE_MARKUPCLASS(CXTPMarkupSpan)

protected:
	CXTPMarkupSpan();
	~CXTPMarkupSpan();

public:
	CXTPMarkupInlineCollection* GetInlines() const;

public:
	virtual CXTPMarkupInline* GetFirstInline() const;

protected:
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual BOOL HasContentObject() const;
	virtual BOOL AllowWhiteSpaceContent() const;
	virtual int GetLogicalChildrenCount() const;
	virtual CXTPMarkupObject* GetLogicalChild(int nIndex) const;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupSpan);

	afx_msg LPDISPATCH OleGetInlines();

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPMarkupInlineCollection* m_pInlines;
};

AFX_INLINE int CXTPMarkupSpan::GetLogicalChildrenCount() const
{
	return m_pInlines ? 1 : 0;
}

AFX_INLINE CXTPMarkupObject* CXTPMarkupSpan::GetLogicalChild(int nIndex) const
{
	return nIndex == 0 ? (CXTPMarkupObject*)m_pInlines : NULL;
}

AFX_INLINE CXTPMarkupInlineCollection* CXTPMarkupSpan::GetInlines() const
{
	return m_pInlines;
}


class _XTP_EXT_CLASS CXTPMarkupBold : public CXTPMarkupSpan
{
	DECLARE_MARKUPCLASS(CXTPMarkupBold)

protected:
	CXTPMarkupBold();

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupBold);
//}}AFX_CODEJOCK_PRIVATE
#endif
};


class _XTP_EXT_CLASS CXTPMarkupItalic : public CXTPMarkupSpan
{
	DECLARE_MARKUPCLASS(CXTPMarkupItalic)

protected:
	CXTPMarkupItalic();


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupItalic);
//}}AFX_CODEJOCK_PRIVATE
#endif
};


class _XTP_EXT_CLASS CXTPMarkupUnderline  : public CXTPMarkupSpan
{
	DECLARE_MARKUPCLASS(CXTPMarkupUnderline)

protected:
	CXTPMarkupUnderline();

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupUnderline);
//}}AFX_CODEJOCK_PRIVATE
#endif
};


class _XTP_EXT_CLASS CXTPMarkupHyperlink : public CXTPMarkupSpan
{
	DECLARE_MARKUPCLASS(CXTPMarkupHyperlink)

protected:
	CXTPMarkupHyperlink();

protected:
	virtual void OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* e);
	virtual void OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* e);

protected:
	virtual void OnClick();

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupHyperlink);
	afx_msg LPDISPATCH OleGetClickEvent();

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	BOOL m_bPressed;

public:
	static CXTPMarkupRoutedEvent* m_pClickEvent;
};


#endif // !defined(__XTPMARKUPSPAN_H__)
