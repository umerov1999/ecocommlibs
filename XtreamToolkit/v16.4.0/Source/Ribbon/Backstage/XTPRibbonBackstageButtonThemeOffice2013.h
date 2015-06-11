// XTPRibbonBackstageButtonOffice2013Theme.h: interface for the CXTPRibbonBackstageButtonOffice2013Theme class.
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
#if !defined(__XTPRIBBONBACKSTAGEBUTTONTHEMEOFFICE2013T_H__)
#define __XTPRIBBONBACKSTAGEBUTTONTHEMEOFFICE2013T_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPRibbonBackstageButtonThemeOffice2013 : public CXTPButtonThemeOffice2013
{
public:
	CXTPRibbonBackstageButtonThemeOffice2013();

public:
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

protected:
	friend class CBackstageButtonCtrl;
};

#endif // !defined(__XTPRIBBONBACKSTAGEBUTTONTHEMEOFFICE2013T_H__)
