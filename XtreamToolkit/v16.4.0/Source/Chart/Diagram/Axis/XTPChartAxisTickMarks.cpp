// XTPChartAxisTickMarks.cpp
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

#include "stdafx.h"
#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>

#include "XTPChartAxisTickMarks.h"
#include "XTPChartAxis.h"



CXTPChartAxisTickMarks::CXTPChartAxisTickMarks(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;

	m_bVisible = TRUE;
	m_bMinorVisible = TRUE;
	m_nThickness = 1;
	m_nMinorThickness = 1;
	m_nLength = 5;
	m_nMinorLength = 2;

	m_bCrossAxis = FALSE;


#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisTickMarks::~CXTPChartAxisTickMarks()
{

}
void CXTPChartAxisTickMarks::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("MinorVisible"), m_bMinorVisible, TRUE);
	PX_Int(pPX, _T("Thickness"), m_nThickness, 1);
	PX_Int(pPX, _T("MinorThickness"), m_nMinorThickness, 1);
	PX_Int(pPX, _T("Length"), m_nLength, 5);
	PX_Int(pPX, _T("MinorLength"), m_nMinorLength, 2);
	PX_Bool(pPX, _T("CrossAxis"), m_bCrossAxis, FALSE);
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisTickMarks, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "Visible", 1, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "MinorVisible", 2, IsMinorVisible, SetMinorVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "Thickness", 3, GetThickness, SetThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "MinorThickness", 4, GetMinorThickness, SetMinorThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "Length", 5, GetLength, SetLength, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "MinorLength", 6, GetMinorLength, SetMinorLength, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxisTickMarks, "CrossAxis", 7, IsCrossAxis, SetCrossAxis, VT_BOOL)

END_DISPATCH_MAP()

// {561BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAxisTickMarks =
{ 0x561bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAxisTickMarks, CXTPChartElement)
INTERFACE_PART(CXTPChartAxisTickMarks, IID_IChartAxisTickMarks, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisTickMarks, IID_IChartAxisTickMarks)


#endif
