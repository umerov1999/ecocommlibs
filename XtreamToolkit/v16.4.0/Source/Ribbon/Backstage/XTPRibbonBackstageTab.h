// XTPRibbonBackstageTab.h: interface for the CXTPRibbonBackstageTab class.
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
#if !defined(__XTPRIBBONBACKSTAGETAB_H__)
#define __XTPRIBBONBACKSTAGETAB_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPRibbonBackstageTab : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPRibbonBackstageTab)

public:
	CWnd* GetControlPane() const;

	void SetControlPaneMinSize(CSize sz);

protected:
	CXTPRibbonBackstageTab(HWND hwndControl = 0);



protected:

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonBackstageTab);

//}}AFX_CODEJOCK_PRIVATE
#endif
protected:
	HWND m_hwndControl;
	CSize m_szControlPaneMinSize;

	friend class CXTPRibbonBackstageView;
};

AFX_INLINE void CXTPRibbonBackstageTab::SetControlPaneMinSize(CSize sz)
{
	m_szControlPaneMinSize = sz;
}

#endif // !defined(__XTPRIBBONBACKSTAGETAB_H__)