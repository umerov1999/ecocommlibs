// XTPReportSection.h: interface for the CXTPReportSection class.
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
#if !defined(__XTPREPORTSECTION_H__)
#define __XTPREPORTSECTION_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportControl;
class CXTPReportRecords;
class CXTPReportRecord;
class CXTPReportRows;
class CXTPReportRow;
class CXTPReportSelectedRows;
class CXTPReportBorder;


//-----------------------------------------------------------------------
// Summary:
//     Section height algorithm.
//-----------------------------------------------------------------------
enum XTPReportSectionHeightMode
{
	xtpReportSectionHeightModeFixed,      // Equal to 'px'   in HTML
	xtpReportSectionHeightModeRelative,   // Equal to '%'    in HTML
	xtpReportSectionHeightModeAuto,       // Equal to 'auto' in HTML
	xtpReportSectionHeightModeExpand,     // Equal to '*'    in HTML

	xtpReportSectionHeightModeAutoExpand, // Minimum auto size
	xtpReportSectionHeightModeAutoShrink, // Maximum auto size
};


//-----------------------------------------------------------------------
// Summary:
//     A section represents
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPReportSection : public CXTPCmdTarget
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a report section.
	//-----------------------------------------------------------------------
	CXTPReportSection(CXTPReportControl *pControl, XTPReportRowType rowType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destructs a report section.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportSection();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the border of this section.
	//-----------------------------------------------------------------------
	CXTPReportBorder* GetBorder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow user moving the selection to rows.
	// Parameters:
	//     bAllowAccess - TRUE - selection of rows is allowed, FALSE otherwise.
	// See Also: IsAllowAccess
	//-----------------------------------------------------------------------
	void AllowAccess(BOOL bAllowAccess);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if user can move selection to rows.
	// Returns:
	//     TRUE if user can move selection to rows, FALSE otherwise.
	// See Also: AllowAccess.
	//-----------------------------------------------------------------------
	BOOL IsAllowAccess() const;

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
	//     Call this member to determine if the record items are editable.
	// Returns:
	//     TRUE is the record items are editable, otherwise FALSE.
	// See Also: AllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsAllowEdit() const;

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
	// See Also: AllowGroup
	//-----------------------------------------------------------------------
	BOOL IsAllowSort() const;

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
	//     Call this member to determine if rows selection is enabled.
	// Returns:
	//     TRUE if rows selection is enabled, FALSE otherwise.
	// See Also: SelectionEnable
	//-----------------------------------------------------------------------
	BOOL IsSelectionEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide/show section.
	// Parameters:
	//     bShow - TRUE is records will be displayed, FALSE to hide records.
	// See Also: IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the section currently visible.
	// Returns:
	//     TRUE if section is visible, otherwise FALSE.
	// See Also: SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current height mode.
	// Returns:
	//     Current height mode.
	// See Also: SetHeightMode
	//-----------------------------------------------------------------------
	XTPReportSectionHeightMode GetHeightMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current height mode.
	// Parameters:
	//     heightMode - New height mode.
	// See Also: GetHeightMode
	//-----------------------------------------------------------------------
	void SetHeightMode(XTPReportSectionHeightMode heightMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current suggested height.
	// Returns:
	//     Current suggested height.
	// See Also: SetHeightMode
	//-----------------------------------------------------------------------
	int GetHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the suggested height.
	// Parameters:
	//     nHeight - New suggested height.
	// See Also: GetHeight
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current divider position for this section.
	// Returns:
	//     Current divider position for this section.
	// See Also: SetDividerPosition
	//-----------------------------------------------------------------------
	XTPReportSectionDividerPosition GetDividerPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the divider position for this section.
	// Parameters:
	//     dividerPosition - New divider position.
	// See Also: GetDividerPosition
	//-----------------------------------------------------------------------
	void SetDividerPosition(XTPReportSectionDividerPosition dividerPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current divider style for this section.
	// Returns:
	//     Current divider style for this section.
	// See Also: SetDividerStyle
	//-----------------------------------------------------------------------
	XTPReportSectionDividerStyle GetDividerStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the divider style for this section.
	// Parameters:
	//     dividerStyle - New divider style.
	// See Also: GetDividerStyle
	//-----------------------------------------------------------------------
	void SetDividerStyle(XTPReportSectionDividerStyle dividerStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current divider color for this section.
	// Returns:
	//     Current divider color for this section.
	// See Also: SetDividerColor
	//-----------------------------------------------------------------------
	CXTPPaintManagerColor GetDividerColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the divider color for this section.
	// Parameters:
	//     colorDivider - New divider color.
	// See Also: GetDividerColor
	//-----------------------------------------------------------------------
	void SetDividerColor(CXTPPaintManagerColor clrDivider);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current rectangle for this section.
	// Returns:
	//     Current rectangle for this section.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current divider rectangle for this section.
	// Returns:
	//     Current divider rectangle for this section.
	//-----------------------------------------------------------------------
	CRect GetDividerRect() const;

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
	//     Call this method to determine the row type: body, header, or footer.
	// Returns:
	//     A value of enum XTPReportRowType.
	//-----------------------------------------------------------------------
	XTPReportRowType GetRowType() const;

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
	virtual CXTPReportRecord* AddRecord(CXTPReportRecord *pRecord);

	virtual void CXTPReportSection::AddRecordEx(
		CXTPReportRecord *pRecord,
		CXTPReportRecord *pParentRecord = NULL,
		int nRowChildIndex    = -1,
		int nRecordChildIndex = -1);

	virtual void UpdateRecord(CXTPReportRecord *pRecord, BOOL bUpdateChildren);

	virtual BOOL RemoveRecordEx(CXTPReportRecord *pRecord,
		BOOL bAdjustLayout     = TRUE,
		BOOL bRemoveFromParent = TRUE);

	virtual BOOL RemoveRowEx(CXTPReportRow *pRow, BOOL bAdjustLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method deletes all rows and records from the section.
	//-----------------------------------------------------------------------
	virtual void ResetContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Populates the section with rows.
	//-----------------------------------------------------------------------
	virtual void Populate(BOOL bUpdate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Re-sorts the rows of this section.
	//-----------------------------------------------------------------------
	virtual void ReSortRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates indexes of all rows.
	// Parameters:
	//     bAdjustLayout - If TRUE, layout is adjusted.
	//     bReverseOrder - If TRUE, row indices are updated in reverse order,
	//                     starting from the last row.
	//-----------------------------------------------------------------------
	virtual void RefreshIndexes(BOOL bAdjustLayout, BOOL bReverseOrder);

	virtual int _DoExpand(int nIndex, CXTPReportRow *pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses all children of the specified row.
	// Parameters:
	//     pRow - A row to collapse.
	//-----------------------------------------------------------------------
	virtual int _DoCollapse(CXTPReportRow *pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the section on the provided DC.
	// Parameters:
	//     pDC      - Provided DC to draw rows image with.
	//     rcClient - A rectangle to draw rows image into.
	// Returns:
	//     Index of last printed row
	//-----------------------------------------------------------------------
	virtual void Draw(CDC *pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if section is in virtual mode
	// Returns;
	//     TRUE if virtual mode is enabled; FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsVirtualMode() const;

	virtual int InsertRow(int nIndex, CXTPReportRow *pRow);

	virtual int GetPageRowCount(CDC *pDC) const;

	virtual int GetTotalHeight(CDC *pDC) const;

	virtual int GetRowsHeight(CDC *pDC) const;

	virtual BOOL GetRowAt(CDC *pDC, int nWidth, int nOffset, int &nIndex, int &nTop);

	virtual int GetRowOffset(CDC *pDC, int nWidth, int nIndex);

	virtual BOOL EnsureVisible(CDC *pDC, CXTPReportRow *pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets the markup context
	//-----------------------------------------------------------------------
	void SetMarkupContext(CXTPMarkupContext *pMarkupContext);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the number of records after using m_strFilterText.
	// Returns:
	//     Number of records after using m_strFilterText.
	//-----------------------------------------------------------------------
	int GetPopulatedRecordsCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the section.
	// Returns:
	//     Index of the section.
	//-----------------------------------------------------------------------
	int GetIndex() const;

protected:

	BOOL EnsureVisibleSmooth(CDC *pDC, CXTPReportRow *pRow);
	int  EnsureVisibleCalc  (CDC *pDC, CXTPReportRow *pRow);
	BOOL EnsureVisibleBlock (CDC *pDC, CXTPReportRow *pRow);
	BOOL EnsureVisibleIcon  (CDC *pDC, CXTPReportRow *pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Builds rows tree based on provided data record.
	// Parameters:
	//     pTree      - Rows tree to add items into.
	//     pParentRow - Parent tree row.
	//     pRecords   - Records collection for transferring to rows.
	// Remarks:
	//     Builds rows tree based on provided data record.
	//     Recursively calls itself when build nested branches of rows
	//-----------------------------------------------------------------------
	void BuildTree(
		CXTPReportRows    *pTree,
		CXTPReportRow     *pParentRow,
		CXTPReportRecords *pRecords);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts rows tree corresponding to the sort order taken from columns.
	// Parameters:
	//     pTree - A rows tree collection to sort.
	//-----------------------------------------------------------------------
	virtual void SortTree(CXTPReportRows* pTree);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts rows corresponding to the sort order taken from columns.
	// Parameters:
	//     pRows - A rows collection to sort.
	//-----------------------------------------------------------------------
	virtual void SortRows(CXTPReportRows *pRows);

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
	//     Returns the current topmost row.
	//-----------------------------------------------------------------------
	virtual int GetScrollIndexV();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current topmost row.
	//-----------------------------------------------------------------------
	virtual void SetScrollIndexV(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the currently top row index.
	//-----------------------------------------------------------------------
	virtual int GetScrollOffsetV() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the currently top row index.
	//-----------------------------------------------------------------------
	virtual void SetScrollOffsetV(int nScrollOffsetV);

	XTPReportScrollMode GetScrollModeV() const;
	XTPReportScrollMode GetScrollModeH() const;

protected:

	CXTPMarkupContext *m_pMarkupContext; // Markup context
	CXTPReportBorder  *m_pBorder;        // Border

	CXTPReportControl *m_pControl;       // Parent control.
	CXTPReportRecords *m_pRecords;       // Records container.
	CXTPReportRows    *m_pRows;          // Rows container.
	CXTPReportRows    *m_pTree;          // Tree rows container.

	int m_nPopulatedRecordsCount;  // Number of records after using m_strFilterText.

	//////////////////////////////////////////////////////////////////////////
	// Divider
	//////////////////////////////////////////////////////////////////////////

	CRect                           m_rcSection;       // The area occupied by the section.
	CRect                           m_rcDivider;       // Divider rectangle
	CXTPPaintManagerColor           m_clrDivider;      // Divider color
	XTPReportSectionDividerPosition m_dividerPosition; // Divider position
	XTPReportSectionDividerStyle    m_dividerStyle;    // Divider style

	XTPReportSectionHeightMode     m_heightMode;   // Height mode algorithm
	int  m_nHeight;                                // Suggested height (Fixed / Relative)

	BOOL m_bAllowAccess;                 // TRUE if rows can be selected.
	BOOL m_bAllowEdit;                   // TRUE if rows can be edited.
	BOOL m_bAllowGroup;                  // TRUE if rows can be grouped.
	BOOL m_bAllowSort;                   // TRUE if rows can be sorted.
	BOOL m_bSelectionEnable;             // TRUE if selection enabled.
	BOOL m_bVisible;                     // TRUE if section is visible.

	//////////////////////////////////////////////////////////////////////////
	// Scrolling
	//////////////////////////////////////////////////////////////////////////
	int m_nScrollIndexV;		// Current top row in the visible area.
									// IMPORTANT: With smooth scrolling this member may
									// be -1, when the scrolling offset is changed and
									// the new top row index is not yet recalculated.
									// GetTopRowIndex() always returns the correct value.
	
	int m_nScrollOffsetV;   // Vertical scroll offset


	XTPReportRowType m_rowType;

	CUIntArray m_UaSorted;                      //Sorted array.

private:


	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPReportControl;
	friend class CXTPReportSections;
	friend class CXTPReportGroupRow;
	//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE void CXTPReportSection::AllowAccess(BOOL bAllowAccess)
{
	m_bAllowAccess = bAllowAccess;
}

AFX_INLINE BOOL CXTPReportSection::IsAllowAccess() const
{
	return m_bAllowAccess;
}

AFX_INLINE void CXTPReportSection::AllowEdit(BOOL bAllowEdit)
{
	m_bAllowEdit = bAllowEdit;
}

AFX_INLINE BOOL CXTPReportSection::IsAllowEdit() const
{
	return m_bAllowEdit;
}

AFX_INLINE void CXTPReportSection::AllowGroup(BOOL bAllowGroup)
{
	m_bAllowGroup = bAllowGroup;
}

AFX_INLINE BOOL CXTPReportSection::IsAllowGroup() const
{
	return m_bAllowGroup;
}

AFX_INLINE void CXTPReportSection::AllowSort(BOOL bAllowSort)
{
	m_bAllowSort = bAllowSort;
}

AFX_INLINE BOOL CXTPReportSection::IsAllowSort() const
{
	return m_bAllowSort;
}

AFX_INLINE void CXTPReportSection::SelectionEnable(BOOL bEnable)
{
	m_bSelectionEnable = bEnable;
}

AFX_INLINE BOOL CXTPReportSection::IsSelectionEnabled() const
{
	return m_bSelectionEnable;
}

AFX_INLINE void CXTPReportSection::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
}

AFX_INLINE BOOL CXTPReportSection::IsVisible() const
{
	return m_bVisible;
}

AFX_INLINE CXTPReportRecords* CXTPReportSection::GetRecords() const
{
	return m_pRecords;
}

AFX_INLINE CXTPReportRows* CXTPReportSection::GetRows() const
{
	return m_pRows;
}

AFX_INLINE XTPReportSectionHeightMode CXTPReportSection::GetHeightMode() const
{
	return m_heightMode;
}

AFX_INLINE void CXTPReportSection::SetHeightMode(XTPReportSectionHeightMode heightMode)
{
	m_heightMode = heightMode;
}

AFX_INLINE int CXTPReportSection::GetHeight() const
{
	return m_nHeight;
}

AFX_INLINE void CXTPReportSection::SetHeight(int nHeight)
{
	m_nHeight = nHeight;
}

AFX_INLINE XTPReportSectionDividerPosition CXTPReportSection::GetDividerPosition() const
{
	return m_dividerPosition;
}

AFX_INLINE void CXTPReportSection::SetDividerPosition(XTPReportSectionDividerPosition dividerPosition)
{
	m_dividerPosition = dividerPosition;
}

AFX_INLINE XTPReportSectionDividerStyle CXTPReportSection::GetDividerStyle() const
{
	return m_dividerStyle;
}

AFX_INLINE void CXTPReportSection::SetDividerStyle(XTPReportSectionDividerStyle dividerStyle)
{
	m_dividerStyle = dividerStyle;
}

AFX_INLINE CXTPPaintManagerColor CXTPReportSection::GetDividerColor() const
{
	return m_clrDivider;
}

AFX_INLINE void CXTPReportSection::SetDividerColor(CXTPPaintManagerColor clrDivider)
{
	m_clrDivider = clrDivider;
}

AFX_INLINE CRect CXTPReportSection::GetRect() const
{
	return m_rcSection;
}

AFX_INLINE CRect CXTPReportSection::GetDividerRect() const
{
	return m_rcDivider;
}

AFX_INLINE int CXTPReportSection::GetPopulatedRecordsCount() const
{
	return m_nPopulatedRecordsCount;
}

AFX_INLINE XTPReportScrollMode CXTPReportSection::GetScrollModeH() const
{
	XTPReportScrollMode scrollMode = xtpReportScrollModeNone;

	if (NULL != m_pControl)
	{
		scrollMode = m_pControl->GetScrollMode(xtpReportOrientationHorizontal);
	}

	return scrollMode;
}

AFX_INLINE XTPReportScrollMode CXTPReportSection::GetScrollModeV() const
{
	XTPReportScrollMode scrollMode = xtpReportScrollModeNone;

	if (NULL != m_pControl)
	{
		scrollMode = m_pControl->GetScrollMode(xtpReportOrientationVertical);
	}

	return scrollMode;
}


#endif //#if !defined(__XTPREPORTSECTION_H__)
