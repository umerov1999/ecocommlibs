// XTPResizeRect.h: interface for the CXTPResizeRect class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPRESIZERECT_H__)
#define __XTPRESIZERECT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------
// Summary:
//     The XTP_RESIZE data type is defined as a <i>float</i> and is
//     used for storing single-precision floating point x and y window
//     coordinates.
// See Also:
//     CXTPResizeRect, XTP_RESIZERECT, CXTPResizePoint, XTP_RESIZEPOINT
// ----------------------------------------------------------------------
typedef float XTP_RESIZE;

// ---------------------------------------------------------------------
// Summary:
//     XTP_RESIZERECT structure is used by the CXTPResizeRect class
//     for defining single -precision floating point coordinates of the
//     upper-left and lower-right corners of a rectangle.
// Example:
//     The following example demonstrates alternate ways to initialize a XTP_RESIZERECT structure:
// <code>
// XTP_RESIZERECT rc1;
// rc1.left = 20;
// rc1.top = 30;
// rc1.right = 180;
// rc1.bottom = 230;
//
// XTP_RESIZERECT rc2 = {20, 30, 180, 230};
// </code>
// See Also:
//     CXTPResizeRect, XTP_RESIZE
// ---------------------------------------------------------------------
struct XTP_RESIZERECT
{
	XTP_RESIZE left;     // Specifies the x-coordinate of the upper-left corner of a rectangle.
	XTP_RESIZE top;      // Specifies the y-coordinate of the upper-left corner of a rectangle.
	XTP_RESIZE right;    // Specifies the x-coordinate of the lower-right corner of a rectangle.
	XTP_RESIZE bottom;   // Specifies the y-coordinate of the lower-right corner of a rectangle.
};

