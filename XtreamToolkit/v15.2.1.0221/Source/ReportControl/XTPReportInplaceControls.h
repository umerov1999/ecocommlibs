// XTPReportInplaceControls.h
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPREPORTINPLACECONTROLS_H__)
#define __XTPREPORTINPLACECONTROLS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportRecord;
class CXTPReportControl;
class CXTPReportRecordItem;
class CXTPReportRecordItemConstraints;
class CXTPReportRecordItemConstraint;



//===========================================================================
// Summary:
//     CXTPReportInplaceControl is the base class for all in-place controls of
//     report control item.
// Remarks:
//     There are 3 built-in in-place controls for Report control item:
//     * CXTPReportInplaceEdit - represents edit control to allow users change cell
//       of report control
//     * CXTPReportInplaceButton - represents single button of item.
//     * CXTPReportInplaceList - represents in-place list box to allow user select
//       constraints of report item
// See Also: CXTPReportInplaceEdit, CXTPReportInplaceButton, CXTPReportInplaceList
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceControl : public XTP_REPORTRECORDITEM_ARGS
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceControl object
	//-----------------------------------------------------------------------
	CXTPReportInplaceControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportInplaceControl object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportInplaceControl();

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called internal to fill item arguments of in-place control
	// Parameters:
	//     pItemArgs - Arguments of item.
	//-------------------------------------------------------------------------
	virtual void SetItemArgs(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

};

//===========================================================================
// Summary:
//     CXTPReportInplaceEdit is the CXTPReportInplaceControl derived  class,
//     it represents edit control to allow users change cell of report control.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceEdit : public CEdit, public CXTPReportInplaceControl
{
	DECLARE_DYNAMIC(CXTPReportInplaceEdit)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceEdit object
	//-----------------------------------------------------------------------
	CXTPReportInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportInplaceEdit object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportInplaceEdit();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to hide in-place edit control
	//-------------------------------------------------------------------------
	void HideWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves a child report control item
	// Returns:
	//     A pointer to a CXTPReportRecordItem
	//-----------------------------------------------------------------------
	CXTPReportRecordItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place edit control.
	// Parameters:
	//     pItemArgs - Parameters of item cell.
	//-----------------------------------------------------------------------
	void Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set edit text.
	// Parameters:
	//     lpszString - String to set.
	//-----------------------------------------------------------------------
	void SetWindowText(LPCTSTR lpszString);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to change font of in-place edit control
	// Parameters:
	//     pFont - New font to be set.
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPReportInplaceControl)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportInplaceControl)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:
	CString m_strValue;                                     // The value of item before user change it.
	CString m_strText_prev;                                 // The text value from previous change.
	CFont m_fntEdit;                                        // Font of in-place edit.
	CXTPReportRecordItemConstraint* m_pSelectedConstraint;  // Selected constraint.
	COLORREF m_clrText;                                     // Text color of in-place edit.
	BOOL m_bSetWindowText;                                  // Indicates that text is being set with SetWindowText function.
//private:

	friend class CXTPReportRecordItem;
};

AFX_INLINE CXTPReportRecordItem* CXTPReportInplaceEdit::GetItem() const
{
	return pItem;
}

AFX_INLINE BOOL CXTPReportInplaceEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

AFX_INLINE BOOL CXTPReportInplaceEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

const UINT XTP_ID_REPORT_EXPANDBUTTON = 100; //<ALIAS CXTPReportInplaceButton::CXTPReportInplaceButton@UINT>
const UINT XTP_ID_REPORT_COMBOBUTTON = 101; //<ALIAS CXTPReportInplaceButton::CXTPReportInplaceButton@UINT>
const UINT XTP_ID_REPORT_SPINBUTTON = 102; //<ALIAS CXTPReportInplaceButton::CXTPReportInplaceButton@UINT>

