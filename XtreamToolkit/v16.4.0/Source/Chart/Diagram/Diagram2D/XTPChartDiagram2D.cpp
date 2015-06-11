// XTPChartDiagram2D.cpp
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
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartPanel.h>
#include <Chart/XTPChartDiagram.h>
#include <Chart/XTPChartDiagramView.h>

#include <Chart/Appearance/XTPChartAppearance.h>

#include <Chart/Diagram/Axis/XTPChartAxis.h>
#include <Chart/Diagram/Axis/XTPChartAxisGridLines.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2D.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DPane.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DView.h>


IMPLEMENT_SERIAL(CXTPChartDiagram2D, CXTPChartDiagram, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartDiagram2D::CXTPChartDiagram2D()
{
	SetAxisCountX(2);
	SetAxisCountY(2);

	m_pPane = new CXTPChartDiagram2DPane(this);

	m_bRotated = FALSE;


	m_bAllowZoom = FALSE;
	m_bAllowScroll = TRUE;
}

void CXTPChartDiagram2D::SetPane(CXTPChartDiagram2DPane* pPane)
{
	if (m_pPane != pPane)
	{
		SAFE_RELEASE(m_pPane);
		m_pPane = pPane;
	}
}

CXTPChartDiagram2D::~CXTPChartDiagram2D()
{
	SetAxisCountX(0);
	SetAxisCountY(0);

	SAFE_RELEASE(m_pPane);
}

CXTPChartDiagramView* CXTPChartDiagram2D::CreateView(CXTPChartDeviceContext* /*pDC*/, CXTPChartElementView* pParent)
{
	return new CXTPChartDiagram2DView(this, pParent);
}

void CXTPChartDiagram2D::UpdateLayout(CXTPChartDeviceContext* /*pDC*/, CXTPChartDiagramView* /*pView*/, CRect /*rcBounds*/)
{
}


CXTPChartDiagram2DAppearance* CXTPChartDiagram2D::GetAppearance() const
{
	return CXTPChartElement::GetAppearance()->GetDiagram2DAppearance();
}

void CXTPChartDiagram2D::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram::DoPropExchange(pPX);

	PX_Bool(pPX, _T("Rotated"), m_bRotated, FALSE);

	PX_Bool(pPX, _T("AllowZoom"), m_bAllowZoom, FALSE);
	PX_Bool(pPX, _T("AllowScroll"), m_bAllowScroll, TRUE);



	CXTPPropExchangeSection secAxisX(pPX->GetSection(_T("AxisX")));
	GetAxisX()->DoPropExchange(&secAxisX);

	CXTPPropExchangeSection secAxisY(pPX->GetSection(_T("AxisY")));
	GetAxisY()->DoPropExchange(&secAxisY);

	CXTPPropExchangeSection secSecondaryAxisX(pPX->GetSection(_T("SecondaryAxisX")));
	GetSecondaryAxisX()->DoPropExchange(&secSecondaryAxisX);

	CXTPPropExchangeSection secSecondaryAxisY(pPX->GetSection(_T("SecondaryAxisY")));
	GetSecondaryAxisY()->DoPropExchange(&secSecondaryAxisY);

	CXTPPropExchangeSection secPane(pPX->GetSection(_T("Pane")));
	m_pPane->DoPropExchange(&secPane);
}


int CXTPChartDiagram2D::GetAxisCountX() const
{
	return static_cast<int>(m_arrAxesX.GetSize());
}

int CXTPChartDiagram2D::GetAxisCountY() const
{
	return static_cast<int>(m_arrAxesY.GetSize());
}


void CXTPChartDiagram2D::SetAxisCountX(int nAxisCountNewX)
{
	const int nAxisCountOldX = GetAxisCountX();

	if (nAxisCountNewX > nAxisCountOldX)
	{
		// Append axes
		for (int nAxisX=nAxisCountOldX; nAxisX<nAxisCountNewX; nAxisX++)
		{
			CXTPChartAxisX *pAxisX = new CXTPChartAxisX(this);

			if (nAxisX > xtpChartAxisPrimary)
			{
				// Secondary axis
				pAxisX->m_bSecondary = TRUE;
				pAxisX->SetVisible(FALSE);
				pAxisX->SetAlignment(xtpChartAxisFar);
				pAxisX->GetGridLines()->SetVisible(FALSE);
				pAxisX->SetInterlaced(FALSE);
			}

			m_arrAxesX.Add(pAxisX);
		}
	}

	if (nAxisCountNewX < nAxisCountOldX)
	{
		// Remove axes
		for(int nAxisX=nAxisCountNewX; nAxisX<nAxisCountOldX; nAxisX++)
		{
			CXTPChartAxis *pAxis = m_arrAxesX.GetAt(nAxisX);
			SAFE_RELEASE(pAxis);
		}
	}
}


