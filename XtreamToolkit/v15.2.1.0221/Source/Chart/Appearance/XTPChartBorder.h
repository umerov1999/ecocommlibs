// XTPChartBorder.h
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
#if !defined(__XTPCHARTBORDER_H__)
#define __XTPCHARTBORDER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartColor;
class CXTPChartDeviceCommand;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     This class abstracts the border of a chart, it is a kind of chart element
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBorder : public CXTPChartElement
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBorder object.
	// Parameters:
	//     pOwner     - The owner element of this element.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBorder(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBorder object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartBorder();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the chart border is visible or not.
	// Returns:
	//     A BOOL value of TURE if the border is visible and FALSE if the not.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IsVisible() const;

	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function returns the color of the border.
	// Returns:
	//     A CXTPChartColor object denote the ARGB value of the border.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetColor() const;
	void SetColor(const CXTPChartColor& clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the thickness of the chart border.
	// Returns:
	//     An integer value tells the thickness of the border.
	// Remarks:
	//-----------------------------------------------------------------------
	int GetThickness() const;

	void SetThickness(int Thickness);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the border of the chart.
	// Parameters:
	//     rcBounds  - The bounding rectange.
	//     color     - Reference to a chart color object.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     handles the rendering of an element in the chart.Here it handles the drawing
	//     of the border of the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(const CXTPChartRectF& rcBounds, const CXTPChartColor& color);
	CXTPChartDeviceCommand* CreateInnerBorderDeviceCommand(const CXTPChartRectF& rcBounds, const CXTPChartColor& color);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store/Load the properties of chart boundary
	//     object using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);


protected:

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartBorder);
	void OleSetColor(OLE_COLOR clr);
	OLE_COLOR OleGetColor();
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartColor m_clrColor;       //The boundary color.
	int m_nThickness;               //The thickness of the boundary.
	BOOL m_bVisible;                //TRUE if the boundary is visible, FALSE if invisible.
};


AFX_INLINE BOOL CXTPChartBorder::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE void CXTPChartBorder::SetVisible(BOOL bVisible) {
	m_bVisible = bVisible;
	OnChartChanged();
}
AFX_INLINE CXTPChartColor CXTPChartBorder::GetColor() const {
	return m_clrColor;
}
AFX_INLINE void CXTPChartBorder::SetColor(const CXTPChartColor& clr) {
	m_clrColor = clr;
	OnChartChanged();
}
AFX_INLINE int CXTPChartBorder::GetThickness() const {
	return m_nThickness;
}
AFX_INLINE void CXTPChartBorder::SetThickness(int nThickness) {
	m_nThickness = nThickness;
	OnChartChanged();
}


#endif //#if !defined(__XTPCHARTBORDER_H__)
