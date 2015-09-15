#include ".\kposition.h"

KPosition::KPosition(void)
: x(0)
, y(0)
, z(0)
{
}

KPosition::~KPosition(void)
{
}


KPosition::KPosition(double ix, double iy, double iz)
{
	x = ix;
	y = iy;
	z = iz;
}

KVector KPosition::operator-(KPosition pos)
{
	KVector temp;

	temp.x = x - pos.x;
	temp.y = y - pos.y;
	temp.z = z - pos.z;

	return temp;
}

void KPosition::operator=(KPosition pos)
{
	x = pos.x;
	y = pos.y;
	z = pos.z;
}
