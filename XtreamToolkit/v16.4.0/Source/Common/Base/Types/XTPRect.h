// XTPRect.h
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
#if !defined(__XTPRECT_H__)
#define __XTPRECT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Common/XTPMathUtils.h>

//{{AFX_CODEJOCK_PRIVATE
template<class T>
class CXTPEpsilon
{
	static const T Epsilon;
};
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This class abstracts a rectangular entity and its operations.
//     This object is composed of four of specified type T for x and y 
//     coordinates and width and height of a rectangle.
// Parameters:
//     T - type of each rectangle component.
//===========================================================================
template<class T>
class CXTPRectT
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor, creates a CXTPRectT object and initializes
	//     the values to zero.
	//-----------------------------------------------------------------------
	CXTPRectT()
		: X(T(0))
		, Y(T(0))
		, Width(T(0))
		, Height(T(0))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies rectangle from another rectangle.
	// Parameters:
	//     other - a reference to a rectangle to copy from.
	//-----------------------------------------------------------------------
	CXTPRectT(const CXTPRectT& other)
		: X(other.X)
		, Y(other.Y)
		, Width(other.Width)
		, Height(other.Height)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a CXTPRectT object from
	//     CRect type object and initializes the values.
	// Parameters:
	//     rc - A reference to a CRect.
	//-----------------------------------------------------------------------
	CXTPRectT(const CRect& rc)
		: X(T(rc.left))
		, Y(T(rc.top))
		, Width(T(rc.Width()))
		, Height(T(rc.Height()))
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a CXTPRectT object.
	// Parameters:
	//     x      - The x coordinate.
	//     y      - The y coordinate.
	//     width  - The width of the rectangle.
	//     height - The height of the rectangle.
	//-----------------------------------------------------------------------
	CXTPRectT(T x, T y, T width, T height)
		: X(x)
		, Y(y)
		, Width(width)
		, Height(height)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a CXTPRectT object from point
	//     and a size objects.
	// Parameters:
	//     location - The rectangle location.
	//     size     - The size of the rectangle.
	//-----------------------------------------------------------------------
	CXTPRectT(const CXTPPoint2T<T>& location, const CXTPSizeT<T>& size)
		: X(location.X)
		, Y(location.Y)
		, Width(size.Width)
		, Height(size.Height)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a CXTPRectT object from top-left
	//     and bottom-right point objects.
	// Parameters:
	//     ptTopLeft - The top-left rectangle coordinates.
	//     ptBottomRight - The bottom-right rectangle coordinates.
	//-----------------------------------------------------------------------
	CXTPRectT(const CXTPPoint2T<T>& ptTopLeft, const CXTPPoint2T<T>& ptBottomRight)
		: X(ptTopLeft.X)
		, Y(ptTopLeft.Y)
		, Width(ptBottomRight.X - ptTopLeft.X)
		, Height(ptBottomRight.Y - ptTopLeft.Y)
	{
		Normalize();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new rectangle data from from CRect type object and 
	//     initializes the values.
	// Parameters:
	//     rc - A reference to a CRect.
	//-----------------------------------------------------------------------
	void SetRect(const CRect& rc)
	{
		X = T(rc.left);
		Y = T(rc.top);
		Width = T(rc.Width());
		Height = T(rc.Height());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new rectangle data.
	// Parameters:
	//     x      - The x coordinate.
	//     y      - The y coordinate.
	//     width  - The width of the rectangle.
	//     height - The height of the rectangle.
	//-----------------------------------------------------------------------
	void SetRect(T x, T y, T width, T height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new rectangle data from from point and a size objects.
	// Parameters:
	//     location - The rectangle location.
	//     size     - The size of the rectangle.
	//-----------------------------------------------------------------------
	void SetRect(const CXTPPoint2T<T>& location, const CXTPSizeT<T>& size)
	{
		X = location.X;
		Y = location.Y;
		Width = size.Width;
		Height = size.Height;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new rectangle data from top-left and bottom-right point objects.
	// Parameters:
	//     ptTopLeft - The top-left rectangle coordinates.
	//     ptBottomRight - The bottom-right rectangle coordinates.
	//-----------------------------------------------------------------------
	void SetRect(const CXTPPoint2T<T>& ptTopLeft, const CXTPPoint2T<T>& ptBottomRight)
	{
		X = ptTopLeft.X;
		Y = ptTopLeft.Y;
		Width = ptBottomRight.X - ptTopLeft.X;
		Height = ptBottomRight.Y - ptTopLeft.Y;
		Normalize();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Normalizes rectangle, i.e. if its width or height are negative 
	//     the components will be adjusted so that both width and height
	//     are guaranteed positive while the size doesn't get changed.
	//-----------------------------------------------------------------------
	void Normalize()
	{
		if (Width < T(0))
		{
			X += Width;
			Width = -Width;
		}

		if (Height < T(0))
		{
			Y += Height;
			Height = -Height;
		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the coordinate location of the rectangle.
	// Returns:
	//     A CXTPPoint2f object denoting the current location of the rectangle
	//     in a plane.
	//-----------------------------------------------------------------------
	CXTPPoint2T<T> GetLocation() const
	{
		return CXTPPoint2T<T>(X, Y);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the size of the rectangle.
	// Returns:
	//     A CXTPSizeF object denoting size of the rectangle.
	//-----------------------------------------------------------------------
	CXTPSizeT<T> GetSize() const
	{
		return CXTPSizeT<T>(Width, Height);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the starting x coordinate of the rectangle.
	// Returns:
	//     A value denoting the left of the rectangle.
	//-----------------------------------------------------------------------
	T GetLeft() const
	{
		return X;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the starting y coordinate of the rectangle.
	// Returns:
	//     A value denoting the top of the rectangle.
	//-----------------------------------------------------------------------
	T GetTop() const
	{
		return Y;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the ending x coordinate of the rectangle.
	// Returns:
	//     A value denoting the right of the rectangle.
	//-----------------------------------------------------------------------
	T GetRight() const
	{
		return X + Width;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the ending y coordinate of the rectangle.
	// Returns:
	//     A value denoting the bottom of the rectangle.
	//-----------------------------------------------------------------------
	T GetBottom() const
	{
		return Y + Height;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the rectangular area is epty.
	// Returns:
	//     TRUE if the rectangle is of finite area and FALSE if it is empty.
	//-----------------------------------------------------------------------
	bool IsEmptyArea() const
	{
		return (Width <= CXTPEpsilon<T>::Epsilon)
			|| (Height <= CXTPEpsilon<T>::Epsilon);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to empty rectangle.
	//-----------------------------------------------------------------------
	void SetEmpty() const
	{
		X = Y = Width = Height = T(0);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the current rectangle is equal
	//     to another.
	// Parameters:
	//     rect - The rectangle which is to be compared with the current object
	//     for equality.
	// Returns:
	//     TRUE if the second rectangle is equal to the current object and FALSE
	//     they are unequal.
	//-----------------------------------------------------------------------
	bool Equals(const CXTPRectT& rect) const
	{
		return (*this == rect);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares to another rectangle
	// Parameters:
	//     rect - Another rectangle to compare to.
	// Returns:
	//     true if the two rectangles are equal, false otherwise.
	//-----------------------------------------------------------------------
	bool operator == (const CXTPRectT& rect) const
	{
		return (X == rect.X)
			&& (Y == rect.Y)
			&& (Width == rect.Width)
			&& (Height == rect.Height);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares to another rectangle
	// Parameters:
	//     rect - Another rectangle to compare to.
	// Returns:
	//     true if the two rectangles are not equal, false otherwise.
	//-----------------------------------------------------------------------
	bool operator != (const CXTPRectT& rect) const
	{
		return !(*this == rect);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether a point is inside the rectangle
	//     or not.
	// Parameters:
	//     x - The x coordinate of the point.
	//     y - The y coordinate of the point.
	// Returns:
	//     TRUE if the point is inside the rectangle and FALSE if the point is
	//     outside the rectangle.
	//-----------------------------------------------------------------------
	bool Contains(T x, T y) const
	{
		return (x >= X)
			&& (x < X + Width)
			&& (y >= Y)
			&& (y < Y + Height);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether a point is inside the rectangle
	//     or not.
	// Parameters:
	//     pt - The point.
	// Returns:
	//     TRUE if the point is inside the rectangle and FALSE if the point is
	//     outside the rectangle.
	//-----------------------------------------------------------------------
	bool Contains(const CXTPPoint2T<T>& pt) const
	{
		return Contains(pt.X, pt.Y);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether a rectangle is inside the rectangle
	//     or not.
	// Parameters:
	//     pt - The rectangle.
	// Returns:
	//     TRUE if the second rectangle is inside the current rectangle and FALSE if
	//     the not.
	//-----------------------------------------------------------------------
	bool Contains(const CXTPRectT& rect) const
	{
		return (X <= rect.X)
			&& (rect.GetRight() <= GetRight())
			&& (Y <= rect.Y)
			&& (rect.GetBottom() <= GetBottom());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to inflate the rectangle by an arbitrary value.
	// Parameters:
	//     dx - The change in x value.
	//     dy - The change in y value.
	//-----------------------------------------------------------------------
	void Inflate(T dx, T dy)
	{
		X -= dx;
		Y -= dy;
		Width += dx * T(2);
		Height += dy * T(2);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to inflate the rectangle by an arbitrary value
	//     denoted by a CXTPPoint2T object.
	// Parameters:
	//     point - The change value.
	//-----------------------------------------------------------------------
	void Inflate(const CXTPPoint2T<T>& point)
	{
		Inflate(point.X, point.Y);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to deflate the rectangle by an arbitrary value
	//     denoted by top-left and bottom-right coordinates.
	// Parameters:
	//     l - The top-left X coordinate.
	//     t - The top-left Y coordinate.
	//     r - The bottom-right X coordinate.
	//     b - The bottom-right Y coordinate.
	//-----------------------------------------------------------------------
	void DeflateRect(T l, T t, T r, T b)
	{
		X += l;
		Y += t;
		Width -= l + r;
		Height -= t + b;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call check whether the current rectangle object shares some common
	//     area with another rectangle object.
	// Parameters:
	//     rect - The other rectangle.
	// Returns:
	//     TRUE if the operation succeed and FALSE if operation failed.
	//-----------------------------------------------------------------------
	bool Intersect(const CXTPRectT& rect)
	{
		return Intersect(*this, *this, rect);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call check whether two rectangle objects share common area
	//     and save the shared rectangle.
	// Parameters:
	//     c - The reference to destination rectangle.
	//     a - The first source rectangle.
	//     b - The second source rectangle.
	// Returns:
	//     TRUE if the operation succeed and FALSE if operation failed.
	//-----------------------------------------------------------------------
	static bool Intersect(CXTPRectT& c, const CXTPRectT& a, const CXTPRectT& b)
	{
		T right = min(a.GetRight(), b.GetRight());
		T bottom = min(a.GetBottom(), b.GetBottom());
		T left = max(a.GetLeft(), b.GetLeft());
		T top = max(a.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether this object intersect with a given rectangle.
	// Parameters:
	//     rect - The reference to the other rectangle.
	// Returns:
	//     TRUE if the rectangles intersect and FALSE if rectangles do not intersect.
	//-----------------------------------------------------------------------
	bool IntersectsWith(const CXTPRectT& rect) const
	{
		return (GetLeft() < rect.GetRight())
			&& (GetTop() < rect.GetBottom())
			&& (GetRight() > rect.GetLeft())
			&& (GetBottom() > rect.GetTop());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this static function to calculates the union of two
	//     source rectangles and places the coordinates of the union
	//     rectangle into the destination rectangle.
	// Parameters:
	//     c - The reference to destination rectangle.
	//     a - The first source rectangle.
	//     b - The second source rectangle.
	// Returns:
	//     TRUE if the operation succeed and FALSE if operation failed.
	// Remarks:
	//     The union is the smallest rectangle that contains both source rectangles.
	//-----------------------------------------------------------------------
	static bool Union(CXTPRectT& c, const CXTPRectT& a, const CXTPRectT& b)
	{
		T right = max(a.GetRight(), b.GetRight());
		T bottom = max(a.GetBottom(), b.GetBottom());
		T left = min(a.GetLeft(), b.GetLeft());
		T top = min(a.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to move the rectangle by an offset.
	// Parameters:
	//     point - The offset as a point.
	//-----------------------------------------------------------------------
	void Offset(const CXTPPoint2T<T>& point)
	{
		Offset(point.X, point.Y);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to move the rectangle by an offset.
	// Parameters:
	//     dx - The offset x value.
	//     dy - The offset y value.
	//-----------------------------------------------------------------------
	void Offset(T dx, T dy)
	{
		X += dx;
		Y += dy;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the center point of the rectangle.
	// Returns:
	//     A CXTPPoint2f object representing the center of the rectangle.
	//-----------------------------------------------------------------------
	CXTPPoint2T<T> GetCenter()
	{
		return CXTPPoint2T<T>(X + Width / T(2), Y + Height / T(2));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Rounds rectangle components.
	//-----------------------------------------------------------------------
	void Round()
	{
		T r = T(CXTPMathUtils::Round(double(X + Width)));
		T d = T(CXTPMathUtils::Round(double(Y + Height)));

		X = T(CXTPMathUtils::Round(double(X)));
		Y = T(CXTPMathUtils::Round(double(Y)));
		Width = r - X;
		Height = d - Y;
	}

public:
	T X;         //The x coordinate(left).
	T Y;         //The y coordinate(top).
	T Width;     //The width of the rectangle.
	T Height;    //The height of the rectangle.
};

// CXTPRectT shortcut for integer rectangle object.
typedef CXTPRectT<int> CXTPRectI;

// CXTPRectT shortcut for float rectangle object.
typedef CXTPRectT<float> CXTPRectF;

// CXTPRectT shortcut for double rectangle object.
typedef CXTPRectT<double> CXTPRectD;

//{{AFX_CODEJOCK_PRIVATE
#endif // !defined(__XTPRECT_H__)
//}}AFX_CODEJOCK_PRIVATE
