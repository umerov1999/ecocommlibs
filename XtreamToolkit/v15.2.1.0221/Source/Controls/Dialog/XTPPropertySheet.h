// XTPPropertySheet.h : interface for the CXTPPropertySheet class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPPROPERTYSHEET_H__)
#define __XTPPROPERTYSHEET_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPPropertyPageNavigator;
class CXTPPropertyPage;

//-----------------------------------------------------------------------
// Summary:
//     XTPPropertyPageBorder is an enumeration used to set the client frame style drawn around the client of tabs.
// Example:
//     <code>wndSheet.SetPageBorderStyle(xtpPageBorderFrame);</code>
// See Also: CXTPPropertySheet
//
// <KEYWORDS xtpPageBorderNone, xtpPageBorderBottomLine, xtpPageBorderFrame>
//-----------------------------------------------------------------------
enum XTPPropertyPageBorder
{
	xtpPageBorderNone,
	xtpPageBorderBottomLine,
	xtpPageBorderFrame
};

//===========================================================================
// Summary:
//     CXTPPropertySheet is a CDialog derived class
//     It represents property sheet, otherwise known as tab dialog box.
//     A property sheet consists of a CXTPPropertySheet object and one or more CXTPPropertyPage objects.
// See Also:
//     CXTPPropertySheet::DoModal, CXTPPropertyPage, CXTPPropertyPageNavigator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertySheet : public CDialog
{
	DECLARE_DYNAMIC(CXTPPropertySheet)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertySheet object.
	// Parameters:
	//     nIDCaption - ID of the caption to be used for the property sheet.
	//     pParentWnd - Points to the parent window of the property sheet. If NULL, the parent window will be the main window of the application.
	//     iSelectPage  - The index of the page that will initially be on top. Default is the first page added to the sheet
	//     pszCaption  - Points to a string containing the caption to be used for the property sheet. Cannot be NULL
	// Remarks:
	//     Use this member function to construct a CPropertySheet object.
	//     To display the property sheet, call DoModal or Create.
	//     The string contained in the first parameter will be placed in the caption bar for the property sheet.
	//-----------------------------------------------------------------------
	CXTPPropertySheet();
	CXTPPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0); // <combine CXTPPropertySheet::CXTPPropertySheet>
	CXTPPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0); // <combine CXTPPropertySheet::CXTPPropertySheet>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertySheet object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPPropertySheet();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Displays a modeless property sheet.
	// Parameters:
	//     pParentWnd - Points to parent window. If NULL, parent is the desktop.
	//     dwStyle - Window styles for property sheet
	//     dwExStyle  - Extended window styles for property sheet
	// Remarks:
	//     The default style, expressed by passing -1 as dwStyle, is actually
	//     WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | DS_CONTEXT_HELP | WS_VISIBLE.
	//
	//     To display a modal property sheet, call DoModal instead.
	// See Also:
	//     CXTPPropertySheet::DoModal
	//-----------------------------------------------------------------------
	BOOL Create(CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD)-1, DWORD dwExStyle = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Displays a modal property sheet.
	// Returns:
	//     IDOK or IDCANCEL if the function was successful; otherwise 0 or -1.
	// Remarks:
	//     Call this member function to display a modal property sheet.
	//     The return value corresponds to the ID of the control that closed the property sheet.
	//     After this function returns, the windows corresponding to the property sheet and all the pages
	//     will have been destroyed.
	//
	//     To display a modeless property sheet, call Create instead.
	// See Also:
	//     CXTPPropertySheet::Create
	//-----------------------------------------------------------------------
	INT_PTR DoModal();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a page to the property sheet.
	// Parameters:
	//     pPage - Points to the page to be added to the property sheet. Cannot be NULL.
	// Remarks:
	//     This member function adds the supplied page with the rightmost tab in the property sheet.
	//     Add pages to the property sheet in the left-to-right order you want them to appear.
	//
	//     AddPage adds the CPropertyPage object to the CPropertySheet object's list of pages but does
	//     not actually create the window for the page. The framework postpones creation of the window for the page until the user selects that page.
	// See Also:
	//     CXTPPropertySheet::GetPageCount
	//-----------------------------------------------------------------------
	void AddPage(CXTPPropertyPage* pPage);
	void InsertPage(int nIndex, CXTPPropertyPage* pPage);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function removes a page from the property sheet and destroys the associated window.
	// Parameters:
	//     pPage - Points to the page to be removed from the property sheet. Cannot be NULL.
	//     nPage  - Index of the page to be removed. Must be between 0 and one less than the number of pages in the property sheet, inclusive.
	// See Also:
	//     CXTPPropertySheet::AddPage
	//-----------------------------------------------------------------------
	void RemovePage(int nPage);
	void RemovePage(CXTPPropertyPage* pPage); // <combine CXTPPropertySheet::RemovePage@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the number of pages in the property sheet.
	// Returns:
	//     The number of pages in the property sheet.
	// Remarks:
	//     Call this member function to determine the number of pages currently in the property sheet.
	// See Also:
	//     CXTPPropertySheet::AddPage, CXTPPropertySheet::GetPage
	//-----------------------------------------------------------------------
	int GetPageCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the specified page.
	// Parameters:
	//     nPage - Index of the desired page, starting at 0.
	//             Must be between 0 and one less than the number of pages in the property sheet, inclusive.
	// Returns:
	//     The pointer to the page corresponding to the nPage parameter.
	// Remarks:
	//     Call this member function to determine the number of pages currently in the property sheet.
	// See Also:
	//     CXTPPropertySheet::AddPage, CXTPPropertySheet::GetPageCount
	//-----------------------------------------------------------------------
	CXTPPropertyPage* GetPage(int nPage) const;


	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the specified page of the property sheet.
	// Parameters:
	//     pPage - Points to the page with the index to be found. Cannot be NULL
	// Returns:
	//     The index number of a page.
	// Remarks:
	//     Use this member function to retrieve the index number of the specified page in the property sheet.
	//     For example, you would use GetPageIndex to get the page index in order to use SetActivePage or GetPage..
	// See Also:
	//     CXTPPropertySheet::GetActivePage
	//-----------------------------------------------------------------------
	int GetPageIndex(CXTPPropertyPage* pPage) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the active page object.
	// Returns:
	//     The pointer to the active page.
	// Remarks:
	//     Call this member function to retrieve the property sheet window's active page.
	//     Use this member function to perform some action on the active page
	// See Also:
	//     CXTPPropertySheet::GetPage
	//-----------------------------------------------------------------------
	CXTPPropertyPage* GetActivePage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the active page of the property sheet.
	// Returns:
	//     The index number of the active page.
	// Remarks:
	//     Call this member function to get the index number of the property sheet window's active page,
	//     then use the returned index number as the parameter for GetPage.
	// See Also:
	//     CXTPPropertySheet::GetActivePage
	//-----------------------------------------------------------------------
	int GetActiveIndex() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     Programmatically sets the active page object.
	// Parameters:
	//     pPage - Points to the page to set in the property sheet. It cannot be NULL.
	//     nPage - Index of the page to set. It must be between 0 and one less than the number of pages in the property sheet, inclusive.
	// Returns:
	//     Nonzero if the property sheet is activated successfully; otherwise 0.
	// Remarks:
	//     Use this member function to change the active page.
	//     For example, use SetActivePage if a user's action on one page should cause another page to become the active page.
	// See Also:
	//     CXTPPropertySheet::GetActivePage
	//-----------------------------------------------------------------------
	BOOL SetActivePage(int nPage);
	BOOL SetActivePage(CXTPPropertyPage* pPage); // <combine CXTPPropertySheet::SetActivePage@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the caption of the property sheet.
	// Parameters:
	//     lpszText - Points to the text to be used as the caption in the title bar of the property sheet.
	// Remarks:
	//     Call this member function to specify the property sheet's caption
	//     (the text displayed in the title bar of a frame window).
	// See Also:
	//     CXTPPropertySheet::GetPage
	//-----------------------------------------------------------------------
	void SetTitle(LPCTSTR lpszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns bounding rectangle for all property pages.
	// Returns:
	//     CRect bounding rectangle for all property pages.
	//-----------------------------------------------------------------------
	CRect GetPageRect() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns currently used navigator object
	// Returns:
	//     CXTPPropertyPageNavigator class pointer presented navigator object
	// See Also:
	//     CXTPPropertySheet::SetNavigator
	//-----------------------------------------------------------------------
	CXTPPropertyPageNavigator* GetNavigator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets navigator object that will be used to switch property pages
	// Parameters:
	//     pNavigator - CXTPPropertyPageNavigator class pointer that will be set as new navigator
	// Remarks:
	//     CXTPPropertyPageNavigator class used to specify how user will navigate and switch pages.
	//     Default navigator classes are:
	//     * <b>CXTPPropertyPageTabNavigator</b> - represents TabControl navigator
	//     * <b>CXTPPropertyPageListNavigator</b> - represents List navigator
	//     * <b>CXTPPropertyPageTreeNavigator</b> - represents Tree navigator
	// Example:
	// <code>
	//     CXTPPropertyPageListNavigator* pList = new CXTPPropertyPageListNavigator();
	//     pList->SetListStyle(xtpListBoxOffice2007);
	//     ps.SetNavigator(pList);
	// </code>
	// See Also:
	//     CXTPPropertySheet::GetNavigator
	//-----------------------------------------------------------------------
	void SetNavigator(CXTPPropertyPageNavigator* pNavigator);


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new button to property sheet window
	// Parameters:
	//     nIDCaption - ID of the caption to be used for the property sheet button.
	//     nID        - identifier to be used for the property sheet button.
	// Returns:
	//     Nonzero if the property sheet is activated successfully; otherwise 0.
	// See Also:
	//     CXTPPropertySheet::RemoveButtons
	//-----------------------------------------------------------------------
	virtual BOOL AddButton(UINT nIDCaption, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes all buttons from property sheet
	// See Also:
	//     CXTPPropertySheet::AddButton
	//-----------------------------------------------------------------------
	void RemoveButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets border around client page rectangle
	// Parameters:
	//     nBorder - Border style to be set
	// See Also:
	//     XTPPropertyPageBorder, CXTPPropertySheet::GetPageBorderStyle
	//-----------------------------------------------------------------------
	void SetPageBorderStyle(XTPPropertyPageBorder nBorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves border style around client page rectangle
	// Returns:
	//     XTPPropertyPageBorder Border style.
	// See Also:
	//     XTPPropertyPageBorder, CXTPPropertySheet::SetPageBorderStyle
	//-----------------------------------------------------------------------
	XTPPropertyPageBorder GetPageBorderStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow user resize property sheet
	// Parameters:
	//     bResizable - TRUE to allow user resize property sheet
	//-----------------------------------------------------------------------
	void SetResizable(BOOL bResizable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set custom size for property pages
	// Parameters:
	//     szPage - New page size to be set
	//-----------------------------------------------------------------------
	void SetPageSize(CSize szPage);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function saves the window placement to the registry.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	// Returns:
	//     TRUE if succeeds
	//-----------------------------------------------------------------------
	BOOL SavePlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads saved window placement information from
	//     the registry.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	// Returns:
	//     TRUE if position was changed
	//-----------------------------------------------------------------------
	BOOL LoadPlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads saved window placement information from
	//     the registry. This version is the same as LoadPlacement but there
	//     is no need for calling SavePlacement when the window is destroyed.
	//     This will be called automatically.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	// Returns:
	//     TRUE if position was changed
	//-----------------------------------------------------------------------
	BOOL AutoLoadPlacement(LPCTSTR pszSection);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create all property sheet buttons
	//-----------------------------------------------------------------------
	virtual BOOL CreateButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create property page window
	// Parameters:
	//     pPage - Page to be created
	//-----------------------------------------------------------------------
	virtual BOOL CreatePage(CXTPPropertyPage* pPage);

//{{AFX_CODEJOCK_PRIVATE
public:
	CFont* GetFont();
	void EndDialog(int nEndID);
	void RecalcLayout(LPSIZE pPageSize = NULL, BOOL bMoveWindow = FALSE);
	BOOL IsWizard() const;
	HWND GetNavigatorHwnd() const;

protected:
	void CommonConstruct(CWnd* pParentWnd, UINT iSelectPage);

protected:
	virtual BOOL CreateClient();
	void ResizeParentToFit(CSize szClient, BOOL bMoveWindow);
	BOOL IsPositionRelativeToWindow();
	void MoveButtons(AFX_SIZEPARENTPARAMS* pLayout, CSize szClient);
	void CreateFont(CXTPPropertyPage* pActivePage);

	virtual CButton* CreateButton();

private:
	void PageInfoChanged(CXTPPropertyPage* pPage, BOOL bModified);
	BOOL OnButtonClick(UINT nButton);
	LRESULT SendPageNotify(CXTPPropertyPage* pPage, int code, LPARAM lParam = 0);
	TCHAR ToUpper(TCHAR vkTCHAR);
//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_VIRTUAL(CXTPPropertySheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertySheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CString m_strCaption;           // Caption of the dialog
	LPDLGTEMPLATE m_lpDlgTemplate;  // Dialog template
	CArray<CButton*, CButton*> m_arrButtons;    // Array of buttons of property sheet
	CXTPPropertyPageNavigator* m_pNavigator;    // Currently used navigator
	CFont m_fntNormal;                          // Font used to draw buttons and navigator
	CArray<CXTPPropertyPage*, CXTPPropertyPage*> m_arrPages;  // Array of propety pages
	CXTPPropertyPage* m_pActivePage;            // Pointer to active property page
	CRect m_rcPage;         // Bounding rectangle of active page
	BOOL m_bAllowApply;     // TRUE to allow enable apply button
	BOOL m_bModeless;       // TRUE if property sheet was created as modeless window
	BOOL m_bResizable;      // TRUE if dialog can be resized
	CSize m_szWindow;       // Last window size of property sheet
	CSize m_szButton;       // Button size
	CScrollBar m_scSizeIcon;    // Resize glyph
	XTPPropertyPageBorder  m_nPageBorderStyle;  // Client border style

	CString m_strSection;

public:
	PROPSHEETHEADER m_psh;  // The Windows PROPSHEETHEADER structure. Provides access to basic property sheet parameters.
};

AFX_INLINE int CXTPPropertySheet::GetPageCount() const {
	return (int)m_arrPages.GetSize();
}
AFX_INLINE CXTPPropertyPage* CXTPPropertySheet::GetPage(int nPage) const {
	return nPage >= 0 && nPage < m_arrPages.GetSize() ? m_arrPages.GetAt(nPage) : NULL;
}
AFX_INLINE CXTPPropertyPage* CXTPPropertySheet::GetActivePage() const {
	return m_hWnd ? m_pActivePage : GetPage(m_psh.nStartPage);
}
AFX_INLINE int CXTPPropertySheet::GetActiveIndex() const {
	return m_hWnd ? GetPageIndex(m_pActivePage) : m_psh.nStartPage;
}
AFX_INLINE CXTPPropertyPageNavigator* CXTPPropertySheet::GetNavigator() const {
	return m_pNavigator;
}
AFX_INLINE CFont* CXTPPropertySheet::GetFont() {
	return &m_fntNormal;
}
AFX_INLINE CRect CXTPPropertySheet::GetPageRect() const {
	return m_rcPage;
}
AFX_INLINE XTPPropertyPageBorder CXTPPropertySheet::GetPageBorderStyle() const {
	return m_nPageBorderStyle;
}
AFX_INLINE INT_PTR CXTPPropertySheet::DoModal() {
	return CDialog::DoModal();
}


#endif // !defined(__XTPPROPERTYSHEET_H__)
