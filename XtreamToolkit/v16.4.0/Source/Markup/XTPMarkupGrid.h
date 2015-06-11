// XTPMarkupGrid.h: interface for the CXTPMarkupGrid class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPMARKUPGRID_H__)
#define __XTPMARKUPGRID_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupBuilder;
class CXTPMarkupUIElement;

//===========================================================================
// Summary: CXTPMarkupGridLength is class defined length of the grid rows and columns
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupGridLength : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupGridLength);
public:
	//===========================================================================
	// Summary: Unit type of the length
	//===========================================================================
	enum GridUnitType
	{
		unitTypeAuto,  // Automatically calculated length
		unitTypePixel, // Length in pixels
		unitTypeStar   // Length in percents
	};
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupGridLength object
	// Parameters:
	//     nValue - Length of the grid elements
	//      type - Type of the Length units
	//-----------------------------------------------------------------------
	CXTPMarkupGridLength(double nValue = 0, GridUnitType type = unitTypePixel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves type of the length units
	// Returns:
	//     unitTypePixel if length calculated in pixels and unitTypeStar if its calculated in percents
	//-----------------------------------------------------------------------
	GridUnitType GetUnitType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves value of the length
	// Returns:
	//     Length in pixels or percents.
	//-----------------------------------------------------------------------
	double GetValue() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if length uses percent units
	// Returns:
	//     TRUE if length uses percent units
	//-----------------------------------------------------------------------
	BOOL IsStar() const {
		return m_type == unitTypeStar;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if length is Auto type
	// Returns:
	//     TRUE if type of the length is unitTypeAuto
	//-----------------------------------------------------------------------
	BOOL IsAuto() const {
		return m_type == unitTypeAuto;
	}

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Helper method used to convert String to Length
	// Parameters:
	//     pBuilder - Pointer to builder object
	//     pObject - String representation of the Length
	// Returns
	//     New CXTPMarkupGridLength object
	//-----------------------------------------------------------------------
	CXTPMarkupObject* ConvertFrom(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject) const;

public:
	double m_nValue;     // Value
	GridUnitType m_type; // Unit type
};

//===========================================================================
// Summary: CXTPMarkupDefinitionBase is base class for CXTPMarkupColumnDefinition and CXTPMarkupRowDefinition
//          that defines constrains and definitions for Grid rows and columns
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupDefinitionBase : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupDefinitionBase);
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupDefinitionBase object
	// Parameters:
	//     bIsColumnDefinition - TRUE for CXTPMarkupColumnDefinition, FALSE for CXTPMarkupRowDefinition
	//-----------------------------------------------------------------------
	CXTPMarkupDefinitionBase(BOOL bIsColumnDefinition = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns minimum user defined size
	// Returns:
	//     Minimum constrain for column or row
	//-----------------------------------------------------------------------
	int GetUserMinSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns maximum user defined size
	// Returns:
	//     Maximum constrain for column or row
	//-----------------------------------------------------------------------
	int GetUserMaxSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns user defined size
	// Returns:
	//     User defined size for column or row
	//-----------------------------------------------------------------------
	CXTPMarkupGridLength* GetUserSize() const;

//{{AFX_CODEJOCK_PRIVATE
	// Implementation
private:
	void OnBeforeLayout();
	void UpdateMinSize(int nMinSize);
	double GetPreferredSize() const;

protected:
	static void AFX_CDECL OnDefinitionPropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* /*e*/);

protected:
	BOOL m_bIsColumnDefinition;
	int m_nMinSize;
	int m_nSizeType;
	double m_nMeasureSize;
	double m_nSizeCache;
	int m_nFinalOffset;

	friend class CXTPMarkupGrid;
//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupDefinitionBase);
//}}AFX_CODEJOCK_PRIVATE
#endif

};

