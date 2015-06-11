// XTPComboBoxThemeOffice2000.h: interface for the CXTPComboBoxThemeOffice2000 class.
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
#ifndef XTPComboBoxThemeOffice2000_h__
#define XTPComboBoxThemeOffice2000_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPComboBoxTheme is a class used to perform Button Theme
//     drawing tasks.  This is the base class for all button themes.
//===========================================================================
class _XTP_EXT_CLASS CXTPComboBoxThemeOffice2000 : public CXTPComboBoxTheme
{
public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTPComboBoxTheme object
	// CXTPComboBoxThemeOffice2000
	CXTPComboBoxThemeOffice2000();

	// -----------------------------------------------------------
	// Summary:
	//     Destroys a CXTPComboBoxTheme object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------
	virtual ~CXTPComboBoxThemeOffice2000();

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
};


#endif // XTPComboBoxThemeOffice2000_h__
