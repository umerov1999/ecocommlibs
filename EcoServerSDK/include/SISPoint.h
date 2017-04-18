#pragma once
class SISPoint 
{ 
public:
	double		x;
	double		y;
	double		z;
	
public:
	/**
	@brief Creator
	@param
	@return
	@remark Initialize member variable
	*/
	SISPoint() : x(0), y(0), z(0) {};

	/**
	@brief Creator
	@param X [in]X Value
	@param Y [in]Y Value
	@param Z [in]Z Value
	@return
	@remark Initialize member variable using parameter
	*/
	SISPoint(double X, double Y, double Z=0.0f) : x(X), y(Y), z(Z) {};
	
	/**
	@brief Creator
	@param p [in]Object of SISPoint
	@return
	@remark Initialize member variable using value of input object
	*/
	SISPoint(SISPoint const * p) : x(p->x), y(p->y), z(p->z) {};
	
	/**
	@brief Creator
	@param p [in]Object of SISPoint
	@return
	@remark Initialize member variable using value of input object
	*/
	SISPoint(SISPoint const & p) : x(p.x), y(p.y), z(p.z) {};
	
	/**
	@brief Set X, Y, Z value
	@param X [in]X Value
	@param Y [in]Y Value
	@param Z [in]Z Value
	@return
	@remark Initialize member variable using parameter
	*/
	void Set(double X, double Y, double Z=0.0f) { x=X; y=Y; z=Z; };
	
	/**
	@brief Set object of SISPoint
	@param p [in]Object of SISNPoint
	@return
	@remark Initialize x, y, z variable using value of input object
	*/
	void Set(SISPoint *p) { x=p->x; y=p->y; z=p->z; };
	
	/**
	@brief Set object of SISPoint
	@param p [in]Object of SISNPoint
	@return
	@remark Initialize x, y, z variable using value of input object
	*/
	void Set(SISPoint const *p) { x=p->x; y=p->y; z=p->z; };
	
	/**
	@brief Set object of SISPoint
	@param p [in]Object of SISNPoint
	@return
	@remark Initialize x, y, z variable using value of input object
	*/
	void Set(SISPoint &p) { x=p.x; y=p.y; z=p.z; };
	
	/**
	@brief Set object of SISPoint
	@param p [in]Object of SISNPoint
	@return
	@remark Initialize x, y, z variable using value of input object
	*/
	void Set(const SISPoint &p) { x=p.x; y=p.y; z=p.z; };

	/**
	@brief Plus X, Y, Z value
	@param X [in]X Value
	@param Y [in]Y Value
	@param Z [in]Z Value
	@return
	@remark Initialize member variable using value of input object
	*/
	void Add(double X, double Y, double Z=0.0){ x+=X; y+=Y; z+=Z; };

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint operator =(const SISPoint &p)
	{
		x = p.x; y = p.y; z = p.z;
		return SISPoint(x, y, z);
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint const operator -(const SISPoint &p2) const
	{
 		return SISPoint(x-p2.x, y-p2.y, z-p2.z);
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint const operator +(const SISPoint &p2) const
	{
 		return SISPoint(x+p2.x, y+p2.y, z+p2.z);
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint const operator *(double const rhs) const
	{
 		return SISPoint(x*rhs, y*rhs, z*rhs);
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint const operator /(double const rhs) const
	{
 		return SISPoint(x/rhs, y/rhs, z/rhs);
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint const & operator += (SISPoint const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint const & operator -= (SISPoint const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint const & operator *= (double const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	SISPoint const & operator /= (double const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	bool operator ==(SISPoint const & rhs) const
	{
		return Equal(&rhs);
	}
	
	/**
	@brief Default Operator
	@param
	@return
	@remark
	*/
	bool operator !=(SISPoint const & rhs) const
	{
		return !Equal(&rhs);
	}


#define SISPoint_EPSILON (1e-5f)

	bool Equal(SISPoint const *p, double epsi = SISPoint_EPSILON) const { 
		return (bool)(
			(x - p->x < epsi) && (x - p->x > -epsi) &&
			(y - p->y < epsi) && (y - p->y > -epsi) &&
			(z - p->z < epsi) && (z - p->z > -epsi));
	};

	bool Equal(SISPoint const &p, double epsi = SISPoint_EPSILON) const { 
		return Equal(&p, epsi);
	};

};