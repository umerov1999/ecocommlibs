#ifndef C_STD_STDINT_H
#define C_STD_STDINT_H 0x01010000
/**
 * @file c/std/stdint.h
 * Integer types.
 * This file provides partial C99 Integer types (7.18) content support.
 */

#ifndef VO_INTEGER_TYPES_USE_COMPILER_STDINT_H
# if defined (_MSC_VER) && (_MSC_VER < 1600)
#  define VO_INTEGER_TYPES_USE_COMPILER_STDINT_H 0
# else
#  define VO_INTEGER_TYPES_USE_COMPILER_STDINT_H 1
# endif
#endif

#if VO_INTEGER_TYPES_USE_COMPILER_STDINT_H

#include <stdint.h>

#else // else of: #if VO_INTEGER_TYPES_USE_COMPILER_STDINT_H

// -- 7.18.1.1 Exact-width integer types ---------------------------------------

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
#ifdef _MSC_VER
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#endif

// -- 7.18.1.2 Minimum-width integer types -------------------------------------

// We choose not to provide int_leastN_t nor uint_leastN_t

// -- 7.18.1.3 Fastest minimum-width integer types -----------------------------

// We choose not to provide int_fastN_t nor uint_fastN_t

// -- 7.18.1.4 Integer types capable of holding object pointers ----------------

#ifdef _MSC_VER
#if _MSC_VER < 1400
#include <stddef.h>
#else
#include <crtdefs.h>
#endif
#endif

// -- 7.18.1.5 Greatest-width integer types ------------------------------------

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

// -- 7.18.2 Limits of specified-width integer types ---------------------------

#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

#define INT8_MAX      (127)
#define INT8_MIN      (-128)
#define UINT8_MAX     (255)

#define INT16_MAX     (32767)
#define INT16_MIN     (-INT16_MAX - 1)
#define UINT16_MAX    (2 * INT16_MAX + 1)

#define INT32_MAX     (2147483647L)
#define INT32_MIN     (-INT32_MAX - 1)
#define UINT32_MAX    (2UL * INT32_MAX + 1)

#define INT64_MAX     (9223372036854775807LL)
#define INT64_MIN     (-INT64_MAX - 1)
#define UINT64_MAX    (2ULL * INT64_MAX + 1)

#define INTMAX_MAX    INT64_MAX
#define INTMAX_MIN    INT64_MIN
#define UINTMAX_MAX   UINT64_MAX

#endif // end of: #if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

// -- 7.18.4 Macros for integer constants --------------------------------------

#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

#define INT8_C(x)     x
#define UINT8_C(x)    x
#define INT16_C(x)    x
#define UINT16_C(x)   x
#define INT32_C(x)    x##L
#define UINT32_C(x)   x##UL
#define INT64_C(x)    x##LL
#define UINT64_C(x)   x##ULL
#define INTMAX_C(x)   x##LL
#define UINTMAX_C(x)  x##ULL

#endif // end of: #if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

#endif // end of else of: #if VO_INTEGER_TYPES_USE_COMPILER_STDINT_H

#endif // end of: #ifndef C_STD_STDINT_H
