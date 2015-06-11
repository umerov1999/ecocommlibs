// XTPChartSeries.cpp
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

#include <Common/XTPPropExchange.h>

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartContent.h>
#include <Chart/XTPChartPanel.h>
#include <Chart/XTPChartDiagram.h>
#include <Chart/Appearance/XTPChartAppearance.h>
#include <Chart/Appearance/XTPChartPalette.h>
#include <Chart/Utils/XTPChartNumberFormat.h>


//////////////////////////////////////////////////////////////////////////
// CXTPChartSeries

IMPLEMENT_DYNAMIC(CXTPChartSeries, CXTPChartElement)

CXTPChartSeries::CXTPChartSeries()
	: m_pLegendItem(NULL)
{
	m_pStyle = NULL;
	m_pDiagram = NULL;

	m_pPoints = new CXTPChartSeriesPointCollection();
	m_pPoints->m_pOwner = this;

	m_pLegendItem = new CXTPChartLegendItem();
	m_pLegendItem->m_pOwner = this;

	m_nArgumentScaleType = xtpChartScaleQualitative;
	m_nValueScaleType = xtpChartScaleNumerical;

	m_nPaletteIndex = 0;
	m_bVisible = TRUE;

	m_bLegendVisible = TRUE;

	m_pPointLegendFormat = new CXTPChartNumberFormat(this);
	m_pPointLegendFormat->SetPattern(_T("{A}: {V}"));

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartSeries::~CXTPChartSeries()
{
	SAFE_RELEASE(m_pStyle);
	SAFE_RELEASE(m_pPoints);
	SAFE_RELEASE(m_pLegendItem);
	SAFE_RELEASE(m_pPointLegendFormat);

	SetDiagram(NULL);
}

CXTPChartLegendItem* CXTPChartSeries::GetLegendItem() const
{
	return m_pLegendItem;
}

CXTPChartColor CXTPChartSeries::GetColor() const
{

	if (GetStyle()->GetColor().GetValue() != CXTPChartColor::Empty)
		return GetStyle()->GetColor();

	return GetAppearance()->GetPalette()->GetColor(GetPaletteIndex());
}

CXTPChartColor CXTPChartSeries::GetColor2() const
{
	if (GetStyle()->GetColor().GetValue() != CXTPChartColor::Empty)
		return CXTPChartAppearance::GetLightColor(GetStyle()->GetColor());

	return GetAppearance()->GetPalette()->GetColor2(GetPaletteIndex());
}

CXTPChartSeriesStyle* CXTPChartSeries::SetStyle(CXTPChartSeriesStyle* pStyle)
{
	if (!pStyle)
		return pStyle;

	SAFE_RELEASE(m_pStyle);

	m_pStyle = pStyle;
	m_pStyle->m_pOwner = this;

	GetContent()->OnSeriesStyleChanged(this);

	OnChartChanged();

	return pStyle;
}

BOOL CXTPChartSeries::SetDiagram(CXTPChartDiagram* pDiagram)
{
	if (pDiagram == m_pDiagram)
		return TRUE;

	if (pDiagram)
	{
		if (!m_pStyle)
			return FALSE;

		if (!m_pStyle->IsStyleDiagram(pDiagram))
			return FALSE;
	}

	if (m_pDiagram)
	{
		m_pDiagram->OnSeriesRemoved(this);

		CMDTARGET_RELEASE(m_pDiagram);
	}


	if (pDiagram)
	{
		if (!pDiagram->OnSeriesAdded(this))
			return FALSE;

		pDiagram->InternalAddRef();
	}

	m_pDiagram = pDiagram;

	OnChartChanged();

	return TRUE;
}

void CXTPChartSeries::SetPointLegendFormat(LPCTSTR lpszFormat)
{
	m_pPointLegendFormat->SetPattern(lpszFormat);
}

void CXTPChartSeries::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("LegendText"), m_strLegendText);
	PX_String(pPX, _T("Name"), m_strName);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Enum(pPX, _T("ArgumentScaleType"), m_nArgumentScaleType, xtpChartScaleQualitative);
	PX_Enum(pPX, _T("ValueScaleType"), m_nValueScaleType, xtpChartScaleNumerical);

	PX_Bool(pPX, _T("LegendVisible"), m_bLegendVisible, TRUE);

	CXTPPropExchangeSection secPointLegendFormat(pPX->GetSection(_T("PointLegendFormat")));
	m_pPointLegendFormat->DoPropExchange(&secPointLegendFormat);

	CXTPPropExchangeSection secPoints(pPX->GetSection(_T("Points")));
	m_pPoints->DoPropExchange(&secPoints);

	PX_Object(pPX, _T("Style"), m_pStyle, RUNTIME_CLASS(CXTPChartSeriesStyle));

	if (pPX->IsLoading())
	{
		m_pStyle->m_pOwner = this;
	}

	CXTPChartContent* pContent = GetContent();
	ASSERT (pContent);

	if (pPX->IsStoring())
	{
		int nIndex = 0;
		for (; nIndex < pContent->GetPanels()->GetCount(); nIndex++)
		{
			if (pContent->GetPanels()->GetAt(nIndex) == m_pDiagram)
			{
				break;
			}
		}
		PX_Int(pPX, _T("Diagram"), nIndex, -1);
	}
	else
	{
		int nIndex;
		PX_Int(pPX, _T("Diagram"), nIndex, -1);

		if (nIndex >= 0 && nIndex < pContent->GetPanels()->GetCount())
		{
			CXTPChartDiagram* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram, pContent->GetPanels()->GetAt(nIndex));

			SetDiagram(pDiagram);
		}
	}
}

