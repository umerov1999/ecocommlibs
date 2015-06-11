//===========================================================================
// Summary: Class CXTPCaptionButtonThemeOfficeXP is a CXTPButtonOfficeXPTheme
//          derived class.  This class is used to create a button caption that
//          resembles an Office XP button caption.
//===========================================================================
class _XTP_EXT_CLASS CXTPCaptionButtonThemeOfficeXP : public CXTPButtonThemeOfficeXP
{
public:
	//-----------------------------------------------------------------------
	// Summary: Default class constructor.  Call this member function to
	//          construct a CXTPCaptionButtonThemeOfficeXP object.
	//-----------------------------------------------------------------------
	CXTPCaptionButtonThemeOfficeXP();

protected:
	//-----------------------------------------------------------------------
	// Summary: Call this member function to draw the button caption text.
	// Parameters:
	//       pDC     - A pointer to a valid device context.
	//       pButton - A pointer to a valid CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void DrawButtonText(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary: Call this member function to draw the background of the caption.
	// Parameters:
	//     lpDIS   - An LPDRAWITEMSTRUCT struct that contains the information the
	//               owner window must have to determine how to paint an owner-drawn
	//               control.
	//     pButton - A pointer to valid CXTPButton object.
	// Returns:
	//     A TRUE if the background was successfully drawn.  FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary: Call this member function to get the text color of the button caption.
	// Parameters:
	//      pButton - A pointer to a valid CXTPButton object.
	// Returns:
	//      An RGB value specifying the text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor(CXTPButton* pButton);
};
