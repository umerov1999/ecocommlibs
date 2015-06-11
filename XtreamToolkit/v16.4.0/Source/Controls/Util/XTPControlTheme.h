// XTPControlTheme.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK
// SOFTWARE GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT
// TO USE THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCONTROLSTHEME_H__)
#define __XTPCONTROLSTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPResourceImages;

//--------------------------------------------------------------------
// Summary:
//      Enumeration used to determine the active theme style.
// Remarks:
//      The XTPControlTheme enumeration type defines the constants that
//      indicate the theme for a particular window or application.
//--------------------------------------------------------------------
enum XTPControlTheme
{
	xtpControlThemeDefault,                // Default Windows classic theme.
	xtpControlThemeFlat,                   // Flat appearance style.
	xtpControlThemeUltraFlat,              // Ultra flat appearance style.
	xtpControlThemeOffice2000,             // Office 2000 style theme.
	xtpControlThemeOfficeXP,               // Office XP style theme.
	xtpControlThemeOffice2003,             // Office 2003 style theme.
	xtpControlThemeNativeWinXP,            // Windows XP and later native theme support.
	xtpControlThemeResource,               // Office 2007/Office 2010/Windows7 external resource DLL.
	xtpControlThemeVisualStudio2005,       // Visual Studio 2005 style theme.
	xtpControlThemeVisualStudio2008,       // Visual Studio 2008 style theme.
	xtpControlThemeVisualStudio2010,       // Visual Studio 2010 style theme.
	xtpControlThemeCustom,                 // User defined custom theme.
	xtpControlThemeOffice2013,             // Office 2013 control theme.
	xtpControlThemeVisualStudio2012,       // VS 2012 Light style theme.
	xtpControlThemeVisualStudio2012Light,  // VS 2012 Light style theme.
	xtpControlThemeVisualStudio2012Dark    // VS 2012 Dark style theme.
};

//--------------------------------------------------------------------
// Summary:
//      Enumeration used to determine the display state for a themed
//      window.
// Remarks:
//      The XTPControlDrawState enumeration type defines the constants that
//      indicate the display state for a particular window.
//--------------------------------------------------------------------
enum XTPControlDrawState
{
	xtpControlDrawStateNormal    = 0x0000, // The window should be drawn normal.
	xtpControlDrawStateDisabled  = 0x0001, // The window should be drawn disabled.
	xtpControlDrawStateHotLight  = 0x0002, // The window has the mouse cursor hovering.
	xtpControlDrawStateHotButton = 0x0004, // The window has the mouse cursor hovering over a drop button.
	xtpControlDrawStateSelected  = 0x0008, // The window has an item selected.
	xtpControlDrawStatePushed    = 0x0010, // The window has an item selected and the left mouse button is pressed.
	xtpControlDrawStateFocus     = 0x0020, // The window has keyboard focus.
};

//===========================================================================
// Summary:
//     CXTPControlTheme can be used as a base class for user defined themes.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlTheme : public CCmdTarget
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a new CXTPControlTheme object and handles initialization.
	// Parameters:
	//     m_eScheme - Specifies the color scheme to be used.
	//-----------------------------------------------------------------------
	CXTPControlTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the style for the theme.
	// Returns:
	//     A XTPControlTheme enumerator index value representing the theme style.
	//-----------------------------------------------------------------------
	XTPControlTheme GetThemeStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Static member posts the specified message to all immediate children
	//      of the specified HWND object. If bDeep is TRUE, the message is
	//      additionally sent to all other descendant windows.
	// Parameters:
	//     hWnd   - HWND to the top level window, usually CMainFrame or CDialog.
	//     nTheme - New visual theme. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     nTheme can be one of the following values:
	//     * <b>xtpControlThemeDefault</b> Default theme either Windows classic or Windows Visual Style.
	//     * <b>xtpControlThemeFlat</b> Flat appearance style.
	//     * <b>xtpControlThemeUltraFlat</b> Ultra flat appearance style.
	//     * <b>xtpControlThemeOffice2000</b> Office 2000 style theme.
	//     * <b>xtpControlThemeOfficeXP</b> Office XP style theme.
	//     * <b>xtpControlThemeOffice2003</b> Office 2003 style theme.
	//     * <b>xtpControlThemeVisualStudio2005</b> Visual Studio 2005 style theme.
	//     * <b>xtpControlThemeVisualStudio2008</b> Visual Studio 2008 style theme.
	//     * <b>xtpControlThemeVisualStudio2010</b> Visual Studio 2010 style theme.
	//     * <b>xtpControlThemeResource</b> Office 2007 / Offiec 2010/ Widows 7 external resource DLL.
	//-----------------------------------------------------------------------
	static void SetTheme(HWND hWnd, XTPControlTheme nTheme);

protected:

	XTPControlTheme  m_nTheme;  // Currently selected theme style index.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTPCONTROLSTHEME_H__)
