// XTPResizeRect.cpp: implementation of the CXTPResizeRect class.
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

#include "stdafx.h"

#include "XTPResizeRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPResizeRect::CXTPResizeRect()
{
	left = top = right = bottom = 0;
}

//////////////////////////////////////////////////////////////////////

CXTPResizeRect::CXTPResizeRect(XTP_RESIZE l, XTP_RESIZE t, XTP_RESIZE r, XTP_RESIZE b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

CXTPResizeRect& CXTPResizeRect::operator=(const RECT& rc)
{
	left = (XTP_RESIZE) rc.left;
	top = (XTP_RESIZE) rc.top;
	right = (XTP_RESIZE) rc.right;
	bottom = (XTP_RESIZE) rc.bottom;

	return *this;
}

CXTPResizeRect& CXTPResizeRect::operator=(const XTP_RESIZERECT& rrc)
{
	left = rrc.left;
	top = rrc.top;
	right = rrc.right;
	bottom = rrc.bottom;

	return *this;
}

CXTPResizeRect& CXTPResizeRect::operator+=(const XTP_RESIZERECT& rrc)
{
	left += rrc.left;
	top += rrc.top;
	right += rrc.right;
	bottom += rrc.bottom;

	return *this;
}

CXTPResizeRect CXTPResizeRect::operator & (const XTP_RESIZERECT& rrc)
{
	ASSERT(IsNormalized() && ((CXTPResizeRect&) rrc).IsNormalized());

	CXTPResizeRect rrcI(
		__max(left, rrc.left),
		__max(top, rrc.top),
		__min(right, rrc.right),
		__min(bottom, rrc.bottom));

	// only intersection if resulting rect is normalized
	return (rrcI.IsNormalized()) ? rrcI : CXTPResizeRect(0, 0, 0, 0);
}
