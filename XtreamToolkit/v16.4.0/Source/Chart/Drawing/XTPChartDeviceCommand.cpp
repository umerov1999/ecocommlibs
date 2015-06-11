// XTPChartDeviceCommand.cpp
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

#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"

#include "XTPChartDeviceCommand.h"
#include "XTPChartDeviceContext.h"

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartDeviceCommand

CXTPChartDeviceCommand::CXTPChartDeviceCommand()
{

}

CXTPChartDeviceCommand::~CXTPChartDeviceCommand()
{
	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		delete m_arrChildren[i];
	}

	m_arrChildren.RemoveAll();
}

CXTPChartDeviceCommand* CXTPChartDeviceCommand::AddChildCommand(CXTPChartDeviceCommand* pCommand)
{
	if (pCommand)
	{
		m_arrChildren.Add(pCommand);
	}
	return pCommand;
}

void CXTPChartDeviceCommand::Execute(CXTPChartDeviceContext* pDC)
{
	BeforeExecute(pDC);
	ExecuteOverride(pDC);

	for (int i = 0; i < m_arrChildren.GetSize(); i++)
	{
		m_arrChildren[i]->Execute(pDC);
	}

	AfterExecute(pDC);
}

void CXTPChartDeviceCommand::BeforeExecute(CXTPChartDeviceContext* /*pDC*/)
{

}

void CXTPChartDeviceCommand::AfterExecute(CXTPChartDeviceContext* /*pDC*/)
{

}

void CXTPChartDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* /*pDC*/)
{

}

CXTPChartElement* CXTPChartDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	for (int i = (int)m_arrChildren.GetUpperBound(); i >= 0; i--)
	{
		CXTPChartElement* pElement = m_arrChildren[i]->HitTest(point, pParent);
		if (pElement)
			return pElement;
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartHitTestElementCommand

CXTPChartHitTestElementCommand::CXTPChartHitTestElementCommand(CXTPChartElement* pElement)
{
	m_pElement = pElement;
	m_rcBounds.SetRectEmpty();
}

CXTPChartHitTestElementCommand::CXTPChartHitTestElementCommand(CXTPChartElement* pElement, const CRect& rcBounds)
{
	m_pElement = pElement;
	m_rcBounds = rcBounds;
}

CXTPChartHitTestElementCommand::CXTPChartHitTestElementCommand(CXTPChartElement* pElement, const CXTPChartRectF& rcBounds)
{
	m_pElement = pElement;

	m_rcBounds.left = (int)rcBounds.GetLeft();
	m_rcBounds.right = (int)rcBounds.GetRight();
	m_rcBounds.top = (int)rcBounds.GetTop();
	m_rcBounds.bottom = (int)rcBounds.GetBottom();
}

CXTPChartElement* CXTPChartHitTestElementCommand::HitTest(CPoint point, CXTPChartElement* /*pParent*/) const
{
	if (!m_rcBounds.IsRectEmpty())
	{
		if (!m_rcBounds.PtInRect(point))
			return NULL;
	}

	for (int i = (int)m_arrChildren.GetSize() - 1; i >= 0; i--)
	{
		CXTPChartElement* pElement = m_arrChildren[i]->HitTest(point, m_pElement);
		if (pElement)
			return pElement;
	}
	return NULL;
}
