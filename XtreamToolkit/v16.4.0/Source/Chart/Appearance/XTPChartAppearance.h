// XTPChartAppearance.h
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
#if !defined(__XTPCHARTAPPEARANCE_H__)
#define __XTPCHARTAPPEARANCE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartPalette;
class CXTPChartElement;
class CXTPChartFillStyle;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     This class represents the appearance of a chart element.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartElementAppearance : public CXTPChartElement
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartElementAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartElementAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartElementAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartElementAppearance();
};

//===========================================================================
// Summary:
//     This class represents the appearance of a chart title.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTitleAppearance : public CXTPChartElementAppearance
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTitleAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartTitleAppearance(CXTPChartElement* pOwner);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//      appearance of a chart title using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	CXTPChartColor TextColor;        //The text color of chart title.
};


//===========================================================================
// Summary:
//     This class represents the appearance of a chart legend.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartLegendAppearance : public CXTPChartElementAppearance
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartLegendAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartLegendAppearance(CXTPChartElement* pOwner);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a chart legend using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	CXTPChartColor TextColor;                //The legend text color.
	CXTPChartColor BackgroundColor;          //The legend background color.
	CXTPChartColor BorderColor;              //The legend border color.
};

//===========================================================================
// Summary:
//     This class represents the appearance of a chart content.This class is
//     is subclassed from CXTPChartElementAppearance class.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartContentAppearance : public CXTPChartElementAppearance
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartContentAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartContentAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartContentAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartContentAppearance();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a chart content using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the title appearance object pointer.
	// Returns:
	//     Returns the pointer to CXTPChartTitleAppearance object.
	//-------------------------------------------------------------------------
	CXTPChartTitleAppearance* GetTitleAppearance() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the legend appearance object pointer.
	// Returns:
	//     Returns the pointer to CXTPChartLegendAppearance object.
	//-------------------------------------------------------------------------
	CXTPChartLegendAppearance* GetLegendAppearance() const;

public:
	CXTPChartColor BackgroundColor;                      //The background color of the chart.
	CXTPChartColor BorderColor;                          //The border color of the chart.

protected:

	CXTPChartTitleAppearance* m_pTitleAppearance;        //Pointer to the chart title appearance object.
	CXTPChartLegendAppearance* m_pLegendAppearance;      //Pinter to the legend appearance object.
};

//===========================================================================
// Summary:
//     This class represents the appearance of a chart axis.This class is
//     is subclassed from CXTPChartElementAppearance class.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAxisAppearance : public CXTPChartElementAppearance
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAxisAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartAxisAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAxisAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAxisAppearance();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a chart axis using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:

	CXTPChartColor Color;                        //The color of the axis.
	CXTPChartColor InterlacedColor;              //The primary interlaced color.
	CXTPChartColor InterlacedColor2;             //The secondary interlaced color.
	CXTPChartFillStyle* InterlacedFillStyle;     //The fill style used for interlacing.

	CXTPChartColor GridLinesColor;               //The color of the grid lines.
	CXTPChartColor GridLinesMinorColor;          //The minor color of the grid lines.
	CXTPChartColor AxisLabelTextColor;           //The text color of the axis label.
	CXTPChartColor AxisTitleTextColor;           //The text color of the axis title.

	CXTPChartColor ConstantLineColor;            //The color of the contant line parallal to the axis.
	CXTPChartColor ConstantLineTextColor;        //The text color of the contant color.

	CXTPChartColor StripColor;                   //The strip color.
	CXTPChartColor StripColor2;                  //The strip gradient color.
};

//===========================================================================
// Summary:
//     This class represents the appearance of diagram in 2D.This class
//     is subclassed from CXTPChartElementAppearance class.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDiagram2DAppearance : public CXTPChartElementAppearance
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDiagram2DAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartDiagram2DAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartDiagram2DAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartDiagram2DAppearance();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a diagram in 2D, using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	CXTPChartAxisAppearance* GetAxisAppearance() const;

public:
	CXTPChartColor BackgroundColor;              //The first background color of the diagram.
	CXTPChartColor BackgroundColor2;             //The second background color of the diagram.
	CXTPChartColor BorderColor;                  //The border color of the diagram.
	CXTPChartFillStyle* BackgroundFillStyle;     //The background fill style of the diagram.

protected:
	CXTPChartAxisAppearance* m_pAxisAppearance;  //Pointer to the chart axis appearance object.
};


class _XTP_EXT_CLASS CXTPChartSeriesLabelAppearance : public CXTPChartElementAppearance
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFinanceStyleAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartSeriesLabelAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFinanceStyleAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSeriesLabelAppearance();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a diagram in 2D, using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	CXTPChartColor BackgroundColor;
	CXTPChartColor TextColor;
	CXTPChartColor BorderColor;
	CXTPChartColor ConnectorColor;
};


class _XTP_EXT_CLASS CXTPChartSeriesStyleAppearance : public CXTPChartElementAppearance
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSeriesStyleAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartSeriesStyleAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartSeriesStyleAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSeriesStyleAppearance();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a diagram in 2D, using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	CXTPChartSeriesLabelAppearance* GetLabelAppearance() const;


protected:
	CXTPChartSeriesLabelAppearance* m_pLabelAppearance;
};

//===========================================================================
// Summary:
//     This class represents the finance series appearance of the chart. Chart appearance determined by
//     the 2D digram appearance, chart content appearance and the color palette used.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFinanceStyleAppearance : public CXTPChartSeriesStyleAppearance
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFinanceStyleAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartFinanceStyleAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFinanceStyleAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartFinanceStyleAppearance();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a diagram in 2D, using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	CXTPChartColor UpColor;    // Up day line color.
	CXTPChartColor DownColor;  // Down day line Color.
};

