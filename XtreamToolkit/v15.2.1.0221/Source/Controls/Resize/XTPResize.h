// XTPResize.h: interface for the CXTPResize class.
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
#if !defined(__XTPRESIZE_H__)
#define __XTPRESIZE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// -------------------------------------------------------------------
// Summary:
//     Enumeration used to determine attributes for resizing windows.
// Remarks:
//     XTPResize type defines the constants used by the CXTPResize
//     class to determine how a window will appear when it is resized.
// See Also:
//     CXTPResize
//
// <KEYWORDS xtpResizeNoSizeIcon, xtpResizeNoHorizontal, xtpResizeNoVertical, xtpResizeNoMinsize, xtpResizeNoClipChildren, xtpResizeNoTransparentGroup>
// -------------------------------------------------------------------
enum XTPResize
{
	xtpResizeNoSizeIcon         = 0x01, // Do not add size icon.
	xtpResizeNoHorizontal       = 0x02, // No horizontal resizing.
	xtpResizeNoVertical         = 0x04, // No vertical resizing.
	xtpResizeNoMinsize          = 0x08, // Do not require a minimum size.
	xtpResizeNoClipChildren     = 0x10, // Do not set clip children style.
	xtpResizeNoTransparentGroup = 0x20  // Do not set transparent style for group boxes.
};

class CXTPResize;

//===========================================================================
// Summary:
//     CXTPResizeItem is a stand alone helper class. It is used by CXTPResize to
//     maintain information about each item to be sized or moved.
//===========================================================================
class _XTP_EXT_CLASS CXTPResizeItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResizeItem object
	// Parameters:
	//     pWnd        - Pointer to the window to be sized or moved.
	//     rrcSizing   - Reference to a CXTPResizeRect object.
	//     rcWindow    - Reference to a CRect object.
	//     bAutoDelete - TRUE if the window is to be deleted.
	//-----------------------------------------------------------------------
	CXTPResizeItem(CWnd* pWnd, const CXTPResizeRect& rrcSizing, CRect& rcWindow, BOOL bAutoDelete);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPResizeItem object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPResizeItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Called by CXTPResize to make group box controls window style
	//     transparent.
	// Parameters:
	//     pXTPResize - Points the resize manager.
	// Returns:
	//     True if the WS_EX_TRANSPARENT style was set for the window,
	//     otherwise false.
	//-----------------------------------------------------------------------
	virtual bool MakeTransparent(CXTPResize* pXTPResize);

public:
	BOOL m_bIsGroupBox;             // TRUE if the window is a group box.
	BOOL m_bAutoDelete;             // TRUE if the window is to be deleted.
	BOOL m_bInitialSize;            // Initial size/move has been completed.
	CWnd* m_pWnd;                   // A pointer to the window to be sized or moved.
	CXTPResizeRect m_rrcSizing;      // Sizing option.
	CXTPResizeRect m_rrcWindow;      // Last control size.
	CXTPResizeRect m_rrcInitWindow;  // Initial control size.
};

//===========================================================================
// Summary:
//     The CXTPSizeIcon class is a CStatic derived helper class. It is used
//     by CXTPResize to display the sizing grip in the lower right corner of
//     a sizing window.
//===========================================================================
class _XTP_EXT_CLASS CXTPSizeIcon : public CScrollBar
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSizeIcon object
	//-----------------------------------------------------------------------
	CXTPSizeIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSizeIcon object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPSizeIcon();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPSizeIcon)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	HCURSOR m_hCursor; // Handle to the cursor displayed for the size icon

};

