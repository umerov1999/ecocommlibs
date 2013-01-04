// XTPChartFillStyle.cpp
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

#include "../Types/XTPChartTypes.h"

#include "../XTPChartDefines.h"
#include "../XTPChartElement.h"

#include "../Drawing/XTPChartDeviceCommand.h"
#include "../Drawing/XTPChartCircleDeviceCommand.h"
#include "../Drawing/XTPChartRectangleDeviceCommand.h"
#include "../Drawing/XTPChartPolygonDeviceCommand.h"

#include "XTPChartFillStyle.h"

LPCTSTR lpszGradientAngle[] =
{
	_T("TopToBottom"),
	_T("BottomToTop"),
	_T("LeftToRight"),
	_T("RightToLeft"),
	_T("TopLeftToBottomRight"),
	_T("BottomRightToTopLeft"),
	_T("TopRightToBottomLeft"),
	_T("BottomLeftToTopRight"),
	_T("ToCenterHorizontal"),
	_T("FromCenterHorizontal"),
	_T("ToCenterVertical"),
	_T("FromCenterVertical")
};




CXTPChartFillStyle::CXTPChartFillStyle(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;

	m_nFillMode = xtpChartFillSolid;

	m_nHatchStyle = xtpChartHatchStyleSolidDiamond;

	m_nGradientAngle = xtpChartGradientAngle90;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartDeviceCommand* CXTPChartFillStyle::CreateDeviceCommand(const CXTPChartRectF& bounds, const CXTPChartColor& color, const CXTPChartColor& color2)
{
	if (m_nFillMode == xtpChartFillSolid)
	{
		return new CXTPChartSolidRectangleDeviceCommand(bounds, color);
	}

	if (m_nFillMode == xtpChartFillHatch)
	{
		return new CXTPChartHatchRectangleDeviceCommand(bounds, color, color2, m_nHatchStyle);
	}

	if (m_nFillMode == xtpChartFillGradient)
	{
		switch(m_nGradientAngle)
		{
			case xtpChartGradientAngle180:
				return new CXTPChartGradientRectangleDeviceCommand(bounds, color, color2, xtpChartLinearGradientModeVertical);

			case xtpChartGradientAngle0:
				return new CXTPChartGradientRectangleDeviceCommand(bounds, color2, color, xtpChartLinearGradientModeVertical);

			case xtpChartGradientAngle90:
				return new CXTPChartGradientRectangleDeviceCommand(bounds, color, color2, xtpChartLinearGradientModeHorizontal);

			case xtpChartGradientAngle270:
				return new CXTPChartGradientRectangleDeviceCommand(bounds, color2, color, xtpChartLinearGradientModeHorizontal);

			case xtpChartGradientAngle135:
				return new CXTPChartGradientRectangleDeviceCommand(bounds, color, color2, xtpChartLinearGradientModeForwardDiagonal);

			case xtpChartGradientAngle315:
				return new CXTPChartGradientRectangleDeviceCommand(bounds, color2, color, xtpChartLinearGradientModeForwardDiagonal);

			case xtpChartGradientAngle225:
				return new CXTPChartGradientRectangleDeviceCommand(bounds, color2, color, xtpChartLinearGradientModeBackwardDiagonal);

			case xtpChartGradientAngle45:
				return new CXTPChartGradientRectangleDeviceCommand(bounds, color, color2, xtpChartLinearGradientModeBackwardDiagonal);
		}
	}


	return NULL;
}


CXTPChartDeviceCommand* CXTPChartFillStyle::CreateCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color, const CXTPChartColor& color2)
{
	if (m_nFillMode == xtpChartFillSolid)
	{
		return new CXTPChartSolidCircleDeviceCommand(center, radius, color);
	}

	if (m_nFillMode == xtpChartFillHatch)
	{
		return new CXTPChartHatchCircleDeviceCommand(center, radius, color, color2, m_nHatchStyle);
	}

	if (m_nFillMode == xtpChartFillGradient)
	{
		switch(m_nGradientAngle)
		{
		case xtpChartGradientAngle180:
			return new CXTPChartGradientCircleDeviceCommand(center, radius, color, color2, xtpChartLinearGradientModeVertical);

		case xtpChartGradientAngle0:
			return new CXTPChartGradientCircleDeviceCommand(center, radius, color2, color, xtpChartLinearGradientModeVertical);

		case xtpChartGradientAngle90:
			return new CXTPChartGradientCircleDeviceCommand(center, radius, color, color2, xtpChartLinearGradientModeHorizontal);

		case xtpChartGradientAngle270:
			return new CXTPChartGradientCircleDeviceCommand(center, radius, color2, color, xtpChartLinearGradientModeHorizontal);

		case xtpChartGradientAngle135:
			return new CXTPChartGradientCircleDeviceCommand(center, radius, color, color2, xtpChartLinearGradientModeForwardDiagonal);

		case xtpChartGradientAngle315:
			return new CXTPChartGradientCircleDeviceCommand(center, radius, color2, color, xtpChartLinearGradientModeForwardDiagonal);

		case xtpChartGradientAngle225:
			return new CXTPChartGradientCircleDeviceCommand(center, radius, color2, color, xtpChartLinearGradientModeBackwardDiagonal);

		case xtpChartGradientAngle45:
			return new CXTPChartGradientCircleDeviceCommand(center, radius, color, color2, xtpChartLinearGradientModeBackwardDiagonal);
		}
	}


	return NULL;
}



