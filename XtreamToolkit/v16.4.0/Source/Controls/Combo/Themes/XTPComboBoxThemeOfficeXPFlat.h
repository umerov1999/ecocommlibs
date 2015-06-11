// XTPComboBoxThemeOfficeXP.h: interface for the CXTPComboBoxThemeOfficeXPFlat class.
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
#ifndef XTPComboBoxThemeOfficeXP_h__
#define XTPComboBoxThemeOfficeXP_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPComboBoxTheme is a class used to perform Button Theme
//     drawing tasks.  This is the base class for all button themes.
//===========================================================================
class _XTP_EXT_CLASS CXTPComboBoxThemeOfficeXPFlat : public CXTPComboBoxTheme
{

public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPComboBoxTheme object
	// CXTPComboBoxThemeOfficeXPFlat
	CXTPComboBoxThemeOfficeXPFlat(XTPControlTheme nTheme);

	// -----------------------------------------------------------
	// Summary:
	//     Destroys a CXTPComboBoxTheme object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------
	virtual ~CXTPComboBoxThemeOfficeXPFlat();

	virtual void DrawBackground(CDC *pDC, CXTPComboBox* pComboBox);

	virtual void DrawButton(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	// Parameters:
	//     pComboBox - Points to a CXTPComboBox object.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPComboBox* pComboBox);

	XTPControlTheme m_nTheme; // Office XP, Flat or Ultra Flat
};


#endif // XTPComboBoxThemeOfficeXP_h__
