//CXTPScrollBarPaintManager.h: interface for the CXTPScrollBarPaintManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPSCROLLBARPAINTMANAGER_H__)
#define __XTPSCROLLBARPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPScrollBase;

//===========================================================================
// Summary:
//     CXTPScrollBarPaintManager is standalone class used to draw CXTPCommandBarScrollBarCtrl object
//===========================================================================
class _XTP_EXT_CLASS CXTPScrollBarPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPScrollBarPaintManager object
	//-----------------------------------------------------------------------
	CXTPScrollBarPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPScrollBarPaintManager object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPScrollBarPaintManager() = 0;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw scrollbar of the gallery
	// Parameters:
	//     pDC - Pointer to device context
	//     pScrollBar - ScrollBar to draw
	//-----------------------------------------------------------------------
	virtual void DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar);

	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates gallery metrics
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:

	struct XTPScrollMetricsV
	{
		BOOL  bEnabled;
		CRect rcScrollBar;
		CRect rcArrowUp;
		CRect rcArrowDown;
		CRect rcTrack;
		CRect rcLowerTrack;
		CRect rcBtnTrack;
		CRect rcUpperTrack;
	};

	struct XTPScrollMetricsH
	{
		BOOL  bEnabled;
		CRect rcScrollBar;
		CRect rcArrowLeft;
		CRect rcArrowRight;
		CRect rcTrack;
		CRect rcLowerTrack;
		CRect rcBtnTrack;
		CRect rcUpperTrack;
	};

	static void CalcVScroll(CXTPScrollBase *pScroll, XTPScrollMetricsV *pVScroll);
	static void CalcHScroll(CXTPScrollBase *pScroll, XTPScrollMetricsH *pHScroll);

protected:

	enum XTPArrowGlyph
	{
		xtpArrowGlyphLeft,
		xtpArrowGlyphRight,
		xtpArrowGlyphUp,
		xtpArrowGlyphDown,
		xtpArrowGlyphCount
	};

	void DrawArrowGlyph(CDC *pDC, CRect rcArrow, XTPArrowGlyph glyph, BOOL bEnabled, COLORREF clrText = COLORREF_NULL);

public:
	int m_cxHScroll;            // Width, in pixels, of the arrow bitmap on a horizontal scroll bar
	int m_cyHScroll;            // Height, in pixels, of a horizontal scroll bar.

	int m_cyVScroll;            // Height, in pixels, of the arrow bitmap on a vertical scroll bar.
	int m_cxVScroll;            // Width, in pixels, of a vertical scroll bar;

	int m_cThumb;               // Width of thumb button.

	COLORREF m_crBackPushed;    // Color when pushed.
	COLORREF m_crBackHilite;    // Color when border.
	COLORREF m_crBorderHilite;  // Color of border when highlighted.
	COLORREF m_crBorder;        // Color of border.
	COLORREF m_crBack;          // Color of background.
};

#endif //#if !defined(__XTPSCROLLBARPAINTMANAGER_H__)