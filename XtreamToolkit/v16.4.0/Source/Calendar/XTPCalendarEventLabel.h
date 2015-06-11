// XTPCalendarEventLabel.h: interface for the CXTPCalendarEventLabel and
// CXTPCalendarEventLabels classes.
//
// This file is a part of the XTREME CALENDAR MFC class library.
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
#if !defined(_XTPCALENDAREVENTLABEL_H__)
#define _XTPCALENDAREVENTLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
class CXTPCalendarEventLabel;
class CXTPCalendarEventLabels;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     This class define Calendar Event label properties.
// See Also: CXTPCalendarEvent
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarEventLabel : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarEventLabel)
	//}}AFX_CODEJOCK_PRIVATE
public:
	int         m_nLabelID; // Label ID.
	COLORREF    m_clrColor; // Label color used to fill the event background.
	CString     m_strName;  // Label name.

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class constructor.
	// Parameters:
	//     nID      - Label ID.
	//     clrColor - Label color.
	//     pcszName - Label name.
	//-----------------------------------------------------------------------
	CXTPCalendarEventLabel(int nID = 0, COLORREF clrColor = 0, LPCTSTR pcszName = _T(""));

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles class member deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarEventLabel() {};

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set new object properties.
	// Parameters:
	//     rSrc - Reference to the source object.
	// Remarks:
	//     This is the overloaded assignment operator.
	// Returns:
	//     Reference to this class instance.
	//-----------------------------------------------------------------------
	const CXTPCalendarEventLabel& operator=(const CXTPCalendarEventLabel& rSrc)
	{
		m_nLabelID = rSrc.m_nLabelID;
		m_clrColor = rSrc.m_clrColor;
		m_strName = rSrc.m_strName;

		return *this;
	}
protected:

#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg OLE_COLOR OleGetColor();
	afx_msg void OleSetColor(OLE_COLOR clrOleColor);

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarEventLabel);
	//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================

/////////////////////////////////////////////////////////////////////////////

//===========================================================================
// Summary:
//     This class is used as a collection of CXTPCalendarEventLabel
//     objects.
// See Also: CXTPCalendarEventLabel, CXTPCalendarEvent
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarEventLabels :
						public CXTPCalendarPtrCollectionT<CXTPCalendarEventLabel>
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarEventLabels)
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default class constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarEventLabels();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles class members deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarEventLabels();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill the collection using
	//     a default label list.
	//-----------------------------------------------------------------------
	virtual void InitDefaultValues();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function updates default colors of labels when theme is changed
	//-----------------------------------------------------------------------
	void UpdateDefaultValues(COLORREF *clrEventColors, UINT uCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets a flag when non-standard labels are used
	//-----------------------------------------------------------------------
	void SetDefaultValues(BOOL bDefault);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to search for the label with a
	//     specified ID.
	// Parameters:
	//     nLabelID - An int that contains the label ID.
	// Returns:
	//     A pointer to a CXTPCalendarEventLabel object or NULL.
	//-----------------------------------------------------------------------
	CXTPCalendarEventLabel* Find(int nLabelID) const;
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds label index in the internal array by its ID.
	// Parameters:
	//     nLabelID - Label ID
	// Returns:
	//     Label index in the internal label array.
	// See Also: Find
	//-----------------------------------------------------------------------
	int FindIndex(int nLabelID) const;

protected:

	BOOL m_bDefaultValues;

#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
//  afx_msg long OleGetItemCount();
//  afx_msg LPDISPATCH OleGetItem(long nIndex);

	afx_msg void OleAddLabel(long nLabelID, OLE_COLOR clrOleColor, LPCTSTR pcszName);
//  afx_msg void OleRemoveAll();

	afx_msg LPDISPATCH OleFind(long nID);

	//afx_msg void OleInitDefaultValues();

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarEventLabels);

	DECLARE_ENUM_VARIANT(CXTPCalendarEventLabels);
	//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class define Calendar Event Category properties.
