// XTPMathStats.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPMATHSTATS_H__)
#define __XTPMATHSTATS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class _XTP_EXT_CLASS CXTPMathArithmeticMean
{
public:

	CXTPMathArithmeticMean()
		: m_dArithmeticMean(0)
		, m_dCount         (0)
	{
	}

	double Add(double dValue);

	operator double() const
	{
		return m_dArithmeticMean;
	}

protected:
	double m_dArithmeticMean;
	double m_dCount;
};


class _XTP_EXT_CLASS CXTPMathVariance
{
public:

	CXTPMathVariance()
		: m_dVariance(0)
		, m_dCount   (0)
	{
	}

	double Add(double dValue, double dArithmeticMean);

	operator double () const;

protected:
	double m_dVariance;
	double m_dCount;
};


class _XTP_EXT_CLASS CXTPMathCovariance
{
public:
	CXTPMathCovariance();

	double Add(
		double dValueX, double dArithmeticMeanX,
		double dValueY, double dArithmeticMeanY);

	operator double () const;

protected:
	double m_dCovariance;
	double m_dCount;
};


#endif //#if !defined(__XTPMATHSTATS_H__)
