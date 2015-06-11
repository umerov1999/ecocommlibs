// XTPRibbonBackstageButton.h: interface for the CXTPRibbonBackstageButton class.
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
#if !defined(__XTPRIBBONBACKSTAGEBUTTON_H__)
#define __XTPRIBBONBACKSTAGEBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPRibbonBackstageButton : public CXTPButton
{
#ifndef _XTP_ACTIVEX_BUTTON
	DECLARE_DYNCREATE(CXTPRibbonBackstageButton)
#endif

public:
	CXTPRibbonBackstageButton();

	virtual ~CXTPRibbonBackstageButton();

	virtual BOOL SetTheme(XTPControlTheme theme);
public:
	void SetShowShadow(BOOL bShowShadow);

	BOOL GetShowShadow() const;

	void ShowShadow(BOOL bShowShadow);

	void SetTabStyle(BOOL bTabStyle);

protected:
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPRibbonBackstageButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

public:
	BOOL m_bChecked;
	BOOL m_bShowShadow;
	BOOL m_bTabStyle;
};

AFX_INLINE void CXTPRibbonBackstageButton::SetShowShadow(BOOL bShowShadow)
{
	m_bShowShadow = bShowShadow;
}

AFX_INLINE BOOL CXTPRibbonBackstageButton::GetShowShadow() const
{
	return m_bShowShadow;
}

AFX_INLINE void CXTPRibbonBackstageButton::ShowShadow(BOOL bShowShadow)
{
	SetShowShadow(bShowShadow);
}

#endif // !defined(__XTPRIBBONBACKSTAGEBUTTON_H__)