//===========================================================================
// Summary: CXTPMarkupColumnDefinition is base CXTPMarkupDefinitionBase derived class.
//          It implements ColumnDefinition XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupColumnDefinition : public CXTPMarkupDefinitionBase
{
	DECLARE_MARKUPCLASS(CXTPMarkupColumnDefinition);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupColumnDefinition object
	//-----------------------------------------------------------------------
	CXTPMarkupColumnDefinition();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set minimum width constraint for column
	// Parameters:
	//     nWidth - New minimum width constraint
	//-----------------------------------------------------------------------
	void SetMinWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set maximum width constraint for column
	// Parameters:
	//     nWidth - New maximum width constraint
	//-----------------------------------------------------------------------
	void SetMaxWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set width for the column
	// Parameters:
	//     nWidth - New width for the column
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set width for the column
	// Parameters:
	//     pLength - New width for the column
	//-----------------------------------------------------------------------
	void SetWidth(CXTPMarkupGridLength* pLength);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupColumnDefinition);

	afx_msg void OleSetMinWidth(int nWidth);
	afx_msg void OleSetMaxWidth(int nWidth);
	afx_msg void OleSetWidth(int nWidth);
	afx_msg int OleGetMinWidth();
	afx_msg int OleGetMaxWidth();
	afx_msg int OleGetWidth();
	afx_msg int OleGetUnitType();
	afx_msg void OleSetUnitType(int nType);
//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	static CXTPMarkupDependencyProperty* m_pWidthProperty;
	static CXTPMarkupDependencyProperty* m_pMinWidthProperty;
	static CXTPMarkupDependencyProperty* m_pMaxWidthProperty;
};

//===========================================================================
// Summary: CXTPMarkupRowDefinition is base CXTPMarkupDefinitionBase derived class.
//          It implements RowDefinition XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupRowDefinition : public CXTPMarkupDefinitionBase
{
	DECLARE_MARKUPCLASS(CXTPMarkupRowDefinition);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupRowDefinition object
	//-----------------------------------------------------------------------
	CXTPMarkupRowDefinition();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set minimum height constraint for the row
	// Parameters:
	//     nHeight - New minimum height constraint
	//-----------------------------------------------------------------------
	void SetMinHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set maximum height constraint for row
	// Parameters:
	//     nHeight - New maximum height constraint
	//-----------------------------------------------------------------------
	void SetMaxHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set height for the row
	// Parameters:
	//     nHeight - New height for the row
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set height for the row
	// Parameters:
	//     pLength - New height for the row
	//-----------------------------------------------------------------------
	void SetHeight(CXTPMarkupGridLength* pLength);

//{{AFX_CODEJOCK_PRIVATE
	// Implementation
public:
	static CXTPMarkupDependencyProperty* m_pHeightProperty;
	static CXTPMarkupDependencyProperty* m_pMinHeightProperty;
	static CXTPMarkupDependencyProperty* m_pMaxHeightProperty;
//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupRowDefinition);

	afx_msg void OleSetMinHeight(int nHeight);
	afx_msg void OleSetMaxHeight(int nHeight);
	afx_msg void OleSetHeight(int nHeight);
	afx_msg int OleGetMinHeight();
	afx_msg int OleGetMaxHeight();
	afx_msg int OleGetHeight();
	afx_msg int OleGetUnitType();
	afx_msg void OleSetUnitType(int nType);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary: CXTPMarkupDefinitionCollection is base class for CXTPMarkupDefinitionCollection and CXTPMarkupDefinitionCollection
//          It implements RowDefinition XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupDefinitionCollection : public CXTPMarkupCollection
{
	DECLARE_MARKUPCLASS(CXTPMarkupDefinitionCollection);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupDefinitionCollection object
	//-----------------------------------------------------------------------
	CXTPMarkupDefinitionCollection();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns CXTPMarkupDefinitionBase by its index
	// Parameters:
	//     nIndex - Index of CXTPMarkupDefinitionBase to retrieve
	// Returns: CXTPMarkupDefinitionBase by its index
	//-----------------------------------------------------------------------
	CXTPMarkupDefinitionBase* GetItem(int nIndex) const;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupDefinitionCollection);
	DECLARE_ENUM_VARIANT(CXTPMarkupDefinitionCollection)

	long OleGetItemCount();
	LPDISPATCH OleGetItem(long nIndex);
	void OleAdd(LPDISPATCH lpElementDisp);
	void OleInsert(long Index, LPDISPATCH lpElementDisp);

//}}AFX_CODEJOCK_PRIVATE
#endif

};

