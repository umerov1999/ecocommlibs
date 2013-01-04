// XTPChartAxisStrips.h
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
#if !defined(__XTPCHARTAXISSTRIPS_H__)
#define __XTPCHARTAXISSTRIPS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartAxis;
class CXTPChartFillStyle;
//===========================================================================
// Summary:
//     This is a hybrid object of CXTPChartElement and CXTPChartLegendItem.
//     This class represents a strip parallel to an axis.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisStrip : public CXTPChartElement, public CXTPChartLegendItem
{
	DECLARE_DYNCREATE(CXTPChartAxisStrip)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisStrip object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisStrip();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisStrip object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisStrip();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the strip visible/invisible.
	// Parameters:
	//     bVisible - A boolean TRUE to make the strip visible, FALSE to make
	//                the axis strip invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the axis strip is visible.
	// Returns:
	//     A boolean TRUE if the axis strip is visible, FALSE if the strip
	//     is invisible.
	// Remarks:
	//-----------------------------------------------------------------------
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

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the name for the strip.
	// Parameters:
	//     lpszName - A string value denoting the name of the axis strip.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetName(const CXTPChartString& lpszName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function get the name of the axix strip.
	// Returns:
	//      A string value denoting the name of the axis strip.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the legend item name for the strip.
	// Parameters:
	//     lpszName - A string value denoting the legend item name of the axis strip.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetLegendName(const CXTPChartString& lpszName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function get the legend item name of the axix strip.
	// Returns:
	//      A string value denoting the legend item name of the axis strip.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetLegendName() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the fill style of the axis strip.
	// Returns:
	//     Returns a pointer to CXTPChartFillStyle object which represents the
	//     fill style of the of the axis strip.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartFillStyle* GetFillStyle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the minimum value of the strip in the axis.
	// Returns:
	//     Returns a string describing the minimum value of the strip in the axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartString GetAxisMinValue() const;
	double GetAxisMinValueInternal() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the minimum value of the strip in the axis.
	// Parameters:
	//     lpszValue - a string describing the minimum value of the strip in the axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetAxisMinValue(LPCTSTR lpszValue);
	void SetAxisMinValue(double dValue);


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the maximum value of the strip in the axis.
	// Returns:
	//     Returns a string describing the maximum value of the strip in the axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartString GetAxisMaxValue() const;
	double GetAxisMaxValueInternal() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the maximum value of the strip in the axis.
	// Parameters:
	//     lpszValue - a string describing the maximum value of the strip in the axis.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetAxisMaxValue(LPCTSTR lpszValue);
	void SetAxisMaxValue(double dValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual color of the axis strip.
	// Returns:
	//     A chart color object, representing an ARGB value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualColor() const;

	CXTPChartColor GetActualColor2() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the apparent color of the axis strip.
	// Returns:
	//     A CXTPChartColor object representing the ARGB color value, selected.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the color of the axis strip.
	// Parameters:
	//     color - A chart color object, representing an ARGB value.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetColor(const CXTPChartColor& color);

	CXTPChartColor GetColor2() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the color of the axis strip.
	// Parameters:
	//     color - A chart color object, representing an ARGB value.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetColor2(const CXTPChartColor& color);


public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of an axis strip in the chart.
	// Parameters:
	//     pDC      - Pointer to a CXTPChartDeviceContext object.
	//     rcBounds - The bounding rectangle of the axis strip.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the axis strip.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);

#ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisStrip);

	BSTR OleGetText();
	void OleSetText(LPCTSTR lpszText);
	VARIANT OleGetAxisMinValue();
	void OleSetAxisMinValue(const VARIANT& var);
	VARIANT OleGetAxisMaxValue();
	void OleSetAxisMaxValue(const VARIANT& var);

	BOOL OleGetLegendVisible();
	void OleSetLegendVisible(BOOL bValue);
	BOOL OleGetVisible();
	void OleSetVisible(BOOL bValue);
	LPDISPATCH OleGetFillStyle();

	OLE_COLOR OleGetColor();
	OLE_COLOR OleGetColor2();
	void OleSetColor(OLE_COLOR clr);
	void OleSetColor2(OLE_COLOR clr);


//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	BOOL m_bLegendVisible;                  //TRUE if the legend item is visible, FALSE if not visible.
	BOOL m_bVisible;                        //TRUE if the strip is visible, FALSE if not visible.

	CXTPChartString m_strName;               //The name of the strip.
	CXTPChartString m_strLegendName;         //The name of the legend item.

	CXTPChartString m_strAxisMinValue;       //The minimum value of the strip in the axis.
	CXTPChartString m_strAxisMaxValue;       //The maximum value of the strip in the axis.

	double m_dAxisMinValue;       //The minimum value of the strip in the axis.
	double m_dAxisMaxValue;       //The maximum value of the strip in the axis.

	CXTPChartColor m_clrColor;               //The color of the strip.
	CXTPChartColor m_clrColor2;               //The color of the strip.

	CXTPChartFillStyle* m_pFillStyle;        //The fill style used for the background painting.

	friend class CXTPChartAxisStrips;
};

//===========================================================================
// Summary:
//     This class encapsulate a collection of chart axis strip objects.
//     CXTPChartAxisStrips class is designed as a kind of CXTPChartElement.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisStrips : public CXTPChartElementCollection
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisStrips object.
	// Parameters:
	//     pAxis - The pointer to a chart axix object, to which the strips are associated.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisStrips(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisStrip object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisStrips();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a new strip to the collection.
	// Parameters:
	//     pStrip - A pointer to an axis strip object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisStrip* Add(CXTPChartAxisStrip* pStrip);


	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get a strip at a particular index in the strips collection.
	// Parameters:
	//     nIndex - The zero based index of the item of choice.
	// Returns:
	//     An pointer to the strip object in the selected index.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisStrip* GetAt(int nIndex) const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartAxisStrips);

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	DECLARE_ENUM_VARIANT(CXTPChartAxisStrips)

	afx_msg LPDISPATCH OleAdd(LPCWSTR lpszTitle);
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
};

