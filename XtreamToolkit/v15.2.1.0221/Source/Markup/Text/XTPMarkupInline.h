// XTPMarkupInline.h: interface for the CXTPMarkupInline class.
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
#if !defined(__XTPMARKUPINLINE_H__)
#define __XTPMARKUPINLINE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MARKUP_POSITION_EOF   (POSITION)(-1)
#define MARKUP_POSITION_START (POSITION)( 0)
#define MARKUP_POSITION_END   (POSITION)( 1)

class CXTPMarkupInlineCollection;
class CXTPMarkupDrawingContext;
class CXTPMarkupBrush;
class CXTPMarkupUIElement;
class CXTPMarkupBuilder;
class CXTPMarkupTextBlock;
class CXTPMarkupFont;
class CXTPMarkupFrameworkElement;
class CXTPMarkupStyle;
class CXTPMarkupRenderTransform;

class _XTP_EXT_CLASS CXTPMarkupFrameworkContentElement : public CXTPMarkupInputElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupFrameworkContentElement)

protected:
	CXTPMarkupFrameworkContentElement();

public:
	CXTPMarkupObject* GetTag() const;
	void SetTag(CXTPMarkupObject* pTag);

public:
	CXTPMarkupInputElement* InputHitTest(CPoint point) const;
	CRect GetBoundRect() const;

public:
	CXTPMarkupFrameworkElement* GetParent() const;

protected:
	virtual void OnPropertyChanged(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pOldValue, CXTPMarkupObject* pNewValue);

	static void AFX_CDECL OnStylePropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* e);

public:
	static CXTPMarkupDependencyProperty* m_pTagProperty;
};

class _XTP_EXT_CLASS CXTPMarkupTextElement : public CXTPMarkupFrameworkContentElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupTextElement)

protected:
	CXTPMarkupTextElement();
	virtual ~CXTPMarkupTextElement();

public:
	void SetBackground(CXTPMarkupBrush* pBrush);
	void SetBackground(COLORREF clr);
	static void AFX_CDECL SetBackground(CXTPMarkupObject* pObject, CXTPMarkupBrush* pBrush);
	CXTPMarkupBrush* GetBackground() const;

	void SetForeground(CXTPMarkupBrush* pBrush);
	void SetForeground(COLORREF clr);
	static void AFX_CDECL SetForeground(CXTPMarkupObject* pObject, CXTPMarkupBrush* pBrush);
	CXTPMarkupBrush* GetForeground() const;

	void SetFontSize(int nFontSize);
	int GetFontSize() const;

	void SetFontWeight(int nFontWeight);
	int GetFontWeight() const;

	void SetFontFamily(LPCWSTR lpszFontFamily);
	LPCWSTR GetFontFamily() const;

	void SetFontStyle(int nFontStyle);
	int GetFontStyle() const;

	void SetTextDecorations(int nTextDecorations);
	int GetTextDecorations() const;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupTextElement);


	afx_msg long OleGetTextDecorations();
	afx_msg void OleSetTextDecorations(long nValue);
	afx_msg long OleGetFontSize();
	afx_msg void OleSetFontSize(long nValue);
	afx_msg long OleGetFontWeight();
	afx_msg void OleSetFontWeight(long nValue);
	afx_msg BSTR OleGetFontFamily();
	afx_msg void OleSetFontFamily(LPCTSTR lpszValue);
	afx_msg long OleGetFontStyle();
	afx_msg void OleSetFontStyle(long nValue);
	afx_msg LPDISPATCH OleGetBackground();
	afx_msg void OleSetBackground(LPDISPATCH lpBackground);
	afx_msg LPDISPATCH OleGetForeground();
	afx_msg void OleSetForeground(LPDISPATCH lpBackground);
	afx_msg VARIANT OleGetTag();
	afx_msg void OleSetTag(const VARIANT& va);

//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	static CXTPMarkupDependencyProperty* m_pBackgroundProperty;
	static CXTPMarkupDependencyProperty* m_pForegroundProperty;

	static CXTPMarkupDependencyProperty* m_pFontSizeProperty;
	static CXTPMarkupDependencyProperty* m_pFontWeightProperty;
	static CXTPMarkupDependencyProperty* m_pFontFamilyProperty;
	static CXTPMarkupDependencyProperty* m_pFontStyleProperty;
	static CXTPMarkupDependencyProperty* m_pTextDecorationsProperty;
	static CXTPMarkupDependencyProperty* m_pFontQualityProperty;
	static CXTPMarkupDependencyProperty* m_pFontCharsetProperty;
};

class _XTP_EXT_CLASS CXTPMarkupInline : public CXTPMarkupTextElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupInline)

protected:
	CXTPMarkupInline();
	virtual ~CXTPMarkupInline();

public:
	virtual CXTPMarkupInline* GetFirstInline() const;
	CXTPMarkupInline* GetNextInline() const;


public:
	XTPMarkupBaselineAlignment GetBaselineAlignment() const;
	void SetBaselineAlignment(XTPMarkupBaselineAlignment nBaselineAlignment);

