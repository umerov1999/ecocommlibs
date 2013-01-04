// XTPRibbonBackstageControls.h
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPRIBBONBACKSTAGECONTROLS_H__)
#define __XTPRIBBONBACKSTAGECONTROLS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     CXTPRibbonBackstageView is CDialog derived class used for Backstage parent frame
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonBackstageButton : public CXTPButton
{
	DECLARE_DYNAMIC(CXTPRibbonBackstageButton)

public:
	CXTPRibbonBackstageButton();
	void ShowShadow(BOOL bShowShadow);
	void SetTabStyle(BOOL bTabStyle);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

	BOOL m_bChecked;

};


class _XTP_EXT_CLASS CXTPRibbonBackstageButtonTheme : public CXTPButtonResourceTheme
{
public:
	CXTPRibbonBackstageButtonTheme();

public:
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

protected:
	BOOL m_bTabStyle;
	BOOL m_bShowShadow;

	friend class CXTPRibbonBackstageButton;
	friend class CBackstageButtonCtrl;
};


class _XTP_EXT_CLASS CXTPRibbonBackstageSeparator : public CStatic
{
	DECLARE_DYNAMIC(CXTPRibbonBackstageSeparator);
	// Construction
public:
	CXTPRibbonBackstageSeparator();

	// Attributes
public:
	void SetVerticalStyle(BOOL bVerticalStyle);

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTPRibbonBackstageSeparator)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CXTPRibbonBackstageSeparator();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXTPRibbonBackstageSeparator)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()

protected:
	CBrush m_brush;
	BOOL m_bVerticalStyle;
};


class _XTP_EXT_CLASS CXTPRibbonBackstagePage : public CXTPResizePropertyPage
{
	DECLARE_DYNAMIC(CXTPRibbonBackstagePage)
public:
	CXTPRibbonBackstagePage(UINT nID);


protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

protected:
	CFont m_fntCaption;
	CFont m_fntTitle;

};


class _XTP_EXT_CLASS CXTPRibbonBackstageLabel : public CStatic
{
	// Construction
public:
	CXTPRibbonBackstageLabel();

	// Attributes
public:

	// Operations
public:
	void SetTextColor(COLORREF clrText);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTPRibbonBackstageLabel)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CXTPRibbonBackstageLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXTPRibbonBackstageLabel)
	afx_msg void OnPaint();
	//}}AFX_MSG

	COLORREF m_clrText;

	DECLARE_MESSAGE_MAP()
};



AFX_INLINE void CXTPRibbonBackstageLabel::SetTextColor(COLORREF clrText) {
	m_clrText = clrText;
}

AFX_INLINE void CXTPRibbonBackstageSeparator::SetVerticalStyle(BOOL bVerticalStyle) {
	m_bVerticalStyle = bVerticalStyle;
}


#endif // !defined(__XTPRIBBONBACKSTAGECONTROLS_H__)
