// XTPChartAppearance.cpp
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"

#include "../Types/XTPChartTypes.h"
#include "../XTPChartDefines.h"
#include "../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../XTPChartContent.h"

#include "XTPChartAppearance.h"
#include "XTPChartPalette.h"
#include "XTPChartFillStyle.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartElementAppearance

CXTPChartElementAppearance::CXTPChartElementAppearance(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;
}

CXTPChartElementAppearance::~CXTPChartElementAppearance()
{

}

//////////////////////////////////////////////////////////////////////////
// CXTPChartAxisAppearance

CXTPChartAxisAppearance::CXTPChartAxisAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	InterlacedFillStyle = new CXTPChartFillStyle(this);
}

CXTPChartAxisAppearance::~CXTPChartAxisAppearance()
{
	SAFE_RELEASE(InterlacedFillStyle);
}

void CXTPChartAxisAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("Color"), Color);
	PX_Color(pPX, _T("InterlacedColor"), InterlacedColor);
	PX_Color(pPX, _T("InterlacedColor2"), InterlacedColor2);

	CXTPPropExchangeSection pxInterlacedFillStyle(pPX->GetSection(_T("InterlacedFillStyle")));
	InterlacedFillStyle->DoPropExchange(&pxInterlacedFillStyle);

	CXTPPropExchangeSection pxGridLines(pPX->GetSection(_T("GridLines")));
	PX_Color(&pxGridLines, _T("Color"), GridLinesColor);
	PX_Color(&pxGridLines, _T("MinorColor"), GridLinesMinorColor);

	CXTPPropExchangeSection pxAxisLabel(pPX->GetSection(_T("AxisLabel")));
	PX_Color(&pxAxisLabel, _T("TextColor"), AxisLabelTextColor);

	CXTPPropExchangeSection pxAxisTitle(pPX->GetSection(_T("AxisTitle")));
	PX_Color(&pxAxisTitle, _T("TextColor"), AxisTitleTextColor);

	CXTPPropExchangeSection pxAxisConstantLine(pPX->GetSection(_T("ConstantLine")));
	PX_Color(&pxAxisConstantLine, _T("Color"), ConstantLineColor);
	PX_Color(&pxAxisConstantLine, _T("TextColor"), ConstantLineTextColor);

	CXTPPropExchangeSection pxAxisStrip(pPX->GetSection(_T("Strip")));
	PX_Color(&pxAxisStrip, _T("Color"), StripColor);
	PX_Color(&pxAxisStrip, _T("Color2"), StripColor2);

}

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DAppearance

CXTPChartDiagram2DAppearance::CXTPChartDiagram2DAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	m_pAxisAppearance = new CXTPChartAxisAppearance(this);

	BackgroundFillStyle = new CXTPChartFillStyle(this);
}

CXTPChartDiagram2DAppearance::~CXTPChartDiagram2DAppearance()
{
	SAFE_RELEASE(m_pAxisAppearance);
	SAFE_RELEASE(BackgroundFillStyle);
}

void CXTPChartDiagram2DAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("BackgroundColor"), BackgroundColor);
	PX_Color(pPX, _T("BackgroundColor2"), BackgroundColor2);
	PX_Color(pPX, _T("BorderColor"), BorderColor);

	CXTPPropExchangeSection pxBackgroundFillStyle(pPX->GetSection(_T("BackgroundFillStyle")));
	BackgroundFillStyle->DoPropExchange(&pxBackgroundFillStyle);

	CXTPPropExchangeSection pxAxis(pPX->GetSection(_T("Axis")));
	m_pAxisAppearance->DoPropExchange(&pxAxis);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesLabelAppearance



CXTPChartSeriesLabelAppearance::CXTPChartSeriesLabelAppearance(CXTPChartElement* pOwner)
: CXTPChartElementAppearance(pOwner)
{
}

CXTPChartSeriesLabelAppearance::~CXTPChartSeriesLabelAppearance()
{
}
void CXTPChartSeriesLabelAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("BackgroundColor"), BackgroundColor);
	PX_Color(pPX, _T("TextColor"), TextColor);
	PX_Color(pPX, _T("BorderColor"), BorderColor);
	PX_Color(pPX, _T("ConnectorColor"), ConnectorColor);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeriesStyleAppearance


CXTPChartSeriesStyleAppearance::CXTPChartSeriesStyleAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	m_pLabelAppearance = new CXTPChartSeriesLabelAppearance(this);
}

CXTPChartSeriesStyleAppearance::~CXTPChartSeriesStyleAppearance()
{
	SAFE_RELEASE(m_pLabelAppearance);
}

void CXTPChartSeriesStyleAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection pxTitle(pPX->GetSection(_T("Label")));
	m_pLabelAppearance->DoPropExchange(&pxTitle);
}



