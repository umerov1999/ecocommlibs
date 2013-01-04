// XTPChartSeries.h
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
#if !defined(__XTPCHARTSERIES_H__)
#define __XTPCHARTSERIES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartSeriesPointCollection;
class CXTPChartSeriesPointView;
class CXTPChartSeriesPoint;
class CXTPChartContent;
class CXTPChartDiagramView;
class CXTPChartDiagram;
class CXTPChartSeriesStyle;
class CXTPChartNumberFormat;

//===========================================================================
// Summary:
//     This class represents a series in the chart, which is a kind of
//     CXTPChartElement.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSeries : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartSeries)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSeries object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeries();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartSeries object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSeries();

public:

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the collection of the series points.
	// Returns:
	//     A pointer to CXTPChartSeriesPointCollection object.
	//-------------------------------------------------------------------------
	CXTPChartSeriesPointCollection* GetPoints() const;

	void SetPoints(CXTPChartSeriesPointCollection* pPoints);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the series style of the chart series.
	// Returns:
	//     A pointer to CXTPChartSeriesStyle object, the type of the specific
	//     object depends on the style of series chosen, eg: bar style, bubble
	//     style etc.
	//-------------------------------------------------------------------------
	CXTPChartSeriesStyle* GetStyle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the series style of the chart series.
	// Parameters:
	//     pView - A pointer to CXTPChartSeriesStyle object, the type of the specific
	//             object depends on the style of series chosen, eg: bar style, bubble
	//             style etc.
	//-------------------------------------------------------------------------
	CXTPChartSeriesStyle* SetStyle(CXTPChartSeriesStyle* pView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to set the diagram object associated with this object.
	// Parameters:
	//     pDiagram - A pointer to CXTPChartDiagram object, the type of the specific
	//                object depends on the diagram type chosen, eg:2D or 3D diagram.
	//
	//-------------------------------------------------------------------------
	BOOL SetDiagram(CXTPChartDiagram* pDiagram);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the diagram object associated with this object.
	// Returns:
	//     A pointer to CXTPChartDiagram object, the type of the specific
	//     object depends on the diagram type using, eg:2D or 3D diagram.
	//-------------------------------------------------------------------------
	CXTPChartDiagram* GetDiagram() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to set the scale type used for the argument.
	// Parameters:
	//     scaleType - An enumerated value specifying the scale type used.The common scale
	//                 types used are Qualitative, Numerical and DateTime.
	//-------------------------------------------------------------------------
	void SetArgumentScaleType(XTPChartScaleType scaleType);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the scale type used for the argument.
	// Returns:
	//     An enumerated value specifying the scale type used.The common scale
	//     types used are Qualitative, Numerical and DateTime.
	//-------------------------------------------------------------------------
	XTPChartScaleType GetArgumentScaleType() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to set the scale type used for the value.
	// Parameters:
	//     scaleType - An enumerated value specifying the scale type used.The common scale
	//                 types used are Qualitative, Numerical and DateTime.
	//-------------------------------------------------------------------------
	void SetValueScaleType(XTPChartScaleType scaleType);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the scale type used for the value.
	// Returns:
	//     An enumerated value specifying the scale type used.The common scale
	//     types used are Qualitative, Numerical and DateTime.
	//-------------------------------------------------------------------------
	XTPChartScaleType GetValueScaleType() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the palette index.
	// Returns:
	//     An integer value specifying the palette index.
	//-------------------------------------------------------------------------
	int GetPaletteIndex() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the name of the chart series.
	// Parameters:
	//     lpszName - A LPCSTR value specifying the name of the series.
	//-------------------------------------------------------------------------
	void SetName(LPCTSTR lpszName);
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the name of the chart series.
	// Returns:
	//     A LPCSTR value specifying the name of the series.
	//-------------------------------------------------------------------------
	CXTPChartString GetName() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the name of the legend item.
	// Parameters:
	//     lpszLegendText - A LPCSTR value specifying the name of the legend item
	//                      for this series.
	//-------------------------------------------------------------------------
	void SetLegendText(LPCTSTR lpszLegendText);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to set the name of the legend item.
	// Parameters:
	//     lpszLegendText - A LPCSTR value specifying the name of the legend item
	//                      for this series.
	//-------------------------------------------------------------------------
	CXTPChartString GetLegendText() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get pointer to CXTPChartNumberFormat that specify how to convert number to string for legend
	// Returns:
	//     Pointer to CXTPChartNumberFormat class
	// See Also: CXTPChartNumberFormat
	//-------------------------------------------------------------------------
	CXTPChartNumberFormat* GetPointLegendFormat() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Obsolete method. Use GetPointLegendFormat()->SetPattern(lpszFormat) instead
	// Parameters:
	//     lpszFormat - New Pattern to be set
	// See Also: CXTPChartNumberFormat
	//-------------------------------------------------------------------------
	void SetPointLegendFormat(LPCTSTR lpszFormat);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the series visible/invisible.
	// Parameters:
	//     bVisible - TRUE to make the series visible, and FALSE to make it invisible.
	//-------------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to set the series visible/invisible.
	// Parameters:
	//     bVisible - TRUE to make the series visible, and FALSE to make it invisible.
	//-------------------------------------------------------------------------
	BOOL IsVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the legend item visible/invisible.
	// Parameters:
	//     bVisible - A boolean TRUE to make the strip visible, FALSE to make
	//                the legend item invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetLegendVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the legend item is visible.
	// Returns:
	//     A boolean TRUE if the axis strip is visible, FALSE if the legend item
	//     is invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IsLegendVisible() const;


public:
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	void Release();

protected:
	CXTPChartSeriesStyle* m_pStyle;              //The series style object.
	CXTPChartSeriesPointCollection* m_pPoints;   //The collection of points which form the series.
	XTPChartScaleType m_nArgumentScaleType;      //The scale type of the argument.
	XTPChartScaleType m_nValueScaleType;         //The scale type of the value.

	CXTPChartDiagram* m_pDiagram;                //The diagram object.

	CXTPChartString m_strName;                   //The name of the series.
	CXTPChartString m_strLegendText;             //The legend text of the series.
	CXTPChartNumberFormat* m_pPointLegendFormat; //The legend text format of the series.

	BOOL m_bVisible;                             //TRUE if the series visible and FALSE if it is invisible.
	BOOL m_bLegendVisible;                       //TRUE if the legend item is visible, FALSE if not visible.

	int m_nPaletteIndex;                        //The palette index.



protected:
	friend class CXTPChartSeriesCollection;
	friend class CXTPChartContent;
};

