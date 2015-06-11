// XTPShortcutBarNativeWinXPTheme.h interface for the CXTPShortcutBarPane class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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
#if !defined(__XTPSHORTCUTBARNATIVEWINXPTHEME_H__)
#define __XTPSHORTCUTBARNATIVEWINXPTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPShortcutBarNativeWinXPTheme is a CXTPShortcutBarOffice2003Theme derived
//     class that represents the Windows XP skinned shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarNativeWinXPTheme : public CXTPShortcutBarOffice2003Theme
{
public:
	CXTPShortcutBarNativeWinXPTheme();
	~CXTPShortcutBarNativeWinXPTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the shortcut
	//     bar paint manager.
	// Remarks:
	//     This refreshes all the visual metrics of the shortcut bar such
	//     as the pane caption color, pane font, pane caption height, etc.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the client area of the ShortcutBar control
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control
	//-----------------------------------------------------------------------
	void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the specified item of the ShortcutBar control.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Item to be drawn.
	//-----------------------------------------------------------------------
	void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

protected:
	CXTPWinThemeWrapper* m_themeToolbar;
	CXTPWinThemeWrapper* m_themeRebar;
};

#endif //  !defined(__XTPSHORTCUTBARNATIVEWINXPTHEME_H__)
