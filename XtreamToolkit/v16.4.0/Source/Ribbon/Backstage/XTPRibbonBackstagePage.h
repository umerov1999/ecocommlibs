// XTPRibbonBackstagePage.h: interface for the CXTPRibbonBackstagePage class.
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
#if !defined(__XTPRIBBONBACKSTAGEPAGE_H__)
#define __XTPRIBBONBACKSTAGEPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPRibbonBackstagePage : public CXTPResizePropertyPage
{
	DECLARE_DYNAMIC(CXTPRibbonBackstagePage)
public:
	CXTPRibbonBackstagePage(UINT nID);


protected:
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPRibbonBackstagePage)
	afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
	//}}AFX_MSG


protected:
	CFont m_fntCaption;
	CFont m_fntTitle;
};

#endif // !defined(__XTPRIBBONBACKSTAGEPAGE_H__)
