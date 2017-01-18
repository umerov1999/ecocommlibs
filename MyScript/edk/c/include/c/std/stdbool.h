#ifndef C_STD_STDBOOL_H
#define C_STD_STDBOOL_H 0x01010000
/**
 * @file c/std/stdbool.h
 * Boolean type and values.
 * This file provides C99 Boolean type and values (7.16) content support.
 */

#ifndef __cplusplus

#ifndef VO_BOOLEAN_TYPE_AND_VALUES_USE_COMPILER_STDBOOL_H
# if defined(_MSC_VER) && _MSC_VER < 1800
#  define VO_BOOLEAN_TYPE_AND_VALUES_USE_COMPILER_STDBOOL_H 0
# else
#  define VO_BOOLEAN_TYPE_AND_VALUES_USE_COMPILER_STDBOOL_H 1
# endif
#endif

#if VO_BOOLEAN_TYPE_AND_VALUES_USE_COMPILER_STDBOOL_H

#include <stdbool.h>

#else // else of: #if VO_BOOLEAN_TYPE_AND_VALUES_USE_COMPILER_STDBOOL_H

#ifndef __cplusplus

// gnuc has built-in _Bool as tell the standard
#ifndef __GNUC__
typedef unsigned char _Bool;
#endif
#define bool _Bool
#define true 1
#define false 0

#endif // end of else of: #ifdef __cplusplus

#define __bool_true_false_are_defined 1

#endif // end of else of: #if VO_BOOLEAN_TYPE_AND_VALUES_USE_COMPILER_STDBOOL_H

#endif // end of: #ifndef __cplusplus

#endif // end of: #ifndef C_STD_STDBOOL_H
