// XTPListBox.h interface for the CXTPListBox class.
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
#if !defined(__XTPLISTBOX_H__)
#define __XTPLISTBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

enum XTPListBoxStyle
{
	xtpListBoxStandard,
	xtpListBoxOfficeXP,
	xtpListBoxOffice2007,
};

//===========================================================================
// Summary:
//     CXTPListBox is a CListBox derived class. CXTPListBox extends the standard
//     list box control to enable flicker free drawing.
//===========================================================================
class _XTP_EXT_CLASS CXTPListBox : public CListBox
{
	DECLARE_DYNAMIC(CXTPListBox)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPListBox object
	//-----------------------------------------------------------------------
	CXTPListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPListBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPListBox();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize the list box.  This method
	//     should be called directly after creating or sub-classing the control.
	// Parameters:
	//     bAutoFont - True to enable automatic font initialization.
	//-----------------------------------------------------------------------
	virtual void Initialize(bool bAutoFont = true);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get background color of listbox
	//-----------------------------------------------------------------------
	virtual COLORREF GetBackColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set list style
	// Input:
	//     style - XTPListBoxStyle  style of the ListBox (xtpListBoxStandard, xtpListBoxOfficeXP, xtpListBoxOffice2007)
	// See Also: XTPListBoxStyle, GetListStyle
	//-----------------------------------------------------------------------
	void SetListStyle(XTPListBoxStyle style);


	void SetTheme(XTPControlTheme nTheme);


	//-----------------------------------------------------------------------
	// Summary: Call this method to get style of the ListBox
	// Returns: XTPListBoxStyle value contains style of listbox
	// See Also: XTPListBoxStyle, SetListStyle
	//-----------------------------------------------------------------------
	XTPListBoxStyle GetListStyle() const;

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	bool m_bPreSubclassInit;

	//{{AFX_VIRTUAL(CXTPListBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual void Init();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPListBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	XTPListBoxStyle m_nStyle;    // Listbox style.
	int m_nItemHeight;          // Height of the items.
	int m_nTextPadding;         // Text padding.
	int m_nHotItem;             // Hot item index.
};

AFX_INLINE void CXTPListBox::SetListStyle(XTPListBoxStyle style) {
	m_nStyle = style;
	if (m_hWnd) Invalidate(FALSE);
}
AFX_INLINE XTPListBoxStyle CXTPListBox::GetListStyle() const {
	return m_nStyle;
}


#endif // #if !defined(__XTPLISTBOX_H__)
