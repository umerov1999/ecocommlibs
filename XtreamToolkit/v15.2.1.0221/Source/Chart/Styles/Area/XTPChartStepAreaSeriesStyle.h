// XTPChartStepAreaSeriesStyle.h
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
#if !defined(__XTPCHARTStepAreaSERIESSTYLE_H__)
#define __XTPCHARTStepAreaSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartStepAreaStyle;


//===========================================================================
// Summary:
//     This class represents a StepArea series style,which is a kind of
//     CXTPChartAreaSeriesStyle.
// Remarks:
//     A Step Line chart or StepArea graph is a type of graph, which displays information
//     as a series of data points connected by straight Step Line segments.It is a
//     basic type of chart common in many fields.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStepAreaSeriesStyle : public CXTPChartAreaSeriesStyle
{
	DECLARE_SERIAL(CXTPChartStepAreaSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStepAreaSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartStepAreaSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartStepAreaSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartStepAreaSeriesStyle();

public:
	BOOL GetInvertedStep() const;
	void SetInvertedStep(BOOL bInvertedStep);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the StepArea series.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     A pointer to CXTPChartStepAreaSeriesView object which is a kind of
	//     CXTPChartSeriesView.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

	void DoPropExchange(CXTPPropExchange* pPX);
#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartStepAreaSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartStepAreaSeriesStyle)
	LPDISPATCH OleGetStepAreaStyle();
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	BOOL m_bInvertedStep;

};

//===========================================================================
// Summary:
//     This class represents the view of a StepArea series ,which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStepAreaSeriesView : public CXTPChartAreaSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStepAreaSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartStepAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a StepArea series.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the StepArea series.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

};

AFX_INLINE BOOL CXTPChartStepAreaSeriesStyle::GetInvertedStep() const {
	return m_bInvertedStep;
}
AFX_INLINE void CXTPChartStepAreaSeriesStyle::SetInvertedStep(BOOL bInvertedStep) {
	m_bInvertedStep = bInvertedStep;
	OnChartChanged();
}

#endif //#if !defined(__XTPCHARTStepAreaSERIESSTYLE_H__)
