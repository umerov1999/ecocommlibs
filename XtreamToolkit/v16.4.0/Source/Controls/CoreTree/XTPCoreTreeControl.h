// XTPCoreTreeControl.h: interface for the CXTPCoreTreeControl class.
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
#if !defined(__XTPCORETREECONTROL_H__)
#define __XTPCORETREECONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define XTP_TVN_CHECKCHANGED   (TVN_FIRST-110)

class CXTPCoreTreeItems;
class CXTPCoreTreePaintManager;
class CXTPCoreTreeItem;
class CXTPImageManager;
class CXTPMarkupContext;

enum XTPCoreTreeDropPosition
{
	xtpCoreTreeDropBefore,
	xtpCoreTreeDropAfter,
	xtpCoreTreeDropChild,
};

/////////////////////////////////////////////////////////////////////////////
// CXTPCoreTreeControl window
class _XTP_EXT_CLASS CXTPCoreTreeControl : public CWnd
{
	DECLARE_DYNAMIC(CXTPCoreTreeControl)
	class CTreeDropTarget;

public:
	CXTPCoreTreeControl();
	virtual ~CXTPCoreTreeControl();

public:
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

public:
	CXTPCoreTreeItems* GetItems() const;
	void ResetContent();

	void SetFocusedItem(CXTPCoreTreeItem* pItem);
	CXTPCoreTreeItem* GetFocusedItem() const;

	CXTPCoreTreeItem* GetDragOverItem() const;
	XTPCoreTreeDropPosition GetDropPosition() const;



public:
	void SetPaintManager(CXTPCoreTreePaintManager* pPaintManager);
	CXTPCoreTreePaintManager* GetPaintManager() const;

	void SetImageManager(CXTPImageManager* pImageManager);
	CXTPImageManager* GetImageManager() const;


public:
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

public:
	void OnDraw(CDC* pDC, CRect rcClipBox);
	void RecalcLayout();

	void SetLockRedraw(BOOL bLockRedraw);

	void OnCheckClicked(CXTPCoreTreeItem* pItem);


public:
	void RedrawControl(LPCRECT lpcRect = NULL);
	CXTPCoreTreeItem* HitTest(CPoint point) const;

	void EnsureVisible(CXTPCoreTreeItem* pItem);

	int GetScrollOffset() const;
	void OnScrollChanged(int nScrollOffset);


public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to enable markup for tab captions
	// Parameters:
	//     bEnable - TRUE to enable markup
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns markup context
	//-------------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;


public:
	void SetDragOverItem(CXTPCoreTreeItem* pDragOveritem, XTPCoreTreeDropPosition nDropPosition = xtpCoreTreeDropAfter);

public:
	static CLIPFORMAT AFX_CDECL GetClipboardFormat();

public:
	LRESULT SendMessageToParent(int nMessage, NMHDR* pNMHDR = NULL);

protected:
	virtual void OnItemRemoved(CXTPCoreTreeItem* pItem);

protected:
	int RecalcTreeItems(CXTPCoreTreeItems* pItems, CDC* pDC, int nLevel, LPCRECT lprcMargin);
	void DrawTreeItems(CXTPCoreTreeItems* pItems, CDC* pDC, CRect rcClipBox);
	void ExpandeTreeItems(CXTPCoreTreeItems* pItems);

	CXTPCoreTreeItem* HitTestTreeItems(CXTPCoreTreeItems* pItems, CPoint point) const;

	CXTPCoreTreeItem* GetNextFocusableItem(CXTPCoreTreeItem* pFocusedItem) const;
	CXTPCoreTreeItem* GetPrevFocusableItem(CXTPCoreTreeItem* pFocusedItem) const;

	void UpdateScrollBar();

protected:
	BOOL DragItem(CXTPCoreTreeItem* pItem, DROPEFFECT dropEffectMask = DROPEFFECT_COPY | DROPEFFECT_MOVE);

	virtual void StartDragItem(CXTPCoreTreeItem* pItem);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragHover(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);


protected:
	//{{AFX_VIRTUAL(CXTPCoreTreeControl)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCoreTreeControl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnNcPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()


protected:
	CXTPCoreTreeItems* m_pItems;
	CXTPCoreTreePaintManager* m_pPaintManager;
	CXTPImageManager* m_pImageManager;
	BOOL m_bPreSubclassWindow;

	int m_nClientHeight;
	CRect m_rcControl;

	BOOL m_bScrollVisible;
	BOOL m_bInUpdateScrollBar;
	int m_nLockRedraw;                             // TRUE when control is locked.

	CXTPCoreTreeItem* m_pFocusedItem;

	CXTPCoreTreeItem* m_pPressedItem;
	CXTPCoreTreeItem* m_pDraggingItem;
	CXTPCoreTreeItem* m_pDragOverItem;
	XTPCoreTreeDropPosition m_nDropPosition;
	CPoint m_ptPressed;
	CTreeDropTarget* m_pDropTarget;                // Internal drag'n'drop helper.
	static CLIPFORMAT m_cfItem;                     // Clipboard format.

	CXTPMarkupContext* m_pMarkupContext;


	friend class CXTPCoreTreeItems;
	friend class CTreeDropTarget;
};


AFX_INLINE CXTPCoreTreeItems* CXTPCoreTreeControl::GetItems() const {
	return m_pItems;
}

AFX_INLINE CXTPCoreTreePaintManager* CXTPCoreTreeControl::GetPaintManager() const {
	return m_pPaintManager;
}
AFX_INLINE CXTPCoreTreeItem* CXTPCoreTreeControl::GetFocusedItem() const {
	return m_pFocusedItem;
}
AFX_INLINE CXTPImageManager* CXTPCoreTreeControl::GetImageManager() const {
	return m_pImageManager;
}
AFX_INLINE CXTPCoreTreeItem* CXTPCoreTreeControl::GetDragOverItem() const {
	return m_pDragOverItem;
}
AFX_INLINE XTPCoreTreeDropPosition CXTPCoreTreeControl::GetDropPosition() const {
	return m_nDropPosition;
}
AFX_INLINE CLIPFORMAT AFX_CDECL CXTPCoreTreeControl::GetClipboardFormat() {
	return m_cfItem;
}
AFX_INLINE CXTPMarkupContext* CXTPCoreTreeControl::GetMarkupContext() const {
	return m_pMarkupContext;
}


#endif // !defined(__XTPCORETREECONTROL_H__)
