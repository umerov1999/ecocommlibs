// XTPReportThickness.cpp: interface for the CXTPReportThickness class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "Common/XTPMacros.h"
#include "XTPReportThickness.h"


CXTPReportThickness::CXTPReportThickness()
	: m_nLeft  (0)
	, m_nTop   (0)
	, m_nRight (0)
	, m_nBottom(0)
{
	// Nothing
}

CXTPReportThickness::CXTPReportThickness(const CXTPReportThickness &thickness)
	: m_nLeft  (thickness.m_nLeft)
	, m_nTop   (thickness.m_nTop)
	, m_nRight (thickness.m_nRight)
	, m_nBottom(thickness.m_nBottom)
{
	// Nothing
}

CXTPReportThickness::CXTPReportThickness(const int nThickness)
	: m_nLeft  (nThickness)
	, m_nTop   (nThickness)
	, m_nRight (nThickness)
	, m_nBottom(nThickness)
{
	// Nothing
}

CXTPReportThickness::CXTPReportThickness(
	const int nLeft,
	const int nTop,
	const int nRight,
	const int nBottom)
	: m_nLeft  (nLeft)
	, m_nTop   (nTop)
	, m_nRight (nRight)
	, m_nBottom(nBottom)
{
	// Nothing
}

void CXTPReportThickness::Set(const CXTPReportThickness &thickness)
{
	m_nLeft   = thickness.m_nLeft;
	m_nTop    = thickness.m_nTop;
	m_nRight  = thickness.m_nRight;
	m_nBottom = thickness.m_nBottom;
}

void CXTPReportThickness::Set(const int nThickness)
{
	m_nLeft   = nThickness;
	m_nTop    = nThickness;
	m_nRight  = nThickness;
	m_nBottom = nThickness;
}

void CXTPReportThickness::Set(
	const int nLeft,
	const int nTop,
	const int nRight,
	const int nBottom)
{
	m_nLeft   = nLeft;
	m_nTop    = nTop;
	m_nRight  = nRight;
	m_nBottom = nBottom;
}

int CXTPReportThickness::GetLeft() const
{
	return m_nLeft;
}

void CXTPReportThickness::SetLeft(const int nThickness)
{
	m_nLeft = nThickness;
}

int CXTPReportThickness::GetTop() const
{
	return m_nTop;
}

void CXTPReportThickness::SetTop(const int nThickness)
{
	m_nTop = nThickness;
}

int CXTPReportThickness::GetRight() const
{
	return m_nRight;
}

void CXTPReportThickness::SetRight(const int nThickness)
{
	m_nRight = nThickness;
}

int CXTPReportThickness::GetBottom() const
{
	return m_nBottom;
}

void CXTPReportThickness::SetBottom(const int nThickness)
{
	m_nBottom = nThickness;
}

CSize CXTPReportThickness::GetSize() const
{
	return CSize(m_nLeft + m_nRight, m_nTop + m_nBottom);
}

CRect CXTPReportThickness::DeflateRect(CRect rcDeflate, const CXTPReportThickness *pThickness)
{
	if (NULL != pThickness)
	{
		rcDeflate.left   += pThickness->GetLeft();
		rcDeflate.top    += pThickness->GetTop();
		rcDeflate.right  -= pThickness->GetRight();
		rcDeflate.bottom -= pThickness->GetBottom();
	}

	return rcDeflate;
}

CXTPReportThickness& CXTPReportThickness::operator = (const CXTPReportThickness &thickness)
{
	Set(thickness);
	return *this;
}


CXTPReportThickness& CXTPReportThickness::operator = (const int nThickness)
{
	Set(nThickness);
	return *this;
}
