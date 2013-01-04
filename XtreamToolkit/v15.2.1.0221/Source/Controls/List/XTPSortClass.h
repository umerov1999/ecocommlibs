// XTPSortClass.h interface for the CXTPSortClass  class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPSORTCLASS_H__)
#define __XTPSORTCLASS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine sort type.
// Remarks:
//     XTPSortType type defines the constants used by the CXTPSortClass
//     to determine for what type of sorting to perform.
// See Also:
//     CXTPSortClass::Sort
//
// <KEYWORDS xtpSortInt, xtpSortString, xtpSortDateTime, xtpSortDecimal>
// ----------------------------------------------------------------------
enum XTPSortType
{
	xtpSortInt = 1,  // Sort type integer.
	xtpSortString,   // Sort type string.
	xtpSortDateTime, // Sort type date / time.
	xtpSortDecimal   // Sort type decimal.
};

//===========================================================================
// Summary:
//     CXTPSortClass is a stand alone class. This class will sort a list control
//     by a column of text, integer, float or date/time type. It could be
//     easily extended for other data types. To use this class, instantiate
//     a CXTPSortClass object, passing in a pointer to the list control as
//     the first argument and the column index as the second argument.
//
// Example:
//     The following example demonstrates how to use CXTPSortClass.
// <code>
// CXTPSortClass sortClass(pListCtrl, 0);
// </code>
//
//     Then call the sort method setting ascending as the first argument either
//     TRUE or FALSE, and the type of sort to perform in the second argument.
//
//      The following example demonstrates how to use the sortClass.Sort member functin.
// <code>
// sortClass.Sort(TRUE, xtpSortString);
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPSortClass
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSortClass object
	// Parameters:
	//     pListCtrl  - Pointer to a valid CListCtrl object.
	//     nCol       - Index of the column to be sorted.
	//     pfnCompare - Address of the application-defined comparison function,
	//                  see CListCtrl::SortItems for more details.
	//-----------------------------------------------------------------------
	CXTPSortClass(CListCtrl* pListCtrl, const int nCol, PFNLVCOMPARE pfnCompare = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSortClass object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPSortClass();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to perform the actual sort procedure.
	// Parameters:
	//     bAsc  - true to sort ascending.
	//     iType - The type of sort to perform can be one of the values listed
	//             in the Remarks section.
	// Remarks:
	//     Sort type can be one of the following.<p/>
	//     * <b>xtpSortInt</b> Sort type int.
	//     * <b>xtpSortString</b> Sort type string.
	//     * <b>xtpSortDateTime</b> Sort type date / time.
	//     * <b>xtpSortDecimal</b> Sort type decimal.
	//     * <b>xtpSortCustom</b>Sort type user defined.
	//-----------------------------------------------------------------------
	virtual void Sort(bool bAsc, int iType);

	//-----------------------------------------------------------------------
	// Summary:
	//     This callback member function is called to compare two data items during
	//     sorting operations.
	// Parameters:
	//     lParam1    - Specify the item data for the two items being compared.
	//     lParam2    - Specify the item data for the two items being compared.
	//     lParamSort - Specifies the application defined value that is passed to the
	//     comparison function.
	// Returns:
	//     The comparison function must return a negative
	//     value if the first item should precede the second, a positive value
	//     if the first item should follow the second, or zero if the two items
	//     are equivalent.
	//-----------------------------------------------------------------------
	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the sort class to remove non-numeric
	//     characters from the specified source string.
	// Parameters:
	//     strSource - String to remove non-numeric characters from.
	// Returns:
	//     The length of the specified string after non-numeric characters
	//     have been removed.
	//-----------------------------------------------------------------------
	static int AFX_CDECL RemoveNonNumeric(CString& strSource);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by application defined comparison
	//     function to perform integer sorting.
	// Parameters:
	//     lpszItem1 - NULL terminated string representing the first int
	//                 being compared.
	//     lpszItem2 - NULL terminated string representing the second int
	//                 being compared.
	//     sOrder    - Represents sort order, set to 1 for ascending, and -1
	//                 for descending.
	// Returns:
	//     A negative value if the first item should precede the second, a
	//     positive value if the first item should follow the second, or zero
	//     if the two items are equivalent.
	// See Also:
	//     SortString, SortDateTime, SortDecimal
	//-----------------------------------------------------------------------
	static int AFX_CDECL SortInt(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by application defined comparison
	//     function to perform string sorting.
	// Parameters:
	//     lpszItem1 - NULL terminated string representing the first string
	//                 being compared.
	//     lpszItem2 - NULL terminated string representing the second string
	//                 being compared.
	//     sOrder    - Represents sort order, set to 1 for ascending, and -1
	//                 for descending.
	// Returns:
	//     A negative value if the first item should precede the second, a
	//     positive value if the first item should follow the second, or zero
	//     if the two items are equivalent.
	// See Also:
	//     SortInt, SortDateTime, SortDecimal
	//-----------------------------------------------------------------------
	static int AFX_CDECL SortString(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by application defined comparison
	//     function to perform date / time sorting.
	// Parameters:
	//     lpszItem1 - NULL terminated string representing the first date / time
	//                 being compared.
	//     lpszItem2 - NULL terminated string representing the second date / time
	//                 being compared.
	//     sOrder    - Represents sort order, set to 1 for ascending, and -1
	//                 for descending.
	// Returns:
	//     A negative value if the first item should precede the second, a
	//     positive value if the first item should follow the second, or zero
	//     if the two items are equivalent.
	// See Also:
	//     SortInt, SortString, SortDecimal
	//-----------------------------------------------------------------------
	static int AFX_CDECL SortDateTime(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by application defined comparison
	//     function to perform decimal sorting.
	// Parameters:
	//     lpszItem1 - NULL terminated string representing the first decimal
	//                 being compared.
	//     lpszItem2 - NULL terminated string representing the second decimal
	//                 being compared.
	//     sOrder    - Represents sort order, set to 1 for ascending, and -1
	//                 for descending.
	// Returns:
	//     A negative value if the first item should precede the second, a
	//     positive value if the first item should follow the second, or zero
	//     if the two items are equivalent.
	// See Also:
	//     SortInt, SortString, SortDateTime
	//-----------------------------------------------------------------------
	static int AFX_CDECL SortDecimal(LPCTSTR lpszItem1, LPCTSTR lpszItem2, const short sOrder);

	//-----------------------------------------------------------------------
	// Summary:
	//     This structure is used by the CXTPSortClass for storing a temporary
	//     item data pointer that contains each list items name (used for sorting)
	//     and original item data (if any).
	//-----------------------------------------------------------------------
	struct ITEMDATA
	{
		CString   strItem; // NULL terminated string that represents the list item's text.
		DWORD_PTR dwData;   // List item's application defined value (if any).
	};

protected:
	CListCtrl*   m_pListCtrl;  // Pointer to the CListCtrl object to perform the sort on
	PFNLVCOMPARE m_pfnCompare; // Address of the application-defined comparison function, see CListCtrl::SortItems for more details.
};

#endif // #if !defined(__XTPSORTCLASS_H__)
