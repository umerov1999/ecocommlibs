// XTPComboBoxTheme.h: interface for the CXTPComboBoxTheme class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#ifndef XTPComboBoxTheme_h__
#define XTPComboBoxTheme_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPWinThemeWrapper;
class CXTPComboBox;

//===========================================================================
// Summary:
//     CXTPComboBoxTheme is a class used to perform Button Theme
//     drawing tasks.  This is the base class for all button themes.
//===========================================================================
class _XTP_EXT_CLASS CXTPComboBoxTheme : public CXTPControlTheme
{
protected:

	void UpdateBackgroundBrush();

public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPComboBoxTheme object
	// ----------------------------------------
	CXTPComboBoxTheme();

	// -----------------------------------------------------------
	// Summary:
	//     Destroys a CXTPComboBoxTheme object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------
	virtual ~CXTPComboBoxTheme();

	virtual void FillSolidRect(HDC hdc, int x, int y, int cx, int cy, HBRUSH hBrush);

	virtual void DrawFrame(HDC hdc, LPRECT lprc, int nSize, HBRUSH hBrush);

	virtual BOOL DrawComboBox(CDC* pDC, CXTPComboBox* pComboBox);

	// non-resource themes (GDI themes)
	virtual void DrawBackground(CDC *pDC, CXTPComboBox* pComboBox);

	virtual void DrawButton(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn);

	virtual void DrawButtonTriangle(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pComboBox - Points to a CXTPComboBox object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPComboBox* pComboBox);

	//-----------------------------------------------------------------------
	// Summary      : Call this member function to retrieve the background color
	//                for the CXTPComboBox.
	// Returns      : An RGB color value representing the background color for
	//                the CXTPComboBox.
	// See Also     : SetTextColor, ResetTextColor, SetBackColor, GetTextColor
	//-----------------------------------------------------------------------
	COLORREF GetBackColor() const;

	//-----------------------------------------------------------------------
	// Summary      : Call this member function to set the background color
	//                for the CXTPComboBox.
	// Returns      : An RGB color value representing the old background color
	//                value for the CXTPComboBox.
	// See Also     : SetTextColor, ResetTextColor, GetBackColor, GetTextColor
	//-----------------------------------------------------------------------
	COLORREF SetBackColor(COLORREF clrBackground);

	//-----------------------------------------------------------------------
	// Summary      : Call this member function to reset the background color
	//                for the CXTPComboBox.
	// See Also     : GetTextColor, SetTextColor, GetBackColor, GetTextColor
	//-----------------------------------------------------------------------
	void ResetBackColor();

	//-----------------------------------------------------------------------
	// Summary      : Call this member function to retrieve the text color
	//                for the CXTPComboBox.
	// Returns      : An RGB color value representing the text color for
	//                the CXTPComboBox.
	// See Also     : SetTextColor, ResetTextColor, SetBackColor, GetBackColor
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary      : Call this member function to set the text color
	//                for the CXTPComboBox.
	// Returns      : An RGB color value representing the old text color for
	//                the CXTPComboBox.
	// See Also     : GetTextColor, ResetTextColor, SetBackColor, GetBackColor
	//-----------------------------------------------------------------------
	COLORREF SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary      : Call this member function to reset the text color
	//                for the CXTPComboBox.
	// See Also     : GetTextColor, SetTextColor, SetBackColor, GetBackColor
	//-----------------------------------------------------------------------
	void ResetTextColor();

	const CBrush &GetBackgroundBrush() const;

protected:
	friend class CXTPComboBox;

	CXTPWinThemeWrapper* m_themeComboBox;

	int m_nThumbWidth;

	CXTPPaintManagerColor m_clrBorderDisabled;//does not work
	CXTPPaintManagerColor m_clrBorderHot;
	CXTPPaintManagerColor m_clrBorderNormal;

	CXTPPaintManagerColor m_clrButtonPressed;
	CXTPPaintManagerColor m_clrButtonHot;
	CXTPPaintManagerColor m_clrButtonNormal;//color of button when combobox is disabled

	CXTPPaintManagerColor m_clrButtonArrowPressed;
	CXTPPaintManagerColor m_clrButtonArrowHot;
	CXTPPaintManagerColor m_clrButtonArrowNormal;

	CXTPPaintManagerColor m_clrBackground;  // RGB color value representing background color.
	CXTPPaintManagerColor m_clrText;  // RGB color value representing text color.

	CBrush m_brBackground;
	CXTPPaintManagerColorGradient m_grcButton;
	CXTPPaintManagerColorGradient m_grcButtonHilite;
	CXTPPaintManagerColorGradient m_grcButtonPushed;
};

AFX_INLINE COLORREF CXTPComboBoxTheme::GetBackColor() const
{
	return m_clrBackground;
}

AFX_INLINE COLORREF CXTPComboBoxTheme::SetBackColor(COLORREF clrBackground)
{
	COLORREF clrOldBackground = m_clrBackground;
	m_clrBackground.SetCustomValue(clrBackground);
	return clrOldBackground;
}

AFX_INLINE void CXTPComboBoxTheme::ResetBackColor()
{
	m_clrBackground.SetDefaultValue();
}

AFX_INLINE COLORREF CXTPComboBoxTheme::GetTextColor() const
{
	return m_clrText;
}

AFX_INLINE COLORREF CXTPComboBoxTheme::SetTextColor(COLORREF clrText)
{
	COLORREF clrOldText = m_clrText;
	m_clrText.SetCustomValue(clrText);
	return clrOldText;
}

AFX_INLINE void CXTPComboBoxTheme::ResetTextColor()
{
	m_clrText.SetDefaultValue();
}

AFX_INLINE const CBrush & CXTPComboBoxTheme::GetBackgroundBrush() const
{
	return m_brBackground;
}

#endif // XTPComboBoxTheme_h__
