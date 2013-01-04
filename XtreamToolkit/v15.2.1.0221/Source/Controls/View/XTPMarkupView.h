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
#if !defined(__XTPMARKUPVIEW_H__)
#define __XTPMARKUPVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef _XTP_INCLUDE_MARKUP


class CXTPMarkupDrawingContext;
class CXTPMarkupUIElement;
class CXTPMarkupContext;

class _XTP_EXT_CLASS CXTPMarkupView : public CView, CXTPMarkupContext
{
protected: // create from serialization only
	CXTPMarkupView();
	DECLARE_DYNCREATE(CXTPMarkupView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkupPadView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CXTPMarkupView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void OnInitialUpdate();

	void DrawMarkup(CXTPMarkupDrawingContext* pDC, CRect rc);
	void SetMarkupText(LPCTSTR lpszMarkup);

public:
	CXTPMarkupUIElement* m_pUIElement;
	BOOL m_bInRender;

// Generated message map functions
protected:
	//{{AFX_MSG(CMarkupPadView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

#endif // !defined(__XTPMARKUPVIEW_H__)
