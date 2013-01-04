// XTPPropertyPage.h : interface for the CXTPPropertyPage class.
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
#if !defined(__XTPPROPERTYPAGE_H__)
#define __XTPPROPERTYPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPPropertyPage is a CPropertyPage derived class
//     Objects of class CPropertyPage represent individual pages of a property sheet
// See Also:
//     CXTPPropertySheet, CXTPPropertySheet::DoModal, CXTPPropertyPageNavigator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyPage : public CPropertyPage, public CXTPResize
{
	DECLARE_DYNCREATE(CXTPPropertyPage)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyPage object.
	// Parameters:
	//     nIDTemplate - ID of the template used for this page.
	//     lpszTemplateName - Points to a string containing the name of the template for this page. Cannot be NULL
	//     nIDCaption - ID of the name to be placed in the tab for this page. If 0, the name will be taken from the dialog template for this page
	// Remarks:
	//     Use this member function to construct a CXTPPropertyPage object.
	//     To display the property sheet, call DoModal or Create.
	//     The string contained in the first parameter will be placed in the caption bar for the property sheet.
	//-----------------------------------------------------------------------
	CXTPPropertyPage();
	CXTPPropertyPage(UINT nIDTemplate, UINT nIDCaption = 0); // <combine CXTPPropertyPage::CXTPPropertyPage>
	CXTPPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0); // <combine CXTPPropertyPage::CXTPPropertyPage>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyPage object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPPropertyPage();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns caption text used for property sheet tabs
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets caption text used to draw property sheet tabs
	// Parameters:
	//     lpszCaption - New caption to be set
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

//{{AFX_CODEJOCK_PRIVATE
public:
	void EndDialog(int nID);

protected:
	virtual BOOL CreatePage(CWnd* pParentWnd);

	BOOL LoadTemplate();
	BOOL LoadTemplate(HGLOBAL& hTemplate, HINSTANCE& hInst);

	void CommonConstruct();

	void UpdateBars();
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	void GetScrollBarSizes(CSize& sizeSb);
	void GetScrollBarState(CSize sizeClient, CSize& needSb,
	CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);
	CPoint GetDeviceScrollPosition() const;
	void ScrollToDevicePosition(POINT ptDev);
	BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);
	BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll);
	virtual void AdjustResizeRect(CSize& rcWindow);
	void ScrollPage(int xAmount, int yAmount);
	void OnChildSetFocus(HWND hWndFocus);
//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertyPage)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

	friend class CXTPPropertySheet;

public:
	DWORD_PTR m_dwData; // Page specific 32-bit value

protected:
	CXTPPropertySheet* m_pSheet;    // Parent CXTPPropertySheet object
	BOOL m_bModified;               // TRUE to indicate that the property page settings have been modified
	BOOL m_bInsideUpdate;

	CSize m_totalDev;
	CSize m_lineDev;

	BOOL m_bCreated;
};

//{{AFX_CODEJOCK_PRIVATE

class _XTP_EXT_CLASS CXTPPropertyPageStaticCaption : public CWnd
{
public:
	CXTPPropertyPageStaticCaption();

public:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

};

//}}AFX_CODEJOCK_PRIVATE


#endif // !defined(__XTPPROPERTYPAGE_H__)
