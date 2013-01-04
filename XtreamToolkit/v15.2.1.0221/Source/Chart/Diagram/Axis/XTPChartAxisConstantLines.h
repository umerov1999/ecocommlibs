// XTPChartAxisConstantLines.h
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
#if !defined(__XTPCHARTAXISCONSTANTLINES_H__)
#define __XTPCHARTAXISCONSTANTLINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartLineStyle;
class CXTPChartLineStyle;

//===========================================================================
// Summary:
//     CXTPChartAxisConstantLine is a hybrid object derived from CXTPChartTextElement
//     and CXTPChartLegendItem, this class represents a constant line parallel
//     to an axis.Normally this class is own by an axis object(CXTPChartaxis).
//
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisConstantLine : public CXTPChartTextElement, public CXTPChartLegendItem
{
	DECLARE_DYNCREATE(CXTPChartAxisConstantLine)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisConstantLine object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisConstantLine();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisConstantLine object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisConstantLine();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the line visible.
	// Parameters:
	//     bVisible - BOOL, TRUE value for set the line visible and FALSE
	//     to hide the constant line.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the axis constat line is visible or not.
	// Return:
	//     BOOL value TRUE  if the line visible and FALSE if the line is invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the corresponding legend item visible or not.
	// Parameters:
	//     bVisible - BOOL, TRUE value for set the legend visible and FALSE
	//     to hide the legend.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetLegendVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the corresponding legend item is visible or not.
	// Returns:
	//     BOOL value TRUE if the legend item visible and FALSE if the legend item invisible.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IsLegendVisible() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the text of the contant line legend item.
	// Parameters:
	//     lpszTitle - The string value for the new text, its type is CXTPChartString.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetText(const CXTPChartString& lpszTitle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the text of the contant line legend item.
	// Returns:
	//     The string value for the legend item text, its type is CXTPChartString.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetText() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the text color of the contant line legend item.
	// Parameters:
	//     clrTextColor - The CXTPChartColor value for the text color.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetTextColor(const CXTPChartColor& clrTextColor);
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the text color of the contant line legend item.
	// Returns:
	//     The CXTPChartColor value for the text color.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the actual text color of the contant line legend item.
	// Returns:
	//     The CXTPChartColor value for the actual text color.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the font of the contant line legend item text.
	// Returns:
	//     The CXTPChartFont value for the font used to render the text.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFont* GetFont() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the font of the contant line legend item text.
	// Parameters:
	//     pFont - The CXTPChartFont value for the font to be used to render the text.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetFont(CXTPChartFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to set the text alignment of the contant line legend item text.
	// Parameters:
	//     nAlignment - The enumerated value for the text alignment, the common alignments are
	//                  near, far and center.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetAlignment(XTPChartStringAlignment nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the alignment of the contant line legend item text.
	// Returns:
	//     The XTPChartStringAlignment enumeration corresponding to the chosen alignment.
	// Remarks:
	//-----------------------------------------------------------------------
	XTPChartStringAlignment GetAlignment() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the antialiasing ON or OFF.Antialiasing is
	//     the smooothing of sharp edges of text and drawing.
	// Parameters:
	//     bAntialiasing - A BOOL value TRUE to set the antialiasing ON and
	//                     FALSE to OFF the anitialiasing operation.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	void SetAntialiasing(BOOL bAntialiasing);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether antialiasing is enabled or not.
	//     Antialiasing is the smooothing of sharp edges of text and drawing.
	// Returns:
	//     Returns boolean TRUE if the antialiasing is turned on FALSE if the
	//     antialiasing is turned off.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL GetAntialiasing() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the legend name.
	// Returns:
	//     The string value for the legend name.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetLegendName() const;

	void SetLegendName(const CXTPChartString& strName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the constant line style.
	// Returns:
	//     The pointer to an object of type CXTPChartLineStyle.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartLineStyle* GetLineStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the value of the constant line in the axis.
	// Returns:
	//     A string denoting the axis value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetAxisValue() const;
	double GetAxisValueInternal() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the value of the constant line in the axis.
	// Parameters:
	//     lpszValue - A string denoting the axis value.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetAxisValue(LPCTSTR lpszValue);
	void SetAxisValue(double dValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to get the value of the actual color of the constant line.
	// Returns:
	//     A CXTPChartColor object denoting the ARGB value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualColor() const;


	BOOL IsShowBehind() const;
	void SetShowBehind(BOOL bShowBehind);

	void ShowTextBelow(BOOL bShowTextBelow);
	BOOL IsTextBelow() const;

	CXTPChartColor GetColor() const;
	void SetColor(const CXTPChartColor& color);

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the legend item.
	// Parameters:
	//     pDC       - Pointer to a CXTPChartDeviceContext object.
	//     rcBounds  - The bounding rectange.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this polymorphic object handles
	//     the rendering of an element in the chart.Here it handles the drawing
	//     of the the legend item.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartAxisConstantLine);

	BSTR OleGetText();
	void OleSetText(LPCTSTR lpszText);
	VARIANT OleGetAxisValue();
	void OleSetAxisValue(const VARIANT& var);

	BOOL OleGetLegendVisible();
	void OleSetLegendVisible(BOOL bValue);
	BOOL OleGetVisible();
	void OleSetVisible(BOOL bValue);
	BOOL OleGetShowTextBelow();
	void OleSetShowTextBelow(BOOL bValue);
	BOOL OleGetAlignment();
	void OleSetAlignment(int nValue);
	LPDISPATCH OleGetLineStyle();
	LPDISPATCH OleGetFont();

	OLE_COLOR OleGetColor();
	OLE_COLOR OleGetTextColor();
	void OleSetColor(OLE_COLOR clr);
	void OleSetTextColor(OLE_COLOR clr);


//}}AFX_CODEJOCK_PRIVATE
#endif


protected:
	BOOL m_bLegendVisible;                  //TRUE if the legend is visible, FALSE if invisible.
	BOOL m_bVisible;                        //TRUE if the constant line is visible, FALSE if invisible.

	CXTPChartString m_strText;               //The text of the constant line legend item.
	CXTPChartString m_strLegendName;         //The legend name.
	CXTPChartString m_strAxisValue;          //The string value of constant line parallel to an axis.
	double m_dAxisValue;

	CXTPChartColor m_clrTextColor;           //The text color of the constant line legend item.
	CXTPChartColor m_clrColor;               //The color of the constant line.

	XTPChartStringAlignment m_nAlignment;    //The text alignment of the constant line legend item.
	BOOL m_bAntialiasing;                   //TRUE if antialiasing is used, FALSE if not using antialiasing.
	CXTPChartFont* m_pFont;                  //The font used for the text of the constant line legend item.
	BOOL m_bShowTextBelow;                  //TRUE if the text is below, FALSE if not.
	BOOL m_bShowBehind;                     //TRUE if the text is behind, FALSE if not.

	CXTPChartLineStyle* m_pLineStyle;        //The chart line style object.

	friend class CXTPChartAxisConstantLines;
};

//===========================================================================
// Summary:
//     This class encapsulates a collection of CXTPChartAxisConstantLine objects.
// Remarks:
//
class _XTP_EXT_CLASS CXTPChartAxisConstantLines : public CXTPChartElementCollection
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisConstantLines object.
	// Parameters:
	//     pAxis - The pointer to a CXTPChartAxis object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisConstantLines(CXTPChartAxis* pAxis);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisConstantLines object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisConstantLines();


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This function a new constant line to the collection of lines.
	// Parameters:
	//     pConstantLine - The pointer to a CXTPChartAxisConstantLine object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisConstantLine* Add(CXTPChartAxisConstantLine* pConstantLine);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the constant line object at a particular
	//     index in the collection.
	// Parameters:
	//     nIndex - The zero based index of the of item in the collection.
	// Returns:
	//     The pointer to a CXTPChartAxisConstantLine object, at the index selected.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisConstantLine* GetAt(int nIndex) const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

public:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartAxisConstantLines);

	afx_msg long OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long nIndex);
	DECLARE_ENUM_VARIANT(CXTPChartAxisConstantLines)

	afx_msg LPDISPATCH OleAdd(LPCWSTR lpszTitle);
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
};


