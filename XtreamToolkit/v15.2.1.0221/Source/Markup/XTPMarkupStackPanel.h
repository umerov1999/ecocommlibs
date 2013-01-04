// XTPMarkupStackPanel.h: interface for the CXTPMarkupStackPanel class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPMARKUPSTACKPANEL_H__)
#define __XTPMARKUPSTACKPANEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary: CXTPMarkupStackPanel is CXTPMarkupPanel derived class. It implements StackPanel XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupStackPanel : public CXTPMarkupPanel
{
	DECLARE_MARKUPCLASS(CXTPMarkupStackPanel)
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupStackPanel object
	//-----------------------------------------------------------------------
	CXTPMarkupStackPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupStackPanel object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupStackPanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set orientation of child elements inside StackPanel
	// Parameters:
	//     orientation - xtpMarkupOrientationHorizontal to position elements horizontally; xtpMarkupOrientationVertical otherwise.
	//-----------------------------------------------------------------------
	void SetOrientation(XTPMarkupOrientation orientation);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get current orientation of child elements inside StackPanel
	// Returns:xtpMarkupOrientationHorizontal to position elements horizontally; xtpMarkupOrientationVertical otherwise.
	//-----------------------------------------------------------------------
	XTPMarkupOrientation GetOrientation() const;

protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual CSize ArrangeOverride(CSize szFinalSize);

public:
	static CXTPMarkupDependencyProperty* m_pOrientationProperty;
//}}AFX_CODEJOCK_PRIVATE

};

//===========================================================================
// Summary: CXTPMarkupWrapPanel is CXTPMarkupPanel derived class. It implements WrapPanel XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupWrapPanel : public CXTPMarkupPanel
{
	DECLARE_MARKUPCLASS(CXTPMarkupWrapPanel)
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupWrapPanel object
	//-----------------------------------------------------------------------
	CXTPMarkupWrapPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupWrapPanel object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupWrapPanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set orientation of child elements inside StackPanel
	// Parameters:
	//     orientation - xtpMarkupOrientationHorizontal to position elements horizontally; xtpMarkupOrientationVertical otherwise.
	//-----------------------------------------------------------------------
	void SetOrientation(XTPMarkupOrientation orientation);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get current orientation of child elements inside StackPanel
	// Returns:xtpMarkupOrientationHorizontal to position elements horizontally; xtpMarkupOrientationVertical otherwise.
	//-----------------------------------------------------------------------
	XTPMarkupOrientation GetOrientation() const;

protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation

	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual CSize ArrangeOverride(CSize szFinalSize);

protected:
	void ArrangeLine(int nPanelHeight, int nLineHeight, int nFirstElement, int nCount);

public:
	static CXTPMarkupDependencyProperty* m_pOrientationProperty;
	static CXTPMarkupDependencyProperty* m_pItemHeightProperty;
	static CXTPMarkupDependencyProperty* m_pItemWidthProperty;

//}}AFX_CODEJOCK_PRIVATE

};

enum XTPMarkupDock
{
	xtpMarkupDockLeft,
	xtpMarkupDockTop,
	xtpMarkupDockRight,
	xtpMarkupDockBottom
};

//===========================================================================
// Summary: CXTPMarkupDockPanel is CXTPMarkupPanel derived class. It implements DockPanel XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupDockPanel : public CXTPMarkupPanel
{
	DECLARE_MARKUPCLASS(CXTPMarkupDockPanel)
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupDockPanel object
	//-----------------------------------------------------------------------
	CXTPMarkupDockPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupDockPanel object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupDockPanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set dock position of the child elements
	// Parameters:
	//     pElement - Child element to set dock position to
	//     dock - Dock Position of element
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetDock(CXTPMarkupUIElement* pElement, XTPMarkupDock dock);


	//-----------------------------------------------------------------------
	// Summary:
	//     Determines dock position of the child elements
	// Parameters:
	//     pElement - Child element to get dock position.
	//-----------------------------------------------------------------------
	static XTPMarkupDock AFX_CDECL GetDock(CXTPMarkupUIElement* pElement);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to fill last child element inside whole client area of DockPanel.
	// Parameters:
	//     bLastChildFill - TRUE to fill last child element inside client area.
	//-----------------------------------------------------------------------
	void SetLastChildFill(BOOL bLastChildFill);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if last child element is filled inside whole client area of DockPanel.
	// Returns:
	//     TRUE to fill last child element inside client area; FALSE otherwise
	//-----------------------------------------------------------------------
	BOOL GetLastChildFill() const;


protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual CSize ArrangeOverride(CSize szFinalSize);

private:
	static void AFX_CDECL OnDockChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* e);
	static CXTPMarkupObject* AFX_CDECL ConvertDock(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject);

