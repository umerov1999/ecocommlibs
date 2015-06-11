// XTPMarkupRun.h: interface for the CXTPMarkupRun class.
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
#if !defined(__XTPMARKUPRUN_H__)
#define __XTPMARKUPRUN_H__
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


class _XTP_EXT_CLASS CXTPMarkupRun : public CXTPMarkupInline
{
	DECLARE_MARKUPCLASS(CXTPMarkupRun)

protected:
	CXTPMarkupRun();
	virtual ~CXTPMarkupRun();

protected:
	virtual void OnFinalRelease();

public:
	CString GetText() const;
	LPCWSTR GetTextW() const;
	void SetText(LPCWSTR lpszText);
	void SetText(CXTPMarkupString* pText);

public:
	virtual POSITION GetContentStartPosition() const;
	virtual void GetContentNextPosition(POSITION& pos) const;
	virtual BOOL IsWordBreakPosition(POSITION pos) const;
	virtual BOOL IsLineBreakPosition(POSITION pos) const;
	virtual BOOL IsCaretReturnPosition(POSITION pos) const;
	virtual BOOL IsWhiteSpacePosition(POSITION pos) const;
	virtual CSize Measure(CXTPMarkupDrawingContext* pDC, POSITION posStart, POSITION posEnd);
	virtual void Render(CXTPMarkupDrawingContext* pDC, CRect rc, POSITION posStart, POSITION posEnd);

protected:
	virtual void GetLogFont(LOGFONT *lf, CXTPMarkupRenderTransform *pRenderTransform) const;

protected:
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual BOOL HasContentObject() const;
	virtual void PrepareMeasure(CXTPMarkupDrawingContext *pDC, CXTPMarkupRenderTransform *pRenderTransform);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupRun);

	afx_msg VARIANT OleGetText();
	afx_msg void OleSetText(const VARIANT& lpCaption);


//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	int m_nBaseline;
	virtual int GetBaseline() const;

	CXTPMarkupFont* m_pFont;

public:
	static CXTPMarkupDependencyProperty* m_pTextProperty;
};


class _XTP_EXT_CLASS CXTPMarkupLineBreak : public CXTPMarkupRun
{
	DECLARE_MARKUPCLASS(CXTPMarkupLineBreak);

protected:
	CXTPMarkupLineBreak();
};


#endif // !defined(__XTPMARKUPRUN_H__)
