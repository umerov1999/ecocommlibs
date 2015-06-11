// XTPReportSelectedRows.h: interface for the CXTPReportSelectedRows class.
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
#if !defined(__XTPREPORTSELECTEDROWS_H__)
#define __XTPREPORTSELECTEDROWS_H__
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
//     Encapsulates a collection of CXTPReportRow pointers that represent
//     the selected rows in a Report Control.
// Remarks:
//     Use this class to programmatically manage a collection of
//     CXTPReportRow pointers that represent the selected rows in a
//     Report Control. This class is commonly used to add or remove rows
//     from the collection.
//
//     Typical work flow is using Add and Remove methods for changing
//     contents of the selection and using Contains method for checking
//     a specific row for its presence in the selection.
// Example:
//     The following example demonstrates how to programmatically use
//     the CXTPReportSelectedRows class to select rows in the Report control.
// <code>
// CXTPReportSelectedRows* pSelRows = pReportControl->GetSelectedRows();
// pSelRows->Add(pRow1);
// pSelRows->Add(pRow2);
// ASSERT(TRUE == pSelRows->Contains(pRow1));
// ASSERT(TRUE == pSelRows->Contains(pRow2));
// pSelRows->Remove(pRow1);
// ASSERT(FALSE == pSelRows->Contains(pRow1));
// pSelRows->Select(pRow1);
// ASSERT(TRUE == pSelRows->Contains(pRow1));
// ASSERT(FALSE == pSelRows->Contains(pRow2));
// </code>
//
// See Also: CXTPReportRow, CXTPReportSelectedRows, CXTPReportControl::GetSelectedRows
//===========================================================================
class _XTP_EXT_CLASS CXTPReportSelectedRows : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPReportSelectedRows collection.
	// Parameters:
	//     pControl - Pointer to the parent report control.
	// Remarks:
	//     This collection could be created only in association with
	//     the CXTPReportControl object.
	// Example:
	// <code>
	// // from CXTPReportControl member function
	// CXTPReportSelectedRows* pSelectedRows = new CXTPReportSelectedRows(this);
	// </code>
	// See Also: CXTPReportSelectedRows overview
	//-----------------------------------------------------------------------
	CXTPReportSelectedRows(CXTPReportControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects a block of rows.
	// Parameters:
	//  nBlockBegin - First row index from the block.
	//  nBlockEnd   - Final row index from the block.
	//  bControlKey - BOOL flag to clear previous during new selection step
	// Remarks:
	//     This function uses nBlockBegin and nBlockEnd as the bound for the
	//     required selection. It enumerates parent report control rows collection
	//     and adds all rows from nBlockEnd to nBlockEnd inclusively to the selection.
	//-----------------------------------------------------------------------
	void SelectBlock(int nBlockBegin, int nBlockEnd, BOOL bControlKey = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears itself, removing selection.
	// Remarks:
	//     Removes all the elements from the selection.
	//-----------------------------------------------------------------------
	BOOL Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears itself, removing selection.
	// Remarks:
	//     Removes all the elements from the selection.
	// Parameters:
	//     bNotifyOnClear - TRUE to send clear notification.
	//-----------------------------------------------------------------------
	BOOL Clear(BOOL bNotifyOnClear);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a row to the selection.
	// Parameters:
	//     pRow - Pointer to the row to be added to the selection.
	// Remarks:
	//     This method adds a pointer to the provided row to the selection.
	//     After adding, Contains method will return TRUE for all
	//     checking attempts of this row pointer.
	// Example:
	//     See example at CXTPReportSelectedRows overview
	// See Also: CXTPReportSelectedRows overview, Remove, Select, Clear, Contains
	//-----------------------------------------------------------------------
	BOOL Add(CXTPReportRow *pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a rows to the selection.
	// Parameters:
	//     nIndexBegin - First row index of block to be selected.
	//     nIndexEnd - Last row index of block to be selected.
	//-----------------------------------------------------------------------
	void AddBlock(int nIndexBegin, int nIndexEnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a row from the selection.
	// Parameters:
	//     pRow - Pointer to the row to be removed from the selection.
	// Remarks:
	//     This method removes a provided row pointer from the selection.
	//     After removing, Contains method will return FALSE for all
	//     checking attempts of this row pointer.
	// Example:
	//     See example at CXTPReportSelectedRows overview
	// See Also: CXTPReportSelectedRows overview, Add, Select, Clear, Contains
	//-----------------------------------------------------------------------
	void Remove(CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a value indicating whether the CXTPReportSelectedRows
	//     contains the specified CXTPReportRow pointer.
	// Parameters:
	//     pRow - The CXTPReportRow pointer to search for in the CXTPReportSelectedRows.
	// Remarks:
	//     Use this method to determine whether the CXTPReportSelectedRows
	//     contains the specified CXTPReportRow pointer.
	// Returns:
	//     TRUE if row is contained in the selection, FALSE otherwise.
	// Example:
	//     See example at CXTPReportSelectedRows overview
	// See Also: CXTPReportSelectedRows overview, Add, Remove, Select, Clear
	//-----------------------------------------------------------------------
	BOOL Contains(const CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inverts selection for the specified row.
	// Parameters:
	//     pRow - Pointer to the specified row.
	// Remarks:
	//     This methods checks the specified method for its presence in
	//     the collection and adds or removes it in the reverse order
	//     depending on the result.
	// Example:
	// <code>
	// CXTPReportSelectedRows* pSelRows = pReportControl->GetSelectedRows();
	// ASSERT(TRUE == pSelRows->Contains(pRow1));
	// pSelRows->Invert(pRow1);
	// ASSERT(FALSE == pSelRows->Contains(pRow1));
	// pSelRows->Invert(pRow1);
	// ASSERT(TRUE == pSelRows->Contains(pRow1));
	// </code>
	// See Also: Add, Remove, Contains
	//-----------------------------------------------------------------------
	void Invert(CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Selects only the specified row.
	// Parameters:
	//     pRow - Pointer to the specified row.
	// Remarks:
	//     This method clears the initial selection and
	//     selects only the specified row.
	// Example:
	//     See example at CXTPReportSelectedRows overview
	// See Also: CXTPReportSelectedRows overview, Add, Remove, Contains, Clear
	//-----------------------------------------------------------------------
	BOOL Select(CXTPReportRow *pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of selected rows in the collection.
	// Remarks:
	//     Call this method to retrieve the number of selected rows
	//     in the array.
	// Returns:
	//     The number of items in the collection.
	// See Also: CXTPReportRows overview
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves selected row by index.
	// Parameters:
	//     nIndex - Index of selected row to retrieve.
	// Returns:
	//     A pointer to report row object.
	// Remarks:
	//     Recommended to use GetFirstSelectedRowPosition / GetNextSelectedRow methods.
	// See Also: GetFirstSelectedRowPosition, GetNextSelectedRow
	//-----------------------------------------------------------------------
	CXTPReportRow* GetAt (int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the first selected row in the list report control.
	// Returns:
	//     value that can be used for iteration or object pointer retrieval;
	//     NULL if no items are selected.
	// See Also: GetNextSelectedItem
	//-----------------------------------------------------------------------
	POSITION GetFirstSelectedRowPosition();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets next selected row in the list report control.
	// Parameters:
	//     pos - A reference to a POSITION value returned by a previous call to
	//           GetNextSelectedRow or GetFirstSelectedRowPosition.
	//           The value is updated to the next position by this call.
	// Returns:
	//     The pointer of the next selected row in the list report control.
	//-----------------------------------------------------------------------
	CXTPReportRow* GetNextSelectedRow(POSITION& pos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if selection was changed.
	// Returns:
	//     TRUE if the selection was changed and FALSE else.
	//-----------------------------------------------------------------------
	BOOL IsChanged() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to reset changed flag.
	// Parameters:
	//     bChanged - TRUE to reset.
	//-----------------------------------------------------------------------
	void SetChanged(BOOL bChanged = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//    Use this function to swap to integers.
	// Parameters:
	//     indexB - First value.
	//     indexE - The second integer.
	// Returns:
	//     TRUE if the operation is successful, FALSE else.
	//-----------------------------------------------------------------------
	BOOL SwapIfNeed(int& indexB, int& indexE);

	//-----------------------------------------------------------------------
	// Summary:
	//  This member used to enable\disable clear notifications during
	//  CXTPReportSelectedRows::Select and SelectBlock.
	// Parameters:
	//  bNotifyOnClear - TRUE, then calling CXTPReportSelectedRows::Select and SelectBlock
	//                   will send xtpReportSelectionClear notifications when
	//                   the selected rows are internally cleared (normal behavior),
	//                   if FALSE it will not send the clear notifications during
	//                   CXTPReportSelectedRows::Select and SelectBlock.
	//-----------------------------------------------------------------------
	void SetNotifyOnClear(BOOL bNotifyOnClear = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//  This member used determine whether clear notifications are sent during
	//  CXTPReportSelectedRows::Select and SelectBlock.
	// Returns:
	//  TRUE, then calling CXTPReportSelectedRows::Select and SelectBlock
	//  will send xtpReportSelectionClear notifications when
	//  the selected rows are internally cleared (normal behavior),
	//  if FALSE it will not send the clear notifications during
	//  CXTPReportSelectedRows::Select and SelectBlock.
	//-----------------------------------------------------------------------
	BOOL GetNotifyOnClear() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL _NotifySelChanging(XTPReportSelectionChangeType nType, CXTPReportRow* pRow = NULL);
	void _NotifyStateChanged(int nBegin, int nEnd);
	void _InsertBlock(int nIndexInsert, int nIndexBegin, int nIndexEnd);
	void _OnExpanded(int nIndex, int nCount);
	void _OnCollapsed(int nIndex, int nCount);

	struct SELECTED_BLOCK
	{
		int nIndexBegin;
		int nIndexEnd;
	};

	CXTPReportControl* m_pControl;      // Pointer to the parent report control.
	int m_nRowBlockBegin;               // Pointer to the row where rows block begin from.
	int m_nPosSelected;
	BOOL m_bNotifyOnClear; // If TRUE, then calling CXTPReportSelectedRows::Select and SelectBlock will send xtpReportSelectionClear notificatoins when the selected rows are internally cleared (normal behavior), if FALSE it will not send the clear notifications during CXTPReportSelectedRows::Select and SelectBlock

	CArray<SELECTED_BLOCK, SELECTED_BLOCK&> m_arrSelectedBlocks;

	BOOL m_bChanged;
	XTPReportRowType m_nRowType;        // Only rows of one type can be selected (i.e. body, header, or footer).
	//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPReportSelectedRows);

	afx_msg void OleAdd(LPDISPATCH lpRow);
	afx_msg void OleDeleteAll();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();

	DECLARE_ENUM_VARIANTLIST(CXTPReportSelectedRows);

	enum
	{
		dispidCount = 1L,
	};

//}}AFX_CODEJOCK_PRIVATE
#endif
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPReportControl;
	friend class CXTPReportSection;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE void CXTPReportSelectedRows::SetNotifyOnClear(BOOL bNotifyOnClear)
{
	m_bNotifyOnClear = bNotifyOnClear;
}

AFX_INLINE BOOL CXTPReportSelectedRows::GetNotifyOnClear() const
{
	return m_bNotifyOnClear;
}

AFX_INLINE BOOL CXTPReportSelectedRows::IsChanged() const
{
	return m_bChanged;
}

AFX_INLINE void CXTPReportSelectedRows::SetChanged(BOOL bChanged)
{
	m_bChanged = bChanged;
}


#endif //#if !defined(__XTPREPORTSELECTEDROWS_H__)
