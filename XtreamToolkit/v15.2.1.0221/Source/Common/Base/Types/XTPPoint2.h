// XTPPoint2.h
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

#if !defined(__XTPPOINT2_H__)
#define __XTPPOINT_H2__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class _XTP_EXT_CLASS CXTPPoint2i
{
public:
	CXTPPoint2i()
		: X(0)
		, Y(0)
	{
	}

	CXTPPoint2i(int x, int y)
		: X(x)
		, Y(y)
	{
	}


public:

	int X;
	int Y;
};


//===========================================================================
// Summary:
//     This class abstracts a Point entity and its operations.This object composed
//     of two float values for x and y coordinates.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPPoint2f
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor, creates a CXTPPoint2 object and initializes
	//     the values.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint2f();

	CXTPPoint2f(int x, int y);

	CXTPPoint2f(float x, float y);

	CXTPPoint2f(double x, double y);

	CXTPPoint2f(const CXTPPoint2i &other)
		: X(float(other.X))
		, Y(float(other.Y))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Copy constructor, creates a CXTPPoint2 object and initializes
	//     the values.
	// Parameters:
	//     size - A reference to a CXTPPoint2 object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint2f(const CXTPPoint2f &other)
		: X(other.X)
		, Y(other.Y)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds two CXTPPoint2 objects.
	// Parameters:
	//     point - A reference to a CXTPPoint2 object.
	// Returns:
	//     A CXTPPoint2 object which contain the result of operation.
	//-----------------------------------------------------------------------
	CXTPPoint2f operator+(const CXTPPoint2f &other) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Subtracts two CXTPPoint2 objects.
	// Parameters:
	//     point - A reference to a CXTPPoint2 object.
	// Returns:
	//     A CXTPPoint2 object which contain the result of operation.
	//-----------------------------------------------------------------------
	CXTPPoint2f operator-(const CXTPPoint2f &other) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a CXTPPoint2 object.
	// Parameters:
	//     point - A reference to a CXTPPoint2 object.
	// Returns:
	//     A CXTPPoint2 object which contain the result of operation.
	//-----------------------------------------------------------------------
	CXTPPoint2f& operator+=(const CXTPPoint2f &other);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to know whether the two CXTPPoint2 objects are
	//     equal.
	// Parameters:
	//     point - A reference to a CXTPPoint2 object.
	// Returns:
	//     TRUE if the objects are equal and FALSE if they are unequal.
	// Remarks:
	//     It is a constant member function.
	//-----------------------------------------------------------------------
	bool operator==(const CXTPPoint2f &other) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to round the point values to nearest integer.
	// Returns:
	//     A CPoint object which contain integer values for x and y coordinates.
	// Remarks:
	//     It is a constant member function.
	//-----------------------------------------------------------------------

	CXTPPoint2i Round() const
	{
		return CXTPPoint2i(int(X), int(Y));
	}

public:

	float X;     //The x coordinate.
	float Y;     //The y coordinate.
};


#endif // !defined(__XTPPOINT_H2__)
