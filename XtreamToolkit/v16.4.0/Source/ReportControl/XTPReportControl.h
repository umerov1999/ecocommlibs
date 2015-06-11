// XTPReportControl.h: interface for the CXTPReportControl class.
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
#if !defined(__XTPREPORTCONTROL_H__)
#define __XTPREPORTCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPCompatibleDC;
class CXTPPropExchange;
class CXTPToolTipContext;
class CXTPImageManager;
class CXTPMarkupContext;

struct XTP_REPORTRECORDITEM_DRAWARGS;
struct XTP_REPORTRECORDITEM_METRICS;
struct XTP_REPORTRECORDITEM_ARGS;
struct XTP_NM_REPORTTOOLTIPINFO;

class CXTPReportRecords;
class CXTPReportRecordItem;
class CXTPReportColumns;
class CXTPReportGroups;
class CXTPReportColumn;
class CXTPReportRecord;
class CXTPReportRows;
class CXTPReportRow;
class CXTPReportPaintManager;
class CXTPReportColumnOrder;
class CXTPReportInplaceEdit;
class CXTPReportInplaceButtons;
class CXTPReportInplaceList;
class CXTPReportNavigator;
class CXTPReportIconNavigator;
class CXTPReportRecordItemConstraint;
class CXTPReportDataManager;
class CXTPReportHyperlink;
class CXTPReportRecordItemRange;
class CXTPReportSelectedRows;
class CXTPReportBehavior;
class CXTPReportBehaviorRowMouseButton;
class CXTPReportTip;
class CXTPReportSection;
class CXTPReportSections;
class CXTPReportBorder;
class CXTPReportHitTestInfo;