public:
	static CXTPMarkupDependencyProperty* m_pLastChildFillProperty;
	static CXTPMarkupDependencyProperty* m_pDockProperty;
//}}AFX_CODEJOCK_PRIVATE


};


//===========================================================================
// Summary: CXTPMarkupUniformGrid is CXTPMarkupPanel derived class. It implements UniformGrid XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupUniformGrid : public CXTPMarkupPanel
{
	DECLARE_MARKUPCLASS(CXTPMarkupUniformGrid)
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupUniformGrid object
	//-----------------------------------------------------------------------
	CXTPMarkupUniformGrid();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupUniformGrid object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupUniformGrid();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set columns number of the Grid
	// Parameters:
	//     nColumns - Column count to set
	//-----------------------------------------------------------------------
	void SetColumns(int nColumns);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines number of columns of the Grid
	// Returns:
	//     Number of columns of the Grid
	//-----------------------------------------------------------------------
	int GetColumns() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set rows number of the Grid
	// Parameters:
	//     nRows - rows count to set
	//-----------------------------------------------------------------------
	void SetRows(int nRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines number of rows of the Grid
	// Returns:
	//     Number of rows of the Grid
	//-----------------------------------------------------------------------
	int GetRows() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set first column for first row of the grid
	// Parameters:
	//     nFirstColumn - First column position of first row
	//-----------------------------------------------------------------------
	void SetFirstColumn(int nFirstColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines position of first column of first row
	// Parameters:
	//     First column position of first row
	//-----------------------------------------------------------------------
	int GetFirstColumn() const;

protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual CSize ArrangeOverride(CSize szFinalSize);

private:
	void UpdateComputedValues();

private:
	int m_nColumns;
	int m_nRows;
	int m_nFirstColumn;

public:
	static CXTPMarkupDependencyProperty* m_pFirstColumnProperty;
	static CXTPMarkupDependencyProperty* m_pColumnsProperty;
	static CXTPMarkupDependencyProperty* m_pRowsProperty;
//}}AFX_CODEJOCK_PRIVATE

};




AFX_INLINE void CXTPMarkupDockPanel::SetLastChildFill(BOOL bLastChildFill) {
	SetValue(m_pLastChildFillProperty, CXTPMarkupBool::CreateValue(bLastChildFill));
}
AFX_INLINE BOOL CXTPMarkupDockPanel::GetLastChildFill() const {
	CXTPMarkupBool* pLastChildFill = MARKUP_STATICCAST(CXTPMarkupBool, GetValue(m_pLastChildFillProperty));
	return pLastChildFill != NULL ? (BOOL)*pLastChildFill : TRUE;
}
AFX_INLINE int CXTPMarkupUniformGrid::GetColumns() const {
	CXTPMarkupInt* pColumns = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pColumnsProperty));
	return pColumns != NULL ? (int)*pColumns : 0;
}
AFX_INLINE int CXTPMarkupUniformGrid::GetRows() const{
	CXTPMarkupInt* pRows = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pRowsProperty));
	return pRows != NULL ? (int)*pRows : 0;
}
AFX_INLINE int CXTPMarkupUniformGrid::GetFirstColumn() const {
	CXTPMarkupInt* pFirstColumn = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pFirstColumnProperty));
	return pFirstColumn != NULL ? (int)*pFirstColumn : 0;
}
AFX_INLINE void CXTPMarkupUniformGrid::SetFirstColumn(int nFirstColumn){
	SetValue(m_pFirstColumnProperty, new CXTPMarkupInt(nFirstColumn));
}
AFX_INLINE void CXTPMarkupUniformGrid::SetColumns(int nColumns){
	SetValue(m_pColumnsProperty, new CXTPMarkupInt(nColumns));
}
AFX_INLINE void CXTPMarkupUniformGrid::SetRows(int nRows){
	SetValue(m_pRowsProperty, new CXTPMarkupInt(nRows));
}


#endif // !defined(__XTPMARKUPSTACKPANEL_H__)
