// XTPTrackControl.h: interface for the CXTPTrackControl class.
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
#if !defined(__XTPTRACKCONTROL_H__)
#define __XTPTRACKCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTrackBlock;
class CXTPTrackMarker;
class CXTPTrackSelectedBlocks;
class CXTPTrackMarkers;


//-----------------------------------------------------------------------
// Summary:
//     This notification is send to the parent, when the track slider is
//     changed.
//-----------------------------------------------------------------------
#define  XTP_NM_TRACK_SLIDERCHANGED (NM_FIRST-200)

//-----------------------------------------------------------------------
// Summary:
//     This notification is send to the parent, when the track time line is
//     changed.
//-----------------------------------------------------------------------
#define  XTP_NM_TRACK_TIMELINECHANGED (NM_FIRST-201)

//-----------------------------------------------------------------------
// Summary:
//     This notification is send to the parent, when the track marker is
//     changed.
//-----------------------------------------------------------------------
#define  XTP_NM_TRACK_MARKERCHANGED (NM_FIRST-202)

//-----------------------------------------------------------------------
// Summary:
//     This notification is send to the parent, when the track block position
//     changed.
//-----------------------------------------------------------------------
#define  XTP_NM_TRACK_BLOCKCHANGED (NM_FIRST-203)

//-----------------------------------------------------------------------
// Summary:
//     This notification is send to the parent, when the selected blocks position
//     changed.
//-----------------------------------------------------------------------
#define  XTP_NM_TRACK_SELECTEDBLOCKSCHANGED (NM_FIRST-204)

//-----------------------------------------------------------------------
// Summary:
//     This structure contains the notification details.
//-----------------------------------------------------------------------
struct XTP_NM_TRACKCONTROL
{
	NMHDR hdr;                  //The NMHDR instance.
	CXTPTrackBlock* pBlock;     //The track block.
	CXTPTrackMarker* pMarker;   //The track marker.
};


class CXTPTrackPaintManager;
class CXTPTrackUndoManager;

