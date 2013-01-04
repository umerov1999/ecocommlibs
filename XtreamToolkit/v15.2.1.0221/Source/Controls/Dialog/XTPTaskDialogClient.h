// XTPTaskDialogClient.h: interface for the CXTPTaskDialogClient class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPTASKCLIENTWND_H__)
#define __XTPTASKCLIENTWND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTaskDialogFrame;
class CXTPImageManagerIcon;
class CXTPMarkupUIElement;
class CXTPMarkupContext;
class CXTPMarkupObject;
class CXTPMarkupRoutedEventArgs;
class CXTPButton;

//===========================================================================
// CXTPTaskDialogClient window
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskDialogClient : public CDialog
{
public:
	class CStringMarkup
	{
	public:
		CStringMarkup();
		~CStringMarkup();

	public:
		BOOL IsEmpty() const {
			return m_strCaption.IsEmpty();
		}

		void SetCaption(const CString& strCaption);
		operator const CString& () const {
			return m_strCaption;
		}
		void SetFinalRect(const CRect& rcFinalRect);
	public:
		CString m_strCaption;
		CXTPMarkupUIElement* m_pUIElement;
		CXTPTaskDialogClient* m_pClient;
		CRect m_rcFinalRect;

	};
public:
	CXTPTaskDialogClient();
	virtual ~CXTPTaskDialogClient();

public:
	HRESULT SendNotify(UINT uNotify, WPARAM wParam = 0, LPARAM lParam = 0);

public:
	virtual BOOL CreateClient(BOOL bMoveWindow);

	BOOL IsLinkButtonsUsed() const;
	BOOL IsLinkButtonsIconVisible() const;
	BOOL HasLinkButtons() const;
	BOOL IsExpandedByDefault() const;
	BOOL IsExpandFooterArea() const;
	BOOL HasFixedWidth() const;
	BOOL IsPositionRelativeToWindow() const;
	BOOL IsProgressBarVisible() const;
	BOOL IsHyperlinksEnabled() const;
	BOOL IsFooterIconHandleUsed() const;
	BOOL IsMainIconHandleUsed() const;
	BOOL CanCancel() const;

	CXTPMarkupContext* GetMarkupContext() const;

protected:

	int                                     m_nMaxClientWidth;  // Maximum of client width
	int                                     m_nMinClientWidth;  // Minimum of client width
	int                                     m_nPadding;         // Padding of elements
	BOOL                                    m_bExpanded;        // TRUE if Expanded area is is expanded
	BOOL                                    m_bVerification;    // TRUE if verification button is checked
	BOOL                                    m_bUseSysIcons;     // TRUE to use system icons
	BOOL                                    m_bThemeReady;      // TRUE if System theme used
	HICON                                   m_hIconFooter;      // Footer icon
	HICON                                   m_hIconMain;        // Main instruction icon

	CXTPImageManagerIcon*                   m_pIconLinkGlyph;   // Link glyph

	COLORREF                                m_crContent;        // Content background color
	COLORREF                                m_crContentText;    // Content text color
	COLORREF                                m_crFooter;         // Footer color
	COLORREF                                m_crFooterText;     // Footer text color
	COLORREF                                m_crMainText;       // Main instruction text color
	COLORREF                                m_cr3DLight;        // 3D Ligh color
	COLORREF                                m_cr3DShadow;       // 3D Shadow color
	COLORREF                                m_cr3DHighLight;    // 3D HighLight color
	CFont                                   m_font;             // Content font
	CFont                                   m_fontLarge;        // Main instruction font
	CPoint                                  m_ptMargin;         // Margin of elements
	CPoint                                  m_ptPadding;        // Padding of elements
	CPoint                                  m_ptBtnSpacing;     // Space between buttons
	CPoint                                  m_ptBorder;         // Border space around dialog
	CSize                                   m_sizeIconFooter;   // Size of footer icon
	CSize                                   m_sizeIconMain;     // Size of main icon
	CSize                                   m_sizeCommandButton;    // Size of push buttons
	CRect                                   m_rcMainInstruction;    // Position of Main Instruction
	CRect                                   m_rcContent;            // Position of Content Text
	CRect                                   m_rcCommandButtons;     // Position of Push buttons
	CRect                                   m_rcExpandedInformation;    // Expanded Information
	CRect                                   m_rcFooter;             // Position of Footer
	CRect                                   m_rcRadioButtons;       // Position of Radio buttons
	CRect                                   m_rcLinkButtons;        // Position of Link Buttons
	CStringMarkup                           m_strMainInstruction;   // Main instruction text
	CStringMarkup                           m_strContent;           // Content text
	CStringMarkup                           m_strFooter;            // Footer text
	CStringMarkup                           m_strVerificationText;  // Verification text
	CStringMarkup                           m_strExpandedInformation;   // Expanded text
	CStringMarkup                           m_strExpandedControlText;   // Text for expanded button
	CStringMarkup                           m_strCollapsedControlText;  // Text for collapsed button
	TASKDIALOGCONFIG*                       m_pConfig;                  // Pointer to TASKDIALOGCONFIG
	CXTPMarkupContext*                      m_pMarkupContext;           // Markup context

	CButton*                                m_pBtnVerify;           // Verify button pointer
	CXTPButton*                             m_pBtnExpando;          // Expando button pointer
	CXTPTaskDialogLinkCtrl                  m_wndContent;           // Content window
	CXTPTaskDialogLinkCtrl                  m_wndExpandedInformation;       // ExpandedInformation window
	CXTPTaskDialogLinkCtrl                  m_wndFooter;            // Footer window
	CXTPTaskDialogProgressCtrl              m_wndProgressBar;       // ProgressBar window
	CEdit                                   m_wndInputBox;          // InputBox window
	CArray<CButton*,CButton*>               m_arrCommandButtons;    // Array of push buttons
	CArray<CButton*,CButton*>               m_arrLinkButtons;       // Array of link buttons
	CArray<CButton*,CButton*>               m_arrRadioButtons;      // Array of radio buttons

	int                                     m_nSelRadioButtonID; // ID of the radio button that was selected when the task dialog was closed.
	CMap<CWnd*,CWnd*,SIZE,SIZE&> m_mapWndSize;          // Size map
	BOOL                                    m_bMessageBoxStyle;         // TRUE to use extended style
	BOOL                                    m_bEnableMarkup;            // TRUE to enable markup
	BOOL                                    m_bShowInputBox;            // TRUE to show input box
	CString                                 m_strInputBoxText;          // Input box text.

protected:

	BOOL IsWindow(CWnd* pWnd) const;

	virtual void CreateIcons();
	virtual void LoadStrings();
	virtual void CreateFonts();

	CString LoadItemString(PCWSTR pszBuffer);

	virtual BOOL CreateCommandButtons();
	virtual BOOL CreateLinkButtons();
	virtual BOOL CreateRadioButtons();
	virtual BOOL CreateVerifyExpandButtons();
	virtual BOOL CreateProgressBar();
	virtual BOOL CreateInputBox();

	CButton* AddCommandButton(int nButtonID, int nIDLabel, CSize sizeButton);
	CButton* AddCommandButton(int nButtonID, LPCTSTR lpszLabel, CSize sizeButton);
	CButton* CreateButton(int nButtonID, LPCTSTR lpszLabel, CSize sizeButton, DWORD dwButtonStyle = BS_PUSHBUTTON | BS_MULTILINE, CXTPButtonTheme* pTheme = NULL);

	HICON CreateIcon(HICON hIcon, PCWSTR lpwzIcon, CSize sizeIcon, BOOL bUseHandle);

	CSize CalcCommandButtonSize();
	CSize CalcButtonSize(CString strButton, CPoint* pMarginButton = NULL, int nMaxWidth = 0);
	CSize CalcLinkButtonSize(CXTPButton* pButton, CString strButton, int nMaxCmdLinkWidth);

	CSize CalcDynamicLayout(int nIdealWidth);


	CRect CalcCommandRect(int nIdealWidth);
	int CalcIdealTextWidth(const CStringMarkup& strItemText, int nMinWidth, BOOL bLargeFont);
	CRect CalcTextRect(const CStringMarkup& strItemText, int nWidth, BOOL bLargeFont);
	int CalcLinkButtonIdealWidth(CString strButton, int cxWidth);
	void DrawText(CDC* pDC, const CStringMarkup& strItemText, CRect rcItem, COLORREF clrText, CFont* pFont);

	void RecalcLayout(BOOL bMoveWindow = FALSE);
	BOOL ResizeParentToFit(CSize szClient, BOOL bMoveWindow);
	void SetCommandButtonPos();
	void SetLinkButtonPos();
	void SetVerifyButtonPos();
	void SetExpandoButtonPos();
	void SetLinkCtrlPos();

	HICON LoadSysIcon(PCWSTR lpwzIcon, CSize sizeIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves a button to the desired location.
	// Parameters:
	//     pButton - Button to move.
	//     xPos - New size\position of the button.
	//     yPos - New size\position of the button.
	//     cx - New size\position of the button.
	//     cy - New size\position of the button.
	//     bRedraw - TRUE to redraw the button.
	//     rect - New size\position of the button.
	// Returns:
	//     TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL MoveButton(CButton* pButton, int xPos, int yPos, int cx, int cy, BOOL bRedraw = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves a button to the desired location.
	// Parameters:
	//     pButton - Button to move.
	//     rect - New size\position of the button.
	//     bRedraw - TRUE to redraw the button.
	// Returns:
	//     TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL MoveButton(CButton* pButton, const CRect& rect, BOOL bRedraw = FALSE);

	void ZOrder(CWnd* pWnd);
	void UpdateZOrder();

	int CalcClientWidth();

	virtual void RefreshMetrics();
	virtual void RefreshColors();

	void DeleteControls();

	virtual void DrawMainInstruction(CDC* pDC);
	virtual void DrawContent(CDC* pDC);
	virtual void DrawFootNote(CDC* pDC);
	virtual void DrawBackground(CDC* pDC, CRect rect);
	virtual void DrawExpandedInfo(CDC* pDC);

	void DrawIcon(CDC* pDC, HICON hIcon, CPoint pt, CSize sz);
	void OnExpandExpandoClicked();

//{{AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPTaskDialogClient)

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTaskDialogClient)

	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMarkupHyperlinkClick(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pArgs);
	afx_msg void OnDestroy();
	afx_msg void OnInputBoxChanged();

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

//}}AFX_CODEJOCK_PRIVATE

	friend class CStringMarkup;
};

