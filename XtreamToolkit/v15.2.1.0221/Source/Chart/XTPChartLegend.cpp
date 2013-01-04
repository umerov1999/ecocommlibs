// XTPChartLegend.cpp
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

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include "XTPChartElementView.h"
#include "XTPChartLegend.h"
#include "XTPChartContent.h"

#include "Drawing/XTPChartDeviceCommand.h"
#include "Appearance/XTPChartBorder.h"
#include "Appearance/XTPChartFillStyle.h"
#include "Appearance/XTPChartAppearance.h"
#include "Utils/XTPChartTextPainter.h"

IMPLEMENT_DYNAMIC(CXTPChartLegend, CXTPChartTextElement)

CXTPChartLegend::CXTPChartLegend(CXTPChartContent* pOwner)
{
	m_pOwner = (CXTPChartElement*)pOwner;

	m_pFont =  CXTPChartFont::GetTahoma8();

	m_bVisible = FALSE;

	m_bAntialiasing = FALSE;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillSolid);

	m_szMarkerSize = CSize(20, 16);
	m_bMarkerVisible = TRUE;

	m_nDirection = xtpChartLegendTopToBottom;
	m_nHorizontalAlignment = xtpChartLegendFarOutside;
	m_nVerticalAlignment = xtpChartLegendNear;

	m_nColumnCount = 0;

	m_ptOffset = CPoint(0, 0);


}

CXTPChartLegend::~CXTPChartLegend()
{
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pBorder);
	SAFE_RELEASE(m_pFillStyle);
}

void CXTPChartLegend::SetFont(CXTPChartFont* pFont)
{
	SAFE_RELEASE(m_pFont);
	m_pFont = pFont;
	OnChartChanged();
}

void CXTPChartLegend::SetTextColor(const CXTPChartColor& clrTextColor)
{
	m_clrTextColor = clrTextColor;
	OnChartChanged();
}

void CXTPChartLegend::SetBackgroundColor(const CXTPChartColor& clrBackgroundColor)
{
	m_clrBackgroundColor = clrBackgroundColor;
	OnChartChanged();
}

CXTPChartColor CXTPChartLegend::GetTextColor() const
{
	return m_clrTextColor;
}

CXTPChartColor CXTPChartLegend::GetActualTextColor() const
{
	if (!m_clrTextColor.IsEmpty())
		return m_clrTextColor;

	return GetContent()->GetAppearance()->GetContentAppearance()->GetLegendAppearance()->TextColor;
}

CXTPChartColor CXTPChartLegend::GetBackgroundColor() const
{
	return m_clrBackgroundColor;
}

CXTPChartColor CXTPChartLegend::GetActualBackgroundColor() const
{
	if (!m_clrBackgroundColor.IsEmpty())
		return m_clrBackgroundColor;

	return GetContent()->GetAppearance()->GetContentAppearance()->GetLegendAppearance()->BackgroundColor;
}

CXTPChartColor CXTPChartLegend::GetActualBorderColor() const
{
	if (!m_pBorder->GetColor().IsEmpty())
		return m_pBorder->GetColor();

	return GetContent()->GetAppearance()->GetContentAppearance()->GetLegendAppearance()->BorderColor;
}


CXTPChartLegendView* CXTPChartLegend::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);

	if (!m_bVisible)
		return NULL;

	CXTPChartLegendView* pView = new CXTPChartLegendView(this, pParentView);


	return pView;
}

void CXTPChartLegend::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, FALSE);
	PX_Bool(pPX, _T("MarkerVisible"), m_bMarkerVisible, FALSE);
	PX_Size(pPX, _T("MarkerSize"), m_szMarkerSize, CSize(0, 0));
	PX_Bool(pPX, _T("Antialiasing"), m_bAntialiasing, FALSE);

	PX_Int(pPX, _T("ColumnCount"), m_nColumnCount, 0);

	PX_Enum(pPX, _T("HorizontalAlignment"), m_nHorizontalAlignment, xtpChartLegendFarOutside);
	PX_Enum(pPX, _T("VerticalAlignment"), m_nVerticalAlignment, xtpChartLegendNear);
	PX_Enum(pPX, _T("Direction"), m_nDirection, xtpChartLegendTopToBottom);

	PX_Color(pPX, _T("BackColor"), m_clrBackgroundColor);
	PX_Color(pPX, _T("TextColor"), m_clrTextColor);


	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	PX_Point(pPX, _T("Offset"), m_ptOffset, CPoint(0, 0));
}


