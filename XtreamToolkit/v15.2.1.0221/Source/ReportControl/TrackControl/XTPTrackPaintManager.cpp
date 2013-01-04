// XTPTrackControl.cpp : implementation of the CXTPTrackControl class.
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

#include "stdafx.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "../XTPReportDefines.h"
#include "../XTPReportColumn.h"
#include "../XTPReportPaintManager.h"
#include "../XTPReportControl.h"

#include "XTPTrackControl.h"
#include "XTPTrackPaintManager.h"
#include "XTPTrackBlock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPTrackPaintManager::CXTPTrackPaintManager()
{
	RefreshMetrics();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif

	m_nTimeLineStep = 1;
	m_bTimeLineStepAuto = TRUE;

#ifdef _XTP_ACTIVEX
	m_bCustomTimeLineLabel = FALSE;
#endif
}

CXTPTrackPaintManager::~CXTPTrackPaintManager()
{

}

void CXTPTrackPaintManager::RefreshMetrics()
{
	CXTPReportPaintManager::RefreshMetrics();

	m_clrWorkArea = RGB(245, 245, 245);

	m_clrTrackHeader = GetSysColor(COLOR_3DSHADOW);
	m_clrTrackTimeArea = GetSysColor(COLOR_3DFACE);

	m_clrTimeHeaderDarkDark = GetSysColor(COLOR_3DDKSHADOW);
	m_clrTimeHeaderDark = GetSysColor(COLOR_3DSHADOW);
	m_clrTimeSliderBackground = GetSysColor(COLOR_3DSHADOW);

	m_clrTimeHeaderDivider = GetSysColor(COLOR_3DDKSHADOW);

	m_clrScrollBarLight = GetSysColor(COLOR_WINDOW);
	m_clrScrollBarDark = GetSysColor(COLOR_3DFACE);

	m_clrMarker = GetSysColor(COLOR_3DFACE);

	m_clrSelectedArea = GetSysColor(COLOR_HIGHLIGHT);

}

#ifdef _XTP_ACTIVEX
COleControl* GetParentOleControl(CWnd* pWnd);
#endif

CString CXTPTrackPaintManager::FormatTime(int nPosition)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CString strCaption;
	strCaption.Format(pTrackControl->m_strTimeFormat, nPosition);

	return strCaption;

}

CString CXTPTrackPaintManager::FormatTimeLineTick(int nPosition, COLORREF& clrTick, int& nTickLength)
{
	CString strCaption = FormatTime(nPosition);

#ifdef _XTP_ACTIVEX
	if (m_bCustomTimeLineLabel)
	{
		BSTR bstrValue = strCaption.AllocSysString();

		COleControl* pControl = GetParentOleControl(m_pControl);
		ASSERT (pControl);

		pControl->FireEvent(1014, EVENT_PARAM(VTS_I4 VTS_PBSTR VTS_PCOLOR VTS_PI4), nPosition, &bstrValue, &clrTick, &nTickLength);

		strCaption = bstrValue;
		SysFreeString(bstrValue);
	}
#else
	UNREFERENCED_PARAMETER(clrTick);
	UNREFERENCED_PARAMETER(nTickLength);
#endif

	return strCaption;
}

