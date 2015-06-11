// XTPReportRecordItem.h: interface for the CXTPReportRecordItem class.
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
#if !defined(__XTPREPORTRECORDITEM_H__)
#define __XTPREPORTRECORDITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupUIElement;
class CXTPMarkupContext;

class CXTPReportControl;
class CXTPReportRow;
class CXTPReportRecordItem;
class CXTPReportHyperlink;
class CXTPReportHyperlinks;
class CXTPReportColumn;
class CXTPReportPaintManager;
class CXTPReportRecord;
class CXTPReportInplaceButton;
class CXTPPropExchange;
class CXTPReportRecordItemConstraint;
class CXTPReportRecordItemConstraints;
class CXTPReportRecordItemControlHookWnd;
class CXTPReportRecordItemControl;
class CXTPReportRecordItemControls;
class CXTPReportBorder;


//===========================================================================
// Summary:
//     Basic set of parameters transferred to item handlers.
//     This structure groups together essential parameters used in
//     items processing activities. It is parent for all other structures:
//     XTP_REPORTRECORDITEM_DRAWARGS, XTP_REPORTRECORDITEM_CLICKARGS
// Example:
// <code>
// XTP_REPORTRECORDITEM_ARGS itemArgs(pControl, pRow, pColumn);
// pItem->OnBeginEdit(&itemArgs);
// </code>
// See Also: CXTPReportRecordItem::OnBeginEdit
//===========================================================================
struct _XTP_EXT_CLASS XTP_REPORTRECORDITEM_ARGS
{
//{{AFX_CODEJOCK_PRIVATE
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a XTP_REPORTRECORDITEM_ARGS object
	// Parameters:
	//     pControl - Parent report control pointer
	//     pRow     - Item's row pointer
	//     pColumn  - Item's column pointer
	//-----------------------------------------------------------------------
	XTP_REPORTRECORDITEM_ARGS();
	XTP_REPORTRECORDITEM_ARGS(CXTPReportControl* pControl, CXTPReportRow* pRow, CXTPReportColumn* pColumn);
	// <COMBINE XTP_REPORTRECORDITEM_ARGS::XTP_REPORTRECORDITEM_ARGS>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGrid object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~XTP_REPORTRECORDITEM_ARGS();

	void AddRef();
	void Release();

	const XTP_REPORTRECORDITEM_ARGS& operator=(const XTP_REPORTRECORDITEM_ARGS& src);
//}}AFX_CODEJOCK_PRIVATE

	CXTPReportControl* pControl;    // Pointer to the main Report control.
	CXTPReportRow* pRow;            // Pointer to the associated row.
	CXTPReportColumn* pColumn;      // Report column at click position, if any, NULL otherwise.
	CXTPReportRecordItem* pItem;    // Pointer to the associated item.
	CRect rcItem;                   // Item position in control client coordinates.
};

//===========================================================================
// Summary:
//     A set of parameters transferred to Draw item handler.
//     It inherits essential parameters from XTP_REPORTRECORDITEM_ARGS.
// Remarks:
//     This structure is used in almost all functions concerning drawing
//     activities
// Example:
// <code>
// // fill structure
// XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
// drawArgs.pDC = pDC;
// drawArgs.pControl = m_pControl;
// drawArgs.pRow = pRow;
// // call function
// pItem->Draw(&drawArgs);
// </code>
// See Also: CXTPReportPaintManager
//===========================================================================
struct _XTP_EXT_CLASS XTP_REPORTRECORDITEM_DRAWARGS : public XTP_REPORTRECORDITEM_ARGS
{
//{{AFX_CODEJOCK_PRIVATE
	XTP_REPORTRECORDITEM_DRAWARGS()
	{
		nTextAlign = DT_LEFT;
		pDC = NULL;
	}
//}}AFX_CODEJOCK_PRIVATE

	CDC* pDC;                       // Pointer to control drawing context.
	int nTextAlign;                 // Text alignment mode DT_LEFT || DT_RIGHT || DT_CENTER
};

//===========================================================================
// Summary:
//     A set of parameters transferred to mouse click item handlers.
//     It inherits essential parameters from XTP_REPORTRECORDITEM_ARGS.
// Remarks:
//     XTP_REPORTRECORDITEM_CLICKARGS is used in most functions involved
//     in mouse events processing
// See Also: CXTPReportControl, CXTPReportRecordItem::OnClick
//===========================================================================
struct _XTP_EXT_CLASS XTP_REPORTRECORDITEM_CLICKARGS : public XTP_REPORTRECORDITEM_ARGS
{
	CPoint ptClient;                // Coordinates of the mouse click point.
};

//===========================================================================
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from Item
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window
// Example:
// <code>
// BEGIN_MESSAGE_MAP(CPropertiesView, CView)
//     ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, ID_REPORT_CONTROL, OnPropertyChanged)
// END_MESSAGE_MAP()
//
// void CPropertiesView::OnPropertyChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//
//     switch (pItemNotify->pItem->GetItemData())
//     {
//         // Some code.
//     }
// }
// </code>
// See Also: XTP_NM_REPORTINPLACEBUTTON
//===========================================================================
struct XTP_NM_REPORTRECORDITEM
{
	NMHDR hdr;                          // Standard structure, containing information about a notification message.
	CXTPReportRow* pRow;                // Pointer to the row associated with the notification.
	CXTPReportRecordItem* pItem;        // Pointer to the record item associated with the notification.
	CXTPReportColumn* pColumn;          // Pointer to the column associated with the notification.
	int   nHyperlink;                   // Index of clicked Hyperlink, if any, or -1 otherwise.
	POINT pt;                           // Point where the message has happened.
};

