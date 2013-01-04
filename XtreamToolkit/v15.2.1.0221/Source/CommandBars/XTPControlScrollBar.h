// XTPControlScrollBar.h : interface for the CXTPControlScrollBar class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPCONTROLSCROLLBAR_H__)
#define __XTPCONTROLSCROLLBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPControlScrollBar;


//===========================================================================
// Summary:
//     CXTPControlScrollBar is a CXTPControl derived class.
//     It represents an scrollbar control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlScrollBar : public CXTPControl, public CXTPScrollBase
{
	DECLARE_XTP_CONTROL(CXTPControlScrollBar)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlScrollBar object
	//-----------------------------------------------------------------------
	CXTPControlScrollBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlScrollBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPControlScrollBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the scrollbar current position
	// Parameters:
	//     nPos - New position of the scrollbar control
	// See Also: GetPos, SetRange
	//-----------------------------------------------------------------------
	void SetPos(int nPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the current position of the scrollbar
	// Returns:
	//     The position of the scrollbar control
	// See Also: SetPos
	//-----------------------------------------------------------------------
	int GetPos() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the upper and lower limits of the scrollbar control's range
	// Parameters:
	//     nMin - Specifies the lower limit of the range
	//     nMax - Specifies the upper limit of the range
	// See Also: SetPos
	//-----------------------------------------------------------------------
	void SetRange(int nMin, int nMax);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when position of scrollbar was changed
	// Parameters:
	//     nSBCode - Specifies a scroll-bar code that indicates the user's scrolling request
	//     nPos - Contains the current scroll-box position
	//-----------------------------------------------------------------------
	virtual void OnScroll(UINT nSBCode, UINT nPos);

protected:
//{{AFX_CODEJOCK_PRIVATE
	CSize GetSize(CDC* pDC);
	BOOL HasFocus() const;
	void Draw(CDC* pDC);
	void OnCalcDynamicSize(DWORD dwMode);
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));
	virtual int GetCustomizeMinWidth() const;
	virtual BOOL IsCustomizeResizeAllow() const;
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);
	void DoPropExchange(CXTPPropExchange* pPX);
	void OnMouseMove(CPoint point);
//}}AFX_CODEJOCK_PRIVATE


protected:
//{{AFX_CODEJOCK_PRIVATE
	virtual CRect GetScrollBarRect();
	virtual void GetScrollInfo(SCROLLINFO* psi);
	virtual void DoScroll(int cmd, int pos);
	virtual void RedrawScrollBar();
	virtual CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;
	virtual BOOL IsScrollBarEnabled() const;
	virtual CWnd* GetParentWindow() const;
//}}AFX_CODEJOCK_PRIVATE


protected:
	int m_nMin;             // Lower range
	int m_nMax;             // Upper range
	int m_nPos;             // Current position of scrollbar
	int m_nPage;            // Current page of the scrollbar

	BOOL m_bInScroll;


	friend class CXTPControlScrollBarCtrl;
};

AFX_INLINE BOOL CXTPControlScrollBar::IsCustomizeResizeAllow() const {
	return TRUE;
}
AFX_INLINE int CXTPControlScrollBar::GetCustomizeMinWidth() const {
	return 20;
}


#endif // !defined(__XTPCONTROLSCROLLBAR_H__)
