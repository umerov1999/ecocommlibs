// XTPPropertyGridItemFlags.h interface for the CXTPPropertyGridItemFlags class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#if !defined(__XTPPROPERTYGRIDITEMFLAGS_H__)
#define __XTPPROPERTYGRIDITEMFLAGS_H__
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPPropertyGridItemFlags is a CXTPPropertyGridItem derived class.
//     It is used to create a Flags value item in a Property Grid control.
// Remarks:
//     A flag item is used to combine multiple boolean values into a
//     single item.  When using a flag item, you must add constraints
//     to the flag item.  Each constraint will have a value of TRUE or
//     FALSE.  The flag item will have an expand button that when clicked
//     will display all boolean constraints for that item.  The value of
//     the flag item will be the sum of all the boolean constraints with
//     a value of TRUE.
//
//          NOTE: Constraint values must be in powers of 2 if you will have an
//                "All" constraint that indicates that the value of all boolean
//                 constraints is TRUE or FALSE.
//
//          In the example below, the flag item's initial value will be 9, this
//          value is stored in CXTPPropertyGridItemFlags::m_nValue and can be
//          set with SetFlags and retrieved with GetFlags.  The string value will
//          be "[Windows 98; Windows 95]" and this value is stored in
//          CXTPPropertyGridItem::m_strValue, which can be retrieved
//          with CXTPPropertyGridItem::GetValue.  You must use the SetFlags method
//          to change the flag items value by code.  The string value is what is
//          displayed to the user.
//
// Example:
// This sample illustrates how to add an item of type Flags to your grid.
// <code>
// //Adds a category to the grid called "Software"
// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Software"));
//
// //Adds a CXTPPropertyGridItemFlags item with a caption of "Supported OS" and an initial value of 9
// CXTPPropertyGridItemFlags* pItem = (CXTPPropertyGridItemFlags*)(pStandard->AddChildItem(new CXTPPropertyGridItemFlags(_T("Supported OS"), 1 + 8)));
//
// //Adds a constraint that will set all items TRUE or FALSE, note the value of the constraint is the sum
// //of all the other constraints.  This item will also be updated when the values of the other constraints
// //have changed.
// pItem->GetConstraints()->AddConstraint(_T("All Windows"), 1 + 2 + 4 + 8 + 16 + 32);
//
// //Adds a constraint that can have a value of TRUE or FALSE
// pItem->GetConstraints()->AddConstraint(_T("Windows 98"), 1);
// pItem->GetConstraints()->AddConstraint(_T("Windows 2000"), 2);
// pItem->GetConstraints()->AddConstraint(_T("Windows XP"), 4);
// pItem->GetConstraints()->AddConstraint(_T("Windows 95"), 8);
// pItem->GetConstraints()->AddConstraint(_T("Windows NT"), 16);
// pItem->GetConstraints()->AddConstraint(_T("Windows 2003"), 32);
//
// //This changes the value of the flag item to 21 and the string
// //caption to [Windows 98; Windows XP; Windows NT]
// pItem->SetFlags(21);
//
// //GetFlags will return the sum of all true constraints
// TRACE(_T("Flags Item Value= %d\n"), pItem->GetFlags());
//
// //GetValue will return a string containing the text of all true constraints
// TRACE(_T("Flags Item String= %s\n"), pItem->GetValue());
// </code>
//
// See Also: CXTPPropertyGridItemFlags::SetFlags, CXTPPropertyGridItemFlags::GetFlags, CXTPPropertyGridItem::GetValue
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemFlags : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemFlag object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     nValue     - Initial value of item.  See the description
	//                  of CXTPPropertyGridItemFlags for a more
	//                  detailed description of this value.
	//     pBindFlags - If not NULL, then the value of this item
	//                  is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemFlags has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToFlags member.
	//
	//            Bound variables store the values of the property grid items
	//            and can be accessed without using the property grid methods
	//            and properties.  Bound variables allow the property grid to
	//            store data in variables.  When the value of a PropertyGridItem
	//            is changed, the value of the bound variable will be changed to
	//            the PropertyGridItem value.  The advantage of binding is that
	//            the variable can be used and manipulated without using
	//            PropertyGridItem methods and properties.
	//
	//            NOTE:  If the value of the variable is changed without using
	//            the PropertyGrid, the PropertyGridItem value will not be
	//            updated until you call CXTPPropertyGrid::Refresh.
	//
	// See Also: BindToFlags
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemFlags(LPCTSTR strCaption, int nValue = 0, int* pBindFlags = NULL);
	CXTPPropertyGridItemFlags(UINT nID, int nValue = 0, int* pBindFlags = NULL);  // <COMBINE CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlags@LPCTSTR@int@int*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemFlags object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemFlags();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     nValue - The new value of the item.  This should be the sum
	//              of the constraints data.  See the description
	//              of CXTPPropertyGridItemFlags for a more
	//              detailed description of this value.
	// See Also: CXTPPropertyGridItemFlags, GetFlags
	//-----------------------------------------------------------------------
	virtual void SetFlags(int nValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the value of the flag item.  This will
	//     be the sum of the constraint values that have a value of TRUE,
	//     not the displayed text.  To retrieve the text, use
	//     CXTPPropertyGrid::GetValue.
	// Returns:
	//     The sum of the constraint values with a value of TRUE.
	// Remarks:
	//     See CXTPPropertyGridItemFlags for more information.
	// See Also: CXTPPropertyGridItemFlags, SetFlags, CXTPPropertyGrid::GetValue.
	//-----------------------------------------------------------------------
	virtual int GetFlags() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind the item to a flags object.
	// Parameters:
	//     pBindFlags - Flags object to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToFlags member.
	//
	//     Bound variables store the values of the property grid items
	//     and can be accessed without using the property grid methods
	//     and properties.  Bound variables allow the property grid to
	//     store data in variables.  When the value of a PropertyGridItem
	//     is changed, the value of the bound variable will be changed to
	//     the PropertyGridItem value.  The advantage of binding is that
	//     the variable can be used and manipulated without using
	//     PropertyGridItem methods and properties.
	//
	//     NOTE:  If the value of the variable is changed without using
	//     the PropertyGrid, the PropertyGridItem value will not be
	//     updated until you call CXTPPropertyGrid::Refresh.
	//-----------------------------------------------------------------------
	virtual void BindToFlags(int* pBindFlags);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change an item's value.
	//     Override this method to add new functionality.
	//     You should call the base class version of this function from your
	//     override.
	// Parameters:
	//     strValue - New value of the item.
	//-----------------------------------------------------------------------
	virtual void SetValue(CString strValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called before the item becomes visible in the
	//     property grid.
	// Remarks:
	//     Before the item is inserted, it is first check to see if it
	//     is bound to a variable, if it is, then the value of the item
	//     is updated with the value stored in the bound variable.
	//
	//     OnBeforeInsert is called when an item is inserted,
	//     when a category is inserted, when a category or item is
	//     expanded, and when the sort property has changed.
	//-------------------------------------------------------------------------
	virtual void OnBeforeInsert();

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when constraints are added to this item.
	// Remarks:
	//     OnConstraintsChanged is called when
	//     CXTPPropertyGridItemConstraints::AddConstraint is called.
	// See Also: CXTPPropertyGridItemConstraints::AddConstraint
	//-------------------------------------------------------------------------
	virtual void OnConstraintsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the read-only state of the item.
	// Parameters:
	//     bReadOnly - Specifies whether to set or remove the read-only state of the
	//     item. A value of TRUE sets the state to read-only. A value of FALSE sets
	//     the state to read/write.
	//-----------------------------------------------------------------------
	virtual void SetReadOnly(BOOL bReadOnly);

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves string notation of selected flags.
	//-------------------------------------------------------------------------
	virtual CString GetFlagsString();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh child bool items.
	//-------------------------------------------------------------------------
	virtual void UpdateChilds();

	class CXTPPropertyGridItemFlag;

private:
	void _Init(int nValue);
	BOOL HasFlag(const CString& strValue, CString strFlag) const;


protected:
	int    m_nValue;       // Value of the item.  This is the sum of the constraint values that have a value of TRUE.
	int*   m_pBindFlags;   // Binded value. This is a pointer to the variable bound to this item.

private:
	DECLARE_DYNAMIC(CXTPPropertyGridItemFlags)


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();

	void BindDispatch();
//}}AFX_CODEJOCK_PRIVATE
#endif
	friend class CXTPPropertyGridItemFlag;
};

AFX_INLINE int CXTPPropertyGridItemFlags::GetFlags() const {
	return m_nValue;
}

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPPROPERTYGRIDITEMFLAGS_H__
//}}AFX_CODEJOCK_PRIVATE
