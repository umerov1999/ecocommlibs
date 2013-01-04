// XTPChartRadarDiagram.h
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
#if !defined(__XTPCHARTRADARDIAGRAM_H__)
#define __XTPCHARTRADARDIAGRAM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartRadarDiagramAppearance;
class CXTPChartDiagram2DAppearance;
class CXTPChartFillStyle;

//===========================================================================
// Summary:
//     Radar diagram style
// Remarks:
//===========================================================================
enum XTPChartRadarDiagramStyle
{
	xtpChartRadarDiagramCircle,
	xtpChartRadarDiagramPolygon
};

//===========================================================================
// Summary:
//     This class represents a chart 2D diagram, which is a kind of CXTPChartDiagram.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarDiagram : public CXTPChartDiagram
{
	DECLARE_SERIAL(CXTPChartRadarDiagram)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarDiagram object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarDiagram();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRadarDiagram object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRadarDiagram();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the X axis associated with the 2D diagram.
	// Returns:
	//     A pointer to CXTPChartAxis object, the specific type is CXTPChartAxisX.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxisX() const;
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the Y axis associated with the 2D diagram.
	// Returns:
	//     A pointer to CXTPChartAxis object, the specific type is CXTPChartAxisY.
	//-------------------------------------------------------------------------
	CXTPChartAxis* GetAxisY() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the diagram appearance object is associated
	//     with this object.
	// Returns:
	//     A pointer to CXTPChartRadarDiagramAppearance object.
	//-------------------------------------------------------------------------
	CXTPChartDiagram2DAppearance* GetAppearance() const;

	CXTPChartFillStyle* GetFillStyle() const;

	CXTPChartColor GetBorderColor() const;

	CXTPChartColor GetActualBackgroundColor() const;
	CXTPChartColor GetActualBackgroundColor2() const;
	CXTPChartColor GetActualBorderColor() const;

	CXTPChartFillStyle* GetActualFillStyle() const;


	CXTPChartColor GetBackgroundColor() const;
	CXTPChartColor GetBackgroundColor2() const;

	void SetBackgroundColor(const CXTPChartColor& color);
	void SetBackgroundColor2(const CXTPChartColor& color);
	void SetBorderColor(const CXTPChartColor& color);

	int GetStartAngle() const;
	void SetStartAngle(int nAngle);


	void SetStyle(XTPChartRadarDiagramStyle style);
	XTPChartRadarDiagramStyle GetStyle() const;


public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of 2D diagram object.
	// Parameters:
	//     pDC     - A pointer to chart device context object.
	//     pParent - A pointer to the parent view.
	// Returns:
	//     A pointer to CXTPChartDiagramView object.
	//-------------------------------------------------------------------------
	virtual CXTPChartDiagramView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent);

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to update the layout if there are some changes in
	//     the diagram.
	// Parameters:
	//     pDC      - A pointer to chart device context object.
	//     rcBounds - The rectangular diagram boundary.
	// Remarks:
	//-------------------------------------------------------------------------
	void UpdateLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView, CRect rcBounds);


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartRadarDiagram);
	DECLARE_OLECREATE_EX(CXTPChartRadarDiagram)
	LPDISPATCH OleGetAxisX();
	LPDISPATCH OleGetAxisY();

	OLE_COLOR OleGetBackgroundColor();
	void OleSetBackgroundColor(OLE_COLOR clr);
	OLE_COLOR OleGetBackgroundColor2();
	void OleSetBackgroundColor2(OLE_COLOR clr);
	OLE_COLOR OleGetBorderColor();
	void OleSetBorderColor(OLE_COLOR clr);
	LPDISPATCH OleGetBackgroundFillStyle();


//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartAxis* m_pAxisX;             //The pointer to X axis object.
	CXTPChartAxis* m_pAxisY;             //The pointer to Y axis object.

	CXTPChartFillStyle* m_pFillStyle;
	CXTPChartColor m_clrBackgroundColor;
	CXTPChartColor m_clrBackgroundColor2;
	CXTPChartColor m_clrBorderColor;

	int m_nStartAngle;
	XTPChartRadarDiagramStyle m_nStyle;

	friend class CXTPChartRadarDiagramView;
};


AFX_INLINE CXTPChartAxis* CXTPChartRadarDiagram::GetAxisX() const {
	return m_pAxisX;
}
AFX_INLINE CXTPChartAxis* CXTPChartRadarDiagram::GetAxisY() const {
	return m_pAxisY;
}
AFX_INLINE CXTPChartFillStyle* CXTPChartRadarDiagram::GetFillStyle() const {
	return m_pFillStyle;
}
AFX_INLINE void CXTPChartRadarDiagram::SetBackgroundColor(const CXTPChartColor& color) {
	m_clrBackgroundColor = color;
	OnChartChanged(xtpChartUpdateRedraw);
}
AFX_INLINE void CXTPChartRadarDiagram::SetBackgroundColor2(const CXTPChartColor& color) {
	m_clrBackgroundColor2 = color;
	OnChartChanged(xtpChartUpdateRedraw);
}
AFX_INLINE void CXTPChartRadarDiagram::SetBorderColor(const CXTPChartColor& color) {
	m_clrBorderColor = color;
	OnChartChanged(xtpChartUpdateRedraw);
}
AFX_INLINE int CXTPChartRadarDiagram::GetStartAngle() const {
	return m_nStartAngle;
}
AFX_INLINE void CXTPChartRadarDiagram::SetStartAngle(int nAngle) {
	m_nStartAngle = nAngle;
	OnChartChanged();
}
AFX_INLINE void CXTPChartRadarDiagram::SetStyle(XTPChartRadarDiagramStyle style) {
	m_nStyle = style;
	OnChartChanged();
}
AFX_INLINE XTPChartRadarDiagramStyle CXTPChartRadarDiagram::GetStyle() const {
	return m_nStyle;
}


#endif //#if !defined(__XTPCHARTRADARDIAGRAM_H__)
