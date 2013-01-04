// XTPSyntaxEditSelection.h: interface for the CXTPSyntaxEditAutoCompleteWnd class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITSELECTION_H__)
#define __XTPSYNTAXEDITSELECTION_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{AFX_CODEJOCK_PRIVATE
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//      This class used to represent syntax edit text selection.
//      It allow to get and set selection properties in 2 measurements:
//      string and display. Also selection may be 'Normal' if its start less
//      then its end, and 'Reverse' otherwise.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditSelection : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPSyntaxEditCtrl;
	friend class CXTPSyntaxEditSelection;

	DECLARE_DYNAMIC(CXTPSyntaxEditSelection)
	//}}AFX_CODEJOCK_PRIVATE
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//      Copy constructor.
	// Parameters:
	//      rSrc - [in] A reference to source object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditSelection(const CXTPSyntaxEditSelection& rSrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//      Get the selection start position where column is a zero based
	//      char index in the string.
	// Returns:
	//      A reference to XTP_EDIT_LINECOL object which contains selection start.
	// See Also:
	//      GetEnd_str, GetStart_disp, GetEnd_disp, GetNormalStart_str,
	//      GetNormalStart_disp, GetNormalEnd_str, GetNormalEnd_disp,
	//      SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//      Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	const XTP_EDIT_LINECOL& GetStart_str() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get the selection end position where column is a zero based
	//      char index in the string.
	// Returns:
	//      A reference to XTP_EDIT_LINECOL object which contains selection end.
	// See Also:
	//      GetStart_str, GetStart_disp, GetEnd_disp, GetNormalStart_str,
	//      GetNormalStart_disp, GetNormalEnd_str, GetNormalEnd_disp,
	//      SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//      Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	const XTP_EDIT_LINECOL& GetEnd_str() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get the selection start position where column is a 1 based
	//      column index on the screen.
	// Returns:
	//      A reference to XTP_EDIT_LINECOL object which contains selection start.
	// See Also:
	//      GetStart_str, GetEnd_str, GetEnd_disp, GetNormalStart_str,
	//      GetNormalStart_disp, GetNormalEnd_str, GetNormalEnd_disp,
	//      SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//      Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	const XTP_EDIT_LINECOL& GetStart_disp() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get the selection end position where column is a 1 based
	//      column index on the screen.
	// Returns:
	//      A reference to XTP_EDIT_LINECOL object which contains selection end.
	// See Also:
	//      GetStart_str, GetEnd_str, GetStart_disp, GetNormalStart_str,
	//      GetNormalStart_disp, GetNormalEnd_str, GetNormalEnd_disp,
	//      SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//      Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	const XTP_EDIT_LINECOL& GetEnd_disp() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get the normalized selection start.
	// Returns:
	//      A reference to XTP_EDIT_LINECOL object which contains normalized
	//      selection start.
	// See Also:
	//      GetStart_str, GetEnd_str, GetStart_disp, GetEnd_disp, GetNormalEnd_str,
	//      GetNormalEnd_disp, SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//      Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	XTP_EDIT_LINECOL GetNormalStart_str() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get the normalized selection start.
	// Returns:
	//      A reference to XTP_EDIT_LINECOL object which contains normalized
	//      selection start.
	//-----------------------------------------------------------------------
	XTP_EDIT_LINECOL GetNormalStart_disp() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get the normalized selection end.
	// Returns:
	//      A reference to XTP_EDIT_LINECOL object which contains normalized
	//      selection end.
	// See Also:
	//      GetStart_str, GetEnd_str, GetStart_disp, GetEnd_disp, GetNormalStart_str,
	//      GetNormalStart_disp, SetStart_str, SetStart_disp, SetEnd_str, SetEnd_disp,
	//      Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	XTP_EDIT_LINECOL GetNormalEnd_str() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get the normalized selection end.
	// Returns:
	//      A reference to XTP_EDIT_LINECOL object which contains normalized
	//      selection end.
	//-----------------------------------------------------------------------
	XTP_EDIT_LINECOL GetNormalEnd_disp() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the selection start position.
	// Parameters:
	//      nTextRow - A text row number.
	//      nStrPos  - A zero based char index in the string.
	// See Also:
	//      SetEnd_str, SetEnd_disp, GetStart_str, GetEnd_str, GetStart_disp,
	//      GetEnd_disp, GetNormalStart_str, GetNormalStart_disp, GetNormalEnd_str,
	//      GetNormalEnd_disp, Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	void SetStart_str(int nTextRow, int nStrPos);

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the selection start position.
	// Parameters:
	//      nTextRow - A text row number.
	//      nDispCol - A 1 based column index on the screen.
	//-----------------------------------------------------------------------
	void SetStart_disp(int nTextRow, int nDispCol);

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the selection end position.
	// Parameters:
	//      nTextRow - A text row number.
	//      nStrPos  - A zero based char index in the string.
	// See Also:
	//      SetStart_str, SetStart_disp, GetStart_str, GetEnd_str, GetStart_disp,
	//      GetNormalStart_str, GetNormalStart_disp, GetNormalEnd_str,
	//      GetNormalEnd_disp, Reset_str, Reset_disp
	//-----------------------------------------------------------------------
	void SetEnd_str(int nTextRow, int nStrPos);

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the selection end position.
	// Parameters:
	//      nTextRow - A text row number.
	//      nDispCol - A 1 based column index on the screen.
	//-----------------------------------------------------------------------
	void SetEnd_disp(int nTextRow, int nDispCol);

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the selection start and end positions. The selection is empty
	//      in this case.
	// Parameters:
	//      nTextRow - A text row number.
	//      nStrPos  - A zero based char index in the string.
	// See Also:
	//      SetStart_str, SetStart_disp, GetStart_str, GetEnd_str, GetStart_disp,
	//      GetEnd_disp, GetNormalStart_str, GetNormalStart_disp, GetNormalEnd_str,
	//      GetNormalEnd_disp
	//-----------------------------------------------------------------------
	void Reset_str(int nTextRow, int nStrPos);

	//-----------------------------------------------------------------------
	// Summary:
	//      Set the selection start and end positions. The selection is empty
	//      in this case.
	// Parameters:
	//      nTextRow - A text row number.
	//      nDispCol - A 1 based column index on the screen.
	//-----------------------------------------------------------------------
	void Reset_disp(int nTextRow, int nDispCol);

	//-----------------------------------------------------------------------
	// Summary:
	//      Used to determine is selection mode 'Word' (enabled by press and
	//      hold Ctrl before selecting).
	// Returns:
	//      TRUE if selection mode is 'Word', FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsWordSelectionMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Used to determine is selection mode 'Block' (enabled by press and
	//      hold Alt before selecting).
	// Returns:
	//      TRUE if selection mode is 'Block', FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsbBlockSelectionMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Used to determine is selection exists (start != end).
	// Returns:
	//      TRUE if selection  exists, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsSelExist() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Used to determine is selection 'Normal' (start \< end).
	// Returns:
	//      TRUE if selection  'Normal', FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsSelNormal() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to determine is specified position inside
	//      the selection.
	// Parameters:
	//      nTextRow - A text row number.
	//      nStrPos  - A zero based char index in the string.
	// Returns:
	//      TRUE if specified position inside the selection, FALSE otherwise.
	//-----------------------------------------------------------------------
	// See Also:
	//      IsIntersectSel_str, IsIntersectSel_disp
	//-----------------------------------------------------------------------
	BOOL IsInSel_str(int nTextRow, int nStrPos) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to determine is specified position inside
	//      the selection.
	// Parameters:
	//      nTextRow - A text row number.
	//      nDispCol - A 1 based column index on the screen.
	// Returns:
	//      TRUE if specified position inside the selection, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsInSel_disp(int nTextRow, int nDispCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to determine is specified text range
	//      intersect the selection.
	// Parameters:
	//      nTextRow  - A text row number.
	//      nStrPos1  - Begin text range position (as zero based char index in the string).
	//      nStrPos2  - End text range position (as zero based char index in the string).
	// Returns:
	//      TRUE if specified text range intersect the selection, FALSE otherwise.
	// See Also:
	//      IsIntersectSel_str, IsIntersectSel_disp
	//-----------------------------------------------------------------------
	BOOL IsIntersectSel_str(int nTextRow, int nStrPos1, int nStrPos2) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to determine is specified text range
	//      intersect the selection.
	// Parameters:
	//      nTextRow  - A text row number.
	//      nDispCol1 - Begin text range position (as 1 based column index on the screen).
	//      nDispCol2 - End text range position (as 1 based column index on the screen).
	// Returns:
	//      TRUE if specified text range intersect the selection, FALSE otherwise.
	// See Also:
	//      IsIntersectSel_str, IsIntersectSel_disp
	//-----------------------------------------------------------------------
	BOOL IsIntersectSel_disp(int nTextRow, int nDispCol1, int nDispCol2) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Equality operator.
	// Parameters:
	//     rSrc - Second CXTPSyntaxEditSelection object to compare.
	// Remarks:
	//     Compare 2 CXTPSyntaxEditSelection objects.
	// Returns:
	//     TRUE if specified objects are equal, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL operator==(const CXTPSyntaxEditSelection& rSrc) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Non-Equality operator.
	// Parameters:
	//     rSrc - Second CXTPSyntaxEditSelection object to compare.
	// Remarks:
	//     Compare 2 CXTPSyntaxEditSelection objects.
	// Returns:
	//     TRUE if specified objects are not equal, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL operator!=(const CXTPSyntaxEditSelection& rSrc) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Copy operator.
	// Parameters:
	//     rSrc - The source object.
	// Remarks:
	//     Initialize members from the specified object.
	// Returns:
	//     Reference to the current object.
	//-----------------------------------------------------------------------
	const CXTPSyntaxEditSelection& operator=(const CXTPSyntaxEditSelection& rSrc);

	//{{AFX_CODEJOCK_PRIVATE
	int GetSelStartForRow_str(int nTextRow, int nDispLine);
	int GetSelEndForRow_str(int nTextRow, int nDispLine, BOOL* pbInfinitSelEnd);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL _IsInSel(BOOL bStr, int nTextRow, int nColX) const;
	BOOL _IsIntersectSel(BOOL bStr, int nTextRow, int nCol1, int nCol2) const;
	//}}AFX_CODEJOCK_PRIVATE

