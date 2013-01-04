// XTPShortcutBarPane.h interface for the CXTPShortcutBarPane class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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
#if !defined(__XTPSHORTCUTBARPANE_H__)
#define __XTPSHORTCUTBARPANE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPShortcutBar;
class CXTPShortcutBarPane;

//-----------------------------------------------------------------------
// Summary:
//     CXTPShortcutBarPaneItem is the base class representing an item of the panes.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPShortcutBarPaneItem
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarPaneItem object.
	// Parameters:
	//     lpszCaption - Caption of the item
	//     pWnd - Child window associated with item
	//     nHeight - Height of the item
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem(LPCTSTR lpszCaption, CWnd* pWnd, int nHeight);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns rectangle of caption.
	//-----------------------------------------------------------------------
	CRect GetCaptionRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption of the pane.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the text caption of the CXTPShortcutBarPaneItem.
	// Parameters:
	//     lpszCaption - New Caption of the pane.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set height of the item.
	// Parameters:
	//     nHeight - Height of client area of the item.
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine height of the item.
	// Returns:
	//     Height of the item.
	//-----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide caption
	// Parameters:
	//     bShowCaption - TRUE to show caption; FALSE to hide
	//-----------------------------------------------------------------------
	void ShowCaption(BOOL bShowCaption);

	//-----------------------------------------------------------------------
	// Summary: Determines if item currently highlighted
	// Returns: TRUE if the item is highlighted
	//-----------------------------------------------------------------------
	BOOL IsHighlighted() const;

	//-----------------------------------------------------------------------
	// Summary: Determines if user can expand and collapse item
	// Returns: TRUE if item can be expanded
	//-----------------------------------------------------------------------
	BOOL IsExpandable() const;

	//-----------------------------------------------------------------------
	// Summary: Determines if the item is expanded
	// Returns: TRUE if the item is currently expanded
	//-----------------------------------------------------------------------
	BOOL IsExpanded() const;

	//-----------------------------------------------------------------------
	// Summary: Call this method to allow expand items
	// Input:   bExpandable - TRUE to allow expand items
	//-----------------------------------------------------------------------
	void SetExpandable(BOOL bExpandable);

	// ---------------------------------------------------
	// Summary:
	//     Expands the item
	// Parameters:
	//     bExpanded :  TRUE to expand, FALSE to collapse.
	// ---------------------------------------------------
	void SetExpanded(BOOL bExpanded);

protected:
	CString m_strCaption;   // Caption of the item
	CRect m_rcCaption;      // Rectangle of the caption.
	CRect m_rcClient;       // Bounding rectangle of the client area.
	CWnd* m_pWndClient;     // Associated child window.
	int m_nHeight;          // Height of the item.
	BOOL m_bShowCaption;    // TRUE to show caption
	CXTPShortcutBarPane* m_pPane;   // Parent pane object

	BOOL m_bExpandable;     // TRUE if pane is expandable
	BOOL m_bExpanded;       // TRUE if pane is expanded

	friend class CXTPShortcutBarPane;
};

