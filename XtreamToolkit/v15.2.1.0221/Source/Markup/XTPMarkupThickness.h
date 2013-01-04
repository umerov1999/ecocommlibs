// XTPMarkupThickness.h: interface for the CXTPMarkupThickness class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPMARKUPTHICKNESS_H__)
#define __XTPMARKUPTHICKNESS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupObject;
class CXTPMarkupBuilder;

//===========================================================================
// Summary:
//     The CXTPMarkupThickness class describes the thickness of the
//     left, top, right and bottom side of a rectangle.
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupThickness : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupThickness);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupThickness object and sets
	//     the left, top, right and bottom thickness to 0.
	//-----------------------------------------------------------------------
	CXTPMarkupThickness();

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupThickness object and sets
	//     the left, top, right and bottom thickness.
	// Parameters:
	//     other - The left, top, right and bottom thickness.
	//-----------------------------------------------------------------------
	CXTPMarkupThickness(const CXTPMarkupThickness &other);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPThickness object and sets
	//     the left, top, right and bottom thickness.
	// Parameters:
	//     nThickness - The left, top, right and bottom thickness.
	//-----------------------------------------------------------------------
	CXTPMarkupThickness(long nThickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPThickness object and sets
	//     the left, top, right and bottom thickness.
	// Parameters:
	//     nLeft   - The left thickness.
	//     nTop    - The top thickness.
	//     nRight  - The right thickness.
	//     nBottom - The bottom thickness.
	//-----------------------------------------------------------------------
	CXTPMarkupThickness(long nLeft, long nTop, long nRight, long nBottom);

public:
	static CXTPMarkupThickness* AFX_CDECL CreateValue();

	CSize GetSize() const;

	BOOL IsZero() const;

	static CRect AFX_CDECL HelperDeflateRect(CRect rt, CXTPMarkupThickness* thick);

	CXTPMarkupObject* ConvertFrom(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the left thickness.
	// Returns:
	//     The left thickness.
	//-----------------------------------------------------------------------
	long GetLeft() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the left thickness.
	// Parameters:
	//     nThickness - The thickness.
	//-----------------------------------------------------------------------
	void SetLeft(long nThickness);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the top thickness.
	// Returns:
	//     The top thickness.
	//-----------------------------------------------------------------------
	long GetTop() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the right thickness.
	// Returns:
	//     The right thickness.
	//-----------------------------------------------------------------------
	long GetRight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the bottom thickness.
	// Returns:
	//     The bottom thickness.
	//-----------------------------------------------------------------------
	long GetBottom() const;


	virtual BOOL IsEqual(const CXTPMarkupObject *pObject) const;



protected:
	long left;
	long top;
	long right;
	long bottom;
};


AFX_INLINE long CXTPMarkupThickness::GetLeft() const {
	return left;
}
AFX_INLINE long CXTPMarkupThickness::GetTop() const {
	return top;
}
AFX_INLINE long CXTPMarkupThickness::GetRight() const {
	return right;
}
AFX_INLINE long CXTPMarkupThickness::GetBottom() const {
	return bottom;
}
AFX_INLINE CSize CXTPMarkupThickness::GetSize() const {
	return CSize(left + right, top + bottom);
}
AFX_INLINE BOOL CXTPMarkupThickness::IsZero() const {
	return left == 0 && right == 0 && top == 0 && bottom == 0;
}


#endif // #if !defined(__XTPMARKUPTHICKNESS_H__)