void CXTPChartSeries::SetPoints(CXTPChartSeriesPointCollection* pPoints)
{
	ASSERT(pPoints);
	if (!pPoints)
		return;

	SAFE_RELEASE(m_pPoints);

	m_pPoints = pPoints;
	m_pPoints->m_pOwner = this;

	OnChartChanged();
}

void CXTPChartSeries::Release()
{
	SetDiagram(NULL);

	CXTPChartElement::Release();
}





#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartSeries, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartSeries, "Style", 1, OleGetStyle, OleSetStyle, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeries, "Diagram", 2, OleGetDiagram, OleSetDiagram, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeries, "Points", 3, OleGetPoints, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeries, "PointLegendFormat", 8, OleGetPointLegendFormat, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartSeries, "LegendItem", 12, OleGetLegendItem, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartSeries, "ArgumentScaleType", 4, OleGetArgumentScaleType, OleSetArgumentScaleType, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartSeries, "ValueScaleType", 5, OleGetValueScaleType, OleSetValueScaleType, VT_I4)

	DISP_PROPERTY_NOTIFY_ID(CXTPChartSeries, "Name", 6, m_strName, OleChartChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartSeries, "LegendText", 7, m_strLegendText, OleChartChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartSeries, "Visible", 9, m_bVisible, OleChartChanged, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPChartSeries, "LegendVisible", 10, OleGetLegendVisible, OleSetLegendVisible, VT_BOOL)


	DISP_FUNCTION_ID(CXTPChartSeries, "EnableVirtualMode", 11, OleEnableVirtualMode, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8)

END_DISPATCH_MAP()

// {CD7BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartSeries =
{ 0xcd7bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartSeries, CXTPChartElement)
	INTERFACE_PART(CXTPChartSeries, IID_IChartSeries, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSeries, IID_IChartSeries)


long CXTPChartSeries::OleGetArgumentScaleType()
{
	return m_nArgumentScaleType;
}

void CXTPChartSeries::OleSetArgumentScaleType(long ArgumentScaleType)
{
	SetArgumentScaleType((XTPChartScaleType)ArgumentScaleType);
}

long CXTPChartSeries::OleGetValueScaleType()
{
	return m_nValueScaleType;
}

void CXTPChartSeries::OleSetValueScaleType(long ValueScaleType)
{
	SetValueScaleType((XTPChartScaleType)ValueScaleType);
}

LPDISPATCH CXTPChartSeries::OleGetStyle()
{
	return XTPGetDispatch(GetStyle());
}

LPDISPATCH CXTPChartSeries::OleGetPointLegendFormat()
{
	return XTPGetDispatch(m_pPointLegendFormat);
}

LPDISPATCH CXTPChartSeries::OleGetLegendItem()
{
	return XTPGetDispatch(m_pLegendItem);
}

void CXTPChartSeries::OleSetStyle(LPDISPATCH lpStyle)
{
	if (lpStyle)
	{
		lpStyle->AddRef();
		SetStyle((CXTPChartSeriesStyle*)CCmdTarget::FromIDispatch(lpStyle));
	}
}

LPDISPATCH CXTPChartSeries::OleGetPoints()
{
	return XTPGetDispatch(GetPoints());
}

LPDISPATCH CXTPChartSeries::OleGetDiagram()
{
	return XTPGetDispatch(GetDiagram());
}

void CXTPChartSeries::OleSetDiagram(LPDISPATCH lpDiagram)
{
	if (lpDiagram)
	{
		SetDiagram((CXTPChartDiagram*)CCmdTarget::FromIDispatch(lpDiagram));
	}
}

void CXTPChartSeries::OleChartChanged()
{
	OnChartChanged();
}


BOOL CXTPChartSeries::OleGetLegendVisible()
{
	return m_bLegendVisible;
}

void  CXTPChartSeries::OleSetLegendVisible(BOOL bValue)
{
	SetLegendVisible(bValue);
}


double OleGetPointValue(CXTPChartSeries* pSeries, double x)
{
	CXTPChartContent* pContent = pSeries->GetContent();

	COleControl* pOleControl = pContent->GetOleControl();

	double y = 0;
	pOleControl->FireEvent(10, EVENT_PARAM(VTS_DISPATCH VTS_R8 VTS_PR8), XTPGetDispatch(pSeries, FALSE), x, &y);

	return y;
}

void CXTPChartSeries::OleEnableVirtualMode(double MinRange, double MaxRange, double Step)
{
	SetPoints(new CXTPChartSeriesVirtualPointCollection(MinRange, MaxRange, Step, OleGetPointValue));

}

#endif
