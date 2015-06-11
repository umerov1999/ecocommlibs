// XTPButtonTheme.h: interface for the CXTPButtonTheme class.
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
#if !defined(__XTPBUTTONTHEME_H__)
#define __XTPBUTTONTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPButton;
class CXTPWinThemeWrapper;

#define CBR_CLIENTRECT 0
#define CBR_CHECKBOX   1
#define CBR_RADIOBOX   2
#define CBR_CHECKTEXT  3
#define CBR_RADIOTEXT  4
#define CBR_GROUPTEXT  5
#define CBR_GROUPFRAME 6
#define CBR_PUSHBUTTON 7

//===========================================================================
// Summary:
//     CXTPButtonTheme is a class used to perform Button Theme
//     drawing tasks.  This is the base class for all button themes.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonTheme : public CXTPControlTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonTheme object
	// ----------------------------------------
	CXTPButtonTheme();

	// -----------------------------------------------------------
	// Summary:
	//     Destroys a CXTPButtonTheme object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------
	virtual ~CXTPButtonTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the focus rectangle
	//     for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void DrawFocusRect(CDC* pDC, CXTPButton* pButton);

	virtual void DrawButton(CDC* pDC, CXTPButton* pButton);

	virtual void DrawButtonVisualStyleBackground(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the text for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void DrawPushButtonText(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function draws the icon for the button, if one has
	//     been defined.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void DrawPushButtonIcon(CDC* pDC, CXTPButton* pButton);
	void DrawPushButtonDropDown(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the color used to draw
	//     the button text.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     An RGB value that represents the button text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor(CXTPButton* pButton);

	virtual void DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton);
	virtual void DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the text for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void DrawButtonText(CDC* pDC, CXTPButton* pButton);

	virtual void DrawGroupBox(CDC* pDC, CXTPButton* pButton);

	void CalcRects(CDC* pDC, CXTPButton* pButton, CRect* pButtonText, UINT* pDrawFlags, CRect* pButtonIcon);
	void CalcRect(CDC* pDC, CXTPButton* pButton, LPRECT lprc, int code);
	BOOL IsVisualThemeUsed(CXTPButton* pButton);
	void AlphaEllipse(CDC* pDC, CRect rc, COLORREF clrBorder, COLORREF clrFace);

	void EnableToolbarStyle(BOOL bEnable) { m_bToolbarStyle = bEnable; }

	CXTPPaintManagerColor m_crTextDisabled;     // RGB value for disabled text color.
	CXTPPaintManagerColor m_crBorderHilite;     // RGB value for border highlight color.
	CXTPPaintManagerColor m_crBorderShadow;     // RGB value for border shadow color.
	CXTPPaintManagerColor m_crBorder3DHilite;   // RGB value for 3D border highlight color.
	CXTPPaintManagerColor m_crBorder3DShadow;   // RGB value for 3D border shadow color.
	CXTPPaintManagerColor m_crBack;
	CXTPPaintManagerColor m_crText;

	CXTPWinThemeWrapper* m_themeButton;

	BOOL m_bOffsetHiliteText;
	int m_nBorderWidth;
	BOOL m_bFlatGlyphs;
	BOOL m_bToolbarStyle;

	int m_cxBorder;
	int m_cyBorder;
	int m_cyEdge;
	int m_cxEdge;
};


#endif // !defined(__XTPBUTTONTHEME_H__)
