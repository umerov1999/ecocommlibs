// XTPControlGallery.h
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPCONTROLGALLERY_H__)
#define __XTPCONTROLGALLERY_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPControlGalleryItems;
class CXTPControlGallery;
class CXTPControlGalleryPaintManager;
class CXTPImageManagerIcon;
class CXTPMarkupUIElement;
class CXTPMarkupContext;

const UINT XTP_GN_PREVIEWSTART      = 0x1010;  // Gallery Notification - Occurs when the gallery preview is started.
const UINT XTP_GN_PREVIEWCANCEL     = 0x1011;  // Gallery Notification - Occurs when the gallery preview is canceled.
const UINT XTP_GN_PREVIEWAPPLY      = 0x1012;  // Gallery Notification - Occurs when the gallery preview is applied.
const UINT XTP_GN_PREVIEWCHANGE     = 0x1013;  // Gallery Notification - Occurs when the gallery preview is changed.
const UINT XTP_GN_STARTDRAG         = 0x1014;  // Gallery Notification - Occurs when an item is dragged in the gallery.
const UINT XTP_GN_SELCHANGE         = 0x1015;  // Gallery Notification - Occurs when the gallery selection has changed.

class CXTPControlGalleryItem;

//===========================================================================
// Summary:
//     CXTPControlGallery is a CXTPControlPopup derived class. It represents gallery control of toolbar.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGallery : public CXTPControlPopup, public CXTPScrollBase
{
	DECLARE_XTP_CONTROL(CXTPControlGallery)

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Structure contains item position description
	//-------------------------------------------------------------------------
	struct GALLERYITEM_POSITION
	{
		RECT rcItem;                    // Bounding rectangle of the item
		CXTPControlGalleryItem* pItem;  // Item pointer
		BOOL bBeginRow;                 // TRUE it item starts new row
	};


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGallery object
	//-----------------------------------------------------------------------
	CXTPControlGallery();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlGallery object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPControlGallery();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set items for gallery
	// Parameters:
	//     pItems - Pointer to items to be set
	//-----------------------------------------------------------------------
	void SetItems(CXTPControlGalleryItems* pItems);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set margins around items
	// Parameters:
	//     nLeft   - Left margin to be set
	//     nTop    - Top margin to be set
	//     nRight  - nRight margin to be set
	//     nBottom - Bottom margin to be set
	//-----------------------------------------------------------------------
	void SetItemsMargin(int nLeft, int nTop, int nRight, int nBottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set size of the control
	// Parameters:
	//     szControl - New control size to be set
	//-----------------------------------------------------------------------
	void SetControlSize(CSize szControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set size of the control. Depending on number of rows, columns, current item size and current DPI
	// Parameters:
	//     nRownColumns     - number of columns
	//     nRows            - number of rows
	//     szItem           - default item size
	//-----------------------------------------------------------------------
	void SetControlSize(int nColumns, int nRows, CSize szItem);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get count of items in gallery
	// Returns:
	//     Count of items in the gallery
	// See Also: SetItems, GetItem
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve item by its index
	// Parameters:
	//     nIndex - Index of the item to be retrieved
	// Returns:
	//     Item in specified index; NULL if failed
	// See Also: SetItems, GetItemCount
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get items associated with the gallery
	// See Also: SetItems
	//-----------------------------------------------------------------------
	virtual CXTPControlGalleryItems* GetItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set selected item in the gallery
	// Parameters:
	//     nSelected - Item to select
	// See Also: GetSelectedItem
	//-----------------------------------------------------------------------
	void SetSelectedItem(int nSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get selected item in the gallery
	// See Also: SetSelectedItem
	//-----------------------------------------------------------------------
	int GetSelectedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set checked item identifier for the gallery
	// Parameters:
	//     nId - Item id.
	// See Also: GetCheckedItem
	//-----------------------------------------------------------------------
	void SetCheckedItem(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if item is checked.
	// Parameters:
	//     pItem - Item to test.
	// Returns:
	//     TRUE if the specified item is checked, FALSE otherwise.
	// See Also: SetCheckedItem
	//-----------------------------------------------------------------------
	virtual BOOL IsItemChecked(CXTPControlGalleryItem* pItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get checked item identifier.
	// Returns:
	//     Checked item's identifier.
	// See Also: SetCheckedItem
	//-----------------------------------------------------------------------
	int GetCheckedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if there is selection in gallery.
	// Returns:
	//     TRUE if the item is checked, FALSE otherwise.
	// See Also: GetSelectedItem
	//-----------------------------------------------------------------------
	BOOL IsItemSelected() const;

public:

	static BOOL m_bAutoScaleDpi;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to draw borders around gallery
	// Parameters:
	//     bShowBorders - TRUE to show borders around gallery
	// See Also: IsShowBorders
	//-----------------------------------------------------------------------
	void ShowBorders(BOOL bShowBorders);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if borders are visible
	// See Also: ShowBorders
	//-----------------------------------------------------------------------
	BOOL IsShowBorders() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide labels of the gallery
	// Parameters:
	//     bShowLabels - TRUE to show labels
	//-----------------------------------------------------------------------
	void ShowLabels(BOOL bShowLabels);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show/hide scrollbar of the gallery
	// Parameters:
	//     bShowScrollBar - TRUE to show scrollbar
	//-----------------------------------------------------------------------
	void ShowScrollBar(BOOL bShowScrollBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if scrollbar is visible
	//-----------------------------------------------------------------------
	BOOL IsScrollBarVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to automatically show/hide scrollbar of the gallery
	// Parameters:
	//     bShowScrollBar - TRUE to show scrollbar auto
	//-----------------------------------------------------------------------
	void SetScrollBarAuto(BOOL bScrollBarAuto);
	BOOL IsScrollBarAuto() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines total borders width for gallery.
	// Returns:
	//     Rectangle contains border margins.
	//-----------------------------------------------------------------------
	virtual CRect GetBorders() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines bounding rectangle of the item.
	// Parameters:
	//     nIndex - Item index.
	// Returns:
	//     Bounding rectangle for the item.
	//-----------------------------------------------------------------------
	CRect GetItemDrawRect(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified item.
	// Parameters:
	//     point - Specifies the point to be tested.
	//     lpRect - Retrieves rectangle where item is located. Can be NULL
	// Returns:
	//     An index of CXTPControlGalleryItem that occupies the specified
	//     point or NULL if no control occupies the point.
	//-----------------------------------------------------------------------
	int HitTestItem(CPoint point, LPRECT lpRect = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines bounding rectangle of the items
	//-----------------------------------------------------------------------
	CRect GetItemsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraw items
	// Parameters:
	//     lpRect - Rectangle to redraw
	//     bAnimate - TURE to animate changes
	//-----------------------------------------------------------------------
	void InvalidateItems(LPCRECT lpRect = NULL, BOOL bAnimate = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves CXTPControlGalleryPaintManager pointer to draw gallery
	//-----------------------------------------------------------------------
	CXTPControlGalleryPaintManager* GetGalleryPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary: Retrieves CXTPScrollBarPaintManager pointer to draw ScrollBar
	// Returns: Pointer to CXTPScrollBarPaintManager object
	//-----------------------------------------------------------------------
	CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to make sure that an item is visible in the gallery.
	//     This will scroll the gallery until the item is visible.
	// Parameters:
	//     nIndex - Item index that should be visible.
	//-----------------------------------------------------------------------
	void EnsureVisible(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set scroll position of gallery
	// Parameters:
	//     nScrollPos - New scrollbar position to be set
	// See Also: GetScrollPos
	//-----------------------------------------------------------------------
	void SetScrollPos(int nScrollPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines position of scroll bar of gallery control
	// See Also: SetScrollPos
	//-----------------------------------------------------------------------
	int GetScrollPos() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to scroll gallery for specified top item
	// Parameters:
	//     nIndex - Item to be scrolled to.
	// See Also: SetScrollPos
	//-----------------------------------------------------------------------
	int SetTopIndex(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow user resize gallery
	// Parameters:
	//     bResizeWidth - Allow change width
	//     bResizeHeight - Allow change height
	// See Also: IsResizable
	//-----------------------------------------------------------------------
	void SetResizable(BOOL bResizeWidth = TRUE, BOOL bResizeHeight = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if control can be resized by user
	// See Also: SetResizable
	//-----------------------------------------------------------------------
	BOOL IsResizable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when selection was changed by user
	//-----------------------------------------------------------------------
	virtual void OnSelectedItemChanged();

public:

	//-----------------------------------------------------------------------
	// Summary: Determines if Gallery needs separator after last item
	// Returns: TRUE to draw separator
	//-----------------------------------------------------------------------
	BOOL HasBottomSeparator() const;

	//-----------------------------------------------------------------------
	// Summary: Returns next index of focusable item.
	// Input:   nIndex - Index to start
	//          nDirection - Direction to find (+1 or -1)
	// Returns: Index of focusable item
	//-----------------------------------------------------------------------
	long GetNext(long nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary: Returns next index of focusable item in same row.
	// Input:   nIndex - Index to start
	//          nDirection - Direction to find (+1 or -1)
	// Returns: Index of focusable item
	//-----------------------------------------------------------------------
	long GetNextInRow(long nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary: Returns next index of focusable item in same column.
	// Input:   nIndex - Index to start
	//          nDirection - Direction to find (+1 or -1)
	// Returns: Index of focusable item
	//-----------------------------------------------------------------------
	long GetNextInColumn(long nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary: Returns next index of focusable item.
	// Input:   nIndex - Index to start
	//          nDirection - Direction to find (+1 or -1)
	// Returns: Index of focusable item
	//-----------------------------------------------------------------------
	long GetNextInPage(long nIndex, int nDirection) const;

	//-----------------------------------------------------------------------
	// Summary: Call this method to search item
	// Input:   nStartAfter - Index to start search
	//          lpszItem - Item caption to find
	//          bExact - TRUE to find whole caption or only part
	// Returns: Index of item
	//-----------------------------------------------------------------------
	int FindItem(int nStartAfter, LPCTSTR lpszItem, BOOL bExact) const;

	//-----------------------------------------------------------------------
	// Summary: Returns caption of the item
	// Input:   nIndex - Index of item to get caption
	// Returns: Caption of item with nIndex index.
	//-----------------------------------------------------------------------
	CString GetItemCaption(int nIndex);

	//-----------------------------------------------------------------------
	// Summary: Determines if scrollbar part is enabled
	// Input:   ht - HitTest of scrollbar part
	// Returns: TRUE if part is enabled
	//-----------------------------------------------------------------------
	BOOL IsScrollButtonEnabled(int ht);

	//-----------------------------------------------------------------------
	// Summary: Determines if scrollbar is enabled
	// Returns: TRUE if scrollbar is enabled
	//-----------------------------------------------------------------------
	BOOL IsScrollBarEnabled() const;

	//-----------------------------------------------------------------------
	// Summary: Determines if gallery need to draw as simple button.
	// Returns: TRUE to show gallery as button
	//-----------------------------------------------------------------------
	BOOL IsShowAsButton() const;

	//-------------------------------------------------------------------------
	// Summary: Call this method to hide selection
	//-------------------------------------------------------------------------
	void HideSelection();

	void SetTransparent(BOOL bTransparent = TRUE);
	BOOL IsTransparent() const;


protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to set position of all items
	//-------------------------------------------------------------------------
	void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is called to draw all items
	// Parameters:
	//      pDC - Pointer to device context to draw
	//-----------------------------------------------------------------------
	void DrawItems(CDC* pDC);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Points to a source CXTPControl object
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called before recalculating the parent command
	//     bar size to calculate the dimensions of the control.
	// Parameters:
	//     dwMode - Flags used to determine the height and width of the
	//              dynamic command bar.
	//-----------------------------------------------------------------------
	void OnCalcDynamicSize(DWORD dwMode);

//{{AFX_CODEJOCK_PRIVATE
public:
	virtual void SetRect(CRect rcControl);
	virtual void OnItemsChanged();
	BOOL OnHookMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
	virtual CSize GetSize(CDC* pDC);
	virtual void Draw(CDC* pDC);

	virtual void OnMouseMove(CPoint point);

	virtual void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));
	virtual void OnLButtonUp(CPoint point);
	virtual BOOL OnSetSelected(int bSelected);
	void DoScroll(int cmd, int pos);
	void GetScrollInfo(SCROLLINFO* pSI);
	void PerformMouseMove(CPoint point);
	void PerformMouseDown(CPoint point);
	void PerformMouseUp(CPoint point);
	virtual void FillControl(CDC* pDC, CRect rcControl);
	virtual void SetAction(CXTPControlAction* pAction);
	virtual int GetCustomizeMinWidth() const;
	virtual int GetCustomizeMinHeight() const;
	virtual BOOL IsCustomizeResizeAllow() const;

protected:
	void DrawScrollBar(CDC* pDC);
	CRect GetScrollBarRect();
	void CalcScrollBarInfo(LPRECT lprc, XTP_SCROLLBAR_POSINFO* pSBInfo, SCROLLINFO* pSI);

	BOOL IsFocused() const;
	BOOL OnHookKeyDown(UINT nChar, LPARAM lParam);
	virtual void OnScrollChanged();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	void OnExecute();
	BOOL OnSetPopup(BOOL bPopup);
	void OnMouseHover();

protected:
	CWnd* GetParentWindow() const;
	virtual void AdjustExcludeRect(CRect& rc, BOOL /*bVertical*/);
	virtual void RedrawScrollBar();
	void DoAnimate();
	void StartAnimation(int nScrollPos);
	void RepositionCore(BOOL bCheckScroll);

//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	virtual HRESULT GetAccessibleChildCount(long* pcountChildren);
	virtual HRESULT GetAccessibleChild(VARIANT varChild, IDispatch** ppdispChild);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long *pxLeft, long *pyTop, long *pcxWidth, long* pcyHeight, VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual HRESULT GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction);
	virtual HRESULT AccessibleDoDefaultAction(VARIANT varChild);
	virtual HRESULT AccessibleSelect(long flagsSelect, VARIANT varChild);
//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlGallery);

	void OleSetItems(LPDISPATCH lpDispatch);
	LPDISPATCH OleGetItems();

	DECLARE_PROPERTY(SelectedItem, LPDISPATCH);
	DECLARE_PROPERTY(CheckedItem, int);

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:

	int m_nChecked;             // Checked id
	int m_nSelected;            // Index of Selected item
	BOOL m_bHideSelection;      // TRUE to hide selection
	BOOL m_bPressed;            // TRUE if item is pressed
	CPoint m_ptPressed;         // Position of pressed item
	int m_nScrollPos;           // Scroll position
	int m_nScrollPosTarget;     // Scroll position
	double m_dAnimationStep;    // Number of steps to complete animation

	BOOL m_bKeyboardSelected;   // TRUE if item was selected using keyboard

	int m_nTotalHeight;         // Total height of items

	BOOL m_bShowLabels;         // TRUE to show labels

	BOOL m_bShowScrollBar;      // TRUE to show scrollbars
	BOOL m_bShowBorders;        // TRUE to show borders
	CRect m_rcMargin;           // Margins of gallery items
	BOOL m_bPreview;            // TRUE if control in preview mode
	int m_nResizable;           // TRUE if the popup galley is resizable, see XTPGalleryResizable

	BOOL m_bScrollBarAuto;
	BOOL m_bTransparent;

	CArray<GALLERYITEM_POSITION, GALLERYITEM_POSITION&> m_arrRects; // bounding rectangles

	friend class CXTPControlGalleryItems;
};

#define GETPARTSTATE3(ht, bEnabled) (!bEnabled ? 4 : nPressedHt == ht ? 3 : nHotHt == ht ? 2 : 0)
#define IS_PART_SELECTED(nState)    (nState == 2)
#define IS_PART_PRESSED(nState)     (nState == 3)
#define IS_PART_ENABLED(nState)     (nState != 4)

//===========================================================================
// Summary:
//     CXTPControlGalleryPaintManager is standalone class used to draw CXTPControlGallery object
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGalleryPaintManager object
	// Parameters:
	//     pPaintManager - PaintManager of commandbars
	//-----------------------------------------------------------------------
	CXTPControlGalleryPaintManager(CXTPPaintManager* pPaintManager);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw label control
	// Parameters:
	//     pDC - Pointer to device context
	//     pLabel - Label item
	//     rc - Bounding rectangle of the label
	//-----------------------------------------------------------------------
	virtual void DrawLabel(CDC* pDC, CXTPControlGalleryItem* pLabel, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw background of the gallery
	// Parameters:
	//      pDC - Pointer to device context
	//      pGallery - CXTPControlGallery object to draw
	//      rc - Bounding rectangle of gallery object
	//-----------------------------------------------------------------------
	virtual void FillControl(CDC* pDC, CXTPControlGallery* pGallery, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to draw scrollbar of inplace gallery
	// Parameters:
	//     pDC - Pointer to device context
	//     pGallery - CXTPControlGallery object to draw
	// See Also: DrawScrollBar
	//-----------------------------------------------------------------------
	virtual void DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery);

	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates gallery metrics
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:
	int m_cyPopupUp;            // Height of Up arrow of in place scrollbar
	int m_cyPopupDown;          // Height of Down arrow of in place scrollbar
	int m_cxPopup;              // Width of arrows of in place scrollbar

	COLORREF m_clrBorder;

	CXTPPaintManager *m_pPaintManager;
};




AFX_INLINE BOOL CXTPControlGallery::IsShowBorders() const {
	return m_bShowBorders;
}
AFX_INLINE void CXTPControlGallery::ShowLabels(BOOL bShowLabels) {
	m_bShowLabels = bShowLabels;
}
AFX_INLINE void CXTPControlGallery::ShowScrollBar(BOOL bShowScrollBar) {
	m_bShowScrollBar = bShowScrollBar;
}
AFX_INLINE void CXTPControlGallery::ShowBorders(BOOL bShowBorders) {
	m_bShowBorders = bShowBorders;
}
AFX_INLINE BOOL CXTPControlGallery::IsItemSelected() const {
	return !m_bHideSelection && GetSelectedItem() != -1;
}
AFX_INLINE BOOL CXTPControlGallery::IsResizable() const {
	return !IsShowAsButton() && GetItems() ? m_nResizable : FALSE;
}
AFX_INLINE BOOL CXTPControlGallery::IsCustomizeResizeAllow() const {
	return IsResizable();
}
AFX_INLINE void CXTPControlGallery::SetScrollBarAuto(BOOL bScrollBarAuto) {
	m_bScrollBarAuto = bScrollBarAuto;
}
AFX_INLINE BOOL CXTPControlGallery::IsScrollBarVisible() const {
	return m_bShowScrollBar;
}
AFX_INLINE BOOL CXTPControlGallery::IsScrollBarAuto() const {
	return m_bScrollBarAuto;
}
AFX_INLINE void CXTPControlGallery::SetTransparent(BOOL bTransparent) {
	m_bTransparent = bTransparent;
}
AFX_INLINE BOOL CXTPControlGallery::IsTransparent() const {
	return m_bTransparent;
}



#endif // #if !defined(__XTPCONTROLGALLERY_H__)
