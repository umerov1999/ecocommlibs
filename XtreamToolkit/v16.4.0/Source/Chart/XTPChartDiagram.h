// XTPChartDiagram.h
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
#if !defined(__XTPCHARTDIAGRAM_H__)
#define __XTPCHARTDIAGRAM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartSeriesView;
class CXTPChartElementView;
class CXTPChartDiagramView;
class CXTPChartSeries;
class CXTPChartPanel;

typedef CArray<CXTPChartSeries*, CXTPChartSeries*> CXTPChartSeriesArray;



//===========================================================================
// Summary:
//     This class represents a chart diagram, which is a kind of CXTPChartElement.
//     This class act as a base class for 2D and 3D diagrams.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDiagram : public CXTPChartPanel
{
	DECLARE_DYNAMIC(CXTPChartDiagram)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDiagram object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDiagram();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the diagram.
	// Parameters:
	//     pDC      - The chart device context.
	//     pParent  - A pointer to the parent view object.
	// Returns:
	//     A pointer to CXTPChartDiagramView object.
	// Remarks:
	//     This is a virtual function, so the sub classes can give their type
	//     specific implementation for this function.
	//-----------------------------------------------------------------------
	virtual CXTPChartDiagramView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate series layout of the diagram.
	// Parameters:
	//     pDC      - The chart device context.
	//     pView  - A pointer to the diagram view object.
	// Remarks:
	//     This is a virtual function, so the sub classes can give their type
	//     specific implementation for this function.
	//-----------------------------------------------------------------------
	virtual void CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the series collection object.
	// Returns:
	//     A reference to CXTPChartSeriesArray object.
	// Remarks:
	//-----------------------------------------------------------------------
	const CXTPChartSeriesArray& GetSeries() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function called when a series is added to the series collection
	//     in the chart content object which has a collection of series.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual BOOL OnSeriesAdded(CXTPChartSeries* pSeries);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function called when a series is removed from the series collection
	//     in the chart content object which has a collection of series.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	// Remarks:
	//-----------------------------------------------------------------------
	void OnSeriesRemoved(CXTPChartSeries* pSeries);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to decrease the usage count of the object.
	//-------------------------------------------------------------------------
	void Release();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:

	friend class CXTPChartContent;
	friend class CXTPChartSeries;


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartDiagram);
	LPDISPATCH OleGetTitles();
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartSeriesArray m_arrSeries;        //The series collection.
};

class _XTP_EXT_CLASS CXTPChartDiagramDomain
{
public:
	virtual CXTPChartRectF GetInnerBounds() const = 0;
};

AFX_INLINE const CXTPChartSeriesArray& CXTPChartDiagram::GetSeries() const
{
	return m_arrSeries;
}

#endif //#if !defined(__XTPCHARTDIAGRAM_H__)