//-----------------------------------------------------------------------
// Summary:
//     This class represents a track control, The Track Control is an
//     extension to the Report Control.  It can do basically everything
//     the the Report Control can do with the addition of a Track Control.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackControl : public CXTPReportControl
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the CXTPTrackControl object.
	//-----------------------------------------------------------------------
	CXTPTrackControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys the CXTPTrackControl object.Does the clean up.
	//-----------------------------------------------------------------------
	~CXTPTrackControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Register the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of resource where control is located
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the time line Position.
	// Parameters:
	//     nTimeLinePos - An integer value specifying the time line Position.
	//-----------------------------------------------------------------------
	void SetTimeLinePosition(int nTimeLinePos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the time line Position.
	// Returns:
	//     An integer value specifying the time line Position.
	//-----------------------------------------------------------------------
	int GetTimeLinePosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the minimum time line Position.
	// Returns:
	//     An integer value specifying the minimum time line Position.
	//-----------------------------------------------------------------------
	int GetTimeLineMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the maximum time line Position.
	// Returns:
	//     An integer value specifying the maximum time line Position.
	//-----------------------------------------------------------------------
	int GetTimeLineMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the bounds of the work area.
	// Parameters:
	//     nWorkAreaMin - The start value of the work area rectangle.
	//     nWorkAreaMax - The end value of the work area rectangle.
	//-----------------------------------------------------------------------
	void SetWorkArea(int nWorkAreaMin, int nWorkAreaMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the view port of the work area.
	// Parameters:
	//     nViewPortMin - The start value of the view port rectangle.
	//     nViewPortMax - The end value of the view port rectangle.
	//-----------------------------------------------------------------------
	void SetViewPort(int nViewPortMin, int nViewPortMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the time line range
	// Parameters:
	//     nTimeScaleMin - The start value of the view port rectangle.
	//     nTimeScaleMax - The end value of the view port rectangle.
	//-----------------------------------------------------------------------
	void SetTimeLineRange(int nTimeScaleMin, int nTimeScaleMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the minimum value of work area range.
	// Returns:
	//     An integer value specifying the minimum value of the work area.
	//-----------------------------------------------------------------------
	int GetWorkAreaMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the maximum value of the work area.
	// Returns:
	//     An integer value specifying the maximum value of the work area.
	//-----------------------------------------------------------------------
	int GetWorkAreaMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the minimum value of the view port.
	// Returns:
	//     An integer value specifying the minimum value of the view port.
	//-----------------------------------------------------------------------
	int GetViewPortMin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the maximum value of the view port.
	// Returns:
	//     An integer value specifying the maximum value of the view port.
	//-----------------------------------------------------------------------
	int GetViewPortMax() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the track paint manager associated with
	//     this control.
	// Returns:
	//     A pointer to the track paint manager.
	//-----------------------------------------------------------------------
	CXTPTrackPaintManager* GetTrackPaintManager() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     the track control.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the selected blocks in the track control.
	// Returns:
	//     A pointer to the selected blocks object.
	//-----------------------------------------------------------------------
	CXTPTrackSelectedBlocks* GetSelectedBlocks() const;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to populate the track control with the records.
	//-----------------------------------------------------------------------
	void Populate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws all rows on the provided DC.
	// Parameters:
	//     pDC - Provided DC to draw rows image with.
	//     rcClient - A rectangle to draw rows image into.
	//-----------------------------------------------------------------------
	int DrawRows(CDC* pDC, CRect& rcClient, int y, CXTPReportRows *pRows, int nTopRow, int nColumnFrom, int nColumnTo, int *pnHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the track offset.
	// Returns:
	//     An integer value specifying the track offset.
	//-----------------------------------------------------------------------
	int GetTrackOffset() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to adjust the layout of the track control when
	//     there is a change in metrics of the control.
	//-----------------------------------------------------------------------
	void AdjustLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the track column.
	// Returns:
	//     A pointer to the report column object.
	//-----------------------------------------------------------------------
	CXTPReportColumn* GetTrackColumn() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the track point from a coordinate point
	//     on the track control.
	// Parameters:
	//     nPosition - Position to convert
	// Returns:
	//     An integer value specifying the track position.
	//-----------------------------------------------------------------------
	int PositionToTrack(int nPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the coordinate point on the track control
	//     from a track point.
	// Parameters:
	//     nTrack - Coordintae to convert
	// Returns:
	//     An integer value specifying the position on the tack control.
	//-----------------------------------------------------------------------
	int TrackToPosition(int nTrack) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the zero based index of the track column.
	// Returns:
	//     An integer specifying the track column index.
	//-----------------------------------------------------------------------
	int GetTrackColumnIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the snap margin.
	// Returns:
	//     An integer specifying the snap margin.
	// Remarks:
	//     Snap margin specifies the margin that a block must be dragged into
	//     before it will snap to another block or snap to a marker.
	//-----------------------------------------------------------------------
	int GetSnapMargin() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the time line area.
	// Returns:
	//     A CRect object representing the bounds of the time line area.
	// Remarks:
	//-----------------------------------------------------------------------
	CRect GetTimelineArea() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to ensure a block visible.
	// Parameters:
	//     pBlock - A pointer to the track block object, whose visibility to be ensured.
	// Remarks:
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the track markers object, associated with
	//     the track control.
	// Returns:
	//     A pointer to the track markers object, which has a collection of
	//     track marker.
	// Remarks:
	//     CXTPTrackMarkers a collection of all the track markers in the track
	//     control.This collection has methods to add, delete and access Markers.
	//     Markers are used for block alignment, blocks can be "snapped" to the
	//     left or right of these markers.
	//-----------------------------------------------------------------------
	CXTPTrackMarkers* GetMarkers() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get undo manager associated with the track control.
	// Returns:
	//     A pointer to the track undo manager object.
	// Remarks:
	//     The Track Undo Manager allows certain edit operations to be undone
	//     and re-done in the track control.  Supported operations are
	//     Add/Remove/Move markers,  Add/Remove/Move/Resize Blocks.
	//     If any of these operations are performed they can easily be
	//     undone\re-done using the methods of the CXTPTrackUndoManager.
	//-----------------------------------------------------------------------
	CXTPTrackUndoManager* GetUndoManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the report part at a specified position.
	// Parameters:
	//     pt    - A point to test.
	//     pInfo - Pointer to a CXTPReportHitTestInfo that will hold the
	//             returned information.
	// Returns:
	//     TRUE if the method was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, CXTPReportHitTestInfo *pInfo) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to send messages to the parent window of the track
	//     control.
	// Parameters:
	//     nMessage - The message.
	//     pBlock   - The related block.
	//     pMarker  - The related marker.
	// Remarks:
	//     This function is useful to send notification to the parent when some
	//     event happens in the track control.
	//-----------------------------------------------------------------------
	LRESULT SendMessageToParent(UINT nMessage, CXTPTrackBlock* pBlock = NULL, CXTPTrackMarker* pMarker = NULL) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to when you are about to drag the blocks.
	// Parameters:
	//     bResize - Tells whether the user is trying to resize the block.
	//               TRUE if beginning to resize and FALSE else(simple drag).
	// Remarks:
	//-----------------------------------------------------------------------
	void StartDragBlocks(BOOL bResize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the possible snap position for a block
	//     in a particular position on track.
	// Parameters:
	//     nPosition - The cursor position based on which the snap position
	//                 is to be calculated.
	// Return:
	//     An integer specifying the snap position
	// Remarks:
	//-----------------------------------------------------------------------
	int SnapPosition(int nPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to when you are about to do the drag selection.
	// Remarks:
	//-----------------------------------------------------------------------
	void StartDragSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to adjust the track column width.
	// Remarks:
	//-----------------------------------------------------------------------
	void AdjustTrackColumnWidth();

	void SetPaintManager(CXTPTrackPaintManager *pPaintManager);
	void SetTheme(XTPReportPaintTheme paintTheme, BOOL bEnableMetrics/*=FALSE*/);

	CPoint m_ptStartDrag;   //The point on which the dragging starts.

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void OnMoveBlock(CPoint pt, BOOL bResize);
	void OnMoveSelection(CPoint pt);
	void CreateDragBlocks();
	void ReleaseDragBlocks();
	void OnDraw(CDC* pDC);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	int m_nTimeLineMin;             //Minimum value for the time line.
	int m_nTimeLineMax;             //Maximum value for the time line.

	double m_nViewPortMin;          //Minimum value for the view port.
	double m_nViewPortMax;          //Maximum value for the view port.

	int m_nOldTrackWidth;           //The old track width.
	double m_nOldTrackViewPortRange;//The old track view port range.

	int m_nWorkAreaMin;             //The minimum value for the work area.
	int m_nWorkAreaMax;             //The Maximum value for the work area.

	int m_nTimeLinePosition;        //The trim line position.
	int m_nTrackColumnIndex;        //The track column index.

	CRect m_rcSelectedArea;         //The selected area.

	CXTPTrackMarkers* m_pMarkers;   //The marker collection object.

	CRect m_rcTimelineArea;         //The area of the time line.

	//-----------------------------------------------------------------------
	// Summary:
	//     This structure abstracts the state of block being dragged.
	//-----------------------------------------------------------------------
	struct DRAGBLOCK
	{
		int nOldPos;            //The old position.
		int nOldLength;         //The old length.
		CRect rcOrigin;         //The original rectangle.
		CXTPTrackBlock* pBlock; //Pointer to the block.
	};
	CArray<DRAGBLOCK, DRAGBLOCK&> m_arrDragBlocks;  //The collection of blocks which are being dragged.

	CXTPTrackSelectedBlocks* m_pSelectedBlocks;     //The pointer to the selected blocks object.
	CXTPTrackUndoManager* m_pUndoManager;           //The pointer to the undo manager object.

public:
	HCURSOR m_hMoveCursor;                          //The cursor while moving the blocks.
	HCURSOR m_hResizeCursor;                        //The cursor while resizing the blocks.

	int m_nSnapMargin;                              //The snap margin, specifies the margin that a block must be dragged into before it will snap to another block or snap to a marker.
	BOOL m_bSnapToBlocks;                           //TRUE if the current block is about to be snapped to the blocks, FALSE else.
	BOOL m_bSnapToMarkers;                          //TRUE if the current block is is about to be snapped to the markers, FALSE else.

	BOOL m_bScaleOnResize;                          //Specifies whether to scale the track control view area when resizing the track control.


	BOOL m_bFlexibleDrag;                           //Enables or disables flexible dragging of blocks in the track control.

	BOOL m_bAllowBlockRemove;                       //Specifies whether the user can move blocks from one track to another.
	BOOL m_bAllowBlockScale;                        //Specifies whether the user can resize a block.
	BOOL m_bAllowBlockMove;                         //Specifies whether the user can move a block.

	BOOL m_bShowWorkArea;                           //TRUE to show the work area and FALSE to hide it.
	BOOL m_bShowTimeLinePosition;

	CString m_strTimeFormat;                        //The string which specifies the time format.

	friend class CXTPTrackControlItem;
};


AFX_INLINE int CXTPTrackControl::GetTimeLinePosition() const
{
	return m_nTimeLinePosition;
}

AFX_INLINE void CXTPTrackControl::SetWorkArea(int nWorkAreaMin, int nWorkAreaMax)
{
	m_nWorkAreaMin = nWorkAreaMin;
	m_nWorkAreaMax = nWorkAreaMax;
	RedrawControl();
}

AFX_INLINE void CXTPTrackControl::SetTimeLineRange(int nTimeScaleMin, int nTimeScaleMax)
{
	m_nTimeLineMin = nTimeScaleMin;
	m_nTimeLineMax = nTimeScaleMax;
	RedrawControl();
}

AFX_INLINE int CXTPTrackControl::GetWorkAreaMin() const
{
	return m_nWorkAreaMin;
}

AFX_INLINE int CXTPTrackControl::GetWorkAreaMax() const
{
	return m_nWorkAreaMax;
}

AFX_INLINE int CXTPTrackControl::GetViewPortMin() const
{
	return (int)m_nViewPortMin;
}

AFX_INLINE int CXTPTrackControl::GetViewPortMax() const
{
	return (int)m_nViewPortMax;
}

AFX_INLINE int CXTPTrackControl::GetTimeLineMin() const
{
	return m_nTimeLineMin;
}

AFX_INLINE int CXTPTrackControl::GetTimeLineMax() const
{
	return m_nTimeLineMax;
}

AFX_INLINE int CXTPTrackControl::GetSnapMargin() const
{
	return m_nSnapMargin;
}

AFX_INLINE CRect CXTPTrackControl::GetTimelineArea() const
{
	return m_rcTimelineArea;
}

AFX_INLINE CXTPTrackPaintManager* CXTPTrackControl::GetTrackPaintManager() const
{
	return (CXTPTrackPaintManager*)GetPaintManager();
}

AFX_INLINE CXTPTrackSelectedBlocks* CXTPTrackControl::GetSelectedBlocks() const
{
	return m_pSelectedBlocks;
}

AFX_INLINE CXTPTrackMarkers* CXTPTrackControl::GetMarkers() const
{
	return m_pMarkers;
}

AFX_INLINE CXTPTrackUndoManager* CXTPTrackControl::GetUndoManager() const {
	return m_pUndoManager;
}

#endif //#if !defined(__XTPTRACKCONTROL_H__)