void CXTPTrackPaintManager::DrawTimeLine(CDC* pDC)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea = pTrackControl->GetElementRect(xtpReportElementRectHeaderArea);

	CXTPReportColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	int nLeftOffset = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset, rcTimeLineArea.top + 10);
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.top);

	rcTimeArea.DeflateRect(7, 0);

	pDC->FillSolidRect(rcTimeArea, m_clrTrackTimeArea);

	int nTimeScaleMin = pTrackControl->GetTimeLineMin();
	int nTimeScaleMax = pTrackControl->GetTimeLineMax();

	int delta = pTrackControl->PositionToTrack(100) - pTrackControl->PositionToTrack(0);

	int dx = 1;

	if (m_bTimeLineStepAuto)
	{
		if (delta < 30)
			dx = 1000;
		else if (delta < 150)
			dx = 100;
		else if (delta < 300)
			dx = 20;
		else if (delta < 500)
			dx = 10;
		else if (delta < 2000)
			dx = 5;
		else
			dx = 1;

		m_nTimeLineStep = dx;
	}
	else
	{
		dx = m_nTimeLineStep;

	}

	CXTPFontDC dcFont(pDC, pTrackControl->GetPaintManager()->GetTextFont());
	pDC->SetTextColor(0);

	int nFirstPos = nTimeScaleMin / dx * dx;
	if (nFirstPos < nTimeScaleMin)
		nFirstPos += dx;

	for (int pos = nFirstPos; pos <= nTimeScaleMax; pos += dx)
	{
		int x = pTrackControl->PositionToTrack(pos);

		if (x < nLeftOffset - 20 || x > nRightOffset + 20)
			continue;

		COLORREF clrTick = RGB(35, 35, 35);
		int nTickLength = 8;

		CString strCaption = FormatTimeLineTick(pos, clrTick, nTickLength);

		pDC->FillSolidRect(x, rcTimeArea.bottom - min(4, nTickLength), 1, nTickLength, clrTick);

		int dx = pDC->GetTextExtent(strCaption).cx;

		pDC->DrawText(strCaption, CRect (x - dx / 2, rcTimeArea.top,x - dx / 2 + dx, rcTimeArea.bottom), DT_VCENTER | DT_SINGLELINE);
	}

}

void CXTPTrackPaintManager::DrawMarkers(CDC* pDC)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea = pTrackControl->GetElementRect(xtpReportElementRectHeaderArea);

	CXTPReportColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	int nLeftOffset = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset, rcTimeLineArea.top + 10);
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.bottom);
	CRect rcColumnArea(nLeftOffset, rcHeaderArea.top, nRightOffset, rcHeaderArea.bottom - 3);

	int nPos = pTrackControl->PositionToTrack(pTrackControl->GetTimeLinePosition());

	COLORREF clrDarkDark = RGB(43,90,127);

	CXTPPenDC pen (*pDC, clrDarkDark);
	CXTPBrushDC brush (*pDC, RGB(153,192,231));

	if (pTrackControl->m_bShowTimeLinePosition)
	{
		int X = nPos;
		//int Y = rcColumnArea.top + 7;
		int Y = rcColumnArea.top - 6;
		CPoint pts[] = {CPoint(X - 5, Y), CPoint(X, Y + 5), CPoint(X + 5, Y), CPoint(X + 5, Y - 10), CPoint(X - 5, Y - 10)};

		pDC->Polygon(pts, 5);
	}


	pen.Color(0);
	brush.Color(m_clrMarker);

	CXTPFontDC dcFont(pDC, pTrackControl->GetPaintManager()->GetTextFont());
	pDC->SetTextColor(0);

	for (int i = 0; i < pTrackControl->GetMarkers()->GetCount(); i++)
	{
		CXTPTrackMarker* pMarker = pTrackControl->GetMarkers()->GetAt(i);

		int nPos = pTrackControl->PositionToTrack(pMarker->GetPosition());

		CString strCaption = pMarker->GetCaption();
		if (!strCaption.IsEmpty())
		{
			int dx = pDC->GetTextExtent(strCaption).cx;

			pDC->FillSolidRect(CRect (nPos, rcColumnArea.top + 2, nPos + 12 + dx, rcColumnArea.bottom - 2), RGB(230, 230, 230));
			pDC->Draw3dRect(CRect (nPos, rcColumnArea.top + 2, nPos + 12 + dx, rcColumnArea.bottom - 2), RGB(123, 123, 123), RGB(123, 123, 123));

			pDC->DrawText(strCaption, CRect (nPos + 8, rcColumnArea.top, nPos + 8 + dx, rcColumnArea.bottom), DT_VCENTER | DT_SINGLELINE);

		}

		int X = nPos;
		int Y = rcHeaderArea.bottom - 10;

		CPoint pts[] = {CPoint(X - 5, Y), CPoint(X, Y - 5), CPoint(X + 5, Y), CPoint(X + 5, Y + 5), CPoint(X - 5, Y + 5)};
		pDC->Polygon(pts, 5);

		pMarker->m_rcMarker = CRect(nPos - 5, rcHeaderArea.bottom - 20, nPos + 5, rcHeaderArea.bottom - 5);
	}

	if (pTrackControl->m_bShowTimeLinePosition)
	{
		pDC->FillSolidRect(CRect(nPos, rcColumnArea.top, nPos + 1, rcHeaderArea.bottom), RGB(205, 0, 0));
	}

}


