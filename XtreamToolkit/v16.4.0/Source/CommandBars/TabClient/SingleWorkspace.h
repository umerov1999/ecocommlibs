// SingleWorkspace.h : header file
//
// This file is a part of the XTREME CALENDAR MFC class library.
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
#if !defined(__SINGLEWORKSPACE_H__)
#define __SINGLEWORKSPACE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CSingleWorkspace is a CWorkspace derived class. internal used.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabClientWnd::CSingleWorkspace : public CWnd, public CWorkspace
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CSingleWorkspace object, handles cleanup and deallocation
	//-------------------------------------------------------------------------
	~CSingleWorkspace();

	//-----------------------------------------------------------------------
	// Summary:
	//     Initiates redrawing of the control
	// Parameters:
	//     lpRect - The rectangular area of the window that is invalid.
	//     bAnimate - TRUE to animate changes in bounding rectangle.
	// Remarks:
	//     Call this member function if you want to initialize redrawing
	//     of the control. The control will be redrawn taking into account
	//     its latest state.
	//-----------------------------------------------------------------------
	virtual void RedrawControl(LPCRECT lpRect, BOOL bAnimate);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to determine whether a
	//     point is in the bounding rectangle of the specified tool.
	// Parameters:
	//     point - Specifies the x- and y coordinate of the cursor. These
	//             coordinates are always relative to the upper-left corner of the window
	//     pTI   - A pointer to a TOOLINFO structure.
	// Returns:
	//     If the tooltip control was found, the window control ID. If
	//     the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get window handle of workspace
	//-----------------------------------------------------------------------
	virtual CWnd* GetWindow() const;

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//{{AFX_MSG(CSingleWorkspace)
	public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

};

//{{AFX_CODEJOCK_PRIVATE
#endif //#if !defined(__SINGLEWORKSPACE_H__)
//}}AFX_CODEJOCK_PRIVATE