//===========================================================================
// Summary:
//     CXTPResizeRect is an XTP_RESIZERECT structure derived class. The
//     CXTPResizeRect class is similar to an XTP_RESIZERECT structure. CXTPResizeRect
//     also includes member functions to manipulate CXTPResizeRect objects and
//     XTP_RESIZERECT structures.
// See Also:
//     XTP_RESIZERECT, CXTPResizePoint, XTP_RESIZEPOINT, XTP_RESIZE
//===========================================================================
class _XTP_EXT_CLASS CXTPResizeRect: public XTP_RESIZERECT
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResizeRect object
	// Parameters:
	//     rc - Refers to the RECT structure with the coordinates for CXTPResizeRect.
	//     rrc - Refers to the XTP_RESIZERECT structure with the coordinates for
	//           CXTPResizeRect.
	//     l - Specifies the left position of CXTPResizeRect.
	//     t - Specifies the top of CXTPResizeRect.
	//     r - Specifies the right position of CXTPResizeRect.
	//     b - Specifies the bottom of CXTPResizeRect.
	//-----------------------------------------------------------------------
	CXTPResizeRect();
	CXTPResizeRect(const RECT& rc); // <combine CXTPResizeRect::CXTPResizeRect>
	CXTPResizeRect(const XTP_RESIZERECT& rrc); // <combine CXTPResizeRect::CXTPResizeRect>
	CXTPResizeRect(XTP_RESIZE l, XTP_RESIZE t, XTP_RESIZE r, XTP_RESIZE b); // <combine CXTPResizeRect::CXTPResizeRect>

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator copies the dimensions of a rectangle to CXTPResizeRect.
	// Parameters:
	//     rc - Refers to a source rectangle. It can be a RECT or CRect.
	// Returns:
	//     A reference to a CXTPResizeRect object.
	//-----------------------------------------------------------------------
	CXTPResizeRect& operator=(const RECT& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator copies the dimensions of a rectangle to CXTPResizeRect.
	// Parameters:
	//     rrc - Refers to a source rectangle. It can be a XTP_RESIZERECT or CXTPResizeRect.
	// Returns:
	//     A reference to a CXTPResizeRect object.
	//-----------------------------------------------------------------------
	CXTPResizeRect& operator=(const XTP_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator adds the specified offsets to CXTPResizeRect or inflates
	//     CXTPResizeRect.
	// Parameters:
	//     rrc - Points to an XTP_RESIZERECT structure or a CXTPResizeRect object that
	//           contains the number of units to inflate each side of CXTPResizeRect.
	// Returns:
	//     A reference to a CXTPResizeRect object.
	//-----------------------------------------------------------------------
	CXTPResizeRect& operator+=(const XTP_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator adds the given offsets to CRect or inflates CRect.
	// Parameters:
	//     rrc - Points to an XTP_RESIZERECT structure or a CXTPResizeRect object that
	//           contains the number of units to inflate each side of the return value.
	// Returns:
	//     The resulting CXTPResizeRect object.
	//-----------------------------------------------------------------------
	CXTPResizeRect operator+(const XTP_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator creates the intersection of CXTPResizeRect and a rectangle,
	//     and returns the resulting CXTPResizeRect.
	// Parameters:
	//     rrc - Contains an XTP_RESIZERECT or a CXTPResizeRect.
	// Returns:
	//     A CXTPResizeRect that is the intersection of CXTPResizeRect and 'rrc'. The
	//     intersection is the largest rectangle that is contained in both rectangles.
	//-----------------------------------------------------------------------
	CXTPResizeRect operator & (const XTP_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator determines whether CXTPResizeRect is equal to a rectangle.
	// Parameters:
	//     rrc - Refers to a source rectangle. It can be an XTP_RESIZERECT or a CXTPResizeRect.
	// Returns:
	//     true if equal, otherwise returns false.
	//-----------------------------------------------------------------------
	bool operator==(const XTP_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator determines whether CXTPResizeRect is not equal to a rectangle.
	// Parameters:
	//     rrc - Refers to a source rectangle. It can be an XTP_RESIZERECT or a CXTPResizeRect.
	// Returns:
	//     false if equal, otherwise returns true.
	//-----------------------------------------------------------------------
	bool operator!= (const XTP_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator converts a CXTPResizeRect to a CRect. When you use this
	//     function, you do not need the address-of (&) operator. This operator
	//     will be automatically used when you pass a CXTPResizeRect object to
	//     a function that expects a CRect.
	//-----------------------------------------------------------------------
	operator CRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines if CXTPResizeRect is normalized.
	// Returns:
	//     true if normalized, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsNormalized();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function calculates the width of a CXTPResizeRect by subtracting
	//     the left value from the right value. The resulting value can be negative.
	// Returns:
	//     The width of a CXTPResizeRect.
	//-----------------------------------------------------------------------
	XTP_RESIZE Width();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function calculates the height of a CXTPResizeRect by subtracting
	//     the top value from the bottom value. The resulting value can be negative.
	// Returns:
	//     The height of a CXTPResizeRect.
	//-----------------------------------------------------------------------
	XTP_RESIZE Height();
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPResizeRect::CXTPResizeRect(const RECT& rc) {
	(operator=)(rc);
}
AFX_INLINE CXTPResizeRect::CXTPResizeRect(const XTP_RESIZERECT& rrc) {
	(operator=)(rrc);
}
AFX_INLINE CXTPResizeRect CXTPResizeRect::operator+(const XTP_RESIZERECT& rrc) {
	return CXTPResizeRect(left + rrc.left, top + rrc.top, right + rrc.right, bottom + rrc.bottom);
}
AFX_INLINE bool CXTPResizeRect::IsNormalized() {
	return ((left <= right) && (top <= bottom));
}
AFX_INLINE bool CXTPResizeRect::operator==(const XTP_RESIZERECT& rrc) {
	return left == rrc.left && top == rrc.top && right == rrc.right && bottom == rrc.bottom;
}
AFX_INLINE bool CXTPResizeRect::operator!= (const XTP_RESIZERECT& rrc) {
	return !operator==(rrc);
}
AFX_INLINE CXTPResizeRect::operator CRect() {
	return CRect((int)left, (int)top, (int)right, (int)bottom);
}
AFX_INLINE XTP_RESIZE CXTPResizeRect::Width() {
	return right - left;
}
AFX_INLINE XTP_RESIZE CXTPResizeRect::Height() {
	return bottom - top;
}

//-----------------------------------------------------------------------
// Summary:
//     CXTPResizeRect constants used by the CXTPResize class for defining the
//     resize attributes for a child in a resizable window.
// Parameters:
//     XTP_ATTR_RESIZE - Resize.
//     XTP_ATTR_REPOS - Reposition.
//     XTP_ATTR_HORRESIZE - Horizontal resize.
//     XTP_ATTR_HORREPOS - Horizontal reposition.
//     XTP_ATTR_VERRESIZE - Vertical resize.
//     XTP_ATTR_VERREPOS - Vertical reposition.
// Example:
//     The following example demonstrates how to use CXTPResizeRect.
// <code>
// // Set control resizing.
// SetResize(ID_WIZBACK, XTP_ATTR_REPOS(1));
// SetResize(ID_WIZNEXT, XTP_ATTR_REPOS(1));
// SetResize(ID_WIZFINISH, XTP_ATTR_REPOS(1));
// SetResize(ID_WIZLINE, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMRIGHT);
// </code>
// See Also:
//     CXTPResize, CXTPResizeDialog, CXTPResizeFormView, CXTPResizeGroupBox, CXTPResizePropertyPage, CXTPResizePropertySheet,  CXTPResizePoint, CXTPResizeRect
//-----------------------------------------------------------------------
#define XTP_ATTR_RESIZE(x)    CXTPResizeRect(0, 0, x, x)
// <COMBINE XTP_ATTR_RESIZE>
#define XTP_ATTR_REPOS(x)     CXTPResizeRect(x, x, x, x)
// <COMBINE XTP_ATTR_RESIZE>
#define XTP_ATTR_HORRESIZE(x) CXTPResizeRect(0, 0, x, 0)
// <COMBINE XTP_ATTR_RESIZE>
#define XTP_ATTR_HORREPOS(x)  CXTPResizeRect(x, 0, x, 0)
// <COMBINE XTP_ATTR_RESIZE>
#define XTP_ATTR_VERRESIZE(x) CXTPResizeRect(0, 0, 0, x)
// <COMBINE XTP_ATTR_RESIZE>
#define XTP_ATTR_VERREPOS(x)  CXTPResizeRect(0, x, 0, x)

#endif // !defined(__XTPRESIZERECT_H__)