public:
	virtual POSITION GetContentStartPosition() const;
	virtual void GetContentNextPosition(POSITION& pos) const;
	virtual BOOL IsWordBreakPosition(POSITION pos) const;
	virtual BOOL IsLineBreakPosition(POSITION pos) const;
	virtual BOOL IsCaretReturnPosition(POSITION pos) const;
	virtual BOOL IsWhiteSpacePosition(POSITION pos) const;

	virtual void Arrange(CRect rcFinalRect, POSITION posStart, POSITION posEnd);
	virtual CSize Measure(CXTPMarkupDrawingContext* pDC, POSITION posStart, POSITION posEnd);
	virtual void Render(CXTPMarkupDrawingContext* pDC, CRect rc, POSITION posStart, POSITION posEnd);

	virtual void PrepareMeasure(CXTPMarkupDrawingContext *pDC, CXTPMarkupRenderTransform *pRenderTransform);

	virtual int GetBaseline() const;

	CXTPMarkupTextBlock* GetTextBlock() const;


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupInline);

	afx_msg int OleGetBaselineAlignment();
	afx_msg void OleSetBaselineAlignment(int nValue);
//}}AFX_CODEJOCK_PRIVATE
#endif


protected:
	int m_nIndex;

public:
	static CXTPMarkupDependencyProperty* m_pBaselineAlignmentProperty;


private:
	friend class CXTPMarkupInlineCollection;
};



class _XTP_EXT_CLASS CXTPMarkupInlineUIContainer : public CXTPMarkupInline
{
	DECLARE_MARKUPCLASS(CXTPMarkupInlineUIContainer);

protected:
	CXTPMarkupInlineUIContainer();
	CXTPMarkupInlineUIContainer(CXTPMarkupUIElement* pElement);
	~CXTPMarkupInlineUIContainer();
public:

	void SetChild(CXTPMarkupUIElement* pElement);
	CXTPMarkupUIElement* GetChild() const;

public:
	virtual POSITION GetContentStartPosition() const;
	virtual void GetContentNextPosition(POSITION& pos) const;
	virtual BOOL IsWordBreakPosition(POSITION pos) const;
	virtual void Render(CXTPMarkupDrawingContext* pDC, CRect rc, POSITION posStart, POSITION posEnd);
	virtual CSize Measure(CXTPMarkupDrawingContext* pDC, POSITION posStart, POSITION posEnd);
	virtual void Arrange(CRect rcFinalRect, POSITION posStart, POSITION posEnd);

protected:
	virtual BOOL HasContentObject() const;
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual int GetLogicalChildrenCount() const;
	virtual CXTPMarkupObject* GetLogicalChild(int nIndex) const;

	CXTPMarkupInputElement* InputHitTest(CPoint point) const;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupInlineUIContainer);

	afx_msg LPDISPATCH OleGetChild();
	afx_msg void OleSetChild(LPDISPATCH lpDisp);

//}}AFX_CODEJOCK_PRIVATE
#endif


protected:
	CXTPMarkupUIElement* m_pChild;
};


class _XTP_EXT_CLASS CXTPMarkupInlineCollection : public CXTPMarkupCollection
{
	DECLARE_MARKUPCLASS(CXTPMarkupInlineCollection);

public:
	CXTPMarkupInlineCollection();
	~CXTPMarkupInlineCollection();
public:
	CXTPMarkupInline* GetInline(int nIndex) const;

protected:
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual void OnItemAdded(CXTPMarkupObject* pItem, int nIndex);

#ifdef _XTP_ACTIVEX
protected:
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupInlineCollection);
	DECLARE_ENUM_VARIANT(CXTPMarkupInlineCollection)

	long OleGetItemCount();
	LPDISPATCH OleGetItem(long nIndex);
	void OleAdd(LPDISPATCH lpElementDisp);
	void OleInsert(long Index, LPDISPATCH lpElementDisp);
//}}AFX_CODEJOCK_PRIVATE
#endif

private:
	friend class CXTPMarkupInline;
	friend class CXTPMarkupSpan;
	friend class CXTPMarkupTextBlock;
};


//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPMarkupInline* CXTPMarkupInlineCollection::GetInline(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrItems.GetSize() ? (CXTPMarkupInline*)m_arrItems[nIndex] : NULL;
}
AFX_INLINE int CXTPMarkupInlineUIContainer::GetLogicalChildrenCount() const {
	return m_pChild ? 1 : 0;
}
AFX_INLINE CXTPMarkupObject* CXTPMarkupInlineUIContainer::GetLogicalChild(int nIndex) const {
	return nIndex == 0 ? (CXTPMarkupObject*)m_pChild : NULL;
}
AFX_INLINE XTPMarkupBaselineAlignment CXTPMarkupInline::GetBaselineAlignment() const {
	CXTPMarkupEnum* pBaseline = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pBaselineAlignmentProperty));
	return pBaseline ? (XTPMarkupBaselineAlignment)(int)*pBaseline : xtpMarkupBaseline;
}
AFX_INLINE void CXTPMarkupInline::SetBaselineAlignment(XTPMarkupBaselineAlignment nBaselineAlignment) {
	SetValue(m_pBaselineAlignmentProperty, CXTPMarkupEnum::CreateValue(nBaselineAlignment));
}
AFX_INLINE void CXTPMarkupFrameworkContentElement::SetTag(CXTPMarkupObject* pTag) {
	SetValue(m_pTagProperty, pTag);
}
AFX_INLINE CXTPMarkupObject* CXTPMarkupFrameworkContentElement::GetTag() const {
	return GetValue(m_pTagProperty);
}


#endif // !defined(__XTPMARKUPINLINE_H__)
