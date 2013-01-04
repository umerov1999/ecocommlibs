// XTPChartPyramidDiagram.cpp
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

#include "stdafx.h"

#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"

#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"
#include "../../XTPChartSeriesView.h"

#include "XTPChartPyramidDiagram.h"
#include "XTPChartPyramidSeriesStyle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidDiagram

IMPLEMENT_SERIAL(CXTPChartPyramidDiagram, CXTPChartDiagram, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPyramidDiagram::CXTPChartPyramidDiagram()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPChartDiagramView* CXTPChartPyramidDiagram::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPyramidDiagramView(this, pParent);
}

void CXTPChartPyramidDiagram::CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView)
{
	if (pView->GetCount() == 0)
		return;

	for (int i = 0; i < pView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartPyramidSeriesView* pSeriesView = (CXTPChartPyramidSeriesView*)pView->GetSeriesView()->GetAt(i);

		pSeriesView->CalculatePointLayout(pDC, pView->GetBounds());
		pSeriesView->CalculateLabelLayout(pDC);
	}
}



//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidDiagramView

CXTPChartPyramidDiagramView::CXTPChartPyramidDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent)
	: CXTPChartDiagramView(pDiagram, pParent)
{
	m_pSeriesView = new CXTPChartElementView(this);

	m_pLabelsView = new CXTPChartElementView(this);

}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartPyramidDiagram, CXTPChartDiagram)
END_DISPATCH_MAP()

// {8AABCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPyramidDiagram =
{ 0x8aabcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPyramidDiagram, CXTPChartDiagram)
	INTERFACE_PART(CXTPChartPyramidDiagram, IID_IChartPyramidDiagram, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPyramidDiagram, IID_IChartPyramidDiagram)

// {8AA90E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartPyramidDiagram, "Codejock.ChartPyramidDiagram." _XTP_AXLIB_VERSION,
0x8aa90e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

#endif
