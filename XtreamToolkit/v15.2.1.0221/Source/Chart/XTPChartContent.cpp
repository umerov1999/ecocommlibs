// XTPChartContent.cpp
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
#include "Common/XTPMarkupRender.h"

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
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

		if (!pSeries->GetStyle())
			continue;

		if (pSeries->GetStyle()->IsColorEach())
		{
			for (int j = 0; j < pSeries->GetPoints()->GetCount(); j++)
			{
				pSeries->GetPoints()->GetAt(j)->m_nPaletteIndex = nIndex++;
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



