// XTPReportRecordItemControls.h
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPREPORTRECORDITEMCONTROLS_H__)
#define __XTPREPORTRECORDITEMCONTROLS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct XTP_NM_REPORTRECORDITEM;
struct XTP_REPORTRECORDITEM_CLICKARGS;
class CXTPReportRecordItemControl;


//-----------------------------------------------------------------------
// Summary:
//      This enum defines types for the item control.
//-----------------------------------------------------------------------
enum XTPReportItemControlType
{
	xtpItemControlTypeUndefined = 0, // Undefined type.

	xtpItemControlTypeButton    = 1, // Button.
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines alignment styles for the item control.
//-----------------------------------------------------------------------
enum XTPReportItemControlAlignment
{
	xtpItemControlUnknown     = 0,     // Unknown (empty) value.

	xtpItemControlLeft        = 0x001, // Aligns control to the left.
	xtpItemControlRight       = 0x002, // Aligns control to the right.
};

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from item control
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window
// Remarks:
//     Use this structure to get item control which produce the message.
// See Also: XTP_NM_REPORT_ITEMBUTTONCLICK
//-----------------------------------------------------------------------
struct XTP_NM_REPORTITEMCONTROL : public XTP_NM_REPORTRECORDITEM
{
	CXTPReportRecordItemControl* pItemControl; // Pointer to the item control.
};



//===========================================================================
// Summary:
//     This class represents a generic item control and handles control's
//     behaviour, common for all derived item controls.
//     You cannot use this class directly in the control.
//     If you wish to add your own item control, you must inherit
//     it from CXTPReportRecordItemControl. Some member functions in this class
//     are virtual.
// See Also:
//     CXTPReportRecordItemButton
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemControl : public CXTPCmdTarget
{
	DECLARE_SERIAL(CXTPReportRecordItemControl)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a record item control.
	// Parameters:
	//     szCaption - An item control caption.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemControl(LPCTSTR szCaption = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPReportRecordItemControl default destructor
	//-----------------------------------------------------------------------
	virtual ~CXTPReportRecordItemControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report item control
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item control caption.
	// Returns:
	//     Item control caption.
	// See Also:
	//     SetCaption
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control caption.
	// Parameters:
	//     szCaption - Item control caption.
	// See Also:
	//     GetCaption
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR szCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the type of the item control.
	// Returns:
	//     Item control type.
	// See Also:
	//     XTPReportItemControlType
	//-----------------------------------------------------------------------
	int GetType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the index of the item control.
	// Returns:
	//     Item control index.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the item control caption font.
	// Returns:
	//     Pointer to the current caption font.
	// See Also:
	//     SetFont
	//-----------------------------------------------------------------------
	CFont* GetFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control caption font.
	// Parameters:
	//     pFont - New font for caption.
	// See Also:
	//     GetFont
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the item control caption color.
	// Returns:
	//     Current caption color.
	// See Also:
	//     SetCaptionColor
	//-----------------------------------------------------------------------
	COLORREF GetCaptionColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control caption color.
	// Parameters:
	//     clrCaption - New color.
	// See Also:
	//     GetCaptionColor
	//-----------------------------------------------------------------------
	void SetCaptionColor(COLORREF clrCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item control size.
	// Returns:
	//     Item control size.
	// See Also:
	//     SetSize, GetWidth, SetWidth, GetHeight, SetHeight
	//-----------------------------------------------------------------------
	CSize GetSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control size.
	//     If the size is set to (<=0, <=0), the control occupies the rest of the cell.
	// Parameters:
	//     sizeControl - item control size.
	// See Also:
	//     GetSize, GetWidth, SetWidth, GetHeight, SetHeight
	//-----------------------------------------------------------------------
	void SetSize(CSize sizeControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item control width.
	// Returns:
	//     Item control width.
	// See Also:
	//     GetSize, SetSize, SetWidth, GetHeight, SetHeight
	//-----------------------------------------------------------------------
	int GetWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control width.
	//     If the width is less than 0, the control occupies the rest cell width.
	// Parameters:
	//     nWidth - item control width.
	// See Also:
	//     GetSize, SetSize, GetWidth, GetHeight, SetHeight
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item control height.
	// Returns:
	//     Item control height.
	// See Also:
	//     GetSize, SetSize, GetWidth, SetWidth, SetHeight
	//-----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control height.
	//     If the height is less than 0, the control height is the same as the cell height.
	// Parameters:
	//     nHeight - item control height.
	// See Also:
	//     GetSize, SetSize, GetWidth, SetWidth, GetHeight
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the rectangle occupied by the item control.
	// Returns:
	//     Item control rectangle.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item control flags.
	// Returns:
	//     Item control flags.
	// See Also:
	//     SetFlags
	//-----------------------------------------------------------------------
	UINT GetFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control flags.
	// Parameters:
	//     unFlags - new item control flags.
	// See Also:
	//     GetFlags
	//-----------------------------------------------------------------------
	void SetFlags(UINT unFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the item control alignment.
	// Returns:
	//     Item control alignment.
	// See Also:
	//     XTPReportItemControlAlignment, SetAlignment
	//-----------------------------------------------------------------------
	int GetAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control alignment.
	// Parameters:
	//     nAlignment - Item control alignment.
	// See Also:
	//     XTPReportItemControlAlignment, GetAlignment
	//-----------------------------------------------------------------------
	void SetAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item control is enabled or disabled.
	// Returns:
	//     TRUE if item control is enabled, FALSE otherwise.
	// See Also:
	//     SetEnable
	//-----------------------------------------------------------------------
	BOOL GetEnable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable the item control.
	//     If the control is disabled, it is drawn as disabled and does not
	//     react on user input.
	// Parameters:
	//     bEnable - TRUE to enable item control, FALSE to disable.
	// See Also:
	//     GetEnable
	//-----------------------------------------------------------------------
	void SetEnable(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the current item control state.
	//     The state is control specific.
	// Returns:
	//     Item control state.
	//-----------------------------------------------------------------------
	int GetState() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the item control themed.
	// Parameters:
	//     bThemed - TRUE to set item control themed, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetThemed(BOOL bThemed);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item control is themed.
	// Returns:
	//     TRUE if item control is themed, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetThemed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon index for a specified control state.
	//     The icon specified for the certain state is drawn during control drawing.
	//     If no icon is specified for a certain state, the icon for state = 0 is drawn.
	//     If no icon specified for any state, no icon is drawn.
	// Parameters:
	//     nState - item control state.
	//     nIconIndex - icon index for specified state.
	// See Also:
	//     GetIconIndex
	//-----------------------------------------------------------------------
	void SetIconIndex(int nState, int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the icon index for a given item control state.
	// Parameters:
	//     nState - item control state, for which icon index is retrieved.
	// Returns:
	//     Icon index for a given item control state.
	// See Also:
	//     SetIconIndex
	//-----------------------------------------------------------------------
	int GetIconIndex(int nState) const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the rectangle occupied by the item control.
	// Parameters:
	//     rcControl - item control rectangle.
	// See Also:
	//     GetRect
	//-----------------------------------------------------------------------
	void SetRect(CRect rcControl);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnLButtonDown function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs) { UNREFERENCED_PARAMETER(pClickArgs); }

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnLButtonUp function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs) { UNREFERENCED_PARAMETER(pClickArgs); }

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that mouse cursor has entered the item controls's area.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseEnter(UINT nFlags, CPoint point) { UNREFERENCED_PARAMETER(nFlags); UNREFERENCED_PARAMETER(point); }

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that mouse cursor has left the item controls's area.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseLeave(UINT nFlags, CPoint point) { UNREFERENCED_PARAMETER(nFlags); UNREFERENCED_PARAMETER(point); }

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point) { UNREFERENCED_PARAMETER(nFlags); UNREFERENCED_PARAMETER(point); }

protected:
	int m_nType;                                  // Control type. See XTPReportItemControlType for details.
	int m_nIndex;                                 // Control index.
	CString m_strCaption;                         // Control caption.
	CFont m_fntCaption;                           // Control caption font.
	COLORREF m_clrCaption;                        // Control caption color.
	CSize m_sizeControl;                          // Control size. If size is (<=0, <=0), the control occupies entire cell.
	CRect m_rcControl;                            // Control rectangle.
	UINT m_unFlags;                               // Control flags.
	XTPReportItemControlAlignment m_Alignment;    // Control alignment. See XTPReportItemControlAlignment for details.
	BOOL m_bEnabled;                              // Control enable flag.
	int m_nState;                                 // Control state. Depends on control type.
	BOOL m_bThemed;                               // TRUE if control is themed, FALSE otherwise.
	CMap<int, int, int, int> m_mapIcon;           // Control icon map.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportRecordItemControl);

	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetCaption(LPCTSTR pcszCaption);

	LPFONTDISP OleGetFont();
	void OleSetFont(LPFONTDISP pFontDisp);

	afx_msg void OleSetSize(long cx, long cy);
	afx_msg void OleGetRect(long* pnLeft, long* pnTop, long* pnRight, long* pnBottom);
//}}AFX_CODEJOCK_PRIVATE
#endif

	friend class CXTPReportRecordItemControls;
	friend class CXTPReportRecordItemButton;
	friend class CXTPReportPaintManager;
};

//===========================================================================
// Summary:
//     This Class represents an item button and inherits basic functionality
//     from CXTPReportRecordItemControl class.
//     You create a button simply by calling a constructor with one
//     parameter - text string.
// See Also: CXTPReportRecordItemControl
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemButton : public CXTPReportRecordItemControl
{
	DECLARE_SERIAL(CXTPReportRecordItemButton)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an item button.
	// Parameters:
	//     szCaption - The button caption.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemButton(LPCTSTR szCaption = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report item control
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnLButtonDown function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnLButtonUp function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that mouse cursor has entered the item controls's area.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseEnter(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies the item control that mouse cursor has left the item controls's area.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseLeave(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     nFlags - additional flags.
	//     point  - mouse point.
	// Remarks:
	//     Usually this function is called by ReportRecordItem's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

protected:
	int m_nSavedState; // Button state when the mouse cursor leaves the button area.


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportRecordItemButton);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//      This typedef used to declare a base class for CXTPReportRecordItemControls.
//===========================================================================
typedef CXTPArrayT<CXTPReportRecordItemControl*, CXTPReportRecordItemControl*, LPDISPATCH> CXTPReportRecordItemControls_base;

//===========================================================================
// Summary:
//     This class represents inplace controls of the single item
//===========================================================================
//===========================================================================
// Summary:
//      This class represents array of pointers to CXTPReportRecordItemControl objects.
//      InternalRelease is called for each item in the array destructor.
//      Also serialization for array items is provided (see DoPropExchange method).
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemControls : public CXTPReportRecordItemControls_base
//class _XTP_EXT_CLASS CXTPReportRecordItemControls : public CXTPHeapObjectT<CXTPReportRecordItemControls_base, CXTPReportDataAllocator>
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPReportRecordItemControls)
	typedef CXTPReportRecordItemControls_base TBase;
	//}}AFX_CODEJOCK_PRIVATE
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportRecordItemControls object.
	// Parameters:
	//     pRecordItem - pointer to parent record item.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemControls();
	CXTPReportRecordItemControls(CXTPReportRecordItem* pRecordItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportRecordItemControls object, handles
	//     cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPReportRecordItemControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report item controls
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new control to control list.
	// Parameters:
	//     nType    - Type of the control to be added.
	//     nIndex - ordinal for position.
	// Returns:
	//     A pointer to the report record item control object.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemControl* AddControl(int nType, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new control to control list.
	// Parameters:
	//     pControl - Pointer to the control to be added.
	//     nIndex - ordinal for position
	// Returns:
	//     A pointer to the report record item control object.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemControl* AddControl(CXTPReportRecordItemControl* pControl, int nIndex = -1);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all controls of the CXTPReportRecordItemControls.
	//-------------------------------------------------------------------------
	//void RemoveControls();

	//-----------------------------------------------------------------------
	// Summary: Removed all controls from collection
	//-----------------------------------------------------------------------
	virtual void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary: Removes control by its index
	// Input:   nIndex - Index of control to remove
	//-----------------------------------------------------------------------
	virtual void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary: Call this method to copy all controls from one list to another
	// Input:   pSrc - List to copy controls from.
	//-----------------------------------------------------------------------
	virtual void CopyFrom(CXTPReportRecordItemControls* pSrc);

protected:
	//-----------------------------------------------------------------------
	// Summary: This method is called to update CXTPReportRecordItemControl::m_nIndex properties.
	// Input:   nStartFrom - First item to update
	//-----------------------------------------------------------------------
	virtual void RefreshIndexes(int nIndexStart=0);

public:
	CXTPReportRecordItem* m_pRecordItem; // pointer to parent record item.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportRecordItemControls);
	DECLARE_ENUM_VARIANT(CXTPReportRecordItemControls)

	afx_msg int OleGetCount();
	afx_msg LPDISPATCH OleAddButton(int nIndex);
	//afx_msg LPDISPATCH OleGetButton(int nIndex);
	virtual LPDISPATCH OleGetItem(long nIndex);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This Class represents an item control hook window.
//     An instance of this window is created when the user presses the left mouse button on an item control.
//     Further all mouse input goes to this window until the use releases the left mouse button.
//     The window hooks mouse events and redirects them to the report record item.
// See Also: CXTPReportRecordItem, CXTPReportRecordItemControl
//===========================================================================
class CXTPReportRecordItemControlHookWnd : public CWnd
{
public:

	//-----------------------------------------------------------------------
	// Summary: Constructs CXTPReportRecordItemControlHookWnd window
	// Input:   pClickArgs - Parameters of cell
	//-----------------------------------------------------------------------
	CXTPReportRecordItemControlHookWnd(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportRecordItemControlHookWnd object, handles
	//     cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPReportRecordItemControlHookWnd();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//}}AFX_CODEJOCK_PRIVATE

	XTP_REPORTRECORDITEM_CLICKARGS m_ClickArgs; // Parameters of cell
};

//////////////////////////////////////////////////////////////////////////
AFX_INLINE void CXTPReportRecordItemControl::SetCaption(LPCTSTR szCaption) {
	m_strCaption = szCaption;
}
AFX_INLINE CString CXTPReportRecordItemControl::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE int CXTPReportRecordItemControl::GetType() const {
	return m_nType;
}
AFX_INLINE int CXTPReportRecordItemControl::GetIndex() const  {
	return m_nIndex;
}
AFX_INLINE COLORREF CXTPReportRecordItemControl::GetCaptionColor() const {
	return m_clrCaption;
}
AFX_INLINE void CXTPReportRecordItemControl::SetCaptionColor(COLORREF clrCaption) {
	m_clrCaption = clrCaption;
}
AFX_INLINE CSize CXTPReportRecordItemControl::GetSize() const {
	return m_sizeControl;
}
AFX_INLINE void CXTPReportRecordItemControl::SetSize(CSize sizeControl) {
	m_sizeControl = sizeControl;
}
AFX_INLINE int CXTPReportRecordItemControl::GetWidth() const {
	return m_sizeControl.cx;
}
AFX_INLINE void CXTPReportRecordItemControl::SetWidth(int nWidth) {
	m_sizeControl.cx = nWidth;
}
AFX_INLINE int CXTPReportRecordItemControl::GetHeight() const {
	return m_sizeControl.cy;
}
AFX_INLINE void CXTPReportRecordItemControl::SetHeight(int nHeight) {
	m_sizeControl.cy = nHeight;
}
AFX_INLINE CRect CXTPReportRecordItemControl::GetRect() const{
	return m_rcControl;
}
AFX_INLINE void CXTPReportRecordItemControl::SetFlags(UINT unFlags) {
	m_unFlags = unFlags;
}
AFX_INLINE UINT CXTPReportRecordItemControl::GetFlags() const {
	return m_unFlags;
}
AFX_INLINE int CXTPReportRecordItemControl::GetAlignment() const {
	return m_Alignment;
}
AFX_INLINE void CXTPReportRecordItemControl::SetAlignment(int nAlignment) {
	m_Alignment = (XTPReportItemControlAlignment)nAlignment;
}
AFX_INLINE BOOL CXTPReportRecordItemControl::GetEnable() const {
	return m_bEnabled;
}
AFX_INLINE void CXTPReportRecordItemControl::SetEnable(BOOL bEnable) {
	m_bEnabled = bEnable;
}
AFX_INLINE int CXTPReportRecordItemControl::GetState() const {
	return m_nState;
}
AFX_INLINE void CXTPReportRecordItemControl::SetThemed(BOOL bThemed) {
	m_bThemed = bThemed;
}
AFX_INLINE BOOL CXTPReportRecordItemControl::GetThemed() const {
	return m_bThemed;
}
AFX_INLINE void CXTPReportRecordItemControl::SetIconIndex(int nState, int nIconIndex) {
	m_mapIcon.SetAt(nState, nIconIndex);
}
AFX_INLINE void CXTPReportRecordItemControl::SetRect(CRect rcControl) {
	m_rcControl = rcControl;
}

#endif //#if !defined(__XTPREPORTRECORDITEMCONTROLS_H__)
