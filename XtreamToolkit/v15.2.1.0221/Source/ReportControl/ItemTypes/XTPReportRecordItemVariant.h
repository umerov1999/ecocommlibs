// XTPReportRecordItemVariant.h: interface for the CXTPReportRecordItemVariant class.
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
#if !defined(__XTPREPORTRECORDITEMVARIANT_H__)
#define __XTPREPORTRECORDITEMVARIANT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     Represents a variant item and inherits basic functionality
//     from CXTPReportRecordItem class.
//     You can create a bitmap record item simply by calling a constructor with
//     the only one parameter - value of VARIANT type.
// See Also: CXTPReportRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemVariant : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CXTPReportRecordItemVariant)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a new variant record item.
	// Parameters:
	//     lpValue - Pointer to the initial item value.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemVariant(const VARIANT& lpValue = COleVariant((long)0));

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a text caption of this item.
	// Parameters:
	//     pColumn - Corresponded column of the item.
	// Remarks:
	//     Overwrites parents' member function
	// Returns:
	//     Text caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new value for the item.
	// Parameters:
	//     var - New value for the item.
	//-----------------------------------------------------------------------
	void SetValue(const COleVariant& var);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new value for the item.
	// Parameters:
	//     var - New value for the item.
	//-----------------------------------------------------------------------
	void SetValue(const VARIANT& var);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new value for the item.
	// Parameters:
	//     var - New value for the item.
	//-----------------------------------------------------------------------
	void SetValue(const VARIANT* var);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the value of the item.
	// Returns:
	//     The value of the item.
	//-----------------------------------------------------------------------
	COleVariant GetValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares this item with the provided one.
	// Parameters:
	//  pCol - pointer to column
	//  pItem - tem provided for comparing with current.
	// Remarks:
	//     Overwrites parents' member function
	// Returns:
	//     Zero if pItem is equal with current;
	//     Less than zero if less than pItem;
	//     Greater than zero if greater than pItem.
	//-----------------------------------------------------------------------
	int Compare(CXTPReportColumn* pCol, CXTPReportRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when value of in-place edit control changed
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//     szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	virtual void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user select specified constraint in in-place list
	// Parameters:
	//     pItemArgs   - Pointer to structure with items arguments
	//     pConstraint - Selected constraint
	//-----------------------------------------------------------------------
	void OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves associated value with selected constraint.
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments
	// Returns:
	//     DWORD value associated with selected constraint.
	//-----------------------------------------------------------------------
	DWORD GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report record item
	//     using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);//<COMBINE CXTPReportRecordItem::DoPropExchange>

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before value changed by user
	// Parameters:
	//     pItemArgs  - item arguments
	//     lpNewValue - new value to be set
	// Returns:
	//     FALSE to cancel edit operation.
	//-----------------------------------------------------------------------
	virtual BOOL OnValueChanging(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPVARIANT lpNewValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when in-place editor value changed by user
	// Parameters:
	//     pItemArgs   - item arguments
	//     rstrNewText - new string to be set in the editor
	// Returns:
	//     FALSE to cancel edit operation.
	//-----------------------------------------------------------------------
	virtual BOOL OnEditChanging(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CString& rstrNewText);

protected:

#ifdef _XTP_ACTIVEX
	DECLARE_DISPATCH_MAP()
#endif

public:
	COleVariant m_oleValue;    // Item value.
	static int m_nSortLocale;  // Sort locale
};

#endif // !defined(__XTPREPORTRECORDITEMVARIANT_H__)
