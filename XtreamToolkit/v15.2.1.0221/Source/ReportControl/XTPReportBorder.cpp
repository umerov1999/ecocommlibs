// XTPReportBorder.cpp : implementation of the CXTPReportBorder class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "StdAfx.h"

#include "Common/XTPPropExchange.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPReportDefines.h"
#include "XTPReportThickness.h"
#include "XTPReportBorder.h"


CXTPReportBorderEdge::CXTPReportBorderEdge()
	: m_lineStyle(xtpReportLineStyleNone)
{
	// Nothing
}

XTPReportLineStyle CXTPReportBorderEdge::GetLineStyle() const
{
	return m_lineStyle;
}

void CXTPReportBorderEdge::SetLineStyle(XTPReportLineStyle lineStyle)
{
	m_lineStyle = lineStyle;
}



CXTPReportBorder::CXTPReportBorder()
	: m_crBorder        (RGB(0,0,0))
	, m_crBackground    (XTP_REPORT_COLOR_DEFAULT)

	, m_pMargin         (NULL)
	, m_pBorderThickness(NULL)
	, m_pPadding        (NULL)
{
	m_pMargin          = new CXTPReportThickness();
	m_pBorderThickness = new CXTPReportThickness();
	m_pPadding         = new CXTPReportThickness();

	// Instantiate edges
	m_pEdgeLeft   = new CXTPReportBorderEdge();
	m_pEdgeTop    = new CXTPReportBorderEdge();
	m_pEdgeRight  = new CXTPReportBorderEdge();
	m_pEdgeBottom = new CXTPReportBorderEdge();
}

CXTPReportBorder::~CXTPReportBorder()
{
	SAFE_DELETE(m_pMargin);
	SAFE_DELETE(m_pBorderThickness);
	SAFE_DELETE(m_pPadding);

	// Delete edges
	SAFE_DELETE(m_pEdgeLeft);
	SAFE_DELETE(m_pEdgeTop);
	SAFE_DELETE(m_pEdgeRight);
	SAFE_DELETE(m_pEdgeBottom);
}

void CXTPReportBorder::Draw(CDC *pDC, CRect rcBorder)
{
	DrawH(pDC, rcBorder);
	DrawV(pDC, rcBorder);
}

void CXTPReportBorder::DrawH(CDC *pDC, CRect rcBorder)
{
	if (rcBorder.bottom < rcBorder.top)
	{
		return;
	}

	rcBorder = CXTPReportThickness::DeflateRect(rcBorder, GetMargin());

	CRect rcTop   (rcBorder.left, rcBorder.top, rcBorder.right, rcBorder.top + m_pBorderThickness->GetTop());
	CRect rcBottom(rcBorder.left, rcBorder.bottom - m_pBorderThickness->GetBottom(), rcBorder.right, rcBorder.bottom);

	CBrush brush(m_crBorder);
	pDC->FillRect(rcTop,    &brush);
	pDC->FillRect(rcBottom, &brush);
}

void CXTPReportBorder::DrawV(CDC *pDC, CRect rcBorder)
{
	if (rcBorder.bottom < rcBorder.top)
	{
		return;
	}

	rcBorder = CXTPReportThickness::DeflateRect(rcBorder, GetMargin());

	CRect rcLeft  (rcBorder.left, rcBorder.top, rcBorder.left + m_pBorderThickness->GetLeft(), rcBorder.bottom);
	CRect rcRight (rcBorder.right - m_pBorderThickness->GetRight(), rcBorder.top, rcBorder.right, rcBorder.bottom);

	CBrush brush(m_crBorder);
	pDC->FillRect(rcLeft,   &brush);
	pDC->FillRect(rcRight,  &brush);
}

CXTPReportBorderEdge* CXTPReportBorder::GetEdge(const XTPReportBorderEdge edge) const
{
	CXTPReportBorderEdge *pEdge = NULL;

	switch(edge)
	{
	case xtpReportBorderEdgeLeft:
		pEdge = m_pEdgeLeft;
		break;
	case xtpReportBorderEdgeTop:
		pEdge = m_pEdgeTop;
		break;
	case xtpReportBorderEdgeRight:
		pEdge = m_pEdgeRight;
		break;
	case xtpReportBorderEdgeBottom:
		pEdge = m_pEdgeBottom;
		break;
	}

	return pEdge;
}

COLORREF CXTPReportBorder::GetBorderColor() const
{
	return m_crBorder;
}

void CXTPReportBorder::SetBorderColor(COLORREF crBorder)
{
	m_crBorder = crBorder;
}

CSize CXTPReportBorder::GetSize() const
{
	return GetMargin()->GetSize() + GetBorderThickness()->GetSize() + GetPadding()->GetSize();
}

CXTPReportThickness* CXTPReportBorder::GetMargin() const
{
	return m_pMargin;
}

CXTPReportThickness* CXTPReportBorder::GetBorderThickness() const
{
	return m_pBorderThickness;
}

CXTPReportThickness* CXTPReportBorder::GetPadding() const
{
	return m_pPadding;
}

CRect CXTPReportBorder::DeflateRect(CRect rcDeflate, const CXTPReportBorder *pBorder)
{
	rcDeflate = CXTPReportThickness::DeflateRect(rcDeflate, pBorder->GetMargin());
	rcDeflate = CXTPReportThickness::DeflateRect(rcDeflate, pBorder->GetBorderThickness());
	rcDeflate = CXTPReportThickness::DeflateRect(rcDeflate, pBorder->GetPadding());
	return rcDeflate;
}
