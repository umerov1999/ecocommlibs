// XTPShortcutBarOffice2000Theme.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBAROFFICE2000THEME_H__)
#define __XTPSHORTCUTBAROFFICE2000THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     CXTPShortcutBarOffice2000Theme is a CXTPShortcutBarPaintManager derived
//     class that represents the Office 2003 shortcut bar theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarOffice2000Theme : public CXTPShortcutBarPaintManager
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
	//     Size of the gripper, which is 4 pixels for the Office 2000 theme.
	//-----------------------------------------------------------------------
	int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a rectangle.  The rectangle is
	//     used to draw the shortcut items.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rc     - Specifies the rectangle in logical units.
	//     nPen   - Specifies the color used to paint the rectangle.
	//     nBrush - Specifies the color used to fill the rectangle.
	// Remarks:
	//     Rectangle is used to draw the rectangle of the shortcut bar items
	//     in the Office XP theme.  This will draw the normal, pressed, and
	//     hot versions of the item.
	//-----------------------------------------------------------------------
	void Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the "gripper" lines in the gripper.
	// Parameters:
	//     pDC  - Points to the client device context.
	//     x0   - Specifies the logical x-coordinate of the start position.
	//     y0   - Specifies the logical y-coordinate of the start position.
	//     x1   - Specifies the logical x-coordinate of the endpoint for the line.
	//     y1   - Specifies the logical y-coordinate of the endpoint for the line.
	//     nPen - Specifies the color used to paint the line.
	//-----------------------------------------------------------------------
	void Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen);

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

#endif // !defined(__XTPSHORTCUTBAROFFICE2000THEME_H__)
