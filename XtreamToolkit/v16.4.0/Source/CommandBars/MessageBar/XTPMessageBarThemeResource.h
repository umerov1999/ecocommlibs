class CXTPResourceTheme;
class CXTPMessageBar;
class CXTPMessageBarButton;

class _XTP_EXT_CLASS CXTPMessageBarThemeResource : public CXTPMessageBarPaintManager
{
public:
	CXTPMessageBarThemeResource(CXTPResourceTheme *pTheme);

	//-----------------------------------------------------------------------
	// Summary: Draws the message bar.
	// Input:   pDC - Pointer to a valid device context.
	//          pBar - Pointer to the message bar to draw.
	//-----------------------------------------------------------------------
	virtual void FillMessageBar(CDC* pDC, CXTPMessageBar* pBar);

	//-----------------------------------------------------------------------
	// Summary: Call this member to draw a button in the message bar.
	// Input:   pDC - Pointer to a valid device context.
	//          pButton - Button to draw.
	//-----------------------------------------------------------------------
	virtual void DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton);

protected:
	CXTPResourceTheme *m_pTheme;
};