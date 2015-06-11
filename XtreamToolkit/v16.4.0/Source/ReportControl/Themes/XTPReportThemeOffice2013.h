// XTPReportThemeOffice2013.h: interface for the CXTPReportPaintManager class.
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
#if !defined(__XTPReportThemeOffice2013_H__)
#define __XTPReportThemeOffice2013_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPReportThemeOffice2013 is a CXTPReportPaintManager derived
//     class.  This represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportThemeOffice2013 : public CXTPReportPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPReportThemeOffice2013();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads default images used by report control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     report control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws text for column header.
	// Parameters:
	//     pDC              - Pointer to the used Device Context.
	//     rcText           - Size and position of the text to render.
	//     strCaption       - NULL terminated string representing caption text.
	//     nHeaderAlignment - DT_ text alignment flags.
	//     bIsHeader        - TRUE if column is to be rendered for header.
	//-----------------------------------------------------------------------
	virtual void DrawColumnText(CDC* pDC, CRect rcText, CString strCaption, int nHeaderAlignment, BOOL bIsHeader, BOOL bColumnPressed);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw background of the column using current column style
	// Parameters:
	//     pDC - Point to used Device Context
	//     pColumn - Column needs to draw
	//     rcColumn - Bounding rectangle of the column
	//     bColumnPressed - TRUE if column is pressed by user.
	//     pHeader - Pointer to a header the column belongs to.
	//-----------------------------------------------------------------------
	virtual void DrawColumnBackground(CDC* pDC,
		CXTPReportColumn* pColumn,
		CRect rcColumn,
		BOOL& bColumnPressed,
		BOOL& bDraggingFromHeader,
		CXTPReportHeader* pHeader = NULL);

	virtual void DrawPlainColumnBackground(CDC* /*pDC*/, CRect /*rcColumn*/);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws triangle that shows the sort order of column.
	// Parameters:
	//     pDC        - Pointer to the used Device Context.
	//     rcTriangle - Triangle area rectangle coordinates.
	//     bToDown    - If TRUE the top of triangle turn downward, otherwise upward.
	// Remarks:
	//     You use this function to draw a triangle image on the header of the column
	//     that is sorted. Triangle represents the direction of sorting: ascending or descending.
	//     Sizes of the triangle depend on the rectangle area that is provided to draw
	// Example:
	// <code>
	// // draw ascendant triangle
	// CRect rcTriangle;
	// rcTriangle.CopyRect(rcHeader);
	// rcTriangle.DeflateRect(40, 5, 5, 5);
	// DrawTriangle(pDC, rcTriangle, TRUE);
	//
	// // draw descendant triangle
	// CRect rcTriangle;
	// rcTriangle.CopyRect(rcHeader);
	// rcTriangle.DeflateRect(40, 5, 5, 5);
	// DrawTriangle(pDC, rcTriangle, FALSE);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, BOOL bDraggingFromHeader, int nShadowWidth, BOOL bColumnPressed);

	virtual void DrawColumnInverted(CDC *pDC, CRect rcColumn, BOOL bColumnPressed, BOOL bDraggingFromHeader, int nShadowWidth);

	virtual void DrawWindowFrame(CDC* pDC, CRect rcWindow);

	//-----------------------------------------------------------------------
	// Summary:
	//      Fill font and colors members of the metrics structure
	//      for the specified group row.
	// Parameters:
	//      pRow        - Pointer to the Group Row.
	//      pMetrics    - Pointer to the metrics structure to be filled.
	//      bPrinting   - Is printing mode.
	//-----------------------------------------------------------------------
	virtual void FillGroupRowMetrics(CXTPReportGroupRow* pRow, XTP_REPORTRECORDITEM_METRICS* pMetrics, BOOL bPrinting);

	virtual void DrawGroupRowBack(CDC* pDC, CRect rcRow, XTP_REPORTRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws group row.
	// Parameters:
	//     pDC   - Pointer to the used Device Context.
	//     pRow  - Pointer to the Group Row.
	//     rcRow - Group Row area rectangle coordinates.
	//     pMetrics - Pointer to a XTP_REPORTRECORDITEM_METRICS object.
	// Example:
	// <code>
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// pDC->SetBkMode(TRANSPARENT);
	// pPaintManager->DrawGroupRow(pDC, this, rcRow);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawGroupRow(CDC* pDC, CXTPReportGroupRow* pRow, CRect rcRow, XTP_REPORTRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws rows that were focused.
	// Parameters:
	//     pDC     - Pointer to the used Device Context.
	//     rcRow   - Area rectangle coordinates.
	// Example:
	// <code>
	// CRect rcClient(GetParent()->GetClientRect());
	// CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// GetPaintManager()->DrawResizingRect(pDC, pHeader, rcClient);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawFocusedRow(CDC* pDC, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns default row height for print mode.
	// Parameters:
	//     pDC   - Pointer to the used Device Context.
	//     pRow  - Pointer to CXTPReportRow object.
	//     nTotalWidth - Width of the row
	// Returns:
	//     The height of the default rectangle where row's items will draw.
	// Example:
	//     <code>int nRowHeight = GetRowHeight(pDC, pRow)</code>
	//-----------------------------------------------------------------------
	virtual int GetRowHeight(CDC* pDC, CXTPReportRow* pRow);

	virtual CRect GetGroupRowBmpSize(CRect rcRow, int nBitmapOffset);

	virtual CRect GetGroupRowTextSize(CRect rcBitmap, CRect rcRow, int nBitmapWidth, int nNoIconWidth, int nTextOffset);

	CXTPPaintManagerColor m_clrGroupText;
	CXTPPaintManagerColor m_clrGroupTextHilite;
	CXTPPaintManagerColor m_clrGroupBack;
	CXTPPaintManagerColor m_clrGroupBackHilite;
	CXTPPaintManagerColor m_clrGlyph;             // Header glyph color.
	CXTPPaintManagerColor m_clrGlyphHighlight;    // Header highlighted glyph color.
};

#endif //#if !defined(__XTPReportThemeOffice2013_H__)