void CXTPTrackPaintManager::DrawTrackHeader(CDC* pDC)
{
	CXTPTrackControl* pTrackControl = (CXTPTrackControl*)m_pControl;

	CXTPReportColumn* pTrackColumn = pTrackControl->GetTrackColumn();
	if (!pTrackColumn || !pTrackColumn->IsVisible())
		return;


	CRect rcTimeLineArea = pTrackControl->GetTimelineArea();
	CRect rcHeaderArea = pTrackControl->GetElementRect(xtpReportElementRectHeaderArea);

	pDC->FillSolidRect(rcTimeLineArea, m_clrTrackHeader);

	int nLeftOffset = pTrackColumn->GetRect().left;
	int nRightOffset = pTrackColumn->GetRect().right;

	CRect rcSliderArea(nLeftOffset, rcTimeLineArea.top, nRightOffset, rcTimeLineArea.top + 10);
	CRect rcTimeArea(nLeftOffset, rcSliderArea.bottom, nRightOffset, rcHeaderArea.bottom);
	CRect rcColumnArea(nLeftOffset, rcHeaderArea.top, nRightOffset, rcHeaderArea.bottom - 3);


	pDC->FillSolidRect(nLeftOffset, rcTimeLineArea.top, nRightOffset - nLeftOffset, rcTimeLineArea.Height() + rcHeaderArea.Height(), m_clrTimeHeaderDarkDark);
	pDC->FillSolidRect(nLeftOffset - 1, rcTimeLineArea.top, 1, rcTimeLineArea.Height() + rcHeaderArea.Height(), m_clrTimeHeaderDivider);


	pDC->FillSolidRect(nLeftOffset + 7, rcColumnArea.top, nRightOffset - nLeftOffset - 14, rcColumnArea.Height(), m_clrTimeHeaderDark);
	pDC->FillSolidRect(nLeftOffset, rcColumnArea.bottom - 1, nRightOffset - nLeftOffset, 1, RGB(35, 35, 35));


	pDC->FillSolidRect(nLeftOffset + 7, rcSliderArea.top, nRightOffset - nLeftOffset - 14, rcSliderArea.Height(), m_clrTimeSliderBackground);


	pDC->FillSolidRect(nLeftOffset, rcSliderArea.bottom, 7, rcColumnArea.top - rcSliderArea.bottom, m_clrTimeHeaderDark);
	pDC->FillSolidRect(nRightOffset - 7, rcSliderArea.bottom, 7, rcColumnArea.top - rcSliderArea.bottom, m_clrTimeHeaderDark);

	CRect rcClipArea(nLeftOffset, rcTimeLineArea.top, nRightOffset, rcHeaderArea.bottom);
	pDC->IntersectClipRect(rcClipArea);


// TimeLine
	DrawTimeLine(pDC);

// Work ScrollBar

	if (pTrackControl->m_bShowWorkArea)
	{
		int nWorkAreaMin = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMin());
		int nWorkAreaMax = pTrackControl->PositionToTrack(pTrackControl->GetWorkAreaMax());


		CRect rcWorkAreaScrollBar(nWorkAreaMin, rcColumnArea.top, nWorkAreaMax, rcColumnArea.bottom);

		XTPDrawHelpers()->GradientFill(pDC, rcWorkAreaScrollBar, m_clrScrollBarLight, m_clrScrollBarDark, FALSE);
		pDC->Draw3dRect(rcWorkAreaScrollBar.left - 1, rcWorkAreaScrollBar.top, rcWorkAreaScrollBar.Width() + 2, rcWorkAreaScrollBar.Height(), 0, RGB(155, 155, 155));

		XTPDrawHelpers()->GradientFill(pDC, CRect(rcWorkAreaScrollBar.left - 7, rcWorkAreaScrollBar.top, rcWorkAreaScrollBar.left + 0, rcWorkAreaScrollBar.bottom),
			RGB(153,192,231), RGB(27,102,157), FALSE);

		pDC->Draw3dRect(CRect(rcWorkAreaScrollBar.left - 7, rcWorkAreaScrollBar.top, rcWorkAreaScrollBar.left + 0, rcWorkAreaScrollBar.bottom),
			RGB(43,90,127), RGB(43,90,127));

		XTPDrawHelpers()->GradientFill(pDC, CRect(rcWorkAreaScrollBar.right - 0, rcWorkAreaScrollBar.top, rcWorkAreaScrollBar.right + 7, rcWorkAreaScrollBar.bottom),
			RGB(153,192,231), RGB(27,102,157), FALSE);

		pDC->Draw3dRect(CRect(rcWorkAreaScrollBar.right - 0, rcWorkAreaScrollBar.top, rcWorkAreaScrollBar.right + 7, rcWorkAreaScrollBar.bottom),
			RGB(43,90,127), RGB(43,90,127));
	}


