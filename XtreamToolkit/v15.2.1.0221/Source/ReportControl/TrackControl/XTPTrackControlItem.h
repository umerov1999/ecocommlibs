// XTPTrackControlItem.h: interface for the CXTPTrackControlItem class.
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
#if !defined(__XTPTRACKCONTROLITEM_H__)
#define __XTPTRACKCONTROLITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTrackControl;
class CXTPTrackBlock;
typedef CArray<CXTPTrackBlock*, CXTPTrackBlock*> CXTPTrackBlockArray;

//===========================================================================
// Summary:
//     This Class represents a container cell for set of Blocks and Keys
//     and inherits basic functionality from CXTPReportRecordItem class.
// See Also: CXTPReportRecordItem
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackControlItem : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CXTPTrackControlItem)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPTrackControlItem item.
	//-----------------------------------------------------------------------
	CXTPTrackControlItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackControlItem default destructor
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackControlItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a text caption of this item.
	// Parameters:
	//     pColumn - Corresponded column of the item.
	// Remarks:
	//     Overwrites parents' member function
	// Returns:
	//     Text caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contains drawing arguments:
	// Remarks:
	//     Call this member function to draw an item. Actually this function
	//     only prepares and calls needed drawing functions from PaintManager.
	//     Thus if you wish to change the look of your report item, you must just provide
	//     your own implementation of PaintManager
	// Example:
	// <code>
	// // fill structure
	// XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
	// drawArgs.pDC = pDC;
	// drawArgs.pControl = m_pControl;
	// drawArgs.pRow = this;
	// // call function
	// Draw(&drawArgs);
	// </code>
	//
	// See Also: XTP_REPORTRECORDITEM_DRAWARGS
	//-----------------------------------------------------------------------
	virtual int Draw(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to get the block at a point of hit.
	// Parameters:
	//     ptPoint - Coordinate to test
	// Returns:
	//     A pointer to CXTPTrackBlock, which is hit.
	//-----------------------------------------------------------------------
	CXTPTrackBlock* HitTest(CPoint ptPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to remove a block
	// Parameters:
	//     pBlock - A pointer to the block to be removed.
	// Returns:
	//     TRUE if the removal is successful and FALSE else.
	//-----------------------------------------------------------------------
	BOOL Remove(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to remove a block
	// Parameters:
	//     nIndex - Zero based index of the block to be removed.
	// Returns:
	//     TRUE if the removal is successful and FALSE else.
	//-----------------------------------------------------------------------
	BOOL RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to remove all blocks
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a block to the track item.
	// Parameters:
	//     pBlock - A pointer to track block which is to be added.
	// Returns:
	//     A pointer to the newly added block.
	//-----------------------------------------------------------------------
	CXTPTrackBlock* Add(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to the track control associated with this item.
	// Returns:
	//     A pointer to the track control.
	//-----------------------------------------------------------------------
	CXTPTrackControl* GetTrackControl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to the track control associated with this item.
	// Parameters:
	//     nIndex - Index of block to return
	// Returns:
	//     A pointer to the track control.
	//-----------------------------------------------------------------------
	CXTPTrackBlock* GetBlock(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to the number of block in the track control item.
	// Returns:
	//     An integer telling the number of blocks in a track.
	//-----------------------------------------------------------------------
	int GetBlockCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this method to know whether the track control item is locked
	//     or not.
	// Returns:
	//     A boolean value of TRUE if the track item is locked and FALSE if
	//     not.
	// Remarks:
	//     If a track is locked it can't be moved and none of it's blocks or
	//     keys can be moved.  When locked, blocks and keys from other tracks
	//     can't be dragged into this track.
	//-----------------------------------------------------------------------
	BOOL IsLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this method to set the track control item locked/unlocked.
	// Parameters:
	//     bLocked - A boolean value of TRUE if the track control item is to
	//     be locked and FALSE if  not.
	// Remarks:
	//     If a track is locked it can't be moved and none of it's blocks or
	//     keys can be moved.  When locked, blocks and keys from other tracks
	//     can't be dragged into this track.
	//-----------------------------------------------------------------------
	void SetLocked(BOOL bLocked = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Remarks:
	//     Usually this function is called by ReportRow's OnLButtonDown function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

//  virtual BOOL OnRButtonDown(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button up clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Remarks:
	//     Usually this function is called by ReportRow's OnLButtonUp function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonUp(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Remarks:
	//     Usually this function is called by ReportRow's OnClick function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments:
	// Remarks:
	//     Usually this function is called by ReportRow's OnDblClick function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnDblClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     point  - mouse point
	//     nFlags - additional flags.
	//     pControl - pointer to report control.
	// Remarks:
	//     Usually this function is called by ReportRow's OnMouseMove function
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point, CXTPReportControl* pControl = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a report record item using
	//     the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	//-----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to do a hit test on the track control
	//     item and fill the TOOLINFO with the contextual display string.
	// Parameters:
	//     point - The point of hit.
	//     pTI   - The tool info.
	// Returns:
	//     An INT_PTR value pointing to the track control item on which the
	//     hit test done.
	//-----------------------------------------------------------------------
	INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call the RecalcLayout method to correctly size the track items when
	//     the layout has changed.
	//-----------------------------------------------------------------------
	void RecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call the function to adjust the block position when the block is
	//     moved or resized.
	// Parameters:
	//     pBlock  - Pointer to the track block which is moved or resized.
	//     bResize - Tells whether the block is being resized.
	//-----------------------------------------------------------------------
	void AdjustBlockPosition(CXTPTrackBlock* pBlock, BOOL bResize = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to do a redrawing of the track control.
	//-----------------------------------------------------------------------
	void RedrawControl();
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to highlight the work area.
	// Parameters:
	//     pDC    - Pointer to the device context.
	//     rcItem - The work area rectangle.
	//-----------------------------------------------------------------------
	virtual void HighlightWorkArea(CDC* pDC, CRect rcItem);


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()


	LPDISPATCH OleGetBlock(int nIndex);
	int OleGetBlockCount();
	LPDISPATCH OleAddBlock(BOOL bKey, int nPosition, int nLength);
	void OleRemoveBlock(int nIndex);

	BOOL OleGetLocked();
	void OleSetLocked(BOOL bLocked);

	DECLARE_OLETYPELIB_EX(CXTPTrackControlItem);
//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	BOOL m_bLocked;                 // TRUE if whole track is locked

protected:
	CXTPTrackBlockArray m_arrBlocks; // Blocks array

protected:

	struct DOCK_INFO;
	class CDockInfoArray;

	friend class CXTPTrackControl;

};

AFX_INLINE CXTPTrackBlock* CXTPTrackControlItem::GetBlock(int nIndex) const
{
	return m_arrBlocks[nIndex];
}

AFX_INLINE int CXTPTrackControlItem::GetBlockCount() const
{
	return (int)m_arrBlocks.GetSize();
}

AFX_INLINE BOOL CXTPTrackControlItem::IsLocked() const
{
	return m_bLocked;
}

AFX_INLINE void CXTPTrackControlItem::SetLocked(BOOL bLocked)
{
	m_bLocked = bLocked;
}


#endif //#if !defined(__XTPTRACKCONTROLITEM_H__)