//===========================================================================
// Summary: CXTPMarkupRowDefinitionCollection is CXTPMarkupDefinitionCollection derived class
//          It implements collection of CXTPMarkupRowDefinition classes
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupRowDefinitionCollection : public CXTPMarkupDefinitionCollection
{
	DECLARE_MARKUPCLASS(CXTPMarkupRowDefinitionCollection);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupRowDefinitionCollection object
	//-----------------------------------------------------------------------
	CXTPMarkupRowDefinitionCollection();
};

//===========================================================================
// Summary: CXTPMarkupColumnDefinitionCollection is CXTPMarkupDefinitionCollection derived class
//          It implements collection of CXTPMarkupColumnDefinition classes
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupColumnDefinitionCollection : public CXTPMarkupDefinitionCollection
{
	DECLARE_MARKUPCLASS(CXTPMarkupColumnDefinitionCollection);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupColumnDefinitionCollection object
	//-----------------------------------------------------------------------
	CXTPMarkupColumnDefinitionCollection();
};

//===========================================================================
// Summary: CXTPMarkupGrid is CXTPMarkupPanel derived class. It implements Grid XAML Tag.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupGrid : public CXTPMarkupPanel
{
private:
	enum LayoutTimeSizeType
	{
		sizeTypeNone = 0,
		sizeTypePixel = 1,
		sizeTypeAuto = 2,
		sizeTypeStar = 4
	};

private:
	struct CELLCACHE
	{
		int nColumnIndex;
		int nRowIndex;
		int nColumnSpan;
		int nRowSpan;
		int nSizeTypeU;
		int nSizeTypeV;
		int nNext;
	public:
		BOOL IsStarU() const;
		BOOL IsAutoU() const;
		BOOL IsStarV() const;
		BOOL IsAutoV() const;
	};

	struct SPANKEY
	{
		int nCount;
		int nStart;
		BOOL bU;
		int nValue;
	};

private:
	DECLARE_MARKUPCLASS(CXTPMarkupGrid)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupGrid object
	//-----------------------------------------------------------------------
	CXTPMarkupGrid();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupGrid object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupGrid();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns collection of constrants and definitions for Grid columns
	// Returns:
	//     Pointer to collection of CXTPMarkupColumnDefinition classes
	//-----------------------------------------------------------------------
	CXTPMarkupColumnDefinitionCollection* GetColumnDefinitions() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns collection of constrants and definitions for Grid rows
	// Returns:
	//     Pointer to collection of CXTPMarkupRowDefinition classes
	//-----------------------------------------------------------------------
	CXTPMarkupRowDefinitionCollection* GetRowDefinitions() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set column index for grid child element
	// Parameters:
	//     pElement - Child element of the grid
	//     nColumn - Column index to assign for pElement
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetColumn(CXTPMarkupUIElement* pElement, int nColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves column index of grid child element
	// Parameters:
	//     pElement - Child element of the grid to check
	// Returns:
	//     Index of the column for pElement element
	//-----------------------------------------------------------------------
	static int AFX_CDECL GetColumn(CXTPMarkupUIElement* pElement);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set row index for grid child element
	// Parameters:
	//     pElement - Child element of the grid
	//     nRow - Row index to assign for pElement
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetRow(CXTPMarkupUIElement* pElement, int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves row index of grid child element
	// Parameters:
	//     pElement - Child element of the grid to check
	// Returns:
	//     Index of the row for pElement element
	//-----------------------------------------------------------------------
	static int AFX_CDECL GetRow(CXTPMarkupUIElement* pElement);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set total number of columns that child content spans within a Grid.
	// Parameters:
	//     pElement - Child element of the grid
	//     nColumnSpan - Number of columns that child content spans
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetColumnSpan(CXTPMarkupUIElement* pElement, int nColumnSpan);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves total number of columns that child content spans within a Grid.
	// Parameters:
	//     pElement - Child element of the grid to check
	// Returns:
	//     Number of columns that child content spans
	//-----------------------------------------------------------------------
	static int AFX_CDECL GetColumnSpan(CXTPMarkupUIElement* pElement);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set total number of rows that child content spans within a Grid.
	// Parameters:
	//     pElement - Child element of the grid
	//     nRowSpan - Number of rows that child content spans
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetRowSpan(CXTPMarkupUIElement* pElement, int nRowSpan);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves total number of rows that child content spans within a Grid.
	// Parameters:
	//     pElement - Child element of the grid to check
	// Returns:
	//     Number of rows that child content spans
	//-----------------------------------------------------------------------
	static int AFX_CDECL GetRowSpan(CXTPMarkupUIElement* pElement);

//{{AFX_CODEJOCK_PRIVATE
	// Implementation

protected:
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize availableSize);
	CSize ArrangeOverride(CSize arrangeSize);
	void SetPropertyObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue);

private:
	void ValidateDefinitionsLayout(CXTPMarkupDefinitionCollection* pDefinitions, BOOL bTreatStarAsAuto);
	void ValidateCells();
	void ValidateDefinitions();
	int GetLengthTypeForRange(CXTPMarkupDefinitionCollection* pDefinitions, int start, int count);
	void MeasureCellsGroup(CXTPMarkupDrawingContext* pDC, int cellsHead, CSize referenceSize, BOOL ignoreDesiredSizeU, BOOL forceInfinityV);
	void MeasureCell(CXTPMarkupDrawingContext* pDC, int cell, BOOL forceInfinityV);
	int GetMeasureSizeForRange(CXTPMarkupDefinitionCollection* pDefinitions, int start, int count);
	int CalculateDesiredSize(CXTPMarkupDefinitionCollection* pDefinitions);
	void ResolveStar(CXTPMarkupDefinitionCollection* pDefinitions, double availableSize);
	int GetFinalSizeForRange(CXTPMarkupDefinitionCollection* pDefinitions, int start, int count);
	void SetFinalSize(CXTPMarkupDefinitionCollection* pDefinitions, int finalSize);
	void EnsureMinSizeInDefinitionRange(CXTPMarkupDefinitionCollection* definitions, int start, int count, int requestedSize);

	static int _cdecl DistributionOrderComparer(const void *arg1, const void *arg2);
	static int _cdecl StarDistributionOrderComparer(const void *arg1, const void *arg2);
	static int _cdecl SpanMaxDistributionOrderComparer(const void *arg1, const void *arg2);
	static int _cdecl SpanPreferredDistributionOrderComparer(const void *arg1, const void *arg2);

	static void AFX_CDECL OnCellAttachedPropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* e);