// Zoom Slider


	rcSliderArea.DeflateRect(7, 0);


	int nZoomAreaMin = rcSliderArea.left +
		MulDiv(pTrackControl->GetViewPortMin() - pTrackControl->GetTimeLineMin(), rcSliderArea.Width(), (pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin()));
	int nZoomAreaMax = rcSliderArea.left +
		MulDiv(pTrackControl->GetViewPortMax() - pTrackControl->GetTimeLineMin(), rcSliderArea.Width(), (pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin()));

	CRect rcZoomSlider(nZoomAreaMin, rcSliderArea.top, nZoomAreaMax, rcSliderArea.bottom);

	XTPDrawHelpers()->GradientFill(pDC, rcZoomSlider, m_clrScrollBarLight, m_clrScrollBarDark, FALSE);
	pDC->Draw3dRect(rcZoomSlider.left - 1, rcZoomSlider.top, rcZoomSlider.Width() + 2, rcZoomSlider.Height(), 0, 0);

	XTPDrawHelpers()->GradientFill(pDC, CRect(rcZoomSlider.left - 7, rcZoomSlider.top, rcZoomSlider.left + 0, rcZoomSlider.bottom),
		RGB(153,192,231), RGB(27,102,157), FALSE);

	pDC->Draw3dRect(CRect(rcZoomSlider.left - 7, rcZoomSlider.top, rcZoomSlider.left + 0, rcZoomSlider.bottom),
		RGB(43,90,127), RGB(43,90,127));

	XTPDrawHelpers()->GradientFill(pDC, CRect(rcZoomSlider.right - 0, rcZoomSlider.top, rcZoomSlider.right + 7, rcZoomSlider.bottom),
		RGB(153,192,231), RGB(27,102,157), FALSE);

	pDC->Draw3dRect(CRect(rcZoomSlider.right - 0, rcZoomSlider.top, rcZoomSlider.right + 7, rcZoomSlider.bottom),
		RGB(43,90,127), RGB(43,90,127));

	if (pTrackControl->m_bShowTimeLinePosition)
	{
		int nZoomAreaTimeLinePos = rcSliderArea.left +
			MulDiv(pTrackControl->GetTimeLinePosition() - pTrackControl->GetTimeLineMin(), rcSliderArea.Width(), (pTrackControl->GetTimeLineMax() - pTrackControl->GetTimeLineMin()));
		pDC->FillSolidRect(CRect(nZoomAreaTimeLinePos, rcZoomSlider.top, nZoomAreaTimeLinePos + 1, rcZoomSlider.bottom), RGB(205, 0, 0));
	}



	DrawMarkers(pDC);

	pDC->SelectClipRgn(0);
}

