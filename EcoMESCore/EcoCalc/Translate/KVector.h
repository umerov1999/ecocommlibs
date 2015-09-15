#pragma once

#include "KPosition.h"

class KPosition;

class KVector
{
public:
	KVector(void);
	~KVector(void);
	double x;
	double y;
	double z;
	KVector(double ix, double iy, double iz);
	void Nomalize(void);
	double GetLength(void);

	void operator=(const KVector vec);
	void operator=(KPosition pos);
};
