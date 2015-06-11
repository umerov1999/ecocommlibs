// XTPChartAxis.h
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
#if !defined(__XTPCHARTAXIS_H__)
#define __XTPCHARTAXIS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartDiagram;
class CXTPChartDiagram2D;
class CXTPChartAxisGridLines;
class CXTPChartAxisLabel;
class CXTPChartAxisTitle;
class CXTPChartAxisTickMarks;
class CXTPChartScaleTypeMap;
class CXTPChartAxisRange;
class CXTPChartAxisView;
class CXTPChartDiagramView;
class CXTPChartDeviceContext;
class CXTPChartAxisAppearance;
class CXTPChartFillStyle;
class CXTPChartAxisConstantLines;
class CXTPChartAxisCustomLabels;
class CXTPChartAxisStrips;
class CXTPChartElementView;

//===========================================================================
// Summary:
//     Enumerates the axis alignment.
// Remarks:
//===========================================================================
enum XTPChartAxisAlignment
{
	xtpChartAxisNear,    //For Y axis, near means left side of the chart. For X axis it is the bottom side of the chart.
	xtpChartAxisFar      //For Y axis, near means right side of the chart. For X axis it is the top side of the chart.
};

//===========================================================================
// Summary:
//     Enumerates the axis alignment.
// Remarks:
//===========================================================================
enum XTPChartAxisDateTimeScaleUnit
{
	xtpChartScaleMinute,
	xtpChartScaleHour,
	xtpChartScaleDay,
	xtpChartScaleMonth,
	xtpChartScaleYear,
};

