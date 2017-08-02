#pragma once

#ifndef _SISPoint_H
#define _SISPoint_H

class SISPoint 
{ 
public:
	float		x;
	float		y;
	float		z;
	
	SISPoint() : x(0), y(0), z(0) {};

	SISPoint(float X, float Y, float Z=0.0f) : x(X), y(Y), z(Z) {};
	
	SISPoint(SISPoint const * p) : x(p->x), y(p->y), z(p->z) {};
	
	SISPoint(SISPoint const & p) : x(p.x), y(p.y), z(p.z) {};
	
	void Set(float X, float Y, float Z=0.0f) { x=X; y=Y; z=Z; };
	
	void Set(SISPoint *p) { x=p->x; y=p->y; z=p->z; };
	
	void Set(SISPoint const *p) { x=p->x; y=p->y; z=p->z; };
	
	void Set(SISPoint &p) { x=p.x; y=p.y; z=p.z; };
	
	void Set(const SISPoint &p) { x=p.x; y=p.y; z=p.z; };

	void Add(float X, float Y, float Z=0.0){ x+=X; y+=Y; z+=Z; };

	SISPoint const operator -(const SISPoint &p2) const
	{
 		return SISPoint(x-p2.x, y-p2.y, z-p2.z);
	}

	SISPoint const operator +(const SISPoint &p2) const
	{
 		return SISPoint(x+p2.x, y+p2.y, z+p2.z);
	}

	SISPoint const operator *(float const rhs) const
	{
 		return SISPoint(x*rhs, y*rhs, z*rhs);
	}

	SISPoint const operator /(float const rhs) const
	{
 		return SISPoint(x/rhs, y/rhs, z/rhs);
	}

	SISPoint const & operator += (SISPoint const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	SISPoint const & operator -= (SISPoint const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	SISPoint const & operator *= (float const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	SISPoint const & operator /= (float const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator ==(SISPoint const & rhs) const
	{
		return Equal(&rhs);
	}
	
	bool operator !=(SISPoint const & rhs) const
	{
		return !Equal(&rhs);
	}


#define SISPoint_EPSILON (1e-5f)

	bool Equal(SISPoint const *p, float epsi = SISPoint_EPSILON) const { 
		return (bool)(
			(x - p->x < epsi) && (x - p->x > -epsi) &&
			(y - p->y < epsi) && (y - p->y > -epsi) &&
			(z - p->z < epsi) && (z - p->z > -epsi));
	};

	bool Equal(SISPoint const &p, float epsi = SISPoint_EPSILON) const { 
		return Equal(&p, epsi);
	};

};

#endif