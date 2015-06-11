// XTPPropertyGridToolTip.h interface for the CXTPPropertyGridToolTip class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#if !defined(__XTPPROPERTYGRIDTOOLTIP_H__)
#define __XTPPROPERTYGRIDTOOLTIP_H__
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPPropertyGridToolTip is a CWnd derived class.
//     It is an internal class used by Property Grid control
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridToolTip : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridToolTip object
	//-----------------------------------------------------------------------
	CXTPPropertyGridToolTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Activate Tooltip control.
	// Parameters:
	//     bActive - TRUE to activate.
	//     pItem   - Item which tooltip to show
	//     bValuePart - TRUE if tooltip of value part will be visible
	//-----------------------------------------------------------------------
	void Activate(BOOL bActive, CXTPPropertyGridItem* pItem, BOOL bValuePart);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates Tooltip control.
	// Parameters:
	//     pParentWnd - Points to a CXTPPropertyGridView object.
	//-----------------------------------------------------------------------
	void Create(CXTPPropertyGridView* pParentWnd);

protected:

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPPropertyGridView)
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	CXTPPropertyGridView* m_pGrid;      // Parent window.
	CFont m_fnt;                        // Font used to display the tool tip

	friend class CXTPPropertyGridView;
};

//{{AFX_CODEJOCK_PRIVATE
AFX_INLINE BOOL CXTPPropertyGridToolTip::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
//}}AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPPROPERTYGRIDTOOLTIP_H__
//}}AFX_CODEJOCK_PRIVATE
