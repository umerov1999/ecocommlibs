// XTPRibbonMarkupFrameTheme.h : interface for the CXTPRibbonMarkupFrameTheme class.
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
#if !defined(__XTPRIBBONMARKUPFRAMETHEME_H__)
#define __XTPRIBBONMARKUPFRAMETHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPMarkupThemePart;

//===========================================================================
// Summary:
//     The CXTPRibbonMarkupFrameTheme class is used to enable a system style theme for Command Bars.
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonMarkupFrameTheme : public CXTPFramePaintManager
{
protected:

	enum XTPBorderPart
	{
		xtpBorderPartTop,
		xtpBorderPartLeft,
		xtpBorderPartLeftCaption,
		xtpBorderPartRight,
		xtpBorderPartRightCaption,
		xtpBorderPartBottom,
		xtpBorderPartCount
	};

public:

	//-----------------------------------------------------------------------
	// Summary: Constructs a CXTPRibbonMarkupFrameTheme object.
	// Input:   pPaintManager - Reference to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPRibbonMarkupFrameTheme(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonMarkupFrameTheme object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonMarkupFrameTheme();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame if Ribbon was found
	// Parameters:
	//      pDC - Pointer to device context
	//      pFrameHook - CXTPCommandBarsFrameHook hook object
	//-----------------------------------------------------------------------
	virtual void DrawFrame(CDC *pDC, CXTPCommandBarsFrameHook *pFrameHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame border
	// Parameters:
	//      pDC - Pointer to device context
	//      pFrameHook - CXTPCommandBarsFrameHook hook object
	//-----------------------------------------------------------------------
	virtual void DrawFrameBorder(CDC *pDC, CXTPCommandBarsFrameHook *pFrameHook);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the frame border part
	//-----------------------------------------------------------------------
	virtual CXTPMarkupThemePart* GetFrameBorder(int nId, BOOL bActive) = 0;
};

#endif // !defined(__XTPRIBBONMARKUPFRAMETHEME_H__)