//===========================================================================
// Summary:
//     This abstract base class represents a chart axis as a kind of chart element.
//     It abstarcts all the features of a chart axis.
// Remarks:
//     This is an abstract base class, act as the parent for the classes
//     CXTPChartAxisX and CXTPChartAxisY.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxis : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartAxis)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxis object.
	// Parameters:
	//     pDiagram     - Pointer to a CXTPChartDiagram2D object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxis(CXTPChartDiagram* pDiagram);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxis object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxis();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartAxisView)of the axis.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pParentView - The parent view of the axis.
	// Returns:
	//     Returns an pointer to CXTPChartAxisView object newly created.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartAxisView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function updates the axis, with respect to the series values.
	// Remarks:Call this function whenever there is a change in series values.
	// See Also:
	//-------------------------------------------------------------------------
	void UpdateRange(CXTPChartDeviceContext* pDC, CXTPChartAxisView* pView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Override this method to tell that whether the axis is vertical.
	// Returns:
	//     Returns TRUE if the axis is vertical, FALSE if the axis is not vertical.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsVertical() const = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Override this method to tell that whether the axis is value type.
	// Returns:
	//     Returns TRUE if the axis is value type, FALSE if the axis is not value type.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsValuesAxis() const = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the pointer to chart diagram 2D object associated
	//     with the chart axis object.
	// Returns:
	//     Returns apointer to CXTPChartDiagram2D object.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDiagram* GetDiagram() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the scale type used for the axis.
	// Returns:
	//     Returns an enumerated value specifying the scale type used.
	// Remarks:
	//     The main scale types are - Qualitative, Numerical, Date time.
	// See Also:
	//-------------------------------------------------------------------------
	XTPChartScaleType GetScaleType() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the scale type map object associated with the axis.
	// Returns:
	//     Returns the pointer to a scale type map object.
	// Remarks:CXTPChartScaleTypeMap object keep track of the apparent and internal(actual)
	//         value of a point in the axis.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartScaleTypeMap* GetScaleTypeMap() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Override this function to return the side margin value of the chart axis.
	// Parameters:
	//     nMinValue - The minimum value of the chart axis.
	//     nMaxValue - The maximum value of the chart axis.
	// Returns:
	//     Returns a double value, represents the margin.
	// See Also:
	//-------------------------------------------------------------------------
	virtual double CalcSideMarginsValue(double nMinValue, double nMaxValue) const = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get object which represents the range of the axis.
	// Returns:
	//     Returns the pointer to the chart axis range object.
	// Remarks:
	//     CXTPChartAxisRange object handles the range related things of an axis.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxisRange* GetRange() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get object which represents the label of the axis.
	// Returns:
	//     Returns the pointer to the axis label object.
	// Remarks:CXTPChartAxisLabel object handles the chart label and its appearance.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxisLabel* GetLabel() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get object which represents the axis gridlines.
	// Returns:
	//     Returns the pointer to the axis gridlines object.
	// Remarks:
	//     CXTPChartAxisGridLines object handles the axis gridlines and its appearance.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxisGridLines* GetGridLines() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the associated object which abstracts the axis
	//     tick marks.
	// Returns:
	//     Returns the pointer to the axis tick marks object.
	// Remarks:
	//     CXTPChartAxisTickMarks object handles the axis tick marks and its appearance.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxisTickMarks* GetTickMarks() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the associated object which deals with constant lines
	//     parallel to the axis.
	// Returns:
	//     Returns the pointer to the axis constant lines object.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxisConstantLines* GetConstantLines() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the associated object which deals with custom labels of aixs
	// Returns:
	//     Returns the pointer to the axis custom labels object.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxisCustomLabels* GetCustomLabels() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the associated object which deals with strips
	//     parallel to the axis.
	// Returns:
	//     Returns the pointer to the axis strips object.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxisStrips* GetStrips() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the associated object which deals the axis title
	// Returns:
	//     Returns the pointer to the axis title object.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartAxisTitle* GetTitle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the color of the axis.
	// Returns:
	//     Returns the pointer to CXTPChartColor object, which give ARGB color value.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetActualColor() const;
	CXTPChartColor GetColor() const;
	void SetColor(const CXTPChartColor& color);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the first interlaced strips color of the axis.
	// Returns:
	//     Returns the pointer to CXTPChartColor object, which give ARGB color value.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetInterlacedColor() const;
	CXTPChartColor GetActualInterlacedColor() const;
	void SetInterlacedColor(const CXTPChartColor& color);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get the second interlaced strips color of the axis.
	// Returns:
	//     Returns the pointer to CXTPChartColor object, which give ARGB color value.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartColor GetInterlacedColor2() const;
	CXTPChartColor GetActualInterlacedColor2() const;
	void SetInterlacedColor2(const CXTPChartColor& color);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the second interlaced strips background
	//     fill style.
	// Returns:
	//     Returns the pointer to CXTPChartFillStyle object, which abstracts
	//     various fill styles.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartFillStyle* GetInterlacedFillStyle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the alignment of the axis.
	// Returns:
	//     Returns an enumerated value XTPChartAxisAlignment,corresponding
	//     to the alignment selected.
	// Remarks:
	///     Usage
	//      xtpChartAxisNear - for Y axis, near means left side of the chart
	//                        for X axis it is the bottom side of the chart.
	//      xtpChartAxisFar -  for Y axis, near means right side of the chart
	//                        for X axis it is the top side of the chart.
	//-------------------------------------------------------------------------
	XTPChartAxisAlignment GetAlignment() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to set the alignment of the axis.
	// Parameters:
	//     nAlignment - enumerated value XTPChartAxisAlignment,corresponding
	//     to the alignment to be selected.
	// Remarks:
	///     Usage
	//      xtpChartAxisNear - for Y axis, near means left side of the chart
	//                        for X axis it is the bottom side of the chart.
	//      xtpChartAxisFar -  for Y axis, near means right side of the chart
	//                        for X axis it is the top side of the chart.
	//-------------------------------------------------------------------------
	void SetAlignment(XTPChartAxisAlignment nAlignment);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether the interlaced strips are used.
	// Returns:
	//     Returns TRUE if the interlaced strips are enabled, FALSE if it is
	//     not enabled.
	// Remarks:
	//-------------------------------------------------------------------------
	BOOL IsInterlaced() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to enable/disable the interlaced strips.
	// Parameters:
	//     bInterlaced - TRUE if the interlaced strips are to be enabled, FALSE
	//                  to remove the interlaced strips.
	// Remarks:
	//-------------------------------------------------------------------------
	void SetInterlaced(BOOL bInterlaced);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to get whether the axis is visible.
	// Returns:
	//     Returns TRUE if the axis is visible, FALSE if the axis is
	//     not visible.
	// Remarks:
	//-------------------------------------------------------------------------
	BOOL IsVisible() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to show/hide the axis.
	// Parameters:
	//     bVisible -TRUE to make the axis visible, FALSE to hide the axis.
	// Remarks:
	//-------------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the thickness of the axis line.
	// Returns:
	//     Returns an int value denoting the thickness of the axis line.
	// Remarks:
	//-------------------------------------------------------------------------
	int GetThickness() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the thickness of the axis line.
	// Parameters:
	//     nThicknes - The thickness of the axis line.
	// Remarks:
	//-------------------------------------------------------------------------
	void SetThickness(int nThicknes);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the minor count of the axis  grid lines.
	// Parameters:
	//     nMinorCount - The number of minor grid lines.
	// Remarks:
	//-------------------------------------------------------------------------
	void SetMinorCount(int nMinorCount);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function gives the minor count of the axis  grid lines.
	// Returns:
	//     The number of minor grid lines.
	// Remarks:
	//-------------------------------------------------------------------------
	int GetMinorCount() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to reverse order the axis values.
	// Parameters:
	//     bRevered - TRUE for normal order, FALSE for reverse order.
	// Remarks:
	//-------------------------------------------------------------------------
	void SetReversed(BOOL bRevered);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function gives whether the axis points are reverse ordered or not.
	// Returns:
	//     TRUE for normal order, FALSE for reverse order.
	// Remarks:
	//-------------------------------------------------------------------------
	BOOL IsReversed() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This function returns the chart axis appeance object.
	// Returns:
	//     The pointer to chart axis appeance object.
	// Remarks:
	//-------------------------------------------------------------------------
	CXTPChartAxisAppearance* GetAppearance() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Sets value specifies whether space between grid lines in calculated automatically
	// Parameters:
	//     bGridSpacingAuto - TRUE to calculate space between grid lines automatically
	// Remarks:
	//     Call SetGridSpacing to set this space in case of bGridSpacingAuto = FALSE
	//-------------------------------------------------------------------------
	void SetGridSpacingAuto(BOOL bGridSpacingAuto);

	BOOL GetGridSpacingAuto() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to set interval between grid lines
	// Parameters:
	//     dGridSpacing - Space between grid lines
	// Remarks:
	//     Call SetGridSpacingAuto(FALSE) before
	//-------------------------------------------------------------------------
	void SetGridSpacing(double dGridSpacing);

	double GetGridSpacing() const;


	BOOL IsSecondary() const;

	BOOL IsAllowZoom() const;
	void SetAllowZoom(BOOL bAllowZoom);

	//-------------------------------------------------------------------------
	// Summary:
	//     Set whether logarithmic scale should be used
	// Parameters:
	//     bLogarithmic - TRUE to use logarithmic scale
	// See Also:
	//     SetLogarithmicBase
	//-------------------------------------------------------------------------
	void SetLogarithmic(BOOL bLogarithmic);

	BOOL IsLogarithmic() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Set logarithmic base when logarithmic scale is used
	// Parameters:
	//     dLogarithmicBase - logarithmic base
	// See Also:
	//     SetLogarithmic
	//-------------------------------------------------------------------------
	void SetLogarithmicBase(double dLogarithmicBase);

	double GetLogarithmicBase() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to set single unit for date time scale
	// Parameters:
	//     unit - new units to be set
	// See Also:
	//     SetLogarithmic
	//-------------------------------------------------------------------------
	void SetDateTimeScaleUnit(XTPChartAxisDateTimeScaleUnit unit);

	XTPChartAxisDateTimeScaleUnit GetDateTimeScaleUnit() const;



public:
	void DoPropExchange(CXTPPropExchange* pPX);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxis);

	afx_msg LPDISPATCH OleGetConstantLines();
	afx_msg LPDISPATCH OleGetCustomLabels();
	afx_msg LPDISPATCH OleGetStrips();
	afx_msg LPDISPATCH OleGetLabel();
	afx_msg LPDISPATCH OleGetTickMarks();
	afx_msg LPDISPATCH OleGetTitle();
	afx_msg LPDISPATCH OleGetGridLines();
	afx_msg LPDISPATCH OleGetRange();
	afx_msg LPDISPATCH OleGetInterlacedFillStyle();

	afx_msg OLE_COLOR OleGetColor();
	afx_msg void OleSetColor(OLE_COLOR clr);
	afx_msg OLE_COLOR OleGetInterlacedColor();
	afx_msg void OleSetInterlacedColor(OLE_COLOR clr);
	afx_msg OLE_COLOR OleGetInterlacedColor2();
	afx_msg void OleSetInterlacedColor2(OLE_COLOR clr);

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartDiagram* m_pDiagram;                 //The pointer to chart digram 2d object.
	BOOL m_bVisible;                                //TRUE if the axis is visible, FALSE if invisible.
	BOOL m_bReversed;                               //FALSE if the axis valus are in natural order, TRUE if it is in reverse order.
	int m_nThickness;                               //The thickness of axis line.
	int m_nMinorCount;                              //The number of minors present.
	BOOL m_bSecondary;
	XTPChartAxisAlignment m_nAlignment;             //The chart axis alignment, near or far.
	CXTPChartColor m_clrColor;                      //The chart color.
	CXTPChartColor m_clrInterlacedColor;            //The first interlaced color.
	CXTPChartColor m_clrInterlacedColor2;           //The second interlaced color.
	CXTPChartFillStyle* m_pInterlacedFillStyle;     //The fill style of interlaced strips.

	BOOL m_bInterlaced;                             //TRUE if the chart has interlaced strips parallel to the axis.

	double m_dGridSpacing;                          //The spacing of the grid lines.
	BOOL m_bGridSpacingAuto;                        //TURE if automatic spaceing of grid lines selected.FALSE for specific values.
	BOOL m_bAllowZoom;

	BOOL m_bLogarithmic;                            // Logarithmic
	double m_dLogarithmicBase;                      // Logarithic base

	CXTPChartAxisGridLines* m_pGridLines;           //Pointer to axis gridlines object.
	CXTPChartAxisLabel* m_pLabel;                   //Pointer to axis lable object.
	CXTPChartAxisTitle* m_pTitle;                   //Pointer to axis title object.
	CXTPChartAxisTickMarks* m_pTickMarks;           //Pointer to axis tick marks object.
	CXTPChartScaleTypeMap* m_pScaleTypeMap;         //Pointer to axis scale type object.
	CXTPChartAxisRange* m_pRange;                   //Pointer to axis range object.
	CXTPChartAxisConstantLines* m_pConstantLines;   //Pointer to axis constant lines object.
	CXTPChartAxisCustomLabels* m_pCustomLabels;     //Pointer to axis custom labels object.
	CXTPChartAxisStrips* m_pStrips;                 //Pointer to axis strips object.

	XTPChartScaleType m_nScaleType;                 //Enumeated value denote the axis scale type.

	XTPChartAxisDateTimeScaleUnit m_nDateTimeScaleUnit;

	friend class CXTPChartDiagram2DAxisView;
	friend class CXTPChartRadarAxisView;
	friend class CXTPChartRadarAxisXView;
	friend class CXTPChartRadarAxisYView;
	friend class CXTPChartDiagram2D;
};

