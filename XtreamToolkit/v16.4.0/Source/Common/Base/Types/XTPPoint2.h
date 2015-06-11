// XTPPoint2.h
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
#if !defined(__XTPPOINT2_H__)
#define __XTPPOINT2_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

template<class T>
class CXTPPoint2T;

// CXTPPoint2T shortcut for integer point object.
typedef CXTPPoint2T<int> CXTPPoint2i;

// CXTPPoint2T shortcut for float point object.
typedef CXTPPoint2T<float> CXTPPoint2f;

// CXTPPoint2T shortcut for double point object.
typedef CXTPPoint2T<double> CXTPPoint2d;

//===========================================================================
// Summary:
//     This class represents a two-dimensional point.
// Parameters:
//     T - type of a single point coordinate component. Must be an integral type.
//===========================================================================
template<class T>
class CXTPPoint2T
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPPoint2T object with all components equal to 0.
	//-----------------------------------------------------------------------
	CXTPPoint2T()
		: X(T(0))
		, Y(T(0))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint2T object from another CXTPPoint2T object.
	// Parameters:
	//     point - A reference to a source point object.
	//-----------------------------------------------------------------------
	CXTPPoint2T(const CXTPPoint2T& point)
		: X(point.X)
		, Y(point.Y)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint2T object.
	// Parameters:
	//     x - The X coordinate.
	//     y - The Y coordinate.
	//-----------------------------------------------------------------------
	CXTPPoint2T(int x, int y)
		: X(T(x))
		, Y(T(y))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint2T object.
	// Parameters:
	//     x - The X coordinate.
	//     y - The Y coordinate.
	//-----------------------------------------------------------------------
	CXTPPoint2T(float x, float y)
		: X(T(x))
		, Y(T(y))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint2T object.
	// Parameters:
	//     x - The X coordinate.
	//     y - The Y coordinate.
	//-----------------------------------------------------------------------
	CXTPPoint2T(double x, double y)
		: X(T(x))
		, Y(T(y))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets point component values.
	// Parameters:
	//     x - The X coordinate.
	//     y - The Y coordinate.
	//-----------------------------------------------------------------------
	void SetPoint(T x, T y)
	{
		X = x;
		Y = y;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Offsets the point to another location. The differentials provided
	//     are added to each component.
	// Parameters:
	//     dx - The change in X coordinate.
	//     dy - The change in Y coordinate.
	//-----------------------------------------------------------------------
	void Offset(T dx, T dy)
	{
		X += dx;
		Y += dy;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Rounds point to integer values.
	// Returns:
	//     A rounded CXTPPoint2T object with integer components.
	//-----------------------------------------------------------------------
	CXTPPoint2T<int> Round() const
	{
		return CXTPPoint2T<int>(
			CXTPMathUtils::Round(double(X)),
			CXTPMathUtils::Round(double(Y)));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares two points.
	// Parameters:
	//     point - point object reference to compare to.
	// Returns:
	//     true if points are equal, otherwise the return value is false.
	//-----------------------------------------------------------------------
	bool operator == (const CXTPPoint2T& point) const
	{
		return (&point == this)
			|| (X == point.X && Y == point.Y);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares two points.
	// Parameters:
	//     point - point object reference to compare to.
	// Returns:
	//     true if points are not equal, otherwise the return value is false.
	//-----------------------------------------------------------------------
	bool operator != (const CXTPPoint2T& point) const
	{
		return !(*this == point);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the point object is zero, i.e. each component is equal to 0.
	// Returns:
	//     true if the point object is empty, false otherwise.
	//-----------------------------------------------------------------------
	bool IsZero() const
	{
		return (T(0) == X && T(0) == Y);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to point with integer components.
	// Returns:
	//     Converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint2i() const
	{
		return CXTPPoint2i(int(X), int(Y));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to point with float components.
	// Returns:
	//     Converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint2f() const
	{
		return CXTPPoint2f(float(X), float(Y));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to point with double components.
	// Returns:
	//     Converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint2d() const
	{
		return CXTPPoint2d(double(X), double(Y));
	}

public:
	T X;       //The X coordinate.
	T Y;       //The Y coordinate.
};


//{{AFX_CODEJOCK_PRIVATE
#endif // !defined(__XTPPOINT2_H__)
//}}AFX_CODEJOCK_PRIVATE
