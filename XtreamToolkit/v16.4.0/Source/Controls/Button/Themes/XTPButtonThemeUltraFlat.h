//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPBUTTONTHEMEULTRAFLAT_H__)
#define __XTPBUTTONTHEMEULTRAFLAT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTPButtonThemeUltraFlat is a class used to perform Ultra Flat Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonThemeUltraFlat : public CXTPButtonTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonThemeUltraFlat object
	// ----------------------------------------
	CXTPButtonThemeUltraFlat();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);
	virtual void DrawGroupBox(CDC* pDC, CXTPButton* pButton);
	virtual void DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton);
	virtual void DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton);

	CXTPPaintManagerColor m_crBackHilite;   // RGB value for highlighted background color.
	CXTPPaintManagerColor m_crBackPushed;   // RGB value for pushed background color.
	CXTPPaintManagerColor m_crBackChecked;  // RGB value for when the control is checked.

	CXTPPaintManagerColor m_crBorderPushed; // RGB value for border pushed color.

	CXTPPaintManagerColor m_crTextPushed;  // RGB value for pushed text color.
	CXTPPaintManagerColor m_crTextHilite;  // RGB value for highlighted text color.

	CXTPPaintManagerColor m_crCheckMark; // RGB value for check mark color.

	BOOL m_bHiglightButtons;
};

#endif //#if !defined(__XTPBUTTONTHEMEULTRAFLAT_H__)
