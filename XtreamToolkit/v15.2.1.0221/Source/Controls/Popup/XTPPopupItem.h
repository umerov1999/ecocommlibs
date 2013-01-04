// XTPPopupItem.h: interface for the CXTPPopupItem class.
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
#if !defined(_XTPPOPUPITEM_H__)
#define _XTPPOPUPITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPPopupControl;
class CXTPImageManagerIcon;
class CXTPRichRender;
class CXTPMarkupUIElement;

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the icon type of CXTPPopupItem.
// Example:
// <code>
// CXTPPopupItem* pItemIcon = m_wndPopupControl.AddItem(
//     new CXTPPopupItem(CRect(14, 40, 20, 49)));
// pItemIcon->SetIcons(IDB_LOGOOFFICE, 0, xtpPopupItemIconNormal);
// </code>
// See Also:
//     CXTPPopupItem::SetIcon, CXTPPopupItem::SetIcons
//
// <KEYWORDS xtpPopupItemIconNormal, xtpPopupItemIconSelected, xtpPopupItemIconPressed>
//-----------------------------------------------------------------------
enum XTPPopupItemIcon
{
	xtpPopupItemIconNormal = 1,     //normal icon handler
	xtpPopupItemIconSelected = 2,   //selected icon handler
	xtpPopupItemIconPressed = 4     //pressed icon handler
};

//===========================================================================
// Summary:
//     Cpecial Id for close item
//===========================================================================
#define XTP_ID_POPUP_CLOSE -1

