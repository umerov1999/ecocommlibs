// XTPCustomizeToolbarsPage.h : interface for the CXTPCustomizeToolbarsPage class.
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
#if !defined(__XTPCUSTOMIZETOOLBARSPAGE_H__) && defined(_XTP_INCLUDE_CONTROLS)
#define __XTPCUSTOMIZETOOLBARSPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCustomizeSheet;
class CXTPWinThemeWrapper;

//{{AFX_CODEJOCK_PRIVATE
class CXTPCustomizeToolbarsPageCheckListBox : public CCheckListBox
{
public:
	CXTPCustomizeToolbarsPageCheckListBox();
	~CXTPCustomizeToolbarsPageCheckListBox();

protected:
	BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void PreSubclassWindow();
	LRESULT OnSetFont(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

protected:
	CXTPWinThemeWrapper* m_themeHelper;
	CSize m_sizeCheck;
};
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPCustomizeToolbarsPage is a CPropertyPage derived class.
//     It represents the Toolbars page of the Customize dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPCustomizeToolbarsPage : public CXTPPropertyPage
{
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNCREATE(CXTPCustomizeToolbarsPage)
//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCustomizeToolbarsPage object
	// Parameters:
	//     pSheet - Points to a CXTPCustomizeSheet object that this page
	//     belongs to.
	//-----------------------------------------------------------------------
	CXTPCustomizeToolbarsPage(CXTPCustomizeSheet* pSheet = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCustomizeToolbarsPage object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	~CXTPCustomizeToolbarsPage();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Displays or hides the new toolbar button.
	// Parameters:
	//     bEnable - TRUE to allow display the add new toolbar button, FALSE to hide it.
	//-----------------------------------------------------------------------
	void AllowNewToolbars(BOOL bEnable);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPCustomizeToolbarsPage)
	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCustomizeToolbarsPage)
	afx_msg void OnSelectionChanged();
	afx_msg void OnChkChange();
	afx_msg void OnNewToolbar();
	afx_msg void OnRenameToolbar();
	afx_msg void OnDeleteToolbar();
	afx_msg void OnResetToolbar();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	void RefreshToolbarsList();

public:
	enum
	{
		IDD = XTP_IDD_PAGE_TOOLBARS     // Property page identifier
	};

	CXTPCustomizeSheet* m_pSheet;       // Parent property sheet
	CXTPCustomizeToolbarsPageCheckListBox m_checkList;  // Toolbars list box
	CButton m_btnReset;                 // Reset button
	CButton m_btnRename;                // Rename button
	CButton m_btnDelete;                // Delete button
	CButton m_btnNew;                   // New button

protected:
	BOOL m_bAllowNewToolbars;       // TRUE to allow display the add new toolbar button, FALSE to hide it.

};

AFX_INLINE void CXTPCustomizeToolbarsPage::AllowNewToolbars(BOOL bEnable) {
	m_bAllowNewToolbars = bEnable;
}

#endif // !defined(__XTPCUSTOMIZETOOLBARSPAGE_H__)
