// XTPMarkupPage.h: interface for the CXTPMarkupPage class.
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
#if !defined(__XTPMARKUPPAGE_H__)
#define __XTPMARKUPPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//===========================================================================
// Summary: CXTPMarkupPage is CXTPMarkupFrameworkElement derived class. It implements Page XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupPage : public CXTPMarkupFrameworkElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupPage);
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupPage object
	//-----------------------------------------------------------------------
	CXTPMarkupPage();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupPage object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupPage();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new Child object for the Page element
	// Parameters:
	//     pContent - New Child element
	// See Also: SetContent
	//-----------------------------------------------------------------------
	void SetContent(CXTPMarkupUIElement* pContent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns Child object of the Page element
	// Returns:
	//     Child CXTPMarkupUIElement object
	// See Also: SetContent
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetContent() const;


public:
//{{AFX_CODEJOCK_PRIVATE

	// Implementation
	virtual int GetVisualChildrenCount() const;
	virtual CXTPMarkupVisual* GetVisualChild(int nIndex) const;

protected:
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual CSize ArrangeOverride(CSize szFinalSize);
	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent);
	virtual BOOL HasContentObject() const;

	CXTPMarkupInputElement* InputHitTestOverride(CPoint /*point*/) const;

protected:
	CXTPMarkupUIElement* m_pContent;
//}}AFX_CODEJOCK_PRIVATE


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupPage);

	afx_msg LPDISPATCH OleGetChild();
	afx_msg void OleSetChild(LPDISPATCH lpDisp);
//}}AFX_CODEJOCK_PRIVATE
#endif
};


class _XTP_EXT_CLASS CXTPMarkupWindowContainer : public CXTPMarkupFrameworkElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupWindowContainer);
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupPage object
	//-----------------------------------------------------------------------
	CXTPMarkupWindowContainer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupPage object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupWindowContainer();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new Child object for the Page element
	// Parameters:
	//     pContent - New Child element
	// See Also: SetContent
	//-----------------------------------------------------------------------
	void SetChild(HWND hwndContent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns Child object of the Page element
	// Returns:
	//     Child CXTPMarkupUIElement object
	// See Also: SetContent
	//-----------------------------------------------------------------------
	HWND GetChild() const;


public:
	//{{AFX_CODEJOCK_PRIVATE

	// Implementation

protected:
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual CSize ArrangeOverride(CSize szFinalSize);
	void OnRender(CXTPMarkupDrawingContext* pDC);
	void RenderClipped(CXTPMarkupDrawingContext* pDC);

protected:
	HWND m_hWndChild;
	CSize m_szChild;
	//}}AFX_CODEJOCK_PRIVATE


#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupWindowContainer);

	afx_msg HWND OleGetChild();
	afx_msg void OleSetChild(HWND hWnd);
	//}}AFX_CODEJOCK_PRIVATE
#endif
};


AFX_INLINE int CXTPMarkupPage::GetVisualChildrenCount() const {
	return m_pContent != NULL ? 1 : 0;
}
AFX_INLINE CXTPMarkupVisual* CXTPMarkupPage::GetVisualChild(int nIndex) const {
	return nIndex == 0 ? m_pContent : NULL;
}


#endif // !defined(__XTPMARKUPPAGE_H__)
