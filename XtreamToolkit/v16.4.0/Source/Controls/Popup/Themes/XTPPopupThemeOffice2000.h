// XTPPopupThemeOffice2000.h: interface of the CXTPPopupThemeOffice2000 class.
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
#if !defined(__XTPPOPUPTHEMEOFFICE2000_H__)
#define __XTPPOPUPTHEMEOFFICE2000_H__
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPPopupThemeOffice2000 is a class used to perform Office2000
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupThemeOffice2000 : public CXTPPopupPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupThemeOffice2000 object.
	//-----------------------------------------------------------------------
	CXTPPopupThemeOffice2000();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupThemeOffice2000 object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupThemeOffice2000();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh draw metrics.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function will draw the background of popup window.
	// Parameters:
	//     pDC - Points to the device context.
	//     rcClient - Size of area to fill.
	//     pControl - Popup control to draw
	//-----------------------------------------------------------------------
	virtual void DrawBackground(CDC* pDC, CXTPPopupControl* pControl, CRect rcClient);
};

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPPOPUPTHEMEOFFICE2000_H__
//}}AFX_CODEJOCK_PRIVATE
