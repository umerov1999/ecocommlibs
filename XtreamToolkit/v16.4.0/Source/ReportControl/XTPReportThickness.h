// XTPReportThickness.h: Interface of the CXTPReportThickness class.
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
#if !defined(__XTPREPORTTHICKNESS_H__)
#define __XTPREPORTTHICKNESS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===========================================================================
// Summary:
//     The CXTPReportThickness class describes the thickness of the
//     left, top, right and bottom side of a rectangle.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportThickness : public CXTPCmdTarget
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportThickness object and sets
	//     the left, top, right and bottom thickness to 0.
	//-----------------------------------------------------------------------
	CXTPReportThickness();

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportThickness object and sets
	//     the left, top, right and bottom thickness.
	// Parameters:
	//     thickness - The left, top, right and bottom thickness.
	//-----------------------------------------------------------------------
	CXTPReportThickness(const CXTPReportThickness &thickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportThickness object and sets
	//     the left, top, right and bottom thickness.
	// Parameters:
	//     nThickness - The left, top, right and bottom thickness.
	//-----------------------------------------------------------------------
	CXTPReportThickness(const int nThickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportThickness object and sets
	//     the left, top, right and bottom thickness.
	// Parameters:
	//     nLeft   - The left thickness.
	//     nTop    - The top thickness.
	//     nRight  - The right thickness.
	//     nBottom - The bottom thickness.
	//-----------------------------------------------------------------------
	CXTPReportThickness(
		const int nLeft,
		const int nTop,
		const int nRight,
		const int nBottom
		);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the left, top, right and bottom thickness.
	// Parameters:
	//     thickness - The left, top, right and bottom thickness.
	//-----------------------------------------------------------------------
	void Set(const CXTPReportThickness &thickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the left, top, right and bottom thickness.
	// Parameters:
	//     nThickness - The left, top, right and bottom thickness.
	//-----------------------------------------------------------------------
	void Set(const int nThickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the left, top, right and bottom thickness.
	// Parameters:
	//     nLeft   - The left thickness.
	//     nTop    - The top thickness.
	//     nRight  - The right thickness.
	//     nBottom - The bottom thickness.
	//-----------------------------------------------------------------------
	void Set(
		const int nLeft,
		const int nTop,
		const int nRight,
		const int nBottom
		);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the left thickness.
	// Returns:
	//     The left thickness.
	//-----------------------------------------------------------------------
	int GetLeft() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the left thickness.
	// Parameters:
	//     nThickness - The thickness.
	//-----------------------------------------------------------------------
	void SetLeft(const int nThickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the top thickness.
	// Returns:
	//     The top thickness.
	//-----------------------------------------------------------------------
	int GetTop() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the top thickness.
	// Parameters:
	//     nThickness - The thickness.
	//-----------------------------------------------------------------------
	void SetTop(const int nThickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the right thickness.
	// Returns:
	//     The right thickness.
	//-----------------------------------------------------------------------
	int GetRight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the right thickness.
	// Parameters:
	//     nThickness - The thickness.
	//-----------------------------------------------------------------------
	void SetRight(const int nThickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the bottom thickness.
	// Returns:
	//     The bottom thickness.
	//-----------------------------------------------------------------------
	int GetBottom() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the bottom thickness.
	// Parameters:
	//     nThickness - The thickness.
	//-----------------------------------------------------------------------
	void SetBottom(const int nThickness);

	CSize GetSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the left, top, right and bottom thickness.
	// Parameters:
	//     thickness - The left, top, right and bottom thickness.
	//-----------------------------------------------------------------------
	CXTPReportThickness& operator = (const CXTPReportThickness &thickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the left, top, right and bottom thickness.
	// Parameters:
	//     nThickness - The left, top, right and bottom thickness.
	//-----------------------------------------------------------------------
	CXTPReportThickness& operator = (const int nThickness);

	static CRect DeflateRect(CRect rcDeflate, const CXTPReportThickness *pThickness);

protected:

	int m_nLeft;        // Left thickness
	int m_nTop;         // Top thickness
	int m_nRight;       // Right thickness
	int m_nBottom;      // Bottom thickness
};

//{{AFX_CODEJOCK_PRIVATE
#endif // #if !defined(__XTPREPORTTHICKNESS_H__)
//}}AFX_CODEJOCK_PRIVATE
