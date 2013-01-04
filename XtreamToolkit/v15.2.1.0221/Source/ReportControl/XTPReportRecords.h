// XTPReportRecords.h: interface for the CXTPReportRecords class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPREPORTRECORDS_H__)
#define __XTPREPORTRECORDS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportRecord;
class CXTPPropExchange;
class CXTPMarkupContext;
class CXTPReportRecordItem;
class CXTPReportRecordItemRange;

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of text search parameters
// Remarks:
//     Call CXTPReportRecords::FindRecordItem to search for a record item by text
// Example:
//     <code>m_wndReport.GetRecords()->FindRecordItem(... , xtpReportTextSearchWholeWord | xtpReportTextSearchBackward);</code>
// See Also: CXTPReportRecords::FindRecordItem
//-----------------------------------------------------------------------
enum  XTPReportTextSearchParms
{
	xtpReportTextSearchExactPhrase = 1,         // Search exact phrase
	xtpReportTextSearchMatchCase   = 2,         // Match case during search
	xtpReportTextSearchBackward    = 4,         // Search backward
	xtpReportTextSearchExactStart  = 8,         // Search phrase starting in the beginning of item caption
};


//===========================================================================
// Summary:
//     This class represents a records collection class.
//     It supports an array of CXTPReportRecord pointers.
// Example:
//     See example for CXTPReportRecords::Add method.
// See Also: CXTPReportRecord
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecords : public CXTPHeapObjectT<CCmdTarget, CXTPReportDataAllocator>
{
	DECLARE_DYNAMIC(CXTPReportRecords)
	void _Init();
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPReportRecord pointer array.
	// Parameters:
	//  bArray - flag for array-based case (if TRUE)
	// Example:
	// <code>
	// // Declare a local CXTPReportRecords object.
	// CXTPReportRecords myList;
	//
	// // Declare a dynamic CXTPReportRecords object.
	// CXTPReportRecords* pTree = new CXTPReportRecords();
	// </code>
	//-----------------------------------------------------------------------
	CXTPReportRecords(BOOL bArray = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPReportRecord pointer array in case if it is
	//     used as a member of CXTPReportRecord object to store children.
	// Parameters:
	//     pOwnerRecord - A pointer to owner CXTPReportRecord object.
	//-----------------------------------------------------------------------
	CXTPReportRecords(CXTPReportRecord* pOwnerRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPReportRecords object. Performs cleanup operations.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportRecords();

	//-----------------------------------------------------------------------
	// Summary:
	//     Used to get CXTPReportRecord owner object;
	// Returns:
	//     A pointer to owner CXTPReportRecord object or NULL.
	//-----------------------------------------------------------------------
	CXTPReportRecord* GetOwnerRecord() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of CXTPReportRecord elements in this collection.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array.
	//     Because indexes are zero-based, the size is 1 greater than
	//     the largest index.
	// Returns:
	//     The number of items in the collection.
	// Example:
	//     See example for CXTPReportRecords::Add method.
	// See Also: CXTPReportRecords overview
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a record at the specified numeric index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	// Remarks:
	//     Returns the array element at the specified index.
	// Returns:
	//     The record element currently at this index.
	// Example:
	//     See example for CXTPReportRecords::Add method.
	//-----------------------------------------------------------------------
	CXTPReportRecord* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new Record element to the end of an array.
	// Parameters:
	//     pRecord - The record element to be added to this array.
	// Remarks:
	//     Use this method to add the specified CXTPReportRecord pointer
	//     to the end of the CXTPReportRecords collection.
	// Returns:
	//     A pointer to the report record object.
	// Example:
	// <code>
	// CXTPReportRecords* pList = new CXTPReportRecords();
	// pList->Add(new CXTPReportRecord());
	// pList->Add(new CXTPReportRecord());
	// CXTPReportRecord* pRecord0 = pList->GetAt(0);
	// CXTPReportRecord* pRecord1 = pList->GetAt(1);
	// ASSERT(2 == pList->GetCount());
	// pList->RemoveAll();
	// ASSERT(0 == pList->GetCount());
	// </code>
	// See Also: CXTPReportRecords overview, GetAt, RemoveAll, GetCount
	//-----------------------------------------------------------------------
	CXTPReportRecord* Add(CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report records
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all the elements from this array.
	// Remarks:
	//     Removes all the pointers from this array and releases instances
	//     of all stored CXTPReportRecord objects.
	// Example:
	//     See example for CXTPReportRecords::Add method.
	// See Also: CXTPReportRecords overview
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes record by its index.
	// Parameters:
	//     nIndex - index of record to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes specified record from collection.
	// Parameters:
	//     pRecord - Record to be removed.
	// Returns:
	//     Removed record index or -1 if specified record is not in this
	//     collection.
	//-----------------------------------------------------------------------
	int RemoveRecord(CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add record in the specified position.
	// Parameters:
	//     nIndex - Index of record to insert.
	//     pRecord - Record to be inserted.
	//-----------------------------------------------------------------------
	void InsertAt(int nIndex, CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to move records to specified position
	// Parameters:
	//     nIndex - index to move records
	//     pRecords - Records to move
	//-----------------------------------------------------------------------
	void Move(int nIndex, CXTPReportRecords* pRecords);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to move record to specified position
	// Parameters:
	//     nIndex - index to move records
	//     pRecord - Record to move
	//     bUpdateIndexes - flag to Update indexes to 0-N set
	//-----------------------------------------------------------------------
	void MoveRecord(int nIndex, CXTPReportRecord* pRecord, BOOL bUpdateIndexes = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary: Gets markup context
	// Returns: Returns markup context
	//-----------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets the markup context
	//-----------------------------------------------------------------------
	void SetMarkupContext(CXTPMarkupContext *pMarkupContext);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a value indicating whether string comparisons are case-sensitive.
	// Parameters:
	//     bCaseSensitive - TRUE if string comparisons are case-sensitive; otherwise, FALSE. The default is FALSE.
	// See Also: IsCaseSensitive
	//-----------------------------------------------------------------------
	void SetCaseSensitive(BOOL bCaseSensitive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines whether string comparisons are case-sensitive.
	// Returns:
	//     TRUE if case sensitive, FALSE else.
	// See Also: SetCaseSensitive
	//-----------------------------------------------------------------------
	BOOL IsCaseSensitive() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Helper function to compares two strings.
	// Parameters:
	//     str1 - First string to compare
	//     str2 - Second string to compare
	// Returns:
	//     Zero if the strings are identical, < 0 if this str1 object is less than str2,
	//     or > 0 if this str1 object is greater than str2.
	// See Also: SetCaseSensitive
	//-----------------------------------------------------------------------
	virtual int Compare(const CString& str1, const CString& str2) const;

		//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to change the array size.
	// Parameters:
	//     nNewSize - An int that contains the new array size.
	//     nGrowBy  - The minimum number of element slots to allocate if a size
	//                increase is necessary.
	//-----------------------------------------------------------------------
	virtual void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set a new element to the position
	//     specified in the nIndex parameter.
	// Parameters:
	//     nIndex      - An integer index that is greater than or equal
	//                   to 0 and less than the value returned by
	//                   GetCount.
	//     pRecord    - A pointer to a CXTPReportRecord object.
	//                  The element to add to the array.
	//-----------------------------------------------------------------------
	virtual void SetAt(INT_PTR nIndex, CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to record indexes.
	// Parameters:
	//     nStart      - An integer index to start update from.
	//-----------------------------------------------------------------------
	virtual void UpdateIndexes(int nStart = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to search for a record with the specified bookmark.
	// Parameters:
	//     vtBookmark - Bookmark to find.
	//     bSearchInChildren - TRUE to search in children too, FALSE otherwise.
	// Returns:
	//     Report record item found or NULL otherwise.
	//-----------------------------------------------------------------------
	virtual CXTPReportRecord* FindRecordByBookmark(VARIANT vtBookmark, BOOL bSearchInChildren);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to search for a record item by text.
	// Parameters:
	//     nStartRecord - Starting record index.
	//     nEndRecord - End record index.
	//     nStartColumn - Starting column index.
	//     nEndColumn - End column index.
	//     nRecord - Record index to start search from.
	//     nItem - Record item index to start search from.
	//     pcszText - Search text.
	//     nFlags - Search parameters.
	// Returns:
	//     Report record item found or NULL otherwise.
	//-----------------------------------------------------------------------
	virtual CXTPReportRecordItem* FindRecordItem(int nStartRecord, int nEndRecord,
												int nStartColumn, int nEndColumn,
												int nRecord, int nItem,
												LPCTSTR pcszText, int nFlags);


	void MergeItems(const CXTPReportRecordItemRange& range);


protected:
	//{{AFX_CODEJOCK_PRIVATE
	void SetVirtualMode(CXTPReportRecord* pVirtualRecord, int nCount);
	BOOL IsVirtualMode() const;
	virtual void _DoPropExchange(CXTPPropExchange* pPX);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CArray<CXTPReportRecord*, CXTPReportRecord*> m_arrRecords;  // Internal storage array for Record items.
	CXTPReportRecord  *m_pVirtualRecord;      // Virtual record.
	int m_nVirtualRecordsCount;               // Virtual records count.
	CXTPReportControl *m_pControl;            // Pointer to the parent report control.
	//{{AFX_CODEJOCK_PRIVATE
	BOOL m_bArray;
	//}}AFX_CODEJOCK_PRIVATE

	BOOL m_bCaseSensitive; // Indicating whether string comparisons are case-sensitive.

	CXTPReportRecord* m_pOwnerRecord; // Store pointer to owner record object (or NULL);

	CXTPMarkupContext* m_pMarkupContext; // Markup Context;



	friend class CXTPReportControl;
	friend class CXTPReportRecord;
	friend class CXTPReportSection;
};

AFX_INLINE BOOL CXTPReportRecords::IsVirtualMode() const
{
	return m_pVirtualRecord != NULL;
}

AFX_INLINE void CXTPReportRecords::SetCaseSensitive(BOOL bCaseSensitive)
{
	m_bCaseSensitive = bCaseSensitive;
}

AFX_INLINE BOOL CXTPReportRecords::IsCaseSensitive() const
{
	return m_bCaseSensitive;
}

AFX_INLINE CXTPReportRecord* CXTPReportRecords::GetOwnerRecord() const
{
	return m_pOwnerRecord;
}

AFX_INLINE CXTPMarkupContext* CXTPReportRecords::GetMarkupContext() const
{
	return m_pMarkupContext;
}

AFX_INLINE void CXTPReportRecords::SetMarkupContext(CXTPMarkupContext *pMarkupContext)
{
	m_pMarkupContext = pMarkupContext;
}

#endif //#if !defined(__XTPREPORTRECORDS_H__)
