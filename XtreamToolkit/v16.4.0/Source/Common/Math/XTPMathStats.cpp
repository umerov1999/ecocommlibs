// XTPMathStats.cpp
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

#include "StdAfx.h"
#include <Common/Math/XTPMathStats.h>
#include <math.h>



double CXTPMathArithmeticMean::Add(double dValue)
{
	++m_dCount;
	m_dArithmeticMean = ((dValue - m_dArithmeticMean) / (m_dCount)) + m_dArithmeticMean;
	return m_dArithmeticMean;
}



/////////////////////////////////////////////////////////////////////////////

double CXTPMathVariance::Add(double dValue, double dArithmeticMean)
{
	++m_dCount;
	dValue = pow(dValue - dArithmeticMean, 2);
	m_dVariance = ((dValue - m_dVariance) / (m_dCount)) + m_dVariance;
	return m_dVariance;
}

CXTPMathVariance::operator double () const
{
	return m_dVariance;
}

/////////////////////////////////////////////////////////////////////////////

CXTPMathCovariance::CXTPMathCovariance()
	: m_dCovariance(0)
	, m_dCount     (0)
{
	// Nothing
}

double CXTPMathCovariance::Add(
	double dValueX, double dArithmeticMeanX,
	double dValueY, double dArithmeticMeanY)
{
	++m_dCount;

	double dValue = (dValueX-dArithmeticMeanX)*(dValueY-dArithmeticMeanY);
	m_dCovariance = ((dValue - m_dCovariance) / (m_dCount)) + m_dCovariance;

	return m_dCovariance;
}

CXTPMathCovariance::operator double () const
{
	return m_dCovariance;
}
