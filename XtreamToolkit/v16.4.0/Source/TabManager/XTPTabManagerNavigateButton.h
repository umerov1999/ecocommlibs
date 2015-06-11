class CXTPTabManager;
class CXTPTabManagerItem;

//===========================================================================
// Summary:
//     CXTPTabManagerNavigateButton is a class used to represent the
//     tab navigation buttons.
// Remarks:
//     Navigation button are the buttons that appear in the tab header
//     area.  The buttons include the left arrow, right arrow, and close
//     buttons.  Any combination of these buttons can be shown.  You
//     can choose to never display then, always display them, or
//     automatically display them.  If yo automatically display the buttons,
//     they are only displayed when needed.  I.e, When there are too many
//     tabs to display in the tab header, the arrow buttons appear.
//
//          By default these all these buttons are displayed in the
//          CommandBars TabWorkSpace.  You will need to add them to the
//          TabControl.
//
//          If the xtpTabNavigateButtonAutomatic flag is used, then the button
//          will appear only when needed.  I.e. When the XTPTabLayoutStyle is
//          set to xtpTabLayoutAutoSize, all tab might not fit in the tab header
//          area.  When there are more tabs than can fit in the header, the
//          button will automatically be displayed.
//
// Example:
//     This example code illustrates how to specify when the tab navigation
//     buttons are displayed.
// <code>
// //Finds the left navigation button and specifies that it is always displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonLeft)->SetFlags(xtpTabNavigateButtonAlways);
// //Finds the right navigation button and specifies that it is never displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonRight)->SetFlags(xtpTabNavigateButtonNone);
// //Finds the close navigation button and specifies that it is always displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonClose)->SetFlags(xtpTabNavigateButtonAlways);
//
// //Called to recalculate tab area and reposition components
// Reposition();
// </code>
// See Also: XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton
//===========================================================================
class _XTP_EXT_CLASS CXTPTabManagerNavigateButton
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManagerNavigateButton object.
	// Parameters:
	//     pManager - CXTPTabManager object to draw the button on.
	//     nID      - Id of the button, can be one of the values listed in the
	//                Remarks section.
	//     dwFlags  - Indicates when to display the button.
	// Remarks:
	//     Standard ids of buttons are listed below:
	//     * <b>xtpTabNavigateButtonLeft</b>  Left tab navigation button.
	//     * <b>xtpTabNavigateButtonRight</b> Right tab navigation button.
	//     * <b>xtpTabNavigateButtonClose</b> Close tab navigation button.
	// See also:
	//     CXTPTabManager::FindNavigateButton, XTPTabNavigateButtonFlags,
	//     XTPTabNavigateButton
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton(CXTPTabManager* pManager, UINT nID, XTPTabNavigateButtonFlags dwFlags);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerNavigateButton object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPTabManagerNavigateButton();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to adjust the width of the tab header area.
	//     This will subtract the width of the navigate button from
	//     the width of the tab header area supplied.
	// Parameters:
	//     nWidth - Width of the tab header area.
	//
	// Remarks:
	//     The width will only be adjusted if the xtpTabNavigateButtonAlways
	//     is set.  If the tabs are vertical, then the height of the buttons
	//     are subtracted from the tab header area.
	//-----------------------------------------------------------------------
	virtual void AdjustWidth(int& nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify when the navigation button is displayed.
	// Parameters:
	//     dwFlags - Indicates when to display the button.
	// See Also: XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton, GetFlags
	//-----------------------------------------------------------------------
	void SetFlags(XTPTabNavigateButtonFlags dwFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine when the navigation button is displayed.
	// Returns:
	//     XTPTabNavigateButtonFlags indicating when the navigation
	//     button is displayed.
	// See Also: XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton, SetFlags
	//-----------------------------------------------------------------------
	XTPTabNavigateButtonFlags GetFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve size of the button.
	// See Also: GetRect
	//-----------------------------------------------------------------------
	virtual CSize GetSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get parent Item of the button.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw the navigation button in the tab
	//     header area.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user presses the left mouse button.
	// Parameters:
	//     hWnd - Handle to the CWnd object beneath the mouse cursor.
	//     pt   - Specifies the x- and y-coordinate of the cursor. These
	//            coordinates are always relative to the upper-left
	//            corner of the window.
	// Remarks:
	//     When the user clicks on a tab navigation button,
	//     CXTPTabManagerNavigateButton::PerformClick will call the OnExecute
	//     member, the OnExecute member will then call the
	//     CXTPTabManager::OnNavigateButtonClick member passing in the ID
	//     of the tab navigation button that was pressed.
	//
	//          This member performs the common operations of when a button
	//          is clicked such as drawing the "pressed" button and calling
	//          the OnExecute function of the clicked tab navigation button.
	//
	// See Also: CXTPTabManager::OnNavigateButtonClick
	//-----------------------------------------------------------------------
	virtual void PerformClick(HWND hWnd, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the navigation button is enabled.
	// Returns:
	//     TRUE if the navigation button is enabled, FALSE if it is disabled.
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if the navigation button
	//     is currently pressed.
	// Returns:
	//     TRUE if the navigation button is currently pressed.
	// Remarks:
	//     This member is used in the CXTPTabColorSetDefault::FillNavigateButton
	//     function and all other objects derived from CXTPTabColorSetDefault to
	//     determine how to colorize the button.
	// See Also: CXTPTabColorSetDefault::FillNavigateButton
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if the navigation button
	//     is currently highlighted.  This occurs when the mouse cursor
	//     is positioned over the navigation button.
	// Remarks:
	//     This member is used in the CXTPTabColorSetDefault::FillNavigateButton
	//     function and all other objects derived from CXTPTabColorSetDefault to
	//     determine how to colorize the button.
	// Returns:
	//     TRUE if the navigation button is currently highlighted.
	// See Also: CXTPTabColorSetDefault::FillNavigateButton
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the navigation button is
	//     currently visible.
	// Returns:
	//     TRUE if the navigation button is currently visible, FALSE
	//     otherwise.
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle of the
	//     navigation button.
	// Returns:
	//     Bounding rectangle of navigation button.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to set bounding rectangle of the
	//     navigation button.
	// Parameters:
	//     rcButton - Bounding rectangle of navigation button to set.
	//-----------------------------------------------------------------------
	void SetRect(CRect rcButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get identifier of the button
	// Returns:
	//     Identifier of the button
	// Remarks:
	//     See XTPTabNavigateButton for list of standard identifiers
	// See Also:
	//     XTPTabNavigateButton
	//-----------------------------------------------------------------------
	UINT GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to update self position.
	// Parameters:
	//     rcNavigateButtons - Bounding rectangle of the tab navigation
	//                         buttons.
	//-----------------------------------------------------------------------
	virtual void Reposition(CRect& rcNavigateButtons);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tooltip for this button.  This is the
	//     text that will be displayed when the mouse pointer is positioned
	//     over the button.
	// Parameters: lpszTooltip - Tooltip to set for this button.
	// See Also: GetTooltip
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tooltip displayed when the mouse
	//     pointer is positioned over the button.
	// Returns:
	//     The tooltip displayed when the mouse if over the button.
	// See Also: SetTooltip
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw a symbol in the tab navigation
	//     button. I.e. "left", "right" or "x".  This member must be overridden in
	//     derived classes.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Bounding rectangle of tab navigation button.
	//-----------------------------------------------------------------------
	virtual void DrawEntry(CDC* pDC, CRect rc) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a navigation button is clicked.
	// Parameters:
	//     bTick - If TRUE, CXTPTabManager::OnNavigateButtonClick will
	//             be called passing in the Id of the navigation
	//             button that was clicked as a parameter.
	//-----------------------------------------------------------------------
	virtual void OnExecute(BOOL bTick);

protected:
	UINT m_nID;                             // Id of the navigation button.
	XTPTabNavigateButtonFlags m_dwFlags;    // Flag that indicates when the navigation button will be drawn.
	CXTPTabManager* m_pManager;             // Pointer to the TabManager the navigation buttons are drawn on.
	CRect m_rcButton;                       // Bounding rectangle of the navigation button.

	BOOL m_bEnabled;                        // TRUE if the navigation button is enables.
	BOOL m_bPressed;                        // TRUE if the navigation button is pressed\clicked.
	CString m_strToolTip;                   // Tooltip for the button.
	CXTPTabManagerItem* m_pItem;            // Owner item. NULL if common.

private:
	friend class CXTPTabManager;
	friend class CXTPTabManagerNavigateButtons;
};


//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerNavigateButton

AFX_INLINE void CXTPTabManagerNavigateButton::SetFlags(XTPTabNavigateButtonFlags dwFlags) {
	m_dwFlags = dwFlags;
}
AFX_INLINE XTPTabNavigateButtonFlags CXTPTabManagerNavigateButton::GetFlags() const {
	return m_dwFlags;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsEnabled() const {
	return m_bEnabled;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsPressed() const {
	return m_bPressed;
}
AFX_INLINE CRect CXTPTabManagerNavigateButton::GetRect() const {
	return m_rcButton;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsVisible() const {
	return !m_rcButton.IsRectEmpty();
}
AFX_INLINE UINT CXTPTabManagerNavigateButton::GetID() const {
	return m_nID;
}
AFX_INLINE void CXTPTabManagerNavigateButton::SetTooltip(LPCTSTR lpszTooltip) {
	m_strToolTip = lpszTooltip;
}
AFX_INLINE CString CXTPTabManagerNavigateButton::GetTooltip() const {
	return m_strToolTip;
}
AFX_INLINE CXTPTabManagerItem* CXTPTabManagerNavigateButton::GetItem() const {
	return m_pItem;
}