//===========================================================================
// Summary:
//     This class represents the collection of series in the chart, which is a
//     kind of CXTPChartElement.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSeriesCollection : public CXTPChartElementCollection
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSeriesCollection object.
	// Parameters:
	//     pOwner - A pointer to the chart container object, which owns the series
	//              collection.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesCollection(CXTPChartContent* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartSeriesCollection object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSeriesCollection();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to add a series to the collection.
	// Parameters:
	//     pSeries - A pointer to the chart series, which is to be added to the
	//               collection.
	//-------------------------------------------------------------------------
	CXTPChartSeries* Add(CXTPChartSeries* pSeries);


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get a series at a particular index in the series
	//     collection.
	// Returns:
	//     A pointer to chart series object.
	//-------------------------------------------------------------------------
	CXTPChartSeries* GetAt(int nIndex) const;


public:
	virtual void DoPropExchange(CXTPPropExchange* pPX);


protected:


	friend class CXTPChartContent;
};


AFX_INLINE CXTPChartSeriesPointCollection* CXTPChartSeries::GetPoints() const {
	return m_pPoints;
}
AFX_INLINE CXTPChartSeriesStyle* CXTPChartSeries::GetStyle() const {
	return m_pStyle;
}

AFX_INLINE CXTPChartSeries* CXTPChartSeriesCollection::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrElements.GetSize() ? (CXTPChartSeries*)m_arrElements.GetAt(nIndex) : NULL;
}
AFX_INLINE CXTPChartDiagram* CXTPChartSeries::GetDiagram() const {
	return m_pDiagram;
}

AFX_INLINE void CXTPChartSeries::SetArgumentScaleType(XTPChartScaleType scaleType) {
	m_nArgumentScaleType = scaleType;
	OnChartChanged();
}
AFX_INLINE XTPChartScaleType CXTPChartSeries::GetArgumentScaleType() const {
	return m_nArgumentScaleType;
}
AFX_INLINE void CXTPChartSeries::SetValueScaleType(XTPChartScaleType scaleType) {
	m_nValueScaleType = scaleType;
	OnChartChanged();
}
AFX_INLINE XTPChartScaleType CXTPChartSeries::GetValueScaleType() const {
	return m_nValueScaleType;
}
AFX_INLINE int CXTPChartSeries::GetPaletteIndex() const {
	return m_nPaletteIndex;
}

AFX_INLINE void CXTPChartSeries::SetName(LPCTSTR lpszName) {
	m_strName = lpszName;
	OnChartChanged();
}
AFX_INLINE CXTPChartString CXTPChartSeries::GetName() const {
	return m_strName;
}
AFX_INLINE void CXTPChartSeries::SetLegendText(LPCTSTR lpszLegendText) {
	m_strLegendText = lpszLegendText;
	OnChartChanged();
}
AFX_INLINE CXTPChartString CXTPChartSeries::GetLegendText() const {
	return m_strLegendText;
}
AFX_INLINE void CXTPChartSeries::SetVisible(BOOL bVisible) {
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartSeries::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE CXTPChartNumberFormat* CXTPChartSeries::GetPointLegendFormat() const {
	return m_pPointLegendFormat;
}
AFX_INLINE BOOL CXTPChartSeries::IsLegendVisible() const {
	return m_bLegendVisible;
}
AFX_INLINE void CXTPChartSeries::SetLegendVisible(BOOL bLegendVisible) {
	m_bLegendVisible = bLegendVisible;
	OnChartChanged();
}


#endif //#if !defined(__XTPCHARTSERIES_H__)
