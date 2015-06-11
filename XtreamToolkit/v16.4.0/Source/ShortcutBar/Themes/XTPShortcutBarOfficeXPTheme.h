// XTPShortcutBarOfficeXPTheme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBAROFFICEXPTHEME_H__)
#define __XTPSHORTCUTBAROFFICEXPTHEME_H__
//}}AFX_CODEJOCK_PRIVATE


//===========================================================================
// Summary:
//     CXTPShortcutBarOfficeXPTheme is a CXTPShortcutBarPaintManager derived
//     class that represents the Office 2003 shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarOfficeXPTheme : public CXTPShortcutBarPaintManager
{
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
	//     Size of the gripper, which is 7 pixels for the Office XP theme.
	//-----------------------------------------------------------------------
	int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);


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
};

#endif // !defined(__XTPSHORTCUTBAROFFICEXPTHEME_H__)