//===========================================================================
// Summary:
//     This class represents the pie series appearance of the chart. Chart appearance determined by
//     the 2D digram appearance, chart content appearance and the color palette used.
//===========================================================================
class CXTPChartPieStyleAppearance : public CXTPChartSeriesStyleAppearance
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPieStyleAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartPieStyleAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPieStyleAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPieStyleAppearance();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a diagram in 2D, using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	CXTPChartColor BorderColor;    // Border color.

};

//===========================================================================
// Summary:
//     This class represents the appearance of the chart. Chart appearance determined by
//     the 2D digram appearance, chart content appearance and the color palette used.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAppearance : public CXTPChartElementAppearance
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAppearance object.
	// Parameters:
	//     pOwner     - Pointer to an owner element.
	//-----------------------------------------------------------------------
	CXTPChartAppearance(CXTPChartElement* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAppearance object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAppearance();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the 2D digram appearance object pointer of the chart.
	// Returns:
	//     Returns the pointer to CXTPChartDiagram2DAppearance object.
	//-------------------------------------------------------------------------
	CXTPChartDiagram2DAppearance* GetDiagram2DAppearance() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the content appearance object pointer of the chart.
	// Returns:
	//     Returns the pointer to CXTPChartContentAppearance object.
	//-------------------------------------------------------------------------
	CXTPChartContentAppearance* GetContentAppearance() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the finance series appearance object pointer of the chart.
	// Returns:
	//     Returns the pointer to CXTPChartFinanceSeriesAppearance object.
	//-------------------------------------------------------------------------
	CXTPChartFinanceStyleAppearance* GetFinanceStyleAppearance() const;

	CXTPChartSeriesStyleAppearance* GetSeriesStyleAppearance() const;

	CXTPChartPieStyleAppearance* GetPieStyleAppearance() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the color palette object pointer of the chart.
	// Returns:
	//     Returns the pointer to CXTPChartPalette object.
	//-------------------------------------------------------------------------
	CXTPChartPalette* GetPalette() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     appearance of a chart, here this function calls the DoPropExchange method
	//     of the constituent appearance elements.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the pointer to the chart appeance object.
	// Returns:
	//     Returns the pointer to CXTPChartAppearance object.
	//-------------------------------------------------------------------------
	static CXTPChartAppearance* AFX_CDECL GetAppearance(const CXTPChartElement* pElement);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to load the properties from a resource XML.
	// Returns:
	//     BOOL value TRUE if the properties are loaded successfully, BOOL
	//     value FALSE if the function fails.
	//-------------------------------------------------------------------------
	BOOL LoadAppearance(LPCTSTR lpszAppearance);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to load the chart palette values from a resource XML.
	// Returns:
	//     BOOL value TRUE if the properties are loaded successfully, BOOL
	//     value FALSE if the function fails.
	//-------------------------------------------------------------------------
	BOOL LoadPalette(LPCTSTR lpszPallete);

public:
	static CXTPChartColor AFX_CDECL GetLightColor(const CXTPChartColor& clr);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartAppearance);
	afx_msg void OleSetAppearance(LPCTSTR lpszName);
	afx_msg void OleSetPalette(LPCTSTR lpszName);
	afx_msg void OleLoadAppearance(LPDISPATCH lpPropExchage);
	afx_msg void OleLoadPalette(LPDISPATCH lpPropExchage);
	afx_msg LPDISPATCH OleGetPalette();
	//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartDiagram2DAppearance* m_pDiagram2DAppearance;         // Pointer to chart diagram 2d appearance object.
	CXTPChartContentAppearance* m_pContentAppearance;             // Pointer to chart content appearance object.
	CXTPChartPalette* m_pPalette;                                 // Pointer to chart palette object.

	CXTPChartSeriesStyleAppearance* m_pSeriesStyleAppearance;
	CXTPChartFinanceStyleAppearance* m_pFinanceStyleAppearance;   // Pointer to chart financial content appearance object.
	CXTPChartPieStyleAppearance* m_pPieStyleAppearance;           // Pointer to chart Pie content appearance object.
};


AFX_INLINE CXTPChartDiagram2DAppearance* CXTPChartAppearance::GetDiagram2DAppearance() const {
	return m_pDiagram2DAppearance;
}
AFX_INLINE CXTPChartContentAppearance* CXTPChartAppearance::GetContentAppearance() const {
	return m_pContentAppearance;
}

AFX_INLINE CXTPChartAxisAppearance* CXTPChartDiagram2DAppearance::GetAxisAppearance() const {
	return m_pAxisAppearance;
}
AFX_INLINE CXTPChartPalette* CXTPChartAppearance::GetPalette() const {
	return m_pPalette;
}
AFX_INLINE CXTPChartTitleAppearance* CXTPChartContentAppearance::GetTitleAppearance() const {
	return m_pTitleAppearance;
}
AFX_INLINE CXTPChartLegendAppearance* CXTPChartContentAppearance::GetLegendAppearance() const {
	return m_pLegendAppearance;
}
AFX_INLINE CXTPChartFinanceStyleAppearance* CXTPChartAppearance::GetFinanceStyleAppearance() const {
	return m_pFinanceStyleAppearance;
}
AFX_INLINE CXTPChartSeriesStyleAppearance* CXTPChartAppearance::GetSeriesStyleAppearance() const {
	return m_pSeriesStyleAppearance;
}
AFX_INLINE CXTPChartPieStyleAppearance* CXTPChartAppearance::GetPieStyleAppearance() const {
	return m_pPieStyleAppearance;
}
AFX_INLINE CXTPChartSeriesLabelAppearance* CXTPChartSeriesStyleAppearance::GetLabelAppearance() const {
	return m_pLabelAppearance;
}


#endif //#if !defined(__XTPCHARTAPPEARANCE_H__)
