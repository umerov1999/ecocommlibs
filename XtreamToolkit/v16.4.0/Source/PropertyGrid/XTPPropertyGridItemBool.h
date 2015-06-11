// XTPPropertyGridItemBool.h interface for the CXTPPropertyGridItemBool class.
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
#if !defined(__XTPPROPERTYGRIDITEMBOOL_H__)
#define __XTPPROPERTYGRIDITEMBOOL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPPropertyGridItemBool is a CXTPPropertyGridItem derived class.
//     It is used to create a Boolean value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemBool : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemBool object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     bValue     - Initial value of item.
	//     pBindBool  - If not NULL, then the value of this item
	// Remarks:
	//     Class CXTPPropertyGridItemBool has no default constructor.
	//
	//     When using the second constructor, the Identifier (nID) of the
	//     second constructor can be linked with a STRINGTABLE resource
	//     with the same id in such form "Caption\\nDescription".
	//
	//     BINDING:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToBool member.
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
	// See Also: BindToBool
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemBool(LPCTSTR strCaption, BOOL bValue = FALSE, BOOL* pBindBool = NULL);
	CXTPPropertyGridItemBool(UINT nID, BOOL bValue = FALSE, BOOL* pBindBool = NULL); // <COMBINE CXTPPropertyGridItemBool::CXTPPropertyGridItemBool@LPCTSTR@BOOL@BOOL*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemBool object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemBool();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     bValue - The new BOOL value of the item.
	//-----------------------------------------------------------------------
	virtual void SetBool(BOOL bValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the Boolean value of the item.
	// Returns:
	//     TRUE if the items value is TRUE, FALSE is the items value is
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetBool() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind the item to a BOOL object.
	// Parameters:
	//     pBindBool - BOOL object to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToBool member.
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
	virtual void BindToBool(BOOL* pBindBool);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the label for true and false values in the
	//     drop list.  By default these values are set to True and False.
	// Parameters:
	//     strTrueText  - NULL terminated string that represents the True label.
	//     strFalseText - NULL terminated string that represents the False label.
	// Returns:
	//     True if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	BOOL SetTrueFalseText(LPCTSTR strTrueText, LPCTSTR strFalseText);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to draw bool item as check box frame
	// Parameters:
	//     bCheckBoxStyle - TRUE to draw item as check box
	// See Also: IsCheckBoxStyle
	//-----------------------------------------------------------------------
	void SetCheckBoxStyle(BOOL bCheckBoxStyle = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines is item has check box style.
	// Returns:
	//     TRUE if item drawn as check box frame.
	// See Also: SetCheckBoxStyle
	//-----------------------------------------------------------------------
	BOOL IsCheckBoxStyle() const;

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

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the label for true or false values in the
	//     drop list. This method will search for the value and update it if it
	//     exists. If the value is also the current value for the property grid
	//     item, that value is updated as well.
	// Parameters:
	//     strValueText - NULL terminated string that represents the value label.
	//     strNewText   - NULL terminated string that represents the new value label.
	// Returns:
	//     True if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	BOOL SetValueText(CString& strValueText, LPCTSTR strNewText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called before the item becomes visible in the
	//     property grid.
	// Remarks:
	//     Before the item is inserted, it is first check to see if it
	//     is bound to a variable, if it is, then the value of the item
	//     is updated with the value stored in the bound variable.
	//
	//          OnBeforeInsert is called when an item is inserted,
	//          when a category is inserted, when a category or item is
	//          expanded, and when the sort property has changed.
	//-----------------------------------------------------------------------
	virtual void OnBeforeInsert();


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is drawn. Override this function if
	//     needed.
	// Parameters:
	//     dc - Reference to the device context to be used for rendering an image
	//     of the item.
	//     rcValue - Bounding rectangle of the item.
	// Returns:
	//     TRUE if item is self-drawn.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the value of the item that the
	//     user is viewing.
	// Returns:
	//     CString value of the data the user is currently seeing.
	//     I.e. If a password mask it used, then the string returned
	//     will be filled with asterisks (*).  If no password mask is used,
	//     it will return the same thing as GetValue.
	//-----------------------------------------------------------------------
	virtual CString GetViewValue();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the left mouse button
	//     on the item.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y-coordinate of the cursor.
	// Returns:
	//     TRUE if the message processed, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary: This method is called to check to see if the SpaceBar was pressed and
	//          then "check" the item.
	// Parameters:
	//               nChar - Specifies the virtual-key code of the given key.
	// Remarks: Checks to see if the item has a checkbox style, is not read only, and
	//          the user pressed the SpaceBar key, if all is True, then the checkbox
	//          will be checked\un-checked.
	// Returns: TRUE if the item has a checkbox style, is not read only, and
	//          the user pressed the SpaceBar key.  FALSE if any other key is pressed.
	// See Also: PtInCheckBoxRect
	//-----------------------------------------------------------------------
	virtual BOOL OnKeyDown (UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user double-clicks the left
	//     mouse button on the item.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y-coordinate of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary: Determines if a CPoint is inside the Check Box displayed in the
	//          item when the check box style is used.
	// Parameters:
	//               point - CPoint to text to see if it is inside the check box.
	// Returns: TRUE if the CPoint is inside the check box, FALSE if the point is
	//          not inside the check box.
	// See Also: OnKeyDown
	//-----------------------------------------------------------------------
	BOOL PtInCheckBoxRect(CPoint point);

private:

	void _Init(BOOL bValue);

protected:

	BOOL    m_bValue;       // Value of the item.
	BOOL*   m_pBindBool;    // Binded value.  This is a pointer to the variable bound to this item.
	CString m_strTrueText;  // Text of the TRUE label.
	CString m_strFalseText; // Text of the FALSE label.
	BOOL    m_bCheckBoxStyle;  // Check box style

private:

	DECLARE_DYNAMIC(CXTPPropertyGridItemBool)

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemDouble)

	void BindDispatch();
//}}AFX_CODEJOCK_PRIVATE
#endif
};


//===========================================================================
// Summary:
//     CXTPPropertyGridItemEnum is a CXTPPropertyGridItem derived class.
//     It is used to create an Enum value item in a Property Grid control.
// Remarks:
//     A Enum item is used to display a list of constraints.  This is
//     very similar to a normal constraint list.  The difference is that
//     an Enum constraint list will return the Value of the constraint when
//     selected, where a normal constraint wold return the text of the
//     constraint.
//
// Example:
//     This code illustrates how to add an item of type Enum to your grid.
// <code>
// //Adds a category to the grid called "Software"
// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Software"));
//
// //Adds a CXTPPropertyGridItemFlags item with a caption of "Current OS" and an initial value of 2
// //This will cause the constraint with a value of 2 to be selected
// CXTPPropertyGridItemEnum* pItem = (CXTPPropertyGridItemEnum*)(pStandard->AddChildItem(new CXTPPropertyGridItemEnum(_T("Current OS"), 2)));
//
// //Adds some constraints along with a value
// pItem->GetConstraints()->AddConstraint(_T("Windows 98"), 1);
// pItem->GetConstraints()->AddConstraint(_T("Windows 2000"), 2);
// pItem->GetConstraints()->AddConstraint(_T("Windows XP"), 3);
//
// //GetEnum will return the Data value of the constraint selected
// TRACE(_T("Enum Item Value= %d\n"), pItem->GetEnum());
//
// //GetValue will return the String of the constraint selected
// TRACE(_T("Enum Item String= %s\n"), pItem->GetValue());
// </code>
//
// See Also: CXTPPropertyGridItemEnum::SetEnum, CXTPPropertyGridItemEnum::GetEnum, CXTPPropertyGridItem::GetValue
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemEnum : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemEnum object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     nValue     - Initial value of item.  This should be the
	//                  value of a constraint.
	//     pBindEnum  - If not NULL, then the value of this item
	//                  is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemEnum has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToEnum member.
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
	// See Also: BindToEnum
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemEnum(LPCTSTR strCaption, int nValue = 0, int* pBindEnum = NULL);
	CXTPPropertyGridItemEnum(UINT nID, int nValue = 0, int* pBindEnum = NULL); // <COMBINE CXTPPropertyGridItemEnum::CXTPPropertyGridItemEnum@LPCTSTR@int@int*>


	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemEnum object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemEnum();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value. The
	//     second method is called when user click an item in the
	//     constraint list.
	// Parameters:
	//     nValue      - The new value of the item.  This should be
	//                   the value of a constraint.
	//     pConstraint - The currently selected constraint.
	// See Also: CXTPPropertyGridItemEnum, GetEnum
	//-----------------------------------------------------------------------
	virtual void SetEnum(int nValue);
	virtual void SetEnum(CXTPPropertyGridItemConstraint* pConstraint); // <COMBINE CXTPPropertyGridItemEnum::SetEnum@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the selected value of the item.
	// Returns:
	//     Value of the currently selected constraint.  This returns the
	//     value, not the text.
	// See Also: CXTPPropertyGridItemEnum, SetEnum
	//-----------------------------------------------------------------------
	virtual int GetEnum() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind the item to a Enum object.
	// Parameters:
	//     pBindEnum - Enum object to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToEnum member.
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
	virtual void BindToEnum(int* pBindEnum);

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
	//          OnBeforeInsert is called when an item is inserted,
	//          when a category is inserted, when a category or item is
	//          expanded, and when the sort property has changed.
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

private:

	void _Init(int nValue);

protected:

	int    m_nValue;       // Value of the item.  This will contain the value of the selected constraint, not the text
	int*   m_pBindEnum;    // Binded value.  This is a pointer to the variable bound to this item.

private:

	DECLARE_DYNAMIC(CXTPPropertyGridItemEnum)

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();

	void BindDispatch();
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPPropertyGridItemBool::GetBool() const {
	return m_bValue;
}
AFX_INLINE int CXTPPropertyGridItemEnum::GetEnum() const {
	return m_nValue;
}

#endif // #if !defined(__XTPPROPERTYGRIDITEMBOOL_H__)
