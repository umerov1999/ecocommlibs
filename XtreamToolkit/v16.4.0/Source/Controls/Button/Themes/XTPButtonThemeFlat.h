//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPBUTTONTHEMEFLAT_H__)
#define __XTPBUTTONTHEMEFLAT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTPButtonThemeFlat is a class used to perform Flat Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonThemeFlat : public CXTPButtonTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonThemeFlat object
	// ----------------------------------------
	CXTPButtonThemeFlat();

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
	void DrawButtonBackground(CDC* pDC, CXTPButton* pButton);
};

#endif //#if !defined(__XTPBUTTONTHEMEFLAT_H__)
