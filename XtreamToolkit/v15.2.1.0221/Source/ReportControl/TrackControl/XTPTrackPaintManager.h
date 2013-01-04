// XTPTrackPaintManager.h: interface for the CXTPTrackPaintManager class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPTRACKPAINTMANAGER_H__)
#define __XTPTRACKPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//-----------------------------------------------------------------------
// Summary:
//     This class represents a track control paint manager.This class does
//     painting of various elements of the track control.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackPaintManager : public CXTPReportPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPTrackPaintManager object.
	//-----------------------------------------------------------------------
	CXTPTrackPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackPaintManager destructor, does the cleanups.
	//-----------------------------------------------------------------------
	~CXTPTrackPaintManager();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//      Initializes all drawings defaults (fonts, colors, etc.).Most of
	//      them are system defaults.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Call this function to draw the track header.
	// Parameters:
	//      pDC - Pointer to the device context.
	// Remarks:
	//      It a virtual function, so the derived classes can give their
	//      specific implementations.
	//-----------------------------------------------------------------------
	virtual void DrawTrackHeader(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this function to draw the markers.
	// Parameters:
	//      pDC - Pointer to the device context.
	// Remarks:
	//      It a virtual function, so the derived classes can give their
	//      specific implementations.
	//-----------------------------------------------------------------------
	virtual void DrawMarkers(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this function to draw the time line.
	// Parameters:
	//      pDC - Pointer to the device context.
	// Remarks:
	//      It a virtual function, so the derived classes can give their
	//      specific implementations.
	//-----------------------------------------------------------------------
	virtual void DrawTimeLine(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//      Call this function to format the time and tick marker, relative to a time line
	//      position.
	// Parameters:
	//      nPosition - A point in the time line.
	//      clrTick   - Tick Mark color
	//      nTickLength - Tick Mark length
	// Returns:
	//      A CString which contains the formatted time.
	// Remarks:
	//      It a virtual function, so the derived classes can give their
	//      specific implementations.
	//-----------------------------------------------------------------------
	virtual CString FormatTimeLineTick(int nPosition, COLORREF& clrTick, int& nTickLength);
	virtual CString FormatTime(int nPosition);

public:
	COLORREF m_clrWorkArea;             //The color of the work area.
	COLORREF m_clrTrackHeader;          //The track header color.
	COLORREF m_clrTrackTimeArea;        //The track time area color.

	COLORREF m_clrTimeHeaderDarkDark;   //The header deep dark color.
	COLORREF m_clrTimeHeaderDark;       //The header dark color.
	COLORREF m_clrTimeSliderBackground; //The slider background color.
	COLORREF m_clrTimeHeaderDivider;    //The header divider color.

	COLORREF m_clrScrollBarLight;       //The scroll bar light color.
	COLORREF m_clrScrollBarDark;        //The scroll bar dark color.

	COLORREF m_clrSelectedArea;         //The color of the selected area.


	COLORREF m_clrMarker;               //The color of the marker.

	int m_nTimeLineStep;
	BOOL m_bTimeLineStepAuto;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTrackPaintManager);

#define DECLARE_PROPERTY_UN(theProperty, theType)\
	afx_msg theType OleGet##theProperty();\
	afx_msg void OleSet##theProperty(theType);

	DECLARE_PROPERTY_UN(WorkArea, COLORREF);
	DECLARE_PROPERTY_UN(TrackHeader, COLORREF);
	DECLARE_PROPERTY_UN(TrackTimeArea, COLORREF);
	DECLARE_PROPERTY_UN(TimeHeaderDarkDark, COLORREF);
	DECLARE_PROPERTY_UN(TimeHeaderDark, COLORREF);
	DECLARE_PROPERTY_UN(TimeSliderBackground, COLORREF);
	DECLARE_PROPERTY_UN(TimeHeaderDivider, COLORREF);
	DECLARE_PROPERTY_UN(ScrollBarLight, COLORREF);
	DECLARE_PROPERTY_UN(ScrollBarDark, COLORREF);
	DECLARE_PROPERTY_UN(Marker, COLORREF);
	DECLARE_PROPERTY_UN(SelectedArea, COLORREF);

	void OleSetTimeLineStep(int nStep);
	int OleGetTimeLineStep();
	BOOL m_bCustomTimeLineLabel;

//}}AFX_CODEJOCK_PRIVATE
#endif
};

#endif //#if !defined(__XTPTRACKPAINTMANAGER_H__)
