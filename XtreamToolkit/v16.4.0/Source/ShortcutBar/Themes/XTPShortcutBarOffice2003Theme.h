// XTPShortcutBarOffice2003Theme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBAROFFICE2003THEME_H__)
#define __XTPSHORTCUTBAROFFICE2003THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPShortcutBarOffice2003Theme is a CXTPShortcutBarPaintManager derived
//     class that represents the Office 2003 shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarOffice2003Theme : public CXTPShortcutBarPaintManager
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarOffice2003Theme object.
	//-------------------------------------------------------------------------
	CXTPShortcutBarOffice2003Theme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the client area of the ShortcutBar control.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control.
	//-----------------------------------------------------------------------
	void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame of the ShortcutBar control.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control.
	// Remarks:
	//     This is the border that is drawn around the entire shortcut bar.
	//-----------------------------------------------------------------------
	void DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the specified item of the ShortcutBar control.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Item to be drawn.
	//-----------------------------------------------------------------------
	void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw gripper of the ShortcutBar control
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control
	//     bDraw - TRUE to draw, FALSE to determine size of the gripper.
	// Returns:
	//     Size of the gripper, which is 7 pixels for the Office 2003 theme.
	//-----------------------------------------------------------------------
	int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the caption of the specified pane.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pPane - Shortcut Pane to be drawn
	//     bDraw - TRUE to draw pane, FALSE to determine size of the caption
	// Remarks:
	//     Panes of the shortcut bar are displayed in the shortcut bar's client
	//     area.  Each pane consists of a caption and one or more sub-panes.
	//     Each sub-pane has it's own caption and is drawn with the
	//     DrawPaneItemCaption method.
	//
	//     The caption of the shortcut bar pane is the bar on the top the pane.
	//     The caption of the pane is set when the pane is created in the
	//     CXTPShortcutBarPane::Create method.
	//
	//     The pane caption can be retrieved with CXTPShortcutBarPane::GetCaption
	//
	//     Shortcut bar pane's have sub-item's which also have a caption that is
	//     drawn with the DrawPaneItemCaption method.
	// Returns:
	//     Size of the caption.  Which is 24 pixes.
	//-----------------------------------------------------------------------
	int DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the caption of the specified shortcut
	//     pane item.  This is the sub-pane of a shortcut bar pane.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Shortcut item pane to be drawn
	//     bDraw - TRUE to draw, FALSE to determine size of the shortcut
	//             pane caption.
	// Remarks:
	//     Panes of the shortcut bar are displayed in the shortcut bar's client
	//     area.  Each pane consists of a caption and one or more sub-panes.
	//     Each sub-pane has it's own caption and is drawn with the
	//     DrawPaneItemCaption method.
	//
	//     The caption of the pane item is set when the pane item is created in the
	//     CXTPShortcutBarPane::AddItem method.
	//
	//     The sub-items of a pane can be retrieved with the CXTPShortcutBarPane::GetItem
	//     method.
	// Returns:
	//     Size of the pane item caption.  This is retrieved from CXTPShortcutBarPaintManager::m_nPaneItemHeight.
	//-----------------------------------------------------------------------
	int DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the shortcut
	//     bar paint manager.
	// Remarks:
	//     This refreshes all the visual metrics of the shortcut bar such
	//     as the pane caption color, pane font, pane caption height, etc.
	//
	//     If luna colors are enabled, the the shortcut bar will be colored
	//     to match the current system them if the XTPCurrentSystemTheme is
	//     xtpSystemThemeBlue, xtpSystemThemeSilver, or xtpSystemThemeOlive.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the image of a Shortcut Bar item.
	// Parameters:
	//     pDC       - Points to the client device context.
	//     pt        - Location at which to draw the image within the specified
	//                 device context.
	//     sz        - Size of the image.
	//     pImage    - Points to a CXTPImageManagerIcon object.
	//     bSelected - TRUE if the shortcut bar item is selected\has focus.
	//                 I.e. When the user clicks on the item.
	//     bPressed  - TRUE if the shortcut bar item is currently pressed.
	//                 I.e. The user is clicking on the item.
	//     bChecked  - TRUE if the shortcut bar item is checked.  I.e.
	//                 toggle buttons.
	//     bEnabled  - TRUE to draw item enabled
	// Remarks:
	//     This member draws the image of a shortcut bar item.  The
	//     DrawShortcutItem method uses this method to draw shortcut bar
	//     images.
	//-----------------------------------------------------------------------
	void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw navigation item of minimized ShortcutBar
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Shortcut item to be drawn
	//-----------------------------------------------------------------------
	void FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

public:
	CXTPPaintManagerColorGradient m_grcCaption;         // Caption color
	CXTPPaintManagerColorGradient m_grcItemCaption;     // Items color
	CXTPPaintManagerColorGradient m_grcItemHotCaption;     // Items color
	CXTPPaintManagerColor m_clrItemCaptionShadow;       // Color of subcaption shadow
	CXTPPaintManagerColor m_clrItemCaptionTop;          // Color under subcaption rectangle
	CXTPPaintManagerColor m_clrCaptionText;             // Color of items text
	CXTPPaintManagerColor m_clrItemCaptionText;         // Color of subcaption text

	BOOL m_bShowTopLeftCaptionShadow;     // True to draw rectangle around items
	BOOL m_bLunaColorDisabled;
};

#endif // !defined(__XTPSHORTCUTBAROFFICE2003THEME_H__)
