// XTPPopupThemeResource.h: interface of the CXTPPopupThemeResource class.
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
#if !defined(__XTPPOPUPTHEMERESOURCE_H__)
#define __XTPPOPUPTHEMERESOURCE_H__
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     v is a class used to perform Office2007
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupThemeResource : public CXTPPopupThemeOffice2003
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupThemeOffice2003 object.
	//-----------------------------------------------------------------------
	CXTPPopupThemeResource();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh draw metrics.
	//-----------------------------------------------------------------------
	void RefreshMetrics();
};

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPPOPUPTHEMERESOURCE_H__
//}}AFX_CODEJOCK_PRIVATE
