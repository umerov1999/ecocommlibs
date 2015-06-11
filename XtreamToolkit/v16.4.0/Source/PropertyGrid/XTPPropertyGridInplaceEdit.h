// XTPPropertyGridInplaceEdit.h interface for the CXTPPropertyGridInplaceEdit class.
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
#if !defined(__XTPPROPERTYGRIDINPLACEEDIT_H__)
#define __XTPPROPERTYGRIDINPLACEEDIT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class forwards.

class CXTPPropertyGridView;
class CXTPPropertyGridItem;

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceEdit is a CEdit derived class.
//     It is an internal class used by Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceEdit : public CXTPMaskEditT<CEdit>
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceEdit)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceEdit object
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceEdit object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set edit text value.
	// Parameters:
	//     strValue - String value.
	//-----------------------------------------------------------------------
	virtual void SetValue(LPCTSTR strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to hide in-place edit control.
	//-----------------------------------------------------------------------
	virtual void HideWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place edit control.
	// Parameters:
	//     pItem - Points to a CXTPPropertyGridItem object
	//     rect -  Specifies the size and position of the item.
	//-----------------------------------------------------------------------
	virtual void Create(CXTPPropertyGridItem* pItem, CRect rect);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a parent item is destroyed.
	//-----------------------------------------------------------------------
	virtual void DestroyItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves a child property grid item
	// Returns:
	//     A pointer to a CXTPPropertyGridItem object
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to select a constraint in child item constraint list.
	// Parameters:
	//     nDirection - Direction of constraint search.
	//     bCircle - TRUE to find by cycle.
	// Returns:
	//     TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL SelectConstraint(int nDirection, BOOL bCircle);

protected:

	CString              m_strValue;    // Previous item value
	CBrush               m_brBack;      // Background brush
	COLORREF             m_clrBack;     // Background color
	CXTPPropertyGridView* m_pGrid;      // Parent Grid view
	CXTPPropertyGridItem* m_pItem;      // Current Item
	BOOL m_bDelayCreate;                // TRUE to create Edit after activate
	BOOL m_bCreated;                    // TRUE if Create method was executed
	BOOL m_bEditMode;                   // TRUE after edit first receve focus
	BOOL m_bCancel;                     // User cancel edit with Escape
	BOOL m_bCheckAutoComplete;          // Check Auto Complete
	BOOL m_bIgnoreEditChanged;          // TRUE to Ignore EnChange notification

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPPropertyGridInplaceEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CXTPPropertyGridInplaceEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnChange();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	DECLARE_MESSAGE_MAP()
	friend class CXTPPropertyGridItem;
	friend class CXTPPropertyGrid;
};

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceControl is CWnd derived class, represents base class for all item in place controls.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceControl : public CWnd
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceControl)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceControl object
	// Parameters:
	//     pItem - Parent property grid item
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceControl(CXTPPropertyGridItem* pItem);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item creates its in-place control.
	// Parameters:
	//     rcValue - Boundary rectangle of control.
	// See Also: OnDestroyWindow
	//-----------------------------------------------------------------------
	virtual void OnCreateWindow(CRect& rcValue) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item destroys its in-place control.
	// See Also: OnCreateWindow
	//-----------------------------------------------------------------------
	virtual void OnDestroyWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item require to calculate its value rectangle
	// Parameters:
	//     rcValue - Bounding rectangle of value.
	// See Also: OnCreateWindow
	//-----------------------------------------------------------------------
	virtual void OnAdjustValueRect(CRect& rcValue) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when value of parent item was changed.
	//-----------------------------------------------------------------------
	virtual void OnValueChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when user drag mouse wheel in edit.
	// Parameters:
	//     pEdit  - Edit control currently selected.
	//     zDelta - Mouse wheel delta.
	// Returns:
	//     TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL OnEditMouseWheel(CEdit* pEdit, short zDelta);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	virtual void OnFinalRelease();

	//{{AFX_MSG(CXTPPropertyGridInplaceControl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPPropertyGridItem* m_pItem;          // Parent item pointer
	int m_nWidth;                           // Width of in-place item
};

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceSlider is CXTPPropertyGridInplaceControl derived class.
//     It represents in-place slider of the items. Call  CXTPPropertyGridItem::AddSliderControl() to
//     add slider for property grid item
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceSlider : public CXTPPropertyGridInplaceControl
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceSlider)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceSlider object
	// Parameters:
	//     pItem - Parent property grid item
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceSlider(CXTPPropertyGridItem* pItem);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item creates its in-place control.
	// Parameters:
	//     rcValue - Boundary rectangle of control.
	// See Also: OnDestroyWindow
	//-----------------------------------------------------------------------
	virtual void OnCreateWindow(CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item require to calculate its value rectangle
	// Parameters:
	//     rcValue - Bounding rectangle of value.
	// See Also: OnCreateWindow
	//-----------------------------------------------------------------------
	virtual void OnAdjustValueRect(CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when value of parent item was changed.
	//-----------------------------------------------------------------------
	virtual void OnValueChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves slider control.
	// Returns:
	//     A pointer to a CSliderCtrl object.
	//-----------------------------------------------------------------------
	CSliderCtrl* GetSliderCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set lower range of slider control
	// Parameters:
	//     nMin - lower range
	// See Also: SetMax
	//-----------------------------------------------------------------------
	void SetMin(int nMin);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set upper range of slider control
	// Parameters:
	//     nMax - upper range
	// See Also: SetMin
	//-----------------------------------------------------------------------
	void SetMax(int nMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves lower range of slider control.
	// Returns:
	//     Lower range of slider control.
	//-----------------------------------------------------------------------
	int GetMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves upper range of slider control.
	// Returns:
	//     Maximum range of slider control.
	//-----------------------------------------------------------------------
	int GetMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when user drag mouse wheel in edit.
	// Parameters:
	//     pEdit  - Edit control currently selected.
	//     zDelta - Mouse wheel delta.
	// Returns:
	//     TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL OnEditMouseWheel(CEdit* pEdit, short zDelta);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when slide was moved by user
	// Parameters:
	//     nSBCode - A slider code that indicates the user's scrolling request
	//     nPos    - Contains the current slider position
	//-----------------------------------------------------------------------
	virtual void OnScroll(UINT nSBCode, UINT nPos);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	int CalcualteEditWidth();

	//{{AFX_MSG(CXTPPropertyGridInplaceSlider)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridInplaceSlider);
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	int m_nMin;         // Lower range
	int m_nMax;         // Upper range
	int m_nValue;       // Value of slider

private:
	COLORREF m_clrBack;
	CBrush m_brBack;
};

AFX_INLINE CSliderCtrl* CXTPPropertyGridInplaceSlider::GetSliderCtrl() const {
	return (CSliderCtrl*)this;
}
AFX_INLINE void CXTPPropertyGridInplaceSlider::SetMin(int nMin) {
	m_nMin = nMin;
}
AFX_INLINE void CXTPPropertyGridInplaceSlider::SetMax(int nMax) {
	m_nMax = nMax;
}
AFX_INLINE int CXTPPropertyGridInplaceSlider::GetMin() const {
	return m_nMin;
}
AFX_INLINE int CXTPPropertyGridInplaceSlider::GetMax() const {
	return m_nMax;
}

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceSpinButton is CXTPPropertyGridInplaceControl derived class.
//     It represents in-place spin button of the items. Call  CXTPPropertyGridItem::AddSpinButton() to
//     add spin button for property grid item
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceSpinButton : public CXTPPropertyGridInplaceControl
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceSpinButton)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceSpinButton object
	// Parameters:
	//     pItem - Parent property grid item
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceSpinButton(CXTPPropertyGridItem* pItem);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item creates its in-place control.
	// Parameters:
	//     rcValue - Boundary rectangle of control.
	// See Also: OnDestroyWindow
	//-----------------------------------------------------------------------
	virtual void OnCreateWindow(CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item require to calculate its value rectangle
	// Parameters:
	//     rcValue - Bounding rectangle of value.
	// See Also: OnCreateWindow
	//-----------------------------------------------------------------------
	virtual void OnAdjustValueRect(CRect& rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when value of parent item was changed.
	//-----------------------------------------------------------------------
	virtual void OnValueChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves CSpinButtonCtrl control.
	// Returns:
	//     A pointer to a CSpinButtonCtrl object.
	//-----------------------------------------------------------------------
	CSpinButtonCtrl* GetSpinButtonCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set lower range of spin control
	// Parameters:
	//     nMin - lower range
	// See Also: SetMax
	//-----------------------------------------------------------------------
	void SetMin(int nMin);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set upper range of spin control
	// Parameters:
	//     nMax - upper range
	// See Also: SetMin
	//-----------------------------------------------------------------------
	void SetMax(int nMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves lower range of spin control.
	// Returns:
	//     Lower range of slider control.
	//-----------------------------------------------------------------------
	int GetMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves upper range of spin control.
	// Returns:
	//     Maximum range of slider control.
	//-----------------------------------------------------------------------
	int GetMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This virtual method is called when user drag mouse wheel in edit.
	// Parameters:
	//     pEdit  - Edit control currently selected
	//     zDelta - Mouse wheel delta
	//-----------------------------------------------------------------------
	virtual BOOL OnEditMouseWheel(CEdit* pEdit, short zDelta);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPPropertyGridInplaceSpinButton)
	afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridInplaceSpinButton);
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	int m_nMin;         // Lower range
	int m_nMax;         // Upper range
};

AFX_INLINE CSpinButtonCtrl* CXTPPropertyGridInplaceSpinButton::GetSpinButtonCtrl() const {
	return (CSpinButtonCtrl*)this;
}
AFX_INLINE void CXTPPropertyGridInplaceSpinButton::SetMin(int nMin) {
	m_nMin = nMin;
}
AFX_INLINE void CXTPPropertyGridInplaceSpinButton::SetMax(int nMax) {
	m_nMax = nMax;
}
AFX_INLINE int CXTPPropertyGridInplaceSpinButton::GetMin() const {
	return m_nMin;
}
AFX_INLINE int CXTPPropertyGridInplaceSpinButton::GetMax() const {
	return m_nMax;
}

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceControls is standalone class it represents
//     collection of CXTPPropertyGridInplaceControl classes.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceControls
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceControls object
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceControls();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceControls object, handles
	//     cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPPropertyGridInplaceControls();
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all in-place controls from collection.
	// See Also: Add
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve total number of controls in collection.
	// Returns:
	//     Total number of controls in collection.
	// See Also: GetAt
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new control to collection
	// Parameters:
	//     pWindow - Pointer to new in-place control
	// See Also: GetAt
	//-----------------------------------------------------------------------
	void Add(CXTPPropertyGridInplaceControl* pWindow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve control by its index.
	// Parameters:
	//     nIndex - Index of control to retrieve.
	// Returns:
	//     CXTPPropertyGridInplaceControl from the specified index.
	// See Also: GetCount, Add
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceControl* GetAt(int nIndex);


protected:
	CArray<CXTPPropertyGridInplaceControl*, CXTPPropertyGridInplaceControl*> m_arrControls;         // Array of controls
};


AFX_INLINE CXTPPropertyGridItem* CXTPPropertyGridInplaceEdit::GetItem() {
	return m_pItem;
}
AFX_INLINE BOOL CXTPPropertyGridInplaceEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPPropertyGridInplaceEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}
AFX_INLINE BOOL CXTPPropertyGridInplaceControl::OnEditMouseWheel(CEdit* /*pEdit*/, short /*zDelta*/) {
	return FALSE;
}


#endif // #if !defined(__XTPPROPERTYGRIDINPLACEEDIT_H__)
