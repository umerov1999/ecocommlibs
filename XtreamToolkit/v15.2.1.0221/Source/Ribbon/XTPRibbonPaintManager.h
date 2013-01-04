// XTPRibbonPaintManager.h: interface for the CXTPRibbonPaintManager class.
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
#if !defined(__XTPRIBBONPAINTMANAGER_H__)
#define __XTPRIBBONPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPRibbonGroup;
class CXTPRibbonBar;
class CXTPRibbonTab;
class CXTPResourceImage;
class CXTPResourceImages;
class CXTPTabPaintManager;
class CXTPRibbonTabContextHeaders;
class CXTPPopupToolBar;
class CXTPPaintManager;
class CXTPRibbonControlSystemButton;


//===========================================================================
// Summary: Standalone class used by the CXTPRibbonBar as a paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonPaintManager : public CXTPCmdTarget
{
private:
	class CRibbonAppearanceSet;
	class CRibbonColorSet;

public:

	// --------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonTheme object
	// Parameters:
	//     pPaintManager :  \Returns a CXTPPaintManager object.
	// --------------------------------------------------------
	CXTPRibbonPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonTheme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonPaintManager();

public:

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
	virtual void FillGroupRect(CDC* pDC, CXTPRibbonTab* pActiveTab, CRect rcGroups);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill background of Ribbon Bar control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pRibbonBar - Points to a CXTPRibbonBar object
	//-----------------------------------------------------------------------
	virtual void FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar);

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
	virtual void DrawGroup(CDC* pDC, CXTPRibbonGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the height of the group caption bar.
	// Returns:
	//     Integer containing the height of the group caption bar.
	// See Also: CXTPRibbonGroup::RepositionControls, CXTPRibbonGroup::SetRect
	//-----------------------------------------------------------------------
	int GetGroupCaptionHeight() const;

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
	virtual CSize DrawControlGroupPopup(CDC* pDC, CXTPControlPopup* pControlGroupPopup, BOOL bDraw);

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
	virtual CSize DrawControlGroupOption(CDC* pDC, CXTPControl* pControlGroupOption, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw single tab of ribbon bar
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pItem - Ribbon tab to draw
	//-----------------------------------------------------------------------
	virtual void DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member fills in the tab header and tab client area.  It
	//     will then draw tab client frame.
	// Parameters:
	//     pRibbonBar - Pointer to ribbon to draw the tab control.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the tab control.
	//-----------------------------------------------------------------------
	virtual void FillRibbonTabControl(CDC* pDC, CXTPRibbonBar* pRibbonBar, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw popup toolbar of reduced group
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Parent group of popuped toolbar
	//     pCommandBar - popup toolbar to draw
	//-----------------------------------------------------------------------
	virtual void FillGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pGroup, CXTPCommandBar* pCommandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw background of minimzed popup bar
	// Parameters:
	//     pDC - Pointer to device context
	//     pPopupToolBar - Pointer to popup toolbar.
	//-----------------------------------------------------------------------
	void FillTabPopupToolBar(CDC* pDC, CXTPPopupToolBar* pPopupToolBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to Draw Ribbon caption
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pRibbonBar - Ribbon Bar to draw
	//     bActive - TRUE if ribbon frame is active
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon caption if EnableFrameTheme was not called.
	// Parameters:
	//     pDC -  Pointer to a valid device context
	//     pRibbonBar - Ribbon Bar to draw
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon frame if EnableFrameTheme was not called.
	// Parameters:
	//     pDC -  Pointer to a valid device context
	//     pRibbonBar - Ribbon Bar to draw
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFramePart(CDC* pDC, CXTPRibbonBar* pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws top-left system button
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControl - System button control
	//     rc - Bounding rectangle of control
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameSystemButton(CDC* pDC, CXTPRibbonControlSystemButton* pControl, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon context headers
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pContextHeaders - Collection of context header
	//     pRibbonBar - Parent RibbonBar pointer
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameContextHeaders(CDC* pDC, CXTPRibbonBar* pRibbonBar, CXTPRibbonTabContextHeaders* pContextHeaders);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw quick access customize button
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControl - Pointer to Quick Access customize button
	// See Also: DrawRibbonQuickAccessMoreButton
	//-----------------------------------------------------------------------
	virtual void DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw quick access more button
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControl - Pointer to Quick Access more button
	// See Also: DrawRibbonQuickAccessButton
	//-----------------------------------------------------------------------
	virtual void DrawRibbonQuickAccessMoreButton(CDC* pDC, CXTPControlPopup* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw scroll groups buttons
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pControl - Scroll button to draw
	//     bScrollLeft - TRUE to draw left button; FALSE - right
	//-----------------------------------------------------------------------
	virtual void DrawRibbonScrollButton(CDC* pDC, CXTPControl* pControl, BOOL bScrollLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves edit control height
	//-----------------------------------------------------------------------
	int GetEditHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set font height used for Ribbon
	// Parameters:
	//     nFontHeight - New font height
	// See Also: GetFontHeight
	//-----------------------------------------------------------------------
	void SetFontHeight(int nFontHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get font height used for Ribbon
	// Returns:
	//     Font height used for Ribbon
	// See Also: SetFontHeight
	//-----------------------------------------------------------------------
	int GetFontHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar of more button
	// Parameters:
	//      pDC - Pointer to a valid device context
	//      pBar - ToolBar to fill
	//-----------------------------------------------------------------------
	virtual void FillMorePopupToolBarEntry(CDC* pDC, CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves ribbon caption height
	//-----------------------------------------------------------------------
	int GetRibbonCaptionHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads a bitmap image from the specified path.
	// Parameters:
	//     lpszFileName - The name of the file that contains the bitmaps
	//                    to load.
	// Remarks:
	//     The images are the bitmaps that represent all the visual components
	//     of the Ribbon Bar.  For example tab buttons, group buttons, menu buttons,
	//     toolbar buttons, option button, toolbar dropdown, etc.
	//     The images are loaded using LoadImage and are stored in the m_pImages
	//     image collection.
	//     Images for the Office 2007 theme can be found in the \Source\Ribbon\Res
	//     folder.
	// Returns: New CXTPResourceImage containing the loaded bitmap.
	// See Also: SetImageHandle, m_pImages
	//-----------------------------------------------------------------------
	CXTPResourceImage* LoadImage(LPCTSTR lpszFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get a CXTPResourceImages pointer.
	// Returns:
	//     A CXTPResourceImages pointer.
	//-----------------------------------------------------------------------
	CXTPResourceImages* GetImages() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw caption text for Aero transparent caption
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     rcCaptionText - Caption text rectangle
	//     strText - Caption text
	//     pSite - Frame to draw
	//     bActive - TRUE if frame is active
	//-----------------------------------------------------------------------
	void DrawDwmCaptionText(CDC* pDC, CRect rcCaptionText, CString strText, CWnd* pSite, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar of the system button
	// Parameters:
	//      pDC - Pointer to a valid device context
	//      pBar - ToolBar to fill
	//-----------------------------------------------------------------------
	virtual void FillSystemPopupBarEntry(CDC* pDC, CXTPPopupBar* pBar);

	// ---------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar buttons
	// Parameters:
	//     pDC :      Pointer to a valid device context
	//     pButton :  Popup bar button to draw
	//
	// ---------------------------------------------------
	virtual void DrawSystemPopupBarButton(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the ribbon group when the group is minimized
	//     and only a button is displayed.  When the button is pressed the group is
	//     displayed as a popup.
	// Parameters:
	//      pDC - Pointer to a valid device context
	//      pButton - Button to draw
	//-----------------------------------------------------------------------
	virtual BOOL DrawRibbonGroupControlEntry(CDC* pDC, CXTPControl* pButton);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the tab paint manager.
	//     The tab paint manager is used to customize the appearance of
	//     CXTPTabManagerItem objects and the tab manager.  I.e. Tab colors,
	//     styles, etc...  This member must be overridden in
	//     derived classes.
	// Returns:
	//     Pointer to CXTPTabPaintManager that contains the visual elements
	//     of the tabs.
	//-----------------------------------------------------------------------
	CXTPTabPaintManager* GetTabPaintManager() const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a CXTPTabPaintManager object used to
	//     store the color and appearance information of the tabs in the
	//     Ribbon Bar.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* CreateTabPaintManager();

	// ---------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar buttons.
	// Parameters:
	//     pDC :      Pointer to a valid device context.
	//     pRibbonBar - Pointer to ribbon to draw the
	//                  application icon.
	//     rc :      Bounding rectangle of icon.
	//     hIcon :   Pointer to the application icon to use.
	// ---------------------------------------------------
	void DrawRibbonApplicationIcon(CDC* pDC, CXTPRibbonBar* pRibbonBar, CRect rc, HICON hIcon);




public:
	BOOL m_bHotTrackingGroups;              // TRUE to highlight group under cursor
	int m_nTabsHeight;                      // Tabs heights
	CXTPPaintManagerFont m_fontGroupCaption;// Font used to draw text in the group caption

	COLORREF m_clrRecentFileListEdgeShadow;     // Color of the File List edge shadow
	COLORREF m_clrRecentFileListEdgeHighLight;  // Color of the File List edge highlight

	int m_nGroupSpacing;                        // Group spacing

protected:
	int m_nGroupCaptionHeight;              // Group caption bar height

	CXTPTabPaintManager* m_pTabPaintManager;// Tab paint manager

protected:
	COLORREF m_clrRibbonFace;               // Ribbon face color
	COLORREF m_clrRibbonGroupCaptionText;   // Ribbon group captions color

	COLORREF m_clrFileButtonText;       // Text caption to display when the system button is displayed as a tab button (Windows 7 style).  Button style must be button caption.

	COLORREF m_clrMinimizedFrameEdgeHighLight;      // Minimzed bottom edge
	COLORREF m_clrMinimizedFrameEdgeShadow;         // Minimzed bottom edge shadow
	CXTPPaintManagerColorGradient m_clrMenuGripper; // Color gradient of the menu gripper

	CXTPPaintManager* m_pPaintManger;  // Paint manager used for the ribbon

	int m_nRibbonCaptionHeight;       // Height of the caption
	COLORREF m_clrRibbonInactiveFace; // Color of the ribbon when it is inactive

	CRect m_rcFileButtonMargin; // Margins of the system button when it is displayed as a tab button (Windows 7 style).

	int m_nFileButtonImageCount;

public:
	BOOL m_bFlatFrame;  // TRUE to draw a flat frame.

private:
	HICON m_hApplicationIcon;
	BOOL m_bAlphaApplicationIcon;

protected:

	friend class CXTPRibbonBar;
};


AFX_INLINE int CXTPRibbonPaintManager::GetGroupCaptionHeight() const {
	return m_nGroupCaptionHeight;
}
AFX_INLINE CXTPTabPaintManager* CXTPRibbonPaintManager::GetTabPaintManager() const {
	return m_pTabPaintManager;
}
AFX_INLINE int CXTPRibbonPaintManager::GetRibbonCaptionHeight() const {
	return m_nRibbonCaptionHeight;
}

#endif // !defined(__XTPRIBBONPAINTMANAGER_H__)
