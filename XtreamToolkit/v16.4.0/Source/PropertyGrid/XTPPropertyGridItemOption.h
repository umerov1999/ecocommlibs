// XTPPropertyGridItemOption.h interface for the CXTPPropertyGridItemOption class.
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
#if !defined(__XTPPROPERTYGRIDITEMOPTION_H__)
#define __XTPPROPERTYGRIDITEMOPTION_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     CXTPPropertyGridItemOption is a CXTPPropertyGridItem derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemOption : public CXTPPropertyGridItem
{
	DECLARE_DYNAMIC(CXTPPropertyGridItemOption)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemOption object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     nValue     - Initial value for this item.
	//     pBindOption - If not NULL, then the value of this item
	//                  is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemOption has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToOption member.
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
	// See Also: BindToOption
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemOption(LPCTSTR strCaption, int nValue = 0, int* pBindOption = NULL);
	CXTPPropertyGridItemOption(UINT nID, int nValue = 0, int* pBindOption = NULL);  // <COMBINE CXTPPropertyGridItemOption::CXTPPropertyGridItemOption@LPCTSTR@int@int*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemOption object
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemOption();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     nValue - The new integer value of the item.
	//-----------------------------------------------------------------------
	void SetOption(int nValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind an item to a long value.
	// Parameters:
	//     pBindOption - Long value to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToNumber member.
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
	void BindToOption(int* pBindOption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the integer value of the item.
	// Returns:
	//     The integer value of the item.
	//-----------------------------------------------------------------------
	int GetOption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies whether the Option Item will be displayed as a check box
	//     or an option button.
	// Parameters:
	//     bCheckBoxStyle - Set to TRUE to use a check box.
	//-----------------------------------------------------------------------
	void SetCheckBoxStyle(BOOL bCheckBoxStyle = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determins whether the Option Item will be displayed as a check box
	//     or an option button.
	// Returns:
	//     True if the option button will be displayed as a check box, FALSE if
	//     the option button will be displayed as a option\radio button.
	//-----------------------------------------------------------------------
	BOOL IsCheckBoxStyle() const;

protected:

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
	void OnBeforeInsert();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change an item's value.
	//     Override this method to add new functionality.
	//     You should call the base class version of this function from your
	//     override.
	// Parameters:
	//     strValue - New value of the item.
	//-----------------------------------------------------------------------
	void SetValue(CString strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this method and fill in the MEASUREITEMSTRUCT structure
	//     to inform Windows of the list-box dimensions.
	// Parameters:
	//     lpMeasureItemStruct - Specifies a long pointer to a MEASUREITEMSTRUCT
	//     structure.
	//-----------------------------------------------------------------------
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

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
	BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user double-clicks the left
	//     mouse button.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y-coordinate of the cursor.
	//-----------------------------------------------------------------------
	void OnLButtonDblClk(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the left mouse button.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down
	//              (see Visual C++ documentation).
	//     point  - Specifies the x- and y-coordinate of the cursor.
	// Returns:
	//     TRUE if the message processed, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL OnLButtonDown(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by PropertyGrid to set focus to child inplace window.
	// Parameters:
	//     bForward - Forward if user move focus forward, FLASE if backword (with Shift+Tab).
	// Returns:
	//     TRUE if successful, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL OnInpaceControlFocus(BOOL bForward);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is de-selected.
	//-----------------------------------------------------------------------
	void OnDeselect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a non-system key is pressed.
	// Parameters:
	//     nChar - Specifies the virtual-key code of the given key.
	//-----------------------------------------------------------------------
	BOOL OnKeyDown(UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to toggle the value of the option button.
	// Parameters:
	//     nIndex - Specifies the index of the option in the option list to be checked.
	//-----------------------------------------------------------------------
	void ToggleConstraint(int nIndex);

private:
	void _Init(int nValue);

protected:
	int    m_nValue;      // Value of the item.  This will contain the value of the selected constraint, not the text
	int*   m_pBindOption; // Binded value.  This is a pointer to the variable bound to this item.
	BOOL m_bCheckBoxStyle; //     True if the option button will be displayed as a check box, FALSE if
	                       //     the option button will be displayed as a option\radio button.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OleSetValue(const VARIANT* varValue);
	afx_msg const VARIANT OleGetValue();

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPPropertyGridItemBool)

	void BindDispatch();
//}}AFX_CODEJOCK_PRIVATE
#endif

};

AFX_INLINE int CXTPPropertyGridItemOption::GetOption() const {
	return m_nValue;
}
AFX_INLINE void CXTPPropertyGridItemOption::SetCheckBoxStyle(BOOL bCheckBoxStyle /*= TRUE*/) {
	m_bCheckBoxStyle = bCheckBoxStyle;
}
AFX_INLINE BOOL CXTPPropertyGridItemOption::IsCheckBoxStyle() const {
	return m_bCheckBoxStyle;
}

#endif //#if !defined(__XTPPROPERTYGRIDITEMOPTION_H__)
