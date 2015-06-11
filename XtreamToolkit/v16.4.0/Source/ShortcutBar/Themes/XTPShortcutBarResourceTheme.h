// XTPShortcutBarResourceTheme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBARRESOURCETHEME_H__)
#define __XTPSHORTCUTBARRESOURCETHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     CXTPShortcutBarResourceTheme is a CXTPShortcutBarOffice2003Theme derived
//     class that represents the Office 2007, 2010 and 2012 shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarResourceTheme : public CXTPShortcutBarOffice2003Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarResourceTheme object.
	//-------------------------------------------------------------------------
	CXTPShortcutBarResourceTheme();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the shortcut
	//     bar paint manager.
	// Remarks:
	//     This refreshes all the visual metrics of the shortcut bar such
	//     as the pane caption color, pane font, pane caption height, etc.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw gripper of the ShortcutBar control
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control
	//     bDraw - TRUE to draw, FALSE to determine size of the gripper.
	// Returns:
	//     Size of the gripper, which is 7 pixels for the Office XP theme.
	//-----------------------------------------------------------------------
	int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

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
	//     This method is called to fill the client area of the ShortcutBar control.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control.
	//-----------------------------------------------------------------------
	void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the caption of the specified shortcut
	//     pane item.  This is the sub-pane of a shortcut bar pane.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Shortcut item pane to be drawn
	//     bDraw - bDraw - TRUE to draw, FALSE to determine size of the shortcut
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
	//     Size of the pane item caption.  This is retrieved from m_nPaneItemHeight.
	//-----------------------------------------------------------------------
	virtual int DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw);

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
	//     Size of the caption.  Unless overridden, the default caption is 24 pixes.
	//-----------------------------------------------------------------------
	virtual int DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the expand button.
	// Parameters:
	//     pDC - Points to the client device context.
	//     rc - Client rectangle of the expand button.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC* pDC, CRect rc);


	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw navigation item of minimized ShortcutBar
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Shortcut item to be drawn
	//-----------------------------------------------------------------------
	virtual void FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

	virtual void DrawPaneCaptionMinimizeButton(CDC* pDC, CXTPShortcutBarPane* pPane);

public:
	BOOL m_bFlatStyle;
};

typedef CXTPShortcutBarResourceTheme CXTPShortcutBarOffice2007Theme;
typedef CXTPShortcutBarResourceTheme CXTPShortcutBarOffice2010Theme;

#endif // !defined(__XTPSHORTCUTBARRESOURCETHEME_H__)
