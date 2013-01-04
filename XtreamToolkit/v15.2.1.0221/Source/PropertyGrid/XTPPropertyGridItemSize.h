// XTPPropertyGridItemSize.h interface for the CXTPPropertyGridItemSize class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#if !defined(__XTPPROPERTYGRIDITEMSIZE_H__)
#define __XTPPROPERTYGRIDITEMSIZE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPPropertyGridItemSize is a CXTPPropertyGridItem derived class.
//     It is used to create a size value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemSize : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemSize object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     size       - Initial value of item.
	//     pBindSize  - If not NULL, then the value of this item
	//                   is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemSize has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToSize member.
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
	// See Also: BindToSize
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemSize(LPCTSTR strCaption, CSize size = 0, CSize* pBindSize = NULL);
	CXTPPropertyGridItemSize(UINT nID, CSize size = 0, CSize* pBindSize = NULL); //<COMBINE CXTPPropertyGridItemSize::CXTPPropertyGridItemSize@LPCTSTR@CSize@CSize*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemSize object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemSize();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     size - The new size value of the item.
	//-----------------------------------------------------------------------
	virtual void SetSize(CSize size);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the size value of the item.
	// Returns:
	//     A CSize object representing the size value of the item.
	//-----------------------------------------------------------------------
	virtual CSize GetSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind an item to a CSize object.
	// Parameters:
	//     pBindSize - CSize object to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToSize member.
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
	virtual void BindToSize(CSize* pBindSize);

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
	virtual void OnBeforeInsert();

private:

	void SetWidth(LPCTSTR strWidth);
	void SetHeight(LPCTSTR strHeight);

	CString SizeToString(CSize size);
	CSize StringToSize(LPCTSTR str);

	virtual void OnAddChildItem();
	virtual void SetValue(CString strValue);

	void UpdateChilds();

private:

	class CXTPPropertyGridItemSizeWidth;
	class CXTPPropertyGridItemSizeHeight;

	CSize                          m_szValue;
	CXTPPropertyGridItemSizeWidth*  m_pItemWidth;
	CXTPPropertyGridItemSizeHeight* m_pItemHeight;

	CSize* m_pBindSize;

private:

	DECLARE_DYNAMIC(CXTPPropertyGridItemSize)

	friend class CXTPPropertyGridItemSizeWidth;
	friend class CXTPPropertyGridItemSizeHeight;

};


//===========================================================================
// Summary:
//     CXTPPropertyGridItemPoint is a CXTPPropertyGridItem derived class.
//     It is used to create a point value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemPoint : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemPoint object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     ptPoint     - Initial value of item.
	//     pBindPoint  - If not NULL, then the value of this item
	//                   is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemPoint has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToPoint member.
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
	// See Also: BindToPoint
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemPoint(LPCTSTR strCaption, CPoint pt = 0, CPoint* pBindPoint = NULL);
	CXTPPropertyGridItemPoint(UINT nID, CPoint pt = 0, CPoint* pBindPoint = NULL); //<COMBINE CXTPPropertyGridItemPoint::CXTPPropertyGridItemPoint@LPCTSTR@CPoint@CPoint*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemPoint object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemPoint();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     pt - The new Point value of the item.
	//-----------------------------------------------------------------------
	virtual void SetPoint(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the Point value of the item.
	// Returns:
	//     A CPoint object representing the Point value of the item.
	//-----------------------------------------------------------------------
	virtual CPoint GetPoint() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind an item to a CPoint object.
	// Parameters:
	//     pBindPoint - CPoint object to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToPoint member.
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
	virtual void BindToPoint(CPoint* pBindPoint);

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
	virtual void OnBeforeInsert();

private:

	void SetX(LPCTSTR sx);
	void SetY(LPCTSTR sy);

	CString PointToString(CPoint pt);
	CPoint StringToPoint(LPCTSTR str);

	virtual void OnAddChildItem();
	virtual void SetValue(CString strValue);

	void UpdateChilds();

private:

	class CItemX;
	class CItemY;

	CPoint                      m_ptValue;
	CItemX* m_pItemX;
	CItemY* m_pItemY;

	CPoint* m_pBindPoint;

private:

	DECLARE_DYNAMIC(CXTPPropertyGridItemPoint)

	friend class CItemX;
	friend class CItemY;

};


//////////////////////////////////////////////////////////////////////

AFX_INLINE CSize CXTPPropertyGridItemSize::GetSize() const {
	return m_szValue;
}
AFX_INLINE CPoint CXTPPropertyGridItemPoint::GetPoint() const {
	return m_ptValue;
}


#endif // #if !defined(__XTPPROPERTYGRIDITEMSIZE_H__)
