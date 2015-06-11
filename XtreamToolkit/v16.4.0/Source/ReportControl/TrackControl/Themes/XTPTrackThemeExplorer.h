// XTPTrackThemeExplorer.h: interface for the CXTPTrackPaintManager class.
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
#if !defined(__XTPTRACKTHEMEExplorer_H__)
#define __XTPTRACKTHEMEExplorer_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPTrackThemeExplorer is a CXTPTrackPaintManager derived
//     class.  This represents an Visual Studio 2005 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeExplorer : public CXTPTrackPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeExplorer();

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
	virtual void DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, int nShadowWidth = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw standard glyphs of report control
	// Parameters:
	//     pDC      - Pointer to the used Device Context.
	//     rcColumn - Bitmap area rectangle coordinates.
	//     iImage   - Image index.
	// Returns:
	//     Size of drawn bitmap.
	//-----------------------------------------------------------------------
	virtual CSize DrawGlyph(CDC* pDC, CRect rcColumn, int iImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if hot tracking is enabled for current column style.
	// Returns:
	//     TRUE if enabled; FALSE otherwise
	// Remarks:
	//     Hot Tracking can be used only for xtpReportColumnOffice2003 and xtpReportColumnExplorer styles.
	// See Also:
	//     m_columnStyle, m_bHotTracking
	//-----------------------------------------------------------------------
	virtual BOOL IsColumHotTrackingEnabled() const;

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

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the Fixed Rows divider (for ex. dividing header record rows and body rows).
	// Parameters:
	//     pDC   - Pointer to the used Device Context.
	//     rc - Rectangle coordinates.
	//     pControl - Pointer to a report control.
	//     bHeaderRows - whether the divider is under header rows or above footer rows.
	//     bVScrollBarVisible - TRUE, if vertical scrollbar is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	void DrawSectionDivider(CDC* pDC,
		const CRect& rc,
		XTPReportSectionDividerPosition dividerPosition,
		XTPReportSectionDividerStyle    dividerStyle,
		CXTPPaintManagerColor           dividerColor);
};

#endif //#if !defined(__XTPTRACKTHEMEExplorer_H__)
