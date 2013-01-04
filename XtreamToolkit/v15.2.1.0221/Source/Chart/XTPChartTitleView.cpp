// XTPChartTitleView.cpp
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
#include "XTPChartTitle.h"
#include "XTPChartElementView.h"
#include "XTPChartTitleView.h"

#include "Utils/XTPChartTextPainter.h"
#include "Drawing/XTPChartDeviceCommand.h"
#include "Appearance/XTPChartAppearance.h"


CXTPChartTitleView::CXTPChartTitleView(CXTPChartTitle* pTitle, CXTPChartString& strText, CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_strText = strText;
	m_pTitle = pTitle;
	m_ptOrigin = CPoint(0, 0);
}

CXTPChartTitleView::~CXTPChartTitleView()
{

}

CXTPChartDeviceCommand* CXTPChartTitleView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pTitle);

	CXTPChartRotatedTextPainterNearLine painter(pDC, m_strText, m_pTitle, m_ptOrigin, m_pTitle->GetNearTextPosition(), (float)m_pTitle->GetTextAngle());

	CXTPChartDeviceCommand* pTextCommand =  painter.CreateDeviceCommand(pDC, m_pTitle->GetActualTextColor());

	pCommand->AddChildCommand(pTextCommand);

	return pCommand;
}

void CXTPChartTitleView::CalculateView(CXTPChartDeviceContext* pDC, CRect& rcBounds, CRect rcChart)
{

	CXTPChartString strText = m_strText;

	CXTPChartRotatedTextPainterNearLine painter(pDC, strText, m_pTitle, CPoint(0, 0) , m_pTitle->GetNearTextPosition(), (float)m_pTitle->GetTextAngle());
	CXTPChartSizeF sizeString = painter.GetSize();

	CRect rect = painter.GetRoundedBounds();

	XTPChartDocking nDocking = m_pTitle->GetDocking();

	int size = (nDocking == xtpChartDockLeft || nDocking == xtpChartDockRight ? rect.Width() : rect.Height());

	CPoint origin = m_pTitle->CalcOrigin(rcBounds, rcChart, rect);
	rect.OffsetRect(origin);


	m_ptOrigin = origin;

	if (nDocking == xtpChartDockLeft) rcBounds.left += size + m_pTitle->GetIndent();
	else if (nDocking == xtpChartDockRight) rcBounds.right -= size + m_pTitle->GetIndent();
	else if (nDocking == xtpChartDockTop) rcBounds.top += size + m_pTitle->GetIndent();
	else if (nDocking == xtpChartDockBottom) rcBounds.bottom -= size + m_pTitle->GetIndent();
}
