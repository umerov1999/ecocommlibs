// XTPComboBoxEditCtrl.h : interface of the CXTPComboBoxEditCtrl class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPCOMBOBOXEDITCTRL_H__)
#define __XTPCOMBOBOXEDITCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

class _XTP_EXT_CLASS CXTPComboBoxEditCtrl : public CEdit
{

protected:

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPComboBox)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	void OnMouseLeave();
	void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

};

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPCOMBOBOXEDITCTRL_H__
//}}AFX_CODEJOCK_PRIVATE
