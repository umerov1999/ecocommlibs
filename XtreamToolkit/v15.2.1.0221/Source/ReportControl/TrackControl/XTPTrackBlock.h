// XTPTrackBlock.h: interface for the CXTPTrackBlock class.
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
#if !defined(__XTPTRACKBLOCK_H__)
#define __XTPTRACKBLOCK_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTrackControlItem;

//-----------------------------------------------------------------------
// Summary:
//     This class represents a track control block. Track Blocks are a visual
//     representation of units of time in a track. They can have both a
//     minimum and maximum time period.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackBlock : public CXTPCmdTarget
{
	DECLARE_SERIAL(CXTPTrackBlock);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the CXTPTrackBlock object.
	//-----------------------------------------------------------------------
	CXTPTrackBlock();
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the block.
	// Parameters:
	//     pDC       - The pointer to the device context.
	//     rc        - The bounds of the drawing area.
	//     bSelected - Tells whether the block is selected or not.
	// Returns:
	//     An integer value specifying whether the drawing is successful.A value
	//     of 1 is to represent success and 0 for failure.
	// Remarks:
	//     This is a virtual function hence the sub classes can give their
	//     implementation for this function.
	//-----------------------------------------------------------------------
	virtual int Draw(CDC* pDC, CRect rc, BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether the item is a key.
	// Returns:
	//     A BOOL value of TRUE tells the block is a key and FALSE else.
	// Remarks:
	//     This is a virtual function hence the sub classes can give their
	//     implementation(meaning) for this function.
	//-----------------------------------------------------------------------
	virtual BOOL IsKey() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether the block is resizable.
	// Returns:
	//     A BOOL value of TRUE tells the block is resizable and FALSE else.
	// Remarks:
	//     This is a virtual function hence the sub classes can give their
	//     implementation(meaning) for this function.
	//-----------------------------------------------------------------------
	virtual BOOL IsResizable() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the position of the block.
	// Parameters:
	//     nPosition - An integer value specifying the position of the block on
	//                 the track.
	//-----------------------------------------------------------------------
	void SetPosition(int nPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the position of the block.
	// Returns:
	//     An integer value specifying the position of the block on the track.
	//-----------------------------------------------------------------------
	int GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the length of the block.
	// Parameters:
	//     nLength - An integer value specifying the length of the block on
	//                 the track.
	//-----------------------------------------------------------------------
	void SetLength(int nLength);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the length of the block.
	// Returns:
	//     An integer value specifying the length of the block on the track.
	//-----------------------------------------------------------------------
	int GetLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the color of the block.
	// Parameters:
	//     clr - A COLORREF value specifying the color of the block.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the bounding rectangle of the block.
	// Returns:
	//     A CRect value specifying the rectangle of the block
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether the block is locked, which means
	//     the block cannot be moved or resized.
	// Returns:
	//     A BOOL value specifying whether the blocked is locked or not.
	//-----------------------------------------------------------------------
	BOOL IsLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the block locked, which means the block
	//     cannot be moved or resized.
	// Parameters:
	//     bLocked - Tells whether the blocked is to be locked or not, TRUE is
	//               for locking a block and is the default argument.
	//-----------------------------------------------------------------------
	void SetLocked(BOOL bLocked = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the percentage of height of a block
	//     compared to it's track.
	// Parameters:
	//     dHeightPercent - A double value denoting the height percentage.
	// Remarks:
	//     Blocks can also be different different heights and set to different
	//     vertical alignments within the track.  This is used just as a visual
	//     aid to make different blocks in the track visually different from
	//     each other.
	//-----------------------------------------------------------------------
	void SetHeightPercent(double dHeightPercent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the percentage of height of a block
	//     compared to it's track.
	// Returns:
	//     A double value denoting the height percentage.
	//-----------------------------------------------------------------------
	double GetHeightPercent() const;

	void SetHeightFixed(int nHeightPercent);
	int GetHeightFixed() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the vertical alignment of a block in a
	//     track.
	// Parameters:
	//     nAlignment - A double value denoting the vertical alignment.
	// Remarks:
	//     VerticalAlignment is used for keys and blocks with a height that
	//     is not 100 the size of the track height and vertically aligns the
	//     block in the center of the track.
	//-----------------------------------------------------------------------
	void SetVerticalAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the vertical alignment of a block in a
	//     track.
	// Returns:
	//     A double value denoting the vertical alignment.
	// Remarks:
	//     VerticalAlignment is used for keys and blocks with a height that
	//     is not 100 the size of the track height and vertically aligns the
	//     block in the center of the track.
	//-----------------------------------------------------------------------
	int GetVerticalAlignment() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the track of the block.
	//     track.
	// Returns:
	//     A pointer to the track control item(track).
	// Remarks:
	//     Tracks are what hold the blocks and keys.
	//-----------------------------------------------------------------------
	CXTPTrackControlItem* GetItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the track of the block.
	//     track.
	// Returns:
	//     A pointer to the track control item(track).
	// Remarks:
	//     Tracks are what hold the blocks and keys.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to select the block.
	// Parameters:
	//     bSelect  - BOOL value use to select or unselect the block.
	//-----------------------------------------------------------------------
	void Select(BOOL bSelect = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether the block is selected.
	// Returns:
	//     BOOL value of TRUE if the block is selected and FALSE if not.
	//-----------------------------------------------------------------------
	BOOL IsSelected() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the caption of the block.
	// Returns:
	//     A string value representing the caption of the block.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set/change the caption of the block.
	// Parameters:
	//     lpszCaption - String value of the caption.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the text tool tip for the item
	// Returns:
	//     The text tool tip for the item.
	//-----------------------------------------------------------------------
	virtual CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set/change the item tool tip.
	// Parameters:
	//     lpszTooltip - Tool tip of the item
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set/change the description of the item.
	// Parameters:
	//     lpszDescriptionText - Description of the block.
	//-----------------------------------------------------------------------
	void SetDescriptionText(LPCTSTR lpszDescriptionText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the description of the item.
	// Returns:
	//     A CString representing the description of the block.
	//-----------------------------------------------------------------------
	CString GetDescriptionText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the minimum length of the item.
	// Parameters:
	//     nMinLength - The minimum length of the block.
	//-----------------------------------------------------------------------
	void SetMinLength(int nMinLength);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the minimum length of the item.
	// Returns:
	//     An integer value specifying the minimum length of the block.
	//-----------------------------------------------------------------------
	int GetMinLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the maximum length of the item.
	// Parameters:
	//     nMaxLength - The maximum length of the block.
	//-----------------------------------------------------------------------
	void SetMaxLength(int nMaxLength);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the maximum length of the item.
	// Returns:
	//     An integer value specifying the maximum length of the block.
	//-----------------------------------------------------------------------
	int GetMaxLength() const;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     the block.
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
	//     Call this member to remove block
	//-----------------------------------------------------------------------
	void Remove();
public:

protected:
	CRect m_rcBlock;                // Drawing bounding rectangle of block
	CXTPTrackControlItem* m_pItem;  // Pointer to parent item of block.
	int m_nMRUPosition;             // Last position that was set by user
	int m_nPosition;                // Position that resolve overlaps with items on same track.
	int m_nLength;                  // Length of block
	BOOL m_bLocked;                 // TRUE if block is not movable.
	COLORREF m_clrBlock;            // Color of block.
	CString m_strToolTip;           // Tooltip text
	CString m_strDescription;       // Description text
	CString m_strCaption;           // Caption text.
	int m_nIndex;                   // Index in parent track collection.
	double m_dHeightPercent;        // Percent of whole row.
	int m_nHeightFixed;
	int m_nVerticalAlignment;       // Vertical alignment of blocks with m_dHeightPercent < 1
	int m_nLastDragTime;            // Last drag moment of user (used as priority for flexible draging)
	int m_nMinLength;               // Minimum resize length
	int m_nMaxLength;               // Maximum resize length

	BOOL m_bDragging;               // Block is currently dragging


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackBlock);


	int OleGetPosition();
	void OleSetPosition(int nPosition);
	int OleGetLength();
	void OleSetLength(int nLength);
	BOOL OleGetLocked();
	void OleSetLocked(BOOL bLocked);
	COLORREF OleGetColor();
	void OleSetColor(OLE_COLOR clr);
	BSTR OleGetTooltip();
	void OleSetTooltip(LPCTSTR lpszTooltip);
	BSTR OleGetDescriptionText();
	void OleSetDescriptionText(LPCTSTR lpszDescription);
	double OleGetHeightPercent();
	void OleSetHeightPercent(double dHeightPercent);
	int OleGetHeightFixed();
	int OleGetVerticalAlignment();
	void OleSetHeightFixed(int dHeightFixed);
	void OleSetVerticalAlignment(int nVerticalAlignment);
	int OleGetMinLength();
	void OleSetMinLength(int nMinLength);
	int OleGetMaxLength();
	void OleSetMaxLength(int nMaxLength);
	LPDISPATCH OleGetItem();
	int OleGetIndex();
	BOOL OleGetIsKey();
	BSTR OleGetCaption();
	void OleSetCaption(LPCTSTR lpszCaption);
//}}AFX_CODEJOCK_PRIVATE
#endif
private:
	friend class CXTPTrackControlItem;
	friend class CXTPTrackControl;
};

//-----------------------------------------------------------------------
// Summary:
//     This class represents a track control key.Keys are used simply as
//     "notes" or reference points in the track.  They can be added anywhere
//     in the track to make note of some event at the given moment in time.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackKey : public CXTPTrackBlock
{
	DECLARE_SERIAL(CXTPTrackKey);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the CXTPTrackKey object.
	//-----------------------------------------------------------------------
	CXTPTrackKey();
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the key.
	// Parameters:
	//     pDC       - The pointer to the device context.
	//     rc        - The bounds of the drawing area.
	//     bSelected - Tells whether the block is selected or not.
	// Returns:
	//     An integer value specifying whether the drawing is successful.A value
	//     of 1 is to represent success and 0 for failure.
	// Remarks:
	//     This is a virtual function hence the sub classes can give their
	//     implementation for this function.
	//-----------------------------------------------------------------------
	virtual int Draw(CDC* pDC, CRect rc, BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether the item is a key.
	// Returns:
	//     A BOOL value of TRUE tells the item is a key and FALSE else.
	// Remarks:
	//     This is a virtual function hence the sub classes can give their
	//     implementation(meaning) for this function.
	//-----------------------------------------------------------------------
	virtual BOOL IsKey() const;
};

//-----------------------------------------------------------------------
// Summary:
//     This class represents a track control marker. Markers are used for
//     block alignment, blocks can be "snapped" to the left or right of
//     these markers.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackMarker : public CXTPCmdTarget
{
	DECLARE_SERIAL(CXTPTrackMarker);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the CXTPTrackMarker object.
	//-----------------------------------------------------------------------
	CXTPTrackMarker();
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the position of the marker.
	// Parameters:
	//     nPosition - An integer value specifying the position of the marker on
	//                 the time line.
	//-----------------------------------------------------------------------
	void SetPosition(int nPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the position of the marker.
	// Returns:
	//     An integer value specifying the position of the marker on the time
	//     line.
	//-----------------------------------------------------------------------
	int GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set/change the caption of the marker.
	// Parameters:
	//     lpszCaption - String value of the caption.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the caption of the marker.
	// Returns:
	//     A string value representing the caption of the marker.
	//-----------------------------------------------------------------------
	CString GetCaption() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the bounding rectangle of the marker.
	// Returns:
	//     A CRect value specifying the rectangle of the marker.
	//-----------------------------------------------------------------------
	CRect GetRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove the marker.
	//-----------------------------------------------------------------------
	void Remove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the Track control to which the marker is
	//     associated with.
	// Returns:
	//     A pointer to the track control object.
	//-----------------------------------------------------------------------
	CXTPTrackControl* GetControl() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     the marker.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CString m_strCaption;       // Caption of marker
	int m_nPosition;            // Position of marker.
	CRect m_rcMarker;           // Drawing position of marker
	CXTPTrackControl* m_pControl; // Parent control.

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackMarker);
//}}AFX_CODEJOCK_PRIVATE
#endif

private:
	friend class CXTPTrackPaintManager;
	friend class CXTPTrackMarkers;
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the track control markers. Markers are used for
//     block alignment, blocks can be "snapped" to the left or right of
//     these markers.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackMarkers : public CXTPCmdTarget
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the CXTPTrackMarkers object.
	// Parameters:
	//     pControl - Pointer to parent Track Control object
	//-----------------------------------------------------------------------
	CXTPTrackMarkers(CXTPTrackControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys the CXTPTrackMarkers object.Does the clean up.
	//-----------------------------------------------------------------------
	~CXTPTrackMarkers();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create and add a marker to the marker
	//     collection.
	// Parameters:
	//     nPosition   - Position of the marker.
	//     lpszCaption - Caption of the marker.
	// Returns:
	//     A pointer to the newly created track marker object.
	//-----------------------------------------------------------------------
	CXTPTrackMarker* Add(int nPosition, LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a marker to the marker collection.
	// Parameters:
	//     pMarker - New marker to add
	// Returns:
	//     A pointer to the added track marker object.
	//-----------------------------------------------------------------------
	CXTPTrackMarker* Add(CXTPTrackMarker* pMarker);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a marker at a particular index in
	//     the marker collection.
	// Parameters:
	//     nIndex - The zero based index.
	// Returns:
	//     A pointer to the track marker object at the given index, if the
	//     index is not valid in the context NULL is returned.
	//-----------------------------------------------------------------------
	CXTPTrackMarker* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove a marker at a particular index
	//     in from marker collection.
	// Parameters:
	//     nIndex   - The zero based index.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove a marker from marker collection.
	// Parameters:
	//     pMarker   - The pointer to the marker to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPTrackMarker* pMarker);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove a marker from marker collection.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the index of a  marker in the marker
	//     collection.
	// Parameters:
	//     pMarker   - The pointer to the marker whose index is to be found
	// Returns:
	//     An integer value denoting the zero based index of the marker.
	//-----------------------------------------------------------------------
	int IndexOf(CXTPTrackMarker* pMarker);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the number of markers in the
	//     collection.
	// Returns:
	//     An integer value denoting the number of items in the marker collection.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to get the index of the marker at a point.
	// Parameters:
	//     pt - Coordinate to test.
	// Returns:
	//     An integer value denoting the zero based index of the marker hit.
	//-----------------------------------------------------------------------
	int HitTest(CPoint pt) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties related to the
	//     the marker collection.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CXTPTrackControl* m_pControl;                               // Parent control
	CArray<CXTPTrackMarker*, CXTPTrackMarker*> m_arrMarkers;    // Array of markers

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackMarkers);
	DECLARE_ENUM_VARIANT(CXTPTrackMarkers)

	LPDISPATCH OleAdd(int Position, LPCTSTR lpszCaption);
	LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();
//}}AFX_CODEJOCK_PRIVATE
#endif

private:
	friend class CXTPTrackControl;
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the selected block of the track control. This
//     class helps to perform a common task to the selected blocks, like
//     delete.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackSelectedBlocks : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs the CXTPTrackSelectedBlocks object.
	//-----------------------------------------------------------------------
	CXTPTrackSelectedBlocks();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the number of blocks in the
	//     selected blocks.
	// Returns:
	//     An integer value denoting the number of items in the selected blocks.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a block to the selected blocks.
	// Parameters:
	//     pBlock   - A pointer to the block.
	//-----------------------------------------------------------------------
	void Add(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get whether the block is selected.
	// Parameters:
	//     pBlock   - A pointer to the block.
	// Returns:
	//     BOOL value of TRUE if the block is selected and FALSE if not.
	//-----------------------------------------------------------------------
	BOOL IsSelected(const CXTPTrackBlock* pBlock) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the block at an index in the
	//     selected blocks.
	// Parameters:
	//     nIndex - Index of block to retrieve
	// Returns:
	//     An pointer to the track block.
	//-----------------------------------------------------------------------
	CXTPTrackBlock* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove all the blocks from the selection.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove a block at an index from the
	//     selection.
	// Parameters:
	//     nIndex - A zero based index of the block to be removed.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove a block at an index from the
	//     selection.
	// Parameters:
	//     pBlock - Pointer to the block to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPTrackBlock* pBlock);


protected:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackSelectedBlocks);
	DECLARE_ENUM_VARIANT(CXTPTrackSelectedBlocks)

	LPDISPATCH OleGetItem(long nIndex);
	int OleGetItemCount();
//}}AFX_CODEJOCK_PRIVATE
#endif
protected:
	CArray<CXTPTrackBlock*, CXTPTrackBlock*> m_arrBlocks;
};

AFX_INLINE BOOL CXTPTrackBlock::IsKey() const {
	return FALSE;
}
AFX_INLINE void CXTPTrackBlock::SetColor(COLORREF clr) {
	m_clrBlock = clr;
}
AFX_INLINE CRect CXTPTrackBlock::GetRect() const {
	return m_rcBlock;
}
AFX_INLINE int CXTPTrackBlock::GetPosition() const {
	return m_nPosition;
}
AFX_INLINE int CXTPTrackBlock::GetLength() const {
	return m_nLength;
}
AFX_INLINE CString CXTPTrackBlock::GetTooltip() const {
	return m_strToolTip;
}
AFX_INLINE  void CXTPTrackBlock::SetTooltip(LPCTSTR lpszTooltip) {
	m_strToolTip = lpszTooltip;
}
AFX_INLINE CString CXTPTrackBlock::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE  void CXTPTrackBlock::SetCaption(LPCTSTR lpszCaption) {
	m_strCaption = lpszCaption;
}
AFX_INLINE void CXTPTrackBlock::SetDescriptionText(LPCTSTR lpszDescriptionText) {
	m_strDescription = lpszDescriptionText;
}
AFX_INLINE CString CXTPTrackBlock::GetDescriptionText() const {
	return m_strDescription;
}
AFX_INLINE void CXTPTrackBlock::SetLocked(BOOL bLocked) {
	m_bLocked = bLocked;
}
AFX_INLINE void CXTPTrackBlock::SetHeightPercent(double dHeightPercent) {
	m_dHeightPercent = dHeightPercent;
}
AFX_INLINE double CXTPTrackBlock::GetHeightPercent() const {
	return m_dHeightPercent;
}
AFX_INLINE void CXTPTrackBlock::SetHeightFixed(int nHeightFixed) {
	m_nHeightFixed = nHeightFixed;
}
AFX_INLINE int CXTPTrackBlock::GetHeightFixed() const {
	return m_nHeightFixed;
}
AFX_INLINE void CXTPTrackBlock::SetVerticalAlignment(int nAlignment) {
	m_nVerticalAlignment = nAlignment;
}
AFX_INLINE int CXTPTrackBlock::GetVerticalAlignment() const {
	return m_nVerticalAlignment;
}
AFX_INLINE CXTPTrackControlItem* CXTPTrackBlock::GetItem() const {
	return m_pItem;
}
AFX_INLINE int CXTPTrackBlock::GetIndex() const {
	return m_nIndex;
}
AFX_INLINE BOOL CXTPTrackBlock::IsResizable() const {
	return !IsKey();
}
AFX_INLINE void CXTPTrackBlock::SetMinLength(int nMinLength) {
	m_nMinLength = nMinLength;
}
AFX_INLINE int CXTPTrackBlock::GetMinLength() const {
	return m_nMinLength;
}
AFX_INLINE void CXTPTrackBlock::SetMaxLength(int nMaxLength) {
	m_nMaxLength = nMaxLength;
}
AFX_INLINE int CXTPTrackBlock::GetMaxLength() const {
	return m_nMaxLength;
}
AFX_INLINE BOOL CXTPTrackKey::IsKey() const {
	return TRUE;
}
AFX_INLINE int CXTPTrackMarker::GetPosition() const {
	return m_nPosition;
}
AFX_INLINE void CXTPTrackMarker::SetCaption(LPCTSTR lpszCaption) {
	m_strCaption = lpszCaption;
}
AFX_INLINE CString CXTPTrackMarker::GetCaption() const {
	return m_strCaption;
}
AFX_INLINE CRect CXTPTrackMarker::GetRect() const {
	return m_rcMarker;
}
AFX_INLINE CXTPTrackControl* CXTPTrackMarker::GetControl() const {
	return m_pControl;
}

typedef CArray<CXTPTrackBlock*, CXTPTrackBlock*> CXTPTrackBlockArray;


#endif //#if !defined(__XTPTRACKBLOCK_H__)
