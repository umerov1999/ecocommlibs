// XTPChartElementView.cpp
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

#include "Types/XTPChartTypes.h"
#include "XTPChartElementView.h"
#include "Drawing/XTPChartDeviceCommand.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartElementView

CXTPChartElementView::CXTPChartElementView(CXTPChartElementView* pParentView)
{
	m_pParentView = pParentView;
	ASSERT (pParentView);

	if (pParentView)
	{
		pParentView->m_arrChildren.Add(this);
	}

	m_pContainer = m_pParentView->m_pContainer;
}

CXTPChartElementView::CXTPChartElementView(CXTPChartContainer* pContainer)
{
	m_pParentView = NULL;

	m_pContainer = pContainer;
}

CXTPChartElementView::~CXTPChartElementView()
{
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
		delete m_arrChildren[i];

	m_arrChildren.RemoveAll();
}

CXTPChartElementView* CXTPChartElementView::AddChildView(CXTPChartElementView* pChild)
{
	if (pChild)
	{
		m_arrChildren.Add(pChild);
		pChild->m_pParentView =  this;
	}
	return pChild;
}

CXTPChartDeviceCommand* CXTPChartElementView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		pCommand->AddChildCommand(m_arrChildren[i]->CreateDeviceCommand(pDC));
	}


	return pCommand;
}

void CXTPChartElementView::Release()
{
	delete this;
}

void CXTPChartElementView::OnMouseMove(UINT /*nFlags*/, CPoint /*point*/)
{

}

void CXTPChartElementView::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{

}

BOOL CXTPChartElementView::OnSetCursor(CPoint /*point*/)
{
	return FALSE;
}
