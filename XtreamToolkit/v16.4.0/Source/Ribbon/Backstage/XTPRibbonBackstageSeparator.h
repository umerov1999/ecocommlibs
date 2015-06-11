// XTPRibbonBackstageSeparator.h: interface for the CXTPRibbonBackstageSeparator class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPRIBBONBACKATAGESEPARATOR_H__)
#define __XTPRIBBONBACKATAGESEPARATOR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef CXTPRibbonBackstageSeparatorBase
#define CXTPRibbonBackstageSeparatorBase CStatic
#endif

class CXTPMarkupContext;
class CXTPMarkupUIElement;

class CXTPRibbonBackstageSeparatorPaintManager;

class _XTP_EXT_CLASS CXTPRibbonBackstageSeparator : public CXTPRibbonBackstageSeparatorBase
{
	DECLARE_DYNCREATE(CXTPRibbonBackstageSeparator);

public:
	CXTPRibbonBackstageSeparator();
	virtual ~CXTPRibbonBackstageSeparator();

public:
	virtual BOOL SetTheme(XTPControlTheme theme);

	BOOL IsVerticalStyle() const;

	void SetVerticalStyle(BOOL bVerticalStyle);

public:
	//{{AFX_VIRTUAL(CXTPRibbonBackstageSeparator)
	virtual void OnDraw(CDC *pDC);
	//}}AFX_VIRTUAL

protected:
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPRibbonBackstageSeparator)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	//}}AFX_MSG

protected:
	CXTPRibbonBackstageSeparatorPaintManager *m_pTheme;

	BOOL m_bVerticalStyle;

	CXTPMarkupContext   *m_pMarkupContext;
	CXTPMarkupUIElement *m_pMarkupUIElement;
};

AFX_INLINE BOOL CXTPRibbonBackstageSeparator::IsVerticalStyle() const
{
	return m_bVerticalStyle;
}

AFX_INLINE void CXTPRibbonBackstageSeparator::SetVerticalStyle(BOOL bVerticalStyle)
{
	m_bVerticalStyle = bVerticalStyle;
}

#endif // !defined(__XTPRIBBONBACKATAGESEPARATOR_H__)