class CXTPMarkupUIElement;
class CXTPImageManagerIcon;
class CXTPTabManagerNavigateButtons;
class CXTPTabManager;

//===========================================================================
// Summary:
//     CXTPTabManagerItem is a CCmdTarget derived class that represents
//     a tab item.  This is the base class for the tabs that appear
//     in the CommandBars TabWorkSpace, DockingPanes, and TabControl.
//===========================================================================
class _XTP_EXT_CLASS  CXTPTabManagerItem : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPTabManagerItem)

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManagerItem object.
	//-------------------------------------------------------------------------
	CXTPTabManagerItem();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerItem object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabManagerItem();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the text caption that is displayed
	//     in this tab's button.
	// Parameters:
	//     lpszCaption - Text caption of the tab.
	// See Also: GetCaption
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the text caption to the tab.
	// Returns:
	//     Text caption of tab.  This is the text displayed in the tab
	//     button.
	// See Also: SetCaption
	//-----------------------------------------------------------------------
	virtual CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tooltip for this tab.  This is the
	//     text that will be displayed when the mouse pointer is positioned
	//     over the tab button.
	// Parameters: lpszTooltip - Tooltip to set for this tab.
	// See Also: GetTooltip, CXTPTabManager::GetItemTooltip
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tooltip displayed when the mouse
	//     pointer is positioned over the tab button.
	// Returns:
	//     The tooltip displayed when the mouse if over the tab button.
	// See Also: SetTooltip, CXTPTabManager::GetItemTooltip
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the color of the tab button.
	// Returns:
	//     The color of the tab button.
	// See Also: SetColor, CXTPTabManager::GetItemColor
	//-----------------------------------------------------------------------
	COLORREF GetColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the color of the tab button.
	// Parameters:   clr - New color of the tab button.
	// Remarks:
	//     After the new color is set, CXTPTabManager::RedrawControl is
	//     called.
	// See Also: GetColor, CXTPTabManager::GetItemColor
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle of the tab
	//     button.
	// Returns:
	//     Bounding rectangle of the tab button.
	// See Also: SetRect
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the width of the tab button.
	// Returns:
	//     Width of the tab button.
	// Remarks:
	//     The width might change depending on the tab layout and
	//     position of the tabs.
	// See Also: XTPTabLayoutStyle, XTPTabPosition
	//-----------------------------------------------------------------------
	int GetButtonLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the width of the content.
	// Returns:
	//     Width of the content of the button.
	// Remarks:
	//     The width might change depending on the tab layout and
	//     position of the tabs.
	// See Also: XTPTabLayoutStyle, XTPTabPosition
	//-----------------------------------------------------------------------
	int GetContentLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the index of the tab within the
	//     collection of tabs.
	// Returns:
	//     Index of tab.
	// See Also: CXTPTabManager::GetItem, CXTPTabManager
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the image index of the tab.
	// Returns:
	//     Index of the image displayed in the tab button.  The index is
	//     the index of an image within an CXTPImageManager or CImageList
	//     object.
	// Remarks:
	//     This is the index of an image within an CXTPImageManager or CImageList
	//     object that will be displayed in the tab button.  CXTPTabManager::ShowIcons
	//     is used to hide\show icons.
	// See Also: SetImageIndex, CXTPTabControl::InsertItem, CXTPTabControl::GetImageManager,
	//           CXTPTabControl::SetImageList, CXTPTabManager::ShowIcons
	//-----------------------------------------------------------------------
	int GetImageIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the image index of the tab.
	// Parameters: nImage - Index of an image within an CXTPImageManager or CImageList
	//                      object that will be displayed in the tab button.
	// Remarks:
	//     This is the index of an image within an CXTPImageManager or CImageList
	//     object that will be displayed in the tab button.  CXTPTabManager::ShowIcons
	//     is used to hide\show icons.
	// See Also: GetImageIndex, CXTPTabControl::InsertItem, CXTPTabControl::GetImageManager,
	//           CXTPTabControl::SetImageList, CXTPTabManager::ShowIcons
	//-----------------------------------------------------------------------
	void SetImageIndex(int nImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the tab item.
	// Remarks:
	//     This returns the "extra" information such as pointers or
	//     integers that was associated with the tab item using the SetData
	//     function.
	//
	//          NOTE: Do not use GetData when working with docking pane tabs.
	//                With DockingPanes, GetData returns a pointer to a CXTPDockingPane.
	//                Use CXTPDockingPane::GetPaneData when working with
	//                docking panes.
	// Returns:
	//     The 32-bit value associated with the tab item.
	// See Also: SetData, CXTPDockingPane::SetPaneData, CXTPDockingPane::GetPaneData
	//-----------------------------------------------------------------------
	DWORD_PTR GetData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the 32-bit value associated with the
	//     tab item.
	// Parameters:
	//     dwData - Contains the new value to associate with the tab item.
	// Remarks:
	//     This allows any "extra" information such as pointers or
	//     integers to be associated with the tab item.
	//
	//          NOTE: Do not use SetData when working with docking pane tabs.
	//                For DockingPanes, dwData stores a pointer to a CXTPDockingPane.
	//                Use CXTPDockingPane::SetPaneData when working with
	//                docking panes.
	// See Also: SetData, CXTPDockingPane::SetPaneData
	//-----------------------------------------------------------------------
	void SetData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to associate an object with the tab.  This
	//     will tell the tab what to display in the tab client area when
	//     the tab is selected.
	//     I.e. The HWND of a dialog.
	// Parameters:   hWnd - HWND of object that should be displayed in this tab
	//                      when the tab is selected.
	// See Also: GetHandle
	//-----------------------------------------------------------------------
	void SetHandle(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the HWND of the object associated
	//     with the tab.  This is the HWND of the object that is displayed
	//     in the tab client area when the tab is selected.
	// Returns:
	//     HWND of object associated with the tab.
	// See Also: SetHandle
	//-----------------------------------------------------------------------
	HWND GetHandle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the bounding rectangle of the tab.
	// Parameters:   rcItem - Bounding rectangle of tab.
	// See Also: GetRect
	//-----------------------------------------------------------------------
	void SetRect(CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tab is visible.
	// Returns:
	//     TRUE if the tab is visible, FALSE otherwise.
	// Remarks:
	//     A tab might not be visible because of the tab layout.  You
	//     can also use the SetVisible member to hide\show a tab.
	// See Also: SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide or show the tab.
	// Parameters: bVisible - TRUE to show the tab, FALSE to hide the tab.
	// See Also: IsVisible
	//-----------------------------------------------------------------------
	virtual void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tab is enabled.
	// Returns:
	//     TRUE if the tab is enabled, FALSE if the tab is disabled.
	// See Also: SetEnabled
	//-----------------------------------------------------------------------
	BOOL IsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set whether the tab is enabled.
	// Parameters:   bEnabled - TRUE to enable the tab, FALSE to disable the tab.
	// Remarks:
	//     If FALSE, the tab will appear "grayed out" and the user can
	//     not access the tab.
	// See Also: IsEnabled
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get row of the item.
	//-----------------------------------------------------------------------
	int GetItemRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine if the item was shrinked.
	//-----------------------------------------------------------------------
	BOOL IsItemShrinked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get collection of navigate buttons.
	// Returns:
	//     Array of CXTPTabManagerNavigateButton classes.
	// See Also:
	//     CXTPTabManagerNavigateButtons, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButtons* GetNavigateButtons();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine it the tab is currently
	//     selected.  I.e. when a tab is clicked.
	// Returns:
	//     TRUE if the tab is currently selected, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine it the tab has focus
	// Returns:
	//     TRUE if the tab has focus, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsFocused() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the tab is highlighted.
	// Returns:
	//     TRUE if the tab is highlighted (If CXTPTabPaintManager::m_bHotTracking
	//     is TRUE), otherwise FALSE.
	// Remarks:
	//     A tab will be highlighted when CXTPTabPaintManager::m_bHotTracking
	//     is TRUE and the mouse pointer is positioned over the tab button.
	//     and when dragging a tab.  I.e. Reordering tabs.
	// See Also: IsPressed, IsFocused
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//    Call this method to determine if the tab is pressed
	// Returns:
	//     TRUE if the tab is pressed, otherwise FALSE.
	// See Also: IsHighlighted
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to delete this tab.
	//-------------------------------------------------------------------------
	void Remove();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to select this tab.
	// Remarks:
	//     This will invoke the OnItemClick method.  A tab can also be selected
	//     by clicking on the tab.
	//-------------------------------------------------------------------------
	void Select();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to reorder item.
	// Parameters:
	//     nIndex - New item position to be set.
	//-----------------------------------------------------------------------
	void Move(int nIndex);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to determine if item can be closed
	// Returns:
	//     TRUE if item can be closed.
	//-------------------------------------------------------------------------
	BOOL IsClosable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to forbid this tab to be closed
	// Parameters:
	//     bClosable - TRUE to allow user close this tab; FALSE to forbid
	//-----------------------------------------------------------------------
	void SetClosable(BOOL bClosable);

	//-----------------------------------------------------------------------
	// Summary: Call this method to get Markup element that renders caption of the tab
	// Returns: Pointer to CXTPMarkupUIElement element
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the CXTPTabManager object.
	//     The CXTPTabManager object manages all CXTPTabManagerItem objects.
	// Returns:
	//     Pointer to parent CXTPTabManager.
	//-------------------------------------------------------------------------
	CXTPTabManager* GetTabManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to draw icon of single tab
	// Parameters:
	//     pDC - Pointer to device context to draw
	//     rcIcon - Bounding rectangle of icon
	//     pImage - Pointer to image associated with tab
	// See Also: CXTPImageManagerIcon
	//-----------------------------------------------------------------------
	void DrawImage(CDC* pDC, CRect rcIcon, CXTPImageManagerIcon* pImage);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to update position of TabManager.
	//-----------------------------------------------------------------------
	void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item was removed from TabManager collection
	//-----------------------------------------------------------------------
	virtual void OnRemoved();

private:
	BOOL DrawRotatedImage(CDC* pDC, CRect rcItem, CXTPImageManagerIcon* pImage);

protected:
	int             m_nButtonLength;      // Width of the tab button.
	int             m_nContentLength;     // Width of the tab button.
	int             m_nImage;       // Image index, this is the index of an image within a CXTPImageManger or CImageList object.
	int             m_nIndex;       // Index of the tab within the collection of tabs.
	BOOL            m_bEnabled;     // TRUE if the tab is enabled.
	BOOL            m_bVisible;     // TRUE if the tab is visible.
	HWND            m_hWnd;         // A CWnd pointer to the window associated with a tab.
	HICON           m_hIcon;        // Icon associated with the tab.
	DWORD_PTR       m_dwData;       // Developer specific data
	CRect           m_rcItem;       // Rectangle of this tab.
	CString         m_strCaption;   // User specified label for the tab.
	CString         m_strToolTip;   // Tooltip text for the tab.
	COLORREF        m_clrItem;      // Color of tab button.
	CXTPTabManager* m_pTabManager;  // Parent item.
	int             m_nItemRow;     // Row of the item
	BOOL            m_bClosable;    // TRUE if item can be closed.

	CXTPTabManagerNavigateButtons m_arrNavigateButtons; // Array of navigate buttons
	CXTPMarkupUIElement* m_pMarkupUIElement;            // Markup element pointer.

private:
	BOOL            m_bFound;


	friend class CXTPTabManager;
	friend class CXTPTabPaintManager;
	friend class CXTPTabWorkspace;
	friend class CXTPTabClientWnd;
	friend class CXTPGroupClientWnd;


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetCaption(LPCTSTR lpszNewValue);
	afx_msg long OleGetIndex();
	afx_msg long OleGetTag();
	afx_msg void OleSetTag(long nNewValue);
	afx_msg BOOL OleGetSelected();
	afx_msg void OleSetSelected(BOOL bNewValue);
	afx_msg long OleGetImage();
	afx_msg void OleSetImage(long nNewValue);
	afx_msg void OleSetHandle(long hWnd);
	afx_msg void OleSetVisible(BOOL bVisible);
	afx_msg void OleGetRect(long* Left, long* Top, long* Right, long* Botton);
	afx_msg void OleEnsureVisible();
	DECLARE_DISPATCH_MAP();
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTabManagerItem)
	COleVariant m_oleTag;
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerItem

AFX_INLINE CXTPTabManager* CXTPTabManagerItem::GetTabManager() const{
	return m_pTabManager;
}
AFX_INLINE CRect CXTPTabManagerItem::GetRect() const{
	return m_rcItem;
}
AFX_INLINE int CXTPTabManagerItem::GetIndex() const {
	return m_nIndex;
}
AFX_INLINE DWORD_PTR CXTPTabManagerItem::GetData() const {
	return m_dwData;
}
AFX_INLINE void CXTPTabManagerItem::SetData(DWORD_PTR dwData) {
	m_dwData = dwData;
}
AFX_INLINE int CXTPTabManagerItem::GetImageIndex() const {
	return m_nImage;
}
AFX_INLINE BOOL CXTPTabManagerItem::IsClosable() const {
	return m_bClosable;
}
AFX_INLINE int CXTPTabManagerItem::GetButtonLength() const {
	return m_nButtonLength;
}
AFX_INLINE int CXTPTabManagerItem::GetContentLength() const {
	return m_nContentLength;
}
AFX_INLINE void CXTPTabManagerItem::SetTooltip(LPCTSTR lpszTooltip) {
	m_strToolTip = lpszTooltip;
}
AFX_INLINE int CXTPTabManagerItem::GetItemRow() const {
	return m_nItemRow;
}
AFX_INLINE BOOL CXTPTabManagerItem::IsItemShrinked() const {
	return m_nButtonLength < m_nContentLength;
}
AFX_INLINE CXTPTabManagerNavigateButtons* CXTPTabManagerItem::GetNavigateButtons() {
	return &m_arrNavigateButtons;
}
