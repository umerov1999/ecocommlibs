// XTPSyntaxEditDrawTextProcessor.h: interface for the CXTPSyntaxEditDrawTextProcessor class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITDRAWTEXTPROCESSOR_H__)
#define __XTPSYNTAXEDITDRAWTEXTPROCESSOR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//{{AFX_CODEJOCK_PRIVATE
template<class TYPE, class ARG_TYPE>
class CXTPReserveArray : protected CArray<TYPE, ARG_TYPE>
{
protected:
	int m_nDataSize;
public:

	CXTPReserveArray()
	{
		m_nDataSize = 0;
	}

	int GetDataSize() const
	{
		return m_nDataSize;
	}

	void SetDataSize(int nDataSize, int nReservedSize = -1, int nGrowBy = -1)
	{
		ASSERT(nReservedSize == -1 || nReservedSize >= nDataSize);
		nReservedSize = max(nReservedSize, nDataSize);

		m_nDataSize = nDataSize;
		SetSize(nReservedSize, nGrowBy);
	}

	void AddData(ARG_TYPE newElement)
	{
		SetAtGrow(m_nDataSize, newElement);
		m_nDataSize++;
	}

	void RemoveAll()
	{
		m_nDataSize = 0;
	}

	TYPE operator[](int nIndex) const
	{
		ASSERT(nIndex >= 0 && nIndex < GetDataSize());
		return GetAt(nIndex);
	}
	TYPE& operator[](int nIndex)
	{
		ASSERT(nIndex >= 0 && nIndex < GetDataSize());
		return ElementAt(nIndex);
	}

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const {
		return CArray<TYPE, ARG_TYPE>::GetData();
	}