//===========================================================================
// Summary:
//     CXTPChartAxisY specialized class from CXTPChartAxis class represents the X axis
//     of a chart.
// Remarks:It includes the features, specific to X axis.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisX : public CXTPChartAxis
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisX object.
	// Parameters:
	//     pDiagram     - Pointer to a CXTPChartDiagram2D object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisX(CXTPChartDiagram2D* pDiagram);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This is the override of base the class member, it tells that whether X axis is vertical.
	// Returns:
	//     Returns TRUE if the X axis is vertical, FALSE if the X axis is not vertical.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsVertical() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This is the override of base the class member,tells that whether the X axis is value type.
	// Returns:
	//     Returns TRUE if the X axis is value type, FALSE if the X axis is not value type.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsValuesAxis() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This override returns the side margin value of the chart X axis.
	// Parameters:
	//     nMinValue - The minimum value of the chart X axis.
	//     nMaxValue - The maximum value of the chart X axis.
	// Returns:
	//     Returns a double value, represents the margin.
	// See Also:
	//-------------------------------------------------------------------------
	virtual double CalcSideMarginsValue(double nMinValue, double nMaxValue) const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartAxisView)of the axis.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pParentView - The parent view of the axis.
	// Returns:
	//     Returns an pointer to CXTPChartAxisView object newly created.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartAxisView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);

};

