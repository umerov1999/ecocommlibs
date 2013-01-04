// XTPChartAxis.cpp
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
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../Appearance/XTPChartAppearance.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../Diagram2D/XTPChartDiagram2D.h"
#include "XTPChartAxis.h"
#include "XTPChartAxisLabel.h"
#include "XTPChartAxisTitle.h"
#include "XTPChartAxisTickMarks.h"
#include "XTPChartAxisGridLines.h"
#include "XTPChartAxisRange.h"
#include "XTPChartAxisConstantLines.h"
#include "XTPChartAxisCustomLabels.h"
#include "XTPChartAxisStrips.h"
#include "XTPChartScaleTypeMap.h"
#include "XTPChartAxisView.h"

IMPLEMENT_DYNAMIC(CXTPChartAxis, CXTPChartElement)

CXTPChartAxis::CXTPChartAxis(CXTPChartDiagram* pDiagram)
{
	m_pDiagram = pDiagram;
	m_pOwner = pDiagram;

	m_bVisible = TRUE;
	m_nAlignment = xtpChartAxisNear;

	m_bReversed = FALSE;

	m_nThickness = 1;
	m_nMinorCount = 1;

	m_dGridSpacing = 1;
	m_bGridSpacingAuto = TRUE;

	m_bAllowZoom = TRUE;

	m_bInterlaced = FALSE;

	m_bSecondary = FALSE;

	m_nScaleType = xtpChartScaleNumerical;

	m_bLogarithmic = FALSE;
	m_dLogarithmicBase = 10;

	m_pGridLines = new CXTPChartAxisGridLines(this);
	m_pLabel = new CXTPChartAxisLabel(this);
	m_pTitle = new CXTPChartAxisTitle(this);
	m_pTickMarks = new CXTPChartAxisTickMarks(this);

	m_pConstantLines = new CXTPChartAxisConstantLines(this);
	m_pStrips = new CXTPChartAxisStrips(this);
	m_pCustomLabels = new CXTPChartAxisCustomLabels(this);

	m_pRange = new CXTPChartAxisRange(this);

	m_pInterlacedFillStyle = new CXTPChartFillStyle(this);
	m_pInterlacedFillStyle->SetFillMode(xtpChartFillEmpty);

	m_nDateTimeScaleUnit = xtpChartScaleDay;

	m_pScaleTypeMap = CXTPChartScaleTypeMap::Create(this);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAxis::~CXTPChartAxis()
{
	SAFE_RELEASE(m_pGridLines);
	SAFE_RELEASE(m_pLabel);
	SAFE_RELEASE(m_pTitle);
	SAFE_RELEASE(m_pTickMarks);
	SAFE_RELEASE(m_pScaleTypeMap);
	SAFE_RELEASE(m_pRange);
	SAFE_RELEASE(m_pInterlacedFillStyle);
	SAFE_RELEASE(m_pConstantLines);
	SAFE_RELEASE(m_pStrips);
	SAFE_RELEASE(m_pCustomLabels);
}

void CXTPChartAxis::UpdateRange(CXTPChartDeviceContext* pDC, CXTPChartAxisView* pView)
{
	if (pView->m_arrSeries.GetSize() > 0)
	{
		CXTPChartSeries* pSeries = pView->m_arrSeries[0]->GetSeries();

		m_nScaleType = IsValuesAxis() ?  pSeries->GetValueScaleType() : pSeries->GetArgumentScaleType();
	}

	SAFE_RELEASE(m_pScaleTypeMap);
	m_pScaleTypeMap = CXTPChartScaleTypeMap::Create(this);

	if (!IsValuesAxis())
	{
		m_pScaleTypeMap->UpdateSeries(pView);
	}

	for (int i = 0; i < pView->m_arrSeries.GetSize(); i++)
	{
		CXTPChartSeriesView* pSeriesView = pView->m_arrSeries[i];
		pSeriesView->UpdateRange(pDC);
	}

	m_pRange->UpdateRange(pView);
}

CXTPChartAxisView* CXTPChartAxis::CreateView(CXTPChartDeviceContext* /*pDC*/, CXTPChartElementView* /*pParentView*/)
{
	ASSERT (FALSE);
	return NULL;
}


CXTPChartAxisView* CXTPChartAxisX::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDiagram2DAxisView* pView = new CXTPChartDiagram2DAxisView(this, pParentView);

	return pView;
}

CXTPChartAxisView* CXTPChartAxisY::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDiagram2DAxisView* pView = new CXTPChartDiagram2DAxisView(this, pParentView);

	return pView;
}


CXTPChartAxisAppearance* CXTPChartAxis::GetAppearance() const
{
	return CXTPChartElement::GetAppearance()->GetDiagram2DAppearance()->GetAxisAppearance();
}

CXTPChartColor CXTPChartAxis::GetActualColor() const
{
	if (!m_clrColor.IsEmpty())
		return m_clrColor;

	return GetAppearance()->Color;
}

