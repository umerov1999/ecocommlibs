// XTPTreeTheme.h: interface for the CXTPTreeTheme class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#ifndef XTPTreeTheme_h__
#define XTPTreeTheme_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTreeBase;
class CTreeCtrl;

// NOTE: this is stub designed to support CXTPTreeBase theming in future.
// Do not inherit from this class yet.

//===========================================================================
// Summary:
//     CXTPTreeTheme is a class used to perform Tree control Theme
//     drawing tasks.  This is the base class for all tree themes.
//===========================================================================
class _XTP_EXT_CLASS CXTPTreeTheme : public CXTPControlTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPTreeTheme object
	// ----------------------------------------
	CXTPTreeTheme();

	// -----------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTreeTheme object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------
	virtual ~CXTPTreeTheme();

	virtual void RefreshMetrics(CXTPTreeBase* pTreeBase);

	COLORREF m_clrBackground;  // RGB color value representing background color.
	COLORREF m_clrText;  // RGB color value representing text color.

	COLORREF m_clrItemBackgroundSelected;
	COLORREF m_clrItemBackgroundSelectedDisabled;

	COLORREF m_clrItemBackgroundNotFocused;
	COLORREF m_clrItemBackgroundNotFocusedDisabled;

	BOOL m_bDrawFocusRect;

	virtual COLORREF GetTreeBackColor() const;

	virtual COLORREF GetTreeTextColor() const;

	virtual COLORREF GetItemTextColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crText, CTreeCtrl* pTreeBase) const;

	virtual COLORREF GetItemBackColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crBack, CTreeCtrl* pTreeBase) const;

public:

	BOOL IsDrawFocusRect() const { return m_bDrawFocusRect; }
};

class _XTP_EXT_CLASS CXTPTreeVisualStudio2012Theme : public CXTPTreeTheme
{
public:

	CXTPTreeVisualStudio2012Theme(BOOL bLight);

	virtual void RefreshMetrics(CXTPTreeBase* pTreeBase);

protected:

	BOOL m_bLight;
};


#endif // XTPTreeTheme_h__
