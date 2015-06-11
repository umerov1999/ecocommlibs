// XTPCommandBarsVisualStudio2012Theme.h : interface for the CXTPCommandBarsVisualStudio2012Theme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPCOMMANDBARSVISUALSTUDIO2012THEME_H__)
#define __XTPCOMMANDBARSVISUALSTUDIO2012THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPCommandBarsColorSet;

//===========================================================================
// Summary:
//     The CXTPCommandBarsVisualStudio2012Theme class is used to enable the Visual Studio 2012 theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsVisualStudio2012Theme : public CXTPCommandBarsModernUITheme
{
	DECLARE_DYNAMIC(CXTPCommandBarsVisualStudio2012Theme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsVisualStudio2012Theme object.
	//-----------------------------------------------------------------------
	explicit CXTPCommandBarsVisualStudio2012Theme(XTPPaintTheme paintTheme = xtpThemeVisualStudio2012Light);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsVisualStudio2012Theme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBarsVisualStudio2012Theme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar's gripper.
	// Parameters:
	//     pDC   - Pointer to a valid device context
	//     pBar  - Points to a CXTPCommandBar object
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the gripper.
	// Returns:
	//     Size of the gripper.
	//-----------------------------------------------------------------------
	virtual CSize DrawCommandBarGripper(CDC *pDC, CXTPCommandBar *pBar, BOOL bDraw = TRUE);

	virtual COLORREF GetControlEditBackColor(CXTPControl* pControl);

	virtual CSize DrawControlComboBox(CDC *pDC, CXTPControlComboBox *pControlCombo, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a dockbar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPDockBar object.
	//-----------------------------------------------------------------------
	virtual void FillDockBar(CDC *pDC, CXTPDockBar *pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the command bar's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPCommandBar object to draw.
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC *pDC, CXTPCommandBar *pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     rc          - Rectangle to draw.
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barType     - Parent's bar type
	//     barPosition - Parent's bar position.
	//     nID         - Bar's control ID.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPControl object to draw.
	// Returns:
	//     TRUE if the rectange was drawn, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawRectangle(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw pop-up bar right gripper.
	// Parameters:
	//     pDC       - Points to a valid device context.
	//     xPos         - Specifies the logical x coordinate of the upper-left corner of the rectangle.
	//     yPos         - Specifies the logical y coordinate of the upper-left corner of the destination rectangle.
	//     cx        - Specifies the width of the rectangle.
	//     cy        - Specifies the height of the rectangle.
	//     bExpanded - TRUE if expanded.gripper.
	//-----------------------------------------------------------------------
	virtual void DrawPopupBarGripper(CDC *pDC, int xPos, int yPos, int cx, int cy, BOOL bExpanded);

	virtual void DrawExpandGlyph(CDC* pDC, CXTPControl* pButton, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr);

	virtual void DrawHiddenGlyph(CDC* pDC, CXTPControl* pButton, COLORREF clr, BOOL bVertical);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw a split button expand symbol
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     pControl    - Points to a CXTPControl object
	//     pt          - Position of expand symbol
	//     bSelected   - TRUE if the control is selected.
	//     bPopuped    - TRUE if the control is popuped.
	//     bEnabled    - TRUE if the control is enabled.
	//     bVert       - TRUE if control is vertical
	//-----------------------------------------------------------------------
	virtual void DrawDropDownGlyph(CDC* pDC, CXTPControl* pControl, CPoint pt, BOOL bSelected, BOOL bPopuped, BOOL bEnabled, BOOL bVert);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a command bar's separator.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     pBar     - Points to a CXTPCommandBar object
	//     pControl - Points to a CXTPControl object
	//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
	// Returns:
	//     This method is called to draw a command bar's separator.
	//-----------------------------------------------------------------------
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw = TRUE);

	virtual void DrawCheckMark(CDC* pDC, CRect rcCheck, BOOL bIsRadio, BOOL bEnabled, BOOL bSelected);

	virtual COLORREF GetGlyphColor(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame for the SplitButton
	//     control
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     rc      - Bounding rectangle to draw
	//-----------------------------------------------------------------------
	virtual void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() { return xtpThemeVisualStudio2012; }

protected:

	virtual BOOL RequiresResourceImages();
	virtual void DrawControlPopupGlyph(CDC* pDC, CXTPControl* pButton);

	CXTPCommandBarsColorSet *m_pColorSet;
};

AFX_INLINE BOOL CXTPCommandBarsVisualStudio2012Theme::RequiresResourceImages()
{
	return TRUE;
}


#endif // !defined(__XTPCOMMANDBARSVISUALSTUDIO2012THEME_H__)