CXTPChartColor CXTPChartAxis::GetColor() const
{
	return m_clrColor;
}

CXTPChartColor CXTPChartAxis::GetActualInterlacedColor() const
{
	if (!m_clrInterlacedColor.IsEmpty())
		return m_clrInterlacedColor;

	return GetAppearance()->InterlacedColor;
}

CXTPChartColor CXTPChartAxis::GetActualInterlacedColor2() const
{
	if (!m_clrInterlacedColor2.IsEmpty())
		return m_clrInterlacedColor2;

	return GetAppearance()->InterlacedColor2;
}

CXTPChartColor CXTPChartAxis::GetInterlacedColor() const
{
	return m_clrInterlacedColor;
}

CXTPChartColor CXTPChartAxis::GetInterlacedColor2() const
{
	return m_clrInterlacedColor2;
}

CXTPChartFillStyle* CXTPChartAxis::GetInterlacedFillStyle() const
{
	if (m_pInterlacedFillStyle->GetFillMode() != xtpChartFillEmpty)
		return m_pInterlacedFillStyle;

	return GetAppearance()->InterlacedFillStyle;
}
//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisX

CXTPChartAxisX::CXTPChartAxisX(CXTPChartDiagram2D* pDiagram)
	: CXTPChartAxis(pDiagram)
{
	m_nMinorCount = 4;

	m_bInterlaced = TRUE;
}


BOOL CXTPChartAxisX::IsVertical() const
{
	CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, GetDiagram());

	if (pDiagram && pDiagram->IsRotated())
		return TRUE;

	return FALSE;
}

BOOL CXTPChartAxisX::IsValuesAxis() const
{
	return FALSE;
}

double CXTPChartAxisX::CalcSideMarginsValue(double nMinValue, double nMaxValue) const
{
	if (m_nScaleType == xtpChartScaleNumerical)
		return (nMaxValue - nMinValue) / 15.0;
	return 0.5;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisY

CXTPChartAxisY::CXTPChartAxisY(CXTPChartDiagram2D* pDiagram)
	: CXTPChartAxis(pDiagram)
{
	m_nMinorCount = 2;

	m_pRange->SetShowZeroLevel(TRUE);
}


BOOL CXTPChartAxisY::IsVertical() const
{
	CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, GetDiagram());

	if (pDiagram && pDiagram->IsRotated())
		return FALSE;

	return TRUE;
}

BOOL CXTPChartAxisY::IsValuesAxis() const
{
	return TRUE;
}
double CXTPChartAxisY::CalcSideMarginsValue(double nMinValue, double nMaxValue) const
{
	if (m_nScaleType != xtpChartScaleNumerical)
		return 0.5;

	double delta;
	if (nMinValue > 0 && nMaxValue > 0)
		delta = nMaxValue;
	else if (nMinValue < 0 && nMaxValue < 0)
		delta = -nMinValue;
	else
		delta = nMaxValue - nMinValue;

	return delta / 10.0;
}

