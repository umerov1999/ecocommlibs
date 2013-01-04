// XTPChartTitleView.h
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
#if !defined(__XTPCHARTTITLEVIEW_H__)
#define __XTPCHARTTITLEVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartTitle;
class CXTPChartElementView;
class CXTPChartDeviceCommand;
class CXTPChartDeviceContext;
class CXTPChartTitle;

/////////////////////////////////////////////////////////////////////////
// CXTPChartTitleView

class CXTPChartTitleView : public CXTPChartElementView
{
public:
	CXTPChartTitleView(CXTPChartTitle* pTitle, CXTPChartString& strText, CXTPChartElementView* pParentView);

	~CXTPChartTitleView();

public:
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	void CalculateView(CXTPChartDeviceContext* pDC, CRect& rcBounds, CRect rcChart);

	CXTPChartString m_strText;
	CXTPChartTitle* m_pTitle;
	CPoint m_ptOrigin;

};

#endif //#if !defined(__XTPCHARTTITLEVIEW_H__)
