// XTPChartTrendline.h
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
#if !defined(__XTPCHARTTRENDLINE_H__)
#define __XTPCHARTTRENDLINE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


enum XTPChartSeriesTrendlineType
{
	xtpChartSeriesTrendlineTypeNone,
	xtpChartSeriesTrendlineTypeLinear,
	xtpChartSeriesTrendlineTypeMovingAverageSimple,
// xtpChartSeriesTrendlineTypeMovingAverageCumulative,
// xtpChartSeriesTrendlineTypeMovingAverageWeighted,
// xtpChartSeriesTrendlineTypeMovingAverageExponential,
	xtpChartSeriesTrendlineTypeMinimum,
	xtpChartSeriesTrendlineTypeMaximum,
};

class _XTP_EXT_CLASS CXTPChartSeriesTrendline : public CXTPChartSeries
{
	DECLARE_DYNAMIC(CXTPChartSeriesTrendline)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSeriesTrendline object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesTrendline();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartSeriesTrendline object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSeriesTrendline();

public:

	CXTPChartSeries* GetSource() const;

	void SetSource(CXTPChartSeries *pSeries);

	XTPChartSeriesTrendlineType GetType() const;

	void SetType(XTPChartSeriesTrendlineType type);

	int GetPeriod() const;

	void SetPeriod(int nPeriod);

protected:

	void Release();

protected:

	XTPChartSeriesTrendlineType m_type;
	int m_nPeriod;

	CXTPChartSeries *m_pSource;

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartSeriesTrendline);
	DECLARE_OLECREATE_EX(CXTPChartSeriesTrendline)

	afx_msg LPDISPATCH OleGetSeries();

	afx_msg LPDISPATCH OleGetSource();
	afx_msg void OleSetSource(LPDISPATCH lpSeries);

//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE CXTPChartSeries* CXTPChartSeriesTrendline::GetSource() const
{
	return m_pSource;
}

AFX_INLINE XTPChartSeriesTrendlineType CXTPChartSeriesTrendline::GetType() const
{
	return m_type;
}

AFX_INLINE int CXTPChartSeriesTrendline::GetPeriod() const
{
	return m_nPeriod;
}

AFX_INLINE void CXTPChartSeriesTrendline::SetPeriod(int nPeriod)
{
	m_nPeriod = nPeriod;
}

class _XTP_EXT_CLASS CXTPChartSeriesTrendlinePointCollection : public CXTPChartSeriesVirtualPointCollection
{
	DECLARE_DYNAMIC(CXTPChartSeriesTrendlinePointCollection)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSeriesTrendlinePointCollection object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesTrendlinePointCollection();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartSeriesTrendlinePointCollection object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartSeriesTrendlinePointCollection();

public:
	virtual CXTPChartSeriesPoint* GetAt(int nIndex) const;

	virtual int GetCount() const;

protected:

	CXTPChartSeriesTrendline *m_pTrendline;

	friend class CXTPChartSeriesTrendline;
};


#endif //#if !defined(__XTPCHARTTRENDLINE_H__)
