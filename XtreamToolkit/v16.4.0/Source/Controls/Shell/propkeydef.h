// propkeydef.h : header file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPCONTROLS_PROPKEYDEF_H__)
#define __XTPCONTROLS_PROPKEYDEF_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef PID_FIRST_USABLE
#define PID_FIRST_USABLE 2
#endif

#ifndef REFPROPERTYKEY
#ifdef __cplusplus
#define REFPROPERTYKEY const PROPERTYKEY &
#else // !__cplusplus
#define REFPROPERTYKEY const PROPERTYKEY * __MIDL_CONST
#endif // __cplusplus
#endif //REFPROPERTYKEY

#ifdef DEFINE_PROPERTYKEY
#undef DEFINE_PROPERTYKEY
#endif

#ifdef INITGUID
#define DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) EXTERN_C const PROPERTYKEY name = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }
#else
#define DEFINE_PROPERTYKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) EXTERN_C const PROPERTYKEY name
#endif // INITGUID

#ifndef IsEqualPropertyKey
#define IsEqualPropertyKey(a, b)   (((a).pid == (b).pid) && IsEqualIID((a).fmtid, (b).fmtid) )
#endif  // IsEqualPropertyKey

#ifndef _PROPERTYKEY_EQUALITY_OPERATORS_
#define _PROPERTYKEY_EQUALITY_OPERATORS_
#ifdef __cplusplus
extern "C++"
{
__inline int operator == (REFPROPERTYKEY pkeyOne, REFPROPERTYKEY pkeyOther) { return IsEqualPropertyKey(pkeyOne, pkeyOther); }
__inline int operator != (REFPROPERTYKEY pkeyOne, REFPROPERTYKEY pkeyOther) { return !(pkeyOne == pkeyOther); }
}
#endif // __cplusplus
#endif // _PROPERTYKEY_EQUALITY_OPERATORS_

//{{AFX_CODEJOCK_PRIVATE
#endif //#if !defined(__XTPCONTROLS_PROPKEYDEF_H__)
//}}AFX_CODEJOCK_PRIVATE
