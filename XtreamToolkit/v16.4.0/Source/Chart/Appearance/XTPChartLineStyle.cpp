// XTPChartLineStyle.cpp
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

#include "../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"

#include "../XTPChartDefines.h"
#include "../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>

#include "../Drawing/XTPChartDeviceCommand.h"
#include "../Drawing/XTPChartLineDeviceCommand.h"

#include "XTPChartLineStyle.h"



CXTPChartLineStyle::CXTPChartLineStyle(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;
	m_nDashStyle = xtpChartDashStyleSolid;
	m_nThickness = 1;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPChartLineStyle::SetDashStyle(XTPChartDashStyle nDashStyle)
{
	m_nDashStyle = nDashStyle;
	if (m_pOwner) m_pOwner->OnChartChanged();
}

void CXTPChartLineStyle::SetThickness(int nThickness)
{
	m_nThickness = nThickness;
	if (m_pOwner) m_pOwner->OnChartChanged();
}

CXTPChartDeviceCommand* CXTPChartLineStyle::CreateDeviceCommand(const CXTPChartPointF& point1, const CXTPChartPointF& point2, const CXTPChartColor& color)
{
	if (m_nDashStyle == xtpChartDashStyleEmpty)
		return NULL;

	if (m_nDashStyle == xtpChartDashStyleSolid)
		return new CXTPChartSolidLineDeviceCommand(point1, point2, color, m_nThickness);

	return new CXTPChartDashedLineDeviceCommand(point1, point2, color, m_nThickness, m_nDashStyle, m_arrDashArray);
}


CXTPChartDeviceCommand* CXTPChartLineStyle::CreateDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color)
{
	if (m_nDashStyle == xtpChartDashStyleEmpty)
		return NULL;

	if (m_nDashStyle == xtpChartDashStyleSolid)
		return new CXTPChartSolidPolylineDeviceCommand(points, color, m_nThickness);

	return new CXTPChartDashedPolylineDeviceCommand(points, color, m_nThickness, m_nDashStyle, m_arrDashArray);
}


CXTPChartDeviceCommand* CXTPChartLineStyle::CreateSplineDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color)
{
	if (m_nDashStyle == xtpChartDashStyleEmpty)
		return NULL;

	if (m_nDashStyle == xtpChartDashStyleSolid)
		return new CXTPChartSolidSplineDeviceCommand(points, color, m_nThickness);

	return new CXTPChartDashedSplineDeviceCommand(points, color, m_nThickness, m_nDashStyle, m_arrDashArray);
}

void CXTPChartLineStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Int(pPX, _T("Thickness"), m_nThickness, 1);
	PX_Enum(pPX, _T("DashStyle"), m_nDashStyle, xtpChartDashStyleSolid);

	if (pPX->IsStoring())
	{
		int nCount = (int)m_arrDashArray.GetSize();

		PX_Int(pPX, _T("DashArray"), nCount, 0);

		for (int i = 0; i < nCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Dash%d"), i);

			PX_Float(pPX, strSection, m_arrDashArray[i]);
		}
	}
	else
	{
		int nCount;
		PX_Int(pPX, _T("DashArray"), nCount, 0);

		m_arrDashArray.SetSize(nCount);

		for (int i = 0; i < nCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Dash%d"), i);

			PX_Float(pPX, strSection, m_arrDashArray[i]);
		}
	}
}

void CXTPChartLineStyle::SetDashArray(const CXTPChartLineDashArray& dashArray)
{
	m_arrDashArray.Copy(dashArray);
	m_nDashStyle = xtpChartDashStyleCustom;

	m_pOwner->OnChartChanged();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartLineStyle, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPChartLineStyle, "Thickness", 1, GetThickness, SetThickness, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartLineStyle, "DashStyle", 2, GetDashStyle, SetDashStyle, VT_I4)
	DISP_FUNCTION_ID(CXTPChartLineStyle, "SetDashArray", 3, OleSetDashArray, VT_EMPTY, VTS_VARIANT)
END_DISPATCH_MAP()


// {56DBCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartLineStyle =
{ 0x56dbcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartLineStyle, CCmdTarget)
	INTERFACE_PART(CXTPChartLineStyle, IID_IChartLineStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartLineStyle, IID_IChartLineStyle)

void CXTPChartLineStyle::OleSetDashArray(const VARIANT& var)
{
	if ((var.vt & VT_ARRAY) != 0)
	{
		int nType = var.vt & (~VT_ARRAY);

		if (nType != VT_VARIANT && nType != VT_I4 && nType != VT_I2 && nType != VT_R4 && nType != VT_R8)
			return;

		COleSafeArray saSrc(var);

		if (saSrc.GetDim() != 1)
			return;

		int nCount = saSrc.GetOneDimSize();

		long nLBound = 0;
		saSrc.GetLBound(1, &nLBound);

		if (nCount < 1)
			return;

		CXTPChartLineDashArray array;
		array.SetSize(nCount);

		for (long nIndex = 0; nIndex < nCount; nIndex++)
		{
			long nElement = nIndex + nLBound;

			if (nType == VT_VARIANT)
			{
				VARIANT varData;
				saSrc.GetElement(&nElement, &varData);

				COleVariant var(varData);
				var.ChangeType(VT_R4);

				array[nIndex] = var.fltVal;
			}
			else if (nType == VT_I2 || nType == VT_I4)
			{
				int nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				array[nIndex] = nItem;
			}
			else if (nType == VT_R4)
			{
				float nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				array[nIndex] = nItem;

			}
			else if (nType == VT_R8)
			{
				double nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				array[nIndex] = nItem;

			}
		}

		SetDashArray(array);
	}
}

#endif
