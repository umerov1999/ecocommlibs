// XTPTrackThemeOffice2003.h: interface for the CXTPTrackPaintManager class.
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
#if !defined(__XTPTRACKTHEMEOffice2003_H__)
#define __XTPTRACKTHEMEOffice2003_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPTrackThemeOffice2003 is a CXTPTrackPaintManager derived
//     class.  This represents an Office 2003 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeOffice2003 : public CXTPTrackPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeOffice2003();

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
	//     Draws column header with all related attributes (sort order, icon, etc).
	// Parameters:
	//     pDC           - Pointer to the used Device Context.
	//     pColumn       - Column header area rectangle coordinates.
	//     pHeader       - Pointer to report header.
	//     rcColumn      - Column area rectangle coordinates.
	//     bDrawExternal - whether draw Column on Header area, default is FALSE.
	//     nShadowWidth  - size in pixels for bottom shadow, default is 0.
	// Example:
	// <code>
	// CXTPTrackPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CXTPReportColumn* pColumn = CXTPReportColumn* pColumn = m_pColumns->GetAt(1);
	// CRect rcHeader(rcClient);
	// int nHeaderHeight = m_pControl->GetHeaderHeight();
	// rcHeader.bottom = rcHeader.top + nHeaderHeight;
	// GetPaintManager()->DrawColumn(pDC, pColumn, this, rcHeader);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawColumn(CDC* pDC, CXTPReportColumn* pColumn, CXTPReportHeader* pHeader,
							CRect rcColumn, BOOL bDrawExternal = FALSE, int nShadowWidth = 0);

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

#endif //#if !defined(__XTPTRACKTHEMEOffice2003_H__)
