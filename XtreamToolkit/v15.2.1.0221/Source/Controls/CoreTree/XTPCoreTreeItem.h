// XTPCoreTreeItem.h: interface for the CXTPCoreTreeItem class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPCORETREEITEM_H__)
#define __XTPCORETREEITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCoreTreeItems;
class CXTPCoreTreeControl;
class CXTPImageManagerIcon;
class CXTPPropExchange;
class CXTPMarkupUIElement;

class _XTP_EXT_CLASS CXTPCoreTreeItem : public CObject
{
	DECLARE_SERIAL(CXTPCoreTreeItem);
public:
	CXTPCoreTreeItem();

protected:
	virtual ~CXTPCoreTreeItem();

public:
	void SetCaption(LPCTSTR lpszCaption);
	void SetLabel(BOOL bLabel = TRUE);

	CString GetCaption() const;
	BOOL IsLabel() const;

	CRect GetRect() const;
	CRect GetScreenRect() const;

	void SetExpanded(BOOL bExpanded = TRUE);
	BOOL IsExpanded() const;

	BOOL IsSelected() const;

	void ShowCheckbox(BOOL bHasCheckbox = TRUE);
	BOOL IsCheckboxVisible() const;

	void SetChecked(int nChecked);
	int IsChecked() const;

	void SetIconId(int nIconId);
	int GetIconId() const;

	void SetEnabled(BOOL bEnabled);
	BOOL IsEnabled() const;

	CXTPImageManagerIcon* GetImage() const;


	CRect GetButtonRect() const;
	CRect GetCheckboxRect() const;
	CRect GetCaptionRect() const;

	void Remove();

	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the item
	// Returns:
	//     The 32-bit value associated with the item
	//-----------------------------------------------------------------------
	DWORD_PTR GetItemData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the item.
	// Parameters:
	//     dwData - Contains the new value to associate with the item.
	//-----------------------------------------------------------------------
	void SetItemData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary: Returns Markup element that renders Caption of item
	// Returns: Pointer to CXTPMarkupUIElement element
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

public:
	DWORD AddRef();
	DWORD Release();

public:
	void Invalidate();

public:
	BOOL HasChildren() const;
	CXTPCoreTreeItems* GetChildren() const;
	CXTPCoreTreeItems* GetParentItems() const;

public:
	CXTPCoreTreeItem* GetNextSiblingItem() const;
	CXTPCoreTreeItem* GetPrevSiblingItem() const;

	CXTPCoreTreeItem* GetParentItem() const;
	CXTPCoreTreeItem* GetFirstChildItem() const;

	BOOL IsParent(CXTPCoreTreeItem* pItem) const;
	CXTPCoreTreeControl* GetTreeControl() const;

public:
	virtual CSize CalcSize(CDC* pDC, int nWidth) const;
	virtual void Draw(CDC* pDC);

public:
	void SetRect(CRect rc);

public:
	static CXTPCoreTreeItem* AFX_CDECL CreateFromOleData(COleDataObject* pDataObject);
	static CXTPCoreTreeItem* AFX_CDECL CreateFromOleFile(CFile* pFile);
	HGLOBAL CacheGlobalData();
	BOOL PrepareDrag (COleDataSource& srcItem);

protected:
	CString m_strCaption;
	BOOL m_bLabel;
	mutable CXTPCoreTreeItems* m_pChildren;
	CXTPCoreTreeItem* m_pNextItem;
	CXTPCoreTreeItem* m_pPrevItem;
	CXTPCoreTreeItems* m_pParentItems;
	CRect m_rcItem;
	BOOL m_bExpanded;
	CXTPCoreTreeControl* m_pControl;
	BOOL m_bHasCheckBox;
	int m_nChecked;
	int m_nIconId;
	BOOL m_bEnabled;
	DWORD_PTR m_dwData;
	long m_dwRef;
	CXTPMarkupUIElement* m_pMarkupUIElement;    // Markup element that renders caption


	friend class CXTPCoreTreeItems;
};



class _XTP_EXT_CLASS CXTPCoreTreeItems
{
public:
	CXTPCoreTreeItems();

protected:
	virtual ~CXTPCoreTreeItems();

public:
	CXTPCoreTreeItem* AddItem(LPCTSTR lpszCaption);

