// XTPRibbonBackstageView.h: interface for the CXTPRibbonBackstageView class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPRIBBONBACKSTAGEVIEW_H__)
#define __XTPRIBBONBACKSTAGEVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPRibbonControlSystemButton;

const UINT XTP_BS_TABCHANGED = 0x1100;

class _XTP_EXT_CLASS CXTPRibbonBackstageCommand : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPRibbonBackstageCommand)

protected:
	CXTPRibbonBackstageCommand();

protected:
	friend class CXTPRibbonBackstageView;
};

class _XTP_EXT_CLASS CXTPRibbonBackstageTab : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPRibbonBackstageTab)

public:
	CWnd* GetControlPane() const;

	void SetControlPaneMinSize(CSize sz);

protected:
	CXTPRibbonBackstageTab(HWND hwndControl = 0);



protected:

protected:
	HWND m_hwndControl;
	CSize m_szControlPaneMinSize;

	friend class CXTPRibbonBackstageView;
};

//===========================================================================
// Summary:
//     CXTPRibbonBackstageView is CDialog derived class used for Backstage parent frame
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonBackstageView : public CXTPCommandBar
{
	DECLARE_XTP_COMMANDBAR(CXTPRibbonBackstageView);

protected:
	CXTPRibbonBackstageView();
	~CXTPRibbonBackstageView();

public:
	static CXTPRibbonBackstageView* AFX_CDECL CreateBackstageView(CXTPCommandBars* pCommandBars);



public:
	void SetActiveTab(CXTPRibbonBackstageTab* pTab);
	void SetFocusedControl(CXTPControl* pFocusedControl);

	CXTPImageManager* GetImageManager() const;

	CXTPControl* HitTest(CPoint point) const;
	int IndexOf(CXTPControl* pControl) const;
	void OnExecute(CXTPControl* pControl);

	CWnd* GetActiveControlPane() const;

public:
	CXTPRibbonBackstageCommand* AddCommand(UINT nID);
	CXTPRibbonBackstageTab* AddTab(CWnd* pWnd, UINT nID);

public:
	virtual void OnCancel();

protected:
	void RecalcLayout();
	void DrawControl(CDC* pDC, CXTPControl* pControl);

	BOOL Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst = FALSE);
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);

	BOOL IsCommand(CXTPControl* pControl) const;
	void ShowBackstageView();


protected:
	DECLARE_MESSAGE_MAP();

	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnMouseLeave();
	afx_msg void OnDestroy();


protected:

protected:
	int m_nMenuWidth;

	CFont m_fntCommand;
	CFont m_fntTab;

	CXTPControl* m_pHighlightedControl;
	CXTPControl* m_pFocusedControl;
	CXTPRibbonBackstageTab* m_pActiveTab;
	CXTPRibbonControlSystemButton* m_pSystemButton;
	CArray<HWND, HWND> m_arrChildWindows;
};

AFX_INLINE CXTPImageManager* CXTPRibbonBackstageView::GetImageManager() const {
	return CXTPCommandBar::GetImageManager();
}
AFX_INLINE void CXTPRibbonBackstageTab::SetControlPaneMinSize(CSize sz) {
	m_szControlPaneMinSize = sz;
}

#endif // !defined(__XTPRIBBONBACKSTAGEVIEW_H__)
