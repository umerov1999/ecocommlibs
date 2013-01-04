// XTPStatusBar.h : interface for the CXTPStatusBar class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPSTATUSBAR_H__)
#define __XTPSTATUSBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPCommandBars;
class CXTPPaintManager;
class CXTPStatusBar;
class CXTPImageManagerIcon;
class CXTPImageManager;
class CXTPToolTipContext;
class CXTPPopupBar;
class CXTPPropExchange;

class CXTPMarkupUIElement;
class CXTPMarkupContext;


//===========================================================================
// Summary:
//     CXTPStatusBarPane is a CCmdTarget derived class. It represents single pane of status bar
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarPane : public CXTPCmdTarget
{
private:
	DECLARE_DYNAMIC(CXTPStatusBarPane)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarPane object
	//-----------------------------------------------------------------------
	CXTPStatusBarPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarPane object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBarPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the text caption that is displayed
	//     in this pane.
	// Parameters:
	//     lpszText - Text caption of the pane.
	// See Also: GetText
	//-----------------------------------------------------------------------
	void SetText(LPCTSTR lpszText);

	//-----------------------------------------------------------------------
	// Input:   lpszCaption - The Caption is the text that is displayed for
	//                        the pane in the status bar customization window.
	// Summary: Sets the text that is displayed for the pane in the status
	//          bar customization window.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary: Gets the text that is displayed for the pane in the status
	//          bar customization window.
	// Returns: Text caption.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Input:   lpszValue - The Value of the pane can be any string value and
	//                      can be used to store extra data if customization
	//                      is disabled.  If customization is enabled, the
	//                      Value is the text that is displayed for the pane
	//                      in the status bar customization window.
	// Summary: Sets the value of the pane.  This is displayed in the status
	//          bar customization window.
	//-----------------------------------------------------------------------
	void SetValue(LPCTSTR lpszValue);

	//-----------------------------------------------------------------------
	// Summary: Gets the value of the pane.  This is displayed in the status
	//          bar customization window.
	// Returns: The value of the status bar pane.
	//-----------------------------------------------------------------------
	CString GetValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the text caption to the pane.
	// Returns:
	//     Text caption of pane.  This is the text displayed in the pane
	// See Also: SetText
	//-----------------------------------------------------------------------
	CString GetText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of this pane.
	// Parameters:
	//     nWidth - new width to be set
	// See Also: GetWidth
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the width of the pane.
	// Returns:
	//     Current width of the pane
	// See Also: SetWidth
	//-----------------------------------------------------------------------
	virtual int GetWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the style of a status bar's pane.
	//     A pane's style determines how the pane appears.
	// Parameters:
	//     nStyle - Style of the pane whose style is to be set
	//-----------------------------------------------------------------------
	void SetStyle(UINT nStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the style of a status bar's pane.
	// Returns:
	//     Style of the indicator pane.
	//-----------------------------------------------------------------------
	UINT GetStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the ID of a status bar's pane.
	// Returns:
	//     ID of the indicator pane.
	//-----------------------------------------------------------------------
	UINT GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the index of a status bar's pane.
	// Returns:
	//     Index of the indicator pane.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove the pane
	//-----------------------------------------------------------------------
	void Remove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the pane is visible.
	// Returns:
	//     TRUE if the tab is visible, FALSE otherwise.
	// See Also: SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide or show the pane.
	// Parameters:
	//     bVisible - TRUE to show the tab, FALSE to hide the tab.
	//     bRecalcLayout - TRUE to recalculate layout after.
	// See Also: IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible, BOOL bRecalcLayout = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color of an item text.
	// Parameters:
	//     clrText -  COLORREF object that represents the color of an item
	//     text.
	//-----------------------------------------------------------------------
	void SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the disabled color of an item text.
	// Parameters:
	//     clrText -  COLORREF object that represents the disabled color of an item
	//     text.
	//-----------------------------------------------------------------------
	void SetTextGrayColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the color of an item text.
	// Returns:
	//     A COLORREF object that represents the color of an item text.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the disabled color of an item text.
	// Returns:
	//     A COLORREF object that represents the disabled color of an item text.
	//-----------------------------------------------------------------------
	COLORREF GetTextGrayColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color of an item background.
	// Parameters:
	//     clrBackground -  COLORREF object that represents the background color of an item
	//     text.
	//-----------------------------------------------------------------------
	void SetBackgroundColor(COLORREF clrBackground);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the background color of an item.
	// Returns:
	//     The background color of a item.
	//-----------------------------------------------------------------------
	COLORREF GetBackgroundColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font of an item text.
	// Parameters:
	//     pFntText -  point to CFont object.
	//     pLogfText -  point to LOGFONT structure.
	//-----------------------------------------------------------------------
	void SetTextFont(CFont* pFntText);
	void SetTextFont(PLOGFONT pLogfText); // <combine CXTPStatusBarPane::SetTextFont@CFont* >

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font of an item text.
	// Returns:
	//     Pointer to CFont object.
	//-----------------------------------------------------------------------
	virtual CFont* GetTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves icon index.
	// Returns:
	//     Icon index.
	// See Also: SetIconIndex
	//-----------------------------------------------------------------------
	int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set icon index of item.
	// Parameters:
	//     nIconIndex - Icon index to be set.
	// See Also: GetIconIndex
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the alignment of an item text.
	// Parameters:
	//     nAlign -  The alignment style of an item text.
	//-----------------------------------------------------------------------
	void SetTextAlignment(int nAlign);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the alignment of an item text.
	// Returns:
	//     The alignment style of an item text.
	//-----------------------------------------------------------------------
	int GetTextAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tooltip for this pane.  This is the
	//     text that will be displayed when the mouse pointer is positioned
	//     over the pane.
	// Parameters: lpszTooltip - Tooltip to set for this tab.
	// See Also: GetTooltip
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tooltip displayed when the mouse
	//     pointer is positioned over the pane.
	// Returns:
	//     The tooltip displayed when the mouse if over the pane.
	// See Also: SetTooltip
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Fits the width of the pane to the its text width.
	//-----------------------------------------------------------------------
	void BestFit();

	//-----------------------------------------------------------------------
	// Summary: Gets the best fit width which fits the width of the pane to
	//          the its text width.
	// Returns: Best fit width.
	//-----------------------------------------------------------------------
	int GetBestFit();

	//-----------------------------------------------------------------------
	// Summary: Call this method to add separator before pane
	// Input:   bBeginGroup - TRUE to start new group
	//-----------------------------------------------------------------------
	void SetBeginGroup(BOOL bBeginGroup = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Gets a boolean value indicating whether the pane has a separator
	//          placed before it and thus begins a new group.
	// Returns: TRUE if the pane begins a group, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetBeginGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get parent status bar object
	// Returns:
	//     Pointer to parent status bar object
	//-----------------------------------------------------------------------
	CXTPStatusBar* GetStatusBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get image of the pane
	// Returns:
	//     CXTPImageManagerIcon Pointer to be drawn.
	//-----------------------------------------------------------------------
	virtual CXTPImageManagerIcon* GetImage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns margins for pane entry
	// See Also: SetMargins
	//-----------------------------------------------------------------------
	CRect GetMargins() const;

	//-----------------------------------------------------------------------
	// Summary: Gets the padding placed around the Text of a pane.
	// Returns: Padding placed around the Text of a pane.
	//-----------------------------------------------------------------------
	CRect GetPadding() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set margins for pane
	// Parameters:
	//     nLeft - Left margin
	//     nTop - Top margin
	//     nRight - Right margin
	//     nBottom - Bottom margin
	// See Also: GetMargins
	//-----------------------------------------------------------------------
	void SetMargins(int nLeft, int nTop, int nRight, int nBottom);

	//-----------------------------------------------------------------------
	// Input:   nLeft - Padding placed on the left side of the Text of a pane.
	//          nTop - Padding placed on the top of the Text of a pane.
	//          nRight - Padding placed on the right side of the Text of a pane.
	//          nBottom - Padding placed on the bottom of the Text of a pane.
	// Summary: Sets the padding placed around the Text of a pane in pixels.
	//-----------------------------------------------------------------------
	void SetPadding(int nLeft, int nTop, int nRight, int nBottom);

	//-----------------------------------------------------------------------
	// Input:   bButton - TRUE if the pane will act as a button.
	// Summary: Specifies whether the pane will also act as a button.
	// Remarks: Button panes will look and act like a button on mouse over
	//          and when clicked.  On mouse over the button pane will become
	//          "hot" giving a visual que that it is a button and can be
	//          clicked.  When clicked the button pane will change to a
	//          "pressed" state.
	//-----------------------------------------------------------------------
	void SetButton(BOOL bButton = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Specifies whether the pane will also act as a button.
	// Returns: TRUE if the pane acts as a button, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsButton() const;

	//-----------------------------------------------------------------------
	// Summary: Gets a boolean value indicating whether the switch pane is enabled.
	// Returns: TRUE if enabled, FALSE if diabled.
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

	//-----------------------------------------------------------------------
	// Input:   bEnabled - TRUE to enable the pane, FALSE to disable the pane.
	// Summary: Specifies whether the switch pane is enabled or disabled.
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary: This member is called to determine if the pane button is
	//          currently highlighted. This occurs when the mouse cursor is
	//          positioned over the pane button.
	// Returns: TRUE if the pane button is currently highlighted.
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary: Call this method to determine if the pane button is pressed.
	// Returns: TRUE if the pane button is pressed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Input:   bVisible - TRUE to enable customization, FALSE to disable
	//                     customization.
	// Summary: Enables or disables statusbar customization.
	// Remarks: SetCustomizationVisible will enable a status bar customization
	//          window to appear when the status bar is right-clicked on.  The
	//          customization window allows all the panes in the statusbar to
	//          be hidden or displayed (if already hidden).  All panes will be
	//          displayed in the window with the exception of the Pane with
	//          Id equal to 0.  The items displayed in the window are the
	//          Caption of the pane and the Value of the pane.  Also indicated
	//          is the Visibility of the pane.
	//-----------------------------------------------------------------------
	void SetCustomizationVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary: Gets whether customization is enabled.
	// Returns: TRUE if customization is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetCustomizationVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the pane button state is
	//     checked.
	// Returns:
	//     TRUE if the pane button is checked, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsChecked() const;

	//-----------------------------------------------------------------------
	// Input:   bChecked - TRUE to check the pane, FALSE to un-check the pane.
	// Summary: Sets a boolean value indicating whether the pane is checked.
	//-----------------------------------------------------------------------
	void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the pane has borders
	//     enabled.
	// Returns:
	//     TRUE if the pane has borders, otherwise FALSE. If the pane is a
	//     button, the return value is always FALSE since buttons will handle
	//     drawing their own borders.
	// See Also:
	//     IsButton
	//-----------------------------------------------------------------------
	BOOL HasBorders() const;

	//-----------------------------------------------------------------------
	// Input:   bShowBorders - TRUE to show the pane borders, FALSE to hide them.
	// Summary: Call this member function to show the pane borders.
	// Remarks: If the pane is a button then this has no effect.
	//-----------------------------------------------------------------------
	void ShowBorders(BOOL bShowBorders);

	//-------------------------------------------------------------------------
	// Summary: Call this member function to redraw the pane.
	//-------------------------------------------------------------------------
	void Redraw() const;

	//-----------------------------------------------------------------------
	// Summary: Call this member to get the bounding rectangle for the pane.
	// Returns: A CRect object.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary: This member functions are used to get stored CXTPMarkupUIElement object.
	// Returns: A pointer to CXTPMarkupUIElement object.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw pane item. Override it to draw custom pane.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rcItem - Item rectangle
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Input:   point - A CPoint that specifies the x- and y- coordinates of
	//                  the cursor. These coordinates are always relative to
	//                  the upper-left corner of the window.
	// Summary: This member function is used to processes left mouse button events.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(CPoint point);

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y coordinate of the cursor.
	// Summary: This member is called when the mouse cursor moves.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y coordinate of the cursor. These
	//                  coordinates are always relative to the upper-left
	//                  corner of the window.
	//          pTI - A pointer to a TOOLINFO structure.
	// Summary: The framework calls this member function to determine whether
	//          a point is in the bounding rectangle of the specified tool.
	// Returns: If the tooltip control was found, the window control ID. If
	//          the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;


protected:
	UINT     m_nID;           // IDC of indicator: 0 => normal text area.
	int      m_cxText;        // Width of the string area in pixels. On both sides there is a 3 pixel gap and a 1 pixel border, making a pane 6 pixels wider.
	UINT     m_nStyle;        // Style flags (SBPS_*).
	CString  m_strText;       // Text in the pane.
	DWORD    m_dwHideFlags;   // TRUE if pane is visible
	COLORREF m_clrText;       // Text color
	COLORREF m_clrTextGray;   // Disabled text color
	COLORREF m_clrBackground; // Background color
	CString  m_strToolTip;    // Tooltip text for the item

	CRect m_rcPane;         // Bounding rectangle for the pane

	HWND m_hWndPane;        // Child handle
	BOOL m_bAutoDeleteWnd;  // TRUE to auto delete child.
	int m_nAlignment;       // Alignment of the item.

	CXTPStatusBar* m_pStatusBar;    // Parent StatusBar object
	int m_nIndex;           // Index of the item
	CFont m_fntText;        // Font of an item text
	int m_nIconIndex;       // Icon index of the pane
	CRect m_rcPadding;      // Margins of the pane
	CRect m_rcMargins;      // Margins of the pane
	BOOL m_bButton;         // TRUE if pane is clickable button
	BOOL m_bPressed;        // TRUE if button was pressed
	CXTPMarkupUIElement* m_pMarkupUIElement; // Markup Context
	BOOL m_bBeginGroup;         // TRUE if the pane begins a group
	CString m_strCaption;       // The text that is displayed for the pane in the status bar customization window
	CString m_strValue;         // The value of the pane.  This is displayed in the status bar customization window
	int m_nBestFit;             // Width for the pane to have the best fit
	BOOL m_bCustomizable;       // True to enable customization, False to disable customization

	friend class CXTPStatusBar;
};

//===========================================================================
// Summary:
//     CXTPStatusBar is a CControlBar derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBar : public CControlBar
{
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPStatusBar)
//}}AFX_CODEJOCK_PRIVATE

private:
	class CStatusCmdUI;
	class CControlCustomization;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBar object
	//-----------------------------------------------------------------------
	CXTPStatusBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a status bar (a child window) and associates it with the CStatusBar object.
	//     Also sets the initial font and sets the status bar's height to a default value.
	// Parameters:
	//     pParentWnd - Pointer to the CWnd object whose Windows window is the parent of the status bar.
	//     dwStyle    - The status-bar style. In addition to the standard Windows styles, these styles are supported
	//     nID        - The toolbar's child-window ID
	// Returns:
	//     Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, UINT nID = AFX_IDW_STATUS_BAR);
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets each indicator's ID to the value specified by the corresponding
	//     element of the array lpIDArray, loads the string resource specified
	//     by each ID, and sets the indicator's text to the string.
	// Parameters:
	//     lpIDArray - Pointer to an array of IDs.
	//     nIDCount  - Number of elements in the array pointed to by lpIDArray.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL SetIndicators (const UINT* lpIDArray, int nIDCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the command bar's object.
	// Parameters:
	//     pCommandBars - CXTPCommandBars to be set.
	//-----------------------------------------------------------------------
	void SetCommandBars(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the paint manager.
	// Returns:
	//     A pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the status bar to reposition pane
	//     item controls.
	//-----------------------------------------------------------------------
	void PositionControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a control to an indicator pane.
	// Parameters:
	//     pWnd        - Points to a control window.
	//     nID         - ID of the indicator pane.
	//     bAutoDelete - TRUE if the control is to be deleted when destroyed.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL AddControl(CWnd* pWnd, UINT nID, BOOL bAutoDelete = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the width for an indicator pane.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	//     cxText - New width for the indicator pane.
	//-----------------------------------------------------------------------
	void SetPaneWidth(int nIndex, int cxText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add an indicator pane to the status bar.
	// Parameters:
	//     nID    - ID of the indicator pane.
	//     nIndex - Index of the indicator pane.
	//     pPane  - New CXTPStatusBarPane object
	// Returns:
	//     CXTPStatusBarPane pointer if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane* AddIndicator(UINT nID, int nIndex = -1);
	CXTPStatusBarPane* AddIndicator(CXTPStatusBarPane* pPane, UINT nID, int nIndex = -1);  // <combine CXTPStatusBar::AddIndicator@UINT@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove an indicator pane from the status bar.
	// Parameters:
	//     nID - ID of the indicator pane.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveIndicator(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the number of panes that are created
	//     for the status bar.
	// Returns:
	//     An integer value that represents the number of panes for the status bar.
	//-----------------------------------------------------------------------
	int GetPaneCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates total number of visible panes.
	// Returns:
	//     Total number of panes currently visible.
	//-----------------------------------------------------------------------
	int GetVisiblePaneCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the pane information for the given index.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	// Returns:
	//     A pointer to an CXTPStatusBarPane class.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane* GetPane(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to find the pane for the given Identifier.
	// Parameters:
	//     nID - Identifier of the indicator pane.
	// Returns:
	//     A pointer to an CXTPStatusBarPane class.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane* FindPane(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove all indicators.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove an indicator pane from the status bar.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the image manager of the image manager.
	// Returns:
	//     A pointer to a CXTPImageManager object
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine which pane, if any, is at the
	//     specified screen position.
	// Parameters:
	//     pt - Point to be tested.
	//     lpRect - Returns rectangle of the item under point pt.
	// Returns:
	//     CXTPStatusBarPane pointer, or returns NULL if no pane is
	//     at the specified position.
	//-----------------------------------------------------------------------
	CXTPStatusBarPane* HitTest(CPoint pt, CRect* lpRect = NULL) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable customization of the statusbar
	// Parameters:
	//     bEnable - TRUE to enable customization.
	//-----------------------------------------------------------------------
	void EnableCustomization(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Input:   lpszProfileName - Points to a null-terminated string that
	//                            specifies the name of a section in the
	//                            initialization file or a key in the Windows
	//                            registry where state information is stored.
	// Summary: Call this function to retrieve state information from the
	//          registry or .INI file.
	//-----------------------------------------------------------------------
	void LoadState(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Input:   lpszProfileName - Points to a null-terminated string that
	//                            specifies the name of a section in the
	//                            initialization file or a key in the Windows
	//                            registry where state information is stored.
	// Summary: Call this function to save the state information to the
	//          registry or .INI file
	//-----------------------------------------------------------------------
	void SaveState(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw disabled text.
	// Parameters:
	//     bDraw - TRUE to draw disabled text, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetDrawDisabledText(BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if paint manager draw disabled text.
	//-----------------------------------------------------------------------
	BOOL GetDrawDisabledText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get tooltip context pointer.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single pane of the status bar.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     nIndex - Index of the pane to draw.
	//     rcItem - Bounding rectangle of the pane
	//-----------------------------------------------------------------------
	virtual void DrawPaneEntry(CDC* pDC, int nIndex, CRect rcItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create a status bar
	// Parameters:
	//     pParentWnd - Pointer to the CWnd object whose Windows window is the parent of the status bar.
	//     dwCtrlStyle - Additional styles for the creation of the embedded CStatusBarCtrl object.
	//     dwStyle - The status-bar style
	//     nID - The status bar's child-window ID.
	// Returns: Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = 0,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
		UINT nID = AFX_IDW_STATUS_BAR);


	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the indicator index for a given ID. The index of the first indicator is 0
	// Parameters:
	//      nIDFind - String ID of the indicator whose index is to be retrieved
	// Returns:
	//      The index of the indicator if successful; -1 if not successful.
	// See Also: GetItemID
	//-----------------------------------------------------------------------
	int CommandToIndex(UINT nIDFind) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the ID of the indicator specified by nIndex
	// Parameters:
	//     nIndex - Index of the indicator whose ID is to be retrieved.
	// Returns:
	//     The ID of the indicator specified by nIndex.
	// See Also: CommandToIndex
	//-----------------------------------------------------------------------
	UINT GetItemID(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies the coordinates of the indicator specified by nIndex into the structure pointed to by lpRect.
	//     Coordinates are in pixels relative to the upper-left corner of the status bar.
	// Parameters:
	//     nIndex - Index of the indicator whose rectangle coordinates are to be retrieved.
	//     lpRect - Points to aRECT structure or a CRect object that will receive the coordinates of the indicator specified by nIndex.
	// See Also: GetPaneInfo, CommandToIndex
	//-----------------------------------------------------------------------
	void GetItemRect(int nIndex, LPRECT lpRect) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the text that appears in a status-bar pane.
	//     The second form of this member function fills a CString object with the string text.
	// Parameters:
	//     nIndex  - Index of the pane whose text is to be retrieved
	//     rString - A reference to a CString object that contains the text to be retrieved.
	// See Also: SetPaneText
	//-----------------------------------------------------------------------
	CString GetPaneText(int nIndex) const;
	void GetPaneText(int nIndex, CString& rString) const; // <combine CXTPStatusBar::GetPaneText@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the pane text to the string pointed to by lpszNewText.
	// Parameters:
	//     nIndex - Index of the pane whose text is to be set.
	//     lpszNewText - Pointer to the new pane text
	//     bUpdate - If TRUE, the pane is invalidated after the text is set.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// See Also: GetPaneText
	//-----------------------------------------------------------------------
	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets nID, nStyle, and cxWidth to the ID, style, and width of the indicator
	//     pane at the location specified by nIndex.
	// Parameters:
	//     nIndex  - Index of the pane whose information is to be retrieved.
	//     nID     - Reference to a UINT that is set to the ID of the pane
	//     nStyle  - Reference to a UINT that is set to the style of the pane.
	//     cxWidth - Reference to an integer that is set to the width of the pane
	// See Also: SetPaneInfo, GetItemRect
	//-----------------------------------------------------------------------
	void GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the specified indicator pane to a new ID, style, and width.
	// Parameters:
	//     nIndex  - Index of the indicator pane whose style is to be set.
	//     nID     - New ID for the indicator pane
	//     nStyle  - New style for the indicator pane
	//     cxWidth - New width for the indicator pane
	// See Also: GetPaneInfo
	//-----------------------------------------------------------------------
	void SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the style of a status bar's pane. A pane's style determines how the pane appears.
	// Parameters:
	//     nIndex - Index of the pane whose style is to be retrieved.
	// Returns:
	//     The style of the status-bar pane specified by nIndex
	// See Also: SetPaneStyle
	//-----------------------------------------------------------------------
	UINT GetPaneStyle(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the style of a status bar's pane. A pane's style determines how the pane appears.
	// Parameters:
	//     nIndex - Index of the pane whose style is to be set.
	//     nStyle - Style of the pane whose style is to be set
	// See Also: GetPaneStyle
	//-----------------------------------------------------------------------
	void SetPaneStyle(int nIndex, UINT nStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function allows direct access to the underlying common control
	// Returns:
	//     Contains a reference to a CStatusBarCtrl object.
	//-----------------------------------------------------------------------
	CStatusBarCtrl& GetStatusBarCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set font for the status bar.
	// Parameters:
	//      pFont - Font to set
	//      bRedraw - TRUE to redraw status bar
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Call this method to get font for the status bar.
	// Returns: Font used for the status bar.
	//-----------------------------------------------------------------------
	CFont* GetFont();

	//-----------------------------------------------------------------------
	// Input:   nIndex - Position of the ribbon bar divider.
	// Summary: Sets the position of the ribbon divider index.  This specifies
	//          the panes that get the second color for the ribbon theme.
	// Remarks: RibbonDividerIndex is a zero based index and specifies the
	//          panes that get the second color for the ribbon theme.  All
	//          panes with an index larger than the divider index will get
	//          the second ribbon color applied.
	//-----------------------------------------------------------------------
	void SetRibbonDividerIndex(int nIndex);

	//-----------------------------------------------------------------------
	// Summary: Gets the position of the ribbon divider index
	// Returns: Position of the ribbon bar divider.
	//-----------------------------------------------------------------------
	int GetRibbonDividerIndex() const;

	//-----------------------------------------------------------------------
	// Input:   bEnableMarkup - TRUE to enable markup, FALSE to disable markup.
	// Summary: Allows the use of a XAML snippet to be used for the Text of
	//          panes in the Status Bar.
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Returns markup context.
	// Returns: Returns markup context.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary: Call this method to reposition all panes
	//-----------------------------------------------------------------------
	virtual void RecalcLayout();

//{{AFX_CODEJOCK_PRIVATE
	// Obsolete
	void UpdateAllPanes(BOOL bUpdateRects, BOOL bUpdateText);

#if _MSC_VER > 1100
	void SetBorders(LPCRECT lpRect);
	void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);
#endif
//}}AFX_CODEJOCK_PRIVATE

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the stretch gripper of the status bar.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcClient - Client rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawStretch(CDC* pDC, CRect& rcClient);

protected:
//{{AFX_CODEJOCK_PRIVATE
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	void CalcInsideRect(CRect& rect, BOOL bHorz) const;
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	void EnableDocking(DWORD dwDockStyle);
#endif

public:
	virtual void OnPaneClick(CXTPStatusBarPane* pPane);
	virtual void OnSwitchPaneClick(CXTPStatusBarPane* pPane, long Id);

	virtual CXTPPopupBar* GetStatusBarPopup();

//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//{{AFX_MSG(CXTPStatusBar)
	public:
	afx_msg void OnPaint();
	afx_msg void OnDraw(CDC* pDC);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg LRESULT OnIdleUpdateCmdUI (WPARAM wParam, LPARAM lParam);
	protected:
	afx_msg LRESULT OnNcHitTest(CPoint);
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg void OnNcPaint();
	afx_msg void OnWindowPosChanging(LPWINDOWPOS);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMinHeight(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void HandleMouseMove(CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL IsWin4();

private:
	DISABLE_WNDCREATE()
	DISABLE_WNDCREATEEX()


protected:
	CArray <CXTPStatusBarPane*, CXTPStatusBarPane*> m_arrPanes;  // Array of panes
	int m_nMinHeight;                           // Minimum client height
	CFont m_fontStatus;                         // Font of status bar
	BOOL m_bDrawDisabledText;                   // TRUE to draw disabled text

	CXTPStatusBarPane* m_pHighlightedPane;      // Pane currently highlighted
	int m_nRibbonDividerIndex;                  // Position of dark part for Ribbon Theme
	BOOL m_bShowSizeGripper;                    // TRUE to show size gripper
	CXTPMarkupContext* m_pMarkupContext;        // Markup Context

	int m_cxPaneSpacing;            // Gap between panes
	int m_bCustomizable;            // TRUE to allow customize status bar.
private:
	CXTPCommandBars* m_pCommandBars;
	CXTPToolTipContext* m_pToolTipContext;          // Tooltip Context.

	friend class CXTPStatusBarPane;
};



//===========================================================================
// Summary:
//     CXTPStatusBarSwitchPane is a CXTPStatusBarPane derived class. CXTPStatusBarSwitchPane works with
//     CXTStatusBar and allows you to create switches in your status
//     bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarSwitchPane : public CXTPStatusBarPane
{
	DECLARE_DYNAMIC(CXTPStatusBarSwitchPane)
public:

	//-------------------------------------------------------------------------
	// Summary: Holds status bar pane switch properties.
	//-------------------------------------------------------------------------
	struct SWITCH
	{
		CString strToolTip; // Tooltip of the switch.
		UINT nID;           // Id of the switch.
		int nWidth;         // Width of the switch.
		BOOL bChecked;      // TRUE to "check" the switch.
		BOOL bHighlighted;  // TRUE if the switch is highligted.
		BOOL bPressed;      // TRUE if the switch is pressed.
		BOOL bEnabled;      // TRUE if enabled
	};
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarSwitchPane object.
	//-----------------------------------------------------------------------
	CXTPStatusBarSwitchPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarSwitchPane object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPStatusBarSwitchPane();

public:

	//-----------------------------------------------------------------------
	// Input:   lpIDArray - Pointer to Array with identifiers
	//          nIDCount - Count of elements in array
	// Summary: Call this method to set switches
	// Returns: TRUE if successful;
	//-----------------------------------------------------------------------
	BOOL SetSwitches(const UINT* lpIDArray, int nIDCount);

	//-----------------------------------------------------------------------
	// Input:   nID - Id of the switch.
	//          lpszToolTip - Tooltip of the switch.
	// Summary: Adds a switch to the Switch Pane.
	// Returns: Reference to a SWITCH object.
	//-----------------------------------------------------------------------
	SWITCH* AddSwitch(UINT nID);
	SWITCH* AddSwitch(UINT nID, LPCTSTR lpszToolTip); // <combine CXTPStatusBarSwitchPane::AddSwitch@UINT>

	//-----------------------------------------------------------------------
	// Summary: Returns switch by its position
	// Input:   nIndex - Index of switch to get
	// Returns: POinter to SWITCH.
	//-----------------------------------------------------------------------
	SWITCH* GetSwitch(int nIndex) const;
	SWITCH* FindSwitch(UINT nID) const;

	//-----------------------------------------------------------------------
	// Summary: Gets the total number of switches in the switch pane.
	// Returns: Total number of switches in the switch pane.
	//-----------------------------------------------------------------------
	int GetSwitchCount() const;

	//-------------------------------------------------------------------------
	// Summary: Removes all switches from the pane.
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary: Removes a switch from the Switch Pane.
	// Parameters:
	//          nID - Id of the switch to remove.
	//-----------------------------------------------------------------------
	void RemoveSwitch(UINT nID);

	//-----------------------------------------------------------------------
	// Summary: Specifies whether the switch pane is enabled.
	// Parameters:
	//          nID - Id of the switch to enable\disable.
	//          bEnabled - TRUE to enable, FALSE to disable.
	//-----------------------------------------------------------------------
	void EnableSwitch(UINT nID, BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Input:   nID - Id of the switch to be checked.
	// Summary: Sets the Id of the switch that is currently checked.
	// Remarks: Checked is used to get or set the currently selected\checked
	//          switch in the switch pane.
	//-----------------------------------------------------------------------
	void SetChecked(UINT nID);

	//-----------------------------------------------------------------------
	// Summary: Gets the width of the switch pane.
	// Returns: Width of the switch pane.
	//-----------------------------------------------------------------------
	virtual int GetWidth();

public:

	//-----------------------------------------------------------------------
	// Input:   point - Point to test.
	//          rcItem - Retrieves bounding rectangle of the item.
	// Summary: Call this method to determine which pane, if any, is at the
	//          specified screen position.
	// Returns: A SWITCH object containing data pf the pane at the specified point.
	//-----------------------------------------------------------------------
	SWITCH* HitTest(CPoint point, CRect& rcItem) const;

protected:

	//-----------------------------------------------------------------------
	// Input:   point - A CPoint that specifies the x- and y- coordinates of
	//                  the cursor. These coordinates are always relative to
	//                  the upper-left corner of the window.
	// Summary: This member function is used to processes left mouse button events.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw pane switch item. Override it to
	//     draw a custom switch.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rcItem - Item rectangle
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y coordinate of the cursor.
	// Summary: This member is called when the mouse cursor moves.
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Input:   point - Specifies the x- and y coordinate of the cursor. These
	//                  coordinates are always relative to the upper-left
	//                  corner of the window.
	//          pTI - A pointer to a TOOLINFO structure.
	// Summary: The framework calls this member function to determine whether
	//          a point is in the bounding rectangle of the specified tool.
	// Returns: If the tooltip control was found, the window control ID. If
	//          the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;


protected:
	CArray<SWITCH*, SWITCH*> m_arrSwitches; // Collection of switces in the switch pane.
	SWITCH* m_pHighlighted;                 // TRUE if the switch is currently highlighted.
};


//===========================================================================
// Summary:
//     CXTPStatusBarScrollBarPane is a CXTPStatusBarPane derived class. CXTPStatusBarScrollBarPane works with
//     CXTStatusBar and allows you to create themed ScrollBar in your status
//     bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarScrollBarPane : public CXTPStatusBarPane, public CXTPScrollBase
{
	DECLARE_DYNAMIC(CXTPStatusBarScrollBarPane)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarScrollBarPane object.
	//-----------------------------------------------------------------------
	CXTPStatusBarScrollBarPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarScrollBarPane object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPStatusBarScrollBarPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set position of scrollbar.
	// Parameters:
	//     nValue    - Scrollbar position to be set.
	//-----------------------------------------------------------------------
	void SetPos(int nValue);

	//-----------------------------------------------------------------------
	// Input:   nMin - Lower limit of the scrollbar control's range.
	//          nMax - Upper limit of the scrollbar control's range.
	// Summary: Sets the rangle of the scroll bar.
	//-----------------------------------------------------------------------
	void SetRange(int nMin, int nMax);

	//-----------------------------------------------------------------------
	// Summary: Gets the current position of the scroll bar, whose return value
	//          is always between the values for the Max and Min properties, inclusive.
	// Returns: Current position of the scroll bar.
	//-----------------------------------------------------------------------
	int GetPos() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set tooltip for scroll part of scrollbar.
	// Parameters:
	//     nPart    - Part of scrollbar to set tooltip. Can be XTP_HTSCROLLUP, XTP_HTSCROLLDOWN, XTP_HTSCROLLUPPAGE, XTP_HTSCROLLDOWNPAGE, XTP_HTSCROLLTHUMB
	//     lpszTooltip - Text of tooltip to set
	//-----------------------------------------------------------------------
	void SetTooltipPart(int nPart, LPCTSTR lpszTooltip);

protected:
//{{AFX_CODEJOCK_PRIVATE
	virtual CRect GetScrollBarRect();
	virtual void GetScrollInfo(SCROLLINFO* psi);
	virtual void DoScroll(int cmd, int pos);
	virtual void RedrawScrollBar();
	virtual CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;
	virtual BOOL IsScrollBarEnabled() const;
	virtual CWnd* GetParentWindow() const;
	virtual INT_PTR CXTPStatusBarScrollBarPane::OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	virtual void OnDraw(CDC* pDC, CRect rcItem);
	virtual void OnMouseMove(CPoint point);
	virtual void OnLButtonDown(CPoint point);
//}}AFX_CODEJOCK_PRIVATE


protected:
	int m_nMin;             // Lower limit of the scrollbar control's range.
	int m_nMax;             // Upper limit of the scrollbar control's range.
	int m_nValue;           // Scrollbar position.
	int m_nPage;            // Scrollbar Page.
	CString m_strToolTipPart[5]; // Text of tooltip.
};

//===========================================================================
// Summary:
//     CXTPStatusBarSliderPane is a CXTPStatusBarPane derived class. CXTPStatusBarSliderPane works with
//     CXTStatusBar and allows you to create themed Slider in your status
//     bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarSliderPane : public CXTPStatusBarScrollBarPane
{
	DECLARE_DYNAMIC(CXTPStatusBarSliderPane)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarSliderPane object.
	//-----------------------------------------------------------------------
	CXTPStatusBarSliderPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarSliderPane object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPStatusBarSliderPane();

protected:

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve pointer to CXTPScrollBarPaintManager.
	//----------------------------------------------------------------------
	virtual CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;

public:

	//-----------------------------------------------------------------------
	// Summary: Call this method to set ticks for slider
	// Input:   pTicks - Pointer to double array of ticks
	//          nCount - Count of elements in array.
	//-----------------------------------------------------------------------
	void SetTicks(double* pTicks, int nCount);

};


//===========================================================================
// Summary:
//     CXTPStatusBarProgressPane is a CXTPStatusBarPane derived class. CXTPStatusBarProgressPane works with
//     CXTStatusBar and allows you to create progress bar in your status bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarProgressPane : public CXTPStatusBarPane, public CXTPProgressBase
{
	DECLARE_DYNAMIC(CXTPStatusBarProgressPane)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarProgressPane object
	//-----------------------------------------------------------------------
	CXTPStatusBarProgressPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarProgressPane object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPStatusBarProgressPane();

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to redraw progress bar.
	//----------------------------------------------------------------------
	virtual void RedrawProgress();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve pointer to CXTPProgressPaintManager.
	//----------------------------------------------------------------------
	virtual CXTPProgressPaintManager* GetProgressPaintManager() const;

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to get bounding rectangle of progressbar
	//----------------------------------------------------------------------
	virtual CRect GetProgressRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw pane item. Override it to draw custom pane.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rcItem - Item rectangle
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcItem);

public:
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPStatusBarPane::SetCaption(LPCTSTR lpszCaption){
	m_strCaption = lpszCaption;
}
AFX_INLINE CString CXTPStatusBarPane::GetCaption() const{
	return m_strCaption;
}
AFX_INLINE void CXTPStatusBarPane::SetValue(LPCTSTR lpszValue){
	m_strValue = lpszValue;
}
AFX_INLINE CString CXTPStatusBarPane::GetValue() const{
	return m_strValue;
}
AFX_INLINE BOOL CXTPStatusBarPane::IsHighlighted() const {
	return m_pStatusBar->m_pHighlightedPane == this;
}
AFX_INLINE BOOL CXTPStatusBarPane::IsPressed() const {
	return m_bPressed;
}
AFX_INLINE void CXTPStatusBarPane::SetBeginGroup(BOOL bBeginGroup) {
	m_bBeginGroup = bBeginGroup;
}
AFX_INLINE BOOL CXTPStatusBarPane::GetBeginGroup() const {
	return m_bBeginGroup;
}
AFX_INLINE BOOL CXTPStatusBarPane::HasBorders() const {
	return ((m_nStyle & SBPS_NOBORDERS) == 0);
}
AFX_INLINE void CXTPStatusBarPane::ShowBorders(BOOL bShowBorders) {
	SetStyle(bShowBorders ? m_nStyle & ~SBPS_NOBORDERS : m_nStyle | SBPS_NOBORDERS);
}
AFX_INLINE void CXTPStatusBar::SetRibbonDividerIndex(int nIndex) {
	m_nRibbonDividerIndex = nIndex;
}
AFX_INLINE int CXTPStatusBar::GetRibbonDividerIndex() const {
	return m_nRibbonDividerIndex;
}

AFX_INLINE void CXTPStatusBar::SetCommandBars(CXTPCommandBars* pCommandBars) {
	m_pCommandBars = pCommandBars;
}
AFX_INLINE int CXTPStatusBar::GetPaneCount() const {
	return (int)m_arrPanes.GetSize();
}
AFX_INLINE CStatusBarCtrl& CXTPStatusBar::GetStatusBarCtrl() const{
	return *(CStatusBarCtrl*)this;
}

#if _MSC_VER > 1100
AFX_INLINE void CXTPStatusBar::SetBorders(int cxLeft, int cyTop, int cxRight, int cyBottom){
	ASSERT(cyTop >= 2); CControlBar::SetBorders(cxLeft, cyTop, cxRight, cyBottom);
}AFX_INLINE void CXTPStatusBar::SetBorders(LPCRECT lpRect) {
	SetBorders(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}
#endif

#ifdef _DEBUG
// status bars do not support docking
AFX_INLINE void CXTPStatusBar::EnableDocking(DWORD) {
	ASSERT(FALSE);
}
#endif

AFX_INLINE void CXTPStatusBar::SetDrawDisabledText(BOOL bDraw) {
	m_bDrawDisabledText = bDraw;
}
AFX_INLINE BOOL CXTPStatusBar::GetDrawDisabledText() const {
	return m_bDrawDisabledText;
}
AFX_INLINE CXTPToolTipContext* CXTPStatusBar::GetToolTipContext() const {
	return m_pToolTipContext;
}
AFX_INLINE CXTPMarkupContext* CXTPStatusBar::GetMarkupContext() const {
	return m_pMarkupContext;
}
AFX_INLINE CXTPMarkupUIElement* CXTPStatusBarPane::GetMarkupUIElement() const {
	return m_pMarkupUIElement;
}


#endif //#if !defined(__XTPSTATUSBAR_H__)
