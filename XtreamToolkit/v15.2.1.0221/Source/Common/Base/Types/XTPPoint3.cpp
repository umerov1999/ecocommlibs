// XTPPoint3.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

#include "StdAfx.h"
#include "XTPPoint2.h"
#include "XTPPoint3.h"


CXTPPoint3d::CXTPPoint3d()
	: X(0)
	, Y(0)
	, Z(0)
{
}

CXTPPoint3d::CXTPPoint3d(double x, double y, double z)
{
	X = x;
	Y = y;
	Z = z;
}

CXTPPoint3d::CXTPPoint3d(const CXTPPoint2f& other)
	: X(other.X)
	, Y(other.Y)
	, Z(0)
{
}

CXTPPoint3d:: operator CXTPPoint2f() const
{
	return CXTPPoint2f(float(X), float(Y));

}

void CXTPPoint3d::Offset(double dx, double dy, double dz)
{
	X += dx;
	Y += dy;
	Z += dz;
}
