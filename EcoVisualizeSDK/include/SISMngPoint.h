#pragma once

#ifndef _SISMngPoint_H
#define _SISMngPoint_H

class SISMngPoint 
{ 
public:
	UINT		nID;
	float		x;
	float		y;
	float		z;
	
	SISMngPoint() : nID(0), x(0), y(0), z(0) {};

	SISMngPoint(UINT nId, float X, float Y, float Z = 0.0f) : nID(nId), x(X), y(Y), z(Z) {};
	
	SISMngPoint(SISMngPoint const * p) : nID(p->nID), x(p->x), y(p->y), z(p->z) {};
	
	SISMngPoint(SISMngPoint const & p) : nID(p.nID), x(p.x), y(p.y), z(p.z) {};
	
	void Set(UINT nId, float X, float Y, float Z = 0.0f) { nID = nId; x = X; y = Y; z = Z; };
	
	void Set(SISMngPoint *p) { nID = p->nID; x = p->x; y = p->y; z = p->z; };
	
	void Set(SISMngPoint const *p) { nID = p->nID; x = p->x; y = p->y; z = p->z; };
	
	void Set(SISMngPoint &p) { nID = p.nID; x = p.x; y = p.y; z = p.z; };
	
	void Set(const SISMngPoint &p) { nID = p.nID; x = p.x; y = p.y; z = p.z; };

	void Add(float X, float Y, float Z=0.0){ x+=X; y+=Y; z+=Z; };

	SISMngPoint const operator -(const SISMngPoint &p2) const
	{
 		return SISMngPoint(x-p2.x, y-p2.y, z-p2.z);
	}

	SISMngPoint const operator +(const SISMngPoint &p2) const
	{
 		return SISMngPoint(x+p2.x, y+p2.y, z+p2.z);
	}

	SISMngPoint const operator *(float const rhs) const
	{
 		return SISMngPoint(x*rhs, y*rhs, z*rhs);
	}

	SISMngPoint const operator /(float const rhs) const
	{
 		return SISMngPoint(x/rhs, y/rhs, z/rhs);
	}

	SISMngPoint const & operator += (SISMngPoint const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	SISMngPoint const & operator -= (SISMngPoint const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	SISMngPoint const & operator *= (float const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	SISMngPoint const & operator /= (float const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator ==(SISMngPoint const & rhs) const
	{
		return Equal(&rhs);
	}
	
	bool operator !=(SISMngPoint const & rhs) const
	{
		return !Equal(&rhs);
	}


#define SISMngPoint_EPSILON (1e-5f)

	bool Equal(SISMngPoint const *p, float epsi = SISMngPoint_EPSILON) const { 
		return (bool)(
			(nID == p->nID) && 
			(x - p->x < epsi) && (x - p->x > -epsi) &&
			(y - p->y < epsi) && (y - p->y > -epsi) &&
			(z - p->z < epsi) && (z - p->z > -epsi));
	};

	bool Equal(SISMngPoint const &p, float epsi = SISMngPoint_EPSILON) const { 
		return Equal(&p, epsi);
	};

};

#endif