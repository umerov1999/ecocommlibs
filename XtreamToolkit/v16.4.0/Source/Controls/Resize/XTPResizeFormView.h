// XTPResizeFormView.h: interface for the CXTPResizeFormView class.
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
#if !defined(__XTPRESIZEFORMVIEW_H__)
#define __XTPRESIZEFORMVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPResizeFormView is a multiple inheritance class derived from CFormView
//     and CXTPResize. CXTPResizeFormView is used to create a resizable CFormView
//     type object that allows its form items to be resized or moved dynamically.
//===========================================================================
class _XTP_EXT_CLASS CXTPResizeFormView : public CFormView, public CXTPResize
{
	DECLARE_DYNCREATE(CXTPResizeFormView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResizeFormView object
	// Parameters:
	//     nID    - Contains the ID number of a dialog template resource.
	//     nFlags - Flags that are to be passed to CXTPResize that specify the attributes
	//              of the resizing property page. They can be one or more of the values
	//              in the Remarks section:
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//     * <b>xtpResizeNoSizeIcon</b> Do not add size icon.
	//     * <b>xtpResizeNoHorizontal</b> No horizontal resizing.
	//     * <b>xtpResizeNoVertical</b> No vertical resizing.
	//     * <b>xtpResizeNoMinsize</b> Do not require a minimum size.
	//     * <b>xtpResizeNoClipChildren</b> Do not set clip children style.
	//     * <b>xtpResizeNoTransparentGroup</b> Do not set transparent style
	//       for group boxes.
	//-----------------------------------------------------------------------
	CXTPResizeFormView(const UINT nID = 0, const UINT nFlags = 0);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method called in Size method to adjust window rectangle to fit real client size
	// Parameters:
	//     rcWindow - Window rectangle to change
	//-----------------------------------------------------------------------
	virtual void AdjustResizeRect(CSize& rcWindow);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPResizeFormView)
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPResizeFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

};

#endif // !defined(__XTPRESIZEFORMVIEW_H__)
