// XTPChartContent.h
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
#if !defined(__XTPCHARTCONTENT_H__)
#define __XTPCHARTCONTENT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartDeviceContext;
class CXTPChartDiagram;
class CXTPChartTitleCollection;
class CXTPChartElementView;
class CXTPChartSeriesCollection;
class CXTPChartLegend;
class CXTPChartPanelCollection;
class CXTPChartSeries;
class CXTPChartAppearance;
class CXTPChartLegendView;
class CXTPPropExchange;
class CXTPMarkupContext;
class CXTPChartContentView;
class CXTPChartDiagramView;
class CXTPChartBorder;
class CXTPChartDeviceCommand;

enum XTPChartPanelDirection
{
	xtpChartPanelVertical,
	xtpChartPanelHorizontal
};


//===========================================================================
// Summary:
//     This class represents the chart content, which is a kind of CXTPChartElement.
//     This class has a panel collection, series collection, title collection
//     and legend.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartContent : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartContent)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartContent object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartContent();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartContent object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartContent();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the title collection object.
	// Returns:
	//     A pointer to CXTPChartTitleCollection object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTitleCollection* GetTitles() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the series collection object.
	// Returns:
	//     A pointer to CXTPChartSeriesCollection object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesCollection* GetSeries() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the chart legend object.
	// Returns:
	//     A pointer to CXTPChartLegend object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartLegend* GetLegend() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the primary diagram of the chart. Its the diagram of first Panel.
	// Returns:
	//     A pointer to CXTPChartDiagram object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDiagram* GetPrimaryDiagram() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the panels of the chart.
	// Returns:
	//     A pointer to CXTPChartPanelCollection object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPanelCollection* GetPanels() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the appearance of the chart.
	// Returns:
	//     A pointer to CXTPChartAppearance object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAppearance* GetAppearance() const;

	CXTPChartBorder* GetBorder() const;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the content.
	// Parameters:
	//     pDC      - A pointer to the chart device context object.
	//     rcBounds - The bound of the chart content.
	// Remarks:
	//-----------------------------------------------------------------------
	void DrawContent(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the device command tree.
	// Parameters:
	//     hDC      - The GDI device context HANDLE.
	//     rcBounds - The rectangle of the device context.
	// Returns:
	//     A pointer to CXTPChartDeviceCommand object - root of Command tree.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceComand(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the device context for the drawing.
	// Parameters:
	//     hDC      - The GDI device context HANDLE.
	//     rcBounds - The rectangle of the device context.
	// Returns:
	//     A pointer to CXTPChartDeviceContext object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDeviceContext* CreateDeviceContext(CXTPChartContainer* pContainer, HDC hDC, CRect rcBounds, BOOL bWindowDC);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the background color of the chart content.
	// Returns:
	//     A CXTPChartColor object which describes an ARGB value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetBackgroundColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the background color of the chart content.
	// Parameters:
	//     clr - Color of the background
	// Remarks:
	//-----------------------------------------------------------------------
	void SetBackgroundColor(const CXTPChartColor& clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the appearance background color of the chart content.
	// Returns:
	//     A CXTPChartColor object which describes an ARGB value.
	// Remarks:
	//     Call SetBackgroundColor to set custom background color. If custom background not set - color from current appearance will be used.
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualBackgroundColor() const;
	CXTPChartColor GetActualBorderColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function whenever a change in the content of the chart.
	//     This call causes a redraw.
	// Parameters:
	//     updateOptions - Options that indicated what kind of changes done.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void OnChartChanged(XTPChartUpdateOptions updateOptions = xtpChartUpdateView);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of content
	//      using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the view of the chart content.
	// Parameters:
	//     pDC      - Pointer to the chart device context object.
	//     rcBounds - The bounding rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartContentView* CreateView(CXTPChartDeviceContext* pDC, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to update the color of the points from the palette.
	// Remarks:
	//-----------------------------------------------------------------------
	void UpdateDiagram();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to update the content whenever there is a change
	//     in the series style.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual void OnSeriesStyleChanged(CXTPChartSeries* pSeries);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get distance between 2 panels
	// Returns:
	//     Distance between 2 panels.
	//
	// Remarks:
	//     10 pixels is used by default
	//-----------------------------------------------------------------------
	int GetPanelDistance() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set distance between 2 panels
	// Parameters:
	//     nDistance - New distance to b set
	// Remarks:
	//-----------------------------------------------------------------------
	void SetPanelDistance(int nDistance);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get layout direction of panels - top-to-bottom or left-to-right.
	// Returns:
	//     XTPChartPanelDirection value indicates direction of panels (xtpChartPanelVertical/xtpChartPanelHorizontal)   //
	//-----------------------------------------------------------------------
	XTPChartPanelDirection GetPanelDirection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set layout direction of panels - top-to-bottom or left-to-right.
	// Parameters:
	//     nDirection - XTPChartPanelDirection value indicates direction of panels (xtpChartPanelVertical/xtpChartPanelHorizontal)  //
	//-----------------------------------------------------------------------
	void SetPanelDirection(XTPChartPanelDirection nDirection);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to enable markup for tab captions
	// Parameters:
	//     bEnable - TRUE to enable markup
	//-------------------------------------------------------------------------
	void EnableMarkup(BOOL bEnable = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Returns markup context
	//-------------------------------------------------------------------------
	CXTPMarkupContext* GetMarkupContext() const;

protected:
	void AddContainer(CXTPChartContainer* pContainer);
	void RemoveContainer(CXTPChartContainer* pContainer);



protected:
	CXTPChartPanelCollection* m_pPanels;        //The panel collection.
	CXTPChartTitleCollection* m_pTitles;        //The title collection.
	CXTPChartSeriesCollection* m_pSeries;       //The series collection.
	CXTPChartLegend* m_pLegend;                 //The legend object.

	CXTPChartAppearance* m_pAppearance;         //The chart appearance object.

	CXTPChartBorder* m_pBorder;                 //The chart border.

	CXTPChartColor m_clrBackground;             //The chart background color.

	CArray<CXTPChartContainer*, CXTPChartContainer*> m_arrContainers;   //The chart containers

	CXTPMarkupContext* m_pMarkupContext;        // Markup context

	int m_nPanelDistance;                       // Panel distance
	XTPChartPanelDirection m_nPanelDirection;   // Panel direction.

	friend class CXTPChartSeriesCollection;
	friend class CXTPChartSeries;
	friend class CXTPChartControl;
};


AFX_INLINE CXTPChartTitleCollection* CXTPChartContent::GetTitles() const {
	return m_pTitles;
}

AFX_INLINE CXTPChartSeriesCollection* CXTPChartContent::GetSeries() const {
	return m_pSeries;
}
AFX_INLINE CXTPChartLegend* CXTPChartContent::GetLegend() const {
	return m_pLegend;
}
AFX_INLINE CXTPChartPanelCollection* CXTPChartContent::GetPanels() const {
	return m_pPanels;
}
AFX_INLINE CXTPChartAppearance* CXTPChartContent::GetAppearance() const {
	return m_pAppearance;
}
AFX_INLINE CXTPMarkupContext* CXTPChartContent::GetMarkupContext() const {
	return m_pMarkupContext;
}
AFX_INLINE int CXTPChartContent::GetPanelDistance() const {
	return m_nPanelDistance;
}
AFX_INLINE void CXTPChartContent::SetPanelDistance(int nDistance) {
	m_nPanelDistance = nDistance;
	OnChartChanged();
}
AFX_INLINE XTPChartPanelDirection CXTPChartContent::GetPanelDirection() const {
	return m_nPanelDirection;
}
AFX_INLINE void CXTPChartContent::SetPanelDirection(XTPChartPanelDirection nDirection) {
	m_nPanelDirection = nDirection;
	OnChartChanged();
}
AFX_INLINE void CXTPChartContent::SetBackgroundColor(const CXTPChartColor& clr) {
	m_clrBackground = clr;
	OnChartChanged();
}
AFX_INLINE CXTPChartBorder* CXTPChartContent::GetBorder() const {
	return m_pBorder;
}

#endif //#if !defined(__XTPCHARTCONTENT_H__)
