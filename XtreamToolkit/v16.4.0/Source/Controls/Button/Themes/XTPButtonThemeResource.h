//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPBUTTONTHEMERESOURCE_H__)
#define __XTPBUTTONTHEMERESOURCE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTPButtonThemeResource is a class used to perform Office 2007 Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonThemeResource : public CXTPButtonThemeUltraFlat
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonThemeResource object
	// ----------------------------------------
	CXTPButtonThemeResource();

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
	virtual void DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton);
	virtual void DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPButton* pButton);
};

#endif //#if !defined(__XTPBUTTONTHEMERESOURCE_H__)
