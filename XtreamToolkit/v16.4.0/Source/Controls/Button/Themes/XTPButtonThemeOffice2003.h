//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPBUTTONTHEMEOFFICE2003_H__)
#define __XTPBUTTONTHEMEOFFICE2003_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTPButtonThemeOffice2003 is a class used to perform Office 2003 Theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonThemeOffice2003 : public CXTPButtonThemeOfficeXP
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPButtonThemeOffice2003 object
	// ----------------------------------------
	CXTPButtonThemeOffice2003();

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
	void DrawButtonBackground(CDC *pDC, CXTPButton *pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pButton - Points to a CXTPButton object.
	//-----------------------------------------------------------------------
	void RefreshMetrics(CXTPButton *pButton);

public:
	BOOL m_bLunaTheme;
};

#endif //#if !defined(__XTPBUTTONTHEMEOFFICE2003_H__)
