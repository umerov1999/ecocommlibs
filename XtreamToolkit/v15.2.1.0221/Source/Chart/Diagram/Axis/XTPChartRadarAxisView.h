// XTPChartRadarAxisView.h
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
#if !defined(__XTPCHARTRADARAXISVIEW_H__)
#define __XTPCHARTRADARAXISVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartRadarDiagramView;

//===========================================================================
// Summary:
//     This class abstracts the view of a chart axis and its child items in
//     a bounding rectangle context.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAxisView : public CXTPChartAxisView
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarAxisView object.
	// Parameters:
	//     pAxis     - Pointer to a CXTPChartAxis object.
	//     pParentView - Pointer to a CXTPChartElementView object.
	// Remarks:
	// See Also:
	//-----------------------------------------------------------------------
	CXTPChartRadarAxisView(CXTPChartAxis* pAxis, CXTPChartElementView* pParentView);
	~CXTPChartRadarAxisView();

public:
	virtual CXTPChartDeviceCommand* CreateGridLinesDeviceCommand(CXTPChartDeviceContext* pDC) = 0;
	virtual CXTPChartDeviceCommand* CreateInterlacedDeviceCommand(CXTPChartDeviceContext* pDC) = 0;
	virtual CXTPChartDeviceCommand* CreateConstantLinesDeviceCommand(CXTPChartDeviceContext* pDC, BOOL bBehind) = 0;
	virtual CXTPChartDeviceCommand* CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC) = 0;

public:
	CXTPChartRadarDiagramView* GetDiagramView() const;

public:
	BOOL IsPolygonDiagramStyle() const;


};

AFX_INLINE CXTPChartRadarAxisView::CXTPChartRadarAxisView(CXTPChartAxis* pAxis, CXTPChartElementView* pParentView)
	: CXTPChartAxisView(pAxis, pParentView) {
}
AFX_INLINE CXTPChartRadarAxisView::~CXTPChartRadarAxisView() {
}

AFX_INLINE CXTPChartRadarDiagramView* CXTPChartRadarAxisView::GetDiagramView() const {
	return (CXTPChartRadarDiagramView*)GetParentView()->GetParentView();
}


#endif //#if !defined(__XTPCHARTRADARAXISVIEW_H__)
