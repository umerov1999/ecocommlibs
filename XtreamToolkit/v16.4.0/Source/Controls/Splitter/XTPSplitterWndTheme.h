// XTPSplitterWndTheme.h: interface for the CXTPSplitterWndTheme class.
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
#if !defined(__XTPSPLITTERTHEME_H__)
#define __XTPSPLITTERTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPSplitterWnd;

//===========================================================================
// Summary:
//     Class CXTPSplitterWndTheme is derived from CXTPControlTheme.
//     This class is used to apply a Theme to splitter windows.
//===========================================================================
class _XTP_EXT_CLASS CXTPSplitterWndTheme : public CXTPControlTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Construct a CXTPSplitterWndTheme object.
	//-----------------------------------------------------------------------
	CXTPSplitterWndTheme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the splitter windows face and the
	//     borders to the system default colors.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the splitter window using the
	//     specified theme.
	// Parameters:
	//     pDC       - Pointer to the device context to draw to.
	//     pSplitter - Points to the owner splitter window.
	//     nType     - Index value representing the splitter type, splitBox (0),
	//                 splitBar (1), splitIntersection (2) or splitBorder (3).
	//     rectArg   - Size of the splitter area to draw.
	//-------------------------------------------------------------------------
	virtual void DrawSplitter(CDC* pDC, CXTPSplitterWnd* pSplitter, int nType, const CRect& rectArg);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to use tab background color from resource.
	// Parameters:
	//     bEnable - TRUE to enable tab background color for splitter window.
	//-----------------------------------------------------------------------
	void EnableTabColors(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve a pointer to the brush used by the
	//     split tracker.
	// Returns:
	//     A pointer to a CBrush object representing the split tracker.
	//-------------------------------------------------------------------------
	virtual CBrush* GetTrackBrush();

public:

	BOOL m_bTabColors;  // TRUE to use tab background color from resource.
	CXTPPaintManagerColorGradient m_crBack;           // Splitter window background color.
	CXTPPaintManagerColorGradient m_crBoxBack;        // Splitter box window background color.
	CXTPPaintManagerColorGradient m_crOuterBorder;    // Splitter window outer border color.
	CXTPPaintManagerColorGradient m_crOuterBoxBorder; // Splitter box window outer border color.
	CXTPPaintManagerColorGradient m_crInnerBorder;    // Splitter window inner border color.
	CXTPPaintManagerColorGradient m_crInnerBoxBorder; // Splitter box window inner border color.
};

//===========================================================================
// Summary:
//     Class CXTPSplitterWndThemeOfficeXP is derived from CXTPSplitterWndTheme.
//     This class is used to implement the Office 2003 theme for splitter
//     windows.
//===========================================================================
class _XTP_EXT_CLASS CXTPSplitterWndThemeOfficeXP : public CXTPSplitterWndTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Construct a CXTPSplitterWndThemeOfficeXP object.
	//-----------------------------------------------------------------------
	CXTPSplitterWndThemeOfficeXP();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the splitter windows face and the
	//     borders to the system default colors.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve a pointer to the brush used by the
	//     split tracker.
	// Returns:
	//     A pointer to a CBrush object representing the split tracker.
	//-------------------------------------------------------------------------
	virtual CBrush* GetTrackBrush();
};

//===========================================================================
// Summary:
//     Class CXTPSplitterWndThemeOffice2003 is derived from CXTPSplitterWndTheme.
//     This class is used to implement the Office 2003 theme for splitter
//     windows.
//===========================================================================
class _XTP_EXT_CLASS CXTPSplitterWndThemeOffice2003: public CXTPSplitterWndThemeOfficeXP
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Construct a CXTPSplitterWndThemeOffice2003 object.
	//-----------------------------------------------------------------------
	CXTPSplitterWndThemeOffice2003();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the splitter windows face and the
	//     borders to the system default colors.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the splitter window using the
	//     specified theme.
	// Parameters:
	//     pDC       - Pointer to the device context to draw to.
	//     pSplitter - Points to the owner splitter window.
	//     nType     - Index value representing the splitter type, splitBox (0),
	//                 splitBar (1), splitIntersection (2) or splitBorder (3).
	//     rectArg   - Size of the splitter area to draw.
	//-------------------------------------------------------------------------
	virtual void DrawSplitter(CDC* pDC, CXTPSplitterWnd* pSplitter, int nType, const CRect& rectArg);
};

//===========================================================================
// Summary:
//     Class CXTPSplitterWndThemeResource is derived from CXTPSplitterWndTheme.
//     This class is used to implement the Office 2007/2010 theme for splitter
//     windows.
//===========================================================================
class _XTP_EXT_CLASS CXTPSplitterWndThemeResource: public CXTPSplitterWndThemeOffice2003
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Construct a CXTPSplitterWndThemeResource object.
	//-----------------------------------------------------------------------
	CXTPSplitterWndThemeResource();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the splitter windows face and the
	//     borders to the system default colors.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     Class CXTPSplitterWndThemeVisualStudio2010 is derived from CXTPSplitterWndTheme.
//     This class is used to implement the Office 2003 theme for splitter
//     windows.
//===========================================================================
class _XTP_EXT_CLASS CXTPSplitterWndThemeVisualStudio2010 : public CXTPSplitterWndThemeOffice2003
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Construct a CXTPSplitterWndThemeVisualStudio2010 object.
	//-----------------------------------------------------------------------
	CXTPSplitterWndThemeVisualStudio2010();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the splitter windows face and the
	//     borders to the system default colors.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the splitter window using the
	//     specified theme.
	// Parameters:
	//     pDC       - Pointer to the device context to draw to.
	//     pSplitter - Points to the owner splitter window.
	//     nType     - Index value representing the splitter type, splitBox (0),
	//                 splitBar (1), splitIntersection (2) or splitBorder (3).
	//     rectArg   - Size of the splitter area to draw.
	//-------------------------------------------------------------------------
	virtual void DrawSplitter(CDC* pDC, CXTPSplitterWnd* pSplitter, int nType, const CRect& rectArg);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTPSPLITTERTHEME_H__)