//===========================================================================
// Summary:
//     A set of parameters transferred with XTP_NM_REPORT_REQUESTEDIT message
// Remarks:
//     XTP_NM_REPORTREQUESTEDIT is used to cancel edit operations
// See Also: XTP_NM_REPORT_REQUESTEDIT
//===========================================================================
struct XTP_NM_REPORTREQUESTEDIT : public XTP_NM_REPORTRECORDITEM
{
	BOOL bCancel;   //TRUE to cancel operation.
};

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
struct XTP_NM_REPORTVALUECHANGING : public XTP_NM_REPORTRECORDITEM
{
	BOOL bCancel;
	LPVARIANT lpNewValue;
};
//}}AFX_CODEJOCK_PRIVATE
#endif

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from Item
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window
// Remarks:
//     Use this structure to get in-place button which produce XTP_NM_REPORT_INPLACEBUTTONDOWN message.
// See Also: XTP_NM_REPORT_INPLACEBUTTONDOWN
//-----------------------------------------------------------------------
struct XTP_NM_REPORTINPLACEBUTTON : public XTP_NM_REPORTRECORDITEM
{
	CXTPReportInplaceButton* pButton;           // Pointer to in-place button
};

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from Item
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window.
// Remarks:
//     It is sent before clipboard Copy/Paste operations.
// See Also:
//      XTP_NM_REPORT_BEFORE_COPY_TOTEXT, XTP_NM_REPORT_BEFORE_PASTE_FROMTEXT,
//      XTP_NM_REPORT_BEFORE_PASTE, CXTPReportControl::OnBeforeCopyToText(),
//      CXTPReportControl::OnBeforePasteFromText(),
//      CXTPReportControl::OnBeforePaste().
//-----------------------------------------------------------------------
struct XTP_NM_REPORT_BEFORE_COPYPASTE
{
	NMHDR hdr;                          // Standard structure, containing information about a notification message.

	CXTPReportRecord**  ppRecord;       // [in/out] A pointer to record pointer;
	CStringArray*       parStrings;     // [in/out] A pointer to strings array with record items values.
};


//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from Item
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window
// Remarks:
//     Use this structure to notify about constraint changing.
// See Also: XTP_NM_REPORTCONSTRAINTSELECTING
//-----------------------------------------------------------------------
struct XTP_NM_REPORTCONSTRAINTSELECTING : public XTP_NM_REPORTRECORDITEM
{
	CXTPReportRecordItemConstraint* pConstraint; // Pointer to the constraint associated with the notification.
};

//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from Item
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window
// Remarks:
//     Use this structure to notify about tooltip showing.
// See Also: XTP_NM_REPORTTOOLTIPTEXT
//-----------------------------------------------------------------------
struct XTP_NM_REPORTTOOLTIPINFO : public XTP_NM_REPORTRECORDITEM
{
	CString* pstrText; // Pointer to a CString object with Tooltip text.
	// int nImageIndex;
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines alignment style for the column icon and text.
//-----------------------------------------------------------------------
enum XTPReportColumnIconAlignment
{
	xtpColumnTextLeft         = DT_LEFT,   // Aligns text to the left.
	xtpColumnTextCenter       = DT_CENTER, // Centers text horizontally in the column.
	xtpColumnTextRight        = DT_RIGHT,  // Aligns text to the right.
	xtpColumnTextVCenter      = DT_VCENTER,// Centers text vertically.
	xtpColumnTextWordBreak    = DT_WORDBREAK, // Breaks words. Lines are automatically broken between words if a word would extend past the edge of the rectangle. A carriage return-line feed sequence also breaks the line.

	xtpColumnTextMask         = 0xFF,      // A mask for text alignment styles.

	xtpColumnIconLeft         = 0x00100000, // Aligns icon to the left.
	xtpColumnIconCenter       = 0x00200000, // Centers icon horizontally in the column.
	xtpColumnIconRight        = 0x00400000, // Aligns icon to the right.

	xtpColumnIconTop          = 0x01000000, // Aligns icon to the top.
	xtpColumnIconVCenter      = 0x02000000, // Aligns icon to the center.
	xtpColumnIconBottom       = 0x04000000, // Aligns icon to the bottom.

	xtpColumnIconMask         = 0x0FF00000  // A mask for icon alignment styles.
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines alignment style for the group row custom icon.
// See Also:
//      XTP_REPORTRECORDITEM_METRICS::nGroupRowIconAlignment
//-----------------------------------------------------------------------
enum XTPReportGroupRowIconAlignment
{
	xtpGroupRowIconUnknown      = 0,    // Unknown (empty) value.

	xtpGroupRowIconLeft         = 0x001, // Draw icon at the left side of group row rect.
	xtpGroupRowIconBeforeText   = 0x002, // Draw icon before caption text (between Expand/Collapse icon and text).
	xtpGroupRowIconAfterText    = 0x004, // Draw icon after caption text.
	xtpGroupRowIconRight        = 0x008, // Draw icon at the right side of group row rect.

	xtpGroupRowIconHmask        = 0x00F, // A mask for horizontal alignment flags.

	xtpGroupRowIconVTop         = 0x100, // Vertical alignment: top of group row rect.
	xtpGroupRowIconVCenter      = 0x200, // Vertical alignment: center of group row rect.
	xtpGroupRowIconVCenterToText= 0x400, // Vertical alignment: center of caption text rect.
	xtpGroupRowIconVBottom      = 0x800, // Vertical alignment: bottom of group row rect.

	xtpGroupRowIconVmask        = 0xF00, // A mask for vertical alignment flags.
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines flags for virtual row metrics.
// See Also:
//      XTP_REPORTRECORDITEM_METRICS::nVirtRowFlags
//-----------------------------------------------------------------------
enum XTPReportVirtualRowFlags
{
	xtpVirtRowUnknown           = 0,     // Unknown (empty) value.
	xtpVirtRowHasChildren       = 0x001, // A row has children.
	xtpVirtRowLastChild         = 0x002  // A row is the last child of its parent.
};

//-----------------------------------------------------------------------
// Summary:
//     Record item drawing metrics.
//     Its helper structures group together parameters to store general
//     drawing metrics
//-----------------------------------------------------------------------
struct XTP_REPORTRECORDITEM_METRICS : public CXTPCmdTarget
{
	CFont* pFont;               // Drawing font.
	COLORREF clrForeground;     // Item foreground color.
	COLORREF clrBackground;     // Item background color.
	CString strText;            // Item text.

