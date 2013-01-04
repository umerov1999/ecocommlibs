// XTPChartPieDiagramView.h
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
#if !defined(__XTPCHARTPIEDIAGRAMVIEW_H__)
#define __XTPCHARTPIEDIAGRAMVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartPie;
class CXTPChartDiagramDomain;

//===========================================================================
// Summary:
//     This class abstracts the view of a pie diagram.This class is a kind of
//     CXTPChartDiagramView.
// Remarks:
//     A pie chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pie chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPieDiagramView : public CXTPChartDiagramView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPieDiagramView object.
	// Parameters:
	//     pDiagram - A pointer to pie diagram object.
	//     pParent  - A pointer to the parent view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPieDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent);

protected:
};


#endif //#if !defined(__XTPCHARTPIEDIAGRAMVIEW_H__)
