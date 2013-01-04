// XTPChartMathUtils.cpp
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

#include "stdafx.h"
#include <math.h>
#include <float.h>

#include "XTPChartMathUtils.h"


double CXTPChartMathUtils::m_dPI = acos(-1.0);
double CXTPChartMathUtils::m_dEPS = 1e-6;


double CXTPChartMathUtils::NormalizeRadian(double angleRadian)
{
	double x = fabs(angleRadian) / (m_dPI * 2);
	x = m_dPI * 2 * (x - floor(x));
	return x * Sign(angleRadian);
}

double CXTPChartMathUtils::NormalizeDegree(double angle)
{
	double result = angle - 360 * ((int)angle / 360);
	if (result < 0)
		result += 360;
	return result;
}

int CXTPChartMathUtils::Compare(double value1, double value2, double epsilon)
{
	if (fabs(value1 - value2) <= epsilon)
		return 0;

	if (value1 > value2)
		return 1;

	return - 1;
}

int CXTPChartMathUtils::Compare(double value1, double value2)
{
	return Compare(value1, value2, m_dEPS);
}


int CXTPChartMathUtils::Sign(double value)
{
	if (fabs(value) < m_dEPS)
		return 0;

	return value > 0 ? +1 : -1;
}

double CXTPChartMathUtils::Radian2Degree(double angleRadian)
{
	return angleRadian * 180 / m_dPI;
}

double CXTPChartMathUtils::Degree2Radian(double angleDegree)
{
	return angleDegree * m_dPI / 180.0;
}

int CXTPChartMathUtils::Round(double dValue)
{
	int x = (int)floor(dValue);
	return fabs(x - dValue) < fabs(x + 1 - dValue) ? x : x + 1;
}

double CXTPChartMathUtils::GetNaN()
{
	unsigned long nan[2]={0xffffffff, 0x7fffffff};
	double g = *( double* )nan;

	return g;
}

BOOL CXTPChartMathUtils::IsNan(double x)
{
	return _isnan(x);
}
