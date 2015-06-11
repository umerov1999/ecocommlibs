// XTPControlGalleryOffice2013Theme.h : interface for the CXTPControlGalleryOffice2013Theme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPSCROLLBARTHEMEOFFICE2013_H__)
#define __XTPSCROLLBARTHEMEOFFICE2013_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



struct CXTPScrollBarColorsOffice2013
{
	COLORREF m_clrBackground;

	COLORREF m_clrButtonBorder;
	COLORREF m_clrButtonBackground;
	COLORREF m_clrButtonArrow;

	COLORREF m_clrThumbBorder;
	COLORREF m_clrThumbBackground;
};

//===========================================================================
// Summary:
//     The CXTPControlGalleryResourceTheme class is used to enable a Gallery Office 2013 style theme for Command Bars.
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPScrollBarThemeOffice2013 : public CXTPScrollBarPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPControlGalleryOffice2013Theme object.
	// Input:   pPaintManager - Reference to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPScrollBarThemeOffice2013();

	virtual ~CXTPScrollBarThemeOffice2013();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw scrollbar of the gallery.
	// Parameters:
	//     pDC - Pointer to device context.
	//     pGallery - ScrollBar to draw.
	//-----------------------------------------------------------------------
	virtual void DrawScrollBar(CDC *pDC, CXTPScrollBase *pGallery);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:

	CXTPScrollBarColorsOffice2013 m_colorNormal;
	CXTPScrollBarColorsOffice2013 m_colorSelected;
	CXTPScrollBarColorsOffice2013 m_colorPressed;
};


#endif // !defined(__XTPSCROLLBARTHEMEOFFICE2013_H__)