//     Visual effects for Event Categories (event and day's cells background
//     colors) implemented in Office 2007 theme only. If one or more categories
//     set for event - the category's colors are used, otherwise label's colors
//     used (is label defined). For Office 2003 theme label's colors used in
//     any case.
// See Also: CXTPCalendarEvent, CXTPCalendarEventCategories
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarEventCategory : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNCREATE(CXTPCalendarEventCategory)
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default class constructor.
	// Parameters:
	//      nID         - Category identifier.
	//      pcszName    - Category name.
	//      clrBorder   - Event border color.
	//      clrBkBase   - Base color to calculate few background colors for event
	//                    background and other elements.
	//-----------------------------------------------------------------------
	CXTPCalendarEventCategory(UINT nID = 0, LPCTSTR pcszName = _T(""), COLORREF clrBorder = 0,
							  COLORREF clrBkBase = 0);
	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarEventCategory();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a unique category ID.
	// Returns:
	//     A unique category ID.
	// See Also: SetID
	//-----------------------------------------------------------------------
	UINT GetID() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set a unique category ID.
	// Parameters:
	//     nID - A unique category ID.
	// See Also: GetID
	//-----------------------------------------------------------------------
	void SetID(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a category name.
	// Returns:
	//     A category name.
	// See Also: SetName
	//-----------------------------------------------------------------------
	CString GetName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set a category name.
	// Parameters:
	//     pcszName - A category name.
	// See Also: GetName
	//-----------------------------------------------------------------------
	void SetName(LPCTSTR pcszName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain an event border color.
	// Returns:
	//     An event border color.
	// See Also: SetBorderColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetBorderColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set an event border color.
	// Parameters:
	//     clrColor - An event border color.
	// See Also: GetBorderColor
	//-----------------------------------------------------------------------
	virtual void SetBorderColor(COLORREF clrColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain a background base color.
	// Returns:
	//     A background base color.
	// See Also: SetBkBaseColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetBkBaseColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set a background base color.
	// Parameters:
	//     clrColor - A background base color.
	// See Also: GetBkBaseColor
	//-----------------------------------------------------------------------
	virtual void SetBkBaseColor(COLORREF clrColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain an event background color.
	// Returns:
	//     An event background gradient color.
	// See Also: GetBkBaseColor, SetBkBaseColor
	//-----------------------------------------------------------------------
	virtual CXTPPaintManagerColorGradient& GetBackgroundColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain All Day Events area background color.
	// Returns:
	//     All Day Events area background color.
	// See Also: GetBkBaseColor, SetBkBaseColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetBkColorAllDayEvents() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain Day View Work Cell background color.
	// Returns:
	//     A Day View Work Cell background color.
	// See Also: GetBkBaseColor, SetBkBaseColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetBkColorWorkCell() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain Day View NonWork Cell background color.
	// Returns:
	//     A Day View NonWork Cell background color.
	// See Also: GetBkBaseColor, SetBkBaseColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetBkColorNonWorkCell() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to obtain Day View Cell border color.
	// Returns:
	//     A Day View Cell border color.
	// See Also: GetBkBaseColor, SetBkBaseColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetColorCellBorder() const;

protected:
	//      nID         -
	//      pcszName    - Category name.
	//      clrBorder   - Event border color.
	//      clrBkBase   - Base color to calculate few background colors for event
	//                    background and other elements.

	UINT     m_nID;         // Stores category identifier.
	CString  m_strName;     // Stores category name.

	COLORREF m_clrBorder;   // Stores Event border color.
	COLORREF m_clrBkBase;   // Stores Base color to calculate few background colors for event background and other elements.

	CXTPPaintManagerColorGradient m_grclrBackground; // Stores an event background gradient color.

protected:
#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarEventCategory);
	DECLARE_OLECREATE_EX(CXTPCalendarEventCategory);

	OLE_COLOR OleGetBorderColor();
	void OleSetBorderColor(OLE_COLOR oleColor);

	LPDISPATCH OleGetBackground(); // implemented in the CalendarThemesAxParts.cpp

	//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class is used as a collection of CXTPCalendarEventCategory
//     objects.
// See Also: CXTPCalendarEventCategory, CXTPCalendarEvent
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarEventCategories : public
						CXTPCalendarPtrCollectionT<CXTPCalendarEventCategory>
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNCREATE(CXTPCalendarEventCategories)
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//CXTPCalendarEventCategories(BOOL bDefaultColorsSet);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarEventCategories();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	// Remarks:
	//     Handles class members deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarEventCategories();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to fill the collection using
	//     a default label list.
	//-----------------------------------------------------------------------
	virtual void InitDefaultValues();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to search for the event category with
	//     a specified ID.
	// Parameters:
	//     nID - An UINT that contains the category ID.
	// Returns:
	//     A pointer to a CXTPCalendarEventCategory object or NULL.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarEventCategory* Find(UINT nID) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Finds event category index in the collection by its ID.
	// Parameters:
	//     nID - An UINT that contains the category ID.
	// Returns:
	//     Category index in the collection.
	// See Also: Find
	//-----------------------------------------------------------------------
	virtual int FindIndex(UINT nID) const;

protected:
	CString LoadCategoryName(UINT nIDResource) const;

#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarEventCategories);
	DECLARE_OLECREATE_EX(CXTPCalendarEventCategories);
	DECLARE_ENUM_VARIANT(CXTPCalendarEventCategories);

	afx_msg void OleSetItem(long nIndex, LPDISPATCH pdispItem);
	afx_msg void OleAdd(LPDISPATCH pdispItem);
	afx_msg LPDISPATCH OleFind(long nID);

	//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class represents the collection of UINT elements.
// Remarks:
//     It also implements and some useful operations on elements.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarUIntArray : public CXTPArrayT<UINT, UINT>
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarUIntArray)
	//}}AFX_CODEJOCK_PRIVATE
public:

	//{{AFX_CODEJOCK_PRIVATE
	typedef CXTPArrayT<UINT, UINT> TBase;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to find ID in the collection.
	// Parameters:
	//     uID - ID to find.
	// Returns:
	//     Zero based index in the collection or -1 if element does not find.
	//-----------------------------------------------------------------------
	int Find(UINT uID) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove ID from the collection.
	// Parameters:
	//     uID - ID to remove.
	// Returns:
	//     TRUE if specified ID find and removed, FALSE otherwise.
	// See Also: CArray::RemoveAt, CArray::RemoveAll
	//-----------------------------------------------------------------------
	virtual BOOL RemoveID(UINT uID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store collection data as comma separated
	//     string.
	// Returns:
	//     Comma separated string with collection elements.
	// See Also: LoadFromString
	//-----------------------------------------------------------------------
	virtual CString SaveToString() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to read collection data from comma separated
	//     string.
	// Parameters:
	//     pcszData - String with collection data.
	// See Also: SaveToString
	//-----------------------------------------------------------------------
	virtual void LoadFromString(LPCTSTR pcszData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load an collection data using the
	//     specified data object.
	// Parameters:
	//     pPX         - Source or destination CXTPPropExchange data object reference.
	//     pcszSection - Section name for elements set.
	//     pcszElement - Element value property name.
	// Remarks:
	//     This member function is used to store or load event data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszSection, LPCTSTR pcszElement);

protected:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_ENUM_VARIANTLIST(CXTPCalendarUIntArray);

	void OleLoadFromString(LPCTSTR strData);
	BSTR OleSaveToString();
//}}AFX_CODEJOCK_PRIVATE
#endif
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// Summary:
//     This class represents the collection of custom icons IDs.
// Remarks:
//     It also implements and some useful operations on custom icons
//     IDs collection.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarIconIDs : public CXTPCalendarUIntArray
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarIconIDs)
	//}}AFX_CODEJOCK_PRIVATE
public:

	//{{AFX_CODEJOCK_PRIVATE
	typedef CXTPCalendarUIntArray TBase;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// See Also: ~CXTPCalendarIconIDs()
	//-----------------------------------------------------------------------
	CXTPCalendarIconIDs();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	// See Also: CXTPCalendarIconIDs()
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarIconIDs();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a collection data using the
	//     specified data object.
	// Parameters:
	//     pPX      - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load event data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPCalendarIconIDs);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class represents the collection of event categories IDs.
// Remarks:
//     It also implements and some useful operations.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarEventCategoryIDs : public CXTPCalendarUIntArray
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarEventCategoryIDs)
	//}}AFX_CODEJOCK_PRIVATE
public:

	//{{AFX_CODEJOCK_PRIVATE
	typedef CXTPCalendarUIntArray TBase;
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPCalendarEventCategoryIDs();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarEventCategoryIDs();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a collection data using the
	//     specified data object.
	// Parameters:
	//     pPX      - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load event data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPCalendarEventCategoryIDs);
//}}AFX_CODEJOCK_PRIVATE
#endif

};

AFX_INLINE void CXTPCalendarIconIDs::DoPropExchange(CXTPPropExchange* pPX) {
	TBase::DoPropExchange(pPX, _T("CustomIcons"), _T("IconID"));
}
AFX_INLINE void CXTPCalendarEventCategoryIDs::DoPropExchange(CXTPPropExchange* pPX) {
	TBase::DoPropExchange(pPX, _T("EventCategories"), _T("CategoryID"));
}
AFX_INLINE UINT CXTPCalendarEventCategory::GetID() const {
	return m_nID;
}
AFX_INLINE void CXTPCalendarEventCategory::SetID(UINT nID) {
	m_nID = nID;
}
AFX_INLINE CString CXTPCalendarEventCategory::GetName() const {
	return m_strName;
}
AFX_INLINE void CXTPCalendarEventCategory::SetName(LPCTSTR pcszName) {
	m_strName = pcszName;
}
AFX_INLINE COLORREF CXTPCalendarEventCategory::GetBorderColor() const {
	return m_clrBorder;
}
AFX_INLINE void CXTPCalendarEventCategory::SetBorderColor(COLORREF clrColor) {
	m_clrBorder = clrColor;
}
AFX_INLINE COLORREF CXTPCalendarEventCategory::GetBkBaseColor() const {
	return m_clrBkBase;
}
AFX_INLINE void CXTPCalendarEventCategory::SetBkBaseColor(COLORREF clrColor) {
	m_clrBkBase = clrColor;
}


#endif // !defined(_XTPCALENDAREVENTLABEL_H__)
