// XTPChartPieSeriesStyle.cpp
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

#include <Common/XTPPropExchange.h>

#include <Chart/Types/XTPChartTypes.h>

#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartSeriesPointView.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartSeriesLabel.h>
#include <Chart/XTPChartPanel.h>
#include <Chart/XTPChartDiagram.h>

#include <Chart/Appearance/XTPChartBorder.h>
#include <Chart/Appearance/XTPChartAppearance.h>

#include "XTPChartPieSeriesStyle.h"
#include "XTPChartPieDiagram.h"
#include "XTPChartPieSeriesLabel.h"
#include "XTPChartPieSeriesPointView.h"
#include "XTPChartPieSeriesView.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesStyleBase

IMPLEMENT_DYNAMIC(CXTPChartPieSeriesStyleBase, CXTPChartSeriesStyle)


CXTPChartPieSeriesStyleBase::CXTPChartPieSeriesStyleBase()
{
	m_nHolePercent = 0;
	m_nExplodedDistancePercent= 10;
	m_nRotation = 0;

	m_bColorEach = TRUE;
}

CXTPChartPieSeriesStyleBase::~CXTPChartPieSeriesStyleBase()
{

}

int CXTPChartPieSeriesStyleBase::GetDepth() const
{
	return 0;
}

void CXTPChartPieSeriesStyleBase::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("HolePercent"), m_nHolePercent, 0);
	PX_Int(pPX, _T("ExplodedDistancePercent"), m_nExplodedDistancePercent, 10);
	PX_Int(pPX, _T("Rotation"), m_nRotation, 0);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesStyle

IMPLEMENT_SERIAL(CXTPChartPieSeriesStyle, CXTPChartPieSeriesStyleBase, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPieSeriesStyle::CXTPChartPieSeriesStyle()
{
	m_pBorder = new CXTPChartBorder(this);

	SetLabel(new CXTPChartPieSeriesLabel());
}

CXTPChartPieSeriesStyle::~CXTPChartPieSeriesStyle()
{
	SAFE_RELEASE(m_pBorder);
}

CXTPChartDiagram* CXTPChartPieSeriesStyle::CreateDiagram()
{
	return new CXTPChartPieDiagram();
}

CXTPChartSeriesView* CXTPChartPieSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartPieSeriesView(pSeries, pDiagramView);
}

BOOL CXTPChartPieSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	if (DYNAMIC_DOWNCAST(CXTPChartPieDiagram, pDiagram) == NULL)
		return FALSE;

	return pDiagram->GetSeries().GetSize() == 0;
}

CXTPChartColor CXTPChartPieSeriesStyle::GetBorderActualColor() const
{
	if (!m_pBorder->GetColor().IsEmpty())
		return m_pBorder->GetColor();


	return GetAppearance()->GetPieStyleAppearance()->BorderColor;
}

void CXTPChartPieSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPieSeriesStyleBase::DoPropExchange(pPX);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);
}



#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartPieSeriesStyle, CXTPChartPieSeriesStyleBase)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPieSeriesStyle, "HolePercent", 100, m_nHolePercent, OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPieSeriesStyle, "ExplodedDistancePercent", 101, m_nExplodedDistancePercent, OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPieSeriesStyle, "Rotation", 102, m_nRotation, OleChartChanged, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartPieSeriesStyle, "Border", 103, OleGetBorder, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {233BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPieSeriesStyle =
{ 0x233bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPieSeriesStyle, CXTPChartPieSeriesStyleBase)
	INTERFACE_PART(CXTPChartPieSeriesStyle, IID_IChartPieSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPieSeriesStyle, IID_IChartPieSeriesStyle)

// {14490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartPieSeriesStyle, "Codejock.ChartPieSeriesStyle." _XTP_AXLIB_VERSION,
						0x23490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

LPDISPATCH CXTPChartPieSeriesStyle::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}


#endif
