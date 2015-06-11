// XTPResizePoint.h: interface for the CXTPResizePoint class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPRESIZEPOINT_H__)
#define __XTPRESIZEPOINT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// -----------------------------------------------------------------
// Summary:
//     XTP_RESIZEPOINT structure is used by the CXTPResizePoint
//     class for defining single -precision floating point x- and
//     y-coordinates of a point.
// See Also:
//     CXTPResizePoint
// -----------------------------------------------------------------
struct XTP_RESIZEPOINT
{
	XTP_RESIZE x; // Specifies the x-coordinate of a point
	XTP_RESIZE y; // Specifies the y-coordinate of a point
};

//===========================================================================
// Summary:
//     CXTPResizePoint is an XTP_RESIZEPOINT structure derived class. The
//     CXTPResizePoint class is similar to the XTP_RESIZEPOINT structure.
//     It also includes member functions to manipulate CXTPResizePoint and
//     XTP_RESIZEPOINT structures. A CXTPResizePoint object can be used wherever
//     an XTP_RESIZEPOINT structure is used.
//===========================================================================
class _XTP_EXT_CLASS CXTPResizePoint : public XTP_RESIZEPOINT
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResizePoint object
	// Parameters:
	//     cx - Specifies the x-coordinate of a point.
	//     cy - Specifies the y-coordinate of a point.
	//-----------------------------------------------------------------------
	CXTPResizePoint();
	CXTPResizePoint(XTP_RESIZE cx, XTP_RESIZE cy); // <combine CXTPResizePoint::CXTPResizePoint>
};

//-----------------------------------------------------------------------
// Summary:
//     CXTPResizePoint constants used by the CXTPResize class for defining the
//     anchor position for a child in a resizable window.
// Parameters:
//     XTP_ANCHOR_TOPLEFT - Specifies a top left position.
//     XTP_ANCHOR_TOPCENTER - Specifies a top center position.
//     XTP_ANCHOR_TOPRIGHT - Specifies a top right position.
//     XTP_ANCHOR_MIDDLELEFT - Specifies a middle left position.
//     XTP_ANCHOR_MIDDLECENTER - Specifies a middle center position.
//     XTP_ANCHOR_MIDDLERIGHT - Specifies a middle right position.
//     XTP_ANCHOR_BOTTOMLEFT - Specifies a bottom left position.
//     XTP_ANCHOR_BOTTOMCENTER - Specifies a bottom center position.
//     XTP_ANCHOR_BOTTOMRIGHT - Specifies a bottom right position.
// Example:
//     The following example demonstrates the use of CXTPResizePoint.
// <code>
// // Set control resizing.
// SetResize(IDC_EDIT_MASKEDIT,   XTP_ANCHOR_TOPLEFT,     XTP_ANCHOR_TOPRIGHT);
// SetResize(IDC_TXT_MASK,        XTP_ANCHOR_TOPCENTER,   XTP_ANCHOR_TOPRIGHT);
// SetResize(IDC_LBOX_MASK,       XTP_ANCHOR_TOPCENTER,   XTP_ANCHOR_TOPRIGHT);
// SetResize(IDC_BTN_UPDATE,      XTP_ANCHOR_TOPCENTER,   XTP_ANCHOR_TOPCENTER);
// SetResize(IDC_EDIT_MASK,       XTP_ANCHOR_TOPLEFT,     XTP_ANCHOR_TOPCENTER);
// SetResize(IDC_EDIT_PROMPT,     XTP_ANCHOR_TOPLEFT,     XTP_ANCHOR_TOPCENTER);
// SetResize(IDC_EDIT_LITERAL,    XTP_ANCHOR_TOPLEFT,     XTP_ANCHOR_TOPCENTER);
// SetResize(IDC_EDIT_DEFAULT,    XTP_ANCHOR_TOPLEFT,     XTP_ANCHOR_TOPCENTER);
// SetResize(IDC_GBOX_PROPERTIES, XTP_ANCHOR_TOPLEFT,     XTP_ANCHOR_TOPCENTER);
// SetResize(IDCANCEL,            XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
// SetResize(IDOK,                XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
// </code>
// See Also:
//     CXTPResize, CXTPResizeDialog, CXTPResizeFormView, CXTPResizeGroupBox, CXTPResizePropertyPage, CXTPResizePropertySheet,  CXTPResizePoint, CXTPResizeRect
//-----------------------------------------------------------------------
#define XTP_ANCHOR_BOTTOMCENTER CXTPResizePoint(.5, 1)
//<COMBINE XTP_ANCHOR_BOTTOMCENTER>
#define XTP_ANCHOR_BOTTOMLEFT   CXTPResizePoint(0, 1)
//<COMBINE XTP_ANCHOR_BOTTOMCENTER>
#define XTP_ANCHOR_BOTTOMRIGHT  CXTPResizePoint(1, 1)
//<COMBINE XTP_ANCHOR_BOTTOMCENTER>
#define XTP_ANCHOR_MIDDLECENTER CXTPResizePoint(.5, .5)
//<COMBINE XTP_ANCHOR_BOTTOMCENTER>
#define XTP_ANCHOR_MIDDLELEFT   CXTPResizePoint(0, .5)
//<COMBINE XTP_ANCHOR_BOTTOMCENTER>
#define XTP_ANCHOR_MIDDLERIGHT  CXTPResizePoint(1, .5)
//<COMBINE XTP_ANCHOR_BOTTOMCENTER>
#define XTP_ANCHOR_TOPLEFT      CXTPResizePoint(0, 0)
//<COMBINE XTP_ANCHOR_BOTTOMCENTER>
#define XTP_ANCHOR_TOPRIGHT     CXTPResizePoint(1, 0)
//<COMBINE XTP_ANCHOR_BOTTOMCENTER>
#define XTP_ANCHOR_TOPCENTER    CXTPResizePoint(.5, 0)

#endif // !defined(__XTPRESIZEPOINT_H__)
