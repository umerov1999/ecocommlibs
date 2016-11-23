/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef typedefs_h
#define typedefs_h

// This file defines Anoto specific 
// typedefs and defines

////////////////////////////////////////
//              TYPEDEFS              //
////////////////////////////////////////

/** \ingroup Anotocommon
    \defgroup Types Basic Types
@{*/

typedef char* unicode; 

#if defined(BMU)

	#include <common.h>
	typedef unsigned char boolean;

#else
#if defined(DOXYGEN) || defined(__arm) || defined(__GNUC__) || defined(__SYMBIAN32__) || defined(__CYGWIN__) || (defined(__APPLE__) && defined(__POWERPC__))

/** Unsigned 8 bit number */	
	typedef unsigned char u8;
/** Signed 8 bit number */	
	typedef signed char s8;
	
/** Unsigned 16 bit number */	
	typedef unsigned short int u16;
/** Signed 16 bit number */	
	typedef signed short int s16;
	
/** Unsigned 32 bit number */	
	typedef unsigned int u32;
/** Signed 32 bit number */	
	typedef signed int s32;

/** Unsigned 64 bit number */	
	typedef unsigned long long u64;
/** Signed 64 bit number */	
	typedef signed long long s64;

/** Boolean */	
	typedef s32 boolean;

	#define ULL(x) (x ## ULL)
	#define SLL(x) (x ## LL)

#else
#if defined(_WIN32) 

	typedef unsigned __int8 u8;
	typedef signed __int8 s8;
	
	typedef unsigned __int16 u16;
	typedef signed __int16 s16;
	
	typedef unsigned __int32 u32;
	typedef signed __int32 s32;

	typedef unsigned __int64 u64;
	typedef signed __int64 s64;
	
	#define booleantype s32
	#define boolean booleantype
	#define ULL( x ) x
	#define SLL( x ) (## x ## i64)

#else
	#error "You need to define _WIN32 or something"
#endif

#endif
#endif

///////////////////////////////////////
//              DEFINES              //
///////////////////////////////////////
#include <stdlib.h>

#ifdef __GCC__
#define TYPE_MAX_U64 0xFFFFFFFFFFFFFFFFULL
#define TYPE_MAX_S64 0x7FFFFFFFFFFFFFFFLL
#else
#define TYPE_MAX_U64 ULL(0xFFFFFFFFFFFFFFFF)
#define TYPE_MAX_S64 ULL(0x7FFFFFFFFFFFFFFF)
#endif

#define TYPE_MAX_U32 0xFFFFFFFF
#define TYPE_MAX_S32 0x7FFFFFFF

#ifndef TRUE
/** TRUE */	
#define TRUE 1
#endif

#ifndef FALSE
/** FALSE */	
#define FALSE 0
#endif

/*@}*/

#if defined(__arm__)
#define __weak __attribute__((weak))
#endif

#endif // typedefs_h