CXTPChartDeviceCommand* CXTPChartFillStyle::CreateSplineDeviceCommand(const CXTPChartPoints& arrPoints, const CXTPChartColor& color, const CXTPChartColor& /*color2*/, BOOL bTwoSides)
{
	return new CXTPChartSolidSplinePolygonDeviceCommand(arrPoints, color, bTwoSides);
}

CXTPChartDeviceCommand* CXTPChartFillStyle::CreateDeviceCommand(const CXTPChartPoints& arrPoints, const CXTPChartColor& color, const CXTPChartColor& color2)
{
	if (m_nFillMode == xtpChartFillSolid)
	{
		return new CXTPChartSolidPolygonDeviceCommand(arrPoints, color);
	}

	if (m_nFillMode == xtpChartFillHatch)
	{
		return new CXTPChartHatchPolygonDeviceCommand(arrPoints, color, color2, m_nHatchStyle);
	}

	if (m_nFillMode == xtpChartFillGradient)
	{
		switch(m_nGradientAngle)
		{
		case xtpChartGradientAngle180:
			return new CXTPChartGradientPolygonDeviceCommand(arrPoints, color, color2, xtpChartLinearGradientModeVertical);

		case xtpChartGradientAngle0:
			return new CXTPChartGradientPolygonDeviceCommand(arrPoints, color2, color, xtpChartLinearGradientModeVertical);

		case xtpChartGradientAngle90:
			return new CXTPChartGradientPolygonDeviceCommand(arrPoints, color, color2, xtpChartLinearGradientModeHorizontal);

		case xtpChartGradientAngle270:
			return new CXTPChartGradientPolygonDeviceCommand(arrPoints, color2, color, xtpChartLinearGradientModeHorizontal);

		case xtpChartGradientAngle135:
			return new CXTPChartGradientPolygonDeviceCommand(arrPoints, color, color2, xtpChartLinearGradientModeForwardDiagonal);

		case xtpChartGradientAngle315:
			return new CXTPChartGradientPolygonDeviceCommand(arrPoints, color2, color, xtpChartLinearGradientModeForwardDiagonal);

		case xtpChartGradientAngle225:
			return new CXTPChartGradientPolygonDeviceCommand(arrPoints, color2, color, xtpChartLinearGradientModeBackwardDiagonal);

		case xtpChartGradientAngle45:
			return new CXTPChartGradientPolygonDeviceCommand(arrPoints, color, color2, xtpChartLinearGradientModeBackwardDiagonal);
		}
	}
	return NULL;
}

void CXTPChartFillStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		CString strFillMode, strGradientAngle;

		PX_String(pPX, _T("FillMode"), strFillMode);

		if (strFillMode.CompareNoCase(_T("Gradient")) == 0)
		{
			m_nFillMode = xtpChartFillGradient;
			PX_String(pPX, _T("GradientAngle"), strGradientAngle);

			for (int i = 0; i < _countof(lpszGradientAngle); i++)
			{
				if (strGradientAngle.CompareNoCase(lpszGradientAngle[i]) == 0)
				{
					m_nGradientAngle = (XTPChartGradientAngle)i;
					break;
				}
			}

		}
		else if (strFillMode.CompareNoCase(_T("Empty")) == 0)
		{
			m_nFillMode = xtpChartFillEmpty;
		}
		else if (strFillMode.CompareNoCase(_T("Hatch")) == 0)
		{
			m_nFillMode = xtpChartFillHatch;

			PX_Enum(pPX, _T("HatchStyle"), m_nHatchStyle, xtpChartHatchStyleSolidDiamond);
		}
		else
		{
			m_nFillMode = xtpChartFillSolid;
		}
	}
	else
	{
		CString strFillMode = m_nFillMode == xtpChartFillEmpty ? _T("Empty") : m_nFillMode == xtpChartFillGradient ? _T("Gradient") :
			m_nFillMode == xtpChartFillHatch ? _T("Hatch") : _T("");

		PX_String(pPX, _T("FillMode"), strFillMode, _T(""));

		if (m_nFillMode == xtpChartFillGradient)
		{
			CString  strGradientAngle = lpszGradientAngle[m_nGradientAngle];

			PX_String(pPX, _T("GradientAngle"), strGradientAngle);
		}

		if (m_nFillMode == xtpChartFillHatch)
		{
			PX_Enum(pPX, _T("HatchStyle"), m_nHatchStyle, xtpChartHatchStyleHorizontal);
		}
	}

}


void CXTPChartFillStyle::SetFillMode(XTPChartFillMode nFillMode)
{
	m_nFillMode = nFillMode;
	m_pOwner->OnChartChanged();
}

void CXTPChartFillStyle::SetGradientAngle(XTPChartGradientAngle nAngle)
{
	m_nGradientAngle = nAngle;
	m_pOwner->OnChartChanged();
}

void CXTPChartFillStyle::SetHatchStyle(XTPChartHatchStyle nStyle)
{
	m_nHatchStyle = nStyle;
	m_pOwner->OnChartChanged();
}

CXTPChartFillStyle* CXTPChartFillStyle::CreateRotatedStyle()
{
	if (m_nFillMode != xtpChartFillGradient)
	{
		CXTPChartFillStyle* pStyle = this;
		pStyle->InternalAddRef();
		return pStyle;
	}

	CXTPChartFillStyle *pStyle = new CXTPChartFillStyle(m_pOwner);
	pStyle->m_nFillMode = xtpChartFillGradient;

	switch(m_nGradientAngle)
	{
	case xtpChartGradientAngle0:   pStyle->m_nGradientAngle = xtpChartGradientAngle90;  break;
	case xtpChartGradientAngle45:  pStyle->m_nGradientAngle = xtpChartGradientAngle135; break;
	case xtpChartGradientAngle90:  pStyle->m_nGradientAngle = xtpChartGradientAngle180; break;
	case xtpChartGradientAngle135: pStyle->m_nGradientAngle = xtpChartGradientAngle225; break;
	case xtpChartGradientAngle180: pStyle->m_nGradientAngle = xtpChartGradientAngle270; break;
	case xtpChartGradientAngle225: pStyle->m_nGradientAngle = xtpChartGradientAngle315; break;
	case xtpChartGradientAngle270: pStyle->m_nGradientAngle = xtpChartGradientAngle0;   break;
	case xtpChartGradientAngle315: pStyle->m_nGradientAngle = xtpChartGradientAngle45;  break;
	}

	return pStyle;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartFillStyle, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPChartFillStyle, "FillMode",          1, OleGetFillMode,          SetFillMode,          VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartFillStyle, "HatchStyle",        2, OleGetHatchStyle,        SetHatchStyle,        VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartFillStyle, "GradientAngle", 3, OleGetGradientAngle, SetGradientAngle, VT_I4)
END_DISPATCH_MAP()


// {76DBCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartFillStyle =
{ 0x76dbcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartFillStyle, CCmdTarget)
	INTERFACE_PART(CXTPChartFillStyle, IID_IChartFillStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartFillStyle, IID_IChartFillStyle)

int CXTPChartFillStyle::OleGetFillMode()
{
	return GetFillMode();
}

int CXTPChartFillStyle::OleGetHatchStyle()
{
	return GetHatchStyle();
}

int CXTPChartFillStyle::OleGetGradientAngle()
{
	return GetGradientAngle();
}

#endif