//===========================================================================
// Summary:
//     CXTPChartAxisY specialized class from CXTPChartAxis class represents the Y axis
//     of a chart.
// Remarks:It includes the features, specific to Y axis.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisY : public CXTPChartAxis
{
public:
	CXTPChartAxisY(CXTPChartDiagram2D* pDiagram);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This is the override of base the class member, it tells that whether Y axis is vertical.
	// Returns:
	//     Returns TRUE if the Y axis is vertical, FALSE if the Y axis is not vertical.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsVertical() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This is the override of base the class member,tells that whether the Y axis is value type.
	// Returns:
	//     Returns TRUE if the Y axis is value type, FALSE if the Y axis is not value type.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual BOOL IsValuesAxis() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This override returns the side margin value of the chart Y axis.
	// Parameters:
	//     nMinValue - The minimum value of the chart Y axis.
	//     nMaxValue - The maximum value of the chart Y axis.
	// Returns:
	//     Returns a double value, represents the margin.
	// See Also:
	//-------------------------------------------------------------------------
	virtual double CalcSideMarginsValue(double nMinValue, double nMaxValue) const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view (CXTPChartAxisView)of the axis.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pParentView - The parent view of the axis.
	// Returns:
	//     Returns an pointer to CXTPChartAxisView object newly created.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartAxisView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView);

};

