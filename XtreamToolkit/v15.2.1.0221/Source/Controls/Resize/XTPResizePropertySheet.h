// XTPResizePropertySheet.h: interface for the CXTPResizePropertySheet class.
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
#if !defined(__XTPRESIZEPROPERTYSHEET_H__)
#define __XTPRESIZEPROPERTYSHEET_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPResizePropertySheet is a multiple inheritance class derived from
//     CPropertySheet and CXTPResize. CXTPResizePropertySheet is used to create
//     a resizable CPropertySheet type object that allows its dialog items
//     to be resized or moved dynamically.
//===========================================================================
class _XTP_EXT_CLASS CXTPResizePropertySheet : public CPropertySheet, public CXTPResize
{
	DECLARE_DYNCREATE(CXTPResizePropertySheet)

public:
	//----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResizePropertySheet object
	// Parameters:
	//     nCaption    - ID of the caption to be used for the property sheet.
	//     pszCaption  - Points to a string containing the caption to be used for the property
	//                   sheet. It cannot be NULL.
	//     pParent     - Points to the parent window of the property sheet. If NULL, the parent
	//                   window will be the main window of the application.
	//     nSelectPage - The index of the page that will initially be on top. Default is the
	//                   first page added to the sheet.
	//     nFlags      - Flags that are to be passed to CXTPResize that specify the attributes
	//                   of the resizing property page. They can be one or more of the values in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//     * <b>xtpResizeNoSizeIcon</b> Do not add size icon.
	//     * <b>xtpResizeNoHorizontal</b> No horizontal resizing.
	//     * <b>xtpResizeNoVertical</b> No vertical resizing.
	//     * <b>xtpResizeNoMinsize</b> Do not require a minimum size.
	//     * <b>xtpResizeNoClipChildren</b> Do not set clip children style.
	//     * <b>xtpResizeNoTransparentGroup</b> Do not set transparent style
	//       for group boxes.
	//
	//----------------------------------------------------------------------
	CXTPResizePropertySheet();
	CXTPResizePropertySheet(const UINT nCaption, CWnd* pParent = 0, const UINT nSelectPage = 0, const UINT nFlags = 0); // <combine CXTPResizePropertySheet::CXTPResizePropertySheet>
	CXTPResizePropertySheet(LPCTSTR pszCaption, CWnd* pParent = 0, const UINT nSelectPage = 0, const UINT nFlags = 0); // <combine CXTPResizePropertySheet::CXTPResizePropertySheet>

protected:

	//----------------------------------------------------------------------
	// Summary:
	//     This member function determines if the property sheet is in wizard mode.
	// Returns:
	//     TRUE if the property sheet is a wizard, otherwise returns FALSE.
	//----------------------------------------------------------------------
	BOOL IsWizard();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPResizePropertySheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
	afx_msg BOOL OnPageChanging(NMHDR* pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

public:
	CXTPNoFlickerWnd <CTabCtrl> m_tabCtrl;  // Tab control.


	friend class CXTPResizePropertyPage;
};

//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#ifndef PSH_WIZARD_LITE
#define PSH_WIZARD_LITE         0x00400000
#endif  // PSH_WIZARD_LITE
//}}AFX_CODEJOCK_PRIVATE

AFX_INLINE BOOL CXTPResizePropertySheet::IsWizard() {
	return ((m_psh.dwFlags & (PSH_WIZARD | PSH_WIZARD97 | PSH_WIZARD_LITE)) != 0);
}

#endif // !defined(__XTPRESIZEPROPERTYSHEET_H__)