protected:

	BOOL bSelectingRunning;             // Stores 'SelectingRunning' mode
	BOOL bWordSelectionMode;            // Stores 'Word' selecting mode
	BOOL bBlockSelectionMode;           // Stores 'Block' selecting mode
	int  nSelStartTextRowFromLeftBar;   // Stores start selection line for full line selecting (from the left bar).

private:
	CXTPSyntaxEditCtrl* m_pOwnerCtrl;

	XTP_EDIT_LINECOL    selStart_disp;
	XTP_EDIT_LINECOL    selEnd_disp;

	XTP_EDIT_LINECOL    selStart_str;
	XTP_EDIT_LINECOL    selEnd_str;

protected:

};


//////////////////////////////////////////////////////////////////////////
AFX_INLINE const XTP_EDIT_LINECOL& CXTPSyntaxEditSelection::GetStart_str() const{
	return selStart_str;
}
AFX_INLINE const XTP_EDIT_LINECOL& CXTPSyntaxEditSelection::GetEnd_str() const{
	return selEnd_str;
}
AFX_INLINE const XTP_EDIT_LINECOL& CXTPSyntaxEditSelection::GetStart_disp() const {
	return selStart_disp;
}
AFX_INLINE const XTP_EDIT_LINECOL& CXTPSyntaxEditSelection::GetEnd_disp() const {
	return selEnd_disp;
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsWordSelectionMode() const {
	return bWordSelectionMode;
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsbBlockSelectionMode() const {
	return bBlockSelectionMode;
}
AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditSelection::GetNormalStart_str() const {
	if (bBlockSelectionMode)
		return XTP_EDIT_LINECOL::min2(selStart_str, selEnd_str);
	return selStart_str <= selEnd_str ? selStart_str : selEnd_str;
}
AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditSelection::GetNormalEnd_str() const {
	if (bBlockSelectionMode)
		return XTP_EDIT_LINECOL::max2(selStart_str, selEnd_str);
	return selEnd_str <= selStart_str ? selStart_str : selEnd_str;
}
AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditSelection::GetNormalStart_disp() const {
	if (bBlockSelectionMode)
		return XTP_EDIT_LINECOL::min2(selStart_disp, selEnd_disp);
	return selStart_disp <= selEnd_disp ? selStart_disp : selEnd_disp;
}
AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditSelection::GetNormalEnd_disp() const {
	if (bBlockSelectionMode)
		return XTP_EDIT_LINECOL::max2(selStart_disp, selEnd_disp);
	return selEnd_disp <= selStart_disp ? selStart_disp : selEnd_disp;
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsSelNormal() const {
	return selStart_disp <= selEnd_disp;
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsInSel_str(int nTextRow, int nStrPos)  const {
	ASSERT(nStrPos >= 0);
	return _IsInSel(TRUE, nTextRow, nStrPos);
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsInSel_disp(int nTextRow, int nDispCol) const {
	return _IsInSel(FALSE, nTextRow, nDispCol);
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsIntersectSel_str(int nTextRow, int nStrPos1, int nStrPos2) const {
	return _IsIntersectSel(TRUE, nTextRow, nStrPos1, nStrPos2);
}
AFX_INLINE BOOL CXTPSyntaxEditSelection::IsIntersectSel_disp(int nTextRow, int nDispCol1, int nDispCol2) const {
	return _IsIntersectSel(FALSE, nTextRow, nDispCol1, nDispCol2);
}


#endif // !defined(__XTPSYNTAXEDITSELECTION_H__)
