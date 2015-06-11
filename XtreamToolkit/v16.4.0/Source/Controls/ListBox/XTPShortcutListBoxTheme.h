// XTPShortcutListBoxTheme.h: interface for the CXTPShortcutListBoxTheme class.
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
#if !defined(__XTPSHORTCUTLISTBOXTHEME_H__)
#define __XTPSHORTCUTLISTBOXTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPShortcutListBox;
class CXTPImageManagerIcon;

//===========================================================================
// Summary:
//     CXTPShortcutListBoxTheme is used to draw the CXTPShortcutListBox object.  All themes
//     used for CXTPButton should inherit from this base class.
//     Note:  A Menu List box is similar in appearance to an outlook bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutListBoxTheme : public CXTPControlTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutListBoxTheme object.
	//-----------------------------------------------------------------------
	CXTPShortcutListBoxTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutListBoxTheme object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutListBoxTheme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the colors for the list box control.
	//     Also, this function gets the system default size for icons, and the
	//     default dimensions of a 3D border.  Lastly, the function sets the
	//     default color values for the different parts of buttons. i.e button
	//     pressed style, button highlight, button top left color, and button
	//     bottom right color.
	// Parameters:
	//     pList - Points to the CXTPShortcutListBox object.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPShortcutListBox* pList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw an item in the menu list box.
	// Parameters:
	//     pMenuListBox - A pointer to a CXTPShortcutListBox object.
	//     lpDIS        - A LPDRAWITEMSTRUCT struct that contains information
	//                    about how the list box should be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawItem(CXTPShortcutListBox* pMenuListBox, LPDRAWITEMSTRUCT lpDIS);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the background color of the menu list box.
	// Returns:
	//     A COLORREF value corresponding to the background color.
	//-----------------------------------------------------------------------
	COLORREF GetBackColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the color of the text used in the menu list box.
	// Returns:
	//     A COLORREF value corresponding to the text color.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor();

public:
	CXTPPaintManagerColor m_crItemText;                 // Item text color
	CXTPPaintManagerColor m_crItemTextDisabled;         // Disabled item text color
	CXTPPaintManagerColorGradient m_crgBack;            // Background color
	CXTPPaintManagerColorGradient m_crgItemBorder;      // Item border color
	CXTPPaintManagerColorGradient m_crgItemBackHot;     // Selected Item color
	CXTPPaintManagerColorGradient m_crgItemBackPressed; // Pressed Item color

public:
	int m_cyEdge;  // Height in pixels of the space between the icon an text.
	int m_cxIcon;  // Width of the menu icon.
	int m_cyIcon;  // Height of the menu icon.

protected:
	//-----------------------------------------------------------------------
	// Summary: This method is called to draw image of the item
	// Parameters:
	//     pDC - Pointer to valid device context
	//     rcIcon - Bounding rectangle
	//     pImage - Pointer to image to draw
	//     bEnabled - TRUE to draw enabled state of the image
	//     bSelected - TRUE to draw selected state of the image
	//     bPressed - TRUE to draw pressed state of the image
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CRect& rcIcon, CXTPImageManagerIcon* pImage, BOOL bEnabled, BOOL bSelected, BOOL bPressed);

	//-----------------------------------------------------------------------
	// Summary: This method is called to draw text of the item
	// Input:   pDC - Pointer to valid device context
	//          rcText - Bounding rectangle
	//          pCI - Pointer to CONTENT_ITEM to draw
	//-----------------------------------------------------------------------
	virtual void DrawText(CDC* pDC, CRect& rcText, CXTPShortcutListBox::CONTENT_ITEM* pCI);
};


//===========================================================================
// Summary:
//     Class CXTPShortcutListBoxThemeOfficeXP is derived from CXTPShortcutListBoxTheme.
//     This class is used to implement the Office XP theme for Menu List Boxes.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutListBoxThemeOfficeXP : public CXTPShortcutListBoxTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutListBoxThemeOfficeXP object.
	//-------------------------------------------------------------------------
	CXTPShortcutListBoxThemeOfficeXP();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the colors for the list box control.
	//     Also, this function gets the system default size for icons, and the
	//     default dimensions of a 3D border.  Lastly, the function sets the
	//     default color values for the different parts of buttons. i.e button
	//     pressed style, button highlight, button top left color, and button
	//     bottom right color.
	// Parameters:
	//     pList - Points to the CXTPShortcutListBox object.
	//-------------------------------------------------------------------------
	void RefreshMetrics(CXTPShortcutListBox* pList);

protected:

	//-----------------------------------------------------------------------
	// Summary: This method is called to draw image of the item
	// Parameters:
	//     pDC - Pointer to valid device context
	//     rcIcon - Bounding rectangle
	//     pImage - Pointer to image to draw
	//     bEnabled - TRUE to draw enabled state of the image
	//     bSelected - TRUE to draw selected state of the image
	//     bPressed - TRUE to draw pressed state of the image
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CRect& rcIcon, CXTPImageManagerIcon* pImage, BOOL bEnabled, BOOL bSelected, BOOL bPressed);

	//-----------------------------------------------------------------------
	// Summary: This method is called to draw text of the item
	// Input:   pDC - Pointer to valid device context
	//          rcText - Bounding rectangle
	//          pCI - Pointer to CONTENT_ITEM to draw
	//-----------------------------------------------------------------------
	virtual void DrawText(CDC* pDC, CRect& rcText, CXTPShortcutListBox::CONTENT_ITEM* pCI);
};


//===========================================================================
// Summary:
//     Class CXTPShortcutListBoxThemeOffice2003 is derived from CXTPShortcutListBoxThemeXP.
//     This class is used to implement the Office 2003 theme for Menu List Boxes.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutListBoxThemeOffice2003 : public CXTPShortcutListBoxThemeOfficeXP
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutListBoxThemeOffice2003 object.
	//-------------------------------------------------------------------------
	CXTPShortcutListBoxThemeOffice2003();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the colors for the list box control.
	//     Also, this function gets the system default size for icons, and the
	//     default dimensions of a 3D border.  Lastly, the function sets the
	//     default color values for the different parts of buttons. i.e button
	//     pressed style, button highlight, button top left color, and button
	//     bottom right color.
	// Parameters:
	//     pList - Points to the CXTPShortcutListBox object.
	//-------------------------------------------------------------------------
	void RefreshMetrics(CXTPShortcutListBox* pList);

protected:
	//-----------------------------------------------------------------------
	// Summary: This method is called to draw image of the item
	// Parameters:
	//     pDC - Pointer to valid device context
	//     rcIcon - Bounding rectangle
	//     pImage - Pointer to image to draw
	//     bEnabled - TRUE to draw enabled state of the image
	//     bSelected - TRUE to draw selected state of the image
	//     bPressed - TRUE to draw pressed state of the image
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CRect& rcIcon, CXTPImageManagerIcon* pImage, BOOL bEnabled, BOOL bSelected, BOOL bPressed);
};


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTPSHORTCUTLISTBOXTHEME_H__)
