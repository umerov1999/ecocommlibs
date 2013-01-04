// XTPTabCtrlButtons.h : interface for the CXTPTabCtrlButtons class.
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
#if !defined(__XTPTABCTRLBUTTONS_H_)
#define __XTPTABCTRLBUTTONS_H_
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPTabCtrlButtons;
class CXTPTabBase;

#define XTP_IDC_TAB_CLOSE                106

//////////////////////////////////////////////////////////////////////
// Summary: CXTPTabCtrlButton is a stand alone structure class.  It is internal used in CXTPTabCtrlButtons class.
class _XTP_EXT_CLASS CXTPTabCtrlButton
{
public:
	CXTPTabCtrlButton (DWORD wStyle);

	void Draw(CDC* pDC, COLORREF clrButton);
	DWORD Click(CXTPTabCtrlButtons* pWnd, CPoint pt, BOOL bRepeat = FALSE);
	void SetRect(CRect rc);
	CRect GetRect();
	BOOL PtInRect(POINT pt) const;
	void CheckForMouseOver (CWnd* pWnd, CPoint pt);
	void SetEnable(BOOL bEnable);

	CRect m_Rect;
	DWORD m_wStyle;
	CTabCtrl* m_pTabCtrl;
	CXTPTabCtrlButtons* m_pButtons;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPTabCtrlButton::CXTPTabCtrlButton(DWORD wStyle) {
	m_wStyle = wStyle;
	m_pButtons = 0;
	m_pTabCtrl = 0;
	m_Rect.SetRectEmpty();
}
AFX_INLINE void CXTPTabCtrlButton::SetRect (CRect rc) {
	m_Rect = rc;
}
AFX_INLINE CRect CXTPTabCtrlButton::GetRect() {
	return m_Rect;
}
AFX_INLINE BOOL CXTPTabCtrlButton::PtInRect (POINT pt) const {
	return m_Rect.PtInRect (pt) != 0;
}

//////////////////////////////////////////////////////////////////////
// Summary: CXTPTabCtrlButtonsis a CWnd derived class.  It is internally used in the CXTPTabBase class.
class _XTP_EXT_CLASS CXTPTabCtrlButtons: public CWnd
{
public:
	CXTPTabCtrlButtons();
	~CXTPTabCtrlButtons();

	BOOL Create(CXTPTabBase* pCtrlBase);

	int GetWidth();

	void ShowButtons(BOOL bShow);
	BOOL ShowButtons();
	void Refresh();
	void SubclassTabButtons(CWnd* pWnd);
	void SetButtonStyle(DWORD dwFlags);
	void UnSubclassTabButtons();

	CTabCtrl* GetTabCtrl() const;

private:
	CRect AdjustRect();
private:
	CXTPTabCtrlButton m_btnClose;
	CXTPTabCtrlButton m_btnLeft;
	CXTPTabCtrlButton m_btnRight;

	DWORD m_dwFlags;
	CTabCtrl* m_pTabCtrl;
	CXTPTabBase* m_pTabCtrlBase;
	BOOL m_bShow;

	HWND m_hwndSubclassed;
	HWND m_hwndPrimary;

private:

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

	friend class CXTPTabBase;
	friend class CXTPTabCtrlButton;
};

AFX_INLINE CTabCtrl* CXTPTabCtrlButtons::GetTabCtrl() const {
	return m_pTabCtrl;
}

AFX_INLINE BOOL CXTPTabCtrlButtons::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // #if !defined(__XTPTABCTRLBUTTONS_H_)
