// XTPTaskDialogFrame.h: interface for the CXTPTaskDialogFrame class.
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
#if !defined(__XTPTASKFRAMEWND_H__)
#define __XTPTASKFRAMEWND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskDialogFrame window

class _XTP_EXT_CLASS CXTPTaskDialogFrame : public CXTPTaskDialogClient
{
public:
	CXTPTaskDialogFrame(TASKDIALOGCONFIG* pConfig);
	virtual ~CXTPTaskDialogFrame();

public:
	virtual INT_PTR DoModal();
	BOOL Create(CWnd* pParentWnd = NULL);

protected:
	virtual CString GetFrameTitle() const;
	virtual DWORD GetFrameStyle() const;

protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTaskDialogFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTaskDialogFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnDialogSetElementText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogClickButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogClickRadioButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogEnableButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogEnableRadioButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogClickVerificationButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogUpdateIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogSetButtonElevationState(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnDialogSetMarqueeProgressBar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogSetProgressBarState(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogSetProgressBarRange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogSetProgressBarPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogStartProgressBarMarquee(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDialogNavigatePage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG


protected:
	LPDLGTEMPLATE m_lpDlgTemplate;
	DWORD m_dwTickCount;
	DWORD m_dwCallbackInterval;

private:
	friend class CXTPTaskCmdLinkButton;
};


#endif // !defined(__XTPTASKFRAMEWND_H__)