//===========================================================================
// Summary:
//     CXTPPopupItem is a class used to management popup item object
//===========================================================================
class _XTP_EXT_CLASS  CXTPPopupItem : public CXTPCmdTarget
{
	DECLARE_DYNCREATE(CXTPPopupItem)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupItem object.
	// Parameters:
	//     rcItem        - Bounding rectangle of the item.
	//     lpszCaption   - NULL terminated string specifying the caption
	//                     text.
	//     clrBack       - Background color of the item.
	//     clrBorder     - Border color of the item
	//-----------------------------------------------------------------------
	CXTPPopupItem(LPRECT rcItem = NULL, LPCTSTR lpszCaption = NULL, COLORREF clrBack = (COLORREF)-1, COLORREF clrBorder = (COLORREF)-1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupItem object.
	//-----------------------------------------------------------------------
	~CXTPPopupItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the item.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get rectangular of an item.
	// Returns:
	//     CRect object containing rectangular of an item.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get button state of an item.
	// Returns:
	//     TRUE if button state is enabled, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set button state of an item.
	// Parameters:
	//     bButton - TRUE to enable button state, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetButton(BOOL bButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get selected state of an item.
	// Returns:
	//     TRUE if item selected, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get pressed state of an item.
	// Returns:
	//     TRUE if item pressed, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPressed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set ID number for an item.
	// Parameters:
	//     nID - value of ID number.
	//-----------------------------------------------------------------------
	void SetID(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get ID number of an item.
	//     window.
	// Returns:
	//     Value of ID number.
	//-----------------------------------------------------------------------
	int GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text of an item.
	// Parameters:
	//     str -  a CString object that represents the item text.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the text of an item.
	// Returns:
	//     A CString object that represents the item text.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	// ------------------------------------------------------------------
	// Summary:
	//     Call this member function to set RichText of an item.
	// Parameters:
	//     str :          a CString object that represents the item text.
	//     nIDResource :  Resource used.
	//
	// ------------------------------------------------------------------
	void SetRTFText(LPCTSTR str);
	void SetRTFText(UINT nIDResource); // <combine CXTPPopupItem::SetRTFText@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary: Call this method to set Markup text for the item
	// Parameters:  str - Markup text to set
	//-----------------------------------------------------------------------
	void SetMarkupText(LPCTSTR str);

	//-------------------------------------------------------------------------
	// Summary: Call this method to set item size using size of ites content.
	//-------------------------------------------------------------------------
	void FitToContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color of an item text.
	// Parameters:
	//     clrText    - RGB color value that represents the item's text color.
	//     clrHotText - RGB color value that represents the item's text color
	//                  when the mouse is hovering.
	//-----------------------------------------------------------------------
	void SetTextColor(COLORREF clrText, COLORREF clrHotText = (COLORREF)-1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the color of an item text.
	// Returns:
	//     A COLORREF object that represents the color of an item text.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the background color of an item.
	// Returns:
	//     The background color of a popup control item.
	//-----------------------------------------------------------------------
	COLORREF GetBackgroundColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the border color of an item.
	// Returns:
	//     The border color of a popup control item.
	//-----------------------------------------------------------------------
	COLORREF GetBorderColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the alignment of an item text.
	// Parameters:
	//     nAlign -  The alignment style of an item text.
	//-----------------------------------------------------------------------
	void SetTextAlignment(int nAlign);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the alignment of an item text.
	// Returns:
	//     The alignment style of an item text.
	//-----------------------------------------------------------------------
	int GetTextAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the fntText of an item text.
	// Parameters:
	//     pFntText -  point to CFont object.
	//     pLogfText -  point to LOGFONT structure.
	//-----------------------------------------------------------------------
	void SetTextFont(CFont* pFntText);
	void SetTextFont(PLOGFONT pLogfText); // <combine CXTPPopupItem::SetTextFont@CFont* >

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the fntText of an item text.
	// Returns:
	//     Pointer to CFont object.
	//-----------------------------------------------------------------------
	virtual CFont* GetTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get hyperlink support of a text
	//     item.
	// Returns:
	//     TRUE if hyperlink support, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHyperLink() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the sytem hand cursor
	//     should be displayed when the mouse moves over this item.
	// Returns:
	//     TRUE if the hand cursor should be displayed, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHandCursor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set hyperlink support of a text
	//     item.
	// Parameters:
	//     bHyperLink - TRUE to set hyperlink support, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetHyperLink(BOOL bHyperLink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable the system hand cursor display
	//     when the mouse moves over this item.
	// Parameters:
	//     bHandCursor - TRUE to use the system hand cursor on mouse over, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetHandCursor(BOOL bHandCursor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get bold state of a text item.
	// Returns:
	//     TRUE if item state is bold, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsBold() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the fntText display is
	//     underlined if the item is a hyperlink and the mouse hovering.
	// Returns:
	//     TRUE if item state is underlined on mouse hover, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsUnderline() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set bold state of a text item.
	// Parameters:
	//     bBold - TRUE to set bold state, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetBold(BOOL bBold);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set underline state of a text item.
	//     If bUnderline is TRUE the text will be drawn underlined when the
	//     mouse hovers if the item is a hyperlink.
	// Parameters:
	//     bUnderline - TRUE to set draw text underlined on mouse hover, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetUnderline(BOOL bUnderline);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update rectangular height of a text.
	//-----------------------------------------------------------------------
	void CalculateHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update rectangular width of a text.
	//-----------------------------------------------------------------------
	void CalculateWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the normal, selected ore pressed
	//     icon for icon item.
	// Parameters:
	//     hIcon - Handle to the icon.
	//     itemIcon - type of icon in use, see XTPPopupItemIcon.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetIcon(HICON hIcon, XTPPopupItemIcon itemIcon = xtpPopupItemIconNormal);
	BOOL SetIcon(UINT nIDIcon, XTPPopupItemIcon itemIcon = xtpPopupItemIconNormal); // <combine CXTPPopupItem::SetIcon@HICON@XTPPopupItemIcon>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the normal, selected ore pressed
	//     icon from resource bitmap picture.
	// Parameters:
	//     nIDBitmap - Resource ID for the bitmap.
	//     hBitmap        - bitmap handle to set.
	//     clrTransparent - transparent color.
	//     itemIcon - type of icon in use, see XTPPopupItemIcon.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetIcons(UINT nIDBitmap, COLORREF clrTransparent, int itemIcon);
	BOOL SetIcons(HBITMAP hBitmap, COLORREF clrTransparent, int itemIcon, BOOL bAlphaBitmap = FALSE); // <combine CXTPPopupItem::SetIcons@UINT@COLORREF@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a handle the normal, selected
	//     ore pressed icon from icon item.
	// Returns:
	//     CXTPImageManagerIcon pointer to icon.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns parent popup control object.
	// Returns:
	//     Pointer to parent CXTPPopupControl.
	//-----------------------------------------------------------------------
	CXTPPopupControl* GetPopupControl() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Calculates offset to move icon in center.
	//-------------------------------------------------------------------------
	void CenterIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieve icon offset.
	// Returns:
	//     Poisition of icon inside item bounding rectangle.
	//-----------------------------------------------------------------------
	CPoint GetIconOffset() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves icon index.
	// Returns:
	//     Icon index.
	// See Also: SetIconIndex, CXTPPopupControl::GetImageManager
	//-----------------------------------------------------------------------
	int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set icon index of item.
	// Parameters:
	//     nIconIndex - Icon index to be set.
	// See Also: GetIconIndex, CXTPPopupControl::GetImageManager
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary: Call this method to get Markup element that renders the item.
	// Returns: Pointer to Markup element.
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw parent control.
	//-------------------------------------------------------------------------
	void RedrawControl();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when item is inserted to control.
	//-------------------------------------------------------------------------
	virtual void OnItemInserted();

protected:
	int                  m_nID;         // ID of the popup item.
	int                  m_nIndex;      // Zero based index of the item in the parent popup control's item list.
	int                  m_nIconIndex;  // Zero based index of the item's icon in the parent control's image list.
	BOOL                 m_bButton;     // TRUE if the popup item is a button, FALSE otherwise.
	BOOL                 m_bHyperLink;  // TRUE if the popup item is a hyperlink, FALSE otherwise.
	BOOL                 m_bBold;       // TRUE if the popup item's text is bold, FALSE otherwise.
	BOOL                 m_bUnderline;  // TRUE if the popup item's text is underlined on mouse over, FALSE otherwise.
	UINT                 m_nAlign;     // Specifies the method of formatting the text. It can be any combination of the values described for the nAlign parameter in CDC::DrawText.
	CFont                m_fntText;     // Popup item's fntText.
	CRect                m_rcItem;      // Size and location of the popup item.
	CPoint               m_ptOffset;    // XY coordinates representing the offset for the popup item's icon.
	CString              m_strCaption;  // Popup item's display text.
	COLORREF             m_clrText;     // RGB color value representing the item's normal text color.
	COLORREF             m_clrHotText;  // RGB color value representing the item's mouse over text color.
	COLORREF             m_clrBack;     // RGB color value representing the item's background color.
	COLORREF             m_clrBorder;   // RGB color value representing the item's border color.
	CXTPPopupControl*    m_pControl;    // Point to CXTPPopupControl object containing this item
	CXTPRichRender*      m_pRichRender; // Richtext Render
	CXTPMarkupUIElement* m_pUIElement;  // Markup Element that renders the item.

protected:

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPopupItem)

	void OleSetIcons(int hBitmap, COLORREF clrTransparent, int itemIcon);
	void OleSetIcon(int hIcon, int itemIcon);
	LPFONTDISP OleGetFont();
	void OleSetFont(LPFONTDISP pFontDisp);

	afx_msg long OleGetLeft();
	afx_msg void OleSetLeft(long nLeft);
	afx_msg long OleGetTop();
	afx_msg void OleSetTop(long nTop);
	afx_msg long OleGetWidth();
	afx_msg void OleSetWidth(long nWidth);
	afx_msg long OleGetHeight();
	afx_msg void OleSetHeight(long nHeight);
	afx_msg BSTR OleGetCaption();

	enum {
		dispidCaption = 1L,
	};
//}}AFX_CODEJOCK_PRIVATE
#endif
	friend class CXTPPopupControl;
	friend class CXTPPopupPaintManager;
};



AFX_INLINE CRect CXTPPopupItem::GetRect() const {
	return m_rcItem;
}
AFX_INLINE BOOL CXTPPopupItem::IsButton() const {
	return m_bButton;
}
AFX_INLINE void CXTPPopupItem::SetButton(BOOL bButton) {
	m_bButton = bButton;
}
AFX_INLINE void CXTPPopupItem::SetID(int nID) {
	m_nID = nID;
}
AFX_INLINE int CXTPPopupItem::GetID() const {
	return m_nID;
}
AFX_INLINE COLORREF CXTPPopupItem::GetTextColor() const {
	return IsSelected() ? ((m_clrHotText != (COLORREF)-1) ? m_clrHotText : m_clrText) : m_clrText;
}
AFX_INLINE int CXTPPopupItem::GetTextAlignment() const {
	return m_nAlign;
}
AFX_INLINE void CXTPPopupItem::SetTextAlignment(int nAlign) {
	m_nAlign = nAlign;
	RedrawControl();
}
AFX_INLINE CString CXTPPopupItem::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE void CXTPPopupItem::SetTextColor(COLORREF clrText, COLORREF clrHotText) {
	m_clrText = clrText; m_clrHotText = clrHotText;
	RedrawControl();
}
AFX_INLINE BOOL CXTPPopupItem::IsHyperLink() const {
	return m_bHyperLink;
}
AFX_INLINE BOOL CXTPPopupItem::IsBold() const {
	return m_bBold;
}
AFX_INLINE BOOL CXTPPopupItem::IsUnderline() const {
	return m_bUnderline;
}
AFX_INLINE void CXTPPopupItem::SetBold(BOOL bBold) {
	m_bBold = bBold;
	RedrawControl();
}
AFX_INLINE void CXTPPopupItem::SetUnderline(BOOL bUnderline) {
	m_bUnderline = bUnderline;
	RedrawControl();
}
AFX_INLINE void CXTPPopupItem::SetHyperLink(BOOL bHyperLink) {
	m_bHyperLink = bHyperLink;
}
AFX_INLINE COLORREF CXTPPopupItem::GetBackgroundColor() const {
	return m_clrBack;
}
AFX_INLINE COLORREF CXTPPopupItem::GetBorderColor() const {
	return m_clrBorder;
}
AFX_INLINE CPoint CXTPPopupItem::GetIconOffset() const {
	return m_ptOffset;
}
AFX_INLINE void CXTPPopupItem::SetIconIndex(int nIconIndex) {
	m_nIconIndex = nIconIndex;
	RedrawControl();
}
AFX_INLINE int CXTPPopupItem::GetIconIndex() const {
	return m_nIconIndex == -1 ? m_nIndex : m_nIconIndex;
}
AFX_INLINE CXTPPopupControl* CXTPPopupItem::GetPopupControl() const {
	return m_pControl;
}
AFX_INLINE CXTPMarkupUIElement* CXTPPopupItem::GetMarkupUIElement() const {
	return m_pUIElement;
}

#endif // !defined(_XTPPOPUPITEM_H__)