void CXTPChartDiagram2D::SetAxisCountY(int nAxisCountNewY)
{
	const int nAxisCountOldY = GetAxisCountY();

	if (nAxisCountNewY > nAxisCountOldY)
	{
		// Append axes
		for (int nAxisY=nAxisCountOldY; nAxisY<nAxisCountNewY; nAxisY++)
		{
			CXTPChartAxisY *pAxisY = new CXTPChartAxisY(this);

			if (nAxisY > xtpChartAxisPrimary)
			{
				// Secondary axis
				pAxisY->m_bSecondary = TRUE;
				pAxisY->SetVisible(FALSE);
				pAxisY->SetAlignment(xtpChartAxisFar);
				pAxisY->GetGridLines()->SetVisible(FALSE);
				pAxisY->SetInterlaced(FALSE);
			}

			m_arrAxesY.Add(pAxisY);
		}
	}

	if (nAxisCountNewY < nAxisCountOldY)
	{
		// Remove axes
		for(int nAxisY=nAxisCountNewY; nAxisY<nAxisCountOldY; nAxisY++)
		{
			CXTPChartAxis *pAxis = m_arrAxesY.GetAt(nAxisY);
			SAFE_RELEASE(pAxis);
		}
	}
}



CXTPChartAxis* CXTPChartDiagram2D::GetAxisX(int nIndex) const
{
	CXTPChartAxis *pAxis = NULL;

	if (nIndex>=0 && nIndex<m_arrAxesX.GetSize())
	{
		pAxis = m_arrAxesX.GetAt(nIndex);
	}

	return pAxis;
}

CXTPChartAxis* CXTPChartDiagram2D::GetAxisY(int nIndex) const
{
	CXTPChartAxis *pAxis = NULL;

	if (nIndex>=0 && nIndex<m_arrAxesY.GetSize())
	{
		pAxis = m_arrAxesY.GetAt(nIndex);
	}

	return pAxis;
}


// OLD METHODS:

CXTPChartAxis* CXTPChartDiagram2D::GetAxisX() const
{
	return GetAxisX(xtpChartAxisPrimary);
}

CXTPChartAxis* CXTPChartDiagram2D::GetAxisY() const
{
	return GetAxisY(xtpChartAxisPrimary);
}


CXTPChartAxis* CXTPChartDiagram2D::GetSecondaryAxisX() const
{
	return GetAxisX(xtpChartAxisSecondary);
}

CXTPChartAxis* CXTPChartDiagram2D::GetSecondaryAxisY() const
{
	return GetAxisY(xtpChartAxisSecondary);
}



#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartDiagram2D, CXTPChartDiagram)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "AxisX", 1, OleGetAxisX, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "AxisY", 2, OleGetAxisY, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "SecondaryAxisX", 3, OleGetSecondaryAxisX, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "SecondaryAxisY", 4, OleGetSecondaryAxisY, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "Rotated", 5, IsRotated, SetRotated, VT_BOOL)


	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "BackgroundColor", 7, OleGetBackgroundColor, OleSetBackgroundColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "BackgroundColor2", 8, OleGetBackgroundColor2, OleSetBackgroundColor2, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "BorderColor", 9, OleGetBorderColor, OleSetBorderColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "BackgroundFillStyle", 10, OleGetBackgroundFillStyle, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "AllowZoom", 20, IsAllowZoom, SetAllowZoom, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartDiagram2D, "AllowScroll", 21, IsAllowScroll, SetAllowScroll, VT_BOOL)


END_DISPATCH_MAP()

// {EB1BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartDiagram2D =
{ 0xeb1bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartDiagram2D, CXTPChartDiagram)
	INTERFACE_PART(CXTPChartDiagram2D, IID_IChartDiagram2D, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartDiagram2D, IID_IChartDiagram2D)

// {98190E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartDiagram2D, "Codejock.ChartDiagram2D." _XTP_AXLIB_VERSION,
						0x98190e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


LPDISPATCH CXTPChartDiagram2D::OleGetAxisX()
{
	return XTPGetDispatch(GetAxisX());
}

LPDISPATCH CXTPChartDiagram2D::OleGetAxisY()
{
	return XTPGetDispatch(GetAxisY());
}

LPDISPATCH CXTPChartDiagram2D::OleGetSecondaryAxisX()
{
	return XTPGetDispatch(GetSecondaryAxisX());
}

LPDISPATCH CXTPChartDiagram2D::OleGetSecondaryAxisY()
{
	return XTPGetDispatch(GetSecondaryAxisY());
}

OLE_COLOR CXTPChartDiagram2D::OleGetBackgroundColor()
{
	return m_pPane->GetBackgroundColor().ToOleColor();
}

void CXTPChartDiagram2D::OleSetBackgroundColor(OLE_COLOR clr)
{
	m_pPane->SetBackgroundColor(CXTPChartColor::FromOleColor(clr));
}

OLE_COLOR CXTPChartDiagram2D::OleGetBackgroundColor2()
{
	return m_pPane->GetBackgroundColor2().ToOleColor();
}

void CXTPChartDiagram2D::OleSetBackgroundColor2(OLE_COLOR clr)
{
	m_pPane->SetBackgroundColor2(CXTPChartColor::FromOleColor(clr));
}

OLE_COLOR CXTPChartDiagram2D::OleGetBorderColor()
{
	return m_pPane->GetBorderColor().ToOleColor();
}

void CXTPChartDiagram2D::OleSetBorderColor(OLE_COLOR clr)
{
	m_pPane->SetBorderColor(CXTPChartColor::FromOleColor(clr));
}

LPDISPATCH CXTPChartDiagram2D::OleGetBackgroundFillStyle()
{
	return XTPGetDispatch((CCmdTarget*)m_pPane->GetFillStyle());
}

#endif
