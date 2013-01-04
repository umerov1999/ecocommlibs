//===========================================================================
// Summary:
//     This class abstracts a rectangular entity and its operations.This object composed
//     of four float values for x and y coordinates and width and height.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPRectF
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default constructor, creates a CXTPRectF object and initializes
	//     the values.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPRectF()
	{
		X = Y = Width = Height = 0.0f;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a CXTPRectF object from
	//     CRect type object and initializes the values.
	// Parameters:
	//     rc - A reference to a CRect.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPRectF(const CRect& rc)
	{
		X = (float)rc.left;
		Y = (float)rc.top;
		Width = (float)rc.Width();
		Height = (float)rc.Height();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a CXTPRectF object.
	// Parameters:
	//     x      - The x coordinate.
	//     y      - The y coordinate.
	//     width  - The width of the rectangle.
	//     height - The height of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPRectF(float x, float y, float width, float height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded constructor, creates a CXTPRectF object from chart point
	//     and chart size objects.
	// Parameters:
	//     location - The rectangle location.
	//     size     - The size of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPRectF(const CXTPPoint2f& location, const CXTPSizeF& size)
	{
		X = location.X;
		Y = location.Y;
		Width = size.Width;
		Height = size.Height;
	}

	CXTPRectF(const CXTPPoint2f& ptTopLeft, const CXTPPoint2f& ptBottomRight)
	{
		X = ptTopLeft.X;
		Y = ptTopLeft.Y;
		Width = ptBottomRight.X - ptTopLeft.X;
		Height = ptBottomRight.Y - ptTopLeft.Y;

		Normalize();
	}

	void Normalize()
	{
		if (Width < 0)
		{
			X += Width;
			Width = -Width;
		}

		if (Height < 0)
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
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint2f GetLocation() const
	{
		return CXTPPoint2f(X, Y);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the size of the rectangle.
	// Returns:
	//     A CXTPSizeF object denoting size of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPSizeF GetSize() const
	{
		return CXTPSizeF(Width, Height);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the starting x coordinate of the rectangle.
	// Returns:
	//     A float value denoting the left of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	float GetLeft() const
	{
		return X;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the starting y coordinate of the rectangle.
	// Returns:
	//     A float value denoting the top of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	float GetTop() const
	{
		return Y;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the ending x coordinate of the rectangle.
	// Returns:
	//     A float value denoting the right of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	float GetRight() const
	{
		return X+Width;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the ending y coordinate of the rectangle.
	// Returns:
	//     A float value denoting the bottom of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	float GetBottom() const
	{
		return Y+Height;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the rectangular area is epty.
	// Returns:
	//     TRUE if the rectangle is of finite area and FALSE if it is empty.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IsEmptyArea() const
	{
		return (Width <= float_EPSILON) || (Height <= float_EPSILON);
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
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL Equals(const CXTPRectF & rect) const
	{
		return X == rect.X &&
			   Y == rect.Y &&
			   Width == rect.Width &&
			   Height == rect.Height;
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
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL Contains(float x,
				  float y) const
	{
		return x >= X && x < X+Width &&
			   y >= Y && y < Y+Height;
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
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL Contains(const CXTPPoint2f& pt) const
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
	//
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL Contains(const CXTPRectF& rect) const
	{
		return (X <= rect.X) && (rect.GetRight() <= GetRight()) &&
			   (Y <= rect.Y) && (rect.GetBottom() <= GetBottom());
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to inflate the rectangle by an arbitrary value.
	// Parameters:
	//     dx - The change in x value.
	//     dy - The change in y value.
	// Remarks:
	//-----------------------------------------------------------------------
	void Inflate(float dx, float dy)
	{
		X -= dx;
		Y -= dy;
		Width += 2*dx;
		Height += 2*dy;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to inflate the rectangle by an arbitrary value
	//     denoted by a CXTPPoint2f object.
	// Parameters:
	//     point - The change value.
	// Remarks:
	//-----------------------------------------------------------------------
	void Inflate(const CXTPPoint2f& point)
	{
		Inflate(point.X, point.Y);
	}

	void DeflateRect(float l, float t, float r, float b)
	{
		X += l;
		Y += t;
		Width -= l + r;
		Height -= t + b;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function calculates the intersection, the current object
	//     and another rectangle and places the coordinates of the intersection
	//     rectangle into the current object.
	// Parameters:
	//     rect - The other rectangle.
	// Returns:
	//     TRUE if the operation succeed and FALSE if operation failed.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL Intersect(const CXTPRectF& rect)
	{
		return Intersect(*this, *this, rect);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this static function to calculates the intersection of two
	//     source rectangles and places the coordinates of the intersection
	//     rectangle into the destination rectangle.
	// Parameters:
	//     c - The reference to destination rectangle.
	//     a - The first source rectangle.
	//     b - The second source rectangle.
	// Returns:
	//     TRUE if the operation succeed and FALSE if operation failed.
	// Remarks:
	//-----------------------------------------------------------------------
	static BOOL Intersect(CXTPRectF& c, const CXTPRectF& a, const CXTPRectF& b)
	{
		float right = min(a.GetRight(), b.GetRight());
		float bottom = min(a.GetBottom(), b.GetBottom());
		float left = max(a.GetLeft(), b.GetLeft());
		float top = max(a.GetTop(), b.GetTop());

		c.X = left;
		c.Y = top;
		c.Width = right - left;
		c.Height = bottom - top;
		return !c.IsEmptyArea();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether this object intersect with a given
	//     rectangle.
	// Parameters:
	//     rect - The reference to the other rectangle.
	// Returns:
	//     TRUE if the rectangles intersect and FALSE if rectangles do not intersect.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL IntersectsWith(const CXTPRectF& rect) const
	{
		return (GetLeft() < rect.GetRight() &&
				GetTop() < rect.GetBottom() &&
				GetRight() > rect.GetLeft() &&
				GetBottom() > rect.GetTop());
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
	static BOOL Union(OUT CXTPRectF& c, const CXTPRectF& a, const CXTPRectF& b)
	{
		float right = max(a.GetRight(), b.GetRight());
		float bottom = max(a.GetBottom(), b.GetBottom());
		float left = min(a.GetLeft(), b.GetLeft());
		float top = min(a.GetTop(), b.GetTop());

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
	// Remarks:
	//-----------------------------------------------------------------------
	void Offset(const CXTPPoint2f& point)
	{
		Offset(point.X, point.Y);
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to move the rectangle by an offset.
	// Parameters:
	//     dx - The offset x value.
	//     dy - The offset y value.
	// Remarks:
	//-----------------------------------------------------------------------
	void Offset(float dx, float dy)
	{
		X += dx;
		Y += dy;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the center point of the rectangle.
	// Returns:
	//     A CXTPPoint2f object representing the center of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPPoint2f GetCenter()
	{
		return CXTPPoint2f(X + Width / 2.0f, Y + Height / 2.0f);
	}

	void Round();

public:

	float X;         //The x coordinate(left).
	float Y;         //The y coordinate(top).
	float Width;     //The width of the rectangle.
	float Height;    //The height of the rectangle.
};