AFX_INLINE CXTPChartAxisConstantLine* CXTPChartAxisConstantLines::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrElements.GetSize() ? (CXTPChartAxisConstantLine*)m_arrElements.GetAt(nIndex) : NULL;
}


AFX_INLINE CXTPChartString CXTPChartAxisConstantLine::GetText() const {
	return m_strText;
}
AFX_INLINE CXTPChartFont* CXTPChartAxisConstantLine::GetFont() const {
	return m_pFont;
}
AFX_INLINE XTPChartStringAlignment CXTPChartAxisConstantLine::GetAlignment() const {
	return m_nAlignment;
}

AFX_INLINE void CXTPChartAxisConstantLine::SetAntialiasing(BOOL bAntialiasing) {
	m_bAntialiasing = bAntialiasing;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::GetAntialiasing() const {
	return m_bAntialiasing;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetVisible(BOOL bVisible) {
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE CXTPChartLineStyle* CXTPChartAxisConstantLine::GetLineStyle() const {
	return m_pLineStyle;
}
AFX_INLINE CXTPChartString CXTPChartAxisConstantLine::GetAxisValue() const {
	return m_strAxisValue;
}
AFX_INLINE double CXTPChartAxisConstantLine::GetAxisValueInternal() const {
	return m_dAxisValue;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetAxisValue(LPCTSTR lpszValue) {
	m_strAxisValue = lpszValue;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisConstantLine::SetAxisValue(double dValue) {
	m_dAxisValue = dValue;
	m_strAxisValue.Empty();
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::IsLegendVisible() const {
	return m_bLegendVisible;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetLegendVisible(BOOL bLegendVisible) {
	m_bLegendVisible = bLegendVisible;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisConstantLine::ShowTextBelow(BOOL bShowTextBelow) {
	m_bShowTextBelow = bShowTextBelow;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::IsTextBelow() const {
	return m_bShowTextBelow;
}
AFX_INLINE CXTPChartColor CXTPChartAxisConstantLine::GetColor() const {
	return m_clrColor;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetColor(const CXTPChartColor& color) {
	m_clrColor = color;
	OnChartChanged();
}
AFX_INLINE void CXTPChartAxisConstantLine::SetLegendName(const CXTPChartString& strName) {
	m_strLegendName = strName;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartAxisConstantLine::IsShowBehind() const {
	return m_bShowBehind;
}
AFX_INLINE void CXTPChartAxisConstantLine::SetShowBehind(BOOL bShowBehind) {
	m_bShowBehind = bShowBehind;
	OnChartChanged();
}

#endif //#if !defined(__XTPCHARTAXISCONSTANTLINES_H__)
