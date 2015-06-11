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

	//-----------------------------------------------------------------------
	// Summary: Some themes may display status bar panes as dark. This method turns this effect on/off.
	//-----------------------------------------------------------------------
	void SetDark(BOOL bDark = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Determines if a pane is to be drawn as dark in some themes.
	// Returns: TRUE if pane is dark.
	//-----------------------------------------------------------------------
	BOOL IsDark() const;

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
	BOOL m_bIsDark;             // True if a pane should be dark in some themes.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPStatusBarPane);

	afx_msg BSTR OleGetText();
	afx_msg LPFONTDISP OleGetFont();
	afx_msg void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg HWND OleGetHandle();
	afx_msg void OleSetHandle(HWND hWnd);
	afx_msg int OleGetHideFlags();
	afx_msg void OleSetHideFlags(int dwHideFlags);
	afx_msg void OleSetVisible(BOOL bVisible);
	afx_msg void OleRedrawPane();
	afx_msg void OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom);

//}}AFX_CODEJOCK_PRIVATE
#endif
	friend class CXTPStatusBar;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPStatusBarPane::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;
}

AFX_INLINE CString CXTPStatusBarPane::GetCaption() const
{
	return m_strCaption;
}

AFX_INLINE void CXTPStatusBarPane::SetValue(LPCTSTR lpszValue)
{
	m_strValue = lpszValue;
}

AFX_INLINE CString CXTPStatusBarPane::GetValue() const
{
	return m_strValue;
}

AFX_INLINE BOOL CXTPStatusBarPane::IsHighlighted() const
{
	return m_pStatusBar->m_pHighlightedPane == this;
}

AFX_INLINE BOOL CXTPStatusBarPane::IsPressed() const
{
	return m_bPressed;
}

AFX_INLINE void CXTPStatusBarPane::SetBeginGroup(BOOL bBeginGroup)
{
	m_bBeginGroup = bBeginGroup;
}

AFX_INLINE BOOL CXTPStatusBarPane::GetBeginGroup() const
{
	return m_bBeginGroup;
}

AFX_INLINE BOOL CXTPStatusBarPane::HasBorders() const
{
	return ((m_nStyle & SBPS_NOBORDERS) == 0);
}

AFX_INLINE void CXTPStatusBarPane::ShowBorders(BOOL bShowBorders)
{
	SetStyle(bShowBorders ? m_nStyle & ~SBPS_NOBORDERS : m_nStyle | SBPS_NOBORDERS);
}

AFX_INLINE CXTPMarkupUIElement* CXTPStatusBarPane::GetMarkupUIElement() const
{
	return m_pMarkupUIElement;
}

AFX_INLINE void CXTPStatusBarPane::SetDark(BOOL bDark /*= TRUE*/)
{
	m_bIsDark = bDark;
}

AFX_INLINE BOOL CXTPStatusBarPane::IsDark() const
{
	return m_bIsDark;
}
