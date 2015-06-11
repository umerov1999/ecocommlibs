// XTPChartContent.cpp
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
#include "Common/XTPMarkupRender.h"

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "XTPChartElementView.h"
#include "XTPChartContent.h"
#include "XTPChartPanel.h"
#include "XTPChartDiagram.h"
#include "XTPChartTitle.h"
#include "XTPChartSeries.h"
#include "XTPChartSeriesStyle.h"
#include "XTPChartSeriesPoint.h"
#include "XTPChartContentView.h"
#include "XTPChartControl.h"

#include "XTPChartLegend.h"

#include "Drawing/XTPChartDeviceContext.h"
#include "Drawing/XTPChartDeviceCommand.h"
#include "Appearance/XTPChartAppearance.h"
#include "Appearance/XTPChartBorder.h"



//////////////////////////////////////////////////////////////////////////
// CXTPChartContent
IMPLEMENT_DYNAMIC(CXTPChartContent, CXTPChartElement)

CXTPChartContent::CXTPChartContent()
{
	m_pTitles = new CXTPChartTitleCollection(this);

	m_pSeries = new CXTPChartSeriesCollection(this);

	m_pLegend = new CXTPChartLegend(this);

	m_pPanels = new CXTPChartPanelCollection(this);

	m_pAppearance = new CXTPChartAppearance(this);

	m_pBorder = new CXTPChartBorder(this);

	m_pMarkupContext = NULL;


	m_nPanelDistance = 10;
	m_nPanelDirection = xtpChartPanelHorizontal;


	CXTPChartDeviceContext::Register(TRUE);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartContent::~CXTPChartContent()
{

	SAFE_RELEASE(m_pTitles);
	SAFE_RELEASE(m_pSeries);
	SAFE_RELEASE(m_pPanels);

	SAFE_RELEASE(m_pLegend);
	SAFE_RELEASE(m_pAppearance);
	SAFE_RELEASE(m_pBorder);

	XTPMarkupReleaseContext(m_pMarkupContext);

	CXTPChartDeviceContext::Register(FALSE);
}


CXTPChartContentView* CXTPChartContent::CreateView(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartContentView* pContentView = new CXTPChartContentView(pDC->GetContainer(), this);

	pContentView->CreateView(pDC);
	pContentView->CalculateView(pDC, rcBounds);


	return pContentView;
}


CXTPChartDeviceCommand* CXTPChartContent::CreateDeviceComand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartElementView* pView = CreateView(pDC, rcBounds);

	CXTPChartDeviceCommand* pCommand = NULL;

	if (pView)
	{
		pCommand = pView->CreateDeviceCommand(pDC);
	}

	SAFE_RELEASE(pView);

	return pCommand;
}

void CXTPChartContent::DrawContent(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartDeviceCommand* pCommand = CreateDeviceComand(pDC, rcBounds);

	if (pCommand)
	{
		pDC->Execute(pCommand);

		delete pCommand;
	}
}

void CXTPChartContent::OnSeriesStyleChanged(CXTPChartSeries* pSeries)
{
	int i;

	ASSERT(pSeries->GetStyle());

	CXTPChartDiagram* pDiagram = pSeries->GetDiagram();

	if (pDiagram)
	{
		if (pSeries->GetStyle()->IsStyleDiagram(pSeries->GetDiagram()))
			return;

		pSeries->SetDiagram(NULL);
	}


	for (i = 0; i < m_pPanels->GetCount(); i++)
	{
		CXTPChartDiagram* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram, m_pPanels->GetAt(i));
		if (!pDiagram)
			continue;

		if (pSeries->SetDiagram(pDiagram))
		{
			return;
		}
	}

	pDiagram = pSeries->GetStyle()->CreateDiagram();

	VERIFY(pSeries->SetDiagram(pDiagram));

	m_pPanels->Add(pDiagram);
}

void CXTPChartContent::UpdateDiagram()
{
	int i;
	int nIndex = 0;

	for (i = 0; i < GetSeries()->GetCount(); i++)
	{
		CXTPChartSeries* pSeries = GetSeries()->GetAt(i);
		if (!pSeries->IsVisible())
			continue;

		CXTPChartSeriesStyle *pStyle = pSeries->GetStyle();

		if (!pStyle)
			continue;

		if (pStyle->IsColorEach())
		{
			CXTPChartSeriesPointCollection *pPoints = pSeries->GetPoints();

			for (int j = 0; j < pPoints->GetCount(); j++)
			{
				pPoints->GetAt(j)->m_nPaletteIndex = nIndex++;
			}
		}
		else
		{
			pSeries->m_nPaletteIndex = nIndex++;
		}
	}
}


CXTPChartDeviceContext* CXTPChartContent::CreateDeviceContext(CXTPChartContainer* pContainer, HDC hDC, CRect rcBounds, BOOL bWindowDC)
{
	UNREFERENCED_PARAMETER(rcBounds);
	UNREFERENCED_PARAMETER(bWindowDC);

	UpdateDiagram();
	return new CXTPChartDeviceContext(pContainer, hDC);
}


CXTPChartDiagram* CXTPChartContent::GetPrimaryDiagram() const
{
	if (m_pPanels->GetCount() == 0)
		return NULL;
	return DYNAMIC_DOWNCAST(CXTPChartDiagram, m_pPanels->GetAt(0));
}