AFX_INLINE CXTPChartAxisStrip* CXTPChartAxisStrips::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrElements.GetSize() ? (CXTPChartAxisStrip*)m_arrElements.GetAt(nIndex) : NULL;
}

AFX_INLINE CXTPChartString CXTPChartAxisStrip::GetName() const {
	return m_strName;
}
AFX_INLINE void CXTPChartAxisStrip::SetVisible(BOOL bVisible) {
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisStrip::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE CXTPChartFillStyle* CXTPChartAxisStrip::GetFillStyle() const {
	return m_pFillStyle;
}
AFX_INLINE CXTPChartString CXTPChartAxisStrip::GetAxisMinValue() const {
	return m_strAxisMinValue;
}
AFX_INLINE double CXTPChartAxisStrip::GetAxisMinValueInternal() const {
	return m_dAxisMinValue;
}
AFX_INLINE void CXTPChartAxisStrip::SetAxisMinValue(LPCTSTR lpszValue) {
	m_strAxisMinValue = lpszValue;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisStrip::SetAxisMinValue(double dValue) {
	m_dAxisMinValue = dValue;
	m_strAxisMinValue.Empty();
	OnChartChanged();
}
AFX_INLINE CXTPChartString CXTPChartAxisStrip::GetAxisMaxValue() const {
	return m_strAxisMaxValue;
}
AFX_INLINE double CXTPChartAxisStrip::GetAxisMaxValueInternal() const {
	return m_dAxisMaxValue;
}
AFX_INLINE void CXTPChartAxisStrip::SetAxisMaxValue(LPCTSTR lpszValue) {
	m_strAxisMaxValue = lpszValue;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisStrip::SetAxisMaxValue(double dValue) {
	m_dAxisMaxValue = dValue;
	m_strAxisMaxValue.Empty();
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisStrip::IsLegendVisible() const {
	return m_bLegendVisible;
}
AFX_INLINE void CXTPChartAxisStrip::SetLegendVisible(BOOL bLegendVisible) {
	m_bLegendVisible = bLegendVisible;
	OnChartChanged();
}
AFX_INLINE CXTPChartColor CXTPChartAxisStrip::GetColor() const {
	return m_clrColor;
}
AFX_INLINE CXTPChartColor CXTPChartAxisStrip::GetColor2() const {
	return m_clrColor2;
}
AFX_INLINE void CXTPChartAxisStrip::SetLegendName(const CXTPChartString& lpszName) {
	m_strLegendName = lpszName;
	OnChartChanged();
}

#endif //#if !defined(__XTPCHARTAXISStripS_H__)
