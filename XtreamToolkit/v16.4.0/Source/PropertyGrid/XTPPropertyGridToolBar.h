// XTPPropertyGridToolBar.h: interface of the CXTPPropertyGridToolBar class.
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
#if !defined(__XTPPROPERTYGRIDTOOLBAR_H__)
#define __XTPPROPERTYGRIDTOOLBAR_H__
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPPropertyGridToolBar is a CToolBar derived class.
//     It is an internal class used by Property Grid control
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridToolBar : public CToolBar
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridToolBar object
	//-----------------------------------------------------------------------
	CXTPPropertyGridToolBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridToolBar object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPPropertyGridToolBar();

protected:

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPPropertyGridToolBar)
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertyGridToolBar)
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnPaint();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE
};

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPPROPERTYGRIDTOOLBAR_H__
//}}AFX_CODEJOCK_PRIVATE
