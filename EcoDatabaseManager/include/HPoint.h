#pragma once
class /*MVO_API*/ HPoint 
{ 
public:
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version, BOOL bSendNRecv)
	{
		ar & x;
		ar & y;
		ar & z;
	}

public:
	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.
	float		z;	//!< The z-coordinate of a 3-dimensional point.
	
	/*! The default constructor. */
	HPoint() : x(0), y(0), z(0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	HPoint(float X, float Y, float Z=0.0f) : x(X), y(Y), z(Z) {};
	
	/*! This constructor accepts a pointer to an HPoint object. */
	HPoint(HPoint const * p) : x(p->x), y(p->y), z(p->z) {};
	
	/*! This constructor accepts an HPoint reference. */
	HPoint(HPoint const & p) : x(p.x), y(p.y), z(p.z) {};
	
	/*!  This method packs an HPoint object with coordinate values.  */
	void Set(float X, float Y, float Z=0.0f) { x=X; y=Y; z=Z; };
	
	/*!  This method packs an HPoint object with coordinate values from a given HPoint.  */
	void Set(HPoint *p) { x=p->x; y=p->y; z=p->z; };
	
	/*!  This method packs an HPoint object with coordinate values from a given HPoint.  */
	void Set(HPoint const *p) { x=p->x; y=p->y; z=p->z; };
	
	/*!  This method packs an HPoint object with coordinate values from a given HPoint that is passed by reference.  */
	void Set(HPoint &p) { x=p.x; y=p.y; z=p.z; };
	
	/*!  This method packs an HPoint object with coordinate values from a given HPoint that is passed by reference.  */
	void Set(const HPoint &p) { x=p.x; y=p.y; z=p.z; };

	/*!  This method increases/decreases the coordinate values of an existing HPoint object.  
	\param X The amount in which you want to increment or decrement the x coordinate.
	\param Y The amount in which you want to increment or decrement the y coordinate.
	\param Z The amount in which you want to increment or decrement the z coordinate.
	*/
	void Add(float X, float Y, float Z=0.0){ x+=X; y+=Y; z+=Z; };

	HPoint const operator -(const HPoint &p2) const
	{
 		return HPoint(x-p2.x, y-p2.y, z-p2.z);
	}

	HPoint const operator +(const HPoint &p2) const
	{
 		return HPoint(x+p2.x, y+p2.y, z+p2.z);
	}

	HPoint const operator *(float const rhs) const
	{
 		return HPoint(x*rhs, y*rhs, z*rhs);
	}

	HPoint const operator /(float const rhs) const
	{
 		return HPoint(x/rhs, y/rhs, z/rhs);
	}

	HPoint const & operator += (HPoint const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	HPoint const & operator -= (HPoint const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	HPoint const & operator *= (float const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	HPoint const & operator /= (float const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator ==(HPoint const & rhs) const
	{
		return Equal(&rhs);
	}
	
	bool operator !=(HPoint const & rhs) const
	{
		return !Equal(&rhs);
	}


/*!\def HPOINT_EPSILON */
#define HPOINT_EPSILON (1e-5f)  //!<  HPOINT_EPSILON


	/*! This method checks if the xyz components of the point are equal.  */
	bool Equal(HPoint const *p, float epsi = HPOINT_EPSILON) const { 
		return (bool)(
			(x - p->x < epsi) && (x - p->x > -epsi) &&
			(y - p->y < epsi) && (y - p->y > -epsi) &&
			(z - p->z < epsi) && (z - p->z > -epsi));
	};

	/*!  This method checks if the xyz components of the point are equal.  */
	bool Equal(HPoint const &p, float epsi = HPOINT_EPSILON) const { 
		return Equal(&p, epsi);
	};
};