//===========================================================================
// Summary:
//     CXTPShortcutBarPane is a CWnd derived class. It is used as client area
//     of ShortcutBar control.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarPane : public CWnd
{
	DECLARE_DYNAMIC(CXTPShortcutBarPane)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarPane object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBarPane object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBarPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates the CXTPShortcutBarPane control.
	// Parameters:
	//     lpszCaption - Caption of the pane.
	//     pParent - parent CXTPShortcutBar class.
	//-----------------------------------------------------------------------
	BOOL Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the caption of the pane.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the text caption of the CXTPShortcutBarPane.
	// Parameters:
	//     lpszCaption - New Caption of the pane.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add sub item of pane.
	// Parameters:
	//     lpszCaption - Caption of the item that is to be added.
	//     pWnd - Associated child window of the item.
	//     nHeight - Height of the item.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem* AddItem(LPCTSTR lpszCaption, CWnd* pWnd, int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves item with specified index.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to a CXTPShortcutBarPaneItem object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the indent placed around the
	//     client area of the ShortcutBar.  The indent will be placed around the
	//     corresponding CXTPShortcutBarPaneItem when displayed.
	// Parameters:
	//     left   - Amount of space placed between the left client border
	//              and the left border of the ShortcutBar client area.
	//     top    - Amount of space placed between the top client border
	//              and the top border of the ShortcutBar client area.
	//     right  - Amount of space placed between the right client border
	//              and the right border of the ShortcutBar client area.
	//     bottom - Amount of space placed between the bottom client border
	//              and the bottom border of the ShortcutBar client area.
	//-----------------------------------------------------------------------
	void SetIndent(int left, int top, int right, int bottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide caption
	// Parameters:
	//     bShowCaption - TRUE to show caption; FALSE to hide
	//-----------------------------------------------------------------------
	void ShowCaption(BOOL bShowCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the minimum height of the client area
	// Parameters:
	//     nMinHeight - Specifies the minimum height in pixels of the client.
	//-----------------------------------------------------------------------
	void SetMinimumClientHeight(int nMinHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the minimum height of the client area
	// Returns:
	//     minimum height in pixels of the client.
	//-----------------------------------------------------------------------
	virtual int GetMinimumClientHeight() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified item.
	// Parameters:
	//     point - Specifies the point to be tested.
	// Returns:
	//     A pointer to a CXTPShortcutBarPaneItem control that occupies the specified
	//     point or NULL if no control occupies the point.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem* HitTest(CPoint point) const;

	//-------------------------------------------------------------------------
	// Summary: Recalculates layout of the Pane
	//-------------------------------------------------------------------------
	void RecalcLayout();

	CXTPShortcutBar* GetShortcutBar() const;



	CRect GetMinimizeButtonRect() const;
	BOOL IsMinimizeButtonHighlighted() const;
	BOOL IsMinimizeButtonPressed() const;

	virtual COLORREF GetBackgroundColor() const;
	virtual COLORREF GetTextColor() const;

	void SetFlatStyle(BOOL bFlatStyle);
	BOOL IsFlatStyle() const;


protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPShortcutBarPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg void OnDraw(CDC* pDC);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnMouseLeave();
	void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	CXTPShortcutBar* m_pShortcutBar;    // Parent CXTPShortcutBar class.
	CString m_strCaption;               // Caption of the pane.
	CArray<CXTPShortcutBarPaneItem*, CXTPShortcutBarPaneItem*> m_arrItems;  // Array of items.
	CRect m_rcIndent;                   // Indentation\padding around the client pane
	BOOL m_bShowCaption;                // TRUE to show caption
	int m_nMinClientHeight;             // The minimum height of client area.
	CXTPShortcutBarPaneItem* m_pHighlighted;            // Highlighted shortcut item
	HCURSOR m_hHandCursor; // Hand cursor

	CRect m_rcMinimizeButton;
	BOOL m_bMinimizeButtonHighlighted;
	BOOL m_bMinimizeButtonPressed;
	BOOL m_bFlatStyle;

	friend class CXTPShortcutBarPaneItem;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CString CXTPShortcutBarPane::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE void CXTPShortcutBarPane::SetIndent(int left, int top, int right, int bottom) {
	m_rcIndent.SetRect(left, top, right, bottom);
}
AFX_INLINE void CXTPShortcutBarPane::ShowCaption(BOOL bShowCaption) {
	m_bShowCaption = bShowCaption;
}
AFX_INLINE BOOL CXTPShortcutBarPane::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE CRect CXTPShortcutBarPaneItem::GetCaptionRect() const {
	return m_rcCaption;
}
AFX_INLINE int CXTPShortcutBarPaneItem::GetHeight() const {
	return m_nHeight;
}
AFX_INLINE BOOL CXTPShortcutBarPaneItem::IsHighlighted() const {
	return m_pPane->m_pHighlighted == this;
}
AFX_INLINE CString CXTPShortcutBarPaneItem::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE void CXTPShortcutBarPaneItem::ShowCaption(BOOL bShowCaption) {
	m_bShowCaption = bShowCaption;
}
AFX_INLINE void CXTPShortcutBarPane::SetMinimumClientHeight(int nMinHeight) {
	m_nMinClientHeight = nMinHeight;
}
AFX_INLINE int CXTPShortcutBarPane::GetMinimumClientHeight() const {
	return m_nMinClientHeight;
}
AFX_INLINE BOOL CXTPShortcutBarPaneItem::IsExpandable() const {
	return m_bExpandable && m_bShowCaption;
}
AFX_INLINE BOOL CXTPShortcutBarPaneItem::IsExpanded() const {
	return m_bExpanded;
}
AFX_INLINE void CXTPShortcutBarPaneItem::SetExpandable(BOOL bExpandable) {
	m_bExpandable = bExpandable;
}
AFX_INLINE void CXTPShortcutBarPaneItem::SetExpanded(BOOL bExpanded) {
	if (m_bExpanded != bExpanded)
	{
		m_bExpanded = bExpanded;
		m_pPane->RecalcLayout();
	}
}
AFX_INLINE CXTPShortcutBar* CXTPShortcutBarPane::GetShortcutBar() const {
	return m_pShortcutBar;
}
AFX_INLINE CRect CXTPShortcutBarPane::GetMinimizeButtonRect() const {
	return m_rcMinimizeButton;
}
AFX_INLINE BOOL CXTPShortcutBarPane::IsMinimizeButtonHighlighted() const {
	return m_bMinimizeButtonHighlighted;
}
AFX_INLINE BOOL CXTPShortcutBarPane::IsMinimizeButtonPressed() const {
	return m_bMinimizeButtonPressed;
}
AFX_INLINE void CXTPShortcutBarPane::SetFlatStyle(BOOL bFlatStyle) {
	m_bFlatStyle = bFlatStyle;
	if (m_hWnd) Invalidate(FALSE);
}
AFX_INLINE BOOL CXTPShortcutBarPane::IsFlatStyle() const {
	return m_bFlatStyle;
}

#endif // !defined(__XTPSHORTCUTBARPANE_H__)
