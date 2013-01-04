// XTPReportNavigator.h: interface for the CXTPReportNavigator class.
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
#if !defined(__XTPREPORTNAVIGATOR_H__)
#define __XTPREPORTNAVIGATOR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPReportControl;
class CXTPReportRow;
class CXTPReportHeader;
class CXTPReportColumn;

//===========================================================================
// Summary:
//     Utility class, handling Report Control item navigation.
// Remarks:
//
// See Also: CXTPReportControl overview
//===========================================================================
class _XTP_EXT_CLASS CXTPReportNavigator : public CXTPCmdTarget
{
	friend class CXTPReportControl;
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default navigator constructor, handles properties initialization.
	// Parameters:
	// pReportControl - pointer to CXTPReportControl object
	// See Also: RefreshMetrics
	//-----------------------------------------------------------------------
	CXTPReportNavigator(CXTPReportControl* pReportControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default navigator destructor, handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportNavigator();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to start edit currently focused item.
	//-----------------------------------------------------------------------
	virtual void BeginEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row down.
	// Parameters:
	//     bShiftKey - TRUE when selecting a new focused row, FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row down by 1.
	// See Also: MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveDown(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row down.
	// Parameters:
	//     bShiftKey - TRUE when selecting a new focused row, FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row down by 1.
	// See Also: MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveDownStep(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row up.
	// Parameters:
	//     bShiftKey     - TRUE when selecting a new focused row, FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row up by 1.
	// See Also: MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveUp(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row up.
	// Parameters:
	//     bShiftKey     - TRUE when selecting a new focused row, FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row up by 1.
	// See Also: MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveUpStep(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row down at one page.
	// Parameters:
	//     bShiftKey     - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move currently
	//     focused row down by 1 page (an count of rows visible in a view).
	// See Also: MoveDown, MoveUp, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MovePageDown(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row up at one page.
	// Parameters:
	//     bShiftKey - TRUE when selecting rows up to new focused row,
	//                    FALSE otherwise.
	//     bControlKey - TRUE to ignore the current rows selection.
	// Remarks:
	//     Call this member function if you want to move currently
	//     row up by 1 page (and count of rows visible in a view).
	// See Also: MoveDown, MoveUp, MovePageDown, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MovePageUp(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the beginning of the report view.
	// Parameters:
	//     bShiftKey     - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row to the top of the rows list.
	// See Also: MoveDown, MoveUp, MovePageDown, MovePageUp, MoveLast
	//-----------------------------------------------------------------------
	void MoveFirstRow(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the end of the report view.
	// Parameters:
	//     bShiftKey     - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row to the bottom of the rows list.
	// See Also: MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst
	//-----------------------------------------------------------------------
	void MoveLastRow(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the specified row index.
	// Parameters:
	//     nRowIndex        - Index of the row to move focus.
	//     bShiftKey     - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row to the the specified row index.
	// See Also: MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst
	//-----------------------------------------------------------------------
	void MoveToRow(int nRowIndex, BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column to the beginning of the row.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused column to the beginning of the currently selected row.
	// See Also: MoveLastColumn
	//-----------------------------------------------------------------------
	void MoveFirstColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column to the end of the row.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused column to the end of the currently selected row.
	// See Also: MoveFirstColumn
	//-----------------------------------------------------------------------
	void MoveLastColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column left.
	// Parameters:
	//     bShiftKey - TRUE when selecting a new focused row, FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused column left by 1.
	// See Also: MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MoveLeft(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column left.
	// Parameters:
	//     bShiftKey     - TRUE when selecting a new focused row, FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused column right by 1.
	// See Also: MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MoveRight(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column to the specified index.
	// Parameters:
	//     nColumnIndex         - Index of the column to focus.
	//     bClearIfNonFocusable - Used when column the focused row specified
	//                            by nColumnIndex is not focusable:
	//                            if TRUE column focus will be cleared (no
	//                            column focused), otherwise focused column will
	//                            not be changed.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused column to the specified index.
	// See Also: MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MoveToColumn(int nColumnIndex, BOOL bClearIfNonFocusable = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Setting this property to TRUE moves focus (selected row) to header rows.
	// Parameters:
	//     bCurrentFocusInHeadersRows - TRUE when Current Focus In Headers Rows, FALSE otherwise.
	//
	// See Also: GetCurrentFocusInHeadersRows
	//-----------------------------------------------------------------------
	void SetCurrentFocusInHeadersRows(BOOL bCurrentFocusInHeadersRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Setting this property to TRUE moves focus (selected row) to footer rows.
	// Parameters:
	//     bCurrentFocusInFootersRows - TRUE when Current Focus In Footers Rows, FALSE otherwise.
	//
	// See Also: GetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	void SetCurrentFocusInFootersRows(BOOL bCurrentFocusInFootersRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE, if a focused row belongs to header rows, FALSE otherwise.
	//
	// See Also: SetCurrentFocusInHeadersRows
	//-----------------------------------------------------------------------
	BOOL GetCurrentFocusInHeadersRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE, if a focused row belongs to footer rows, FALSE otherwise.
	//
	// See Also: SetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	BOOL GetCurrentFocusInFootersRows();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused column left or right.
	// Parameters:
	//     bShiftKey     - TRUE when selecting a new focused row, FALSE otherwise.
	//     bControlKey - TRUE if ignore current rows selection.
	//     bBack - TRUE to move left/ FALSE to move right
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused column
	// See Also: MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	virtual void MoveLeftRight(BOOL bBack, BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the first visible body row of a target place: body, header, or footer.
	// Parameters:
	//     RowType     - target type of the focused row.
	// Remarks:
	//     Call this member function if you want to move the focused row to the first visible target row.
	// See Also:
	//     SetCurrentFocusInHeadersRows, SetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	void MoveFirstVisibleRow(XTPReportRowType RowType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the last visible row of a target place: body, header, or footer.
	// Parameters:
	//     RowType     - target type of the focused row.
	// Remarks:
	//     Call this member function if you want to move the focused row to the last visible target row.
	// See Also:
	//     SetCurrentFocusInHeadersRows, SetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	void MoveLastVisibleRow(XTPReportRowType RowType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check for dead end by moving up or down.
	// Parameters:
	//     bMoveDown  - TRUE to move down and FALSE for move UP to find dead end.
	// Returns:
	//     An integer value specifying the index of the dead end row.
	// See Also:
	//     SetCurrentFocusInHeadersRows, SetCurrentFocusInFootersRows
	//-----------------------------------------------------------------------
	int CheckDeadEnd(BOOL bMoveDown);

protected:
	CXTPReportControl* m_pReportControl; // Associated report control.

};


//===========================================================================
// Summary:
//     Utility class, handling Report Control icon navigation.
// Remarks:
//
// See Also: CXTPReportControl overview
//===========================================================================
class _XTP_EXT_CLASS CXTPReportIconNavigator : public CXTPReportNavigator
{
	friend class CXTPReportControl;
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default navigator constructor, handles properties initialization.
	// Parameters:
	// pReportControl - pointer to CXTPReportControl object
	// See Also: RefreshMetrics
	//-----------------------------------------------------------------------
	CXTPReportIconNavigator(CXTPReportControl* pReportControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default navigator destructor, handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportIconNavigator();

public:

	virtual void BeginEdit();

	virtual void MoveDown(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);
	virtual void MoveUp(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	virtual void MoveDownStep(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);
	virtual void MoveUpStep(BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);

	virtual void MovePageDown(BOOL bShiftKey, BOOL bControlKey);
	virtual void MovePageUp(BOOL bShiftKey, BOOL bControlKey);

	virtual void MoveLeftRight(BOOL bBack, BOOL bShiftKey = FALSE, BOOL bControlKey = FALSE);
};


#endif //#if !defined(__XTPREPORTNAVIGATOR_H__)
