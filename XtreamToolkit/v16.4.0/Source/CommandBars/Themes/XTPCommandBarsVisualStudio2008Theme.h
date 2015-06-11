//===========================================================================
// Summary:
//     The CXTPVisualStudio2008Theme class is used to enable a Visual Studio 2008 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPVisualStudio2008Theme : public CXTPVisualStudio2005Theme
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPVisualStudio2008Theme object.
	//-----------------------------------------------------------------------
	CXTPVisualStudio2008Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPVisualStudio2008Theme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPVisualStudio2008Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve the command bar's borders.
	// Parameters:
	//     pBar - Points to a CXTPCommandBar object that the borders need to get.
	// Returns:
	//     Borders of the command bar.
	//-----------------------------------------------------------------------
	virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar right gripper.
	// Parameters:
	//     pDC       - Points to a valid device context.
	//     xPos         - Specifies the logical x coordinate of the upper-left corner of the rectangle.
	//     yPos         - Specifies the logical y coordinate of the upper-left corner of the destination rectangle.
	//     cx        - Specifies the width of the rectangle.
	//     cy        - Specifies the height of the rectangle.
	//     bExpanded - TRUE if expanded.gripper.
	//-----------------------------------------------------------------------
	virtual void DrawPopupBarGripper(CDC* pDC, int xPos, int yPos, int cx, int cy, BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary: This method is called to fill the control's face.
	// Parameters:
	//    pDC         - Pointer to a valid device context
	//    rc          - Rectangle to draw.
	//    bSelected   - TRUE if the control is selected.
	//    bPressed    - TRUE if the control is pushed.
	//    bEnabled    - TRUE if the control is enabled.
	//    bChecked    - TRUE if the control is checked.
	//    bPopuped    - TRUE if the control is popuped.
	//    barType     - Parent's bar type.
	//    barPosition - Parent's bar position.
	//    nID         - Bar's control ID.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves buttons text color.
	// Parameters:
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barType     - Parent's bar type
	//     barPosition - Parent's bar position.
	// Returns:
	//     Buttons text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine offset of popuped bar.
	// Parameters:
	//     rc        - Control's bounding rectangle.
	//     pControl  - Points to a CXTPControl object
	//     bVertical - TRUE if control docked vertically.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

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
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw);

public:

	//-----------------------------------------------------------------------
	// Summary: Draws pixel in specific position with specify Alpha.
	// Parameters:
	//          pDC - Pointer to device context.
	//          xPos - Position to draw.
	//          yPos - Position to draw.
	//          clr - Color of pixel to draw.
	//          nAlpha - Opacity of pixel.
	//-----------------------------------------------------------------------
	static void AFX_CDECL AlphaPixel(CDC* pDC, int xPos, int yPos, COLORREF clr, int nAlpha);
	static void AFX_CDECL AlphaBorder(CDC* pDC, CRect rc, COLORREF clrBorder);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() { return xtpThemeVisualStudio2008; }


public:
	CXTPPaintManagerColorGradient m_clrMenuSelection;           // Gradient used to draw the current menu seleciton.
	CXTPPaintManagerColor m_clrMenuSelectionBorder;         // Color used to draw the border of the current menu selection.
	CXTPPaintManagerColor m_clrMenuSelectionText;           // Color used to draw the text of the current menu selection.

};
