// XTPChartAxisGridLines.cpp
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

#include "../../XTPChartDefines.h"
#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartElement.h"

#include "XTPChartAxisGridLines.h"
#include "XTPChartAxis.h"
#include "../../Appearance/XTPChartAppearance.h"
#include "../../Appearance/XTPChartLineStyle.h"



CXTPChartAxisGridLines::CXTPChartAxisGridLines(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;
	m_bVisible = TRUE;
	m_bMinorVisible = FALSE;

	m_pLineStyle = new CXTPChartLineStyle(this);
	m_pMinorLineStyle = new CXTPChartLineStyle(this);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxisGridLines::~CXTPChartAxisGridLines()
{
	SAFE_RELEASE(m_pLineStyle);
	SAFE_RELEASE(m_pMinorLineStyle);
}

CXTPChartColor CXTPChartAxisGridLines::GetColor() const
{
	if (!m_clrColor.IsEmpty())
		return m_clrColor;

	return GetAxis()->GetAppearance()->GridLinesColor;
}

CXTPChartColor CXTPChartAxisGridLines::GetMinorColor() const
{
	if (!m_clrMinorColor.IsEmpty())
		return m_clrMinorColor;

	return GetAxis()->GetAppearance()->GridLinesMinorColor;
}


void CXTPChartAxisGridLines::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("MinorVisible"), m_bMinorVisible, FALSE);

	CXTPPropExchangeSection secLineStyle(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&secLineStyle);

	CXTPPropExchangeSection secMinorLineStyle(pPX->GetSection(_T("MinorLineStyle")));
	m_pMinorLineStyle->DoPropExchange(&secMinorLineStyle);

}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxisGridLines, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxisGridLines, "Visible", 1, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxisGridLines, "MinorVisible", 2, IsMinorVisible, SetMinorVisible, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartAxisGridLines, "LineStyle", 3, OleGetLineStyle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAxisGridLines, "MinorLineStyle", 4, OleGetMinorLineStyle, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

// {569BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAxisGridLines =
{ 0x569bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAxisGridLines, CXTPChartElement)
INTERFACE_PART(CXTPChartAxisGridLines, IID_IChartAxisGridLines, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxisGridLines, IID_IChartAxisGridLines)


LPDISPATCH CXTPChartAxisGridLines::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);

}
LPDISPATCH CXTPChartAxisGridLines::OleGetMinorLineStyle()
{
	return XTPGetDispatch(m_pMinorLineStyle);

}


#endif
