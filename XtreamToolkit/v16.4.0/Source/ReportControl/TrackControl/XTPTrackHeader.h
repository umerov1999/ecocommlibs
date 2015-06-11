// XTPTrackHeader.h: interface for the CXTPTrackHeader class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPTRACKEADER_H__)
#define __XTPTRACKEADER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//-----------------------------------------------------------------------
// Summary:
//     This class represents a track control header.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackHeader : public CXTPReportHeader
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPTrackHeader object.
	// Parameters:
	//     pControl - Pointer to the report control.
	//     pColumns - Pointer to the report columns.
	//-----------------------------------------------------------------------
	CXTPTrackHeader(CXTPReportControl* pControl, CXTPReportColumns* pColumns);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackHeader default destructor
	//-----------------------------------------------------------------------
	~CXTPTrackHeader();
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the block.
	// Parameters:
	//     pDC         - The pointer to the device context.
	//     rcHeader    - The header rectangle.
	//     nLeftOffset - The left offset.
	// Remarks:
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC, CRect rcHeader, int nLeftOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the mouse left button is clicked on
	//     the header.
	// Parameters:
	//     ptClick   - The point where the mouse event occurred
	// Remarks:
	//-----------------------------------------------------------------------
	void OnLButtonDown(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the mouse is moved over the header.
	// Parameters:
	//     nFlags   - Tells the state of mouse buttons, while the mouse is moved.
	//     point    - The point where the mouse event occurred
	// Remarks:
	//-----------------------------------------------------------------------
	void OnMouseMove(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the scroll bar about to drag/resize.
	// Parameters:
	//     bResize  - Tells the whether the scrollbar is about to resize, a
	//                TRUE value hints the resizing and FALSE simple dragging.
	// Remarks:
	//-----------------------------------------------------------------------
	void StartDragScrollBar(BOOL bResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the slider is about to drag/resize.
	// Parameters:
	//     bResize  - Tells the whether the slider is about to resize, a
	//                TRUE value hints the resizing and FALSE simple dragging.
	// Remarks:
	//-----------------------------------------------------------------------
	void StartDragSlider(BOOL bResize);


	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called when the marker is about to drag
	// Parameters:
	//     nMarkerIndex  - The zero based index of the marker, which is about
	//                     to drag
	// Remarks:
	//-----------------------------------------------------------------------
	void StartDragMarker(int nMarkerIndex);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to move/resize the slider.
	// Parameters:
	//     pt      -  The new position of the slider.
	//     bResize -  TRUE value hints the resizing and FALSE simple dragging.
	// Remarks:
	//-----------------------------------------------------------------------
	void OnMoveSlider(CPoint pt, BOOL bResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to move the marker.
	// Parameters:
	//     pt           -  The new position of the marker.
	//     nMarkerIndex -  The zero based index of the marker, which is moving.
	// Remarks:
	//-----------------------------------------------------------------------
	void OnMoveMarker(CPoint pt, int nMarkerIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to move/resize the scrollbar.
	// Parameters:
	//     pt      -  The new start position of the scroll bar.
	//     bResize -  TRUE value hints the resizing and FALSE simple dragging.
	// Remarks:
	//-----------------------------------------------------------------------
	void OnMoveScrollBar(CPoint pt, BOOL bResize);


protected:
	CPoint m_ptStartDrag;   //The drag start point.
	int m_nOldWorkAreaMin;  //The old work area minimum value.
	int m_nOldWorkAreaMax;  //The old work area maximum value.
	int m_nOldViewPortMin;  //The old view port minimum value.
	int m_nOldViewPortMax;  //The old view port maximum value.

	int m_nOldMarkerPos;    //The old marker position.
};

#endif //#if !defined(__XTPTRACKEADER_H__)
