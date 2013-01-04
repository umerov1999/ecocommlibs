// XTPChartTransformationDeviceCommand.cpp
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
#include <math.h>

#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "../Utils/XTPChartMathUtils.h"

#include "XTPChartDeviceCommand.h"
#include "XTPChartTransformationDeviceCommand.h"
#include "XTPChartDeviceContext.h"


using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPChartRotateDeviceCommand

CXTPChartRotateDeviceCommand::CXTPChartRotateDeviceCommand(float fAngle)
{
	m_fAngle = fAngle;
	m_ptRotateVector = CXTPPoint3d(0, 0, 1);
}

CXTPChartRotateDeviceCommand::CXTPChartRotateDeviceCommand(float fAngle, const CXTPPoint3d& rotateVector)
{
	m_fAngle = fAngle;
	m_ptRotateVector = rotateVector;
}

void CXTPChartRotateDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GdipRotateWorldTransform(pDC->GetGraphics(), m_fAngle, MatrixOrderPrepend);
}

CXTPChartElement* CXTPChartRotateDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	double angle = m_fAngle * CXTPChartMathUtils::m_dPI / 180.0;

	REAL x = (REAL)(point.x * cos(angle) - point.y * sin(angle));
	REAL y =  (REAL)(point.x * sin(angle) + point.y * cos(angle));

	return CXTPChartDeviceCommand::HitTest(CPoint((int)x, (int)y), pParent);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSaveStateDeviceCommand

CXTPChartSaveStateDeviceCommand::CXTPChartSaveStateDeviceCommand()
{
	m_nState = 0;
}


void CXTPChartSaveStateDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	GdipSaveGraphics(pDC->GetGraphics(), &m_nState);
}

void CXTPChartSaveStateDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	GdipRestoreGraphics(pDC->GetGraphics(), m_nState);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartTranslateDeviceCommand

CXTPChartTranslateDeviceCommand::CXTPChartTranslateDeviceCommand(double dx, double dy, double dz)
{
	m_dx = dx;
	m_dz = dz;
	m_dy = dy;
}

void CXTPChartTranslateDeviceCommand::ExecuteOverride(CXTPChartDeviceContext* pDC)
{
	GdipTranslateWorldTransform(pDC->GetGraphics(), (float)m_dx, (float)m_dy, MatrixOrderPrepend);
}

CXTPChartElement* CXTPChartTranslateDeviceCommand::HitTest(CPoint point, CXTPChartElement* pParent) const
{
	return CXTPChartDeviceCommand::HitTest(CPoint(int(point.x - m_dx), int(point.y - m_dy)), pParent);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartClipDeviceCommand

CXTPChartClipDeviceCommand::CXTPChartClipDeviceCommand(CRect rcClip)
{
	m_rcClip = rcClip;
}

void CXTPChartClipDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	GdipGetClipBoundsI(pDC->GetGraphics(), (GpRect*)&m_rcState);

	GdipSetClipRectI(pDC->GetGraphics(), m_rcClip.left, m_rcClip.top, m_rcClip.Width(), m_rcClip.Height(), CombineModeIntersect);
}

void CXTPChartClipDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	GdipSetClipRectI(pDC->GetGraphics(), m_rcState.left, m_rcState.top, m_rcState.Width(), m_rcState.Width(), CombineModeReplace);

}

//////////////////////////////////////////////////////////////////////////
// CXTPChartClipRegioDeviceCommand


CXTPChartClipRegionDeviceCommand::CXTPChartClipRegionDeviceCommand(const CXTPChartPoints& points)
{
	GpPath* pGpPath = NULL;
	GdipCreatePath(FillModeAlternate, &pGpPath);
	GdipAddPathPolygon(pGpPath, (GpPointF*)points.GetData(), (int)points.GetSize());

	m_pGpClip = NULL;
	GdipCreateRegionPath(pGpPath, &m_pGpClip);

	m_pGpState = NULL;
	GdipCreateRegion(&m_pGpState);

	GdipDeletePath(pGpPath);
}

CXTPChartClipRegionDeviceCommand::~CXTPChartClipRegionDeviceCommand()
{
	GdipDeleteRegion(m_pGpClip);
	GdipDeleteRegion(m_pGpState);
}

void CXTPChartClipRegionDeviceCommand::BeforeExecute(CXTPChartDeviceContext* pDC)
{
	GdipGetClip(pDC->GetGraphics(), m_pGpState);

	GdipSetClipRegion(pDC->GetGraphics(), m_pGpClip, CombineModeExclude);
}

void CXTPChartClipRegionDeviceCommand::AfterExecute(CXTPChartDeviceContext* pDC)
{
	GdipSetClipRegion(pDC->GetGraphics(), m_pGpState, CombineModeReplace);
}
