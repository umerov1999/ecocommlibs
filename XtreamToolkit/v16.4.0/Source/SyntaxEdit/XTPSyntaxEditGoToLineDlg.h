// XTPSyntaxEditGoToLineDlg.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPSYNTAXEDITGOTOLINEDLG_H__)
#define __XTPSYNTAXEDITGOTOLINEDLG_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//      This class implements a GoToLine dialog for a SyntaxEdit control.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditGoToLineDlg : public CDialog
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default object constructor.
	// Parameters:
	//      pParent - pointer to a parent window.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditGoToLineDlg(CWnd* pParent = NULL);

	//{{AFX_CODEJOCK_PRIVATE
	enum { IDD = XTP_IDD_EDIT_GOTOLINE };
	CEdit   m_wndEditLineNo;
	int     m_iLineNo;
	CString m_csLineNo;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//      This method used to show dialog. If window is not created this
	//      method loads a dialog template from resources and created modeless
	//      dialog.
	// Parameters:
	//      pEditCtrl   - A pointer to CXTPSyntaxEditCtrl.
	//      bSelectLine - Select Line option state.
	//      bHideOnFind - Hide On Find option state.
	// Returns:
	//      TRUE if successful, FALSE otherwise.
	// See Also: ShowWindow
	//-----------------------------------------------------------------------
	BOOL ShowDialog(CXTPSyntaxEditCtrl* pEditCtrl, BOOL bSelectLine = FALSE, BOOL bHideOnFind = FALSE);

	//{{AFX_CODEJOCK_PRIVATE
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_CODEJOCK_PRIVATE

protected:

	//-----------------------------------------------------------------------
	// Summary: Load window position from application profile.
	// Returns: TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL LoadPos();

	//-----------------------------------------------------------------------
	// Summary: Save window position to application profile.
	// Returns: TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL SavePos();

	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OnChangeEditLineNo();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGoTo();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

	int                 m_iMaxLineNo;   // Store a maximum line number.
	BOOL                m_bHideOnFind;  // Store Hide On Find option.
	BOOL                m_bSelectLine;  // Store Select Line option.
	CPoint              m_ptWndPos;     // Store window position.
	CXTPSyntaxEditCtrl* m_pEditCtrl;    // Store attached edit control.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTPSYNTAXEDITGOTOLINEDLG_H__)
