// XTPRect.cpp
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
#include "XTPPoint2.h"
#include "XTPSize.h"
#include "XTPRect.h"

#include <float.h>

template<class T>
const T CXTPEpsilon<T>::Epsilon = 0;

template<>
const float CXTPEpsilon<float>::Epsilon = FLT_EPSILON;

template<>
const double CXTPEpsilon<double>::Epsilon = DBL_EPSILON;

template<>
const long double CXTPEpsilon<long double>::Epsilon = LDBL_EPSILON;
