//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPBUTTONTHEMEVISUALSTUDIO2012_H__)
#define __XTPBUTTONTHEMEVISUALSTUDIO2012_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTPButtonThemeVisualStudio2012 is a class used to perform Visual Studio 2012 theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonThemeVisualStudio2012 : public CXTPButtonThemeOffice2013
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonThemeVisualStudio2012 object
	// ----------------------------------------
	CXTPButtonThemeVisualStudio2012(BOOL bLight);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void RefreshMetrics(CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the color used to draw
	//     the button text.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	// Returns:
	//     An RGB value that represents the button text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor(CXTPButton* pButton);

	virtual void DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton);
	virtual void DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton);

protected:
	BOOL m_bLight;
};

#endif //#if !defined(__XTPBUTTONTHEMEVISUALSTUDIO2012_H__)
