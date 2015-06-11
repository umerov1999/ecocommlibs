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
#if !defined(__XTPCONTROLGALLERYOFFICE2013THEME_H__)
#define __XTPCONTROLGALLERYOFFICE2013THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//===========================================================================
// Summary:
//     The CXTPControlGalleryResourceTheme class is used to enable a Gallery Office 2013 style theme for Command Bars.
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryOffice2013Theme : public CXTPControlGalleryPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPControlGalleryOffice2013Theme object.
	// Input:   pPaintManager - Reference to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPControlGalleryOffice2013Theme(CXTPPaintManager *pPaintManager);

protected:

	void DrawScrollButton(CDC* pDC, const CRect &rc, int type, CXTPControlGallery* pGallery);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to draw scrollbar of inplace gallery
	// Parameters:
	//     pDC - Pointer to device context
	//     pGallery - CXTPControlGallery object to draw
	// See Also: DrawScrollBar
	//-----------------------------------------------------------------------
	virtual void DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery);

protected:


	COLORREF m_clrScrollTriangle;
	COLORREF m_clrScrollTriangleDisabled;
	COLORREF m_clrScrollTriangleHot;
	COLORREF m_clrScrollTrianglePressed;

	COLORREF m_clrScrollBorderDisabled;
	COLORREF m_clrScrollBorderNormal;
	COLORREF m_clrScrollBorderHot;

	COLORREF m_clrScrollFillHot;
	COLORREF m_clrScrollFillPressed;
};


#endif // !defined(__XTPCONTROLGALLERYOFFICE2013THEME_H__)