AFX_INLINE BOOL CXTPTaskDialogClient::IsLinkButtonsUsed() const {
	return ((m_pConfig->dwFlags & (TDF_USE_COMMAND_LINKS | TDF_USE_COMMAND_LINKS_NO_ICON)) != 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsProgressBarVisible() const {
	return ((m_pConfig->dwFlags & (TDF_SHOW_PROGRESS_BAR | TDF_SHOW_MARQUEE_PROGRESS_BAR)) != 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsLinkButtonsIconVisible() const {
	return ((m_pConfig->dwFlags & TDF_USE_COMMAND_LINKS_NO_ICON) == 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::HasLinkButtons() const {
	return ((m_pConfig->pButtons != NULL) && (m_pConfig->cButtons > 0) && IsLinkButtonsUsed());
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsExpandedByDefault() const {
	return ((m_pConfig->dwFlags & TDF_EXPANDED_BY_DEFAULT) != 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsExpandFooterArea() const {
	return ((m_pConfig->dwFlags & TDF_EXPAND_FOOTER_AREA) != 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::HasFixedWidth() const {
	return (m_pConfig->cxWidth != 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsPositionRelativeToWindow() const {
	return m_pConfig->hwndParent && (m_pConfig->dwFlags & TDF_POSITION_RELATIVE_TO_WINDOW);
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsHyperlinksEnabled() const {
	return ((m_pConfig->dwFlags & TDF_ENABLE_HYPERLINKS) != 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsFooterIconHandleUsed() const {
	return ((m_pConfig->dwFlags & TDF_USE_HICON_FOOTER) != 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsMainIconHandleUsed() const {
	return ((m_pConfig->dwFlags & TDF_USE_HICON_MAIN) != 0);
}
AFX_INLINE BOOL CXTPTaskDialogClient::IsWindow(CWnd* pWnd) const {
	return (pWnd && ::IsWindow(pWnd->GetSafeHwnd()));
}
AFX_INLINE BOOL CXTPTaskDialogClient::MoveButton(CButton* pButton, const CRect& rect, BOOL bRedraw /*=FALSE*/) {
	return MoveButton(pButton, rect.left, rect.top, rect.Width(), rect.Height(), bRedraw);
}
AFX_INLINE CXTPMarkupContext* CXTPTaskDialogClient::GetMarkupContext() const {
	return m_pMarkupContext;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTPTASKCLIENTWND_H__)