//===========================================================================
// Summary:
//     CXTPResize is a base class. It is used by resizing dialogs, property
//     sheets, and form views. It acts as a manager to maintain size and location
//     of the dialog and dialog items.
//===========================================================================
class _XTP_EXT_CLASS CXTPResize
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResize object
	// Parameters:
	//     pWnd   - Points to the parent or owner window object, of type CWnd, to which the
	//              resizing object belongs.
	//     nFlags - Flags that are to be passed to CXTPResize that specify the attributes
	//              of the resizing property page. See Remarks section for a list of flags.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//          * <b>xtpResizeNoSizeIcon</b> Do not add size icon.
	//          * <b>xtpResizeNoHorizontal</b> No horizontal resizing.
	//          * <b>xtpResizeNoVertical</b> No vertical resizing.
	//          * <b>xtpResizeNoMinsize</b> Do not require a minimum size.
	//          * <b>xtpResizeNoClipChildren</b> Do not set clip children style.
	//          * <b>xtpResizeNoTransparentGroup</b> Do not set transparent style for group boxes.
	//-----------------------------------------------------------------------
	CXTPResize(CWnd* pWnd, const UINT nFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPResize object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPResize();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     The SetResize function specifies how much each side of a control will
	//     move when the dialog is resized. If a control should be repositioned
	//     (e.g. an OK button) then all four sides should move by the same amount
	//     of pixels, as the dialog is resized. If a control should be resized
	//     just as much as the dialog (e.g. the list control in the file dialog),
	//     then the left and top sides shouldn't move, and the right and bottom
	//     sides should move by the same amount of pixels as the dialog.
	// Parameters:
	//     nID           - Specifies the control's ID.
	//     left          - How much the left side will move when the dialog is resized.
	//     top           - How much the top side will move when the dialog is resized.
	//     right         - How much the right side will move when the dialog is resized.
	//     bottom        - How much the bottom side will move when the dialog is resized.
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, XTP_RESIZE left, XTP_RESIZE top, XTP_RESIZE right, XTP_RESIZE bottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     The SetResize function specifies how much each side of a control will
	//     move when the dialog is resized. If a control should be repositioned
	//     (e.g. an OK button) then all four sides should move by the same amount
	//     of pixels, as the dialog is resized. If a control should be resized
	//     just as much as the dialog (e.g. the list control in the file dialog),
	//     then the left and top sides shouldn't move, and the right and bottom
	//     sides should move by the same amount of pixels as the dialog.
	// Parameters:
	//     nID           - Specifies the control's ID.
	//     rrcSizing     - How much the left, top, right, and bottom sides will move when
	//                     the dialog is resized.
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, const XTP_RESIZERECT& rrcSizing);

	//-----------------------------------------------------------------------
	// Summary:
	//     The SetResize function specifies how much each side of a control will
	//     move when the dialog is resized. If a control should be repositioned
	//     (e.g. an OK button) then all four sides should move by the same amount
	//     of pixels, as the dialog is resized. If a control should be resized
	//     just as much as the dialog (e.g. the list control in the file dialog),
	//     then the left and top sides shouldn't move, and the right and bottom
	//     sides should move by the same amount of pixels as the dialog.
	// Parameters:
	//     nID           - Specifies the control's ID.
	//     hWnd          - HWND of the dialog item to be sized.
	//     rrcSizing     - How much the left, top, right, and bottom sides will move when
	//                     the dialog is resized.
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, const HWND hWnd, const XTP_RESIZERECT& rrcSizing);

	//-----------------------------------------------------------------------
	// Summary:
	//     The SetResize function specifies how much each side of a control will
	//     move when the dialog is resized. If a control should be repositioned
	//     (e.g. an OK button) then all four sides should move by the same amount
	//     of pixels, as the dialog is resized. If a control should be resized
	//     just as much as the dialog (e.g. the list control in the file dialog),
	//     then the left and top sides shouldn't move, and the right and bottom
	//     sides should move by the same amount of pixels as the dialog.
	// Parameters:
	//     nID           - Specifies the control's ID.
	//     hWnd          - HWND of the dialog item to be sized.
	//     rpTopLeft     - How much the top and left sides will move when the dialog is resized.
	//     rpBottomRight - How much the bottom and right sides will move when the dialog is resized.
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, const HWND hWnd, const XTP_RESIZEPOINT& rpTopLeft, const XTP_RESIZEPOINT& rpBottomRight);

	//-----------------------------------------------------------------------
	// Summary:
	//     The SetResize function specifies how much each side of a control will
	//     move when the dialog is resized. If a control should be repositioned
	//     (e.g. an OK button) then all four sides should move by the same amount
	//     of pixels, as the dialog is resized. If a control should be resized
	//     just as much as the dialog (e.g. the list control in the file dialog),
	//     then the left and top sides shouldn't move, and the right and bottom
	//     sides should move by the same amount of pixels as the dialog.
	// Parameters:
	//     nID           - Specifies the control's ID.
	//     rpTopLeft     - How much the top and left sides will move when the dialog is resized.
	//     rpBottomRight - How much the bottom and right sides will move when the dialog is resized.
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, const XTP_RESIZEPOINT& rpTopLeft, const XTP_RESIZEPOINT& rpBottomRight);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the minimum size explicitly. Initial size
	//     is the default.
	// Parameters:
	//     sz - Specifies the minimum width and height the dialog can be sized to.
	//-----------------------------------------------------------------------
	void SetMinSize(CSize& sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the maximum size. No maximum is the default.
	// Parameters:
	//     sz - Specifies the maximum width and height the dialog can be sized to.
	//-----------------------------------------------------------------------
	void SetMaxSize(CSize& sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function saves the window placement to the registry.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	// Returns:
	//     TRUE if succeeds
	//-----------------------------------------------------------------------
	BOOL SavePlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads saved window placement information from
	//     the registry.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	// Returns:
	//     TRUE if position was changed
	//-----------------------------------------------------------------------
	BOOL LoadPlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads saved window placement information from
	//     the registry. This version is the same as LoadPlacement but there
	//     is no need for calling SavePlacement when the window is destroyed.
	//     This will be called automatically.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	// Returns:
	//     TRUE if position was changed
	//-----------------------------------------------------------------------
	BOOL AutoLoadPlacement(LPCTSTR pszSection);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     The SetResize function specifies how much each side of a control will
	//     move when the dialog is resized. If a control should be repositioned
	//     (e.g. an OK button) then all four sides should move by the same amount
	//     of pixels, as the dialog is resized. If a control should be resized
	//     just as much as the dialog (e.g. the list control in the file dialog),
	//     then the left and top sides shouldn't move, and the right and bottom
	//     sides should move by the same amount of pixels as the dialog.
	// Parameters:
	//     nID           - Specifies the control's ID.
	//     pWnd          - Points to the dialog item to be resized.
	//     hWnd          - HWND of the dialog item to be sized.
	//     rrcSizing     - How much the left, top, right, and bottom sides will move when
	//                     the dialog is resized.
	//     rcWindow      - Initial size of the dialog item.
	//     left          - How much the left side will move when the dialog is resized.
	//     top           - How much the top side will move when the dialog is resized.
	//     right         - How much the right side will move when the dialog is resized.
	//     bottom        - How much the bottom side will move when the dialog is resized.
	//     rpTopLeft     - How much the top and left sides will move when the dialog is resized.
	//     rpBottomRight - How much the bottom and right sides will move when the dialog is resized.
	//-----------------------------------------------------------------------
	void SetResize(CWnd* pWnd, const CXTPResizeRect& rrcSizing, CRect rcWindow);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to remove the specified dialog item
	//     from the list of items to be resized.
	// Parameters:
	//     nID - Specifies the control's ID.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveResize(const UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to purge the list that contains dialog
	//     items to be sized.
	//-----------------------------------------------------------------------
	void RemoveAllControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called from OnInitDialog or OnInitialUpdate
	//     to initialize the resize manager.
	//-----------------------------------------------------------------------
	virtual void Init();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called from OnSize to move and resize the dialog
	//     items that are managed.
	//-----------------------------------------------------------------------
	virtual void Size();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when a property sheet in wizard mode
	//     has changed pages to alert the resize manager that the property sheet
	//     (common control) has moved the page back to its original size/position
	//     on the sheet.
	//-----------------------------------------------------------------------
	void Reset();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called to offset dialog items by the amount specified by ptOffset.
	// Parameters:
	//     ptOffset - CPoint determines the amount in pixesl to offset
	//                dialog items.
	//-----------------------------------------------------------------------
	void Offset(CPoint ptOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to reset m_rrcWindow for item.
	// Parameters:
	//     pWnd - Window to reset.
	//-----------------------------------------------------------------------
	void UpdateControlRect(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called from OnGetMinMaxInfo to get the
	//     maximized position or dimensions or the minimum or maximum tracking
	//     size. The maximized size is the size of the window when its borders
	//     are fully extended. The maximum tracking size of the window is the
	//     largest window size that can be achieved by using the borders to size
	//     the window. The minimum tracking size of the window is the smallest
	//     window size that can be achieved by using the borders to size the window.
	// Parameters:
	//     pMMI - Points to a MINMAXINFO structure that contains information about a
	//            window's maximized size and position and its minimum and maximum
	//            tracking size. For more information about this structure, see the
	//            MINMAXINFO structure.
	//-----------------------------------------------------------------------
	void GetMinMaxInfo(MINMAXINFO* pMMI);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called in Size method to adjust window rectangle to fit real client size
	// Parameters:
	//     rcWindow - Window rectangle to change
	//-----------------------------------------------------------------------
	virtual void AdjustResizeRect(CSize& rcWindow);

protected: // flags

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the specified flag
	//     has been set for the resize manager.
	// Parameters:
	//     eFlag - Flag to check, it can be one of the values in the Remarks section.
	// Remarks:
	//     Styles to be added can one of the following:
	//          * <b>xtpResizeNoSizeIcon</b> Do not add size icon.
	//          * <b>xtpResizeNoHorizontal</b> No horizontal resizing.
	//          * <b>xtpResizeNoVertical</b> No vertical resizing.
	//          * <b>xtpResizeNoMinsize</b> Do not require a minimum size.
	//          * <b>xtpResizeNoClipChildren</b> Do not set clip children style.
	//          * <b>xtpResizeNoTransparentGroup</b> Do not set transparent style
	//          for group boxes.
	// Returns:
	//     TRUE if the specified flag has been set, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL HasFlag(XTPResize eFlag);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set a specific flag for the resize
	//     manager.
	// Parameters:
	//     eFlag - Flag to check, it can be one of the values in the Remarks section.
	// Remarks:
	//     Styles to be added can one of the following:<p/>
	//     * <b>xtpResizeNoSizeIcon</b> Do not add size icon.
	//     * <b>xtpResizeNoHorizontal</b> No horizontal resizing.
	//     * <b>xtpResizeNoVertical</b> No vertical resizing.
	//     * <b>xtpResizeNoMinsize</b> Do not require a minimum size.
	//     * <b>xtpResizeNoClipChildren</b> Do not set clip children style.
	//     * <b>xtpResizeNoTransparentGroup</b> Do not set transparent style
	//       for group boxes.
	//-----------------------------------------------------------------------
	void SetFlag(XTPResize eFlag);

protected: // helper methods

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by CXTPResize to resize a child window.
	// Parameters:
	//     hdwp     - [in] Handle to a multiple-window - position structure that contains size and position information for one or more windows. This structure is returned by BeginDeferWindowPos or by the most recent call to DeferWindowPos.
	//     pItem    - [in] Pointer to a CXTPResizeItem object that contains additional information about the window being sized.
	//     dx       - [in] Specifies the window's new width, in pixels.
	//     dy       - [in] Specifies the window's new height, in pixels.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Defer(HDWP& hdwp, CXTPResizeItem* pItem, int dx, int dy);

protected:

	typedef CArray<CXTPResizeItem*, CXTPResizeItem*&> CResizeItemArray;   // Used by CXTPResize to maintain a list of children that are sizable.

	UINT            m_nFlagsXX;     // flags passed from constructor
	CWnd*           m_pWnd;         // the associative relation to the window to be resized
	CSize           m_szWindow;     // last dialog size
	CSize           m_szInitWindow; // Initial dialog size
	CSize           m_szMin;        // smallest size allowed
	CSize           m_szMax;        // largest size allowed
	CString         m_strSection;   // section in registry where window placement information is saved.
	CXTPSizeIcon     m_scSizeIcon;   // size icon window
	CResizeItemArray    m_arrItems;     // array of controls

	friend class CXTPResizeItem;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPResize::SetMinSize(CSize& sz) {
	m_szMin = sz;
}
AFX_INLINE void CXTPResize::SetMaxSize(CSize& sz) {
	m_szMax = sz;
}
AFX_INLINE BOOL CXTPResize::HasFlag(XTPResize eFlag) {
	return (m_nFlagsXX & eFlag) != 0;
}
AFX_INLINE void CXTPResize::SetResize(const UINT nID, const HWND hWnd, const XTP_RESIZEPOINT& rpTopLeft, const XTP_RESIZEPOINT& rpBottomRight) {
	SetResize(nID, hWnd, CXTPResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y));
}
AFX_INLINE void CXTPResize::SetResize(const UINT nID, const XTP_RESIZERECT& rrcSizing) {
	SetResize(nID, NULL, rrcSizing);
}
AFX_INLINE void CXTPResize::SetResize(const UINT nID, const XTP_RESIZEPOINT& rpTopLeft, const XTP_RESIZEPOINT& rpBottomRight) {
	SetResize(nID, CXTPResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y));
}
AFX_INLINE void CXTPResize::SetResize(const UINT nID, XTP_RESIZE left, XTP_RESIZE top, XTP_RESIZE right, XTP_RESIZE bottom) {
	SetResize(nID, CXTPResizeRect(left, top, right, bottom));
}

#endif // !defined(__XTPRESIZE_H__)
