// XTPReportRows.h: interface for the CXTPReportRows class.
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
#if !defined(__XTPREPORTROWS_H__)
#define __XTPREPORTROWS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportRow;
class CXTPReportRecord;
class CXTPReportRecordItem;
class CXTPReportControl;
class CXTPReportColumns;

//===========================================================================
// Summary:
//     This enum defines selection change action.
// See Also: XTP_NM_REPORT_SELCHANGING, XTP_NM_SELECTION_CHANGING
//===========================================================================
enum XTPReportSelectionChangeType
{
	xtpReportSelectionAdd,      // Row will be added to selected rows collection.
	xtpReportSelectionRemove,   // Row will be removed from selected rows collection.
	xtpReportSelectionClear     // The selected rows collection will be cleared.
};

//===========================================================================
// Summary:
//     This struct defines data for XTP_NM_REPORT_SELCHANGING notification.
// See Also: XTP_NM_REPORT_SELCHANGING, XTP_NM_SELECTION_CHANGING
//===========================================================================
struct XTP_NM_SELECTION_CHANGING
{
	NMHDR hdr;                          // Standard structure, containing information
	                                    // about a notification message.
	CXTPReportRow* pRow;                // Pointer to the row associated with the
	                                    // notification. It is NULL for xtpReportSelectionClear action.
	XTPReportSelectionChangeType nType; // Selection change action.
};

//===========================================================================
// Summary:
//     This structure that contains information for use in processing the
//     XTP_NM_REPORT_STATECHANGED message.
//===========================================================================
struct XTP_NM_REPORTSTATECHANGED
{
	NMHDR hdr;    // Standard structure, containing information
	int   nBegin; // Zero-based index of the first item in the range of items.
	int   nEnd;   // Zero-based index of the last item in the range of items.
};

class _XTP_EXT_CLASS CXTPReportScreenRows : public CArray<CXTPReportRow*, CXTPReportRow*>
{
public:

	CXTPReportScreenRows();
	~CXTPReportScreenRows();

public:
	CXTPReportRow* HitTest(CPoint pt) const;
	void Clear();
};


class _XTP_EXT_CLASS CXTPReportRowsArray : public CArray<CXTPReportRow*, CXTPReportRow*>
{
	friend class CXTPReportSection;

public:
	void ReserveSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1)
	{
		if (GetSize() != 0)
		{
			ASSERT(FALSE);
			return;
		}

		SetSize(1, nNewSize);
		Add(NULL);
		SetSize(1, nGrowBy < 0 ? 0 : nGrowBy);

		m_nSize = 0;
	}
};