protected:
	CXTPMarkupRowDefinitionCollection* m_pRowDefinitions;
	CXTPMarkupColumnDefinitionCollection* m_pColumnDefinitions;

public:
	static CXTPMarkupDependencyProperty* m_pColumnDefinitionsProperty;
	static CXTPMarkupDependencyProperty* m_pRowDefinitionsProperty;
	static CXTPMarkupDependencyProperty* m_pRowProperty;
	static CXTPMarkupDependencyProperty* m_pColumnProperty;

	static CXTPMarkupDependencyProperty* m_pColumnSpanProperty;
	static CXTPMarkupDependencyProperty* m_pRowSpanProperty;

private:
	CELLCACHE* m_pCellCachesCollection;
	int m_nCellGroup[4];
	BOOL m_bHasStarCellsU;
	BOOL m_bHasStarCellsV;
	BOOL m_bHasGroup2CellsInAutoRows;

	CXTPMarkupDefinitionCollection* m_pDefinitionsU;
	CXTPMarkupDefinitionCollection* m_pDefinitionsV;

	friend struct CELLCACHE;
	friend class CXTPMarkupDefinitionBase;

//}}AFX_CODEJOCK_PRIVATE

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupGrid);

	afx_msg int OleGetRow(LPDISPATCH lpElementDisp);
	afx_msg void OleSetRow(LPDISPATCH lpElementDisp, int nValue);
	afx_msg int OleGetRowSpan(LPDISPATCH lpElementDisp);
	afx_msg void OleSetRowSpan(LPDISPATCH lpElementDisp, int nValue);
	afx_msg int OleGetColumn(LPDISPATCH lpElementDisp);
	afx_msg void OleSetColumn(LPDISPATCH lpElementDisp, int nValue);
	afx_msg int OleGetColumnSpan(LPDISPATCH lpElementDisp);
	afx_msg void OleSetColumnSpan(LPDISPATCH lpElementDisp, int nValue);
	afx_msg LPDISPATCH OleGetColumnDefinitions();
	afx_msg LPDISPATCH OleGetRowDefinitions();
