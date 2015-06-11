// XTPPoint3.h
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
#if !defined(__XTPPOINT3_H__)
#define __XTPPOINT3_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Common/XTPMathUtils.h>

#include "XTPPoint2.h"

template<class T>
class CXTPPoint3T;

// CXTPPoint3T shortcut for integer point object.
typedef CXTPPoint3T<int> CXTPPoint3i;

// CXTPPoint3T shortcut for float point object.
typedef CXTPPoint3T<float> CXTPPoint3f;

// CXTPPoint3T shortcut for double point object.
typedef CXTPPoint3T<double> CXTPPoint3d;


//===========================================================================
// Summary:
//     This class represents a three-dimensional point.
// Parameters:
//     T - type of a single point coordinate component. Must be an integral type.
//===========================================================================
template<class T>
class CXTPPoint3T
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPPoint3T object with all components equal to 0.
	//-----------------------------------------------------------------------
	CXTPPoint3T()
		: X(T(0))
		, Y(T(0))
		, Z(T(0))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint3T object.
	// Parameters:
	//     x - The X coordinate.
	//     y - The Y coordinate.
	//     z - The Z coordinate.
	//-----------------------------------------------------------------------
	CXTPPoint3T(T x, T y, T z = T(0))
		: X(x)
		, Y(y)
		, Z(z)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint3T object from CXTPPoint2i point.
	// Parameters:
	//     point - A reference to a source point object.
	//-----------------------------------------------------------------------
	CXTPPoint3T(const CXTPPoint2i& point)
		: X(T(point.X))
		, Y(T(point.Y))
		, Z(T(0))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint3T object from CXTPPoint2f point.
	// Parameters:
	//     point - A reference to a source point object.
	//-----------------------------------------------------------------------
	CXTPPoint3T(const CXTPPoint2f& point)
		: X(T(point.X))
		, Y(T(point.Y))
		, Z(T(0))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint3T object from CXTPPoint2d point.
	// Parameters:
	//     point - A reference to a source point object.
	//-----------------------------------------------------------------------
	CXTPPoint3T(const CXTPPoint2d& point)
		: X(T(point.X))
		, Y(T(point.Y))
		, Z(T(0))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint3T object from CXTPPoint3i object.
	// Parameters:
	//     point - A reference to a source point object.
	//-----------------------------------------------------------------------
	CXTPPoint3T(const CXTPPoint3i& point)
		: X(T(point.X))
		, Y(T(point.Y))
		, Z(T(point.Z))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint3T object from CXTPPoint3f object.
	// Parameters:
	//     point - A reference to a source point object.
	//-----------------------------------------------------------------------
	CXTPPoint3T(const CXTPPoint3f& point)
		: X(T(point.X))
		, Y(T(point.Y))
		, Z(T(point.Z))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPoint3T object from CXTPPoint3d object.
	// Parameters:
	//     point - A reference to a source point object.
	//-----------------------------------------------------------------------
	CXTPPoint3T(const CXTPPoint3d& point)
		: X(T(point.X))
		, Y(T(point.Y))
		, Z(T(point.Z))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to CXTPPoint2i point type.
	// Returns:
	//     A converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint2i() const
	{
		return CXTPPoint2i(int(X), int(Y));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to CXTPPoint2f point type.
	// Returns:
	//     A converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint2f() const
	{
		return CXTPPoint2f(float(X), float(Y));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to CXTPPoint2i point type.
	// Returns:
	//     A converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint2d() const
	{
		return CXTPPoint2d(double(X), double(Y));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to CXTPPoint3i point type.
	// Returns:
	//     A converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint3i() const
	{
		return CXTPPoint3i(int(X), int(Y), int(Z));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to CXTPPoint3f point type.
	// Returns:
	//     A converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint3f() const
	{
		return CXTPPoint3f(float(X), float(Y), float(Z));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts point to CXTPPoint3d point type.
	// Returns:
	//     A converted point object.
	//-----------------------------------------------------------------------
	operator CXTPPoint3d() const
	{
		return CXTPPoint3d(double(X), double(Y), double(Z));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets point component values.
	// Parameters:
	//     x - The X coordinate.
	//     y - The Y coordinate.
	//     z - The Z coordinate.
	//-----------------------------------------------------------------------
	void SetPoint(T x, T y, T z = T(0))
	{
		X = x;
		Y = y;
		Z = z;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Offsets the point to another location. The differentials provided
	//     are added to each component.
	// Parameters:
	//     dx - The change in X coordinate.
	//     dy - The change in Y coordinate.
	//     dz - The change in Z coordinate.
	//-----------------------------------------------------------------------
	void Offset(T dx, T dy, T dz = T(0))
	{
		X += dx;
		Y += dy;
		Z += dz;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Rounds point to integer values.
	// Returns:
	//     A rounded CXTPPoint3T object with integer components.
	//-----------------------------------------------------------------------
	CXTPPoint3T<int> Round() const
	{
		return CXTPPoint3T<int>(
			CXTPMathUtils::Round(double(X)),
			CXTPMathUtils::Round(double(Y)),
			CXTPMathUtils::Round(double(Z)));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares two points.
	// Parameters:
	//     point - point object reference to compare to.
	// Returns:
	//     true if points are equal, otherwise the return value is false.
	//-----------------------------------------------------------------------
	bool operator == (const CXTPPoint3T& point) const
	{
		return (&point == this)
			|| (X == point.X && Y == point.Y && Z == point.Z);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares two points.
	// Parameters:
	//     point - point object reference to compare to.
	// Returns:
	//     true if points are not equal, otherwise the return value is false.
	//-----------------------------------------------------------------------
	bool operator != (const CXTPPoint3T& point) const
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
		return (T(0) == X
			&& T(0) == Y
			&& T(0) == Z);
	}

public:
	T X;       //The X coordinate.
	T Y;       //The Y coordinate.
	T Z;       //The Z coordinate.
};

#endif //#if !defined(__XTPPOINT3_H__)