//===========================================================================
// Summary:
//     This class represents a rows collection class.
//     It supports an array of CXTPReportRow pointers.
// Example:
//     See CXTPReportRows::Add for an example of how to work with this class.
// See Also: CXTPReportRow, CXTPReportSelectedRows
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRows : public CXTPHeapObjectT<CXTPCmdTarget, CXTPReportDataAllocator>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPReportRow pointer array.
	// Example:
	// <code>
	// // Declare a local CXTPReportRows object.
	// CXTPReportRows myList;
	//
	// // Declare a dynamic CXTPReportRows object.
	// CXTPReportRows* pTree = new CXTPReportRows();
	// </code>
	//-----------------------------------------------------------------------
	CXTPReportRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPReportRows object. Performs cleanup operations.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns whether this row collection contains a specific row.
	//-----------------------------------------------------------------------
	virtual BOOL Contains(const CXTPReportRow *pRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all the elements from this array.
	// Parameters:
	//     bResetRow - TRUE to remove visible flag of child rows.
	// Remarks:
	//     Removes all the pointers from this array and releases instances
	//     of all stored CXTPReportRow objects.
	// Example:
	//     See example for CXTPReportRows::Add method.
	// See Also: CXTPReportRows overview
	//-----------------------------------------------------------------------
	virtual void Clear(BOOL bResetRow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of CXTPReportRow elements in this collection.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array.
	//     Because indexes are zero-based, the size is 1 greater than
	//     the largest index.
	// Example:
	//     See example for CXTPReportRows::Add method.
	// Returns:
	//     The number of items in the collection.
	// See Also: CXTPReportRows overview
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a row at the specified numeric index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	// Remarks:
	//     Returns the array element at the specified index.
	// Example:
	//     See example for CXTPReportRows::Add method.
	// Returns:
	//     The row element currently at this index.
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the currently displayed rows of this rows collection.
	//-----------------------------------------------------------------------
	virtual CXTPReportScreenRows *GetScreenRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the currently focused row index.
	//-----------------------------------------------------------------------
	virtual int GetFocusedRowIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns whether a row of the collection currently has the focus.
	//-----------------------------------------------------------------------
	virtual BOOL HasFocus() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the currently focused row index.
	//-----------------------------------------------------------------------
	virtual void SetFocusedRowIndex(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the currently focused row index.
	//-----------------------------------------------------------------------
	virtual CXTPReportRow *GetFocusedRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new Row element to the end of an array.
	// Parameters:
	//     pRow - The row element to be added to this array.
	// Remarks:
	//     Use this method to add the specified CXTPReportRow pointer
	//     to the end of the CXTPReportRows collection.
	// Returns:
	//     The index of the added row.
	// Example:
	// <code>
	// CXTPReportRows* pTree = new CXTPReportRows();
	// pTree->Add(new CXTPReportRow(pControl, pRecord1));
	// pTree->Add(new CXTPReportRow(pControl, pRecord2));
	// CXTPReportRow* pRow0 = pTree->GetAt(0);
	// CXTPReportRow* pRow1 = pTree->GetAt(1);
	// ASSERT(pRow0 == pTree->GetPrev(pRow1));
	// ASSERT(pRow1 == pTree->GetNext(pRow0));
	// pTree->RemoveAt(0);
	// ASSERT(1 == pTree->GetCount());
	// pTree->InsertAt(0, pRow0);
	// ASSERT(2 == pTree->GetCount());
	// pTree->Clear();
	// ASSERT(0 == pTree->GetCount());
	// </code>
	// See Also:
	//     CXTPReportRows overview, GetAt, InsertAt, Clear, GetNext, GetPrev, GetCount
	//-----------------------------------------------------------------------
	virtual int Add(CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes an item from the collection on specified position.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	// Remarks:
	//     In the process, it shifts down all the elements above the
	//     removed element. It decrements the upper bound of the array
	//     but does not free memory.
	//     It also releases an instance of the removed element.
	// Example:
	//     See example for CXTPReportRows::Add method.
	//-----------------------------------------------------------------------
	virtual void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes specified row from the collection.
	// Parameters:
	//     pRow   - The row element to be removed.
	// Returns:
	//     An integer value specifying the index of the removed item, -1 if item
	//     is not removed.
	//-----------------------------------------------------------------------
	virtual int RemoveRow(CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds one more Row object to the collection at the specified position.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	//     pRow   - The row element to be placed in this array.
	// Remarks:
	//     InsertAt inserts one element at a specified index in an array.
	//     In the process, it shifts up (by incrementing the index) the
	//     existing element at this index, and it shifts up all the elements
	//     above it.
	// Example:
	//     See example for CXTPReportRows::Add method.
	//-----------------------------------------------------------------------
	virtual void InsertAt(int nIndex, CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns next row in the list.
	// Parameters:
	//     pRow            - A reference to the current element of the list.
	//     bSkipGroupFocus - TRUE to skip all groups.
	// Remarks:
	//     This function finds a pointer to the provided row element
	//     in the list, and returns a pointer to the elements following
	//     the found one.
	// Returns:
	//     A pointer to a next element of the list.
	// Example:
	//     See example for CXTPReportRows::Add method.
	// See Also: GetPrev, CXTPReportControl::SkipGroupsFocus
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* GetNext(CXTPReportRow* pRow, BOOL bSkipGroupFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns previous row in the list.
	// Parameters:
	//     pRow            - A reference to the current element of the list.
	//     bSkipGroupFocus - TRUE to skip all groups.
	// Remarks:
	//     This function finds a pointer to the provided row element
	//     in the list, and returns a pointer to the elements before
	//     the found one.
	// Returns:
	//     A pointer to a previous element of the list.
	// Example:
	//     See example for CXTPReportRows::Add method.
	// See Also: GetNext, CXTPReportControl::SkipGroupsFocus
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* GetPrev(CXTPReportRow* pRow, BOOL bSkipGroupFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set virtual mode with nCount rows.
	// Parameters:
	//     pRow   - Virtual row
	//     nCount - Count of virtual rows
	//-----------------------------------------------------------------------
	virtual void SetVirtualMode(CXTPReportRow* pRow, int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find row corresponded with specified record
	// Parameters:
	//     pRecord  - Record need to find.
	// Returns:
	//     A pointer to associated row object or NULL.
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* Find(CXTPReportRecord *pRecord);
	virtual CXTPReportRow* Find(CXTPReportRecord *pRecord, BOOL bRecursive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find row corresponded with specified record.
	//     This function performs search in children too.
	// Parameters:
	//     pRecord  - Record need to find.
	// Returns:
	//     A pointer to associated row object or NULL.
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* FindInTree(CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find a position to insert a new row at.
	//     This function performs search in children too.
	// Parameters:
	//     pRow  - A row to insert.
	//     bInsertAfter  - the function sets it to TRUE if a row has to be inserted after the row found.
	// Returns:
	//     A pointer to the found row object or NULL.
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* FindInsertionPos(CXTPReportRow* pRow, BOOL& bInsertAfter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates child indices of all rows in the collection.
	// Parameters:
	//     bRunInChildren - if TRUE, indices are refreshed in children also.
	//-----------------------------------------------------------------------
	virtual void RefreshChildIndices(BOOL bRunInChildren = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts collection elements.
	// Parameters:
	//     pCompareFunc - A T_CompareFunc function pointer that is used
	//                    to compare rows.
	// Remarks:
	//     This method uses Visual C++ run-time library (MSVCRT)
	//     implementation of the quick-sort function, qsort, for sorting
	//     stored CXTPReportRow objects.
	//
	//     Sort() internally uses CompareRows method for comparing 2 rows.
	// See Also: CompareRows
	//-----------------------------------------------------------------------
	virtual void SortEx(XTPReportRowsCompareFunc pCompareFunc);
	virtual void Sort(); // <COMBINE SortEx>

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares 2 rows using groups and sort orders of the report control.
	// Parameters:
	//     pRow1 - First row for comparison.
	//     pRow2 - Second row for comparison.
	// Remarks:
	//     This function is called directly by Sort method.
	//
	//     The default implementation returns the result of the comparison
	//     of *pRow1 and *pRow2.
	//
	//     This implementation uses CXTPReportRecordItem::Compare for comparing
	//     two corresponding items from the row records. First it compares
	//     record items in the group order, then by sort order. It returns
	//     the first comparison result differ from equality. If no differences was
	//     found, it returns 0.
	//
	// Returns:
	//     Zero if pRow1 is equal to pRow2;
	//     Less than zero if pRow1 is less than pRow2;
	//     Greater than zero if pRow1 is greater than pRow2.
	// See Also: Sort, CXTPReportRecordItem::Compare
	//-----------------------------------------------------------------------
	static int _cdecl CompareRows(const CXTPReportRow** pRow1, const CXTPReportRow** pRow2);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares 2 rows using groups and sort orders of the report control.
	// Parameters:
	//     pRow1 - First row for comparison.
	//     pRow2 - Second row for comparison.
	// Remarks:
	//     This function is called directly by Sort method.
	//
	//     The default implementation returns the result of the comparison
	//     of *pRow1 and *pRow2.
	//
	//     This implementation uses CXTPReportRecordItem::Compare for comparing
	//     two corresponding items from the row records. First it compares
	//     record items in the group order, then by sort order. It returns
	//     the first comparison result differ from equality. If no differences was
	//     found, it returns 0.
	//
	// Returns:
	//     Zero if pRow1 is equal to pRow2;
	//     Less than zero if pRow1 is less than pRow2;
	//     Greater than zero if pRow1 is greater than pRow2.
	// See Also: Sort, CXTPReportRecordItem::Compare
	//-----------------------------------------------------------------------
	static int _cdecl CompareRows2(const CXTPReportRow** pRow1, const CXTPReportRow** pRow2); //<COMBINE CompareRows>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to search for a record item by rows.
	// Parameters:
	//     nStartIndex - The starting index.
	//     nEndIndex   - The end index.
	//     nStartColumn- The start column.
	//     nEndColumn  - The end column.
	//     nItem       - Record item index to start search from.
	//     nRecordIndex- Record index to start search from.
	//     pcszText    - The search text.
	//     nFlags      - The search flags.
	// Remarks:
	//     The various values for nFlags are,
	//     xtpReportTextSearchExactPhrase - Search exact phrase.
	//     xtpReportTextSearchMatchCase   - Match case during search.
	//     xtpReportTextSearchBackward    - Search backwards.
	//     xtpReportTextSearchExactStart  - Search phrase where the start of the phrase matches exactly.
	//                                      For those familiar with grep this is just like using the
	//                                      "^" to state that the text must start with the specified search.
	// Returns:
	//    Report record item found or NULL otherwise.
	//-----------------------------------------------------------------------
	virtual CXTPReportRecordItem* FindRecordItemByRows(
		int nStartIndex, int nEndIndex,
		int nStartColumn, int nEndColumn,
		int nRecordIndex, int nItem,
		LPCTSTR pcszText, int nFlags);

	//{{AFX_CODEJOCK_PRIVATE
	virtual void ReserveSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
	virtual void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);
	virtual void SetAt(INT_PTR nIndex, CXTPReportRow* pRow);
	//}}AFX_CODEJOCK_PRIVATE
protected:

	//{{AFX_CODEJOCK_PRIVATE

	CXTPReportRowsArray m_arrRows;   // Internal storage for CXTPReportRow objects.
	//}}AFX_CODEJOCK_PRIVATE

	CXTPReportRow  *m_pVirtualRow;   // Virtual row.
	CXTPReportScreenRows *m_pScreenRows;   // Rows currently presented on screen.
	int m_nVirtualRowsCount;         // Count of virtual rows.

	int m_nFocusedRow;                     // Current focused row index

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportRows);

	afx_msg LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();
	DECLARE_ENUM_VARIANT(CXTPReportRows)

	afx_msg LPDISPATCH OleFindRow(LPDISPATCH pRecordDisp);
	afx_msg LPDISPATCH OleFindRowInTree(LPDISPATCH pRecordDisp);
	afx_msg LPDISPATCH OleFindRecordItemByRows(int nStartIndex, int nEndIndex,
												int nStartColumn, int nEndColumn,
												int nRecord, int nItem,
												LPCTSTR pcszText, int nFlags);
	enum
	{
		dispidCount = 1L,
	};

//}}AFX_CODEJOCK_PRIVATE
#endif

	friend class CXTPReportControl;
	friend class CXTPTrackControl;
	friend class CXTPReportNavigator;
	friend class CXTPReportSection;
};

#endif //#if !defined(__XTPREPORTROWS_H__)