AFX_INLINE CXTPChartDiagram* CXTPChartAxis::GetDiagram() const {
	return m_pDiagram;
}
AFX_INLINE XTPChartScaleType CXTPChartAxis::GetScaleType() const {
	return m_nScaleType;
}
AFX_INLINE CXTPChartAxisRange* CXTPChartAxis::GetRange() const {
	return m_pRange;
}
AFX_INLINE CXTPChartAxisLabel* CXTPChartAxis::GetLabel() const {
	return m_pLabel;
}

AFX_INLINE CXTPChartAxisGridLines* CXTPChartAxis::GetGridLines() const {
	return m_pGridLines;
}
AFX_INLINE CXTPChartAxisConstantLines* CXTPChartAxis::GetConstantLines() const {
	return m_pConstantLines;
}
AFX_INLINE CXTPChartAxisCustomLabels* CXTPChartAxis::GetCustomLabels() const {
	return m_pCustomLabels;
}
AFX_INLINE CXTPChartAxisStrips* CXTPChartAxis::GetStrips() const {
	return m_pStrips;
}
AFX_INLINE CXTPChartAxisTickMarks* CXTPChartAxis::GetTickMarks() const {
	return m_pTickMarks;
}
AFX_INLINE BOOL CXTPChartAxis::IsInterlaced() const {
	return m_bInterlaced;
}
AFX_INLINE void CXTPChartAxis::SetInterlaced(BOOL bInterlaced) {
	m_bInterlaced = bInterlaced;
	OnChartChanged();
}
AFX_INLINE CXTPChartScaleTypeMap* CXTPChartAxis::GetScaleTypeMap() const {
	return m_pScaleTypeMap;
}
AFX_INLINE CXTPChartAxisTitle* CXTPChartAxis::GetTitle() const {
	return m_pTitle;
}
AFX_INLINE XTPChartAxisAlignment CXTPChartAxis::GetAlignment() const {
	return m_nAlignment;
}
AFX_INLINE void CXTPChartAxis::SetAlignment(XTPChartAxisAlignment nAlignment) {
	m_nAlignment = nAlignment;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxis::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE void CXTPChartAxis::SetVisible(BOOL bVisible) {
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE int CXTPChartAxis::GetThickness() const {
	return m_nThickness;
}
AFX_INLINE void CXTPChartAxis::SetThickness(int nThickness) {
	m_nThickness = nThickness;
	OnChartChanged();
}
AFX_INLINE int CXTPChartAxis::GetMinorCount() const {
	return m_nMinorCount;
}
AFX_INLINE void CXTPChartAxis::SetMinorCount(int nMinorCount) {
	m_nMinorCount = nMinorCount;
	OnChartChanged();
}

AFX_INLINE void CXTPChartAxis::SetReversed(BOOL bRevered) {
	m_bReversed = bRevered;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxis::IsReversed() const {
	return m_bReversed;
}
AFX_INLINE BOOL CXTPChartAxis::GetGridSpacingAuto() const {
	return m_bGridSpacingAuto;
}
AFX_INLINE void CXTPChartAxis::SetGridSpacingAuto(BOOL bGridSpacingAuto) {
	m_bGridSpacingAuto = bGridSpacingAuto;
	OnChartChanged();
}
AFX_INLINE double CXTPChartAxis::GetGridSpacing() const {
	return m_dGridSpacing;
}
AFX_INLINE void CXTPChartAxis::SetGridSpacing(double dGridSpacing) {
	m_dGridSpacing = dGridSpacing;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxis::IsSecondary() const {
	return m_bSecondary;
}
AFX_INLINE void CXTPChartAxis::SetColor(const CXTPChartColor& color) {
	m_clrColor = color;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxis::SetInterlacedColor(const CXTPChartColor& color) {
	m_clrInterlacedColor = color;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxis::SetInterlacedColor2(const CXTPChartColor& color) {
	m_clrInterlacedColor2 = color;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxis::IsAllowZoom() const {
	return m_bAllowZoom;
}
AFX_INLINE void CXTPChartAxis::SetAllowZoom(BOOL bAllowZoom) {
	m_bAllowZoom = bAllowZoom;
}
AFX_INLINE void CXTPChartAxis::SetLogarithmic(BOOL bLogarithmic) {
	m_bLogarithmic = bLogarithmic;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxis::IsLogarithmic() const {
	return m_bLogarithmic;
}
AFX_INLINE void CXTPChartAxis::SetLogarithmicBase(double dLogarithmicBase) {
	m_dLogarithmicBase = dLogarithmicBase;
	OnChartChanged();
}
AFX_INLINE double CXTPChartAxis::GetLogarithmicBase() const {
	return m_dLogarithmicBase;
}
AFX_INLINE void CXTPChartAxis::SetDateTimeScaleUnit(XTPChartAxisDateTimeScaleUnit unit) {
	m_nDateTimeScaleUnit = unit;
	OnChartChanged();
}
AFX_INLINE XTPChartAxisDateTimeScaleUnit CXTPChartAxis::GetDateTimeScaleUnit() const {
	return m_nDateTimeScaleUnit;
}


#endif //#if !defined(__XTPCHARTAXIS_H__)
