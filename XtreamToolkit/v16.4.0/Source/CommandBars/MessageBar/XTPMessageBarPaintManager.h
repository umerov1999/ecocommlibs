class _XTP_EXT_CLASS CXTPMessageBarPaintManager
{
public:
	CXTPMessageBarPaintManager(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	//          pBar - Points to a CXTPMessageBar object.
	// Summary: Draws the message bar.
	//-----------------------------------------------------------------------
	virtual void FillMessageBar(CDC* pDC, CXTPMessageBar* pBar);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	//          pButton - Button to draw.
	// Summary: Call this member to draw a button in the message bar.
	//-----------------------------------------------------------------------
	virtual void DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton);

	//-----------------------------------------------------------------------
	// Input:   x - Specifies the left position.
	//          y - Specifies the right position.
	//          cy - Specifies the height of the client area.
	// Summary: Call this member to retrieve the close button coordinates.
	// Returns: A CRect object containing the location of the close button.
	//-----------------------------------------------------------------------
	virtual CRect GetCloseButtonRect(int x, int y, int cy);

protected:
	CXTPPaintManager *m_pPaintManager;
};
