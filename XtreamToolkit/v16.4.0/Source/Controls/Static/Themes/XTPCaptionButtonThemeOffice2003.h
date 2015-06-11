//===========================================================================
// Summary: Class CXTPCaptionButtonThemeOffice2003 is a CXTPButtonOffice2003Theme
// derived class.  This class is used to create a button caption that resembles
// an Office 2003 button caption.
//===========================================================================
class _XTP_EXT_CLASS CXTPCaptionButtonThemeOffice2003 : public CXTPButtonThemeOffice2003
{
protected:
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