//////////////////////////////////////////////////////////////////////////
// CXTPChartFinanceStyleAppearance

CXTPChartFinanceStyleAppearance::CXTPChartFinanceStyleAppearance(CXTPChartElement* pOwner)
	: CXTPChartSeriesStyleAppearance(pOwner)
{
}

CXTPChartFinanceStyleAppearance::~CXTPChartFinanceStyleAppearance()
{
}

void CXTPChartFinanceStyleAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyleAppearance::DoPropExchange(pPX);

	PX_Color(pPX, _T("UpColor"), UpColor);
	PX_Color(pPX, _T("DownColor"), DownColor);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPieStyleAppearance

CXTPChartPieStyleAppearance::CXTPChartPieStyleAppearance(CXTPChartElement* pOwner)
	: CXTPChartSeriesStyleAppearance(pOwner)
{
}

CXTPChartPieStyleAppearance::~CXTPChartPieStyleAppearance()
{
}

void CXTPChartPieStyleAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyleAppearance::DoPropExchange(pPX);

	PX_Color(pPX, _T("BorderColor"), BorderColor);
}



//////////////////////////////////////////////////////////////////////////
// CXTPChartTitleAppearance

CXTPChartTitleAppearance::CXTPChartTitleAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{

}

void CXTPChartTitleAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("TextColor"), TextColor);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartLegendAppearance

CXTPChartLegendAppearance::CXTPChartLegendAppearance(CXTPChartElement* pOwner)
: CXTPChartElementAppearance(pOwner)
{

}

void CXTPChartLegendAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("TextColor"), TextColor);
	PX_Color(pPX, _T("BackgroundColor"), BackgroundColor);
	PX_Color(pPX, _T("BorderColor"), BorderColor);
}




//////////////////////////////////////////////////////////////////////////
// CXTPChartContentAppearance

CXTPChartContentAppearance::CXTPChartContentAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	m_pTitleAppearance = new CXTPChartTitleAppearance(this);
	m_pLegendAppearance = new CXTPChartLegendAppearance(this);
}

CXTPChartContentAppearance::~CXTPChartContentAppearance()
{
	SAFE_RELEASE(m_pLegendAppearance);
	SAFE_RELEASE(m_pTitleAppearance);
}

void CXTPChartContentAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("BackgroundColor"), BackgroundColor);
	PX_Color(pPX, _T("BorderColor"), BorderColor);

	CXTPPropExchangeSection pxTitle(pPX->GetSection(_T("ChartTitle")));
	m_pTitleAppearance->DoPropExchange(&pxTitle);

	CXTPPropExchangeSection pxLegend(pPX->GetSection(_T("Legend")));
	m_pLegendAppearance->DoPropExchange(&pxLegend);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartAppearance

CXTPChartAppearance::CXTPChartAppearance(CXTPChartElement* pOwner)
	: CXTPChartElementAppearance(pOwner)
{
	m_pDiagram2DAppearance = new CXTPChartDiagram2DAppearance(this);
	m_pContentAppearance = new CXTPChartContentAppearance(this);

	m_pSeriesStyleAppearance = new CXTPChartSeriesStyleAppearance(this);
	m_pFinanceStyleAppearance = new CXTPChartFinanceStyleAppearance(this);
	m_pPieStyleAppearance = new CXTPChartPieStyleAppearance(this);

	m_pPalette = new CXTPChartPalette(this);

	// load default colors.
	VERIFY(LoadAppearance(_T("CHART_APPEARANCE_NATURE")));
	VERIFY(LoadPalette(_T("CHART_PALETTE_NATURE")));

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartAppearance::~CXTPChartAppearance()
{
	SAFE_RELEASE(m_pDiagram2DAppearance);
	SAFE_RELEASE(m_pContentAppearance);

	SAFE_RELEASE(m_pSeriesStyleAppearance);
	SAFE_RELEASE(m_pFinanceStyleAppearance);
	SAFE_RELEASE(m_pPieStyleAppearance);

	SAFE_RELEASE(m_pPalette);
}

void CXTPChartAppearance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection pxContent(pPX->GetSection(_T("Content")));
	m_pContentAppearance->DoPropExchange(&pxContent);

	CXTPPropExchangeSection pxDiagram2D(pPX->GetSection(_T("Diagram2D")));
	m_pDiagram2DAppearance->DoPropExchange(&pxDiagram2D);

	CXTPPropExchangeSection pxSeriesStyleAppearance(pPX->GetSection(_T("SeriesStyle")));
	m_pSeriesStyleAppearance->DoPropExchange(&pxSeriesStyleAppearance);

	CXTPPropExchangeSection pxFinanceSeriesAppearance(pPX->GetSection(_T("FinanceSeriesStyle")));
	m_pFinanceStyleAppearance->DoPropExchange(&pxFinanceSeriesAppearance);

	CXTPPropExchangeSection pxPieSeriesAppearance(pPX->GetSection(_T("PieSeriesStyle")));
	m_pPieStyleAppearance->DoPropExchange(&pxPieSeriesAppearance);
}

