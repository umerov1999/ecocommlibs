//===========================================================================
// Summary: TabWorkspace control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlTabWorkspace : public CXTPControlButton, public CXTPTabClientWnd::CWorkspace
{
	DECLARE_XTP_CONTROL(CXTPControlTabWorkspace)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlTabWorkspace object.
	//-----------------------------------------------------------------------
	CXTPControlTabWorkspace();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlTabWorkspace object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPControlTabWorkspace();

public:

	//-----------------------------------------------------------------------
	// Input:   lpRect - The rectangular area of the window that is invalid.
	//          bAnimate - TRUE to animate changes in bounding rectangle.
	// Summary: This member is called when visual properties of the tabs are
	//          changed. For example, color, mouse-over, and on-click. This
	//          member must be overridden in derived classes.
	//-----------------------------------------------------------------------
	virtual void RedrawControl(LPCRECT lpRect, BOOL bAnimate);

	//-----------------------------------------------------------------------
	// Summary: This method is called to get window handle of workspace.
	// Returns: This method is called to get window handle of workspace.
	//-----------------------------------------------------------------------
	virtual CWnd* GetWindow() const;

	//-----------------------------------------------------------------------
	// Summary: Retrieves the paint manager.
	// Returns: A pointer to a CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	CXTPTabPaintManager* GetPaintManager() const;

	//-------------------------------------------------------------------------
	// Summary: This method is called to recalculate layout of tab client.
	//-------------------------------------------------------------------------
	virtual void OnRecalcLayout();

	//-----------------------------------------------------------------------
	// Summary: Call this member to get the current position of the tabs.
	// Returns: Current XTPTabPosition of the tabs.
	// Remarks: The position refers to where the tabs are located in relation
	//          to the TabClient. The tabs can be positioned on the top,
	//          bottom, left, or right side of the TabClient.
	//-----------------------------------------------------------------------
	XTPTabPosition GetPosition() const;

	//-------------------------------------------------------------------------
	// Summary: This method is called when item was removed from TabManager
	//          collection.
	//-------------------------------------------------------------------------
	virtual void OnRemoved();

protected:

	//-----------------------------------------------------------------------
	// Input:   rcControl - Bounding recyangle of the tab control.
	// Summary: This member is called to set bounding rectangle of the
	//          tab control.
	//-----------------------------------------------------------------------
	virtual void SetRect(CRect rcControl);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	// Summary: Call this member to draw the tab workspace.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	// Summary: Call this member to retrieve size of the tab workspace.
	// Returns: Size of the tab workspace.
	//-----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y coordinate of the cursor. These
	//                  coordinates are always relative to the upper-left
	//                  corner of the window.
	//          pTI - A pointer to a TOOLINFO structure.
	// Summary: The framework calls this member function to determine whether
	//          a point is in the bounding rectangle of the specified tool.
	// Returns: If the tooltip control was found, the window control ID. If
	//          the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	//-----------------------------------------------------------------------
	// Input:   bKeyboard - TRUE if the control is selected using the keyboard.
	//          pt - Mouse cursor position.
	// Summary: This method is called when the user clicks the control.
	//-----------------------------------------------------------------------
	virtual void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y coordinate of the cursor.
	// Summary: This member is called when the mouse cursor moves.
	//-----------------------------------------------------------------------
	void OnMouseMove(CPoint point);

protected:
	BOOL m_bForceRecalc;            // TRUE to force the tab control to recalculate it's layout.

};
