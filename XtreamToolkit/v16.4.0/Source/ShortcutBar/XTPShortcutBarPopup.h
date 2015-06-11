// XTPShortcutBarPopup.h interface for the CXTPShortcutBarPopup class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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
#if !defined(__XTPSHORTCUTBARPOPUP_H__)
#define __XTPSHORTCUTBARPOPUP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPShortcutBar;

class _XTP_EXT_CLASS CXTPShortcutBarPopup : public CWnd
{
public:
	CXTPShortcutBarPopup(CXTPShortcutBar* pShortcutBar, HWND hWndChild);
	~CXTPShortcutBarPopup();

public:
	void DoModal();
	void Close();

protected:
	DECLARE_MESSAGE_MAP();
	afx_msg void OnPaint();
	afx_msg LRESULT OnFloatStatus(WPARAM wParam, LPARAM);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg int OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	BOOL IsTrackingMode() const;
	void PumpMessage();

protected:
	HWND m_hWndChild;
	CXTPShortcutBar* m_pShortcutBar;

public:
	static int m_nPopupWidth;
};


#endif // !defined(__XTPSHORTCUTBARPOPUP_H__)