#ifdef _XTP_ACTIVEX

#define DISP_PROPERTY_UN_ID(theClass, szExternalName, dispid, vtPropType)\
	DISP_PROPERTY_EX_ID(theClass, #szExternalName, dispid, OleGet##szExternalName, OleSet##szExternalName, vtPropType)

BEGIN_DISPATCH_MAP(CXTPTrackPaintManager, CXTPReportPaintManager)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, WorkArea, 1000, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TrackHeader, 1001, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TrackTimeArea, 1002, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TimeHeaderDarkDark, 1003, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TimeHeaderDark, 1004, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TimeSliderBackground, 1005, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, TimeHeaderDivider, 1006, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, ScrollBarLight, 1007, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, ScrollBarDark, 1008, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, Marker, 1009, VT_COLOR)
	DISP_PROPERTY_UN_ID(CXTPTrackPaintManager, SelectedArea, 1010, VT_COLOR)

	DISP_PROPERTY_EX_ID(CXTPTrackPaintManager, "TimeLineStep", 1112, OleGetTimeLineStep, OleSetTimeLineStep, VT_I4)
	DISP_PROPERTY_ID(CXTPTrackPaintManager, "TimeLineStepAuto", 1113, m_bTimeLineStepAuto, VT_BOOL)
	DISP_PROPERTY_ID(CXTPTrackPaintManager, "CustomTimeLineLabel", 1114, m_bCustomTimeLineLabel, VT_BOOL)

END_DISPATCH_MAP()


// {A5989CE5-E015-4f87-885D-DE3326A63BDE}
static const GUID IID_ITrackPaintManager =
{ 0xA5989ce5, 0xe015, 0x4f87, { 0x88, 0x5d, 0xde, 0x33, 0x26, 0xa6, 0x3b, 0xde } };

BEGIN_INTERFACE_MAP(CXTPTrackPaintManager, CXTPReportPaintManager)
	INTERFACE_PART(CXTPTrackPaintManager, IID_ITrackPaintManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTrackPaintManager, IID_ITrackPaintManager)

#define IMPLEMENT_PROPERTY_CLR(theClass, theProperty, theMember)\
	COLORREF theClass::OleGet##theProperty()\
	{\
		return theMember;\
	}\
	void theClass::OleSet##theProperty(COLORREF oleColor)\
	{\
		theMember = AxTranslateColor(oleColor);\
	}

IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, WorkArea, m_clrWorkArea)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TrackHeader, m_clrTrackHeader)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TrackTimeArea, m_clrTrackTimeArea)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TimeHeaderDarkDark, m_clrTimeHeaderDarkDark)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TimeHeaderDark, m_clrTimeHeaderDark)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TimeSliderBackground, m_clrTimeSliderBackground)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, TimeHeaderDivider, m_clrTimeHeaderDivider)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, ScrollBarLight, m_clrScrollBarLight)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, ScrollBarDark, m_clrScrollBarDark)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, Marker, m_clrMarker)
IMPLEMENT_PROPERTY_CLR(CXTPTrackPaintManager, SelectedArea, m_clrSelectedArea)


void CXTPTrackPaintManager::OleSetTimeLineStep(int nStep)
{
	m_nTimeLineStep = nStep;
	if (m_pControl) m_pControl->RedrawControl();
}

int CXTPTrackPaintManager::OleGetTimeLineStep()
{
	return m_nTimeLineStep;

}

#endif
