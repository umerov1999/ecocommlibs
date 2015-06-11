// XTPCommandBarsThemeMsMoney.h: interface for the CXTPCommandBarsThemeMsMoney class.
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
#if !defined(__XTPCOMMANDBARSTHEMEMSMONEY_H__)
#define __XTPCOMMANDBARSTHEMEMSMONEY_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPCommandBarsThemeMsMoney : public CXTPDefaultTheme
{
public:
	CXTPCommandBarsThemeMsMoney(COLORREF clrGradientLight, COLORREF clrGradientDark, COLORREF clrLineLight, COLORREF clrLineDark);
	virtual ~CXTPCommandBarsThemeMsMoney();

	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);
	virtual CSize DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw);
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);
	virtual CSize DrawControlToolBarParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);
	virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);
	virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw);
	virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBarImage = TRUE);
	virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

private:
	COLORREF m_clrGradientLight;
	COLORREF m_clrGradientDark;
	COLORREF m_clrLineLight;
	COLORREF m_clrLineDark;

};

//{{AFX_CODEJOCK_PRIVATE
#endif // !defined(__XTPCOMMANDBARSTHEMEMSMONEY_H__)
//}}AFX_CODEJOCK_PRIVATE
