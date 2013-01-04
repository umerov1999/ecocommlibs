#include "StdAfx.h"
#include "XTPPoint2.h"

CXTPPoint2f::CXTPPoint2f()
	: X(0)
	, Y(0)
{
}

CXTPPoint2f::CXTPPoint2f(int x, int y)
	: X(float(x))
	, Y(float(y))
{
}

CXTPPoint2f::CXTPPoint2f(float x, float y)
	: X(x)
	, Y(y)
{
}

CXTPPoint2f::CXTPPoint2f(double x, double y)
	: X(float(x))
	, Y(float(y))
{
}
