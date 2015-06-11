// XTPControlGalleryItem.h
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
#if !defined(__XTPCONTROLGALLERYITEM_H__)
#define __XTPCONTROLGALLERYITEM_H__
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPControlGalleryItem is a CCmdTarget derived class. It represents single item of
//     gallery object.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlGalleryItem : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPControlGalleryItem);

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

	static int GetDefaultHeight();

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

#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPControlGalleryItem);
	COleVariant m_oleTag;
	BOOL OleGetLabel();
	DECLARE_PROPERTY(ItemWidth, int);
	DECLARE_PROPERTY(ItemHeight, int);

	//}}AFX_CODEJOCK_PRIVATE
#endif

	friend class CXTPControlGalleryItems;
	friend class CXTPControlGallery;
};

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

AFX_INLINE BOOL CXTPControlGalleryItem::IsLabel() const {
	return m_bLabel;
}
AFX_INLINE void CXTPControlGalleryItem::SetToolTip(LPCTSTR lpszToolTip) {
	m_strToolTip = lpszToolTip;
}

AFX_INLINE void CXTPControlGalleryItem::SetData(DWORD_PTR dwData) {
	m_dwData = dwData;
}
AFX_INLINE DWORD_PTR CXTPControlGalleryItem::GetData() const {
	return m_dwData;
}

AFX_INLINE void CXTPControlGalleryItem::SetImageWidth(int nImageWidth) {
	m_nImageWidth = nImageWidth;
}
AFX_INLINE BOOL CXTPControlGalleryItem::OnClick() {
	return FALSE;
}
AFX_INLINE CXTPMarkupUIElement* CXTPControlGalleryItem::GetMarkupUIElement() const {
	return m_pMarkupUIElement;
}

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPCONTROLGALLERYITEM_H__
//}}AFX_CODEJOCK_PRIVATE