//===========================================================================
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from Item
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window
// Example:
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_GETITEMMETRICS, XTP_ID_REPORT_CONTROL, OnReportGetItemMetrics)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportGetItemMetrics(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTITEMMETRICS* pItemNotify = (XTP_NM_REPORTITEMMETRICS*)pNotifyStruct;
//
//     ASSERT(pItemNotify->pDrawArgs);
//     ASSERT(pItemNotify->pDrawArgs->pControl);
//     ASSERT(pItemNotify->pDrawArgs->pRow);
//
//     //     pItemNotify->pDrawArgs->pColumn   - may be NULL (for a group row)
//     //     pItemNotify->pDrawArgs->pItem     - may be NULL (for a group row)
//
//     ASSERT(pItemNotify->pItemMetrics);
//
//     // TODO: customize members of pItemNotify->pItemMetrics.
// }
// </code>
// See Also:
//     CXTPReportControl, CXTPReportControl::GetItemMetrics(),
//     XTP_NM_REPORT_GETITEMMETRICS
//===========================================================================
struct XTP_NM_REPORTITEMMETRICS
{
	NMHDR hdr;                                      // Standard structure, containing information about a notification message.
	XTP_REPORTRECORDITEM_DRAWARGS*  pDrawArgs;      // Pointer to XTP_REPORTRECORDITEM_DRAWARGS structure.
	XTP_REPORTRECORDITEM_METRICS*   pItemMetrics;   // Pointer to XTP_REPORTRECORDITEM_METRICS structure to be filled.
};

//===========================================================================
// Summary:
//     This structure is sent to main window in a WM_NOTIFY message from from report control
//     to determine Records was dragged or dropped
// Example:
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_BEGINDRAG, XTP_ID_REPORT_CONTROL, OnReportBeginDrag)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportBeginDrag(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTDRAGDROP* pItemNotify = (XTP_NM_REPORTDRAGDROP*)pNotifyStruct;
//
//     ASSERT(pItemNotify->pRecords);
// }
// </code>
// See Also:
//     XTP_NM_REPORT_DROP, XTP_NM_REPORT_BEGINDRAG, CXTPReportControl::EnableDragDrop
//===========================================================================
struct XTP_NM_REPORTDRAGDROP
{
	NMHDR hdr;                          // Standard structure, containing information about a notification message.
	CXTPReportRecords* pRecords;        // Records will be dragged/dropped
	CXTPReportRecord*  pTargetRecord;   // Target record (if any)
	int nTargetRow;                     // Index of target row
	BOOL bAbove;                        // Where to insert (relative to the target record)
	DROPEFFECT dropEffect;              // The DropEffect flags.
	CPoint pt;                          // The current location of the cursor in client coordinates.
	int nState;                         // The transition state (0 - enter, 1 - leave, 2 - over).
	DWORD dwKeyState;                   // The key state.
	COleDataSource* pDataSource;        // Data Source - issue 22675
	COleDataObject* pDataObject;        // Data Object - Needed for drag over and drop
	BOOL bReturnValue;                  // TRUE to accept dropEffect, FALSE to not accept dropEffect
};

//===========================================================================
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message before
//     processing OnKeyDown event.
// See Also:
//     CXTPReportControl::OnPreviewKeyDown, XTP_NM_REPORT_PREVIEWKEYDOWN,
//     CXTPReportControl::OnKeyDown, CWnd::OnKeyDown.
//===========================================================================
struct XTP_NM_REPORTPREVIEWKEYDOWN
{
	NMHDR hdr;      // Standard structure, containing information about a notification message.

	// OnKeyDown parameters
	UINT nChar;     // [in/out] Specifies the virtual key code of the given key.
	UINT nRepCnt;   // [in] Repeat count.
	UINT nFlags;    // [in] Specifies the scan code, key-transition code, previous key state, and context code.

	BOOL bCancel;   // [out] TRUE to cancel processing key, FALSE to continue.
};

//===========================================================================
// Summary:
//     Enumeration of operational mouse modes.
// Remarks:
//     ReportControl has several
//     Mouse states that handled by control. This enumeration helps to
//     clearly identify each of these
//         - Sends Notifications:
//         - Sends Messages:
// See Also: CXTPReportControl::SetMouseMode, CXTPReportControl::GetMouseMode
//
// <KEYWORDS xtpReportMouseNothing, xtpReportMouseOverColumnDivide, xtpReportMousePrepareDragColumn, xtpReportMouseDraggingColumn>
//===========================================================================
enum XTPReportMouseMode
{
	xtpReportMouseNothing,            // User is just watching to the list.
	xtpReportMouseOverColumnDivide,   // User watching, but mouse is under column divider.
	xtpReportMousePrepareDragColumn,  // User holds mouse button pressed on the column header.
	xtpReportMouseDraggingColumn,     // User is dragging column header.
	xtpReportMouseOverRowDivide,      // User watching, but mouse is under row divider.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of drag and drop flags
// Remarks:
//     Call CXTPReportControl::EnableDragDrop to allow drag/drop operations
// Example:
//     <code>m_wndReport.EnableDragDrop("MyApplication", xtpReportAllowDrop | xtpReportAllowDrag);</code>
// See Also: CXTPReportControl::EnableDragDrop
//-----------------------------------------------------------------------
enum  XTPReportDragDrop
{
	xtpReportAllowDrop     = 1,         // Allow Drop records to report
	xtpReportAllowDragCopy = 2,         // Allow copy records from report
	xtpReportAllowDragMove = 4,         // Allow move records from report
	xtpReportAllowDrag     = 6,         // Allow copy and move records from report
	xtpReportDontDropAsText = 8        // Do not drag record as plain text
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration describes the drop marker.
//-----------------------------------------------------------------------
enum XTPReportDropMarker
{
	xtpReportDropBetween = 1,   //Drop the row between two rows.
	xtpReportDropSelect = 2,    //Drop the selected rows.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of watermark alignment flags
// Remarks:
//     Call CXTPReportControl::SetWatermarkAlignment to modify watermark alignment
// Example:
//     <code>m_wndReport.SetWatermarkAlignment(xtpWatermarkCenter | xtpWatermarkBottom);</code>
// See Also: CXTPReportControl::SetWatermarkAlignment, GetWatermarkAlignment
//-----------------------------------------------------------------------
enum  XTPReportWatermarkAlignment
{
	xtpReportWatermarkUnknown      = 0,    // Unknown (empty) value.

	xtpReportWatermarkLeft         = 0x0001, // Horizontal alignment: left side of report control client rect.
	xtpReportWatermarkCenter       = 0x0002, // Horizontal alignment: center of report control client rect.
	xtpReportWatermarkRight        = 0x0004, // Horizontal alignment: right side of report control client rect.
	xtpReportWatermarkHmask        = 0x000F, // A mask for horizontal alignment flags.

	xtpReportWatermarkTop          = 0x0010, // Vertical alignment: top side of report control client rect.
	xtpReportWatermarkVCenter      = 0x0020, // Vertical alignment: center of report control client rect.
	xtpReportWatermarkBottom       = 0x0040, // Vertical alignment: bottom side of report control client rect.
	xtpReportWatermarkVmask        = 0x00F0, // A mask for vertical alignment flags.

	xtpReportWatermarkStretch       = 0x0100, // Stretch watermark to entire report control client rect.
	xtpReportWatermarkEnlargeOnly   = 0x0200, // Watermark can be enlarged only, shrinking is disabled.
	xtpReportWatermarkShrinkOnly    = 0x0400, // Watermark can be shirnked only, enlarging is disabled.
	xtpReportWatermarkPreserveRatio = 0x0800, // Watermark aspect ratio is preserved.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of GetElementRect flags
// Remarks:
//     Call CXTPReportControl::GetElementRect to get report element rectangle
// Example:
//     <code>m_wndReport.GetElementRect(xtpReportRectGroupByArea, rc);</code>
// See Also: CXTPReportControl::GetElementRect
//-----------------------------------------------------------------------
enum  XTPReportElementRect
{
	xtpReportElementRectReportArea = 0,           // report area rectangle
	xtpReportElementRectGroupByArea,              // report area rectangle
	xtpReportElementRectHeaderArea,               // report area rectangle
	xtpReportElementRectFooterArea,               // report area rectangle
	xtpReportElementRectHeaderRecordsArea,        // report area rectangle
	xtpReportElementRectFooterRecordsArea,        // report area rectangle
	xtpReportElementRectHeaderRecordsDividerArea, // report area rectangle
	xtpReportElementRectFooterRecordsDividerArea, // report area rectangle
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of options for SetRecordsTreeFilterMode method
// Remarks:
//     Call CXTPReportControl::SetRecordsTreeFilterMode to set options for Filtering tree
// Example:
//     <code>m_wndReport.SetRecordsTreeFilterMode(xtpReportFilterTreeSimple);</code>
// See Also: CXTPReportControl::SetRecordsTreeFilterMode
//-----------------------------------------------------------------------
enum XTPReportFilterMode
{
	xtpReportFilterTreeSimple               = 0,
	xtpReportFilterTreeByParentAndChildren  = 1,
	xtpReportFilterTreeByEndChildrenOnly    = 2
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of scroll update modes.
//-----------------------------------------------------------------------
enum XTPReportScrollUpdateMode
{
	xtpReportScrollUpdateModeImmediate, // Immediately updates the scroll position
	xtpReportScrollUpdateModeDeferred   // Updates the scroll position deferred
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of scroll modes.
//-----------------------------------------------------------------------
enum XTPReportScrollMode
{
	xtpReportScrollModeNone       = 0,   // No scrolling
	xtpReportScrollModeBlockCount = 1,   // Scrolls an entire row or column
	xtpReportScrollModeBlockSize  = 3,   // Scrolls an entire row or column
	xtpReportScrollModeSmooth     = 2,   // Smooth scrolling

	// Deprecated
	xtpReportScrollModeBlock      = xtpReportScrollModeBlockCount,
};


//-----------------------------------------------------------------------
// Summary:
//     Stores the mouse and key states.
//-----------------------------------------------------------------------
struct XTPReportInputState
{
	XTPReportInputState()
		: ptMouse    (-1, -1)
		, bKeyControl(FALSE)
		, bKeyShift  (FALSE)
		, nRow       (-1)
		, bSelected  (FALSE)
	{
		// Nothing
	}

	CPoint ptMouse;       // Mouse position
	BOOL   bKeyControl;   // Control key state
	BOOL   bKeyShift;     // Shift key state
	int    nRow;          // Clicked row index
	BOOL   bSelected;     // Clicked row selection state
};

//===========================================================================
// Summary:
//     The CXTPReportControl class provides an implementation of
//     the Report control.
//
// Remarks:
//     A "report control" is a window that displays a hierarchical list
//     of items, such as emails in the inbox. Each item is called a CXTPReportRow
//     and consists of its properties and corresponding CXTPReportRecord,
//     which contains all the corresponding data (mostly text).
//     Each Row item (as well as Record) can have a list of sub-items
//     associated with it. By clicking a Row item, the user can expand and
//     collapse the associated list of sub-items.
//
//     The CXTPReportRecords collection holds all the CXTPReportRecord objects
//     that are assigned to the Report control. It could be accessible via
//     GetRecords() method. The records in this collection
//     are referred to as the root records. Any record that is subsequently
//     added to a root record is referred to as a child record. Because each
//     CXTPReportRecord can contain a collection of other CXTPReportRecord
//     objects, you might find it difficult to determine your location in the
//     tree structure when you iterate through the collection.
//
//     Record nodes can be expanded to display the next level of child records.
//     The user can expand the CXTPReportRecord by clicking the plus-sign (+)
//     button, if one is displayed, or you can expand the CXTPReportRecord by
//     calling the CXTPReportRecord::SetExpanded method.
//     To expand all the child records, call the ExpandAll method.
//     You can collapse the child CXTPReportRecord level by calling the
//     CXTPReportRecord::SetExpanded(FALSE) method, or the user can press
//     the minus-sign (-) button, if one is displayed. You can also call
//     CollapseAll method to collapse all child records.
//
//     Each record contains an array of record items which are implemented
//     with CXTPReportRecordItem and its descendants. You can create your own
//     types of items simply by inheritance from the base record item class.
//
//     Each record item has an association with corresponding CXTPReportColumn
//     item. The item will be shown below the corresponding column header
//     depending on its position in report control columns array. If a column
//     has not an associated item in the record, there will be an empty item
//     shown in the corresponding cell.
//
//     Columns array is represented by CXTPReportColumns collection and could
//     be accessed via GetColumns() method.
//
//     As a finalization of adding data to the report control, which means
//     adding columns and records, Populate() method should be called. It
//     performs population of control rows with data - creates a rows tree if
//     necessary, rebuilds groups if grouping if enabled, and sorts rows
//     on a specified manner. See Also an example below.
//
//     Handling notification messages sent by the control to the parent
//     window is allowed with ON_NOTIFY handler. The control is using
//     SendMessageToParent function to send notifications. See below for
//     the example of how messages could be handled in a parent window:
//
// <code>
// ON_NOTIFY(NM_CLICK, ID_REPORT_CONTROL, OnReportItemClick)
// ON_NOTIFY(NM_RCLICK, ID_REPORT_CONTROL, OnReportItemRClick)
// ON_NOTIFY(NM_DBLCLK, ID_REPORT_CONTROL, OnReportItemDblClick)
// ON_NOTIFY(XTP_NM_SHOWFIELDCHOOSER, ID_REPORT_CONTROL, OnShowFieldChooser)
// ON_NOTIFY(XTP_NM_HEADER_RCLICK, ID_REPORT_CONTROL, OnReportColumnRClick)
// ON_NOTIFY(NM_KEYDOWN, ID_REPORT_CONTROL, OnReportKeyDown)
// </code>
//
//     You can also change the appearance of the CXTPReportControl control
//     by setting some of its display and style properties.
//
//     Also Report control has an ability to store and restore its
//     settings, which includes all columns with their settings, and some
//     required control's settings. It is implemented via standard MFC and XTP
//     serialization and available with the member functions
//     SerializeState(CArchive& ar), DoPropExchange(CXTPPropExchange* pPX);
//
//     Report control supports Copy/Paste clipboard operations.
//     See methods: CanCut(), CanCopy(), CanPaste(), Cut(), Copy(), Paste().
//     There are 2 clipboard formats are supported:
//          Binary  - contains all record(s) data;
//          Text    - contains visible columns texts.
//
//     To support binary format the XTP serialization is used -
//     DoPropExchange() methods are implemented for CXTPReportRecord,
//     CXTPReportRecordItem and derived CXTPReportRecordItemXXX classes.
//     Also some part of standard MFC serialization is used
//     (see DECLARE_SERIAL macro) to automatically create classes when
//     loading from the data source.
//
//     If you are creating custom records and records items classes you have
//     to use DECLARE_SERIAL macro and may need to override DoPropExchange()
//     methods to serialize custom data as well as standard records data.
//
//     The storing records way is simple: CXTPReportRecord (or derived class)
//     is stored first, then record items (CXTPReportRecordItemXXX classes)
//     are stored one by one.
//     The class information, which allow to create object instances when
//     loading, is stored for all classes. See CArchive::WriteClass(),
//     CArchive::ReadClass() and other CArchive members for more details
//     about this idea.
//
//     When report control loads records from the data source:
//     The record class is created automatically (using stored
//     class information).
//     Then items collection cleared and record items are created
//     automatically and added to items collection.
//     For example see ReportSample project: CMessageRecord class.
//
//     We support text format with '\t' dividers for record items and
//     "\r\n" dividers for records (simple tab-separated text).
//     Such format is also supported by Excel and some other applications.
//
//     There are few methods and corresponding notifications which allow to
//     customize copy/paste operations:
//      OnBeforeCopyToText(); OnBeforePasteFromText(); OnBeforePaste();
//
// See Also:
//     CXTPReportView, CXTPReportHeader, CXTPReportRow, CXTPReportRecord,
//     CXTPReportColumn,
//     CXTPReportRecords, CXTPReportRows, CXTPReportColumns,
//     CXTPReportSubListControl, CXTPReportFilterEditControl
//===========================================================================
class _XTP_EXT_CLASS CXTPReportControl : public CWnd, public CXTPAccessible
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNCREATE(CXTPReportControl)
	DECLARE_INTERFACE_MAP()

	friend class CXTPReportSubListControl;
	friend class CXTPReportRow;
	friend class CXTPReportGroupRow;
	friend class CXTPReportHeader;
	friend class CXTPReportNavigator;
	friend class CXTPReportInplaceEdit;
	friend class CXTPReportHeaderDragWnd;
	friend class CXTPReportInplaceList;
	friend class CXTPReportColumn;
	friend class CXTPReportView;
	class CReportDropTarget;
	//}}AFX_CODEJOCK_PRIVATE

public:

	//===========================================================================
	// Summary:
	//     Internal report update helper.
	//===========================================================================
	class _XTP_EXT_CLASS CUpdateContext
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPReportControlUpdateContext object.
		// Parameters:
		//     pControl - Pointer to a Report Control object.
		//-----------------------------------------------------------------------
		CUpdateContext(CXTPReportControl* pControl)
		{
			m_pControl = pControl;
			pControl->BeginUpdate();
		}

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CXTPReportControlUpdateContext object, handles cleanup and de-allocation.
		//-------------------------------------------------------------------------
		~CUpdateContext()
		{
			m_pControl->EndUpdate();
		}
	protected:
		CXTPReportControl* m_pControl;          // Updated report control pointer
	};

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to use custom heap feature.
	//
	// Remarks:
	//     If it is enabled, report data will be allocated in custom (separate)
	//     heap instead of standard application heap. This optimize memory
	//     using (fragmentation) for big amount of data.
	//     For custom heap used for classes derived from CXTPHeapObjectT template,
	//     like CXTPReportRecord, CXTPReportRecordItem, CXTPReportRow and so others.
	//     This template just overrides operators new and delete.
	//     <p/>It must be called on initialization before any allocations of classes
	//     which use custom heap. OnInitInstance is a fine place for this.
	// Returns:
	//     TRUE if custom heap feature is enabled for all report allocators,
	//     FALSE otherwise.
	//-------------------------------------------------------------------------
	static BOOL AFX_CDECL UseReportCustomHeap();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to enable batch allocation feature for report rows.
	//
	// Remarks:
	//      Batch allocation means that memory allocated not for one object only,
	//      but for many objects at one time (for 1024 objects by default).
	//      Next allocations take memory from this big block. New blocks allocated
	//      when necessary. This increase performance and reduce heap fragmentation.
	//      Batch allocation mechanism responsible for allocation/deallocation
	//      blocks of memory from heap and internally organize free/busy lists of
	//      memory pieces. When object deleted, its memory stored in free list and
	//      used for new objects.
	//      When all memory pieces from block free, it may be deallocated from
	//      heap automatically (this depends on options in _TBatchAllocData)
	//      or by FreeExtraData call,
	//      <p/>It must be called on initialization before any allocations of classes
	//      which use batch allocation. OnInitInstance is a fine place for this.
	// Returns:
	//     TRUE if batch allocation feature is enabled for report rows,
	//     FALSE otherwise.
	//-------------------------------------------------------------------------
	static BOOL AFX_CDECL UseRowBatchAllocation();

	//-----------------------------------------------------------------------
	// Summary:
	//      This member function when rows batch allocation enabled to check
	//      all allocated blocks and deallocate which are completely free.
	// See Also:
	//      UseRowBatchAllocation, CXTPBatchAllocObjT
	//-----------------------------------------------------------------------
	static void AFX_CDECL FreeRowBatchExtraData();

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportControl object.
	// Remarks:
	//     You construct a CXTPReportControl object in two steps.
	//     First, call the constructor CXTPReportControl and then call
	//     Create method, which initializes the window.
	//
	// Example:
	// <code>
	// // Declare a local CXTPReportControl object.
	// CXTPReportControl myReport;
	//
	// // Declare a dynamic CXTPReportControl object.
	// CXTPReportControl* pMyReport = new CXTPReportControl();
	//
	// if (!myReport.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WM_VSCROLL, CRect(0, 0, 0, 0), this, ID_REPORT_CONTROL))
	// {
	//     TRACE(_T("Failed to create view window\n"));
	// }
	// </code>
	// See Also: Create
	//-----------------------------------------------------------------------
	CXTPReportControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportControl object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create a report control.
	// Parameters:
	//     dwStyle     - Specifies the window style attributes.
	//                   It could be a combination of standard window styles.
	//     rect        - The size and position of the window, in client
	//                   coordinates of pParentWnd.
	//     pParentWnd  - Specifies the report control parent window.
	//     nID         - Specifies the report control identifier.
	//     pContext    - The create context of the window.
	// Remarks:
	//     You construct a CXTPReportControl object in two steps.
	//     First, call the constructor CXTPReportControl and then call
	//     Create method, which initializes the window.
	// Example:
	//     See the example for CXTPReportControl::CXTPReportControl
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// See Also: CXTPReportControl::CXTPReportControl
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     (Re)Stores control configuration to/from the provided archive stream.
	// Parameters:
	//     ar - Archive stream for serializing.
	//-----------------------------------------------------------------------
	virtual void SerializeState(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes configuration to/from the provided data source.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new column to the end of the columns array.
	// Parameters:
	//     pColumn - Pointer to the specified column class object.
	// Remarks:
	//     Call this member function if you want to add a new column
	//     to a report control. It will be added to the end of the columns array.
	// Example:
	// <code>
	// // this function adds a column with "Subject" caption and 250 pixels initial width
	// void AddSubjectColumn(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->AddColumn(new CXTPReportColumn(1, _T("Subject"), 250));
	// }
	// </code>
	// See Also: CXTPReportColumn overview
	//-----------------------------------------------------------------------
	CXTPReportColumn* AddColumn(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds record to records collection.
	// Parameters:
	//     pRecord - Record data with items.
	// Remarks:
	//     Call this member function if you want to add a data record to the
	//     report control's internal storage.
	// Example:
	// <code>
	// // this function adds 2 empty records to a report control
	// void Add2Empties(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->AddRecord(new CXTPReportRecord());
	//     pReportCtrl->AddRecord(new CXTPReportRecord());
	// }
	// </code>
	// Returns:
	//     Pointer to the recently added record object.
	// See Also: CXTPReportRecord overview, GetRecords
	//-----------------------------------------------------------------------
	virtual CXTPReportRecord* AddRecord(CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds record to records collection and associates a row with it.
	// Parameters:
	//     pRecord - Record data with items.
	//     pParentRecord - Parent record.
	//     nRowChildIndex    - child index a row to be inserted at for case when index does not defined by other conditions (or -1).
	//     nRecordChildIndex - child index a record to be inserted at (or -1).
	// Remarks:
	//     Call this member function if you want to add a data record to the
	//     report control's internal storage and associate a row with it.
	// Example:
	// <code>
	// // this function adds 2 empty records to a report control
	// void Add2Empties(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->AddRecordEx(new CXTPReportRecord());
	//     pReportCtrl->AddRecordEx(new CXTPReportRecord());
	// }
	// </code>
	// See Also: CXTPReportRecord overview, GetRecords, RemoveRecordEx, RemoveRowEx
	//           UpdateRecord
	//-----------------------------------------------------------------------
	virtual void AddRecordEx(CXTPReportRecord* pRecord, CXTPReportRecord* pParentRecord = NULL,
					int nRowChildIndex = -1, int nRecordChildIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes record with data and associated row. It also remove all
	//     children records and their rows.
	// Parameters:
	//     pRecord       - Pointer to a record object.
	//     bAdjustLayout - If TRUE AdjustLayout will be called.
	//     bRemoveFromParent - If TRUE the record is to be removed from the parent.
	// Remarks:
	//     Call this member function if you want to remove record on the fly,
	//     without Populate call.
	// Returns:
	//     TRUE if operation succeeded, FALSE otherwise.
	// See Also: RemoveRowEx, AddRecordEx, CXTPReportRecords::RemoveRecord,
	//           UpdateRecord
	//-----------------------------------------------------------------------
	virtual BOOL RemoveRecordEx(CXTPReportRecord* pRecord, BOOL bAdjustLayout = TRUE, BOOL bRemoveFromParent = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes row and associated record. It also remove all
	//     children records and rows.
	// Parameters:
	//     pRow          - Pointer to a row object.
	//     bAdjustLayout - If TRUE AdjustLayout will be called.
	// Remarks:
	//     Call this member function if you want to remove row and record on the fly,
	//     without Populate call.
	// Returns:
	//     TRUE if operation succeeded, FALSE otherwise.
	// See Also: RemoveRecordEx, AddRecordEx, CXTPReportRecords::RemoveRecord,
	//           UpdateRecord
	//-----------------------------------------------------------------------
	virtual BOOL RemoveRowEx(CXTPReportRow* pRow, BOOL bAdjustLayout = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates record.
	// Parameters:
	//     pRecord - Record data with items.
	//     bUpdateChildren - If TRUE the children will be updated as well.
	// Remarks:
	//     Call this member function if you modified a record and want it
	//     to be updated accordingly to the current grouping and sorting.
	// See Also: CXTPReportRecord overview, GetRecords
	//-----------------------------------------------------------------------
	virtual void UpdateRecord(CXTPReportRecord* pRecord, BOOL bUpdateChildren);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prevents the control from redrawing until the EndUpdate
	//     method is called.
	// Remarks:
	//     If you want to add items one at a time using the AddRecord method,
	//     or to make some another operations in a single sequence,
	//     you can use the BeginUpdate method to prevent the control
	//     from repainting the CXTPReportControl each time an item is added.
	//     Once you have completed the task of adding items to the control,
	//     call the EndUpdate method to enable the CXTPReportControl to repaint.
	//     This way of adding items can prevent flickered drawing of
	//     the CXTPReportControl when a large number of items are being
	//     added to the control.
	// Example:
	// <code>
	// // This function collapses all rows for the specified report control
	// void CollapseAll(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->BeginUpdate();
	//     for (int i = pReportCtrl->GetRows()->GetCount() - 1; i >= 0; i --)
	//         pReportCtrl->GetRows()->GetAt(i)->SetExpanded(FALSE);
	//
	//     pReportCtrl->EndUpdate();
	// }
	// </code>
	// See Also: EndUpdate, RedrawControl, AddRecord
	//-----------------------------------------------------------------------
	void BeginUpdate();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method deletes all rows and records from Report Control.
	// Parameters:
	//     bUpdateControl - Set TRUE to redraw control, otherwise FALSE.
	//-----------------------------------------------------------------------
	void ResetContent(BOOL bUpdateControl = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resumes drawing of the report control after drawing is
	//     suspended by the BeginUpdate method.
	// Remarks:
	//     If you want to add items one at a time using the AddRecord method,
	//     or to make some other operations in a single sequence,
	//     you can use the BeginUpdate method to prevent the control
	//     from repainting the CXTPReportControl each time an item is added.
	//     Once you have completed the task of adding items to the control,
	//     call the EndUpdate method to enable the CXTPReportControl to repaint.
	//     This way of adding items can prevent flickered drawing of
	//     the CXTPReportControl when a large number of items are being
	//     added to the control.
	// Example:  See example for CXTPReportControl::BeginUpdate method.
	// See Also: BeginUpdate, RedrawControl
	//-----------------------------------------------------------------------
	void EndUpdate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE if control in Update state.
	//-----------------------------------------------------------------------
	int GetLockUpdateCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Initiates report control redrawing.
	// Remarks:
	//     Call this member function if you want to initialize the
	//     report control redrawing. The control will be redrawn taking
	//     into account its latest state.
	// See Also: BeginUpdate, EndUpdate
	//-----------------------------------------------------------------------
	void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get tooltip context pointer.
	// Returns:
	//     A pointer to the tooltip context object.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable built in drag and drop operations
	// Parameters:
	//     lpszClipboardFormat - Name of clipboard format to be used for Report Control
	//     dwFlags - Combination of XTPReportDragDrop flags, can be one or more of following:
	//        <b>xtpReportAllowDrop</b> - Allow Drop records to report
	//        <b>xtpReportAllowDragCopy</b> - Allow copy records from report
	//        <b>xtpReportAllowDragMove</b> - Allow move records from report
	//        <b>xtpReportAllowDrag</b> - Allow copy and move records from report
	// Returns:
	//     Clipboard format that will be used with Report Control
	// See Also: XTPReportDragDrop
	//-----------------------------------------------------------------------
	CLIPFORMAT EnableDragDrop(LPCTSTR lpszClipboardFormat, DWORD dwFlags, DWORD dwDropMarkerFlags = xtpReportDropBetween);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the drop marker flags.
	// Parameter:
	//     dwDropMarkerFlags - The drop marker flags.
	//-----------------------------------------------------------------------
	void SetDropMarkerFlags(DWORD dwDropMarkerFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the drop marker flags.
	// Returns:
	//     A DWORD value specifying the drop marker flags.
	//-----------------------------------------------------------------------
	DWORD GetDropMarkerFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Set or clear grid drawing style.
	// Parameters:
	//     orientation - xtpReportOrientationVertical for changing vertical grid style,
	//                   xtpReportOrientationHorizontal for changing horizontal grid style.
	//     gridStyle - New grid style. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     Call this member function if you want to change a style
	//     of report grid lines.
	//
	//     Possible grid line styles are the following:
	//     * <b>xtpReportGridNoLines</b>  Empty line
	//     * <b>xtpReportGridSmallDots</b> Line is drawn with small dots
	//     * <b>xtpReportGridLargeDots</b> Line is drawn with large dots
	//     * <b>xtpReportGridDashes</b> Line is drawn with dashes
	//     * <b>xtpReportGridSolid</b> Draws solid line
	//
	// See Also: XTPReportGridStyle overview, GetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	void SetGridStyle(XTPReportOrientation orientation, XTPReportGridStyle gridStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current grid drawing mode.
	// Parameters:
	//     bVertical - xtpReportOrientationVertical for vertical grid style,
	//                 xtpReportOrientationHorizontal for horizontal grid style.
	// Remarks:
	//     Call this member function if you want to retrieve current
	//     grid lines drawing style for the report control.
	// Returns:
	//     Current grid drawing style.
	// See Also: XTPReportGridStyle overview, SetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	XTPReportGridStyle GetGridStyle(XTPReportOrientation orientation) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Change color of GridLines.
	// Parameters:
	//     clrGridLine - New Grid Lines color.
	// Remarks:
	//     Call this member function if you want to change a color of
	//     report control grid lines.
	// Returns:
	//     Old Grid Lines color.
	// See Also: SetGridStyle, GetGridStyle
	//-----------------------------------------------------------------------
	COLORREF SetGridColor(COLORREF clrGridLine);

	//-----------------------------------------------------------------------
	// Summary:
	//     Interface for accessing list columns.
	// Remarks:
	//     Use this member function to retrieve an access to the collection
	//     of report columns.
	// Returns:
	//     The report Columns collection.
	// Example:
	// <code>
	// // this function adds a column with "Subject" caption and 250 pixels initial width
	// void AddSubjectColumn(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->GetColumns()->Add(new CXTPReportColumn(1, _T("Subject"), 250));
	// }
	// </code>
	// See Also: CXTPReportColumns overview
	//-----------------------------------------------------------------------
	CXTPReportColumns* GetColumns() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the currently used control's Paint Manager.
	// Remarks:
	//     Call this member function to get the paint manager object used
	//     for drawing a report control window.
	// Returns:
	//     Pointer to the paint manager object.
	// See Also: CXTPReportPaintManager overview
	//-----------------------------------------------------------------------
	CXTPReportPaintManager* GetPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set custom paint manager.
	// Parameters:
	//     pPaintManager - A pointer to the custom paint manager.
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPReportPaintManager* pPaintManager);


	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the control's Navigator.
	// Remarks:
	//     Call this member function to get the navigator object used
	//     for cell navigation.
	// Returns:
	//     Pointer to the navigator object.
	// See Also: CXTPReportNavigator overview
	//-----------------------------------------------------------------------
	CXTPReportNavigator* GetNavigator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs control population, creating view from the data.
	// Remarks:
	//     Call this member function to populate control's Rows collection
	//     with the data containing in Records collection.
	//     It automatically creates Tree View references if necessary
	//     (for example, in Grouping mode).
	//     , it is the main function which should be called for
	//     (re)populating all data changes that you have made into the
	//     Records collection.
	// See Also: CXTPReportPaintManager overview
	//-----------------------------------------------------------------------
	virtual void Populate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs control's header rows population, creating view from the data.
	// Remarks:
	//     Call this member function to populate control's header rows collection
	//     with the data containing in HeaderRecords collection. Useful when main
	//     rows are controlled through AddRecordEx/RemoveRecordEx/UpdateRecord
	//     methods.
	// See Also: Populate, PopulateFooterRows, AddRecordEx, RemoveRecordEx, UpdateRecord
	//-----------------------------------------------------------------------
	virtual void PopulateHeaderRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs control's footer rows population, creating view from the data.
	// Remarks:
	//     Call this member function to populate control's footer rows collection
	//     with the data containing in FooterRecords collection. Useful when main
	//     rows are controlled through AddRecordEx/RemoveRecordEx/UpdateRecord
	//     methods.
	// See Also: Populate, PopulateHeaderRows, AddRecordEx, RemoveRecordEx, UpdateRecord
	//-----------------------------------------------------------------------
	virtual void PopulateFooterRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a report control row is at least partially visible.
	// Parameters:
	//     pRow - A pointer to the row that is to be visible.
	// Remarks:
	//     Ensures that a report row item is at least partially visible.
	//     The list view control is scrolled if necessary.
	// See Also: MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPReportRow *pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a report control column is at least partially visible.
	// Parameters:
	//     pCheckColumn - A pointer to the column that is to be visible.
	// Remarks:
	//     Ensures that a report column item is at least partially visible.
	//     The list view control is scrolled if necessary.
	// See Also: MoveRight, MoveLeft, MoveToColumn, MoveFirstColumn, MoveLastColumn
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPReportColumn* pCheckColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the report part at a specified position.
	// Parameters:
	//     pt    - A point to test.
	//     pInfo - Pointer to a CXTPReportHitTestInfo that will hold the
	//             returned information.
	// Returns:
	//     TRUE if the method was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, CXTPReportHitTestInfo *pInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the report row at a specified position.
	// Parameters:
	//     pt - A point to test.
	// Returns:
	//     The row at the position specified by pt, otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPReportRow* HitTest(CPoint pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the selected rows.
	// Remarks:
	//     Use this member function to retrieve an access to the collection
	//     of currently selected report rows.
	// Returns:
	//     The selected rows collection.
	// See Also: CXTPReportSelectedRows overview.
	//-----------------------------------------------------------------------
	CXTPReportSelectedRows* GetSelectedRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the row that currently has focus
	//     in the report control's view.
	// Returns:
	//     Returns pointer to the focused row, or NULL otherwise.
	//-----------------------------------------------------------------------
	CXTPReportRow* GetFocusedRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Makes the provided row as currently focused.
	// Parameters:
	//     pRow             - The row to set as focused to.
	//     bControlKey - TRUE if select new focused row.
	//                        FALSE otherwise.
	// Returns:
	//     TRUE if specified row has been focused, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL SetFocusedRow(CXTPReportRow* pRow, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Makes the provided row as currently focused.
	// Parameters:
	//     pRow             - The row to set as focused to.
	//     bControlKey - TRUE if select new focused row.
	//     bShiftKey     - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	// Returns:
	//     TRUE if specified row has been focused, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL SetFocusedRow(CXTPReportRow* pRow, BOOL bShiftKey, BOOL bControlKey);

	//-----------------------------------------------------------------------
	// Summary:
	//     Makes the provided row as currently focused.
	// Parameters:
	//.....bEnsureVisible   - TRUE if the row will be made visible
	//     pRow             - The row to set as focused to.
	//     bControlKey      - TRUE if select new focused row.
	//     bShiftKey        - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	// Returns:
	//     TRUE if specified row has been focused, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL SetFocusedRow(BOOL bEnsureVisible, CXTPReportRow* pRow, BOOL bShiftKey, BOOL bControlKey);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns rows from the visible report area.
	// Parameters:
	//     nStartRow - Row index to start calculating from.
	//     bMoveDown - Rows moving direction.
	// Remarks:
	//     Call this member function if you want to calculate the amount
	//     of currently visible rows between nStartRow and the top/bottom
	//     of the current view.
	// Returns:
	//     Visible row index starting from.
	// See Also: MovePageDown, MovePageUp
	//-----------------------------------------------------------------------
	int GetReportAreaRows(int nStartRow, BOOL bMoveDown);

	//-----------------------------------------------------------------------
	// Summary:
	//     Force provided row to the top.
	// Parameters:
	//     nIndex - An index of the row to force.
	// Remarks:
	//     The system scrolls the report control view until either the
	//     item specified by nIndex appears at the top of the view
	//     the maximum scroll range has been reached.
	// See Also: GetReportAreaRows, EnsureVisible
	//-----------------------------------------------------------------------
	void SetTopRow(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve top row index.
	// Returns:
	//     Top row index.
	// See Also: SetTopRow
	//-----------------------------------------------------------------------
	int GetTopRowIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set horizontal scroll position.
	// Parameters:
	//     nOffset - Horizontal scroll position.
	// Remarks:
	//     This method takes effect only if auto column sizing is disable
	// See Also: CXTPReportHeader::SetAutoColumnSizing
	//-----------------------------------------------------------------------
	void SetScrollOffsetH(int nOffset);

		//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set vertical scroll position.
	// Parameters:
	//     nOffset - Vertical scroll position.
	//-----------------------------------------------------------------------
	void SetScrollOffsetV(int nOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set right-to-left mode.
	// Parameters:
	//     bRightToLeft - TRUE to set right-to-left reading-order properties.
	//-----------------------------------------------------------------------
	void SetLayoutRTL(BOOL bRightToLeft);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get right-to-left mode.
	// Returns:
	//     TRUE if layout is RTL, FALSE - otherwise.
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL();

	//-----------------------------------------------------------------------
	// Summary:
	//     Enable/disable preview mode for the control.
	// Parameters:
	//     bIsPreviewMode - TRUE for enabling preview mode,
	//                      FALSE for disabling.
	// Remarks:
	//     Call this member function if you want to hide or show the
	//     row preview item.
	// See Also: IsPreviewMode
	//-----------------------------------------------------------------------
	void EnablePreviewMode(BOOL bIsPreviewMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current preview mode.
	// Remarks:
	//     Call this member function if you want to determine whether the
	//     row preview item is shown or not.
	// Returns:
	//     TRUE when preview mode is enabled,
	//     FALSE when preview mode is disabled.
	// See Also: EnablePreviewMode
	//-----------------------------------------------------------------------
	BOOL IsPreviewMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides Group By control area.
	// Parameters:
	//     bEnable - TRUE for showing GroupBy area,
	//               FALSE for hiding GroupBy area.
	// Remarks:
	//     Call this member function if you want to hide or show
	//     Group By area in the report header.
	// See Also: IsGroupByVisible
	//-----------------------------------------------------------------------
	void ShowGroupBy(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current Group By area mode.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     Group By area in the report header is visible or not.
	// Returns:
	//     TRUE when Group By area is shown on the control,
	//     when Group By area is hidden.
	// See Also: ShowGroupBy
	//-----------------------------------------------------------------------
	BOOL IsGroupByVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide/show column headers.
	// Parameters:
	//     bShow - TRUE is column headers will be displayed, FALSE to hide column headers.
	// See Also: IsHeaderVisible, ShowFooter
	//-----------------------------------------------------------------------
	void ShowHeader(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine is column headers are currently visible.
	// Returns:
	//     TRUE is column headers are visible, FALSE is column headers are hidden.
	// See Also: ShowHeader
	//-----------------------------------------------------------------------
	BOOL IsHeaderVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide/show report footers.
	// Parameters:
	//     bShow - TRUE is column footers will be displayed, FALSE to hide column footers.
	// See Also: IsFooterVisible, ShowHeader
	//-----------------------------------------------------------------------
	void ShowFooter(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine is column footers are currently visible.
	// Returns:
	//     TRUE is column footers are visible, FALSE is column footers are hidden.
	// See Also: ShowHeader
	//-----------------------------------------------------------------------
	BOOL IsFooterVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enable/disable group shade.
	// Parameters:
	//     bEnable - TRUE for enabling group items shade,
	//               FALSE for disabling.
	// Remarks:
	//     Call this member function if you want to hide or show
	//     group rows' headings.
	// See Also:
	//     IsShadeGroupHeadingsEnabled, IsGroupRowsBold, SetGroupRowsBold,
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	void ShadeGroupHeadings(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current group shade mode.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     group rows' headings are enabled or not.
	// Returns:
	//     TRUE when group items shading is enabled,
	//     FALSE when it is disabled.
	// See Also:
	//     ShadeGroupHeadings, IsGroupRowsBold, SetGroupRowsBold,
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	BOOL IsShadeGroupHeadingsEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Set group rows text style: bold or normal.
	// Parameters:
	//     bBold - TRUE for bold text style, FALSE for normal.
	// Remarks:
	//     Call this member function if you want to change group rows
	//     bold text style.
	// See Also:
	//     IsGroupRowsBold, ShadeGroupHeadings, IsShadeGroupHeadingsEnabled,
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	void SetGroupRowsBold(BOOL bBold = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current group rows text bold style.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     group rows' text are bold or normal.
	// Returns:
	//     TRUE when group text are bold,
	//     FALSE when it is normal.
	// See Also:
	//     SetGroupRowsBold, ShadeGroupHeadings, IsShadeGroupHeadingsEnabled,
	//     GetItemMetrics
	//-----------------------------------------------------------------------
	BOOL IsGroupRowsBold() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the number of fixed columns.
	// Parameters:
	//     nCount - Number of fixed columns.
	//-----------------------------------------------------------------------
	void SetFixedColumnsCount(int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the number of fixed columns.
	//-----------------------------------------------------------------------
	int GetFixedColumnsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve count of none-scrolled columns
	//     at the left side.
	// Returns:
	//     Count of none-scrolled columns
	//     at the left side.
	//-----------------------------------------------------------------------
	int GetFreezeColumnsCount() const;
	int GetFreezeColumnsIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the number of none-scrollable columns
	// Parameters:
	//     nCount - number of none-scrollable columns
	//     bAdjust 0 flag used to adjust horizontal scrollbar vertical position
	//     if some of freezed columns change width and scrollbar located
	//     under not-freezed columns only (similar to Excel scrollbar location)
	//-----------------------------------------------------------------------
	void SetFreezeColumnsCount(int nCount, BOOL bAdjust = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve count of the columns at the left side
	// where reordering is disabled.
	// Returns:
	//     Count of drop-disable columns
	//     at the left side.
	//-----------------------------------------------------------------------
	int GetDisableReorderColumnsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set count of the columns at the left side
	// where reordering is disabled.
	// Parameters:
	//     nCount - Count of reorder-disabled columns at the left side.
	//-----------------------------------------------------------------------
	void SetDisableReorderColumnsCount(int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//      Get dynamic horizontal scrolling mode.
	// Returns:
	//      Returns TRUE if horizontal scrolling is by full columns,
	//      otherwise horizontal scrolling is by pixels.
	// See Also: SetFullColumnScrolling
	//-----------------------------------------------------------------------
	BOOL IsFullColumnScrolling() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set horizontal scrolling mode.
	// Parameters:
	//      bSet - If TRUE full columns scrolling mode is set,
	//             if FALSE horizontal scrolling by pixels mode is set.
	// See Also: IsFullColumnScrolling
	//-----------------------------------------------------------------------
	void SetFullColumnScrolling(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all items and column in specified position.
	// Parameters:
	//     nIndex - Index of item to remove.
	//-----------------------------------------------------------------------
	void ReleaseItem(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to pin footer records to the last body row.
	//     By default, footer records are docked to the footer.
	// Parameters:
	//     bPin - TRUE is footer records will be displayed immediately after the body rows.
	// See Also: IsFooterRowsVisible, ShowFooterRows
	//-----------------------------------------------------------------------
	void PinFooterRows(BOOL bPin = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if footer records are pinned to the body rows.
	// Returns:
	//     TRUE if footer records are pinned to the body rows. FALSE if footer records are docked to the footer.
	// See Also: PinFooterRows
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsPinned() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to pin footer records to the last body row.
	//     By default, footer records are docked to the footer.
	// Parameters:
	//     bPin - TRUE is footer records will be displayed immediately after the body rows.
	// See Also: IsFooterRowsVisible, ShowFooterRows
	//-----------------------------------------------------------------------
	void PinFooterRowsPrinted(BOOL bPin = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if footer records are pinned to the body rows.
	// Returns:
	//     TRUE if footer records are pinned to the body rows. FALSE if footer records are docked to the footer.
	// See Also: PinFooterRowsPrinted
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsPinnedPrinted() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to enable markup for Records
	// Parameters:
	//     bEnable - TRUE to enable markup
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns markup context
	// Returns:
	//     A pointer to the markup context object.
	//-------------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the image manager of Report Control.
	// Returns:
	//     Pointer to the image manager of Report Control.
	// See Also: SetImageManager
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new image manager.
	// Parameters:
	//     pImageManager - Image manager to be set
	// Example:
	// <code>
	// CXTPImageManager* pImageManager = new CXTPImageManager();
	// pImageManager->SetIcons(IDR_MAINFRAME);
	// m_wndReport.SetImageManager(pImageManager);
	// </code>
	// See Also: GetImageManager
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Initiates ImageList of Paint Manager.
	// Parameters:
	//     pImageList - Image list.
	// Remarks:
	//     You use this function to set up your own ImageList
	//     with set bitmaps that represent various states of rows
	//     and depict any other information.
	// Note:
	//     Recommended to use SetImageManager/GetImageManager methods instead.
	// Example:
	// <code>
	// CImageList listIcons;
	// listIcons.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 1));
	// CBitmap bmp;
	// // load bitmap by id
	// bmp.LoadBitmap(IDB_BMREPORT);
	// ilIcons.Add(&bmp, RGB(255, 0, 255));
	// m_wndReport.SetImageList(&lIcons);
	// </code>
	// See Also: GetImageManager
	//-----------------------------------------------------------------------
	void SetImageList(CImageList* pImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a pointer to the associated report header object.
	// Remarks:
	//     Call this member function if you want to retrieve access
	//     to the report header object properties and methods.
	// Returns:
	//     A pointer to the associated report header.
	// See Also: CXTPReportHeader overview
	//-----------------------------------------------------------------------
	CXTPReportHeader* GetReportHeader() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change report header of Report Control
	// Parameters:
	//     pReportHeader - Report header to be set
	// Example:
	//     <code>m_wndReport.SetReportHeader(new CMyReportHeader());</code>
	// See Also: CXTPReportHeader overview
	//-----------------------------------------------------------------------
	void SetReportHeader(CXTPReportHeader* pReportHeader);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns indentation for the tree view row of the specified depth level.
	// Parameters:
	//     nLevel - Tree depth level.
	// Remarks:
	//     Calculates row indentation in pixels based on the provided
	//     indentation level.
	// Returns:
	//     Row indentation in pixels.
	// See Also: Populate
	//-----------------------------------------------------------------------
	int GetIndent(int nLevel) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies parent control of some event that has happened.
	// Parameters:
	//     pRow       - Specified row of event if used.
	//     pItem      - Specified item of event if used.
	//     pColumn    - Specified column of event if used.
	//     nMessage   - A message to sent to parent window.
	//     pPoint     - A point where the message was sent from in
	//                        client coordinates.
	//     nHyperlink - Hyperlink order number, where the message was sent
	//                  from (-1 if message was not send from the hyperlink).
	// Remarks:
	//     Sends a message to the parent in the form of a WM_NOTIFY message
	//     with a specific structure attached.
	// Returns:
	//     The result of the message processing;
	//     its value depends on the message sent. (see CWnd::SendMessage)
	// See Also: CXTPReportControl overview, SendNotifyMessage
	//-----------------------------------------------------------------------
	LRESULT SendMessageToParent(CXTPReportRow* pRow, CXTPReportRecordItem* pItem, CXTPReportColumn* pColumn, UINT nMessage, CPoint* pPoint, int nHyperlink = -1) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends the specified message to the window.
	// Parameters:
	//     nMessage - The message to be sent.
	//     pNMHDR - Notify header
	// Returns:
	//     Nonzero if successful; otherwise returns zero.
	//-----------------------------------------------------------------------
	LRESULT SendNotifyMessage(UINT nMessage, NMHDR* pNMHDR = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses all rows.
	// Remarks:
	//     The CollapseAll method collapses all the CXTPReportRow objects,
	//     including all the child rows, that are in the report control.
	// See Also: ExpandAll, CXTPReportRow::SetExpanded
	//-----------------------------------------------------------------------
	void CollapseAll();

	//-----------------------------------------------------------------------
	// Summary:
	//  Expands all rows.
	// Parameters:
	//  bRecursive - BOOL flag to expand on one level (if FALSE) or recursive
	// Remarks:
	//     The ExpandAll method expands all the CXTPReportRow objects,
	//     including all the child rows, that are in the report control.
	// See Also: CollapseAll, CXTPReportRow::SetExpanded
	//-----------------------------------------------------------------------
	void ExpandAll(BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the report allows multiple selections.
	// Returns:
	//     TRUE if the report allows multiple selections.
	// See Also: SetMultipleSelection, GetSelectedRows
	//-----------------------------------------------------------------------
	BOOL IsMultipleSelection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes allowing multiple selections for the control.
	// Parameters:
	//     bMultipleSelection - TRUE for enabling, FALSE for disabling.
	// Remarks:
	//     Sets the flag that determines whether the report allows multiple selections.
	// See Also: IsMultipleSelection, GetSelectedRows
	//-----------------------------------------------------------------------
	void SetMultipleSelection(BOOL bMultipleSelection);

	//-----------------------------------------------------------------------
	// Summary:
	// See Also: SetMultiSelectionMode
	// Returns:
	//     TRUE if the report allows multi-selections i.e. is VK_CTRL is always ON
	//-----------------------------------------------------------------------
	BOOL IsMultiSelectionMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the multiple selection mode for the control.
	// Parameters:
	//     bMultiSelectionMode - TRUE for enabling, FALSE for disabling.
	// Remarks:
	//     Sets the flag that determines whether the report is in multi-selection mode i.e. VK_CTRL is always ON
	// See Also: IsMultiSelectionMode
	//-----------------------------------------------------------------------
	void SetMultiSelectionMode(BOOL bMultiSelectionMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables showing tooltips for the control.
	// Parameters:
	//     bEnable - TRUE for enabling, FALSE for disabling.
	// Remarks:
	//     Call this member function to enable or disable tooltips
	//     show for the report control window.
	//-----------------------------------------------------------------------
	void EnableToolTips(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether groups are skipped when navigating
	//     the ReportControl with the Up and Down keys.
	// Parameters:
	//     bSkipFocus - If TRUE, when navigating the rows with the Up and Down
	//                  keys in the ReportControl the group headings will be skipped
	//                  and the next non-group heading row will be selected.
	//                  If FALSE, when navigating the rows with the Up and Down
	//                  keys in the ReportControlall rows will be selected,
	//                  even group headings.
	//-----------------------------------------------------------------------
	void SkipGroupsFocus(BOOL bSkipFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine is groups are skipped when navigating the
	//     with the Up and Down arrow keys.
	// Returns:
	//     TRUE if groups are skipped, FALSE is groups receive focus when navigating
	//     the report control with the Up and Down arrow keys.
	//-----------------------------------------------------------------------
	BOOL IsSkipGroupsFocusEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve indentation of the header
	// Returns:
	//     Header indent.
	//-----------------------------------------------------------------------
	int GetHeaderIndent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts scroll bars depending on currently visible rows.
	//-----------------------------------------------------------------------
	virtual void AdjustScrollBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts main control areas depending on current control size.
	//-----------------------------------------------------------------------
	virtual void AdjustLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to the currently selected column.
	// Returns:
	//     A pointer to the currently selected item.
	//-----------------------------------------------------------------------
	CXTPReportColumn* GetFocusedColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set selected column.
	// Parameters:
	//     pColumn - column to be selected.
	// Returns:
	//     TRUE if specified column has been focused, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL SetFocusedColumn(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member set the selection state of a raw.
	// Parameters:
	//     index - The index of the row.
	//     state - Tells where the check box is checked or not.
	//-----------------------------------------------------------------------
	void SetSelectionState(int index, int state);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether focused rows will be forced visible.
	// Parameters:
	//     bFocusSubItems - If TRUE, when a ReportRecordItem is focused, the
	//                      entire row will become visible.
	// See Also: IsEnsureFocusedRowVisible
	//-----------------------------------------------------------------------
	void EnsureFocusedRowVisible(BOOL bEnsureFocusedRowVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if focused rows will be forced visible.
	// Returns:
	//     TRUE focused rows will be forced visible, otherwise false.
	//-----------------------------------------------------------------------
	BOOL IsEnsureFocusedRowVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether each individual CXTPReportRecordItem
	//     will show focus when the item in a row is clicked.
	// Parameters:
	//     bFocusSubItems - If TRUE, when a ReportRecordItem is clicked, the
	//                      entire row will become highlighted except the individual
	//                      item that was clicked.
	//                      If FALSE, the entire row will become highlighted when
	//                      an item is clicked, including the item that was clicked.
	// See Also: IsFocusSubItems
	//-----------------------------------------------------------------------
	void FocusSubItems(BOOL bFocusSubItems);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if individual CXTPReportRecordItem(s) will
	//     receive focus when that item in the CXTPReportRow that they belong to is clicked.
	// Returns:
	//     TRUE if individual items can receive focus, FALSE if only the entire row can receive focus.
	// See Also: FocusSubItems
	//-----------------------------------------------------------------------
	BOOL IsFocusSubItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether a CXTPReportRecordItem can be edited by
	//     single-clicking on the item.
	// Parameters:
	//     bEditOnClick - If TRUE, when the CXTPReportRecordItem is single-clicked,
	//                    the item will become editable.  The entire ReportControl
	//                    or the specific CXTPReportRecordItem must have the bAllowEdit
	//                    property set to TRUE for this to work.
	// See Also: AllowEdit, IsAllowEdit, IsEditOnClick
	//-----------------------------------------------------------------------
	void EditOnClick(BOOL bEditOnClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine whether items are edited with a single-click.
	// Returns:
	//     TRUE it items can be edited with a single-click
	// See Also: AllowEdit, IsAllowEdit, EditOnClick
	//-----------------------------------------------------------------------
	BOOL IsEditOnClick() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member enable the delay click edit feature.
	// Parameters:
	//     bEditOnDelayClick - A TRUE to enable the delay click edit and FALSE
	//                         to turn off this feature.
	//-----------------------------------------------------------------------
	void EditOnDelayClick(BOOL bEditOnDelayClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the delay click edit feature
	//     is enabled or not.
	// Returns:
	//     A BOOL value denoting whether the delay click edit is enabled or
	//     not, TRUE if enabled and FALSE if not.
	//-----------------------------------------------------------------------
	BOOL IsEditOnDelayClick() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether a CXTPReportRecordItem can be edited by
	//     double-clicking on the item.
	// Parameters:
	//     bEditOnClick - If TRUE, when the CXTPReportRecordItem is double-clicked,
	//                    the item will become editable.  The entire ReportControl
	//                    or the specific CXTPReportRecordItem must have the bAllowEdit
	//                    property set to TRUE for this to work.
	// See Also: AllowEdit, IsAllowEdit, IsEditOnDoubleClick
	//-----------------------------------------------------------------------
	void EditOnDoubleClick(BOOL bEditOnClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine whether items are edited with a double-click.
	// Returns:
	//     TRUE it items can be edited with a double-click
	// See Also: AllowEdit, IsAllowEdit, EditOnDoubleClick
	//-----------------------------------------------------------------------
	BOOL IsEditOnDoubleClick() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the row which requested for editing most
	//     recently.
	// Returns:
	//     An integer value specifying the index of the raw which requested
	//     a delay edit most recently.
	//-----------------------------------------------------------------------
	int GetLastRqstEditRow() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the column which requested for editing
	//     most recently.
	// Returns:
	//     An integer value specifying the index of the column which requested
	//     a delay edit most recently.
	//-----------------------------------------------------------------------
	int GetLastRqstEditCol() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set an arbitrary row and column as most recent
	//     delay edit requester.
	// Parameters:
	//     iRqstEditRow - The row.
	//     iRqstEditCol - The column.
	//-----------------------------------------------------------------------
	void SetLastRqstEdit(int iRqstEditRow, int iRqstEditCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to stop the last request timer which is used for
	//     delay edit.
	//-----------------------------------------------------------------------
	void EnsureStopLastRqstTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to start the last request timer which is used for
	//     delay edit.
	//-----------------------------------------------------------------------
	void StartLastRqstTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the ID of the delay edit timer.
	// Returns:
	//     A UINT_PTR value specifying the id of the timer.
	//-----------------------------------------------------------------------
	UINT_PTR GetDelayEditTimer() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to make sure that the delay edit timer has
	//     stopped properly.
	//-----------------------------------------------------------------------
	void EnsureStopDelayEditTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to start the delay edit timer.
	//-----------------------------------------------------------------------
	void StartDelayEditTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable auto check mode for check box items.
	// Parameters:
	//     bAutoCheck - TRUE to enable auto check mode, FALSE to disable auto check mode.
	// Remarks:
	//     When TRUE, the check box will become checked or unchecked automatically when the
	//     user clicks on the check box.
	// See Also: IsAutoCheckItems, CXTPReportRecordItem::OnClick, CXTPReportRecordItem::OnChar
	//-----------------------------------------------------------------------
	void SetAutoCheckItems(BOOL bAutoCheck);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine id auto check mode for check box items is enabled.
	// Remarks:
	//     If TRUE is returned, the check box will become checked or unchecked automatically when the
	//     user clicks on the check box.
	// Returns:
	//      TRUE if auto check mode is enabled, FALSE if auto check mode is disable.
	// See Also: SetAutoCheckItems, CXTPReportRecordItem::OnClick, CXTPReportRecordItem::OnChar
	//-----------------------------------------------------------------------
	BOOL IsAutoCheckItems() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to start editing a report cell.
	//     It makes a specified cell focused and starts in-place editing
	//     control which was specified there.
	// Remarks:
	//     Note that all editing options should be enabled for this method
	//     to be executed successfully.
	// Parameters:
	//     pItemArgs - Arguments of item to be edit.
	// Remarks:
	//     Call this method with NULL as parameter to stop item edit.
	// See Also: AllowEdit, CXTPReportColumn::SetEditable
	//-----------------------------------------------------------------------
	void EditItem(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves in-place edit pointer.
	// Returns:
	//     Pointer to in-place edit control.
	//-----------------------------------------------------------------------
	CXTPReportInplaceEdit* GetInplaceEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves in-place buttons of report control.
	// Returns:
	//     Collection of in-place buttons.
	//-----------------------------------------------------------------------
	CXTPReportInplaceButtons* GetInplaceButtons() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves in-place list of report controls
	// Returns:
	//     Pointer to in-place list.
	//-----------------------------------------------------------------------
	CXTPReportInplaceList* GetInplaceList() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the ReportControl has focus.
	// Returns:
	//     TRUE if the ReportControl has focus, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL HasFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the currently active item, which
	//     is the item that has focus and use edit mode.
	// Returns:
	//     A pointer to the currently active(focused) CXTPReportRecordItem if in edit mode or NULL.
	//-----------------------------------------------------------------------
	CXTPReportRecordItem* GetActiveItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the currently focused item
	// Returns:
	//     A pointer to the currently focused CXTPReportRecordItem.
	//-----------------------------------------------------------------------
	CXTPReportRecordItem* GetFocusedRecordItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specifies the indentation placed before the
	//     text of each child node in a hierarchical tree structure.
	// Parameters:
	//     nIndent - Indentation used when displaying child nodes.
	//-----------------------------------------------------------------------
	void SetTreeIndent(int nIndent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable virtual mode of the control
	// Parameters:
	//  pVirtualRecord - record to be used as virtual for all rows.
	//  nCount - Count of virtual records.
	//  nFields -  number of fields to assign);
	// Example:
	// <code>
	// class CVirtualRecord : public CXTPReportRecord
	// {
	// public:
	//     CVirtualRecord()
	//     {
	//         AddItem(new CXTPReportRecordItem());
	//         AddItem(new CXTPReportRecordItem());
	//         AddItem(new CXTPReportRecordItem());
	//     }
	//
	//     void GetItemMetrics (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
	//     {
	//         // Draw virtual record
	//     }
	// }
	// ...
	// m_wndReport.SetVirtualMode(new CVirtualRecord(), 540);
	// </code>
	// See Also: IsVirtualMode
	//-----------------------------------------------------------------------
	void SetVirtualMode(CXTPReportRecord* pVirtualRecord, int nCount, int nFields = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if control in virtual mode
	// Returns;
	//     TRUE if virtual mode is enabled; FALSE otherwise.
	// See Also: IsVirtualMode
	//-----------------------------------------------------------------------
	BOOL IsVirtualMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the text from the associated filter edit control.
	// Returns:
	//     Text string entered by user inside filter edit control.
	// See Also: SetFilterText
	//-----------------------------------------------------------------------
	virtual CString GetFilterText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new filter for control.
	// Parameters:
	//     strFilterText - Filter text to be applied for control.
	// Remarks:
	//     You must call Populate method to update rows.
	// See Also: GetFilterText, Populate
	//-----------------------------------------------------------------------
	virtual void SetFilterText(LPCTSTR strFilterText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determine if control search filter text in hidden columns too.
	//     This option is disabled by default.
	// Returns;
	//     TRUE if control search filter text in hidden columns; FALSE otherwise.
	// See Also: SetFilterHiddenColumns, GetFilterText, SetFilterText
	//-----------------------------------------------------------------------
	virtual BOOL IsFilterHiddenColumns() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable or disable search filter text in hidden
	//     columns.
	// Parameters:
	//     bFilterHidden - TRUE to search filter text in hidden columns;
	//                     FALSE to search in visible columns only.
	// Remarks:
	//     This option is disabled by default.
	//     You must call Populate method to update rows.
	// See Also: IsFilterHiddenColumns, GetFilterText, SetFilterText, Populate
	//-----------------------------------------------------------------------
	virtual void SetFilterHiddenColumns(BOOL bFilterHidden);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set filtering mode for Record tree
	// Parameters:
	//     nMode - int (XTPReportFilterMode enumerator)
	// See Also: GetRecordsTreeFilterMode, XTPReportFilterMode
	//-----------------------------------------------------------------------
	void SetRecordsTreeFilterMode(int nMode);

	//-----------------------------------------------------------------------
	// Summary:
	//  GetRecordsTreeFilterMode
	// Returns:
	//  int as selected filter mode for record tree
	// See Also: SetRecordsTreeFilterMode, XTPReportFilterMode
	//-----------------------------------------------------------------------
	int GetRecordsTreeFilterMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Register the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of resource where control is located
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns TRUE if one or more records are selected indicating the
	//      Cut command can be used.
	// Returns:
	//      A BOOL value of TRUE if cut operation is possible in the current
	//      context, FALSE if not.
	// See Also:
	//      CanPaste, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL CanCut();

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns TRUE if one or more records are selected indicating the
	//      Copy command can be used.
	// Returns:
	//      A BOOL value of TRUE if copy operation is possible in the current
	//      context, FALSE if not.
	// See Also:
	//      CanPaste, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL CanCopy();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE if BinaryRecords or Text data exists in the clipboard.
	// Returns:
	//      A BOOL value of TRUE if paste operation is possible in the current
	//      context, FALSE if not.
	// See Also:
	//      CanCut, CanCopy, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL CanPaste();

	//-----------------------------------------------------------------------
	// Summary:
	//      Copy selected records data (in the Binary and Text data formats) to
	//      the clipboard and delete records.
	// See Also:
	//      CanCut, CanCopy, CanPaste, Copy, Paste,
	//      OnBeforeCopyToText, OnBeforePasteFromText, OnBeforePaste.
	//-----------------------------------------------------------------------
	virtual void Cut();

	//-----------------------------------------------------------------------
	// Summary:
	//      Copy selected records data (in the Binary and Text data formats) to
	//      the clipboard.
	// See Also:
	//      CanCut, CanCopy, CanPaste, Cut, Paste,
	//      OnBeforeCopyToText, OnBeforePasteFromText, OnBeforePaste.
	//-----------------------------------------------------------------------
	virtual void Copy();

	//-----------------------------------------------------------------------
	// Summary:
	//      Reads records from the clipboard and add them to records collection.
	//      The Binary data format is used rather than Text format.
	// See Also:
	//      CanCut, CanCopy, CanPaste, Cut, Copy,
	//      OnBeforeCopyToText, OnBeforePasteFromText, OnBeforePaste.
	//-----------------------------------------------------------------------
	virtual void Paste();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE if inplace edit control is active (visible and focused),
	//     FALSE otherwise.
	// See Also:
	//      GetInplaceEdit.
	//-----------------------------------------------------------------------
	virtual BOOL IsEditMode();

	//-----------------------------------------------------------------------
	// Summary:
	//     Set compare function to sort.
	// Parameters:
	//     pCompareFunc - A XTPReportRowsCompareFunc function pointer that is used
	//                    to compare rows or NULL to use the default one.
	// Remarks:
	//     This method uses Visual C++ run-time library (MSVCRT)
	//     implementation of the quick-sort function, qsort, for sorting
	//     stored CXTPReportRow objects.
	//     If pCompareFunc = NULL the default compare function is used.
	//     Call Populate() method to resort items.
	//
	// See Also:
	//     SortRows, CXTPReportRows::SortEx, CXTPReportRows::Sort,
	//     CXTPReportRows::XTPReportRowsCompareFunc
	//-----------------------------------------------------------------------
	virtual void SetRowsCompareFunc(XTPReportRowsCompareFunc pCompareFunc);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this function to determine is sort order applied for record children.
	// Returns:
	//      TRUE if sort order applied for record children, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsSortRecordChilds() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to apply sort order for record children
	// Parameters:
	//     bSortRecordChilds - TRUE to sort record children.
	//-----------------------------------------------------------------------
	void SetSortRecordChilds(BOOL bSortRecordChilds);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this function to get horizontal scrolling step.
	// Returns:
	//      Horizontal scrolling step in pixels.
	//-----------------------------------------------------------------------
	int GetHScrollStep() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this function to set horizontal scrolling step.
	// Parameters:
	//      nStep - Horizontal scrolling step in pixels.
	//-----------------------------------------------------------------------
	void SetHScrollStep(int nStep);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this function to get vertical scrolling timer resolution.
	// Returns:
	//      Vertical scrolling timer resolution in milliseconds.
	//-----------------------------------------------------------------------
	UINT GetAutoVScrollTimerResolution() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this function to set vertical scrolling timer resolution.
	// Parameters:
	//      nNewTimerResolution - Vertical scrolling timer resolution in milliseconds.
	// See also:
	//      CWnd::SetTimer
	//-----------------------------------------------------------------------
	void SetAutoVScrollTimerResolution(UINT nNewTimerResolution);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the preview allows editing.
	// Returns:
	//     TRUE if the preview allows editing and FALSE if not.
	//-----------------------------------------------------------------------
	BOOL IsPreviewAllowEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to enable the preview editing.
	// Parameter:
	//     bAllowEdit - TRUE to enable the preview editing and FALSE to disable.
	//-----------------------------------------------------------------------
	void PreviewAllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns whether collapsing/expanding rows is locked.
	// Returns:
	//     TRUE if collapsing/expanding rows is locked.
	//-----------------------------------------------------------------------
	BOOL IsLockExpand() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets whether collapsing/expanding rows is locked.
	// Parameter:
	//     bLockExpand - TRUE to lock collapsing/expanding rows.
	//-----------------------------------------------------------------------
	void LockExpand(BOOL bLockExpand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to recalculate and redraw the rows.
	// Parameter:
	//     bAll - TRUE to make the recalculation and drawing apply to the entire
	//            rows and FALSE to make it applicable to the row in focus..
	//-----------------------------------------------------------------------
	void Recalc(BOOL bAll = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables or disables double-buffering.
	// Parameter:
	//     bEnable - TRUE to enable double-buffering, FALSE to disable it
	//-----------------------------------------------------------------------
	void EnableDoubleBuffering(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if double buffering is enabled.
	// Returns:
	//     TRUE if double buffering is enabled, FALSE otherwise.
	// See Also: EnableDoubleBuffering
	//-----------------------------------------------------------------------
	BOOL IsDoubleBuffering() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines whether the first row in the report will become selected
	//     when initially populated.
	// Returns:
	//     TRUE if enabled, FALSE otherwise.
	// See Also: SelectionEnable
	//-----------------------------------------------------------------------
	BOOL IsInitialSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the first row in the report
	//     will become selected when initially populated.  Use InitialSelectionEnable
	//     to stop the report from automatically selecting the first row after populate.
	// Parameters:
	//     bEnable - TRUE to enable initial rows selection.
	//               FALSE to disable initial rows selection.
	// See Also: IsSelectionEnabled
	//-----------------------------------------------------------------------
	void InitialSelectionEnable(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the row focus is visible.
	// Returns:
	//     TRUE if the the row focus is visible, FALSE otherwise.
	// See Also: ShowRowFocus
	//-----------------------------------------------------------------------
	BOOL IsRowFocusVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable showing the row focus.
	// Parameters:
	//     bShow - TRUE to enable showing the row focus.
	//             FALSE to disable showing the row focus.
	// See Also: IsRowFocusVisible
	//-----------------------------------------------------------------------
	void ShowRowFocus(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine current watermark alignment flags.
	// Returns:
	//     Current watermark alignment.
	// See Also: XTPReportWatermarkAlignment, SetWatermarkAlignment.
	//-----------------------------------------------------------------------
	int GetWatermarkAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set watermark alignment flags.
	// Parameters:
	//     nWatermarkAlignment - watermark alignment flags.
	// See Also: XTPReportWatermarkAlignment, GetWatermarkAlignment.
	//-----------------------------------------------------------------------
	void SetWatermarkAlignment(int nWatermarkAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw a water mark
	// Parameters:
	//     pDC - Pointer to the device context.
	//     rc  - The rectangular bounds of the water mark.
	//-----------------------------------------------------------------------
	void DrawWatermark(CDC *pDC, CRect rcWatermark, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve report element rectangle.
	// Parameters:
	//  nElement - number
	// Returns:
	//     Report element rectangle.
	//-----------------------------------------------------------------------
	CRect GetElementRect(int nElement) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the report data manager
	// Remarks:
	//     Use this member function to retrieve an access to the data manager
	// Returns:
	//     The report data manager.
	//-----------------------------------------------------------------------
	CXTPReportDataManager* GetDataManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new operational control mouse mode.
	// Parameters:
	//     nMode - New mouse mode. For available values, see XTPReportMouseMode enum.
	// See Also: XTPReportMouseMode overview, GetMouseMode
	//-----------------------------------------------------------------------
	void SetMouseMode(XTPReportMouseMode nMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current control mouse mode.
	// Returns:
	//     Current control mouse mode.
	//     For available values, see XTPReportMouseMode enum.
	// See Also: XTPReportMouseMode overview, SetMouseMode
	//-----------------------------------------------------------------------
	XTPReportMouseMode GetMouseMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the watermark bitmap to be shown in the report control background.
	// Returns:
	//     hBitmap - bitmap handle.
	//-----------------------------------------------------------------------
	virtual HBITMAP GetWatermarkBitmap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a watermark bitmap to be shown in the report control background.
	// Parameters:
	//     hBitmap - bitmap handle.
	//     Transparency - transparency value.
	// Returns:
	//     TRUE if watermark bitmap successfully added or removed, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL SetWatermarkBitmap(HBITMAP hBitmap, BYTE Transparency);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a watermark bitmap to be shown in the report control background.
	// Parameters:
	//     szPath - path to bitmap file.
	//     Transparency - transparency value.
	// Returns:
	//     TRUE if watermark bitmap successfully added or removed, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL SetWatermarkBitmap(LPCTSTR szPath, BYTE Transparency);


	//-----------------------------------------------------------------------
	// Summary:
	//     Call function to get the next focusable column
	// Parameter:
	//     pRow         -  A pointer to the report row object.
	//     nColumnIndex -  The column index.
	//     nDirection   -  A positive integer specifying the right side and
	//                     negative integer specifying the left side of the
	//                     column, with index nColumnIndex.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual CXTPReportColumn* GetNextFocusableColumn(CXTPReportRow* pRow, int nColumnIndex, int nDirection);

	CXTPReportBehavior* GetBehavior() const;

	//-----------------------------------------------------------------------
	// Summary:
	//  Call this function to specify whether item icons are shown when the item
	//  is getting edited.
	// Parameters:
	//  bShow - BOOL flag to set or not
	//-----------------------------------------------------------------------
	void ShowIconWhenEditing(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//  Call this function to determine whether item icons are shown when the item
	//  is getting edited.
	// Returns:
	//  TRUE if item icons are shown while being edited.  FALSE by default.
	//-----------------------------------------------------------------------
	BOOL IsShowIconWhenEditing();

	//-----------------------------------------------------------------------
	// Summary:
	//  This member used to set Fast Deselect Mode (like Windows Explorer use)
	// Parameters:
	//  bFastDeselect - BOOL flag to set or not
	//-----------------------------------------------------------------------
	void SetFastDeselectMode(BOOL bFastDeselect = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//  This member used to set column as icon view column
	// Parameters:
	//  pColumn - pointer to column
	//-----------------------------------------------------------------------
	void SetIconColumn(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//  This member used to create column as icon view column
	//  and also allows to reuse it for number column in report view
	// Parameters:
	//  bUseColumnForNum - flag to reuse icon column as number column in report view
	//  nWidth - width of number column
	//-----------------------------------------------------------------------
	void CreateIconColumn(BOOL bUseColumnForNum = FALSE, int nWidth = 40);

	//-----------------------------------------------------------------------
	// Summary:
	//  This member used to create column as icon view column
	//  and also allows to reuse it for number column in report view
	//  can be used on for non-virtual mode
	// Parameters:
	//  nCol - column index to use as icon column property (field)
	//  nIcon - icon index to use in icon view
	//  bUseColumnForNum - flag to reuse icon column as number column in report view
	//  nWidth - width of number column
	//-----------------------------------------------------------------------
	void AssignIconViewPropNumAndIconNum(int nCol = 0, int nIcon = 0,
		BOOL bUseColumnForNum = FALSE, int nWidth = 20);

	//-----------------------------------------------------------------------
	// Summary:
	//  This member used to toggle between IconView and ReportView modes
	// Parameters:
	//  bIconView - BOOL flag (TRUE for IconView mode, FALSE for ReportView mode)
	//-----------------------------------------------------------------------
	void SetIconView(BOOL bIconView = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//  Call this function to know whether the icon view mode is active or not.
	// Returns:
	//  TRUE if IconView mode active
	//-----------------------------------------------------------------------
	BOOL IsIconView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//  Call this function to get the rows per line.
	// Returns:
	//  integer value denoting the rows per line
	//-----------------------------------------------------------------------
	int GetRowsPerLine() const;

	//-----------------------------------------------------------------------
	// Summary:
	//  Call this functions to get the number of rows in a line of arbitrary
	//  length.
	// Parameters:
	//  iTotalWidth - passed param for total width
	// Returns:
	//  An integer specifying the number of lines in a row.
	//-----------------------------------------------------------------------
	int GetNumRowsOnLine(int iTotalWidth) const;

	//-----------------------------------------------------------------------
	// Summary:
	//  Call this function to set the report navigator.
	// Parameters:
	//  pNavigator - pointer to flag CXTPReportNavigator
	//-----------------------------------------------------------------------
	void SetNavigator(CXTPReportNavigator* pNavigator);

	//-----------------------------------------------------------------------
	// Summary:
	//      ShowRowNumber
	// Parameters:
	//  bSet - BOOL flag to show or hide Row Number column
	//-----------------------------------------------------------------------
	void ShowRowNumber(BOOL bSet);

	//-----------------------------------------------------------------------
	// Summary:
	//      IsShowRowNumber
	// Returns:
	//      TRUE if Row Number visible
	//-----------------------------------------------------------------------
	BOOL IsShowRowNumber();

	//-----------------------------------------------------------------------
	// Summary:
	//      Helper function
	// Parameters:
	//       row  - row number.
	//       col  - column number.
	//      sText - string to set
	// Returns:
	//      TRUE if success
	//-----------------------------------------------------------------------
	BOOL SetCellText(int row, int col, CString sText);

	//-----------------------------------------------------------------------
	// Summary:
	//      Helper function
	// Parameters:
	//          row      - row number.
	//          col      - column number.
	//          sFormula - Formula to set.
	// Returns:
	//      TRUE if success
	//-----------------------------------------------------------------------
	BOOL SetCellFormula(int row, int col, CString sFormula);

	//-----------------------------------------------------------------------
	// Summary:
	//     Unselect all Group Rows
	//-----------------------------------------------------------------------
	void UnselectGroupRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to release the sorted items and clear the memory.
	//-----------------------------------------------------------------------
	void ReleaseSorted();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the horizontal scroll position.
	// Returns:
	//     An integer denoting the horizontal scroll position.
	//-----------------------------------------------------------------------
	int GetLeftOffset() const;

	//assessors for m_bDragMode
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether current mouse context is dragging.
	// Returns:
	//     BOOL value of TRUE if the report control item(s) are in drag mode
	//     FALSE if not.
	//-----------------------------------------------------------------------
	BOOL IsDragMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to reset the icon view column to is default values.
	//     This should be called if the column in ever destroyed.
	//-----------------------------------------------------------------------
	void SetIconViewToDefaults();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the scroll mode
	//-----------------------------------------------------------------------
	XTPReportScrollMode GetScrollMode(XTPReportOrientation orientation) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the scroll mode
	//-----------------------------------------------------------------------
	void SetScrollMode(
		XTPReportOrientation orientation,
		XTPReportScrollMode  scrollMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a pointer to the sections collection.
	// Returns:
	//     Pointer to the sections collection.
	//-----------------------------------------------------------------------
	CXTPReportSections *GetSections() const;

#if XTP_REPORT_DEPRECATED()

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current grid drawing mode.
	// Parameters:
	//     bVertical - TRUE for vertical grid style,
	//                 FALSE for horizontal grid style.
	// Remarks:
	//     Call this member function if you want to retrieve current
	//     grid lines drawing style for the report control.
	// Returns:
	//     Current grid drawing style.
	// See Also: XTPReportGridStyle overview, SetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	XTPReportGridStyle GetGridStyle(BOOL bVertical) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Set or clear grid drawing style.
	// Parameters:
	//     bVertical - TRUE for changing vertical grid style,
	//                 FALSE for changing horizontal grid style.
	//     gridStyle - New grid style. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     Call this member function if you want to change a style
	//     of report grid lines.
	//
	//     Possible grid line styles are the following:
	//     * <b>xtpReportGridNoLines</b>  Empty line
	//     * <b>xtpReportGridSmallDots</b> Line is drawn with small dots
	//     * <b>xtpReportGridLargeDots</b> Line is drawn with large dots
	//     * <b>xtpReportGridDashes</b> Line is drawn with dashes
	//     * <b>xtpReportGridSolid</b> Draws solid line
	//
	// See Also: XTPReportGridStyle overview, GetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	void SetGridStyle(BOOL bVertical, XTPReportGridStyle gridStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the data records.
	// Returns:
	//     The data records collection.
	// Remarks:
	//     Call this member function if you want to retrieve an access
	//     to the collection of report records. You may then perform
	//     standard operations on the collection like adding, removing, etc.
	// See Also: CXTPReportRecords overview, AddRecord
	//-----------------------------------------------------------------------
	CXTPReportRecords* GetRecords() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the header records.
	// Returns:
	//     The header records collection.
	// Remarks:
	//     Call this member function if you want to retrieve an access
	//     to the collection of report header records. You may then perform
	//     standard operations on the collection like adding, removing, etc.
	// See Also: CXTPReportRecords overview, AddRecord
	//-----------------------------------------------------------------------
	CXTPReportRecords* GetHeaderRecords() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the footer records.
	// Returns:
	//     The footer records collection.
	// Remarks:
	//     Call this member function if you want to retrieve an access
	//     to the collection of report footer records. You may then perform
	//     standard operations on the collection like adding, removing, etc.
	// See Also: CXTPReportRecords overview, AddRecord
	//-----------------------------------------------------------------------
	CXTPReportRecords* GetFooterRecords() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the report rows
	// Remarks:
	//     Use this member function to retrieve an access to the collection
	//     of report rows, representing current control view.
	//
	//     Note that rows collection could be rebuilt automatically
	//     on executing Populate method.
	// Returns:
	//     The report rows collection.
	// Example:
	//     See example for CXTPReportControl::BeginUpdate method.
	// See Also: CXTPReportRows overview, Populate
	//-----------------------------------------------------------------------
	CXTPReportRows* GetRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the report header rows
	// Remarks:
	//     Use this member function to retrieve an access to the collection
	//     of report header rows, representing current control view.
	//
	//     Note that rows collection could be rebuilt automatically
	//     on executing Populate method.
	// Returns:
	//     The report header rows collection.
	// Example:
	//
	// See Also: CXTPReportRows overview, Populate
	//-----------------------------------------------------------------------
	CXTPReportRows* GetHeaderRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the report footer rows
	// Remarks:
	//     Use this member function to retrieve an access to the collection
	//     of report footer rows.
	//
	//     Note that rows collection could be rebuilt automatically
	//     on executing Populate method.
	// Returns:
	//     The report footer rows collection.
	// Example:
	//
	// See Also: CXTPReportRows overview, Populate
	//-----------------------------------------------------------------------
	CXTPReportRows* GetFooterRows() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve Report bounding rectangle.
	// Returns:
	//     Report bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetReportRectangle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve Header records bounding rectangle.
	// Returns:
	//     Header records bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetHeaderRowsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve footer rows bounding rectangle.
	// Returns:
	//     Footer rows bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetFooterRowsRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow user moving the selection to header rows.
	// Parameters:
	//     bAllowAccess - TRUE - selection of header rows is allowed, FALSE otherwise.
	// See Also: IsHeaderRowsAllowAccess
	//-----------------------------------------------------------------------
	void HeaderRowsAllowAccess(BOOL bAllowAccess);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow user moving the selection to footer rows.
	// Parameters:
	//     bAllowAccess - TRUE - selection of footer rows is allowed, FALSE otherwise.
	// See Also: IsFooterRowsAllowAccess
	//-----------------------------------------------------------------------
	void FooterRowsAllowAccess(BOOL bAllowAccess);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if user can move selection to header rows.
	// Returns:
	//     TRUE if user can move selection to header rows, FALSE otherwise.
	// See Also: HeaderRowsAllowAccess.
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsAllowAccess() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if user can move selection to footer rows.
	// Returns:
	//     TRUE if user can move selection to footer rows, FALSE otherwise.
	// See Also: FooterRowsAllowAccess.
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsAllowAccess() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if rows selection is enabled.
	// Returns:
	//     TRUE if rows selection is enabled, FALSE otherwise.
	// See Also: SelectionEnable
	//-----------------------------------------------------------------------
	BOOL IsSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable rows selection.
	// Parameters:
	//     bEnable - TRUE to enable rows selection.
	//                  FALSE to disable rows selection.
	// See Also: IsSelectionEnabled
	//-----------------------------------------------------------------------
	void SelectionEnable(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable selection in header rows.
	// Parameters:
	//     bEnable - TRUE - selection of header rows is enabled, FALSE otherwise.
	// See Also: IsHeaderRowsSelectionEnabled
	//-----------------------------------------------------------------------
	void HeaderRowsEnableSelection(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if selection in header rows is enabled.
	// Returns:
	//     TRUE if selection in header rows is enabled, FALSE otherwise.
	// See Also: HeaderRowsEnableSelection.
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable selection in footer rows.
	// Parameters:
	//     bEnable - TRUE - selection of footer rows is enabled, FALSE otherwise.
	// See Also: IsFooterRowsSelectionEnabled
	//-----------------------------------------------------------------------
	void FooterRowsEnableSelection(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if selection in footer rows is enabled.
	// Returns:
	//     TRUE if selection in footer rows is enabled, FALSE otherwise.
	// See Also: FooterRowsEnableSelection.
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow the text in all CXTPReportRecordItem(s) to be edited.  This will
	//     add an edit box to the item where the text can be edited.
	// Parameters:
	//     bAllowEdit - TRUE to add an edit box to the CXTPReportRecordItem(s) so they are editable.
	//                  FALSE to remove the edit box and not allow them to be edited.
	// See Also: IsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	void AllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the CXTPReportRecordItem(s)are editable.
	// Returns:
	//     TRUE is the CXTPReportRecordItem(s) are editable, FALSE otherwise.
	// See Also: AllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsAllowEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow the text in all CXTPReportRecordItem(s) to be edited (for header records only).
	//     This will add an edit box to the item where the text can be edited.
	// Parameters:
	//     bAllowEdit - TRUE to add an edit box to the CXTPReportRecordItem(s) so they are editable.
	//                  FALSE to remove the edit box and not allow them to be edited.
	// See Also: IsHeaderRowsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	void HeaderRowsAllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the CXTPReportRecordItem(s) are editable (for header records only).
	// Returns:
	//     TRUE is the CXTPReportRecordItem(s) are editable, FALSE otherwise.
	// See Also: HeaderRowsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsAllowEdit() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow the text in all CXTPReportRecordItem(s) to be edited (for footer records only).
	//     This will add an edit box to the item where the text can be edited.
	// Parameters:
	//     bAllowEdit - TRUE to add an edit box to the CXTPReportRecordItem(s) so they are editable.
	//                  FALSE to remove the edit box and not allow them to be edited.
	// See Also: IsFooterRowsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	void FooterRowsAllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the CXTPReportRecordItem(s) are editable (for footer records only).
	// Returns:
	//     TRUE is the CXTPReportRecordItem(s) are editable, FALSE otherwise.
	// See Also: FooterRowsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsAllowEdit() const;


	//////////////////////////////////////////////////////////////////////////
	// Allow Group
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable grouping of rows.
	// Parameters:
	//     bAllowGroup - TRUE to enable grouping, FALSE to disable it.
	// See Also: IsAllowGroup
	//-----------------------------------------------------------------------
	void AllowGroup(BOOL bAllowGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if grouping of rows is allowed.
	// Returns:
	//     TRUE if grouping of rows is allowed, otherwise FALSE.
	// See Also: AllowGroup
	//-----------------------------------------------------------------------
	BOOL IsAllowGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable grouping of header rows.
	// Parameters:
	//     bAllowGroup - TRUE to enable grouping, FALSE to disable it.
	// See Also: IsHeaderRowsAllowGroup
	//-----------------------------------------------------------------------
	void HeaderRowsAllowGroup(BOOL bAllowGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if grouping of header rows is allowed.
	// Returns:
	//     TRUE if grouping of header rows is allowed, otherwise FALSE.
	// See Also: HeaderRowsAllowGroup
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsAllowGroup() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable grouping of footer rows.
	// Parameters:
	//     bAllowGroup - TRUE to enable grouping, FALSE to disable it.
	// See Also: IsFooterRowsAllowGroup
	//-----------------------------------------------------------------------
	void FooterRowsAllowGroup(BOOL bAllowGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if grouping of footer rows is allowed.
	// Returns:
	//     TRUE if grouping of footer rows is allowed, otherwise FALSE.
	// See Also: FooterRowsAllowGroup
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsAllowGroup() const;


	//////////////////////////////////////////////////////////////////////////
	// Allow Sort
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable sorting of rows.
	// Parameters:
	//     bAllowSort - TRUE to enable sorting, FALSE to disable it.
	// See Also: IsAllowGroup
	//-----------------------------------------------------------------------
	void AllowSort(BOOL bAllowSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if sorting of rows is allowed.
	// Returns:
	//     TRUE if sorting of rows is allowed, otherwise FALSE.
	// See Also: AllowSort
	//-----------------------------------------------------------------------
	BOOL IsAllowSort() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable sorting of header rows.
	// Parameters:
	//     bAllowSort - TRUE to enable sorting, FALSE to disable it.
	// See Also: IsHeaderRowsAllowSort
	//-----------------------------------------------------------------------
	void HeaderRowsAllowSort(BOOL bAllowSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if sorting of header rows is allowed.
	// Returns:
	//     TRUE if sorting of header rows is allowed, otherwise FALSE.
	// See Also: HeaderRowsAllowSort
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsAllowSort() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable sorting of footer rows.
	// Parameters:
	//     bAllowSort - TRUE to enable sorting, FALSE to disable it.
	// See Also: IsFooterRowsAllowSort
	//-----------------------------------------------------------------------
	void FooterRowsAllowSort(BOOL bAllowSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if sorting of footer rows is allowed.
	// Returns:
	//     TRUE if sorting of footer rows is allowed, otherwise FALSE.
	// See Also: FooterRowsAllowSort
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsAllowSort() const;


	//////////////////////////////////////////////////////////////////////////
	// Show Rows
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide/show header records.
	// Parameters:
	//     bShow - TRUE is header records will be displayed, FALSE to hide header records.
	// See Also: IsHeaderRowsVisible, ShowFooterRows
	//-----------------------------------------------------------------------
	void ShowHeaderRows(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if header records are currently visible.
	// Returns:
	//     TRUE if header records are visible, FALSE if header records are hidden.
	// See Also: ShowHeaderRows
	//-----------------------------------------------------------------------
	BOOL IsHeaderRowsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide/show footer records.
	// Parameters:
	//     bShow - TRUE is footer records will be displayed, FALSE to hide footer records.
	// See Also: IsFooterRowsVisible, ShowHeaderRows
	//-----------------------------------------------------------------------
	void ShowFooterRows(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if footer records are currently visible.
	// Returns:
	//     TRUE if footer records are visible, FALSE if footer records are hidden.
	// See Also: ShowFooterRows
	//-----------------------------------------------------------------------
	BOOL IsFooterRowsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the report control theme.
	// Parameters:
	//     paintTheme     - theme of report control. Can be one of the values
	//                      listed in the Remarks section.
	//     bEnableMetrics - Set to TRUE to allow the theme to override control metrics.
	// Remarks:
	//     paintTheme can be one of the following values:
	//      * <b>xtpReportThemeDefault</b> Enables default theme.
	//      * <b>xtpReportThemeOfficeXP</b> Enables Office XP style theme.
	//      * <b>xtpReportThemeOffice2003</b> Enables Office 2003 style theme.
	//      * <b>xtpReportThemeOffice2007</b> Enables Office 2007 style theme.
	//      * <b>xtpReportThemeOffice2010</b> Enables Office 2010 style theme.
	//      * <b>xtpReportThemeOffice2013</b> Enables Office 2013 style theme.
	//      * <b>xtpReportThemeVisualStudio2005</b> Enables Visual Studio 2005 style theme.
	//      * <b>xtpReportThemeVisualStudio2010</b> Enables Visual Studio 2010 style theme.
	//      * <b>xtpReportThemeVisualStudio2012Light</b> Enables VS 2012 Light style Property Report theme.
	//      * <b>xtpReportThemeVisualStudio2012Dark</b> Enables VS 2012 Dark style Property Report theme.
	//      * <b>xtpReportThemeResource</b> Enables visual style theme.
	//-----------------------------------------------------------------------
	void SetTheme(XTPReportPaintTheme paintTheme, BOOL bEnableMetrics=FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves current theme of the report control.
	//-----------------------------------------------------------------------
	XTPReportPaintTheme GetCurrentTheme() const;

#endif // XTP_REPORT_DEPRECATED()

protected:

	int GetPageRowCount(CDC *pDC) const;

	void AdjustScrollBarH(BOOL bUpdate, UINT nMask);   // Adjusts the horizontal scrollbar
	void AdjustScrollBarV(BOOL bUpdate, UINT nMask);   // Adjusts the vertical scrollbar
	void UpdateScrollBarV();



	void Paint(CDC *pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs all drawing logic.
	// Parameters:
	//     pDC - Provided DC to draw control image with.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws all rows on the provided DC.
	// Parameters:
	//     pDC - Provided DC to draw rows image with.
	//     rcClient - A rectangle to draw rows image into.
	// Returns:
	//     Index of last printed row
	//-----------------------------------------------------------------------
	virtual int DrawRows(CDC *pDC, CRect &rcClient, int y, CXTPReportRows *pRows, int nTopRow, int nColumnFrom, int nColumnTo, int *pnHeight = NULL);

	virtual void DrawIconView(CDC *pDC, CRect &rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws 'NoItems' text on the provided DC.
	// Parameters:
	//     pDC - Provided DC to draw text.
	//     rcClient - A rectangle to draw text into.
	//-----------------------------------------------------------------------
	virtual void DrawNoItems(CDC* pDC, const CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns changed status flag of the control. Used for caching control image drawing.
	// Returns:
	//     TRUE if the internal control state was changed since last drawing, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsChanged() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new changed status flag for the control. Used for caching control image drawing.
	// Parameters:
	//     bChanged - TRUE when something was changed in the control contents and control needs to be republished.
	//-----------------------------------------------------------------------
	void SetChanged(BOOL bChanged = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     ReSorts rows tree corresponding to the sort order taken from columns.
	//     Works faster than Populate.
	//-----------------------------------------------------------------------
	virtual void ReSortRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Update field chooser control with its content.
	//-----------------------------------------------------------------------
	void UpdateSubList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts the specified row at the specified position.
	// Parameters:
	//     nIndex - A position to insert row at.
	//     pRow   - A row to insert.
	// Remarks:
	//     Inserts the specified row to the rows array at the specified position
	//     with all its children expanded. Use _DoExpand() to expand
	//     all child items
	// Returns:
	//     A number of the inserted rows.
	//-----------------------------------------------------------------------
	virtual int InsertRow(int nIndex, CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks a record for filter text.
	// Parameters:
	//     pRecord         - A record to apply filter to.
	//     strFilterText   - Filter string text.
	//     bIncludePreview - Include preview item in filtering or not.
	// Remarks:
	//     This member function represents filter functionality. First, it parses
	//     the input text of a filter string by tokens; second it enumerates all visible
	//     columns to find text matching to the filter string. Returns TRUE if
	//     matching found, FALSE otherwise
	// Returns:
	//     TRUE if record is filtered with the specified filter,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL ApplyFilter(CXTPReportRecord* pRecord, CString strFilterText, BOOL bIncludePreview);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called after a user selects a row or a column
	// Remarks:
	//     After user selects row or column, ReportControl sends notification to the
	//     parent window by calling OnSelectionChanged()
	// See Also: XTP_NM_REPORT_SELCHANGED, SendNotifyMessage()
	//-----------------------------------------------------------------------
	virtual void OnSelectionChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called before the focus change
	// Parameters:
	//     pNewRow  - A pointer to row object which been focused;
	//     pNewCol  - A pointer to column object which been focused;
	// Remarks:
	//     Before user changes focused row or column, ReportControl sends notification to the
	//     parent window by calling OnFocusChanging()
	// Returns:
	//     Returns True if selection changing is OK, False if selection is to be
	//     canceled.
	// See Also: XTP_NM_REPORT_FOCUS_CHANGING, SendNotifyMessage(), OnFocusChanged
	//-----------------------------------------------------------------------
	virtual BOOL OnFocusChanging(CXTPReportRow* pNewRow, CXTPReportColumn* pNewCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns drawing metrics for the row.
	// Parameters:
	//     pDrawArgs    - Pointer to the provided draw arguments structure for calculating metrics.
	//     pItemMetrics - Pointer to the metrics structure to fill.
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(
		XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
		XTP_REPORTRECORDITEM_METRICS  *pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create new CXTPReportRow object. Overwrite it
	//     to use derived CXTPReportRow class.
	// Returns:
	//     New CXTPReportRow object.
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* CreateRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create new CXTPReportGroupRow object. Overwrite it
	//     to use derived CXTPReportGroupRow class.
	// Returns:
	//     New CXTPReportGroupRow object.
	//-----------------------------------------------------------------------
	virtual CXTPReportGroupRow* CreateGroupRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create new CXTPReportRow object for
	//     header/footer rows. Overwrite it to use derived CXTPReportRow class.
	// Returns:
	//     New CXTPReportRow object.
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* CreateHeaderFooterRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user start drag row
	// Parameters:
	//     point    - Drag point
	//     nFlags   - Tells the various state of the mouse buttons.
	// Remarks:
	//     Use can override this method or catch LVN_BEGINDRAG message to proceed drag
	//     operations.  OnBeginDrag will not know which mouse button is used for the
	//     drag unless you pass nFlags.
	//-----------------------------------------------------------------------
	virtual void OnBeginDrag(CPoint point);
	virtual void OnBeginDrag(CPoint point, UINT nFlags); // <COMBINE CXTPReportControl::OnBeginDrag@CPoint>

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before add record text data to the clipboard.
	// Parameters:
	//     pRecord      - [in] A pointer to source record;
	//     rarStrings   - [in/out] Reference to strings array with record items
	//                    values. By default this array contains visible items
	//                    captions.
	// Remarks:
	//     If you would like to customize text data for the clipboard you can
	//     change rarStrings as you need.
	//
	//     Default implementation sends XTP_NM_REPORT_BEFORE_COPY_TOTEXT
	//     notification.
	// Returns:
	//     TRUE to cancel copying this record to the clipboard in text format,
	//     FALSE to continue.
	// See Also:
	//     OnBeforePasteFromText, OnBeforePaste, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeCopyToText(CXTPReportRecord* pRecord, CStringArray& rarStrings);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before creating new record using text data
	//     from the clipboard.
	// Parameters:
	//     arStrings    - [in] Strings array with record items values.
	//     ppRecord     - [out] A pointer to new record pointer;
	// Remarks:
	//     If you would like to customize creating new record from text data
	//     you have to create new record object with record items and fill
	//     them using default values and strings provided in arStrings parameter.
	//
	//     If new record will not be set to ppRecord parameter, control will
	//     create CXTPReportRecord object with
	//     CXTPReportRecordItemVariant items and fill visible items using
	//     strings provided in arStrings parameter.
	//
	//     Default implementation sends XTP_NM_REPORT_BEFORE_PASTE_FROMTEXT
	//     notification.
	// Returns:
	//     TRUE to cancel adding this record, FALSE to continue.
	// See Also:
	//     OnBeforeCopyToText, OnBeforePaste, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforePasteFromText(CStringArray& arStrings, CXTPReportRecord** ppRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called after creating new record using binary data
	//     from the clipboard, but before add it to records collection.
	// Parameters:
	//     ppRecord     - [in/out] A pointer to new record pointer;
	// Remarks:
	//     If you would like to customize new record created from binary data
	//     you may to create new or change provided record using
	//     ppRecord parameter.
	//
	//     Default implementation sends XTP_NM_REPORT_BEFORE_PASTE
	//     notification.
	// Returns:
	//     TRUE to cancel adding this record, FALSE to continue.
	// See Also:
	//     OnBeforeCopyToText, OnBeforePasteFromText, Cut, Copy, Paste.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforePaste(CXTPReportRecord** ppRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before processing OnKeyDown event.
	// Parameters:
	//     rnChar     - [in/out] A reference to variable which specifies
	//                  the virtual key code of the given key. For a list of
	//                  of standard virtual key codes, see Winuser.h.
	//     nRepCnt    - [in] Repeat count (the number of times the keystroke
	//                  is repeated as a result of the user holding down the key).
	//     nFlags     - [in] Specifies the scan code, key-transition code,
	//                  previous key state, and context code.
	// Remarks:
	//     If you would like to customize keyboard behavior you can change
	//     rnChar parameter value or perform your logic and return FALSE to
	//     disable default processing.
	//
	//     Default implementation sends XTP_NM_REPORT_PREVIEWKEYDOWN
	//     notification.
	// Returns:
	//     TRUE to continue processing key, FALSE to cancel.
	// See Also:
	//     OnKeyDown, CWnd::OnKeyDown, XTP_NM_REPORT_PREVIEWKEYDOWN.
	//-----------------------------------------------------------------------
	virtual BOOL OnPreviewKeyDown(UINT& rnChar, UINT nRepCnt, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used by the report control to calculate BestFit
	//     column width.
	// Parameters:
	//     pColumn - Pointer to a column for width calculation.
	// Returns:
	//     BestFit column width (or zero).
	//-----------------------------------------------------------------------
	virtual int OnGetColumnDataBestFitWidth(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used by the report control to calculate items
	//     captions maximum width.
	// Parameters:
	//     pDC      - Pointer to a compatible CDC object.
	//     pRows    - Pointer to a rows collection.
	//     pColumn  - Pointer to a column for width calculation.
	//     nStartRow    - Start row index.
	//     nRowsCount   - Rows count.
	// Returns:
	//     Items captions maximum width.
	//-----------------------------------------------------------------------
	virtual int OnGetItemsCaptionMaxWidth(CDC* pDC, CXTPReportRows* pRows,
										  CXTPReportColumn* pColumn, int nStartRow = 0, int nRowsCount= -1);
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to enable monitoring mouse position for automatic
	//     vertical scrolling.
	// Remarks:
	//     Used when dragging records.
	// See Also:
	//     EnsureStopAutoVertScroll, DoAutoVertScrollIfNeed
	//-----------------------------------------------------------------------
	virtual void EnsureStartAutoVertScroll();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to stop monitoring mouse position for automatic
	//     vertical scrolling.
	// Remarks:
	//     Used when dragging records.
	// See Also:
	//     EnsureStartAutoVertScroll, DoAutoVertScrollIfNeed
	//-----------------------------------------------------------------------
	virtual void EnsureStopAutoVertScroll();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to monitoring mouse position. If mouse
	//     will be moved to 20 pixels before top or before bottom report borders
	//     the control will be scrolled vertically.
	// Parameters:
	//     ptClick - Coordinates of initial click (or mouse down).
	//     pt      - Current mouse position.
	// Remarks:
	//     Used when dragging records.
	// See Also:
	//     EnsureStartAutoVertScroll, EnsureStopAutoVertScroll
	//-----------------------------------------------------------------------
	virtual void DoAutoVertScrollIfNeed(CPoint ptClick, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns calculated rows height.
	// Parameters:
	//  pRows - CXTPReportRows*
	//  nTotalWidth - Width of the row
	//  nMaxHeight - The maximum rows height to stop calculation.
	//                  Set this parameter as -1 to calculate all rows height.
	// Returns:
	//     The height of the default rectangle where row's items will draw.
	// Example:
	//     <code>int nHeaderRowsHeight = GetRowHeight(pDC, pRow)</code>
	//-----------------------------------------------------------------------
	virtual int GetRowsHeight(CXTPReportRows* pRows, int nTotalWidth, int nMaxHeight = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to notify about changing constraint selection.
	// Parameters:
	//     pRow         - A pointer to current row object;
	//     pItem        - A pointer to current record item object;
	//     pColumn      - A pointer to current column object;
	//     pConstraint  - A pointer to hot selected constraint;
	// Remarks:
	//     Send for every selection change before new selection applied or canceled.
	// Returns:
	//     TRUE if notification successfully sent, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL OnConstraintSelecting(CXTPReportRow* pRow, CXTPReportRecordItem* pItem, CXTPReportColumn* pColumn,
									   CXTPReportRecordItemConstraint* pConstraint);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get tooltip text for the item
	// Parameters:
	//     pRow             - A pointer to current row object;
	//     pItem            - A pointer to current record item object;
	//     rstrToolTipText  - A reference to CString object to customize tool tip
	//                        text. It contains default tool tip text;
	// Remarks:
	//     If rstrToolTipText set to empty - the default tool tip will be shown
	//     when not enough space to draw item text.
	//     If rstrToolTipText set to single space (" ") no tool tip will be shown.
	// Returns:
	//     A reference to XTP_NM_REPORTTOOLTIPINFO structure.
	//-----------------------------------------------------------------------
	virtual const XTP_NM_REPORTTOOLTIPINFO& OnGetToolTipInfo(CXTPReportRow* pRow, CXTPReportRecordItem* pItem, CString& rstrToolTipText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjust control indentation properties depending on current tree depth.
	// See Also: GetIndent
	//-----------------------------------------------------------------------
	void AdjustIndentation();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when an item is dragged over the report control.
	// Parameters:
	//     pDataObject - Points to the COleDataObject being dragged over the
	//                   drop target.
	//     dwKeyState  - State of keys on keyboard.  Contains the state of the
	//                   modifier keys. This is a combination of any number of
	//                   the following: MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON,
	//                   MK_MBUTTON, and MK_RBUTTON.
	//     point - The current mouse position relative to the report control.
	//     nState - The transition state (0 - enter, 1 - leave, 2 - over).
	// Returns:
	//     A value from the DROPEFFECT enumerated type, which indicates the type
	//     of drop that would occur if the user dropped the object at this
	//     position. The type of drop often depends on the current key state as
	//     indicated by dwKeyState. A standard mapping of key states to DROPEFFECT
	//     values is:
	//       * <b>DROPEFFECT_NONE</b> The data object cannot be dropped in this
	//                                window.
	//       * <b>DROPEFFECT_COPY</b> for <b>MK_CONTROL</b>  Creates a copy of
	//                                                       the dropped object.
	//       * <b>DROPEFFECT_MOVE</b> for <b>MK_ALT</b> Creates a copy of the dropped
	//                                                  object and delete the original object.
	//                                                  This is typically the default drop effect,
	//                                                  when the view can accept the data object.
	//-----------------------------------------------------------------------
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point, int nState);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when an item has been dropped into the report control.
	// Parameters:
	//     pDataObject - Points to the COleDataObject that is dropped into the drop target.
	//     dropEffect  - The drop effect that the user has requested. Can be any of the values listed in the Remarks section.
	//     point - The current mouse position relative to the report control.
	// Remarks:
	//     The <i>dropEffect</i> parameter can be one of the following values:
	//     * <b>DROPEFFECT_COPY</b> Creates a copy of the data object being dropped.
	//     * <b>DROPEFFECT_MOVE</b> Moves the data object to the current mouse location.
	// Returns:
	//     TRUE if the drop was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call function to relay the tooltip event.
	// Parameter:
	//     message -  Windows message.
	// Remarks:
	//     It is a virtual function.
	//-----------------------------------------------------------------------
	virtual void RelayToolTipEvent(UINT message);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call function to get the number of rows per unit mouse scroll.
	// Returns:
	//     A UINT value specifying the number of rows.
	//-----------------------------------------------------------------------
	static UINT AFX_CDECL GetMouseScrollLines();

	//{{AFX_CODEJOCK_PRIVATE
	virtual CString _GetSelectedRowsVisibleColsText();
	BOOL _GetSelectedRows(CXTPReportRecords* pRecords, CXTPInternalCollectionT<CXTPReportRow> * pRows = NULL);

	void _SelectChilds(CXTPReportRecords *pRecords);
	void _SelectRows(CXTPReportRecords *pRecords);

	virtual BOOL _ReadRecordsFromText(LPCTSTR pcszText, CXTPReportRecords& rarRecords);
	virtual CXTPReportRecord* _CreateRecodFromText(LPCTSTR pcszRecord);

	virtual BOOL _WriteSelectedRowsData(CXTPPropExchange* pPX);
	virtual BOOL _ReadRecordsFromData(CXTPPropExchange* pPX, CXTPReportRecords& rarRecords);
	BOOL _WriteRecordsData(CXTPPropExchange* pPX, CXTPReportRecords* pRecords);
	virtual void DrawDropMarker(CDC* pDC);
	virtual void DrawExtDropMarker(CDC* pDC, int y);
	virtual BOOL _ApplyFilter(CXTPReportRecord* pRecord, CString strFilterText, BOOL bIncludePreview);
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
	// System accessibility support.
	virtual HRESULT GetAccessibleParent(IDispatch** ppdispParent);
	virtual HRESULT GetAccessibleChildCount(long* pcountChildren);
	virtual HRESULT GetAccessibleChild(VARIANT varChild, IDispatch** ppdispChild);
	virtual HRESULT GetAccessibleName(VARIANT varChild, BSTR* pszName);
	virtual HRESULT GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole);
	virtual HRESULT AccessibleLocation(long *pxLeft, long *pyTop, long *pcxWidth, long* pcyHeight, VARIANT varChild);
	virtual HRESULT AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarChild);
	virtual HRESULT GetAccessibleState(VARIANT varChild, VARIANT* pvarState);
	virtual CCmdTarget* GetAccessible();
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPReportControl)
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pResult);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportControl)
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);

	// Window events
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR uTimerID);
	afx_msg void OnNcPaint();

	// Focus
	afx_msg void OnSetFocus (CWnd *pOldWnd);
	afx_msg void OnKillFocus(CWnd *pNewWnd);

	// Scrolling
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);

	// Mouse events
	afx_msg void OnLButtonDown  (UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp    (UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown  (UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp    (UINT nFlags, CPoint point);
	afx_msg void OnMouseMove    (UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel   (UINT nFlags, short zDelta, CPoint point);
	afx_msg void OnMouseLeave   ();

	// Key events
	afx_msg void OnKeyDown   (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp     (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp  (UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar      (UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnSysColorChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg LRESULT OnGetObject(WPARAM wParam, LPARAM lParam);

#ifdef _XTP_ACTIVEX
// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CXTPReportControl)
	eventidDrawItem = 11L,
	eventidMeasureRow = 12L,
	eventidDrawPreviewItem = 24L,
	eventidMeasurePreviewItem = 33L,
	//}}AFX_DISP_ID
	};
#endif

#ifdef _XTP_ACTIVEX

public:
	void SetCustomDraw(long dwFlags);

// Event maps
	//{{AFX_EVENT(CXTPReportControl)
	void FireMeasureRow(LPDISPATCH Row, OLE_HANDLE hDC, int nWidth, int* pnHeight)
	{((COleControl*)GetParent())->FireEvent(eventidMeasureRow,EVENT_PARAM(VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_PI4), Row, hDC, nWidth, pnHeight);}

	void FireMeasureRowV(LPDISPATCH Row, OLE_HANDLE hDC, int nWidth, VARIANT* pHeight)
	{((COleControl*)GetParent())->FireEvent(eventidMeasureRow + 100, EVENT_PARAM(VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_PVARIANT), Row, hDC, nWidth, pHeight);}

	void FireDrawItem(LPDISPATCH Row, LPDISPATCH Column, LPDISPATCH Item, OLE_HANDLE hDC, CRect rc, BOOL* pbDoDefault)
	{((COleControl*)GetParent())->FireEvent(eventidDrawItem,EVENT_PARAM(VTS_DISPATCH VTS_DISPATCH VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL),
	Row, Column, Item, hDC, rc.left, rc.top, rc.right, rc.bottom, pbDoDefault); }

	void FireDrawItemV(LPDISPATCH Row, LPDISPATCH Column, LPDISPATCH Item, OLE_HANDLE hDC, CRect rc, VARIANT* pbDoDefault)
	{((COleControl*)GetParent())->FireEvent(eventidDrawItem + 100,EVENT_PARAM(VTS_DISPATCH VTS_DISPATCH VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT),
	Row, Column, Item, hDC, rc.left, rc.top, rc.right, rc.bottom, pbDoDefault); }

	void FireDrawPreviewItem(LPDISPATCH Row, LPDISPATCH Item, OLE_HANDLE hDC, CRect rc, BOOL* pbDoDefault)
	{((COleControl*)GetParent())->FireEvent(eventidDrawPreviewItem,EVENT_PARAM(VTS_DISPATCH VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL),
	Row,  Item, hDC, rc.left, rc.top, rc.right, rc.bottom, pbDoDefault); }

	void FireDrawPreviewItemV(LPDISPATCH Row, LPDISPATCH Item, OLE_HANDLE hDC, CRect rc, VARIANT* pbDoDefault)
	{((COleControl*)GetParent())->FireEvent(eventidDrawPreviewItem + 100,EVENT_PARAM(VTS_DISPATCH VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT),
	Row,  Item, hDC, rc.left, rc.top, rc.right, rc.bottom, pbDoDefault); }

	void FireMeasurePreviewItem(LPDISPATCH Row, OLE_HANDLE hDC, int nWidth, int* pnHeight)
	{((COleControl*)GetParent())->FireEvent(eventidMeasurePreviewItem,EVENT_PARAM(VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_PI4), Row, hDC, nWidth, pnHeight);}

	void FireMeasurePreviewItemV(LPDISPATCH Row, OLE_HANDLE hDC, int nWidth, VARIANT* pHeight)
		{((COleControl*)GetParent())->FireEvent(eventidMeasurePreviewItem + 100, EVENT_PARAM(VTS_DISPATCH VTS_HANDLE VTS_I4 VTS_PVARIANT), Row, hDC, nWidth, pHeight);}
	//}}AFX_EVENT
#endif
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	void OnButton(UINT nFlags, CPoint point, CXTPReportBehaviorRowMouseButton* behavior);

	int _GetFocusedRowType() const;

	int _GetFocusedRowIndex() const;

	BOOL _SetFocusedRow(CXTPReportRow *pRow);

	BOOL _SetSelectedRow(CXTPReportRow *pRow, int nFocusedRowIndex, BOOL bShiftKey, BOOL bControlKey);


	void DrawDefaultGrid(CDC* pDC, CRect rcClient, int nRowHeight, int nLeftOffset);
	void RedrawScrollBar(int nBar);

	void GetColumnTotalSize(int &nTotalCount, int &nTotalWidth) const;
	void GetColumnOutOfViewSize(int &nOutOfViewCount, int &nOutOfViewWidth) const;
	void GetColumnScrollSize(int nOutOfViewWidth, int &nScrollCount, int &nScrollWidth) const;

	void UpdateScrollBarVisibility(int nBar);

public:

	BOOL m_bThemeMetrics;                       // TRUE to allow themes to override control metrics.
	BOOL m_bMovePivot;                          // Tells whether the freeze column is displayed or not when columns are added to the group by box.
	BOOL m_bStrictFiltering;                    // Tells whether strict filtering is enabled or not.

	BOOL m_bForcePagination;                    // Specifies whether to force the report to be split up into "pages" while in print preview mode.
	BOOL m_bSelectionExcludeGroupRows;          // TRUE if selection exclude group rows

	XTPReportInputState m_mouseDownState;   // MouseDown state

	DWORD m_dwLastMouseMessage;

	CXTPReportRow* m_pHotExpandButtonRow;       // Row with hot expand button
	CXTPReportHyperlink *m_pHotHyperlink;       // Hot hyperlink

protected:

	CXTPReportBehavior* m_pBehavior;  //

	BOOL m_bDoubleBuffering;        // Enables double-buffering
	BOOL m_bChanged;                // Internal member for storing changed flag.
	BOOL m_bInternalMove;           // Internal member for storing DD run-time flag.

	int m_nLockUpdateCount;         // A counter of the update locks. An image will be redrawn only when the lock counter is equal to zero.

	CRect m_rcGroupByArea;          // The area occupied by Group By item.
	CRect m_rcHeaderArea;           // The area occupied by the header.
	CRect m_rcFooterArea;           // The area occupied by the footer.

	CXTPReportColumns* m_pColumns;  // List columns container.

	CXTPReportRows *m_pRows;        // Virtual list rows container. Used for changing order, etc.

	CXTPReportHeader   *m_pReportHeader;        // List header member.

	CXTPReportSections *m_pSections;
	CXTPReportSection  *m_pSectionHeader;
	CXTPReportSection  *m_pSectionBody;
	CXTPReportSection  *m_pSectionFooter;
	CXTPReportSection  *m_pSectionScroll;

	CXTPMarkupContext      *m_pMarkupContext;   // Markup context
	CXTPReportPaintManager *m_pPaintManager;    // Paint manager
	CXTPReportNavigator    *m_pNavigator;       // Navigator

	CXTPReportNavigator     *m_pNavigatorReport;
	CXTPReportIconNavigator *m_pNavigatorIcon;

	//////////////////////////////////////////////////////////////////////////
	// Scrolling
	//////////////////////////////////////////////////////////////////////////

	XTPReportScrollUpdateMode m_scrollUpdateMode; // Scroll update mode
	XTPReportScrollMode m_scrollModeH;   // Horizontal scroll mode
	XTPReportScrollMode m_scrollModeV;   // Vertical scroll mode

	int m_nScrollOffsetH;                // Horizontal scroll position.

	int m_nScrollStepH;                  // Horizontal scroll step (in pixels).
	int m_nScrollStepV;                  // Vertical scroll step (in pixels).


	int m_nFixedColumnsCount;

	int m_nDisableReorderColumnsCount;          // Count of columns at the left side where reordering is disabled.



	CXTPReportSelectedRows* m_pSelectedRows;    // Container for the currently selected rows.

	CXTPReportRow* m_pHotRow;                   // Hot row

	int m_nRowsPerWheel;                        // Amount of rows to scroll by mouse wheel.

	CXTPReportTip* m_pTip;                       // Tip window.
	CBitmap m_bmpCache;                         // Cached window bitmap.

	BOOL m_bGroupByEnabled;                     // TRUE if Group By box is enabled
	BOOL m_bSortRecordChilds;                   // TRUE to apply sort order for Record children.

	XTPReportMouseMode m_mouseMode;             // Current mouse operation mode
	BOOL m_bBlockSelection;                     // TRUE if multiple selection enabled.
	BOOL m_bMultiSelectionMode;                 // TRUE if multi selection mode enabled (i.e. VK_CTRL is always on).
	BOOL m_bShowTooltips;                       // TRUE if showing tool tips enabled.
	BOOL m_bAutoCheckItems;                     // TRUE to enable auto check mode, FALSE to disable auto check mode.

	CXTPImageManager* m_pImageManager;          // Contains image list for report control
	BOOL m_bSkipGroupsFocus;                    // TRUE if group rows are skipped when navigating rows with the Up and Down arrow keys

	BOOL m_bLockExpand;                         // TRUE if collapsing/expanding rows is locked.
	BOOL m_bEnsureFocusedRowVisible;            // TRUE if focused rows will be forced visible
	BOOL m_bFocusSubItems;                      // TRUE if sub-items can receive focus.
	BOOL m_bEditOnClick;                        // TRUE if sub-items become editable on a single-click
	BOOL m_bEditOnDelayClick;                   // TRUE if sub-items become editable on a delay-click
	BOOL m_bEditOnDoubleClick;                  // TRUE if sub-items become editable on a double-click

	BOOL m_bPreviewAllowEdit;                   // TRUE if Preview of the row can be edited.
	BOOL m_bHeaderVisible;                      // TRUE if column headers are visible.
	BOOL m_bFooterVisible;                      // TRUE if column footer are visible.
	BOOL m_bPinFooterRows;                      // TRUE if footer rows are drawn immediately after the body rows.
	BOOL m_bPinFooterRowsPrinted;               // TRUE if footer rows are printed immediately after the body rows.
	BOOL m_bInitialSelectionEnable;             // TRUE if Initial (in Populate() call) selection enabled.
	BOOL m_bRowFocusVisible;                    // TRUE if showing focused row rectangle enabled.

	CXTPReportColumn* m_pFocusedColumn;         // Pointer to the currently focused CXTPReportColumn.

	CXTPToolTipContext* m_pToolTipContext;              // Tool tip Context.
	CXTPReportRecordItem* m_pActiveItem;                // Pointer to the currently focused CXTPReportRecordItem.
	CXTPReportInplaceEdit* m_pInplaceEdit;              // In-place edit pointer
	CXTPReportInplaceButtons* m_pInplaceButtons;        // In-place buttons pointer
	CXTPReportInplaceList* m_pInplaceList;              // In-place list pointer
	BOOL m_bVScrollBarVisible;                          // TRUE if vertical scroll bar is visible
	BOOL m_bHScrollBarVisible;                          // TRUE if horizontal scroll bar is visible
	UINT m_nAutoVScrollTimerResolution;                 // Vertical scrolling timer resolution in milliseconds
	CPoint m_ptDrag;                                    // Drag position
	BOOL m_bPrepareDrag;                                // TRUE if user click the report control and doesn't release button.
	CString m_strFilterText;                            // Filter text.
	BOOL m_bFilterHiddenColumns;                        // Search filter text in hidden columns too.
	int m_nRecordsTreeFilterMode;                       // Tree Filter mode.

	CLIPFORMAT m_cfReport;                              // Report Clipboard format for drag/drop operations

	CReportDropTarget* m_pDropTarget;                   // Internal drag/drop helper.
	BOOL m_bDragMode;                                   // TRUE if records currently dragging
	BOOL m_bInternalDrag;                               // TRUE if records begin drag from this control
	DWORD m_dwDragDropFlags;                            // Drag/drop flags.
	DWORD m_dwDropMarkerFlags;                          // The drop marker flags.
	CXTPReportSelectedRows* m_pSelectedRowsBeforeDrag;  // The selected rows before a dragging action.
	int m_nDropPos;                                     // Position of records to be dropped
	CXTPReportRecords* m_pDropRecords;                  // Drop records.

	BOOL        m_bAdjustLayoutRunning;                 // TRUE if AdjustLayout handler is entered, FALSE otherwise. Used to prevent OnSize reenter and stack overflow in Win95/98/ME.
	UINT_PTR    m_uAutoScrollTimerID;                   // Auto scroll timer ID or 0.

	//{{AFX_CODEJOCK_PRIVATE
	long m_nOLEDropMode;                                // Store OLE drop mode.
	BOOL m_nOLEDropAbove;                               // Drop above record?
	//}}AFX_CODEJOCK_PRIVATE

	CXTPReportRow* m_ptrVirtualEditingRow;              // Currently editing row in virtual mode.

	XTPReportRowsCompareFunc m_pRowsCompareFunc;   // Pointer to rows compare function.

	HBITMAP m_hbmpWatermark;                            // Watermark bitmap handle.
	BYTE m_WatermarkTransparency;                       // Watermark bitmap transparency value.
	BITMAP m_bmWatermark;                               // Watermark bitmap info.
	XTPReportWatermarkAlignment m_WatermarkAlignment;   // Watermark alignment flags.
	int m_nEnsureVisibleRowIdx;                         // Ensure visible row index.
	int m_nEnsureVisibleColumnIdx;                      // Ensure visible column index.

	CXTPReportDataManager* m_pDataManager;              // Data manager.

	BOOL m_bShowIconWhenEditing;                        // Set to TRUE to show item icons while the item is being edited.

#ifdef _XTP_ACTIVEX

public:
	BOOL m_bCustomDrawItem;
	BOOL m_bCustomDrawPreviewItem;
	BOOL m_bCustomMeasureRow;
	BOOL m_bCustomBeforeDrawRow;
	BOOL m_bCustomMeasurePreviewItem;

#endif

private:
	XTP_NM_REPORTTOOLTIPINFO* m_pCachedToolTipInfo;

public:



	int m_iIconWidth;               // icon geometry settings
	int m_iIconHeight;              // icon geometry settings
	int m_iIconWidthSpacing;        // icon geometry settings
	int m_iIconHeightSpacing;       // icon geometry settings

	int m_iIconPropNum;             // icon view setting
	int m_iIconNum;                 // icon view setting

	BOOL m_bMarkupEnabled;          //flag to check markup settings
	int m_iIconViewColumn;          // icon view setting
	BOOL m_bIconColumnIndexNotValid;     // used to indicate icon column is created, but does not yet have a valid index




	BOOL m_bStrictBestFit;          // use BestFit only for non-autosize mode
	BOOL m_bWasShiftKey;            // Flag set on MouseUp or KeyUp if Shift key was On
	BOOL m_bSortedDragDrop;         // Flag to set Drag Drop mode:
	                                // like Vista Windows Explorer or like XP Windows Explorer
	BOOL m_bTrapTabKey;             // Flag to set Trap Tab key in m_bEditOnClick && m_bAllowEdit case
	BOOL m_bDesktopTrackerMode;     // Flag to set Vista Tracker Mode On or Off
	BOOL m_bUnrestrictedDragDrop;   // Child can be drop to any pos - default = FALSE
	BOOL m_bFreeHeightMode;         // Flag to set RC Free Height Mode
	int m_nDefaultRowFreeHeight;    // RowHeight for initialization
	CString m_sCustomTitle;         // used for unique title - e.g. for PrintJob name
	BOOL m_bRClickDrag;             // Allow drag on right click
	BOOL m_bKeepSelectionAfterSort; // Tells whether to keep the selection after sorting items.

	int m_iColumnForNum;            // Index of the Row # column is used

protected:

	BOOL m_bCreated;                            // TRUE if created using the Create() method
	BOOL m_bIconView;                           // TRUE if icon view, FALSE else.
	// Height of a grid cell for items in large icon view, in pixels. Each item fits into a rectangle of size SM_CXICONSPACING by SM_CYICONSPACING when arranged. This value is always greater than or equal to SM_CYICON.
	int m_iColumnForNumPrev;
	int m_iIconViewRowsPerLine;                 //The icon view rows per line.
	BOOL m_bUseIconColumnForNum;
	CXTPReportColumnOrder* m_pPrevVisible;      //The pervious visible column order.
	CXTPReportColumnOrder* m_pPrevGroupsOrder;  //The pervious group column order.
	// Visible columns before setting icon view
	BOOL m_bPrevFocusSubItems;                  //TRUE if there are previous focused sub items, FALSE if not.
	BOOL m_bPrevHeaderAutoSize;                 //TRUE if the previous header is auto size, FALSE if not.

	BOOL m_bPrevHeaderRows;                     //TRUE if there are previous header rows, FALSE if not.
	BOOL m_bPrevFooterRows;                     //TRUE if there are previous footer rows, FALSE if not.
	int m_nPrevTreeIndent;                      //The tree indentation.
	BOOL m_bPrevHeaderShow;                     //Tells whether the previous header is visible or not.
	BOOL m_bPrevFooterShow;                     //Tells whether the previous footer is visible or not.
	BOOL m_bPrevPreviewMode;                    //Tells whether the preview mode is enabled or not previously.

	BOOL m_bNoNeedSortedDragDrop;               // Dynamic flag during Sorted DragDrop operation
	//(set during OnDragOver state - used during OnDrop state)

	UINT_PTR m_uiDelayEditTimer;                //The delay edit timer.
	UINT m_uiDelayEditMaxTime;                  //The delay edit time interval.

	int m_iLastRqstEditRow;                     //The last row which requested a delay edit.
	int m_iLastRqstEditCol;                     //The last column which requested a delay edit.

//ICON_VIEW_MODE RELATED <<
	UINT_PTR m_uRqstEditTimer;                  //The edit timer id.
	BOOL m_bFastDeselectMode;                   //Tells whether the fast deselect mode is enabled or not.
	XTPReportPaintTheme m_themeCurrent;         //Currently set theme.

	friend class CReportControlCtrl;
	friend class CReportDropTarget;
	friend class CXTPReportPaintManager;
	friend class CXTPReportSections;
	friend class CXTPReportSection;
};

//===========================================================================
// Summary:
//      Helper class with static member functions to control active locale for
//      Report Control and perform some locale dependent tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportControlLocale
{
private:
	CXTPReportControlLocale(){};
public:

	//-----------------------------------------------------------------------
	// Summary:
	//      Determine which locale is used active locale: current user locale
	//      or resource file locale.
	// Returns:
	//      TRUE if resource file locale is used active locale, FALSE otherwise.
	// See Also:
	//      SetUseResourceFileLocale, CXTPResourceManager::GetResourcesLangID,
	//      GetActiveLCID, LOCALE_USER_DEFAULT
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL IsUseResourceFileLocale();

	//-----------------------------------------------------------------------
	// Summary:
	//      Used to set which locale is used active locale: current user locale
	//      or resource file locale.
	// Parameters:
	//      bUseResourceFileLocale - If TRUE resource file locale will be used,
	//                               if FALSE current user locale will be used.
	// See Also:
	//      IsUseResourceFileLocale, CXTPResourceManager::GetResourcesLangID,
	//      GetActiveLCID, LOCALE_USER_DEFAULT
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetUseResourceFileLocale(BOOL bUseResourceFileLocale);

	//-----------------------------------------------------------------------
	// Summary:
	//      Returns active locale ID (current user locale or resource file locale)
	// Returns:
	//      LOCALE_USER_DEFAULT or resource file locale ID.
	// See Also:
	//      IsUseResourceFileLocale, SetUseResourceFileLocale,
	//      CXTPResourceManager::GetResourcesLangID, LOCALE_USER_DEFAULT
	//-----------------------------------------------------------------------
	static LCID AFX_CDECL GetActiveLCID();

	//-----------------------------------------------------------------------
	// Summary:
	//      Use to change VARIANT type using active locale ID.
	// Parameters:
	//      rVarValue   - [in, out] Reference to VARIANT value to change type.
	//      vartype     - [in] new variant type.
	//      bThrowError - [in] if FALSE function returns TRUE or FALSE,
	//                         if TRUE function throw exception in case of error.
	// Returns:
	//      TRUE if successful, FALSE otherwise.
	// See Also:
	//      GetActiveLCID, ::VariantChangeTypeEx API function.
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL VariantChangeTypeEx(VARIANT& rVarValue, VARTYPE vartype, BOOL bThrowError = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Format a string using strftime() function format specifiers.
	//      The active locale ID is used.
	// Parameters:
	//      dt                - [in] A COleDateTime object with date and time value to format.
	//      lpcszFormatString - [in] Format-control string.
	// Returns:
	//      A string which contains date-time formatted using active locale ID.
	// See Also:
	//      GetActiveLCID, strftime
	//-----------------------------------------------------------------------
	static CString AFX_CDECL FormatDateTime(const COleDateTime& dt, LPCTSTR lpcszFormatString);

private:
	static BOOL s_bUseResourceFileLocale;

private:
	static CString AFX_CDECL _FormatDateTime(const COleDateTime& dt, LPCTSTR lpcszFormatString, LCID lcLocaleID);

	static void AFX_CDECL _InitMappedSpecs();

	static void AFX_CDECL _ProcessMappedSpecs(CString& rstrFormat, const SYSTEMTIME* pST, LCID lcLocaleID);
	static void AFX_CDECL _ProcessDateTimeSpecs(CString& rstrFormat, const SYSTEMTIME* pST, LCID lcLocaleID);
	static void AFX_CDECL __ProcessDate_x(CString& rstrFormat, const SYSTEMTIME* pST, LCID lcLocaleID);
	static void AFX_CDECL __ProcessTime_X(CString& rstrFormat, const SYSTEMTIME* pST, LCID lcLocaleID);
	static void AFX_CDECL _ProcessOtherSpecs(CString& rstrFormat, const COleDateTime& dt);

private:
	struct XTP_TIMESPEC
	{
		LPCTSTR pcszSpec;
		LPCTSTR pcszFormat;
		BOOL    bTime;
	};

	static CArray<XTP_TIMESPEC, XTP_TIMESPEC&> s_arMappedSpecs;

	static void AFX_CDECL _AddsMappedSpec(LPCTSTR pcszSpec, LPCTSTR pcszFormat, BOOL bTime);
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPReportPaintManager* CXTPReportControl::GetPaintManager() const {
	return m_pPaintManager;
}
AFX_INLINE CXTPReportNavigator* CXTPReportControl::GetNavigator() const {
	return m_pNavigator;
}
AFX_INLINE CXTPReportSelectedRows* CXTPReportControl::GetSelectedRows() const {
	return m_pSelectedRows;
}

AFX_INLINE CXTPReportColumns* CXTPReportControl::GetColumns() const {
	return m_pColumns;
}
AFX_INLINE XTPReportMouseMode CXTPReportControl::GetMouseMode() const {
	return m_mouseMode;
}
AFX_INLINE BOOL CXTPReportControl::IsChanged() const {
	return m_bChanged;
}
AFX_INLINE void CXTPReportControl::SetChanged(BOOL bChanged) {
	m_bChanged = bChanged;
}
AFX_INLINE void CXTPReportControl::ShowGroupBy(BOOL bEnable) {
//  ASSERT(!IsVirtualMode());
	m_bGroupByEnabled = bEnable;
	AdjustLayout();
	AdjustScrollBars();
}
AFX_INLINE BOOL CXTPReportControl::IsGroupByVisible() const {
	return m_bGroupByEnabled;
}
AFX_INLINE void CXTPReportControl::ShowHeader(BOOL bShow /*= TRUE*/) {
	m_bHeaderVisible = bShow;
	AdjustLayout();
	AdjustScrollBars();
}
AFX_INLINE BOOL CXTPReportControl::IsHeaderVisible()const {
	return m_bHeaderVisible;
}
AFX_INLINE void CXTPReportControl::ShowFooter(BOOL bShow /*= TRUE*/) {
	m_bFooterVisible = bShow;
	AdjustLayout();
	AdjustScrollBars();
}
AFX_INLINE BOOL CXTPReportControl::IsFooterVisible()const {
	return m_bFooterVisible;
}
AFX_INLINE BOOL CXTPReportControl::IsMultipleSelection() const {
	return m_bBlockSelection;
}
AFX_INLINE void CXTPReportControl::SetMultipleSelection(BOOL bSet) {
	m_bBlockSelection = bSet;
}

AFX_INLINE BOOL CXTPReportControl::IsMultiSelectionMode() const
{
	return m_bMultiSelectionMode;
}

AFX_INLINE void CXTPReportControl::SetMultiSelectionMode(BOOL bSet)
{
	m_bMultiSelectionMode = bSet;
}

AFX_INLINE void CXTPReportControl::EnableToolTips(BOOL bEnable) {
	m_bShowTooltips = bEnable;
}
AFX_INLINE void CXTPReportControl::SkipGroupsFocus(BOOL bSkipFocus) {
	m_bSkipGroupsFocus = bSkipFocus;
}
AFX_INLINE BOOL CXTPReportControl::IsSkipGroupsFocusEnabled()const {
	return m_bSkipGroupsFocus;
}
AFX_INLINE CXTPImageManager* CXTPReportControl::GetImageManager() const {
	return m_pImageManager;
}
AFX_INLINE CXTPReportColumn* CXTPReportControl::GetFocusedColumn() const {
	return m_pFocusedColumn;
}
AFX_INLINE void CXTPReportControl::FocusSubItems(BOOL bFocusSubItems) {
	m_bFocusSubItems = bFocusSubItems;
	m_pFocusedColumn = NULL;
}
AFX_INLINE CXTPReportInplaceEdit* CXTPReportControl::GetInplaceEdit() const {
	return m_pInplaceEdit;
}
AFX_INLINE CXTPReportInplaceButtons* CXTPReportControl::GetInplaceButtons() const {
	return m_pInplaceButtons;
}
AFX_INLINE CXTPReportInplaceList* CXTPReportControl::GetInplaceList() const {
	return m_pInplaceList;
}
AFX_INLINE CXTPReportRecordItem* CXTPReportControl::GetActiveItem() const {
	return m_pActiveItem;
}
AFX_INLINE void CXTPReportControl::EnsureFocusedRowVisible(BOOL bEnsureFocusedRowVisible) {
	m_bEnsureFocusedRowVisible = bEnsureFocusedRowVisible;
}
AFX_INLINE BOOL CXTPReportControl::IsEnsureFocusedRowVisible() const {
	return m_bEnsureFocusedRowVisible;
}

AFX_INLINE BOOL CXTPReportControl::IsFocusSubItems() const
{
	return m_bFocusSubItems;
}

AFX_INLINE BOOL CXTPReportControl::IsDoubleBuffering() const
{
	return m_bDoubleBuffering;
}

AFX_INLINE void CXTPReportControl::EditOnClick(BOOL bEditOnClick)
{
	if (bEditOnClick)
	{
		m_bEditOnDelayClick = FALSE;
	}
	m_bEditOnClick = bEditOnClick;
}

AFX_INLINE BOOL CXTPReportControl::IsEditOnClick() const
{
	return m_bEditOnClick;
}

AFX_INLINE void CXTPReportControl::EditOnDelayClick(BOOL bEditOnDelayClick)
{
	if (bEditOnDelayClick)
	{
		m_bEditOnClick = FALSE;
	}
	m_bEditOnDelayClick = bEditOnDelayClick;
}

AFX_INLINE BOOL CXTPReportControl::IsEditOnDelayClick() const
{
	return m_bEditOnDelayClick;
}

AFX_INLINE void CXTPReportControl::EditOnDoubleClick(BOOL bEditOnClick)
{
	m_bEditOnDoubleClick = bEditOnClick;
}

AFX_INLINE BOOL CXTPReportControl::IsEditOnDoubleClick() const
{
	return m_bEditOnDoubleClick;
}

AFX_INLINE int CXTPReportControl::GetLastRqstEditRow() const
{
	return m_iLastRqstEditRow;
}

AFX_INLINE int CXTPReportControl::GetLastRqstEditCol() const
{
	return m_iLastRqstEditCol;
}

// Information for delay editing.
AFX_INLINE UINT_PTR CXTPReportControl::GetDelayEditTimer() const
{
	return m_uiDelayEditTimer;
}

AFX_INLINE void CXTPReportControl::SetLastRqstEdit(int iLastRqstEditRow, int iLastRqstEditCol)
{
	m_iLastRqstEditRow = iLastRqstEditRow;
	m_iLastRqstEditCol = iLastRqstEditCol;
}

AFX_INLINE CString CXTPReportControl::GetFilterText()
{
	return m_strFilterText;
}

AFX_INLINE void CXTPReportControl::SetFilterText(LPCTSTR strFilterText)
{
	m_strFilterText = strFilterText;
}

AFX_INLINE CXTPReportHeader* CXTPReportControl::GetReportHeader() const
{
	return m_pReportHeader;
}

AFX_INLINE void CXTPReportControl::SetAutoCheckItems(BOOL bAutoCheck)
{
	m_bAutoCheckItems = bAutoCheck;
}

AFX_INLINE BOOL CXTPReportControl::IsAutoCheckItems() const
{
	return m_bAutoCheckItems;
}

AFX_INLINE int CXTPReportControl::GetDisableReorderColumnsCount() const
{
	return m_nDisableReorderColumnsCount;
}

AFX_INLINE int CXTPReportControl::GetLockUpdateCount() const
{
	return m_nLockUpdateCount;
}

AFX_INLINE void CXTPReportControl::SetRowsCompareFunc(XTPReportRowsCompareFunc pCompareFunc)
{
	m_pRowsCompareFunc = pCompareFunc;
}

AFX_INLINE int CXTPReportControl::GetHScrollStep() const
{
	return m_nScrollStepH;
}

AFX_INLINE void CXTPReportControl::SetHScrollStep(int nStep)
{
	ASSERT(nStep > 0);
	m_nScrollStepH = max(1, nStep);
}

AFX_INLINE UINT CXTPReportControl::GetAutoVScrollTimerResolution() const
{
	return m_nAutoVScrollTimerResolution;
}

AFX_INLINE BOOL CXTPReportControl::IsFooterRowsPinned() const
{
	return m_bPinFooterRows;
}

AFX_INLINE void CXTPReportControl::PinFooterRowsPrinted(BOOL bPin)
{
	m_bPinFooterRowsPrinted = bPin;
}

AFX_INLINE BOOL CXTPReportControl::IsFooterRowsPinnedPrinted() const
{
	return m_bPinFooterRowsPrinted;
}

AFX_INLINE BOOL CXTPReportControl::IsPreviewAllowEdit() const
{
	return m_bPreviewAllowEdit;
}

AFX_INLINE void CXTPReportControl::PreviewAllowEdit(BOOL bAllowEdit)
{
	m_bPreviewAllowEdit = bAllowEdit;
}

AFX_INLINE void CXTPReportControl::SetSortRecordChilds(BOOL bSortRecordChilds)
{
	m_bSortRecordChilds = bSortRecordChilds;
}

AFX_INLINE BOOL CXTPReportControl::IsSortRecordChilds() const
{
	return m_bSortRecordChilds;
}

AFX_INLINE BOOL CXTPReportControl::IsFilterHiddenColumns() const
{
	return m_bFilterHiddenColumns;
}

AFX_INLINE void CXTPReportControl::SetFilterHiddenColumns(BOOL bFilterHidden)
{
	m_bFilterHiddenColumns = bFilterHidden;
}

AFX_INLINE int CXTPReportControl::GetRecordsTreeFilterMode() const
{
	return m_nRecordsTreeFilterMode;
}

AFX_INLINE void CXTPReportControl::SetRecordsTreeFilterMode(int nMode)
{
	m_nRecordsTreeFilterMode = nMode;
}

AFX_INLINE BOOL CXTPReportControl::IsInitialSelectionEnabled() const
{
	return m_bInitialSelectionEnable;
}

AFX_INLINE BOOL CXTPReportControl::IsRowFocusVisible() const
{
	return m_bRowFocusVisible;
}

AFX_INLINE void CXTPReportControl::ShowRowFocus(BOOL bShow)
{
	m_bRowFocusVisible = bShow;
}

AFX_INLINE int CXTPReportControl::GetWatermarkAlignment() const
{
	return m_WatermarkAlignment;
}

AFX_INLINE void CXTPReportControl::SetWatermarkAlignment(int nWatermarkAlignment)
{
	m_WatermarkAlignment = (XTPReportWatermarkAlignment)nWatermarkAlignment;
}

AFX_INLINE BOOL CXTPReportControl::IsIconView() const
{
	return m_bIconView;
}

AFX_INLINE int CXTPReportControl::GetRowsPerLine() const
{
	return m_iIconViewRowsPerLine;
}

AFX_INLINE int CXTPReportControl::GetLeftOffset() const
{
	return m_nScrollOffsetH;
}

AFX_INLINE void CXTPReportControl::SetDropMarkerFlags(DWORD dwDropMarkerFlags)
{
	m_dwDropMarkerFlags = dwDropMarkerFlags;
}

AFX_INLINE DWORD CXTPReportControl::GetDropMarkerFlags() const
{
	return m_dwDropMarkerFlags;
}

AFX_INLINE BOOL CXTPReportControl::IsDragMode() const
{
	return m_bDragMode;
}

AFX_INLINE void CXTPReportControl::SetFastDeselectMode(BOOL bFastDeselect)
{
	m_bFastDeselectMode = bFastDeselect;
}

AFX_INLINE CXTPReportBehavior* CXTPReportControl::GetBehavior() const
{
	return m_pBehavior;
}

AFX_INLINE void CXTPReportControl::LockExpand(BOOL bLockExpand)
{
	m_bLockExpand = bLockExpand;
}

AFX_INLINE BOOL CXTPReportControl::IsLockExpand() const
{
	return m_bLockExpand;
}

AFX_INLINE XTPReportPaintTheme CXTPReportControl::GetCurrentTheme() const
{
	return m_themeCurrent;
}



//----------------------------------------------------------------------
class CXTPPromptDlg : public CDialog
{
	DECLARE_DYNAMIC(CXTPPromptDlg )
public:
	CXTPPromptDlg (CWnd* pParent = NULL);
	virtual ~CXTPPromptDlg ();
	virtual void OnOK();
	virtual INT_PTR DoModal();
	virtual BOOL OnInitDialog();
	CString m_sName;
};

extern UINT XTP_WM_REPORT_SETTHEME;

#endif //#if !defined(__XTPREPORTCONTROL_H__)
