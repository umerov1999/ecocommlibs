// XTPFrameThemeOffice2007System.h : interface for the CXTPFrameThemeOffice2007System class.
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
#if !defined(__XTPRIBBONOFFICE2007SYSTEMFRAMETHEME_H__)
#define __XTPRIBBONOFFICE2007SYSTEMFRAMETHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

CXTPMarkupThemePart;

//===========================================================================
// Summary:
//     The CXTPFrameThemeOffice2007System class is used to enable a system style theme for Command Bars.
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPFrameThemeOffice2007System : public CXTPRibbonMarkupFrameTheme
{
// Construction / Destruction
public:

	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPFrameThemeOffice2007System object.
	// Input:   pPaintManager - Reference to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPFrameThemeOffice2007System(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFrameThemeOffice2007System object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPFrameThemeOffice2007System();

// Frame border
public:

	virtual HRGN CalcFrameRegion(CXTPCommandBarsFrameHook *pFrameHook, CSize sz) const;

	virtual CRect CalcFrameBorder(const CXTPCommandBarsFrameHook *pFrameHook) const;

public:

	virtual CXTPMarkupThemePart* GetFrameBorder(int nId, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame caption bar.
	// Parameters:
	//     pDC - Pointer to device context
	//     rcCaptionBar - Caption bar rectangle
	//     pSite - Frame to draw
	//     bActive - TRUE if frame is active
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBar(CDC *pDC, CRect rcCaptionBar, CWnd *pSite, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw caption button
	// Parameters:
	//      pDC - Pointer to device context
	//      rc - Bounding rectangle of the button
	//      nId - Identifier of the button
	//      bSelected - TURE if button is highlighted
	//      bPressed -  TURE if button is pressed
	//      bActive - TURE if frame is active
	//-----------------------------------------------------------------------
	void DrawFrameCaptionButton(CDC* pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed, BOOL bActive);

	CRect GetCaptionButtonMargin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	virtual void LoadParts(HZIP hZip, CXTPMarkupContext *pMarkupContext);

protected:

	BOOL LoadResource();

	BOOL m_bLoaded; // Indicates if the frame parts have been loaded

	CXTPMarkupThemePart *m_pCaptionActive;
	CXTPMarkupThemePart *m_pCaptionInactive;
	CXTPMarkupThemePart *m_pBorderLeftActive;
	CXTPMarkupThemePart *m_pBorderLeftInactive;
	CXTPMarkupThemePart *m_pBorderLeftCaptionActive;
	CXTPMarkupThemePart *m_pBorderLeftCaptionInactive;
	CXTPMarkupThemePart *m_pBorderTopActive;
	CXTPMarkupThemePart *m_pBorderTopInactive;
	CXTPMarkupThemePart *m_pBorderRightActive;
	CXTPMarkupThemePart *m_pBorderRightInactive;
	CXTPMarkupThemePart *m_pBorderRightCaptionActive;
	CXTPMarkupThemePart *m_pBorderRightCaptionInactive;
	CXTPMarkupThemePart *m_pBorderBottomActive;
	CXTPMarkupThemePart *m_pBorderBottomInactive;

	CXTPMarkupThemePart *m_pCaptionButtonSelected;
	CXTPMarkupThemePart *m_pCaptionButtonPressed;


	CXTPMarkupThemePart *m_pGlyphMinimize9;
	CXTPMarkupThemePart *m_pGlyphMinimize11;
	CXTPMarkupThemePart *m_pGlyphMaximize9;
	CXTPMarkupThemePart *m_pGlyphMaximize11;
	CXTPMarkupThemePart *m_pGlyphRestore9;
	CXTPMarkupThemePart *m_pGlyphRestore11;
	CXTPMarkupThemePart *m_pGlyphClose9;
	CXTPMarkupThemePart *m_pGlyphClose11;

	CXTPMarkupContext* m_pMarkupContext;  // Store pointer to CXTPMarkupContext object.
};

#endif // !defined(__XTPRIBBONOFFICE2007SYSTEMFRAMETHEME_H__)
