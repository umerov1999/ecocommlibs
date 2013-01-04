// XTPRibbonSystemTheme.h: interface for the CXTPRibbonSystemTheme class.
//
// This file is a part of the XTREME RIBBON MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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
#if !defined(__XTPRIBBONSYSTEMTHEME_H__)
#define __XTPRIBBONSYSTEMTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupContext;


//===========================================================================
// Summary: CXTPRibbonSystemTheme class
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonSystemTheme : public CXTPRibbonPaintManager
{
public:

	// --------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonSystemTheme object
	// Parameters:
	//     pPaintManager :  \Returns a CXTPPaintManager object.
	// --------------------------------------------------------
	CXTPRibbonSystemTheme(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonSystemTheme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonSystemTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to Draw Ribbon caption
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pRibbonBar - Ribbon Bar to draw
	//     bActive - TRUE if ribbon frame is active
	//-----------------------------------------------------------------------
	void DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill background of Ribbon Bar control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pRibbonBar - Points to a CXTPRibbonBar object
	//-----------------------------------------------------------------------
	void FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar);

	//-------------------------------------------------------------------------
	// Summary:
	//      This method is called to refresh the visual metrics of manager.
	// Remarks:
	//      Refreshes all of the colors in the Ribbon Bar.  Many of the colors
	//      used are set with the GROUP_COLOR structure.
	// See Also:
	//     GROUP_COLOR
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill background of a CXTPRibbonGroup.
	// Parameters:
	//               pDC - Pointer to a valid device context
	//               pActiveTab - Pointer to the tab that is currently active
	//               rcGroups - Bounding rectangle of the group
	// Remarks:
	//      Performs a gradient fill using the GROUP_COLOR::clrBackground for
	//      the main group background and GROUP_COLOR::clrCaption to fill the
	//      group caption rectangle.
	// See Also: CXTPRibbonGroup::GetRect, GROUP_COLOR
	//-----------------------------------------------------------------------
	void FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab, CRect rcGroups);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the specified CXTPRibbonGroup.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Pointer to the CXTPRibbonGroup to be drawn
	// Remarks:
	//     Draws the entire groups including background, caption and text.
	//     This will not draw the option button or group popup.
	// See Also: GROUP_COLOR, FillRibbonBar, FillGroupRect, RefreshMetrics
	//-----------------------------------------------------------------------
	void DrawGroup(CDC* pDC, CXTPRibbonGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Renders and markup strings in the ribbon.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc - Bounding rectangle for the markup.
	//     lpszText - Markup string to render.
	//-----------------------------------------------------------------------
	void RenderMarkup(CDC* pDC, CRect rc, LPCTSTR lpszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group option button.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControlGroupOption - Pointer to a CXTPControl object.
	//     bDraw - TRUE to draw option button; FALSE to retrieve the size of the control.
	// Remarks:
	//     The option button is typically used to launch a dialog associated with the group.
	//     It is drawn on the right side of the group caption.
	// Returns:
	//     If bDraw is TRUE, then the size of the option button control is returned.
	// See Also: CXTPRibbonGroup::ShowOptionButton, CXTPRibbonGroup::GetControlGroupOption
	//-----------------------------------------------------------------------
	CSize DrawControlGroupOption(CDC* pDC, CXTPControl* pControlGroupOption, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the group popup button.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControlGroupPopup - Pointer to a CXTPControlPopup object.
	//     bDraw - TRUE to draw group popup button; FALSE to retrieve the size of the control.
	// Remarks:
	//     A single group button will be displayed if there is not enough
	//     room to display the items in the group.  A group popup is displayed
	//     when the button is clicked.  The popup contains all the items in
	//     the group.  When a group button is clicked, a CXTPControlPopup
	//     is displayed that contains the items of the group.
	// Returns:
	//     If bDraw is TRUE, then the size of the group popup button is returned.
	// See Also: CXTPRibbonGroup::GetControlGroupPopup
	//-----------------------------------------------------------------------
	CSize DrawControlGroupPopup(CDC* pDC, CXTPControlPopup* pControlGroupPopup, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw popup toolbar of reduced group
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Parent group of popuped toolbar
	//     pCommandBar - popup toolbar to draw
	//-----------------------------------------------------------------------
	void FillGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pGroup, CXTPCommandBar* pCommandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw quick access customize button
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControl - Pointer to Quick Access customize button
	// See Also: DrawRibbonQuickAccessMoreButton
	//-----------------------------------------------------------------------
	void DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw quick access more button
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControl - Pointer to Quick Access more button
	// See Also: DrawRibbonQuickAccessButton
	//-----------------------------------------------------------------------
	void DrawRibbonQuickAccessMoreButton(CDC* pDC, CXTPControlPopup* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar of more button
	// Parameters:
	//      pDC - Pointer to a valid device context
	//      pBar - ToolBar to fill
	//-----------------------------------------------------------------------
	void FillMorePopupToolBarEntry(CDC* pDC, CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw single tab of ribbon bar
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pItem - Ribbon tab to draw
	//-----------------------------------------------------------------------
	void DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar of the system button
	// Parameters:
	//      pDC - Pointer to a valid device context
	//      pBar - ToolBar to fill
	//-----------------------------------------------------------------------
	void FillSystemPopupBarEntry(CDC* pDC, CXTPPopupBar* pBar);

	// ---------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar buttons
	// Parameters:
	//     pDC :      Pointer to a valid device context
	//     pButton :  Popup bar button to draw
	//
	// ---------------------------------------------------
	void DrawSystemPopupBarButton(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw scroll groups buttons
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControl - Scroll button to draw
	//     bScrollLeft - TRUE to draw left button; FALSE - right
	//-----------------------------------------------------------------------
	void DrawRibbonScrollButton(CDC* pDC, CXTPControl* pControl, BOOL bScrollLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws top-left system button
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControl - System button control
	//     rc - Bounding rectangle of control
	//-----------------------------------------------------------------------
	void DrawRibbonFrameSystemButton(CDC* pDC, CXTPRibbonControlSystemButton* pControl, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon group when the group is minimized
	//     and only a button is displayed.  When the button is pressed the group is
	//     displayed as a popup.
	// Parameters:
	//      pDC - Pointer to a valid device context
	//      pButton - Button to draw
	//-----------------------------------------------------------------------
	BOOL DrawRibbonGroupControlEntry(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon caption if EnableFrameTheme was not called
	// Parameters:
	//     pDC -  Pointer to a valid device context
	//     pRibbonBar - Ribbon Bar to draw
	//-----------------------------------------------------------------------
	void DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar);

protected:
	CXTPMarkupContext* m_pMarkupContext;  // Store pointer to CXTPMarkupContext object.

};

#endif // !defined(__XTPRIBBONSYSTEMTHEME_H__)
