// XTPReportBorder.h: Interface of the CXTPReportBorder class.
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
#if !defined(__XTPREPORTBORDER_H__)
#define __XTPREPORTBORDER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportThickness;


enum XTPReportBorderEdge
{
	xtpReportBorderEdgeLeft,
	xtpReportBorderEdgeTop,
	xtpReportBorderEdgeRight,
	xtpReportBorderEdgeBottom,
	xtpReportBorderEdgeCount
};


class _XTP_EXT_CLASS CXTPReportBorderEdge : public CXTPCmdTarget
{
public:

	CXTPReportBorderEdge();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the line style of the border edge.
	// Returns:
	//     The line style.
	//-----------------------------------------------------------------------
	XTPReportLineStyle GetLineStyle() const;

	void SetLineStyle(XTPReportLineStyle lineStyle);

protected:

	XTPReportLineStyle m_lineStyle;
};


class _XTP_EXT_CLASS CXTPReportBorder : public CXTPCmdTarget
{
public:

	CXTPReportBorder();

	virtual ~CXTPReportBorder();

	virtual void Draw (CDC *pDC, CRect rcBorder);
	virtual void DrawH(CDC *pDC, CRect rcBorder);
	virtual void DrawV(CDC *pDC, CRect rcBorder);

	CSize GetSize() const;

	//////////////////////////////////////////////////////////////////////////
	// Properties
	//////////////////////////////////////////////////////////////////////////
public:

	CXTPReportBorderEdge* GetEdge(const XTPReportBorderEdge edge) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the border color.
	// Returns:
	//     The border color.
	//-----------------------------------------------------------------------
	COLORREF GetBorderColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the border color.
	// Parameters:
	//     crBorderColor - The border color.
	//-----------------------------------------------------------------------
	void SetBorderColor(COLORREF crBorder);

	CXTPReportThickness *GetMargin() const;

	CXTPReportThickness *GetBorderThickness() const;

	CXTPReportThickness *GetPadding() const;

	static CRect DeflateRect(CRect rcDeflate, const CXTPReportBorder *pBorder);

	//////////////////////////////////////////////////////////////////////////
	// Members
	//////////////////////////////////////////////////////////////////////////
protected:

	COLORREF m_crBackground;                        // Background color
	COLORREF m_crBorder;                                // Border color

	CXTPReportBorderEdge *m_pEdgeLeft;          // Left edge
	CXTPReportBorderEdge *m_pEdgeTop;           // Top edge
	CXTPReportBorderEdge *m_pEdgeRight;         // Right edge
	CXTPReportBorderEdge *m_pEdgeBottom;        // Bottom edge

	CXTPReportThickness *m_pMargin;             // Outer margin
	CXTPReportThickness *m_pBorderThickness;    // The border thickness
	CXTPReportThickness *m_pPadding;                // Inner padding
};

//{{AFX_CODEJOCK_PRIVATE
#endif // #if !defined(__XTPREPORTBORDER_H__)
//}}AFX_CODEJOCK_PRIVATE
