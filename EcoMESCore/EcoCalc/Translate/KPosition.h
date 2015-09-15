#pragma once

#include "KVector.h"

class KVector;

class KPosition
{
public:
	KPosition(void);
	~KPosition(void);
	double x;
	double y;
	double z;
	KPosition(double ix, double iy, double iz);

	KVector operator-(KPosition pos);
	void operator=(KPosition pos);
};
