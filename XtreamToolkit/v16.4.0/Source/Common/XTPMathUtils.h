// XTPMathUtils.h
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
#if !defined(__XTPMATHUTILS_H__)
#define __XTPMATHUTILS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     This class provide some mathematical utility functions.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPMathUtils
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to normalize angle in radian i.e. bring the value
	//     within the range of 0 - 2PI.
	// Parameters:
	//     angleRadian - Angle in radians.
	// Returns:
	//     A double value denoting the normalized radian.
	// Remarks:
	//     It is a static function.
	//-----------------------------------------------------------------------
	static double NormalizeRadian(double angleRadian);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to normalize angle in degree i.e. bring the value
	//     within the range of 0 and 360.
	// Parameters:
	//     angle - Angle in degree.
	// Returns:
	//     A double value denoting the normalized degree.
	// Remarks:
	//     It is a static function.
	//-----------------------------------------------------------------------
	static double NormalizeDegree(double angle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check the sign of a double value.
	// Parameters:
	//     value - The double value.
	// Returns:
	//     +1 if the value is positive and -1 if the value negative.
	// Remarks:
	//     It is a static function.
	//-----------------------------------------------------------------------
	static int Sign(double value);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to compare two double values.It also checks
	//     whether they are equal or almost equal.
	// Parameters:
	//     value1  - The first value.
	//     value2  - The second value.
	// Returns:
	//     +1 if the value1 is greater than value2, -1 if vice versa.
	//     0 if they are equal or almost equal.
	// Remarks:
	//     It is a static function.
	//-----------------------------------------------------------------------
	static int Compare(double value1, double value2);

	static int Round(double dValue);
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to compare two double values.It also checks
	//     whether they are equal or almost equal based on a 3rd value.
	// Parameters:
	//     value1  - The first value.
	//     value2  - The second value.
	//     epsilon - The equality threshold.
	// Returns:
	//     +1 if the value1 is greater than value2, -1 if vice versa.
	//     0 if they are equal or almost equal.
	// Remarks:
	//     It is a static function.
	//-----------------------------------------------------------------------
	static int Compare(double value1, double value2, double epsilon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to convert radian angle to degree.
	// Parameters:
	//     angleRadian  - The angle in radian.
	// Returns:
	//     A double value denoting the angle in degree.
	// Remarks:
	//     It is a static function.
	//-----------------------------------------------------------------------
	static double Radian2Degree(double angleRadian);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to convert degree angle to radian.
	// Parameters:
	//     angleDegree  - The angle in degree.
	// Returns:
	//     A double value denoting the angle in radian.
	// Remarks:
	//     It is a static function.
	//-----------------------------------------------------------------------
	static double Degree2Radian(double angleDegree);

	static double GetNaN();
	static BOOL IsNan(double x);

public:
	static double m_dPI;
	static double m_dEPS;
};



#endif //#if !defined(__XTPMATHUTILS_H__)
