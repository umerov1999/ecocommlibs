// XTPRibbonOffice2013Theme.h: interface for the CXTPRibbonOffice2013Theme class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPRIBBONOFFICE2013THEME_H__)
#define __XTPRIBBONOFFICE2013THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPRibbonOffice2013Theme : public CXTPRibbonMarkupTheme
{
public:

	// --------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonOffice2013Theme object
	// Parameters:
	//     pPaintManager :  \Returns a CXTPPaintManager object.
	// --------------------------------------------------------
	CXTPRibbonOffice2013Theme(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonOffice2013Theme object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPRibbonOffice2013Theme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//      This method is called to refresh the visual metrics of manager.
	// Remarks:
	//      Refreshes all of the colors in the Ribbon Bar.  Many of the colors
	//      used are set with the GROUP_COLOR structure.
	// See Also:
	//     GROUP_COLOR
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates ribbon caption height
	//-----------------------------------------------------------------------
	virtual int GetRibbonCaptionHeight(const CXTPRibbonBar *pRibbonBar) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw single tab of ribbon bar
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pItem - Ribbon tab to draw
	//-----------------------------------------------------------------------
	virtual void DrawRibbonTab(CDC *pDC, CXTPRibbonTab *pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon context headers
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pContextHeaders - Collection of context header
	//     pRibbonBar - Parent RibbonBar pointer
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameContextHeaders(CDC *pDC, CXTPRibbonBar *pRibbonBar, CXTPRibbonTabContextHeaders *pContextHeaders);

private:

	BOOL IsGdiplusInitialized() const;

	ULONG_PTR m_GdiPlusToken;
};

#endif // !defined(__XTPRIBBONOFFICE2013THEME_H__)
