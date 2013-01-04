// XTPChartDiagram2DSeriesStyle.cpp
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

#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"


#include "XTPChartDiagram2D.h"
#include "XTPChartDiagram2DSeriesStyle.h"


IMPLEMENT_DYNAMIC(CXTPChartDiagram2DSeriesStyle, CXTPChartSeriesStyle)

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DSeriesStyle

CXTPChartDiagram2DSeriesStyle::CXTPChartDiagram2DSeriesStyle()
{
	m_bSecondaryAxisX = FALSE;
	m_bSecondaryAxisY = FALSE;
}

CXTPChartDiagram2DSeriesStyle::~CXTPChartDiagram2DSeriesStyle()
{
}

void CXTPChartDiagram2DSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);

	PX_Bool(pPX, _T("SecondaryAxisX"), m_bSecondaryAxisX, FALSE);
	PX_Bool(pPX, _T("SecondaryAxisY"), m_bSecondaryAxisY, FALSE);
}

CXTPChartDiagram* CXTPChartDiagram2DSeriesStyle::CreateDiagram()
{
	return new CXTPChartDiagram2D();
}

BOOL CXTPChartDiagram2DSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	return DYNAMIC_DOWNCAST(CXTPChartDiagram2D, pDiagram) != NULL;
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartDiagram2DSeriesStyle, CXTPChartSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DSeriesStyle, "SecondaryAxisX", 30, IsSecondaryAxisX, SetSecondaryAxisX, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2DSeriesStyle, "SecondaryAxisY", 31, IsSecondaryAxisY, SetSecondaryAxisY, VT_BOOL)
END_DISPATCH_MAP()


// {144BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartDiagram2DSeriesStyle =
{ 0x144bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartDiagram2DSeriesStyle, CXTPChartSeriesStyle)
	INTERFACE_PART(CXTPChartDiagram2DSeriesStyle, IID_IChartDiagram2DSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartDiagram2DSeriesStyle, IID_IChartDiagram2DSeriesStyle)

#endif