CXTPChartColor CXTPChartContent::GetActualBackgroundColor() const
{
	if (m_clrBackground.IsEmpty())
		return m_pAppearance->GetContentAppearance()->BackgroundColor;

	return m_clrBackground;
}


CXTPChartColor CXTPChartContent::GetActualBorderColor() const
{
	if (m_pBorder->GetColor().IsEmpty())
		return m_pAppearance->GetContentAppearance()->BorderColor;

	return m_pBorder->GetColor();
}

CXTPChartColor CXTPChartContent::GetBackgroundColor() const
{
	return m_clrBackground;
}

void CXTPChartContent::OnChartChanged(XTPChartUpdateOptions updateOptions /* = xtpChartUpdateView */)
{
	for (int i = 0; i < m_arrContainers.GetSize(); i++)
	{
		m_arrContainers[i]->OnChartChanged(updateOptions);
	}
}

void CXTPChartContent::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Int(pPX, _T("PanelDistance"), m_nPanelDistance, 10);
	PX_Enum(pPX, _T("PanelDirection"), m_nPanelDirection, xtpChartPanelHorizontal);
	PX_Color(pPX, _T("Background"), m_clrBackground);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secLegend(pPX->GetSection(_T("Legend")));
	m_pLegend->DoPropExchange(&secLegend);

	CXTPPropExchangeSection secTitles(pPX->GetSection(_T("Titles")));
	m_pTitles->DoPropExchange(&secTitles);

	CXTPPropExchangeSection secPanels(pPX->GetSection(_T("Panels")));
	m_pPanels->DoPropExchange(&secPanels);

	CXTPPropExchangeSection secSeries(pPX->GetSection(_T("Series")));
	m_pSeries->DoPropExchange(&secSeries);
}

void CXTPChartContent::EnableMarkup(BOOL bEnable)
{
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnable)
	{
		m_pMarkupContext = XTPMarkupCreateContext();
	}
}

void CXTPChartContent::AddContainer(CXTPChartContainer* pContainer)
{
	m_arrContainers.Add(pContainer);
}

void CXTPChartContent::RemoveContainer(CXTPChartContainer* pContainer)
{
	for (int i = 0; i < m_arrContainers.GetSize(); i++)
	{
		if (m_arrContainers.GetAt(i) == pContainer)
		{
			m_arrContainers.RemoveAt(i);
			return;
		}
	}
}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartContent, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartContent, "Series", 1, OleGetSeries, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartContent, "Titles", 2, OleGetTitles, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartContent, "Appearance", 3, OleGetAppearance, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartContent, "Legend", 4, OleGetLegend, VT_DISPATCH, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPChartContent, "EnableMarkup", 7, OleGetEnableMarkup, OleSetEnableMarkup, VT_BOOL)

	DISP_FUNCTION_ID(CXTPChartContent, "Diagrams", 8, OleGetDiagrams, VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPChartContent, "DoPropExchange", 34, OleDoPropExchange, VT_EMPTY, VTS_DISPATCH)


	DISP_PROPERTY_EX_ID(CXTPChartContent, "PanelDistance", 10, GetPanelDistance, SetPanelDistance, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartContent, "PanelDirection", 11, GetPanelDirection, SetPanelDirection, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartContent, "BackgroundColor", 12, OleGetBackgroundColor, OleSetBackgroundColor, VT_COLOR)

	DISP_PROPERTY_EX_ID(CXTPChartContent, "Border", 13, OleGetBorder, SetNotSupported, VT_DISPATCH)

END_DISPATCH_MAP()

// {E37BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartContent =
{ 0xe37bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartContent, CXTPChartElement)
	INTERFACE_PART(CXTPChartContent, IID_IChartContent, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartContent, IID_IChartContent)

// {AE910E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartContent, "Codejock.ChartContent." _XTP_AXLIB_VERSION,
						0xae910e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


LPDISPATCH CXTPChartContent::OleGetSeries()
{
	return XTPGetDispatch(GetSeries());
}

LPDISPATCH CXTPChartContent::OleGetDiagrams()
{
	return XTPGetDispatch(GetPanels());
}


LPDISPATCH CXTPChartContent::OleGetTitles()
{
	return XTPGetDispatch(GetTitles());
}

LPDISPATCH CXTPChartContent::OleGetAppearance()
{
	return XTPGetDispatch(GetAppearance());
}

LPDISPATCH CXTPChartContent::OleGetLegend()
{
	return XTPGetDispatch(GetLegend());
}

LPDISPATCH CXTPChartContent::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}

BOOL CXTPChartContent::OleGetEnableMarkup()
{
	return m_pMarkupContext != NULL;
}

void CXTPChartContent::OleSetEnableMarkup(BOOL bEnabled)
{
	EnableMarkup(bEnabled);
}


void CXTPChartContent::OleDoPropExchange(LPDISPATCH lpPropExchage)
{
	CXTPPropExchangeSection px(PropExchangeFromControl(lpPropExchage));

	if ((CXTPPropExchange*)&px == NULL)
		return;

	DoPropExchange(&px);
}

OLE_COLOR CXTPChartContent::OleGetBackgroundColor()
{
	return m_clrBackground.ToOleColor();
}

void CXTPChartContent::OleSetBackgroundColor(OLE_COLOR clr)
{
	SetBackgroundColor(CXTPChartColor::FromOleColor(clr));
}

COleControl* CXTPChartContent::GetOleControl()
{
	CWnd* pControl = (CXTPChartControl*)m_arrContainers[0];

	return AxGetParentOleControl(pControl);

}

#endif