//===========================================================================
// Summary:
//     CXTPReportInplaceButton is the CXTPReportInplaceControl derived  class,
//     it represents combo button and expand button of report cell.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceButton : public CStatic, public CXTPReportInplaceControl
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceButton object
	// Parameters:
	//     nID - Identifier of in-place button. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     Default identifiers are:
	//     * <b>XTP_ID_REPORT_EXPANDBUTTON</b> Indicates the button acts like an expand button
	//     * <b>XTP_ID_REPORT_COMBOBUTTON</b>  Indicates the button acts like a combo button.
	//     * <b>XTP_ID_REPORT_SPINBUTTON</b>  Indicates the button acts like a spin button.
	//-----------------------------------------------------------------------
	CXTPReportInplaceButton(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place button control.
	// Parameters:
	//     pItemArgs - Parameters of item cell.
	//     rcButtons - Bounding rectangle of the button
	//-----------------------------------------------------------------------
	void Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CRect& rcButtons);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves identifier of the button
	// Returns:
	//     Identifier of the button.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if user pressed the button.
	// Returns:
	//     TRUE if user pressed the button.
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine the button state.
	// Returns:
	//     The button state.
	//-----------------------------------------------------------------------
	int GetState() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves a child report control item.
	// Returns:
	//     A pointer to a CXTPReportRecordItem
	//-----------------------------------------------------------------------
	CXTPReportRecordItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves button width.
	// Returns:
	//     A width of the button.
	//-----------------------------------------------------------------------
	int GetWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if button placed inside the cell rect,
	//     or outside (near right cell side).
	// Returns:
	//     TRUE if button is inside cell, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsInsideCellButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set button place inside the cell rect,
	//     or outside (near right cell side).
	// Parameters:
	//     bInsideCell - TRUE if button is inside cell, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetInsideCellButton(BOOL bInsideCell);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon index for the button.
	// Parameters:
	//     nIconIndex - icon index.
	// See Also:
	//     GetIconIndex
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the icon index for the button.
	// Returns:
	//     Icon index.
	// See Also:
	//     SetIconIndex
	//-----------------------------------------------------------------------
	int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the spin button properties.
	// Returns:
	//     The requested value.
	// See Also:
	//     SetSpinButtonMin, SetSpinButtonMax, SetSpinButtonStep
	//-----------------------------------------------------------------------
	int GetSpinButtonMin() const;
	int GetSpinButtonMax() const;   //<COMBINE CXTPReportInplaceButton::GetSpinButtonMin>
	int GetSpinButtonStep() const;  //<COMBINE CXTPReportInplaceButton::GetSpinButtonMin>

	//-----------------------------------------------------------------------
	// Summary:
	//     Use these member functions to set spin button properties.
	// Parameters:
	//     nValue - New Property value.
	// See Also:
	//     GetSpinButtonMin, GetSpinButtonMax, GetSpinButtonStep
	//-----------------------------------------------------------------------
	void SetSpinButtonMin(int nValue);
	void SetSpinButtonMax(int nValue); //<COMBINE CXTPReportInplaceButton::SetSpinButtonMin>
	void SetSpinButtonStep(int nValue); //<COMBINE CXTPReportInplaceButton::SetSpinButtonMin>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to activate the button.
	//-----------------------------------------------------------------------
	void Activate();
	int m_Items2Show; // Picklist desired item's number to show
protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPReportInplaceButton)
	virtual void OnFinalRelease();
	//{{AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportInplaceButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE



private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

protected:
	int m_nID;                              // Identifier of the button.
	int m_nWidth;                           // Width of the button.
	int m_nFixedHeight;                     // Height of the button.
	BOOL m_bInsideCell;                     // Store button place: inside the cell rect, or outside.
	int m_nIconIndex;                       // Icon index.

	BOOL m_bPressed;                        // TRUE if the button is pressed.
	BOOL m_bOver;                           // TRUE if the mouse cursor is over the button.
	int m_nState;                           // Button state.

	int m_nSpinMin;                         // Spin button only: minimum value.
	int m_nSpinMax;                         // Spin button only: maximum value.
	int m_nSpinStep;                        // Spin button only: step value.

	UINT_PTR m_unSpinTimerCnt;              // Spin button only: timer counter.
	UINT_PTR m_unSpinTimerId;               // Spin button only: timer identifier.
	int m_nSpinIncrement;                   // Spin button only: increment value.

	friend class CXTPReportRecordItem;
};

