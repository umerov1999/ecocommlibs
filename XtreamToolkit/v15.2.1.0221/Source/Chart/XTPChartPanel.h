// XTPChartPanel.h
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
#if !defined(__XTPCHARTPANEL_H__)
#define __XTPCHARTPANEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartDiagram;
class CXTPChartPanelCollection;
class CXTPChartTitleCollection;

//===========================================================================
// Summary:
//     This class abstracts a chart panel, used in panel charts.
// Remarks:
//     Panel Charts are charts that have multiple regions which compare similar
//     data sets side by side (in separate panels) rather than right on top of
//     each other. Essentially a single chart is repeated across a grid, with
//     different data sets in each instance of the chart.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPanel : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartPanel);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPanel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPanel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartMarker object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPanel();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the title collection object.
	// Returns:
	//     A pointer to CXTPChartTitleCollection object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTitleCollection* GetTitles() const;

public:
	void Remove();

	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:

protected:
	CXTPChartTitleCollection* m_pTitles;

protected:
	friend class CXTPChartPanelCollection;
	friend class CXTPChartContent;
};

//===========================================================================
// Summary:
//     This class abstracts a chart panel collection, used in panel charts.
// Remarks:
//     Panel Charts are charts that have multiple regions which compare similar
//     data sets side by side (in separate panels) rather than right on top of
//     each other. Essentially a single chart is repeated across a grid, with
//     different data sets in each instance of the chart.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPanelCollection : public CXTPChartElementCollection
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPanelCollection object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPanelCollection(CXTPChartContent* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPanelCollection object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPanelCollection();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to add a  panel to the collection.
	// Parameters:
	//     pPanel - A pointer to the panel object which is to be added.
	//-------------------------------------------------------------------------
	CXTPChartPanel* Add(CXTPChartPanel* pPanel);


	// Summary:
	//     Call this function to get the panel at a particular index in  the
	//     collection.
	// Parameters:
	//     nIndex - Zero based index of the panel to be fetched.
	// Returns:
	//     A pointer to the chart panel object.
	//-------------------------------------------------------------------------
	CXTPChartPanel* GetAt(int nIndex) const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:


protected:
	friend class CXTPChartContent;
};

AFX_INLINE CXTPChartPanel* CXTPChartPanelCollection::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrElements.GetSize() ? (CXTPChartPanel*)m_arrElements.GetAt(nIndex) : NULL;
}
AFX_INLINE CXTPChartTitleCollection* CXTPChartPanel::GetTitles() const {
	return m_pTitles;
}

#endif //#if !defined(__XTPCHARTPANEL_H__)
