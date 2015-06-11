// XTPDockingPaneDefaultTheme.h : interface for the CXTPDockingPaneDefaultTheme class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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
#if !defined(__XTPDOCKINGPANEDEFAULTTHEME_H__)
#define __XTPDOCKINGPANEDEFAULTTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//===========================================================================
// Summary:
//     CXTPDockingPaneDefaultTheme is CXTPDockingPanePaintManager derived class, represents
//     classic theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeDefault); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneDefaultTheme : public CXTPDockingPanePaintManager
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneDefaultTheme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneDefaultTheme();
public:
	// -----------------------------------------------------
	// Summary:
	//     Call this method to draw frame of floating window
	// Parameters:
	//     pDC :    Pointer to a valid device context
	//     pPane :  Floating window pointer
	//     rc :     CLient rectangle of floating frame
	//
	// -----------------------------------------------------
	void DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc);

	// --------------------------------------------------------------
	// Summary:
	//     Override this member function to draw a caption.
	// Parameters:
	//     pDC :    Reference to the device context in which to draw.
	//     pPane :  Pointer to the tabbed container.
	//     rc :     Client rectangle of the tabbed container.
	//
	// --------------------------------------------------------------
	virtual void DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc);

	// --------------------------------------------------------------
	// Summary:
	//     Override this member function to draw the side panel.
	// Parameters:
	//     pDC :    Reference to the device context in which to draw.
	//     pPane :  Pointer to the side panel.
	//     rc :     Client rectangle of the side panel.
	//
	// --------------------------------------------------------------
	virtual void DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
	// ------------------------------------------------------------------
	// Summary:
	//     Draw common part of tabbed and floating caption.
	// Parameters:
	//     pDC :        Reference to the device context in which to draw.
	//     pPane :      Container which caption need to draw.
	//     rcCaption :  Caption bounding rectangle
	//     strTitle :   Caption text.
	//     bActive :    TRUE if caption is active.
	//     bVertical :  TRUE to draw caption vertically
	//
	// ------------------------------------------------------------------
	virtual void DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical);
};

#endif // #if !defined(__XTPDOCKINGPANEDEFAULTTHEME_H__)