AFX_INLINE int CXTPReportInplaceButton::GetID() const {
	return m_nID;
}
AFX_INLINE BOOL CXTPReportInplaceButton::IsPressed() const {
	return m_bPressed && m_bOver;
}
AFX_INLINE int CXTPReportInplaceButton::GetState() const {
	return m_bOver ? m_nState : 0;
}
AFX_INLINE BOOL CXTPReportInplaceButton::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPReportInplaceButton::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}
AFX_INLINE CXTPReportRecordItem* CXTPReportInplaceButton::GetItem() const {
	return pItem;
}
AFX_INLINE void CXTPReportInplaceButton::SetIconIndex(int nIconIndex) {
	m_nIconIndex  = nIconIndex;
}
AFX_INLINE int CXTPReportInplaceButton::GetIconIndex() const {
	return m_nIconIndex;
}

//===========================================================================
// Summary:
//     CXTPReportInplaceList is the CXTPReportInplaceControl derived  class,
//     it represents list box with constraints of item.
// See Also: CXTPReportRecordItemConstraints
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceList : public CListBox, public CXTPReportInplaceControl
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceList object
	//-------------------------------------------------------------------------
	CXTPReportInplaceList();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place list control.
	// Parameters:
	//     pItemArgs - Parameters of item cell.
	//     pConstaints - constraints of item
	//-----------------------------------------------------------------------
	void Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraints* pConstaints);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to cancel user selection.
	//-------------------------------------------------------------------------
	void Cancel();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to save selected value of list box.
	//-------------------------------------------------------------------------
	void Apply();

	int m_Items2Show;
protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPReportInplaceList)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportInplaceList)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT, CPoint point);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called internal to fill item arguments of in-place control
	// Parameters:
	//     pItemArgs - Arguments of item.
	//-------------------------------------------------------------------------
	virtual void SetItemArgs(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called internally to react on selection changing.
	// Parameters:
	//     nLBIndex - An index of the selected item in the list.
	//-------------------------------------------------------------------------
	virtual void OnSelectionChanged(int nLBIndex);

private:
	CString m_strHotSearchContext;
	DWORD m_dwLastKeyDownTime;
	BOOL m_bApply;

	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPReportInplaceList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPReportInplaceList::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CListBox::Create(dwStyle, rect, pParentWnd, nID);
}
AFX_INLINE int CXTPReportInplaceButton::GetWidth() const {
	return m_nWidth;
}
AFX_INLINE BOOL CXTPReportInplaceButton::IsInsideCellButton() const {
	return m_bInsideCell;
}
AFX_INLINE void CXTPReportInplaceButton::SetInsideCellButton(BOOL bInsideCell) {
	m_bInsideCell = bInsideCell;
}
AFX_INLINE int CXTPReportInplaceButton::GetSpinButtonMin() const {
	return m_nSpinMin;
}
AFX_INLINE int CXTPReportInplaceButton::GetSpinButtonMax() const {
	return m_nSpinMax;
}
AFX_INLINE int CXTPReportInplaceButton::GetSpinButtonStep() const{
	return m_nSpinStep;
}
AFX_INLINE void CXTPReportInplaceButton::SetSpinButtonMin(int nValue) {
	m_nSpinMin = nValue;
}
AFX_INLINE void CXTPReportInplaceButton::SetSpinButtonMax(int nValue) {
	m_nSpinMax = nValue;
}
AFX_INLINE void CXTPReportInplaceButton::SetSpinButtonStep(int nValue) {
	m_nSpinStep = nValue;
}

#endif // #if !defined(__XTPREPORTINPLACECONTROLS_H__)
