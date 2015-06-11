//===========================================================================
// Summary:
//     CXTPDockingPaneCaptionButton is a stand alone internal class.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneCaptionButton
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneCaptionButton object
	// Parameters:
	//     nID   - If of the caption button.
	//     pPane - Pane that the caption button controls.
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButton (int nID, CXTPDockingPaneBase* pPane)
	{
		m_nID = nID; m_pPane = pPane;
		m_bPressed = m_bSelected = FALSE;
		m_dwState = 0;
		m_rcButton.SetRectEmpty();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the button's rectangle.
	// Parameters:
	//     rc - Button's rectangle.
	//-----------------------------------------------------------------------
	void SetRect (CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw a pin button.
	// Parameters:
	//     pDC     - Pointer to the device context in which to draw.
	//     pt      - Center point.
	//     bPinned - TRUE if the button is pinned.
	//-----------------------------------------------------------------------
	static void AFX_CDECL DrawPinnButton(CDC* pDC, CPoint pt, BOOL bPinned);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw the maximize\restore button.
	// Parameters:
	//     pDC     - Pointer to the device context in which to draw.
	//     pt      - Center point.
	//     bMaximize - TRUE if the button is to be drawn maximized, FALSE to
	//                 draw restore button.
	//     clr - Color of button.
	//-----------------------------------------------------------------------
	static void AFX_CDECL DrawMaximizeRestoreButton(CDC* pDC, CPoint pt, BOOL bMaximize, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to click button.
	// Parameters:
	//     pWnd    - Parent window of button.
	//     pt      - Initial mouse position.
	//     bClient - TRUE if <i>pt</i> belongs client area of window.
	//-----------------------------------------------------------------------
	DWORD Click (CWnd* pWnd, CPoint pt, BOOL bClient = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle
	// Returns:
	//     A CRect object
	//-----------------------------------------------------------------------
	CRect GetRect()
	{
		return m_rcButton;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the state of the caption button.
	//     This will tell you if the pin caption button is visible or
	//     if it is pushed.
	// Returns:
	//     1 if the the button is visible, 2 if it is pushed, 3 if it
	//     is both visible and pushed.
	// See Also: XTPDockingPanePinState
	//-----------------------------------------------------------------------
	DWORD GetState() const
	{
		return m_dwState;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the state of the pin caption button.
	// Parameters:
	//     dwState - XTPDockingPanePinState of pin button.
	// See Also: XTPDockingPanePinState
	//-----------------------------------------------------------------------
	void SetState(DWORD dwState)
	{
		m_dwState = dwState;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves button identifier
	// Returns:
	//     Button identifier. Standard values are
	//     * <b>XTP_IDS_DOCKINGPANE_CLOSE</b> Close button
	//     * <b>XTP_IDS_DOCKINGPANE_AUTOHIDE</b> Auto-hide button
	//-----------------------------------------------------------------------
	int GetID() const
	{
		return m_nID;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the pane that the pin button controls.
	// Returns:
	//     Pane that the pin button controls.
	//-----------------------------------------------------------------------
	CXTPDockingPaneBase* GetPane() const
	{
		return m_pPane;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the point belongs to button.
	// Parameters:
	//     pt - Point to test.
	// Returns:
	//     TRUE if point belongs to the button; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL PtInRect (POINT pt) const
	{
		return m_rcButton.PtInRect (pt) != 0;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to refresh styles of the button.
	// Parameters:
	//     pt - Mouse cursor pointer.
	//-----------------------------------------------------------------------
	BOOL CheckForMouseOver (CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw button
	//-----------------------------------------------------------------------
	void InvalidateRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the button is pushed.
	// Returns:
	//     TRUE is the button is pushed, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsPressed() const
	{
		return m_bPressed;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the button mouse hover the button
	// Returns:
	//     TRUE if mouse hover the button.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const
	{
		return m_bSelected;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the pin caption button is visible.
	// Returns:
	//     TRUE if the pin button is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if button is enabled
	// Returns:
	//     TRUE if button is enabled
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

protected:
	CRect m_rcButton;             // Bounding rectangle of the button
	CXTPDockingPaneBase* m_pPane; // Parent pane
	int m_nID;                    // Identifier of the button.
	BOOL m_bPressed;              // TRUE if button is pressed by user.
	BOOL m_bSelected;             // TRUE if user move mouse cursor inside button rectangle
	DWORD m_dwState;              // XTPDockingPanePinState of caption button.

	friend class CXTPDockingPaneTabbedContainer;
	friend class CXTPDockingPaneMiniWnd;
};
