// XTPSkinObjectReBar.h: interface for the CXTPSkinObjectReBar class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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
#if !defined(__XTPSKINOBJECTREBAR_H__)
#define __XTPSKINOBJECTREBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPSkinObjectReBar is a CXTPSkinObjectFrame derived class.
//     It implements standard ReBar control
//===========================================================================
class _XTP_EXT_CLASS CXTPSkinObjectReBar : public CXTPSkinObjectFrame
{
	DECLARE_DYNCREATE(CXTPSkinObjectReBar)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSkinObjectReBar object.
	//-----------------------------------------------------------------------
	CXTPSkinObjectReBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSkinObjectReBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSkinObjectReBar();

private:
	void DrawBandBorders(CDC* pDC);
	void DrawBand(
		CDC* pDC,
		CXTPSkinManagerClass* pClassReBar,
		const CRect& rcBand,
		const REBARBANDINFO& rbbi);

	virtual BOOL CheckDrawingEnabled();

protected:
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPSkinObjectReBar)
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPSkinObjectReBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE
};

#endif // !defined(__XTPSKINOBJECTREBAR_H__)

