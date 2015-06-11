// XTPRibbonBackstageView.h: interface for the CXTPRibbonBackstageView class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPRIBBONBACKSTAGEVIEW_H__)
#define __XTPRIBBONBACKSTAGEVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPRibbonControlSystemButton;
class CXTPRibbonBackstageCommand;
class CXTPRibbonBackstageTab;
class CXTPRibbonBackstagePaintManager;

const UINT XTP_BS_TABCHANGED = 0x1100;



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

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns the currently active tab.
	//-------------------------------------------------------------------------
	CXTPRibbonBackstageTab* GetActiveTab() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets the active tab.
	//-------------------------------------------------------------------------
	void SetActiveTab(CXTPRibbonBackstageTab *pTab);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns the currently focused control
	//-------------------------------------------------------------------------
	CXTPControl* GetFocusedControl() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets the focused control
	//-------------------------------------------------------------------------
	void SetFocusedControl(CXTPControl *pFocusedControl);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns the currently highlighted control
	//-------------------------------------------------------------------------
	CXTPControl* GetHighlightedControl() const;


	BOOL IsCommand(CXTPControl* pControl) const;
	BOOL IsActiveTab(CXTPControl *pControl) const;
	BOOL IsFocusedControl(CXTPControl *pControl) const;
	BOOL IsHighlightedControl(CXTPControl *pControl) const;


	CXTPImageManager* GetImageManager() const;

	CXTPControl* HitTest(CPoint point) const;
	int IndexOf(CXTPControl* pControl) const;
	void OnExecute(CXTPControl* pControl);

	CWnd* GetActiveControlPane() const;

	int GetMenuWidth() const;

	void SetTheme(XTPPaintTheme theme);

public:
	CXTPRibbonBackstageCommand* AddCommand(UINT nID);
	CXTPRibbonBackstageTab* AddTab(CWnd* pWnd, UINT nID);

public:
	virtual void OnCancel();

protected:
	void RecalcLayout();

	BOOL Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst = FALSE);
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);

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
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPRibbonBackstageView);

	afx_msg LPDISPATCH OleAddTab(long nId, LPCTSTR lpszCaption, OLE_HANDLE hWnd);
	afx_msg LPDISPATCH OleAddCommand(long nId, LPCTSTR lpszCaption);
	afx_msg void OleClose();
	afx_msg void OleSetTheme(long nTheme);

//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	int m_nMenuWidth;

	CFont m_fntCommand;
	CFont m_fntTab;

	CXTPRibbonBackstagePaintManager *m_pPaintManager;

	CXTPRibbonControlSystemButton *m_pSystemButton;
	CXTPRibbonBackstageTab *m_pActiveTab;
	CXTPControl *m_pHighlightedControl;
	CXTPControl *m_pFocusedControl;
	CArray<HWND, HWND> m_arrChildWindows;
};

AFX_INLINE CXTPImageManager* CXTPRibbonBackstageView::GetImageManager() const
{
	return CXTPCommandBar::GetImageManager();
}

AFX_INLINE int CXTPRibbonBackstageView::GetMenuWidth() const
{
	return m_nMenuWidth;
}

#endif // !defined(__XTPRIBBONBACKSTAGEVIEW_H__)
