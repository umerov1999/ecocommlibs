// XTPChartLegendItem.cpp
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

#include "StdAfx.h"

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>

CXTPChartLegendItem::CXTPChartLegendItem()
	: m_nLineThickness(UseDefaultLineThickness)
{
}

void CXTPChartLegendItem::SetLineThickness(int nLineThickness)
{
	m_nLineThickness = nLineThickness;
}

int CXTPChartLegendItem::GetActualLineThickness() const
{
	int nActualLineThickness = 1;

	if (UseDefaultLineThickness != m_nLineThickness)
	{
		nActualLineThickness = m_nLineThickness;
	}

	return nActualLineThickness;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartLegendItem, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartLegendItem, "LineThickness", 1, GetActualLineThickness, SetLineThickness, VT_I4)
END_DISPATCH_MAP()


// {9C4EAAC5-AC91-472b-9706-E88469167FAD}
static const GUID IID_IChartLegendItem =
{ 0x9c4eaac5, 0xac91, 0x472b, { 0x97, 0x06, 0xe8, 0x84, 0x69, 0x16, 0x7f, 0xad } };

BEGIN_INTERFACE_MAP(CXTPChartLegendItem, CXTPChartElement)
	INTERFACE_PART(CXTPChartLegendItem, IID_IChartLegendItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartLegendItem, IID_IChartLegendItem)

#endif