BOOL CXTPChartAppearance::LoadAppearance(LPCTSTR lpszAppearance)
{
	CXTPPropExchangeXMLNode px(TRUE, NULL, _T("Appearance"));

	HMODULE hInstance = NULL;
	HRSRC hResource = XTPResourceManager()->FindResource(hInstance, lpszAppearance, RT_HTML);

	if (!hResource)
		return FALSE;

	if (!px.LoadFromResource(hInstance, lpszAppearance , RT_HTML))
		return FALSE;

	if (!px.OnBeforeExchange())
		return FALSE;

	px.SetCompactMode(TRUE);

	DoPropExchange(&px);

	OnChartChanged();

	return TRUE;
}

BOOL CXTPChartAppearance::LoadPalette(LPCTSTR lpszPallete)
{
	CXTPPropExchangeXMLNode px(TRUE, NULL, _T("Palette"));

	HMODULE hInstance = NULL;
	HRSRC hResource = XTPResourceManager()->FindResource(hInstance, lpszPallete, RT_HTML);

	if (!hResource)
		return FALSE;

	if (!px.LoadFromResource(hInstance, lpszPallete, RT_HTML))
		return FALSE;

	if (!px.OnBeforeExchange())
		return FALSE;

	px.SetCompactMode(TRUE);

	m_pPalette->DoPropExchange(&px);

	OnChartChanged();

	return TRUE;
}

CXTPChartAppearance* CXTPChartAppearance::GetAppearance(const CXTPChartElement* pElement)
{
	return pElement->GetContent()->GetAppearance();
}


CXTPChartColor CXTPChartAppearance::GetLightColor(const CXTPChartColor& clr)
{
	COLORREF clrBackground = RGB(clr.GetR(), clr.GetG(), clr.GetB());

	DWORD dwHSLBackground = CXTPDrawHelpers::RGBtoHSL(clrBackground);
	DWORD dwL = GetBValue(dwHSLBackground);
	DWORD dwLight = (dwL + 240) / 2;

	CXTPChartColor res;
	res.SetFromCOLORREF(CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwLight)));

	return res;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartAppearance, CXTPChartElementAppearance)
	DISP_FUNCTION_ID(CXTPChartAppearance, "SetAppearance", 1, OleSetAppearance, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartAppearance, "SetPalette", 2, OleSetPalette, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartAppearance, "LoadAppearance", 3, OleLoadAppearance, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPChartAppearance, "LoadPalette", 4, OleLoadPalette, VT_EMPTY, VTS_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAppearance, "Palette", 5, OleGetPalette, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

// {455BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAppearance =
{ 0x455bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAppearance, CXTPChartElementAppearance)
	INTERFACE_PART(CXTPChartAppearance, IID_IChartAppearance, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAppearance, IID_IChartAppearance)

void CXTPChartAppearance::OleSetAppearance(LPCTSTR lpszName)
{
	CString str(lpszName);
	str.MakeUpper();
	str.Replace(_T(" "), _T(""));

	LoadAppearance(_T("CHART_APPEARANCE_") + str);
}

void CXTPChartAppearance::OleSetPalette(LPCTSTR lpszName)
{
	CString str(lpszName);
	str.MakeUpper();
	str.Replace(_T(" "), _T(""));

	LoadPalette(_T("CHART_PALETTE_") + str);
}


void CXTPChartAppearance::OleLoadAppearance(LPDISPATCH lpPropExchage)
{
	CXTPPropExchangeSection px(PropExchangeFromControl(lpPropExchage));

	if ((CXTPPropExchange*)&px == NULL)
		return;

	if (px->OnBeforeExchange())
	{
		if (px->IsKindOf(RUNTIME_CLASS(CXTPPropExchangeXMLNode)))
			((CXTPPropExchangeXMLNode*)&px)->SetCompactMode(TRUE);

		DoPropExchange(&px);

		OnChartChanged();
	}
}

void CXTPChartAppearance::OleLoadPalette(LPDISPATCH lpPropExchage)
{
	CXTPPropExchangeSection px(PropExchangeFromControl(lpPropExchage));

	if ((CXTPPropExchange*)&px == NULL)
		return;

	if (px->OnBeforeExchange())
	{
		if (px->IsKindOf(RUNTIME_CLASS(CXTPPropExchangeXMLNode)))
			((CXTPPropExchangeXMLNode*)&px)->SetCompactMode(TRUE);

		m_pPalette->DoPropExchange(&px);

		OnChartChanged();
	}
}

LPDISPATCH CXTPChartAppearance::OleGetPalette()
{
	return XTPGetDispatch(GetPalette());
}

#endif