	CXTPCoreTreeItem* AddItemAfter(CXTPCoreTreeItem* pItem, CXTPCoreTreeItem* pPrevItem = NULL);
	CXTPCoreTreeItem* AddItemBefore(CXTPCoreTreeItem* pItem, CXTPCoreTreeItem* pNextItem = NULL);

	void MoveItemAfter(CXTPCoreTreeItem* pItem, CXTPCoreTreeItem* pPrevItem = NULL);
	void MoveItemBefore(CXTPCoreTreeItem* pItem, CXTPCoreTreeItem* pPrevItem = NULL);

public:
	int GetCount() const;

public:
	void RemoveAll();
	void RemoveItem(CXTPCoreTreeItem* pItem);

public:
	POSITION GetHeadPosition() const;
	CXTPCoreTreeItem* GetNext(POSITION& rPosition) const;

public:
	CXTPCoreTreeItem* GetFirstItem() const;
	CXTPCoreTreeItem* GetLastItem() const;

	CXTPCoreTreeItem* GetNextItem(const CXTPCoreTreeItem* pItem) const;
	CXTPCoreTreeItem* GetPrevItem(const CXTPCoreTreeItem* pItem) const;


protected:
	CXTPCoreTreeItem* m_pFirstItem;
	CXTPCoreTreeItem* m_pLastItem;
	CXTPCoreTreeItem* m_pOwnerItem;
	CXTPCoreTreeControl* m_pControl;
	int m_nCount;

	friend class CXTPCoreTreeItem;
	friend class CXTPCoreTreeControl;
};



AFX_INLINE CString CXTPCoreTreeItem::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE BOOL CXTPCoreTreeItem::IsLabel() const {
	return m_bLabel;
}
AFX_INLINE void CXTPCoreTreeItem::SetLabel(BOOL bLabel) {
	m_bLabel = bLabel;
	Invalidate();
}
AFX_INLINE CXTPCoreTreeItems* CXTPCoreTreeItem::GetParentItems() const {
	return m_pParentItems;
}
AFX_INLINE void CXTPCoreTreeItem::ShowCheckbox(BOOL bHasCheckbox) {
	m_bHasCheckBox = bHasCheckbox;
}
AFX_INLINE BOOL CXTPCoreTreeItem::IsCheckboxVisible() const {
	return m_bHasCheckBox;
}
AFX_INLINE void CXTPCoreTreeItem::SetChecked(int nChecked) {
	m_nChecked = nChecked;
	Invalidate();
}
AFX_INLINE int CXTPCoreTreeItem::IsChecked() const {
	return m_nChecked;
}
AFX_INLINE void CXTPCoreTreeItem::SetIconId(int nIconId) {
	m_nIconId = nIconId;
	Invalidate();
}
AFX_INLINE int CXTPCoreTreeItem::GetIconId() const {
	return m_nIconId;
}
AFX_INLINE BOOL CXTPCoreTreeItem::IsEnabled() const {
	return m_bEnabled;
}
AFX_INLINE DWORD_PTR CXTPCoreTreeItem::GetItemData() const {
	return m_dwData;
}
AFX_INLINE void CXTPCoreTreeItem::SetItemData(DWORD_PTR dwData) {
	m_dwData = dwData;
}
AFX_INLINE CXTPMarkupUIElement* CXTPCoreTreeItem::GetMarkupUIElement() const {
	return m_pMarkupUIElement;
}
AFX_INLINE CXTPCoreTreeControl* CXTPCoreTreeItem::GetTreeControl() const {
	return m_pControl;
}
AFX_INLINE int CXTPCoreTreeItems::GetCount() const {
	return m_nCount;
}
AFX_INLINE CXTPCoreTreeItem* CXTPCoreTreeItems::GetFirstItem() const {
	return m_pFirstItem;
}
AFX_INLINE CXTPCoreTreeItem* CXTPCoreTreeItems::GetLastItem() const {
	return m_pLastItem;
}


#endif // !defined(__XTPCORETREEITEM_H__)
