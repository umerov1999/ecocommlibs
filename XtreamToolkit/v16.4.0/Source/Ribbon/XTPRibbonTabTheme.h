// XTPRibbonTabTheme.h: interface for the CXTPRibbonTabTheme class.
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
#if !defined(__XTPRIBBONTABTHEME_H__)
#define __XTPRIBBONTABTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPRibbonPaintManager;
class CXTPTabManager;

class CXTPRibbonTabTheme : public CAppearanceSetPropertyPageFlat
{
public:
	CXTPRibbonTabTheme(CXTPRibbonPaintManager *pRibbonTheme)
		: m_pRibbonTheme(pRibbonTheme)
	{
		m_rcHeaderMargin.SetRect(0, 1, 0, 0);
	}
protected:
	void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);
	void DrawText(CDC* pDC, CXTPTabManager* pManager, const CString& str, LPRECT lpRect, UINT nFormat);

	int GetButtonHeight(const CXTPTabManager *pTabManager);

protected:
	CXTPRibbonPaintManager* m_pRibbonTheme;
};

#endif // !defined(__XTPRIBBONTABTHEME_H__)