//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE CXTPMarkupColumnDefinitionCollection* CXTPMarkupGrid::GetColumnDefinitions() const {
	return m_pColumnDefinitions;
}
AFX_INLINE CXTPMarkupRowDefinitionCollection* CXTPMarkupGrid::GetRowDefinitions() const {
	return m_pRowDefinitions;
}
AFX_INLINE CXTPMarkupDefinitionBase* CXTPMarkupDefinitionCollection::GetItem(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrItems.GetSize() ? (CXTPMarkupDefinitionBase*)m_arrItems[nIndex] : NULL;
}
AFX_INLINE CXTPMarkupGridLength::GridUnitType CXTPMarkupGridLength::GetUnitType() const {
	return m_type;
};
AFX_INLINE double CXTPMarkupGridLength::GetValue() const {
	return m_nValue;
}

AFX_INLINE void CXTPMarkupColumnDefinition::SetMinWidth(int nWidth) {
	SetValue(m_pMinWidthProperty, new CXTPMarkupInt(nWidth));
}
AFX_INLINE void CXTPMarkupColumnDefinition::SetMaxWidth(int nWidth) {
	SetValue(m_pMaxWidthProperty, new CXTPMarkupInt(nWidth));
}
AFX_INLINE void CXTPMarkupColumnDefinition::SetWidth(int nWidth) {
	SetValue(m_pWidthProperty, new CXTPMarkupGridLength(nWidth));
}
AFX_INLINE void CXTPMarkupColumnDefinition::SetWidth(CXTPMarkupGridLength* pLength) {
	SetValue(m_pWidthProperty, pLength);
}
AFX_INLINE void CXTPMarkupRowDefinition::SetMinHeight(int nHeight) {
	SetValue(m_pMinHeightProperty, new CXTPMarkupInt(nHeight));
}
AFX_INLINE void CXTPMarkupRowDefinition::SetMaxHeight(int nHeight) {
	SetValue(m_pMaxHeightProperty, new CXTPMarkupInt(nHeight));
}
AFX_INLINE void CXTPMarkupRowDefinition::SetHeight(int nHeight) {
	SetValue(m_pHeightProperty, new CXTPMarkupGridLength(nHeight));
}
AFX_INLINE void CXTPMarkupRowDefinition::SetHeight(CXTPMarkupGridLength* pLength) {
	SetValue(m_pHeightProperty, pLength);
}



#endif // !defined(__XTPMARKUPGRID_H__)
