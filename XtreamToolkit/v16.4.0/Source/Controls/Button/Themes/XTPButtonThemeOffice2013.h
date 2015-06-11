//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPBUTTONTHEMEOFFICE2013_H__)
#define __XTPBUTTONTHEMEOFFICE2013_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTPButtonThemeOffice2013 is a class used to perform Office 2013 theme
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPButtonThemeOffice2013 : public CXTPButtonThemeUltraFlat
{
public:
	CXTPButtonThemeOffice2013();

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
	virtual void RefreshMetrics(CXTPButton *pButton);
};

#endif //#if !defined(__XTPBUTTONTHEMEOFFICE2013_H__)
