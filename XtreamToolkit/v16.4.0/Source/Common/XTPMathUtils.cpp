// XTPMathUtils.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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

#include "stdafx.h"
#include <math.h>
#include <float.h>

#include <Common/XTPMathUtils.h>


double CXTPMathUtils::m_dPI = acos(-1.);
double CXTPMathUtils::m_dEPS = DBL_EPSILON;


double CXTPMathUtils::NormalizeRadian(double angleRadian)
{
	double x = fabs(angleRadian) / (m_dPI * 2);
	x = m_dPI * 2 * (x - floor(x));
	return x * Sign(angleRadian);
}

double CXTPMathUtils::NormalizeDegree(double angle)
{
	double result = angle - 360 * ((int)angle / 360);
	if (result < 0)
		result += 360;
	return result;
}

int CXTPMathUtils::Compare(double value1, double value2, double epsilon)
{
	if (fabs(value1 - value2) <= epsilon)
		return 0;

	if (value1 > value2)
		return 1;

	return - 1;
}

int CXTPMathUtils::Compare(double value1, double value2)
{
	return Compare(value1, value2, m_dEPS);
}


int CXTPMathUtils::Sign(double value)
{
	if (fabs(value) < m_dEPS)
		return 0;

	return value > 0 ? +1 : -1;
}

double CXTPMathUtils::Radian2Degree(double angleRadian)
{
	return angleRadian * 180 / m_dPI;
}

double CXTPMathUtils::Degree2Radian(double angleDegree)
{
	return angleDegree * m_dPI / 180.0;
}

int CXTPMathUtils::Round(double dValue)
{
	int x = (int)floor(dValue);
	return fabs(x - dValue) < fabs(x + 1 - dValue) ? x : x + 1;
}

double CXTPMathUtils::GetNaN()
{
	unsigned long nan[2]={0xffffffff, 0x7fffffff};
	double g = *( double* )nan;

	return g;
}

BOOL CXTPMathUtils::IsNan(double x)
{
	return _isnan(x);
}