void CXTPChartAxis::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Enum(pPX, _T("Alignment"), m_nAlignment, xtpChartAxisNear);

	PX_Bool(pPX, _T("Interlaced"), m_bInterlaced, FALSE);

	PX_Bool(pPX, _T("Reversed"), m_bReversed, FALSE);
	PX_Bool(pPX, _T("Thickness"), m_nThickness, 1);
	PX_Bool(pPX, _T("MinorCount"), m_nMinorCount, 0);

	PX_Color(pPX, _T("Color"), m_clrColor);
	PX_Color(pPX, _T("InterlacedColor"), m_clrInterlacedColor);
	PX_Color(pPX, _T("InterlacedColor2"), m_clrInterlacedColor2);

	PX_Double(pPX, _T("GridSpacing"), m_dGridSpacing, 0);
	PX_Bool(pPX, _T("GridSpacingAuto"), m_bGridSpacingAuto, TRUE);

	PX_Bool(pPX, _T("AllowZoom"), m_bAllowZoom, TRUE);

	PX_Bool(pPX, _T("Logarithmic"), m_bLogarithmic, FALSE);
	PX_Double(pPX, _T("LogarithmicBase"), m_dLogarithmicBase, 10.0);

	PX_Enum(pPX, _T("DateTimeScaleUnit"), m_nDateTimeScaleUnit, xtpChartScaleDay);


	CXTPPropExchangeSection secGridLines(pPX->GetSection(_T("GridLines")));
	m_pGridLines->DoPropExchange(&secGridLines);

	CXTPPropExchangeSection secInterlacedFillStyle(pPX->GetSection(_T("InterlacedFillStyle")));
	m_pInterlacedFillStyle->DoPropExchange(&secInterlacedFillStyle);

	CXTPPropExchangeSection secLabel(pPX->GetSection(_T("Label")));
	m_pLabel->DoPropExchange(&secLabel);

	CXTPPropExchangeSection secTickMarks(pPX->GetSection(_T("TickMarks")));
	m_pTickMarks->DoPropExchange(&secTickMarks);

	CXTPPropExchangeSection secTitle(pPX->GetSection(_T("Title")));
	m_pTitle->DoPropExchange(&secTitle);

	CXTPPropExchangeSection secRange(pPX->GetSection(_T("Range")));
	m_pRange->DoPropExchange(&secRange);

	CXTPPropExchangeSection secStrips(pPX->GetSection(_T("Strips")));
	m_pStrips->DoPropExchange(&secStrips);

	CXTPPropExchangeSection secConstantLines(pPX->GetSection(_T("ConstantLines")));
	m_pConstantLines->DoPropExchange(&secConstantLines);

	CXTPPropExchangeSection secCustomLabels(pPX->GetSection(_T("CustomLabels")));
	m_pCustomLabels->DoPropExchange(&secCustomLabels);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAxis, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Visible", 1, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Alignment", 2, GetAlignment, SetAlignment, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Interlaced", 3, IsInterlaced, SetInterlaced, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Thickness", 4, GetThickness, SetThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "MinorCount", 5, GetMinorCount, SetMinorCount, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPChartAxis, "ConstantLines", 6, OleGetConstantLines, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Strips", 7, OleGetStrips, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Label", 8, OleGetLabel, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "TickMarks", 9, OleGetTickMarks, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Title", 10, OleGetTitle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "GridLines", 11, OleGetGridLines, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Reversed", 12, IsReversed, SetReversed, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Range", 13, OleGetRange, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartAxis, "InterlacedFillStyle", 14, OleGetInterlacedFillStyle, SetNotSupported, VT_DISPATCH)


	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Color", 15, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "InterlacedColor", 16, OleGetInterlacedColor, OleSetInterlacedColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "InterlacedColor2", 17, OleGetInterlacedColor2, OleSetInterlacedColor2, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "GridSpacing", 18, GetGridSpacing, SetGridSpacing, VT_R8)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "GridSpacingAuto", 19, GetGridSpacingAuto, SetGridSpacingAuto, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartAxis, "AllowZoom", 20, IsAllowZoom, SetAllowZoom, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartAxis, "CustomLabels", 21, OleGetCustomLabels, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartAxis, "Logarithmic", 22, IsLogarithmic, SetLogarithmic, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartAxis, "LogarithmicBase", 23, GetLogarithmicBase, SetLogarithmicBase, VT_R8)

	DISP_PROPERTY_EX_ID(CXTPChartAxis, "DateTimeScaleUnit", 24, GetDateTimeScaleUnit, SetDateTimeScaleUnit, VT_I4)

END_DISPATCH_MAP()

// {EB4BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAxis =
{ 0xeb4bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAxis, CXTPChartElement)
	INTERFACE_PART(CXTPChartAxis, IID_IChartAxis, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAxis, IID_IChartAxis)

LPDISPATCH CXTPChartAxis::OleGetConstantLines()
{
	return XTPGetDispatch(m_pConstantLines);
}

LPDISPATCH CXTPChartAxis::OleGetCustomLabels()
{
	return XTPGetDispatch(m_pCustomLabels);
}


LPDISPATCH CXTPChartAxis::OleGetStrips()
{
	return XTPGetDispatch(m_pStrips);
}

LPDISPATCH CXTPChartAxis::OleGetLabel()
{
	return XTPGetDispatch(m_pLabel);
}


LPDISPATCH CXTPChartAxis::OleGetTickMarks()
{
	return XTPGetDispatch(m_pTickMarks);
}


LPDISPATCH CXTPChartAxis::OleGetTitle()
{
	return XTPGetDispatch(m_pTitle);
}

LPDISPATCH CXTPChartAxis::OleGetGridLines()
{
	return XTPGetDispatch(m_pGridLines);
}

LPDISPATCH CXTPChartAxis::OleGetRange()
{
	return XTPGetDispatch(m_pRange);
}

LPDISPATCH CXTPChartAxis::OleGetInterlacedFillStyle()
{
	return XTPGetDispatch(m_pInterlacedFillStyle);
}


OLE_COLOR CXTPChartAxis::OleGetColor()
{
	return m_clrColor.ToOleColor();
}

void CXTPChartAxis::OleSetColor(OLE_COLOR clr)
{
	SetColor(CXTPChartColor::FromOleColor(clr));
}

OLE_COLOR CXTPChartAxis::OleGetInterlacedColor()
{
	return m_clrInterlacedColor.ToOleColor();
}

void CXTPChartAxis::OleSetInterlacedColor(OLE_COLOR clr)
{
	SetInterlacedColor(CXTPChartColor::FromOleColor(clr));
}

OLE_COLOR CXTPChartAxis::OleGetInterlacedColor2()
{
	return m_clrInterlacedColor2.ToOleColor();
}

void CXTPChartAxis::OleSetInterlacedColor2(OLE_COLOR clr)
{
	SetInterlacedColor2(CXTPChartColor::FromOleColor(clr));
}


#endif
