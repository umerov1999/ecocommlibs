// XTPControlGallery.h
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

//===========================================================================
// Summary:
//     CXTPControlGalleryItem is a CCmdTarget derived class. It represents single item of
//     gallery object.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryItem : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGalleryItem object
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem();
	~CXTPControlGalleryItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get size of the item.
	// Returns:
	//     Size of the item
	// See Also: SetSize, CXTPControlGalleryItems::SetItemSize
	//-----------------------------------------------------------------------
	virtual CSize GetSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set size of the individual item. You can call
	//     CXTPControlGalleryItems::SetItemSize to set default size for items
	// Parameters:
	//     szItem - Size of the item to be set
	//-----------------------------------------------------------------------
	void SetSize(CSize szItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the item
	// Parameters:
	//     pDC      - Pointer to valid device context
	//     pGallery - Parent gallery of the item
	//     rcItem   - Bounding rectangle of the item
	//     bEnabled - TRUE to draw item enabled; FALSE - disabled
	//     bSelected - TRUE to draw item selected
	//     bPressed - TRUE to draw item pressed
	//     bChecked - TRUE to draw item checked
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get image for item
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the item is label
	//-----------------------------------------------------------------------
	BOOL IsLabel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set caption of the item
	// Parameters:
	//     lpszCaption - Caption of the item to be set
	// See Also: GetCaption, SetToolTip, GetToolTip
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set tooltip  of the item
	// Parameters:
	//     lpszToolTip - Tooltip of the item to be set
	// See Also: GetToolTip, SetCaption, GetCaption
	//-----------------------------------------------------------------------
	void SetToolTip(LPCTSTR lpszToolTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves tooltip of the item
	// See Also: SetToolTip, SetCaption, GetCaption
	//-----------------------------------------------------------------------
	CString GetToolTip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves caption of the item
	// See Also: SetCaption, SetToolTip, GetToolTip
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable the item.
	// Parameters:
	//     bEnabled - TRUE if the item is enabled.
	// See Also: GetEnabled
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the state of the item.
	// Returns:
	//     TRUE if the item is enabled; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves index of the item
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the identifier of the item.
	// Parameters:
	//     nId - Identifier to be set.
	//-----------------------------------------------------------------------
	void SetID(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the item's identifier.
	// Returns:
	//     The identifier of the item.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the item.
	// Parameters:
	//     dwData - Contains the new value to associate with the item.
	//-----------------------------------------------------------------------
	void SetData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the item.
	// Returns:
	//     The 32-bit value associated with the item.
	//-----------------------------------------------------------------------
	DWORD_PTR GetData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the description of the item.
	// Parameters:
	//     lpszDescription - Description to be set.
	// See Also: SetCaption, SetTooltip
	//-----------------------------------------------------------------------
	void SetDescription(LPCTSTR lpszDescription);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the item's description.
	// Returns:
	//     The description of the control.
	//-----------------------------------------------------------------------
	CString GetDescription() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Set the preferred image width.
	// Parameters:
	//     nImageWidth - preferred image width.
	//-----------------------------------------------------------------------
	void SetImageWidth(int nImageWidth);

	//-----------------------------------------------------------------------
	// Summary: This member functions are used to get stored CXTPMarkupUIElement object.
	// Returns: A pointer to CXTPMarkupUIElement object.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;


	virtual BOOL OnClick();


protected:
	int m_nId;              // Identifier
	int m_nIndex;           // Index of the item
	int m_nImage;           // Image index
	CString m_strCaption;   // Caption of the item
	CString m_strToolTip;   // Tooltip of the item
	CString m_strDescriptionText; // Description of the item
	CXTPControlGalleryItems* m_pItems;  // Parent items collection
	DWORD_PTR m_dwData;     // 32-bit data
	CSize m_szItem;         // Size of the item; CSize(0, 0) - to use default value
	BOOL m_bLabel;          // TRUE if item is label
	BOOL m_bEnabled;        // TRUE to enable, FALSE to disable.
	int m_nImageWidth;      // preferred image width (0) - to use default value

	CXTPMarkupUIElement* m_pMarkupUIElement;


	friend class CXTPControlGalleryItems;
	friend class CXTPControlGallery;
};

//===========================================================================
// Summary:
//     CXTPControlGalleryItems is a CCmdTarget derived class. It represents a collection
//     of the items for gallery control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryItems : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPControlGalleryItems)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlGalleryItems object.
	// Parameters:
	//     pAction - Action for gallery control to link with.
	// Remarks:
	//     Protected constructor, use CreateItems to create items for gallery
	//-----------------------------------------------------------------------
	CXTPControlGalleryItems(CXTPControlAction* pAction);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlGalleryItems object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPControlGalleryItems();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates new items collection for gallery control
	// Parameters:
	//     pCommandBars - Parent Commandbars.
	//     nId - Identifier of the items
	//-----------------------------------------------------------------------
	static CXTPControlGalleryItems* AFX_CDECL CreateItems(CXTPCommandBars* pCommandBars, int nId);


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all items in collection
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove item.
	// Parameters:
	//     nIndex - Index of the item to be removed.
	//-----------------------------------------------------------------------
	void Remove(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item.
	// Parameters:
	//     nId   - Identifier of the item
	//     nImage - Image of the item
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddItem(int nId, int nImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item.
	// Parameters:
	//     nId   - Identifier of the item
	//     nImage - Image of the item
	//     lpszCaption - Caption of the item to be added
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddItem(LPCTSTR lpszCaption, int nId, int nImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item.
	// Parameters:
	//     pItem - Item to be added
	//     nId   - Identifier of the item
	//     nImage - Image of the item
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddItem(CXTPControlGalleryItem* pItem, int nId = -1, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item.
	// Parameters:
	//     nImage - Image of the item
	//     lpszCaption - Caption of the item to be added
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddItem(LPCTSTR lpszCaption, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert a new item.
	// Parameters:
	//     nIndex - Index to insert new item
	//     pItem - Item to be added
	//     nId   - Identifier of the item
	//     nImage - Image of the item
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* InsertItem(int nIndex, CXTPControlGalleryItem* pItem, int nId = -1, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new label item.
	// Parameters:
	//     nId   - Identifier of the item
	//     lpszCaption - Caption of the item to be added
	// Returns:
	//     A pointer to the added item.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* AddLabel(int nId);
	CXTPControlGalleryItem* AddLabel(LPCTSTR lpszCaption); //<combine CXTPControlGalleryItems::AddLabel@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set image manager for items
	// Parameters:
	//     pImageManager - new image manager to be set
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves image manager of the items
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines default size of the items
	// See Also: SetItemSize
	//-----------------------------------------------------------------------
	CSize GetItemSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set size of the item
	// Parameters:
	//     szItem - New size to be set
	// See Also: GetItemSize
	//-----------------------------------------------------------------------
	void SetItemSize(CSize szItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the count of the items
	// Returns:
	//     The count of the items in collection.
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the item at the specified index.
	// Parameters:
	//     nIndex - An integer index.
	// Returns:
	//     The CXTPControlGalleryItem pointer currently at this index.
	//-----------------------------------------------------------------------
	CXTPControlGalleryItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable/disable clipping items if selection was changed
	// Parameters:
	//     bClipItems - TRUE to enable clipping; FALSE to disable
	// Remarks:
	//     By default clipping is enabled
	//-----------------------------------------------------------------------
	void ClipItems(BOOL bClipItems);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called if items was added or removed in collection
	//-----------------------------------------------------------------------
	virtual void OnItemsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the item
	// Parameters:
	//     pDC      - Pointer to valid device context
	//     pGallery - Parent gallery of the item
	//     pItem    - Item to be drawn
	//     rcItem   - Bounding rectangle of the item
	//     bEnabled - TRUE to draw item enabled; FALSE - disabled
	//     bSelected - TRUE to draw item selected
	//     bPressed - TRUE to draw item pressed
	//     bChecked - TRUE to draw item checked
	//-----------------------------------------------------------------------
	virtual BOOL PreDrawItem(CDC* pDC, CXTPControlGallery* pGallery, CXTPControlGalleryItem* pItem, CRect rcItem,
		BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get actions list for command bars
	//-----------------------------------------------------------------------
	CXTPControlAction* GetAction() const;


	//-----------------------------------------------------------------------
	// Input:   bEnableMarkup - TRUE to enable markup, FALSE to disable markup.
	// Summary: Allows the use of a XAML snippet to be used for the Text of
	//          items
	//-----------------------------------------------------------------------
	void EnableMarkup(BOOL bEnableMarkup = TRUE);

	//-----------------------------------------------------------------------
	// Summary: Returns markup context.
	// Returns: Returns markup context.
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;


private:
	void UpdateIndexes(int nStart /*= 0*/);

protected:
	CSize m_szItem;                         // Size of the items
	CXTPImageManager* m_pImageManager;      // Image manager of items
	BOOL m_bClipItems;                      // TRUE to clip items
	CXTPControlAction* m_pAction;           // Action of the items
	CArray<CXTPControlGalleryItem*, CXTPControlGalleryItem*> m_arrItems;    // Array of items
	CXTPMarkupContext* m_pMarkupContext;

private:
	friend class CXTPControlGalleryItem;
	friend class CXTPControlGallery;
	friend class CXTPControlGalleryAction;


};

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
	void CalcScrollBarInfo(LPRECT lprc, SCROLLBARPOSINFO* pSBInfo, SCROLLINFO* pSI);

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


//===========================================================================
// Summary:
//     CXTPControlGalleryPaintManager is standalone class used to draw CXTPControlGallery object
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryPaintManager : public CXTPScrollBarPaintManager
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
	//     This method is called to draw scrollbar of the gallery
	// Parameters:
	//     pDC - Pointer to device context
	//     pScrollBar - ScrollBar to draw
	//-----------------------------------------------------------------------
	virtual void DrawScrollBar(CDC* pDC, CXTPScrollBase* pScrollBar);

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
};



AFX_INLINE BOOL CXTPControlGalleryItem::IsLabel() const {
	return m_bLabel;
}
AFX_INLINE void CXTPControlGalleryItem::SetToolTip(LPCTSTR lpszToolTip) {
	m_strToolTip = lpszToolTip;
}
AFX_INLINE CString CXTPControlGalleryItem::GetToolTip() const {
	return m_strToolTip;
}
AFX_INLINE CString CXTPControlGalleryItem::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE void CXTPControlGalleryItem::SetDescription(LPCTSTR lpszDescription) {
	m_strDescriptionText = lpszDescription;
}
AFX_INLINE CString CXTPControlGalleryItem::GetDescription() const {
	return m_strDescriptionText;
}
AFX_INLINE void CXTPControlGalleryItem::SetID(int nId) {
	m_nId = nId;
}
AFX_INLINE int CXTPControlGalleryItem::GetID() const {
	return m_nId;
}
AFX_INLINE CXTPImageManager* CXTPControlGalleryItems::GetImageManager() const {
	return m_pImageManager;
}
AFX_INLINE CSize CXTPControlGalleryItems::GetItemSize() const {
	return m_szItem;
}
AFX_INLINE void CXTPControlGalleryItems::SetItemSize(CSize szItem) {
	m_szItem = szItem;
}
AFX_INLINE void CXTPControlGalleryItems::ClipItems(BOOL bClipItems) {
	m_bClipItems = bClipItems;
}

AFX_INLINE void CXTPControlGalleryItem::SetData(DWORD_PTR dwData) {
	m_dwData = dwData;
}
AFX_INLINE DWORD_PTR CXTPControlGalleryItem::GetData() const {
	return m_dwData;
}

AFX_INLINE void CXTPControlGallery::SetControlSize(CSize szControl) {
	m_nWidth = szControl.cx;
	m_nHeight = szControl.cy;
}
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
AFX_INLINE CXTPControlAction* CXTPControlGalleryItems::GetAction() const {
	return m_pAction;
}
AFX_INLINE void CXTPControlGalleryItem::SetImageWidth(int nImageWidth) {
	m_nImageWidth = nImageWidth;
}
AFX_INLINE BOOL CXTPControlGalleryItem::OnClick() {
	return FALSE;
}

AFX_INLINE CXTPMarkupContext* CXTPControlGalleryItems::GetMarkupContext() const {
	return m_pMarkupContext;
}
AFX_INLINE CXTPMarkupUIElement* CXTPControlGalleryItem::GetMarkupUIElement() const {
	return m_pMarkupUIElement;
}



#endif // #if !defined(__XTPCONTROLGALLERY_H__)