	int nGroupRowIcon;          // Group row icon ID. See CXTPReportControl::GetImageManager()
	int nGroupRowIconAlignment; // Group row icon alignment. See XTPEnumGroupRowIconAlignment
	int nColumnAlignment;       // Column alignment. See XTPReportColumnIconAlignment
	int nItemIcon;              // Item icon ID. See CXTPReportControl::GetImageManager()
	int nVirtRowLevel;          // Virtual row level.
	int nVirtRowFlags;          // Virtual row flags.

//{{AFX_CODEJOCK_PRIVATE
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a XTP_REPORTRECORDITEM_METRICS struct
	//-------------------------------------------------------------------------
	XTP_REPORTRECORDITEM_METRICS()
	{
		Reset();

	#ifdef _XTP_ACTIVEX
		EnableAutomation();
	#endif
	}

	void Reset()
	{
		pFont = NULL;
		clrForeground = XTP_REPORT_COLOR_DEFAULT;
		clrBackground= XTP_REPORT_COLOR_DEFAULT;

		nGroupRowIcon = XTP_REPORT_NOICON;
		nGroupRowIconAlignment = xtpGroupRowIconRight | xtpGroupRowIconVCenter;
		nColumnAlignment = xtpColumnTextLeft;
		nItemIcon = XTP_REPORT_NOICON;
		nVirtRowLevel = 0;
		nVirtRowFlags = 0;
	}
//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	~XTP_REPORTRECORDITEM_METRICS()
	{
	}

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	LPFONTDISP OleGetFont();
	void OleSetFont(LPFONTDISP pFontDisp);
	CFont       m_fntMetrics;

//}}AFX_CODEJOCK_PRIVATE
#endif
};



//===========================================================================
// Summary:
//     This class represents collection of the in-place buttons
//     of the single item of the report control.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceButtons : public CArray<CXTPReportInplaceButton*, CXTPReportInplaceButton*>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceButtons object
	//-----------------------------------------------------------------------
	CXTPReportInplaceButtons();
};


//===========================================================================
// Summary:
//     This class represents edit options of the single item or column
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemEditOptions : public CXTPCmdTarget
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportRecordItemEditOptions object.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemEditOptions();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportRecordItemEditOptions object, handles
	//     cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPReportRecordItemEditOptions();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new constraint to constraint list.
	// Parameters:
	//     lpszConstraint - Caption of the constraint to be added.
	//     dwData         - The 32-bit value associated with the constraint.
	// Returns:
	//     A pointer to report record item constraint object.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemConstraint* AddConstraint(LPCTSTR lpszConstraint, DWORD_PTR dwData = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds constraint by its value or caption
	// Parameters:
	//     dwData - The 32-bit value associated with the constraint.
	// Returns:
	//     Constraint pointer if found or NULL  if the constraint
	//     is not found.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemConstraint* FindConstraint(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds constraint by its value or caption
	// Parameters:
	//     lpszConstraint - caption of the constraint
	// Returns:
	//     Constraint pointer if found or NULL  if the constraint
	//     is not found.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemConstraint* FindConstraint(LPCTSTR lpszConstraint);

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves constraints pointer.
	// Returns:
	//     Constraint pointer of the edit options.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemConstraints* GetConstraints()
	{
		return m_pConstraints;
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to add a combo button to the CXTPReportRecordItem.
	// Parameters:
	//  bInside - BOOL falg to keep inside parent control rectangle
	// Returns:
	//  pointer to CXTPReportInplaceButton
	// Remarks:
	//     This adds a drop-down combo button to the ReportRecordItem.  When
	//     the button is pressed, it will display all of the data items that
	//     were added as CXTPReportRecordItemConstraints.
	//-------------------------------------------------------------------------
	CXTPReportInplaceButton* AddComboButton(BOOL bInside = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to add a expand button to the CXTPReportRecordItem.
	// Parameters:
	//  bInside - BOOL falg to keep inside parent control rectangle
	// Returns:
	//  pointer to CXTPReportInplaceButton
	// Remarks:
	//     A small button with three ellipses is displayed and you can use this
	//     button to display your own custom dialog.
	//-------------------------------------------------------------------------
	CXTPReportInplaceButton* AddExpandButton(BOOL bInside = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to add a spin button to the CXTPReportRecordItem.
	// Parameters:
	//  bInside - BOOL flag to keep inside parent control rectangle
	// Returns:
	//  pointer to CXTPReportInplaceButton
	// Remarks:
	//     A spin control with two buttons is displayed and you can use this
	//     button to change the cell numeric value.
	//-------------------------------------------------------------------------
	CXTPReportInplaceButton* AddSpinButton(BOOL bInside = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to get a buttons with the specified index.
	// Parameters:
	//     nIndex - the index of a button to get.
	// Returns:
	//     pointer to CXTPReportInplaceButton
	//-------------------------------------------------------------------------
	CXTPReportInplaceButton* GetButton(int nIndex);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all buttons of the CXTPReportRecordItem.
	//-------------------------------------------------------------------------
	void RemoveButtons();

public:
	BOOL m_bAllowEdit;// TRUE to enable changing the value of the field, FALSE to enable read-mode. If FALSE, m_bConstraintEdit is ignored.
	BOOL m_bConstraintEdit; // TRUE to prevent possibility to type in a custom value in the edit box if applicable.
	CXTPReportRecordItemConstraints* m_pConstraints;         // Constraint list.
	CXTPReportInplaceButtons arrInplaceButtons;     // Array of in-place buttons.
	BOOL m_bSelectTextOnEdit; // Select all text on edit
	BOOL m_bExpandOnSelect; // Expand an expandable contents on edit
	DWORD m_dwEditStyle; // Edit Style (ES_MULTILINE, ES_NUMBER....)
	int m_nMaxLength; // Maximum number of characters that can be entered into an editable item (Edit limit).

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportRecordItemEditOptions);

	afx_msg LPDISPATCH OleGetConstraints();
	afx_msg LPDISPATCH OleGetInplaceButton(long nIndex);
	afx_msg long OleInplaceButtonsCount();

	afx_msg LPDISPATCH OleAddComboButton(const VARIANT& bInside);
	afx_msg LPDISPATCH OleAddExpandButton(const VARIANT& bInside);
	afx_msg LPDISPATCH OleAddSpinButton(const VARIANT& bInside);

//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     Class for working with single list cell.
//     Base class for extended list cells. It determines the behavior
//     of all specific record items.
//     You don't use its class directly in the control.
//     If you wish to add your own Record Item to the control, you must inherit
//     it from CXTPReportRecordItem. Most member functions in this class
//     are virtual.
// See Also:
//     CXTPReportRecordItemDateTime, CXTPReportRecordItemNumber,
//     CXTPReportRecordItemPreview, CXTPReportRecordItemText,
//     CXTPReportRecordItemVariant
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItem : public CXTPHeapObjectT<CCmdTarget, CXTPReportDataAllocator>
{
	DECLARE_SERIAL(CXTPReportRecordItem)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPReportRecordItem default constructor
	//-----------------------------------------------------------------------
	CXTPReportRecordItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPReportRecordItem default destructor
	//-----------------------------------------------------------------------
	virtual ~CXTPReportRecordItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contains drawing arguments:
	// Returns:
	//     An integer specifying the item text width.
	// Remarks:
	//     Call this member function to draw an item. Actually this function
	//     only prepares and calls needed drawing functions from PaintManager.
	//     Thus if you wish to change the look of your report item, you must just provide
	//     your own implementation of PaintManager
	// Example:
	// <code>
	// // fill structure
	// XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
	// drawArgs.pDC = pDC;
	// drawArgs.pControl = m_pControl;
	// drawArgs.pRow = this;
	// // call function
	// Draw(&drawArgs);
	// </code>
	//
	// See Also: XTP_REPORTRECORDITEM_DRAWARGS
	//-----------------------------------------------------------------------
	virtual int Draw(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     pMetrics - structure which contain metrics of the item.
	//-----------------------------------------------------------------------
	virtual void OnDrawCaption(
		XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
		XTP_REPORTRECORDITEM_METRICS  *pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     rcItem - item rectangle.
	//-----------------------------------------------------------------------
	virtual void OnDrawControls(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which report item control, if any, is at a specified position.
	// Parameters:
	//     ptPoint     - Point to test.
	// Remarks:
	//     Call this member function to test an item contents at the
	//     specified position.
	// Returns:
	//     The item control at the specified position, if any,
	//     or NULL otherwise.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemControl* HitTest(CPoint ptPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determine if the click occurred on the hyperlink
	// Parameters:
	//     ptClick - point of the mouse click
	// Returns:
	//     Zero-based index of clicked link, -1 otherwise
	//-----------------------------------------------------------------------
	virtual int HitTestHyperlink(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Returns:
	//     TRUE if handled, FALSE else.
	// Remarks:
	//     Usually this function is called by ReportRow's OnLButtonDown function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Returns:
	//     TRUE if handled, FALSE else.
	// Remarks:
	//     Usually this function is called by ReportRow's OnLButtonUp function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Remarks:
	//     Usually this function is called by ReportRow's OnClick function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments:
	// Remarks:
	//     Usually this function is called by ReportRow's OnDblClick function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnDblClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     point  - mouse point
	//     nFlags - additional flags.
	//     pControl - pointer to report control.
	// Remarks:
	//     Usually this function is called by ReportRow's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point, CXTPReportControl* pControl = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns item text caption.
	// Remarks:
	//     Can be overridden by descendants.
	// Returns:
	//     Item text caption (empty string for base record item class).
	//-----------------------------------------------------------------------
	virtual CString GetCaption()
	{
		return GetCaption(GetColumn());
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns item text caption.
	// Parameters:
	//     pColumn - Corresponded column of the item.
	// Remarks:
	//     Can be overridden by descendants.
	// Returns:
	//     Item text caption (empty string for base record item class).
	//-----------------------------------------------------------------------
	virtual CString GetCaption(CXTPReportColumn* pColumn)
	{
		UNREFERENCED_PARAMETER(pColumn); return m_strCaption;
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves caption text bounding rectangle
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     rcItem    - Bounding rectangle of the item
	//-----------------------------------------------------------------------
	virtual void GetCaptionRect(XTP_REPORTRECORDITEM_ARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns group caption.
	// Parameters:
	//     pColumn - point to the column
	// Returns:
	//     Text of Group Caption.
	//-----------------------------------------------------------------------
	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns resource id of string containing text
	// Parameters:
	//     pColumn - Point to the column
	// Returns:
	//     Integer value of resource id
	//-----------------------------------------------------------------------
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares group captions.
	// Parameters:
	//     pColumn - Point to the column
	//     pItem   - Point to the item
	// Returns:
	//     Zero if the items' values are identical,
	//     < 0 if this item value is less than provided,
	//     or > 0 if this item value is greater than provided.
	//-----------------------------------------------------------------------
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares this item with the provided one.
	//     Provides default behavior for descendants.
	// Parameters:
	//     pColumn - Corresponded column of the items.
	//     pItem - points to the compared Item
	// Returns:
	//     Zero if the items' values are identical,
	//     < 0 if this item value is less than provided,
	//     or > 0 if this item value is greater than provided.
	//-----------------------------------------------------------------------
	virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item text font.
	// Parameters:
	//     pFont - New font for caption.
	//-----------------------------------------------------------------------
	virtual void SetFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get item text font.
	// Returns:
	//     Pointer to the current text font.
	//-----------------------------------------------------------------------
	virtual CFont* GetFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the text tool tip for the item
	// Returns:
	//     The text tool tip for the item.
	//-----------------------------------------------------------------------
	virtual CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set/change the item tool tip.
	// Parameters:
	//     lpszTooltip - Tool tip of the item
	//-----------------------------------------------------------------------
	virtual void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item text color.
	// Parameters:
	//     clrText - New color.
	//-----------------------------------------------------------------------
	virtual void SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns item text color.
	// Returns:
	//     COLORREF
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets/clears bold flag for the item font.
	// Parameters:
	//     bBold - If TRUE set font to bold.
	// Remarks:
	//     By using this function you can easily change font to bold or to normal.
	//-----------------------------------------------------------------------
	virtual void SetBold(BOOL bBold = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the Bold state for the item
	// Returns:
	//     TRUE if item is bold.
	//-----------------------------------------------------------------------
	virtual BOOL IsBold() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item text background color.
	// Parameters:
	//     clrBackground - New background color.
	//-----------------------------------------------------------------------
	virtual void SetBackgroundColor(COLORREF clrBackground);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns item text background color.
	// Returns:
	//     COLORREF
	//-----------------------------------------------------------------------
	virtual COLORREF GetBackgroundColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets item Formula
	// Returns:
	//     String object, containing current Formula
	//-----------------------------------------------------------------------
	virtual CString GetFormula() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item Formula.
	// Parameters:
	//     sFormula - New Formula.
	virtual void SetFormula(LPCTSTR sFormula);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets item caption format string.
	// Returns:
	//     String object, containing current format string
	//-----------------------------------------------------------------------
	virtual CString GetFormatString() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item caption format string.
	// Parameters:
	//     strFormat - New format string.
	// Remarks:
	//     If the format string is set, a caption text will be formatted
	//     accordingly to this format string before drawing it; convenient
	//     in many cases (drawing date for example). Format string is C - like
	//     style, see sprintf() C function or CString.Format() member function
	//-----------------------------------------------------------------------
	virtual void SetFormatString(LPCTSTR strFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to programmatically check if item is
	//     editable.
	// Returns:
	//     BOOLEAN value represents current editable flag
	//-----------------------------------------------------------------------
	virtual BOOL IsEditable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets editable flag.
	// Parameters:
	//     bEditable - new editable flag (default is TRUE).
	// Returns:
	//     Old item editable state.
	//-----------------------------------------------------------------------
	virtual BOOL SetEditable(BOOL bEditable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new Hyperlink.
	// Parameters:
	//     pHyperlink - pointer to hyperlink.
	// Remarks:
	//     Each record item can have a number of hyperlinks.
	//     To add new hyperlink to record item you firs create create
	//     the new instance of hyperlink class.
	// Returns:
	//     zero-based id of new Hyperlink.
	// Example:
	//     <code>AddHyperlink(new CXTPReportHyperlink(27, 7);</code>
	//
	// See Also: CXTPReportHyperlink.
	//-----------------------------------------------------------------------
	virtual int AddHyperlink(CXTPReportHyperlink* pHyperlink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a count of hyperlinks in record item
	// Remarks:
	//     Each record item can have a number of hyperlinks. To process
	//     all of them, you get the count of hyperlinks that are processed by an item
	//     using GetHyperlinksCount() member function
	// Returns:
	//     Count of hyperlinks in item text
	//-----------------------------------------------------------------------
	virtual int GetHyperlinksCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Return Hyperlink by it's index.
	// Parameters:
	//     nHyperlink - zero-based index of hyperlink.
	// Remarks:
	//     To process a hyperlink, you get a pointer to it by calling GetHyperlinkAt();
	// Returns:
	//     Pointer to Hyperlink.
	// Example:
	// <code>
	// int nHyperlinks = GetHyperlinksCount();
	// CXTPReportHyperlink* pHyperlink;
	// for(int nHyperlink = 0; nHyperlink < nHyperlinks; nHyperlink++)
	// {
	//     pHyperlink = GetHyperlinkAt(nHyperlink);
	//     // YOUR PROCESSING HERE
	//     // ...
	//     //
	// }
	// </code>
	// See Also: CXTPReportHyperlink.
	//-----------------------------------------------------------------------
	virtual CXTPReportHyperlink* GetHyperlinkAt(int nHyperlink) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove hyperlink at the specified index.
	// Parameters:
	//     nHyperlink - zero-based index of hyperlink.
	//-----------------------------------------------------------------------
	virtual void RemoveHyperlinkAt(int nHyperlink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills XTP_REPORTRECORDITEM_METRICS structure
	// Parameters:
	//     pDrawArgs    - Draw arguments for calculating item metrics.
	//     pItemMetrics - Pointer to the metrics item to fill with values.
	// Remarks:
	//     Calculates preview item metrics based on provided draw arguments.
	//     Could be overridden by descendants.
	// See Also: XTP_REPORTRECORDITEM_DRAWARGS, XTP_REPORTRECORDITEM_METRICS
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this item is a preview item.
	// Returns:
	//     TRUE for the preview item, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsPreviewItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns item index.
	// Returns:
	//     Item numeric index.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns an ID of the associated resource bitmap.
	// Returns:
	//     An ID of the associated resource bitmap.
	//-----------------------------------------------------------------------
	virtual int GetIconIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new associated bitmap ID.
	// Parameters:
	//     nIconIndex - ID of the bitmap from the application resources.
	// Returns:
	//     Returns an ID of old resource bitmap.
	//-----------------------------------------------------------------------
	virtual int SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current alignment value for the item.
	// Returns:
	//     Current item alignment value.
	//     The result is a combination of alignment styles defined in XTPReportColumnIconAlignment.
	//-----------------------------------------------------------------------
	int GetAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new alignment value for the item.
	// Parameters:
	//     nAlignment - New alignment value. See XTPReportColumnIconAlignment
	//                  for possible alignment styles.
	// Returns:
	//     Previous alignment item value.
	//-----------------------------------------------------------------------
	int SetAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves sort priority of the item.
	// Returns:
	//     An integer specifying the sort priority.
	// See Also: GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	virtual int GetSortPriority() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the sort priority of the item.
	// Parameters:
	//     nSortPriority - Priority will be used for sort routines.
	// See Also:
	//     GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	virtual void SetSortPriority(int nSortPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves sort priority of the item.
	// Returns:
	//     An integer specifying the group priority.
	// See Also: GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	virtual int GetGroupPriority() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group priority of the item.
	// Parameters:
	//     nGroupPriority - Priority will be used for sort routines.
	// See Also: GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	virtual void SetGroupPriority(int nGroupPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group caption of the item.
	// Parameters:
	//     strCaption - Caption of group to set.
	//-----------------------------------------------------------------------
	virtual void SetGroupCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets caption of the item.
	// Parameters:
	//     strCaption - Caption of item to set.
	//-----------------------------------------------------------------------
	virtual void SetCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check the item.
	// Parameters:
	//     bChecked - TRUE if item is checked.
	// See Also:
	//     IsChecked, HasCheckbox
	//-----------------------------------------------------------------------
	virtual void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is checked
	// Returns:
	//     TRUE if the item is checked, FALSE else.
	// See Also: SetChecked, HasCheckbox
	//-----------------------------------------------------------------------
	virtual BOOL IsChecked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function set the item state checked.
	// Parameter:
	//     iState - Specifyes the the check state, 1 for checked and 2 for grayed.
	// See Also: SetChecked, HasCheckbox
	//-----------------------------------------------------------------------
	void SetCheckedState(int iState);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function get the item's checked state.
	// Returns:
	//     An integer denoting the the check state, 1 for checked and 2 for grayed.
	// See Also: SetChecked, HasCheckbox
	//-----------------------------------------------------------------------
	int GetCheckedState() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds check box area for item.
	// Parameters:
	//  bHasCheckbox - TRUE if item has check box area.
	//  bTristate  - TRUE is tri-state checkbox, FALSE - if usual
	// See Also: SetChecked, IsChecked
	//-----------------------------------------------------------------------
	virtual void HasCheckbox(BOOL bHasCheckbox, BOOL bTristate = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if item has check box area
	// Returns:
	//     TRUE if item has check box; otherwise FALSE
	// See Also: SetChecked, IsChecked
	//-----------------------------------------------------------------------
	virtual BOOL GetHasCheckbox() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns whether items with a check box will go into edit mode on click.
	// Returns:
	//     TRUE if items with a check box will go into edit mode on click.
	//-----------------------------------------------------------------------
	BOOL GetCanEditCheckboxItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether items with a check box will go into edit mode on click.
	// Parameters:
	//     bSet - TRUE for items with a check box will go into edit mode on click.
	//-----------------------------------------------------------------------
	void SetCanEditCheckboxItem(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieved parent record object.
	// Returns:
	//     Pointer to parent record object.
	//-----------------------------------------------------------------------
	CXTPReportRecord* GetRecord () const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the item.
	// Returns:
	//     The 32-bit value associated with the item.
	//-----------------------------------------------------------------------
	virtual DWORD_PTR GetItemData() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the 32-bit value associated with the item.
	// Parameters:
	//     dwData - Contains the new value to associate with the item.
	//-----------------------------------------------------------------------
	virtual void SetItemData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a CXTPReportRecordItem can receive focus.
	// Returns:
	//     TRUE is the CXTPReportRecordItem can receive focus, FALSE if
	//     it can not receive focus.
	//-----------------------------------------------------------------------
	virtual BOOL IsFocusable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the CXTPReportRecordItem can have focus.
	// Parameters:
	//     bFocusable - TRUE to allow the item to have focus, FALSE to specify the
	//                  item can not have focus.
	// Remarks:
	//     This will override the CXTPReportControl::FocusSubItems setting which only
	//     allows individual items to receive focus.  The SetFocusable member specifies
	//     whether the item can receive focus, if FALSE, the item can not be edited
	//     and the combo or expand buttons will not be displayed.
	//-----------------------------------------------------------------------
	virtual void SetFocusable(BOOL bFocusable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this method to get Markup element that renders the item
	// Returns:
	//      Pointer to Markup element
	//-----------------------------------------------------------------------
	CXTPMarkupUIElement* GetMarkupUIElement() const;

	//-----------------------------------------------------------------------
	// Summary: The framework calls this member function to determine
	//          whether a point is in the bounding rectangle of the
	//          specified tool.
	// Parameters:   point - Specifies the x- and y coordinate of the cursor.
	//                  These coordinates are always relative to the
	//                  upper-left corner of the window
	//          pTI - A pointer to a TOOLINFO structure.
	// Returns: If the tooltip control was found, the window control ID. If
	//          the tooltip control was not found, -1.
	//-----------------------------------------------------------------------
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI);
//GRID
	void Merge(CXTPReportRecordItem *pMergeItem);

	BOOL IsMerged() const;

	CXTPReportRecordItem* GetMergeItem() const;

	CXTPReportRecordItem* m_pMergeItem;        //
//GRID
protected:

	virtual void ParseBBCode(CString &strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws check box bitmap
	// Parameters:
	//     pDrawArgs - Pointer to structure with drawing arguments
	//     rcItem    - Item area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawCheckBox(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to access hyperlinks collection.
	// Returns:
	//     A pointer to CXTPReportHyperlinks object.
	//-----------------------------------------------------------------------
	CXTPReportHyperlinks* GetHyperlinks();


protected:

	virtual BOOL IsAllowEdit(XTP_REPORTRECORDITEM_ARGS *pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an editable item enters edit mode.
	// Parameters:
	//     pItemArgs - Pointer to a XTP_REPORTRECORDITEM_ARGS struct.
	// Remarks:
	//     An item can enter edit mode when the user clicks on it and starts typing,
	//     or when a check box item is checked\unchecked.
	// Returns:
	//     Returns TRUE if the edit request was successful, FALSE if the edit
	//     request was canceled.
	//-----------------------------------------------------------------------
	virtual BOOL OnRequestEdit(XTP_REPORTRECORDITEM_ARGS *pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user begin edit item.
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//-----------------------------------------------------------------------
	virtual void OnBeginEdit(XTP_REPORTRECORDITEM_ARGS *pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user press key.
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//     nChar     - Pressed character code
	// Returns:
	//     TRUE if item proceeded key.
	//-----------------------------------------------------------------------
	virtual BOOL OnChar(XTP_REPORTRECORDITEM_ARGS *pItemArgs, UINT nChar);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user cancel edit item.
	// Parameters:
	//     pControl - Parent report control.
	//     bApply   - TRUE to save value of in-place edit.
	//-----------------------------------------------------------------------
	virtual void OnCancelEdit(CXTPReportControl *pControl, BOOL bApply);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to save value from in-place edit
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//-----------------------------------------------------------------------
	virtual void OnValidateEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when value of in-place edit control changed
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	//virtual void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
	//{
	//  UNREFERENCED_PARAMETER(pItemArgs);
	//  UNREFERENCED_PARAMETER(szText);
	//}
	virtual void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when in-place editor value changed by user
	// Parameters:
	//     pItemArgs   - item arguments
	//     rstrNewText - new string to be set in the editor
	// Returns:
	//     FALSE to cancel edit operation.
	//-----------------------------------------------------------------------
	//virtual BOOL OnEditChanging(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CString& rstrNewText)
	//{
	//  UNREFERENCED_PARAMETER(pItemArgs);
	//  UNREFERENCED_PARAMETER(rstrNewText);
	//  return TRUE;
	//}
	virtual BOOL OnEditChanging(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CString& rstrNewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when in-place editing is canceled and value
	//     is not changed.
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//-----------------------------------------------------------------------
	virtual void OnEditCanceled(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user select specified constraint in in-place list
	// Parameters:
	//     pItemArgs   - Pointer to structure with items arguments
	//     pConstraint - Selected constraint
	//-----------------------------------------------------------------------
	virtual void OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves associated value with selected constraint.
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments
	// Returns:
	//     DWORD value associated with selected constraint.
	//-----------------------------------------------------------------------
	virtual DWORD GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user press in-place button of the item.
	// Parameters:
	//     pButton - In-place button of the item.
	//-----------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPReportInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves edit options of item.
	// Parameters:
	//     pColumn - Column of the item.
	// Returns:
	//     Pointer to CXTPReportRecordItemEditOptions class.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemEditOptions* GetEditOptions(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves item controls list.
	// Returns:
	//     Pointer to CXTPReportRecordItemControls class.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemControls* GetItemControls();


	double StringToDouble(CString strText);


	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a border for this item.
	//-----------------------------------------------------------------------
	BOOL CreateBorder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys the border of this item.
	//-----------------------------------------------------------------------
	BOOL DestroyBorder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the border of this item.
	//-----------------------------------------------------------------------
	CXTPReportBorder* GetBorder() const;

protected:

	//{{AFX_CODEJOCK_PRIVATE
	//-----------------------------------------------------------------------
	// Summary:
	//     If possible, returns the column associated with this Record Item.
	//     Otherwise, returns NULL
	//-----------------------------------------------------------------------
	virtual CXTPReportColumn* GetColumn();
	virtual void DoMouseButtonClick();
	//}}AFX_CODEJOCK_PRIVATE

protected:

	CXTPReportControl           *m_pControl;    // Parent Report control
	CXTPReportRecord            *m_pRecord;    // Pointer to the associated record
	CXTPReportBorder            *m_pBorder;     // Item border

	CFont* m_pFontCaption;      // Storage for item caption font.

	COLORREF m_clrText;         // Storage for item text color.
	COLORREF m_clrBackground;   // Storage for item background color.
	BOOL m_bBoldText;           // Is bold text.

	CString m_strFormatString;  // Stores string for extra formatting of item caption.
	CString m_strFormula;       // Stores Formula with simple algorithm to calculate.

	BOOL m_bEditable;           // Stores editable flag.
	CXTPReportHyperlinks* m_pHyperlinks; // Array of the Item's hyperlinks


	int m_nIconIndex;               // ID of the bitmap from the application resources.

	int m_nSortPriority;            // Sort priority
	int m_nGroupPriority;           // Group priority

	CString m_strGroupCaption;      // Caption of the group
	CString m_strCaption;           // Caption of the item
	BOOL m_bFocusable;              // TRUE if item accept focus.

	int  m_checkState;              // TRUE if item checked.
	BOOL m_bHasCheckbox;            // TRUE if item has check box.
	BOOL m_bCanEditCheckboxItem;    // TRUE if an item with a check box will get an edit control (only useful if column is wide)
	BOOL m_bTristateCheckbox;       // TRUE if the check nox is tri state.
	DWORD_PTR m_dwData;             // The 32-bit value associated with the item.
	CString m_strTooltip;           // Tooltip of the item.

	CXTPReportRecordItemEditOptions* m_pEditOptions; // Edit options of the item.

	XTPReportColumnIconAlignment m_Alignment;           // Alignment style for item.

	CXTPReportRecordItemControls* m_pItemControls;      // Record item control list.
	CXTPReportRecordItemControl* m_pFocusedItemControl; // Focused record item control.
	BOOL m_bItemControlUnderMouse;                      // TRUE if an item control is under the mouse pointer.
	CXTPReportRecordItemControlHookWnd* m_pItemControlHookWnd;         // Item control hook window.
	CRect m_rcGlyph; // Coordinates of drawn glyph.

	CXTPMarkupUIElement* m_pMarkupUIElement;            // Markup element.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportRecordItem);

	afx_msg LPDISPATCH OleGetRecord();
	afx_msg LPDISPATCH OleGetEditOptions();
	afx_msg void OleCreateEditOptions();
	afx_msg LPDISPATCH OleGetHyperlinks();

	afx_msg BSTR OleGetFormat();
	afx_msg void OleSetFormat(LPCTSTR pcszFormat);

	afx_msg BSTR OleGetFormula();
	afx_msg void OleSetFormula(LPCTSTR pcszFormula);

	afx_msg BSTR OleGetCaption();
	afx_msg void OleSetCaption(LPCTSTR pcszCaption);

	afx_msg BSTR OleGetGroupCaption();
	afx_msg void OleSetGroupCaption(LPCTSTR pcszGroupCaption);

	afx_msg BSTR OleGetTooltip();
	afx_msg void OleSetTooltip(LPCTSTR pcszTooltip);

	afx_msg LPDISPATCH OleGetItemControls();
	afx_msg LPDISPATCH OleGetMarkupUIElement();

	COleVariant m_oleTag;

//}}AFX_CODEJOCK_PRIVATE
#endif

	friend class CXTPReportRecord;
	friend class CXTPReportControl;
	friend class CXTPReportNavigator;
	friend class CXTPReportIconNavigator;
	friend class CXTPReportRecordItemControls;
};

AFX_INLINE CString CXTPReportRecordItem::GetFormula() const
{
	return m_strFormula;
}

AFX_INLINE void CXTPReportRecordItem::SetFormula(LPCTSTR sFormula)
{
	m_strFormula = sFormula;
}

AFX_INLINE CString CXTPReportRecordItem::GetFormatString() const
{
	return m_strFormatString;
}

AFX_INLINE void CXTPReportRecordItem::SetFormatString(LPCTSTR strFormat)
{
	m_strFormatString = strFormat;
}

AFX_INLINE BOOL CXTPReportRecordItem::SetEditable(BOOL bEditable)
{
	BOOL bOldEditable = m_bEditable;
	m_bEditable = bEditable;
	return bOldEditable;
}

AFX_INLINE BOOL CXTPReportRecordItem::IsPreviewItem() const
{
	return FALSE;
}

AFX_INLINE int CXTPReportRecordItem::GetIconIndex() const
{
	return m_nIconIndex;
}

AFX_INLINE int CXTPReportRecordItem::SetIconIndex(int nIconIndex)
{
	int nOldID = GetIconIndex();
	m_nIconIndex = nIconIndex;
	return nOldID;
}

AFX_INLINE int CXTPReportRecordItem::GetAlignment() const
{
	return m_Alignment;
}

AFX_INLINE int CXTPReportRecordItem::SetAlignment(int nAlignment)
{
	int nOld = m_Alignment;
	m_Alignment = (XTPReportColumnIconAlignment)nAlignment;
	return nOld;
}

AFX_INLINE int CXTPReportRecordItem::GetSortPriority() const
{
	return m_nSortPriority;
}

AFX_INLINE void CXTPReportRecordItem::SetSortPriority(int nSortPriority)
{
	m_nSortPriority = nSortPriority;
}

AFX_INLINE int CXTPReportRecordItem::GetGroupPriority() const
{
	return m_nGroupPriority;
}

AFX_INLINE void CXTPReportRecordItem::SetGroupPriority(int nGroupPriority)
{
	m_nGroupPriority = nGroupPriority;
}

AFX_INLINE void CXTPReportRecordItem::SetGroupCaption(LPCTSTR strCaption)
{
	m_strGroupCaption = strCaption;
}

AFX_INLINE void CXTPReportRecordItem::SetChecked(BOOL bChecked)
{
	if (bChecked)
	{
		m_checkState = xtpReportCheckStateChecked;
	}
	else
	{
		m_checkState = xtpReportCheckStateUnchecked;
	}
}

AFX_INLINE BOOL CXTPReportRecordItem::IsChecked() const
{
	return xtpReportCheckStateChecked == m_checkState;
}

AFX_INLINE void CXTPReportRecordItem::SetCheckedState(int iState)
{
	if (m_bTristateCheckbox)
	{
		m_checkState = XTPReportCheckState(iState);
	}
	else
	{
		m_checkState = iState ? xtpReportCheckStateChecked : xtpReportCheckStateUnchecked;
	}
}

AFX_INLINE int CXTPReportRecordItem::GetCheckedState() const
{
	if (m_bTristateCheckbox)
	{
		return m_checkState;
	}
	else
	{
		return m_checkState ? xtpReportCheckStateChecked : xtpReportCheckStateUnchecked;
	}
}

AFX_INLINE void CXTPReportRecordItem::HasCheckbox(BOOL bHasCheckbox, BOOL bTristate)
{
	m_bHasCheckbox = bHasCheckbox;
	m_bTristateCheckbox = bTristate;
}

AFX_INLINE BOOL CXTPReportRecordItem::GetHasCheckbox() const
{
	return m_bHasCheckbox;
}

AFX_INLINE CXTPReportRecord* CXTPReportRecordItem::GetRecord () const
{
	return m_pRecord;
}

AFX_INLINE void CXTPReportRecordItem::SetFocusable(BOOL bFocusable)
{
	m_bFocusable = bFocusable;
}

AFX_INLINE DWORD_PTR CXTPReportRecordItem::GetItemData() const
{
	return m_dwData;
}

AFX_INLINE void CXTPReportRecordItem::SetItemData(DWORD_PTR dwData)
{
	m_dwData = dwData;
}

AFX_INLINE DWORD CXTPReportRecordItem::GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/)
{
	return DWORD(-1);
}

AFX_INLINE CString CXTPReportRecordItem::GetTooltip() const
{
	return m_strTooltip;
}

AFX_INLINE void CXTPReportRecordItem::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strTooltip = lpszTooltip;
}

AFX_INLINE BOOL CXTPReportRecordItem::IsBold() const
{
	return m_bBoldText;
}

AFX_INLINE CXTPMarkupUIElement* CXTPReportRecordItem::GetMarkupUIElement() const
{
	return m_pMarkupUIElement;
}

AFX_INLINE void CXTPReportRecordItem::OnEditCanceled(XTP_REPORTRECORDITEM_ARGS *pItemArgs)
{
	UNREFERENCED_PARAMETER(pItemArgs);
}

#endif //#if !defined(__XTPREPORTRECORDITEM_H__)