	TYPE* GetData() {
		return CArray<TYPE, ARG_TYPE>::GetData();
	}
};
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//      This helper class used by CXTPSyntaxEditCtrl as the Draw Text Processor.
//      It is responsible for drawing chars and remember each char position and
//      other text properties.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditDrawTextProcessor
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditDrawTextProcessor();

	//-----------------------------------------------------------------------
	// Summary:
	//      Get a rectangle to draw text.
	// Returns:
	//      A CRect with rectangle to draw text.
	// See Also: SetTextRect
	//-----------------------------------------------------------------------
	CRect GetTextRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set a rectangle to draw text.
	// Parameters:
	//      rcRect - [in] A CRect object with rectangle to set.
	// See Also: GetTextRect
	//-----------------------------------------------------------------------
	void SetTextRect(const CRect& rcRect);

	//-----------------------------------------------------------------------
	// Summary:
	//      Get a single row height.
	// Returns:
	//      A single row height.
	// See Also: RecalcRowHeight
	//-----------------------------------------------------------------------
	int GetRowHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this method to recalculate a single row height.
	// Parameters:
	//      pDC     : [in] A pointer to device context.
	//      pFont   : [in] A pointer to font to calculate.
	// Returns:
	//      A single row height.
	// See Also: GetRowHeight
	//-----------------------------------------------------------------------
	int RecalcRowHeight(CDC* pDC, CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//      Get a rows count visible for text rectangle.
	// Parameters:
	//      bWithPartlyVisible : If this parameter TRUE - partly visible row
	//                           will be calculated too, otherwise only full
	//                           visible rows count returned.
	// Returns:
	//      A visible rows count.
	// See Also: SetTextRect
	//-----------------------------------------------------------------------
	int GetRowsCount(BOOL bWithPartlyVisible) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get tab size.
	// Returns:
	//      A tab size.
	// See Also: SetTabSize
	//-----------------------------------------------------------------------
	int GetTabSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab size.
	// Parameters:
	//     nTabSize : [in] The tab size to set.
	// Remarks:
	//     Call this member function to set tab size. Size is measured in
	//     space character and should be between 2 to 10.
	// See also: GetTabSize
	//-----------------------------------------------------------------------
	void SetTabSize(int nTabSize);

	//-----------------------------------------------------------------------
	// Summary:
	//      Get scroll offset for x coordinate.
	// Returns:
	//      A scroll offset for x coordinate.
	// See Also: SetScrollXOffset
	//-----------------------------------------------------------------------
	int GetScrollXOffset() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get scroll offset for x coordinate.
	// Parameters:
	//      nOffsetX - A scroll offset to set.
	// See Also: GetScrollXOffset
	//-----------------------------------------------------------------------
	void SetScrollXOffset(int nOffsetX);

	//-----------------------------------------------------------------------
	// Summary:
	//      Get current text metrics.
	// Returns:
	//      A TEXTMETRIC structure.
	// See Also: RecalcRowHeight
	//-----------------------------------------------------------------------
	const TEXTMETRIC& GetTextMetrics() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Get space char width in the pixels.
	// Returns:
	//      A space char width in the pixels.
	// See Also: RecalcRowHeight
	//-----------------------------------------------------------------------
	int GetSpaceWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to align column index to tabs borders
	//      if necessary.
	// Parameters:
	//      nRow            - A zero based row index.
	//      nCol            - A zero based column index.
	//      bVirtualSpace   - Is virtual space enabled.
	// Returns:
	//      An aligned column index.
	// See Also: SetTabSize, GetTabSize
	//-----------------------------------------------------------------------
	int AlignColIdxToTabs(int nRow, int nCol, BOOL bVirtualSpace = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to obtain a specified column x position
	//      in pixels.
	// Parameters:
	//      nRow            - A zero based row index.
	//      nCol            - A zero based column index.
	//      bVirtualSpace   - Is virtual space enabled.
	//      pnChawWidth     - A pointer to int variable to receive column char
	//                        width in pixels. May be NULL.
	// Returns:
	//      A column start x position (in pixels).
	// See Also: ColFromXPos
	//-----------------------------------------------------------------------
	int GetColPosX(int nRow, int nCol, int* pnChawWidth = NULL, BOOL bVirtualSpace = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to obtain a column for specified x position
	//      in pixels.
	// Parameters:
	//      nRow            - A zero based row index.
	//      nX              - A x position (in pixels).
	//      rnCol           - [out] A zero based column index for x position.
	//      bVirtualSpace   - Is virtual space enabled.
	// Returns:
	//      TRUE if succeeded, FALSE otherwise.
	// See Also: GetColPosX, AlignColIdxToTabs
	//-----------------------------------------------------------------------
	BOOL ColFromXPos(int nRow, int nX, int& rnCol, BOOL bVirtualSpace = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to obtain width of the specified row.
	// Parameters:
	//      nRow - A zero based row index.
	// Returns:
	//      A row width in pixels.
	// See Also: GetRowsMaxWidth
	//-----------------------------------------------------------------------
	int GetRowWidth(int nRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to obtain maximum width of all visible rows.
	// Returns:
	//      A maximum row width in pixels.
	// See Also: GetRowsWidth
	//-----------------------------------------------------------------------
	int GetRowsMaxWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to obtain a row and column for specified
	//      point.
	// Parameters:
	//      pt              - A point to test.
	//      rnRow           - [out] A zero based row index for y position.
	//      rnCol           - [out] A zero based column index for x position.
	//      bVirtualSpace   - Is virtual space enabled.
	// Returns:
	//      TRUE if succeeded, FALSE otherwise.
	// See Also: GetColPosX, HitTestRow.
	//-----------------------------------------------------------------------
	BOOL HitTest(const CPoint& pt, int& rnRow, int& rnCol, BOOL bVirtualSpace = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to obtain a row for specified y position.
	// Parameters:
	//      nY              - An y position (in pixels).
	//      rnRow           - [out] A zero based row index for y position.
	// Returns:
	//      TRUE if succeeded, FALSE otherwise.
	// See Also: GetColPosX, HitTest.
	//-----------------------------------------------------------------------
	BOOL HitTestRow(int nY, int& rnRow) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to clear cached row information.
	// Parameters:
	//      nRow    - A zero based row index.
	// See Also: DrawRowPart, PrintRowPart.
	//-----------------------------------------------------------------------
	void ResetRowInfo(int nRow);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to draw row part and store characters positions.
	// Parameters:
	//      pDC      - A pointer to device context.
	//      nRow     - A zero based row index (or -1 to switch to finish current row drawing).
	//      pcszText - A string to draw.
	//      nchCount - A chars count to draw.
	// Returns:
	//      The x offset for next row part.
	// See Also:
	//      PrintRowPart
	//-----------------------------------------------------------------------
	int DrawRowPart(CDC* pDC, int nRow, LPCTSTR pcszText, int nchCount = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//      Use this member function to draw row part and store characters positions.
	// Parameters:
	//      pDC      - A pointer to device context.
	//      nRow     - A zero based row index (or -1 to switch to finish current
	//                 row drawing).
	//      nPosY    - A y offset to print specified row.
	//      nFlags   - Additional printing options. The folowing values supported:
	//                 DT_CALCRECT, DT_SINGLELINE, DT_WORDBREAK
	//      pcszText - A string to draw.
	//      nchCount - A chars count to draw.
	//      pnPrintedTextLen - A pointer to int variable to receive printed text
	//                         length.
	// Returns:
	//      Printed row height.
	// See Also:
	//      DrawRowPart
	//-----------------------------------------------------------------------
	int PrintRowPart(CDC* pDC, int nRow, int nPosY, UINT nFlags, LPCTSTR pcszText,
					 int nchCount = -1, int *pnPrintedTextLen = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Expands the character set by putting space in the position of tab.
	// Parameters:
	//      pszChars - [in] The text to be processed.
	//      strBuffer - [out] Buffer for text to be created after expansion.
	//      nDispPos - [in] The display position to start.
	//      bEnableWhiteSpace - [in] The WhiteSpace mode enabled.
	// Returns:
	//      The end display position after expansion.
	//-----------------------------------------------------------------------
	int ExpandChars(LPCTSTR pszChars, CString& strBuffer, int nDispPos = 0,
					BOOL bEnableWhiteSpace = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Remembers tabs positions for the row.
	// Parameters:
	//      nRow            - [in] A zero based row index.
	//      pcszOrigRowText - [in] The row text (not expanded).
	//-----------------------------------------------------------------------
	void SetRowTabPositions(int nRow, LPCTSTR pcszOrigRowText);

	//-----------------------------------------------------------------------
	// Summary:
	//      Convert display position to string position.
	// Parameters:
	//      nRow            - A zero based row index.
	//      nDispPos        - A zero based display column index.
	//      bVirtualSpace   - Is virtual space enabled.
	// Returns:
	//      The string position.
	//-----------------------------------------------------------------------
	int DispPosToStrPos(int nRow, int nDispPos, BOOL bVirtualSpace) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Convert string position to display position.
	// Parameters:
	//      nRow            - A zero based row index.
	//      nStrPos         - A zero based string position.
	//      bVirtualSpace   - Is virtual space enabled.
	// Returns:
	//      The display position.
	//-----------------------------------------------------------------------
	int StrPosToDispPos(int nRow, int nStrPos, BOOL bVirtualSpace = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Set caret position. In/out parameters rnCol and rnRow may be
	//      adjusted to valid values.
	// Parameters:
	//      pWnd            - [in] A pinter to caret owner window.
	//      szSize          - [in] A caret size.
	//      nRow            - [in] A zero based row index.
	//      rnCol           - [in, out] A zero based column index.
	//      bHideCaret      - [in] Hide or show caret.
	//      bVirtualSpace   - Is virtual space enabled.
	// Returns:
	//      Point of the caret position.
	//-----------------------------------------------------------------------
	CPoint SetCaretPos(CWnd* pWnd, const CSize& szSize, int nRow, int& rnCol,
					 BOOL bHideCaret = FALSE, BOOL bVirtualSpace = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//      Set caret position. In/out parameters rnCol and rnRow may be
	//      adjusted to valid values.
	// Parameters:
	//      pWnd            - [in] A pinter to caret owner window.
	//      pt              - [in] A point to set caret.
	//      szSize          - [in] A caret size.
	//      rnRow           - [in, out] A zero based row index.
	//      rnCol           - [in, out] A zero based column index.
	//      bVirtualSpace   - Is virtual space enabled.
	// Returns:
	//      Point of the caret position.
	//-----------------------------------------------------------------------
	CPoint SetCaretByPoint(CWnd* pWnd, const CPoint& pt, const CSize& szSize,
						 int& rnRow, int& rnCol, BOOL bVirtualSpace = FALSE);

public:

//{{AFX_CODEJOCK_PRIVATE
	class _XTP_EXT_CLASS CXTPRowInfo : public CCmdTarget
	{
	public:
		CXTPRowInfo()
		{
			nMaxWidth = 0;
			const int cnReservedSize = 4096;
			arCharsEnds.SetDataSize(0, cnReservedSize, cnReservedSize);

			arTabs.SetDataSize(0, cnReservedSize, cnReservedSize);
			arDispCol2StrPos.SetDataSize(0, cnReservedSize, cnReservedSize);
			arStrPos2DispCol.SetDataSize(0, cnReservedSize, cnReservedSize);
		}

		void Reset()
		{
			nMaxWidth = 0;
			arCharsEnds.RemoveAll();
		}

		int nMaxWidth;

		CXTPReserveArray<int, int> arCharsEnds;

		CXTPReserveArray<BYTE, BYTE> arTabs;
		CXTPReserveArray<int, int> arDispCol2StrPos;
		CXTPReserveArray<int, int> arStrPos2DispCol;
	};

	CXTPRowInfo* GetRowInfo(int nRow) const;
//}}AFX_CODEJOCK_PRIVATE

protected:
	int         m_nTabSize;     // Store the tab size.
	TEXTMETRIC  m_tmText;       // Store text metrics.
	int         m_nSpaceWidth;  // Store space char width.

	CRect m_rcTextRect;     // Store text rect.
	int m_nRowHeight;       // Store row height.
	int m_nScrollXOffset;   // Store scroll offset for x coordinate.

	int m_nDrawingRow;      // Store currently drawing row.
	int m_nNextRowPosX;     // Store x offset to draw next row part.

	int     m_nPrintingRow;     // Store currently printing row.
	CPoint  m_ptNextPrintPos;   // Store (x, y) offset to print next row part.
	BOOL    m_bUseOutputDC;     // Use output DC for calculations.

//{{AFX_CODEJOCK_PRIVATE
	typedef CXTPInternalCollectionT<CXTPRowInfo> CXTPRowsInfoArray; //
	mutable CXTPRowsInfoArray m_arRows;
//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	// temporary buffers
	CArray<int, int>                m_arBuf_aDx;
	CXTPReserveArray<TCHAR, TCHAR>  m_arExpandCharsBuffer;
//}}AFX_CODEJOCK_PRIVATE
};

AFX_INLINE const TEXTMETRIC& CXTPSyntaxEditDrawTextProcessor::GetTextMetrics() const {
	return m_tmText;
}
AFX_INLINE CRect CXTPSyntaxEditDrawTextProcessor::GetTextRect() const {
	return m_rcTextRect;
}
AFX_INLINE void CXTPSyntaxEditDrawTextProcessor::SetTextRect(const CRect& rcRect) {
	m_rcTextRect = rcRect;
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetRowHeight() const {
	return m_nRowHeight;
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetTabSize() const {
	return m_nTabSize;
}
AFX_INLINE void CXTPSyntaxEditDrawTextProcessor::SetTabSize(int nTabSize) {
	m_nTabSize = nTabSize;
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetScrollXOffset() const {
	return m_nScrollXOffset;
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetRowsCount(BOOL bWithPartlyVisible) const {
	int nHeight = m_rcTextRect.Height();
	int nRowH = max(1, m_nRowHeight);
	int nPartly = bWithPartlyVisible ? 1 : 0;

	return nHeight / nRowH + ((nHeight % nRowH) ? nPartly : 0);
}
AFX_INLINE int CXTPSyntaxEditDrawTextProcessor::GetSpaceWidth() const {
	return m_nSpaceWidth;
}

#endif // !defined(__XTPSYNTAXEDITDRAWTEXTPROCESSOR_H__)
