// XTPChartPointSeriesLabel.h
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
#if !defined(__XTPCHARTPOINTSERIESLABEL_H__)
#define __XTPCHARTPOINTSERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


//===========================================================================
// Summary:
//     This class abstracts the label of a point series, it is a kind of chart
//     series label.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPointSeriesLabel : public CXTPChartDiagram2DSeriesLabel
{
	DECLARE_SERIAL(CXTPChartPointSeriesLabel);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPointSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPointSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPointSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPointSeriesLabel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the angle of the label stem.
	// Parameters:
	//     nAngle - The angle of the label stem.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetAngle(int nAngle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the angle of the label stem.
	// Returns:
	//     An intger value denoting angle of the label stem.
	// Remarks:
	//-----------------------------------------------------------------------
	int GetAngle() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function create the view of the label.
	// Parameters:
	//     pDC -        The device context.
	//     pPointView - A pointer to the series point view object.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

protected:
	int m_nAngle;       //The stem angle.

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartPointSeriesLabel);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE void CXTPChartPointSeriesLabel::SetAngle(int nAngle) {
	m_nAngle = nAngle;
	OnChartChanged();
}
AFX_INLINE int CXTPChartPointSeriesLabel::GetAngle() const {
	return m_nAngle;
}


#endif //#if !defined(__XTPCHARTPOINTSERIESLABEL_H__)
